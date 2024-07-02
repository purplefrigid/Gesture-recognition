#define MYIIC_C
#include "myiic.h"
#include "delay.h"
//////////////////////////////////////////////////////////////////////////////////	 						  
//////////////////////////////////////////////////////////////////////////////////
 
//��ʼ��IIC
void IIC_Init(void)
{					     
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );	//ʹ��GPIOBʱ��
	   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;   //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB,GPIO_Pin_6|GPIO_Pin_7); 	//PB6,PB7 �����
}


//I2C��ʱ
void ADS_delay(void) 
{  
  unsigned char i;    
  for(i=0;i<40;i++); 
}

//I2Cʱ��
void ADS_clock(void)  
{      
  IIC_SCL=1;//SCL=1
  ADS_delay(); 
  IIC_SCL=0;//SCL=0 
  ADS_delay();   
}
//����IIC��ʼ�ź�
void IIC_Start(void)
{
	SDA_OUT();     //sda�����
	IIC_SDA=1;	  	  
	IIC_SCL=1;
	ADS_delay(); 
 	IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	ADS_delay();
  ADS_delay(); 
	IIC_SCL=0;//ǯסI2C���ߣ�׼�����ͻ�������� 
}	  
//����IICֹͣ�ź�
void IIC_Stop(void)
{
	SDA_OUT();//sda�����
	IIC_SCL=1;
	IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
 	ADS_delay();
// 	IIC_SCL=1; 
	IIC_SDA=1;//����I2C���߽����ź�
	ADS_delay();							   	
}
//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
u8 IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	SDA_IN();      //SDA����Ϊ����  
	IIC_SDA=1;ADS_delay();	   
	IIC_SCL=1;ADS_delay();	 
	while(READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC_Stop();
			return 1;
		}
	}
	IIC_SCL=0;//ʱ�����0 	   
	return 0;  
// 	unsigned int i; 
//   SDA_IN();      //SDA����Ϊ���� 
//   ADS_delay(); 
//   IIC_SCL=1; 
//   i=0xF0; 
//   do 
//   { 
//   } 
//   while(--i!=0); 
//   IIC_SCL=0;//ʱ�����0 	 
// 	return 0;  
} 
//����ACKӦ��
void IIC_Ack(void)
{
	ADS_delay(); 
// 	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=0;
	ADS_delay(); 
	IIC_SCL=1;
	ADS_delay(); 
	IIC_SCL=0;
	ADS_delay(); 
}
//������ACKӦ��		    
void IIC_NAck(void)
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=1;
	delay_us(2);
	IIC_SCL=1;
	delay_us(2);
	IIC_SCL=0;
}					 				     
//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��			  
void IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
	SDA_OUT(); 	    
    IIC_SCL=0;//����ʱ�ӿ�ʼ���ݴ���
    for(t=0;t<8;t++)
    {              
        //IIC_SDA=(txd&0x80)>>7;
		if((txd&0x80))
			IIC_SDA=1;
		else
			IIC_SDA=0;
		//ADS_delay();    //��TEA5767��������ʱ���Ǳ����
		ADS_clock();
		txd<<=1; 		
    }	 
		//ADS_delay();
} 	    
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
u8 IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,adsdata;
  SDA_IN();//SDA����Ϊ����
  adsdata=0; 
	 for(i=0;i<8;i++) 
	 {
		adsdata <<=1; 
		if(READ_SDA==1) 
		 adsdata |= 0x01;
		else
		 adsdata |= 0x00;
		//ADS_delay();
		ADS_clock();
	 }  
  return(adsdata); 
}


//I2C�����ݼĴ���0x02 �����ֽ� ���ֽ���ǰ  
//
unsigned int ReadDataCfgReg(u8 index,u8 firstAddress,u8 SecondAddress)  
{ 
  unsigned int temp;
  u8 result[4];
  IIC_Start(); 
  IIC_Send_Byte(0x2A);
  IIC_Wait_Ack();
  ADS_delay();
  result[0]=(IIC_Read_Byte(1)&firstAddress)*0x100;//���ֽ� ����λ���
  IIC_Ack(); 
  result[1]=(IIC_Read_Byte(1));          //temp=���ֽ�+���ֽ�
	IIC_Ack(); 
	if(index==1)
	{
		result[2]=(IIC_Read_Byte(1)&SecondAddress);//
		IIC_Ack(); 
		result[3]=(IIC_Read_Byte(1));          //temp=���ֽ�+���ֽ�
		IIC_Ack(); 
	}
  
	
  IIC_Stop();
  temp = (int)(((result[0] << 24) | (result[1] << 16) | (result[2] << 8) | (result[3] & 0xff)));  
  return(temp); 
     
}
void EnableSelect(u8 order)
{
	switch(order)
	{
		case 0:
			GPIO_SetBits(GPIOC,GPIO_Pin_5);
			GPIO_ResetBits(GPIOC,GPIO_Pin_4);
			break;
		case 1:
			GPIO_SetBits(GPIOC,GPIO_Pin_4);
			GPIO_ResetBits(GPIOC,GPIO_Pin_5);
			break;
	}
}

