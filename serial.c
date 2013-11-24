
#include "serial.h"
void Serial_Init(){
	
	SCON=0x50;//ѡ��ʽһ
	TMOD|=0x20;//��ʱ��һ��������ʽ2
	TH1=0xFD;//���ò�����  //������һ��9600
	TL1=0xFD;//
	PCON=0x00;
	TR1=1;//������ʱ��һ
	#ifdef RECV_EN
		ES=1;
		EA=1;
	#endif

}

void send(uchar s){
	TR0=0;
	SBUF=s;//װ�����ݵ����ڻ���	
	while(TI==0);//��ѯ������ɱ�־�����ʱ�ñ�־λ�ø�
	TR0=1;
	TI=0;//���������ɱ�־λ
}
void send_str(unsigned char * pd)   //�����ַ�����ֱ������0�Ž���
{
	 while((*pd)!='\0') 
	 {
	  send(*pd); //����һ���ַ�
	  pd++;  //�ƶ�����һ���ַ�
	 }
}
void send_ln()  	//���ͻس�
{
	send_str("\r\n");
}
void send_tab()  	//�����Ʊ��
{

	send_str("\x09");
}
void send_uint(uint num)  	//��������
{
	
 	unsigned int temp,i=0,j;
	temp=num;
 	do{
 		temp=temp/10;
		i++;
 	}while(temp>0);
 	while(i>0){
 		j=i;
		temp=1;
		while(j>1){j--;temp*=10;}
		send(num/temp+'0');
		num-=(num/temp)*temp;
 		i--;
 	}
}
void send_int(int num)  	//��������
{
	
 	unsigned int temp,i=0,j;
 	if(num<0){send('-');num=-num;} //���������
	temp=num;
 	do{
 		temp=temp/10;
		i++;
 	}while(temp>0);
 	while(i>0){
 		j=i;
		temp=1;
		while(j>1){j--;temp*=10;}
		send(num/temp+'0');
		num-=(num/temp)*temp;
 		i--;
 	}
}
void send_ulong(ulong num)  	//��������
{
	
 	unsigned int temp,i=0,j;
	temp=num;
 	do{
 		temp=temp/10;
		i++;
 	}while(temp>0);
 	while(i>0){
 		j=i;
		temp=1;
		while(j>1){j--;temp*=10;}
		send(num/temp+'0');
		num-=(num/temp)*temp;
 		i--;
 	}
}
void send_long(long num)  	//��������
{
	
 	unsigned int temp,i=0,j;
 	if(num<0){send('-');num=-num;} //���������
	temp=num;
 	do{
 		temp=temp/10;
		i++;
 	}while(temp>0);
 	while(i>0){
 		j=i;
		temp=1;
		while(j>1){j--;temp*=10;}
		send(num/temp+'0');
		num-=(num/temp)*temp;
 		i--;
 	}
}
void send_float(float num,uchar precise)  	//���͸�������precise��ʶ����
{
	uint temp;
 	if(num<0){send('-');num=-num;} //���������	
	if(num>=1){
		temp=(int)num;
		send_long(temp);
		num=num-temp;
		precise--;
	}else{
		send('0');
	}
	send('.');
	num=num*10;
	while(precise>0){
		precise--;
		temp=(int)num;
		send(temp+'0');
		num=(num-temp)*10;	
	}
}
void send_week(uchar week){
	switch(week){
	case 1:send_str("Monday");break;
	case 2:send_str("Tuesday");break;
	case 3:send_str("Wednesday");break;
	case 4:send_str("Thursday");break;
	case 5:send_str("Friday");break;
	case 6:send_str("Saturday");break;
	case 7:send_str("Sunday");break;
	default:send_str("Week_ERROR");break;
	}
}
#ifdef RECV_EN
uchar xdata Serial_Recv_Ptr=0;
uchar xdata Serial_Proc_Ptr=0;
bit Serial_Empty=1;
bit Serial_Full=0;
bit Serial_DataErr=0;

uchar xdata Serial_Recv_Buff[SERIAL_BUFF_LENGTH];
uchar xdata Serial_Data_Width;
uchar Serial_get_uchar(){
	uchar temp_ch;
	if(Serial_Empty){
		//���������Ϊ�����ȡʧ��
		Serial_DataErr=1;
		return 0;
	}	

	temp_ch=Serial_Recv_Buff[Serial_Proc_Ptr++];

	if(Serial_Full==1){Serial_Full=0;}//������־λ
	if(Serial_Proc_Ptr>=SERIAL_BUFF_LENGTH)Serial_Proc_Ptr=0;//ѭ��ָ��
	if(Serial_Recv_Ptr==Serial_Proc_Ptr){
	//���ָ�����������ʶ���ڻ�����Ϊ�ա�
		Serial_Empty=1;	
	}
	return temp_ch;											
}
uint Serial_get_uint(){
//��ʮ���ƣ�BCD�룩��ʽ��ȡһ��uint
	uint temp_uint=0;
	uchar temp_ch;
	delay(10);
	//delayʹ������ȫװ�ص�������

	//��һ�����ݲ����ǿո���С��ڴ�֮�ϼ����Ϸ���
	temp_ch=Serial_get_uchar();
	if(Serial_DataErr)return 0;//��������򷵻�

	while(temp_ch<=32 && (!Serial_Empty)){	//32�ǿո�
		temp_ch=Serial_get_uchar();
	}
	
	if(temp_ch>'9' || temp_ch<'0'){
		Serial_DataErr=1;
		return 0;
	}	
	while(temp_ch!=' ' && temp_ch!=10 && temp_ch!=13){  //10,13�ǻ��з�����������ո���
		if(temp_ch>'9' || temp_ch<'0'){
		//����õ�����ֵ����ֻ�getCharʧ��������ء�
			Serial_DataErr=1;
			return 0;
		}	
		temp_uint=temp_uint*10+temp_ch-'0';			
		if(Serial_Empty)break;	//����Ѿ�ȡ���գ��򷵻ء���Ϊ֮ǰ��delay�������Ѿ���ȫװ�ء�
		temp_ch=Serial_get_uchar();		
	}
	return temp_uint;
}
void Serial_getline(uchar * str){
//��ʮ���ƣ�BCD�룩��ʽ��ȡһ��string
	uchar temp_ch;
	uint i=0;
	delay(10);
	//delayʹ������ȫװ�ص�������
	//��һ�����ݲ����ǿո���С��ڴ�֮�ϼ����Ϸ���
	temp_ch=Serial_get_uchar();
	if(Serial_DataErr)return;//��������򷵻�
	while(temp_ch<=13 && (!Serial_Empty)){//һֱɨ�赽û�л��еĵط���ʼ��
		temp_ch=Serial_get_uchar();
	}
	str[i++]=temp_ch;
	while(str[i-1]!=10 && str[i-1]!=13){  //10,13�ǻ��з�������Թ�����
		if(Serial_Empty)break;	//����Ѿ�ȡ���գ��򷵻ء���Ϊ֮ǰ��delay�������Ѿ���ȫװ�ء�
		str[i++]=Serial_get_uchar();
	}
	str[i]=0;//���ϴ�β��
}

void Serial_Interrupt() interrupt 4  //uart�ж� ��4Ϊ�����ж�
{
  if(RI==1)  //�յ�����
  {	 
     if(!Serial_Full){Serial_Recv_Buff[Serial_Recv_Ptr++]=SBUF;} //���ջ������յ�����
	 if(Serial_Recv_Ptr==Serial_Proc_Ptr)Serial_Full=1;//���д��������λ��
	 if(Serial_Recv_Ptr>=SERIAL_BUFF_LENGTH)Serial_Recv_Ptr=0;//ѭ��ָ��
	 if(Serial_Empty==1){Serial_Empty=0;}//�崮�ڻ�����Ϊ�յı�־λ��
	 RI=0;   //�յ���0
  }
}
#endif