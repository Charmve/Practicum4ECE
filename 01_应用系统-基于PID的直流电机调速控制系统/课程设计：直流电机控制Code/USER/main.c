#include	"config.h"
#include	"Serial.h"//串口1(P3.0/RxD, P3.1/TxD)
#include <myself.h>
#include	"AD.h"
#include "stdio.h"
#include "math.h"
#include "oled.h"
#include "AnoTc.h"
#include "PWM.h"

#define A1 P16 
#define A2 P15 
#define PWM P25 
#define En P17 
#define EC1 P31 
#define EC2 P32

/**************************************************************
（一）题目 1：直流电机控制系统 

	1、焊接电机控制电路板，调试后电路功能正常； （30 分） 
	2、可以通过 5 向按键左右控制电机正反转、侧向按键控制电机启动/
	停止； （10 分）
	3、通过旋纽电阻控制电机转速或角度（可二选一）；（20 分） 
	4、相关电机转速或角度，转动方向信息在 LCD1602 或 OLED 上显
	示； （20 分） 
	5、通过 PID 调节，通过旋纽控制转速或转动角度（可二选一），数
	据通过串口，在电脑上显示曲线（可借助匿名地面站工具），相应的
	在显示器上显示目标转速或角度。 （15 分） 
	6、其他。 （5 分）
**************************************************************/

//unsigned int xdata aimspeed=0, last_speed = 0;
//extern unsigned int xdata CSpeed;  //电机当前的速度 每0.5秒测一次


void IncPIDCalc1(unsigned int AimSpeed,unsigned int Current)
{ 
	static double ek = 0;//当前误差
	static double ek1 = 0,ek2 = 0; //
	static double Pwm = 0; 
	float Kp = 1.312, Ki = 0.0001, Kd = 0; //上一次最好情况的参数  1.323
	//double Kp = 1.312, Ki = 0.0001, Kd = 0;
	ek = AimSpeed - Current; //增量计算
	Pwm += Kp * (ek - ek1) //E[k]项
					+Ki * ek //E[k-1]项                     
					+Kd * ((ek- ek1) - (ek1-ek2)); //E[k-2]项
	//存储误差，用于下次计算
	ek2 = ek1;
	ek1 = ek;
	//返回增量值
	
	if(Pwm > 255) Pwm = 250;
	if(Pwm < 0) Pwm = 5;
	//Test_Send_User(AimSpeed,Current,Pwm);   //AimSpeed -32
  PWM_Set(255-Pwm);
}

void main()
{
	extern bit bS1Rec;//串口接收到数据标志
	extern bit bFlag2ms,bFlag20ms;//2ms标志	
	extern bit bFlagAD;//AD转换标志	
	extern bit aFlagAD;//AD转换标志
	extern unsigned char cS1Rec;//串口接收到字符
	extern unsigned int cADCResult;//AD高8位
  extern unsigned int AngADResult;	 //用于读取旋钮的角度
	extern unsigned char ch;      //ADC通道号	
	extern bit aFlagAD;//AD转换标志
//	extern unsigned int xdata CSpeed;  //电机当前的速度 每0.5秒测一次
	
	unsigned int xdata aimspeed=0, last_speed = 0;
	extern unsigned int xdata CSpeed;  //电机当前的速度 每0.5秒测一次
	
	unsigned char xdata cKeyPressNumber=0x06;
	unsigned char* xdata cindexTemp=0;
	unsigned int xdata current_speed = 0;
	unsigned int ucount = 0,tcount = 0;//波形打印
	bit motorflag = 1;
	bit flag = 0;
	bFlag2ms=0;bFlag20ms=0;
	GDD = 0;
	
	//Beep = 0;
	MyGPIO_Inilize();//GPIO	
	Uart1Init();//串口1初始化
	Timer0Init();//定时器0初始化
	Int0Init(); //外部中断初始化
	ADCInit(); //ADC初始化
	Motor_Init(); //电机初始化
	OLED_Init();	//初始化OLED  	
	MENU_oLED();  //OLED菜单显示
	PWM_Init(0x00);	
	//IncPIDInit();

	while(1)
	{	
		//读取按键值
		if(bFlagAD==1) //2m读取一次按键AD值（五向按键和侧向按键）
		{
			bFlagAD=0;
			ucount++;
			cKeyPressNumber=Judge5DKeyNum(cADCResult);//读取按键值			
		}	
		else{}							
		
		//电子数字钟
		if(bFlag20ms == 0){
			tcount++; 
			OLED_ShowString(0,7,"     12/29 10:42",12);
//			OLED_ShowNum(52,4,CSpeed,4,16);
			if(tcount>= 60) {
				tcount = 0;
				OLED_ShowString(0,7,"     12/29 10:43",12);
			}
		}
		else {
			if (tcount == 0) OLED_ShowString(0,7,"     12/29 10 43",12);
			else OLED_ShowString(0,7,"     12/29 10 42",12);
		}
		
		//实时显示旋钮AD值
		if(aFlagAD == 0) 
		{
			ET0 = 0;ch = 1; //关定时器
			ADC_CONTR = ADC_POWER | ADC_SPEEDLL | ADC_START | ch;
			while(aFlagAD == 0) ;
					OLED_ShowNum(52,2,AngADResult,3,16);
			    //PWM_Set(255-aimspeed);
			    //Test_Send_User(aimspeed,CSpeed,00);
			//修改：根据读到的AD对应的标度作为速度的目标值
			ch = 0;ET0 = 1;
		}
		else{	}
		
		//每50ms调节一次PID
		if(ucount >= 10){  //20ms
			ucount = 0;
			current_speed = CSpeed;
			current_speed = (last_speed + CSpeed)/2;
			aimspeed = 1.3813 * AngADResult + 47.978; //线性标度目标值
			OLED_ShowNum(52,4,current_speed*3/2,4,16);
			IncPIDCalc1(aimspeed,current_speed);
			Test_Send_User(AngADResult,current_speed,00);   //AimSpeed -32
			if(!(abs(current_speed-aimspeed) <= 5))
			{
				TestLed = !TestLed; //directing the PID is runnning
			} 
			else{
				Beep = !Beep;
				//DelayNms(1000);
				Beep = !Beep;
				//DelayNms(1000);
			}
			last_speed = CSpeed;
	  }
		else{}
		
		switch(cKeyPressNumber) //根据按键执行 
		{ 
			case 0: //KeyStartStop 直流电机启动与停止
			{
				if(!flag)
				{					
					flag = 1;			
					Motor_Start();	
				}
				else{
					flag = 0;
					Motor_Stop();
				}  
				cKeyPressNumber=0x06;break;//串口发送00; 
			}
				
			case 1://KeyDown 向下键每次减5%	
			{
				  TestLed = !TestLed;
				  //Motor_dec();
					//S1SendData(0x01);
					cKeyPressNumber=0x06;break;//串口发送01; 
			}
				
			case 2:	//KeyMiddle 
			{	
					//S1SendData(0x02);          							
			    cKeyPressNumber=0x06;break;//串口发送02; 
			}
				
			case 3: //KeyRight 直流电机正转
			{
					Motor_Turn(0x01); //1正转
				  OLED_ShowCHinese(110,2,8);//正		
					//S1SendData(0x03);
					cKeyPressNumber=0x06;break;//串口发送03; 
			}
				
			case 4: //KeyLeft  直流电机反转
			{				
					Motor_Turn(0x00); //0 反转
				OLED_ShowCHinese(110,2,9);//反
					//S1SendData(0x04);
					cKeyPressNumber=0x06;break;//串口发送04; 
			}
				
			case 5: //KeyUp	向上键每次增5%	
			{
					Motor_add();
					//S1SendData(0x05);
					cKeyPressNumber=0x06;break;//串口发送05; 
			}
				
			default:// NoneKey	
				  //S1SendData(0x06);
					break;
			}	
	}
}
