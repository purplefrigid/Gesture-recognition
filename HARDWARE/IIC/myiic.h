#ifndef __MYIIC_H
#define __MYIIC_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
					  
//////////////////////////////////////////////////////////////////////////////////

//IO��������
// #define SDA_IN()  {GPIOB->CRH&=0XFFFF0FFF;GPIOB->CRH|=8<<12;}
// #define SDA_OUT() {GPIOB->CRH&=0XFFFF0FFF;GPIOB->CRH|=3<<12;}
#define SDA_IN()  {GPIOB->CRH&=0X0FFFFFFF;GPIOB->CRL|=(u32)8<<28;} 
#define SDA_OUT() {GPIOB->CRH&=0X0FFFFFFF;GPIOB->CRL|=(u32)3<<28;}

//IO��������	 
#define IIC_SCL    PBout(6) //SCL
#define IIC_SDA    PBout(7) //SDA	 
#define READ_SDA   PBin(7)  //����SDA 

#if defined MYIIC_C
int Velocity;                       //������*1000
u32 tempVel;
//IIC���в�������
void IIC_Init(void);                //��ʼ��IIC��IO��				 
void IIC_Start(void);				//����IIC��ʼ�ź�
void IIC_Stop(void);	  			//����IICֹͣ�ź�
void IIC_Send_Byte(u8 txd);			//IIC����һ���ֽ�
u8 IIC_Read_Byte(unsigned char ack);//IIC��ȡһ���ֽ�
u8 IIC_Wait_Ack(void); 				//IIC�ȴ�ACK�ź�
void IIC_Ack(void);					//IIC����ACK�ź�
void IIC_NAck(void);				//IIC������ACK�ź�
void ADS_delay(void);
void IIC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
void EnableSelect(u8);
u8 IIC_Read_One_Byte(u8 daddr,u8 addr);	 
//I2C�����ݼĴ���
unsigned int ReadDataCfgReg(u8 index,u8 firstAddress,u8 SecondAddress);
#else
extern int Velocity;                       //������*1000
extern u32 tempVel;
//IIC���в�������
extern void IIC_Init(void);                //��ʼ��IIC��IO��				 
extern void IIC_Start(void);				//����IIC��ʼ�ź�
extern void IIC_Stop(void);	  			//����IICֹͣ�ź�
extern void IIC_Send_Byte(u8 txd);			//IIC����һ���ֽ�
extern u8 IIC_Read_Byte(unsigned char ack);//IIC��ȡһ���ֽ�
extern u8 IIC_Wait_Ack(void); 				//IIC�ȴ�ACK�ź�
extern void IIC_Ack(void);					//IIC����ACK�ź�
extern void IIC_NAck(void);				//IIC������ACK�ź�
extern void ADS_delay(void) ;
extern void IIC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
extern void EnableSelect(u8);
extern u8 IIC_Read_One_Byte(u8 daddr,u8 addr);	 
//I2C�����ݼĴ���  
extern unsigned int ReadDataCfgReg(u8 index,u8 firstAddress,u8 SecondAddress);
#endif
#endif
















