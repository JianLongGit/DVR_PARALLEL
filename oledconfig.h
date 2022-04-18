/*
 * oledconfig.h
 *
 *  Created on: 2022-04-18
 *      Author: WangJianLong
 */
#ifndef OLED_CONFIG_
#define OLED_CONFIG_

#include "key.h"
#include "string.h"
#include "olediic.h"
#include "linklist.h"
#include "stm32f4xx.h"

//OLED����������
enum OLEDCOMMANDFLAG{
	OledCommand = 0,	//����
	OledLocation,		//����
	OledData,			//����
};


enum OLEDINVERSEFLAG{
	NoNeedInverse = 0,	//������
	NeedInverse,		//����
};

//ҳ����ƽṹ��
struct OLEDPAGECTRL
{
	uint16_t SendCompleteLen;		//��ǰ�ڵ��ѷ�����ɵ����ݳ���
	uint16_t InverseLocation;		//��ǰ����λ��
	uint16_t SelectModuleNumber;	//ѡ���ģ����
	union 
	{
		uint16_t All;
		struct
		{
			uint16_t CurrentIndex: 8;	//��ǰҳ������
			uint16_t LastIndex: 8;		//��һ��ҳ������
		}Bits;
	}PageIndex;			
};

//OLED��־λ�ṹ��
union OLEDFLAG
{
	uint16_t All;
	struct
	{
		uint16_t AllUpdateFlag: 1;		//ȫ�����±�־λ
		uint16_t UpdateDisplayFlag: 1;	//������ʾ��־λ
		uint16_t OledFaultFlag: 1;		//OLED���ϱ�־λ���ڴ�й¶ or iic��ʱ��
		uint16_t Reserve: 12;
	}Bits;
};

//OLED���ƽṹ��
struct OLEDCTRL
{
	union  OLEDFLAG	Flag;					//OLED��־λ�ṹ��
	struct KEYCONTROL KeyCtrl;				//�������ƽṹ��
	struct OLEDPAGECTRL PageCtrl;			//ҳ����ƽṹ��
	struct DOUBLELINKLISTNODE* LinklistHead;//����ͷ
	struct LINKLISTDATATYPE NodeData;		//����ȡ���Ľڵ�����
	struct OLEDI2CSTRUCT OledI2cCtrl;		//IIC���ƽṹ��
	TIMESTAMPINFO PageUpdateDelay;			//ҳ�����ʱ���
};

//��Ŀ�ṹ�壨һ��ҳ����������Ŀ��
struct ITEMSTRUCT {
	void (*ItemDisplayFunction)(void);	// ��ǰ��Ŀ��ʾ������
	void (*ItemEnterFunction)(void);	// ѡ��ǰ��Ŀʱִ�еĹ��ܺ�����
	uint16_t ItemEnterKeyIndex;			//��ǰ��Ŀ��ȷ��������
};

//ҳ��ṹ��
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

//��ҳ������
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

extern uint16_t OledExternalDataArr[39];

extern struct OLEDPAGESTRUCT PageIndexTable[];
extern struct OLEDCTRL OledCtrl;

extern void ClearScreen(void);
extern void OledInit(void);

#endif
