#include "usart_FreeRTOS.h"
#include "usr_FreeRTOS.h"

#include "usart1.h"
#include "usart2.h"
#include "usart3.h"
#include "usart4.h"
#include "usart5.h"
#include "usart6.h"

SemaphoreHandle_t usart1_Semaphore_bin = NULL;
SemaphoreHandle_t usart2_Semaphore_bin = NULL;
SemaphoreHandle_t usart3_Semaphore_bin = NULL;
SemaphoreHandle_t usart4_Semaphore_bin = NULL;
SemaphoreHandle_t usart5_Semaphore_bin = NULL;
SemaphoreHandle_t usart6_Semaphore_bin = NULL;

void Usart_FreeRTOS_Semaphore_init()
{
  usart1_Semaphore_bin = xSemaphoreCreateBinary();
  usart2_Semaphore_bin = xSemaphoreCreateBinary();
  usart3_Semaphore_bin = xSemaphoreCreateBinary();
  usart4_Semaphore_bin = xSemaphoreCreateBinary();
  usart5_Semaphore_bin = xSemaphoreCreateBinary();
  usart6_Semaphore_bin = xSemaphoreCreateBinary();

  xSemaphoreGive(usart1_Semaphore_bin);
  xSemaphoreGive(usart2_Semaphore_bin);
  xSemaphoreGive(usart3_Semaphore_bin);
  xSemaphoreGive(usart4_Semaphore_bin);
  xSemaphoreGive(usart5_Semaphore_bin);
  xSemaphoreGive(usart6_Semaphore_bin);
}

void USART_Send_bin(USART_TypeDef* USARTx,uint8_t* out_buff,uint16_t out_size)
{
  if(USARTx == USART1)
  {
    xSemaphoreTake(usart1_Semaphore_bin,portMAX_DELAY);
    USART1_Send_bin(out_buff,out_size);
  }
  
  if(USARTx == USART2)
  {
    xSemaphoreTake(usart2_Semaphore_bin,portMAX_DELAY);
    USART2_Send_bin(out_buff,out_size);
  }
}

void USART_Printf(USART_TypeDef* USARTx,char* fmt,...)  
{
  
}
