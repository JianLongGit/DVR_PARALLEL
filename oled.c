/*
 *	oled.c
 *
 *	Created on: 2022-03-28
 *	Author: 	WangJianLong
 */
#include "oled.h" 
#include "SysTick.h"	

uint16_t OledInitCommandArr[30] = {0xAE,0x00,0x10,0x40,0x81,0xCF,0xA1,0xC8,0xA6,0xA8,0x3F,0xD3,0x00,0xD5,0x80,0xD9,0xF1,0xDA,0x12,0xDB,0x40,0x20,0x00,0x8D,0x14,0xA4,0xA6,0xAF};

/*******************************************************************************
* 函数功能: Oled结构体初始化
* 输入参数: 
* 输出参数: 无
* 函数说明: 无
*******************************************************************************/
void OledStructInit(void)
{	
	memset(&OledCtrl, 0, sizeof(OledCtrl));
	OledCtrl.LinklistHead = list_create();//申请头节点
	OledCtrl.KeyCtrl.KEYSTATE.Bits.TempKeyValue = KEY_UP_VALUE;
	if((OledExternalDataArr[11]) > PageIndexTable[1].PageInforCtrl.Bits.ItemNum-2)
		PageIndexTable[1].PageInforCtrl.Bits.InverseItemNum = PageIndexTable[1].PageInforCtrl.Bits.ItemNum-1;
	else
		PageIndexTable[1].PageInforCtrl.Bits.InverseItemNum = OledExternalDataArr[11]+1;
	
	if((OledExternalDataArr[11]-6) > PageIndexTable[2].PageInforCtrl.Bits.ItemNum-2)
		PageIndexTable[2].PageInforCtrl.Bits.InverseItemNum = PageIndexTable[2].PageInforCtrl.Bits.ItemNum-1;
	else
		PageIndexTable[2].PageInforCtrl.Bits.InverseItemNum = OledExternalDataArr[11]-6;
	
	if((OledExternalDataArr[11]) > PageIndexTable[1].PageInforCtrl.Bits.ItemNum-2)
		PageIndexTable[1].PageInforCtrl.Bits.InverseItemNum = PageIndexTable[1].PageInforCtrl.Bits.ItemNum-1;
	else
		PageIndexTable[1].PageInforCtrl.Bits.InverseItemNum = OledExternalDataArr[11]+1;
	
	if((OledExternalDataArr[11]-1) > PageIndexTable[1].PageInforCtrl.Bits.ItemNum-2)
		PageIndexTable[1].PageInforCtrl.Bits.InverseItemNum = PageIndexTable[1].PageInforCtrl.Bits.ItemNum-1;
	else
		PageIndexTable[1].PageInforCtrl.Bits.InverseItemNum = OledExternalDataArr[11]-8;
	
	OledCtrl.PageCtrl.InverseLocation = 1;
	OledCtrl.PageCtrl.PageIndex.Bits.CurrentIndex = HomePageIndex;
	OledCtrl.PageCtrl.PageIndex.Bits.LastIndex = HomePageIndex;
	OledCtrl.PageCtrl.SendCompleteLen = 0xFFFF;
	OledCtrl.Flag.Bits.UpdateDisplayFlag = 1;
	OledCtrl.Flag.Bits.AllUpdateFlag = 1;
	DataInsertLinklist(OledCtrl.LinklistHead, OledInitCommandArr, 28, NoNeedInserve, 2);//Oled初始化命令
	ClearScreen(0);//清屏
	OledCtrl.DataNode->Data.CtrlInfor.Bits.NeedSendLen=0;
	TimeStampClear(&OledCtrl.PageUpdateDelay);
}

/*******************************************************************************
* 函数功能: 设置OLED IIC发送的数据及地址
* 输入参数: 
* 输出参数: 无
* 函数说明: 无
*******************************************************************************/
uint16_t SetOledI2cData(struct OLEDCTRL *OledCtrlStruct)
{
	struct OLEDCTRL *Fd = OledCtrlStruct;
	
	if(Fd->PageCtrl.SendCompleteLen >= (Fd->DataNode->Data.CtrlInfor.Bits.NeedSendLen))
	{
		if(Fd->PageCtrl.SendCompleteLen != 0xfff)
		{
			free(Fd->DataNode);
			Fd->DataNode = NULL;
		}
		if(EmptyCheak(Fd->LinklistHead))
		{
			Fd->Flag.Bits.UpdateDisplayFlag = 0;
			return SUCCESS;
		}
		Fd->DataNode = FetchLinklistData(Fd->LinklistHead);
		Fd->PageCtrl.SendCompleteLen = 0;
	}
	
	if(Fd->DataNode->Data.CtrlInfor.Bits.CommandFlag == OledCommand)
	{
		Fd->OledI2cCtrl.OledI2cDataInfor.Byte.Addr = 0x00;
		Fd->OledI2cCtrl.OledI2cDataInfor.Byte.Data = (uint16_t)(Fd->DataNode->Data.DataAddr[Fd->PageCtrl.SendCompleteLen]);
	}
	else
	{
		if(Fd->DataNode->Data.CtrlInfor.Bits.CommandFlag == 2)
			Fd->OledI2cCtrl.OledI2cDataInfor.Byte.Addr = 0x00;
		else
			Fd->OledI2cCtrl.OledI2cDataInfor.Byte.Addr = 0x40;
		if(Fd->DataNode->Data.CtrlInfor.Bits.InverseFlag == NeedInverse)
		{
			Fd->OledI2cCtrl.OledI2cDataInfor.Byte.Data = ~(*(Fd->DataNode->Data.DataAddr[0] + Fd->PageCtrl.SendCompleteLen));
		}
		else
		{
			Fd->OledI2cCtrl.OledI2cDataInfor.Byte.Data = *(Fd->DataNode->Data.DataAddr[0] + Fd->PageCtrl.SendCompleteLen);
		}		
	}
	
	Fd->PageCtrl.SendCompleteLen++;
	return BUSY;
}

/*******************************************************************************
* 函数功能: OLED显示数据
* 输入参数: 
* 输出参数: 无
* 函数说明: 无
*******************************************************************************/
void OledDisplayData(struct OLEDCTRL *OledCtrlStruct)
{
	struct OLEDCTRL *Fd = OledCtrlStruct;
	uint16_t TimeFlag;
	uint16_t WriteFlag;

	TimeFlag = TimeDelay(&Fd->OledI2cCtrl.I2CTimeStamp,6000, TIM3);

	switch(TimeFlag)
	{
		case TimeInit:	 
			if(SetOledI2cData(Fd)==SUCCESS)//设置要发送的数据
			{
				TimeStampClear(&Fd->OledI2cCtrl.I2CTimeStamp);
				return;
			}
			break;

		case TimeContinue:
			WriteFlag = OledI2CSendData(&Fd->OledI2cCtrl);	//IIC发送数据到OLED
			if(SUCCESS == WriteFlag)
			{
				TimeStampClear(&Fd->OledI2cCtrl.I2CTimeStamp);
			}
			break;

		default:	 //IIC发送数据到OLED超时
			Fd->OledI2cCtrl.OledI2CState = OledI2cLoop; //IIC状态设置为写循环
			TimeStampClear(&Fd->OledI2cCtrl.I2CTimeStamp);
			Fd->Flag.Bits.OledFaultFlag = 1;
			break;
	}
}

/*******************************************************************************
* 函数功能: 更新页面控制信息
* 输入参数: 
* 输出参数: 无
* 函数说明: 无
*******************************************************************************/
void UpdatePageCtrlInfor(struct OLEDCTRL *OledCtrlStruct)
{
	struct OLEDCTRL *Fd = OledCtrlStruct;
	
	switch(Fd->KeyCtrl.KEYSTATE.Bits.KeyValue)
	{
		case KEY_UP_VALUE:
			if(Fd->PageCtrl.InverseLocation <= 1)
			{
				Fd->PageCtrl.PageIndex.Bits.CurrentIndex = PageIndexTable[Fd->PageCtrl.PageIndex.Bits.CurrentIndex].PageIndex.Bits.CurrentPageUpIndex;
			}
			else
			{
				Fd->PageCtrl.InverseLocation--;
			}
			break;
		case KEY_DOWN_VALUE:
			if(Fd->PageCtrl.InverseLocation >= PageIndexTable[Fd->PageCtrl.PageIndex.Bits.CurrentIndex].PageInforCtrl.Bits.InverseItemNum)
			{
				Fd->PageCtrl.PageIndex.Bits.CurrentIndex = PageIndexTable[Fd->PageCtrl.PageIndex.Bits.CurrentIndex].PageIndex.Bits.CurrentPageDownIndex;
			}
			else
			{
				Fd->PageCtrl.InverseLocation++;
			}
			break;
		case KEY_ENTER_VALUE:
			if(PageIndexTable[Fd->PageCtrl.PageIndex.Bits.CurrentIndex].Item[Fd->PageCtrl.InverseLocation].ItemEnterFunction != NULL)
			{
				PageIndexTable[Fd->PageCtrl.PageIndex.Bits.CurrentIndex].Item[Fd->PageCtrl.InverseLocation].ItemEnterFunction();
			}
			Fd->PageCtrl.PageIndex.Bits.CurrentIndex = PageIndexTable[Fd->PageCtrl.PageIndex.Bits.CurrentIndex].Item[Fd->PageCtrl.InverseLocation-1].ItemEnterKeyIndex;
			break;
		case KEY_RETURN_VALUE:
			Fd->PageCtrl.PageIndex.Bits.CurrentIndex = PageIndexTable[Fd->PageCtrl.PageIndex.Bits.CurrentIndex].PageIndex.Bits.CurrentPageReturnIndex;
			break;
		default:
			break;
	}
}

/*******************************************************************************
* 函数功能: 监测延时时间
* 输入参数: 
* 输出参数: 无
* 函数说明: 无
*******************************************************************************/
void DetectionDelayTime(struct OLEDCTRL *OledCtrlStruct)
{
	uint16_t i=0;
	struct OLEDCTRL *Fd = OledCtrlStruct;
	
	if(TimeDelay(&Fd->PageUpdateDelay,1000, TIM3) == TimeFinish)
	{
		Fd->PageCtrl.SendCompleteLen = 0xFFF;
		Fd->Flag.Bits.UpdateDisplayFlag = 1;
		for(i=0; i<PageIndexTable[Fd->PageCtrl.PageIndex.Bits.CurrentIndex].PageInforCtrl.Bits.ItemNum; i++)
		{
			PageIndexTable[Fd->PageCtrl.PageIndex.Bits.CurrentIndex].Item[i].ItemDisplayFunction();
		}
	}
}


/*******************************************************************************
* 函数功能: 配置显示数据函数
* 输入参数: 
* 输出参数: 无
* 函数说明: 无
*******************************************************************************/
void ConfigDisplayData(struct OLEDCTRL *OledCtrlStruct)
{
	uint16_t i = 0;
	struct OLEDCTRL *Fd = OledCtrlStruct;
	
	if(Fd->Flag.Bits.OledFaultFlag == 1)
		return;
	
	OledKeyDetection(&Fd->KeyCtrl);
	if(Fd->KeyCtrl.KEYSTATE.Bits.KeyValue)
	{
		UpdatePageCtrlInfor(Fd);
		Fd->KeyCtrl.KEYSTATE.Bits.KeyValue = 0;
		TimeStampClear(&Fd->PageUpdateDelay);
		if(Fd->PageCtrl.PageIndex.Bits.CurrentIndex != Fd->PageCtrl.PageIndex.Bits.LastIndex)
		{
			ClearScreen(0);//清屏
			Fd->Flag.Bits.AllUpdateFlag = 1;
			Fd->PageCtrl.PageIndex.Bits.LastIndex = Fd->PageCtrl.PageIndex.Bits.CurrentIndex;
			Fd->PageCtrl.InverseLocation = 1;

		}
		Fd->PageCtrl.SendCompleteLen = 0xFFF;
		Fd->Flag.Bits.UpdateDisplayFlag = 1;
		Fd->DataNode->Data.CtrlInfor.Bits.NeedSendLen=0;
		for(i=0; i<PageIndexTable[Fd->PageCtrl.PageIndex.Bits.CurrentIndex].PageInforCtrl.Bits.ItemNum; i++)
		{
			PageIndexTable[Fd->PageCtrl.PageIndex.Bits.CurrentIndex].Item[i].ItemDisplayFunction();
		}
	}
	else
	{
		DetectionDelayTime(Fd);
	}
}


/*******************************************************************************
* 函数功能: OLED显示函数
* 输入参数: 
* 输出参数: 无
* 函数说明: 无
*******************************************************************************/
void OledDisplay(struct OLEDCTRL *OledCtrlStruct)
{
	struct OLEDCTRL *Fd = OledCtrlStruct;
	
	OledKeyIOScan(&Fd->KeyCtrl);
	OledKeyDetection(&Fd->KeyCtrl);
	
	if(Fd->Flag.Bits.UpdateDisplayFlag)
	{
		OledDisplayData(Fd);
	}
	else
	{
		ConfigDisplayData(Fd);
	}
	return;
}

