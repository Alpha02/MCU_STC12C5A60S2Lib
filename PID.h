#ifndef _PID_H
#define _PID_H
#include "comm.h"	  
#define PID_factorType uint
#define PID_paramType long

struct PIDstruct{ 
	//PID��3��ϵ��
	PID_factorType Kp;
	PID_factorType Ki;
	PID_factorType Kd;
	//���ó��������տ��ƽ���������ֵ��
	PID_factorType K_divide;
	//PID�Ĳ���
	PID_paramType e;
	PID_paramType ie;
	PID_paramType ie_max;
	PID_paramType de;
	//����ָ�����ڰ󶨿���������ǰ����Ŀ�����������Ͳ���ÿ�θ���ʱ���ݲ�����
	PID_paramType * ctrlVal;
	PID_paramType * u_current;
	PID_paramType * u_target;			
};
//��ʼ��PID�ṹ�岢�趨ϵ��ֵ��
extern void PID_Init(struct PIDstruct * target,PID_factorType Kp,PID_factorType Ki,PID_factorType Kd,PID_factorType K_divide);
//������ָ��󶨵���Ӧ�ı����ϡ��ṹ�壬��ǰ����Ŀ������������
extern void PID_Binding(struct PIDstruct * target,PID_paramType * ctrlVal,PID_paramType * u_current,PID_paramType * u_target);
//PID�ṹ��ĸ��¡�
extern void PID_Update(struct PIDstruct * target);
#endif