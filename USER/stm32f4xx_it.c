/**
  ******************************************************************************
  * @file    GPIO/GPIO_IOToggle/stm32f4xx_it.c 
  * @author  MCD Application Team
  * @version V1.6.0
  * @date    04-September-2015
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2015 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_it.h"
#include <string.h>


#include "usart1.h"
#include "usart2.h"
#include "usart3.h"
#include "usart4.h"
#include "usart5.h"
#include "usart6.h"
#include "usr_FreeRTOS.h"
#include "math.h"

/** @addtogroup STM32F4xx_StdPeriph_Examples
  * @{
  */

/** @addtogroup GPIO_IOToggle
  * @{
  */

/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief   This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
	  
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
//void SVC_Handler(void)
//{
//}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
//void PendSV_Handler(void)
//{
//}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
//void SysTick_Handler(void)
//{
//}

void EXTI1_IRQHandler(void)
{
  if(EXTI_GetITStatus(EXTI_Line1) != RESET)
  {
    EXTI_ClearITPendingBit(EXTI_Line1);
  }
}

void DMA2_Stream0_IRQHandler(void)
{
  
	if(DMA_GetITStatus(DMA2_Stream0, DMA_IT_TCIF0) != RESET)
	{				    
    DMA_ClearITPendingBit(DMA2_Stream0, DMA_IT_TCIF0);
   
	}
}

void DMA1_Stream3_IRQHandler(void)
{
	if(DMA_GetITStatus(DMA1_Stream3, DMA_IT_TCIF0) != RESET)
	{				    
		DMA_ClearITPendingBit(DMA1_Stream3, DMA_IT_TCIF0);

	}
}

void DMA1_Stream0_IRQHandler(void)
{
 
	if(DMA_GetITStatus(DMA1_Stream0, DMA_IT_TCIF0) != RESET)
	{				    
		DMA_ClearITPendingBit(DMA1_Stream0, DMA_IT_TCIF0);
  }
}

/**
  * @brief  This function handles TIM2 Handler.
  * @param  None
  * @retval None
  */
void TIM3_IRQHandler(void)
{
  if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
  {  
    TIM_ClearITPendingBit(TIM3, TIM_IT_Update); 
  }
}

extern uint8_t USART1_Rx_Buffer[USART1_Rx_BufferSize];
extern uint8_t USART2_Rx_Buffer[USART2_Rx_BufferSize];
extern uint8_t USART3_Rx_Buffer[USART3_Rx_BufferSize];
extern uint8_t UART4_Rx_Buffer[UART4_Rx_BufferSize];
extern uint8_t UART5_Rx_Buffer[UART4_Rx_BufferSize];
extern uint8_t USART6_Rx_Buffer[USART6_Rx_BufferSize];

extern SemaphoreHandle_t usart1_Semaphore_bin;
extern SemaphoreHandle_t usart2_Semaphore_bin;


void USART1_IRQHandler(void)
{ 
	if(USART_GetITStatus(USART1, USART_IT_IDLE) != RESET)
	{  
    USART1->SR;
    USART1->DR;	
      
    USART1_Send_bin(USART1_Rx_Buffer,USART1_GetReceiveDataNumber());     
    
    USART1_Reset_RX();
	}
}

void USART2_IRQHandler(void)
{ 
  BaseType_t HighPriorityTaskToken = pdTRUE;
  
	if(USART_GetITStatus(USART2, USART_IT_IDLE) != RESET)
	{  
    USART2->SR;
    USART2->DR;	
      
    USART2_Send_bin(USART2_Rx_Buffer,USART2_GetReceiveDataNumber());

    USART2_Reset_RX();    
	}
  
  if(USART_GetITStatus(USART2, USART_IT_TC) != RESET)
	{  
    USART_ClearITPendingBit(USART2,USART_IT_TC);  
    
    xSemaphoreGiveFromISR(usart2_Semaphore_bin,&HighPriorityTaskToken);
    
    
	}
}

void USART3_IRQHandler(void)
{ 
	if(USART_GetITStatus(USART3, USART_IT_IDLE) != RESET)
	{  
    USART3->SR;
    USART3->DR;	
      
    USART3_Send_data(USART3_Rx_Buffer,USART3_GetReceiveDataNumber());     
    
    USART3_Reset_RX();
	}
}

void UART4_IRQHandler(void)
{ 
	if(USART_GetITStatus(UART4, USART_IT_IDLE) != RESET)
	{  
    UART4->SR;
    UART4->DR;	
      
    UART4_Send_data(UART4_Rx_Buffer,UART4_GetReceiveDataNumber());     
    
    UART4_Reset_RX();
	}
}

void UART5_IRQHandler(void)
{ 
	if(USART_GetITStatus(UART5, USART_IT_IDLE) != RESET)
	{  
    UART5->SR;
    UART5->DR;	
      
    UART5_Send_data(UART5_Rx_Buffer,UART5_GetReceiveDataNumber());     
    
    UART5_Reset_RX();
	}
}

void USART6_IRQHandler(void)
{ 
	if(USART_GetITStatus(USART6, USART_IT_IDLE) != RESET)
	{  
    USART6->SR;
    USART6->DR;	
      
    USART6_Send_data(USART6_Rx_Buffer,USART6_GetReceiveDataNumber());     
    
    USART6_Reset_RX();
	}
}


/******************************************************************************/
/*                 STM32F4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f40xx.s/startup_stm32f427x.s/startup_stm32f429x.s).    */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 

/**
  * @}
  */ 


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
