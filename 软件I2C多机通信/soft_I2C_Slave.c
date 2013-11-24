#include "soft_I2C.h"
sbit SDA = P1^0;
sbit SCL = P1^1;
	const uchar	SlaveAddress=0x0a;
	//�ӻ���ַ��ͨ��Э����ֻ�е�ַƥ��Ż����ָ�
	//uchar mode=0;
	bit ERR_flag=0;
	//ͨ�ų����־λ������һ��ͨ���з������˳�ʱ��λ��
	//�ñ�־λ��λʱ���������ݽ���������
	uint I2C_WDT;

	//I2Cͨ�ſ��Ź���
	#define WDT_MAX 65530
	//��źŵȴ�ʱ�䡣������ʱ�佫����ͨ�š�
	bit I2C_SCL_Wait_While(bit SCL_Status){
		uint counter=0;
	//���ڴӻ��ȴ�ʱ���ߡ����Ź���һ��ʱ������������ͨ��ʧ�ܡ�
		I2C_WDT=0;
		while(SCL==SCL_Status){	
			counter++;
			if(counter>WDT_MAX){counter=0;I2C_WDT+=1;}
			if(I2C_WDT>WDT_MAX){
				I2C_WDT=0;return 0;
				//��������ȴ�ʱ�䣬����0��ͨ�Ž��������
			}
		}
		return 1;		
	}
	bit I2C_Await(){
	//����������
		while(SCL==0);
		//�ȴ�ֱ��ʱ���߱�����
		if(SDA==0){
			while(SDA==0 && SCL==1);
			return 0;
			//SDA�ɵͱ�ߣ������յ�һ��Stop�ź�
			//���SCL�ֱ����ߣ��ź���Ч����ΪStop�źŴ���
		}
		while(SDA==1 && SCL==1);
		if(SCL==0)return 0;
		while(SCL==1);
		//�ȴ�������ʱ�������͡�
		//���SCL�������ߣ��ź���Ч����ΪStop�źŴ���	
		return 1;
		//SDA�ɸ߱�ͣ������յ�һ��Start�ź�
			
	}
	bit I2C_Slave_SendAck(bit Ack){
	//ע��Ack���ͱ�ʾ��Ӧ�𣬲����ͱ�ʾ��Ӧ��
		if(SCL==1)return 0;
		//���ʱ���߱������ˣ�����ʧ�ܡ�
//		send_str("SendAck:");
//		send_uint(Ack);
		SDA=Ack;//ack (0:ACK 1:NACK)
		//�ȴ�ʱ���������ź�
//		send_str("SDA:");
//		send_uint(SDA);
//		send_ln();
		if(!I2C_SCL_Wait_While(0))return 0;
		if(!I2C_SCL_Wait_While(1))return 0;
		SDA=1;
		//��������������������SDA��һֱ�ڵ͵�ƽ	
		return 1;
	}
	bit I2C_Slave_RecAck(){
		bit temp;
		if(SCL==1)return 0;
		//���ʱ���߱������ˣ�����ʧ�ܡ�
		//�ȴ�ʱ���������ź�
		if(!I2C_SCL_Wait_While(0))return 0;
	    temp=SDA;

		//��ʱ��������ʱ��ȡAck�źš�
		if(!I2C_SCL_Wait_While(1))return 0;	
//		send_str("RecAck:");
//		send_uint(temp);
//		send_ln();
	    return temp;
	}

	bit I2C_Slave_SendByte(uchar Dat){
	//�ӻ�����һ�ֽ����ݲ�����Ӧ���źš�
	    uchar i;
		bit ack_receive=0;
		if(SCL==1)return 0;
		//���ʱ���߱������ˣ�����ʧ�ܡ�
	    for(i=0;i<8;i++)
	    {
	        Dat<<=1; //���ݴ���ȡ����(��λ�ȴ�)
	        SDA=CY;	 //���ƺ�����ݴ���CY��
	        //�ȴ�ʱ���������źš�
//			send_str("SendBit:");
//			send_uint(SDA);
//			send_ln();
			if(!I2C_SCL_Wait_While(0))return 0;	        
	        if(!I2C_SCL_Wait_While(1))return 0;
	        SDA=1;
			//����SDA����ֹ��ƽ��ͻ���ȴ�����ACK�źš�
	    }
		
	    ack_receive=(!I2C_Slave_RecAck()); //����Ack�źš�
		//ȡ����������1��ʾ�ɹ���0��ʾʧ�ܡ�	
		return ack_receive;	
	}
	uchar I2C_Slave_RecByte(void)
	{
	//�ӻ�����һ�ֽ�����	
	    uchar i,Dat=0;
	    if(SCL==1){ERR_flag=1;return 0;}
		for(i=0;i<8;i++)
	    {
	        Dat<<=1;
	        if(!I2C_SCL_Wait_While(0))return 0;
	        Dat |=SDA;
			//��λ��
	        if(!I2C_SCL_Wait_While(1))return 0;
		}
		I2C_Slave_SendAck(0);
		//Ӧ��
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
		//�����Ĵ�����ַ
		order=I2C_Slave_RecByte();
		if(ERR_flag){ERR_flag=0;return 0;}
		//�������Ͳ�д��Ĵ�����
		Regs[regAddr]=order;
		return 1;	
	}
	bit I2C_Slave_ReadReg(){
		uchar order;
		uchar regAddr;
		bit ack;
		regAddr=I2C_Slave_RecByte();
		//�����Ĵ�����ַ
		if(ERR_flag){ERR_flag=0;return 0;}
		//�������Ͳ������Ĵ�����
		I2C_Slave_SendByte(Regs[regAddr]);
		//���͸üĴ�����ֵ	
		return 1;
	}
	uchar I2C_Slave_Await_Order(){
	//�ӻ�ָ����Ӧ������
	//����ָ���б�����
/**************************************
0x00	���ջ�����
0x01	���ջ�����RAM���㡣
0x02	���ջ�����EEPROM���㡣
0x04
		�ӽ��ջ�����ʱ�Ĵ���д�����ݡ�
0x08
		�ӽ��ջ�����ʱ�Ĵ����������ݡ�
0x40	0x??
		�ӽ��ջ�ָ����RAM��ʼ����д�����ݡ�ֱ����������Ӧ��
0x80	0x??
		�ӽ��ջ�ָ����RAM��ʼ�����������ݡ�ֱ����������Ӧ��	
**************************************/
		uchar order;
		while(!I2C_Await());
		//ѭ��ֱ���ӵ���ʼ�ź�
		order=I2C_Slave_RecByte();
		//����SlaveAddress
		if(order==SlaveAddress){
		//���Ѱַ��ȷ
			order=I2C_Slave_RecByte();
			//��������
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
