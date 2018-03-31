#include "wpan.h"
#include "delay.h"
#include <string.h>
#include "app.h"

//WPAN�ܳ�
#define WPAN_KEY_LEN 8
const u8 wpan_key[WPAN_KEY_LEN]={0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88};
const u8 bc_addr[WPAN_ADDR_SIZE]={0xff,0xff,0xff,0xff,0xff};
const u8 mytype=DEV_HUMITURE;
const u8 myaddr[WPAN_ADDR_SIZE]={0x02,0xff,0xff,0xff,0xff};
u8 myid;
//  ͨ���� ��ַ�� Ƶ��
static NRF24L01_CH_TypeDef mych;    //���ڵ��ַ
static NRF24L01_CH_TypeDef ch;   //����/����ʹ�õĽڵ��ַ
 pkt_t wpan_pkt;
static u8 wpan_state=0;

extern attr_func attr_list[];

//���תС��
void b2l(u8* big)
{
   *(big) ^= *(big+3)^= *(big) ^= *(big+3);
   *(big+1) ^= *(big+2)^= *(big+1) ^= *(big+2);
}

/* ���Դ���demo
//����1����temp
u8 attr1(u8 rw, dat_t* dat)
{
  if(!rw) { //R0
    dat->u32 = dht[1];
    b2l((u8 *)(&(dat->u32)));
  }
  return 1;
}
*/

//�������ݰ��� ������ɺ��Զ����ؼ����Լ���ͨ��
//0,ok;1,err
 u8 send_pkt(pkt_t * pkt)
{
    u32 t;
    u8 res;
    TX_Mode(&ch);//׼������//130us
    t = systick;
    do {
        res = NRF24L01_TxPacket((u8 *)pkt);//�������� //���ʹ���������ʱ10.7ms//���ͳɹ�830us
    }while(res != TX_DS && (systick - t < 10));//20ms�ڲ�ͣ����//5us 
    RX_Mode(&mych);//��󷵻ؽ���ģʽ//100us
    return (res != TX_DS);
}

//�������ݰ�
//0,ok;1,err
u8 rec_pkt(pkt_t * pkt)
{
    u32 t;
    u8 res = 1;
    RX_Mode(&ch);   //ʹ�öԶ˵�ַ����
    t = systick;
    while(systick - t < 10 && NRF24L01_IRQ); //�ȴ�20ms����ȡ����ֵ
    if(systick - t < 10)
    {
        NRF24L01_RxPacket((u8 *)pkt); //170us��ȡ
        res = 0;
    }
    RX_Mode(&mych);//��󷵻ر�������ģʽ
    return res;
}

//������յ������ݰ�
//����:1,�����ԣ���������ֵ��0��д���ԣ����÷���
static u8 deal_pkt(pkt_t * pkt)
{
u8 i,j,flag;
u8 attr;
u8 rw;
  switch(pkt->header) {
  case WPAN_RW:
    for(i = 0, j = 0, flag = 0; i < WPAN_CMD_N && pkt->pl.cmd[i].type != 0 ; i++) {
      attr = (pkt->pl.cmd[i].type) & WPAN_ATTR_MASK;
      rw = (pkt->pl.cmd[i].type) & WPAN_RW_MASK;
      if(!rw) {     //������
        flag=1;     //��ʾ��Ҫ�������� 
      }
      if(attr <= ATTR_N) { //��Чattr
        attr_list[attr-1](rw, &(pkt->pl.cmd[i].dat));//�����Ժ�����ԭλ�ö�ȡ/д�����ݣ�������ת��
      }
      if(j != i) {  //��ǰŲ����
        memcpy(&pkt->pl.cmd[j], &pkt->pl.cmd[i], WPAN_CMD_LEN);
      }
      j++;
    }
    break;
  case WPAN_INVITE:
    if(wpan_state==0 && memcmp(pkt->pl.u8, wpan_key, WPAN_KEY_LEN) == 0) {//δ�������ܳ���ȷ
      flag=1;//Ҫ����dev_type��addr
      pkt->header = WPAN_JOIN;
      pkt->pl.u8[0] = mytype;//�豸����
      memcpy(pkt->pl.u8 + 1, myaddr, WPAN_ADDR_SIZE);//��ַ
    }
    break;
  case WPAN_ASSIGN:
    if(wpan_state == 0) {//δ����
      flag = 0;
      pkt->header = WPAN_JOIN;
      myid = pkt->pl.u8[0];//�豸id
    }
    break;
  default:
    break; 
  }
  return flag;
}


void wpan_init(NRF24L01_CH_TypeDef *init_ch)
{
    NRF24L01_Init();
    while(NRF24L01_Check());
    memcpy(&mych, init_ch, sizeof(NRF24L01_CH_TypeDef));
    CH_USE(&mych);
    RX_Mode(&ch);
}

//��ѭ��������ݰ�
//�����Ƿ�ɹ��������0�ɹ���1ʧ�ܻ�������
u8 deal_wpan(void)  //wpan����Ƿ��а���������
{
    if(NRF24L01_IRQ==0) {   //�յ���
        NRF24L01_RxPacket((u8 *)&wpan_pkt); //170us��ȡ
        if(deal_pkt(&wpan_pkt)) {   //����1��ʾ����Ϣ��Ҫ��������//40us   
            return send_pkt(&wpan_pkt);
        }
        return 0;
    }
    return 1;
}

u8 wpan_readAttr(u8* addr, u8 attr, u32* val)
{
    memcpy(ch.addr, addr, WPAN_ADDR_SIZE);
    wpan_pkt.header = WPAN_RW;
    wpan_pkt.reserve = 0;
    WPAN_CMD_TYPE(0) = attr & WPAN_ATTR_MASK;
    WPAN_CMD_DAT(0) = *val; //��Ϊ���Ӳ�������ѡ
    if(send_pkt(&wpan_pkt)) return 1;   //����ʧ��
    if(!rec_pkt(&wpan_pkt)) {    //�յ���
        *val = WPAN_CMD_DAT(0); //TODO: check pkt
        return 0;
    }
	else {
		return 1;   //��ʱδ�յ�
	}	  
}

u8 wpan_writeAttr(u8* addr, u8 attr, u32* val)
{
    memcpy(ch.addr, addr, WPAN_ADDR_SIZE);
    wpan_pkt.header = WPAN_RW;
    wpan_pkt.reserve = 0;
    WPAN_CMD_TYPE(0) = WPAN_RW_MASK | (attr & WPAN_ATTR_MASK);
    WPAN_CMD_DAT(0) = *val;
    return send_pkt(&wpan_pkt); 
}
