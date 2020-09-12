#include	"Serial.h"

bit bS1TrBusy;//串口发送忙标志
bit bS1Rec;//串口接收到数据标志
unsigned char cS1Rec;//串口接收到字符

void Uart1Init(void)		//115200bps@11.0592MHz,T2作定时器
{
	SCON = 0x50;		//8位数据,可变波特率
	AUXR |= 0x01;		//串口1选择定时器2为波特率发生器
	AUXR |= 0x04;		//定时器2时钟为Fosc,即1T
//	T2L = 0xE8;		//设定定时初值
//	T2H = 0xFF;		//设定定时初值
  T2L = (65536 - (MAIN_Fosc/4/BAUD));   //设置波特率重装值
  T2H = (65536 - (MAIN_Fosc/4/BAUD))>>8;	
	AUXR |= 0x10;		//启动定时器2
		
////打开串口中断	
	ES=1;           //使能串口1中断
  EA = 1;	
}


/*----------------------------
UART 中断服务程序
-----------------------------*/	
void Uart1Interrupt() interrupt 4 using 1
{
	if(RI)
	{
		RI=0;				//清除RI位
		cS1Rec = SBUF; //将数据读出
		bS1Rec=1;//置接收到数据标志位		
	}
	if(TI)
	{
		TI=0;					//清除TI位
		bS1TrBusy=0; //清忙标志
	}
}

/*----------------------------
发送串口数据
----------------------------*/
void S1SendData(unsigned char dat)
{
    while (bS1TrBusy);//等待前面的数据发送完成
    bS1TrBusy = 1;
    SBUF = dat;                 //写数据到UART数据寄存器
}


/*----------------------------
发送字符串
----------------------------*/
void S1SendString(unsigned char *s)
{
    while (*s)                  //检测字符串结束标志
    {
        S1SendData(*s++);         //发送当前字符
    }
}
