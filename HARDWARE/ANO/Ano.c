#include "Ano.h"

//串口1发送1个字符
//c:要发送的字符
void USART1_SendChar(u8 c)
{
	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
		;
	USART_SendData(USART1, c);
}

//传送数据给匿名四轴上位机软件(V2.6版本)
//fun:功能字. 0XA0~0XAF
//data:数据缓存区,最多28字节!!
//len:data区有效数据个数
void AnoReport(u8 fun, u8 *data, u8 len)
{
	u8 send_buf[32];
	u8 i;
	if (len > 28)
		return;			   //最多28字节数据
	send_buf[len + 3] = 0; //校验数置零
	send_buf[0] = 0X88;	//帧头
	send_buf[1] = fun;	 //功能字
	send_buf[2] = len;	 //数据长度
	for (i = 0; i < len; i++)
		send_buf[3 + i] = data[i]; //复制数据
	for (i = 0; i < len + 3; i++)
		send_buf[len + 3] += send_buf[i]; //计算校验和
	for (i = 0; i < len + 4; i++)
		USART1_SendChar(send_buf[i]); //发送数据到串口1
}

//ANO FDC2214
#ifdef __FDC2X14_H
//发送FDC2214八路数据
void FDC2X14_SendData(int *ChVal)
{
	u8 tbuf[32];
	tbuf[0] = (*ChVal >> 24) & 0xFF;
	tbuf[1] = (*ChVal >> 16) & 0xFF;
	tbuf[2] = (*ChVal >> 8) & 0xFF;
	tbuf[3] = (*ChVal) & 0xFF;
	tbuf[4] = (*(ChVal + 1) >> 24) & 0xFF;
	tbuf[5] = (*(ChVal + 1) >> 16) & 0xFF;
	tbuf[6] = (*(ChVal + 1) >> 8) & 0xFF;
	tbuf[7] = (*(ChVal + 1)) & 0xFF;
	tbuf[8] = (*(ChVal + 2) >> 24) & 0xFF;
	tbuf[9] = (*(ChVal + 2) >> 16) & 0xFF;
	tbuf[10] = (*(ChVal + 2) >> 8) & 0xFF;
	tbuf[11] = (*(ChVal + 2)) & 0xFF;
	tbuf[12] = (*(ChVal + 3) >> 24) & 0xFF;
	tbuf[13] = (*(ChVal + 3) >> 16) & 0xFF;
	tbuf[14] = (*(ChVal + 3) >> 8) & 0xFF;
	tbuf[15] = (*(ChVal + 3)) & 0xFF;
	tbuf[16] = (*(ChVal + 4) >> 24) & 0xFF;
	tbuf[17] = (*(ChVal + 4) >> 16) & 0xFF;
	tbuf[18] = (*(ChVal + 4) >> 8) & 0xFF;
	tbuf[19] = (*(ChVal + 4)) & 0xFF;
	tbuf[20] = (*(ChVal + 5) >> 24) & 0xFF;
	tbuf[21] = (*(ChVal + 5) >> 16) & 0xFF;
	tbuf[22] = (*(ChVal + 5) >> 8) & 0xFF;
	tbuf[23] = (*(ChVal + 5)) & 0xFF;
	tbuf[24] = (*(ChVal + 6) >> 24) & 0xFF;
	tbuf[25] = (*(ChVal + 6) >> 16) & 0xFF;
	tbuf[26] = (*(ChVal + 6) >> 8) & 0xFF;
	tbuf[27] = (*(ChVal + 6)) & 0xFF;
	tbuf[28] = (*(ChVal + 7) >> 24) & 0xFF;
	tbuf[29] = (*(ChVal + 7) >> 16) & 0xFF;
	tbuf[30] = (*(ChVal + 7) >> 8) & 0xFF;
	tbuf[31] = (*(ChVal + 7)) & 0xFF;
	AnoReport(0XA1, tbuf, 32); //自定义帧,0XA1
}
#endif
