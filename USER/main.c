/**
 * @file        main.c
 * @brief
 */

/*============================================================================*/
/*                               Header include                               */
/*============================================================================*/

#include "usr_FreeRTOS.h"

#include "config.h"
#include "stm32f4xx_it.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "TCP_user.h"

void TaskCreatUser(void);
void TaskGroupCreatUser(void);
void TaskQueueCreatUser(void);
void SemaphoreCreat(void);
TpInt32 main(TpVoid)
{
	/* Tim,Usart.. config*/ 
  __set_PRIMASK(1);
  /* ��������*/

  HardWareInit(); 
 
  SemaphoreCreat();
  TaskCreatUser();
  
   /* ��ʼ��RL-TCPnet */
	init_TcpNet ();

  /* �������ȣ���ʼִ������ */
  vTaskStartScheduler();
   /*
     ϵͳ�������е����
     ������е�����п����Ƕѿռ䲻��ʹ�õ���
     #define configTOTAL_HEAP_SIZE        ( ( size_t ) ( 17 * 1024 ) )
   */
   while(1);		
}

/*-----------------------TaskStart---------------------------*/
TaskHandle_t HandleTaskStart = NULL;
void TaskStart(void* pv)
{  
  static uint32_t time_count;

  TickType_t xLastSYSTime;
  
  xLastSYSTime = xTaskGetTickCount();/*��ȡ��ʱʱ��*/
  
  while(1)
  {
      time_count++;
    
      u1_printf("ϵͳ����ʱ�䣺 %d��\r\n",time_count);/*�������ʱ��*/

      vTaskDelayUntil(&xLastSYSTime, 1000);/*��׼��ʱ*/
  }
}
/*-----------------------TaskTCPClient1---------------------------*/
TaskHandle_t HandleTaskTCPClient1 = NULL;
void TaskTCPClient1(void* pv)
{  
  while(1)
  {
     vTaskDelay(1000);
  }
}

/*-----------------------TaskTCPClient2---------------------------*/
TaskHandle_t HandleTaskTCPClient2 = NULL;
void TaskTCPClient2(void* pv)
{  
  while(1)
  {
    /* RL-TCPnet������(��Ҫһֱ���ã����ò�������) */
		 main_TcpNet();
     vTaskDelay(2);
  }
}

/*-----------------------TaskTCPClient2---------------------------*/
TaskHandle_t HandleTaskTCPServer = NULL;
void TaskTCPServer(void* pv)
{  
  while(1)
  {
   // BSDTCPnetTest();
		BSDSocketClientTest();
  }
}
/*-----------------------TaskSysTcik---------------------------*/
TaskHandle_t HandleTaskSysTcik = NULL;
void TaskSysTcik(void* pv)
{  
  TickType_t xLastSYSTime;
  
  xLastSYSTime = xTaskGetTickCount();/*��ȡ��ʱʱ��*/
  
  while(1)
  {
    timer_tick ();

    vTaskDelayUntil(&xLastSYSTime, 100);/*��׼��ʱ*/
  }
}

/*-------------�����������------------*/

void TaskCreatUser(void)
{ 
  xTaskCreate( TaskStart,         /* ������ */
               "TaskStart",       /* ������    */
               100,               /* ����ջ��С����λ��4�ֽ� */
               NULL,              /* �������  */
               1,                 /* �������ȼ�*/
               &HandleTaskStart); /* ������  */
  
  xTaskCreate( TaskTCPClient1,         /* ������ */
               "TaskTCPClient1",       /* ������    */
               500,               /* ����ջ��С����λ��4�ֽ� */
               NULL,              /* �������  */
               2,                 /* �������ȼ�*/
               &HandleTaskTCPClient1); /* ������  */
  
  xTaskCreate( TaskTCPServer,         /* ������ */
             "TaskTCPServer",       /* ������    */
             2048,               /* ����ջ��С����λ��4�ֽ� */
             NULL,              /* �������  */
             1,                 /* �������ȼ�*/
             &HandleTaskTCPServer); /* ������  */
  
  xTaskCreate( TaskTCPClient2,         /* ������ */
               "TaskTCPClient2",       /* ������    */
               50,               /* ����ջ��С����λ��4�ֽ� */
               NULL,              /* �������  */
               1,                 /* �������ȼ�*/
               &HandleTaskTCPClient2); /* ������  */
  
  xTaskCreate( TaskSysTcik,         /* ������ */
               "TaskSysTime",       /* ������    */
               30,               /* ����ջ��С����λ��4�ֽ� */
               NULL,              /* �������  */
               1,                 /* �������ȼ�*/
               &HandleTaskSysTcik); /* ������  */
}

SemaphoreHandle_t  xMutex = NULL;
void SemaphoreCreat()
{
  xMutex = xSemaphoreCreateMutex();
}
