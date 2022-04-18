/*
 *	oled.c
 *
 *	Created on: 2022-03-28
 *	Author: 	WangJianLong
 */
#include "oled.h" 
#include "SysTick.h"	

/*******************************************************************************
* 函数功能: 设置OLED IIC发送的数据及地址
* 输入参数: 
* 输出参数: 无
* 函数说明: 无
*******************************************************************************/
uint16_t SetOledI2cData(struct OLEDCTRL *OledCtrlStruct)
{
	uint16_t ret = 0;
	struct OLEDCTRL *Fd = OledCtrlStruct;
	
	if(Fd->PageCtrl.SendCompleteLen >= (Fd->NodeData.CtrlInfor.Bits.NeedSendLen))//判断节点数据是否发送完
	{
		ret = FetchLinklistData(Fd->LinklistHead,&Fd->NodeData);//从链表尾部取节点
		Fd->PageCtrl.SendCompleteLen = 0;
		if(ret == LLISTEMPTY)
		{
			Fd->Flag.Bits.UpdateDisplayFlag = 0;
			return SUCCESS;
		}
	}
	
	if(Fd->NodeData.CtrlInfor.Bits.CommandFlag == OledLocation)//待发送的数据是坐标
	{
		Fd->OledI2cCtrl.OledI2cDataInfor.Byte.Addr = 0x00;
		Fd->OledI2cCtrl.OledI2cDataInfor.Byte.Data = (uint16_t)(Fd->NodeData.DataAddr[Fd->PageCtrl.SendCompleteLen]);
	}
	else if(Fd->NodeData.CtrlInfor.Bits.CommandFlag == OledCommand)//待发送的数据是命令
	{
		Fd->OledI2cCtrl.OledI2cDataInfor.Byte.Addr = 0x00;
		Fd->OledI2cCtrl.OledI2cDataInfor.Byte.Data = *(Fd->NodeData.DataAddr[0] + Fd->PageCtrl.SendCompleteLen);
	}
	else//待发送的数据是数据
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

	TimeFlag = TimeDelay(&Fd->OledI2cCtrl.I2CTimeStamp,10000, TIM3);

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
				return;
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
* 输入参数: OledCtrlStruct：OLED控制结构体
* 输出参数: 无
* 函数说明: 根据按下的按键值，设置反显位置和页面索引
*******************************************************************************/
void UpdatePageCtrlInfor(struct OLEDCTRL *OledCtrlStruct)
{
	struct OLEDCTRL *Fd = OledCtrlStruct;
	
	switch(Fd->KeyCtrl.KEYSTATE.Bits.KeyValue)
	{
		case KEY_UP_VALUE://向上键按下时，反显位置减1。当反显位置等于1时，当前页面索引等于上键索引
			if(Fd->PageCtrl.InverseLocation <= 1)
			{
				Fd->PageCtrl.PageIndex.Bits.CurrentIndex = PageIndexTable[Fd->PageCtrl.PageIndex.Bits.CurrentIndex].PageIndex.Bits.CurrentPageUpIndex;
			}
			else
			{
				Fd->PageCtrl.InverseLocation--;
			}
			break;
		case KEY_DOWN_VALUE://向下键按下时，反显位置+1。当反显位置大于等于可反显项目数时，当前页面索引等于下键索引
			if(Fd->PageCtrl.InverseLocation >= PageIndexTable[Fd->PageCtrl.PageIndex.Bits.CurrentIndex].PageInforCtrl.Bits.InverseItemNum)
			{
				Fd->PageCtrl.PageIndex.Bits.CurrentIndex = PageIndexTable[Fd->PageCtrl.PageIndex.Bits.CurrentIndex].PageIndex.Bits.CurrentPageDownIndex;
			}
			else
			{
				Fd->PageCtrl.InverseLocation++;
			}
			break;
		case KEY_ENTER_VALUE://向确定键按下时，执行确定键功能函数，当前页面索引等于确定键索引
			if(PageIndexTable[Fd->PageCtrl.PageIndex.Bits.CurrentIndex].Item[Fd->PageCtrl.InverseLocation].ItemEnterFunction != NULL)
			{
				PageIndexTable[Fd->PageCtrl.PageIndex.Bits.CurrentIndex].Item[Fd->PageCtrl.InverseLocation].ItemEnterFunction();
			}
			Fd->PageCtrl.PageIndex.Bits.CurrentIndex = PageIndexTable[Fd->PageCtrl.PageIndex.Bits.CurrentIndex].Item[Fd->PageCtrl.InverseLocation-1].ItemEnterKeyIndex;
			break;
		case KEY_RETURN_VALUE://向返回键按下时，当前页面索引等于返回键索引
			Fd->PageCtrl.PageIndex.Bits.CurrentIndex = PageIndexTable[Fd->PageCtrl.PageIndex.Bits.CurrentIndex].PageIndex.Bits.CurrentPageReturnIndex;
			break;
		default:
			break;
	}
}

/*******************************************************************************
* 函数功能: 监测延时时间
* 输入参数: OledCtrlStruct：OLED控制结构体
* 输出参数: 无
* 函数说明: 无按键按下时，5秒刷新一次页面
*******************************************************************************/
void DetectionDelayTime(struct OLEDCTRL *OledCtrlStruct)
{
	uint16_t i=0;
	struct OLEDCTRL *Fd = OledCtrlStruct;
	
	if(TimeDelay(&Fd->PageUpdateDelay,10000, TIM3) == TimeFinish)//页面刷新时间到，刷新页面。
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
* 函数功能: 配置显示数据函数
* 输入参数: 
* 输出参数: 无
* 函数说明: 无
*******************************************************************************/
void ConfigDisplayData(struct OLEDCTRL *OledCtrlStruct)
{
	uint16_t i = 0;
	struct OLEDCTRL *Fd = OledCtrlStruct;
	
	if(Fd->Flag.Bits.OledFaultFlag == 1)//判断OLED是否发送故障（malloc失败、IIC发送超时）
		return;
	
	OledKeyDetection(&Fd->KeyCtrl);//按键检测
	
	if(Fd->KeyCtrl.KEYSTATE.Bits.KeyValue)//有按键按下时
	{
		UpdatePageCtrlInfor(Fd);//更新页面控制信息
		Fd->KeyCtrl.KEYSTATE.Bits.KeyValue = 0;	//按键值用完清零
		TimeStampClear(&Fd->PageUpdateDelay);//清除页面刷新时间戳
		if(Fd->PageCtrl.PageIndex.Bits.CurrentIndex != Fd->PageCtrl.PageIndex.Bits.LastIndex)//判断是否需要清屏
		{
			ClearScreen();//清屏
			Fd->Flag.Bits.AllUpdateFlag = 1;
			Fd->PageCtrl.InverseLocation = 1;
		}
		for(i=0; i<PageIndexTable[Fd->PageCtrl.PageIndex.Bits.CurrentIndex].PageInforCtrl.Bits.ItemNum; i++)//将要显示的数据插入链表
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
		DetectionDelayTime(Fd);//监测页面刷新时间戳
	}
}


/*******************************************************************************
* 函数功能: OLED显示函数
* 输入参数: 
* 输出参数: 无
* 函数说明: 无
*******************************************************************************/
void OledCtrlFunction(struct OLEDCTRL *OledCtrlStruct)
{
	struct OLEDCTRL *Fd = OledCtrlStruct;
	
	OledKeyIOScan(&Fd->KeyCtrl);	//扫描按键IO口
	
	if(Fd->Flag.Bits.UpdateDisplayFlag)
	{
		OledDisplayData(Fd);	//显示数据
	}
	else
	{
		ConfigDisplayData(Fd);	//配置数据
	}
}

