#include "myself.h"
#include	<STC15F2K60S2.h>
#include	"Serial.h"//串口1(P3.0/RxD, P3.1/TxD)
#include	"intrins.h"

sbit s2=P1^1;        //TCS3200 S2 
sbit s3=P1^0;        //TCS3200 S3
                     //TCS3200 S0 模块内部默认上拉
                     //TCS3200 S1 模块内部默认上拉
                     //TCS3200 OE 模块内部接地

sbit test_pin=P1^2;  //用示波器看这个引脚，可知道定时器中断频率
//变量、常量定义
uchar ge,shi,bai;
uchar rp=3,gp=3,bp=4; //定义比例因子，具体环境可以修改
uchar count;          //颜色标志位（0:红 1:绿 2:蓝）

////显示数组
//uchar disp_R[3];  //红
//uchar disp_G[3];  //绿
//uchar disp_B[3];  //蓝	            

u16 R = 0;
u16 G = 0;
u16 Bl = 0;


//*********************************************
//数值转换出个十百千的ASCII码
//*********************************************
//void conversion(uint temp_data)  
//{  
//    bai=temp_data/100+0x30 ;
//    temp_data=temp_data%100;   //取余运算
//    shi=temp_data/10+0x30 ;
//    ge=temp_data%10+0x30;      //取余运算
//}

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

void Timer0Init(void)		//10毫秒@11.0592MHz
{
	AUXR &= 0x7F;		//定时器时钟12T模式
	TMOD &= 0xF0;		//设置定时器模式    //同时打开定时器1做计数器，计脉冲   P3.5
	TMOD|=0x40;
	TL0 = 0x00;		//设置定时初值
	TH0 = 0xDC;		//设置定时初值
	TL1 = 0x00;		
	TH1 = 0x00;	
	TF0 = 0;		//清除TF0标志
	
	
	EA=1;            //开总中断
 	ET0=1;           //开定时器0中断
	
	TR0=1;           
	TR1=1;           //启动
}

/*******************************************
* 函数名称: c10ms_out() 
* 函数功能: 定时中断0服务程序
            修改颜色标志disp_tc（0:红 1:绿 2:蓝）
            设置S0 S1 S2 选择滤波器
            计算脉冲，读取色值
* 入口参数: 无
* 出口参数: 无
/********************************************/
void Timer0Interrupt() interrupt 1 using 2 //定时器0的中断
{
	u16 temp;
	test_pin=!test_pin; //测试定时器中断频率引脚，可以用示波器观察
	TR0=0;              //关闭定时
	TR1=0;              //关闭计数
//	S1SendData(count);
	//   count+1实现先检测绿色,再检测蓝色,然后检测红色,循环检测       
	if(count==0)
	{
		count++;    
		s2=1;s3=1;             //选择滤波器为绿色     
		
		//	temp=(8<<TH1)+TL1;
		
		temp=TH1;
		temp=(temp<<8)+TL1;
		
	//	temp=(TH1<<8)+TL1;    //计算这段时间内 TCS230 的输出脉冲数	
		temp/=rp;			
		R = temp;
		
//		conversion(temp);
//		disp_R[2]=ge;         //因为这次的中断，是上次选择滤波器的数值
//		disp_R[1]=shi;
//		disp_R[0]=bai;
	}	
	else if(count==1)
	{            
		count++;
		s2=0;s3=1;            //选择滤波器为蓝色
	//	temp=(TH1<<8)+TL1;    //计算这段时间内 TCS230 的输出脉冲数	
		//	temp=(8<<TH1)+TL1;
		
		temp=TH1;
		temp=(temp<<8)+TL1;
		
	temp/=gp;	
		G = temp;
//				if(G>0xFF)
//			G=0xFF;		temp/=bp
//		conversion(temp);
//		disp_G[2]=ge;         //因为这次的中断，是上次选择滤波器的数值
//		disp_G[1]=shi;
//		disp_G[0]=bai;
	}	
	else if(count==2)
	{            
		count=0;
		s2=0;s3=0;            //选择滤波器为红色
		
		//	temp=(8<<TH1)+TL1;
		
		//temp=(TH1<<8)+TL1;    //计算这段时间内 TCS230 的输出脉冲数	
		
		temp=TH1;
		temp=(temp<<8)+TL1;
		temp/=bp;	
		Bl = temp;
//				if(Bl>0xFF)
//			Bl=0xFF;
//		conversion(temp);
//		disp_B[2]=ge;         //因为这次的中断，是上次选择滤波器的数值
//		disp_B[1]=shi;
//		disp_B[0]=bai;	
		
	}
	//定时器计数器重赋初值
//	TH0=0xE0;
//	TL0=0x00; //11。0592M 晶振，为10ms
//	TL0 = 0x00;		//设置定时初值
//	TH0 = 0xDC;		//设置定时初值
	
	TL1=0x00;//计数器清零
	TH1=0x00;//计数器清零
	TR0=1;   //打开定时器
	TR1=1;   //打开计数器
}




