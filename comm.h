#ifndef _COMM_H
	#define _COMM_H
	#include <stc12c5a60s2.h>  
	typedef unsigned char uchar;
	typedef unsigned int uint;
	typedef unsigned long ulong;

	//这些宏定义都是为了能够自由调用接口而产生的
	sbit P0_0 = P0^0;
	sbit P0_1 = P0^1;
	sbit P0_2 = P0^2;
	sbit P0_3 = P0^3;
	sbit P0_4 = P0^4;
	sbit P0_5 = P0^5;
	sbit P0_6 = P0^6;
	sbit P0_7 = P0^7;

	sbit P1_0 = P1^0;
	sbit P1_1 = P1^1;
	sbit P1_2 = P1^2;
	sbit P1_3 = P1^3;
	sbit P1_4 = P1^4;
	sbit P1_5 = P1^5;
	sbit P1_6 = P1^6;
	sbit P1_7 = P1^7;

	sbit P2_0 = P2^0;
	sbit P2_1 = P2^1;
	sbit P2_2 = P2^2;
	sbit P2_3 = P2^3;
	sbit P2_4 = P2^4;
	sbit P2_5 = P2^5;
	sbit P2_6 = P2^6;
	sbit P2_7 = P2^7;

	sbit P3_0 = P3^0;
	sbit P3_1 = P3^1;
	sbit P3_2 = P3^2;
	sbit P3_3 = P3^3;
	sbit P3_4 = P3^4;
	sbit P3_5 = P3^5;
	sbit P3_6 = P3^6;
	sbit P3_7 = P3^7;
	
	#define _P(Port,Bit) P##Port##_##Bit 

	#define _SetRegBit(REG,BIT,VAL) if(VAL==0){REG|=~(0x01<<BIT);}else{REG&=(0x01<<BIT);}
	
	extern void delayus(uint time);
	extern void delay(uint time);
	#define IDI_Int0 	0
	#define IDI_Timer0 	1
	#define IDI_Int1	2
	#define IDI_Timer1  3
	#define IDI_UART1	4
	#define IDI_ADC		5
	#define IDI_LVD		6
	#define IDI_PCA		7
	#define IDI_UART2	8
	#define IDI_SPI		9
	extern void SetInterruptPriority(uchar ID_Interrupt,uchar Priority);
	//设置中断优先级，中断ID号如上面给出。一切初始化都是不计时间成本的。	
#endif