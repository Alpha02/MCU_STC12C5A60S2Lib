#ifndef _I2C_H
	#define _I2C_H
	#include "comm.h"
	extern void I2C_delay();
	extern void I2C_Start();
	extern void I2C_Stop();
	extern void I2C_SendAck(bit Ack);
	extern bit I2C_RecAck();
	extern void I2C_Send_Byte(uchar Dat);
	extern uchar I2C_Rec_Byte(void);
#endif