#ifndef SD_H
#define SD_H
#include "comm.h"
sbit SD_CS  =P1^7; //使能
sbit SD_DI  =P1^1; //数据输入
sbit SD_DO  =P1^2; //数据输出
sbit SD_CLK =P1^3; //时钟

#define SD_ENABLE()  {SD_CS = 0;} //使能SD卡
#define SD_DISABLE() {SD_CS = 1;} //禁止SD卡

#define SD_OK 0 //操作SD卡成功
#define SD_NO 1 //操作SD卡失败

#define TIME_OUT 100 //超时次数

//初始化SD卡
extern unsigned char SD_Init(void);
//读取SD卡扇区数,每扇区512字节,成功返回SD_OK,失败返回_SD_NO
extern unsigned char SD_ReadCapacity(unsigned long *capacity);
//读扇区,成功返回SD_OK,失败返回_SD_NO,读合法扇区以外会死锁
extern unsigned char SD_ReadBlock(unsigned long sector, unsigned char *buffer,unsigned int begin,unsigned int length);
//写扇区,成功返回SD_OK,失败返回_SD_NO,写合法扇区以外也视为正常写入
extern unsigned char SD_WriteBlock(unsigned long sector, unsigned char *buffer);

#endif
