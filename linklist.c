/*
 *	linklist.c
 *
 *	Created on: 2022-03-28
 *	Author: 	WangJianLong
 */

#include "linklist.h"

/*******************************************************************************
* 函数功能: 申请链表节点
* 输入参数: 
* 输出参数: 头节点的首地址
* 函数说明: 
*******************************************************************************/
struct DOUBLELINKLISTNODE* LinklistCareate(void)
{
	struct DOUBLELINKLISTNODE* LinklistHead;
	
	LinklistHead = malloc(sizeof(*LinklistHead));//申请链表头
	if (LinklistHead == NULL)
	{
		return NULL;
	}

	LinklistHead->Next = LinklistHead;
	LinklistHead->Prev = LinklistHead;

	return LinklistHead;
}

/*******************************************************************************
* 函数功能: 数据插入链表
* 输入参数: *LinklistHead：链表头部首地址。*Data：待插入的数据。插入方式（首部模式/尾部模式）
* 输出参数: 
* 函数说明: 无
*******************************************************************************/
uint16_t DataInsertLinklist(struct DOUBLELINKLISTNODE* LinklistHead, struct LINKLISTDATATYPE* Data)
{
	struct DOUBLELINKLISTNODE* Head = LinklistHead;
	struct DOUBLELINKLISTNODE* NewNode = NULL;
	
	//申请节点
	NewNode = (struct DOUBLELINKLISTNODE*)malloc(sizeof(*NewNode));
	
	if(NewNode == NULL)
		return MALLOCFAIL;
	
	//将传入的数据copy到申请节点的数据空间中
	NewNode->Data = (*Data);
	
	//将申请的节点插入链表首部
	NewNode->Prev = Head;
	NewNode->Next= Head->Next;
	NewNode->Prev->Next = NewNode;
	NewNode->Next->Prev = NewNode;
	
	return INSERTSUCCESS;
}

/*******************************************************************************
* 函数功能: 取出链表数据
* 输入参数: 链表头部、取数据方式（首部/尾部）
* 输出参数: 无
* 函数说明: 无
*******************************************************************************/
uint16_t FetchLinklistData(struct DOUBLELINKLISTNODE* LinklistHead, struct LINKLISTDATATYPE* SaveData)
{
	struct DOUBLELINKLISTNODE* Head = LinklistHead;
	struct DOUBLELINKLISTNODE* NewNode;
	uint16_t ret = 0;
	
	//判断链表是否为空
	ret = EmptyCheak(Head);
	if(ret == LLISTEMPTY)
		return LLISTEMPTY;
	
	//根据选择取出的方式 从链表中取出节点
	NewNode = Head->Prev;
	Head->Prev = NewNode->Prev;
	Head->Prev->Next = Head;
	
	//将取出的节点数据copy到要保存的数据空间中
	(*SaveData) = NewNode->Data;
	
	//释放节点
	free(NewNode);
	
	return FETCHSUCCESS;
}

/*******************************************************************************
* 函数功能: 判断链表是否为空
* 输入参数: 
* 输出参数: 链表为空/链表非空
* 函数说明: 无
*******************************************************************************/
uint16_t EmptyCheak(struct DOUBLELINKLISTNODE* LinklistHead)
{
	struct DOUBLELINKLISTNODE* Head = LinklistHead;
	
	if(Head->Next == Head)
		return LLISTEMPTY;
	return LLISTNOEMPTY;
}

