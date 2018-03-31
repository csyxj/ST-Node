#include "24l01.h"
#include "delay.h"
							    
//��ʼ��24L01��IO��
void NRF24L01_Init(void)
{  
	SPI_Init(SPI_FIRSTBIT_MSB, NRF24L01_SPI_SPEED, SPI_MODE_MASTER,\
            SPI_CLOCKPOLARITY_LOW, SPI_CLOCKPHASE_1EDGE, \
            SPI_DATADIRECTION_2LINES_FULLDUPLEX, SPI_NSS_SOFT, 0x07);
   	SPI_Cmd(ENABLE);
	//CSN�������
   	GPIO_Init(NRF24L01_CSN_PORT , 1<<NRF24L01_CSN_PIN, GPIO_MODE_OUT_PP_HIGH_FAST);
   	NRF24L01_CSN_HIGH();
	//CE�������
	GPIO_Init(NRF24L01_CE_PORT , 1<<NRF24L01_CE_PIN, GPIO_MODE_OUT_PP_LOW_FAST);
	//IRQ��������
	GPIO_Init(NRF24L01_IRQ_PORT , 1<<NRF24L01_IRQ_PIN, GPIO_MODE_IN_PU_NO_IT);

	NRF24L01_CE_LOW(); 					//ʧ��24L01	  		 		  
}

//���24L01�Ƿ����(��д�Ĵ�������)
//����ֵ:0���ɹ�;1��ʧ��	
u8 NRF24L01_Check(void)
{
	u8 buf[5]={0XA5,0XA5,0XA5,0XA5,0XA5};
	u8 i;
	NRF24L01_Write_Buf(WRITE_2401_REG|TX_ADDR,buf,5);//д��5���ֽڵĵ�ַ.	
	NRF24L01_Read_Buf(TX_ADDR,buf,5); //����д��ĵ�ַ  
	for(i=0;i<5;i++)if(buf[i]!=0XA5)break;	 							   
	if(i!=5)return 1;//���24L01����	
	return 0;		 //��⵽24L01
}	 
	 
//SPIд�Ĵ���
//reg:ָ���Ĵ�����ַ
//value:д���ֵ
u8 NRF24L01_Write_Reg(u8 reg,u8 value)
{
	u8 status;	
   	NRF24L01_CSN_LOW();                 //ʹ��SPI����
  	status =SPI_ReadWriteByte(reg);//���ͼĴ����� 
  	SPI_ReadWriteByte(value);      //д��Ĵ�����ֵ
  	NRF24L01_CSN_HIGH();                 //��ֹSPI����	   
  	return(status);       			//����״ֵ̬
}

//��ȡSPI�Ĵ���ֵ
//reg:Ҫ���ļĴ���
u8 NRF24L01_Read_Reg(u8 reg)
{
	u8 reg_val;	    
 	NRF24L01_CSN_LOW();          //ʹ��SPI����		
  	SPI_ReadWriteByte(reg);   //���ͼĴ�����
  	reg_val=SPI_ReadWriteByte(NOP);//��ȡ�Ĵ�������
  	NRF24L01_CSN_HIGH();          //��ֹSPI����		    
  	return(reg_val);           //����״ֵ̬
}
	
//��ָ��λ�ö���ָ�����ȵ�����
//reg:�Ĵ���(λ��)
//*pBuf:����ָ��
//len:���ݳ���
//����ֵ,�˴ζ�����״̬�Ĵ���ֵ 
u8 NRF24L01_Read_Buf(u8 reg,u8 *pBuf,u8 len)
{
	u8 status,u8_ctr;	       
  	NRF24L01_CSN_LOW();           //ʹ��SPI����
  	status=SPI_ReadWriteByte(reg);//���ͼĴ���ֵ(λ��),����ȡ״ֵ̬   	   
 	for(u8_ctr=0;u8_ctr<len;u8_ctr++)pBuf[u8_ctr]=SPI_ReadWriteByte(NOP);//��������
  	NRF24L01_CSN_HIGH();       //�ر�SPI����
  	return status;        //���ض�����״ֵ̬
}

//��ָ��λ��дָ�����ȵ�����
//reg:�Ĵ���(λ��)
//*pBuf:����ָ��
//len:���ݳ���
//����ֵ,�˴ζ�����״̬�Ĵ���ֵ
u8 NRF24L01_Write_Buf(u8 reg, u8 *pBuf, u8 len)
{
	u8 status,u8_ctr;	    
 	NRF24L01_CSN_LOW();          //ʹ��SPI����
  	status = SPI_ReadWriteByte(reg);//���ͼĴ���ֵ(λ��)
  	for(u8_ctr=0; u8_ctr<len; u8_ctr++)SPI_ReadWriteByte(*pBuf++); //д������	 
  	NRF24L01_CSN_HIGH();       //�ر�SPI����
  	return status;          //���ض�����״ֵ̬
}	
			   
//����NRF24L01����һ������
//txbuf:�����������׵�ַ
//����ֵ:�������״��
u8 NRF24L01_TxPacket(u8 *txbuf)
{
	u8 sta;
 	//��24L01�����SPIʱ��Ϊ10Mhz��
	//SPIx_SetSpeed(&NRF24L01_SPIx,NRF24L01_SPI_SPEED);
	NRF24L01_CE_LOW();	   //����ģʽI
  	NRF24L01_Write_Buf(WR_TX_PLOAD,txbuf,TX_PLOAD_WIDTH);//д���ݵ�TX BUF  32���ֽ�
 	NRF24L01_CE_HIGH();//��������	   
	while(NRF24L01_IRQ);//�ȴ��������
	sta=NRF24L01_Read_Reg(STATUS);  //��ȡ״̬�Ĵ�����ֵ	   
	NRF24L01_Write_Reg(WRITE_2401_REG+STATUS,sta); //���TX_DS��MAX_RT�жϱ�־
	if(sta&MAX_RT)//�ﵽ����ط�����
	{
		NRF24L01_Write_Reg(FLUSH_TX,NOP);//���TX FIFO�Ĵ��� 
		return MAX_RT; 
	}
	if(sta&TX_DS)//�������
	{
		return TX_DS;
	}
	return 0xff;//����ԭ����ʧ��
}

//����NRF24L01����һ������
//rxbuf:�����������׵�ַ
//����ֵ:0��������ɣ��������������
u8 NRF24L01_RxPacket(u8 *rxbuf)
{
	u8 sta;		    							   
	//��24L01�����SPIʱ��Ϊ10Mhz��
	//SPIx_SetSpeed(&NRF24L01_SPIx,NRF24L01_SPI_SPEED);   
	sta=NRF24L01_Read_Reg(STATUS);  //��ȡ״̬�Ĵ�����ֵ    	 
	NRF24L01_Write_Reg(WRITE_2401_REG|STATUS,sta); //���TX_DS��MAX_RT�жϱ�־
	if(sta&RX_DR)//���յ�����
	{
		NRF24L01_Read_Buf(RD_RX_PLOAD,rxbuf,RX_PLOAD_WIDTH);//��ȡ����
		NRF24L01_Write_Reg(FLUSH_RX,NOP);//���RX FIFO�Ĵ���
		return 0; 
	}	   
	return 1;//û�յ��κ�����
}	
				    
//�ú�����ʼ��NRF24L01��RXģʽ
//����RX��ַ,дRX���ݿ��,ѡ��RFƵ��,�����ʺ�LNA HCURR
//��CE��ߺ�,������RXģʽ,�����Խ���������		   
void RX_Mode(NRF24L01_CH_TypeDef *ch)
{
	NRF24L01_CE_LOW();	//����ģʽI	  
  	NRF24L01_Write_Buf(WRITE_2401_REG|(RX_ADDR_P0+ch->n),(u8*)ch->addr,RX_ADR_WIDTH);//дRX�ڵ��ַ	  
  	NRF24L01_Write_Reg(WRITE_2401_REG|EN_AA,(0x01<<(ch->n)));    //ʹ��ͨ�����Զ�Ӧ��    
  	NRF24L01_Write_Reg(WRITE_2401_REG|EN_RXADDR,(0x01<<(ch->n)));//ʹ��ͨ���Ľ��յ�ַ  	 
  	NRF24L01_Write_Reg(WRITE_2401_REG|RF_CH,ch->f);	     //����RFͨ��Ƶ��,0~127		  
  	NRF24L01_Write_Reg(WRITE_2401_REG|(RX_PW_P0+ch->n),RX_PLOAD_WIDTH);//ѡ��ͨ��0����Ч���ݿ�� 	    
  	NRF24L01_Write_Reg(WRITE_2401_REG|RF_SETUP,NRF24L01_RF_SETUP);//����TX�������,0db����,2Mbps,���������濪��   
  	NRF24L01_Write_Reg(WRITE_2401_REG|CONFIG, 0x0f);//���û�������ģʽ�Ĳ���;PWR_UP,EN_CRC,16BIT_CRC,����ģʽ 
													//ʹ���ж�
  	NRF24L01_CE_HIGH(); //CEΪ��,�������ģʽ 
        Soft_DelayUs(200);
}	
					 
//�ú�����ʼ��NRF24L01��TXģʽ
//����TX��ַ,дTX���ݿ��,����RX�Զ�Ӧ��ĵ�ַ,���TX��������,ѡ��RFƵ��,�����ʺ�LNA HCURR
//PWR_UP,CRCʹ��
//��CE��ߺ�,������RXģʽ,�����Խ���������		   
//CEΪ�ߴ���10us,����������.	 
void TX_Mode(NRF24L01_CH_TypeDef *ch)
{														 
	NRF24L01_CE_LOW();	    
  	NRF24L01_Write_Buf(WRITE_2401_REG|TX_ADDR,ch->addr,TX_ADR_WIDTH);//дTX�ڵ��ַ 
  	NRF24L01_Write_Buf(WRITE_2401_REG|RX_ADDR_P0,ch->addr,RX_ADR_WIDTH); //����RX�ڵ��ַ,�Զ�ACK��Ҫ	  

  	NRF24L01_Write_Reg(WRITE_2401_REG|EN_AA,(0x01<<(ch->n)));     //ʹ��ͨ��0���Զ�Ӧ��    
  	NRF24L01_Write_Reg(WRITE_2401_REG|EN_RXADDR,(0x01<<(ch->n))); //ʹ��ͨ��0�Ľ��յ�ַ  
  	NRF24L01_Write_Reg(WRITE_2401_REG|SETUP_RETR,0x1a);//�����Զ��ط����ʱ��:500us + 86us;����Զ��ط�����:10��
  	NRF24L01_Write_Reg(WRITE_2401_REG|RF_CH,ch->f);       //����RFͨ��Ϊ40
  	NRF24L01_Write_Reg(WRITE_2401_REG|RF_SETUP,NRF24L01_RF_SETUP);  //����TX�������,0db����,2Mbps,���������濪��   
  	NRF24L01_Write_Reg(WRITE_2401_REG|CONFIG,0x0e);    //���û�������ģʽ�Ĳ���;PWR_UP,EN_CRC,16BIT_CRC,����ģʽ,���������ж�
													//ʹ���ж�
	NRF24L01_CE_HIGH();//CEΪ��,10us����������
        Soft_DelayUs(100);

}		  




