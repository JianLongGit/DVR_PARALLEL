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

//OLED的数据类型
enum OLEDCOMMANDFLAG{
	OledCommand = 0,	//命令
	OledLocation,		//坐标
	OledData,			//数据
};


enum OLEDINVERSEFLAG{
	NoNeedInverse = 0,	//不反显
	NeedInverse,		//反显
};

//页面控制结构体
struct OLEDPAGECTRL
{
	uint16_t SendCompleteLen;		//当前节点已发送完成的数据长度
	uint16_t InverseLocation;		//当前反显位置
	uint16_t SelectModuleNumber;	//选择的模块编号
	union 
	{
		uint16_t All;
		struct
		{
			uint16_t CurrentIndex: 8;	//当前页面索引
			uint16_t LastIndex: 8;		//上一次页面索引
		}Bits;
	}PageIndex;			
};

//OLED标志位结构体
union OLEDFLAG
{
	uint16_t All;
	struct
	{
		uint16_t AllUpdateFlag: 1;		//全部更新标志位
		uint16_t UpdateDisplayFlag: 1;	//更新显示标志位
		uint16_t OledFaultFlag: 1;		//OLED故障标志位（内存泄露 or iic超时）
		uint16_t Reserve: 12;
	}Bits;
};

//OLED控制结构体
struct OLEDCTRL
{
	union  OLEDFLAG	Flag;					//OLED标志位结构体
	struct KEYCONTROL KeyCtrl;				//按键控制结构体
	struct OLEDPAGECTRL PageCtrl;			//页面控制结构体
	struct DOUBLELINKLISTNODE* LinklistHead;//链表头
	struct LINKLISTDATATYPE NodeData;		//保存取出的节点数据
	struct OLEDI2CSTRUCT OledI2cCtrl;		//IIC控制结构体
	TIMESTAMPINFO PageUpdateDelay;			//页面更新时间戳
};

//项目结构体（一个页面包含多个项目）
struct ITEMSTRUCT {
	void (*ItemDisplayFunction)(void);	// 当前项目显示函数；
	void (*ItemEnterFunction)(void);	// 选择当前项目时执行的功能函数；
	uint16_t ItemEnterKeyIndex;			//当前项目的确定索引；
};

//页面结构体
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

//各页面索引
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
