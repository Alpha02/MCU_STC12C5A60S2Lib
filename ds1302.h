#ifndef _DS1302_H
	#define _DS1302_H
	#include "comm.h"
	#define _RESET_TIME 0		//确定是否开机时重设时间，0关闭，1打开
	#define _Y_RESET	13
	#define _Y_ADD 		2000
	#define _M_RESET	8
	#define _D_RESET	15
	#define _H_RESET	22
	#define _MIN_RESET	57
	#define _S_RESET	30
	#define _WEEK_RESET	4
	#define DS1302_WRITE_REG	0x00
	#define DS1302_READ_REG 	0x01
	#define DS1302_SECOND		0x80
	#define DS1302_MINUTE		0x82
	#define DS1302_HOUR			0x84
	#define DS1302_DAY			0x86
	#define DS1302_MONTH		0x88
	#define DS1302_YEAR			0x8C
	#define DS1302_WEEK			0x8A
	#define DS1302_WP			0x8E
	#define DS1302_BURST		0xBE
	extern uchar t_y,t_m,t_d,t_h,t_min,t_s,t_week;
	extern uchar ch_read;
	extern uchar SPI_Read();
	extern void SPI_Write(uchar ch);
	extern uchar SPI_ReadReg(uchar reg);
	extern void SPI_WriteReg(uchar reg,uchar dat);
	extern void DS1302_SetAllTime();
	extern void DS1302_Init();
	extern void DS1302_GetAllTime();
	extern void DS1302_MultipleReadReg();
	extern void DS1302_MultipleWriteReg();
#endif