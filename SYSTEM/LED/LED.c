/****************************************************************
 * Copyright (C) 2017, Yagro, all right reserved.
 * File name:    LED.c
 * Date:         2017.08.28
 * Description:  LED driver.
*****************************************************************/

/****************************************************************
 *                        Header include
*****************************************************************/
#include "LED.h"
#include <stdbool.h>

/****************************************************************
 *                       Global variables
*****************************************************************/
static GPIO_TypeDef* LED_GPIO[LEDn]                = {LED1_GPIO,                LED2_GPIO,                LED3_GPIO};
static uint16_t      LED_GPIO_Pin[LEDn]            = {LED1_GPIO_Pin,            LED2_GPIO_Pin,            LED3_GPIO_Pin};
static uint32_t      LED_RCC_AHB1Periph_GPIO[LEDn] = {LED1_RCC_AHB1Periph_GPIO, LED2_RCC_AHB1Periph_GPIO, LED3_RCC_AHB1Periph_GPIO};
static LEDState      ledState[LEDn]                = {LEDStateOff,              LEDStateOff,              LEDStateOff};

/****************************************************************
 *                     Function declaration
*****************************************************************/
static void LED_Init(void);

/****************************************************************
 *                     Function definition
*****************************************************************/

/****************************************************************
 * Function:    LED_Init
 * Description: LED initialization.
 * Input:
 * Output:
 * Return:
*****************************************************************/
static void LED_Init(void)
{
  static bool init_flag = false;
  
  if(init_flag == false)
  {
    init_flag = true;
    
    for(int i = 0; i < LEDn; i++)
    {
      GPIO_InitTypeDef GPIO_InitStructure;
      
      RCC_AHB1PeriphClockCmd(LED_RCC_AHB1Periph_GPIO[i], ENABLE);
      
      GPIO_InitStructure.GPIO_Pin = LED_GPIO_Pin[i];
      GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
      GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
      GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
      GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
      GPIO_Init(LED_GPIO[i], &GPIO_InitStructure);
      
      GPIO_WriteBit(LED_GPIO[i], LED_GPIO_Pin[i], (BitAction)LEDStateOff);
    }
  }
}

/****************************************************************
 * Function:    LED_SetState
 * Description: Set LED state.
 * Input:       pin:   LED pin.
 *              state: LED state.
 * Output:
 * Return:
*****************************************************************/
void LED_SetState(LEDPin pin, LEDState state)
{
  if(state != ledState[pin])
  {
    ledState[pin] = state;
    
    LED_Init();
    
    GPIO_WriteBit(LED_GPIO[pin], LED_GPIO_Pin[pin], (BitAction)state);
  }
}

/****************************************************************
 * Function:    LED_GetState
 * Description: Get LED state.
 * Input:       pin: LED pin.
 * Output:
 * Return:      LED state.
*****************************************************************/
LEDState LED_GetState(LEDPin pin)
{
  return ledState[pin];
}
