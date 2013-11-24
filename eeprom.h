#ifndef _EEPROM_H
	#define _EEPROM_H
	#include "comm.h"
	#include <intrins.h>
	#define	CMD_IDLE	0
	#define	CMD_READ	1
	#define	CMD_PROGRAM	2
	#define	CMD_ERASE	3 
	#define ENABLE_IAP	0x83
	#define IAP_ADDRESS	0x0000 
	#define IAP_ADDR_MAX	0x03FF
	//12C5A60S2包含两个扇区的EEPROM，每个扇区512字节，共1K字节可用。
	//即有IAP_ADDRESS到IAP_ADDRESS+1023可用
	/***********************************
	IAP_Idle():
		关闭ISP/IAP/EEPROM功能以保护MCU
	***********************************/
	extern void IAP_Idle();
	/***********************************
	EEPROM_Read:
		从EEPROM的地址addr读出一字节
	***********************************/
	extern uchar EEPROM_Read(uint addr);
	/***********************************
	EEPROM_Write:
		在EEPROM的地址addr写一字节
	***********************************/
	extern void EEPROM_Write(uint addr,uchar dat);
	/***********************************
	EEPROM_MultipleRead:
		从EEPROM的地址addr读出len长度的字符存储到buff中
		如果len==0，则以字符串格式读出，自动识别串尾
	***********************************/
	extern void EEPROM_MultipleRead(uint addr,uchar* buff,uint len);
	/***********************************
	EEPROM_MultipleWrite:
		在EEPROM的地址addr写入len长度的字符来自buff中
		如果len==0，则以字符串格式写入，自动识别串尾
	***********************************/
	extern void EEPROM_MultipleWrite(uint addr,uchar* buff,uint len);
	/***********************************
	EEPROM_EraseSector:
		EEPROM的addr扇区整个清除（初始化为0xff）
	***********************************/
	extern void EEPROM_EraseSector(uint addr);
#endif