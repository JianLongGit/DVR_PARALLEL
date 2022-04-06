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
#include "string.h"

//节点
struct DOUBLELINKLISTNODE{
    void *Data;							//数据区指针
	struct DOUBLELINKLISTNODE *Next;	//指向下一个节点的指针
	struct DOUBLELINKLISTNODE *Prev;	//指向上一个节点的指针
};

//链表头
struct DOUBLELINKLIST{
    uint16_t DataSize;						//数据区大小
	struct DOUBLELINKLISTNODE *HeadNode;	//头节点
};

//数据插入/取出方式
enum{
	FirstMode = 0,
	BackMode,
};

//插入/取出数据时可能出现的状态
enum{
	LLISTEMPTY = 0,	//链表空
	LLISTNOEMPTY,	//链表非空
	MALLOCFAIL,		//内存申请失败
	INSERTSUCCESS,	//节点插入成功
	FETCHSUCCESS,	//节点取出成功
};

extern struct DOUBLELINKLIST *LinklistCareate(int size);
extern uint16_t DataInsertLinklist(struct DOUBLELINKLIST* LinklistHead, const void *Data, uint16_t Mode);
extern uint16_t FetchLinklistData(struct DOUBLELINKLIST* LinklistHead, void *SaveData, uint16_t Mode);
extern uint16_t EmptyCheak(struct DOUBLELINKLIST* LinklistHead);

#endif
