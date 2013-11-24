//键盘处理，包括基本的按键，键盘扫描，按键中断
//注意矩阵键盘没法多个键同时检测。
#include "keyBoard.h"
#include "serial.h"

//键盘共16个键，用一个uint表示其按键状态
/*
1	2	3	4
5	6	7	8
9	10	11	12
13	14	15	16
*/
sbit keyScan1 = P3^0;
sbit keyScan2 = P3^1;
sbit keyScan3 = P3^2;
sbit keyScan4 = P3^3;

sbit key1 = P3^4;
sbit key2 = P3^5;
sbit key3 = P3^6;
sbit key4 = P3^7;
uint xdata KEY_Last=0x0000; 

bit keyPressed(uchar key_index){
	key_index--;
	PORT_KEYBOARD=~(0x01<<(key_index/4));	
	if((0x01<<(key_index%4)) & ((~PORT_KEYBOARD)>>4)){
		//列号确认
		return	1; 	
	}
	return 0;		
}
bit keyDown(uchar key_index){
#define KEY_SMOOTHING 200
	uchar smoothing_count=0;
	while(keyPressed(key_index)){
		smoothing_count++;

		if(smoothing_count>KEY_SMOOTHING){
			return 1;	
		}
	
	}
	return 0;
}
bit keyUp(uchar key_index){
	if(!keyPressed(key_index)){
		return 0;
	}
	while(keyPressed(key_index));
	return 1;
}
bit keyPress(uchar key_index){
	if(keyDown(key_index)){if(keyUp(key_index))return 1;}
	return 0;
}
uint keyScan(uint key_state){
	uchar i=0;
	uint temp_scan=0x0001;
	for(i=1;i<=16;i++){
		
		if(key_state & temp_scan){
			if(keyPressed(i)){
				
			}else{
				key_state &= (~temp_scan);
			}
		} 
		temp_scan<<=1;
		
	}
	return key_state;

}
uint waitForKey(uint key_state,bit waitForAll){
//0 for anykey
	uint temp_state=0;
	if(key_state==0){
		while(temp_state==0){
			temp_state=keyScan(key_state);
		}
		
	}else{
		if(waitForAll){
			while(temp_state!=key_state){
				temp_state=keyScan(key_state);
			}
		}else{
			_send_value(temp_state);
			while((temp_state&key_state)==0){
				temp_state=keyScan(key_state);
				
			}
		}
	}
	while(keyScan(key_state)!=0);
	//等待键松开；
	KEY_Last=temp_state;
	return temp_state;
}