#ifndef USART1_H
#define USART1_H

#include "stm32f4xx.h"
#include <stdio.h>
#include <string.h>

#define NO_USART_IRQ                 NULL

#define USART1_Tx_BufferSize         512
#define USART1_Rx_BufferSize         512

/****************************************************************
                        USART1 IO config 
****************************************************************/

/*==================================   alternative   =================================*/
//#define USART1_Tx_IO_Group           GPIOA  
//#define USART1_Rx_IO_Group           GPIOA
//#define USART1_Tx_Clock              RCC_AHB1Periph_GPIOA
//#define USART1_Rx_Clock              RCC_AHB1Periph_GPIOA
//#define USART1_Tx_Pin                GPIO_Pin_9
//#define USART1_Rx_Pin                GPIO_Pin_10
//#define USART1_Tx_PinSource          GPIO_PinSource9
//#define USART1_Rx_PinSource          GPIO_PinSource10

#define USART1_Tx_IO_Group           GPIOB  
#define USART1_Rx_IO_Group           GPIOB
#define USART1_Tx_Clock              RCC_AHB1Periph_GPIOB
#define USART1_Rx_Clock              RCC_AHB1Periph_GPIOB
#define USART1_Tx_Pin                GPIO_Pin_6
#define USART1_Rx_Pin                GPIO_Pin_7
#define USART1_Tx_PinSource          GPIO_PinSource6
#define USART1_Rx_PinSource          GPIO_PinSource7
/*====================================================================================*/

/****************************************************************
                        USART1 DMA config 
****************************************************************/
#define USART1_TX_DMA_Source         DMA2_Stream7
#define USART1_TX_DMA_CH             DMA_Channel_4
#define USART1_TX_DMA_flag           DMA_FLAG_TCIF7

/*==================================   alternative   =================================*/
#define USART1_RX_DMA_Source         DMA2_Stream2   /*Alternate: DMA2_Stream5 */
#define USART1_RX_DMA_CH             DMA_Channel_4
#define USART1_RX_DMA_flag           DMA_FLAG_TCIF2

//#define USART1_RX_DMA_Source         DMA2_Stream5   /*Alternate: DMA2_Stream5 */
//#define USART1_RX_DMA_CH             DMA_Channel_4
//#define USART1_RX_DMA_flag           DMA_FLAG_TCIF5
/*====================================================================================*/

void USART1_Init(uint32_t BaudRate,uint16_t USART_irq_mode,FunctionalState Dma_swtich);
void USART1_DMA_Send(uint8_t* send_buff,uint16_t send_size);
void USART1_Reset_RX(void);

uint16_t USART1_GetReceiveDataNumber(void);

void USART1_Send_bin(uint8_t* out_buff,uint16_t out_size);
void USART1_printf(char* fmt,...);
#endif
