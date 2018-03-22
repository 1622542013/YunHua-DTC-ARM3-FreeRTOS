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

/*创建互斥信号量，防止printf任务间，重入*/
SemaphoreHandle_t  u2_printf_sema_bin = NULL;

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

  /* 启动调度，开始执行任务 */
  vTaskStartScheduler();
  /*
   系统不会运行到这里，
   如果运行到这里，有可能是堆空间不够使用导致
  */
  while(1);		
}

/*-----------------------Task appstatus---------------------------*/
TaskHandle_t HandleTaskappstatus;
TickType_t  uxArraySize;
TickType_t  uxArraySizer;
TaskStatus_t task_status[8];
TickType_t  ulTotalRunTime;

void Taskappstatus(void* pv)
{  
  TickType_t xLastSYSTime = 0;

  xLastSYSTime = xTaskGetTickCount();/*读取此时时间*/
	
//	 /* 获取任务总数目*/  
//  uxArraySize = uxTaskGetNumberOfTasks ();  
  
  while(1)
  {
//		  /*获取每个任务的状态信息 */  
//      uxArraySizer = uxTaskGetSystemState( task_status, uxArraySize,&ulTotalRunTime );  
    
      USART_printf_RTOS(USART2,"系统运行时间： %d毫秒\r\n",xLastSYSTime);

      vTaskDelayUntil(&xLastSYSTime, 1000);/*精准延时*/
  }
}



/*-----------------------rev  GNSS---------------------------*/
TaskHandle_t HandleTaskRevGNSS = NULL;
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

TaskHandle_t HandleTaskRevNav = NULL;
void TaskRevNav(void* pv)
{
  BaseType_t result = 0;
  uint8_t    decode_result = 0;
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

/*-----------------------rev  usr---------------------------*/
TaskHandle_t HandleTaskRevUsr = NULL;
void TaskRevUsr(void* pv)
{  
  BaseType_t result = 0;
  USART_usr_queue_struct USR_Rx_Buff = {0};
 
  while(1)
  {       
    result = xQueueReceive(USART_usr_queue_handle,&USR_Rx_Buff,portMAX_DELAY);
    
    if(result == pdTRUE)
    {
      USART_Send_bin_RTOS(USART_usr,USR_Rx_Buff.rx_buff,USR_Rx_Buff.rx_num);
    }
  }
}


TaskHandle_t HandleTaskSendMessage = NULL;
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
      USART_Send_bin_RTOS(USART_SD,(uint8_t *)&nav_data,sizeof(nav_data));
      USART_printf_RTOS(USART2,"*****收到*****\r\n"); 
    }
    else
    {
      USART_printf_RTOS(USART2,"——————————没收到——————————\r\n");
    }
    
    vTaskDelayUntil(&xLastSYSTime, SYSTEM_CYCLE);/*精准延时*/
  }
}

/*-------------创建任务及相关------------*/

void TaskCreatUser(void)
{ 
  xTaskCreate( Taskappstatus,         /* 实时打印任务状态*/
               "Taskappstatus",       /* 任务名*/
               512,                   /* 任务栈大小，单位：4字节 */
               NULL,                  /* 任务参数  */
               1,                     /* 任务优先级*/
               &HandleTaskappstatus); /* 任务句柄  */
  
  xTaskCreate( TaskRevGNSS,         	/* 接收arm2数据，并将数据输出到arm2上 */
               "TaskRevGNSS",         /* 任务名    */
               512,               		/* 任务栈大小，单位：4字节 */
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

/*创建事件标志组，提高RL效率，及时调用main_TcpNet();*/
EventGroupHandle_t Main_tcpnet_group = NULL;

void TaskGroupCreat(void)
{
	Main_tcpnet_group = xEventGroupCreate();
}
