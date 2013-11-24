#ifndef  _PULSEFREQ_H
	#define    _PULSEFREQ_H
	
	#include "comm.h"
	#define TIME_RELOAD 100
	extern int xdata pulse_diff; 
	extern uint xdata pulse_count0;
	extern uint xdata pulse_count1;	
	extern uint xdata  PCA_dt0;		//P1.3周期存储
	extern uint xdata  PCA_dt1;		//P1.4周期存储
	extern void pulse_Clear();
	extern void PulseFreq_Init();
	extern uint Pulse_getPeriodTime(bit idx);
#endif