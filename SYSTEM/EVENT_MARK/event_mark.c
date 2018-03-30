/*============================================================================*/
/* Copyright (C), 2018, Yagro Co., Ltd.                                       */
/* File name:   event_mark.c                                                  */
/* Date:        2018.2.11                                                     */
/* Description:                                                               */
/*============================================================================*/

/*============================================================================*/
/*                               Header include                               */
/*============================================================================*/

#include "event_mark.h"
#include <stm32f4xx.h>

/*============================================================================*/
/*                              Global variables                              */
/*============================================================================*/

AddSaveDataSD   stAddSaveDataSD;
KeyScan         stKeyScan;

/*============================================================================*/
/*                             Function declaration                           */
/*============================================================================*/

void EventMarkAdd(void);
void EventMarkClear(void);

void SetEventMarkValue(void);
void GetEventMarkValue(void);

void KeyStateClear(void);
uint8_t GetKeyState(void);



/*============================================================================*/
/*                             Function definition                            */
/*============================================================================*/

void EventMarkInit(void)
{
		EXTI_InitTypeDef EXTI_InitStructure;
	  GPIO_InitTypeDef GPIO_InitStructure;
	  // 如果使能外部中断0,进行配置
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);	  

		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;;
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
		GPIO_Init(GPIOB, &GPIO_InitStructure);	
		
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

		SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource5);
	
	  //  GPIO_ResetBits(GPIOB,GPIO_Pin_5);

		EXTI_ClearITPendingBit(EXTI_Line5);		

		EXTI_InitStructure.EXTI_Line = EXTI_Line5;
		EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;		
		EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;//EXTI_Trigger_Falling;	
		EXTI_InitStructure.EXTI_LineCmd = ENABLE;				
		EXTI_Init(&EXTI_InitStructure);	
}



/****************************************************************
 * 函数名称: EventMarkAdd
 * 函数功能: EventMark值累加
 * 函数输入: 无
 * 函数输出: 无,函数体内部改变相应变量的数值
 * 函数返回值:无
*****************************************************************/
void EventMarkAdd(void)
{
   stAddSaveDataSD.event_mark_cnt++;
}

/****************************************************************
 * 函数名称: EventMarkAdd
 * 函数功能: EventMark值清零
 * 函数输入: 无
 * 函数输出: 无,函数体内部改变相应变量的数值
 * 函数返回值:无
*****************************************************************/
void EventMarkClear(void)
{
   stAddSaveDataSD.event_mark_cnt = 0;
}

/****************************************************************
 * 函数名称: KeyScanUpDown
 * 函数功能: 按键防抖,根据按键按下/松开状态,置相应的EventMark数值
 * 函数输入: 无
 * 函数输出: 无,函数体内部改变相应变量的数值
 * 函数返回值:无
*****************************************************************/

void KeyScanUpDown(void)
{
	uint8_t state = 0;
	state = GetKeyState();
	if(state)
	{
	    if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_5) != RESET)
			{
				stKeyScan.scan_up_cnt++;
				stKeyScan.scan_down_cnt = 0;
			
			}
			else
			{
				stKeyScan.scan_up_cnt = 0;		
				stKeyScan.scan_down_cnt++;
			}
			
			// 如果是低电平,代表按键按下,EventMark数值为按键按下的次数
			if(stKeyScan.scan_down_cnt==100)
      {
				stKeyScan.scan_down_cnt = 0;
				KeyStateClear();
				GetEventMarkValue();
			  EventMarkAdd();	
        SetEventMarkValue();				
			}
			else if(stKeyScan.scan_up_cnt==100)// 如果是高电平,代表按键松开,EventMark数值为0
			{ 
				stKeyScan.scan_up_cnt = 0;
			  KeyStateClear();
				EventMarkClear();				
			}
		}			
}

/****************************************************************
 * 函数名称: KeyStateChange
 * 函数功能: 置按键按下标志
 * 函数输入: 无
 * 函数输出: 无,函数体内部改变相应变量的数值
 * 函数返回值:无
*****************************************************************/
void KeyStateChange(void)
{
	stKeyScan.key_press = 1;
}
/****************************************************************
 * 函数名称: KeyStateClear
 * 函数功能: 清除按键按下标志
 * 函数输入: 无
 * 函数输出: 无,函数体内部改变相应变量的数值
 * 函数返回值:无
*****************************************************************/
void KeyStateClear(void)
{
  stKeyScan.key_press = 0;
}
/****************************************************************
 * 函数名称: GetKeyState
 * 函数功能: 获取按键是否按下标志
 * 函数输入: 无
 * 函数输出: uint8_t,1:按键按下; 0: 按键未按下
 * 函数返回值:无
*****************************************************************/
uint8_t GetKeyState(void)
{
   return stKeyScan.key_press;
}

/****************************************************************
 * 函数名称: SetEventMarkValue
 * 函数功能: 保存按键按下次数
 * 函数输入: 无
 * 函数输出: 无,函数体内部改变相应变量的数值
 * 函数返回值:无
*****************************************************************/
void SetEventMarkValue(void)
{
   stAddSaveDataSD.event_mark_save_value = stAddSaveDataSD.event_mark_cnt;
}

/****************************************************************
 * 函数名称: GetEventMarkValue
 * 函数功能: 获取按键按下次数
 * 函数输入: 无
 * 函数输出: 无,函数体内部改变相应变量的数值
 * 函数返回值:无
*****************************************************************/
void GetEventMarkValue(void)
{
	 stAddSaveDataSD.event_mark_cnt = stAddSaveDataSD.event_mark_save_value;
}

uint32_t GetEventMark(void)
{
  return stAddSaveDataSD.event_mark_cnt;
}
