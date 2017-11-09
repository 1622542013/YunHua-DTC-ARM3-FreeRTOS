/*============================================================================*/
/* Copyright (C), 2016, Yagro Co., Ltd.                                       */
/* File name:   globals.h                                                      */
/* Date:        2014.4.29                                                     */
/* Description: globals.h                                                     */
/*============================================================================*/

#ifndef  __UMI_USER_DEFINE_H
#define  __UMI_USER_DEFINE_H

/*============================================================================*/
/*                               Header include                               */
/*============================================================================*/

#include "stm32f4xx.h"
#include "definition.h"
 
/*============================================================================*/
/*                                   Macros                                   */
/*============================================================================*/

#define  DATA_FROM_ARM2_HEAD_1  ((TpUchar)0xCD)
#define  DATA_FROM_ARM2_HEAD_2  ((TpUchar)0x34)

#define  MAG_BIN_HEAD           ((TpUint16))

#define  DATA_FROM_ARM2_HEAD    ((TpUint16)0x34CD)

#define F32_MACHEPS             (2.22045e-16)

#define RESULT_OK               				(1)	
#define RESULT_ERROR            				(0)													 
#define RESULT_PARA_NOT_VALID           (-1)
#define RESULT_OUTPUT_RATE_TOOHIGH      (-2)
#define NO_ANSWER               				(-3)
#define RESULT_COMMAND_INVALID          (-4)

#define NMEA_USE                (1)
#define GPS_UTC                 (17000)
#define Cycle_100MS             (10)
#define Cycle_200MS             (20)




#define NMEA_ON

extern uint32_t ulHighFrequencyTimerTicks;

/*============================================================================*/
/*                             struct define                                  */
/*============================================================================*/

#pragma pack(push)
#pragma pack(1)

typedef struct
{
	TpChar  buf[512];
	TpUint16 num;
	TpUchar  start;
	TpUint16 pt ;
  TpUchar  flag;
}Package;

#pragma pack(pop)

/*============================================================================*/
/*                               declaration                             */
/*============================================================================*/


#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/*******************************************************/

	
TpUchar CheckSumByte(TpUchar* Buffer, TpUint16 Length);
short int CheckSum(unsigned char *pinf, int len);
void Delay_500ns(void);
TpBool My_atoi(const char* buff,int32_t* num);
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif

