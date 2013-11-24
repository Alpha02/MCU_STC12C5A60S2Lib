/*********************************************
��·PCA�ⲿ�������ڼ�ʱ�����ֱ�ʹ��P1.3��P1.4�ڡ�
��Բ������̡�
PCA_dt0��PCA_dt1��ʾ������������������Ե����ڡ���С��1ms������ȷ��
diff_pulse�б�ʾ���������
�����������Ҫռ����·PCA������ռ�ö�ʱ�����ⲿ�ж�~
*********************************************/
#include "pulseFreq.h"
#include "serial.h"
int xdata pulse_diff;  	//�������
uint xdata pulse_count0;
uint xdata pulse_count1;
uint PCA_Count0;	//P1.3���ڼ�ʱ
uint PCA_Count1;	//P1.4���ڼ�ʱ
uint xdata PCA_dt0;		//P1.3���ڴ洢
uint xdata PCA_dt1;		//P1.4���ڴ洢
						   	//��ʱ�����ʱ��
const uchar code CH_reload=(65535-TIME_RELOAD)/256;		//��ʱ�����ʱ�䣬��8λ
const uchar code CL_reload=(65535-TIME_RELOAD)%256;		//��ʱ�����ʱ�䣬��8λ
void pulse_Clear(){
	pulse_diff=0;
	pulse_count0=0;
	pulse_count1=0;
	PCA_Count0=0;
	PCA_Count1=0;
	PCA_dt0=0;
	PCA_dt1=0;//��ʼ��ȫ���ս����Ա���㡣
}
void PulseFreq_Init(){
	pulse_Clear();
	CMOD=0x01;//ϵͳʱ��/12,��������жϣ�����������������ʱ�䣩
    CCAPM0=0x31;
	CCAPM1=0x31;//16λ����ģʽ����CCPn/PCAn�����䴥��
	CCON=0x40;//����״̬λ���㣬����
	//ʹ��IO��ΪP1.3��P1.4
	EA=1;
}
//1000 * 12 ��ʱ��Ƶ Ҳ����1msÿ������жϡ�

void PCA_isr() interrupt 7 using 1{	
//PCA�жϣ�ͬʱ���������¼�����ʱ��������������塣
	if(CF==1){
		CH=CH_reload;
		CL=CL_reload;
		CF=0;
		PCA_Count0++;
		PCA_Count1++;
		if(PCA_dt0<PCA_Count0)PCA_dt0=PCA_Count0;
		if(PCA_dt1<PCA_Count1)PCA_dt1=PCA_Count1;			
	}

	if(CCF0==1){
	//�����P1.3����������
		CCF0=0;
		//���־λ
		pulse_diff++;
		pulse_count0++;
		PCA_dt0=PCA_Count0;
		PCA_Count0=0;
//		send_str("dt0: ");
//		send_uint(PCA_dt0);
//		send_ln();
	}
	if(CCF1==1){
	//�����P1.4����������
		CCF1=0;
		//���־λ
		pulse_diff--;
		pulse_count1++;
		PCA_dt1=PCA_Count1;
		PCA_Count1=0;
//		send_str("dt1: ");
//		send_uint(PCA_dt1);
//		send_ln();
	}
}
uint Pulse_getPeriodTime(bit idx){
//��ȡ����ʱ�䣬�����ⲿ���á�
	if(idx==0){
		return PCA_dt0;
	}
	else {
		return PCA_dt1;
	}
}
/*
void main(){
	PulseFreq_Init();
	Serial_Init();
	while(1){
		delay(10);
	}
}
*/