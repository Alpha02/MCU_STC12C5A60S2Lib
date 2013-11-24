#include "SD.h"
#include "comm.h"
#include "serial.h"

//��дһ���ֽ�
static unsigned char SD_WriteByte(unsigned char dat);
//sd��д����
static unsigned char SD_SendCommand(unsigned char cmd, unsigned long arg);

//------------------------------------------------------------
//�ڲ���������
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
	
	//��6��0xff,�ӳ�һ��ʱ��
	//SD_WriteByte(0xff);
	//SD_WriteByte(0xff);
	//SD_WriteByte(0xff);
	//SD_WriteByte(0xff);
	//SD_WriteByte(0xff);
	SD_WriteByte(0xff);

	SD_ENABLE(); //ʹ��SD��
	
	//д������Ͳ���
	SD_WriteByte(cmd | 0x40);
	SD_WriteByte(arg >> 24);
	SD_WriteByte(arg >> 16);
	SD_WriteByte(arg >> 8);
	SD_WriteByte(arg);
	//���һλ��1,��7λ��CRC
	//ֻ��idle���ģʽ��SDת����SPIʱ����Ч
	//֮��CRC��������
	SD_WriteByte(0x95);
	
	//�ȴ���Ӧ
	retry = 0;
	while ((result = SD_WriteByte(0xff)) == 0xff)
	{
		retry++;
		//��ʱ�˳�
		if (retry > TIME_OUT)
			break;
	}

	SD_DISABLE(); //��ֹSD��

	return result; //����״ֵ̬
}

//------------------------------------------------------------
//�ⲿ��������
//------------------------------------------------------------
unsigned char SD_Init(void)
{
	unsigned char result;
	unsigned char retry;
	unsigned char i;
	
	SD_DISABLE(); //��ֹSD��

	//����100��0xff,�ӳ�һ��ʱ��
	for(i = 0; i < 100; i++)
		SD_WriteByte(0xff);

	//��idle����
	retry = 0;
	do
	{
		result = SD_SendCommand(0, 0); //��idle����
		retry++;
		//��ʱ�˳�
		if (retry > TIME_OUT)
			return SD_NO;
	} while (result != 0x01); //�����Ϊ0x01��ѭ��
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
		result = SD_SendCommand(1, 0); //��active����
		retry++;
		//��ʱ�˳�
		if (retry > TIME_OUT)
			return SD_NO;
	} while (result != 0x00); //�����Ϊ0x00��ѭ��

	result = SD_SendCommand(59, 0); //��CRC
	result = SD_SendCommand(16, 512); //��������С512

	return SD_OK; //��������
}

unsigned char SD_ReadBlock(unsigned long sector, unsigned char *buffer,unsigned int begin,unsigned int length)
{
	unsigned char result;
	unsigned char retry;
	unsigned short i;

	retry = 0;
	do
	{
		result = SD_SendCommand(17, sector << 9); //��������
		retry++;
		//��ʱ�˳�
		if (retry > TIME_OUT)
			return SD_NO;
	} while (result);	


	SD_ENABLE(); //ʹ��SD��

	//�����ݵĿ�ʼ
	while (SD_WriteByte(0xff) != 0xfe) ; //if ((retry++) > 50) {SD_DISABLE();return SD_NO;} //���Ͽ��Է�ֹ����
	//�ն�
	for (i = 0; i < begin; i++)
		SD_WriteByte(0xff);
	//��length���ֽ�
	for (i = 0; i < length; i++)
		buffer[i] = SD_WriteByte(0xff);
	for (i = 0; i < 512-begin-length; i++)
		SD_WriteByte(0xff);

	//αCRC
	SD_WriteByte(0xff);
	SD_WriteByte(0xff);
	
	SD_DISABLE(); //��ֹSD��

	return SD_OK; //��������
}

unsigned char SD_WriteBlock(unsigned long sector, unsigned char *buffer)
{
	unsigned char result;
	unsigned char retry;
	unsigned short i;

	retry = 0;
	do
	{
		result = SD_SendCommand(24, sector << 9); //��д����
		retry++;
		//��ʱ�˳�
		if (retry > TIME_OUT)
			return SD_NO;
	} while (result);	

	SD_ENABLE(); //ʹ��SD��
	
	//��6��0xff
	//SD_WriteByte(0xff);
	//SD_WriteByte(0xff);
	//SD_WriteByte(0xff);
	//SD_WriteByte(0xff);
	//SD_WriteByte(0xff);
	SD_WriteByte(0xff);

	SD_WriteByte(0xfe); //����ʼ��
	
	//д512�ֽ�
	for(i = 0; i < 512; i++)
		SD_WriteByte(buffer[i]);
	
	//αCRC
	SD_WriteByte(0xff);
	SD_WriteByte(0xff);
	
	//�ж��Ƿ�д�ɹ�
	result = SD_WriteByte(0xff);
	if( (result & 0x1f) != 0x05)
	{
		SD_DISABLE(); //��ֹSD��
		return SD_NO;
	}

	//�ȴ�������
	while (!SD_WriteByte(0xff)) ; //if ((retry++) > 50) {SD_DISABLE();return SD_NO;} //���Ͽ��Է�ֹ����

	SD_DISABLE(); //��ֹSD��

	return SD_OK; //��������
}

unsigned char SD_ReadCapacity(unsigned long *capacity)
{
	unsigned char csd[16];
	unsigned char result;
	unsigned short i;

	unsigned short csize;
	
	result = SD_SendCommand(9, 0); //����CSD�Ĵ�������

	//�����������ʧ���򷵻�
	if (result)
		return SD_NO;

	SD_ENABLE(); //ʹ��SD��

	//�����ݵĿ�ʼ
	while (SD_WriteByte(0xff) != 0xfe) ; //if ((retry++) > 50) {SD_DISABLE();return SD_NO;} //���Ͽ��Է�ֹ����

	for(i = 0; i < 16; i++)
		csd[i] = SD_WriteByte(0xff);

	//αCRC
	SD_WriteByte(0xff);
	SD_WriteByte(0xff);
	
	SD_DISABLE();  //��ֹSD��

	//����SD������
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