#include "task.h" 
#include "stm32f10x.h"
#include "myiic.h"
#include "FDC2X14.h"
#include "timer.h"
#include "delay.h"
#include "lcd.h"
#include "stdio.h"
#include "usart.h"
#include "Ano.h"
#include "hc05.h"
#include "init.h"
#include "lcd.h"
#include "gesture.h"
#include "key.h"
#include "LCDInfo.h"

#define  uint8 u8
struct _TimeMS TimeMs; 
u8 FirstEnterFlag1=1,FirstEnterFlag2=1,FirstEnterFlag3=1,FirstEnterFlag4=1;
extern u8 Instruction;
u8 Char_Value[12];			//数字打印为字符串暂存变量
u8 publicFlagAll=0;
int TrainData[TrainLimit][CHNUM]={0},tempArray[CHNUM];
int TrainDataOne[CHNUM]={0},TrainDataTwo[CHNUM]={0},TrainDataThree[CHNUM]={0},TrainDataFour[CHNUM]={0},TrainDataFive[CHNUM]={0};
int TrainDataPaper[CHNUM]={0},TrainDataScissor[CHNUM]={0},TrainDataRock[CHNUM]={0};
int offsetScissor[CHNUM]={0},offsetPaper[CHNUM]={0},offsetRock[CHNUM]={0};
int offsetOne[CHNUM]={0},offsetTwo[CHNUM]={0},offsetThree[CHNUM]={0},offsetFour[CHNUM]={0},offsetFive[CHNUM]={0};
int offset[5]={0},offset1[3]={0};
int offset_Min=0,my_index=0;
void TrainDataInit()
{
	int i=0,j=0;
	for(i=0;i<TrainLimit;i++)
	{
		for(j=0;j<CHNUM;j++)
		{
			TrainData[i][j]=0;
		}
	}
}
void Task1(void)
{
	
	u8 FingerCount=0;
	if(publicFlagAll!=1)
		LCD_Clear(WHITE);
	if(FirstEnterFlag1)
	{
		LCD_Clear(WHITE);
		FirstEnterFlag1=0;
	}
	while(ChValue[0]<=1960) 
	{
		if(ChValue[1]<1970&&ChValue[4]<1995&&ChValue[5]<2000&&ChValue[6]<2000&&ChValue[7]<2000)
			FingerCount=5;
		else if(ChValue[1]<1970&&ChValue[5]<2000&&ChValue[6]<2000&&ChValue[7]<2000)
			FingerCount=4;
		else if(ChValue[5]<2000&&ChValue[6]<1987&&ChValue[7]<2000)
			FingerCount=3;
		else if(ChValue[5]<1985&&ChValue[7]<1985)
			FingerCount=2;
		else
			FingerCount=1;
		
	LCD_ShowString(30,280,200,24,24,"FingerCount:     ");
	LCD_ShowxNum(180,280,FingerCount,1,24,0);
	}
	LCD_ShowString(30,280,200,24,24,"No Hand Detected!");
	publicFlagAll=1;
}

void Task2(void)
{
	u8 FingerCount=0;
	if(publicFlagAll!=2)
		LCD_Clear(WHITE);
	if(FirstEnterFlag2)
	{
		LCD_Clear(WHITE);
		FirstEnterFlag2=0;
	}
	while(ChValue[0]<=1960)
	{
		
		if(ChValue[1]<1970&&ChValue[4]<20000&&ChValue[5]<20000&&ChValue[6]<19900&&ChValue[7]<19900)
		{
			FingerCount=5;
		}
		else if((ChValue[5])<1985&&(ChValue[7])<1985)
			FingerCount=2;
		else
			FingerCount=0;
		switch(FingerCount)
		{
			case 0:
				LCD_ShowString(30,280,200,24,24,"rock            ");
				break;
			case 2:
				LCD_ShowString(30,280,200,24,24,"scissor         ");
				break;
			case 5:
				LCD_ShowString(30,280,200,24,24,"paper            ");
				break;
		}
	}
	LCD_ShowString(30,280,200,24,24,"No Hand Detected!");
	publicFlagAll=2;
}

void Task3(void)
{
	u8 result=0,mode=0,group=0,i=0,j=0,trainTimes=0;
	u8 publicFlag=0;
	u8 caseFlag1=1,caseFlag2=1,caseFlag3=1,caseFlag4=1,caseFlag5=1;
	if(publicFlagAll!=3)
		LCD_Clear(WHITE);
	if(FirstEnterFlag3)
	{
		FirstEnterFlag3=0;
	}
	LCD_Clear(WHITE);
	result=KEY_Scan(1);
	
	while(Instruction==0X03)
	{
		LCDShowRowingSelectInfo();
		result=KEY_Scan(1);
		if(result==KEY0_PRES)   //按下KEY0进入训练模式
		{
			LCD_Clear(WHITE);
			while(result!=WKUP_PRES)
			{
				LCDShowRowingTrainInfo(mode);
				result=KEY_Scan(1);
				if(result==KEY1_PRES)
				{
					mode++;
				}
				if(mode%5==0)
					mode=0;
				switch(mode)
				{
					case 0:
						if(caseFlag1)
						{
							TrainDataInit();
							caseFlag1=0;
							group=0;
							LCD_ShowString(30,260,200,24,16,"trainTimes:0     ");
							LCD_ShowString(30,300,200,24,12,"TrainData NGet!");
						}
						if(publicFlag!=0)
						{
							trainTimes=0;
							LCD_ShowString(30,260,200,24,16,"trainTimes:0     ");
							LCD_ShowString(30,300,200,24,12,"TrainData NGet!");
						}
						result=KEY_Scan(1);
						if(result==KEY0_PRES)
						{
							trainTimes++;
							sprintf((char*)Char_Value,"%d",trainTimes); //将LCD ID打印到lcd_id数组
							LCD_ShowString(30,260,200,24,16,"trainTimes:");
							LCD_ShowString(126,260,200,24,16,Char_Value);
							LCD_ShowString(30,300,200,24,12,"TrainData Get!");
							for(i=0;i<CHNUM;i++)
							{
								TrainData[group][i]=ChValue[i];
							}
							group++;
							if(group==3)
								group=0;
						}
						if(trainTimes>0)
						{
							for(i=0;i<CHNUM;i++)
							{
								TrainDataOne[i]=(TrainData[0][i]+TrainData[1][i]+TrainData[2][i])/group;
							}
						}
						publicFlag=0;
						break;
					case 1:
					  	
						if(caseFlag2)
						{
							TrainDataInit();
							caseFlag2=0;
							trainTimes=0;
							group=0;
						}
						if(publicFlag!=1)
						{
							trainTimes=0;
							LCD_ShowString(30,260,200,24,16,"trainTimes:0     ");
							LCD_ShowString(30,300,200,24,12,"TrainData NGet!");
						}
						result=KEY_Scan(1);
						if(result==KEY0_PRES)
						{
							trainTimes++;
							sprintf((char*)Char_Value,"%d",trainTimes); //将LCD ID打印到lcd_id数组?
							LCD_ShowString(30,260,200,24,16,"trainTimes:");
							LCD_ShowString(126,260,200,24,16,Char_Value);
							LCD_ShowString(30,300,200,24,12,"TrainData Get!");
							for(i=0;i<CHNUM;i++)
							{
								TrainData[group][i]=ChValue[i];
							}
							group++;
							if(group==3)
								group=0;
						}
						if(trainTimes>0)
						{
							for(i=0;i<CHNUM;i++)
							{
								TrainDataTwo[i]=(TrainData[0][i]+TrainData[1][i]+TrainData[2][i])/group;
							}
						}
						publicFlag=1;
						break;
					case 2:
						if(caseFlag3)
						{
							TrainDataInit();
							caseFlag3=0;
							trainTimes=0;
							group=0;
						}
						if(publicFlag!=2)
						{
							trainTimes=0;
							LCD_ShowString(30,260,200,24,16,"trainTimes:0     ");
							LCD_ShowString(30,300,200,24,12,"TrainData NGet!");
						}
						result=KEY_Scan(1);
						if(result==KEY0_PRES)
						{
							trainTimes++;
							sprintf((char*)Char_Value,"%d",trainTimes); //将LCD ID打印到lcd_id数组?
							LCD_ShowString(30,260,200,24,16,"trainTimes:");
							LCD_ShowString(126,260,200,24,16,Char_Value);
							LCD_ShowString(30,300,200,24,12,"TrainData Get!");
							for(i=0;i<CHNUM;i++)
							{
								TrainData[group][i]=ChValue[i];
							}
							group++;
							if(group==3)
								group=0;
						}
						if(trainTimes>0)
						{
							for(i=0;i<CHNUM;i++)
							{
								TrainDataThree[i]=(TrainData[0][i]+TrainData[1][i]+TrainData[2][i])/group;
							}
						}
						publicFlag=2;
						break;
					case 3:
						if(caseFlag4)
						{
							TrainDataInit();
							caseFlag4=0;
							trainTimes=0;
							group=0;
						}
						if(publicFlag!=3)
						{
							trainTimes=0;
							LCD_ShowString(30,260,200,24,16,"trainTimes:0     ");
							LCD_ShowString(30,300,200,24,12,"TrainData NGet!");
						}
						result=KEY_Scan(1);
						if(result==KEY0_PRES)
						{
							trainTimes++;
							sprintf((char*)Char_Value,"%d",trainTimes); //将LCD ID打印到lcd_id数组?
							LCD_ShowString(30,260,200,24,16,"trainTimes");
							LCD_ShowString(126,260,200,24,16,Char_Value);
							LCD_ShowString(30,300,200,24,12,"TrainData Get!");
							for(i=0;i<CHNUM;i++)
							{
								TrainData[group][i]=ChValue[i];
							}
							group++;
							if(group==3)
								group=0;
						}
						if(trainTimes>0)
						{
							for(i=0;i<CHNUM;i++)
							{
								TrainDataFour[i]=(TrainData[0][i]+TrainData[1][i]+TrainData[2][i])/group;
							}
						}
						for(i=0;i<TrainLimit;i++)
						{
							for(j=0;j<CHNUM;j++)
								{
									TrainData[i][j]=0;
								}
						}
						publicFlag=3;
						break;
					case 4:
						if(caseFlag5)
						{
							TrainDataInit();
							caseFlag5=0;
							trainTimes=0;
							group=0;
						}
						if(publicFlag!=4)
						{
							trainTimes=0;
							LCD_ShowString(30,260,200,24,16,"trainTimes:0     ");
							LCD_ShowString(30,300,200,24,12,"TrainData NGet!");
						}
						result=KEY_Scan(1);
						if(result==KEY0_PRES)
						{
							trainTimes++;
							sprintf((char*)Char_Value,"%d",trainTimes); //将LCD ID打印到lcd_id数组?
							LCD_ShowString(30,260,200,24,16,"trainTimes");
							LCD_ShowString(126,260,200,24,16,Char_Value);
							LCD_ShowString(30,300,200,24,12,"TrainData Get!");
							for(i=0;i<CHNUM;i++)
							{
								TrainData[group][i]=ChValue[i];
							}
							group++;
							if(group==3)
								group=0;
						}
						if(trainTimes>0)
						{
							for(i=0;i<CHNUM;i++)
							{
								TrainDataFive[i]=(TrainData[0][i]+TrainData[1][i]+TrainData[2][i])/group;
							}
						}
						publicFlag=4;
						break;
					default:
						break;
					}
				}
				result=0;
				LCD_Clear(WHITE);
			}
		if(result==KEY1_PRES)    //按下KEY1进入判决模式
		{
			LCD_Clear(WHITE);
			while(result!=WKUP_PRES)
			{
				result=KEY_Scan(1);
				LCDShowRowingJudgeInfo(my_index);
				if(result==KEY0_PRES)
				{
					int temp=0;
					for(i=0;i<CHNUM;i++)
					{
						tempArray[i]=ChValue[i];
					}
					for(i=0;i<CHNUM;i++)
					{
					offsetOne[i]=tempArray[i]-TrainDataOne[i];
					}
					for(i=0;i<CHNUM;i++)
					{
					offsetTwo[i]=tempArray[i]-TrainDataTwo[i];
					}
					for(i=0;i<CHNUM;i++)
					{
					offsetThree[i]=tempArray[i]-TrainDataThree[i];
					}
					for(i=0;i<CHNUM;i++)
					{
					offsetFour[i]=tempArray[i]-TrainDataFour[i];
					}
					for(i=0;i<CHNUM;i++)
					{
					offsetFive[i]=tempArray[i]-TrainDataOne[i];
					}
					for(i=0;i<CHNUM;i++)
					{
						temp+=offsetOne[i]*offsetOne[i];
					}
					offset[0]=temp;
					temp=0;
					for(i=0;i<CHNUM;i++)
					{
						temp+=offsetTwo[i]*offsetTwo[i];
					}
					offset[1]=temp;
					temp=0;
					for(i=0;i<CHNUM;i++)
					{
						temp+=offsetThree[i]*offsetThree[i];
					}
					offset[2]=temp;
					temp=0;
					for(i=0;i<CHNUM;i++)
					{
						temp+=offsetFour[i]*offsetFour[i];
					}
					offset[3]=temp;
					temp=0;
					for(i=0;i<CHNUM;i++)
					{
						temp+=offsetFive[i]*offsetFive[i];
					}
					offset[4]=temp;
					temp=0;
					offset_Min=offset[0];
					for(i=0;i<5;i++)
					{
					if(offset[i]<offset_Min)
					{
						offset_Min=offset[i];
						my_index = i+1;
					}
					}
				}
			}
			LCD_Clear(WHITE);
			result=0;
		}
	}
	LCD_Clear(WHITE);
	publicFlagAll=3;
}

void Task4(void)
{
	u8 result=0,mode=0,group=0,i=0,trainTimes=0;
	u8 caseFlag1=1,caseFlag2=1,caseFlag3=1;
	u8 publicFlag=0;
	if(publicFlagAll!=4)
		LCD_Clear(WHITE);
	if(FirstEnterFlag4)
		{
			FirstEnterFlag4=0;
		}
	LCD_Clear(WHITE);
	result=KEY_Scan(1);
	while(Instruction==0x04)
	{
		LCDShowMoraSelectInfo();
		result=KEY_Scan(1);
		if(result==KEY0_PRES)   //按下KEY0进入训练模式
		{
			LCD_Clear(WHITE);
			while(result!=WKUP_PRES)
			{
				LCDShowMoraTrainInfo(mode);
				result=KEY_Scan(1);
				if(result==KEY1_PRES)
				{
					mode++;
				}
				if(mode%3==0)
					mode=0;
				switch(mode)
				{
					case 0:
						if(caseFlag1)
						{
							TrainDataInit();
							caseFlag1=0;
							group=0;
							trainTimes=0;
							LCD_ShowString(30,260,200,24,16,"trainTimes:0     ");
							LCD_ShowString(30,300,200,24,12,"TrainData NGet!");
						}
						if(publicFlag!=0)
						{
							trainTimes=0;
							LCD_ShowString(30,260,200,24,16,"trainTimes:0     ");
							LCD_ShowString(30,300,200,24,12,"TrainData NGet!");
						}
						result=KEY_Scan(1);
						if(result==KEY0_PRES)
						{
							trainTimes++;
							sprintf((char*)Char_Value,"%d",trainTimes); //将LCD ID打印到lcd_id数组
							LCD_ShowString(30,260,200,24,16,"trainTimes:");
							LCD_ShowString(126,260,200,24,16,Char_Value);
							LCD_ShowString(30,300,200,24,12,"TrainData Get!");
							for(i=0;i<CHNUM;i++)
							{
								TrainData[group][i]=ChValue[i];
							}
							group++;
							if(group==3)
								group=0;
						}
						if(trainTimes>0)
						{
							for(i=0;i<CHNUM;i++)
							{
								TrainDataOne[i]=(TrainData[0][i]+TrainData[1][i]+TrainData[2][i])/group;
							}
						}
						publicFlag=0;
						break;
					case 1:
						if(caseFlag2)
						{
							TrainDataInit();
							caseFlag2=0;
							group=0;
					  		trainTimes=0;
						}
						if(publicFlag!=1)
						{
							trainTimes=0;
							LCD_ShowString(30,260,200,24,16,"trainTimes:0     ");
							LCD_ShowString(30,300,200,24,12,"TrainData NGet!");
						}
						result=KEY_Scan(1);
						if(result==KEY0_PRES)
						{
							trainTimes++;
							sprintf((char*)Char_Value,"%d",trainTimes); //将LCD ID打印到lcd_id数组?
							LCD_ShowString(30,260,200,24,16,"trainTimes:");
							LCD_ShowString(126,260,200,24,16,Char_Value);
							LCD_ShowString(30,300,200,24,12,"TrainData Get!");
							for(i=0;i<CHNUM;i++)
							{
								TrainData[group][i]=ChValue[i];
							}
							group++;
							if(group==3)
								group=0;
						}
						if(trainTimes>0)
						{
							for(i=0;i<CHNUM;i++)
							{
								TrainDataTwo[i]=(TrainData[0][i]+TrainData[1][i]+TrainData[2][i])/group;
							}
						}
						publicFlag=1;
						break;
					case 2:
							
						if(caseFlag3)
						{
							TrainDataInit();
							caseFlag3=0;
							group=0;
					  		trainTimes=0;
						}
						if(publicFlag!=2)
						{
							trainTimes=0;
							LCD_ShowString(30,260,200,24,16,"trainTimes:0     ");
							LCD_ShowString(30,300,200,24,12,"TrainData NGet!");
						}
						result=KEY_Scan(1);
						if(result==KEY0_PRES)
						{
							trainTimes++;
							sprintf((char*)Char_Value,"%d",trainTimes); //将LCD ID打印到lcd_id数组?
							LCD_ShowString(30,260,200,24,16,"trainTimes:");
							LCD_ShowString(126,260,200,24,16,Char_Value);
							LCD_ShowString(30,300,200,24,12,"TrainData Get!");
							for(i=0;i<CHNUM;i++)
							{
								TrainData[group][i]=ChValue[i];
							}
							group++;
							if(group==3)
								group=0;
						}
						if(trainTimes>0)
						{
							for(i=0;i<CHNUM;i++)
							{
								TrainDataThree[i]=(TrainData[0][i]+TrainData[1][i]+TrainData[2][i])/group;
							}
						}
						publicFlag=2;
						break;
						default:
							break;
				}
			}
			LCD_Clear(WHITE);
		}
		if(result==KEY1_PRES)
		{
			LCD_Clear(WHITE);
			while(result!=WKUP_PRES)
			{
				result=KEY_Scan(1);
				LCDShowMoraJudgeInfo(my_index);
				if(result==KEY0_PRES)
				{
					int temp=0;
					for(i=0;i<CHNUM;i++)
					{
						tempArray[i]=ChValue[i];
					}
					for(i=0;i<CHNUM;i++)
					{
					offsetScissor[i]=tempArray[i]-TrainDataScissor[i];
					}
					for(i=0;i<CHNUM;i++)
					{
					offsetRock[i]=tempArray[i]-TrainDataRock[i];
					}
					for(i=0;i<CHNUM;i++)
					{
					offsetPaper[i]=tempArray[i]-TrainDataPaper[i];
					}
					for(i=0;i<CHNUM;i++)
					{
						temp+=offsetRock[i]*offsetRock[i];
					}
					offset1[0]=temp;
					temp=0;
					for(i=0;i<CHNUM;i++)
					{
						temp+=offsetScissor[i]*offsetScissor[i];
					}
					offset1[1]=temp;
					temp=0;
					for(i=0;i<CHNUM;i++)
					{
						temp+=offsetPaper[i]*offsetPaper[i];
					}
					offset1[2]=temp;
					offset_Min=offset[0];
					for(i=0;i<3;i++)
					{
						if(offset[i]<offset_Min)
						{
						offset_Min=offset[i];
						my_index = i+1;
						}
					}
				}
			}
			LCD_Clear(WHITE);
		}
	}
	publicFlagAll=4;
	LCD_Clear(WHITE);
}









