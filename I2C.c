#include "I2C.h"
//=============IIC总线================
sbit SCL=P0^0;      //IIC时钟引脚定义
sbit SDA=P0^1;      //IIC数据引脚定义
#define SlaveAddress 0xD0   //定义器件在IIC总线中的从地址(写地址）（读地址+1）
//delay
void I2C_delay(){
	delayus(4);
}
/*
开始标志（S）发出后，主设备会传送一个7 位的Slave 地址，并且后面跟着一个第8
位，称为Read/Write 位。R/W 位表示主设备是在接受从设备的数据还是在向其写数据。然
后，主设备释放SDA 线，等待从设备的应答信号（ACK）。每个字节的传输都要跟随有一
个应答位。应答产生时，从设备将SDA 线拉低并且在SCL 为高电平时保持低。数据传输总
是以停止标志（P）结束，然后释放通信线路。然而，主设备也可以产生重复的开始信号去
操作另一台从设备，而不发出结束标志。综上可知，所有的SDA 信号变化都要在SCL 时钟
为低电平时进行，除了开始和结束标志。
*/
//起始信号
void I2C_Start()
{
    SDA=1;
    SCL=1;
    I2C_delay();
    SDA=0;
    I2C_delay();
    SCL=0;
}
//停止信号
void I2C_Stop()
{
    SDA=0;
    SCL=1;
    I2C_delay();
    SDA=1;
    I2C_delay();
}
//发送应答信号
void I2C_SendAck(bit Ack)
{
    SDA=Ack;//ack (0:ACK 1:NACK)
    SCL=1;
    I2C_delay();
    SCL=0;
    I2C_delay();
}
//接收应答信号
bit I2C_RecAck()
{
    SCL=1;
    I2C_delay();
    CY=SDA;
    SCL=0;
    I2C_delay();
    return CY;
}
//向IIC总线发送一个字节(8位)数据
void I2C_Send_Byte(uchar Dat)
{
    uchar i;
    for(i=0;i<8;i++)
    {
        Dat<<=1; //数据从左取出。
        SDA=CY;	 //左移后的数据存在CY中
        SCL=1;
        I2C_delay();
        SCL=0;
        I2C_delay();
    }
    I2C_RecAck(); //接受Ack信号。
}
/*从IIC总线接收一个字节数据*/
uchar I2C_Rec_Byte(void)
{
    uchar i,Dat=0;
    SDA=1;
    for(i=0;i<8;i++)
    {
        Dat<<=1;
        SCL=1;
        I2C_delay();
        Dat |=SDA;
        SCL=0;
        I2C_delay();
    }
    return Dat;
}