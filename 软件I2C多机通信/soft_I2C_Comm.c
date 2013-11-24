#include "soft_I2C.h"
void I2C_delay(){
	delayus(10);
	//大概达到4Mb/s,还包含串口传输耗费的速率
}
/*
void main_example(){
	bit ack_receive=0;
	uchar re=0;
	uchar se=0;	
	uint s_timer=0;
	SCL=1;
	SDA=1;
	Serial_Init();
	send_str("Ready...");
	send_ln();
	#ifdef _SLAVE
		for(re=0;re<255;re++){
			Regs[re]=3;	
		}
	#endif
	while(1){
	 #ifdef _MASTER
		re=I2C_ReadReg(se);
		send(se);
		send(re);
		send(0xff);
		I2C_WriteReg(se,re+1);

		se++; 
	 #endif
	 #ifdef _SLAVE
 		I2C_Slave_Await_Order();
	 #endif
	 }
}
*/