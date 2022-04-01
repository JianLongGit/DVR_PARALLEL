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
		uint16_t OledMallocFaultFlag: 1;//������
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

//��ʾ��Ŀ�Ľṹ�壨һ��ҳ����������Ŀ��
struct ITEMSTRUCT {
	void (*ItemDisplayFunction)(void); // ��ǰ��Ŀ��ʾ������
	void (*ItemEnterFunction)(void); // ѡ��ǰ��Ŀʱִ�еĹ��ܺ�����
	uint16_t ItemEnterKeyIndex; 	 //��ǰ��Ŀ��ȷ��������
};

//��ʾҳ��Ľṹ������
struct OLEDPAGESTRUCT {
	struct ITEMSTRUCT *Item;					//��Ŀָ��;
	union
	{
		uint32_t  All;
		struct {
			uint32_t CurrentPageDownIndex: 	8;	//��ǰҳ����¼�������
			uint32_t CurrentPageUpIndex: 	8;	//��ǰҳ����ϼ�����
			uint32_t CurrentPageReturnIndex: 8; //���ؼ�����
			uint32_t Reserve: 8;
		}Bits;
	}PageIndex;
	union
	{
		uint16_t  All;
		struct {
			uint16_t InverseItemNum: 		8;	//�ɷ��Ե���Ŀ��
			uint16_t ItemNum: 				8;	//����Ŀ��			
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
