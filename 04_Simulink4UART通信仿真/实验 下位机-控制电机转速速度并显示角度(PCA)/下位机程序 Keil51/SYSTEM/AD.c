#include "AD.h"
#include "intrins.h"

unsigned char high;
unsigned char low;
u16 date,angle;
//unsigned char h8,l8;
unsigned char ang[3]={0};

void ADC_init(void)
{
		P1M0 = 0x00;                               //设置P1.0为ADC口
    P1M1 = 0x01;
    ADCCFG = 0x0f;                           //设置ADC时钟为系统时钟/2/16/16
}

 
void ADC_contrl(void)
{
	  ADC_CONTR = 0x80;                           //使能ADC模块
    ADC_CONTR |= 0x40;                          //启动AD转换
    _nop_();
    _nop_();
    while (!(ADC_CONTR & 0x20));                //查询ADC完成标志
    ADC_CONTR &= ~0x20;                         //清完成标志
}

void ADC_data(void)
{
//    ADCCFG = 0x00;                              //设置结果左对齐
//    high = ADC_RES;                              //A存储ADC的12位结果的高8位
//    low = ADC_RESL;                               //B[7:4]存储ADC的12位结果的低4位,B[3:0]为0
	
  ADCCFG = 0x20;                              //设置结果右对齐
  high = ADC_RES;                              //A[3:0]存储ADC的12位结果的高4位,A[7:4]为0
  low = ADC_RESL;                               //B存储ADC的12位结果的低8位
	
}

void Converse(void)   //转换成角度
{
	date=high*16*16+low; 
	angle=(360.0/3510.0)*date; //角度计算公式
	
	ang[0]=(angle>>8)&0xff;  //角度值高八位
	ang[1]=angle&0xff;        //角度值第八位
	
}