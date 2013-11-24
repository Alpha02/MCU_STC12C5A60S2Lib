#include "eeprom.h"

/***********************************
IAP_Idle():
	�ر�ISP/IAP/EEPROM�����Ա���MCU
***********************************/
void IAP_Idle(){
	IAP_CONTR=0; //�ر�IAP����
	IAP_CMD=0;
	IAP_TRIG=0;
	IAP_ADDRH=0x80;	//ָ��ָ���EEPROM����
	IAP_ADDRL=0;
} 
/***********************************
EEPROM_Read:
	��EEPROM�ĵ�ַaddr����һ�ֽ�
***********************************/
uchar EEPROM_Read(uint addr){
	uchar dat;
	if(addr>IAP_ADDR_MAX)return 0x00;	//�������λ�ó���EEPROM��Χ���򷵻�0��
	IAP_CONTR=ENABLE_IAP; //ʹ��IAP����
	IAP_CMD=CMD_READ;	  //���ö�ģʽ
	IAP_ADDRL=addr;		  //���õ�ַ
	IAP_ADDRH=addr>>8;
	
	IAP_TRIG=0x5a;	  //���������ź�
	IAP_TRIG=0xa5;
	_nop_();		  //MCU��ֱͣ���������
	dat=IAP_DATA;  	  //��������
	IAP_Idle();		  //�ر�EEPROM����
	return dat;		  //��������
}
/***********************************
EEPROM_Write:
	��EEPROM�ĵ�ַaddrдһ�ֽ�
***********************************/
void EEPROM_Write(uint addr,uchar dat){
	if(addr>IAP_ADDR_MAX)return;	//���д��λ�ó���EEPROM��Χ���򷵻ء�
	IAP_CONTR=ENABLE_IAP; //ʹ��IAP����
	IAP_CMD=CMD_PROGRAM;  //���ö�ģʽ
	IAP_ADDRL=addr;		  //���õ�ַ
	IAP_ADDRH=addr>>8;
	IAP_DATA=dat;
	IAP_TRIG=0x5a;	  //���������ź�
	IAP_TRIG=0xa5;
	_nop_();		  //MCU��ֱͣ���������
	IAP_Idle();		  //�ر�EEPROM����
}
/***********************************
EEPROM_MultipleRead:
	��EEPROM�ĵ�ַaddr����len���ȵ��ַ��洢��buff��
	���len==0�������ַ�����ʽ�������Զ�ʶ��β
***********************************/
void EEPROM_MultipleRead(uint addr,uchar* buff,uint len){
//�����len��Ϊ0�����Զ�ʶ��buf�Ĵ�β����(��Ϊbuf���ַ���)
	uint xdata i=0;
	if(addr+len>IAP_ADDR_MAX)return;	//�������λ�ó���EEPROM��Χ���򷵻�0��
	IAP_CONTR=ENABLE_IAP; //ʹ��IAP����
	IAP_CMD=CMD_READ;	  //���ö�ģʽ
	while(i<len || (len==0)){
		IAP_ADDRL=(addr+i);		  //���õ�ַ
		IAP_ADDRH=(addr+i)>>8;
		IAP_TRIG=0x5a;	  //���������ź�
		IAP_TRIG=0xa5;
		_nop_();		  //MCU��ֱͣ���������
		buff[i]=IAP_DATA;  	  //��������
		if(len==0 && buff[i]==0)break;//���ַ�������
		i++;
	}
	IAP_Idle();		  //�ر�EEPROM����
}
/***********************************
EEPROM_MultipleWrite:
	��EEPROM�ĵ�ַaddrд��len���ȵ��ַ�����buff��
	���len==0�������ַ�����ʽд�룬�Զ�ʶ��β
***********************************/
void EEPROM_MultipleWrite(uint addr,uchar* buff,uint len){
//�����len��Ϊ0�����Զ�ʶ��buf�Ĵ�β����(��Ϊbuf���ַ���)
	uint xdata i=0;

	if(addr+len>IAP_ADDR_MAX)return;	//���д��λ�ó���EEPROM��Χ���򷵻ء�
	IAP_CONTR=ENABLE_IAP; //ʹ��IAP����
	IAP_CMD=CMD_PROGRAM;  //���ö�ģʽ
	while(i<len || (len==0 ) ){
		IAP_ADDRL=addr+i;		  //���õ�ַ
		IAP_ADDRH=(addr+i)>>8;
		IAP_DATA=buff[i];  	  //��������

		IAP_TRIG=0x5a;	  //���������ź�
		IAP_TRIG=0xa5;
		_nop_();		  //MCU��ֱͣ���������
		if(len==0 && buff[i]==0)break;
		i++;
	}
	IAP_Idle();		  //�ر�EEPROM����

}
/***********************************
EEPROM_EraseSector:
	EEPROM��addr���������������ʼ��Ϊ0xff��
***********************************/
void EEPROM_EraseSector(uint addr){
	IAP_CONTR=ENABLE_IAP; //ʹ��IAP����
	IAP_CMD=CMD_ERASE;  //���ö�ģʽ
	IAP_ADDRL=addr;		  //���õ�ַ
	IAP_ADDRH=addr>>8;
	IAP_TRIG=0x5a;	  //���������ź�
	IAP_TRIG=0xa5;
	_nop_();		  //MCU��ֱͣ���������
	IAP_Idle();		  //�ر�EEPROM����
}