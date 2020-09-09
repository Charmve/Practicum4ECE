#include	"Serial.h"

bit bS1TrBusy;//串口发送忙标志
bit bS1Rec,uart_flag;//串口接收到数据标志
unsigned char cS1Rec[7]={0};//串口接收到字符
unsigned char sum2;
sbit LED=P5^5;
unsigned int sum;
unsigned char count=1;

void Uart1Init(void)		//115200bps@24MHz,T1作定时器
{
	SCON = 0x50;		//8位数据,可变波特率
	AUXR |= 0x40;		//定时器1时钟为Fosc,即1T
	AUXR &= 0xFE;		//串口1选择定时器1为波特率发生器
	TMOD &= 0x0F;		//设定定时器1为16位自动重装方式
	TL1 = 0xCC;		//设定定时初值
	TH1 = 0xFF;		//设定定时初值
	ET1 = 0;		//禁止定时器1中断
	TR1 = 1;		//启动定时器1
	
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
		RI=0;
		cS1Rec[count]=SBUF;

		if(count==1&&cS1Rec[count]==0xAA)           //帧头共五个字节
		{
			count=2;
			sum=0xAA;
		}
		
		else if(count==2&&cS1Rec[count]==0x05)
		{
			count=3;
			sum+=0x05;
		}
		
		else if(count==3&&cS1Rec[count]==0xAF)
		{
			count=4;
			sum+=0xAF;
		}
		
		else if(count==4&&cS1Rec[count]==0xF1)
		{
			count=5;
			sum+=0xF1;
		}
		
		else if(count==5&&cS1Rec[count]==0x01)
		{
			count=6;
			sum+=0x01;                                
		}
		
		else if(count==6)             //数据，一个字节
		{
			count++;
			sum+=cS1Rec[count];   //校验和
			sum2=sum&0xff;//取低8位
		}
		
		else if(count==7&&sum2==cS1Rec[7])   //若校验无误
		{
			//ES=0;            //关闭串口，使用数据结束后重新打开
			uart_flag=1;     //一帧传输结束后置标志位为1
			count=1;         //计数置一，重新开始
		}
		
		else              //若不正确，计数清零
		{
			count=1;
		}
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
