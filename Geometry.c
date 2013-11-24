#include "comm.h"
#include "math.h"
struct Path{
	int x0;
	int y0;
	int x1;
	int y1;
	int x;
	int y;
};
//Bresenham直线算法，步进走直线
bit BresenhamPathStep(int x0,int y0,int x1,int y1,int * x,int * y){
	int dL,tL;
	if((*x)==x1 && (*y)==y1)return 1;
	if(abs(y1-y0)>abs(x1-x0)){
		dL=((x1)-(*x))/abs((x1)-(*x));
		if(dL==0)dL=x0/abs(x0);
		tL=(y1-y0)*(((*x)-x0)*2+dL)/2/(x1-x0);
		if((*y)<tL){(*y)+=1;return 0;}
		if((*y)>tL){(*y)-=1;return 0;}
		
		(*x)=(*x)+dL;
	}else{
		dL=((y1)-(*y))/abs((y1)-(*y));
		if(dL==0)dL=y0/abs(y0);
		tL=(x1-x0)*(((*y)-y0)*2+dL)/2/(y1-y0);
		if((*x)<tL){(*x)+=1;return 0;}
		if((*x)>tL){(*x)-=1;return 0;}
		(*y)=(*y)+dL;
	}
	return 0;
}

void main(){
	int x0=0,y0=0,x1=-10,y1=11,x=0,y=0;
	while(1){
		BresenhamPathStep(x0,y0,x1,y1,&x,&y);
	}
}

