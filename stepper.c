#include "comm.h"
#include "stepper.h"
uchar code CP_Table[8]={0x01,0x03,0x02,0x06,0x04,0x0c,0x08,0x09};
uchar CP_Pointer=0;
//#define ANGLE_DIVIDE 400	//步进电机将一周细分为多少相位的设置。
//#define SIGNAL_DIVIDE 0
//#endif 
#define PI 3.14159

#define STP_MODE_DIR(stp) (((stp)->mode)&0x01)
#define STP_MODE_BAT(stp) (((stp)->mode)&0x02)
#define STP_SET_BAT_0(stp) ((stp)->mode)&=0xfd
#define STP_SET_BAT_1(stp) ((stp)->mode)|=0x02
#define STP_SET_DIR_0(stp) ((stp)->mode)&=0xfe
#define STP_SET_DIR_1(stp) ((stp)->mode)|=0x01	
#define STP_MODE_SIG(stp) (((stp)->mode)&0x04)
#define STP_SET_SIG_0(stp) ((stp)->mode)&=0xfb
#define STP_SET_SIG_1(stp) ((stp)->mode)|=0x04
#define STP_MODE_PORT(stp) (((stp)->mode)/8)
#define STP_SET_PORT(stp,port) ((stp)->mode)=(((stp)->mode)&0x07)|(port<<3)

void Stepper_Init(struct Stepper * stp,bit sig_mode,bit bat_mode,uint angle_divide){
	stp->angle_set=0;
	stp->angle_now=0;	
	stp->mode=0;
	stp->angle_divide=angle_divide;
	if(bat_mode==0){
		STP_SET_BAT_0(stp);	
	}else{
		STP_SET_BAT_1(stp);
	}
	if(sig_mode==0){
		STP_SET_SIG_0(stp);	
	}else{
		STP_SET_SIG_1(stp);
	}
}
void Stepper_Attach(struct Stepper * stp,uchar signal_port){
	STP_SET_PORT(stp,signal_port);	
}
int RadToStepper(float rad,struct Stepper * stp){  		//将弧度转化为步进电机的相位.
	int angle=rad/PI*(stp->angle_divide)/2;
	return angle;
}
void StepperStep(struct Stepper * stp){
	if((STP_MODE_SIG(stp))==0){
			CP_P=0;
			delayus(6400/(stp->angle_divide)*23);	//转速取决于细分，根据硬件响应速度和需求速度更改参数值
			CP_P=1;							//触发脉冲信号
			delayus(6400/(stp->angle_divide)*23);									
			//send_int(DIR_P);
	}else{
		if(STP_MODE_DIR(stp)==0){		//正向
			(stp->signal_pointer)++;
			if((stp->signal_pointer)>=8){stp->signal_pointer=0;}	
		}else{
			(stp->signal_pointer)--;	
			if((stp->signal_pointer)==0){stp->signal_pointer=7;}	
		}
		switch(STP_MODE_PORT(stp)){
			case 0:P0=( P0 | 0x0f ) & (~CP_Table[stp->signal_pointer]);break;
			case 1:P0=( P0 | 0xf0 ) & (~(CP_Table[stp->signal_pointer]<<4));break;
			case 2:P1=( P1 | 0x0f ) & (~CP_Table[stp->signal_pointer]);break;
			case 3:P1=( P1 | 0xf0 ) & (~(CP_Table[stp->signal_pointer]<<4));break;
			case 4:P2=( P2 | 0x0f ) & (~CP_Table[stp->signal_pointer]);break;
			case 5:P2=( P2 | 0xf0 ) & (~(CP_Table[stp->signal_pointer]<<4));break;
			case 6:P3=( P3 | 0x0f ) & (~CP_Table[stp->signal_pointer]);break;
			case 7:P3=( P3 | 0xf0 ) & (~(CP_Table[stp->signal_pointer]<<4));break;

		}
	}
}
void StepperRotate(struct Stepper * stp,int angle,bit record_enable){
//令步进电机转过一定角度,record_enable为1时该转动被记录，为0时不被记录,以作初始化微调之用。
//实际应用中可用微调按钮设record_enable=0调用此函数来校准步进电机的初始方向。 					//如果模式为脉冲驱动
	if(angle>0){						//如果转角>0，
	//	if(angle<10 && DIR_P==0)return;	//用来消抖，电机不会突然以大抖动转向。
		if((STP_MODE_SIG(stp))==0)DIR_P=1;						//切换方向
		STP_SET_DIR_0(stp);						//记录方向
		delayus(10);					//等待
	}if(angle<0){						//如果转角<0
	//	if(angle>-10 && DIR_P==1)return;//用来消抖，电机不会突然以大抖动转向。
		if((STP_MODE_SIG(stp))==0)DIR_P=0;						//切换方向
		angle=-angle;					//角度化为正值
		STP_SET_DIR_1(stp);						//记录方向
		delayus(10);					//等待
	}
	while(angle>0){						//精确转动开始
		StepperStep(stp);
		angle--;
		if(record_enable){				//如果允许记录转动，则记录转动
			if((STP_MODE_DIR(stp))==1){(stp->angle_now)--;}else{(stp->angle_now)++;}
		}
		
	}

}

void runStepper(struct Stepper * stp){						//360度内驱动步进电机以最小弧度转到指定角度。
	if((stp->angle_now)-(stp->angle_set)>(stp->angle_divide)/2){	 //消除套圈影响
		(stp->angle_now)-=(stp->angle_divide);
	}
	if((stp->angle_now)-(stp->angle_set)<-(stp->angle_divide)/2){ //消除套圈影响
		(stp->angle_now)+=(stp->angle_divide);
	}
	//StepperRotate((PID()),1);			//这句被注释起来，因为电机转动的实际控制取决于PID的值。
	//angle_now为当前相位，angle_set为目标相位，根据PID决定电机旋转的控制方式。			
}
