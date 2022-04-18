/*
 *	rtc.c
 *
 *	Created on: 2022-04-18
 *	Author: 	WangJianLong
 */
 
#include "rtc.h"

struct RTCCTRLSTRUCT RTCCtrl;

/*******************************************************************************
* �� �� ��: RTCInit
* ��������: RTC��ʼ��
* ��	��: ��
* ��  	��: 0:��ʼ���ɹ�;  1:LSE����ʧ��
*******************************************************************************/ 
void RTCInit(void)
{
	uint16_t ret = 0;
	struct RTCTIMESTRUCT RTCCtrl;
	RTC_InitTypeDef RTC_InitStructure;
	TIMESTAMPINFO LSERDYDELAY;
	
	TimeStampClear(&LSERDYDELAY);//����ȴ�LSE�ȶ�ʱ���
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR,ENABLE);//��RTC�󱸼Ĵ�����Դʱ��
	PWR_BackupAccessCmd(ENABLE);//�򿪺󱸼Ĵ�������
	RCC_LSEConfig(RCC_LSE_ON);//�����ⲿ32.768K RTCʱ��
	while(RCC_GetFlagStatus(RCC_FLAG_LSERDY)==0) //�ȴ�LSE����
	{
		if(TimeDelay(&LSERDYDELAY,2000,TIM3) == TimeFinish)
		{
			return;
		}		
	}
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE); //ѡ��LSE��ΪRTCʱ��	
	RCC_RTCCLKCmd(ENABLE);//ʹ��RTCʱ��	

	//����RTCʱ��Ƶ��Ϊ1Hz
	RTC_InitStructure.RTC_AsynchPrediv=0x7f;	//7λ�첽��Ƶ����Ƶֵ
	RTC_InitStructure.RTC_SynchPrediv=0xff;		//15λͬ����Ƶ����Ƶֵ		
	RTC_InitStructure.RTC_HourFormat=RTC_HourFormat_24; //24Сʱ��
	ret = RTC_Init(&RTC_InitStructure);
	if(ret == ERROR)
	{
		return;
	}
	
	//����RTCʱ���ʼֵΪ2022-4-18 14:32:00
	RTCCtrl.RtcMinSec.Bits.RtcSecond = 05;
	RTCCtrl.RtcMinSec.Bits.RtcMinute = 39;
	RTCCtrl.RtcDayHour.Bits.RtcHour =  15;
	RTCCtrl.RtcDayHour.Bits.RtcDay = 18;
	RTCCtrl.RtcYearMonth.Bits.RtcMonth = 04;
	RTCCtrl.RtcYearMonth.Bits.RtcYear = 22;
	ret = SetRtcTime(&RTCCtrl);
	if(ret == ERROR)
	{
		return;
	}
}

/*******************************************************************************
* �� �� ��: ReadRtcTime
* ��������: ��RTCʱ��
* ��	��: struct RTCTIMESTRUCT��RTC���ƽṹ��
* ��  	��: ��
*******************************************************************************/ 
void ReadRtcTime(struct RTCTIMESTRUCT* RTCCtrlSt)
{
	struct RTCTIMESTRUCT *RTCCtrl = RTCCtrlSt;
	RTC_TimeTypeDef RTC_StampTimeStructure;
	RTC_DateTypeDef RTC_StampDateStructure;
	
	RTC_GetTime(RTC_Format_BIN, &RTC_StampTimeStructure);//��ȡʱ��
	RTC_GetDate(RTC_Format_BIN, &RTC_StampDateStructure);//��ȡ����
	
	RTCCtrl->RtcMinSec.Bits.RtcSecond = RTC_StampTimeStructure.RTC_Seconds;
	RTCCtrl->RtcMinSec.Bits.RtcMinute = RTC_StampTimeStructure.RTC_Minutes;
	RTCCtrl->RtcDayHour.Bits.RtcHour = RTC_StampTimeStructure.RTC_Hours;
	RTCCtrl->RtcDayHour.Bits.RtcDay = RTC_StampDateStructure.RTC_Date;
	RTCCtrl->RtcYearMonth.Bits.RtcMonth = RTC_StampDateStructure.RTC_Month;
	RTCCtrl->RtcYearMonth.Bits.RtcYear = RTC_StampDateStructure.RTC_Year;
}

/*******************************************************************************
* �� �� ��: SetRtcTime
* ��������: ��RTCʱ��
* ��	��: struct RTCTIMESTRUCT��RTC���ƽṹ��
* ��  	��: ��
*******************************************************************************/ 
uint16_t SetRtcTime(struct RTCTIMESTRUCT* RTCCtrlSt)
{
	uint16_t ret = 0;
	struct RTCTIMESTRUCT *RTCCtrl = RTCCtrlSt;
	RTC_TimeTypeDef RTC_TimeTypeInitStructure;
	RTC_DateTypeDef RTC_DateTypeInitStructure;
	
	//����ʱ��
	RTC_TimeTypeInitStructure.RTC_Hours = RTCCtrl->RtcDayHour.Bits.RtcHour;
	RTC_TimeTypeInitStructure.RTC_Minutes = RTCCtrl->RtcMinSec.Bits.RtcMinute;
	RTC_TimeTypeInitStructure.RTC_Seconds = RTCCtrl->RtcMinSec.Bits.RtcSecond;
	RTC_TimeTypeInitStructure.RTC_H12 = RTC_H12_AM;
	ret = RTC_SetTime(RTC_Format_BIN,&RTC_TimeTypeInitStructure);
	if(ret == ERROR)//��ʼ��ʧ�� �� RTC�Ĵ���δͬ��
	{
		return RtcSetTimeFail;
	}
	
	//��������
	RTC_DateTypeInitStructure.RTC_Date = RTCCtrl->RtcDayHour.Bits.RtcDay;
	RTC_DateTypeInitStructure.RTC_Month = RTCCtrl->RtcYearMonth.Bits.RtcMonth;
	RTC_DateTypeInitStructure.RTC_Year = RTCCtrl->RtcYearMonth.Bits.RtcYear;	
	RTC_DateTypeInitStructure.RTC_WeekDay = 1;
	ret = RTC_SetDate(RTC_Format_BIN,&RTC_DateTypeInitStructure);
	if(ret == ERROR)
	{
		return RtcSetTimeFail;
	}
	
	return RtcInitSuccess;
}


 
