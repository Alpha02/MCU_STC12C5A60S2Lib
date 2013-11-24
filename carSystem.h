#ifndef _CARSYSTEM_H
#define _CARSYSTEM_H
#include "comm.h"
#include "PWM_Timer0.h"
#define FORWARD 1
#define BACK	0
#define ENABLE	0
#define DISABLE	1
//考虑到驱动芯片可能对控制信号有着全然相反的定义，这里使用宏。

sbit L_forward=P3^4;
sbit R_forward=P3^5;
sbit L_back=P3^6;
sbit R_back=P3^7;
extern void car_setSpeed(int L_spd,int R_spd);
extern void car_Init();
#endif