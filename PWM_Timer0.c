#include "PWM_Timer0.h"
#define PWM_ENABLE  1
#define PWM_DISABLE 0
//考虑到驱动芯片可能对控制信号有着全然相反的定义，这里使用宏。
uchar PWM_Rate[2];
uchar PWM_cycle;
const uchar code TH0_Reload = (65535-TIMER0_RELOAD)/256;
const uchar code TL0_Reload = (65535-TIMER0_RELOAD)%256;
sbit PWM_Port0= P1^0;
sbit PWM_Port1= P1^1;
void PWM_Timer0_Init(){
	TR0=1;
	TMOD|=0x01;

	TH0=TH0_Reload;
	TL0=TL0_Reload;
	ET0=1;
	EA=1;
}
void setPWM(uchar rate0,uchar rate1){
	PWM_Rate[0]=rate0;
	PWM_Rate[1]=rate1;
}
void Timer0_isr()interrupt 1 {
	TH0=TH0_Reload;
	TL0=TL0_Reload;
	PWM_cycle++;
	if(PWM_cycle==0){
		PWM_Port0=PWM_ENABLE;
		PWM_Port1=PWM_ENABLE;	
	}else{
		if(PWM_cycle==PWM_Rate[0])PWM_Port0=PWM_DISABLE;
		if(PWM_cycle==PWM_Rate[1])PWM_Port1=PWM_DISABLE;		
	}	
}
