#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "MyI2C.h"

uint8_t ID;								//定义用于存放ID号的变量
int16_t AX, AY, AZ, GX, GY, GZ;			//定义用于存放各个数据的变量

int main(void)
{
	MyI2C_Init();
	OLED_Init();
	
	MyI2C_Start();
	MyI2C_SendByte(0xD0);
	uint8_t ack_bit=MyI2C_ReceiveAck();
	OLED_ShowHexNum(1,1,ack_bit,1);
	
}
