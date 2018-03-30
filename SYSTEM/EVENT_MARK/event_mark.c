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
	  // ���ʹ���ⲿ�ж�0,��������
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
 * ��������: EventMarkAdd
 * ��������: EventMarkֵ�ۼ�
 * ��������: ��
 * �������: ��,�������ڲ��ı���Ӧ��������ֵ
 * ��������ֵ:��
*****************************************************************/
void EventMarkAdd(void)
{
   stAddSaveDataSD.event_mark_cnt++;
}

/****************************************************************
 * ��������: EventMarkAdd
 * ��������: EventMarkֵ����
 * ��������: ��
 * �������: ��,�������ڲ��ı���Ӧ��������ֵ
 * ��������ֵ:��
*****************************************************************/
void EventMarkClear(void)
{
   stAddSaveDataSD.event_mark_cnt = 0;
}

/****************************************************************
 * ��������: KeyScanUpDown
 * ��������: ��������,���ݰ�������/�ɿ�״̬,����Ӧ��EventMark��ֵ
 * ��������: ��
 * �������: ��,�������ڲ��ı���Ӧ��������ֵ
 * ��������ֵ:��
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
			
			// ����ǵ͵�ƽ,����������,EventMark��ֵΪ�������µĴ���
			if(stKeyScan.scan_down_cnt==100)
      {
				stKeyScan.scan_down_cnt = 0;
				KeyStateClear();
				GetEventMarkValue();
			  EventMarkAdd();	
        SetEventMarkValue();				
			}
			else if(stKeyScan.scan_up_cnt==100)// ����Ǹߵ�ƽ,�������ɿ�,EventMark��ֵΪ0
			{ 
				stKeyScan.scan_up_cnt = 0;
			  KeyStateClear();
				EventMarkClear();				
			}
		}			
}

/****************************************************************
 * ��������: KeyStateChange
 * ��������: �ð������±�־
 * ��������: ��
 * �������: ��,�������ڲ��ı���Ӧ��������ֵ
 * ��������ֵ:��
*****************************************************************/
void KeyStateChange(void)
{
	stKeyScan.key_press = 1;
}
/****************************************************************
 * ��������: KeyStateClear
 * ��������: ����������±�־
 * ��������: ��
 * �������: ��,�������ڲ��ı���Ӧ��������ֵ
 * ��������ֵ:��
*****************************************************************/
void KeyStateClear(void)
{
  stKeyScan.key_press = 0;
}
/****************************************************************
 * ��������: GetKeyState
 * ��������: ��ȡ�����Ƿ��±�־
 * ��������: ��
 * �������: uint8_t,1:��������; 0: ����δ����
 * ��������ֵ:��
*****************************************************************/
uint8_t GetKeyState(void)
{
   return stKeyScan.key_press;
}

/****************************************************************
 * ��������: SetEventMarkValue
 * ��������: ���水�����´���
 * ��������: ��
 * �������: ��,�������ڲ��ı���Ӧ��������ֵ
 * ��������ֵ:��
*****************************************************************/
void SetEventMarkValue(void)
{
   stAddSaveDataSD.event_mark_save_value = stAddSaveDataSD.event_mark_cnt;
}

/****************************************************************
 * ��������: GetEventMarkValue
 * ��������: ��ȡ�������´���
 * ��������: ��
 * �������: ��,�������ڲ��ı���Ӧ��������ֵ
 * ��������ֵ:��
*****************************************************************/
void GetEventMarkValue(void)
{
	 stAddSaveDataSD.event_mark_cnt = stAddSaveDataSD.event_mark_save_value;
}

uint32_t GetEventMark(void)
{
  return stAddSaveDataSD.event_mark_cnt;
}
