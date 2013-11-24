#include "ds1302.h"
#include "serial.h"
uchar t_y,t_m,t_d,t_h,t_min,t_s,t_week;
uchar ch_read;
sbit CE = P1^2;
sbit DAT = P1^3;
sbit SCLK = P1^4;

uchar SPI_Read(){
	uchar i;
	ch_read=0;
	//CE=1;
	for(i=0;i<8;i++){
		
		SCLK=1;
		delayus(8);
		SCLK=0;
		delayus(8);			
						
		ch_read=(ch_read>>1)+(((uchar)DAT)<<7);

	}
	return ch_read;
}
void SPI_Write(uchar ch){
	uchar i;
	SCLK=0;
	//CE=1;
	for(i=0;i<8;i++){
		SCLK=0;
		delayus(8);		
		DAT=(bit)(ch&0x01);
		ch=ch>>1;
		SCLK=1;	
		delayus(8);	
	}
}
uchar SPI_ReadReg(uchar reg){
	SCLK=0;
	CE=0;
	delayus(8);
	CE=1;
	delayus(8);
	SPI_Write(DS1302_READ_REG+reg);
	SPI_Read();
	SCLK=0;
	CE=0;
	return ch_read;
}
void SPI_WriteReg(uchar reg,uchar dat){
	CE=0;
	SCLK=0;
	delayus(8);
	CE=1;
	delayus(8);
	SPI_Write(DS1302_WRITE_REG+reg);
	SPI_Write(dat);
	SCLK=0;
	CE=0;
}
void DS1302_SetAllTime(){
	 t_s=t_s/10*16+t_s%10;
	 t_min=t_min/10*16+t_min%10;
	 t_h=t_h/10*16+t_h%10;
	 t_d=t_d/10*16+t_d%10;
	 t_m=t_m/10*16+t_m%10;
	 t_y=t_y/10*16+t_y%10;
	 
	 DS1302_MultipleWriteReg();

}
void DS1302_Init(){
	 SPI_WriteReg(DS1302_WP,0x00);//¹Ø±ÕÐ´±£»¤
	 t_h=SPI_ReadReg(DS1302_HOUR);
	 if((t_h&0x80)>0)t_h-=0x80;
	 #if _RESET_TIME
	 t_s=_S_RESET;
	 t_min=_MIN_RESET;
	 t_h=_H_RESET;
	 t_d=_D_RESET;
	 t_m=_M_RESET;
	 t_y=_Y_RESET;
	 t_week=_WEEK_RESET;
	 DS1302_SetAllTime();
	 send_str("Reset_All_Time");
	 send_ln();
	 #endif
}

void DS1302_GetAllTime(){
/*
	 t_s=SPI_ReadReg(DS1302_SECOND);
	 t_min=SPI_ReadReg(DS1302_MINUTE);
	 t_h=SPI_ReadReg(DS1302_HOUR);
	 t_d=SPI_ReadReg(DS1302_DAY);
	 t_m=SPI_ReadReg(DS1302_MONTH);
	 t_y=SPI_ReadReg(DS1302_YEAR);
	 t_week=SPI_ReadReg(DS1302_WEEK);
*/
	 DS1302_MultipleReadReg();
	 t_s=t_s/16*10+t_s%16;
	 t_min=t_min/16*10+t_min%16;
	 t_h=t_h/16*10+t_h%16;
	 t_d=t_d/16*10+t_d%16;
	 t_m=t_m/16*10+t_m%16;
	 t_y=t_y/16*10+t_y%16;
	 send_uint(t_y+_Y_ADD);send_str(".");
	 send_uint(t_m);send_str(".");
	 send_uint(t_d);send_str(".");
	 send_tab();
	 send_uint(t_h);send_str(":");
	 send_uint(t_min);send_str(":");
	 send_uint(t_s);
	 send_tab();
	 send_week(t_week);
	 send_ln();
}
void DS1302_MultipleReadReg(){
	SCLK=0;
	CE=0;
	delayus(8);
	CE=1;
	delayus(8);
	SPI_Write(DS1302_READ_REG+DS1302_BURST);
	t_s=SPI_Read();
	t_min=SPI_Read();
	t_h=SPI_Read();
	t_d=SPI_Read();
	t_m=SPI_Read();
	t_week=SPI_Read();
	t_y=SPI_Read();
	SCLK=0;
	CE=0;
}
void DS1302_MultipleWriteReg(){
	SCLK=0;
	CE=0;
	delayus(8);
	CE=1;
	delayus(8);
	SPI_Write(DS1302_WRITE_REG+DS1302_BURST);
	SPI_Write(t_s);
	SPI_Write(t_min);
	SPI_Write(t_h);
	SPI_Write(t_d);
	SPI_Write(t_m);
	SPI_Write(t_week);
	SPI_Write(t_y);
	SCLK=0;
	CE=0;
}
