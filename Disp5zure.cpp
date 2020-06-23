#include "stdafx.h"
#include "Syaku8Tuner.h"
#include "Syaku8TunerDoc.h"
#include "Syaku8TunerView.h"
#include "math.h"
#include "s8header.h"
extern int ZUREY,ZUREY1;
//#define ZUREX 32
//#define ZUREY1 85//五線譜
//#define ZUREY 171//CENT表示
extern DATADP DP;
//extern double tone[];//
//extern int waveNow,waveCnt,Lt,BarN;
extern DWORD grayRGB;

int printoneb(CDC* MemDC,CDC* pDC,char *p,int x,int y,int f,int mode=0);
int printoneb(CDC* MemDC,CDC* pDC,char *p,int x,int y,int f,int mode)
{
	int n,dx=8,dy=13;
	if(*p=='.')n=17;
	else if(*p==':')n=18;
	else if(*p=='H')n=19;
	else if(*p=='z')n=20;
	else if(*p=='s')n=21;
	else if(*p==' ')n=22;
	else if(*p=='q')n=1000;//sec
	else if(*p>='a'&&*p<='z')n=*p-'a'+10;
	else if(*p>='A'&&*p<='Z')n=*p-'A'+10;
	else if(*p>='0'&&*p<='9')n=*p-'0';
	else n=0;
	if(mode==0){
		dx=8;dy=13;
		if(n==17)pDC->BitBlt(x,y,4,dy,MemDC,31+dx*(n%4),13*(n/4),f);
		else if(n==22)pDC->BitBlt(x,y,4,dy,MemDC,0,10,f);
		else if(n==1000)pDC->BitBlt(x,y,20,dy,MemDC,31+dx*(21%4),13*(21/4),f);
		else pDC->BitBlt(x,y,dx,dy,MemDC,31+dx*(n%4),13*(n/4),f);
	}else{
		dx=6;dy=10;
		pDC->BitBlt(x,y,dx,dy,MemDC,dx*n,96,f);
	}
	if(n==17)return 4;
	else if(n==22)return 2;
	return dx;
}
//4,96 small font
void printabc(CDC* MemDC,CDC* pDC,char *p,int x,int y,int f,int mode)
{
	int dx=8;
	while(*p!=0){
		dx=printoneb(MemDC,pDC,p,x,y,f,mode);
		p++;
		x+=dx;
	}
}
void DispNote1(CDC* MemDC,CDC* pDC,int pos,int data){
	int x,y,y1=ZUREY1,type,sc,cent;
	char buf[15];
	x=ZUREX+7*pos+14;
	sc=wa[data].tone;//sc:69 A 
	cent=wa[data].cent/2;
	if(cent<=50&&cent>=-50){
			 if(sc%24==23){y=y1-30;type=0;}
		else if(sc%24==22){y=y1-30;type=1;}//b--
		else if(sc%24==21){y=y1-25;type=0;}
		else if(sc%24==20){y=y1-25;type=1;}//a
		else if(sc%24==19){y=y1-20;type=0;}
		else if(sc%24==18){y=y1-20;type=1;}//g--
		else if(sc%24==17){y=y1-15;type=0;}//f
		else if(sc%24==16){y=y1-10;type=0;}
		else if(sc%24==15){y=y1-10;type=1;}//e--
		else if(sc%24==14){y=y1-5 ;type=0;}
		else if(sc%24==13){y=y1-5 ;type=1;}//d
		else if(sc%24==12){y=y1   ;type=0;}//c--
		else if(sc%24==11){y=y1+5 ;type=0;}
		else if(sc%24==10){y=y1+5 ;type=1;}
		else if(sc%24==9){y=y1-60 ;type=0;}
		else if(sc%24==8){y=y1-60 ;type=1;}
		else if(sc%24==7){y=y1-55 ;type=0;;}//g
		else if(sc%24==6){y=y1-55 ;type=1;}//g--b
		else if(sc%24==5){y=y1-50 ;type=0;}//f
		else if(sc%24==4){y=y1-45 ;type=0;}//e
		else if(sc%24==3){y=y1-45 ;type=1;}//e--b
		else if(sc%24==2){y=y1-40 ;type=0;}//d
		else if(sc%24==1){y=y1-40 ;type=1;}//d b
		else if(sc%24==0){y=y1-35 ;type=0;;}//c--

	}else{y=y1+20;type=2;}
	int fux=0,fuy=0;//396 17
	if(type==2)pDC->BitBlt(x+1,/*y-2*/y1-76,7,10,MemDC,fux+(type%4)*8,fuy,SRCCOPY);//測定不可
	else{
		pDC->BitBlt(x,y+1,7,9,MemDC,fux+(type%4)*8,fuy,SRCCOPY);//音符表示
		if((sc/12-1)>0&&(sc/12-1)<9){//オクターブ番号表示
			sprintf(buf,"%d",sc/12-1);
			printabc(MemDC,pDC,buf,x+1,y1-75,SRCCOPY,1);
		}
	}
	if(!DP.f){
		if(data==waveNow)pDC->FillSolidRect(ZUREX+17+pos*7,ZUREY-152,2,207,RGB(0,0,0));//縦線１
	}
}

void DispZureall1(CDC* MemDC,CDC* pDC,int pos,int data)
{
	int cent;
	cent=wa[data].cent/2;
	if(cent>=-50&&cent<=50){
		pDC->FillSolidRect(ZUREX+7*pos+14+1,ZUREY-cent,6,3,RGB(0,0,0));
	}
}
void DispZureInit(CDC* MemDC,CDC* pDC)
{
	int my=ZUREY,my5=ZUREY-132,mx=4;//62;
	int winwidth=DP.winw-38;
	pDC->FillSolidRect(mx+30,my -9,winwidth,1,RGB(200,200,200));
	pDC->FillSolidRect(mx+30,my-19,winwidth,1,RGB(200,200,200));//-40
	pDC->FillSolidRect(mx+30,my-29,winwidth,1,RGB(200,200,200));
	pDC->FillSolidRect(mx+30,my-39,winwidth,1,RGB(200,200,200));
	pDC->FillSolidRect(mx+30,my-49,winwidth,1,RGB(200,200,200));
	pDC->FillSolidRect(mx+30,my+1,winwidth,1,RGB(0,0,0));//00セント
	pDC->FillSolidRect(mx+30,my+11,winwidth,1,RGB(200,200,200));
	pDC->FillSolidRect(mx+30,my+21,winwidth,1,RGB(200,200,200));
	pDC->FillSolidRect(mx+30,my+31,winwidth,1,RGB(200,200,200));
	pDC->FillSolidRect(mx+30,my+41,winwidth,1,RGB(200,200,200));//+40
	pDC->FillSolidRect(mx+30,my+51,winwidth,1,RGB(200,200,200));
//	pDC->FillSolidRect(mx+30,my5-19,winwidth,1,RGB(210,210,210));//五線
	pDC->FillSolidRect(mx+30,my5- 9,winwidth,1,RGB(240,240,240));
	pDC->FillSolidRect(mx+30,my5+ 1,winwidth,1,RGB(0,0,0));
	pDC->FillSolidRect(mx+30,my5+11,winwidth,1,RGB(0,0,0));
	pDC->FillSolidRect(mx+30,my5+21,winwidth,1,RGB(0,0,0));
	pDC->FillSolidRect(mx+30,my5+31,winwidth,1,RGB(0,0,0));
	pDC->FillSolidRect(mx+30,my5+41,winwidth,1,RGB(0,0,0));
	pDC->FillSolidRect(mx+30,my5+51,winwidth,1,RGB(240,240,240));
//	pDC->FillSolidRect(mx+30,my5+61,winwidth,1,RGB(210,210,210));
	pDC->BitBlt(mx,my5-13,30,72,MemDC,0,10,SRCCOPY);//ト音符号
	pDC->BitBlt(mx+4,my-45,22,97,MemDC,63,0,SRCCOPY);//セント表示
	pDC->BitBlt(mx+3,my-57-3,27,9,MemDC,31,85,SRCCOPY);//cent
}

void DispZureall(CDC* MemDC,CDC* pDC)
{
	int i,j;
	DispZureInit(MemDC,pDC);
	if(waveCnt<BarN){
		for(i=0;i<=waveCnt;i++){
			DispZureall1(MemDC,pDC,i,i);
			DispNote1(MemDC,pDC,i,i);
		}
	}else{
		for(i=0;i<BarN;i++){
			if(DP.f)j=(i+waveCnt-BarN+1)%SM;
			else j=(i+Lt)%SM;
			DispZureall1(MemDC,pDC,i,j);
			DispNote1(MemDC,pDC,i,j);
		}
	}
}

void HidariMigi(int n)
{
	if(DP.f)return;
	if(waveCnt<BarN){
		waveNow+=n;
		if(waveNow<0)waveNow=0;
		else if(waveNow>waveCnt)waveNow=waveCnt;
		Lt=0;
	}else if(waveCnt<SM){//バッファサイズ以下なら
		if(waveNow+n>=0&&waveNow+n<=waveCnt){//移動後の位置が範囲内なら
			waveNow+=n;
			if(n>0){
				if(waveNow-Lt>=BarN)Lt=waveNow-BarN+1;//表示範囲を右にはみ出たら
			}
			else if(waveNow-Lt<0)Lt=waveNow;//左にはみ出たら
		}else if(waveNow+n<0){
			waveNow=0;
			Lt=0;
		}else{
			waveNow=waveCnt;
			Lt=waveNow-BarN+1;
		}
	}else{
		int wc,wn;
		wc=waveCnt%SM;
		wn=waveNow;
		if(n>0){//右への移動
			if(wn+n<SM){//移動してもはみ出ない
				if(wn<=wc){//移動前は最後より左
					if(wn+n<=wc){//移動後も最後より左
						waveNow+=n;
						if(Lt<=wn&&(Lt+BarN<=waveNow))Lt=waveNow-BarN+1;
						else if(Lt>wn&&(BarN+Lt<=SM+waveNow))Lt=(waveNow-BarN+1+SM)%SM;
					}else waveNow=wc;
				}else{//移動前は最後より右
					waveNow+=n;
					if(Lt+BarN<=waveNow)Lt=waveNow-BarN+1;
				}
			}else{//はみ出る
				if(wn<=wc){//移動前は最後より左
					waveNow=wc;
				}else{//移動前は最後より右
					if((wn+n)%SM<=wc){//移動後は最後より左
						waveNow=(wn+n)%SM;
						if(wn+n>Lt+BarN)Lt=(waveNow-BarN+1+SM)%SM;
					}else {
						waveNow=wc;
					}
				}
			}
			if(waveNow==wc)Lt=(wc+SM-BarN+1)%SM;
		}else{//左への移動
			if(wn+n>=0){//移動してもはみ出ない
				if(wn>wc){//移動前は最後より右
					if(wn+n>wc){//移動後も最後より右
						waveNow+=n;
						if(Lt<=wn&&waveNow<Lt)Lt=waveNow;
					}else {
						waveNow=(wc+1)%SM;
						Lt=waveNow;
					}
				}else{
					waveNow+=n;
					if(Lt<=wn&&waveNow<Lt)Lt=waveNow;
				}
			}else{//はみ出る
				if(wn>wc){//移動前は最後より右
					waveNow=(wc+1)%SM;
					Lt=waveNow;
				}else{//移動前は最後より左
					if((wn+n+SM)<=wc){//移動後も最後より左
						waveNow=(wc+1)%SM;
						Lt=waveNow;
					}else {
						waveNow=wn+n+SM;
						if(Lt<=wn)Lt=waveNow;
						else if(Lt>wn&&waveNow<Lt)waveNow;
					}
				}
			}
		}
	}
}
