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
                 ���汣��       ����BUG
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

/*���������ź�������ֹprintf����䣬����*/
SemaphoreHandle_t  u2_printf_sema_bin = NULL;

void TaskCreatUser(void);
void TaskGroupCreat(void);
void SemaphoreCreat(void);
void QueueCreat(void);
TpInt32 main(TpVoid)
{
  NVIC_SetVectorTable(FLASH_BASE, 0x20000);

  __set_PRIMASK(1);
  /* ��������*/

  HardWareInit(); 
   
  SemaphoreCreat();
  QueueCreat();
  TaskCreatUser();

  /* �������ȣ���ʼִ������ */
  vTaskStartScheduler();
  /*
   ϵͳ�������е����
   ������е�����п����Ƕѿռ䲻��ʹ�õ���
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

  xLastSYSTime = xTaskGetTickCount();/*��ȡ��ʱʱ��*/
	
//	 /* ��ȡ��������Ŀ*/  
//  uxArraySize = uxTaskGetNumberOfTasks ();  
  
  while(1)
  {
//		  /*��ȡÿ�������״̬��Ϣ */  
//      uxArraySizer = uxTaskGetSystemState( task_status, uxArraySize,&ulTotalRunTime );  
    
      USART_printf_RTOS(USART2,"ϵͳ����ʱ�䣺 %d����\r\n",xLastSYSTime);

      vTaskDelayUntil(&xLastSYSTime, 1000);/*��׼��ʱ*/
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
  xLastSYSTime = xTaskGetTickCount();/*��ȡ��ʱʱ��*/
 
  while(1)
  {    
    result = xQueueReceive(nav_queue,&nav_data,1);
    
    if(result == pdTRUE)
    {
      USART_Send_bin_RTOS(USART_SD,(uint8_t *)&nav_data,sizeof(nav_data));
      USART_printf_RTOS(USART2,"*****�յ�*****\r\n"); 
    }
    else
    {
      USART_printf_RTOS(USART2,"��������������������û�յ���������������������\r\n");
    }
    
    vTaskDelayUntil(&xLastSYSTime, SYSTEM_CYCLE);/*��׼��ʱ*/
  }
}

/*-------------�����������------------*/

void TaskCreatUser(void)
{ 
  xTaskCreate( Taskappstatus,         /* ʵʱ��ӡ����״̬*/
               "Taskappstatus",       /* ������*/
               512,                   /* ����ջ��С����λ��4�ֽ� */
               NULL,                  /* �������  */
               1,                     /* �������ȼ�*/
               &HandleTaskappstatus); /* ������  */
  
  xTaskCreate( TaskRevGNSS,         	/* ����arm2���ݣ��������������arm2�� */
               "TaskRevGNSS",         /* ������    */
               512,               		/* ����ջ��С����λ��4�ֽ� */
               NULL,              		/* �������  */
               2,                 		/* �������ȼ�*/
               &HandleTaskRevGNSS); 	/* ������  */
  
  xTaskCreate( TaskRevNav,         	/* ������ */
               "TaskRevNav",       	/* ������    */
               512,               			/* ����ջ��С����λ��4�ֽ� */
               NULL,              		/* �������  */
               2,                 		/* �������ȼ�*/
               &HandleTaskRevNav); 	/* ������  */
  
  xTaskCreate( TaskRevUsr,         	/* ������ */
               "TaskRevUsr",       	/* ������    */
               512,               			/* ����ջ��С����λ��4�ֽ� */
               NULL,              		/* �������  */
               2,                 		/* �������ȼ�*/
               &HandleTaskRevUsr); 	/* ������  */
  xTaskCreate( TaskSendMessage,         	/* ������ */
               "TaskSendMessage",       	/* ������    */
               1024,               			/* ����ջ��С����λ��4�ֽ� */
               NULL,              		/* �������  */
               2,                 		/* �������ȼ�*/
               &HandleTaskSendMessage); 	/* ������  */
}

/*�ź�������*/
void SemaphoreCreat(void)
{
    Usart_FreeRTOS_Semaphore_init();
}

/*��Ϣ���д���*/
void QueueCreat(void)
{
    Usart_FreeRTOS_Queue_init();
    NavDataQueueInit();
}

/*�����¼���־�飬���RLЧ�ʣ���ʱ����main_TcpNet();*/
EventGroupHandle_t Main_tcpnet_group = NULL;

void TaskGroupCreat(void)
{
	Main_tcpnet_group = xEventGroupCreate();
}
