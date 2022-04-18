/*
 * timer.c
 *
 *  Created on: 2022-03-28
 *      Author: WangJianLong
 */
#include "timer.h"

/***************************************
*��������: ��ʱ��3��ʼ��
*�������: 
*�������:
*����˵��: ����һ��500us;
****************************************/
void TIM3_Int_Init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  ///ʹ��TIM3ʱ��
	
	TIM_TimeBaseInitStructure.TIM_Period = 65535; 	//�Զ���װ��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler = 42000-1;  	//��ʱ����Ƶ
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Down; //���¼���ģʽ
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV4; 
	
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);//��ʼ��TIM3
	
	TIM_Cmd(TIM3,ENABLE); //ʹ�ܶ�ʱ��3	
}

/***************************************
*��������:��ʱ��4��ʼ��
*�������:
*�������:
*����˵��:
****************************************/
void TIM4_Int_Init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);  ///ʹ��TIM4ʱ��
	
	TIM_TimeBaseInitStructure.TIM_Period = 65535; 	//�Զ���װ��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler = 84-1;  //��ʱ����Ƶ
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Down; //���¼���ģʽ
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV4; 
	
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStructure);//��ʼ��TIM4
	
	TIM_Cmd(TIM4,ENABLE); //ʹ�ܶ�ʱ��4
}

/***************************************
*��������:��ȡ��ʱ���ļ���ֵ
*�������:
*�������:
*����˵��:
****************************************/
static uint32_t GetTime(TIM_TypeDef* TIMx)
{
	return TIM_GetCounter(TIMx);
}

/***************************************
*��������:����ʱ�������ʱʱ��
*�������:ptimestampstruct��Ҫ���õ�ʱ����ṹ���ַ
          delaytime����ʱʱ��
          struct CPUTIMER_VARS *Timer����ʱ��
*�������:
*����˵��:
****************************************/
static void TimeStampSet(TIMESTAMPINFO *ptimestampstruct,uint16_t delaytime,TIM_TypeDef* TIMx)
{
    TIMESTAMPINFO *fd = ptimestampstruct;

	fd->stamp = GetTime(TIMx);
	fd->range = delaytime;
}

/***************************************
*��������:���ʱ�������ʱʱ��
*�������:ptimestampstruct��Ҫ�����ʱ����ṹ���ַ
*�������:
*����˵��:
****************************************/
void TimeStampClear(TIMESTAMPINFO *TimeStampStruct)
{
    TIMESTAMPINFO *fd = TimeStampStruct;

	fd->stamp = 0;
	fd->range = 0;
}

/***************************************
*��������:��ѯ��ʱ״̬
*�������:ptimestampstruct��Ҫ��ѯ��ʱ����ṹ���ַ
*�������:TimeFinish   ����ʱʱ�䵽
          Timecontinue����ʱʱ��δ��
*����˵��:���ն�ʱ���ݼ�������ʱʱ���Ƿ�
****************************************/
static uint16_t TimeDelayCheck(TIMESTAMPINFO *ptimestampstruct,TIM_TypeDef* TIMx)
{
    uint16_t timecurrent;
	TIMESTAMPINFO *fd = ptimestampstruct;

	timecurrent = GetTime(TIMx);
	if (timecurrent > fd->stamp)
	{
	     if ((fd->stamp + (65535 - timecurrent)) >= fd->range)
	     {
	          return TimeFinish;
	     }
	     else
	     {
	    	  return TimeContinue;
	     }
	 }
	 else
	 {
		 if ((fd->stamp - timecurrent) >= fd->range)
	     {
	          return TimeFinish;
	     }
	     else
	     {
	          return TimeContinue;
	     }
	 }
}

/***************************************
*��������:ʱ����ʱ���� ��������ʱ������ж���ʱ�Ƿ�
*�������:ptimestampstruct��ʱ����ṹ���ַ
          delaytime����ʱʱ��
*�������:TimeInit��ʱ��������� ֮ǰ���� 
          TimeFinish����ʱʱ�䵽
          Timecontinue����ʱ�У�ʱ��δ��
*����˵��:
****************************************/
uint16_t TimeDelay(TIMESTAMPINFO *TimeStampStruct,uint16_t delaytime,TIM_TypeDef* TIMx)//
{
	TIMESTAMPINFO *fd = TimeStampStruct;

	if(0 == fd->range)
	{
		TimeStampSet(fd, delaytime,TIMx);
		return TimeInit;
	}
	else
	{
		if(TimeDelayCheck(fd,TIMx) == TimeFinish)
		{
			TimeStampClear(fd);
			return TimeFinish;
		}
		else
		{
			return TimeContinue;
		}
	}
}










