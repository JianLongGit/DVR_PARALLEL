/*
 * key.c
 *
 *  Created on: 2022-03-28
 *      Author: WangJianLong
 */

#include "key.h"

/*******************************************************************************
* ��������: ����IO��ʼ��
* �������: 
* �������: ��
* ����˵��: ����IO�ڣ�GPIOC_00��GPIOC_01��GPIOC_02��GPIOC_03
*******************************************************************************/
void OledKeyInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; //����ṹ�����
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN; 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;//�ⲿ�Ѿ�Ĭ��������
	GPIO_Init(GPIOC,&GPIO_InitStructure);	
}

/*******************************************************************************
* ��������: ����IO��ɨ��
* �������: KeyCtrlSt���������ƽṹ��
* �������:
* ����˵��:
*******************************************************************************/
void OledKeyIOScan(struct KEYCONTROL *KeyCtrlSt)
{
	KeyCtrlSt->KEYSTATE.Bits.KeyIOValue = (GPIO_ReadInputData(GPIOC)&0x0f);
}

/*******************************************************************************
* ��������: �������
* �������: KeyCtrlSt���������ƽṹ��
* �������:
* ����˵��:
*******************************************************************************/
void OledKeyDetection(struct KEYCONTROL *KeyCtrlSt)
{
	if(KeyCtrlSt->KEYSTATE.Bits.KeyIOValue == KeyCtrlSt->KEYSTATE.Bits.TempKeyValue)//��ǰ����ֵ����һ�εİ���ֵ��ͬ
	{

		if(TimeDelay(&KeyCtrlSt->KeyDelayTime,200,TIM3) == TimeFinish)//100ms����ʱ�䵽
		{
			KeyCtrlSt->KEYSTATE.Bits.KeyValue = KeyCtrlSt->KEYSTATE.Bits.TempKeyValue;
		}
		return;
	}
	else if(KeyCtrlSt->KEYSTATE.Bits.KeyIOValue != 0)
	{
		if((KeyCtrlSt->KEYSTATE.Bits.KeyIOValue & (KeyCtrlSt->KEYSTATE.Bits.KeyIOValue-1))== 0)//ֻ��һ����������
		{
			KeyCtrlSt->KEYSTATE.Bits.TempKeyValue = KeyCtrlSt->KEYSTATE.Bits.KeyIOValue;
		}
	}
	TimeStampClear(&KeyCtrlSt->KeyDelayTime);
}






