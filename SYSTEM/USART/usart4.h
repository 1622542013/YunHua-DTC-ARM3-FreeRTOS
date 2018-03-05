#ifndef UART4_H
#define UART4_H

#include "stm32f4xx.h"
#include <stdio.h>
#include <string.h>

#define NO_USART_IRQ                 NULL

#define UART4_Tx_BufferSize         512
#define UART4_Rx_BufferSize         512

/****************************************************************
                        UART4 IO config 
****************************************************************/

/*==================================   alternative   =================================*/

//#define UART4_Tx_IO_Group           GPIOA  
//#define UART4_Rx_IO_Group           GPIOA
//#define UART4_Tx_Clock              RCC_AHB1Periph_GPIOA
//#define UART4_Rx_Clock              RCC_AHB1Periph_GPIOA
//#define UART4_Tx_Pin                GPIO_Pin_0
//#define UART4_Rx_Pin                GPIO_Pin_1
//#define UART4_Tx_PinSource          GPIO_PinSource0
//#define UART4_Rx_PinSource          GPIO_PinSource1

#define UART4_Tx_IO_Group           GPIOC  
#define UART4_Rx_IO_Group           GPIOC
#define UART4_Tx_Clock              RCC_AHB1Periph_GPIOC
#define UART4_Rx_Clock              RCC_AHB1Periph_GPIOC
#define UART4_Tx_Pin                GPIO_Pin_10
#define UART4_Rx_Pin                GPIO_Pin_11
#define UART4_Tx_PinSource          GPIO_PinSource10
#define UART4_Rx_PinSource          GPIO_PinSource11
/*====================================================================================*/

/****************************************************************
                        UART4 DMA config 
****************************************************************/
#define UART4_TX_DMA_Source         DMA1_Stream4
#define UART4_TX_DMA_CH             DMA_Channel_4
#define UART4_TX_DMA_flag           DMA_FLAG_TCIF4

#define UART4_RX_DMA_Source         DMA1_Stream2   
#define UART4_RX_DMA_CH             DMA_Channel_4
#define UART4_RX_DMA_flag           DMA_FLAG_TCIF2

void UART4_Init(uint32_t BaudRate,uint16_t USART_irq_mode,FunctionalState Dma_swtich);
void UART4_DMA_Send(uint8_t* send_buff,uint16_t send_size);
void UART4_Reset_RX(void);

uint16_t UART4_GetReceiveDataNumber(void);

void UART4_Send_data(uint8_t* out_buff,uint16_t out_size);
void UART4_Send_data_idle(void);
#endif
