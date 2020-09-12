#ifndef __MYSELF_H__
#define __MYSELF_H__

#include <STC15F2K60S2.h>
sbit TestKey = P5^4;//测试按键
sbit TestLed = P5^5;//测试LED
sbit Beep = P2^7;
sbit GDD = P2^6;

void DelayNms(int n);//延时Nms
void Delay1ms()	;	//@11.0592MHz

void Timer0Init(void);		//1毫秒@11.0592MHz//定时器0初始化
void Int0Init(void);		//外部中断0（下降沿）
bit KeyPress(bit KeyIO);//	将单个按键，按单次整理成函数
unsigned char Judge5DKeyNum(unsigned char ADCValue);//判断键值
unsigned char * Hex2ASCII(long int long28Value);//28位值转化为9位十进制ASCII
void MyGPIO_Inilize(void);
void MENU_oLED(void);

void Motor_Init( void );
void Motor_Start(void);
void Motor_Stop(void);
void Motor_Turn(bit n);
void Motor_add(); 
void Motor_dec();

//void  ye(int senddate,int m,int n);
#endif