#include "eeprom.h"

/***********************************
IAP_Idle():
	关闭ISP/IAP/EEPROM功能以保护MCU
***********************************/
void IAP_Idle(){
	IAP_CONTR=0; //关闭IAP功能
	IAP_CMD=0;
	IAP_TRIG=0;
	IAP_ADDRH=0x80;	//指针指向非EEPROM区域
	IAP_ADDRL=0;
} 
/***********************************
EEPROM_Read:
	从EEPROM的地址addr读出一字节
***********************************/
uchar EEPROM_Read(uint addr){
	uchar dat;
	if(addr>IAP_ADDR_MAX)return 0x00;	//如果读的位置超出EEPROM范围，则返回0。
	IAP_CONTR=ENABLE_IAP; //使能IAP功能
	IAP_CMD=CMD_READ;	  //设置读模式
	IAP_ADDRL=addr;		  //设置地址
	IAP_ADDRH=addr>>8;
	
	IAP_TRIG=0x5a;	  //给出触发信号
	IAP_TRIG=0xa5;
	_nop_();		  //MCU暂停直到操作完成
	dat=IAP_DATA;  	  //读出数据
	IAP_Idle();		  //关闭EEPROM功能
	return dat;		  //返回数据
}
/***********************************
EEPROM_Write:
	在EEPROM的地址addr写一字节
***********************************/
void EEPROM_Write(uint addr,uchar dat){
	if(addr>IAP_ADDR_MAX)return;	//如果写的位置超出EEPROM范围，则返回。
	IAP_CONTR=ENABLE_IAP; //使能IAP功能
	IAP_CMD=CMD_PROGRAM;  //设置读模式
	IAP_ADDRL=addr;		  //设置地址
	IAP_ADDRH=addr>>8;
	IAP_DATA=dat;
	IAP_TRIG=0x5a;	  //给出触发信号
	IAP_TRIG=0xa5;
	_nop_();		  //MCU暂停直到操作完成
	IAP_Idle();		  //关闭EEPROM功能
}
/***********************************
EEPROM_MultipleRead:
	从EEPROM的地址addr读出len长度的字符存储到buff中
	如果len==0，则以字符串格式读出，自动识别串尾
***********************************/
void EEPROM_MultipleRead(uint addr,uchar* buff,uint len){
//如果将len设为0，将自动识别buf的串尾符。(认为buf是字符串)
	uint xdata i=0;
	if(addr+len>IAP_ADDR_MAX)return;	//如果读的位置超出EEPROM范围，则返回0。
	IAP_CONTR=ENABLE_IAP; //使能IAP功能
	IAP_CMD=CMD_READ;	  //设置读模式
	while(i<len || (len==0)){
		IAP_ADDRL=(addr+i);		  //设置地址
		IAP_ADDRH=(addr+i)>>8;
		IAP_TRIG=0x5a;	  //给出触发信号
		IAP_TRIG=0xa5;
		_nop_();		  //MCU暂停直到操作完成
		buff[i]=IAP_DATA;  	  //读出数据
		if(len==0 && buff[i]==0)break;//读字符串结束
		i++;
	}
	IAP_Idle();		  //关闭EEPROM功能
}
/***********************************
EEPROM_MultipleWrite:
	在EEPROM的地址addr写入len长度的字符来自buff中
	如果len==0，则以字符串格式写入，自动识别串尾
***********************************/
void EEPROM_MultipleWrite(uint addr,uchar* buff,uint len){
//如果将len设为0，将自动识别buf的串尾符。(认为buf是字符串)
	uint xdata i=0;

	if(addr+len>IAP_ADDR_MAX)return;	//如果写的位置超出EEPROM范围，则返回。
	IAP_CONTR=ENABLE_IAP; //使能IAP功能
	IAP_CMD=CMD_PROGRAM;  //设置读模式
	while(i<len || (len==0 ) ){
		IAP_ADDRL=addr+i;		  //设置地址
		IAP_ADDRH=(addr+i)>>8;
		IAP_DATA=buff[i];  	  //读出数据

		IAP_TRIG=0x5a;	  //给出触发信号
		IAP_TRIG=0xa5;
		_nop_();		  //MCU暂停直到操作完成
		if(len==0 && buff[i]==0)break;
		i++;
	}
	IAP_Idle();		  //关闭EEPROM功能

}
/***********************************
EEPROM_EraseSector:
	EEPROM的addr扇区整个清除（初始化为0xff）
***********************************/
void EEPROM_EraseSector(uint addr){
	IAP_CONTR=ENABLE_IAP; //使能IAP功能
	IAP_CMD=CMD_ERASE;  //设置读模式
	IAP_ADDRL=addr;		  //设置地址
	IAP_ADDRH=addr>>8;
	IAP_TRIG=0x5a;	  //给出触发信号
	IAP_TRIG=0xa5;
	_nop_();		  //MCU暂停直到操作完成
	IAP_Idle();		  //关闭EEPROM功能
}