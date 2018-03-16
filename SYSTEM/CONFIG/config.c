/**
 * @file        config.c
 * @brief       初始化文件
 */

/*============================================================================*/
/*                               Header include                               */
/*============================================================================*/

#include "config.h"
#include "Sysclock.h"
#include "delay.h"

#include "usart1.h"
#include "usart2.h"
#include "usart3.h"
#include "usart4.h"
#include "usart5.h"
#include "usart6.h"

/*============================================================================*/
/*                              Global variables                              */
/*============================================================================*/

/*============================================================================*/
/*                            Function definition                             */
/*============================================================================*/
void USART_Init_Usr()
{
//   USART1_Init(9600,USART_IT_IDLE,ENABLE);
   USART2_Init(230400,USART_IT_IDLE | USART_IT_TC,ENABLE);
//   USART3_Init(9600,USART_IT_IDLE | USART_IT_TC,ENABLE);
//   UART4_Init(9600,USART_IT_IDLE,ENABLE);
   UART5_Init(115200,USART_IT_IDLE | USART_IT_TC,ENABLE);
//   USART6_Init(9600,USART_IT_IDLE,ENABLE);
}

TpVoid NVIC_Config(TpVoid)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	 
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 8;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
  
  NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 8;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

}


/**
 * @brief 定时器初始化
 * @note  TIM2 ： 20KHZ    以供FreeRTOS系统使用
 *
 * @retval 成功返回值： 无
 * @retval 失败返回值： 无
 * @pre    调用举例 ： 
 */ 

void TIM_Configuration(void)
{
  TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);           /* Enable TIM2 clock */

  TIM_TimeBaseInitStructure.TIM_Prescaler = 1;                   /* Specifies the prescaler value used to divide the TIM clock. */
  TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;/* Specifies the counter mode */
  TIM_TimeBaseInitStructure.TIM_Period = 18260;                  /* Reload value	approximate 20KHz */
  TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);
  TIM_ClearFlag(TIM3, TIM_FLAG_Update);
  TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
  TIM_Cmd(TIM3, ENABLE);
}

void HardWareInit(TpVoid)
{
  USART_Init_Usr();
  NVIC_Config();
}


