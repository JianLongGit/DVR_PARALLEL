#include "oledconfig.h"
#include "oledfont.h"

struct OLEDPAGESTRUCT OledPageTable[] = {0};
struct ITEMSTRUCT ItemTable[] = {0};
struct OLEDCTRL OledCtrl;

/*******************************************************************************
* 函数功能:	坐标插入链表
* 输入参数: x:列坐标，y:页地址
* 输出参数: 无
* 函数说明: 无
*******************************************************************************/
void InsertLocation(uint16_t x, uint16_t y)
{
	struct LINKLISTDATATYPE *Data;
	uint16_t ret = 0;
	
	//将坐标值存入数据区的指针数组中
	Data->DataAddr[0] = (uint16_t *)(0xb0+y);
	Data->DataAddr[1] = (uint16_t *)(((x&0xf0)>>4)|0x10);
	Data->DataAddr[2] = (uint16_t *)(x&0x0f);
	
	//设置节点的控制信息
	Data->CtrlInfor.Bits.CommandFlag = OledLocation;
	Data->CtrlInfor.Bits.InverseFlag = NoNeedInverse;
	Data->CtrlInfor.Bits.NeedSendLen = 3;
	
	//节点插入链表
	ret = DataInsertLinklist(OledCtrl.LinklistHead, Data, FirstMode);
	if(ret == MALLOCFAIL)
	{
		OledCtrl.Flag.Bits.OledFaultFlag = 1;
	}
}


/*******************************************************************************
* 函数功能:	字符库插入链表
* 输入参数: *Point:字符字库首地址； InverseFlag:反显标志； Command：数据/命令/坐标； Sendlen：发送的长度
* 输出参数: 无
* 函数说明: 无
*******************************************************************************/
void InsertCharLib(uint16_t *Point, uint16_t InverseFlag, uint16_t Command, uint16_t Sendlen)
{
	struct LINKLISTDATATYPE *Data;
	uint16_t ret = 0;
	
	//将待发送字库的首地址存入数据区的指针数组中
	Data->DataAddr[0] = Point;
	
	//设置节点的控制信息
	Data->CtrlInfor.Bits.CommandFlag = Command;
	Data->CtrlInfor.Bits.InverseFlag = InverseFlag;
	Data->CtrlInfor.Bits.NeedSendLen = Sendlen;
	
	//节点插入链表
	ret = DataInsertLinklist(OledCtrl.LinklistHead, Data, FirstMode);
	if(ret == MALLOCFAIL)
	{
		OledCtrl.Flag.Bits.OledFaultFlag = 1;
	}
}


/*******************************************************************************
* 函数功能:	字符插入链表
* 输入参数: x:列坐标，y:页地址, *Point:字符字库首地址， InverseFlag:反显标志
* 输出参数: 无
* 函数说明: 无
*******************************************************************************/
void SetDisplayChar(uint16_t x, uint16_t y, uint16_t *Point, uint16_t InverseFlag)
{
	//字符的字库前八个数据的坐标入链表
	InsertLocation(x,y);
	
	//字符的字库前八个数据的首地址入链表
	InsertCharLib(Point,InverseFlag,OledData,8);
	
	//字符的字库后八个数据的坐标入链表
	InsertLocation(x,y+1);
	
	//字符的字库后八个数据的首地址入链表
	InsertCharLib(Point+8,InverseFlag,OledData,8);
}

/*******************************************************************************
* 函数功能:	清屏函数
* 输入参数: 
* 输出参数: 无
* 函数说明: 无
*******************************************************************************/
void ClearScreen(uint16_t InverseFlag)
{
	uint16_t i=0;
	
	InsertLocation(0,0);
	
	for(i=0; i<8; i++)
	{
		InsertCharLib(OledClearScreenArr, InverseFlag, OledData, sizeof(OledClearScreenArr));
	}
	
}

/*******************************************************************************
* 函数功能:	OLED初始化命令
* 输入参数: 
* 输出参数: 无
* 函数说明: 无
*******************************************************************************/
void OledInitCommand(void)
{
	InsertCharLib(OledCommandInitArr, NoNeedInverse, OledCommand, sizeof(OledCommandInitArr));
}

void OledStructInit(void)
{	
	memset(&OledCtrl, 0, sizeof(OledCtrl));
	
	//申请链表头
	OledCtrl.LinklistHead = LinklistCareate(sizeof(struct LINKLISTDATATYPE));
	
	//临时按键值 初始化为 向上键
	OledCtrl.KeyCtrl.KEYSTATE.Bits.TempKeyValue = KEY_UP_VALUE;
	
	//设置页面索引表中各页面 的 可反选项目数
	if((OledExternalDataArr[11]) > PageIndexTable[1].PageInforCtrl.Bits.ItemNum-2)
		PageIndexTable[1].PageInforCtrl.Bits.InverseItemNum = PageIndexTable[1].PageInforCtrl.Bits.ItemNum-1;
	else
		PageIndexTable[1].PageInforCtrl.Bits.InverseItemNum = OledExternalDataArr[11]+1;
	
	if((OledExternalDataArr[11]-6) > PageIndexTable[2].PageInforCtrl.Bits.ItemNum-2)
		PageIndexTable[2].PageInforCtrl.Bits.InverseItemNum = PageIndexTable[2].PageInforCtrl.Bits.ItemNum-1;
	else
		PageIndexTable[2].PageInforCtrl.Bits.InverseItemNum = OledExternalDataArr[11]-6;
	
	if((OledExternalDataArr[11]) > PageIndexTable[3].PageInforCtrl.Bits.ItemNum-2)
		PageIndexTable[3].PageInforCtrl.Bits.InverseItemNum = PageIndexTable[3].PageInforCtrl.Bits.ItemNum-1;
	else
		PageIndexTable[3].PageInforCtrl.Bits.InverseItemNum = OledExternalDataArr[11]+1;
	
	if((OledExternalDataArr[11]-8) > PageIndexTable[4].PageInforCtrl.Bits.ItemNum-2)
		PageIndexTable[4].PageInforCtrl.Bits.InverseItemNum = PageIndexTable[4].PageInforCtrl.Bits.ItemNum-1;
	else
		PageIndexTable[4].PageInforCtrl.Bits.InverseItemNum = OledExternalDataArr[11]-8;
	
	//初始化页面控制结构体
	OledCtrl.PageCtrl.PageIndex.Bits.CurrentIndex = HomePageIndex;
	OledCtrl.PageCtrl.PageIndex.Bits.LastIndex = HomePageIndex;
	OledCtrl.PageCtrl.InverseLocation = 1;
	OledCtrl.PageCtrl.SendCompleteLen = 0xfff;
	
	//初始化OLED标志
	OledCtrl.Flag.Bits.UpdateDisplayFlag = 1;
	
	//OLED初始化命令
	OledInitCommand();
	
	//OLED清屏
	ClearScreen(NeedInverse);//清屏
}

/*******************************************************************************
* 函数功能:	数字插入链表
* 输入参数: 
* 输出参数: 无
* 函数说明: 无
*******************************************************************************/
void DisplayDigit(uint16_t x, uint16_t y, uint16_t Digit, uint16_t BitNum)
{
	uint16_t i = 0;
	uint16_t DightArr[5];
	
	DightArr[0] = (Digit%10);
	DightArr[1] = ((Digit/10)%10);
	DightArr[2] = (Digit/100)%10;
	DightArr[3] = (Digit/1000)%10;
	DightArr[4] = (Digit/10000);
	
	for(i=0; i<BitNum; i++)
	{
		SetDisplayChar(x+8*i, y, DightFont[0]+DightArr[BitNum-i-1], NoNeedInverse);
	}
}

///*******************************************************************************
//* 函数功能: 数字插入链表
//* 输入参数: 
//* 输出参数: 无
//* 函数说明: 无
//*******************************************************************************/
//void DisplayDigit(uint16_t x, uint16_t y, uint16_t Digit, uint16_t BitNum)
//{
//	uint16_t i = 0;
//	uint16_t DightArr[5];
//	uint16_t temp = Digit;
//	
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
//		SetDisplayChar(x+8*i, y, DightFont[0]+DightArr[BitNum-i-1], NoNeedInverse);
//	}
//}

/*****************************首页显示函数********************************/
void SUAItemDisplay(void)
{
	if(OledCtrl.Flag.Bits.AllUpdateFlag == 1)
	{
		SetDisplayChar(0, 0, Font_S, 0);
		SetDisplayChar(8, 0, Font_U, 0);
		SetDisplayChar(16, 0, Font_A, 0);
		SetDisplayChar(24, 0, Font_Colon, 0);
	}
	DisplayDigit(32, 0, OledExternalDataArr[0], 3);
}

void SUBItemDisplay(void)
{
	if(OledCtrl.Flag.Bits.AllUpdateFlag == 1)
	{
		SetDisplayChar(0, 2, Font_S, 0);
		SetDisplayChar(8, 2, Font_U, 0);
		SetDisplayChar(16, 2, Font_B, 0);
		SetDisplayChar(24, 2, Font_Colon, 0);
	}
	DisplayDigit(32, 2, OledExternalDataArr[1], 3);
}

void SUCItemDisplay(void)
{
	if(OledCtrl.Flag.Bits.AllUpdateFlag == 1)
	{
		SetDisplayChar(0, 4, Font_S, 0);
		SetDisplayChar(8, 4, Font_U, 0);
		SetDisplayChar(16, 4, Font_C, 0);
		SetDisplayChar(24, 4, Font_Colon, 0);
	}
	DisplayDigit(32, 4, OledExternalDataArr[2], 3);
}

void SIAItemDisplay(void)
{
	if(OledCtrl.Flag.Bits.AllUpdateFlag == 1)
	{
		SetDisplayChar(64, 0, Font_S, 0);
		SetDisplayChar(72, 0, Font_I, 0);
		SetDisplayChar(80, 0, Font_A, 0);
		SetDisplayChar(88, 0, Font_Colon, 0);
	}
	DisplayDigit(96, 0, OledExternalDataArr[3], 3);
}

void SIBItemDisplay(void)
{
	if(OledCtrl.Flag.Bits.AllUpdateFlag == 1)
	{
		SetDisplayChar(64, 2, Font_S, 0);
		SetDisplayChar(72, 2, Font_I, 0);
		SetDisplayChar(80, 2, Font_B, 0);
		SetDisplayChar(88, 2, Font_Colon, 0);
	}
	DisplayDigit(96, 2, OledExternalDataArr[4], 3);
}

void SICItemDisplay(void)
{
	if(OledCtrl.Flag.Bits.AllUpdateFlag == 1)
	{
		SetDisplayChar(64, 4, Font_S, 0);
		SetDisplayChar(72, 4, Font_I, 0);
		SetDisplayChar(80, 4, Font_C, 0);
		SetDisplayChar(88, 4, Font_Colon, 0);
	}
	DisplayDigit(96, 4, OledExternalDataArr[5], 3);
}

void DSItemDisplay(void)
{
	if(OledCtrl.Flag.Bits.AllUpdateFlag == 1)
	{
		SetDisplayChar(8, 6, Font_D, 0);
		SetDisplayChar(16, 6, Font_S, 0);
		SetDisplayChar(24, 6, Font_Colon, 0);
	}

	//显示设备运行状态
	if(OledExternalDataArr[6] == 0x1)
	{
		SetDisplayChar(32, 6, Font_W, 0);
	}
	else if(OledExternalDataArr[6] == 0x2)
	{
		SetDisplayChar(32, 6, Font_R, 0);
	}
	else
	{
		SetDisplayChar(32, 6, Font_E, 0);
	}
	
	//功率模块故障时显示*号
	if(OledExternalDataArr[38])
	{
		SetDisplayChar(40, 6, Font_Asterisk, 0);
	}
	else
	{
		SetDisplayChar(40, 6, Font_Space, 0);
	}
}

void CSItemDisplay(void)
{
	if(OledCtrl.Flag.Bits.AllUpdateFlag == 1)
	{
		SetDisplayChar(72, 6, Font_C, 0);
		SetDisplayChar(80, 6, Font_S, 0);
		SetDisplayChar(88, 6, Font_Colon, 0);
	}
	//显示通讯模块运行状态
	if(OledExternalDataArr[7] == 0x1)
	{
		SetDisplayChar(96, 6, Font_W, 0);
	}
	else if(OledExternalDataArr[7] == 0x2)
	{
		SetDisplayChar(96, 6, Font_R, 0);
	}
	else
	{
		SetDisplayChar(96, 6, Font_E, 0);
	}
	
	//通讯模块故障时显示*号
	if(OledExternalDataArr[28])
	{
		SetDisplayChar(104, 6, Font_Asterisk, 0);
	}
	else
	{
		SetDisplayChar(104, 6, Font_Space, 0);
	}
}

/****************************模块选择页1显示函数*****************************/
void CMDisplay(void)
{
	SetDisplayChar(8, 2, Font_C, OledCtrl.PageCtrl.InverseLocation==1?NeedInverse:NoNeedInverse);
	SetDisplayChar(16, 2, Font_M, OledCtrl.PageCtrl.InverseLocation==1?NeedInverse:NoNeedInverse);
	SetDisplayChar(24, 2, Font_Space, OledCtrl.PageCtrl.InverseLocation==1?NeedInverse:NoNeedInverse);
	if(OledExternalDataArr[28])
	{
		SetDisplayChar(32, 2, Font_Asterisk, OledCtrl.PageCtrl.InverseLocation==1?NeedInverse:NoNeedInverse);
	}
}

void MSP1M01Display(void)
{
	if(OledExternalDataArr[29] >= 1)
	{
		SetDisplayChar(8, 4, Font_M, OledCtrl.PageCtrl.InverseLocation==2?NeedInverse:NoNeedInverse);
		SetDisplayChar(16, 4, DightFont[0]+0, OledCtrl.PageCtrl.InverseLocation==2?NeedInverse:NoNeedInverse);
		SetDisplayChar(24, 4, DightFont[0]+1, OledCtrl.PageCtrl.InverseLocation==2?NeedInverse:NoNeedInverse);
	}
}

void MSP1M02Display(void)
{
	if(OledExternalDataArr[29] >= 2)
	{
		SetDisplayChar(48, 4, Font_M, OledCtrl.PageCtrl.InverseLocation==3?NeedInverse:NoNeedInverse);
		SetDisplayChar(56, 4, DightFont[0]+0, OledCtrl.PageCtrl.InverseLocation==3?NeedInverse:NoNeedInverse);
		SetDisplayChar(64, 4, DightFont[0]+2, OledCtrl.PageCtrl.InverseLocation==3?NeedInverse:NoNeedInverse);
	}
}

void MSP1M03Display(void)
{
	if(OledExternalDataArr[29] >= 3)
	{
		SetDisplayChar(88, 4, Font_M, OledCtrl.PageCtrl.InverseLocation==4?NeedInverse:NoNeedInverse);
		SetDisplayChar(96, 4, DightFont[0]+0, OledCtrl.PageCtrl.InverseLocation==4?NeedInverse:NoNeedInverse);
		SetDisplayChar(104, 4, DightFont[0]+3, OledCtrl.PageCtrl.InverseLocation==4?NeedInverse:NoNeedInverse);
	}
}

void MSP1M04Display(void)
{
	if(OledExternalDataArr[29] >= 4)
	{
		SetDisplayChar(8, 6, Font_M, OledCtrl.PageCtrl.InverseLocation==5?NeedInverse:NoNeedInverse);
		SetDisplayChar(16, 6, DightFont[0]+0, OledCtrl.PageCtrl.InverseLocation==5?NeedInverse:NoNeedInverse);
		SetDisplayChar(24, 6, DightFont[0]+4, OledCtrl.PageCtrl.InverseLocation==5?NeedInverse:NoNeedInverse);
	}
}

void MSP1M05Display(void)
{
	if(OledExternalDataArr[29] >= 5)
	{
		SetDisplayChar(48, 6, Font_M, OledCtrl.PageCtrl.InverseLocation==6?NeedInverse:NoNeedInverse);
		SetDisplayChar(56, 6, DightFont[0]+0, OledCtrl.PageCtrl.InverseLocation==6?NeedInverse:NoNeedInverse);
		SetDisplayChar(64, 6, DightFont[0]+5, OledCtrl.PageCtrl.InverseLocation==6?NeedInverse:NoNeedInverse);
	}
}

void MSP1M06Display(void)
{
	if(OledExternalDataArr[29] > 6)
	{
		SetDisplayChar(88, 6, Font_M, OledCtrl.PageCtrl.InverseLocation==7?NeedInverse:NoNeedInverse);
		SetDisplayChar(96, 6, DightFont[0]+0, OledCtrl.PageCtrl.InverseLocation==7?NeedInverse:NoNeedInverse);
		SetDisplayChar(104, 6, DightFont[0]+6, OledCtrl.PageCtrl.InverseLocation==7?NeedInverse:NoNeedInverse);
	}
}

void MODULEDisplay(void)
{
	SetDisplayChar(40, 0, Font_M, NoNeedInverse);
	SetDisplayChar(48, 0, Font_O, NoNeedInverse);
	SetDisplayChar(56, 0, Font_D, NoNeedInverse);
	SetDisplayChar(64, 0, Font_U, NoNeedInverse);
	SetDisplayChar(72, 0, Font_L, NoNeedInverse);
	SetDisplayChar(80, 0, Font_E, NoNeedInverse);
}


/***************************模块选择页2显示函数***************************/
void MSP1M07Display(void)
{
	if(OledExternalDataArr[29] >= 7)
	{
		SetDisplayChar(8, 2, Font_M, OledCtrl.PageCtrl.InverseLocation==1?NeedInverse:NoNeedInverse);
		SetDisplayChar(16, 2, DightFont[0]+0, OledCtrl.PageCtrl.InverseLocation==1?NeedInverse:NoNeedInverse);
		SetDisplayChar(24, 2, DightFont[0]+7, OledCtrl.PageCtrl.InverseLocation==1?NeedInverse:NoNeedInverse);
	}
}

void MSP1M08Display(void)
{
	if(OledExternalDataArr[29] >= 8)
	{
		SetDisplayChar(48, 2, Font_M, OledCtrl.PageCtrl.InverseLocation==2?NeedInverse:NoNeedInverse);
		SetDisplayChar(56, 2, DightFont[0]+0, OledCtrl.PageCtrl.InverseLocation==2?NeedInverse:NoNeedInverse);
		SetDisplayChar(64, 2, DightFont[0]+8, OledCtrl.PageCtrl.InverseLocation==2?NeedInverse:NoNeedInverse);
	}
}

void MSP1M09Display(void)
{
	if(OledExternalDataArr[29] >= 9)
	{
		SetDisplayChar(88, 2, Font_M, OledCtrl.PageCtrl.InverseLocation==3?NeedInverse:NoNeedInverse);
		SetDisplayChar(96, 2, DightFont[0]+0, OledCtrl.PageCtrl.InverseLocation==3?NeedInverse:NoNeedInverse);
		SetDisplayChar(104, 2, DightFont[0]+9, OledCtrl.PageCtrl.InverseLocation==3?NeedInverse:NoNeedInverse);
	}
}

void MSP1M10Display(void)
{
	if(OledExternalDataArr[29] >= 10)
	{
		SetDisplayChar(8, 4, Font_M, OledCtrl.PageCtrl.InverseLocation==4?NeedInverse:NoNeedInverse);
		SetDisplayChar(16, 4, DightFont[0]+1, OledCtrl.PageCtrl.InverseLocation==4?NeedInverse:NoNeedInverse);
		SetDisplayChar(24, 4, DightFont[0]+0, OledCtrl.PageCtrl.InverseLocation==4?NeedInverse:NoNeedInverse);
	}
}


/***************************复位页1显示函数****************************/
void ALLDisplay(void)
{
	SetDisplayChar(16, 2, Font_A, OledCtrl.PageCtrl.InverseLocation==1?NeedInverse:NoNeedInverse);
	SetDisplayChar(24, 2, Font_L, OledCtrl.PageCtrl.InverseLocation==1?NeedInverse:NoNeedInverse);
	SetDisplayChar(32, 2, Font_L, OledCtrl.PageCtrl.InverseLocation==1?NeedInverse:NoNeedInverse);
}

void MRP1M01Display(void)
{
	if(OledExternalDataArr[29] >= 1)
	{
		SetDisplayChar(48, 2, Font_M, OledCtrl.PageCtrl.InverseLocation==2?NeedInverse:NoNeedInverse);
		SetDisplayChar(56, 2, DightFont[0]+0, OledCtrl.PageCtrl.InverseLocation==2?NeedInverse:NoNeedInverse);
		SetDisplayChar(64, 2, DightFont[0]+1, OledCtrl.PageCtrl.InverseLocation==2?NeedInverse:NoNeedInverse);
	}
}

void MRP1M02Display(void)
{
	if(OledExternalDataArr[29] >= 2)
	{
		SetDisplayChar(80, 2, Font_M, OledCtrl.PageCtrl.InverseLocation==3?NeedInverse:NoNeedInverse);
		SetDisplayChar(88, 2, DightFont[0]+0, OledCtrl.PageCtrl.InverseLocation==3?NeedInverse:NoNeedInverse);
		SetDisplayChar(96, 2, DightFont[0]+2, OledCtrl.PageCtrl.InverseLocation==3?NeedInverse:NoNeedInverse);
	}
}

void MRP1M03Display(void)
{
	if(OledExternalDataArr[29] >= 3)
	{
		SetDisplayChar(16, 4, Font_M, OledCtrl.PageCtrl.InverseLocation==4?NeedInverse:NoNeedInverse);
		SetDisplayChar(24, 4, DightFont[0]+0, OledCtrl.PageCtrl.InverseLocation==4?NeedInverse:NoNeedInverse);
		SetDisplayChar(32, 4, DightFont[0]+3, OledCtrl.PageCtrl.InverseLocation==4?NeedInverse:NoNeedInverse);
	}
}

void MRP1M04Display(void)
{
	if(OledExternalDataArr[29] >= 4)
	{
		SetDisplayChar(48, 4, Font_M, OledCtrl.PageCtrl.InverseLocation==5?NeedInverse:NoNeedInverse);
		SetDisplayChar(56, 4, DightFont[0]+0, OledCtrl.PageCtrl.InverseLocation==5?NeedInverse:NoNeedInverse);
		SetDisplayChar(64, 4, DightFont[0]+4, OledCtrl.PageCtrl.InverseLocation==5?NeedInverse:NoNeedInverse);
	}
}

void MRP1M05Display(void)
{
	if(OledExternalDataArr[29] >= 5)
	{
		SetDisplayChar(80, 4, Font_M, OledCtrl.PageCtrl.InverseLocation==6?NeedInverse:NoNeedInverse);
		SetDisplayChar(88, 4, DightFont[0]+0, OledCtrl.PageCtrl.InverseLocation==6?NeedInverse:NoNeedInverse);
		SetDisplayChar(96, 4, DightFont[0]+5, OledCtrl.PageCtrl.InverseLocation==6?NeedInverse:NoNeedInverse);
	}
}

void MRP1M06Display(void)
{
	if(OledExternalDataArr[29] > 6)
	{
		SetDisplayChar(16, 6, Font_M, OledCtrl.PageCtrl.InverseLocation==7?NeedInverse:NoNeedInverse);
		SetDisplayChar(24, 6, DightFont[0]+0, OledCtrl.PageCtrl.InverseLocation==7?NeedInverse:NoNeedInverse);
		SetDisplayChar(32, 6, DightFont[0]+6, OledCtrl.PageCtrl.InverseLocation==7?NeedInverse:NoNeedInverse);
	}
}

void MRP1M07Display(void)
{
	if(OledExternalDataArr[29] >= 7)
	{
		SetDisplayChar(48, 6, Font_M, OledCtrl.PageCtrl.InverseLocation==8?NeedInverse:NoNeedInverse);
		SetDisplayChar(56, 6, DightFont[0]+0, OledCtrl.PageCtrl.InverseLocation==8?NeedInverse:NoNeedInverse);
		SetDisplayChar(64, 6, DightFont[0]+7, OledCtrl.PageCtrl.InverseLocation==8?NeedInverse:NoNeedInverse);
	}
}

void MRP1M08Display(void)
{
	if(OledExternalDataArr[29] >= 8)
	{
		SetDisplayChar(80, 6, Font_M, OledCtrl.PageCtrl.InverseLocation==9?NeedInverse:NoNeedInverse);
		SetDisplayChar(88, 6, DightFont[0]+0, OledCtrl.PageCtrl.InverseLocation==9?NeedInverse:NoNeedInverse);
		SetDisplayChar(96, 6, DightFont[0]+8, OledCtrl.PageCtrl.InverseLocation==9?NeedInverse:NoNeedInverse);
	}
}

void RESETDisplay(void)
{
	SetDisplayChar(40, 0, Font_R, 0);
	SetDisplayChar(48, 0, Font_E, 0);
	SetDisplayChar(56, 0, Font_S, 0);
	SetDisplayChar(64, 0, Font_E, 0);
	SetDisplayChar(72, 0, Font_T, 0);
}


/***************************复位页2显示函数****************************/
void MRP1M09Display(void)
{
	if(OledExternalDataArr[29] >= 9)
	{
		SetDisplayChar(16, 2, Font_M, OledCtrl.PageCtrl.InverseLocation==1?NeedInverse:NoNeedInverse);
		SetDisplayChar(24, 2, DightFont[0]+0, OledCtrl.PageCtrl.InverseLocation==1?NeedInverse:NoNeedInverse);
		SetDisplayChar(32, 2, DightFont[0]+9, OledCtrl.PageCtrl.InverseLocation==1?NeedInverse:NoNeedInverse);
	}
}

void MRP1M10Display(void)
{
	if(OledExternalDataArr[29] >= 10)
	{
		SetDisplayChar(48, 2, Font_M, OledCtrl.PageCtrl.InverseLocation==2?NeedInverse:NoNeedInverse);
		SetDisplayChar(56, 2, DightFont[0]+1, OledCtrl.PageCtrl.InverseLocation==2?NeedInverse:NoNeedInverse);
		SetDisplayChar(64, 2, DightFont[0]+0, OledCtrl.PageCtrl.InverseLocation==2?NeedInverse:NoNeedInverse);
	}
}


/***************************通讯模块信息页1显示函数****************************/
void CSDisplay(void)
{
	if(OledCtrl.Flag.Bits.AllUpdateFlag)
	{
		SetDisplayChar(16, 0, Font_C, 0);
		SetDisplayChar(24, 0, Font_S, 0);
		SetDisplayChar(32, 0, Font_Colon, 0);
	}
	
	if(OledExternalDataArr[7])
	{
		SetDisplayChar(40, 0, Font_E, 0);
		SetDisplayChar(48, 0, Font_R, 0);
		SetDisplayChar(56, 0, Font_R, 0);	
	}
	else
	{
		SetDisplayChar(40, 0, Font_R, 0);
		SetDisplayChar(48, 0, Font_U, 0);
		SetDisplayChar(56, 0, Font_N, 0);	
	}
}

void IDDisplay(void)
{
	if(OledCtrl.Flag.Bits.AllUpdateFlag)
	{
		SetDisplayChar(80, 0, Font_I, 0);
		SetDisplayChar(88, 0, Font_D, 0);
		SetDisplayChar(96, 0, Font_Colon, 0);
		DisplayDigit(104, 0, OledExternalDataArr[30], 2);
	}
}

void RTDisplay(void)
{
	DisplayDigit(24, 4, (OledExternalDataArr[31]&0xfff0)>>4, 4);
	DisplayDigit(64, 4, OledExternalDataArr[31]&0x000f, 2);
	DisplayDigit(88, 4, (OledExternalDataArr[32]&0xff00)>>8, 2);
	DisplayDigit(40, 6, OledExternalDataArr[32]&0x00ff, 2);
	DisplayDigit(64, 6, (OledExternalDataArr[33]&0xff00)>>8, 2);
	
	if(OledCtrl.Flag.Bits.AllUpdateFlag)
	{
		SetDisplayChar(0, 4, Font_R, 0);
		SetDisplayChar(8, 4, Font_T, 0);
		SetDisplayChar(16, 4, Font_Colon, 0);
		
		SetDisplayChar(56, 4, Font_Bar, 0);
		SetDisplayChar(80, 4, Font_Bar, 0);
		SetDisplayChar(56, 6, Font_Colon, 0);
	}
}


/***************************通讯模块信息页2显示函数****************************/
void SDDisplay(void)
{
	if(OledCtrl.Flag.Bits.AllUpdateFlag)
	{
		SetDisplayChar(40, 0, Font_S, 0);
		SetDisplayChar(48, 0, Font_D, 0);
		SetDisplayChar(56, 0, Font_Colon, 0);
	}
	DisplayDigit(64, 0, OledExternalDataArr[34], 2);//扰动深度
}

void STDisplay(void)
{
	DisplayDigit(24, 4, (OledExternalDataArr[35]&0xfff0)>>4, 4);	//年
	DisplayDigit(64, 4, OledExternalDataArr[35]&0x000f, 2);	//月
	DisplayDigit(88, 4, (OledExternalDataArr[36]&0xff00)>>8, 2);	//日
	DisplayDigit(32, 6, OledExternalDataArr[36]&0x00ff, 2);	//时
	DisplayDigit(56, 6, (OledExternalDataArr[37]&0xff00)>>8, 2);	//分
	DisplayDigit(80, 6, OledExternalDataArr[37]&0x00ff, 2);	//秒
	
	if(OledCtrl.Flag.Bits.AllUpdateFlag)
	{
		SetDisplayChar(0, 4, Font_S, 0);
		SetDisplayChar(8, 4, Font_T, 0);
		SetDisplayChar(16, 4, Font_Colon, 0);
		
		SetDisplayChar(56, 4, Font_Bar, 0);
		SetDisplayChar(80, 4, Font_Bar, 0);
		SetDisplayChar(48, 6, Font_Colon, 0);
		SetDisplayChar(72, 6, Font_Colon, 0);
	}
}


/***************************通讯模块信息页3显示函数****************************/
void REGDisplay(void)
{
	uint16_t i=0;
	
	if(OledCtrl.Flag.Bits.AllUpdateFlag)
	{
		SetDisplayChar(48, 0, Font_R, 0);
		SetDisplayChar(56, 0, Font_E, 0);
		SetDisplayChar(64, 0, Font_G, 0);
	}
	
	for(i=0; i<16; i++)
	{
		if((OledExternalDataArr[28]>>i) & 0x01)
		{
			SetDisplayChar(i*8, 2, DightFont[0]+1, 0);
		}
		else
		{
			SetDisplayChar(i*8, 2, DightFont[0]+0, 0);
		}
	}
}


/***************************功率模块信息页1显示函数****************************/
void REG1Display(void)
{
	uint16_t i=0;
	uint16_t FaultReg = 0x00;
	
	if(OledCtrl.Flag.Bits.AllUpdateFlag)
	{
		SetDisplayChar(48, 0, Font_R, 0);
		SetDisplayChar(56, 0, Font_E, 0);
		SetDisplayChar(64, 0, Font_G, 0);
		SetDisplayChar(72, 0, DightFont[0]+1, 0);
	}

	if(OledCtrl.PageCtrl.PageIndex.Bits.CurrentIndex == ModuleSeletionPage2Index)
	{
		FaultReg = OledExternalDataArr[OledCtrl.PageCtrl.InverseLocation+12];
	}
	else
	{
		FaultReg = OledExternalDataArr[OledCtrl.PageCtrl.InverseLocation+6];
	}
	
	for(i=0; i<16; i++)
	{
		if((FaultReg>>i) & 0x01)
		{
			SetDisplayChar(i*8, 2, DightFont[0]+1, 0);
		}
		else
		{
			SetDisplayChar(i*8, 2, DightFont[0]+0, 0);
		}
	}
}

void REG2Display(void)
{
	uint16_t i=0;
	uint16_t FaultReg = 0x00;
	
	if(OledCtrl.Flag.Bits.AllUpdateFlag)
	{
		SetDisplayChar(48, 4, Font_R, 0);
		SetDisplayChar(56, 4, Font_E, 0);
		SetDisplayChar(64, 4, Font_G, 0);
		SetDisplayChar(72, 4, DightFont[0]+2, 0);
	}
	
	if(OledCtrl.PageCtrl.PageIndex.Bits.CurrentIndex == ModuleSeletionPage2Index)
	{
		FaultReg = OledExternalDataArr[OledCtrl.PageCtrl.InverseLocation+22];
	}
	else
	{
		FaultReg = OledExternalDataArr[OledCtrl.PageCtrl.InverseLocation+16];
	}
	
	for(i=0; i<16; i++)
	{
		if((FaultReg>>i) & 0x01)
		{
			SetDisplayChar(i*8, 6, DightFont[0]+1, 0);
		}
		else
		{
			SetDisplayChar(i*8, 6, DightFont[0]+0, 0);
		}
	}
}


/*********************************各页面项目表**************************************/

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
		{MSP1M01Display, NULL, PowerModInforPage1Index},
		{MSP1M02Display, NULL, PowerModInforPage1Index},
		{MSP1M03Display, NULL, PowerModInforPage1Index},
		{MSP1M04Display, NULL, PowerModInforPage1Index},
		{MSP1M05Display, NULL, PowerModInforPage1Index},
		{MSP1M06Display, NULL, PowerModInforPage1Index},
		{MODULEDisplay,  NULL, PowerModInforPage1Index},
	};

	struct ITEMSTRUCT ModuleSeletionPage2[] = {
		{MSP1M07Display, NULL, PowerModInforPage1Index},
		{MSP1M08Display, NULL, PowerModInforPage1Index},
		{MSP1M09Display, NULL, PowerModInforPage1Index},
		{MSP1M10Display, NULL, PowerModInforPage1Index},
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

	
/***********************************各页面索引表**************************************/
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
