/****************************************************************
 * Copyright (C) 2016, Yagro, all right reserved.
 * File name:    USART.c
 * Date:         2016.03.22
 * Description:  USART Driver
*****************************************************************/

/****************************************************************
 *                        Header include
*****************************************************************/
#include "USART.h"
#include <string.h>

/****************************************************************
 *                       Global variables
*****************************************************************/
uint8_t USART1_Buffer_Tx[USART1_BufferSize_Tx];
uint8_t USART1_Buffer_Rx[USART1_BufferSize_Rx];

uint8_t USART2_Buffer_Tx[USART2_BufferSize_Tx];
uint8_t USART2_Buffer_Rx[USART2_BufferSize_Rx];

uint8_t USART3_Buffer_Tx[USART3_BufferSize_Tx];
uint8_t USART3_Buffer_Rx[USART3_BufferSize_Rx];

uint8_t USART4_Buffer_Tx[USART4_BufferSize_Tx];
uint8_t USART4_Buffer_Rx[USART4_BufferSize_Rx];

uint8_t USART5_Buffer_Tx[USART5_BufferSize_Tx];
uint8_t USART5_Buffer_Rx[USART5_BufferSize_Rx];

uint8_t USART6_Buffer_Tx[USART6_BufferSize_Tx];
uint8_t USART6_Buffer_Rx[USART6_BufferSize_Rx];

/****************************************************************
 *                     Function declaration
*****************************************************************/


/****************************************************************
 *                     Function definition
*****************************************************************/

/****************************************************************
 * Function:    USART_Configuration
 * Description: USART Configuration.
 * Input:       USART_x
 *              BaudRate
 *              USART_Mode
 *              USART_IT
 *              USART_DMAReq
 * Output:
 * Return:
*****************************************************************/
void USART_Configuration(uint8_t USART_x, uint32_t BaudRate, uint16_t USART_Mode, uint16_t USART_IT, uint16_t USART_DMAReq)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;

  if(USART_x == USART_1)
  {
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_USART1_Tx, ENABLE);  /* Enable USART1_Tx GPIO clock */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_USART1_Rx, ENABLE);  /* Enable USART1_Rx GPIO clock */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);     /* Enable USART1 clock */

    GPIO_PinAFConfig(GPIO_USART1_Tx, GPIO_PinSource_USART1_Tx, GPIO_AF_USART1); /* Connect GPIOxn to USART1_Tx */
    GPIO_PinAFConfig(GPIO_USART1_Rx, GPIO_PinSource_USART1_Rx, GPIO_AF_USART1); /* Connect GPIOxn to USART1_Rx */

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_USART1_Tx;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIO_USART1_Tx, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_USART1_Rx;
    GPIO_Init(GPIO_USART1_Rx, &GPIO_InitStructure);
  }

  else if(USART_x == USART_2)
  {
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_USART2_Tx, ENABLE);  /* Enable USART2_Tx GPIO clock */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_USART2_Rx, ENABLE);  /* Enable USART2_Rx GPIO clock */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);     /* Enable USART2 clock */

    GPIO_PinAFConfig(GPIO_USART2_Tx, GPIO_PinSource_USART2_Tx, GPIO_AF_USART2);	/* Connect GPIOxn to USART2_Tx */
    GPIO_PinAFConfig(GPIO_USART2_Rx, GPIO_PinSource_USART2_Rx, GPIO_AF_USART2);	/* Connect GPIOxn to USART2_Rx */

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_USART2_Tx;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIO_USART2_Tx, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_USART2_Rx;
    GPIO_Init(GPIO_USART2_Rx, &GPIO_InitStructure);
  }

  else if(USART_x == USART_3)
  {
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_USART3_Tx, ENABLE);  /* Enable USART3_Tx GPIO clock */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_USART3_Rx, ENABLE);  /* Enable USART3_Rx GPIO clock */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);     /* Enable USART3 clock */

    GPIO_PinAFConfig(GPIO_USART3_Tx, GPIO_PinSource_USART3_Tx, GPIO_AF_USART3); /* Connect GPIOxn to USART3_Tx */
    GPIO_PinAFConfig(GPIO_USART3_Rx, GPIO_PinSource_USART3_Rx, GPIO_AF_USART3); /* Connect GPIOxn to USART3_Rx */

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_USART3_Tx;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIO_USART3_Tx, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_USART3_Rx;
    GPIO_Init(GPIO_USART3_Rx, &GPIO_InitStructure);
  }

  else if(USART_x == USART_4)
  {
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_USART4_Tx, ENABLE);  /* Enable USART4_Tx GPIO clock */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_USART4_Rx, ENABLE);  /* Enable USART4_Rx GPIO clock */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);      /* Enable USART4 clock */

    GPIO_PinAFConfig(GPIO_USART4_Tx, GPIO_PinSource_USART4_Tx, GPIO_AF_UART4);  /* Connect GPIOxn to USART4_Tx */
    GPIO_PinAFConfig(GPIO_USART4_Rx, GPIO_PinSource_USART4_Rx, GPIO_AF_UART4);  /* Connect GPIOxn to USART4_Rx */

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_USART4_Tx;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIO_USART4_Tx, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_USART4_Rx;
    GPIO_Init(GPIO_USART4_Rx, &GPIO_InitStructure);
  }
  
  else if(USART_x == USART_5)
  {
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_USART5_Tx, ENABLE);  /* Enable USART5_Tx GPIO clock */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_USART5_Rx, ENABLE);  /* Enable USART5_Rx GPIO clock */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);      /* Enable USART5 clock */

    GPIO_PinAFConfig(GPIO_USART5_Tx, GPIO_PinSource_USART5_Tx, GPIO_AF_UART5);  /* Connect GPIOxn to USART5_Tx */
    GPIO_PinAFConfig(GPIO_USART5_Rx, GPIO_PinSource_USART5_Rx, GPIO_AF_UART5);  /* Connect GPIOxn to USART5_Rx */

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_USART5_Tx;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIO_USART5_Tx, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_USART5_Rx;
    GPIO_Init(GPIO_USART5_Rx, &GPIO_InitStructure);
  }
  
  else if(USART_x == USART_6)
  {
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_USART6_Tx, ENABLE);  /* Enable USART6_Tx GPIO clock */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_USART6_Rx, ENABLE);  /* Enable USART6_Rx GPIO clock */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6, ENABLE);     /* Enable USART6 clock */

    GPIO_PinAFConfig(GPIO_USART6_Tx, GPIO_PinSource_USART6_Tx, GPIO_AF_USART6);  /* Connect GPIOxn to USART6_Tx */
    GPIO_PinAFConfig(GPIO_USART6_Rx, GPIO_PinSource_USART6_Rx, GPIO_AF_USART6);  /* Connect GPIOxn to USART6_Rx */

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_USART6_Tx;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIO_USART6_Tx, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_USART6_Rx;
    GPIO_Init(GPIO_USART6_Rx, &GPIO_InitStructure);
  }

  USART_InitStructure.USART_BaudRate = BaudRate;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode;

  if(USART_x == USART_1)
  {
    USART_Init(USART1, &USART_InitStructure);

    if(USART_DMAReq == USART_DMAReq_Tx)
      USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);  /* Enabling the DMA request specified USART1 */
    if(USART_DMAReq == USART_DMAReq_Rx)
      USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);  /* Enabling the DMA request specified USART1 */
    if(USART_DMAReq == (USART_DMAReq_Tx|USART_DMAReq_Rx))
    {
      USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);  /* Enabling the DMA request specified USART1 */
      USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);  /* Enabling the DMA request specified USART1 */
    }

    if( (USART_IT & USART_IT_PE) == USART_IT_PE )
    {
      USART_ITConfig(USART1, USART_IT_PE, ENABLE); /* Enabling interrupts specified USART1 */
    }
    if( (USART_IT & USART_IT_TXE) == USART_IT_TXE )
    {
      USART_ITConfig(USART1, USART_IT_TXE, ENABLE); /* Enabling interrupts specified USART1 */
    }
    if( (USART_IT & USART_IT_TC) == USART_IT_TC )
    {
      USART_ITConfig(USART1, USART_IT_TC, ENABLE); /* Enabling interrupts specified USART1 */
    }
    if( (USART_IT & USART_IT_RXNE) == USART_IT_RXNE )
    {
      USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); /* Enabling interrupts specified USART1 */
    }
    if( (USART_IT & USART_IT_ORE_RX) == USART_IT_ORE_RX )
    {
      USART_ITConfig(USART1, USART_IT_ORE_RX, ENABLE); /* Enabling interrupts specified USART1 */
    }
    if( (USART_IT & USART_IT_IDLE) == USART_IT_IDLE )
    {
      USART_ITConfig(USART1, USART_IT_IDLE, ENABLE); /* Enabling interrupts specified USART1 */
    }
    if( (USART_IT & USART_IT_LBD) == USART_IT_LBD )
    {
      USART_ITConfig(USART1, USART_IT_LBD, ENABLE); /* Enabling interrupts specified USART1 */
    }
    if( (USART_IT & USART_IT_CTS) == USART_IT_CTS )
    {
      USART_ITConfig(USART1, USART_IT_CTS, ENABLE); /* Enabling interrupts specified USART1 */
    }
    if( (USART_IT & USART_IT_ERR) == USART_IT_ERR )
    {
      USART_ITConfig(USART1, USART_IT_ERR, ENABLE); /* Enabling interrupts specified USART1 */
    }
    if( (USART_IT & USART_IT_ORE_ER) == USART_IT_ORE_ER )
    {
      USART_ITConfig(USART1, USART_IT_ORE_ER, ENABLE); /* Enabling interrupts specified USART1 */
    }
    if( (USART_IT & USART_IT_NE) == USART_IT_NE )
    {
      USART_ITConfig(USART1, USART_IT_NE, ENABLE); /* Enabling interrupts specified USART1 */
    }
    if( (USART_IT & USART_IT_FE) == USART_IT_FE )
    {
      USART_ITConfig(USART1, USART_IT_FE, ENABLE); /* Enabling interrupts specified USART1 */
    }

    USART_Cmd(USART1, ENABLE);
  }

  else if(USART_x == USART_2)
  {
    USART_Init(USART2, &USART_InitStructure);

    if(USART_DMAReq == USART_DMAReq_Tx)
      USART_DMACmd(USART2, USART_DMAReq_Tx, ENABLE);  /* Enabling the DMA request specified USART2 */
    if(USART_DMAReq == USART_DMAReq_Rx)
      USART_DMACmd(USART2, USART_DMAReq_Rx, ENABLE);  /* Enabling the DMA request specified USART2 */
    if(USART_DMAReq == (USART_DMAReq_Tx|USART_DMAReq_Rx))
    {
      USART_DMACmd(USART2, USART_DMAReq_Tx, ENABLE);  /* Enabling the DMA request specified USART2 */
      USART_DMACmd(USART2, USART_DMAReq_Rx, ENABLE);  /* Enabling the DMA request specified USART2 */
    }

    if( (USART_IT & USART_IT_PE) == USART_IT_PE )
    {
      USART_ITConfig(USART2, USART_IT_PE, ENABLE); /* Enabling interrupts specified USART2 */
    }
    if( (USART_IT & USART_IT_TXE) == USART_IT_TXE )
    {
      USART_ITConfig(USART2, USART_IT_TXE, ENABLE); /* Enabling interrupts specified USART2 */
    }
    if( (USART_IT & USART_IT_TC) == USART_IT_TC )
    {
      USART_ITConfig(USART2, USART_IT_TC, ENABLE); /* Enabling interrupts specified USART2 */
    }
    if( (USART_IT & USART_IT_RXNE) == USART_IT_RXNE )
    {
      USART_ITConfig(USART2, USART_IT_RXNE, ENABLE); /* Enabling interrupts specified USART2 */
    }
    if( (USART_IT & USART_IT_ORE_RX) == USART_IT_ORE_RX )
    {
      USART_ITConfig(USART2, USART_IT_ORE_RX, ENABLE); /* Enabling interrupts specified USART2 */
    }
    if( (USART_IT & USART_IT_IDLE) == USART_IT_IDLE )
    {
      USART_ITConfig(USART2, USART_IT_IDLE, ENABLE); /* Enabling interrupts specified USART2 */
    }
    if( (USART_IT & USART_IT_LBD) == USART_IT_LBD )
    {
      USART_ITConfig(USART2, USART_IT_LBD, ENABLE); /* Enabling interrupts specified USART2 */
    }
    if( (USART_IT & USART_IT_CTS) == USART_IT_CTS )
    {
      USART_ITConfig(USART2, USART_IT_CTS, ENABLE); /* Enabling interrupts specified USART2 */
    }
    if( (USART_IT & USART_IT_ERR) == USART_IT_ERR )
    {
      USART_ITConfig(USART2, USART_IT_ERR, ENABLE); /* Enabling interrupts specified USART2 */
    }
    if( (USART_IT & USART_IT_ORE_ER) == USART_IT_ORE_ER )
    {
      USART_ITConfig(USART2, USART_IT_ORE_ER, ENABLE); /* Enabling interrupts specified USART2 */
    }
    if( (USART_IT & USART_IT_NE) == USART_IT_NE )
    {
      USART_ITConfig(USART2, USART_IT_NE, ENABLE); /* Enabling interrupts specified USART2 */
    }
    if( (USART_IT & USART_IT_FE) == USART_IT_FE )
    {
      USART_ITConfig(USART2, USART_IT_FE, ENABLE); /* Enabling interrupts specified USART2 */
    }

    USART_Cmd(USART2, ENABLE);
  }

  else if(USART_x == USART_3)
  {
    USART_Init(USART3, &USART_InitStructure);

    if(USART_DMAReq == USART_DMAReq_Tx)
      USART_DMACmd(USART3, USART_DMAReq_Tx, ENABLE);  /* Enabling the DMA request specified USART3 */
    if(USART_DMAReq == USART_DMAReq_Rx)
      USART_DMACmd(USART3, USART_DMAReq_Rx, ENABLE);  /* Enabling the DMA request specified USART3 */
    if(USART_DMAReq == (USART_DMAReq_Tx|USART_DMAReq_Rx))
    {
      USART_DMACmd(USART3, USART_DMAReq_Tx, ENABLE);  /* Enabling the DMA request specified USART3 */
      USART_DMACmd(USART3, USART_DMAReq_Rx, ENABLE);  /* Enabling the DMA request specified USART3 */
    }

    if( (USART_IT & USART_IT_PE) == USART_IT_PE )
    {
      USART_ITConfig(USART3, USART_IT_PE, ENABLE); /* Enabling interrupts specified USART3 */
    }
    if( (USART_IT & USART_IT_TXE) == USART_IT_TXE )
    {
      USART_ITConfig(USART3, USART_IT_TXE, ENABLE); /* Enabling interrupts specified USART3 */
    }
    if( (USART_IT & USART_IT_TC) == USART_IT_TC )
    {
      USART_ITConfig(USART3, USART_IT_TC, ENABLE); /* Enabling interrupts specified USART3 */
    }
    if( (USART_IT & USART_IT_RXNE) == USART_IT_RXNE )
    {
      USART_ITConfig(USART3, USART_IT_RXNE, ENABLE); /* Enabling interrupts specified USART3 */
    }
    if( (USART_IT & USART_IT_ORE_RX) == USART_IT_ORE_RX )
    {
      USART_ITConfig(USART3, USART_IT_ORE_RX, ENABLE); /* Enabling interrupts specified USART3 */
    }
    if( (USART_IT & USART_IT_IDLE) == USART_IT_IDLE )
    {
      USART_ITConfig(USART3, USART_IT_IDLE, ENABLE); /* Enabling interrupts specified USART3 */
    }
    if( (USART_IT & USART_IT_LBD) == USART_IT_LBD )
    {
      USART_ITConfig(USART3, USART_IT_LBD, ENABLE); /* Enabling interrupts specified USART3 */
    }
    if( (USART_IT & USART_IT_CTS) == USART_IT_CTS )
    {
      USART_ITConfig(USART3, USART_IT_CTS, ENABLE); /* Enabling interrupts specified USART3 */
    }
    if( (USART_IT & USART_IT_ERR) == USART_IT_ERR )
    {
      USART_ITConfig(USART3, USART_IT_ERR, ENABLE); /* Enabling interrupts specified USART3 */
    }
    if( (USART_IT & USART_IT_ORE_ER) == USART_IT_ORE_ER )
    {
      USART_ITConfig(USART3, USART_IT_ORE_ER, ENABLE); /* Enabling interrupts specified USART3 */
    }
    if( (USART_IT & USART_IT_NE) == USART_IT_NE )
    {
      USART_ITConfig(USART3, USART_IT_NE, ENABLE); /* Enabling interrupts specified USART3 */
    }
    if( (USART_IT & USART_IT_FE) == USART_IT_FE )
    {
      USART_ITConfig(USART3, USART_IT_FE, ENABLE); /* Enabling interrupts specified USART3 */
    }

    USART_Cmd(USART3, ENABLE);
  }

  else if(USART_x == USART_4)
  {
    USART_Init(UART4, &USART_InitStructure);

    if(USART_DMAReq == USART_DMAReq_Tx)
      USART_DMACmd(UART4, USART_DMAReq_Tx, ENABLE); /* Enabling the DMA request specified USART4 */
    if(USART_DMAReq == USART_DMAReq_Rx)
      USART_DMACmd(UART4, USART_DMAReq_Rx, ENABLE); /* Enabling the DMA request specified USART4 */
    if(USART_DMAReq == (USART_DMAReq_Tx|USART_DMAReq_Rx))
    {
      USART_DMACmd(UART4, USART_DMAReq_Tx, ENABLE); /* Enabling the DMA request specified USART4 */
      USART_DMACmd(UART4, USART_DMAReq_Rx, ENABLE); /* Enabling the DMA request specified USART4 */
    }

    if( (USART_IT & USART_IT_PE) == USART_IT_PE )
    {
      USART_ITConfig(UART4, USART_IT_PE, ENABLE); /* Enabling interrupts specified USART4 */
    }
    if( (USART_IT & USART_IT_TXE) == USART_IT_TXE )
    {
      USART_ITConfig(UART4, USART_IT_TXE, ENABLE); /* Enabling interrupts specified USART4 */
    }
    if( (USART_IT & USART_IT_TC) == USART_IT_TC )
    {
      USART_ITConfig(UART4, USART_IT_TC, ENABLE); /* Enabling interrupts specified USART4 */
    }
    if( (USART_IT & USART_IT_RXNE) == USART_IT_RXNE )
    {
      USART_ITConfig(UART4, USART_IT_RXNE, ENABLE); /* Enabling interrupts specified USART4 */
    }
    if( (USART_IT & USART_IT_ORE_RX) == USART_IT_ORE_RX )
    {
      USART_ITConfig(UART4, USART_IT_ORE_RX, ENABLE); /* Enabling interrupts specified USART4 */
    }
    if( (USART_IT & USART_IT_IDLE) == USART_IT_IDLE )
    {
      USART_ITConfig(UART4, USART_IT_IDLE, ENABLE); /* Enabling interrupts specified USART4 */
    }
    if( (USART_IT & USART_IT_LBD) == USART_IT_LBD )
    {
      USART_ITConfig(UART4, USART_IT_LBD, ENABLE); /* Enabling interrupts specified USART4 */
    }
    if( (USART_IT & USART_IT_CTS) == USART_IT_CTS )
    {
      USART_ITConfig(UART4, USART_IT_CTS, ENABLE); /* Enabling interrupts specified USART4 */
    }
    if( (USART_IT & USART_IT_ERR) == USART_IT_ERR )
    {
      USART_ITConfig(UART4, USART_IT_ERR, ENABLE); /* Enabling interrupts specified USART4 */
    }
    if( (USART_IT & USART_IT_ORE_ER) == USART_IT_ORE_ER )
    {
      USART_ITConfig(UART4, USART_IT_ORE_ER, ENABLE); /* Enabling interrupts specified USART4 */
    }
    if( (USART_IT & USART_IT_NE) == USART_IT_NE )
    {
      USART_ITConfig(UART4, USART_IT_NE, ENABLE); /* Enabling interrupts specified USART4 */
    }
    if( (USART_IT & USART_IT_FE) == USART_IT_FE )
    {
      USART_ITConfig(UART4, USART_IT_FE, ENABLE); /* Enabling interrupts specified USART4 */
    }

    USART_Cmd(UART4, ENABLE);
  }
  
  else if(USART_x == USART_5)
  {
    USART_Init(UART5, &USART_InitStructure);

    if(USART_DMAReq == USART_DMAReq_Tx)
      USART_DMACmd(UART5, USART_DMAReq_Tx, ENABLE); /* Enabling the DMA request specified USART5 */
    if(USART_DMAReq == USART_DMAReq_Rx)
      USART_DMACmd(UART5, USART_DMAReq_Rx, ENABLE); /* Enabling the DMA request specified USART5 */
    if(USART_DMAReq == (USART_DMAReq_Tx|USART_DMAReq_Rx))
    {
      USART_DMACmd(UART5, USART_DMAReq_Tx, ENABLE); /* Enabling the DMA request specified USART5 */
      USART_DMACmd(UART5, USART_DMAReq_Rx, ENABLE); /* Enabling the DMA request specified USART5 */
    }

    if( (USART_IT & USART_IT_PE) == USART_IT_PE )
    {
      USART_ITConfig(UART5, USART_IT_PE, ENABLE); /* Enabling interrupts specified USART5 */
    }
    if( (USART_IT & USART_IT_TXE) == USART_IT_TXE )
    {
      USART_ITConfig(UART5, USART_IT_TXE, ENABLE); /* Enabling interrupts specified USART5 */
    }
    if( (USART_IT & USART_IT_TC) == USART_IT_TC )
    {
      USART_ITConfig(UART5, USART_IT_TC, ENABLE); /* Enabling interrupts specified USART5 */
    }
    if( (USART_IT & USART_IT_RXNE) == USART_IT_RXNE )
    {
      USART_ITConfig(UART5, USART_IT_RXNE, ENABLE); /* Enabling interrupts specified USART5 */
    }
    if( (USART_IT & USART_IT_ORE_RX) == USART_IT_ORE_RX )
    {
      USART_ITConfig(UART5, USART_IT_ORE_RX, ENABLE); /* Enabling interrupts specified USART5 */
    }
    if( (USART_IT & USART_IT_IDLE) == USART_IT_IDLE )
    {
      USART_ITConfig(UART5, USART_IT_IDLE, ENABLE); /* Enabling interrupts specified USART5 */
    }
    if( (USART_IT & USART_IT_LBD) == USART_IT_LBD )
    {
      USART_ITConfig(UART5, USART_IT_LBD, ENABLE); /* Enabling interrupts specified USART5 */
    }
    if( (USART_IT & USART_IT_CTS) == USART_IT_CTS )
    {
      USART_ITConfig(UART5, USART_IT_CTS, ENABLE); /* Enabling interrupts specified USART5 */
    }
    if( (USART_IT & USART_IT_ERR) == USART_IT_ERR )
    {
      USART_ITConfig(UART5, USART_IT_ERR, ENABLE); /* Enabling interrupts specified USART5 */
    }
    if( (USART_IT & USART_IT_ORE_ER) == USART_IT_ORE_ER )
    {
      USART_ITConfig(UART5, USART_IT_ORE_ER, ENABLE); /* Enabling interrupts specified USART5 */
    }
    if( (USART_IT & USART_IT_NE) == USART_IT_NE )
    {
      USART_ITConfig(UART5, USART_IT_NE, ENABLE); /* Enabling interrupts specified USART5 */
    }
    if( (USART_IT & USART_IT_FE) == USART_IT_FE )
    {
      USART_ITConfig(UART5, USART_IT_FE, ENABLE); /* Enabling interrupts specified USART5 */
    }

    USART_Cmd(UART5, ENABLE);
  }
  
  else if(USART_x == USART_6)
  {
    USART_Init(USART6, &USART_InitStructure);

    if(USART_DMAReq == USART_DMAReq_Tx)
      USART_DMACmd(USART6, USART_DMAReq_Tx, ENABLE);  /* Enabling the DMA request specified USART6 */
    if(USART_DMAReq == USART_DMAReq_Rx)
      USART_DMACmd(USART6, USART_DMAReq_Rx, ENABLE);  /* Enabling the DMA request specified USART6 */
    if(USART_DMAReq == (USART_DMAReq_Tx|USART_DMAReq_Rx))
    {
      USART_DMACmd(USART6, USART_DMAReq_Tx, ENABLE);  /* Enabling the DMA request specified USART6 */
      USART_DMACmd(USART6, USART_DMAReq_Rx, ENABLE);  /* Enabling the DMA request specified USART6 */
    }

    if( (USART_IT & USART_IT_PE) == USART_IT_PE )
    {
      USART_ITConfig(USART6, USART_IT_PE, ENABLE); /* Enabling interrupts specified USART6 */
    }
    if( (USART_IT & USART_IT_TXE) == USART_IT_TXE )
    {
      USART_ITConfig(USART6, USART_IT_TXE, ENABLE); /* Enabling interrupts specified USART6 */
    }
    if( (USART_IT & USART_IT_TC) == USART_IT_TC )
    {
      USART_ITConfig(USART6, USART_IT_TC, ENABLE); /* Enabling interrupts specified USART6 */
    }
    if( (USART_IT & USART_IT_RXNE) == USART_IT_RXNE )
    {
      USART_ITConfig(USART6, USART_IT_RXNE, ENABLE); /* Enabling interrupts specified USART6 */
    }
    if( (USART_IT & USART_IT_ORE_RX) == USART_IT_ORE_RX )
    {
      USART_ITConfig(USART6, USART_IT_ORE_RX, ENABLE); /* Enabling interrupts specified USART6 */
    }
    if( (USART_IT & USART_IT_IDLE) == USART_IT_IDLE )
    {
      USART_ITConfig(USART6, USART_IT_IDLE, ENABLE); /* Enabling interrupts specified USART6 */
    }
    if( (USART_IT & USART_IT_LBD) == USART_IT_LBD )
    {
      USART_ITConfig(USART6, USART_IT_LBD, ENABLE); /* Enabling interrupts specified USART6 */
    }
    if( (USART_IT & USART_IT_CTS) == USART_IT_CTS )
    {
      USART_ITConfig(USART6, USART_IT_CTS, ENABLE); /* Enabling interrupts specified USART6 */
    }
    if( (USART_IT & USART_IT_ERR) == USART_IT_ERR )
    {
      USART_ITConfig(USART6, USART_IT_ERR, ENABLE); /* Enabling interrupts specified USART6 */
    }
    if( (USART_IT & USART_IT_ORE_ER) == USART_IT_ORE_ER )
    {
      USART_ITConfig(USART6, USART_IT_ORE_ER, ENABLE); /* Enabling interrupts specified USART6 */
    }
    if( (USART_IT & USART_IT_NE) == USART_IT_NE )
    {
      USART_ITConfig(USART6, USART_IT_NE, ENABLE); /* Enabling interrupts specified USART6 */
    }
    if( (USART_IT & USART_IT_FE) == USART_IT_FE )
    {
      USART_ITConfig(USART6, USART_IT_FE, ENABLE); /* Enabling interrupts specified USART6 */
    }

    USART_Cmd(USART6, ENABLE);
  }
}

/****************************************************************
 * Function:    USART_DMA_TX_Configuration
 * Description: USART DMA TX Configuration.
 * Input:       USART_x
 *              DMA_Memory0BaseAddr
 *              DMA_BufferSize
 * Output:
 * Return:
*****************************************************************/
void USART_DMA_TX_Configuration(uint8_t USART_x, uint32_t DMA_Memory0BaseAddr, uint16_t DMA_BufferSize)
{
  DMA_InitTypeDef DMA_InitStructure;

  if(USART_x == USART_2 || USART_x == USART_3 || USART_x == USART_4 || USART_x == USART_5)
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);  /* Enable DMA1 clock */
  else
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);  /* Enable DMA2 clock */

  if(USART_x == USART_1)
  {
    DMA_DeInit(DMA2_Stream7);
    while(DMA_GetCmdStatus(DMA2_Stream7) != DISABLE);

    DMA_InitStructure.DMA_Channel = DMA_Channel_4;                    /* Channel selection */
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&USART1->DR; /* Set the DMA source: peripheral data register address */
  }

  else if(USART_x == USART_2)
  {
    DMA_DeInit(DMA1_Stream6);
    while(DMA_GetCmdStatus(DMA1_Stream6) != DISABLE);

    DMA_InitStructure.DMA_Channel = DMA_Channel_4;                    /* Channel selection */
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&USART2->DR; /* Set the DMA source: peripheral data register address */
  }

  else if(USART_x == USART_3)
  {
    DMA_DeInit(DMA1_Stream3);
    while(DMA_GetCmdStatus(DMA1_Stream3) != DISABLE);

    DMA_InitStructure.DMA_Channel = DMA_Channel_4;                    /* Channel selection */
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&USART3->DR; /* Set the DMA source: peripheral data register address */
  }

  else if(USART_x == USART_4)
  {
    DMA_DeInit(DMA1_Stream4);
    while(DMA_GetCmdStatus(DMA1_Stream4) != DISABLE);

    DMA_InitStructure.DMA_Channel = DMA_Channel_4;                    /* Channel selection */
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&UART4->DR;  /* Set the DMA source: peripheral data register address */
  }

  else if(USART_x == USART_5)
  {
    DMA_DeInit(DMA1_Stream7);
    while(DMA_GetCmdStatus(DMA1_Stream7) != DISABLE);

    DMA_InitStructure.DMA_Channel = DMA_Channel_4;                    /* Channel selection */
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&UART5->DR;  /* Set the DMA source: peripheral data register address */
  }

  else if(USART_x == USART_6)
  {
    DMA_DeInit(DMA2_Stream6);
    while(DMA_GetCmdStatus(DMA2_Stream6) != DISABLE);

    DMA_InitStructure.DMA_Channel = DMA_Channel_5;                    /* Channel selection */
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&USART6->DR; /* Set the DMA source: peripheral data register address */
  }

  DMA_InitStructure.DMA_Memory0BaseAddr = DMA_Memory0BaseAddr;            /* Memory address */
  DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;                 /* Data Memory to Peripheral */
  DMA_InitStructure.DMA_BufferSize = DMA_BufferSize;                      /* Specifies the DMA channel DMA buffer size */
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;        /* Peripheral address does not increase */
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                 /* Memory address increment */
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; /* Peripheral data width of 8 bit */
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;         /* Memory data width of 8 bit */
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;                           /* Use Normal mode */
  DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;                   /* Medium priority */
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;             /* A single transmission burst memory */
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;     /* Peripherals single transmission burst */

  if(USART_x == USART_1)
  {
    DMA_Init(DMA2_Stream7, &DMA_InitStructure);
    DMA_Cmd(DMA2_Stream7, DISABLE);
  }

  else if(USART_x == USART_2)
  {
    DMA_Init(DMA1_Stream6, &DMA_InitStructure);
    DMA_Cmd(DMA1_Stream6, DISABLE);
  }

  else if(USART_x == USART_3)
  {
    DMA_Init(DMA1_Stream3, &DMA_InitStructure);
    DMA_Cmd(DMA1_Stream3, DISABLE);
  }

  else if(USART_x == USART_4)
  {
    DMA_Init(DMA1_Stream4, &DMA_InitStructure);
    DMA_Cmd(DMA1_Stream4, DISABLE);
  }

  else if(USART_x == USART_5)
  {
    DMA_Init(DMA1_Stream7, &DMA_InitStructure);
    DMA_Cmd(DMA1_Stream7, DISABLE);
  }

  else if(USART_x == USART_6)
  {
    DMA_Init(DMA2_Stream6, &DMA_InitStructure);
    DMA_Cmd(DMA2_Stream6, DISABLE);
  }
  
  /* Starting a byte of data to make a DMA transmission completion flag */
  USART_DMA_TX(USART_x, 1);
}

/****************************************************************
 * Function:    USART_DMA_RX_Configuration
 * Description: USART DMA RX Configuration.
 * Input:       USART_x
 *              DMA_Memory0BaseAddr
 *              DMA_BufferSize
 * Output:
 * Return:
*****************************************************************/
void USART_DMA_RX_Configuration(uint8_t USART_x, uint32_t DMA_Memory0BaseAddr, uint16_t DMA_BufferSize)
{
  DMA_InitTypeDef DMA_InitStructure;

  if(USART_x == USART_2 || USART_x == USART_3 || USART_x == USART_4 || USART_x == USART_5)
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);  /* Enable DMA1 clock */
  else
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);  /* Enable DMA2 clock */

  if(USART_x == USART_1)
  {
    DMA_DeInit(DMA2_Stream2);
    while(DMA_GetCmdStatus(DMA2_Stream2) != DISABLE);

    DMA_InitStructure.DMA_Channel = DMA_Channel_4;                    /* Channel selection */
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&USART1->DR; /* Set the DMA source: peripheral data register address */
  }

  else if(USART_x == USART_2)
  {
    DMA_DeInit(DMA1_Stream5);
    while(DMA_GetCmdStatus(DMA1_Stream5) != DISABLE);

    DMA_InitStructure.DMA_Channel = DMA_Channel_4;                    /* Channel selection */
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&USART2->DR; /* Set the DMA source: peripheral data register address */
  }

  else if(USART_x == USART_3)
  {
    DMA_DeInit(DMA1_Stream1);
    while(DMA_GetCmdStatus(DMA1_Stream1) != DISABLE);

    DMA_InitStructure.DMA_Channel = DMA_Channel_4;                    /* Channel selection */
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&USART3->DR; /* Set the DMA source: peripheral data register address */
  }

  else if(USART_x == USART_4)
  {
    DMA_DeInit(DMA1_Stream2);
    while(DMA_GetCmdStatus(DMA1_Stream2) != DISABLE);

    DMA_InitStructure.DMA_Channel = DMA_Channel_4;                    /* Channel selection */
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&UART4->DR;  /* Set the DMA source: peripheral data register address */
  }
  
  else if(USART_x == USART_5)
  {
    DMA_DeInit(DMA1_Stream0);
    while(DMA_GetCmdStatus(DMA1_Stream0) != DISABLE);

    DMA_InitStructure.DMA_Channel = DMA_Channel_4;                    /* Channel selection */
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&UART5->DR;  /* Set the DMA source: peripheral data register address */
  }
  
  else if(USART_x == USART_6)
  {
    DMA_DeInit(DMA2_Stream1);
    while(DMA_GetCmdStatus(DMA2_Stream1) != DISABLE);

    DMA_InitStructure.DMA_Channel = DMA_Channel_5;                    /* Channel selection */
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&USART6->DR; /* Set the DMA source: peripheral data register address */
  }

  DMA_InitStructure.DMA_Memory0BaseAddr = DMA_Memory0BaseAddr;            /* Memory address */
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;                 /* Data Peripheral to Memory */
  DMA_InitStructure.DMA_BufferSize = DMA_BufferSize;                      /* Specifies the DMA channel DMA buffer size */
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;        /* Peripheral address does not increase */
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                 /* Memory address increment */
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; /* Peripheral data width of 8 bit */
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;         /* Memory data width of 8 bit */
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;                         /* Use Circular mode */
  DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;                   /* Medium priority */
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;             /* A single transmission burst memory */
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;     /* Peripherals single transmission burst */

  if(USART_x == USART_1)
  {
    DMA_Init(DMA2_Stream2, &DMA_InitStructure);
    DMA_Cmd(DMA2_Stream2, ENABLE);
  }

  else if(USART_x == USART_2)
  {
    DMA_Init(DMA1_Stream5, &DMA_InitStructure);
    DMA_Cmd(DMA1_Stream5, ENABLE);
  }

  else if(USART_x == USART_3)
  {
    DMA_Init(DMA1_Stream1, &DMA_InitStructure);
    DMA_Cmd(DMA1_Stream1, ENABLE);
  }

  else if(USART_x == USART_4)
  {
    DMA_Init(DMA1_Stream2, &DMA_InitStructure);
    DMA_Cmd(DMA1_Stream2, ENABLE);
  }
  
  else if(USART_x == USART_5)
  {
    DMA_Init(DMA1_Stream0, &DMA_InitStructure);
    DMA_Cmd(DMA1_Stream0, ENABLE);
  }
  
  else if(USART_x == USART_6)
  {
    DMA_Init(DMA2_Stream1, &DMA_InitStructure);
    DMA_Cmd(DMA2_Stream1, ENABLE);
  }
}

/****************************************************************
 * Function:    USART_DMA_TX
 * Description: Start USART DMA Send.
 * Input:       USART_x
 *              DMA_BufferSize
 * Output:
 * Return:
*****************************************************************/
void USART_DMA_TX(uint8_t USART_x, uint16_t DMA_BufferSize)
{
  if(USART_x == USART_1)
  {
    DMA_ClearFlag(DMA2_Stream7, DMA_FLAG_TCIF7);  /* Clear Pending DMA channel x flag (DMA2 Stream7 transfer completion flag) */
    DMA_Cmd(DMA2_Stream7, DISABLE);
    while(DMA_GetCmdStatus(DMA2_Stream7) != DISABLE);
    DMA_SetCurrDataCounter(DMA2_Stream7, DMA_BufferSize);
    DMA_Cmd(DMA2_Stream7, ENABLE);
  }

  else if(USART_x == USART_2)
  {
    DMA_ClearFlag(DMA1_Stream6, DMA_FLAG_TCIF6);  /* Clear Pending DMA channel x flag (DMA1 DMA1_Stream6 transfer completion flag) */
    DMA_Cmd(DMA1_Stream6, DISABLE);
    while(DMA_GetCmdStatus(DMA1_Stream6) != DISABLE);
    DMA_SetCurrDataCounter(DMA1_Stream6, DMA_BufferSize);
    DMA_Cmd(DMA1_Stream6, ENABLE);
  }

  else if(USART_x == USART_3)
  {
    DMA_ClearFlag(DMA1_Stream3, DMA_FLAG_TCIF3);  /* Clear Pending DMA channel x flag (DMA1 DMA1_Stream3 transfer completion flag) */
    DMA_Cmd(DMA1_Stream3, DISABLE);
    while(DMA_GetCmdStatus(DMA1_Stream3) != DISABLE);
    DMA_SetCurrDataCounter(DMA1_Stream3, DMA_BufferSize);
    DMA_Cmd(DMA1_Stream3, ENABLE);
  }

  else if(USART_x == USART_4)
  {
    DMA_ClearFlag(DMA1_Stream4, DMA_FLAG_TCIF4);  /* Clear Pending DMA channel x flag (DMA1 DMA1_Stream4 transfer completion flag) */
    DMA_Cmd(DMA1_Stream4, DISABLE);
    while(DMA_GetCmdStatus(DMA1_Stream4) != DISABLE);
    DMA_SetCurrDataCounter(DMA1_Stream4, DMA_BufferSize);
    DMA_Cmd(DMA1_Stream4, ENABLE);
  }
  
  else if(USART_x == USART_5)
  {
    DMA_ClearFlag(DMA1_Stream7, DMA_FLAG_TCIF7);  /* Clear Pending DMA channel x flag (DMA1 DMA1_Stream7 transfer completion flag) */
    DMA_Cmd(DMA1_Stream7, DISABLE);
    while(DMA_GetCmdStatus(DMA1_Stream7) != DISABLE);
    DMA_SetCurrDataCounter(DMA1_Stream7, DMA_BufferSize);
    DMA_Cmd(DMA1_Stream7, ENABLE);
  }
  
  else if(USART_x == USART_6)
  {
    DMA_ClearFlag(DMA2_Stream6, DMA_FLAG_TCIF6);  /* Clear Pending DMA channel x flag (DMA2 DMA2_Stream6 transfer completion flag) */
    DMA_Cmd(DMA2_Stream6, DISABLE);
    while(DMA_GetCmdStatus(DMA2_Stream6) != DISABLE);
    DMA_SetCurrDataCounter(DMA2_Stream6, DMA_BufferSize);
    DMA_Cmd(DMA2_Stream6, ENABLE);
  }
}

/****************************************************************
 * Function:    USART_DMA_RX
 * Description: Start USART DMA Receive.
 * Input:       USART_x
 *              DMA_BufferSize
 * Output:
 * Return:
*****************************************************************/
void USART_DMA_RX(uint8_t USART_x, uint16_t DMA_BufferSize)
{
  if(USART_x == USART_1)
  {
    DMA_ClearFlag(DMA2_Stream2, DMA_FLAG_TCIF2);  /* Clear Pending DMA channel x flag (DMA2 DMA2_Stream2 transfer completion flag) */
    DMA_Cmd(DMA2_Stream2, DISABLE);
    while(DMA_GetCmdStatus(DMA2_Stream2) != DISABLE);
    DMA_SetCurrDataCounter(DMA2_Stream2, DMA_BufferSize);
    DMA_Cmd(DMA2_Stream2, ENABLE);
  }

  else if(USART_x == USART_2)
  {
    DMA_ClearFlag(DMA1_Stream5, DMA_FLAG_TCIF5);  /* Clear Pending DMA channel x flag (DMA1 DMA1_Stream5 transfer completion flag) */
    DMA_Cmd(DMA1_Stream5, DISABLE);
    while(DMA_GetCmdStatus(DMA1_Stream5) != DISABLE);
    DMA_SetCurrDataCounter(DMA1_Stream5, DMA_BufferSize);
    DMA_Cmd(DMA1_Stream5, ENABLE);
  }

  else if(USART_x == USART_3)
  {
    DMA_ClearFlag(DMA1_Stream1, DMA_FLAG_TCIF1);  /* Clear Pending DMA channel x flag (DMA1 DMA1_Stream1 transfer completion flag) */
    DMA_Cmd(DMA1_Stream1, DISABLE);
    while(DMA_GetCmdStatus(DMA1_Stream1) != DISABLE);
    DMA_SetCurrDataCounter(DMA1_Stream1, DMA_BufferSize);
    DMA_Cmd(DMA1_Stream1, ENABLE);
  }

  else if(USART_x == USART_4)
  {
    DMA_ClearFlag(DMA1_Stream2, DMA_FLAG_TCIF2);  /* Clear Pending DMA channel x flag (DMA1 DMA1_Stream2 transfer completion flag) */
    DMA_Cmd(DMA1_Stream2, DISABLE);
    while(DMA_GetCmdStatus(DMA1_Stream2) != DISABLE);
    DMA_SetCurrDataCounter(DMA1_Stream2, DMA_BufferSize);
    DMA_Cmd(DMA1_Stream2, ENABLE);
  }
  
  else if(USART_x == USART_5)
  {
    DMA_ClearFlag(DMA1_Stream0, DMA_FLAG_TCIF0);  /* Clear Pending DMA channel x flag (DMA1 DMA1_Stream0 transfer completion flag) */
    DMA_Cmd(DMA1_Stream0, DISABLE);
    while(DMA_GetCmdStatus(DMA1_Stream0) != DISABLE);
    DMA_SetCurrDataCounter(DMA1_Stream0, DMA_BufferSize);
    DMA_Cmd(DMA1_Stream0, ENABLE);
  }
  
  else if(USART_x == USART_6)
  {
    DMA_ClearFlag(DMA2_Stream1, DMA_FLAG_TCIF1);  /* Clear Pending DMA channel x flag (DMA2 DMA2_Stream1 transfer completion flag) */
    DMA_Cmd(DMA2_Stream1, DISABLE);
    while(DMA_GetCmdStatus(DMA2_Stream1) != DISABLE);
    DMA_SetCurrDataCounter(DMA2_Stream1, DMA_BufferSize);
    DMA_Cmd(DMA2_Stream1, ENABLE);
  }
}

/****************************************************************
 * Function:    USART_OUT
 * Description: USART Send.
 * Input:       USART_x
                Buffer
                BufferSize
 * Output:
 * Return:
*****************************************************************/
void USART_OUT(uint8_t USART_x, uint8_t* Buffer, uint16_t BufferSize)
{
  if(USART_x == USART_1)
  {
    while(DMA_GetFlagStatus(DMA2_Stream7, DMA_FLAG_TCIF7)==0);

    memcpy(USART1_Buffer_Tx, Buffer, BufferSize); /* Copy memory */
  }
  
  else if(USART_x == USART_2)
  {
    while(DMA_GetFlagStatus(DMA1_Stream6, DMA_FLAG_TCIF6)==0);

    memcpy(USART2_Buffer_Tx, Buffer, BufferSize); /* Copy memory */
  }
  
  else if(USART_x == USART_3)
  {
    while(DMA_GetFlagStatus(DMA1_Stream3, DMA_FLAG_TCIF3)==0);

    memcpy(USART3_Buffer_Tx, Buffer, BufferSize); /* Copy memory */
  }
  
  else if(USART_x == USART_4)
  {
    while(DMA_GetFlagStatus(DMA1_Stream4, DMA_FLAG_TCIF4)==0);

    memcpy(USART4_Buffer_Tx, Buffer, BufferSize); /* Copy memory */
  }
  
  else if(USART_x == USART_5)
  {
    while(DMA_GetFlagStatus(DMA1_Stream7, DMA_FLAG_TCIF7)==0);

    memcpy(USART5_Buffer_Tx, Buffer, BufferSize); /* Copy memory */
  }
  
  else if(USART_x == USART_6)
  {
    while(DMA_GetFlagStatus(DMA2_Stream6, DMA_FLAG_TCIF6)==0);

    memcpy(USART6_Buffer_Tx, Buffer, BufferSize); /* Copy memory */
  }
  
  USART_DMA_TX(USART_x, BufferSize);
}

/****************************************************************
 * Function:    USART_IN
 * Description: USART Receive.
 * Input:       USART_x
 * Output:
 * Return:
*****************************************************************/
void USART_IN(uint8_t USART_x)
{
  if(USART_x == USART_1)
  {
    memset(USART1_Buffer_Rx, 0, USART1_BufferSize_Rx);  /* Empty memory */
    
    USART_DMA_RX(USART_1, USART1_BufferSize_Rx);
  }
  
  else if(USART_x == USART_2)
  {
    memset(USART2_Buffer_Rx, 0, USART2_BufferSize_Rx);  /* Empty memory */
    
    USART_DMA_RX(USART_2, USART2_BufferSize_Rx);
  }
  
  else if(USART_x == USART_3)
  {
    memset(USART3_Buffer_Rx, 0, USART3_BufferSize_Rx);  /* Empty memory */
    
    USART_DMA_RX(USART_3, USART3_BufferSize_Rx);
  }
  
  else if(USART_x == USART_4)
  {
    memset(USART4_Buffer_Rx, 0, USART4_BufferSize_Rx);  /* Empty memory */
    
    USART_DMA_RX(USART_4, USART4_BufferSize_Rx);
  }
  
  else if(USART_x == USART_5)
  {
    memset(USART5_Buffer_Rx, 0, USART5_BufferSize_Rx);  /* Empty memory */
    
    USART_DMA_RX(USART_5, USART5_BufferSize_Rx);
  }
  
  else if(USART_x == USART_6)
  {
    memset(USART6_Buffer_Rx, 0, USART6_BufferSize_Rx);  /* Empty memory */
    
    USART_DMA_RX(USART_6, USART6_BufferSize_Rx);
  }
}

/****************************************************************
 * Function:    USART_GetReceiveDataNumber
 * Description: USART Get Receive Data Number.
 * Input:       USART_x
 * Output:
 * Return:      Receive Data Number
*****************************************************************/
uint16_t USART_GetReceiveDataNumber(uint8_t USART_x)
{
  uint16_t number = 0;
  
  if(USART_x == USART_1)
  {
    number = USART1_BufferSize_Rx - DMA_GetCurrDataCounter(DMA2_Stream2);
  }
  else if(USART_x == USART_2)
  {
    number = USART2_BufferSize_Rx - DMA_GetCurrDataCounter(DMA1_Stream5);
  }
  else if(USART_x == USART_3)
  {
    number = USART3_BufferSize_Rx - DMA_GetCurrDataCounter(DMA1_Stream1);
  }
  else if(USART_x == USART_4)
  {
    number = USART4_BufferSize_Rx - DMA_GetCurrDataCounter(DMA1_Stream2);
  }
  else if(USART_x == USART_5)
  {
    number = USART5_BufferSize_Rx - DMA_GetCurrDataCounter(DMA1_Stream0);
  }
  else if(USART_x == USART_6)
  {
    number = USART6_BufferSize_Rx - DMA_GetCurrDataCounter(DMA2_Stream1);
  }
  
  return number;
}

#include <stdarg.h>
#include <stdio.h>
void u1_printf(char* fmt,...)  
{  
  static char buff[USART1_BufferSize_Tx];
  
    memset(buff,0,USART1_BufferSize_Tx);
    va_list ap;
    va_start(ap,fmt);
    vsprintf((char*)buff,fmt,ap);
    va_end(ap);
  
    USART_OUT(USART_1, (uint8_t*)buff, strlen(buff));
}
