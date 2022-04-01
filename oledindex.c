#include "oledindex.h"
#include "oledfont.h"

struct OLEDPAGESTRUCT OledPageTable[] = {0};
struct ITEMSTRUCT ItemTable[] = {0};

struct OLEDCTRL OledCtrl;

uint16_t OledExternalDataArr[20] = {220,221,222,100,101,102,1,1,0x3a3a,0xc5c5,0x5555,10,0x0f,0x7E54,0x010c,0x1427,31,0x7E64,0x010c,0x1427}; //OLED需要的外部数据
/*
OledExternalDataArr[0]:A相电压；
OledExternalDataArr[1]:B相电压；
OledExternalDataArr[2]:C相电压；
OledExternalDataArr[3]:A相电流；
OledExternalDataArr[4]:B相电流；
OledExternalDataArr[5]:C相电流；
OledExternalDataArr[6]:设备运行状态
OledExternalDataArr[7]:通讯模块运行状态
OledExternalDataArr[8]:功率模块故障寄存器1
OledExternalDataArr[9]:功率模块故障寄存器2
OledExternalDataArr[10]:通讯模块故障寄存器1
OledExternalDataArr[11]:模块并联数
OledExternalDataArr[12]:通讯地址
OledExternalDataArr[13]:RTC实时时间年月
OledExternalDataArr[14]:RTC实时时间日时
OledExternalDataArr[15]:RTC实时时间分秒
OledExternalDataArr[16]:上次扰动的扰动深度
OledExternalDataArr[17]:上次扰动录波的发生年月
OledExternalDataArr[18]:上次扰动录波的发生日时
OledExternalDataArr[19]:上次扰动录波的发生分秒
*/
uint16_t LocationArr[3] = {0}; //保存坐标的数组
uint16_t DightArr[5];

/*******************************************************************************
* 函数功能:	坐标插入链表
* 输入参数: 
* 输出参数: 无
* 函数说明: 无
*******************************************************************************/
void SetLocation(uint16_t x, uint16_t y)
{
	LocationArr[0] = 0xb0+y;
	LocationArr[1] = ((x&0xf0)>>4)|0x10;
	LocationArr[2] = x&0x0f;
	DataInsertLinklist(OledCtrl.LinklistHead, LocationArr, 3, NoNeedInserve, OledCommand);
}

/*******************************************************************************
* 函数功能:	字符插入链表
* 输入参数: 
* 输出参数: 无
* 函数说明: 无
*******************************************************************************/
void SetDisplayChar(uint16_t x, uint16_t y, uint16_t *Point, uint16_t InverseFlag)
{
	SetLocation(x,y);
	DataInsertLinklist(OledCtrl.LinklistHead, Point, 8, InverseFlag, OledData);
	SetLocation(x,y+1);
	DataInsertLinklist(OledCtrl.LinklistHead, Point+8, 8, InverseFlag, OledData);
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
	
	DightArr[0] = (Digit%10);
	DightArr[1] = ((Digit/10)%10);	
	DightArr[2] = (Digit/100)%10;
	DightArr[3] = (Digit/1000)%10;
	DightArr[4] = (Digit/10000);
	
	for(i=0; i<BitNum; i++)
	{
		if(DightArr[BitNum-i-1] == 0)
			SetDisplayChar(x+8*i, y, Font_0, NoNeedInserve);
		else if(DightArr[BitNum-i-1] == 1)
			SetDisplayChar(x+8*i, y, Font_1, NoNeedInserve);
		else if(DightArr[BitNum-i-1] == 2)
			SetDisplayChar(x+8*i, y, Font_2, NoNeedInserve);
		else if(DightArr[BitNum-i-1] == 3)
			SetDisplayChar(x+8*i, y, Font_3, NoNeedInserve);
		else if(DightArr[BitNum-i-1] == 4)
			SetDisplayChar(x+8*i, y, Font_4, NoNeedInserve);
		else if(DightArr[BitNum-i-1] == 5)
			SetDisplayChar(x+8*i, y, Font_5, NoNeedInserve);
		else if(DightArr[BitNum-i-1] == 6)
			SetDisplayChar(x+8*i, y, Font_6, NoNeedInserve);
		else if(DightArr[BitNum-i-1] == 7)
			SetDisplayChar(x+8*i, y, Font_7, NoNeedInserve);
		else if(DightArr[BitNum-i-1] == 8)
			SetDisplayChar(x+8*i, y, Font_8, NoNeedInserve);
		else if(DightArr[BitNum-i-1] == 9)
			SetDisplayChar(x+8*i, y, Font_9, NoNeedInserve);
			
	}
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
	
	SetLocation(0,0);
	for(i=0; i<=8; i++)
	{
		DataInsertLinklist(OledCtrl.LinklistHead, OledClearScreenArr, 128, InverseFlag, OledData);
	}
}

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
/*	
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
*/
	if(OledExternalDataArr[8]|OledExternalDataArr[9])
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
/*	
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
*/
	if(OledExternalDataArr[10])
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
	SetDisplayChar(8, 2, Font_C, OledCtrl.PageCtrl.InverseLocation==1?NeedInverse:NoNeedInserve);
	SetDisplayChar(16, 2, Font_M, OledCtrl.PageCtrl.InverseLocation==1?NeedInverse:NoNeedInserve);
	if(OledExternalDataArr[10])
	{
		SetDisplayChar(24, 2, Font_Asterisk, OledCtrl.PageCtrl.InverseLocation==1?NeedInverse:NoNeedInserve);
	}
	else
	{
		SetDisplayChar(24, 2, Font_Space, OledCtrl.PageCtrl.InverseLocation==1?NeedInverse:NoNeedInserve);
	}
}

void MSP1M01Display(void)
{
	if(OledExternalDataArr[11] >= 1)
	{
		SetDisplayChar(8, 4, Font_M, OledCtrl.PageCtrl.InverseLocation==2?NeedInverse:NoNeedInserve);
		SetDisplayChar(16, 4, Font_0, OledCtrl.PageCtrl.InverseLocation==2?NeedInverse:NoNeedInserve);
		SetDisplayChar(24, 4, Font_1, OledCtrl.PageCtrl.InverseLocation==2?NeedInverse:NoNeedInserve);
	}
}

void MSP1M02Display(void)
{
	if(OledExternalDataArr[11] >= 2)
	{
		SetDisplayChar(48, 4, Font_M, OledCtrl.PageCtrl.InverseLocation==3?NeedInverse:NoNeedInserve);
		SetDisplayChar(56, 4, Font_0, OledCtrl.PageCtrl.InverseLocation==3?NeedInverse:NoNeedInserve);
		SetDisplayChar(64, 4, Font_2, OledCtrl.PageCtrl.InverseLocation==3?NeedInverse:NoNeedInserve);
	}
}

void MSP1M03Display(void)
{
	if(OledExternalDataArr[11] >= 3)
	{
		SetDisplayChar(88, 4, Font_M, OledCtrl.PageCtrl.InverseLocation==4?NeedInverse:NoNeedInserve);
		SetDisplayChar(96, 4, Font_0, OledCtrl.PageCtrl.InverseLocation==4?NeedInverse:NoNeedInserve);
		SetDisplayChar(104, 4, Font_3, OledCtrl.PageCtrl.InverseLocation==4?NeedInverse:NoNeedInserve);
	}
}

void MSP1M04Display(void)
{
	if(OledExternalDataArr[11] >= 4)
	{
		SetDisplayChar(8, 6, Font_M, OledCtrl.PageCtrl.InverseLocation==5?NeedInverse:NoNeedInserve);
		SetDisplayChar(16, 6, Font_0, OledCtrl.PageCtrl.InverseLocation==5?NeedInverse:NoNeedInserve);
		SetDisplayChar(24, 6, Font_4, OledCtrl.PageCtrl.InverseLocation==5?NeedInverse:NoNeedInserve);
	}
}

void MSP1M05Display(void)
{
	if(OledExternalDataArr[11] >= 5)
	{
		SetDisplayChar(48, 6, Font_M, OledCtrl.PageCtrl.InverseLocation==6?NeedInverse:NoNeedInserve);
		SetDisplayChar(56, 6, Font_0, OledCtrl.PageCtrl.InverseLocation==6?NeedInverse:NoNeedInserve);
		SetDisplayChar(64, 6, Font_5, OledCtrl.PageCtrl.InverseLocation==6?NeedInverse:NoNeedInserve);
	}
}

void MSP1M06Display(void)
{
	if(OledExternalDataArr[11] > 6)
	{
		SetDisplayChar(88, 6, Font_M, OledCtrl.PageCtrl.InverseLocation==7?NeedInverse:NoNeedInserve);
		SetDisplayChar(96, 6, Font_0, OledCtrl.PageCtrl.InverseLocation==7?NeedInverse:NoNeedInserve);
		SetDisplayChar(104, 6, Font_6, OledCtrl.PageCtrl.InverseLocation==7?NeedInverse:NoNeedInserve);
	}
}

void MODULEDisplay(void)
{
	SetDisplayChar(40, 0, Font_M, NoNeedInserve);
	SetDisplayChar(48, 0, Font_O, NoNeedInserve);
	SetDisplayChar(56, 0, Font_D, NoNeedInserve);
	SetDisplayChar(64, 0, Font_U, NoNeedInserve);
	SetDisplayChar(72, 0, Font_L, NoNeedInserve);
	SetDisplayChar(80, 0, Font_E, NoNeedInserve);
}

/***************************模块选择页2显示函数***************************/
void MSP1M07Display(void)
{
	if(OledExternalDataArr[11] >= 7)
	{
		SetDisplayChar(8, 2, Font_M, OledCtrl.PageCtrl.InverseLocation==1?NeedInverse:NoNeedInserve);
		SetDisplayChar(16, 2, Font_0, OledCtrl.PageCtrl.InverseLocation==1?NeedInverse:NoNeedInserve);
		SetDisplayChar(24, 2, Font_7, OledCtrl.PageCtrl.InverseLocation==1?NeedInverse:NoNeedInserve);
	}
}

void MSP1M08Display(void)
{
	if(OledExternalDataArr[11] >= 8)
	{
		SetDisplayChar(48, 2, Font_M, OledCtrl.PageCtrl.InverseLocation==2?NeedInverse:NoNeedInserve);
		SetDisplayChar(56, 2, Font_0, OledCtrl.PageCtrl.InverseLocation==2?NeedInverse:NoNeedInserve);
		SetDisplayChar(64, 2, Font_8, OledCtrl.PageCtrl.InverseLocation==2?NeedInverse:NoNeedInserve);
	}
}

void MSP1M09Display(void)
{
	if(OledExternalDataArr[11] >= 9)
	{
		SetDisplayChar(88, 2, Font_M, OledCtrl.PageCtrl.InverseLocation==3?NeedInverse:NoNeedInserve);
		SetDisplayChar(96, 2, Font_0, OledCtrl.PageCtrl.InverseLocation==3?NeedInverse:NoNeedInserve);
		SetDisplayChar(104, 2, Font_9, OledCtrl.PageCtrl.InverseLocation==3?NeedInverse:NoNeedInserve);
	}
}

void MSP1M10Display(void)
{
	if(OledExternalDataArr[11] >= 10)
	{
		SetDisplayChar(8, 4, Font_M, OledCtrl.PageCtrl.InverseLocation==4?NeedInverse:NoNeedInserve);
		SetDisplayChar(16, 4, Font_1, OledCtrl.PageCtrl.InverseLocation==4?NeedInverse:NoNeedInserve);
		SetDisplayChar(24, 4, Font_0, OledCtrl.PageCtrl.InverseLocation==4?NeedInverse:NoNeedInserve);
	}
}

/***************************复位页1显示函数****************************/
void ALLDisplay(void)
{
	SetDisplayChar(16, 2, Font_A, OledCtrl.PageCtrl.InverseLocation==1?NeedInverse:NoNeedInserve);
	SetDisplayChar(24, 2, Font_L, OledCtrl.PageCtrl.InverseLocation==1?NeedInverse:NoNeedInserve);
	SetDisplayChar(32, 2, Font_L, OledCtrl.PageCtrl.InverseLocation==1?NeedInverse:NoNeedInserve);
}

void MRP1M01Display(void)
{
	if(OledExternalDataArr[11] >= 1)
	{
		SetDisplayChar(48, 2, Font_M, OledCtrl.PageCtrl.InverseLocation==2?NeedInverse:NoNeedInserve);
		SetDisplayChar(56, 2, Font_0, OledCtrl.PageCtrl.InverseLocation==2?NeedInverse:NoNeedInserve);
		SetDisplayChar(64, 2, Font_1, OledCtrl.PageCtrl.InverseLocation==2?NeedInverse:NoNeedInserve);
	}
}

void MRP1M02Display(void)
{
	if(OledExternalDataArr[11] >= 2)
	{
		SetDisplayChar(80, 2, Font_M, OledCtrl.PageCtrl.InverseLocation==3?NeedInverse:NoNeedInserve);
		SetDisplayChar(88, 2, Font_0, OledCtrl.PageCtrl.InverseLocation==3?NeedInverse:NoNeedInserve);
		SetDisplayChar(96, 2, Font_2, OledCtrl.PageCtrl.InverseLocation==3?NeedInverse:NoNeedInserve);
	}
}

void MRP1M03Display(void)
{
	if(OledExternalDataArr[11] >= 3)
	{
		SetDisplayChar(16, 4, Font_M, OledCtrl.PageCtrl.InverseLocation==4?NeedInverse:NoNeedInserve);
		SetDisplayChar(24, 4, Font_0, OledCtrl.PageCtrl.InverseLocation==4?NeedInverse:NoNeedInserve);
		SetDisplayChar(32, 4, Font_3, OledCtrl.PageCtrl.InverseLocation==4?NeedInverse:NoNeedInserve);
	}
}

void MRP1M04Display(void)
{
	if(OledExternalDataArr[11] >= 4)
	{
		SetDisplayChar(48, 4, Font_M, OledCtrl.PageCtrl.InverseLocation==5?NeedInverse:NoNeedInserve);
		SetDisplayChar(56, 4, Font_0, OledCtrl.PageCtrl.InverseLocation==5?NeedInverse:NoNeedInserve);
		SetDisplayChar(64, 4, Font_4, OledCtrl.PageCtrl.InverseLocation==5?NeedInverse:NoNeedInserve);
	}
}

void MRP1M05Display(void)
{
	if(OledExternalDataArr[11] >= 5)
	{
		SetDisplayChar(80, 4, Font_M, OledCtrl.PageCtrl.InverseLocation==6?NeedInverse:NoNeedInserve);
		SetDisplayChar(88, 4, Font_0, OledCtrl.PageCtrl.InverseLocation==6?NeedInverse:NoNeedInserve);
		SetDisplayChar(96, 4, Font_5, OledCtrl.PageCtrl.InverseLocation==6?NeedInverse:NoNeedInserve);
	}
}

void MRP1M06Display(void)
{
	if(OledExternalDataArr[11] > 6)
	{
		SetDisplayChar(16, 6, Font_M, OledCtrl.PageCtrl.InverseLocation==7?NeedInverse:NoNeedInserve);
		SetDisplayChar(24, 6, Font_0, OledCtrl.PageCtrl.InverseLocation==7?NeedInverse:NoNeedInserve);
		SetDisplayChar(32, 6, Font_6, OledCtrl.PageCtrl.InverseLocation==7?NeedInverse:NoNeedInserve);
	}
}

void MRP1M07Display(void)
{
	if(OledExternalDataArr[11] >= 7)
	{
		SetDisplayChar(48, 6, Font_M, OledCtrl.PageCtrl.InverseLocation==8?NeedInverse:NoNeedInserve);
		SetDisplayChar(56, 6, Font_0, OledCtrl.PageCtrl.InverseLocation==8?NeedInverse:NoNeedInserve);
		SetDisplayChar(64, 6, Font_7, OledCtrl.PageCtrl.InverseLocation==8?NeedInverse:NoNeedInserve);
	}
}

void MRP1M08Display(void)
{
	if(OledExternalDataArr[11] >= 8)
	{
		SetDisplayChar(80, 6, Font_M, OledCtrl.PageCtrl.InverseLocation==9?NeedInverse:NoNeedInserve);
		SetDisplayChar(88, 6, Font_0, OledCtrl.PageCtrl.InverseLocation==9?NeedInverse:NoNeedInserve);
		SetDisplayChar(96, 6, Font_8, OledCtrl.PageCtrl.InverseLocation==9?NeedInverse:NoNeedInserve);
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
	if(OledExternalDataArr[11] >= 9)
	{
		SetDisplayChar(16, 2, Font_M, OledCtrl.PageCtrl.InverseLocation==1?NeedInverse:NoNeedInserve);
		SetDisplayChar(24, 2, Font_0, OledCtrl.PageCtrl.InverseLocation==1?NeedInverse:NoNeedInserve);
		SetDisplayChar(32, 2, Font_9, OledCtrl.PageCtrl.InverseLocation==1?NeedInverse:NoNeedInserve);
	}
}

void MRP1M10Display(void)
{
	if(OledExternalDataArr[11] >= 10)
	{
		SetDisplayChar(48, 2, Font_M, OledCtrl.PageCtrl.InverseLocation==2?NeedInverse:NoNeedInserve);
		SetDisplayChar(56, 2, Font_1, OledCtrl.PageCtrl.InverseLocation==2?NeedInverse:NoNeedInserve);
		SetDisplayChar(64, 2, Font_0, OledCtrl.PageCtrl.InverseLocation==2?NeedInverse:NoNeedInserve);
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
		DisplayDigit(104, 0, OledExternalDataArr[12], 2);
	}
}

void RTDisplay(void)
{
	DisplayDigit(24, 4, (OledExternalDataArr[13]&0xfff0)>>4, 4);
	DisplayDigit(64, 4, OledExternalDataArr[13]&0x000f, 2);
	DisplayDigit(88, 4, (OledExternalDataArr[14]&0xff00)>>8, 2);
	DisplayDigit(40, 6, OledExternalDataArr[14]&0x00ff, 2);
	DisplayDigit(64, 6, (OledExternalDataArr[15]&0xff00)>>8, 2);
	
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
	DisplayDigit(64, 0, OledExternalDataArr[16], 2);//扰动深度
}

void STDisplay(void)
{
	DisplayDigit(24, 4, (OledExternalDataArr[17]&0xfff0)>>4, 4);	//年
	DisplayDigit(64, 4, OledExternalDataArr[17]&0x000f, 2);	//月
	DisplayDigit(88, 4, (OledExternalDataArr[18]&0xff00)>>8, 2);	//日
	DisplayDigit(32, 6, OledExternalDataArr[18]&0x00ff, 2);	//时
	DisplayDigit(56, 6, (OledExternalDataArr[19]&0xff00)>>8, 2);	//分
	DisplayDigit(80, 6, OledExternalDataArr[19]&0x00ff, 2);	//秒
	
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
		if((OledExternalDataArr[10]>>i) & 0x01)
		{
			SetDisplayChar(i*8, 2, Font_1, 0);
		}
		else
		{
			SetDisplayChar(i*8, 2, Font_0, 0);
		}
	}
}

/***************************功率模块信息页1显示函数****************************/
void REG1Display(void)
{
	uint16_t i=0;
	
	if(OledCtrl.Flag.Bits.AllUpdateFlag)
	{
		SetDisplayChar(48, 0, Font_R, 0);
		SetDisplayChar(56, 0, Font_E, 0);
		SetDisplayChar(64, 0, Font_G, 0);
		SetDisplayChar(72, 0, Font_1, 0);
	}
	
	for(i=0; i<16; i++)
	{
		if((OledExternalDataArr[8]>>i) & 0x01)
		{
			SetDisplayChar(i*8, 2, Font_1, 0);
		}
		else
		{
			SetDisplayChar(i*8, 2, Font_0, 0);
		}

	}
}

void REG2Display(void)
{
	uint16_t i=0;
	
	if(OledCtrl.Flag.Bits.AllUpdateFlag)
	{
		SetDisplayChar(48, 4, Font_R, 0);
		SetDisplayChar(56, 4, Font_E, 0);
		SetDisplayChar(64, 4, Font_G, 0);
		SetDisplayChar(72, 4, Font_2, 0);
	}
	
	for(i=0; i<16; i++)
	{
		if((OledExternalDataArr[9]>>i) & 0x01)
		{
			SetDisplayChar(i*8, 6, Font_1, 0);
		}
		else
		{
			SetDisplayChar(i*8, 6, Font_0, 0);
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
