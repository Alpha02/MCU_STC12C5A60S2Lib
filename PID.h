#ifndef _PID_H
#define _PID_H
#include "comm.h"	  
#define PID_factorType uint
#define PID_paramType long

struct PIDstruct{ 
	//PID的3个系数
	PID_factorType Kp;
	PID_factorType Ki;
	PID_factorType Kd;
	//设置除数，最终控制结果除以这个值。
	PID_factorType K_divide;
	//PID的参数
	PID_paramType e;
	PID_paramType ie;
	PID_paramType ie_max;
	PID_paramType de;
	//以下指针用于绑定控制量、当前量和目标量。这样就不必每次更新时传递参数。
	PID_paramType * ctrlVal;
	PID_paramType * u_current;
	PID_paramType * u_target;			
};
//初始化PID结构体并设定系数值。
extern void PID_Init(struct PIDstruct * target,PID_factorType Kp,PID_factorType Ki,PID_factorType Kd,PID_factorType K_divide);
//将三个指针绑定到相应的变量上。结构体，当前量，目标量，控制量
extern void PID_Binding(struct PIDstruct * target,PID_paramType * ctrlVal,PID_paramType * u_current,PID_paramType * u_target);
//PID结构体的更新。
extern void PID_Update(struct PIDstruct * target);
#endif