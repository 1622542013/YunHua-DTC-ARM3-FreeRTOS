/****************************************************************
 * Copyright (C) 2017, Yagro, all right reserved.
 * File name:    LED.h
 * Date:         2017.08.28
 * Description:  LED driver.
*****************************************************************/

#ifndef __LED_H
#define __LED_H

/****************************************************************
 *                        Header include
*****************************************************************/
#include "stm32f4xx.h"

/****************************************************************
 *                       Macro definition
*****************************************************************/

/******************** Interface configuration *******************/

#define LEDn                        (3)

#define LED1_RCC_AHB1Periph_GPIO    RCC_AHB1Periph_GPIOB
#define LED1_GPIO                   GPIOB
#define LED1_GPIO_Pin               GPIO_Pin_0

#define LED2_RCC_AHB1Periph_GPIO    RCC_AHB1Periph_GPIOB
#define LED2_GPIO                   GPIOB
#define LED2_GPIO_Pin               GPIO_Pin_1

#define LED3_RCC_AHB1Periph_GPIO    RCC_AHB1Periph_GPIOA
#define LED3_GPIO                   GPIOA
#define LED3_GPIO_Pin               GPIO_Pin_8

/****************************************************************/

/****************************************************************
 *                       Type definition
*****************************************************************/
typedef enum
{
  LEDPin1 = 0,
  LEDPin2 = 1,
  LEDPin3 = 2
  
}LEDPin;

typedef enum
{
  LEDStateOff = 0,
  LEDStateOn  = 1
  
}LEDState;

/****************************************************************
 *                     Structure definition
*****************************************************************/



#ifdef __cplusplus
 extern "C" {
#endif  /* __cplusplus */

/****************************************************************
 *                     Variable declaration
*****************************************************************/


/****************************************************************
 *                     Function declaration
*****************************************************************/
void LED_SetState(LEDPin pin, LEDState state);
LEDState LED_GetState(LEDPin pin);

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif	/* __LED_H */
