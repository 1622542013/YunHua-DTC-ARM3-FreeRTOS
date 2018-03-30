/*============================================================================*/
/* Copyright (C), 2018, Yagro Co., Ltd.                                       */
/* File name:   event_mark.h                                                  */
/* Date:        2018.2.11                                                     */
/* Description:                                                               */
/*============================================================================*/

#ifndef __PROJECT_EVENT_MARK_H__
#define __PROJECT_EVENT_MARK_H__

/*============================================================================*/
/*                               Header include                               */
/*============================================================================*/
#include <stdint.h>

/*============================================================================*/
/*                              Macro definition                              */
/*============================================================================*/


/*============================================================================*/
/*                            Structure definition                            */
/*============================================================================*/

#pragma pack(push)
#pragma pack(1)

// 其他需要存储到SD卡中的数据
typedef struct _tagAddSaveDataSD
{
	float    pitch_headinga; /* pitch from headinga */
	uint16_t event_mark_cnt;
	uint16_t event_mark_save_value;
}AddSaveDataSD;

typedef struct _tagKeyScan
{
	uint32_t scan_up_cnt;
	uint32_t scan_down_cnt;
	uint8_t  key_press;
}KeyScan;


#pragma pack(pop)

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

void EventMarkInit(void);  
  
void KeyStateChange(void);
void KeyScanUpDown(void);	
  
uint8_t GetKeyState(void);
void GetEventMarkValue(void);
uint32_t GetEventMark(void);
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
