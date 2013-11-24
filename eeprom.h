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
	//12C5A60S2��������������EEPROM��ÿ������512�ֽڣ���1K�ֽڿ��á�
	//����IAP_ADDRESS��IAP_ADDRESS+1023����
	/***********************************
	IAP_Idle():
		�ر�ISP/IAP/EEPROM�����Ա���MCU
	***********************************/
	extern void IAP_Idle();
	/***********************************
	EEPROM_Read:
		��EEPROM�ĵ�ַaddr����һ�ֽ�
	***********************************/
	extern uchar EEPROM_Read(uint addr);
	/***********************************
	EEPROM_Write:
		��EEPROM�ĵ�ַaddrдһ�ֽ�
	***********************************/
	extern void EEPROM_Write(uint addr,uchar dat);
	/***********************************
	EEPROM_MultipleRead:
		��EEPROM�ĵ�ַaddr����len���ȵ��ַ��洢��buff��
		���len==0�������ַ�����ʽ�������Զ�ʶ��β
	***********************************/
	extern void EEPROM_MultipleRead(uint addr,uchar* buff,uint len);
	/***********************************
	EEPROM_MultipleWrite:
		��EEPROM�ĵ�ַaddrд��len���ȵ��ַ�����buff��
		���len==0�������ַ�����ʽд�룬�Զ�ʶ��β
	***********************************/
	extern void EEPROM_MultipleWrite(uint addr,uchar* buff,uint len);
	/***********************************
	EEPROM_EraseSector:
		EEPROM��addr���������������ʼ��Ϊ0xff��
	***********************************/
	extern void EEPROM_EraseSector(uint addr);
#endif