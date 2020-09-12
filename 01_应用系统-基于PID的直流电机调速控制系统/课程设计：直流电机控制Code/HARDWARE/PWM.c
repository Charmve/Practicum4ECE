#include "PWM.h"

#define CCP_S0 0x10                 //P_SW1.4
#define CCP_S1 0x20                 //P_SW1.5

void PWM_Init(unsigned char pwm_init)
{
//	ACC &= ~(CCP_S0 | CCP_S1);      //CCP_S0=0 CCP_S1=0
//	P_SW1 = ACC;                    //(P1.2/ECI, P1.1/CCP0, P1.0/CCP1, P3.7/CCP2)
//  ACC = P_SW1;
//  ACC &= ~(CCP_S0 | CCP_S1);      //CCP_S0=1 CCP_S1=0
//  ACC |= CCP_S0;                  //(P3.4/ECI_2, P3.5/CCP0_2, P3.6/CCP1_2, P3.7/CCP2_2)
//  P_SW1 = ACC;  
  ACC = P_SW1;
  ACC &= ~(CCP_S0 | CCP_S1);      //CCP_S0=0 CCP_S1=1
  ACC |= CCP_S1;                  //(P2.4/ECI_3, P2.5/CCP0_3, P2.6/CCP1_3, P2.7/CCP2_3)
  P_SW1 = ACC;  
	CCON = 0;                       //初始化PCA控制寄存器
																	//PCA定时器停止
                                  //清除CF标志
                                  //清除模块中断标志
	CL = 0;                         //复位PCA寄存器
  CH = 0;
  CMOD = 0x02;                    //设置PCA时钟源
                                  //禁止PCA定时器溢出中断
  PCA_PWM0 = 0x00;                //PCA模块0工作于8位PWM
  CCAP0H = CCAP0L = pwm_init;     //PWM0的占空比为87.5% ((100H-20H)/100H)
  CCAPM0 = 0x42;                  //PCA模块0为8位PWM模式
//  PCA_PWM1 = 0x40;                //PCA模块1工作于7位PWM
//  CCAP1H = CCAP1L = 0x20;         //PWM1的占空比为75% ((80H-20H)/80H)
//  CCAPM1 = 0x42;                  //PCA模块1为7位PWM模式

//  PCA_PWM2 = 0x80;                //PCA模块2工作于6位PWM
//  CCAP2H = CCAP2L = 0x20;         //PWM2的占空比为50% ((40H-20H)/40H)
//  CCAPM2 = 0x42;                  //PCA模块2为6位PWM模式
  CR = 1;                         //PCA定时器开始工作
}

void PWM_Set(unsigned char pwm_set)
{
  CCAP0H = CCAP0L = pwm_set;     //PWM0的占空比为87.5% ((100H-20H)/100H)
}


