#ifndef _TIMER_H
#define _TIMER_H

#include "stm32f4xx.h"

void TIM3_Int_Init(void);
void TIM4_Int_Init(void);

typedef struct
{
    uint32_t stamp;  
	uint32_t range;  
}TIMESTAMPINFO;

enum 
{
  TimeInit = 0,
	TimeFinish,
	TimeContinue
};

extern void TimeStampClear(TIMESTAMPINFO *TimeStampStruct);
extern uint16_t TimeDelay(TIMESTAMPINFO *TimeStampStruct,uint16_t delaytime,TIM_TypeDef* TIMx);

#endif
