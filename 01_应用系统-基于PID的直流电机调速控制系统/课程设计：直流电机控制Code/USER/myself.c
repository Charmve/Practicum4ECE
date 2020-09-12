#include "myself.h"
#include "AD.h"
#include "Serial.h"//串口1(P3.0/RxD, P3.1/TxD)
#include "intrins.h"
#include "oled.h"

#define A1 P16 
#define A2 P15 
#define PWM P14 
#define En P17 

bit bFlagTimer0=0,bFlag2ms=0,bFlag20ms=0;
bit TestKeyLastTime=1;//用于保存测试按键前次状态
unsigned char Motor_count = 100;//用于电机占空比控制
unsigned char time_count = 0;
unsigned char xdata cJudgeKey=0;//用于判断按键的变量
unsigned int xdata T0Counter2ms=0,T0Counter20ms=0,T0Counter1s = 0;
unsigned char code KeyThreshold[7]={0x40,0x95,0xB6,0xC7,0xD1,0xEA,0xFF};
extern bit aFlagAD;//AD转换标志
extern unsigned char ch;      //ADC通道号
//static unsigned int xdata nroll;  //电机n转
unsigned int xdata APhra;  //A相测n转
unsigned int xdata BPhra;  //B相测n转
unsigned int xdata CSpeed;  //电机当前的速度 每0.5秒测一次

//	键名				电压值		测量AD值			键值区间
//0	KeyStartStop  0     		0x00		  0x00-0x40
//1	KeyDown  		1/2Vcc		0x81			  0x41-0x95	
//2	KeyMiddle  	2/3Vcc		0xAB-0xAC		0x96-0xB6
//3	KeyRight  	3/4Vcc		0xC0-0xC1		0xB7-0xC7
//4	KeyLeft  		4/5Vcc		0xCE-0xCD		0xC8-0xD1
//5	KeyUp    		5/6Vcc		0xD6			  0xD2-0xEA
//6	NoneKey  			Vcc 		0xFF		  	0xEB-0xFF

//Motor_count = 100; //初始化为100

void DelayNms(int n)//延时Nms//@11.0592MHz
{
	int i=0;
	for(i=0;i<n;i++) 
		Delay1ms();
}

void Delay1ms()		//@11.0592MHz
{
	unsigned char i, j;

	_nop_();
	_nop_();
	_nop_();
	i = 11;
	j = 190;
	do
	{
		while (--j);
	} while (--i);
}

void MENU_oLED(void)
{
	OLED_Clear();
	OLED_ShowCHinese(12,0,2);//直  //存放于数组 Hzk[][32]中
	OLED_ShowCHinese(30,0,3);//流
	OLED_ShowCHinese(48,0,4);//电
	OLED_ShowCHinese(66,0,5);//机
	OLED_ShowCHinese(84,0,6);//控
	OLED_ShowCHinese(102,0,7);//制

	OLED_ShowString(0,3,"ADVal:",48);
	
	OLED_ShowString(1,5,"Speed:     r/s",48);

	OLED_ShowString(0,7,"     12/29 10:42",12);
}

void Timer0Init(void)		//1毫秒@11.0592MHz
{
	AUXR |= 0x80;	//定时器时钟1T模式
	TMOD &= 0xF0;	//设置定时器模式
	TL0 = 0xCD;	//设置定时初值
	TH0 = 0xD4;	//设置定时初值
	PT0 = 1;
	TF0 = 0;  //清除TF0标志
	TR0 = 1;  //定时器0开始计时
  ET0 = 1;  //使能定时器0中断
}

void Timer0Interrupt() interrupt 1 using 2 //定时器0的中断
{
	T0Counter2ms++;
	if(T0Counter2ms>=2)
	{
		bFlag2ms=1;
		T0Counter2ms=0;
		T0Counter20ms++;
		ADC_CONTR = ADC_POWER | ADC_SPEEDHH | ADC_START | 0x00;//2ms启动一次ADC		
		if(T0Counter20ms >= 100)
		{
			aFlagAD = 0;
			ADC_CONTR = ADC_POWER | ADC_SPEEDHH | ADC_START | 0x01;//2ms启动一次ADC
		}
		else{}
		
		if(T0Counter20ms >= 250){
				bFlag20ms=!bFlag20ms;
				T0Counter20ms=0;
			  T0Counter1s++;
			  //TestLed = !TestLed;			
			if(T0Counter1s >= 2)
			{
				CSpeed = (APhra+BPhra) / 20;  //25ms计算一次
		  	APhra = 0;BPhra = 0;
			}
		}	
		else{}		
	}
	else{}
}

void Int0Init(void)		//外部中断0\1（下降沿）
{
	  IT0 = 0;    //设置INT0的中断类型 (1:仅下降沿 0:上升沿和下降沿)
	  EX0 = 1;    //使能INT0中断
    IT1 = 0;    //设置INT1的中断类型 (1:仅下降沿 0:上升沿和下降沿)
    EX1 = 1;    //使能INT1中断
	  PX0 = 0;
	  PX1 = 0;
    EA = 1;
}

//中断服务程序
void exint0() interrupt 0   //INT0中断入口
{
  APhra++; 
	//TestLed = !TestLed;     //将测试口取反
}

void exint1() interrupt 2 //INT1中断入口
{
  BPhra++; 
	//TestLed = !TestLed;     //将测试口取反
}


bit KeyPress(bit KeyIO)
{
	cJudgeKey=(cJudgeKey<<1) | KeyIO; // 判断值左移一位 ，并将当前扫描值入最低
	if(cJudgeKey==0x00)//按键按下，状态稳定。KeyIO按下为0，抬起为1
	{
		TestKeyLastTime=KeyIO;//记录状态
	}
	else if((cJudgeKey==0xFF) && (TestKeyLastTime!=KeyIO))//按键从按下到抬起，稳定
	{
		TestKeyLastTime=KeyIO;
		return 1;
	}			
	else 
	{

	}
	return 0;
}

unsigned char Judge5DKeyNum(unsigned char ADCValue)
{
	static unsigned char xdata cFlag5D=0;//保存连接读入的键值
	static unsigned char xdata c5DkeyLastTime=0;//保存上次按键值
//	static unsigned char xdata cNumLastTime=6;//记录上次确认的键值
	static unsigned char xdata cKeyPressNum;
	unsigned char c5DkeyNow=7;

	
//	unsigned char c5DkeyNow=7;
//	unsigned char i=0;
//	//判断当前读入ADC对应的按键值

//	思路与单个按键相似
	//根据AD值得到当前键值
	if(ADCValue<=KeyThreshold[0]) c5DkeyNow=0;
	else if (ADCValue>KeyThreshold[0] && ADCValue<=KeyThreshold[1]) c5DkeyNow=1;
	else if (ADCValue>KeyThreshold[1] && ADCValue<=KeyThreshold[2]) c5DkeyNow=2;
	else if (ADCValue>KeyThreshold[2] && ADCValue<=KeyThreshold[3]) c5DkeyNow=3;
	else if (ADCValue>KeyThreshold[3] && ADCValue<=KeyThreshold[4]) c5DkeyNow=4;
	else if (ADCValue>KeyThreshold[4] && ADCValue<=KeyThreshold[5]) c5DkeyNow=5;
	else  c5DkeyNow=6;

	//记录按键
	if(c5DkeyNow==6)//抬起，记录一次
		cFlag5D=(cFlag5D<<1)&0;//左移记录１次
	else if(c5DkeyNow==c5DkeyLastTime)//AD判断的键值与上次相同，
		cFlag5D=(cFlag5D<<1)|1;//左移记录１次
	else //特殊情况，本次非抬起，也与上次不同，基本不可能出现
		cFlag5D=(cFlag5D<<1)& 0;//左移记录１次
	
	c5DkeyLastTime=c5DkeyNow;//记录当前AD读的键值	
	
	//判断键值
	if(cFlag5D==0xFF)//连续8次读入一样
		cKeyPressNum=c5DkeyNow;	//记录当前键值
	
  if(cFlag5D==0x00 && cKeyPressNum !=6 )//按键有效抬起，且前一次为有效按键
	{
		c5DkeyNow=cKeyPressNum;	
		cKeyPressNum=0x06;
		return c5DkeyNow;
	}
	else
		return 0x06;		
}

unsigned char* Hex2ASCII(long int long28Value)//8位值转化为ASCII
{
	unsigned char xdata * cindexTempHex=0;
	char xdata i=0;
	
	for(i=7;i>=0;i=i-2)//高位在前
	{
		cindexTempHex[i]=long28Value;//利用默认数据类型转换，char为8位，取出lont int 的低8位
		cindexTempHex[i-1]=cindexTempHex[i]>>4;//取出8位中高4位	
		cindexTempHex[i]=cindexTempHex[i]-(cindexTempHex[i-1]<<4);//取出8位中的低4位
		long28Value=long28Value>>8;//低8位处理完毕，右移			
	}
//	S1SendData(0xAA);		
	for(i=0;i<=7;i++)
	{

//		S1SendData(cindexTempHex[i]);		
		if(cindexTempHex[i]<=9) cindexTempHex[i]+=0x30;//小于9转成ASCII
		else cindexTempHex[i]=cindexTempHex[i]+55;//大于9的数转成ASCII		
//		S1SendData(cindexTempHex[i]);			
	}
	cindexTempHex[8]=0;//数组后加一个结束符
	return cindexTempHex;
}

void MyGPIO_Inilize()
{
	GPIO_Inilize(GPIO_P0,GPIO_PullUp);//IO初始化//上拉准双向口
	GPIO_Inilize(GPIO_P1,GPIO_PullUp);//IO初始化//上拉准双向口
	GPIO_Inilize(GPIO_P2,GPIO_PullUp);//IO初始化//上拉准双向口
	GPIO_Inilize(GPIO_P3,GPIO_PullUp);//IO初始化//上拉准双向口
	GPIO_Inilize(GPIO_P4,GPIO_PullUp);//IO初始化//上拉准双向口
	GPIO_Inilize(GPIO_P5,GPIO_PullUp);//IO初始化//上拉准双向口
	GPIO_Inilize(GPIO_P5.4,GPIO_HighZ);//测试按键口浮空输入	
	DelayNms(1);//空调用，避免占用低128字节的Data空间
}

void Motor_Init(void)
{
	A1 = 0;
	A2 = 0;
	En = 0;
}

void Motor_Start(void)
{
	En = 1;
	A1 = 0;
	A2 = 1;
}

void Motor_Stop(void)
{
	A1 = 0;
	A2 = 0;	
}

void Motor_Turn(bit n)
{
	En = 1;
	if(n){ 	A1 = 0;A2 = 0;	DelayNms(200);A1 = 1; A2 = 0;}
	else{ A1 = 0;A2 = 0; DelayNms(200); A1 = 0; A2 = 1;}
}
	
//电机加速减速
void Motor_add()
{
	En = 1;
	
}	
void Motor_dec()
{
	En = 1;
	
}

//void  ye(int senddate,unsigned int n,unsigned int m)
//{
//	if(senddate==0x02&&n==0) n++;
//	else if(senddate==0x04&&n==1)n--;
//	
//   if(n==0){
//		OLED_ShowString(0,0,"C00:",16);
//	  OLED_ShowString(0,2,"C01:",16);		
//	  OLED_ShowString(0,4,"C02:",16);
//   }
//		
//	else if(n==1){		
//		if(m==0){
//			OLED_ShowString(0,0,"mm0:",16);
//			OLED_ShowString(0,2,"mm1:",16);		
//			OLED_ShowString(0,4,"mm2:",16);	}
//			if(m==0x02){
//				OLED_ShowString(0,0,"km0:",16);
//				OLED_ShowString(0,2,"km1:",16);		
//				OLED_ShowString(0,4,"km2:",16);	}
//			else if(m==0x04){
//				OLED_ShowString(0,0,"lm0:",16);
//				OLED_ShowString(0,2,"lm1:",16);		
//				OLED_ShowString(0,4,"lm2:",16);	}		
//		}
//}
	
