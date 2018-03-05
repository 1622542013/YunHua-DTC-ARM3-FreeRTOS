#ifndef USART3_H
#define USART3_H

#include "stm32f4xx.h"
#include <stdio.h>
#include <string.h>

#define NO_USART_IRQ                 NULL

#define USART3_Tx_BufferSize         512
#define USART3_Rx_BufferSize         512

/****************************************************************
                        USART3 IO config 
****************************************************************/

/*==================================   alternative   =================================*/
#define USART3_Tx_IO_Group           GPIOB  
#define USART3_Rx_IO_Group           GPIOB
#define USART3_Tx_Clock              RCC_AHB1Periph_GPIOB
#define USART3_Rx_Clock              RCC_AHB1Periph_GPIOB
#define USART3_Tx_Pin                GPIO_Pin_10
#define USART3_Rx_Pin                GPIO_Pin_11
#define USART3_Tx_PinSource          GPIO_PinSource10
#define USART3_Rx_PinSource          GPIO_PinSource11

//#define USART3_Tx_IO_Group           GPIOD  
//#define USART3_Rx_IO_Group           GPIOD
//#define USART3_Tx_Clock              RCC_AHB1Periph_GPIOD
//#define USART3_Rx_Clock              RCC_AHB1Periph_GPIOD
//#define USART3_Tx_Pin                GPIO_Pin_8
//#define USART3_Rx_Pin                GPIO_Pin_9
//#define USART3_Tx_PinSource          GPIO_PinSource8
//#define USART3_Rx_PinSource          GPIO_PinSource9

//#define USART3_Tx_IO_Group           GPIOC  
//#define USART3_Rx_IO_Group           GPIOC
//#define USART3_Tx_Clock              RCC_AHB1Periph_GPIOC
//#define USART3_Rx_Clock              RCC_AHB1Periph_GPIOC
//#define USART3_Tx_Pin                GPIO_Pin_10
//#define USART3_Rx_Pin                GPIO_Pin_11
//#define USART3_Tx_PinSource          GPIO_PinSource10
//#define USART3_Rx_PinSource          GPIO_PinSource11
/*====================================================================================*/

/****************************************************************
                        USART3 DMA config 
****************************************************************/

/*==================================   alternative   =================================*/
#define USART3_TX_DMA_Source         DMA1_Stream3
#define USART3_TX_DMA_CH             DMA_Channel_4
#define USART3_TX_DMA_flag           DMA_FLAG_TCIF3

//#define USART3_TX_DMA_Source         DMA1_Stream4
//#define USART3_TX_DMA_CH             DMA_Channel_7
//#define USART3_TX_DMA_flag           DMA_FLAG_TCIF4
/*====================================================================================*/

/*==================================   alternative   =================================*/
#define USART3_RX_DMA_Source         DMA1_Stream1   
#define USART3_RX_DMA_CH             DMA_Channel_4
#define USART3_RX_DMA_flag           DMA_FLAG_TCIF1

/*====================================================================================*/

void USART3_Init(uint32_t BaudRate,uint16_t USART_irq_mode,FunctionalState Dma_swtich);
void USART3_DMA_Send(uint8_t* send_buff,uint16_t send_size);
void USART3_Reset_RX(void);

uint16_t USART3_GetReceiveDataNumber(void);

void USART3_Send_data(uint8_t* out_buff,uint16_t out_size);
void USART3_Send_data_idle(void);
#endif
