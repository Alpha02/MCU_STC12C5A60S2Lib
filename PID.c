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
//������ָ��󶨵���Ӧ�ı����ϡ��ṹ�壬��ǰ����Ŀ������������
	target->ctrlVal=ctrlVal;
	target->u_current=u_current;
	target->u_target=u_target;
}
void PID_Update(struct PIDstruct * target){
	PID_paramType new_e=(*(target->u_current))-(*(target->u_target));	//ƫ����ڵ�ǰ����ȥĿ����
	target->de=new_e-target->e;				//����΢����
	if((target->Ki)!=0){					//���Ki��ֵ��Ϊ0�������ֿ��ƴ��ڡ��Ž��л��ּ��㡣
		PID_paramType temp_ie=target->ie+new_e;	//�������ۼ�
		if(temp_ie<-(target->ie_max)){			//����������������ֵ����Ϊ���ֵ
			target->ie=-(target->ie_max);
		}else if(temp_ie>(target->ie_max)){
			target->ie=target->ie_max;
		}else {target->ie=temp_ie;}
								//���±�����
	}
	target->e=new_e;
	//���¿�����
	*(target->ctrlVal)=-(target->e*target->Kp+target->ie*target->Ki+target->de*target->Kd)/(target->K_divide);
}

