/*
 * olediic.c
 *
 *  Created on: 2022-03-23
 *      Author: WangJianLong
 */

#include "olediic.h"

/*******************************************************************************
 *��������:OLED��Ӳ��I2C��ʼ������
 *�������:��
 *�������:��
 *����˵��:
*******************************************************************************/
void I2C1_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	I2C_InitTypeDef I2C_InitStructure;
	
	//ʹ��IICʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
	
	//IIC��GPIO��ʼ��
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;//��������
	GPIO_Init(GPIOB, &GPIO_InitStructure);
		
	//���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource8, GPIO_AF_I2C1);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_I2C1);

	I2C_SoftwareResetCmd(I2C1, ENABLE);//��λIIC
	__NOP(); //��ʱ1����е����
	I2C_SoftwareResetCmd(I2C1, DISABLE);//ȡ����λ�����busy
	
	//IIC��������
	I2C_InitStructure.I2C_Mode                = I2C_Mode_I2C;
	I2C_InitStructure.I2C_DutyCycle           = I2C_DutyCycle_2;//����ģʽ�µ�ռ�ձ�
	I2C_InitStructure.I2C_OwnAddress1         = OLEDSLAVEADDR;
	I2C_InitStructure.I2C_Ack                 = I2C_Ack_Enable;
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStructure.I2C_ClockSpeed          = (400 * 1000);  //SCL = 400KHz
	I2C_Init(I2C1, &I2C_InitStructure);	
	
	//ʹ��IIC
	I2C_Cmd(I2C1, ENABLE);
}

/*******************************************************************************
 *��������:	OLED I2C���ͺ���
 *�������:	OledI2CCtrl: olediic���ƽṹ��
 *�������:	
 *����˵��:	
*******************************************************************************/
uint16_t OledI2CSendData(struct OLEDI2CSTRUCT *OledI2CCtrl)
{
	struct OLEDI2CSTRUCT *fd = OledI2CCtrl;
	uint16_t flag = BUSY;
	
	switch(fd->OledI2CState)
	{
		case OledI2cInactive: 
			if(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY) == 0)//�ж�IIC�����Ƿ�æ
			{
				I2C_GenerateSTART(I2C1, ENABLE);
				fd->OledI2CState = OledI2cSlaveAddr;
			}
			return BUSY;
		
		case OledI2cSlaveAddr: 
			if(I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))//�ж�EV5�¼��Ƿ����
			{
				I2C_Send7bitAddress(I2C1, OLEDSLAVEADDR, I2C_Direction_Transmitter);
				fd->OledI2CState = OledI2cRegAddr;
			}
			return BUSY;
		
		case OledI2cRegAddr:
			if(I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))//�ж�EV6�������ͣ��¼��Ƿ����
			{
				flag = I2C1->SR1 ;
				flag = I2C1->SR2;//������ȶ�ȡSR1�Ĵ������ڶ�ȡSR2�Ĵ����������ADDRλ
				I2C_SendData(I2C1, fd->OledI2cDataInfor.Byte.Addr);
				fd->OledI2CState = OledI2cData;
			}
			return BUSY;
		
		case OledI2cData:
			if(I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTING))//�ж�EV8�¼��Ƿ����
			{
				I2C_SendData(I2C1, fd->OledI2cDataInfor.Byte.Data);
				fd->OledI2CState = OledI2cStop;
			}
			return BUSY;			
		
		case OledI2cStop:
			if(I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))//�ж�EV8_2�¼��Ƿ����
			{
				I2C_GenerateSTOP(I2C1, ENABLE);
				fd->OledI2CState = OledI2cInactive;
				return SUCCESS;
			}
			return BUSY;
			
		case OledI2cLoop://IIC���ͳ�ʱ������дѭ��״̬
			return BUSY;
		
		default:
			return BUSY;
	}
}
