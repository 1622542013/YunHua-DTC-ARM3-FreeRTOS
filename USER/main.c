/**
 * @file        main.c
 * @brief
 */

/*============================================================================*/
/*                               Header include                               */
/*============================================================================*/

#include "config.h"
#include "stm32f4xx_it.h"

#include "usr_FreeRTOS.h"

void TaskCreatUser(void);
void TaskGroupCreatUser(void);
void TaskQueueCreatUser(void);

TpInt32 main(TpVoid)
{
	/* Tim,Usart.. config*/ 
  __set_PRIMASK(1);
  /* 创建任务*/

  HardWareInit(); 
  
  TaskCreatUser();
  TaskGroupCreatUser();
  TaskQueueCreatUser();
  /* 启动调度，开始执行任务 */
  vTaskStartScheduler();
   /*
     系统不会运行到这里，
     如果运行到这里，有可能是堆空间不够使用导致
     #define configTOTAL_HEAP_SIZE        ( ( size_t ) ( 17 * 1024 ) )
   */
   while(1);		
}


/*-----------任务--------------*/
TaskHandle_t HandleTaskStartCollect = NULL;

void TaskStartCollect(void* pv)
{  
  while(1)
  {

  }
}

/*-------------创建任务及相关------------*/
void TaskGroupCreatUser(void)
{

}

void TaskQueueCreatUser(void)
{
  
}


void TaskCreatUser(void)
{
  
  xTaskCreate( TaskStartCollect,   /* 任务函数 */
               "TaskStartCollect",    /* 任务名    */
               500,              /* 任务栈大小，单位：4字节 */
               NULL,             /* 任务参数  */
               2,                /* 任务优先级*/
               &HandleTaskStartCollect); /* 任务句柄  */
}


