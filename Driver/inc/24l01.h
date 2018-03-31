#ifndef __24L01_H
#define __24L01_H
	 		  
//CPU��ص�ͷ�ļ�
#include "common.h"
#include "spi.h"

//�ϵ�10.3ms��������ģʽ
//����PWR_UP,1.5ms��������ģʽI
//TX_FIFO�ǿգ�PRIM_RX=0,CEΪ��10us����뷢������״̬���ٹ�130us����
//���淢��ģʽ�ǣ�CE����10us�����壬�����������������ģʽI
//PRIM_RX=1,CEΪ�߽����������״̬��֮��130us��ʼ����

//6��ͨ������һ��Ƶ�Σ����в�ͬ��ַ���ҵ�ַ�����λ���벻ͬ��CH1~5���ֽڱ�����ͬ
//��ǿ��⧷�ģʽ�£�6�����ͷ��ѽ���ͨ��0��ΪACK����ͨ��������RX_ADDR_P0=TX_ADDR

//SPI�ɵ��ֽڵ����ֽڣ�ÿ���ֽ��ȷ�MSB
//д�Ĵ���ǰ���������������ģʽ
//����ģʽI��CE=0;����ģʽII:����ģʽʱ��TX_FIFOΪ��
//����ģʽ��PWR_UP=0

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//NRF24L01�Ĵ�����������
#define READ_2401_REG        0x00  //�����üĴ���,��5λΪ�Ĵ�����ַ
#define WRITE_2401_REG       0x20  //д���üĴ���,��5λΪ�Ĵ�����ַ
#define RD_RX_PLOAD     0x61  //��RX��Ч����,1~32�ֽ�
#define WR_TX_PLOAD     0xA0  //дTX��Ч����,1~32�ֽ�
#define FLUSH_TX        0xE1  //���TX FIFO�Ĵ���.����ģʽ����
#define FLUSH_RX        0xE2  //���RX FIFO�Ĵ���.����ģʽ����
#define REUSE_TX_PL     0xE3  //����ʹ����һ������,CEΪ��,���ݰ������Ϸ���.
#define NOP             0xFF  //�ղ���,����������״̬�Ĵ���	
	//��д�Ĵ������ܲ������ֽڼĴ��������ȶ�/д���ֽڵĸ�λ
 
//SPI(NRF24L01)�Ĵ�����ַ
#define CONFIG          0x00  //���üĴ�����ַ;
	#define PRIM_RX		0x01  //bit0:1����ģʽ,0����ģʽ;
	#define	PWR_UP		0x02  //bit1:�ϵ�ѡ��,1�ϵ磬0����
	#define CRCO		0x04  //bit2:CRCģʽ��0,8λCRC��1,16λCRC
	#define EN_CRC		0x08  //bit3:CRCʹ�ܣ����EN_AA ������һλΪ����EN_CRCǿ��Ϊ��
							  //��λֵΪ1
    #define MASK_MAX_RT 0x10  //bit4:�ж�MAX_RT(�ﵽ����ط������ж�)ʹ��;1,ʧ���ж�;0,MAX_RT�жϲ���ʱIRQ ���ŵ�ƽΪ�� 
	#define MASK_TX_DS	0x20  //bit5:�ж�TX_DSʹ��;1��ʧ�ܣ�0��TX_DS�жϲ���ʱIRQ ���ŵ�ƽΪ��
	#define MASK_RX_DR	0x40  //bit6:�ж�RX_DRʹ��;1��ʧ�ܣ�0��RX_RD�жϲ���ʱIRQ ���ŵ�ƽΪ��

#define EN_AA           0x01  //ʹ���Զ�Ӧ����  bit0~5,��Ӧͨ��0~5,ÿ��ͨ����λֵ��Ϊ1
							  //�˹��ܽ�ֹ�����nRF2401 ͨѶ

#define EN_RXADDR       0x02  //���յ�ַ����,bit0~5,��Ӧͨ��0~5
							  //0,1ͨ����λֵ����2~5ͨ����λֵ������

#define SETUP_AW        0x03  //���÷���/���� ��ַ���(��������ͨ��):
	#define AW_3B		0x01  //bit1,0:00,3�ֽ�;01,4�ֽ�;02,5�ֽ�;
	#define AW_4B		0x02  //Ĭ��5�ֽ�
	#define AW_5B		0x03

#define SETUP_RETR      0x04  //�����Զ��ط�;
							  //bit3:0,�Զ��ط�������;0,��ֹ�ط���1���Զ��ط�1�Σ�������15���Զ��ط�15��
							  	//Ĭ���Զ��ط�3��
							  //bit7:4,�Զ��ط���ʱ (250*(x+1)+86)us��Ĭ��x=0

#define RF_CH           0x05  //RFͨ��,bit6:0,����ͨ��Ƶ��;Ĭ��Ϊ2

#define RF_SETUP        0x06  //RF�Ĵ���;
							  //bit4:PLL_LOCK,PLL_LOCK������Ӧ���ڲ���ģʽ,Ĭ��Ϊ0
	#define RF_DR		0x08  //bit3:���ݴ�������(0:1Mbps,1:2Mbps);	Ĭ��2M
	#define RF_PWR		0x06  //bit2:1,���书��;00,-18;01,-12;10,-6;11,0;Ĭ��0dbm
	#define LNA_HCURR	0x01  //bit0:�������Ŵ�������,Ĭ��1

#define STATUS          0x07  //״̬�Ĵ���;
	#define TX_FULL		0x01  //bit0:TX FIFO����־;
	#define RX_P_NO		0x0E  //bit3:1,��������ͨ����(���:6);
					  			//000~101,����ͨ���ţ�111��RX_FIFOΪ��
	#define MAX_RT		0x10  //bit4,�ﵽ�����ط�������д1�����ϵͳ������ͨ��
    #define TX_DS       0x20  //bit5:���ݷ�������ж�;д1���
	#define RX_DR		0x40  //bit6:���������ж�; д1���

#define OBSERVE_TX      0x08  //���ͼ��Ĵ���,
							  //bit7:4,���ݰ���ʧ������;��дRF_CH�Ĵ���ʱ�˼Ĵ�����λ������ʧ 15�����ݰ���˼Ĵ�������
							  //bit3:0,�ط�������;���������ݰ�ʱ�˼Ĵ�����λ 

#define CD              0x09  //�ز����Ĵ���,bit0,�ز����;

#define RX_ADDR_P0      0x0A  //����ͨ��0���յ�ַ,��󳤶�5���ֽ�,��д���ֽ�
								//Ĭ��0xE7E7E7E7E7
#define RX_ADDR_P1      0x0B  //����ͨ��1���յ�ַ,��󳤶�5���ֽ�,��д���ֽ�
								//Ĭ��0xC2C2C2C2C2
#define RX_ADDR_P2      0x0C  //CH2���յ�ַ,Ĭ��0xC3������ֽڿ�����,���ֽ�,����ͬRX_ADDR_P1[39:8]���;
#define RX_ADDR_P3      0x0D  //CH3���յ�ַ,Ĭ��0xC4������ֽڿ�����,���ֽ�,����ͬRX_ADDR_P1[39:8]���;
#define RX_ADDR_P4      0x0E  //CH4���յ�ַ,Ĭ��0xC5������ֽڿ�����,���ֽ�,����ͬRX_ADDR_P1[39:8]���;
#define RX_ADDR_P5      0x0F  //CH5���յ�ַ,Ĭ��0xC6������ֽڿ�����,���ֽ�,����ͬRX_ADDR_P1[39:8]���;

#define TX_ADDR         0x10  //���͵�ַ(���ֽ���ǰ),ShockBurstTMģʽ��,RX_ADDR_P0��˵�ַ���
								//Ĭ��0xE7E7E7E7E7

#define RX_PW_P0        0x11  //��������ͨ��0��Ч���ݿ��(1~32�ֽ�),����Ϊ0��Ƿ�
#define RX_PW_P1        0x12  //��������ͨ��1��Ч���ݿ��(1~32�ֽ�),����Ϊ0��Ƿ�
#define RX_PW_P2        0x13  //��������ͨ��2��Ч���ݿ��(1~32�ֽ�),����Ϊ0��Ƿ�
#define RX_PW_P3        0x14  //��������ͨ��3��Ч���ݿ��(1~32�ֽ�),����Ϊ0��Ƿ�
#define RX_PW_P4        0x15  //��������ͨ��4��Ч���ݿ��(1~32�ֽ�),����Ϊ0��Ƿ�
#define RX_PW_P5        0x16  //��������ͨ��5��Ч���ݿ��(1~32�ֽ�),����Ϊ0��Ƿ�

#define FIFO_STATUS     0x17  //FIFO״̬�Ĵ���;
	#define RX_EMPTY	0x01  //bit0,RX FIFO�Ĵ����ձ�־;
	#define RX_FULL		0x02  //bit1,RX FIFO����־;
    #define TX_EMPTY	0x10  //bit4,TX FIFO�ձ�־;
	#define TX_FULL_F		0x20  //bit5,TX FIFO����־;
	#define TX_REUSE	0x40  //bit6,1,��CEλ�ߵ�ƽ,ѭ��������һ���ݰ�;0,��ѭ��;
								//TX_REUSEͨ��SPI  ָ��REUSE_TX_PL ���ã�ͨ��W_TX_PALOAD ��FLUSH_TX ��λ
//////////////////////////////////////////////////////////////////////////////////////////////////////////

//24L01���ü�������
#define NRF24L01_RF_SETUP	(RF_PWR|LNA_HCURR)	//0db����,2Mbps,���������濪��
#define NRF24L01_SPI_SPEED	SPI_BAUDRATEPRESCALER_8
#define NRF24L01_CE_PORT	GPIOA
#define NRF24L01_CE_PIN		2
#define NRF24L01_IRQ_PORT	GPIOA
#define NRF24L01_IRQ_PIN	1
#define NRF24L01_CSN_PORT	GPIOA
#define NRF24L01_CSN_PIN	3
#define NRF24L01_CE_HIGH()  PAset(NRF24L01_CE_PIN) //24L01��Ƶʹ���ź�
#define NRF24L01_CE_LOW()   PAclr(NRF24L01_CE_PIN) //24L01��Ƶʹ���ź�
#define NRF24L01_CSN_HIGH() PAset(NRF24L01_CSN_PIN) //SPIƬѡ�ź�	   
#define NRF24L01_CSN_LOW()  PAclr(NRF24L01_CSN_PIN)  //SPIƬѡ�ź�	
#define NRF24L01_IRQ  		PAin(NRF24L01_IRQ_PIN)  //IRQ������������
//24L01���ͽ������ݿ�ȶ���
#define TX_ADR_WIDTH    5   //5�ֽڵĵ�ַ���
#define RX_ADR_WIDTH    5   //5�ֽڵĵ�ַ���
#define TX_PLOAD_WIDTH  32  //n�ֽڵ��û����ݿ��
#define RX_PLOAD_WIDTH  32  //n�ֽڵ��û����ݿ��

//ͨ�������ṹ��
typedef struct
{
 	u8 n;	//6ͨ��ѡ��
	u8 addr[TX_ADR_WIDTH];	//ͨ����ַ
	u8 f;	//ͨ��Ƶ�ʣ�0~127
}NRF24L01_CH_TypeDef;

void NRF24L01_Init(void);//��ʼ��
void RX_Mode(NRF24L01_CH_TypeDef *ch);//����ĳͨ��Ϊ����ģʽ
void TX_Mode(NRF24L01_CH_TypeDef *ch);//����ĳͨ��Ϊ����ģʽ
u8 NRF24L01_Write_Buf(u8 reg, u8 *pBuf, u8 u8s);//д������
u8 NRF24L01_Read_Buf(u8 reg, u8 *pBuf, u8 u8s);//��������		  
u8 NRF24L01_Read_Reg(u8 reg);			//���Ĵ���
u8 NRF24L01_Write_Reg(u8 reg, u8 value);//д�Ĵ���
u8 NRF24L01_Check(void);//���24L01�Ƿ����
u8 NRF24L01_TxPacket(u8 *txbuf);//����һ����������
u8 NRF24L01_RxPacket(u8 *rxbuf);//����һ����������

#endif











