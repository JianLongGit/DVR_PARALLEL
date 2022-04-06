/*
 * key.h
 *
 *  Created on: 2022-03-28
 *      Author: WangJianLong
 */

#ifndef KEY_H_
#define KEY_H_

#include "timer.h"
#include "string.h"

#define KEY_RETURN_VALUE	0x1
#define KEY_ENTER_VALUE 	0x2
#define KEY_DOWN_VALUE 		0x4
#define KEY_UP_VALUE		0x8


struct KEYCONTROL {
	union 
	{
		uint16_t All;
		struct
		{
			uint16_t KeyValue: 4;
			uint16_t TempKeyValue: 4; 
			uint16_t KeyIOValue: 4;
			uint16_t Reserve: 4;
		}Bits;
	}KEYSTATE;
	TIMESTAMPINFO KeyDelayTime;
};

extern void OledKeyInit(void);
extern void OledKeyIOScan(struct KEYCONTROL *);
extern void OledKeyDetection(struct KEYCONTROL *);
#endif
