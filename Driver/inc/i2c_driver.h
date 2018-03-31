/***************************************************
@author:�޸� BY oscarhua_zju ԭ������
@date:2011-8-03
@description: ģ��I2C
              ��ֲ-��ʱ�����ʵ�ֺ�Ӳ���˿ڳ�ʼ����������
******************************************************/
#ifndef __I2C_DRIVER_H
#define __I2C_DRIVER_H

#include "common.h"

/* ����I2C�������ӵ�GPIO�˿�, �û�ֻ��Ҫ�޸�����4�д��뼴������ı�SCL��SDA������ */
//#define GPIO_PORT_I2C	GPIOB			/* GPIO�˿� */
//#define RCC_I2C_PORT 	RCC_APB2Periph_GPIOB		/* GPIO�˿�ʱ�� */
//#define I2C_SCL_PIN		GPIO_Pin_6			/* ���ӵ�SCLʱ���ߵ�GPIO */
//#define I2C_SDA_PIN		GPIO_Pin_7			/* ���ӵ�SDA�����ߵ�GPIO */

/* �����дSCL��SDA�ĺ꣬�����Ӵ���Ŀ���ֲ�ԺͿ��Ķ��� */
/*��ע�⣺����д������IAR��߼����Ż�ʱ���ᱻ�����������Ż� */
#define I2C_SCL_1()  PBset(4)		                /* SCL = 1 */
#define I2C_SCL_0()  PBclr(4)				/* SCL = 0 */

#define I2C_SDA_1()  PBset(5)				/* SDA = 1 */
#define I2C_SDA_0()  PBclr(5)				/* SDA = 0 */

#define I2C_SDA_READ()  (PBin(5)!= 0)	                /* ��SDA����״̬ */

#define I2C_WR	0		/* д����bit */
#define I2C_RD	1		/* ������bit */

//��������
void i2c_Cfg(void);
void i2c_Start(void);
void i2c_Stop(void);
void i2c_SendByte(uint8_t _ucByte);
u8 i2c_SendByteA(uint8_t _ucByte);
uint8_t i2c_ReadByte(void);
uint8_t i2c_ReadByteA(void);
uint8_t i2c_WaitAck(void);
void i2c_Ack(void);
void i2c_NAck(void);
uint8_t i2c_CheckDevice(uint8_t _Address);

#endif

/********************************************END OF FILE*********************************************************/


