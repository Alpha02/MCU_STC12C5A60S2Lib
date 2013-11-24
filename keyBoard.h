#ifndef _KEYBOARD_H
#define	_KEYBOARD_H
	#include "comm.h"
	#define PORT_KEYBOARD P1
	#define KEY_1  0x0001
	#define KEY_2  0x0002
	#define KEY_3  0x0004
	#define KEY_4  0x0008
	#define KEY_5  0x0010
	#define KEY_6  0x0020
	#define KEY_7  0x0040
	#define KEY_8  0x0080
	#define KEY_9  0x0100
	#define KEY_10 0x0200
	#define KEY_11 0x0400
	#define KEY_12 0x0800
	#define KEY_13 0x1000
	#define KEY_14 0x2000
	#define KEY_15 0x4000
	#define KEY_16 0x8000
	#define KEY_ALL 0xffff
	extern uint keyScan(uint key_state);
	//调用keyScan时，可以选择性地扫描一部分键以加快扫描速度。
	//例：if(keyScan( KEY_1 | KEY_2 )==KEY_1) 如果KEY1和KEY2中按下了KEY1
	extern bit keyPress(uchar key_index);
	extern uint waitForKey(uint key_state,bit waitForAll);
	extern uint xdata KEY_Last;

#endif