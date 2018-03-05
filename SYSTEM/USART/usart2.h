#ifndef USART2_H
#define USART2_H

#include "stm32f4xx.h"
#include <stdio.h>
#include <string.h>

#define NO_USART_IRQ                 NULL

#define USART2_Tx_BufferSize         512
#define USART2_Rx_BufferSize         512

/****************************************************************
                        USART2 IO config 
****************************************************************/

/*==================================   alternative   =================================*/
#define USART2_Tx_IO_Group           GPIOA  
#define USART2_Rx_IO_Group           GPIOA
#define USART2_Tx_Clock              RCC_AHB1Periph_GPIOA
#define USART2_Rx_Clock              RCC_AHB1Periph_GPIOA
#define USART2_Tx_Pin                GPIO_Pin_2
#define USART2_Rx_Pin                GPIO_Pin_3
#define USART2_Tx_PinSource          GPIO_PinSource2
#define USART2_Rx_PinSource          GPIO_PinSource3

//#define USART2_Tx_IO_Group           GPIOD  
//#define USART2_Rx_IO_Group           GPIOD
//#define USART2_Tx_Clock              RCC_AHB1Periph_GPIOD
//#define USART2_Rx_Clock              RCC_AHB1Periph_GPIOD
//#define USART2_Tx_Pin                GPIO_Pin_5
//#define USART2_Rx_Pin                GPIO_Pin_6
//#define USART2_Tx_PinSource          GPIO_PinSource5
//#define USART2_Rx_PinSource          GPIO_PinSource6
/*====================================================================================*/

/****************************************************************
                        USART2 DMA config 
****************************************************************/
#define USART2_TX_DMA_Source         DMA1_Stream6
#define USART2_TX_DMA_CH             DMA_Channel_4
#define USART2_TX_DMA_flag           DMA_FLAG_TCIF6

/*==================================   alternative   =================================*/
#define USART2_RX_DMA_Source         DMA1_Stream5   
#define USART2_RX_DMA_CH             DMA_Channel_4
#define USART2_RX_DMA_flag           DMA_FLAG_TCIF5

/*====================================================================================*/

void USART2_Init(uint32_t BaudRate,uint16_t USART_irq_mode,FunctionalState Dma_swtich);
void USART2_DMA_Send(uint8_t* send_buff,uint16_t send_size);
void USART2_Reset_RX(void);

uint16_t USART2_GetReceiveDataNumber(void);

void USART2_Send_bin(uint8_t* out_buff,uint16_t out_size);
void USART2_printf(char* fmt,...);

#endif
