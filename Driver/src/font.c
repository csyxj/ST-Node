#define FONT_GLOBAL
#include "font.h"


//8*5 ÿ����ʾ20����8�С������һ��ÿ����ʾ16����
const u8 ASCII85[95][5]=
{
{0x00,0x00,0x00,0x00,0x00},//�ո�
{0x00,0x00,0x5F,0x00,0x00},//!
{0x00,0x07,0x00,0x07,0x00},//"
{0x14,0x7F,0x14,0x7F,0x14},//#
{0x24,0x2A,0x7F,0x2A,0x12},//$
{0x23,0x13,0x08,0x64,0x62},//%
{0x36,0x49,0x56,0x20,0x50},//&
{0x00,0x08,0x07,0x03,0x00},//'
{0x00,0x1C,0x22,0x41,0x00},//(
{0x00,0x41,0x22,0x1C,0x00},//)
{0x24,0x18,0x7E,0x18,0x24},//*
{0x08,0x08,0x3E,0x08,0x08},//+
{0x00,0x80,0x70,0x30,0x00},//,
{0x08,0x08,0x08,0x08,0x08},//-
{0x00,0x00,0x60,0x60,0x00},//.
{0x20,0x10,0x08,0x04,0x02},///
{0x3E,0x41,0x49,0x41,0x3E},//0
{0x00,0x42,0x7F,0x40,0x00},//1
{0x72,0x49,0x49,0x49,0x46},//2
{0x21,0x41,0x49,0x4D,0x32},//3
{0x18,0x14,0x12,0x7F,0x10},//4
{0x27,0x45,0x45,0x45,0x38},//5
{0x3C,0x4A,0x49,0x49,0x31},//6
{0x41,0x21,0x11,0x09,0x07},//7
{0x36,0x49,0x49,0x49,0x36},//8
{0x46,0x49,0x49,0x29,0x16},//9
{0x00,0x00,0x14,0x00,0x00},//:
{0x00,0x40,0x34,0x00,0x00},//;
{0x00,0x08,0x14,0x22,0x41},//<
{0x14,0x14,0x14,0x14,0x14},//=
{0x00,0x41,0x22,0x14,0x08},//>
{0x02,0x01,0x59,0x09,0x06},//?
{0x3E,0x41,0x5D,0x59,0x4E},//@
{0x7C,0x12,0x11,0x12,0x7C},//A
{0x7F,0x49,0x49,0x49,0x36},//B
{0x3E,0x41,0x41,0x41,0x22},//C
{0x7F,0x41,0x41,0x41,0x3E},//D
{0x7F,0x49,0x49,0x49,0x41},//E
{0x7F,0x09,0x09,0x09,0x01},//F
{0x3E,0x41,0x41,0x51,0x73},//G
{0x7F,0x08,0x08,0x08,0x7F},//H
{0x00,0x41,0x7F,0x41,0x00},//I
{0x20,0x40,0x41,0x3F,0x01},//J
{0x7F,0x08,0x14,0x22,0x41},//K
{0x7F,0x40,0x40,0x40,0x40},//L
{0x7F,0x02,0x1C,0x02,0x7F},//M
{0x7F,0x04,0x08,0x10,0x7F},//N
{0x3E,0x41,0x41,0x41,0x3E},//O
{0x7F,0x09,0x09,0x09,0x06},//P
{0x3E,0x41,0x51,0x21,0x5E},//Q
{0x7F,0x09,0x19,0x29,0x46},//R
{0x26,0x49,0x49,0x49,0x32},//S
{0x03,0x01,0x7F,0x01,0x03},//T
{0x3F,0x40,0x40,0x40,0x3F},//U
{0x1F,0x20,0x40,0x20,0x1F},//V
{0x3F,0x40,0x38,0x40,0x3F},//W
{0x63,0x14,0x08,0x14,0x63},//X
{0x03,0x04,0x78,0x04,0x03},//Y
{0x61,0x59,0x49,0x4D,0x43},//Z
{0x00,0x7F,0x41,0x41,0x41},//
{0x02,0x04,0x08,0x10,0x20},//"
{0x00,0x41,0x41,0x41,0x7F},//]
{0x04,0x02,0x01,0x02,0x04},//^
{0x40,0x40,0x40,0x40,0x46},//_
{0x00,0x03,0x07,0x08,0x00},//��
{0x20,0x54,0x54,0x78,0x40},//a
{0x7F,0x28,0x44,0x44,0x38},//b
{0x38,0x44,0x44,0x44,0x28},//c
{0x38,0x44,0x44,0x28,0x7F},//d
{0x38,0x54,0x54,0x54,0x18},//e
{0x00,0x08,0x7E,0x09,0x02},//f
{0x38,0xA4,0xA4,0x9C,0x78},//g
{0x7F,0x08,0x04,0x04,0x78},//h
{0x00,0x44,0x7D,0x40,0x00},//i
{0x20,0x40,0x40,0x3D,0x00},//j
{0x7F,0x10,0x28,0x44,0x00},//k
{0x00,0x41,0x7F,0x40,0x00},//l
{0x7C,0x04,0x78,0x04,0x78},//m
{0x7C,0x08,0x04,0x04,0x78},//n
{0x38,0x44,0x44,0x44,0x38},//o
{0xFC,0x18,0x24,0x24,0x18},//p
{0x18,0x24,0x24,0x18,0xFC},//q
{0x7C,0x08,0x04,0x04,0x08},//r
{0x48,0x54,0x54,0x54,0x24},//s
{0x04,0x04,0x3F,0x44,0x24},//t
{0x3C,0x40,0x40,0x20,0x7C},//u
{0x1C,0x20,0x40,0x20,0x1C},//v
{0x3C,0x40,0x20,0x40,0x3C},//w
{0x44,0x28,0x10,0x28,0x44},//x
{0x4C,0x90,0x90,0x90,0x7C},//y
{0x44,0x64,0x54,0x4C,0x44},//z  
{0x00,0x08,0x36,0x41,0x00},//{
{0x00,0x00,0x77,0x00,0x00},//|
{0x00,0x41,0x36,0x08,0x00},//}
{0x02,0x01,0x02,0x04,0x02}//~
};

//�˴���Ȼд��u8����ʵ����u16����Ӣ�ĵ���ʱ����u8
const u8 GBK1616_ref[]="";
const u8 GBK1616[]= {0x00};//ȡģ˳����ͬ�е�16�У�������16��.
