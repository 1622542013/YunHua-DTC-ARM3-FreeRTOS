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
TaskHandle_t HandleTaskStart = NULL;

void TaskStart(void* pv)
{  
  static uint8_t send_buff[200];
  uint16_t num;
  while(1)
  {
        num = snprintf((char *)send_buff,200,"啦啦啦啦");
        USART_SetSendData(UART4, send_buff, num); 
        vTaskDelay(1000);
  }
}

/*-------------创建任务及相关------------*/

void TaskCreatUser(void)
{ 
  xTaskCreate( TaskStart,           /* 任务函数 */
               "TaskStart",    /* 任务名    */
               500,              /* 任务栈大小，单位：4字节 */
               NULL,             /* 任务参数  */
               2,                /* 任务优先级*/
               &HandleTaskStart); /* 任务句柄  */
}

void USART_ReceiveDataFinishCallback(USART_TypeDef* USARTx)
{
  if(USARTx == UART4)
  {
      int a = 0;
      a= 1;
  }
}



