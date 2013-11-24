#ifndef SD_H
#define SD_H
#include "comm.h"
sbit SD_CS  =P1^7; //ʹ��
sbit SD_DI  =P1^1; //��������
sbit SD_DO  =P1^2; //�������
sbit SD_CLK =P1^3; //ʱ��

#define SD_ENABLE()  {SD_CS = 0;} //ʹ��SD��
#define SD_DISABLE() {SD_CS = 1;} //��ֹSD��

#define SD_OK 0 //����SD���ɹ�
#define SD_NO 1 //����SD��ʧ��

#define TIME_OUT 100 //��ʱ����

//��ʼ��SD��
extern unsigned char SD_Init(void);
//��ȡSD��������,ÿ����512�ֽ�,�ɹ�����SD_OK,ʧ�ܷ���_SD_NO
extern unsigned char SD_ReadCapacity(unsigned long *capacity);
//������,�ɹ�����SD_OK,ʧ�ܷ���_SD_NO,���Ϸ��������������
extern unsigned char SD_ReadBlock(unsigned long sector, unsigned char *buffer,unsigned int begin,unsigned int length);
//д����,�ɹ�����SD_OK,ʧ�ܷ���_SD_NO,д�Ϸ���������Ҳ��Ϊ����д��
extern unsigned char SD_WriteBlock(unsigned long sector, unsigned char *buffer);

#endif
