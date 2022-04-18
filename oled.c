/*
 *	oled.c
 *
 *	Created on: 2022-03-28
 *	Author: 	WangJianLong
 */
#include "oled.h" 
#include "SysTick.h"	

/*******************************************************************************
* ��������: ����OLED IIC���͵����ݼ���ַ
* �������: 
* �������: ��
* ����˵��: ��
*******************************************************************************/
uint16_t SetOledI2cData(struct OLEDCTRL *OledCtrlStruct)
{
	uint16_t ret = 0;
	struct OLEDCTRL *Fd = OledCtrlStruct;
	
	if(Fd->PageCtrl.SendCompleteLen >= (Fd->NodeData.CtrlInfor.Bits.NeedSendLen))//�жϽڵ������Ƿ�����
	{
		ret = FetchLinklistData(Fd->LinklistHead,&Fd->NodeData);//������β��ȡ�ڵ�
		Fd->PageCtrl.SendCompleteLen = 0;
		if(ret == LLISTEMPTY)
		{
			Fd->Flag.Bits.UpdateDisplayFlag = 0;
			return SUCCESS;
		}
	}
	
	if(Fd->NodeData.CtrlInfor.Bits.CommandFlag == OledLocation)//�����͵�����������
	{
		Fd->OledI2cCtrl.OledI2cDataInfor.Byte.Addr = 0x00;
		Fd->OledI2cCtrl.OledI2cDataInfor.Byte.Data = (uint16_t)(Fd->NodeData.DataAddr[Fd->PageCtrl.SendCompleteLen]);
	}
	else if(Fd->NodeData.CtrlInfor.Bits.CommandFlag == OledCommand)//�����͵�����������
	{
		Fd->OledI2cCtrl.OledI2cDataInfor.Byte.Addr = 0x00;
		Fd->OledI2cCtrl.OledI2cDataInfor.Byte.Data = *(Fd->NodeData.DataAddr[0] + Fd->PageCtrl.SendCompleteLen);
	}
	else//�����͵�����������
	{
		Fd->OledI2cCtrl.OledI2cDataInfor.Byte.Addr = 0x40;
		if(Fd->NodeData.CtrlInfor.Bits.InverseFlag == NeedInverse)
		{
			Fd->OledI2cCtrl.OledI2cDataInfor.Byte.Data = ~(*(Fd->NodeData.DataAddr[0] + Fd->PageCtrl.SendCompleteLen));
		}
		else
		{
			Fd->OledI2cCtrl.OledI2cDataInfor.Byte.Data = *(Fd->NodeData.DataAddr[0] + Fd->PageCtrl.SendCompleteLen);
		}		
	}
	
	Fd->PageCtrl.SendCompleteLen++;
	return BUSY;
}

/*******************************************************************************
* ��������: OLED��ʾ����
* �������: 
* �������: ��
* ����˵��: ��
*******************************************************************************/
void OledDisplayData(struct OLEDCTRL *OledCtrlStruct)
{
	struct OLEDCTRL *Fd = OledCtrlStruct;
	uint16_t TimeFlag;
	uint16_t WriteFlag;

	TimeFlag = TimeDelay(&Fd->OledI2cCtrl.I2CTimeStamp,10000, TIM3);

	switch(TimeFlag)
	{
		case TimeInit:	 
			if(SetOledI2cData(Fd)==SUCCESS)//����Ҫ���͵�����
			{
				TimeStampClear(&Fd->OledI2cCtrl.I2CTimeStamp);
				return;
			}
			break;

		case TimeContinue:
			WriteFlag = OledI2CSendData(&Fd->OledI2cCtrl);	//IIC�������ݵ�OLED
			if(SUCCESS == WriteFlag)
			{
				TimeStampClear(&Fd->OledI2cCtrl.I2CTimeStamp);
				return;
			}
			break;

		default:	 //IIC�������ݵ�OLED��ʱ
			Fd->OledI2cCtrl.OledI2CState = OledI2cLoop; //IIC״̬����Ϊдѭ��
			TimeStampClear(&Fd->OledI2cCtrl.I2CTimeStamp);
			Fd->Flag.Bits.OledFaultFlag = 1;
			break;
	}
}

/*******************************************************************************
* ��������: ����ҳ�������Ϣ
* �������: OledCtrlStruct��OLED���ƽṹ��
* �������: ��
* ����˵��: ���ݰ��µİ���ֵ�����÷���λ�ú�ҳ������
*******************************************************************************/
void UpdatePageCtrlInfor(struct OLEDCTRL *OledCtrlStruct)
{
	struct OLEDCTRL *Fd = OledCtrlStruct;
	
	switch(Fd->KeyCtrl.KEYSTATE.Bits.KeyValue)
	{
		case KEY_UP_VALUE://���ϼ�����ʱ������λ�ü�1��������λ�õ���1ʱ����ǰҳ�����������ϼ�����
			if(Fd->PageCtrl.InverseLocation <= 1)
			{
				Fd->PageCtrl.PageIndex.Bits.CurrentIndex = PageIndexTable[Fd->PageCtrl.PageIndex.Bits.CurrentIndex].PageIndex.Bits.CurrentPageUpIndex;
			}
			else
			{
				Fd->PageCtrl.InverseLocation--;
			}
			break;
		case KEY_DOWN_VALUE://���¼�����ʱ������λ��+1��������λ�ô��ڵ��ڿɷ�����Ŀ��ʱ����ǰҳ�����������¼�����
			if(Fd->PageCtrl.InverseLocation >= PageIndexTable[Fd->PageCtrl.PageIndex.Bits.CurrentIndex].PageInforCtrl.Bits.InverseItemNum)
			{
				Fd->PageCtrl.PageIndex.Bits.CurrentIndex = PageIndexTable[Fd->PageCtrl.PageIndex.Bits.CurrentIndex].PageIndex.Bits.CurrentPageDownIndex;
			}
			else
			{
				Fd->PageCtrl.InverseLocation++;
			}
			break;
		case KEY_ENTER_VALUE://��ȷ��������ʱ��ִ��ȷ�������ܺ�������ǰҳ����������ȷ��������
			if(PageIndexTable[Fd->PageCtrl.PageIndex.Bits.CurrentIndex].Item[Fd->PageCtrl.InverseLocation].ItemEnterFunction != NULL)
			{
				PageIndexTable[Fd->PageCtrl.PageIndex.Bits.CurrentIndex].Item[Fd->PageCtrl.InverseLocation].ItemEnterFunction();
			}
			Fd->PageCtrl.PageIndex.Bits.CurrentIndex = PageIndexTable[Fd->PageCtrl.PageIndex.Bits.CurrentIndex].Item[Fd->PageCtrl.InverseLocation-1].ItemEnterKeyIndex;
			break;
		case KEY_RETURN_VALUE://�򷵻ؼ�����ʱ����ǰҳ���������ڷ��ؼ�����
			Fd->PageCtrl.PageIndex.Bits.CurrentIndex = PageIndexTable[Fd->PageCtrl.PageIndex.Bits.CurrentIndex].PageIndex.Bits.CurrentPageReturnIndex;
			break;
		default:
			break;
	}
}

/*******************************************************************************
* ��������: �����ʱʱ��
* �������: OledCtrlStruct��OLED���ƽṹ��
* �������: ��
* ����˵��: �ް�������ʱ��5��ˢ��һ��ҳ��
*******************************************************************************/
void DetectionDelayTime(struct OLEDCTRL *OledCtrlStruct)
{
	uint16_t i=0;
	struct OLEDCTRL *Fd = OledCtrlStruct;
	
	if(TimeDelay(&Fd->PageUpdateDelay,10000, TIM3) == TimeFinish)//ҳ��ˢ��ʱ�䵽��ˢ��ҳ�档
	{
		for(i=0; i<PageIndexTable[Fd->PageCtrl.PageIndex.Bits.CurrentIndex].PageInforCtrl.Bits.ItemNum; i++)
		{
			PageIndexTable[Fd->PageCtrl.PageIndex.Bits.CurrentIndex].Item[i].ItemDisplayFunction();
		}
		Fd->PageCtrl.SendCompleteLen = 0xFFF;
		Fd->Flag.Bits.UpdateDisplayFlag = 1;	
	}
}


/*******************************************************************************
* ��������: ������ʾ���ݺ���
* �������: 
* �������: ��
* ����˵��: ��
*******************************************************************************/
void ConfigDisplayData(struct OLEDCTRL *OledCtrlStruct)
{
	uint16_t i = 0;
	struct OLEDCTRL *Fd = OledCtrlStruct;
	
	if(Fd->Flag.Bits.OledFaultFlag == 1)//�ж�OLED�Ƿ��͹��ϣ�mallocʧ�ܡ�IIC���ͳ�ʱ��
		return;
	
	OledKeyDetection(&Fd->KeyCtrl);//�������
	
	if(Fd->KeyCtrl.KEYSTATE.Bits.KeyValue)//�а�������ʱ
	{
		UpdatePageCtrlInfor(Fd);//����ҳ�������Ϣ
		Fd->KeyCtrl.KEYSTATE.Bits.KeyValue = 0;	//����ֵ��������
		TimeStampClear(&Fd->PageUpdateDelay);//���ҳ��ˢ��ʱ���
		if(Fd->PageCtrl.PageIndex.Bits.CurrentIndex != Fd->PageCtrl.PageIndex.Bits.LastIndex)//�ж��Ƿ���Ҫ����
		{
			ClearScreen();//����
			Fd->Flag.Bits.AllUpdateFlag = 1;
			Fd->PageCtrl.InverseLocation = 1;
		}
		for(i=0; i<PageIndexTable[Fd->PageCtrl.PageIndex.Bits.CurrentIndex].PageInforCtrl.Bits.ItemNum; i++)//��Ҫ��ʾ�����ݲ�������
		{
			PageIndexTable[Fd->PageCtrl.PageIndex.Bits.CurrentIndex].Item[i].ItemDisplayFunction();
		}
		Fd->PageCtrl.PageIndex.Bits.LastIndex = Fd->PageCtrl.PageIndex.Bits.CurrentIndex;		
		Fd->PageCtrl.SendCompleteLen = 0xFFF;
		Fd->Flag.Bits.AllUpdateFlag = 0;
		Fd->Flag.Bits.UpdateDisplayFlag = 1;
		Fd->NodeData.CtrlInfor.Bits.NeedSendLen=0;
	}
	else
	{
		DetectionDelayTime(Fd);//���ҳ��ˢ��ʱ���
	}
}


/*******************************************************************************
* ��������: OLED��ʾ����
* �������: 
* �������: ��
* ����˵��: ��
*******************************************************************************/
void OledCtrlFunction(struct OLEDCTRL *OledCtrlStruct)
{
	struct OLEDCTRL *Fd = OledCtrlStruct;
	
	OledKeyIOScan(&Fd->KeyCtrl);	//ɨ�谴��IO��
	
	if(Fd->Flag.Bits.UpdateDisplayFlag)
	{
		OledDisplayData(Fd);	//��ʾ����
	}
	else
	{
		ConfigDisplayData(Fd);	//��������
	}
}

