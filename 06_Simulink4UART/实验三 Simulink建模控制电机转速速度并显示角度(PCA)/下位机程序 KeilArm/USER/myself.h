#ifndef __MYSELF_H__
#define __MYSELF_H__

#include "config.h"

extern u16 R;
extern u16 G;
extern u16 Bl;


void DelayNms(int n);//延时Nms
void Delay1ms();	//@11.0592MHz

void Timer0Init(void);		//1毫秒@11.0592MHz //定时器0初始化

void conversion(uint temp_data);   //这是因为在h文件中没有uchar、uint之类的定义  所以会出现语法错误!!!

extern uchar disp_R[3];  //红
extern uchar disp_G[3];  //绿
extern uchar disp_B[3];  //蓝


#endif