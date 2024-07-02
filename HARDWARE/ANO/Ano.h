#ifndef __ANO_H
#define __ANO_H

//#include "stm32f4xx_conf.h"
#include "stm32f10x_conf.h"
#include "sys.h"
#include "FDC2X14.h"

//串口1发送1个字符
//c:要发送的字符
void USART1_SendChar(u8 c);
//传送数据给匿名四轴上位机软件(V2.6版本)
//fun:功能字. 0XA0~0XAF
//data:数据缓存区,最多28字节!!
//len:data区有效数据个数
void AnoReport(u8 fun, u8 *data, u8 len);

//ANO FDC2214
#ifdef __FDC2X14_H
//发送FDC2214四路数据
void FDC2X14_SendData(int *ChVal);
#endif

#endif
