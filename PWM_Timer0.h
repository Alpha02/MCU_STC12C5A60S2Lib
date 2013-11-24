#ifndef _PWM_TIMER0_H
	#define	_PWM_TIMER0_H
	#include "comm.h"
	#define TIMER0_RELOAD 100
	extern uchar PWM_Rate[2];
	extern uchar PWM_cycle;
	extern void PWM_Timer0_Init();
	extern void setPWM(uchar rate0,uchar rate1);
#endif