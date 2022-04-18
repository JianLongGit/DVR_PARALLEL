/*
 * olediic.h
 *
 *  Created on: 2022-03-23
 *      Author: WangJianLong
 */

#ifndef OLEDI2C_H_
#define OLEDI2C_H_

#include "timer.h"
#include "stm32f4xx.h"
#include "stm32f4xx_i2c.h"

#define OLEDSLAVEADDR 0x78

enum I2CWRITESTATE {
	OledI2cInactive = 0,
	OledI2cSlaveAddr,
	OledI2cRegAddr,
	OledI2cData,
	OledI2cStop,
	OledI2cLoop,
};

union OLEDI2CDATAINFOR 
{
    uint16_t All;
    struct 
    {
        uint16_t Addr : 8;
        uint16_t Data : 8;
    }Byte;
};

struct OLEDI2CSTRUCT
{
    union OLEDI2CDATAINFOR	OledI2cDataInfor; 
	uint16_t OledI2CState;
    TIMESTAMPINFO I2CTimeStamp;
};

enum 
{
    FalseFlag = 0,
	TrueFlag
};

//extern struct OLEDI2CSTRUCT OledI2cCtrl;
extern void I2C1_Init(void);
uint16_t OledI2CSendData(struct OLEDI2CSTRUCT *OledI2CCtrl);

#endif
