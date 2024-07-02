#include "stm32f10x.h"
#include "myiic.h"
#include "FDC2X14.h"
#include "timer.h"
#include "delay.h"
#include "lcd.h"
#include "LCDInfo.h"
#include "stdio.h"
#include "usart.h"
#include "Ano.h"
#include "task.h"
#include "init.h"
#include "led.h"
u8 Instruction=0;
void BSP_Init()
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2,必须放在各种初始化函数最前面
	SOFTWARE_Init();
	HARDWARE_Init();
	delay_ms(10);
	Calibrate();
}
 int main(void)
 {	
	 BSP_Init();
	 LCD_Clear(WHITE);
	 LCD_ShowString(30,320,200,24,24,"0000");
	 LCD_ShowString(30,10,200,24,24,"Initialize OK");
	 while(1)
	 {
		if(ShowFlag==1)
		{
			GPIO_SetBits(GPIOB,GPIO_Pin_5);
			delay_ms(500);
			GPIO_ResetBits(GPIOB,GPIO_Pin_5);
			delay_ms(500);
			ShowFlag=0;
			LcdShowData();
		}
		
		switch(Instruction)
		{
			case 0x01:
				Task1();
				break;
			case 0x02:
				Task2();
				break;
			case 0x03:
				Task3();
				break;
			case 0x04:
				Task4();
				break;
			default:
				break;
		} 
	}
}
 
