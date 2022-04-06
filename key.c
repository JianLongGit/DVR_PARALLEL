/*
 * key.c
 *
 *  Created on: 2022-03-28
 *      Author: WangJianLong
 */

#include "key.h"

/*******************************************************************************
* 函数功能: 按键IO初始化
* 输入参数: 
* 输出参数: 无
* 函数说明: 按键IO口：GPIOC_00、GPIOC_01、GPIOC_02、GPIOC_03
*******************************************************************************/
void OledKeyInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; //定义结构体变量
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN; 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;//外部已经默认下拉；
	GPIO_Init(GPIOC,&GPIO_InitStructure);	

}

/*******************************************************************************
* 函数功能: 按键IO口扫描
* 输入参数: 
* 输出参数:
* 函数说明:
*******************************************************************************/
void OledKeyIOScan(struct KEYCONTROL *KeyCtrlSt)
{
	KeyCtrlSt->KEYSTATE.Bits.KeyIOValue = (GPIO_ReadInputData(GPIOC)&0x0f);
}

/*******************************************************************************
* 函数功能: 按键检测
* 输入参数: 
* 输出参数:
* 函数说明:
*******************************************************************************/
void OledKeyDetection(struct KEYCONTROL *KeyCtrlSt)
{
	if(KeyCtrlSt->KEYSTATE.Bits.KeyIOValue == KeyCtrlSt->KEYSTATE.Bits.TempKeyValue)
	{

		if(TimeDelay(&KeyCtrlSt->KeyDelayTime,50,TIM3) == TimeFinish)
		{
			KeyCtrlSt->KEYSTATE.Bits.KeyValue = KeyCtrlSt->KEYSTATE.Bits.TempKeyValue;
		}
		return;
	}
	else if(KeyCtrlSt->KEYSTATE.Bits.KeyIOValue != 0)
	{
		if((KeyCtrlSt->KEYSTATE.Bits.KeyIOValue & (KeyCtrlSt->KEYSTATE.Bits.KeyIOValue-1))== 0)
		{
			KeyCtrlSt->KEYSTATE.Bits.TempKeyValue = KeyCtrlSt->KEYSTATE.Bits.KeyIOValue;
		}
	}
	TimeStampClear(&KeyCtrlSt->KeyDelayTime);
}






