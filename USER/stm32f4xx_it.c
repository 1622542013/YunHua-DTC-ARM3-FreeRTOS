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
#include "usart_FreeRTOS.h"
#include "math.h"
#include "event_mark.h"

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

void EXTI9_5_IRQHandler(void)
{
  if(EXTI_GetITStatus(EXTI_Line5) != RESET)
  {
    EXTI_ClearITPendingBit(EXTI_Line5); 
	
    KeyStateChange();
		
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
extern uint32_t ulHighFrequencyTimerTicks;
void TIM3_IRQHandler(void)
{
  if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
  {  
    ulHighFrequencyTimerTicks ++;
    TIM_ClearITPendingBit(TIM3, TIM_IT_Update); 
  }
}

void USART1_IRQHandler(void)
{ 
	if(USART_GetITStatus(USART1, USART_IT_IDLE) != RESET)
	{  
    USART1->SR;
    USART1->DR;	
    
    USART1_Reset_RX();
	}
}

/*-----------------receive user command--------------------*/
void USART2_IRQHandler(void)
{ 
  BaseType_t HighPriorityTaskToken = pdTRUE;
  
	if(USART_GetITStatus(USART2, USART_IT_IDLE) != RESET)
	{  
    USART2->SR;
    USART2->DR;	

    usart2_queue_struct  usr_buff;

    usr_buff.rx_num = USART2_GetReceiveDataNumber(); /*received data number*/
    memcpy(usr_buff.rx_buff,USART2_Rx_Buffer,usr_buff.rx_num); /*push data into queue*/

    xQueueSendFromISR(usart2_queue,&usr_buff,&HighPriorityTaskToken);/*send the queue*/

    portYIELD_FROM_ISR(HighPriorityTaskToken); /*context switch immediately*/

    USART2_Reset_RX();    
	}
  
  if(USART_GetITStatus(USART2, USART_IT_TC) != RESET)
	{  
    USART_ClearITPendingBit(USART2,USART_IT_TC);  
    
    xSemaphoreGiveFromISR(usart2_Semaphore_bin,&HighPriorityTaskToken);   
    portYIELD_FROM_ISR(HighPriorityTaskToken);  
	}
}

/*-------------------------rev nav data----------------------*/
void USART3_IRQHandler(void)
{  
  BaseType_t HighPriorityTaskToken = pdTRUE;
  
	if(USART_GetITStatus(USART3, USART_IT_IDLE) != RESET)
	{  
    USART3->SR;
    USART3->DR;	
    
    usart3_queue_struct nav_buff;

    nav_buff.rx_num = USART3_GetReceiveDataNumber();/*received data number*/
    memcpy(nav_buff.rx_buff,USART3_Rx_Buffer,nav_buff.rx_num);/*push data into queue*/

    xQueueSendFromISR(usart3_queue,&nav_buff,&HighPriorityTaskToken);/*send the queue*/

    portYIELD_FROM_ISR(HighPriorityTaskToken); /*context switch immediately*/

    USART3_Reset_RX();    
	}
  
  if(USART_GetITStatus(USART3, USART_IT_TC) != RESET)
	{  
    USART_ClearFlag(USART3, USART_FLAG_TC);
    
    xSemaphoreGiveFromISR(usart3_Semaphore_bin,&HighPriorityTaskToken);
    portYIELD_FROM_ISR(HighPriorityTaskToken); 
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

/*-------------------------rev gnss data----------------------*/
void UART5_IRQHandler(void)
{   
  BaseType_t HighPriorityTaskToken = pdTRUE;
  
	if(USART_GetITStatus(UART5, USART_IT_IDLE) != RESET)
	{  
    UART5->SR;
    UART5->DR;
    
    uart5_queue_struct gnss_buff;   
    
    gnss_buff.rx_num = UART5_GetReceiveDataNumber();
    memcpy(gnss_buff.rx_buff,UART5_Rx_Buffer,gnss_buff.rx_num);
     
    xQueueSendFromISR(uart5_queue,&gnss_buff,&HighPriorityTaskToken);
         
    portYIELD_FROM_ISR(HighPriorityTaskToken); /*context switch immediately*/

    UART5_Reset_RX();
	}
  
  if(USART_GetITStatus(UART5, USART_IT_TC) != RESET)
	{  
    USART_ClearITPendingBit(UART5,USART_IT_TC);  
       
    xSemaphoreGiveFromISR(uart5_Semaphore_bin,&HighPriorityTaskToken);
    portYIELD_FROM_ISR(HighPriorityTaskToken);  
	}
}

void USART6_IRQHandler(void)
{ 
  BaseType_t HighPriorityTaskToken = pdTRUE;
  
	if(USART_GetITStatus(USART6, USART_IT_IDLE) != RESET)
	{  
    USART6->SR;
    USART6->DR;	
 
    if(strstr((char*)USART6_Rx_Buffer, "$cmd,Config,OK*ff") != 0)
    {
      xEventGroupSetBitsFromISR(Event_group,SD_TIME_OK_BIT,&HighPriorityTaskToken);
      portYIELD_FROM_ISR(HighPriorityTaskToken);
    }

    USART6_Reset_RX();
	}

  if(USART_GetITStatus(USART6, USART_IT_TC) != RESET)
	{  
    USART_ClearITPendingBit(USART6,USART_IT_TC);  
    
    xSemaphoreGiveFromISR(usart6_Semaphore_bin,&HighPriorityTaskToken);
    portYIELD_FROM_ISR(HighPriorityTaskToken);    
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
