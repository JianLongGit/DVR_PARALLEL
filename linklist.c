/*
 *	linklist.c
 *
 *	Created on: 2022-03-28
 *	Author: 	WangJianLong
 */

#include "linklist.h"

/*******************************************************************************
* 函数功能: 申请链表头
* 输入参数: 数据区域的大小
* 输出参数: 头节点
* 函数说明: 无
*******************************************************************************/
struct DOUBLELINKLIST* LinklistCareate(int Size)
{
	struct DOUBLELINKLIST *LinklistHead;
	
	LinklistHead = malloc(sizeof(*LinklistHead));//申请链表头
	if (LinklistHead == NULL)
	{
		return NULL;
	}

	LinklistHead->DataSize = Size;
	LinklistHead->HeadNode->Data = NULL;
	LinklistHead->HeadNode->Next = LinklistHead->HeadNode;
	LinklistHead->HeadNode->Prev = LinklistHead->HeadNode;

	return LinklistHead;
}

/*******************************************************************************
* 函数功能: 数据插入链表
* 输入参数: 链表头部、待插入的数据、插入方式（首部模式/尾部模式）
* 输出参数: 
* 函数说明: 无
*******************************************************************************/
uint16_t DataInsertLinklist(struct DOUBLELINKLIST* LinklistHead, const void *Data, uint16_t Mode)
{
	struct DOUBLELINKLIST* Linklist = LinklistHead;
	struct DOUBLELINKLISTNODE *NewNode;
	
	//申请节点
	NewNode = malloc(sizeof(NewNode));
	if(NewNode == NULL)
		return MALLOCFAIL;
	
	//申请数据空间
	NewNode->Data = malloc(sizeof(Linklist->DataSize));
	if(NewNode->Data == NULL)
		return MALLOCFAIL;
	
	//将传入的数据copy到申请节点的数据空间中
	memcpy(NewNode->Data, Data, Linklist->DataSize);
	
	//根据选择的插入方式 将申请的节点插入链表
	switch(Mode)
	{
		case FirstMode: //首部插入
			NewNode->Prev = Linklist->HeadNode;
			NewNode->Next= Linklist->HeadNode->Next;
			break;
		case BackMode: 	//尾部插入
			NewNode->Prev = Linklist->HeadNode->Prev;
			NewNode->Next = Linklist->HeadNode;
			break;
		default:		//首部插入
			NewNode->Prev = Linklist->HeadNode;
			NewNode->Next= Linklist->HeadNode->Next;
			break;
	}
	NewNode->Prev->Next = NewNode;
	NewNode->Next->Prev = NewNode;
	
	return INSERTSUCCESS;
}

/*******************************************************************************
* 函数功能: 	取出链表数据
* 输入参数: 链表头部、取数据方式（首部/尾部）
* 输出参数: 无
* 函数说明: 无
*******************************************************************************/
uint16_t FetchLinklistData(struct DOUBLELINKLIST* LinklistHead, void *SaveData, uint16_t Mode)
{
	struct DOUBLELINKLIST* Linklist = LinklistHead;
	struct DOUBLELINKLISTNODE *NewNode;
	uint16_t ret = 0;
	
	//判断链表是否为空
	ret = EmptyCheak(Linklist);
	if(ret == LLISTEMPTY)
		return LLISTNOEMPTY;
	
	//根据选择取出的方式 从链表中取出节点
	switch(Mode)
	{
		case FirstMode:
			NewNode = Linklist->HeadNode->Next;
			Linklist->HeadNode->Next = NewNode->Next;
			NewNode->Next->Prev = Linklist->HeadNode;
			break;
		case BackMode:
			NewNode = Linklist->HeadNode->Prev;
			Linklist->HeadNode->Prev = NewNode->Prev;
			NewNode->Prev->Next = Linklist->HeadNode;
			break;
		default:
			NewNode = Linklist->HeadNode->Prev;
			Linklist->HeadNode->Prev = NewNode->Prev;
			NewNode->Prev->Next = Linklist->HeadNode;
			break;
	}
	
	//将取出节点的数据区数据copy到要保存的结构体中
	memcpy(SaveData, NewNode->Data, Linklist->DataSize);
	
	//释放节点
	free(NewNode->Data);
	free(NewNode);
	
	return FETCHSUCCESS;
}

/*******************************************************************************
* 函数功能: 判断链表是否为空
* 输入参数: 
* 输出参数: 链表为空/链表非空
* 函数说明: 无
*******************************************************************************/
uint16_t EmptyCheak(struct DOUBLELINKLIST* LinklistHead)
{
	struct DOUBLELINKLIST *Linklist = LinklistHead;
	
	if(Linklist->HeadNode->Next == Linklist->HeadNode)
		return LLISTEMPTY;
	return LLISTNOEMPTY;
}

