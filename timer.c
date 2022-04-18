/*
 * timer.c
 *
 *  Created on: 2022-03-28
 *      Author: WangJianLong
 */
#include "timer.h"

/***************************************
*函数功能: 定时器3初始化
*输入参数: 
*输出参数:
*函数说明: 计数一次500us;
****************************************/
void TIM3_Int_Init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  ///使能TIM3时钟
	
	TIM_TimeBaseInitStructure.TIM_Period = 65535; 	//自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler = 42000-1;  	//定时器分频
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Down; //向下计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV4; 
	
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);//初始化TIM3
	
	TIM_Cmd(TIM3,ENABLE); //使能定时器3	
}

/***************************************
*函数功能:定时器4初始化
*输入参数:
*输出参数:
*函数说明:
****************************************/
void TIM4_Int_Init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);  ///使能TIM4时钟
	
	TIM_TimeBaseInitStructure.TIM_Period = 65535; 	//自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler = 84-1;  //定时器分频
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Down; //向下计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV4; 
	
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStructure);//初始化TIM4
	
	TIM_Cmd(TIM4,ENABLE); //使能定时器4
}

/***************************************
*函数功能:获取定时器的计数值
*输入参数:
*输出参数:
*函数说明:
****************************************/
static uint32_t GetTime(TIM_TypeDef* TIMx)
{
	return TIM_GetCounter(TIMx);
}

/***************************************
*函数功能:设置时间戳和延时时间
*输入参数:ptimestampstruct：要设置的时间戳结构体地址
          delaytime：延时时间
          struct CPUTIMER_VARS *Timer：定时器
*输出参数:
*函数说明:
****************************************/
static void TimeStampSet(TIMESTAMPINFO *ptimestampstruct,uint16_t delaytime,TIM_TypeDef* TIMx)
{
    TIMESTAMPINFO *fd = ptimestampstruct;

	fd->stamp = GetTime(TIMx);
	fd->range = delaytime;
}

/***************************************
*函数功能:清除时间戳和延时时间
*输入参数:ptimestampstruct：要清除的时间戳结构体地址
*输出参数:
*函数说明:
****************************************/
void TimeStampClear(TIMESTAMPINFO *TimeStampStruct)
{
    TIMESTAMPINFO *fd = TimeStampStruct;

	fd->stamp = 0;
	fd->range = 0;
}

/***************************************
*函数功能:查询延时状态
*输入参数:ptimestampstruct：要查询的时间戳结构体地址
*输出参数:TimeFinish   ：延时时间到
          Timecontinue：延时时间未到
*函数说明:按照定时器递减计算延时时间是否到
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
*函数功能:时间延时函数 用于设置时间戳并判断延时是否到
*输入参数:ptimestampstruct：时间戳结构体地址
          delaytime：延时时间
*输出参数:TimeInit：时间戳刚启用 之前空闲 
          TimeFinish：延时时间到
          Timecontinue：延时中，时间未到
*函数说明:
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










