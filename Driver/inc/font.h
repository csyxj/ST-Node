/***************************************************
@author:�����
@date:2012-1-19
@description: �ֿ��ļ�
******************************************************/
#ifndef __FONT_H
#define __FONT_H

//CPU��ص�ͷ�ļ�
#include "common.h" 
#include <string.h>

#ifdef FONT_GLOBAL
	#define FONT_EXT
#else
	#define FONT_EXT extern
#endif

FONT_EXT const u8 ASCII85[95][5];
FONT_EXT const u8 GBK1616_ref[];
FONT_EXT const u8 GBK1616[];

#endif 
