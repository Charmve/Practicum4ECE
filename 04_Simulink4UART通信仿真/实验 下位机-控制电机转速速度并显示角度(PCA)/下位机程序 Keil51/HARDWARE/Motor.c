#include "Motor.h"
#include "AD.h"

sbit enable=P2^2;  //使能
sbit A1=P2^0;   //输入控制1
sbit A2=P2^1;  //输入控制2

unsigned int countfall;
//unsigned char speed;
//unsigned int time1;
void Motor_Init()
{

	A1=0;
	A2=0;

}

void Motor_TN()
{
	enable=1;
	A1=1;
	A2=0;
}

void Motor_TP()
{
	enable=1;
	A1=0;
	A2=1;
}

void Exinit()
{
    IT0 = 1;                        //set INT0 int type (1:Falling 0:Low level)
    EX0 = 1;                        //enable INT0 interrupt
   // EA = 1;                         //open global interrupt switch

}

void Exint0() interrupt 0           //(location at 0003H)
{
	countfall++;	  //记下降沿个数
}

//void panduany(void)
//{
//		if(countfall==390)       //计到一圈，关中断，显示速度
//		{
//			EX0=0;
//			countfall=0;
//			TR0=0;
//			speed=1000/time1;
//			ang[2]=speed;
//		//	LCD_Write_String(0,1,tempy);//显示到液晶第二行		
//			time1=0;
//			TR0=1;
//			EX0=1;
//		}	
//}



