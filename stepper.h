#ifndef _STEPPER_H
#define	_STEPPER_H
#include "comm.h"
//#define Stepper_PulseSignalMode
//如果步进电机采用脉冲信号控制，只能分配到以下一组IO口。
//步进电机驱动器信号端采用供阴接法，将信号正极连接至单片机IO口。
#define Stepper_Port P1		//步进电机信号端口。
sbit EN_P=Stepper_Port^1;		//使能信号
sbit DIR_P=Stepper_Port^2;	//方向信号
sbit CP_P=Stepper_Port^5;		//脉冲信号
sbit key1=Stepper_Port^4;		//微调按钮。
//#define ANGLE_DIVIDE 6400	//步进电机将一周细分为多少相位的设置。
struct Stepper{
	int angle_set;
	int angle_now;
	uint angle_divide;	  //步距角
	uchar mode;
	//				0				1
	//0：	dir		方向
	//1：	bat		8拍驱动		4拍驱动
	//2：	sig		脉冲驱动	4线信号驱动
	//3~5：port		信号IO口	0为P0低4位，1为P0高4位，2为P1低4位，以此类推		
	uchar signal_pointer;	
};								
//42步进电机的步距角为1.8度，对应200细分。
extern void Stepper_Init(struct Stepper * stp,bit sig_mode,bit bat_mode,uint angle_divide);
//初始化步进电机，设置当前角度为0，sig_mode指定信号模式，bat_mode指定4拍还是8拍，angle_divide说明步进电机的角度细分量
extern void Stepper_Attach(struct Stepper * stp,uchar signal_port);
//将步进电机信号匹配到指定的端口。signal_port：0为P0低4位，1为P0高4位，2为P1低4位，以此类推
extern void StepperRotate(struct Stepper * stp,int angle,bit record_enable);
//令步进电机转过一定角度,record_enable为1时该转动被记录，为0时不被记录,以作初始化微调之用。
//实际应用中可用微调按钮设record_enable=0调用此函数来校准步进电机的初始方向。 					//如果模式为脉冲驱动
#endif