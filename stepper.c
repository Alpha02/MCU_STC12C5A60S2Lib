#include "comm.h"
#include "stepper.h"
uchar code CP_Table[8]={0x01,0x03,0x02,0x06,0x04,0x0c,0x08,0x09};
uchar CP_Pointer=0;
//#define ANGLE_DIVIDE 400	//���������һ��ϸ��Ϊ������λ�����á�
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
int RadToStepper(float rad,struct Stepper * stp){  		//������ת��Ϊ�����������λ.
	int angle=rad/PI*(stp->angle_divide)/2;
	return angle;
}
void StepperStep(struct Stepper * stp){
	if((STP_MODE_SIG(stp))==0){
			CP_P=0;
			delayus(6400/(stp->angle_divide)*23);	//ת��ȡ����ϸ�֣�����Ӳ����Ӧ�ٶȺ������ٶȸ��Ĳ���ֵ
			CP_P=1;							//���������ź�
			delayus(6400/(stp->angle_divide)*23);									
			//send_int(DIR_P);
	}else{
		if(STP_MODE_DIR(stp)==0){		//����
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
//������ת��һ���Ƕ�,record_enableΪ1ʱ��ת������¼��Ϊ0ʱ������¼,������ʼ��΢��֮�á�
//ʵ��Ӧ���п���΢����ť��record_enable=0���ô˺�����У׼��������ĳ�ʼ���� 					//���ģʽΪ��������
	if(angle>0){						//���ת��>0��
	//	if(angle<10 && DIR_P==0)return;	//�����������������ͻȻ�Դ󶶶�ת��
		if((STP_MODE_SIG(stp))==0)DIR_P=1;						//�л�����
		STP_SET_DIR_0(stp);						//��¼����
		delayus(10);					//�ȴ�
	}if(angle<0){						//���ת��<0
	//	if(angle>-10 && DIR_P==1)return;//�����������������ͻȻ�Դ󶶶�ת��
		if((STP_MODE_SIG(stp))==0)DIR_P=0;						//�л�����
		angle=-angle;					//�ǶȻ�Ϊ��ֵ
		STP_SET_DIR_1(stp);						//��¼����
		delayus(10);					//�ȴ�
	}
	while(angle>0){						//��ȷת����ʼ
		StepperStep(stp);
		angle--;
		if(record_enable){				//��������¼ת�������¼ת��
			if((STP_MODE_DIR(stp))==1){(stp->angle_now)--;}else{(stp->angle_now)++;}
		}
		
	}

}

void runStepper(struct Stepper * stp){						//360�������������������С����ת��ָ���Ƕȡ�
	if((stp->angle_now)-(stp->angle_set)>(stp->angle_divide)/2){	 //������ȦӰ��
		(stp->angle_now)-=(stp->angle_divide);
	}
	if((stp->angle_now)-(stp->angle_set)<-(stp->angle_divide)/2){ //������ȦӰ��
		(stp->angle_now)+=(stp->angle_divide);
	}
	//StepperRotate((PID()),1);			//��䱻ע����������Ϊ���ת����ʵ�ʿ���ȡ����PID��ֵ��
	//angle_nowΪ��ǰ��λ��angle_setΪĿ����λ������PID���������ת�Ŀ��Ʒ�ʽ��			
}
