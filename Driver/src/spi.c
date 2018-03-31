/***************************************************
@author:�����
@date:2012-6-3
@description:spi����
******************************************************/
#include "spi.h"
  
/**
  * @brief  SPI����������
  * @param  SPI_BAUDRATEPRESCALER_2~SPI_BAUDRATEPRESCALER_256 
  * @retval None
  */
void SPI_SetSpeed(u16  SPI_BaudRatePrescaler)
{
	//�ı�SPI�Ĳ�����
	SPI->CR1&=~0x38;	//�����ʿ���λ������
	SPI->CR1|=SPI_BaudRatePrescaler;
	  
} 

//SPI ��дһ���ֽ�
//TxData:Ҫд����ֽ�
//����ֵ:��ȡ�����ֽ�
u8 SPI_ReadWriteByte(u8 TxData)
{	
  u8 retry=200;
  
	while((SPI->SR&1<<1)==0);//�ȴ���������
		  
	SPI->DR=TxData;	 	  //����һ��byte 
	while((SPI->SR&1<<0)==0&&retry)//�ȴ�������һ��byte   
	{
	 	retry--; 
	}
	if(!retry)return 0;
	else return SPI->DR;          //�����յ�������				    
}




