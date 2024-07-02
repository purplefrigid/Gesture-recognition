#define MYIIC_C
#include "iic.h"
#include "delay.h"
//////////////////////////////////////////////////////////////////////////////////	 						  
//////////////////////////////////////////////////////////////////////////////////

#define ADDRESS 0xF0

//��ʼ��IIC
void IIC_Init(void)
{					     
	I2C_InitTypeDef I2C_InitStructure;
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1,ENABLE); 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);


	//I2C GPIO Config
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);


  I2C_InitStructure.I2C_ClockSpeed    =  100000;     //200kHZ  
	I2C_InitStructure.I2C_Mode          =  I2C_Mode_I2C;  
	I2C_InitStructure.I2C_DutyCycle     =  I2C_DutyCycle_2; //����datasheet��Tlow/Thigh = 2  
	I2C_InitStructure.I2C_OwnAddress1   =  0x00;          
	I2C_InitStructure.I2C_Ack           =  I2C_Ack_Enable;  
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;  
	I2C_Cmd(I2C1,ENABLE);
	I2C_Init(I2C1, &I2C_InitStructure);
}

void SDA_IN(void)//����SDAΪ����
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );	
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //PA15���ó����룬Ĭ������ 
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIOB��7
	
}
void SDA_OUT(void)//����SDAΪ���
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_Out_PP ;   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}
//I2C��ʱ
void ADS_delay(void) 
{  
  unsigned char i;    
  for(i=0;i<60;i++); 
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
  delay_us(1);	
	IIC_SCL=1;
	delay_us(5); 
 	IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	delay_us(5); 
	IIC_SCL=0;//ǯסI2C���ߣ�׼�����ͻ�������� 
  delay_us(5);
}	  
//����IICֹͣ�ź�
void IIC_Stop(void)
{
	SDA_OUT();//sda�����
	IIC_SCL=1;
	IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
 	delay_us(1); 
	IIC_SDA=1;//����I2C���߽����ź�
	delay_us(5);
 	IIC_SCL=0;
	delay_us(5);	
}
//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
u8 IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	SDA_IN();      //SDA����Ϊ����  
	IIC_SDA=1;delay_us(1);	   
	IIC_SCL=1;delay_us(1);	 
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
} 
//����ACKӦ��
void IIC_Ack(void)
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=0;
	delay_us(2);
	IIC_SCL=1;
	delay_us(2);
	IIC_SCL=0; 
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
		if((txd&0x80)>>7)
			IIC_SDA=1;
		else
			IIC_SDA=0;
		txd<<=1; 	  
		delay_us(5);   //��TEA5767��������ʱ���Ǳ����
		IIC_SCL=1;
		delay_us(5); 
		IIC_SCL=0;	
		delay_us(5);
    }	
} 	    
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
u8 IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN();//SDA����Ϊ����
    for(i=0;i<8;i++ )
		{
        IIC_SCL=0; 
        delay_us(2);
				IIC_SCL=1;
        receive<<=1;
        if(READ_SDA)
				{
					receive |= 0x01;
				}
				else
				{
					receive |= 0x00;
				}
				delay_us(1); 
    }					 
    if (!ack)
        IIC_NAck();//����nACK
    else
        IIC_Ack(); //����ACK   
    return receive;
}


//I2C�����ݼĴ���0x02 �����ֽ� ���ֽ���ǰ  �ӵ�ַΪ0xF0
unsigned int ReadDataCfgReg(void)  
{ 
  unsigned int temp; 
  IIC_Start(); 
  IIC_Send_Byte(0xF1);
  IIC_Wait_Ack();
  ADS_delay();
  temp=(IIC_Read_Byte(1)&0x0F)*0x100;//���ֽ� ����λ��� 
  temp+=(IIC_Read_Byte(1));          //temp=���ֽ�+���ֽ�
  IIC_Stop();  
  return(temp); 
     
}

/*    2.  * ��������I2C_EE_PageWrite    
 * ����  ����EEPROM��һ��дѭ���п���д����ֽڣ���һ��д����ֽ���   
 *         ���ܳ���EEPROMҳ�Ĵ�С��AT24C02ÿҳ��8���ֽڡ�   
 * ����  ��-pBuffer ������ָ��    
 *         -WriteAddr �������ݵ�EEPROM�ĵ�ַ    
 *         -NumByteToWrite Ҫд��EEPROM���ֽ���   
 * ���  ����   
 * ����  ����    
  * ����  ���ⲿ����   
  */    
 
void I2C_EE_PageWrite(u8* pBuffer, u8 WriteAddr, u8 NumByteToWrite)    
 {     
     while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY)); // Added by Najoua  27/08/2008    
          
   /* Send START condition */                                                                                                                               
   I2C_GenerateSTART(I2C1, ENABLE);    
        
 
  /* Test on EV5 and clear it */    
   while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));     
        
   /* Send EEPROM address for write */    
   I2C_Send7bitAddress(I2C1, ADDRESS, I2C_Direction_Transmitter) ;    
        
   /* Test on EV6 and clear it */    
   while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));      
     
   /* Send the EEPROM's internal address to write to */        
   I2C_SendData(I2C1, WriteAddr);      
     
   /* Test on EV8 and clear it */    
   while(! I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));    
     
   /* While there is data to be written */   
   while(NumByteToWrite--)      
   {     
     /* Send the current byte */    
     I2C_SendData(I2C1, *pBuffer);     
     
     /* Point to the next byte to be written */   
     pBuffer++;     
        
     /* Test on EV8 and clear it */    
     while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));      
   }     
   /* Send STOP condition */    
   I2C_GenerateSTOP(I2C1, ENABLE);    
 } 
/*    2.  * ��������I2C_EE_BufferRead    
  * ����  ����EEPROM�����ȡһ�����ݡ�    
  * ����  ��-pBuffer ��Ŵ�EEPROM��ȡ�����ݵĻ�����ָ�롣   
  *         -WriteAddr �������ݵ�EEPROM�ĵ�ַ��     
  *         -NumByteToWrite Ҫ��EEPROM��ȡ���ֽ�����   
  * ���  ����   
  * ����  ����    
  * ����  ���ⲿ����   
.  */    
 void I2C_EE_BufferRead(u8* pBuffer, u8 ReadAddr, u16 NumByteToRead)    
 {       
   //*((u8 *)0x4001080c) |=0x80;      
//      while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY)); // Added by Najoua  27/08/2008    
//          
          
   /* Send START condition */    
   I2C_GenerateSTART(I2C1, ENABLE);    
   //*((u8 *)0x4001080c) &=~0x80;    
        
   /* Test on EV5 and clear it */     
   while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));    
     
   /* Send EEPROM address for write */    
   I2C_Send7bitAddress(I2C1, 0xF0, I2C_Direction_Transmitter) ;    
     
   /* Test on EV6 and clear it */    
   while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));    
        
   /* Clear EV6 by setting again the PE bit */   
   I2C_Cmd(I2C1, ENABLE);    
     
   /* Send the EEPROM's internal address to write to */   
   I2C_SendData(I2C1, ReadAddr);      
     
   /* Test on EV8 and clear it */    
   while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));    
        
   /* Send STRAT condition a second time */      
   I2C_GenerateSTART(I2C1, ENABLE);    
        
   /* Test on EV5 and clear it */    
   while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));    
        
   /* Send EEPROM address for read */    
   I2C_Send7bitAddress(I2C1, ADDRESS, I2C_Direction_Receiver);      
        
   /* Test on EV6 and clear it */    
   while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED) );    
        
   /* While there is data to be read */   
   while(NumByteToRead)      
   {     
     if(NumByteToRead == 1)    
     {     
       /* Disable Acknowledgement */    
       I2C_AcknowledgeConfig(I2C1, DISABLE);    
            
       /* Send STOP Condition */    
       I2C_GenerateSTOP(I2C1, ENABLE);    
     }    
    
     /* Test on EV7 and clear it */    
     if(I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED))      
     {           
       /* Read a byte from the EEPROM */   
       *pBuffer = I2C_ReceiveData(I2C1);    
     
       /* Point to the next location where the byte read will be saved  */    
       pBuffer++;     
            
       /* Decrement the read bytes counter */   
       NumByteToRead--;            
     }       
   }    
     
   /* Enable Acknowledgement to be ready for another reception */   
   I2C_AcknowledgeConfig(I2C1, ENABLE);    
 } 
























