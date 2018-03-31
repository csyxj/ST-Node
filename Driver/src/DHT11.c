#include "DHT11.h"

//Pin1 VDD 3~5V
//Pin2 DATA
//Pin4 GND
//ע���Ǵ��������濴

#define DAT_PORT  GPIOD
#define DAT_PIN   GPIO_PIN_3

//��ȡһ����ʪ��
//dat[0]=�¶ȣ�dat[1]=ʪ��
//���سɹ�0��ʧ��1
u8 Get_DHT11(u8 *dat)
{
 u8 i,j;
 u32 t;
 u8 tmp[5]={0,0,0,0,0};
  //��ʼ����������>18ms
  GPIO_Init(DAT_PORT,DAT_PIN,GPIO_MODE_OUT_PP_LOW_SLOW);
  Soft_DelayMs(20);
  //�ȴ���Ӧ
  GPIO_Init(DAT_PORT,DAT_PIN, GPIO_MODE_IN_PU_NO_IT);
  Soft_DelayUs(30);
  if((DAT_PORT->IDR&(DAT_PIN))) //δ��Ӧ�����Ǹ�
  {
    return 1;
  }
  else  //��Ӧ
  {
    //��ȡ����
    t=systick;
    while((DAT_PORT->IDR&(DAT_PIN))==0 && systick-t<2); //�ȴ���Ӧ�źŽ���  
    if(systick-t>=2)return 1;
    t=systick;
    while((DAT_PORT->IDR&(DAT_PIN)) && systick-t<2); //�ȴ��͵�ƽʱ϶����
    if(systick-t>=2)return 1;
    for(i=0;i<5;i++)
    {
      for(j=0;j<8;j++)
      {
         t=systick;
         while((DAT_PORT->IDR&(DAT_PIN))==0 && systick-t<2); //�ȴ��͵�ƽʱ϶���� 
         if(systick-t>=2)return 1;
         Soft_DelayUs(40);  //0,28us�ߵ�ƽ��1,,70us�ߵ�ƽ
         tmp[i]=tmp[i]<<1;
         if((DAT_PORT->IDR&(DAT_PIN)))  //������Ǹߣ���λΪ1
         {
            tmp[i]++;
            t=systick;
            while((DAT_PORT->IDR&(DAT_PIN)) && systick-t<2); //�ȴ��͵�ƽʱ϶����
            if(systick-t>=2)return 1;
         }
      }
    }
    //��֤���ݣ�����
    if(tmp[0]+tmp[1]+tmp[2]+tmp[3]!=tmp[4]) //У��ʧ��
    {
     return 1; 
    }
    else
    {
     dat[0]=tmp[0];
     dat[1]=tmp[2];
     return 0;
    }
  }
}

