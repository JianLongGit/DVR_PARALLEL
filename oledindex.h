#ifndef OLED_INDEX_
#define OLED_INDEX_

#include "stm32f4xx.h"
#include "stdlib.h"
#include "olediic.h"
#include "linklist.h"
#include "key.h"	

enum OLEDCOMMANDFLAG{
	OledData = 0,
	OledCommand,
};

enum OLEDINVERSEFLAG{
	NoNeedInserve = 0,
	NeedInverse,
};

struct OLEDPAGECTRL
{
	uint16_t SendCompleteLen;
	uint16_t InverseLocation;
	union 
	{
		uint16_t All;
		struct
		{
			uint16_t CurrentIndex: 8;
			uint16_t LastIndex: 8;
		}Bits;
	}PageIndex;			
};

union OLEDFLAG
{
	uint16_t All;
	struct
	{
		uint16_t AllUpdateFlag: 1;
		uint16_t UpdateDisplayFlag: 1;
		uint16_t OledFaultFlag: 1;
		uint16_t OledMallocFaultFlag: 1;//测试用
		uint16_t Reserve: 12;
	}Bits;
};

struct OLEDCTRL
{
	union  OLEDFLAG	Flag;
	struct KEYCONTROL KeyCtrl;
	struct OLEDPAGECTRL PageCtrl;
	struct DOUBLELINKLIST *LinklistHead;
	struct DOUBLELINKLIST* DataNode;
	struct OLEDI2CSTRUCT OledI2cCtrl;
	TIMESTAMPINFO PageUpdateDelay;
};

//显示项目的结构体（一个页面包含多个项目）
struct ITEMSTRUCT {
	void (*ItemDisplayFunction)(void); // 当前项目显示函数；
	void (*ItemEnterFunction)(void); // 选择当前项目时执行的功能函数；
	uint16_t ItemEnterKeyIndex; 	 //当前项目的确定索引；
};

//显示页面的结构体数组
struct OLEDPAGESTRUCT {
	struct ITEMSTRUCT *Item;					//项目指针;
	union
	{
		uint32_t  All;
		struct {
			uint32_t CurrentPageDownIndex: 	8;	//当前页面的下键键索引
			uint32_t CurrentPageUpIndex: 	8;	//当前页面的上键索引
			uint32_t CurrentPageReturnIndex: 8; //返回键索引
			uint32_t Reserve: 8;
		}Bits;
	}PageIndex;
	union
	{
		uint16_t  All;
		struct {
			uint16_t InverseItemNum: 		8;	//可反显的项目数
			uint16_t ItemNum: 				8;	//总项目数			
		}Bits;
	}PageInforCtrl;
};

enum
{
	HomePageIndex = 0,
	ModuleSeletionPage1Index,
	ModuleSeletionPage2Index,
	ResetPage1Index,
	ResetPage2Index,
	CommModInforPage1Index,
	CommModInforPage2Index,
	CommModInforPage3Index,
	PowerModInforPage1Index,
};
extern struct OLEDPAGESTRUCT PageIndexTable[];

extern struct ITEMSTRUCT HomePageItem[];
extern struct ITEMSTRUCT ModuleSeletionPage1[];
extern struct ITEMSTRUCT ModuleSeletionPage2[];
extern struct ITEMSTRUCT ModResetPage1[];
extern struct ITEMSTRUCT ModResetPage2[];
extern struct ITEMSTRUCT CommModInforPage1[];
extern struct ITEMSTRUCT CommModInforPage2[];
extern struct ITEMSTRUCT CommModInforPage3[];
extern struct ITEMSTRUCT  PowerModInforPage1[];
extern uint16_t OledExternalDataArr[20];
extern struct OLEDCTRL OledCtrl;
extern void ClearScreen(uint16_t InverseFlag);

#endif
