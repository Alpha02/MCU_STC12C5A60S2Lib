/*********************************************
两路PCA外部脉冲周期计时器。分别使用P1.3与P1.4口。
针对测速码盘。
PCA_dt0和PCA_dt1表示测量到的两个脉冲各自的周期。最小到1ms。不精确。
diff_pulse中表示脉冲差数。
这个函数库需要占用两路PCA。但不占用定时器和外部中断~
*********************************************/
#include "pulseFreq.h"
#include "serial.h"
int xdata pulse_diff;  	//脉冲差数
uint xdata pulse_count0;
uint xdata pulse_count1;
uint PCA_Count0;	//P1.3周期计时
uint PCA_Count1;	//P1.4周期计时
uint xdata PCA_dt0;		//P1.3周期存储
uint xdata PCA_dt1;		//P1.4周期存储
						   	//计时器溢出时间
const uchar code CH_reload=(65535-TIME_RELOAD)/256;		//计时器溢出时间，高8位
const uchar code CL_reload=(65535-TIME_RELOAD)%256;		//计时器溢出时间，低8位
void pulse_Clear(){
	pulse_diff=0;
	pulse_count0=0;
	pulse_count1=0;
	PCA_Count0=0;
	PCA_Count1=0;
	PCA_dt0=0;
	PCA_dt1=0;//初始化全体非战斗人员清零。
}
void PulseFreq_Init(){
	pulse_Clear();
	CMOD=0x01;//系统时钟/12,允许溢出中断（这样可以数更长的时间）
    CCAPM0=0x31;
	CCAPM1=0x31;//16位捕获模式，由CCPn/PCAn的跳变触发
	CCON=0x40;//各种状态位清零，开启
	//使用IO口为P1.3与P1.4
	EA=1;
}
//1000 * 12 次时钟频 也就是1ms每次溢出中断。

void PCA_isr() interrupt 7 using 1{	
//PCA中断，同时处理三种事件。定时器溢出和两种脉冲。
	if(CF==1){
		CH=CH_reload;
		CL=CL_reload;
		CF=0;
		PCA_Count0++;
		PCA_Count1++;
		if(PCA_dt0<PCA_Count0)PCA_dt0=PCA_Count0;
		if(PCA_dt1<PCA_Count1)PCA_dt1=PCA_Count1;			
	}

	if(CCF0==1){
	//如果是P1.3产生的脉冲
		CCF0=0;
		//清标志位
		pulse_diff++;
		pulse_count0++;
		PCA_dt0=PCA_Count0;
		PCA_Count0=0;
//		send_str("dt0: ");
//		send_uint(PCA_dt0);
//		send_ln();
	}
	if(CCF1==1){
	//如果是P1.4产生的脉冲
		CCF1=0;
		//清标志位
		pulse_diff--;
		pulse_count1++;
		PCA_dt1=PCA_Count1;
		PCA_Count1=0;
//		send_str("dt1: ");
//		send_uint(PCA_dt1);
//		send_ln();
	}
}
uint Pulse_getPeriodTime(bit idx){
//获取周期时间，面向外部调用。
	if(idx==0){
		return PCA_dt0;
	}
	else {
		return PCA_dt1;
	}
}
/*
void main(){
	PulseFreq_Init();
	Serial_Init();
	while(1){
		delay(10);
	}
}
*/