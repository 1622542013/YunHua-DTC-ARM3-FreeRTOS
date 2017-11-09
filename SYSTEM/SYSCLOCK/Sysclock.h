/****************************************************************
 * Copyright (C) 2016, Yagro, all right reserved.
 * File name:    Delay.h
 * Date:         2016.03.22
 * Description:  Delay Driver
*****************************************************************/

#ifndef __SYSCLOCK_H
#define __SYSCLOCK_H

/****************************************************************
 *                        Header include
*****************************************************************/
#include "stm32f4xx.h"

/****************************************************************
 *                       Macro definition
*****************************************************************/
#define Osc_Status_HSI  0x00
#define Osc_Status_HSE  0x04
#define Osc_Status_PLL  0x08
#define Osc_Status_PLLR 0x0c

#define HSI             1
#define HSE             2
/****************************************************************
 *                       Type definition
*****************************************************************/


/****************************************************************
 *                     Structure definition
*****************************************************************/

#pragma pack(push)
#pragma pack(1)


#pragma pack(pop)


#ifdef __cplusplus
 extern "C" {
#endif  /* __cplusplus */

/****************************************************************
 *                     Variable declaration
*****************************************************************/



   

/****************************************************************
 *                     Function declaration
*****************************************************************/
void SysClockInit(void);
uint8_t GetOscStatus(void);

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif	/* __DELAY_H */
