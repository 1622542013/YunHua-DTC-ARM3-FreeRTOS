#include "usr_FreeRTOS.h"

#include "config.h"
#include "stm32f4xx_it.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


void TaskCreatUser(void);
void TaskGroupCreat(void);
void SemaphoreCreat(void);
uint8_t send_bf[100] = "1234567890ABCDEFGHIJKLMNOPQRSTUVWXYZ\r\n";

TpInt32 main(TpVoid)
{
//  __set_PRIMASK(1);
  /* ��������*/

  HardWareInit(); 

//  SemaphoreCreat();
//  TaskCreatUser();
//  TaskGroupCreat();

//  /* �������ȣ���ʼִ������ */
//  vTaskStartScheduler();
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
  TickType_t xLastSYSTime;

  xLastSYSTime = xTaskGetTickCount();/*��ȡ��ʱʱ��*/
	
	 /* ��ȡ��������Ŀ*/  
  uxArraySize = uxTaskGetNumberOfTasks ();  
  
  while(1)
  {
		  /*��ȡÿ�������״̬��Ϣ */  
      uxArraySizer = uxTaskGetSystemState( task_status, uxArraySize,&ulTotalRunTime );  
		
//      u1_printf("ϵͳ����ʱ�䣺 %d��\r\n");/*�������ʱ��*/


      vTaskDelayUntil(&xLastSYSTime, 1000);/*��׼��ʱ*/
  }
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
    
//      u1_printf("ϵͳ����ʱ�䣺 %d��\r\n",time_count);/*�������ʱ��*/

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

/*-----------------------TaskMainTcp---------------------------*/
TaskHandle_t HandleTaskMainTcp = NULL;
void TaskMainTcp(void* pv)
{  
  while(1)
  {
    
  }
}

/*-----------------------TaskTCPClient2---------------------------*/
TaskHandle_t HandleTaskTCPServer = NULL;
void TaskTCPServer(void* pv)
{  
  while(1)
  {
    
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

    vTaskDelayUntil(&xLastSYSTime, 100);/*��׼��ʱ*/
  }
}

/*-------------�����������------------*/

void TaskCreatUser(void)
{ 
   xTaskCreate( Taskappstatus,         /* ������ */
               "Taskappstatus",       /* ������    */
               500,               /* ����ջ��С����λ��4�ֽ� */
               NULL,              /* �������  */
               1,                 /* �������ȼ�*/
               &HandleTaskappstatus); /* ������  */
	
  xTaskCreate( TaskStart,         /* ������ */
               "TaskStart",       /* ������    */
               100,               /* ����ջ��С����λ��4�ֽ� */
               NULL,              /* �������  */
               1,                 /* �������ȼ�*/
               &HandleTaskStart); /* ������  */
  
  xTaskCreate( TaskTCPClient1,         /* ������ */
               "TaskTCPClient1",       /* ������    */
               500,               		 /* ����ջ��С����λ��4�ֽ� */
               NULL,              		 /* �������  */
               1,                 		 /* �������ȼ�*/
               &HandleTaskTCPClient1); /* ������  */
  
  xTaskCreate( TaskTCPServer,         /* ������ */
							 "TaskTCPServer",       /* ������    */
							 1024,                  /* ����ջ��С����λ��4�ֽ� */
							 NULL,                  /* �������  */
							 1,                     /* �������ȼ�*/
							 &HandleTaskTCPServer); /* ������  */
  
  xTaskCreate( TaskMainTcp,         	/* ������ */
               "TaskTCPClient2",      /* ������    */
               100,               			/* ����ջ��С����λ��4�ֽ� */
               NULL,              		/* �������  */
               2,                 		/* �������ȼ�*/
               &HandleTaskMainTcp); 	/* ������  */
  
  xTaskCreate( TaskSysTcik,         	/* ������ */
               "TaskSysTime",       	/* ������    */
               100,               			/* ����ջ��С����λ��4�ֽ� */
               NULL,              		/* �������  */
               3,                 		/* �������ȼ�*/
               &HandleTaskSysTcik); 	/* ������  */
}

/*���������ź�������ֹu1_printf����䣬����*/
SemaphoreHandle_t  xMutex = NULL;
void SemaphoreCreat(void)
{
  xMutex = xSemaphoreCreateMutex();
}

/*�����¼���־�飬���RLЧ�ʣ���ʱ����main_TcpNet();*/
EventGroupHandle_t Main_tcpnet_group = NULL;

void TaskGroupCreat(void)
{
	Main_tcpnet_group = xEventGroupCreate();
}
