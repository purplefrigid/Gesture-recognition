#include "lcd.h"
#include "sys.h"
#include "stdio.h"
#include "FDC2X14.h"

void LcdShowData()
{
	u8 Char_Value[12];			//数字打印为字符串暂存变量
	sprintf((char*)Char_Value,"CH0:%d",ChValue[0]); //将LCD ID打印到lcd_id数组。
	LCD_ShowString(30,50,200,24,24,Char_Value);	
	sprintf((char*)Char_Value,"CH1:%d",ChValue[1]); //将LCD ID打印到lcd_id数组。	
	LCD_ShowString(30,80,200,24,24,Char_Value);
	sprintf((char*)Char_Value,"CH2:%d",ChValue[2]); //将LCD ID打印到lcd_id数组
	LCD_ShowString(30,110,200,24,24,Char_Value);
	sprintf((char*)Char_Value,"CH3:%d",ChValue[3]); //将LCD ID打印到lcd_id数组。	
	LCD_ShowString(30,140,200,24,24,Char_Value);
	sprintf((char*)Char_Value,"CH4:%d",ChValue[4]); //将LCD ID打印到lcd_id数组。
	LCD_ShowString(30,170,200,24,24,Char_Value);	
	sprintf((char*)Char_Value,"CH5:%d",ChValue[5]); //将LCD ID打印到lcd_id数组。	
	LCD_ShowString(30,200,200,24,24,Char_Value);
	sprintf((char*)Char_Value,"CH6:%d",ChValue[6]); //将LCD ID打印到lcd_id数组
	LCD_ShowString(30,230,200,24,24,Char_Value);
	sprintf((char*)Char_Value,"CH7:%d",ChValue[7]); //将LCD ID打印到lcd_id数组。	
	LCD_ShowString(30,260,200,24,24,Char_Value);
	if(FDC2X14ReadCH(1))
	{
		LCD_ShowString(30,320,200,24,24,"0000");
	}
	else
		LCD_ShowString(30,320,200,24,24,"1111");
	//LCD_ShowString(30,320,200,24,24,"FDC2X14ReadCH(1)");
}
void LCDShowRowingSelectInfo()
{
    LCD_ShowString(60,70,200,24,24,"Rowing Mode");	
    LCD_ShowString(30,120,200,24,24,"MODE SELECT");	
    LCD_ShowString(30,180,200,24,24,"PRES K0 TO Train");	
    LCD_ShowString(30,240,200,24,24,"PRES K1 TO Judge");	
}
void LCDShowRowingTrainInfo(u8 mode)
{
    u8 Char_Value[12];
	LCD_ShowString(30,20,200,24,16,"Welcome to Train Mode");	
    LCD_ShowString(30,50,200,24,16,"MODE1:Train One");	
    LCD_ShowString(30,80,200,24,16,"MODE2:Train Two");
    LCD_ShowString(30,110,200,24,16,"MODE3:Train Three");	
    LCD_ShowString(30,140,200,24,16,"MODE4:Train Four");
    LCD_ShowString(30,170,200,24,16,"MODE5:Train Five");	
    LCD_ShowString(30,200,200,24,16,"Current Mode:");
	sprintf((char*)Char_Value,"%d",mode); //将LCD ID打印到lcd_id数组。
	LCD_ShowString(140,200,200,24,16,Char_Value);
    LCD_ShowString(30,230,200,24,12,"PRES K0 to Start Train");	
}
void LCDShowRowingJudgeInfo(u8 result)
{
    LCD_ShowString(10,50,200,24,16,"Put Hand On Sensing Area");	
    LCD_ShowString(10,100,200,24,16,"PRES K0 to Get Result");	
    LCD_ShowString(10,150,200,24,16,"Judge Result: ");
    switch(result)
    {
        case 1:
            LCD_ShowString(80,200,200,24,24,"One    ");
            break;
        case 2:
            LCD_ShowString(80,200,200,24,24,"Two    ");
            break;
        case 3:
            LCD_ShowString(80,200,200,24,24,"Three  ");
            break;
        case 4:
            LCD_ShowString(80,200,200,24,24,"Four   ");
            break;
        case 5:
            LCD_ShowString(80,200,200,24,24,"Five   ");
            break;
		default:
			LCD_ShowString(80,200,200,24,16,"Current No Gesture");
    }
		LCD_ShowString(10,250,200,24,16,"PRES K2 to Exit");	
}
void LCDShowMoraSelectInfo()
{
    LCD_ShowString(60,70,200,24,24,"Mora Mode");	
	LCD_ShowString(30,120,200,24,24,"MODE SELECT");	
    LCD_ShowString(30,180,200,24,24,"PRES K0 TO Train");	
    LCD_ShowString(30,240,200,24,24,"PRES K1 TO Judge");	
}
void LCDShowMoraTrainInfo(u8 mode)
{
	u8 Char_Value[12];
	LCD_ShowString(30,20,200,24,16,"Welcome to Train Mode");	
    LCD_ShowString(30,50,200,24,16,"MODE0:Rock Train");	
    LCD_ShowString(30,80,200,24,16,"MODE1:Scissor Train");
    LCD_ShowString(30,110,200,24,16,"MODE2:Paper Train");	
    LCD_ShowString(30,140,200,24,16,"Current Mode:");
	sprintf((char*)Char_Value,"%d",mode); //将LCD ID打印到lcd_id数组。
	LCD_ShowString(140,140,200,24,16,Char_Value);
    LCD_ShowString(30,170,200,24,12,"PRES K0 to Start Train");	
}
void LCDShowMoraJudgeInfo(u8 result)
{
	LCD_ShowString(10,50,200,24,16,"Put Hand On Sensing Area");	
    LCD_ShowString(10,100,200,24,16,"PRES K0 to Get Result");	
    LCD_ShowString(10,150,200,24,16,"Judge Result: ");
    switch(result)
    {
        case 1:
            LCD_ShowString(80,200,200,24,24,"Rock        ");
            break;
        case 2:
            LCD_ShowString(80,200,200,24,24,"Scissors    ");
            break;
        case 3:
            LCD_ShowString(80,200,200,24,24,"Paper       ");
            break;
		default:
			LCD_ShowString(80,200,200,24,16,"Current No Gesture");
    }
	LCD_ShowString(10,250,200,24,16,"PRES K2 to Exit");	
}



