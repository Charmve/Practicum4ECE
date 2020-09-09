#include	"config.h"
#include "intrins.h"
#include <Motor.h>
#include	"Serial.h"
#include "AD.h"

#define TurnOn 0;  //灯亮
#define TurnOff 1; //灯灭
 
unsigned char speed;
unsigned int time;
unsigned char receive,i;

sbit LED=P5^5;
	
void Timer0Init(void);		//1ms@11.0592MHz
void PWM_init();
void PWM_Contrl(unsigned char count);

void main()
{
	extern bit uart_flag;//串口接收到数据标志
	
	Motor_Init();  //直流电机初始化
	Exinit();      //外部中断初始化
	
	ADC_init();    //AD初始化
	
	PS=1;          //串口优先级为高
	PT0=0;
	Uart1Init();  //串口初始化
	Timer0Init();  //定时器0初始化
	PWM_init();     //PCA初始化
	EA = 1; 

  while(1)
	{	
		ADC_contrl(); //AD设置
		ADC_data();   //读出数据
		Converse();   //角度转换
		Motor_TN();

		if(uart_flag==1)   //若正确接收到一帧数据
		{
			LED=TurnOn;
			receive=cS1Rec[6];
			
			
			uart_flag=0;     //标志位清零
			
		}
		PWM_Contrl(receive);
	}
}


void Timer0Init(void)		//1ms@24MHz
{
	AUXR |= 0x80;		//定时器时钟1T模式
	TMOD &= 0xF0;		//设置定时器模式
	TL0 = 0x40;		//设置定时初值
	TH0 = 0xA2;		//设置定时初值
	TF0 = 0;		//清除TF0标志
	TR0 = 1;		//定时器0开始计时
	
	ET0=1;
}

void Timer0(void) interrupt 1 using 1
{
	time++;
	if(time==1000)  //时间到了一秒
	{
		time=0;
		EX0=0;
		speed=countfall/390;	//计算速度
		ang[2]=speed;
		countfall=0;
		for(i=0;i<3;i++)
		{
			S1SendData(ang[i]);
		}
		if(i==3)
		{
			i=0;
		}
		EX0=1;
	}
	
}

void PWM_init()
{
    CCON = 0x00;
    CMOD = 0x00;                                //PCA时钟为系统时钟/12 3K
    CL = 0x00;
    CH = 0x00;
    CCAPM0 = 0x42;                              //PCA模块0为PWM工作模式
    PCA_PWM0 = 0x00;                            //PCA模块0输出8位PWM
}

void PWM_contrl(unsigned char count)
{
    CCAP0L = count;                              //PWM占空比为[(100H-count)/100H]
    CCAP0H = count;

    CR = 1;                                     //启动PCA计时器
}
