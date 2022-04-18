/*
 *	rtc.c
 *
 *	Created on: 2022-04-18
 *	Author: 	WangJianLong
 */
 
#include "rtc.h"

struct RTCCTRLSTRUCT RTCCtrl;

/*******************************************************************************
* 函 数 名: RTCInit
* 函数功能: RTC初始化
* 输	入: 无
* 输  	出: 0:初始化成功;  1:LSE开启失败
*******************************************************************************/ 
void RTCInit(void)
{
	uint16_t ret = 0;
	struct RTCTIMESTRUCT RTCCtrl;
	RTC_InitTypeDef RTC_InitStructure;
	TIMESTAMPINFO LSERDYDELAY;
	
	TimeStampClear(&LSERDYDELAY);//清除等待LSE稳定时间戳
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR,ENABLE);//打开RTC后备寄存器电源时钟
	PWR_BackupAccessCmd(ENABLE);//打开后备寄存器访问
	RCC_LSEConfig(RCC_LSE_ON);//开启外部32.768K RTC时钟
	while(RCC_GetFlagStatus(RCC_FLAG_LSERDY)==0) //等待LSE就绪
	{
		if(TimeDelay(&LSERDYDELAY,2000,TIM3) == TimeFinish)
		{
			return;
		}		
	}
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE); //选择LSE作为RTC时钟	
	RCC_RTCCLKCmd(ENABLE);//使能RTC时钟	

	//设置RTC时钟频率为1Hz
	RTC_InitStructure.RTC_AsynchPrediv=0x7f;	//7位异步分频器分频值
	RTC_InitStructure.RTC_SynchPrediv=0xff;		//15位同步分频器分频值		
	RTC_InitStructure.RTC_HourFormat=RTC_HourFormat_24; //24小时制
	ret = RTC_Init(&RTC_InitStructure);
	if(ret == ERROR)
	{
		return;
	}
	
	//设置RTC时间初始值为2022-4-18 14:32:00
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
* 函 数 名: ReadRtcTime
* 函数功能: 读RTC时间
* 输	入: struct RTCTIMESTRUCT：RTC控制结构体
* 输  	出: 无
*******************************************************************************/ 
void ReadRtcTime(struct RTCTIMESTRUCT* RTCCtrlSt)
{
	struct RTCTIMESTRUCT *RTCCtrl = RTCCtrlSt;
	RTC_TimeTypeDef RTC_StampTimeStructure;
	RTC_DateTypeDef RTC_StampDateStructure;
	
	RTC_GetTime(RTC_Format_BIN, &RTC_StampTimeStructure);//读取时间
	RTC_GetDate(RTC_Format_BIN, &RTC_StampDateStructure);//读取日期
	
	RTCCtrl->RtcMinSec.Bits.RtcSecond = RTC_StampTimeStructure.RTC_Seconds;
	RTCCtrl->RtcMinSec.Bits.RtcMinute = RTC_StampTimeStructure.RTC_Minutes;
	RTCCtrl->RtcDayHour.Bits.RtcHour = RTC_StampTimeStructure.RTC_Hours;
	RTCCtrl->RtcDayHour.Bits.RtcDay = RTC_StampDateStructure.RTC_Date;
	RTCCtrl->RtcYearMonth.Bits.RtcMonth = RTC_StampDateStructure.RTC_Month;
	RTCCtrl->RtcYearMonth.Bits.RtcYear = RTC_StampDateStructure.RTC_Year;
}

/*******************************************************************************
* 函 数 名: SetRtcTime
* 函数功能: 读RTC时间
* 输	入: struct RTCTIMESTRUCT：RTC控制结构体
* 输  	出: 无
*******************************************************************************/ 
uint16_t SetRtcTime(struct RTCTIMESTRUCT* RTCCtrlSt)
{
	uint16_t ret = 0;
	struct RTCTIMESTRUCT *RTCCtrl = RTCCtrlSt;
	RTC_TimeTypeDef RTC_TimeTypeInitStructure;
	RTC_DateTypeDef RTC_DateTypeInitStructure;
	
	//设置时间
	RTC_TimeTypeInitStructure.RTC_Hours = RTCCtrl->RtcDayHour.Bits.RtcHour;
	RTC_TimeTypeInitStructure.RTC_Minutes = RTCCtrl->RtcMinSec.Bits.RtcMinute;
	RTC_TimeTypeInitStructure.RTC_Seconds = RTCCtrl->RtcMinSec.Bits.RtcSecond;
	RTC_TimeTypeInitStructure.RTC_H12 = RTC_H12_AM;
	ret = RTC_SetTime(RTC_Format_BIN,&RTC_TimeTypeInitStructure);
	if(ret == ERROR)//初始化失败 或 RTC寄存器未同步
	{
		return RtcSetTimeFail;
	}
	
	//设置日期
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


 
