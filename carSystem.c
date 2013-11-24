#include "carSystem.h"
bit L_dir;
bit R_dir;
void car_Init(){
	L_forward=DISABLE;
	R_forward=DISABLE;
	L_back=DISABLE;
	R_back=DISABLE;
	L_dir=FORWARD;
	R_dir=FORWARD;
	car_setSpeed(0,0);

	PWM_Timer0_Init();


}
void car_setSpeed(int L_spd,int R_spd){
	if(L_spd<0){
	//一个判断语句2个指令周期，一个赋值语句1个指令周期。
	//2~5 3~3 考虑到多数情况是不改变前后的，还是加了if好
		if(L_dir!=BACK){
			L_dir	  =	BACK;
			L_forward = ENABLE;
			L_back	  = DISABLE;
		}
		L_spd=-L_spd;		
	}else{
		if(L_dir!=FORWARD){
			L_dir	  =	FORWARD;
			L_forward = DISABLE;
			L_back	  = ENABLE;
		}
		L_dir=FORWARD;
	}	

	if(R_spd<0){
		if(R_dir!=BACK){
			R_dir	  =	BACK;
			R_forward = ENABLE;
			R_back	  = DISABLE;
		}
		R_spd=-R_spd;		
	}else{
		if(R_dir!=FORWARD){
			R_dir	  =	FORWARD;
			R_forward = DISABLE;
			R_back	  = ENABLE;
		}
		R_dir=FORWARD;
	}
	setPWM(L_spd,R_spd);
} 

