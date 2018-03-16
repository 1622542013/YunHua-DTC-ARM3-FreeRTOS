#ifndef __USART_FREERTOS_H
#define __USART_FREERTOS_H

#include "usr_FreeRTOS.h"
#include "usart1.h"
#include "usart2.h"
#include "usart3.h"
#include "usart4.h"
#include "usart5.h"
#include "usart6.h"

extern SemaphoreHandle_t usart1_Semaphore_bin;
extern SemaphoreHandle_t usart2_Semaphore_bin;
extern SemaphoreHandle_t usart3_Semaphore_bin;
extern SemaphoreHandle_t uart4_Semaphore_bin;
extern SemaphoreHandle_t uart5_Semaphore_bin;
extern SemaphoreHandle_t usart6_Semaphore_bin;

extern QueueHandle_t usart1_queue;
extern QueueHandle_t usart2_queue;
extern QueueHandle_t usart3_queue;
extern QueueHandle_t uart4_queue;
extern QueueHandle_t uart5_queue;
extern QueueHandle_t usart6_queue;

#define USART_usr                 USART2
#define USART_usr_queue_struct    Usart2_queue_struct

#define USART_nav                 USART3
#define USART_nav_queue_struct    Usart3_queue_struct

#define USART_gnss      UART5
#define USART_gnss_queue_struct   Uart5_queue_struct

#pragma pack(push)
#pragma pack(1)

typedef struct
{
  uint8_t  rx_buff[USART2_Tx_BufferSize];
  uint32_t rx_num;
}Usart2_queue_struct;

typedef struct
{
  uint8_t  rx_buff[USART3_Tx_BufferSize];
  uint32_t rx_num;
}Usart3_queue_struct;

typedef struct
{
  uint8_t  rx_buff[UART5_Tx_BufferSize];
  uint32_t rx_num;
}Uart5_queue_struct;

#pragma pack(pop)



void Usart_FreeRTOS_Semaphore_init(void);
void Usart_FreeRTOS_Queue_init(void);

void USART_Send_bin_RTOS(USART_TypeDef* USARTx,uint8_t* out_buff,uint16_t out_size);
void USART_printf_RTOS(USART_TypeDef* USARTx,char* fmt,...);

#endif
