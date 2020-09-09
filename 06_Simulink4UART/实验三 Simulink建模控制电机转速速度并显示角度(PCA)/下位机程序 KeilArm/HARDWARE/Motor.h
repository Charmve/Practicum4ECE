#ifndef		__MOTOR_H
#define		__MOTOR_H

#include "config.h"

void Motor_Init();//初始化
void Motor_TN();//顺时钟转动
void Motor_TP();//逆时钟转动
void Exinit(); //外部中断0初始化
//void panduany(void);

extern unsigned int countfall;
//extern unsigned char speed;
//extern unsigned int time1;


#endif