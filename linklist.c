/*
 *	linklist.c
 *
 *	Created on: 2022-03-28
 *	Author: 	WangJianLong
 */

#include "linklist.h"
#include "oledindex.h"

/*******************************************************************************
* ��������: 	����ͷ�ڵ�
* �������: 
* �������: ��
* ����˵��: ��
*******************************************************************************/
struct DOUBLELINKLIST* list_create(void)
{
	struct DOUBLELINKLIST *Head;
	
	Head = malloc(sizeof(*Head));
	if (Head == NULL)
	{
		return NULL;
	}

	Head->Data.CtrlInfor.All = 0;
	Head->Data.DataAddr[0] = NULL;
	Head->Data.DataAddr[1] = NULL;
	Head->Data.DataAddr[2] = NULL;
	Head->Next = Head;
	Head->Prev = Head;

	return Head;
}

/*******************************************************************************
* ��������: 	���ݲ��������ײ�
* �������: 
* �������: ��
* ����˵��: ��
*******************************************************************************/
void DataInsertLinklist(struct DOUBLELINKLIST* LinklistHead, uint16_t *Point, uint16_t DataLen, uint16_t InverseFlag, uint16_t CommandFlag)
{
	struct DOUBLELINKLIST *Head = LinklistHead;
	struct DOUBLELINKLIST *NewNode;
	
	//�����½ڵ�
	NewNode = malloc(sizeof(*NewNode));
	if (NewNode == NULL)
	{
		OledCtrl.Flag.Bits.OledMallocFaultFlag = 1;
		return;
	}

	//���ýڵ�����
	if(CommandFlag == 1)
	{
		NewNode->Data.DataAddr[0] = (uint16_t *)(*Point);
		NewNode->Data.DataAddr[1] = (uint16_t *)(*(Point+1));
		NewNode->Data.DataAddr[2] = (uint16_t *)(*(Point+2));
	}
	else
	{
		NewNode->Data.DataAddr[0] = Point;
	}

	NewNode->Data.CtrlInfor.Bits.NeedSendLen = DataLen;
	NewNode->Data.CtrlInfor.Bits.InverseFlag = InverseFlag;
	NewNode->Data.CtrlInfor.Bits.CommandFlag = CommandFlag;

	//�ڵ��������
	NewNode->Next = Head->Next;
	NewNode->Prev = Head;
	Head->Next->Prev = NewNode;
	Head->Next = NewNode;

	return;
}

/*******************************************************************************
* ��������: 	ȡ������β������
* �������: 
* �������: ��
* ����˵��: ��
*******************************************************************************/
struct DOUBLELINKLIST* FetchLinklistData(struct DOUBLELINKLIST* LinklistHead)
{
	struct DOUBLELINKLIST *Head = LinklistHead;
	struct DOUBLELINKLIST *Node;
	struct DOUBLELINKLIST *NodePrev;
	
	Node = Head->Prev;
	NodePrev = Node->Prev;
	NodePrev->Next = Head;
	Head->Prev = NodePrev;
	
	return Node;
}

void FetchLinklistData1(struct DOUBLELINKLIST** SaveNode, struct DOUBLELINKLIST* LinklistHead)
{
	struct DOUBLELINKLIST *Head = LinklistHead;

	*SaveNode = (Head->Prev);
	Head->Prev = Head->Prev->Prev;
	Head->Prev->Next = Head;
	
	return;
}

/*******************************************************************************
* ��������: �ж������Ƿ�Ϊ��
* �������: 
* �������: 1������Ϊ��		0������ǿ�
* ����˵��: ��
*******************************************************************************/
uint16_t EmptyCheak(struct DOUBLELINKLIST* LinklistHead)
{
	struct DOUBLELINKLIST *Head = LinklistHead;
	
	if(Head->Next == Head)
		return 1;
	return 0;
}

