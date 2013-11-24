#include "I2C.h"
//=============IIC����================
sbit SCL=P0^0;      //IICʱ�����Ŷ���
sbit SDA=P0^1;      //IIC�������Ŷ���
#define SlaveAddress 0xD0   //����������IIC�����еĴӵ�ַ(д��ַ��������ַ+1��
//delay
void I2C_delay(){
	delayus(4);
}
/*
��ʼ��־��S�����������豸�ᴫ��һ��7 λ��Slave ��ַ�����Һ������һ����8
λ����ΪRead/Write λ��R/W λ��ʾ���豸���ڽ��ܴ��豸�����ݻ���������д���ݡ�Ȼ
�����豸�ͷ�SDA �ߣ��ȴ����豸��Ӧ���źţ�ACK����ÿ���ֽڵĴ��䶼Ҫ������һ
��Ӧ��λ��Ӧ�����ʱ�����豸��SDA �����Ͳ�����SCL Ϊ�ߵ�ƽʱ���ֵ͡����ݴ�����
����ֹͣ��־��P��������Ȼ���ͷ�ͨ����·��Ȼ�������豸Ҳ���Բ����ظ��Ŀ�ʼ�ź�ȥ
������һ̨���豸����������������־�����Ͽ�֪�����е�SDA �źű仯��Ҫ��SCL ʱ��
Ϊ�͵�ƽʱ���У����˿�ʼ�ͽ�����־��
*/
//��ʼ�ź�
void I2C_Start()
{
    SDA=1;
    SCL=1;
    I2C_delay();
    SDA=0;
    I2C_delay();
    SCL=0;
}
//ֹͣ�ź�
void I2C_Stop()
{
    SDA=0;
    SCL=1;
    I2C_delay();
    SDA=1;
    I2C_delay();
}
//����Ӧ���ź�
void I2C_SendAck(bit Ack)
{
    SDA=Ack;//ack (0:ACK 1:NACK)
    SCL=1;
    I2C_delay();
    SCL=0;
    I2C_delay();
}
//����Ӧ���ź�
bit I2C_RecAck()
{
    SCL=1;
    I2C_delay();
    CY=SDA;
    SCL=0;
    I2C_delay();
    return CY;
}
//��IIC���߷���һ���ֽ�(8λ)����
void I2C_Send_Byte(uchar Dat)
{
    uchar i;
    for(i=0;i<8;i++)
    {
        Dat<<=1; //���ݴ���ȡ����
        SDA=CY;	 //���ƺ�����ݴ���CY��
        SCL=1;
        I2C_delay();
        SCL=0;
        I2C_delay();
    }
    I2C_RecAck(); //����Ack�źš�
}
/*��IIC���߽���һ���ֽ�����*/
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