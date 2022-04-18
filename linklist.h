/*
 * linklist.h
 *
 *  Created on: 2022-03-28
 *      Author: WangJianLong
 */
 
#ifndef LINKLIST_H_
#define LINKLIST_H_

#include "stm32f4xx.h"
#include "stdlib.h"

//数据区
struct LINKLISTDATATYPE
{
	uint16_t *DataAddr[3];	//保存待发送数据的首地址/坐标；
	union
	{
		uint16_t All;
		struct
		{
			uint16_t NeedSendLen: 12; //需要发送的数据长度
			uint16_t InverseFlag: 1;  //是否反显
			uint16_t CommandFlag: 2;  //命令/坐标/数据
			uint16_t Reserve: 1;
		}Bits;
	}CtrlInfor;
};

//链表节点
struct DOUBLELINKLISTNODE{
    struct LINKLISTDATATYPE Data;		//数据区
	struct DOUBLELINKLISTNODE *Next;	//指向下一个节点的指针
	struct DOUBLELINKLISTNODE *Prev;	//指向上一个节点的指针
};

//插入/取出数据时可能出现的状态
enum{
	LLISTEMPTY = 0,	//链表空
	LLISTNOEMPTY,	//链表非空
	MALLOCFAIL,		//内存申请失败
	INSERTSUCCESS,	//节点插入成功
	FETCHSUCCESS,	//节点取出成功
};

extern struct DOUBLELINKLISTNODE* LinklistCareate(void);
extern uint16_t DataInsertLinklist(struct DOUBLELINKLISTNODE* LinklistHead, struct LINKLISTDATATYPE* Data);
extern uint16_t FetchLinklistData(struct DOUBLELINKLISTNODE* LinklistHead, struct LINKLISTDATATYPE* SaveData);
extern uint16_t EmptyCheak(struct DOUBLELINKLISTNODE* LinklistHead);

#endif
