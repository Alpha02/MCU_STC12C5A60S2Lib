#include "comm.h"
sbit Buzzer = P2^3;
#define BUZZER_ACTIVE //��Դ��������ע����Ϊ��Դ�����������ʹ�ò����ض������Ĺ��ܣ��������Դ��������
//������
extern void beep(uint time_ms);
extern void beeps(uchar time);
extern void playSong();