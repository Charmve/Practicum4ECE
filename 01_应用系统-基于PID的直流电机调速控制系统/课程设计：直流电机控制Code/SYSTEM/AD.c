#include	<STC15F2K60S2.h>
#include	"AD.h"
#include "myself.h"
#include	"Serial.h"
bit bFlagAD=0;
bit aFlagAD=0;
unsigned int cADCResult=0;
unsigned int AngADResult = 0;

unsigned char ch = 0;      //ADC通道号

void ADCInit()
{
		P1ASF = 0x03;                   //增加P11和P10口为AD口
    ADC_RES = 0;                    //清除结果寄存器
	  PADC = 1;
    ADC_CONTR = ADC_POWER | ADC_SPEEDHH | ADC_START | ch;//改为最高速度
		IE = IE | 0xa0 ;                //使能ADC中断
		DelayNms(2);//ADC上电并延时
}

void adc_isr() interrupt 5 using 3//AD 中断处理函数
{
    ADC_CONTR &= !ADC_FLAG;         //清除ADC中断标志
	
	  if(!ch)  {cADCResult = ADC_RES; bFlagAD = 1;}
		else {AngADResult = ADC_RES; aFlagAD = 1;}
}