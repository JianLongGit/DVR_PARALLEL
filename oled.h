#ifndef OLED_H_
#define OLED_H_

#include "oledconfig.h"
#include "stdlib.h"


#define OLEDSlaveAddr	0x78

extern void OledDisplay(struct OLEDCTRL *OledCtrlStruct);
extern void OledStructInit(void);

void OledClean(void);
void OLED_Init(void);

#endif

