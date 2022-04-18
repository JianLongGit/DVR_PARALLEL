/*
 * olediic.c
 *
 *  Created on: 2022-03-23
 *      Author: WangJianLong
 */

#include "olediic.h"

/*******************************************************************************
 *函数功能:OLED的硬件I2C初始化函数
 *输入参数:无
 *输出参数:无
 *函数说明:
*******************************************************************************/
void I2C1_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	I2C_InitTypeDef I2C_InitStructure;
	
	//使能IIC时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
	
	//IIC的GPIO初始化
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;//无上下拉
	GPIO_Init(GPIOB, &GPIO_InitStructure);
		
	//引脚复用映射
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource8, GPIO_AF_I2C1);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_I2C1);

	I2C_SoftwareResetCmd(I2C1, ENABLE);//复位IIC
	__NOP(); //延时1个机械周期
	I2C_SoftwareResetCmd(I2C1, DISABLE);//取消复位，清除busy
	
	//IIC参数配置
	I2C_InitStructure.I2C_Mode                = I2C_Mode_I2C;
	I2C_InitStructure.I2C_DutyCycle           = I2C_DutyCycle_2;//高速模式下的占空比
	I2C_InitStructure.I2C_OwnAddress1         = OLEDSLAVEADDR;
	I2C_InitStructure.I2C_Ack                 = I2C_Ack_Enable;
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStructure.I2C_ClockSpeed          = (400 * 1000);  //SCL = 400KHz
	I2C_Init(I2C1, &I2C_InitStructure);	
	
	//使能IIC
	I2C_Cmd(I2C1, ENABLE);
}

/*******************************************************************************
 *函数功能:	OLED I2C发送函数
 *输入参数:	OledI2CCtrl: olediic控制结构体
 *输出参数:	
 *函数说明:	
*******************************************************************************/
uint16_t OledI2CSendData(struct OLEDI2CSTRUCT *OledI2CCtrl)
{
	struct OLEDI2CSTRUCT *fd = OledI2CCtrl;
	uint16_t flag = BUSY;
	
	switch(fd->OledI2CState)
	{
		case OledI2cInactive: 
			if(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY) == 0)//判断IIC总线是否忙
			{
				I2C_GenerateSTART(I2C1, ENABLE);
				fd->OledI2CState = OledI2cSlaveAddr;
			}
			return BUSY;
		
		case OledI2cSlaveAddr: 
			if(I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))//判断EV5事件是否产生
			{
				I2C_Send7bitAddress(I2C1, OLEDSLAVEADDR, I2C_Direction_Transmitter);
				fd->OledI2CState = OledI2cRegAddr;
			}
			return BUSY;
		
		case OledI2cRegAddr:
			if(I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))//判断EV6（主发送）事件是否产生
			{
				flag = I2C1->SR1 ;
				flag = I2C1->SR2;//软件需先读取SR1寄存器，在读取SR2寄存器才能清除ADDR位
				I2C_SendData(I2C1, fd->OledI2cDataInfor.Byte.Addr);
				fd->OledI2CState = OledI2cData;
			}
			return BUSY;
		
		case OledI2cData:
			if(I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTING))//判断EV8事件是否产生
			{
				I2C_SendData(I2C1, fd->OledI2cDataInfor.Byte.Data);
				fd->OledI2CState = OledI2cStop;
			}
			return BUSY;			
		
		case OledI2cStop:
			if(I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))//判断EV8_2事件是否产生
			{
				I2C_GenerateSTOP(I2C1, ENABLE);
				fd->OledI2CState = OledI2cInactive;
				return SUCCESS;
			}
			return BUSY;
			
		case OledI2cLoop://IIC发送超时，进入写循环状态
			return BUSY;
		
		default:
			return BUSY;
	}
}
