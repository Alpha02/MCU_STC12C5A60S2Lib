#ifndef _SERIAL_H
	#define _SERIAL_H
//	#define _DEBUG 0
	#include "comm.h"

//	#define RECV_EN				//�������Ҫ������ע�͵����
	extern void Serial_Init();
	extern void send(uchar s);
	extern void send_str(unsigned char * pd);
	extern void send_week(uchar week);
	extern void send_ln();
	extern void send_tab();
	extern void send_int(int num);
	extern void send_uint(uint num);
	extern void send_long(long num);
	extern void send_ulong(ulong num);
	extern void send_float(float num,uchar precise);
	#define _send_auto(VAL) if(sizeof(VAL)>4){send_str((uchar *)VAL);}else{if(VAL < 0){send_long((long)VAL);}else{send_ulong((ulong)VAL);}}
	//����һ��ǿ��ĺ꣬�ܰ��������͵ı������= =
	#define _send_float(VAL) {send_str(#VAL);send_str(" = ");send_float(VAL,4);send_tab();}

	#define _send_value(VAL) {send_str(#VAL);send_str(" = ");_send_auto(VAL);send_tab();}
	//����һ���ҳ�ǿ��ĺ꣬�ܰ��������͵ı���������һ�����Ϊһ��= =
	#define _send_value_ln(VAL) {send_str(#VAL);send_str(" = ");_send_auto(VAL);send_ln();}
	//����һ���ҳ�ǿ��ĺ꣬�ܰ��������͵ı���������һ�����Ϊһ��= =
	#ifdef RECV_EN
		//���ʹ�����뺯����
		//������if(!Serial_Empty)����Ƿ��յ�����
		//����յ����Ϳ��Ե��ø���get�����õ�����
		//�õ������Ժ󣬼��Serial_DataErr�������λΪ1��
		//��ʾ���ݶ�ȡ�����������ܲ���ȷ��Ӧ������������������λ���㡣
		#define SERIAL_BUFF_LENGTH 20	
		//����궨���˽��ջ������ĳ��ȣ�������ڵ��ν��յ��������ϴ�
		//Ӧ����������������buffer����ѭ�����е���ʽ�洢�ġ�	
		extern bit Serial_Empty;
		extern bit Serial_Full;
		extern bit Serial_DataErr;
		extern uchar Serial_get_uchar();
		extern uint Serial_get_uint();
		extern void Serial_getline(uchar * str);
		/*���������ʾ����λ�ȡһ���ַ��������
		if(!Serial_Empty){
			Serial_getline(temp);
			if(Serial_DataErr){
			   Serial_DataErr=0;
			}else{
			send_str(temp);
			send_ln();
		}
		*/
	#endif
#endif