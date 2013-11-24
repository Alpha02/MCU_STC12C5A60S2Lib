#ifndef _SERIAL_H
	#define _SERIAL_H
//	#define _DEBUG 0
	#include "comm.h"

//	#define RECV_EN				//如果不需要输入则注释掉这个
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
	//这是一个强大的宏，能把任意类型的变量输出= =
	#define _send_float(VAL) {send_str(#VAL);send_str(" = ");send_float(VAL,4);send_tab();}

	#define _send_value(VAL) {send_str(#VAL);send_str(" = ");_send_auto(VAL);send_tab();}
	//这是一个灰常强大的宏，能把任意类型的变量带名字一起输出为一行= =
	#define _send_value_ln(VAL) {send_str(#VAL);send_str(" = ");_send_auto(VAL);send_ln();}
	//这是一个灰常强大的宏，能把任意类型的变量带名字一起输出为一行= =
	#ifdef RECV_EN
		//如何使用输入函数？
		//首先用if(!Serial_Empty)检测是否收到数据
		//如果收到，就可以调用各种get函数得到数据
		//得到数据以后，检测Serial_DataErr，如果该位为1，
		//表示数据读取出错，则结果可能不正确。应该舍弃这个结果并将该位清零。
		#define SERIAL_BUFF_LENGTH 20	
		//这个宏定义了接收缓冲区的长度，如果串口单次接收的数据量较大，
		//应增大这个参数。这个buffer是以循环队列的形式存储的。	
		extern bit Serial_Empty;
		extern bit Serial_Full;
		extern bit Serial_DataErr;
		extern uchar Serial_get_uchar();
		extern uint Serial_get_uint();
		extern void Serial_getline(uchar * str);
		/*这个例子演示了如何获取一行字符串并输出
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