#include "gesture.h"
#include "FDC2X14.h"
#include "timer.h"
#include "init.h"
#include "sys.h"
#include "delay.h"
#include "math.h"

#define SampleCount 5

int lastValue[CHNUM]={0};
int tempValue[CHNUM]={0};
int JumpRange[CHNUM]={25,20,10,15,10,12,12,20};
u8 Detect(int ChannelNum,int JumpRange)
{
	u8 Detected=0,i=0;
	lastValue[ChannelNum]=ChValue[ChannelNum];
	delay_ms(5);
	if(abs(lastValue[ChannelNum]-ChValue[ChannelNum])>JumpRange)
	{
		Detected=1;
	}
		/*for(i=0;i<SampleCount;i++)
		{
			tempValue[ChannelNum]+=ChValue[ChannelNum];
			delay_ms(5);
		}
		if((lastValue[ChannelNum]-tempValue[ChannelNum]/SampleCount)>=JumpRange)
		{
			Detected=1;
			for(i=0;i<CHNUM;i++)
			{
				tempValue[i]=0;
			}
		}
	}*/
	return Detected;
}


