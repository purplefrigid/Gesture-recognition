
#include "timer.h"
#include "usart.h"
#include "FDC2X14.h"
#include "init.h"
#include "math.h"
#include "myiic.h"
#include "lcd.h"
#define aveCount 10
u16 CalibFlag=0,AveFlag=0;
int Calibration[CHNUM]={0};
int TempValue[CHNUM]={0};
u8 ShowFlag=0;


//通用定时器3中断初始化
//这里时钟选择为APB1的2倍，而APB1为36M
//arr：自动重装值
//psc：时钟预分频数
//这里使用的是定时器3!
u32 count=0;
// u32 count1 = 0;
void TIM3_Int_Init(u16 arr,u16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //时钟使能
	//定时器TIM3初始化
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //使能指定的TIM3中断,允许更新中断

	//中断优先级NVIC设置
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器

	TIM_Cmd(TIM3, ENABLE);  //使能TIMx					 
}
//定时器3中断服务程序
void TIM3_IRQHandler(void)   //TIM3中断
{
	int i;
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  //检查TIM3更新中断发生与否
		{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //清除TIMx更新中断标志 
			count ++;
			if(count>=20)
			{
				count=0;
				if(CalibFlag!=100)
				{
					CalibFlag++;
					for(i=0;i<CHNUM;i++)
					{
						Calibration[i]+=(FDC2X14ReadCH(i+1)-22000000)/1000;
					}
				}
				else
				{
					AveFlag++;
					for(i=0;i<CHNUM;i++)
					{
						TempValue[i]+=abs((FDC2X14ReadCH(i+1)-20000000)/1000-Calibration[i]);
					}
					if(AveFlag==aveCount)
					{
						for(i=0;i<CHNUM;i++)
						{
							ChValue[i]=TempValue[i]/aveCount;
							TempValue[i]=0;
							ShowFlag=1;
						}
						AveFlag=0;
					}
				}
			}
		}
	}
void Calibrate(void)
{
	while(CalibFlag!=100)
	 {
		 LCD_ShowString(30,10,200,24,24,"System Initializi");LCD_ShowString(0,40,200,24,24,"ng......");
	 }
	 Calibration[0]/=100;
	 Calibration[1]/=100;
	 Calibration[2]/=100;
	 Calibration[3]/=100;
	 Calibration[4]/=100;
	 Calibration[5]/=100;
	 Calibration[6]/=100;
	 Calibration[7]/=100;
}
