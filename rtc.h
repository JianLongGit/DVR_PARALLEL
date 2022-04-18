/*
 * rtc.h
 *
 *  Created on: 2022-04-18
 *      Author: WangJianLong
 */
 
#ifndef RTC_H_
#define RTC_H_

#include "stm32f4xx.h"
#include "stm32f4xx_rtc.h"
#include "stm32f4xx_pwr.h"
#include "timer.h"

enum RTCFLAG{
	RtcLSEFault = 0,
	RtcInitFail,
	RtcInitSuccess,
	RtcSetTimeFail,
	RtcSetTimeSuccess,
};

//RTC数据结构体
struct RTCTIMESTRUCT 
{
	union
	{
		uint16_t All;
		struct
		{
			uint16_t RtcSecond : 8;
			uint16_t RtcMinute : 8;
		}Bits;
	}RtcMinSec;//分秒
	union
	{
		uint16_t All;
		struct
		{
			uint16_t RtcDay  : 8;
			uint16_t RtcHour : 8;
		}Bits;
	}RtcDayHour;//日时
	union
	{
		uint16_t All;
		struct
		{
			uint16_t RtcYear : 8;
			uint16_t RtcMonth : 8;
		}Bits;
	}RtcYearMonth;//年月
};

struct RTCCTRLSTRUCT
{
	struct RTCTIMESTRUCT RtcSetTime;
	struct RTCTIMESTRUCT RtcReadTime;
};

extern struct RTCCTRLSTRUCT RTCCtrl;
extern void RTCInit(void);
extern uint16_t SetRtcTime(struct RTCTIMESTRUCT* RTCCtrlSt);
extern void ReadRtcTime(struct RTCTIMESTRUCT* RTCCtrlSt);

#endif
