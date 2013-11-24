#include "comm.h"
void delayus(uint time){
	while(time--);
}
void delay(uint time){
	while(time--){
		delayus(1000);
	}
}
#define _SetPriority(IDRL,REG,BIT,VAL) {IDRL=VAL%2;_SetRegBit(REG,BIT,VAL/2);}
//这个宏专门用于设置优先级寄存器
//stc12c5a60s2有四个优先级
void SetInterruptPriority(uchar ID_Interrupt,uchar Priority){
	switch(ID_Interrupt){
	case IDI_Int0	:_SetPriority(PX0,IPH,0,Priority);break;
	case IDI_Timer0 :_SetPriority(PT0,IPH,1,Priority);break;
	case IDI_Int1	:_SetPriority(PX1,IPH,2,Priority);break;
	case IDI_Timer1 :_SetPriority(PT1,IPH,3,Priority);break;
	case IDI_UART1	:_SetPriority(PS,IPH,4,Priority);break;
	case IDI_ADC	:_SetPriority(PADC,IPH,5,Priority);break;
	case IDI_LVD	:_SetPriority(PLVD,IPH,6,Priority);break;
	case IDI_PCA	:_SetPriority(PPCA,IPH,7,Priority);break;
//后面这两个不可按位寻址比较捉急，况且不常用，先不写。
//	case IDI_UART2	:_SetPriority(PS2,IP2H,0,Priority);break;
//	case IDI_SPI	:_SetPriority(PSPI,IP2H,1,Priority);break;
	default:break;		
	}	
}