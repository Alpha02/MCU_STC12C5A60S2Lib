#include "soft_I2C.h"
#define TARGET_ADDRESS 0x0a
sbit SDA = P1^0;
sbit SCL = P1^1;
//主机I2C协议程序
	void I2C_Start(){
	//开始传输
		SDA=1;
		SCL=1;
		I2C_delay();
		SDA=0;
		I2C_delay();
		SCL=0;
	}
	//结束传输
	void I2C_Stop(){
		SDA=0;
		SCL=1;
		I2C_delay();
		SDA=1;
		I2C_delay();
	}
	void I2C_SendAck(bit Ack)
	//发送应答信号
	{
	//注意Ack拉低表示有应答，不拉低表示无应答。
	    SDA=Ack;//ack (0:ACK 1:NACK)
	    SCL=1;
	    I2C_delay();
	    SCL=0;
		SDA=1;
		//把数据线拉回来。否则SDA会一直在低电平
	    I2C_delay();
		
		
	}
	//接收应答信号
	bit I2C_RecAck()
	{
		bit temp;
	    SCL=1;
	    I2C_delay();
	    temp=SDA;
//		send_str("SDA:");
//		send_uint(temp);
//		send_ln();
	    SCL=0;
	    I2C_delay();
	    return temp;
	}
	bit I2C_SendByte(uchar Dat){
	//发送一字节数据并接收应答信号。如果无应答或应答0则返回0，否则返回1
	    uchar i;
		bit AckReceived=0;
	    for(i=0;i<8;i++)
	    {
	        Dat<<=1; //数据从左取出。(高位先传)
	        SDA=CY;	 //左移后的数据存在CY中
//			send_str("SendBit:");
//			send_uint(SDA);
//			send_ln();
	        SCL=1;
	        I2C_delay();
	        SCL=0;
			//拉高数据线准备接ACK
			SDA=1;
	        I2C_delay();

	    }
	    AckReceived=(!I2C_RecAck()); //接受Ack信号。
//			send_str("ACK:");
//			send_uint(AckReceived);
//			send_ln();
		//取反处理，1表示成功，0表示失败。
		return AckReceived;		
	}
	uchar I2C_RecByte(void)
	//接收一字节数据并发送应答信号
	{
	    uchar i,Dat=0;
	    SDA=1;
		//主设备拉高数据线，准备记录
	    for(i=0;i<8;i++)
	    {
	        Dat<<=1;
	        SCL=1;
	        I2C_delay();
	        Dat |=SDA;
			//高位先
	        SCL=0;
	        I2C_delay();
//			send_str("GetBit:");
//			send_uint(Dat);
//			send_ln();
		}
		I2C_SendAck(0);
		//应答
	    return Dat;
	}
	bit I2C_WriteReg(uchar Reg,uchar Dat){
		bit ack_receive;
		I2C_Start();
		ack_receive=I2C_SendByte(TARGET_ADDRESS);
		//如果收到回复，则进行通讯，否则继续发送开始信号
		if(ack_receive){
			I2C_SendByte(0x40);
			I2C_SendByte(Reg);
			I2C_SendByte(Dat);
			I2C_Stop();
			return 1;
		}else{
			return 0;
		}

	}
	uchar I2C_ReadReg(uchar Reg){
		bit ack_receive;
		uchar ch_temp;
		I2C_Start();
		ack_receive=I2C_SendByte(TARGET_ADDRESS);
		//如果收到回复，则进行通讯，否则继续发送开始信号
		if(ack_receive){
			I2C_SendByte(0x80);
			I2C_SendByte(Reg);
			ch_temp=I2C_RecByte();
			I2C_Stop();
			return ch_temp;
		}else{
			return 0;
		}
	}
