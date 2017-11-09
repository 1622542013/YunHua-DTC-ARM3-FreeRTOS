/****************************************************************
 * Copyright (C) 2016, Yagro, all right reserved.
 * File name:    Delay.c
 * Date:         2014.04.24
 * Description:  Delay Driver
*****************************************************************/

/*============================================================================*/
/*                               Header include                               */
/*============================================================================*/

#include "delay.h"

/*============================================================================*/
/*                                   Macros                                   */
/*============================================================================*/

#define  FACTOR_SYSTICK_MS     ((TpUint32)8400)
#define  FACTOR_MS_TO_US       ((TpUint32)1000)

 
static uint8_t  fac_us;  /* Microsecond delay times multiplier */
static uint16_t fac_ms;  /* Millisecond delay times multiplier */


/****************************************************
 * Function:    Delay_Init
 * Description: Delay Configuration.
 * Input:
 * Output:
 * Return:
*****************************************************/
TpBool SystickInit(TpVoid)
{
  TpBool resutlt = INLIB_ERROR;
  
  SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
  SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;  /* Disability SysTick counter */
  fac_ms = SystemCoreClock/FACTOR_SYSTICK_MS;
  fac_us = SystemCoreClock/FACTOR_SYSTICK_MS/FACTOR_MS_TO_US;
  resutlt = INLIB_OK;
  return resutlt;
}
/****************************************************
 * Function:    Delay_ms
 * Description: Millisecond latency.
 * Input:       nms
 * Output:
 * Return:
*****************************************************/
TpBool Delay_ms(u16 nms)
{
  TpBool resutlt = INLIB_ERROR;
  TpUint32 temp;

  SysTick->LOAD = (uint32_t)nms * fac_ms;   /* Time load (SysTick-> LOAD is 24bit) */
  SysTick->VAL = 0x00;                      /* Empty counter */
  SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk; /* Start the countdown */

  do
  {
    temp = SysTick->CTRL;
  }
  while(temp&0x01 && !(temp&(1<<16)));        /* Wait time is reached */

  SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;  /* Close Counter */
  SysTick->VAL = 0x00;                        /* Empty counter */
  
  resutlt = INLIB_OK;
  return resutlt;
}

/****************************************************
 * Function:    Delay_us
 * Description: Microsecond latency.
 * Input:		    nus
 * Output:
 * Return:
*****************************************************/
TpVoid Delay_us(TpUint32 nus)
{
  uint32_t temp;

  SysTick->LOAD = nus * fac_us;             /* Time load (SysTick-> LOAD is 24bit) */
  SysTick->VAL = 0x00;                      /* Empty counter */
  SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk; /* Start the countdown */

  do
  {
    temp = SysTick->CTRL;
  }
  while(temp&0x01 && !(temp&(1<<16)));        /* Wait time is reached */

  SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;  /* Close Counter */
  SysTick->VAL = 0x00;                        /* Empty counter */
}

/****************************************************
 * Function:    os_time_init
 * Description: For measuring the run time.
 * Input:
 * Output:
 * Return:
*****************************************************/
void os_time_init(void)
{
  SysTick->LOAD = 0xffffff;                 /* Time load (SysTick-> LOAD is 24bit) */
  SysTick->VAL = 0x00;                      /* Empty counter */
  SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk; /* Start the countdown */
}

/****************************************************
 * Function:    os_time_init
 * Description: For measuring the run time.
 * Input:
 * Output:
 * Return:		  Microsecond
*****************************************************/
float os_time(void)
{
  SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;  /* Close Counter */
  return (0xffffff - SysTick->VAL) / 10.5f;    /* Read counter */
}
