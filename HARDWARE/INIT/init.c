#include "init.h"
#include "led.h"
#include "delay.h"
#include "usart.h"
#include "usart2.h"
#include "lcd.h"
#include "FDC2X14.h"
#include "timer.h"
#include "hc05.h"
#include "key.h"


void	HARDWARE_Init(void)
{
	FDC2X14Init();
	LCD_Init();
	//LCD_Clear(WHITE);
	KEY_Init();
	while(HC05_Init()) 		//��ʼ��ATK-HC05ģ��  
	{
		LCD_ShowString(30,90,200,16,16,"ATK-HC05 Error!"); 
		delay_ms(10000);
		LCD_ShowString(30,90,200,16,16,"Please Check!!!"); 
		delay_ms(10000);
	}
	//uart_init(9600);
	LCD_Clear(WHITE);
	LED_Init();
	POINT_COLOR=RED;
}

void SOFTWARE_Init(void)
{
	delay_init();	    	 //��ʱ������ʼ��	  
	uart_init(9600);
	TIM3_Int_Init(7199,9); //��ʱ��10��Ƶ����ʱ�����Ƶ��Ϊ7200000/7200=1000HZ
}
