#ifndef UART5_H
#define UART5_H

#include "stm32f4xx.h"
#include <stdio.h>
#include <string.h>

#define NO_USART_IRQ                 NULL

#define UART5_Tx_BufferSize         1024
#define UART5_Rx_BufferSize         1024

/****************************************************************
                        UART5 IO config 
****************************************************************/
#define UART5_Tx_IO_Group           GPIOC  
#define UART5_Rx_IO_Group           GPIOD
#define UART5_Tx_Clock              RCC_AHB1Periph_GPIOC
#define UART5_Rx_Clock              RCC_AHB1Periph_GPIOD
#define UART5_Tx_Pin                GPIO_Pin_12
#define UART5_Rx_Pin                GPIO_Pin_2
#define UART5_Tx_PinSource          GPIO_PinSource12
#define UART5_Rx_PinSource          GPIO_PinSource2

/****************************************************************
                        UART5 DMA config 
****************************************************************/
#define UART5_TX_DMA_Source         DMA1_Stream7
#define UART5_TX_DMA_CH             DMA_Channel_4
#define UART5_TX_DMA_flag           DMA_FLAG_TCIF7

#define UART5_RX_DMA_Source         DMA1_Stream0   /*Alternate: DMA2_Stream5 */
#define UART5_RX_DMA_CH             DMA_Channel_4
#define UART5_RX_DMA_flag           DMA_FLAG_TCIF0

void UART5_Init(uint32_t BaudRate,uint16_t USART_irq_mode,FunctionalState Dma_swtich);
void UART5_DMA_Send(uint8_t* send_buff,uint16_t send_size);
void UART5_Reset_RX(void);

uint16_t UART5_GetReceiveDataNumber(void);

void USART5_Send_bin(uint8_t* out_buff,uint16_t out_size);

#endif
