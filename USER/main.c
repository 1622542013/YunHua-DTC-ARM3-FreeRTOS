/*
                           _ooOoo_
                          o8888888o
                          88" . "88
                          (| -_- |)
                          O\  =  /O
                       ____/`---'\____
                     .'  \\|     |//  `.
                    /  \\|||  :  |||//  \
                   /  _||||| -:- |||||-  \
                   |   | \\\  -  /// |   |
                   | \_|  ''\---/''  |   |
                   \  .-\__  `-`  ___/-. /
                 ___`. .'  /--.--\  `. . __
              ."" '<  `.___\_<|>_/___.'  >'"".
             | | :  `- \`.;`\ _ /`;.`/ - ` : | |
             \  \ `-.   \_ __\ /__ _/   .-` /  /
        ======`-.____`-.___\_____/___.-`____.-'======
                           `=---='
        ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
                 佛祖保佑       永无BUG
*/

#include "usr_FreeRTOS.h"
#include "usart_FreeRTOS.h"
#include "usart2.h"
#include "config.h"
#include "stm32f4xx_it.h"
#include "GNSS.h"
#include "LED.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "nav_decode.h"
#include "event_mark.h"
#include "DateTime.h"

//#define TASK_STATUS_PRINTF 

TaskHandle_t HandleTaskRevGNSS        = NULL;
TaskHandle_t HandleTaskRevNav         = NULL;
TaskHandle_t HandleTaskSendMessage    = NULL;
TaskHandle_t HandleTaskRevUsr         = NULL;
TaskHandle_t HandleTaskappstatus      = NULL;
TaskHandle_t HandleTaskEventMark      = NULL;

EventGroupHandle_t Event_group = NULL;

void TaskCreatUser(void);
void TaskGroupCreat(void);
void SemaphoreCreat(void);
void QueueCreat(void);


TpInt32 main(TpVoid)
{
  NVIC_SetVectorTable(FLASH_BASE, 0x20000);

  __set_PRIMASK(1);
  /* 创建任务*/

  HardWareInit(); 
   
  SemaphoreCreat();
  QueueCreat();
  TaskCreatUser();
  TaskGroupCreat();
  /* 启动调度，开始执行任务 */
  vTaskStartScheduler();
  /*
   系统不会运行到这里，
   如果运行到这里，有可能是堆空间不够使用导致
  */
  while(1);		
}


/*-----------------------rev  GNSS---------------------------*/
void TaskRevGNSS(void* pv)
{  
  BaseType_t result = 0;  
  USART_gnss_queue_struct Gnss_Rx_struct = {0};
  
  stGnssIn m_stGnssIn = {0};
  static stGnssOut m_stGnssOut= {0};
  GnssOutPut gnss_out_data= {0};
  
  while(1)
  {
    result = xQueueReceive(uart5_queue,&Gnss_Rx_struct,portMAX_DELAY);
    
    if(result == pdTRUE)
    {           
      Package_Rec(Gnss_Rx_struct.rx_buff,Gnss_Rx_struct.rx_num, &m_stGnssIn);
      Combain_GNSS(&m_stGnssIn, &m_stGnssOut);
        
      if((m_stGnssOut.u16Flag & 0x0001) == 0x0001) /*lock the position*/
      {
        gnss_out_data.head = 0xFEDC;
        
        gnss_out_data.week_num    = m_stGnssOut.u16Week;
        gnss_out_data.week_second = m_stGnssOut.u32UTC;
        gnss_out_data.lon         = m_stGnssOut.dLongitude; 
        gnss_out_data.lat         = m_stGnssOut.dLatitude; 
        gnss_out_data.alt         = m_stGnssOut.fAltitude;
        gnss_out_data.flag        = m_stGnssOut.u16Flag;
        gnss_out_data.spd         = m_stGnssOut.fVgroud;
        gnss_out_data.heading     = m_stGnssOut.fHeading;
        gnss_out_data.hdop        = m_stGnssOut.hdop;
        gnss_out_data.orient      = m_stGnssOut.fAzi;
        gnss_out_data.check       = CheckSumByte((TpUchar*)&gnss_out_data,sizeof(gnss_out_data));	       

        USART_Send_bin_RTOS(USART_nav,(uint8_t*)&gnss_out_data,sizeof(gnss_out_data));       
        
        if((m_stGnssOut.u16Flag & GNSS_FIXED) == GNSS_FIXED)
        {
          LED_SetState(LEDPin1, LEDStateOn);
          LED_SetState(LEDPin2, LEDStateOff);
        }
        else
        {
          LED_SetState(LEDPin1, LEDStateOff);
          LED_SetState(LEDPin2, LEDStateOn);
        }
     
        EventBits_t sd_time_ok;        
        sd_time_ok = xEventGroupGetBits(Event_group);
        
        if((sd_time_ok & SD_TIME_OK_BIT) == 0)
        {          
          DateTime      utctime = {0};
          GpsWeekSecond gpstime = {0};
          
          gpstime.week   = m_stGnssOut.u16Week;
          gpstime.second = m_stGnssOut.u32UTC / 1000;
          
          gpstime = GpsWeekSecondAddHour(gpstime, 8);
          utctime = GpsWeekSecondToGregorianCalendarDate(gpstime);
          
          USART_printf_RTOS(USART_SD,"$cmd,set,date,%d-%.2d-%.2d*ff",utctime.year, utctime.month, utctime.day);
          USART_printf_RTOS(USART_SD,"$cmd,set,time,%.2d:%.2d:%.2d*ff",utctime.hour, utctime.minute, utctime.second);
        }   
        memset(&m_stGnssOut,0,sizeof(m_stGnssOut));
      }
      else
      {
          LED_SetState(LEDPin1, LEDStateOff);
          LED_SetState(LEDPin2, LEDStateOff);
      }
    }
  }
}

void TaskRevNav(void* pv)
{
  BaseType_t result = 0;
  USART_nav_queue_struct  nav_rx_struct = {0};
  
  while(1)
  {    
    result = xQueueReceive(USART_nav_queue_handle,&nav_rx_struct,portMAX_DELAY);
    
    if(result == pdTRUE)
    {
       decode_nav(nav_rx_struct.rx_buff,nav_rx_struct.rx_num); 
    }
  }
}

void TaskSendMessage(void* pv)
{  
  BaseType_t result = 0;
  DtcArm2Data nav_data = {0};
  TickType_t xLastSYSTime = 0;
  xLastSYSTime = xTaskGetTickCount();/*读取此时时间*/
 
  while(1)
  {    
    result = xQueueReceive(nav_queue,&nav_data,1);
    
    if(result == pdTRUE)
    { 
      
    }
    else
    {
        memset(&nav_data,0,sizeof(DtcArm2Data));
    }

    nav_data.nav.resv[0] = GetEventMark();
    nav_data.check = CheckSum((TpUchar*)&nav_data,sizeof(DtcArm2Data));
    
    USART_Send_bin_RTOS(USART_SD,(uint8_t *)&nav_data,sizeof(DtcArm2Data)); 
    
#ifndef  TASK_STATUS_PRINTF     
    USART_Send_bin_RTOS(USART_usr,(uint8_t *)&nav_data,sizeof(DtcArm2Data));
#endif
    
    vTaskDelayUntil(&xLastSYSTime, SYSTEM_CYCLE);/*精准延时*/
  }
}

/*-----------------------rev  usr---------------------------*/
void TaskRevUsr(void* pv)
{  
  BaseType_t result = 0;
  USART_usr_queue_struct USR_Rx_Buff = {0};
 
  while(1)
  {       
    result = xQueueReceive(USART_usr_queue_handle,&USR_Rx_Buff,portMAX_DELAY);
    
    if(result == pdTRUE)
    {
 //     USART_Send_bin_RTOS(USART_usr,USR_Rx_Buff.rx_buff,USR_Rx_Buff.rx_num);
    }
  }
}

/*-----------------------scan io mark---------------------------*/
void TaskEventMark(void* pv)
{  
  while(1)
  {   
      KeyScanUpDown();
      vTaskDelay(SYSTEM_CYCLE);
  }
}

/*-----------------------Task appstatus---------------------------*/

void Taskappstatus(void* pv)
{  
  TickType_t xLastSYSTime = 0;

  xLastSYSTime = xTaskGetTickCount();/*读取此时时间*/

  while(1)
  { 
    
#ifdef TASK_STATUS_PRINTF
    
    uint8_t    sys_status_buff[512];
    
    USART_printf_RTOS(USART_usr,"=================================\r\n",sys_status_buff);
    USART_printf_RTOS(USART_usr,"任务名    任务状态    优先级   剩余栈     任务号\r\n",sys_status_buff);
    vTaskList((char*)sys_status_buff);
    USART_printf_RTOS(USART_usr,"%s\r\n",sys_status_buff);
    vTaskGetRunTimeStats((char*)sys_status_buff);
    USART_printf_RTOS(USART_usr,"%s\r\n",sys_status_buff);
#endif
    
    vTaskDelayUntil(&xLastSYSTime, 1000);/*精准延时*/
  }
}



/*-------------创建任务及相关------------*/

void TaskCreatUser(void)
{ 
  xTaskCreate( Taskappstatus,         /* 实时打印任务状态*/
               "Taskappstatus",       /* 任务名*/
               1024,                   /* 任务栈大小，单位：4字节 */
               NULL,                  /* 任务参数  */
               1,                     /* 任务优先级*/
               &HandleTaskappstatus); /* 任务句柄  */
  
  xTaskCreate( TaskRevGNSS,         	/* 接收arm2数据，并将数据输出到arm2上 */
               "TaskRevGNSS",         /* 任务名    */
               1024,               		/* 任务栈大小，单位：4字节 */
               NULL,              		/* 任务参数  */
               2,                 		/* 任务优先级*/
               &HandleTaskRevGNSS); 	/* 任务句柄  */
  
  xTaskCreate( TaskRevNav,         	/* 任务函数 */
               "TaskRevNav",       	/* 任务名    */
               512,               			/* 任务栈大小，单位：4字节 */
               NULL,              		/* 任务参数  */
               2,                 		/* 任务优先级*/
               &HandleTaskRevNav); 	/* 任务句柄  */
  
  xTaskCreate( TaskRevUsr,         	/* 任务函数 */
               "TaskRevUsr",       	/* 任务名    */
               512,               			/* 任务栈大小，单位：4字节 */
               NULL,              		/* 任务参数  */
               2,                 		/* 任务优先级*/
               &HandleTaskRevUsr); 	/* 任务句柄  */
  xTaskCreate( TaskSendMessage,         	/* 任务函数 */
               "TaskSendMessage",       	/* 任务名    */
               1024,               			/* 任务栈大小，单位：4字节 */
               NULL,              		/* 任务参数  */
               2,                 		/* 任务优先级*/
               &HandleTaskSendMessage); 	/* 任务句柄  */
  xTaskCreate( TaskEventMark,         	/* 任务函数 */
               "TaskEventMark",       	/* 任务名    */
               100,               			/* 任务栈大小，单位：4字节 */
               NULL,              		/* 任务参数  */
               2,                 		/* 任务优先级*/
               &HandleTaskEventMark); 	/* 任务句柄  */
}

/*信号量创建*/
void SemaphoreCreat(void)
{
    Usart_FreeRTOS_Semaphore_init();
}

/*消息队列创建*/
void QueueCreat(void)
{
    Usart_FreeRTOS_Queue_init();
    NavDataQueueInit();
}

/*创建事件标志组*/
void TaskGroupCreat(void)
{
	Event_group = xEventGroupCreate();
}
