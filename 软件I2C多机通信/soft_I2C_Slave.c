#include "soft_I2C.h"
sbit SDA = P1^0;
sbit SCL = P1^1;
	const uchar	SlaveAddress=0x0a;
	//从机地址。通信协议中只有地址匹配才会接收指令。
	//uchar mode=0;
	bit ERR_flag=0;
	//通信出错标志位，当从一个通信中非正常退出时置位。
	//该标志位置位时，所得数据将被丢弃。
	uint I2C_WDT;

	//I2C通信看门狗。
	#define WDT_MAX 65530
	//最长信号等待时间。超过此时间将结束通信。
	bit I2C_SCL_Wait_While(bit SCL_Status){
		uint counter=0;
	//用于从机等待时钟线。看门狗在一段时间后溢出。返回通信失败。
		I2C_WDT=0;
		while(SCL==SCL_Status){	
			counter++;
			if(counter>WDT_MAX){counter=0;I2C_WDT+=1;}
			if(I2C_WDT>WDT_MAX){
				I2C_WDT=0;return 0;
				//如果超过等待时间，返回0，通信将会结束。
			}
		}
		return 1;		
	}
	bit I2C_Await(){
	//待机函数。
		while(SCL==0);
		//等待直到时钟线被拉低
		if(SDA==0){
			while(SDA==0 && SCL==1);
			return 0;
			//SDA由低变高，标明收到一个Stop信号
			//如果SCL又被拉高，信号无效，视为Stop信号处理。
		}
		while(SDA==1 && SCL==1);
		if(SCL==0)return 0;
		while(SCL==1);
		//等待主机将时钟线拉低。
		//如果SCL意外拉高，信号无效，视为Stop信号处理。	
		return 1;
		//SDA由高变低，标明收到一个Start信号
			
	}
	bit I2C_Slave_SendAck(bit Ack){
	//注意Ack拉低表示有应答，不拉低表示无应答。
		if(SCL==1)return 0;
		//如果时钟线被拉高了，发送失败。
//		send_str("SendAck:");
//		send_uint(Ack);
		SDA=Ack;//ack (0:ACK 1:NACK)
		//等待时钟线脉冲信号
//		send_str("SDA:");
//		send_uint(SDA);
//		send_ln();
		if(!I2C_SCL_Wait_While(0))return 0;
		if(!I2C_SCL_Wait_While(1))return 0;
		SDA=1;
		//把数据线拉回来。否则SDA会一直在低电平	
		return 1;
	}
	bit I2C_Slave_RecAck(){
		bit temp;
		if(SCL==1)return 0;
		//如果时钟线被拉高了，接收失败。
		//等待时钟线脉冲信号
		if(!I2C_SCL_Wait_While(0))return 0;
	    temp=SDA;

		//在时钟线拉高时读取Ack信号。
		if(!I2C_SCL_Wait_While(1))return 0;	
//		send_str("RecAck:");
//		send_uint(temp);
//		send_ln();
	    return temp;
	}

	bit I2C_Slave_SendByte(uchar Dat){
	//从机发送一字节数据并接收应答信号。
	    uchar i;
		bit ack_receive=0;
		if(SCL==1)return 0;
		//如果时钟线被拉高了，接收失败。
	    for(i=0;i<8;i++)
	    {
	        Dat<<=1; //数据从左取出。(高位先传)
	        SDA=CY;	 //左移后的数据存在CY中
	        //等待时钟线脉冲信号。
//			send_str("SendBit:");
//			send_uint(SDA);
//			send_ln();
			if(!I2C_SCL_Wait_While(0))return 0;	        
	        if(!I2C_SCL_Wait_While(1))return 0;
	        SDA=1;
			//拉高SDA，防止电平冲突，等待接收ACK信号。
	    }
		
	    ack_receive=(!I2C_Slave_RecAck()); //接受Ack信号。
		//取反处理，现在1表示成功，0表示失败。	
		return ack_receive;	
	}
	uchar I2C_Slave_RecByte(void)
	{
	//从机接收一字节数据	
	    uchar i,Dat=0;
	    if(SCL==1){ERR_flag=1;return 0;}
		for(i=0;i<8;i++)
	    {
	        Dat<<=1;
	        if(!I2C_SCL_Wait_While(0))return 0;
	        Dat |=SDA;
			//高位先
	        if(!I2C_SCL_Wait_While(1))return 0;
		}
		I2C_Slave_SendAck(0);
		//应答
	    return Dat;
	}
//	#define RegsNumber 255
	uchar xdata Regs[RegsNumber];
	uchar tempReg;
	bit I2C_Slave_WriteReg(){
		uchar order;
		uchar regAddr;
		bit ack;
		regAddr=I2C_Slave_RecByte();
		//读出寄存器地址
		order=I2C_Slave_RecByte();
		if(ERR_flag){ERR_flag=0;return 0;}
		//如果出错就不写入寄存器。
		Regs[regAddr]=order;
		return 1;	
	}
	bit I2C_Slave_ReadReg(){
		uchar order;
		uchar regAddr;
		bit ack;
		regAddr=I2C_Slave_RecByte();
		//读出寄存器地址
		if(ERR_flag){ERR_flag=0;return 0;}
		//如果出错就不读出寄存器。
		I2C_Slave_SendByte(Regs[regAddr]);
		//发送该寄存器的值	
		return 1;
	}
	uchar I2C_Slave_Await_Order(){
	//从机指令响应函数。
	//基本指令列表如下
/**************************************
0x00	接收机重启
0x01	接收机所有RAM清零。
0x02	接收机所有EEPROM清零。
0x04
		从接收机的临时寄存器写入数据。
0x08
		从接收机的临时寄存器读出数据。
0x40	0x??
		从接收机指定的RAM开始连续写入数据。直到主机放弃应答。
0x80	0x??
		从接收机指定的RAM开始连续读出数据。直到主机放弃应答。	
**************************************/
		uchar order;
		while(!I2C_Await());
		//循环直到接到开始信号
		order=I2C_Slave_RecByte();
		//接收SlaveAddress
		if(order==SlaveAddress){
		//如果寻址正确
			order=I2C_Slave_RecByte();
			//接收命令
			switch(order){
				case 0x00:break;
				case 0x01:break;
				case 0x02:break;
				case 0x04:break;
				case 0x08:break;
				case 0x10:break;
				case 0x20:break;
				case 0x40:I2C_Slave_WriteReg();break;
				case 0x80:I2C_Slave_ReadReg();break;
				default:break;
			}
			//I2C_Slave_SendByte(255-order);	
		}
		return 1;		
	}
