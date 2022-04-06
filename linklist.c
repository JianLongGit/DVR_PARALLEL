/*
 *	linklist.c
 *
 *	Created on: 2022-03-28
 *	Author: 	WangJianLong
 */

#include "linklist.h"

/*******************************************************************************
* ��������: ��������ͷ
* �������: ��������Ĵ�С
* �������: ͷ�ڵ�
* ����˵��: ��
*******************************************************************************/
struct DOUBLELINKLIST* LinklistCareate(int Size)
{
	struct DOUBLELINKLIST *LinklistHead;
	
	LinklistHead = malloc(sizeof(*LinklistHead));//��������ͷ
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
* ��������: ���ݲ�������
* �������: ����ͷ��������������ݡ����뷽ʽ���ײ�ģʽ/β��ģʽ��
* �������: 
* ����˵��: ��
*******************************************************************************/
uint16_t DataInsertLinklist(struct DOUBLELINKLIST* LinklistHead, const void *Data, uint16_t Mode)
{
	struct DOUBLELINKLIST* Linklist = LinklistHead;
	struct DOUBLELINKLISTNODE *NewNode;
	
	//����ڵ�
	NewNode = malloc(sizeof(NewNode));
	if(NewNode == NULL)
		return MALLOCFAIL;
	
	//�������ݿռ�
	NewNode->Data = malloc(sizeof(Linklist->DataSize));
	if(NewNode->Data == NULL)
		return MALLOCFAIL;
	
	//�����������copy������ڵ�����ݿռ���
	memcpy(NewNode->Data, Data, Linklist->DataSize);
	
	//����ѡ��Ĳ��뷽ʽ ������Ľڵ��������
	switch(Mode)
	{
		case FirstMode: //�ײ�����
			NewNode->Prev = Linklist->HeadNode;
			NewNode->Next= Linklist->HeadNode->Next;
			break;
		case BackMode: 	//β������
			NewNode->Prev = Linklist->HeadNode->Prev;
			NewNode->Next = Linklist->HeadNode;
			break;
		default:		//�ײ�����
			NewNode->Prev = Linklist->HeadNode;
			NewNode->Next= Linklist->HeadNode->Next;
			break;
	}
	NewNode->Prev->Next = NewNode;
	NewNode->Next->Prev = NewNode;
	
	return INSERTSUCCESS;
}

/*******************************************************************************
* ��������: 	ȡ����������
* �������: ����ͷ����ȡ���ݷ�ʽ���ײ�/β����
* �������: ��
* ����˵��: ��
*******************************************************************************/
uint16_t FetchLinklistData(struct DOUBLELINKLIST* LinklistHead, void *SaveData, uint16_t Mode)
{
	struct DOUBLELINKLIST* Linklist = LinklistHead;
	struct DOUBLELINKLISTNODE *NewNode;
	uint16_t ret = 0;
	
	//�ж������Ƿ�Ϊ��
	ret = EmptyCheak(Linklist);
	if(ret == LLISTEMPTY)
		return LLISTNOEMPTY;
	
	//����ѡ��ȡ���ķ�ʽ ��������ȡ���ڵ�
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
	
	//��ȡ���ڵ������������copy��Ҫ����Ľṹ����
	memcpy(SaveData, NewNode->Data, Linklist->DataSize);
	
	//�ͷŽڵ�
	free(NewNode->Data);
	free(NewNode);
	
	return FETCHSUCCESS;
}

/*******************************************************************************
* ��������: �ж������Ƿ�Ϊ��
* �������: 
* �������: ����Ϊ��/����ǿ�
* ����˵��: ��
*******************************************************************************/
uint16_t EmptyCheak(struct DOUBLELINKLIST* LinklistHead)
{
	struct DOUBLELINKLIST *Linklist = LinklistHead;
	
	if(Linklist->HeadNode->Next == Linklist->HeadNode)
		return LLISTEMPTY;
	return LLISTNOEMPTY;
}

