#ifndef __PWM_H
#define __PWM_H

#include "stc15f2k60s2.h"

void PWM_Init(unsigned char pwm_init);			//pwm_init为16进制数
void PWM_Set(unsigned char pwm_set);				//0x00-0x100 :  0-100% 占空比


#endif 

