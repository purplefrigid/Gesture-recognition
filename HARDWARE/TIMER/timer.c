
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


//ͨ�ö�ʱ��3�жϳ�ʼ��
//����ʱ��ѡ��ΪAPB1��2������APB1Ϊ36M
//arr���Զ���װֵ
//psc��ʱ��Ԥ��Ƶ��
//����ʹ�õ��Ƕ�ʱ��3!
u32 count=0;
// u32 count1 = 0;
void TIM3_Int_Init(u16 arr,u16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //ʱ��ʹ��
	//��ʱ��TIM3��ʼ��
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM3�ж�,��������ж�

	//�ж����ȼ�NVIC����
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //��ʼ��NVIC�Ĵ���

	TIM_Cmd(TIM3, ENABLE);  //ʹ��TIMx					 
}
//��ʱ��3�жϷ������
void TIM3_IRQHandler(void)   //TIM3�ж�
{
	int i;
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  //���TIM3�����жϷ������
		{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //���TIMx�����жϱ�־ 
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
