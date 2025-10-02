#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "AD.h"

uint16_t volatge;
uint16_t power;
uint16_t current;
int main(void)
{
	/*模块初始化*/
//	OLED_Init();				//OLED初始化
//	AD_Init();					//AD初始化
//	
//	/*显示静态字符串*/
//	OLED_ShowString(1, 1, "AD0:");
//	OLED_ShowString(2, 1, "AD1:");
//	OLED_ShowString(3, 1, "AD2:");
//	OLED_ShowString(4, 1, "AD3:");
//	
//	while (1)
//	{
//		OLED_ShowNum(1, 5, AD_Value[0], 4);		//显示转换结果第0个数据
//		OLED_ShowNum(2, 5, AD_Value[1], 4);		//显示转换结果第1个数据
//		OLED_ShowNum(3, 5, AD_Value[2], 4);		//显示转换结果第2个数据
//		OLED_ShowNum(4, 5, AD_Value[3], 4);		//显示转换结果第3个数据
//		
//		Delay_ms(100);							//延时100ms，手动增加一些转换的间隔时间
//	}
		/*模块初始化*/
	OLED_Init();				//OLED初始化
	AD_Init();					//AD初始化
	
	/*显示静态字符串*/
	OLED_ShowString(1, 1, "V:00.00");
	OLED_ShowString(2, 1, "A:00.00");
	OLED_ShowString(3, 1, "W:00.00");
	
	while (1)
	{
		//PA0口测电压
		//PA1口测电流
		volatge=(float)AD_Value[0]/4095*3.3;
		current=(float)AD_Value[1]/4095*3.3;
		power=current*volatge;
		//显示整数部分
		OLED_ShowNum(1, 3, volatge, 2);		//显示转换结果第0个数据
		OLED_ShowNum(2, 3, current, 2);		//显示转换结果第1个数据
		OLED_ShowNum(3, 3, power, 2);		//显示转换结果第2个数据
		
		//显示小数部分
		OLED_ShowNum(1, 6,(uint16_t)(volatge*100)%100, 2);			
		OLED_ShowNum(2, 6, (uint16_t)(current*100)%100, 2);				
		OLED_ShowNum(3, 6, (uint16_t)(power*100)%100, 2);		
		
		
		Delay_us(100);
	}
}
