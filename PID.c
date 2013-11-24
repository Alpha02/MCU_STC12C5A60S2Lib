#include "PID.h"
#include "serial.h"
void PID_Init(struct PIDstruct * target,PID_factorType Kp,PID_factorType Ki,PID_factorType Kd,PID_factorType K_divide){
	target->Kp=Kp;
	target->Ki=Ki;
	target->Kd=Kd;
	target->K_divide=K_divide;
	target->e=0;		
	target->ie=0;
	target->ie_max=10000;
	target->de=0;			
}
void PID_Binding(struct PIDstruct * target,PID_paramType * u_current,PID_paramType * u_target,PID_paramType * ctrlVal){
//将三个指针绑定到相应的变量上。结构体，当前量，目标量，控制量
	target->ctrlVal=ctrlVal;
	target->u_current=u_current;
	target->u_target=u_target;
}
void PID_Update(struct PIDstruct * target){
	PID_paramType new_e=(*(target->u_current))-(*(target->u_target));	//偏差等于当前量减去目标量
	target->de=new_e-target->e;				//计算微分量
	if((target->Ki)!=0){					//如果Ki的值不为0，即积分控制存在。才进行积分计算。
		PID_paramType temp_ie=target->ie+new_e;	//积分量累加
		if(temp_ie<-(target->ie_max)){			//如果积分量超出最大值则设为最大值
			target->ie=-(target->ie_max);
		}else if(temp_ie>(target->ie_max)){
			target->ie=target->ie_max;
		}else {target->ie=temp_ie;}
								//更新比例量
	}
	target->e=new_e;
	//更新控制量
	*(target->ctrlVal)=-(target->e*target->Kp+target->ie*target->Ki+target->de*target->Kd)/(target->K_divide);
}

