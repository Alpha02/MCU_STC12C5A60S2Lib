#include "comm.h"
sbit Buzzer = P2^3;
#define BUZZER_ACTIVE //有源蜂鸣器，注释则为无源蜂鸣器。如果使用播放特定音调的功能，最好用无源蜂鸣器。
//欢乐颂
extern void beep(uint time_ms);
extern void beeps(uchar time);
extern void playSong();