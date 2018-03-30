#ifndef USART6_H
#define USART6_H

#include "stm32f4xx.h"
#include <stdio.h>
#include <string.h>

#define NO_USART_IRQ                 NULL

#define USART6_Tx_BufferSize         512
#define USART6_Rx_BufferSize         512

/****************************************************************
                        USART6 IO config 
****************************************************************/

/*==================================   alternative   =================================*/
#define USART6_Tx_IO_Group           GPIOC  
#define USART6_Rx_IO_Group           GPIOC
#define USART6_Tx_Clock              RCC_AHB1Periph_GPIOC
#define USART6_Rx_Clock              RCC_AHB1Periph_GPIOC
#define USART6_Tx_Pin                GPIO_Pin_6
#define USART6_Rx_Pin                GPIO_Pin_7
#define USART6_Tx_PinSource          GPIO_PinSource6
#define USART6_Rx_PinSource          GPIO_PinSource7

//#define USART6_Tx_IO_Group           GPIOG  
//#define USART6_Rx_IO_Group           GPIOG
//#define USART6_Tx_Clock              RCC_AHB1Periph_GPIOG
//#define USART6_Rx_Clock              RCC_AHB1Periph_GPIOG
//#define USART6_Tx_Pin                GPIO_Pin_14
//#define USART6_Rx_Pin                GPIO_Pin_9
//#define USART6_Tx_PinSource          GPIO_PinSource14
//#define USART6_Rx_PinSource          GPIO_PinSource9
/*====================================================================================*/

/****************************************************************
                        USART6 DMA config 
****************************************************************/

/*==================================   alternative   =================================*/
#define USART6_TX_DMA_Source         DMA2_Stream6
#define USART6_TX_DMA_CH             DMA_Channel_5
#define USART6_TX_DMA_flag           DMA_FLAG_TCIF6

//#define USART6_TX_DMA_Source         DMA2_Stream7
//#define USART6_TX_DMA_CH             DMA_Channel_5
//#define USART6_TX_DMA_flag           DMA_FLAG_TCIF7

/*==================================   alternative   =================================*/
#define USART6_RX_DMA_Source         DMA2_Stream1   /*Alternate: DMA2_Stream5 */
#define USART6_RX_DMA_CH             DMA_Channel_5
#define USART6_RX_DMA_flag           DMA_FLAG_TCIF1

//#define USART6_RX_DMA_Source         DMA2_Stream2   /*Alternate: DMA2_Stream5 */
//#define USART6_RX_DMA_CH             DMA_Channel_5
//#define USART6_RX_DMA_flag           DMA_FLAG_TCIF2
/*====================================================================================*/

void USART6_Init(uint32_t BaudRate,uint16_t USART_irq_mode,FunctionalState Dma_swtich);
void USART6_DMA_Send(uint8_t* send_buff,uint16_t send_size);
void USART6_Reset_RX(void);

uint16_t USART6_GetReceiveDataNumber(void);

void USART6_Send_bin(uint8_t* out_buff,uint16_t out_size);

#endif
