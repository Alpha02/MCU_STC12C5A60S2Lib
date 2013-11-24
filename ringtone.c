/**************************
蜂鸣器实现模组，可以用来播放提示音，音乐应用
**************************/
#include "ringtone.h"
uint code freq[8]={256,256,228,205,192,171,154,137};
uint code time[8]={51,51,58,64,68,77,85,96};

uchar code song[33]={3,3,4,5,5,4,3,2,1,1,2,3,3,2,2,0,3,3,4,5,5,4,3,2,1,1,2,3,2,1,1,0,0xff};

//休止-do-re-mi-fa-so-la-xi
//0表示休止，0xff结尾

void beep(uint time_ms){
	#ifdef BUZZER_ACTIVE
		Buzzer=0;
		delay(time_ms);
		Buzzer=1;
	#else
		ulong i;
		for(i=0;i/100<time_ms;i++){
			Buzzer=~Buzzer;
		 	delayus(10);
		}
		Buzzer=1;
	#endif		
}
void beeps(uchar time){
	while(time--){
		beep(20);
		delay(70);
	}
	delay(200);
}
void playSong(){
	int note_idx,j,note_time;
	note_idx=0;
	note_time=time[song[note_idx]];
	while(1){
		for(j=note_time;j>0;j--){
			Buzzer=1;
			delayus(freq[song[note_idx]]*10);
			if(j>note_time/2 && song[note_idx]!=0){
				Buzzer=0;
			}
			delayus(freq[song[note_idx]]*10);
		}
		note_idx++;
		if(song[note_idx]==0xff)break;
	}
}
 