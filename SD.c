#include "SD.h"
#include "comm.h"
#include "serial.h"

//读写一个字节
static unsigned char SD_WriteByte(unsigned char dat);
//sd卡写命令
static unsigned char SD_SendCommand(unsigned char cmd, unsigned long arg);

//------------------------------------------------------------
//内部操作函数
//------------------------------------------------------------

unsigned char SD_WriteByte(unsigned char dat)
{
	unsigned char i;

	for (i = 0; i < 8; i++)
	{
		SD_CLK = 0;

		if (dat & 0x80)
			SD_DI = 1;
		else
			SD_DI = 0;

		dat <<= 1;

		if (SD_DO)
			dat |= 0x01;
		else
			dat &= 0xfe;

		SD_CLK = 1;
	}

	return dat;
}

unsigned char SD_SendCommand(unsigned char cmd, unsigned long arg)
{
	unsigned char result;
	unsigned char retry;
	
	//发6个0xff,延迟一段时间
	//SD_WriteByte(0xff);
	//SD_WriteByte(0xff);
	//SD_WriteByte(0xff);
	//SD_WriteByte(0xff);
	//SD_WriteByte(0xff);
	SD_WriteByte(0xff);

	SD_ENABLE(); //使能SD卡
	
	//写入命令和参数
	SD_WriteByte(cmd | 0x40);
	SD_WriteByte(arg >> 24);
	SD_WriteByte(arg >> 16);
	SD_WriteByte(arg >> 8);
	SD_WriteByte(arg);
	//最后一位是1,高7位是CRC
	//只有idle命令将模式从SD转换成SPI时才有效
	//之后CRC将被忽略
	SD_WriteByte(0x95);
	
	//等待响应
	retry = 0;
	while ((result = SD_WriteByte(0xff)) == 0xff)
	{
		retry++;
		//超时退出
		if (retry > TIME_OUT)
			break;
	}

	SD_DISABLE(); //禁止SD卡

	return result; //返回状态值
}

//------------------------------------------------------------
//外部操作函数
//------------------------------------------------------------
unsigned char SD_Init(void)
{
	unsigned char result;
	unsigned char retry;
	unsigned char i;
	
	SD_DISABLE(); //禁止SD卡

	//发送100个0xff,延迟一段时间
	for(i = 0; i < 100; i++)
		SD_WriteByte(0xff);

	//发idle命令
	retry = 0;
	do
	{
		result = SD_SendCommand(0, 0); //发idle命令
		retry++;
		//超时退出
		if (retry > TIME_OUT)
			return SD_NO;
	} while (result != 0x01); //如果不为0x01则循环
	retry = 0;
	do
	{
		result = SD_SendCommand(8, 0);
	}while(retry++<20); 
	retry = 0;
	do
	{

		result = SD_SendCommand(55, 0); 
		result = SD_SendCommand(41, 0); 
	}while(retry++<20); 

	do
	{
		result = SD_SendCommand(1, 0); //发active命令
		retry++;
		//超时退出
		if (retry > TIME_OUT)
			return SD_NO;
	} while (result != 0x00); //如果不为0x00则循环

	result = SD_SendCommand(59, 0); //关CRC
	result = SD_SendCommand(16, 512); //设扇区大小512

	return SD_OK; //正常返回
}

unsigned char SD_ReadBlock(unsigned long sector, unsigned char *buffer,unsigned int begin,unsigned int length)
{
	unsigned char result;
	unsigned char retry;
	unsigned short i;

	retry = 0;
	do
	{
		result = SD_SendCommand(17, sector << 9); //发读命令
		retry++;
		//超时退出
		if (retry > TIME_OUT)
			return SD_NO;
	} while (result);	


	SD_ENABLE(); //使能SD卡

	//等数据的开始
	while (SD_WriteByte(0xff) != 0xfe) ; //if ((retry++) > 50) {SD_DISABLE();return SD_NO;} //加上可以防止死锁
	//空读
	for (i = 0; i < begin; i++)
		SD_WriteByte(0xff);
	//读length个字节
	for (i = 0; i < length; i++)
		buffer[i] = SD_WriteByte(0xff);
	for (i = 0; i < 512-begin-length; i++)
		SD_WriteByte(0xff);

	//伪CRC
	SD_WriteByte(0xff);
	SD_WriteByte(0xff);
	
	SD_DISABLE(); //禁止SD卡

	return SD_OK; //正常返回
}

unsigned char SD_WriteBlock(unsigned long sector, unsigned char *buffer)
{
	unsigned char result;
	unsigned char retry;
	unsigned short i;

	retry = 0;
	do
	{
		result = SD_SendCommand(24, sector << 9); //发写命令
		retry++;
		//超时退出
		if (retry > TIME_OUT)
			return SD_NO;
	} while (result);	

	SD_ENABLE(); //使能SD卡
	
	//发6个0xff
	//SD_WriteByte(0xff);
	//SD_WriteByte(0xff);
	//SD_WriteByte(0xff);
	//SD_WriteByte(0xff);
	//SD_WriteByte(0xff);
	SD_WriteByte(0xff);

	SD_WriteByte(0xfe); //发开始符
	
	//写512字节
	for(i = 0; i < 512; i++)
		SD_WriteByte(buffer[i]);
	
	//伪CRC
	SD_WriteByte(0xff);
	SD_WriteByte(0xff);
	
	//判断是否写成功
	result = SD_WriteByte(0xff);
	if( (result & 0x1f) != 0x05)
	{
		SD_DISABLE(); //禁止SD卡
		return SD_NO;
	}

	//等待操作完
	while (!SD_WriteByte(0xff)) ; //if ((retry++) > 50) {SD_DISABLE();return SD_NO;} //加上可以防止死锁

	SD_DISABLE(); //禁止SD卡

	return SD_OK; //正常返回
}

unsigned char SD_ReadCapacity(unsigned long *capacity)
{
	unsigned char csd[16];
	unsigned char result;
	unsigned short i;

	unsigned short csize;
	
	result = SD_SendCommand(9, 0); //发读CSD寄存器命令

	//如果发送命令失败则返回
	if (result)
		return SD_NO;

	SD_ENABLE(); //使能SD卡

	//等数据的开始
	while (SD_WriteByte(0xff) != 0xfe) ; //if ((retry++) > 50) {SD_DISABLE();return SD_NO;} //加上可以防止死锁

	for(i = 0; i < 16; i++)
		csd[i] = SD_WriteByte(0xff);

	//伪CRC
	SD_WriteByte(0xff);
	SD_WriteByte(0xff);
	
	SD_DISABLE();  //禁止SD卡

	//计算SD卡容量
	i = csd[0] >> 6;

	//CSD ver 1.XX
	if (i == 0)
	{
		i = (csd[5] & 15) + ((csd[10] & 128) >> 7) + ((csd[9] & 3) << 1) + 2;
		csize = (csd[8] >> 6) + ((unsigned short)csd[7] << 2) + ((unsigned short)(csd[6] & 3) << 10) + 1;
		*capacity = (unsigned long)csize << (i - 9);
	}

	//CSD ver 2.00
	if (i == 1)
	{
		csize = csd[9] + ((unsigned short)csd[8] << 8) + 1;
		*capacity = (unsigned long)csize << 10;
	}

	return SD_OK;
}
//uchar xdata SDbuffer[512];
/*
void main(){
	uint i;
	uchar sta;
	ulong cap;
	Serial_Init();
	sta=SD_Init();
	for(i=0;i<512;i++){
		buffer[i]=i;
	}
	for(i=1;i<10000;i++){
		sta=SD_WriteBlock(i+233,buffer);
	//	send_str("Write:");
	//	send_uint(i);
	//	send_ln();
	if(i%100==1)_send_value(i);
	}

	send_str("Read:");
	_send_value(sta);
	delay(200);
	sta=SD_ReadBlock(0,buffer);
	_send_value(sta);
	delay(200);
	for(i=0;i<512;i++){
		_send_value(i);
		_send_value(buffer[i]);
	}

	}
*/