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

//�ڵ�
struct DOUBLELINKLISTNODE{
    void *Data;							//������ָ��
	struct DOUBLELINKLISTNODE *Next;	//ָ����һ���ڵ��ָ��
	struct DOUBLELINKLISTNODE *Prev;	//ָ����һ���ڵ��ָ��
};

//����ͷ
struct DOUBLELINKLIST{
    uint16_t DataSize;						//��������С
	struct DOUBLELINKLISTNODE *HeadNode;	//ͷ�ڵ�
};

//���ݲ���/ȡ����ʽ
enum{
	FirstMode = 0,
	BackMode,
};

//����/ȡ������ʱ���ܳ��ֵ�״̬
enum{
	LLISTEMPTY = 0,	//�����
	LLISTNOEMPTY,	//����ǿ�
	MALLOCFAIL,		//�ڴ�����ʧ��
	INSERTSUCCESS,	//�ڵ����ɹ�
	FETCHSUCCESS,	//�ڵ�ȡ���ɹ�
};

extern struct DOUBLELINKLIST *LinklistCareate(int size);
extern uint16_t DataInsertLinklist(struct DOUBLELINKLIST* LinklistHead, const void *Data, uint16_t Mode);
extern uint16_t FetchLinklistData(struct DOUBLELINKLIST* LinklistHead, void *SaveData, uint16_t Mode);
extern uint16_t EmptyCheak(struct DOUBLELINKLIST* LinklistHead);

#endif
