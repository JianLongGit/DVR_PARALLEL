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

//������
struct LINKLISTDATATYPE
{
	uint16_t *DataAddr[3];	//������������ݵ��׵�ַ/���ꣻ
	union
	{
		uint16_t All;
		struct
		{
			uint16_t NeedSendLen: 12; //��Ҫ���͵����ݳ���
			uint16_t InverseFlag: 1;  //�Ƿ���
			uint16_t CommandFlag: 2;  //����/����/����
			uint16_t Reserve: 1;
		}Bits;
	}CtrlInfor;
};

//����ڵ�
struct DOUBLELINKLISTNODE{
    struct LINKLISTDATATYPE Data;		//������
	struct DOUBLELINKLISTNODE *Next;	//ָ����һ���ڵ��ָ��
	struct DOUBLELINKLISTNODE *Prev;	//ָ����һ���ڵ��ָ��
};

//����/ȡ������ʱ���ܳ��ֵ�״̬
enum{
	LLISTEMPTY = 0,	//�����
	LLISTNOEMPTY,	//����ǿ�
	MALLOCFAIL,		//�ڴ�����ʧ��
	INSERTSUCCESS,	//�ڵ����ɹ�
	FETCHSUCCESS,	//�ڵ�ȡ���ɹ�
};

extern struct DOUBLELINKLISTNODE* LinklistCareate(void);
extern uint16_t DataInsertLinklist(struct DOUBLELINKLISTNODE* LinklistHead, struct LINKLISTDATATYPE* Data);
extern uint16_t FetchLinklistData(struct DOUBLELINKLISTNODE* LinklistHead, struct LINKLISTDATATYPE* SaveData);
extern uint16_t EmptyCheak(struct DOUBLELINKLISTNODE* LinklistHead);

#endif
