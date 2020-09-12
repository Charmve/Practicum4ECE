#ifndef __MOTOR_H__
#define __MOTOR_H__

#include "STC8.h"

void Motor_Front();
void Motor_Re();
void Motor_Stop();
void Pwm_Timer1();

extern unsigned int us;
extern unsigned int ms;
extern unsigned int Pwm_Flag;

#endif

