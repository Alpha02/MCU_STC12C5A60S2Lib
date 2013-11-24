
#include "serial.h"
void Serial_Init(){
	
	SCON=0x50;//选择方式一
	TMOD|=0x20;//定时器一，工作方式2
	TH1=0xFD;//设置波特率  //波特率一律9600
	TL1=0xFD;//
	PCON=0x00;
	TR1=1;//启动定时器一
	#ifdef RECV_EN
		ES=1;
		EA=1;
	#endif

}

void send(uchar s){
	TR0=0;
	SBUF=s;//装载数据到串口缓冲	
	while(TI==0);//查询发送完成标志，完成时该标志位置高
	TR0=1;
	TI=0;//清楚发送完成标志位
}
void send_str(unsigned char * pd)   //发送字符串，直到遇到0才结束
{
	 while((*pd)!='\0') 
	 {
	  send(*pd); //发送一个字符
	  pd++;  //移动到下一个字符
	 }
}
void send_ln()  	//发送回车
{
	send_str("\r\n");
}
void send_tab()  	//发送制表符
{

	send_str("\x09");
}
void send_uint(uint num)  	//发送整数
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
void send_int(int num)  	//发送整数
{
	
 	unsigned int temp,i=0,j;
 	if(num<0){send('-');num=-num;} //处理成正数
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
void send_ulong(ulong num)  	//发送整数
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
void send_long(long num)  	//发送整数
{
	
 	unsigned int temp,i=0,j;
 	if(num<0){send('-');num=-num;} //处理成正数
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
void send_float(float num,uchar precise)  	//发送浮点数，precise标识精度
{
	uint temp;
 	if(num<0){send('-');num=-num;} //处理成正数	
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
		//如果缓冲区为空则读取失败
		Serial_DataErr=1;
		return 0;
	}	

	temp_ch=Serial_Recv_Buff[Serial_Proc_Ptr++];

	if(Serial_Full==1){Serial_Full=0;}//清满标志位
	if(Serial_Proc_Ptr>=SERIAL_BUFF_LENGTH)Serial_Proc_Ptr=0;//循环指针
	if(Serial_Recv_Ptr==Serial_Proc_Ptr){
	//如果指针相遇，则标识串口缓冲区为空。
		Serial_Empty=1;	
	}
	return temp_ch;											
}
uint Serial_get_uint(){
//以十进制（BCD码）方式读取一个uint
	uint temp_uint=0;
	uchar temp_ch;
	delay(10);
	//delay使数据完全装载到缓冲区

	//第一个数据不能是空格或换行。在此之上检测其合法性
	temp_ch=Serial_get_uchar();
	if(Serial_DataErr)return 0;//如果出错则返回

	while(temp_ch<=32 && (!Serial_Empty)){	//32是空格
		temp_ch=Serial_get_uchar();
	}
	
	if(temp_ch>'9' || temp_ch<'0'){
		Serial_DataErr=1;
		return 0;
	}	
	while(temp_ch!=' ' && temp_ch!=10 && temp_ch!=13){  //10,13是换行符，这个当做空格处理
		if(temp_ch>'9' || temp_ch<'0'){
		//如果得到了奇怪的数字或getChar失败则出错返回。
			Serial_DataErr=1;
			return 0;
		}	
		temp_uint=temp_uint*10+temp_ch-'0';			
		if(Serial_Empty)break;	//如果已经取到空，则返回。因为之前的delay，数据已经完全装载。
		temp_ch=Serial_get_uchar();		
	}
	return temp_uint;
}
void Serial_getline(uchar * str){
//以十进制（BCD码）方式读取一个string
	uchar temp_ch;
	uint i=0;
	delay(10);
	//delay使数据完全装载到缓冲区
	//第一个数据不能是空格或换行。在此之上检测其合法性
	temp_ch=Serial_get_uchar();
	if(Serial_DataErr)return;//如果出错则返回
	while(temp_ch<=13 && (!Serial_Empty)){//一直扫描到没有换行的地方开始读
		temp_ch=Serial_get_uchar();
	}
	str[i++]=temp_ch;
	while(str[i-1]!=10 && str[i-1]!=13){  //10,13是换行符，这个略过处理
		if(Serial_Empty)break;	//如果已经取到空，则返回。因为之前的delay，数据已经完全装载。
		str[i++]=Serial_get_uchar();
	}
	str[i]=0;//加上串尾符
}

void Serial_Interrupt() interrupt 4  //uart中断 ，4为串口中断
{
  if(RI==1)  //收到数据
  {	 
     if(!Serial_Full){Serial_Recv_Buff[Serial_Recv_Ptr++]=SBUF;} //接收缓冲区收到数据
	 if(Serial_Recv_Ptr==Serial_Proc_Ptr)Serial_Full=1;//如果写满，则置位。
	 if(Serial_Recv_Ptr>=SERIAL_BUFF_LENGTH)Serial_Recv_Ptr=0;//循环指针
	 if(Serial_Empty==1){Serial_Empty=0;}//清串口缓冲区为空的标志位。
	 RI=0;   //收到清0
  }
}
#endif