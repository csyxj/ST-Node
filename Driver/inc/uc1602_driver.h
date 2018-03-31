/***************************************************
@author:�����
@date:2012-1-11
@description: I2C������
******************************************************/
#ifndef __UC1602_DRIVER_H
#define __UC1602_DRIVER_H

//CPU��ص�ͷ�ļ�
#include "delay.h"
#include "i2c_driver.h" 
#include "font.h"

#define WRAP 1	//�Զ�����
#define GAP85 1	//8*5�ַ����
#define GAPGBK 0	//GBK�ַ����
#define PI 3.14159265359

//��������
static void Com_Head(void);
static void WD_Head(void);
static void RD_Head(void);
void UC1602_Init(void);
void UC1602_Clear(void);
void UC1602_ClearRec(u8 xs,u8 xe,u8 ys,u8 ye);
u8 UC1602_Disp85(u8 *p,u8 x,u8 y);
void UC1602_DispA(u8 p,u8 x,u8 y);
u8 UC1602_DispGBK(u8 *p,u8 x,u8 y);
u8 UC1602_DispPic(u8* p,u8 x,u8 y,u8 l,u8 h);
static void set_xy(u8 x,u8 y);
void UC1602_Scroll(u8 n);
void UC1602_SetP(u8 x,u8 y,u8 color);
u8 UC1602_GetP(u8 x,u8 y);
void UC1602_XorP(u8 x,u8 y);
#pragma inline
void UC1602_SetPage(u8 x,u8 y,u8 dat);
//void UC1602_Test(void);
u8 Int2Str(int num,u8 *str);
void Float2Str(float a,char * str ,u8 len);
void UC1602_DispInt(int num,u8 x,u8 y,u8 len);

#endif 
/********************************************END OF FILE*********************************************************/




