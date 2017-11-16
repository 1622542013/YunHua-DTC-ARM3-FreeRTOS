/****************************************************************
 * Copyright (C) 2016, Yagro, all right reserved.
 * File name:    USART.h
 * Date:         2016.03.22
 * Description:  USART Driver
*****************************************************************/

#ifndef __USART_H
#define __USART_H

/****************************************************************
 *                        Header include
*****************************************************************/
#include "stm32f4xx.h"

/****************************************************************
 *                       Macro definition
*****************************************************************/
#define USART1_BufferSize_Tx  ((uint16_t)1024)
#define USART1_BufferSize_Rx  ((uint16_t)1024)

#define USART2_BufferSize_Tx  ((uint16_t)1)
#define USART2_BufferSize_Rx  ((uint16_t)1)

#define USART3_BufferSize_Tx  ((uint16_t)1)
#define USART3_BufferSize_Rx  ((uint16_t)1)

#define USART4_BufferSize_Tx  ((uint16_t)1)
#define USART4_BufferSize_Rx  ((uint16_t)1)

#define USART5_BufferSize_Tx  ((uint16_t)1)
#define USART5_BufferSize_Rx  ((uint16_t)1)

#define USART6_BufferSize_Tx  ((uint16_t)1)
#define USART6_BufferSize_Rx  ((uint16_t)1)

/********************* USART1 Configuration *********************/

#define RCC_AHB1Periph_USART1_Tx     RCC_AHB1Periph_GPIOA
#define RCC_AHB1Periph_USART1_Rx     RCC_AHB1Periph_GPIOA

#define GPIO_USART1_Tx               GPIOA
#define GPIO_USART1_Rx               GPIOA

#define GPIO_Pin_USART1_Tx           GPIO_Pin_9
#define GPIO_Pin_USART1_Rx           GPIO_Pin_10

#define GPIO_PinSource_USART1_Tx     GPIO_PinSource9
#define GPIO_PinSource_USART1_Rx     GPIO_PinSource10

/****************************************************************/

/********************* USART2 Configuration *********************/

#define RCC_AHB1Periph_USART2_Tx     RCC_AHB1Periph_GPIOA
#define RCC_AHB1Periph_USART2_Rx     RCC_AHB1Periph_GPIOA

#define GPIO_USART2_Tx               GPIOA
#define GPIO_USART2_Rx               GPIOA

#define GPIO_Pin_USART2_Tx           GPIO_Pin_2
#define GPIO_Pin_USART2_Rx           GPIO_Pin_3

#define GPIO_PinSource_USART2_Tx     GPIO_PinSource2
#define GPIO_PinSource_USART2_Rx     GPIO_PinSource3

/****************************************************************/

/********************* USART3 Configuration *********************/

#define RCC_AHB1Periph_USART3_Tx     RCC_AHB1Periph_GPIOB
#define RCC_AHB1Periph_USART3_Rx     RCC_AHB1Periph_GPIOB

#define GPIO_USART3_Tx               GPIOB
#define GPIO_USART3_Rx               GPIOB

#define GPIO_Pin_USART3_Tx           GPIO_Pin_10
#define GPIO_Pin_USART3_Rx           GPIO_Pin_11

#define GPIO_PinSource_USART3_Tx     GPIO_PinSource10
#define GPIO_PinSource_USART3_Rx     GPIO_PinSource11

/****************************************************************/

/********************* USART4 Configuration *********************/

#define RCC_AHB1Periph_USART4_Tx     RCC_AHB1Periph_GPIOC
#define RCC_AHB1Periph_USART4_Rx     RCC_AHB1Periph_GPIOC

#define GPIO_USART4_Tx               GPIOC
#define GPIO_USART4_Rx               GPIOC

#define GPIO_Pin_USART4_Tx           GPIO_Pin_10
#define GPIO_Pin_USART4_Rx           GPIO_Pin_11

#define GPIO_PinSource_USART4_Tx     GPIO_PinSource10
#define GPIO_PinSource_USART4_Rx     GPIO_PinSource11

/****************************************************************/

/********************* USART5 Configuration *********************/

#define RCC_AHB1Periph_USART5_Tx     RCC_AHB1Periph_GPIOC
#define RCC_AHB1Periph_USART5_Rx     RCC_AHB1Periph_GPIOD

#define GPIO_USART5_Tx               GPIOC
#define GPIO_USART5_Rx               GPIOD

#define GPIO_Pin_USART5_Tx           GPIO_Pin_12
#define GPIO_Pin_USART5_Rx           GPIO_Pin_2

#define GPIO_PinSource_USART5_Tx     GPIO_PinSource12
#define GPIO_PinSource_USART5_Rx     GPIO_PinSource2

/****************************************************************/

/********************* USART6 Configuration *********************/

#define RCC_AHB1Periph_USART6_Tx     RCC_AHB1Periph_GPIOC
#define RCC_AHB1Periph_USART6_Rx     RCC_AHB1Periph_GPIOC

#define GPIO_USART6_Tx               GPIOC
#define GPIO_USART6_Rx               GPIOC

#define GPIO_Pin_USART6_Tx           GPIO_Pin_6
#define GPIO_Pin_USART6_Rx           GPIO_Pin_7

#define GPIO_PinSource_USART6_Tx     GPIO_PinSource6
#define GPIO_PinSource_USART6_Rx     GPIO_PinSource7

/****************************************************************/

#define USART_1 ((uint8_t)0x01)
#define USART_2 ((uint8_t)0x02)
#define USART_3 ((uint8_t)0x03)
#define USART_4 ((uint8_t)0x04)
#define USART_5 ((uint8_t)0x05)
#define USART_6 ((uint8_t)0x06)

#define BAUD_RATE_2400      ((uint32_t)2400)
#define BAUD_RATE_4800      ((uint32_t)4800)
#define BAUD_RATE_9600      ((uint32_t)9600)
#define BAUD_RATE_19200     ((uint32_t)19200)
#define BAUD_RATE_38400     ((uint32_t)38400)
#define BAUD_RATE_57600     ((uint32_t)57600)
#define BAUD_RATE_115200    ((uint32_t)115200)
#define BAUD_RATE_230400    ((uint32_t)230400)
#define BAUD_RATE_460800    ((uint32_t)460800)
#define BAUD_RATE_921600    ((uint32_t)921600)

/****************************************************************
 *                       Type definition
*****************************************************************/


/****************************************************************
 *                     Structure definition
*****************************************************************/



#ifdef __cplusplus
 extern "C" {
#endif  /* __cplusplus */

/****************************************************************
 *                     Variable declaration
*****************************************************************/
extern uint8_t USART1_Buffer_Tx[USART1_BufferSize_Tx];
extern uint8_t USART1_Buffer_Rx[USART1_BufferSize_Rx];

extern uint8_t USART2_Buffer_Tx[USART2_BufferSize_Tx];
extern uint8_t USART2_Buffer_Rx[USART2_BufferSize_Rx];

extern uint8_t USART3_Buffer_Tx[USART3_BufferSize_Tx];
extern uint8_t USART3_Buffer_Rx[USART3_BufferSize_Rx];

extern uint8_t USART4_Buffer_Tx[USART4_BufferSize_Tx];
extern uint8_t USART4_Buffer_Rx[USART4_BufferSize_Rx];

extern uint8_t USART5_Buffer_Tx[USART5_BufferSize_Tx];
extern uint8_t USART5_Buffer_Rx[USART5_BufferSize_Rx];

extern uint8_t USART6_Buffer_Tx[USART6_BufferSize_Tx];
extern uint8_t USART6_Buffer_Rx[USART6_BufferSize_Rx];

/****************************************************************
 *                     Function declaration
*****************************************************************/
void USART_Configuration(uint8_t USART_x, uint32_t BaudRate, uint16_t USART_Mode, uint16_t USART_IT, uint16_t USART_DMAReq);
void USART_DMA_TX_Configuration(uint8_t USART_x, uint32_t DMA_Memory0BaseAddr, uint16_t DMA_BufferSize);
void USART_DMA_RX_Configuration(uint8_t USART_x, uint32_t DMA_Memory0BaseAddr, uint16_t DMA_BufferSize);
void USART_DMA_TX(uint8_t USART_x, uint16_t DMA_BufferSize);
void USART_DMA_RX(uint8_t USART_x, uint16_t DMA_BufferSize);
void USART_OUT(uint8_t USART_x, uint8_t* Buffer, uint16_t BufferSize);
void USART_IN(uint8_t USART_x);
uint16_t USART_GetReceiveDataNumber(uint8_t USART_x);

void u1_printf(char* fmt,...);
#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif 	/* __USART_H */
