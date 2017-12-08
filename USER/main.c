/**
 * @file        main.c
 * @brief
 */

/*============================================================================*/
/*                               Header include                               */
/*============================================================================*/

#include "config.h"
#include "stm32f4xx_it.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "usr_FreeRTOS.h"
#include "TCP_user.h"


void TaskCreatUser(void);
void TaskGroupCreatUser(void);
void TaskQueueCreatUser(void);

TpInt32 main(TpVoid)
{
	/* Tim,Usart.. config*/ 
  __set_PRIMASK(1);
  /* 创建任务*/

  HardWareInit(); 
  
 /* 初始化RL-TCPnet */
	init_TcpNet ();
  
  TaskCreatUser();

  /* 启动调度，开始执行任务 */
  vTaskStartScheduler();
   /*
     系统不会运行到这里，
     如果运行到这里，有可能是堆空间不够使用导致
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
  
  xLastSYSTime = xTaskGetTickCount();/*读取此时时间*/
  
  while(1)
  {
      time_count++;
    
      u1_printf("系统运行时间： %d秒\r\n",time_count);/*输出运行时间*/

      vTaskDelayUntil(&xLastSYSTime, 1000);/*精准延时*/
  }
}
/*-----------------------TaskTCPClient1---------------------------*/
TaskHandle_t HandleTaskTCPClient1 = NULL;
void TaskTCPClient1(void* pv)
{  
  while(1)
  {
     // TcpClientTest1();
     vTaskDelay(1000);
  }
}

/*-----------------------TaskTCPClient2---------------------------*/
TaskHandle_t HandleTaskTCPClient2 = NULL;
void TaskTCPClient2(void* pv)
{  
  while(1)
  {
     // TcpClientTest2();
     vTaskDelay(1000);
  }
}

/*-----------------------TaskTCPClient2---------------------------*/
TaskHandle_t HandleTaskTCPServer = NULL;
void TaskTCPServer(void* pv)
{  
  while(1)
  {
     // TcpServer();
    // UDPnetTest();
    BSDTCPnetTest();
  }
}
/*-----------------------TaskSysTcik---------------------------*/
TaskHandle_t HandleTaskSysTcik = NULL;
void TaskSysTcik(void* pv)
{  
  TickType_t xLastSYSTime;
  
  xLastSYSTime = xTaskGetTickCount();/*读取此时时间*/
  
  while(1)
  {
    timer_tick ();
    
    /* RL-TCPnet处理函数(需要一直调用，作用不明！？) */
		main_TcpNet();
    
    vTaskDelayUntil(&xLastSYSTime, 100);/*精准延时*/
  }
}

/*-------------创建任务及相关------------*/

void TaskCreatUser(void)
{ 
  xTaskCreate( TaskStart,         /* 任务函数 */
               "TaskStart",       /* 任务名    */
               50,               /* 任务栈大小，单位：4字节 */
               NULL,              /* 任务参数  */
               2,                 /* 任务优先级*/
               &HandleTaskStart); /* 任务句柄  */
  
//  xTaskCreate( TaskTCPClient1,         /* 任务函数 */
//               "TaskTCPClient1",       /* 任务名    */
//               500,               /* 任务栈大小，单位：4字节 */
//               NULL,              /* 任务参数  */
//               2,                 /* 任务优先级*/
//               &HandleTaskTCPClient1); /* 任务句柄  */
//  
//  xTaskCreate( TaskTCPClient2,         /* 任务函数 */
//               "TaskTCPClient2",       /* 任务名    */
//               500,               /* 任务栈大小，单位：4字节 */
//               NULL,              /* 任务参数  */
//               2,                 /* 任务优先级*/
//               &HandleTaskTCPClient2); /* 任务句柄  */
  
  xTaskCreate( TaskTCPServer,         /* 任务函数 */
               "TaskTCPServer",       /* 任务名    */
               1000,               /* 任务栈大小，单位：4字节 */
               NULL,              /* 任务参数  */
               4,                 /* 任务优先级*/
               &HandleTaskTCPServer); /* 任务句柄  */
  
  xTaskCreate( TaskSysTcik,         /* 任务函数 */
               "TaskSysTime",       /* 任务名    */
               50,               /* 任务栈大小，单位：4字节 */
               NULL,              /* 任务参数  */
               2,                 /* 任务优先级*/
               &HandleTaskSysTcik); /* 任务句柄  */
}

