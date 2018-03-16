#include "usart_FreeRTOS.h"
#include <stdarg.h>
#include <stdio.h>

SemaphoreHandle_t usart1_Semaphore_bin = NULL;
SemaphoreHandle_t usart2_Semaphore_bin = NULL;
SemaphoreHandle_t usart3_Semaphore_bin = NULL;
SemaphoreHandle_t uart4_Semaphore_bin = NULL;
SemaphoreHandle_t uart5_Semaphore_bin = NULL;
SemaphoreHandle_t usart6_Semaphore_bin = NULL;

void Usart_FreeRTOS_Semaphore_init(void)
{
  usart1_Semaphore_bin = xSemaphoreCreateBinary();
  usart2_Semaphore_bin = xSemaphoreCreateBinary();
  usart3_Semaphore_bin = xSemaphoreCreateBinary();
  uart4_Semaphore_bin = xSemaphoreCreateBinary();
  uart5_Semaphore_bin = xSemaphoreCreateBinary();
  usart6_Semaphore_bin = xSemaphoreCreateBinary();

  xSemaphoreGive(usart1_Semaphore_bin);
  xSemaphoreGive(usart2_Semaphore_bin);
  xSemaphoreGive(usart3_Semaphore_bin);
  xSemaphoreGive(uart4_Semaphore_bin);
  xSemaphoreGive(uart5_Semaphore_bin);
  xSemaphoreGive(usart6_Semaphore_bin);
}

QueueHandle_t usart1_queue = NULL;
QueueHandle_t usart2_queue = NULL;
QueueHandle_t usart3_queue = NULL;
QueueHandle_t uart4_queue = NULL;
QueueHandle_t uart5_queue = NULL;
QueueHandle_t usart6_queue = NULL;

Usart2_queue_struct u2_queue;
Uart5_queue_struct u5_queue;

void Usart_FreeRTOS_Queue_init(void)
{
//  usart1_queue = xQueueCreate(1,USART1_Tx_BufferSize);
  usart2_queue = xQueueCreate(1,sizeof(u2_queue));
  uart5_queue = xQueueCreate(1,sizeof(u5_queue));
}

void USART_Send_bin_RTOS(USART_TypeDef* USARTx,uint8_t* out_buff,uint16_t out_size)
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
  
  if(USARTx == UART5)
  {
    xSemaphoreTake(uart5_Semaphore_bin,portMAX_DELAY);
    USART5_Send_bin(out_buff,out_size);
  }
}


void USART_printf_RTOS(USART_TypeDef* USARTx,char* fmt,...)  
{  
  BaseType_t result;
  
  if(USARTx == USART1)
  {
    xSemaphoreTake(usart1_Semaphore_bin,portMAX_DELAY);
  }
  
  if(USARTx == USART2)
  {
    result = xSemaphoreTake(usart2_Semaphore_bin,portMAX_DELAY);
    
    if(result == pdTRUE)
    {  
      char buff[USART2_Tx_BufferSize] = {0};
      
      va_list ap;
      va_start(ap,fmt);
      vsprintf((char*)buff,fmt,ap);
      va_end(ap);

      USART2_Send_bin((uint8_t*)buff, strlen(buff)); 
    }
  }
  
  if(USARTx == UART5)
  {
    result = xSemaphoreTake(uart5_Semaphore_bin,portMAX_DELAY);
    
    if(result == pdTRUE)
    {  
      char buff[UART5_Tx_BufferSize] = {0};
      
      va_list ap;
      va_start(ap,fmt);
      vsprintf((char*)buff,fmt,ap);
      va_end(ap);

      USART5_Send_bin((uint8_t*)buff, strlen(buff)); 
    }
  }
}


