/***************************************************
@author:�����
@date:2012-1-11
@description: uc1602��������
******************************************************/
#include "uc1602_driver.h"  

/***********************************************************
@function:  Com_Head(void)
@description:��������ͷ,д����
@input:      void
@output:     void
************************************************************/
static void Com_Head(void)
{
	i2c_Start();
	i2c_SendByteA(0x78);
}

/***********************************************************
@function:  WD_Head(void)
@description:��������ͷ,д����
@input:      void
@output:     void
************************************************************/
static void WD_Head(void)
{
	i2c_Start();
	i2c_SendByteA(0x7a);
}

/***********************************************************
@function:  RD_Head(void)
@description:��������ͷ,������
@input:      void
@output:     void
************************************************************/
static void RD_Head(void)
{
	i2c_Start();
	i2c_SendByteA(0x7b);
}

/***********************************************************
@function:  Init_UC1602(void)
@description:��ʼ��
@input:      void
@output:     void
************************************************************/
void UC1602_Init(void)
{
        while(i2c_CheckDevice(0x7b));
 	Com_Head();
	i2c_SendByteA(0x0e2);   //��λ
	i2c_Stop();
	Soft_DelayMs(20);
	Com_Head();
			//��ʾ��  Ĭ���ǹ� ����һ��Ҫ����
	i2c_SendByteA(0x0af);
		//ȫ��ʾ    A5 ȫ��ʾ  A4��ȫ��ʾ
	i2c_SendByteA(0x0a4);
		//����
	i2c_Stop();
	UC1602_Clear();
//	Init_GBK1616();//��ʼ���ֿ�
//	//��Դ����
//	i2c_SendByteA(0x2e);  
//	//�¶Ȳ��� 
//	i2c_SendByteA(0x26);
//	//������ѹ   
//	i2c_SendByteA(0x81);   //�̶��ֽ�
//	i2c_SendByteA(0x62);   //�Աȶȣ�0~255��Ĭ��62H

//	//LCD ƫ��
//	i2c_SendByteA(0x0eb);   //LCD ƫ�� EB=1/9  Ĭ��
//	//AC ҳ���Զ����� �����ӷ��� Ĭ��89
//	i2c_SendByteA(0x89);
//	//������ʾ 84��ֹ 85 ����
//	i2c_SendByteA(0x84);
//	//������ʼ�У�������ʼ�� ��Χ0-63 ��Ч�ã��ɲ���
//	i2c_SendByteA(0x40);


//	//MX MY ����˳��  Ĭ��C0 �� CC �ǵ��� ��  c8�ǵ��ľ��� c4�����ľ���
//	i2c_SendByteA(0x0c0);

//	// ��ת��ʾ A6 ����ת A7 ��ת
//	i2c_SendByteA(0x0a6);
//	//COM END �����ý����缫
//	i2c_SendByteA(0x0f1);
//	i2c_SendByteA(0x03f);
}

/***********************************************************
@function:  Clear_UC1602(void)
@description:����
@input:      void
@output:     void
************************************************************/
void UC1602_Clear(void)
{
u8 i,j;
	Com_Head();
		//������
	i2c_SendByteA(0x00);   //�е���λ   0000****	 
	i2c_SendByteA(0x10);   //�и���λλ 00010***
		//������ʼҳ��ַ�� ��8ҳ
	i2c_SendByteA(0x0b0);  //ҳ��ַ		1011****
	i2c_Stop();
	WD_Head();
	for(i=0;i<9;i++)
	{
		for(j=0;j<102;j++)
			i2c_SendByteA(0x00);
	}
	i2c_Stop();
}

/***********************************************************
@function:  UC1602_ClearRec(u8 xs,u8 xe,u8 ys,u8 ye)
@description:���������
@input:      xs xe ys ye
@output:     void
************************************************************/
void UC1602_ClearRec(u8 xs,u8 xe,u8 ys,u8 ye)
{
u8 x,y;
	for(y=ys;y<=ye;y++)
	{
		set_xy(xs,y);
		WD_Head();
		for(x=xs;x<=xe;x++)
		{
			i2c_SendByteA(0x00);
		}	
	}


}

/***********************************************************
@function:  disp_85(u8 *p,u8 x,u8 y)
@description:��ʾ8*5��ASCII�ַ�
@input:      p ASCII������ָ��,x=0~95,y=0~8
@output:     ������ʾ��ɺ���ָ��ڼ���
************************************************************/
u8 UC1602_Disp85(u8 *p,u8 x,u8 y)
{
u8 i,j,k;
	k=0;	//�Զ�������
	x%=96;
	y%=8;
	set_xy(x,y);
	WD_Head();
	for(i=0;p[i]>=32&&p[i]<=126;i++)
	{
		if(WRAP&&(x+i*(5+GAP85)-102*k)>91) //����Զ����п��������0����һ��
		{
			for(j=0;j<102*(k+1)-x-i*(5+GAP85);j++)
				i2c_SendByteA(0x00);	//���0
			k+=1;
		}

		for(j=0;j<5;j++){
	 		i2c_SendByteA(ASCII85[p[i]-0x20][j]);	//��ASCII���д�ַ�
		}
		for(j=0;j<GAP85;j++){
			i2c_SendByteA(0x00);		//����ַ����
		}
	}
	i2c_Stop();
	return y+k;
}

/***********************************************************
@function:  disp_a(u8 p,u8 x,u8 y)
@description:��ʾһ��8*5��ASCII�ַ�
@input:      p ASCII��,x=0~95,y=0~8
@output:     ������ʾ��ɺ���ָ��ڼ���
************************************************************/
void UC1602_DispA(u8 p,u8 x,u8 y)
{
u8 j;
	x%=96;
	y%=8;
	set_xy(x,y);
	WD_Head();
		for(j=0;j<5;j++){
	 		i2c_SendByteA(ASCII85[p-0x20][j]);	//��ASCII���д�ַ�
		}
		for(j=0;j<GAP85;j++){
			i2c_SendByteA(0x00);		//����ַ����
		}
	i2c_Stop();
}

/***********************************************************
@function:  disp_GBK(u8 *p,u8 x,u8 y)
@description:��ʾ16*16�ĺ���
@input:      p ASCII������ָ��,x=0~95,y=0~8
@output:     ������ʾ��ɺ���ָ��ڼ���
************************************************************/
u8 UC1602_DispGBK(u8 *p,u8 x,u8 y)
{
u8 i,j;
u8 index;//�ַ�����������±�
u16 GBKcode;
	while(x>95||y>8);//������ͣ����
	set_xy(x,y);
	for(i=0;p[i];i+=2)
	{
		if(WRAP&&x>(80-GAPGBK)) //����Զ����п�������������һ��
		{
			y+=2;
			x=0;
		}
		GBKcode=p[i]+(p[i+1]<<8);
		for(index=0;GBK1616_ref[index*2]!='0';index++){
			if(((u16)GBK1616_ref[index])==GBKcode)break;//�˴�Ҫ��u8->u16��ת��,*((u16*)(GBK1616_ref+index))
		}	//�������������λ��

		set_xy(x,y);
		WD_Head();
		for(j=0;j<16;j++){
	 		i2c_SendByteA(*(GBK1616+index*32+j));	//��ASCII���д�ַ��ϰ벿��
		}
		for(j=0;j<GAPGBK;j++){
			i2c_SendByteA(0x00);		//����ַ����
		}
		i2c_Stop();
		set_xy(x,y+1);
		WD_Head();
		for(j=16;j<32;j++){
	 		i2c_SendByteA(*(GBK1616+index*32+j));	//��ASCII���д�ַ��°벿��
		}
		for(j=0;j<GAPGBK;j++){
			i2c_SendByteA(0x00);		//����ַ����
		}
		i2c_Stop();
		x+=(16+GAPGBK);
	}
	return y;
}

/***********************************************************
@function:  u8 disp_pic(u8 *p,u8 x,u8 y,u8 l,u8 h)
@description:��x,y����ͼ ,ÿ8��ȡģ������8��ȡģ��
@input:      p,ͼ�������ַ��x,y,l����������h����������
@output:     y������yֵ
************************************************************/
u8 UC1602_DispPic(u8* p,u8 x,u8 y,u8 l,u8 h)
{
u16 index;
u8 i,j;
	for(i=0;i<(h/8+1-!(h%8));i++)
	{
		set_xy(x,y);
		WD_Head();
		for(j=0;j<l;j++)
		{
			index=i*l+j;
	 		i2c_SendByteA(p[index]);
		}
		i2c_Stop();
		y++;
	}
	return y;

}


/***********************************************************
@function:  set_xy(u8 x,u8 y)
@description:�������x,y
@input:      x,y
@output:     void
************************************************************/
static void set_xy(u8 x,u8 y)
{
u8 tmp;
	do{
	tmp=0;
 	//Com_Head();
	i2c_Start();
	tmp|=i2c_SendByteA(0x78);
		//������
	tmp|=i2c_SendByteA(x&0x0f);   //�е���λ   0000****	 
	tmp|=i2c_SendByteA((x>>4)|0x10);   //�и���λλ 00010***
		//������ʼҳ��ַ�� ��8ҳ
	tmp|=i2c_SendByteA(y|0x0b0);  //ҳ��ַ		1011****
	i2c_Stop();
	}while(tmp);
}

/***********************************************************
@function:  UC1602_Scroll(u8 n)
@description:���Ͼ���
@input:      ��n��
@output:     void
************************************************************/
void UC1602_Scroll(u8 n)
{
	Com_Head();
	i2c_SendByteA(0x40|n);
	i2c_Stop();
}

//����ĳλ�õĵ�
void UC1602_SetP(u8 x,u8 y,u8 color)
{
u8 tmp;
	//��������
 	set_xy(x,y/8);
	//��ԭ��������
	RD_Head();
	tmp=i2c_ReadByteA();
	i2c_Stop();
	//д����
	if(color)tmp|=(0x01<<(y%8));
	else tmp&=~(0x01<<(y%8));
	set_xy(x,y/8);
	WD_Head();
	i2c_SendByteA(tmp);
	i2c_Stop();
}

//��ȡĳλ�õ���Ϣ
u8 UC1602_GetP(u8 x,u8 y)
{
u8 tmp;
	//��������
 	set_xy(x,y/8);
	//��ԭ��������
	RD_Head();		
	tmp=i2c_ReadByteA();
	i2c_Stop();
	tmp=(tmp>>(y%8))&0x01;
	return tmp;
}

//��ɫĳλ�õ���Ϣ
void UC1602_XorP(u8 x,u8 y)
{
u8 tmp;
	//��������
 	set_xy(x,y/8);
	//��ԭ��������
	RD_Head();
	tmp=i2c_ReadByteA();
	i2c_Stop();
	//��ɫ
	tmp^=(0x01<<(y%8));
	set_xy(x,y/8);
	WD_Head();
	i2c_SendByteA(tmp);
	i2c_Stop();
}

void UC1602_SetPage(u8 x,u8 y,u8 dat)
{
	//��������
 	set_xy(x,y/8);
	WD_Head();
	i2c_SendByteA(dat);
	i2c_Stop();
}

//void UC1602_Test(void)
//{
//u8 i,j,tmp;
//
//}

//����ת��Ϊ�ַ���
/**
* @brief  ����ת��Ϊ�ַ���������Ϊ����
* @param    
* @retval �ַ�������
*/   
u8 Int2Str(int num,u8 *str)
{
u8 i,j;
u32 tmp=abs(num);
	for(i=0;;i++)	//num�м�λ
	{
		tmp/=10;
	 	if(!tmp)break;		
	}
	if(num<0)i++;
	tmp=abs(num);
	for(j=0;j<=i;j++)
	{
	 	str[i-j]=tmp%10+'0';
		tmp/=10;
	}
	if(num<0)str[0]='-';
	str[i+1]=0;
	return i+1;
}

//������ת�ַ���
/****************************************************************************
* ��    �ƣ�void Float2Str(float a,char * str ,u8 len)
* ��    �ܣ�������ת�ַ���
* ��ڲ�����a	��ת���ĸ�����
			str ������׵�ַ
			len ����
* ���ڲ�������
* ˵    ����Ĭ�ϰ���С���㣬ֻ��ת����
****************************************************************************/
const int pow10[]={1,10,100,1000,10000,100000,1000000,10000000,100000000};
void Float2Str(float a,char * str ,u8 len)
{
u8 i,j;
u16 b=(u16)a;
	for(i=0;;i++)
	{
		b/=10;
	 	if(!b)break;
	}	//�˳���iΪa��λ��-1
	b=(u16)a;
	for(j=0;len&&(i!=0xff);len--,i--,j++)
	{
	 	str[j]=(b/pow10[i])%10+'0';
	}
	if(len)
	{
		str[j]='.';
		j++;
	 	a-=b;
		a*=pow10[len];
		b=(u16)a;
		for(;len;len--,j++)
		{
		 	str[j]=(b/pow10[len-1])%10+'0';
		}
	}
	str[j]=0;
}


//ָ��λ����ʾ����,����룬���ʾlen���ַ�����������油�ո�
void UC1602_DispInt(int num,u8 x,u8 y,u8 len)
{
char str[12];
u8 tmp;
  tmp=Int2Str(num,str);
  if(tmp<len) {
    memset(str+tmp,' ',len-tmp); //���ո�
  }
  str[len]=0;  //�ض� 
  UC1602_Disp85(str,x,y);
  return;
}


//ָ��λ����ʾ������