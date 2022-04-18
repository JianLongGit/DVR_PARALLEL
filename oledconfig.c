/*
 * oledconfig.c
 *
 *  Created on: 2022-04-18
 *      Author: WangJianLong
 */
 
#include "oledfont.h" 
#include "oledconfig.h"

uint16_t OledExternalDataArr[39] = {220,221,222,100,101,102,2,3,0x0001,0x0000,0x0000,0x0004,0x0005,0x0006,0x0007,0x0008,0x0009,0x000A,0x1111,0x0000,0x0000,0x4444,0x5555,0x6666,0x7777,0x8888,0x9999,0xaaaa,0x5555,10,0x0f,0x7E54,0x010c,0x1427,31,0x7E64,0x010c,0x1427,0xffff}; //OLED��Ҫ���ⲿ����					

struct OLEDCTRL OledCtrl;
struct ITEMSTRUCT ItemTable[] = {0};	
struct OLEDPAGESTRUCT OledPageTable[] = {0};

/*******************************************************************************
* ��������:	�����������
* �������: x:�����꣬y:ҳ��ַ
* �������: ��
* ����˵��: ��
*******************************************************************************/
void InsertLocation(uint16_t x, uint16_t y)
{
	struct LINKLISTDATATYPE Data;
	uint16_t ret = 0;
	
	//������ֵ������������ָ��������
	Data.DataAddr[0] = (uint16_t *)(0xb0+y);
	Data.DataAddr[1] = (uint16_t *)(((x&0xf0)>>4)|0x10);
	Data.DataAddr[2] = (uint16_t *)(x&0x0f);
	
	//���ýڵ�Ŀ�����Ϣ
	Data.CtrlInfor.Bits.CommandFlag = OledLocation;
	Data.CtrlInfor.Bits.InverseFlag = NoNeedInverse;
	Data.CtrlInfor.Bits.NeedSendLen = 3;
	
	//�ڵ��������
	ret = DataInsertLinklist(OledCtrl.LinklistHead, &Data);
	if(ret == MALLOCFAIL)
	{
		OledCtrl.Flag.Bits.OledFaultFlag = 1;
	}
}


/*******************************************************************************
* ��������:	�ַ����������
* �������: *Point:�ַ��ֿ��׵�ַ�� InverseFlag:���Ա�־�� Command������/����/���ꣻ Sendlen�����͵ĳ���
* �������: ��
* ����˵��: ��
*******************************************************************************/
void InsertFontLib(uint16_t *Point, uint16_t InverseFlag, uint16_t Command, uint16_t Sendlen)
{
	struct LINKLISTDATATYPE Data;
	uint16_t ret = 0;
	
	//���������ֿ���׵�ַ������������ָ��������
	Data.DataAddr[0] = Point;
	
	//���ýڵ�Ŀ�����Ϣ
	Data.CtrlInfor.Bits.CommandFlag = Command;
	Data.CtrlInfor.Bits.InverseFlag = InverseFlag;
	Data.CtrlInfor.Bits.NeedSendLen = Sendlen;
	
	//�ڵ��������
	ret = DataInsertLinklist(OledCtrl.LinklistHead, &Data);
	if(ret == MALLOCFAIL)
	{
		OledCtrl.Flag.Bits.OledFaultFlag = 1;
	}
}


/*******************************************************************************
* ��������:	�ַ���������
* �������: x:�����꣬y:ҳ��ַ, *Point:�ַ��ֿ��׵�ַ�� InverseFlag:���Ա�־
* �������: ��
* ����˵��: ����ʼλ��(x,y)����ʾ�ַ�
*******************************************************************************/
void SetDisplayChar(uint16_t x, uint16_t y, uint16_t *Point, uint16_t InverseFlag)
{
	//�ַ����ֿ�ǰ�˸����ݵ�����������
	InsertLocation(x,y);
	
	//�ַ����ֿ�ǰ�˸����ݵ��׵�ַ������
	InsertFontLib(Point,InverseFlag,OledData,8);
	
	//�ַ����ֿ��˸����ݵ�����������
	InsertLocation(x,y+1);
	
	//�ַ����ֿ��˸����ݵ��׵�ַ������
	InsertFontLib(Point+8,InverseFlag,OledData,8);
}

/*******************************************************************************
* ��������:	��������
* �������: 
* �������: ��
* ����˵��: 
*******************************************************************************/
void ClearScreen(void)
{
	uint16_t i=0;
	
	InsertLocation(0,0);
	
	for(i=0; i<=8; i++)
	{
		InsertFontLib(OledClearScreenArr, NoNeedInverse, OledData, 128);
	}
}

/*******************************************************************************
* ��������:	OLED��ʼ������
* �������: 
* �������: ��
* ����˵��: ��
*******************************************************************************/
void OledInitCommand(void)
{
	InsertFontLib(OledCommandInitArr, NoNeedInverse, OledCommand, 28);
}

/*******************************************************************************
* ��������:	ҳ���������ʼ��
* �������: 
* �������: ��
* ����˵��: ��
*******************************************************************************/
void PageIndexTableInit(void)
{
	//����ҳ���������и�ҳ��Ŀɷ�ѡ��Ŀ��
	if(OledExternalDataArr[29] > 6)
	{
		PageIndexTable[ModuleSeletionPage1Index].PageInforCtrl.Bits.InverseItemNum = PageIndexTable[ModuleSeletionPage1Index].PageInforCtrl.Bits.ItemNum-1;
		PageIndexTable[ModuleSeletionPage2Index].PageInforCtrl.Bits.InverseItemNum = (OledExternalDataArr[29]-6);
	}
	else
	{
		PageIndexTable[ModuleSeletionPage1Index].PageInforCtrl.Bits.InverseItemNum = OledExternalDataArr[29]+1;
		PageIndexTable[ModuleSeletionPage1Index].PageIndex.Bits.CurrentPageDownIndex = ResetPage1Index;
		PageIndexTable[ResetPage1Index].PageIndex.Bits.CurrentPageUpIndex = ModuleSeletionPage1Index;
	}
	
	if(OledExternalDataArr[29] > 8)
	{
		PageIndexTable[ResetPage1Index].PageInforCtrl.Bits.InverseItemNum = PageIndexTable[ResetPage1Index].PageInforCtrl.Bits.ItemNum-1;
		PageIndexTable[ResetPage2Index].PageInforCtrl.Bits.InverseItemNum = (OledExternalDataArr[29]-8);
	}
	else
	{
		PageIndexTable[ResetPage1Index].PageInforCtrl.Bits.InverseItemNum = OledExternalDataArr[29]+1;
		PageIndexTable[ResetPage1Index].PageIndex.Bits.CurrentPageDownIndex = ResetPage1Index;
	}
}

/*******************************************************************************
* ��������:	OLED��ʼ������
* �������: 
* �������: ��
* ����˵��: ��
*******************************************************************************/
void OledInit(void)
{	
	memset(&OledCtrl, 0, sizeof(OledCtrl));
	
	//��������ͷ
	OledCtrl.LinklistHead = LinklistCareate();
	
	//ҳ���������ʼ��
	PageIndexTableInit();
	
	//��ʱ����ֵ ��ʼ��Ϊ ���ϼ�
	OledCtrl.KeyCtrl.KEYSTATE.Bits.TempKeyValue = KEY_UP_VALUE;
	
	//��ʼ��ҳ����ƽṹ��
	OledCtrl.PageCtrl.PageIndex.Bits.CurrentIndex = HomePageIndex;
	OledCtrl.PageCtrl.PageIndex.Bits.LastIndex = HomePageIndex;
	OledCtrl.PageCtrl.InverseLocation = 1;
	OledCtrl.PageCtrl.SendCompleteLen = 0xfff;
	
	//��ʼ��OLED��־
	OledCtrl.Flag.Bits.UpdateDisplayFlag = 1;
	OledCtrl.Flag.Bits.AllUpdateFlag = 1;
	
	//OLED��ʼ������
	OledInitCommand();
	
	//OLED����
	ClearScreen();//����
}

/*******************************************************************************
* ��������:	���ֲ�������
* �������: x:�����꣬y:ҳ��ַ��Digit:Ҫ��ʾ������(ʮ����)��BitNum:Ҫ��ʾ��λ��
* �������: ��
* ����˵��: ��
*******************************************************************************/
void DisplayDigit(uint16_t x, uint16_t y, uint16_t Digit, uint16_t BitNum)
{
	uint16_t i = 0;
	uint16_t DightArr[5];
	uint16_t (*ptr)[16] = FontLib_Dight;
	
	//��λ���� ���λ������DightArr[4]�����Ϊ������DightArr[0]��
	DightArr[0] = (Digit%10);
	DightArr[1] = ((Digit/10)%10);
	DightArr[2] = (Digit/100)%10;
	DightArr[3] = (Digit/1000)%10;
	DightArr[4] = (Digit/10000);
	
	//����Ҫ��ʾ��λ������λ��ʾ��
	for(i=0; i<BitNum; i++)
	{
		SetDisplayChar(x+8*i, y, *(ptr+DightArr[BitNum-i-1]), NoNeedInverse);
	}
}

///*******************************************************************************
//* ��������: ���ֲ�������
//* �������: 
//* �������: ��
//* ����˵��: ʹ�ö��ֲ��ҷ�
//*******************************************************************************/
//void DisplayDigit(uint16_t x, uint16_t y, uint16_t Digit, uint16_t BitNum)
//{
//	uint16_t i = 0;
//	uint16_t DightArr[5];
//	uint16_t temp = Digit;
//	//�������λ
//	if(temp>=30000)
//	{
//		temp-=30000;
//		for(i=0;temp>=10000;i++)
//		{
//			temp-=10000;

//		}
//		DightArr[4] = i+3;
//	}
//	else
//	{
//		for(i=0;temp>=10000;i++)
//		{
//			temp-=10000;
//		}
//		DightArr[4] = i;
//	}
//	
//	//�����ǧλ
//	if(temp>=5000)
//	{
//		temp-=5000;
//		for(i=0;temp>=1000;i++)
//		{
//			temp-=1000;

//		}
//		DightArr[3] = i+5;
//	}
//	else
//	{
//		for(i=0;temp>=1000;i++)
//		{
//			temp-=1000;
//		}
//		DightArr[3] = i;
//	}
//	
//	//�������λ
//	if(temp>=500)
//	{
//		temp-=500;
//		for(i=0;temp>=100;i++)
//		{
//			temp-=100;

//		}
//		DightArr[2] = i+5;
//	}
//	else
//	{
//		for(i=0;temp>=100;i++)
//		{
//			temp-=100;
//		}
//		DightArr[2] = i;
//	}
//	
//	//�����ʮλ
//	if(Digit>=50)
//	{
//		temp-=50;
//		for(i=0;temp>=10;i++)
//		{
//			temp-=10;

//		}
//		DightArr[1] = i+5;
//	}
//	else
//	{
//		for(i=0;temp>=10;i++)
//		{
//			temp-=10;
//		}
//		DightArr[1] = i;
//	}
//	
//	//�������λ
//	if(temp>=5)
//	{
//		temp-=5;
//		for(i=0;temp>=1;i++)
//		{
//			temp-=1;

//		}
//		DightArr[0] = i+5;
//	}
//	else
//	{
//		for(i=0;temp>=1;i++)
//		{
//			temp-=1;
//		}
//		DightArr[0] = i;
//	}
//	
//	
//	for(i=0; i<BitNum; i++)
//	{
//		SetDisplayChar(x+8*i, y, FontLib_Dight[0]+DightArr[BitNum-i-1], NoNeedInverse);
//	}
//}

/*******************************************************************************
* ��������:	��ҳ�ĸ���Ŀ��ʾ����
* �������: 
* �������: ��
* ����˵��: ��
*******************************************************************************/
void SUAItemDisplay(void)
{
	if(OledCtrl.Flag.Bits.AllUpdateFlag == 1)
	{
		SetDisplayChar(0, 0, FontLib_S, 0);
		SetDisplayChar(8, 0, FontLib_U, 0);
		SetDisplayChar(16, 0, FontLib_A, 0);
		SetDisplayChar(24, 0, FontLib_Colon, 0);
	}
	DisplayDigit(32, 0, OledExternalDataArr[0], 3);
}

void SUBItemDisplay(void)
{
	if(OledCtrl.Flag.Bits.AllUpdateFlag == 1)
	{
		SetDisplayChar(0, 2, FontLib_S, 0);
		SetDisplayChar(8, 2, FontLib_U, 0);
		SetDisplayChar(16, 2, FontLib_B, 0);
		SetDisplayChar(24, 2, FontLib_Colon, 0);
	}
	DisplayDigit(32, 2, OledExternalDataArr[1], 3);
}

void SUCItemDisplay(void)
{
	if(OledCtrl.Flag.Bits.AllUpdateFlag == 1)
	{
		SetDisplayChar(0, 4, FontLib_S, 0);
		SetDisplayChar(8, 4, FontLib_U, 0);
		SetDisplayChar(16, 4, FontLib_C, 0);
		SetDisplayChar(24, 4, FontLib_Colon, 0);
	}
	DisplayDigit(32, 4, OledExternalDataArr[2], 3);
}

void SIAItemDisplay(void)
{
	if(OledCtrl.Flag.Bits.AllUpdateFlag == 1)
	{
		SetDisplayChar(64, 0, FontLib_S, 0);
		SetDisplayChar(72, 0, FontLib_I, 0);
		SetDisplayChar(80, 0, FontLib_A, 0);
		SetDisplayChar(88, 0, FontLib_Colon, 0);
	}
	DisplayDigit(96, 0, OledExternalDataArr[3], 3);
}

void SIBItemDisplay(void)
{
	if(OledCtrl.Flag.Bits.AllUpdateFlag == 1)
	{
		SetDisplayChar(64, 2, FontLib_S, 0);
		SetDisplayChar(72, 2, FontLib_I, 0);
		SetDisplayChar(80, 2, FontLib_B, 0);
		SetDisplayChar(88, 2, FontLib_Colon, 0);
	}
	DisplayDigit(96, 2, OledExternalDataArr[4], 3);
}

void SICItemDisplay(void)
{
	if(OledCtrl.Flag.Bits.AllUpdateFlag == 1)
	{
		SetDisplayChar(64, 4, FontLib_S, 0);
		SetDisplayChar(72, 4, FontLib_I, 0);
		SetDisplayChar(80, 4, FontLib_C, 0);
		SetDisplayChar(88, 4, FontLib_Colon, 0);
	}
	DisplayDigit(96, 4, OledExternalDataArr[5], 3);
}

void DSItemDisplay(void)
{
	if(OledCtrl.Flag.Bits.AllUpdateFlag == 1)
	{
		SetDisplayChar(8, 6, FontLib_D, 0);
		SetDisplayChar(16, 6, FontLib_S, 0);
		SetDisplayChar(24, 6, FontLib_Colon, 0);
	}

	//��ʾ�豸����״̬
	if(OledExternalDataArr[6] == 0x1)
	{
		SetDisplayChar(32, 6, FontLib_W, 0);
	}
	else if(OledExternalDataArr[6] == 0x2)
	{
		SetDisplayChar(32, 6, FontLib_R, 0);
	}
	else
	{
		SetDisplayChar(32, 6, FontLib_E, 0);
	}
	
	//����ģ�����ʱ��ʾ*��
	if(OledExternalDataArr[38])
	{
		SetDisplayChar(40, 6, FontLib_Asterisk, 0);
	}
	else
	{
		SetDisplayChar(40, 6, FontLib_Space, 0);
	}
}

void CSItemDisplay(void)
{
	if(OledCtrl.Flag.Bits.AllUpdateFlag == 1)
	{
		SetDisplayChar(72, 6, FontLib_C, 0);
		SetDisplayChar(80, 6, FontLib_S, 0);
		SetDisplayChar(88, 6, FontLib_Colon, 0);
	}
	//��ʾͨѶģ������״̬
	if(OledExternalDataArr[7] == 0x1)
	{
		SetDisplayChar(96, 6, FontLib_W, 0);
	}
	else if(OledExternalDataArr[7] == 0x2)
	{
		SetDisplayChar(96, 6, FontLib_R, 0);
	}
	else
	{
		SetDisplayChar(96, 6, FontLib_E, 0);
	}
	
	//ͨѶģ�����ʱ��ʾ*��
	if(OledExternalDataArr[28])
	{
		SetDisplayChar(104, 6, FontLib_Asterisk, 0);
	}
	else
	{
		SetDisplayChar(104, 6, FontLib_Space, 0);
	}
}

/*******************************************************************************
* ��������:	ģ��ѡ��ҳ1�ĸ���Ŀ��ʾ����
* �������: 
* �������: ��
* ����˵��: ��
*******************************************************************************/
void CMDisplay(void)
{
	SetDisplayChar(8, 2, FontLib_C, OledCtrl.PageCtrl.InverseLocation==1?NeedInverse:NoNeedInverse);
	SetDisplayChar(16, 2, FontLib_M, OledCtrl.PageCtrl.InverseLocation==1?NeedInverse:NoNeedInverse);
	SetDisplayChar(24, 2, FontLib_Space, OledCtrl.PageCtrl.InverseLocation==1?NeedInverse:NoNeedInverse);
	if(OledExternalDataArr[28])
	{
		SetDisplayChar(32, 2, FontLib_Asterisk, OledCtrl.PageCtrl.InverseLocation==1?NeedInverse:NoNeedInverse);
	}
}

void MSP1M01Display(void)
{
	if(OledExternalDataArr[29] >= 1)
	{
		SetDisplayChar(8, 4, FontLib_M, OledCtrl.PageCtrl.InverseLocation==2?NeedInverse:NoNeedInverse);
		SetDisplayChar(16, 4, &FontLib_Dight[0][0], OledCtrl.PageCtrl.InverseLocation==2?NeedInverse:NoNeedInverse);
		SetDisplayChar(24, 4, &FontLib_Dight[1][0], OledCtrl.PageCtrl.InverseLocation==2?NeedInverse:NoNeedInverse);
	
		if(OledExternalDataArr[8] | OledExternalDataArr[18])
		{
			SetDisplayChar(32, 4, FontLib_Asterisk, OledCtrl.PageCtrl.InverseLocation==2?NeedInverse:NoNeedInverse);
		}	
	}
}

void MSP1M02Display(void)
{
	if(OledExternalDataArr[29] >= 2)
	{
		SetDisplayChar(48, 4, FontLib_M, OledCtrl.PageCtrl.InverseLocation==3?NeedInverse:NoNeedInverse);
		SetDisplayChar(56, 4, &FontLib_Dight[0][0], OledCtrl.PageCtrl.InverseLocation==3?NeedInverse:NoNeedInverse);
		SetDisplayChar(64, 4, &FontLib_Dight[2][0], OledCtrl.PageCtrl.InverseLocation==3?NeedInverse:NoNeedInverse);
	
		if(OledExternalDataArr[9] | OledExternalDataArr[19])
		{
			SetDisplayChar(72, 4, FontLib_Asterisk, OledCtrl.PageCtrl.InverseLocation==3?NeedInverse:NoNeedInverse);
		}	
	}
}

void MSP1M03Display(void)
{
	if(OledExternalDataArr[29] >= 3)
	{
		SetDisplayChar(88, 4, FontLib_M, OledCtrl.PageCtrl.InverseLocation==4?NeedInverse:NoNeedInverse);
		SetDisplayChar(96, 4, &FontLib_Dight[0][0], OledCtrl.PageCtrl.InverseLocation==4?NeedInverse:NoNeedInverse);
		SetDisplayChar(104, 4, &FontLib_Dight[3][0], OledCtrl.PageCtrl.InverseLocation==4?NeedInverse:NoNeedInverse);
	
		if(OledExternalDataArr[10] | OledExternalDataArr[20])
		{
			SetDisplayChar(112, 4, FontLib_Asterisk, OledCtrl.PageCtrl.InverseLocation==4?NeedInverse:NoNeedInverse);
		}	
	}
}

void MSP1M04Display(void)
{
	if(OledExternalDataArr[29] >= 4)
	{
		SetDisplayChar(8, 6, FontLib_M, OledCtrl.PageCtrl.InverseLocation==5?NeedInverse:NoNeedInverse);
		SetDisplayChar(16, 6, &FontLib_Dight[0][0], OledCtrl.PageCtrl.InverseLocation==5?NeedInverse:NoNeedInverse);
		SetDisplayChar(24, 6, &FontLib_Dight[4][0], OledCtrl.PageCtrl.InverseLocation==5?NeedInverse:NoNeedInverse);

		if(OledExternalDataArr[11] | OledExternalDataArr[21])
		{
			SetDisplayChar(32, 6, FontLib_Asterisk, OledCtrl.PageCtrl.InverseLocation==5?NeedInverse:NoNeedInverse);
		}
	}
}

void MSP1M05Display(void)
{
	if(OledExternalDataArr[29] >= 5)
	{
		SetDisplayChar(48, 6, FontLib_M, OledCtrl.PageCtrl.InverseLocation==6?NeedInverse:NoNeedInverse);
		SetDisplayChar(56, 6, &FontLib_Dight[0][0], OledCtrl.PageCtrl.InverseLocation==6?NeedInverse:NoNeedInverse);
		SetDisplayChar(64, 6, &FontLib_Dight[5][0], OledCtrl.PageCtrl.InverseLocation==6?NeedInverse:NoNeedInverse);

		if(OledExternalDataArr[12] | OledExternalDataArr[22])
		{
			SetDisplayChar(72, 6, FontLib_Asterisk, OledCtrl.PageCtrl.InverseLocation==6?NeedInverse:NoNeedInverse);
		}		
	}
}

void MSP1M06Display(void)
{
	if(OledExternalDataArr[29] > 6)
	{
		SetDisplayChar(88, 6, FontLib_M, OledCtrl.PageCtrl.InverseLocation==7?NeedInverse:NoNeedInverse);
		SetDisplayChar(96, 6, &FontLib_Dight[0][0], OledCtrl.PageCtrl.InverseLocation==7?NeedInverse:NoNeedInverse);
		SetDisplayChar(104, 6, &FontLib_Dight[6][0], OledCtrl.PageCtrl.InverseLocation==7?NeedInverse:NoNeedInverse);
		
		if(OledExternalDataArr[13] | OledExternalDataArr[23])
		{
			SetDisplayChar(112, 6, FontLib_Asterisk, OledCtrl.PageCtrl.InverseLocation==7?NeedInverse:NoNeedInverse);
		}		
	}

}

void MODULEDisplay(void)
{
	SetDisplayChar(40, 0, FontLib_M, NoNeedInverse);
	SetDisplayChar(48, 0, FontLib_O, NoNeedInverse);
	SetDisplayChar(56, 0, FontLib_D, NoNeedInverse);
	SetDisplayChar(64, 0, FontLib_U, NoNeedInverse);
	SetDisplayChar(72, 0, FontLib_L, NoNeedInverse);
	SetDisplayChar(80, 0, FontLib_E, NoNeedInverse);
}

void MSPEnterFun(void)
{
	OledCtrl.PageCtrl.SelectModuleNumber = OledCtrl.PageCtrl.InverseLocation;
}
/*******************************************************************************
* ��������:	ģ��ѡ��ҳ2�ĸ���Ŀ��ʾ����
* �������: 
* �������: ��
* ����˵��: ��
*******************************************************************************/
void MSP1M07Display(void)
{
	if(OledExternalDataArr[29] >= 7)
	{
		SetDisplayChar(8, 2, FontLib_M, OledCtrl.PageCtrl.InverseLocation==1?NeedInverse:NoNeedInverse);
		SetDisplayChar(16, 2, &FontLib_Dight[0][0], OledCtrl.PageCtrl.InverseLocation==1?NeedInverse:NoNeedInverse);
		SetDisplayChar(24, 2, &FontLib_Dight[7][0], OledCtrl.PageCtrl.InverseLocation==1?NeedInverse:NoNeedInverse);
	
		if(OledExternalDataArr[14] | OledExternalDataArr[24])
		{
			SetDisplayChar(32, 2, FontLib_Asterisk, OledCtrl.PageCtrl.InverseLocation==1?NeedInverse:NoNeedInverse);
		}	
	}
}

void MSP1M08Display(void)
{
	if(OledExternalDataArr[29] >= 8)
	{
		SetDisplayChar(48, 2, FontLib_M, OledCtrl.PageCtrl.InverseLocation==2?NeedInverse:NoNeedInverse);
		SetDisplayChar(56, 2, &FontLib_Dight[0][0], OledCtrl.PageCtrl.InverseLocation==2?NeedInverse:NoNeedInverse);
		SetDisplayChar(64, 2, &FontLib_Dight[8][0], OledCtrl.PageCtrl.InverseLocation==2?NeedInverse:NoNeedInverse);
	
		if(OledExternalDataArr[15] | OledExternalDataArr[25])
		{
			SetDisplayChar(72, 2, FontLib_Asterisk, OledCtrl.PageCtrl.InverseLocation==2?NeedInverse:NoNeedInverse);
		}	
	}
}

void MSP1M09Display(void)
{
	if(OledExternalDataArr[29] >= 9)
	{
		SetDisplayChar(88, 2, FontLib_M, OledCtrl.PageCtrl.InverseLocation==3?NeedInverse:NoNeedInverse);
		SetDisplayChar(96, 2, &FontLib_Dight[0][0], OledCtrl.PageCtrl.InverseLocation==3?NeedInverse:NoNeedInverse);
		SetDisplayChar(104, 2, &FontLib_Dight[9][0], OledCtrl.PageCtrl.InverseLocation==3?NeedInverse:NoNeedInverse);
			
		if(OledExternalDataArr[16] | OledExternalDataArr[26])
		{
			SetDisplayChar(112, 2, FontLib_Asterisk, OledCtrl.PageCtrl.InverseLocation==3?NeedInverse:NoNeedInverse);
		}
	}
}

void MSP1M10Display(void)
{
	if(OledExternalDataArr[29] >= 10)
	{
		SetDisplayChar(8, 4, FontLib_M, OledCtrl.PageCtrl.InverseLocation==4?NeedInverse:NoNeedInverse);
		SetDisplayChar(16, 4, &FontLib_Dight[1][0], OledCtrl.PageCtrl.InverseLocation==4?NeedInverse:NoNeedInverse);
		SetDisplayChar(24, 4, &FontLib_Dight[0][0], OledCtrl.PageCtrl.InverseLocation==4?NeedInverse:NoNeedInverse);
	
		if(OledExternalDataArr[17] | OledExternalDataArr[27])
		{
			SetDisplayChar(32, 4, FontLib_Asterisk, OledCtrl.PageCtrl.InverseLocation==4?NeedInverse:NoNeedInverse);
		}
	}
}

/*******************************************************************************
* ��������:	��λҳ1�ĸ���Ŀ��ʾ����
* �������: 
* �������: ��
* ����˵��: ��
*******************************************************************************/
void ALLDisplay(void)
{
	SetDisplayChar(16, 2, FontLib_A, OledCtrl.PageCtrl.InverseLocation==1?NeedInverse:NoNeedInverse);
	SetDisplayChar(24, 2, FontLib_L, OledCtrl.PageCtrl.InverseLocation==1?NeedInverse:NoNeedInverse);
	SetDisplayChar(32, 2, FontLib_L, OledCtrl.PageCtrl.InverseLocation==1?NeedInverse:NoNeedInverse);
}

void MRP1M01Display(void)
{
	if(OledExternalDataArr[29] >= 1)
	{
		SetDisplayChar(48, 2, FontLib_M, OledCtrl.PageCtrl.InverseLocation==2?NeedInverse:NoNeedInverse);
		SetDisplayChar(56, 2, &FontLib_Dight[0][0], OledCtrl.PageCtrl.InverseLocation==2?NeedInverse:NoNeedInverse);
		SetDisplayChar(64, 2, &FontLib_Dight[1][0], OledCtrl.PageCtrl.InverseLocation==2?NeedInverse:NoNeedInverse);
	}
}

void MRP1M02Display(void)
{
	if(OledExternalDataArr[29] >= 2)
	{
		SetDisplayChar(80, 2, FontLib_M, OledCtrl.PageCtrl.InverseLocation==3?NeedInverse:NoNeedInverse);
		SetDisplayChar(88, 2, &FontLib_Dight[0][0], OledCtrl.PageCtrl.InverseLocation==3?NeedInverse:NoNeedInverse);
		SetDisplayChar(96, 2, &FontLib_Dight[2][0], OledCtrl.PageCtrl.InverseLocation==3?NeedInverse:NoNeedInverse);
	}
}

void MRP1M03Display(void)
{
	if(OledExternalDataArr[29] >= 3)
	{
		SetDisplayChar(16, 4, FontLib_M, OledCtrl.PageCtrl.InverseLocation==4?NeedInverse:NoNeedInverse);
		SetDisplayChar(24, 4, &FontLib_Dight[0][0], OledCtrl.PageCtrl.InverseLocation==4?NeedInverse:NoNeedInverse);
		SetDisplayChar(32, 4, &FontLib_Dight[3][0], OledCtrl.PageCtrl.InverseLocation==4?NeedInverse:NoNeedInverse);
	}
}

void MRP1M04Display(void)
{
	if(OledExternalDataArr[29] >= 4)
	{
		SetDisplayChar(48, 4, FontLib_M, OledCtrl.PageCtrl.InverseLocation==5?NeedInverse:NoNeedInverse);
		SetDisplayChar(56, 4, &FontLib_Dight[0][0], OledCtrl.PageCtrl.InverseLocation==5?NeedInverse:NoNeedInverse);
		SetDisplayChar(64, 4, &FontLib_Dight[4][0], OledCtrl.PageCtrl.InverseLocation==5?NeedInverse:NoNeedInverse);
	}
}

void MRP1M05Display(void)
{
	if(OledExternalDataArr[29] >= 5)
	{
		SetDisplayChar(80, 4, FontLib_M, OledCtrl.PageCtrl.InverseLocation==6?NeedInverse:NoNeedInverse);
		SetDisplayChar(88, 4, &FontLib_Dight[0][0], OledCtrl.PageCtrl.InverseLocation==6?NeedInverse:NoNeedInverse);
		SetDisplayChar(96, 4, &FontLib_Dight[5][0], OledCtrl.PageCtrl.InverseLocation==6?NeedInverse:NoNeedInverse);
	}
}

void MRP1M06Display(void)
{
	if(OledExternalDataArr[29] > 6)
	{
		SetDisplayChar(16, 6, FontLib_M, OledCtrl.PageCtrl.InverseLocation==7?NeedInverse:NoNeedInverse);
		SetDisplayChar(24, 6, &FontLib_Dight[0][0], OledCtrl.PageCtrl.InverseLocation==7?NeedInverse:NoNeedInverse);
		SetDisplayChar(32, 6, &FontLib_Dight[6][0], OledCtrl.PageCtrl.InverseLocation==7?NeedInverse:NoNeedInverse);
	}
}

void MRP1M07Display(void)
{
	if(OledExternalDataArr[29] >= 7)
	{
		SetDisplayChar(48, 6, FontLib_M, OledCtrl.PageCtrl.InverseLocation==8?NeedInverse:NoNeedInverse);
		SetDisplayChar(56, 6, &FontLib_Dight[0][0], OledCtrl.PageCtrl.InverseLocation==8?NeedInverse:NoNeedInverse);
		SetDisplayChar(64, 6, &FontLib_Dight[7][0], OledCtrl.PageCtrl.InverseLocation==8?NeedInverse:NoNeedInverse);
	}
}

void MRP1M08Display(void)
{
	if(OledExternalDataArr[29] >= 8)
	{
		SetDisplayChar(80, 6, FontLib_M, OledCtrl.PageCtrl.InverseLocation==9?NeedInverse:NoNeedInverse);
		SetDisplayChar(88, 6, &FontLib_Dight[0][0], OledCtrl.PageCtrl.InverseLocation==9?NeedInverse:NoNeedInverse);
		SetDisplayChar(96, 6, &FontLib_Dight[8][0], OledCtrl.PageCtrl.InverseLocation==9?NeedInverse:NoNeedInverse);
	}
}

void RESETDisplay(void)
{
	SetDisplayChar(40, 0, FontLib_R, 0);
	SetDisplayChar(48, 0, FontLib_E, 0);
	SetDisplayChar(56, 0, FontLib_S, 0);
	SetDisplayChar(64, 0, FontLib_E, 0);
	SetDisplayChar(72, 0, FontLib_T, 0);
}

/*******************************************************************************
* ��������:	��λҳ2�ĸ���Ŀ��ʾ����
* �������: 
* �������: ��
* ����˵��: ��
*******************************************************************************/
void MRP1M09Display(void)
{
	if(OledExternalDataArr[29] >= 9)
	{
		SetDisplayChar(16, 2, FontLib_M, OledCtrl.PageCtrl.InverseLocation==1?NeedInverse:NoNeedInverse);
		SetDisplayChar(24, 2, &FontLib_Dight[0][0], OledCtrl.PageCtrl.InverseLocation==1?NeedInverse:NoNeedInverse);
		SetDisplayChar(32, 2, &FontLib_Dight[9][0], OledCtrl.PageCtrl.InverseLocation==1?NeedInverse:NoNeedInverse);
	}
}

void MRP1M10Display(void)
{
	if(OledExternalDataArr[29] >= 10)
	{
		SetDisplayChar(48, 2, FontLib_M, OledCtrl.PageCtrl.InverseLocation==2?NeedInverse:NoNeedInverse);
		SetDisplayChar(56, 2, &FontLib_Dight[1][0], OledCtrl.PageCtrl.InverseLocation==2?NeedInverse:NoNeedInverse);
		SetDisplayChar(64, 2, &FontLib_Dight[0][0], OledCtrl.PageCtrl.InverseLocation==2?NeedInverse:NoNeedInverse);
	}
}

/*******************************************************************************
* ��������:	ͨѶģ����Ϣҳ1�ĸ���Ŀ��ʾ����
* �������: 
* �������: ��
* ����˵��: ��
*******************************************************************************/
void CSDisplay(void)
{
	if(OledCtrl.Flag.Bits.AllUpdateFlag)
	{
		SetDisplayChar(16, 0, FontLib_C, 0);
		SetDisplayChar(24, 0, FontLib_S, 0);
		SetDisplayChar(32, 0, FontLib_Colon, 0);
	}
	
	if(OledExternalDataArr[7])
	{
		SetDisplayChar(40, 0, FontLib_E, 0);
		SetDisplayChar(48, 0, FontLib_R, 0);
		SetDisplayChar(56, 0, FontLib_R, 0);	
	}
	else
	{
		SetDisplayChar(40, 0, FontLib_R, 0);
		SetDisplayChar(48, 0, FontLib_U, 0);
		SetDisplayChar(56, 0, FontLib_N, 0);	
	}
}

void IDDisplay(void)
{
	if(OledCtrl.Flag.Bits.AllUpdateFlag)
	{
		SetDisplayChar(80, 0, FontLib_I, 0);
		SetDisplayChar(88, 0, FontLib_D, 0);
		SetDisplayChar(96, 0, FontLib_Colon, 0);
		DisplayDigit(104, 0, OledExternalDataArr[30], 2);
	}
}

void RTDisplay(void)
{
	if(OledCtrl.Flag.Bits.AllUpdateFlag)
	{
		SetDisplayChar(0, 4, FontLib_R, 0);
		SetDisplayChar(8, 4, FontLib_T, 0);
		SetDisplayChar(16, 4, FontLib_Colon, 0);
		
		SetDisplayChar(56, 4, FontLib_Bar, 0);
		SetDisplayChar(80, 4, FontLib_Bar, 0);
		SetDisplayChar(56, 6, FontLib_Colon, 0);
	}

	DisplayDigit(24, 4, (OledExternalDataArr[31]&0xfff0)>>4, 4);	//��
	DisplayDigit(64, 4, OledExternalDataArr[31]&0x000f, 2);			//��
	DisplayDigit(88, 4, (OledExternalDataArr[32]&0xff00)>>8, 2);	//��
	DisplayDigit(40, 6, OledExternalDataArr[32]&0x00ff, 2);			//ʱ
	DisplayDigit(64, 6, (OledExternalDataArr[33]&0xff00)>>8, 2);	//��
}

/*******************************************************************************
* ��������:	ͨѶģ����Ϣҳ2�ĸ���Ŀ��ʾ����
* �������: 
* �������: ��
* ����˵��: ��
*******************************************************************************/
void SDDisplay(void)
{
	if(OledCtrl.Flag.Bits.AllUpdateFlag)
	{
		SetDisplayChar(40, 0, FontLib_S, 0);
		SetDisplayChar(48, 0, FontLib_D, 0);
		SetDisplayChar(56, 0, FontLib_Colon, 0);
	}
	DisplayDigit(64, 0, OledExternalDataArr[34], 2);//�Ŷ����
}

void STDisplay(void)
{
	DisplayDigit(24, 4, (OledExternalDataArr[35]&0xfff0)>>4, 4);	//��
	DisplayDigit(64, 4, OledExternalDataArr[35]&0x000f, 2);			//��
	DisplayDigit(88, 4, (OledExternalDataArr[36]&0xff00)>>8, 2);	//��
	DisplayDigit(32, 6, OledExternalDataArr[36]&0x00ff, 2);			//ʱ
	DisplayDigit(56, 6, (OledExternalDataArr[37]&0xff00)>>8, 2);	//��
	DisplayDigit(80, 6, OledExternalDataArr[37]&0x00ff, 2);			//��
	
	if(OledCtrl.Flag.Bits.AllUpdateFlag)
	{
		SetDisplayChar(0, 4, FontLib_S, 0);
		SetDisplayChar(8, 4, FontLib_T, 0);
		SetDisplayChar(16, 4, FontLib_Colon, 0);
		
		SetDisplayChar(56, 4, FontLib_Bar, 0);
		SetDisplayChar(80, 4, FontLib_Bar, 0);
		SetDisplayChar(48, 6, FontLib_Colon, 0);
		SetDisplayChar(72, 6, FontLib_Colon, 0);
	}
}

/*******************************************************************************
* ��������:	ͨѶģ����Ϣҳ3�ĸ���Ŀ��ʾ����
* �������: 
* �������: ��
* ����˵��: ��
*******************************************************************************/
void REGDisplay(void)
{
	uint16_t i=0;
	
	if(OledCtrl.Flag.Bits.AllUpdateFlag)
	{
		SetDisplayChar(48, 0, FontLib_R, 0);
		SetDisplayChar(56, 0, FontLib_E, 0);
		SetDisplayChar(64, 0, FontLib_G, 0);
	}
	
	for(i=0; i<16; i++)
	{
		if((OledExternalDataArr[28]>>i) & 0x01)
		{
			SetDisplayChar(i*8, 2, &FontLib_Dight[1][0], 0);
		}
		else
		{
			SetDisplayChar(i*8, 2, &FontLib_Dight[0][0], 0);
		}
	}
}

/*******************************************************************************
* ��������:	����ģ����Ϣҳ1�ĸ���Ŀ��ʾ����
* �������: 
* �������: ��
* ����˵��: ��
*******************************************************************************/
void REG1Display(void)
{
	uint16_t i=0;
	uint16_t FaultReg = 0x00;
	
	if(OledCtrl.Flag.Bits.AllUpdateFlag)
	{
		SetDisplayChar(48, 0, FontLib_R, 0);
		SetDisplayChar(56, 0, FontLib_E, 0);
		SetDisplayChar(64, 0, FontLib_G, 0);
		SetDisplayChar(72, 0, &FontLib_Dight[1][0], 0);		
	}
	
	if(OledCtrl.PageCtrl.PageIndex.Bits.LastIndex == ModuleSeletionPage2Index)
	{
		FaultReg = OledExternalDataArr[OledCtrl.PageCtrl.SelectModuleNumber+12];
	}
	else
	{
		FaultReg = OledExternalDataArr[OledCtrl.PageCtrl.SelectModuleNumber+6];
	}
	
	for(i=0; i<16; i++)
	{
		if((FaultReg>>i) & 0x01)
		{
			SetDisplayChar(i*8, 2, &FontLib_Dight[1][0], 0);
		}
		else
		{
			SetDisplayChar(i*8, 2, &FontLib_Dight[0][0], 0);
		}
	}
}

void REG2Display(void)
{
	uint16_t i=0;
	uint16_t FaultReg = 0x00;
	
	if(OledCtrl.Flag.Bits.AllUpdateFlag)
	{
		SetDisplayChar(48, 4, FontLib_R, 0);
		SetDisplayChar(56, 4, FontLib_E, 0);
		SetDisplayChar(64, 4, FontLib_G, 0);
		SetDisplayChar(72, 4, &FontLib_Dight[2][0], 0);
	}
	
	if(OledCtrl.PageCtrl.PageIndex.Bits.LastIndex == ModuleSeletionPage2Index)
	{
		FaultReg = OledExternalDataArr[OledCtrl.PageCtrl.SelectModuleNumber+22];
	}
	else
	{
		FaultReg = OledExternalDataArr[OledCtrl.PageCtrl.SelectModuleNumber+16];
	}
	
	for(i=0; i<16; i++)
	{
		if((FaultReg>>i) & 0x01)
		{
			SetDisplayChar(i*8, 6, &FontLib_Dight[1][0], 0);
		}
		else
		{
			SetDisplayChar(i*8, 6, &FontLib_Dight[0][0], 0);
		}
	}
}

/*******************************************************************************
* ��������:	��ҳ����Ŀ������
* �������: 
* �������: ��
* ����˵��: ��
*******************************************************************************/
struct ITEMSTRUCT HomePageItem[] = {
	{SUAItemDisplay, NULL, HomePageIndex},
	{SUBItemDisplay, NULL, HomePageIndex},
	{SUCItemDisplay, NULL, HomePageIndex},
	{SIAItemDisplay, NULL, HomePageIndex},
	{SIBItemDisplay, NULL, HomePageIndex},
	{SICItemDisplay, NULL, HomePageIndex},
	{DSItemDisplay,  NULL, HomePageIndex},
	{CSItemDisplay,  NULL, HomePageIndex},
};

struct ITEMSTRUCT ModuleSeletionPage1[] = {
	{CMDisplay, 	 NULL, CommModInforPage1Index},
	{MSP1M01Display, MSPEnterFun, PowerModInforPage1Index},
	{MSP1M02Display, MSPEnterFun, PowerModInforPage1Index},
	{MSP1M03Display, MSPEnterFun, PowerModInforPage1Index},
	{MSP1M04Display, MSPEnterFun, PowerModInforPage1Index},
	{MSP1M05Display, MSPEnterFun, PowerModInforPage1Index},
	{MSP1M06Display, MSPEnterFun, PowerModInforPage1Index},
	{MODULEDisplay,  MSPEnterFun, PowerModInforPage1Index},
};

struct ITEMSTRUCT ModuleSeletionPage2[] = {		
	{MSP1M07Display, MSPEnterFun, PowerModInforPage1Index},
	{MSP1M08Display, MSPEnterFun, PowerModInforPage1Index},
	{MSP1M09Display, MSPEnterFun, PowerModInforPage1Index},
	{MSP1M10Display, MSPEnterFun, PowerModInforPage1Index},
	{MODULEDisplay,  NULL, PowerModInforPage1Index},	
};

struct ITEMSTRUCT ModResetPage1[] = {
	{ALLDisplay, 	 NULL, ResetPage1Index},
	{MRP1M01Display, NULL, ResetPage1Index},
	{MRP1M02Display, NULL, ResetPage1Index},
	{MRP1M03Display, NULL, ResetPage1Index},
	{MRP1M04Display, NULL, ResetPage1Index},
	{MRP1M05Display, NULL, ResetPage1Index},
	{MRP1M06Display, NULL, ResetPage1Index},
	{MRP1M07Display, NULL, ResetPage1Index},
	{MRP1M08Display, NULL, ResetPage1Index},
	{RESETDisplay, 	 NULL, ResetPage1Index},		

};

struct ITEMSTRUCT ModResetPage2[] = {
	{MRP1M09Display, NULL, ResetPage2Index},
	{MRP1M10Display, NULL, ResetPage2Index},
	{RESETDisplay, 	 NULL, ResetPage2Index},		
};

struct ITEMSTRUCT CommModInforPage1[] = {
	{CSDisplay, NULL, CommModInforPage1Index},
	{IDDisplay, NULL, CommModInforPage1Index},
	{RTDisplay, NULL, CommModInforPage1Index},
};

struct ITEMSTRUCT CommModInforPage2[] = {
	{SDDisplay, NULL, CommModInforPage2Index},
	{STDisplay, NULL, CommModInforPage2Index},
};

struct ITEMSTRUCT CommModInforPage3[] = {
	{REGDisplay, NULL, CommModInforPage3Index},
};

struct ITEMSTRUCT PowerModInforPage1[] = {
	{REG1Display, NULL, PowerModInforPage1Index},
	{REG2Display, NULL, PowerModInforPage1Index},
};
	
/*******************************************************************************
* ��������:	ҳ��������
* �������: 
* �������: ��
* ����˵��: ��
*******************************************************************************/
struct OLEDPAGESTRUCT PageIndexTable[] = {
		{HomePageItem,			HomePageIndex				<<16|HomePageIndex				<<8|ModuleSeletionPage1Index,	8<<8|0	},
		{ModuleSeletionPage1,	HomePageIndex				<<16|HomePageIndex				<<8|ModuleSeletionPage2Index,	8<<8|7	},
		{ModuleSeletionPage2,	HomePageIndex				<<16|ModuleSeletionPage1Index	<<8|ResetPage1Index,			5<<8|4	},
		{ModResetPage1,			HomePageIndex				<<16|ModuleSeletionPage2Index	<<8|ResetPage2Index,			10<<8|9	},
		{ModResetPage2,			HomePageIndex				<<16|ResetPage1Index			<<8|ResetPage2Index,			3<<8|2	},
		{CommModInforPage1,		ModuleSeletionPage1Index	<<16|CommModInforPage1Index		<<8|CommModInforPage2Index,		3<<8|0	},
		{CommModInforPage2,		ModuleSeletionPage1Index	<<16|CommModInforPage1Index		<<8|CommModInforPage3Index,		2<<8|0	},
		{CommModInforPage3,		ModuleSeletionPage1Index	<<16|CommModInforPage2Index		<<8|CommModInforPage3Index,		1<<8|0	},
		{PowerModInforPage1,	ModuleSeletionPage1Index	<<16|PowerModInforPage1Index	<<8|PowerModInforPage1Index,	2<<8|0	},
};
