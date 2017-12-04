/****************************************************************
 * Copyright (C) 2016, Yagro, all right reserved.
 * File name:    Delay.c
 * Date:         2014.04.24
 * Description:  Delay Driver
*****************************************************************/


#include "delay.h"

#define Delay_TIM   TIM2  


void os_time_init(void)
{
  SysTick->LOAD = 0xffffff;                 /* Time load (SysTick-> LOAD is 24bit) */
  SysTick->VAL = 0x00;                      /* Empty counter */
  SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk; /* Start the countdown */
}

/****************************************************************
 * Function:    os_time_init
 * Description: For measuring the run time.
 * Input:
 * Output:
 * Return:		  Microsecond
*****************************************************************/
float os_time(void)
{
  SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;  /* Close Counter */
  return (0xffffff - SysTick->VAL) / 10.5f;   /* Read counter */
}

void DelayInit(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);  /* Enable TIM2 clock */

  TIM_TimeBaseInitStructure.TIM_Prescaler = 4199;                    /* Specifies the prescaler value used to divide the TIM clock.(2 Division) */
  TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; /* Specifies the counter mode */
  TIM_TimeBaseInitStructure.TIM_Period = 65535;                   /* Reload value	approximate 2.3KHz */
  TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseInit(Delay_TIM, &TIM_TimeBaseInitStructure);

  TIM_ClearFlag(Delay_TIM, TIM_FLAG_Update);

  TIM_Cmd(Delay_TIM, DISABLE);
}

void Delay_ms(uint32_t ms)
{
  uint16_t counter;
  uint32_t tmp_ms = 0;
  uint32_t tmp_5s_ms = 0;

  TIM_SetCounter(Delay_TIM,0);
  TIM_Cmd(Delay_TIM, ENABLE);
  
  do
  {
    counter = TIM_GetCounter(Delay_TIM);
    
    if(counter > 50000)
    {
      TIM_SetCounter(Delay_TIM,0);
      tmp_5s_ms += 5000;
    }
    
    tmp_ms = tmp_5s_ms + counter / 10;
  }
  while(tmp_ms < ms);
   
   TIM_Cmd(Delay_TIM, DISABLE);
}
