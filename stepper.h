#ifndef _STEPPER_H
#define	_STEPPER_H
#include "comm.h"
//#define Stepper_PulseSignalMode
//�������������������źſ��ƣ�ֻ�ܷ��䵽����һ��IO�ڡ�
//��������������źŶ˲��ù����ӷ������ź�������������Ƭ��IO�ڡ�
#define Stepper_Port P1		//��������źŶ˿ڡ�
sbit EN_P=Stepper_Port^1;		//ʹ���ź�
sbit DIR_P=Stepper_Port^2;	//�����ź�
sbit CP_P=Stepper_Port^5;		//�����ź�
sbit key1=Stepper_Port^4;		//΢����ť��
//#define ANGLE_DIVIDE 6400	//���������һ��ϸ��Ϊ������λ�����á�
struct Stepper{
	int angle_set;
	int angle_now;
	uint angle_divide;	  //�����
	uchar mode;
	//				0				1
	//0��	dir		����
	//1��	bat		8������		4������
	//2��	sig		��������	4���ź�����
	//3~5��port		�ź�IO��	0ΪP0��4λ��1ΪP0��4λ��2ΪP1��4λ���Դ�����		
	uchar signal_pointer;	
};								
//42��������Ĳ����Ϊ1.8�ȣ���Ӧ200ϸ�֡�
extern void Stepper_Init(struct Stepper * stp,bit sig_mode,bit bat_mode,uint angle_divide);
//��ʼ��������������õ�ǰ�Ƕ�Ϊ0��sig_modeָ���ź�ģʽ��bat_modeָ��4�Ļ���8�ģ�angle_divide˵����������ĽǶ�ϸ����
extern void Stepper_Attach(struct Stepper * stp,uchar signal_port);
//����������ź�ƥ�䵽ָ���Ķ˿ڡ�signal_port��0ΪP0��4λ��1ΪP0��4λ��2ΪP1��4λ���Դ�����
extern void StepperRotate(struct Stepper * stp,int angle,bit record_enable);
//������ת��һ���Ƕ�,record_enableΪ1ʱ��ת������¼��Ϊ0ʱ������¼,������ʼ��΢��֮�á�
//ʵ��Ӧ���п���΢����ť��record_enable=0���ô˺�����У׼��������ĳ�ʼ���� 					//���ģʽΪ��������
#endif