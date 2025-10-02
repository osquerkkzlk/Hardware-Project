#include "stm32f10x.h"                  // Device header
#include "Delay.h"
void MYI2C_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_OD;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	GPIO_SetBits(GPIOB,GPIO_Pin_10 | GPIO_Pin_11);
}
//基本函数封装
void MYI2C_W_SDA(uint8_t value)
{
	GPIO_WriteBit(GPIOB,GPIO_Pin_11,(BitAction)value);
	Delay_us(10);
}
uint16_t MYI2C_R_SDA(void)
{
	uint16_t value;
	value=GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11);
	Delay_us(10);
	return value;
}
void MYI2C_W_SCL(uint8_t value)
{
	GPIO_WriteBit(GPIOB,GPIO_Pin_10,(BitAction)value);
	Delay_us(10);
}
uint16_t MYI2C_R_SCL(void)
{
	uint16_t value;
	value=GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_10);
	Delay_us(10);
	return value;
}
//I2C时序基本单元
void MYI2C_Start(void)
{
	MYI2C_W_SCL(1);		
	MYI2C_W_SDA(1);
	MYI2C_W_SDA(0);
	MYI2C_W_SCL(0);
}
void MYI2C_End(void)
{
	MYI2C_W_SDA(0);
	MYI2C_W_SCL(1);
	MYI2C_W_SDA(1);
}
void MYI2C_SendData(void)
{
	uint16_t data;
	for(uint8_t i=0;i<8;i++)
	{
		MYI2C_W_SDA(data & (0x80>>i));
		MYI2C_W_SCL(1);
		Delay_us(10);
		MYI2C_W_SCL(0);
	}
}
uint16_t MYI2C_ReceiveData(void)
{
	uint16_t Byte=0x00;
	MYI2C_W_SDA(1);
	for ( uint8_t i=0; i<8; i++)
	{
		MYI2C_W_SCL(1);
		uint8_t value =MYI2C_R_SDA();
		if(value==1){Byte=(Byte>>i) |value;}
		MYI2C_W_SCL(0);
	}
	return Byte;
}
void MYI2C_Send_ACK(uint8_t ACK_bit)
{
	MYI2C_W_SDA(ACK_bit);
	MYI2C_W_SCL(1);
	Delay_us(10);
	MYI2C_W_SCL(0);

}
uint8_t  MYI2C_Receive_ACK(void)
{
	MYI2C_W_SDA(1);
	MYI2C_W_SCL(1);
	uint8_t ACK_bit =MYI2C_R_SDA();
	Delay_us(10);
	MYI2C_W_SCL(0);
	return ACK_bit;
}
