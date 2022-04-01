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

struct LINKLISTDATATYPE
{
	uint16_t *DataAddr[3];
	union
	{
		uint16_t All;
		struct
		{
			uint16_t NeedSendLen: 12;
			uint16_t InverseFlag: 1;
			uint16_t CommandFlag: 2;
			uint16_t Reserve: 1;
		}Bits;
	}CtrlInfor;
};

struct DOUBLELINKLIST
{
	struct LINKLISTDATATYPE Data;
	struct DOUBLELINKLIST *Next;
	struct DOUBLELINKLIST *Prev;
};

extern void DataInsertLinklist(struct DOUBLELINKLIST* LinklistHead, uint16_t *Point, uint16_t DataLen, uint16_t InverseFlag, uint16_t CommandFlag);
extern struct DOUBLELINKLIST* FetchLinklistData(struct DOUBLELINKLIST* LinklistHead);
extern void FetchLinklistData1(struct DOUBLELINKLIST** SaveNode, struct DOUBLELINKLIST* LinklistHead);
extern uint16_t EmptyCheak(struct DOUBLELINKLIST* LinklistHead);
extern struct DOUBLELINKLIST* list_create(void);

#endif
