/*
 *	linklist.c
 *
 *	Created on: 2022-03-28
 *	Author: 	WangJianLong
 */

#include "linklist.h"

/*******************************************************************************
* ��������: ��������ڵ�
* �������: 
* �������: ͷ�ڵ���׵�ַ
* ����˵��: 
*******************************************************************************/
struct DOUBLELINKLISTNODE* LinklistCareate(void)
{
	struct DOUBLELINKLISTNODE* LinklistHead;
	
	LinklistHead = malloc(sizeof(*LinklistHead));//��������ͷ
	if (LinklistHead == NULL)
	{
		return NULL;
	}

	LinklistHead->Next = LinklistHead;
	LinklistHead->Prev = LinklistHead;

	return LinklistHead;
}

/*******************************************************************************
* ��������: ���ݲ�������
* �������: *LinklistHead������ͷ���׵�ַ��*Data������������ݡ����뷽ʽ���ײ�ģʽ/β��ģʽ��
* �������: 
* ����˵��: ��
*******************************************************************************/
uint16_t DataInsertLinklist(struct DOUBLELINKLISTNODE* LinklistHead, struct LINKLISTDATATYPE* Data)
{
	struct DOUBLELINKLISTNODE* Head = LinklistHead;
	struct DOUBLELINKLISTNODE* NewNode = NULL;
	
	//����ڵ�
	NewNode = (struct DOUBLELINKLISTNODE*)malloc(sizeof(*NewNode));
	
	if(NewNode == NULL)
		return MALLOCFAIL;
	
	//�����������copy������ڵ�����ݿռ���
	NewNode->Data = (*Data);
	
	//������Ľڵ���������ײ�
	NewNode->Prev = Head;
	NewNode->Next= Head->Next;
	NewNode->Prev->Next = NewNode;
	NewNode->Next->Prev = NewNode;
	
	return INSERTSUCCESS;
}

/*******************************************************************************
* ��������: ȡ����������
* �������: ����ͷ����ȡ���ݷ�ʽ���ײ�/β����
* �������: ��
* ����˵��: ��
*******************************************************************************/
uint16_t FetchLinklistData(struct DOUBLELINKLISTNODE* LinklistHead, struct LINKLISTDATATYPE* SaveData)
{
	struct DOUBLELINKLISTNODE* Head = LinklistHead;
	struct DOUBLELINKLISTNODE* NewNode;
	uint16_t ret = 0;
	
	//�ж������Ƿ�Ϊ��
	ret = EmptyCheak(Head);
	if(ret == LLISTEMPTY)
		return LLISTEMPTY;
	
	//����ѡ��ȡ���ķ�ʽ ��������ȡ���ڵ�
	NewNode = Head->Prev;
	Head->Prev = NewNode->Prev;
	Head->Prev->Next = Head;
	
	//��ȡ���Ľڵ�����copy��Ҫ��������ݿռ���
	(*SaveData) = NewNode->Data;
	
	//�ͷŽڵ�
	free(NewNode);
	
	return FETCHSUCCESS;
}

/*******************************************************************************
* ��������: �ж������Ƿ�Ϊ��
* �������: 
* �������: ����Ϊ��/����ǿ�
* ����˵��: ��
*******************************************************************************/
uint16_t EmptyCheak(struct DOUBLELINKLISTNODE* LinklistHead)
{
	struct DOUBLELINKLISTNODE* Head = LinklistHead;
	
	if(Head->Next == Head)
		return LLISTEMPTY;
	return LLISTNOEMPTY;
}

