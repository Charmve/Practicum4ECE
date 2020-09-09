#ifndef		__AD_H
#define		__AD_H

#include "config.h"

void ADC_init(void);//初始化
void ADC_contrl(void);//AD相关控制
void ADC_data(void);//数据格式
void Converse(void);//转换角度

extern unsigned char high;  //高八位
extern unsigned char low;  //低八位
extern u16 date,angle;
//extern unsigned char h8,l8;
extern unsigned char ang[];
#endif
