#include "comm.h"
void I2C_delay();
#define _SLAVE
#ifdef _MASTER
	extern void I2C_Start();
	extern void I2C_Stop();
	extern void I2C_SendAck(bit Ack);
	extern bit I2C_RecAck();
	extern bit I2C_SendByte(uchar Dat);
	extern uchar I2C_RecByte(void);
	extern bit I2C_WriteReg(uchar Reg,uchar Dat);
	extern uchar I2C_ReadReg(uchar Reg);
#endif
#ifdef _SLAVE
	#define RegsNumber 255
	extern uchar xdata Regs[RegsNumber];
	extern uchar tempReg;
	extern bit I2C_SCL_Wait_While(bit SCL_Status);
	extern bit I2C_Await();
	extern bit I2C_Slave_SendAck(bit Ack);
	extern bit I2C_Slave_RecAck();
	extern bit I2C_Slave_SendByte(uchar Dat);
	extern uchar I2C_Slave_RecByte(void);
	extern bit I2C_Slave_WriteReg();
	extern bit I2C_Slave_ReadReg();
	extern uchar I2C_Slave_Await_Order();
#endif