#include "soft_I2C.h"
#define TARGET_ADDRESS 0x0a
sbit SDA = P1^0;
sbit SCL = P1^1;
//����I2CЭ�����
	void I2C_Start(){
	//��ʼ����
		SDA=1;
		SCL=1;
		I2C_delay();
		SDA=0;
		I2C_delay();
		SCL=0;
	}
	//��������
	void I2C_Stop(){
		SDA=0;
		SCL=1;
		I2C_delay();
		SDA=1;
		I2C_delay();
	}
	void I2C_SendAck(bit Ack)
	//����Ӧ���ź�
	{
	//ע��Ack���ͱ�ʾ��Ӧ�𣬲����ͱ�ʾ��Ӧ��
	    SDA=Ack;//ack (0:ACK 1:NACK)
	    SCL=1;
	    I2C_delay();
	    SCL=0;
		SDA=1;
		//��������������������SDA��һֱ�ڵ͵�ƽ
	    I2C_delay();
		
		
	}
	//����Ӧ���ź�
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
	//����һ�ֽ����ݲ�����Ӧ���źš������Ӧ���Ӧ��0�򷵻�0�����򷵻�1
	    uchar i;
		bit AckReceived=0;
	    for(i=0;i<8;i++)
	    {
	        Dat<<=1; //���ݴ���ȡ����(��λ�ȴ�)
	        SDA=CY;	 //���ƺ�����ݴ���CY��
//			send_str("SendBit:");
//			send_uint(SDA);
//			send_ln();
	        SCL=1;
	        I2C_delay();
	        SCL=0;
			//����������׼����ACK
			SDA=1;
	        I2C_delay();

	    }
	    AckReceived=(!I2C_RecAck()); //����Ack�źš�
//			send_str("ACK:");
//			send_uint(AckReceived);
//			send_ln();
		//ȡ������1��ʾ�ɹ���0��ʾʧ�ܡ�
		return AckReceived;		
	}
	uchar I2C_RecByte(void)
	//����һ�ֽ����ݲ�����Ӧ���ź�
	{
	    uchar i,Dat=0;
	    SDA=1;
		//���豸���������ߣ�׼����¼
	    for(i=0;i<8;i++)
	    {
	        Dat<<=1;
	        SCL=1;
	        I2C_delay();
	        Dat |=SDA;
			//��λ��
	        SCL=0;
	        I2C_delay();
//			send_str("GetBit:");
//			send_uint(Dat);
//			send_ln();
		}
		I2C_SendAck(0);
		//Ӧ��
	    return Dat;
	}
	bit I2C_WriteReg(uchar Reg,uchar Dat){
		bit ack_receive;
		I2C_Start();
		ack_receive=I2C_SendByte(TARGET_ADDRESS);
		//����յ��ظ��������ͨѶ������������Ϳ�ʼ�ź�
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
		//����յ��ظ��������ͨѶ������������Ϳ�ʼ�ź�
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
