#include "stdafx.h"
#include "Syaku8Tuner.h"
#include "Syaku8TunerDoc.h"
#include "Syaku8TunerView.h"
#include "math.h"
#include "s8header.h"
#define SRS 5.38//,21.52};//
#define NNN 4096//,1024};//fft number of data
extern BUFTYPE			*wavebuf;
const double SENT50=1.0293;
//const double R12=1.05946309;
double Amp[4100],sint[4100],cost[4100];
void initsincos()
{
	int i;
	for(i=0;i<=2048;i++)
	{
		sint[i] = sin( PAI * 2.0 * i / 4096);
		cost[i] = cos( PAI * 2.0 * i / 4096);
	}
}
void FFTWave1Calc(short *intp)
{ 
	int i,j,k,L,m,q,g,n2,n3,L2,L4,ave,wav[4096+4];
	double yreal[4096+4],yimage[4096+4],yampdata[4096+4];
	double a,b,c,s,a2,b1,dbMax;
	int nn=1;
	for(i=0,ave=0;i<NNN;i++){
		wav[i+1]=*(intp+i);
		ave+=wav[i+1];
	}
	ave=ave/NNN;
	for(i=0;i<NNN;i++){
		yreal[i+1]=(wav[i+1]-ave)*(0.5-0.5*cost[i]); // Hanning window
	}
	yreal[i+1]=0;
	for(i=0;i<=NNN;i++)yimage[i]=0.0;
	m=(int)(log10(NNN)/log10(2));
    L=NNN;
	b1=PAI*2.00/NNN;
	nn=1;    
    for(g=1;g<=m;g++){
	 	L2=L;L=L/2;a2=0;
		for(q=1;q<=L;q++){
			c=cost[(q-1)*nn];
			s=sint[(q-1)*nn];
			a2=a2+b1;
			for(L4=L2;L4<=NNN;L4=L4+L2){
		   		i=L4-L2+q;
				j=i+L;
				a=yreal[i]-yreal[j];
				b=yimage[i]-yimage[j];
               	yreal[i]=yreal[i]+yreal[j];
				yimage[i]=yimage[i]+yimage[j];
				yreal[j]=a*c+b*s;   
				yimage[j]=b*c-a*s;
			}
        }
        b1=b1+b1;
		nn=2*nn;
    }
    // bit reversal 
    j=1;
	n2=NNN/2;
	n3=NNN-1;
    for(i=1;i<=n3;i++){
    	if(i<j){
    		a=yreal[j];
			b=yimage[j];
            yreal[j]=yreal[i];
			yimage[j]=yimage[i];
			yreal[i]=a;
			yimage[i]=b;
            k=n2;
		}
		else k=n2;
        while(k<j){
        	j=j-k;
            k=k/2;
        }
		j=j+k;
    }
    for(i=1;i<NNN/2+1;i++){
    	yreal[i]=yreal[i+1];
		yimage[i]=yimage[i+1];
    }
    for(i=1;i<NNN/2+1;i++){
		yampdata[i]=(yreal[i]*yreal[i]+yimage[i]*yimage[i]);
	}
	for(i=1,dbMax=0;i<NNN/2+1;i++)if(dbMax<yampdata[i])dbMax=yampdata[i];
	if(dbMax*1.0<=0.01)dbMax=1.0;
	b=sqrt(dbMax);
	for(i=1;i<NNN/2+1;i++){
		Amp[i]=(a=sqrt(yampdata[i]))/b*100;
//		AmpdB[i]=20*log10(a);
	}
}

void Settone(double hz);
void Settone(double hz)
{
	int i;
	for(i=TONE1;i<TONE2;i++){
		if(hz>tone[i]&&hz<=tone[i+1])break;
	}
	if(i==TONE2)return;
	if(hz>tone[i]*SENT50)i++;
	if(DP.tone==i)return;
	DP.tone=i;
}
#define WID1 350
#define WID2 10
double Setsoukan();
double Setsoukan()
{
	int srate=1;//DP.srate;
	int i,j,maxi,n,wid,wid1;	
	double nd,nd1,hz,max=0,mix=256,ww;
	short *ip,wav[4100];
	if(DP.hzf==0||DP.hz==0)return 0;
	hz=DP.hz;
	wid1=4090;
	ip=wavebuf+2000;
	for(i=0;i<wid1;i++)wav[i]=*(ip+i);
	wid=int(int((wid1-WID1-WID2*2)/(22050/hz))*(22050/hz)-WID2);
	n=int(22050/hz)+1;
	if(n>WID1)n=WID1;
	for(i=0;i<WID2*2;i++){//”gŒ`‚Æ‚Ì‘ŠŠÖŒW”
		for(ww=0,j=0;j<n;j++)ww=ww+wav[wid+i+j]*wav[j];
		if(max<ww){max=ww;maxi=wid+i;}
	}
	nd=(double)maxi/(22050/hz);
	nd1=nd-int(nd);
	if(nd1<0.5)hz=22050/((double)maxi/int(nd));
	else hz=22050/((double)maxi/int(nd+1));
	if ((DP.hz-hz)*(DP.hz-hz)<SRS*SRS) return hz;
	else return DP.hz;
}
int Chehz(double d[])//  d[0] ‚Ì‰½•ª‚Ìˆê‚ªŠî–{Žü”g”‚©‚ð•Ô‚·
{
	int i,j,ans;
	double dd;
	if(d[1]==0)return 1;
	for(i=1;i<13;i++){//0 ”Ô–Ú‚Ìi•ª‚Ìˆê
		for(j=1;d[j]!=0;j++){
			dd=d[j]/(d[0]/i);
			dd=dd-int(dd);
			if(dd>0.05&&dd<0.95)break;
		}
		if(d[j]==0){
			if(dd<=0.05)ans=int(d[j-1]/(d[0]/i));
			else ans=int(d[j-1]/(d[0]/i)+1);
			return (j-1)*1000+ans;
		}
	}
	return 0;
}

double Setf(double a,double b)
{
	double f,g,r;
	if(a!=0&&b!=0){
		f=a/b;
		g=b/a;
		if(f<=0.5)r=0;
		else if(f>0.5&&f<=1.0)r=f-0.5;
		else if(f>1&&f<=2)r=1.5-g;
		else if(f>2) r=1;
	}
	else return 0.5;
	return r;
}
void Setfft();
void Setfft()
{
	int i,j,k,n,step;
	double	max,min,d[11],m[11];
	min=(double)DP.peaklevel;
	step=DMAX/256*5;
	for(i=int(50/SRS),j=0;i<int(tone[TONE2+1]/SRS+1)&&j<10;i++){//50Hz`3350Hz
		if (Amp[i]>Amp[i-step]+min && Amp[i]>Amp[i+step]+min);
		else continue;
		for(max=0,n=i-step;n<i+step;n++)
			if(Amp[n]>max){max=Amp[n];k=n;}
		i=i+step;
		d[j]=double(k)-0.5+Setf(2*Amp[k]-Amp[k-1]-Amp[k-2],2*Amp[k]-Amp[k+1]-Amp[k+2]);
		m[j]=max;
		d[j]=d[j]*SRS;
		if(d[j]<50)d[j]=0;
//		else if(d[j]>(double)DP.low1&&d[j]<(double)DP.high1)d[j]=0;
//		else if(d[j]>(double)DP.low2&&d[j]<(double)DP.high2)d[j]=0;
		else j++;
	}
	for(i=j;i<10;i++){d[i]=0;m[i]=0;}
	for(i=0;i<4;i++){//choose big 4
		for(k=0,max=0;d[k]!=0&&k<10;k++){
			if(max<m[k]){max=m[k];n=k;}
		}
		if(max!=0)m[n]=0;
	}
	for(i=0;d[i]!=0&&i<10;i++)if(m[i]!=0)d[i]=0;
	for(i=0;i<4;i++){
		for(k=i;k<10;k++){
			if(k==i&&d[k]!=0)break;
			if(d[k]!=0){d[i]=d[k];d[k]=0;break;}
		}
	}
	for(j=0;j<10;j++)if(d[j]==0)break;
	if((n=Chehz(d))&&j!=0){
		DP.hz=d[n/1000]/((n+1000)%1000);
		DP.hzf=1;
		DP.hz=Setsoukan();
	}else{
		DP.hzf=0;
		DP.hz=0;
	}
}

void Setwavehz()
{
	int i,sum;
	double minus=0xffffffffff;
	double lo,Height;//,adB;
	short *ip;
	int wav[520*3],save,max=-0xfff,min=0xfff;//,midline;
	ip=wavebuf+2000;
	for(i=0,sum=0;i<512*3;i++){
		wav[i]=*(ip+i);
		if(wav[i]>max)max=wav[i];
		if(wav[i]<min)min=wav[i];
		sum+=(int)wav[i];
	}
	save=sum/1536;
	for(i=0;i<512*3;i++)wav[i]=wav[i]-save;
	max=max-save;min=min-save;
	FFTWave1Calc(ip);
	Setfft();
	if(DP.hzf==1)DP.hz=DP.hz*(DP.hzcal%100000)/10000;

	int j,bi,minusi,mul=2;
	int midline=max/2+min/2;
	for(j=0;j<256;j++)if(wav[j]>=midline&&wav[j+1]<midline)break;
	Height=220/(double)(max-min);
	for(i=0;i<256;i++){//’¼‘O”gŒ`‚Æ‚Ì‘ŠŠÖŒW”
		for(bi=0,j=0;j<32;j++)bi=bi+wav[i+j]*DP.ynow[j]/128;
		if(bi<minus){minusi=i;minus=bi;}
	}
	for(i=0;i<256;i++){
		lo=(wav[minusi+i*mul]-midline);
		if((DP.ynow[i]=134-(int)(lo*Height))<17)DP.ynow[i]=17;
		else if(DP.ynow[i]>251)DP.ynow[i]=251;
		if(i<75)wa[waveNow].y[i]=DP.ynow[i];
	}

	for(i=0,max=-0xffff;i<256/3/2;i++){
		int step;
//		step=NNN/ 512;
		step=NNN/1024;
//		step=NNN/2048;
		for(lo=0,j=0;j<step*3*2;j++)if(lo<Amp[i*step*3*2+j]){lo=Amp[i*step*3*2+j];}//adB=AmpdB[i*step+j];}
	}
	waveNow=(waveCnt+SM)%SM;
	wa[waveNow].hz=DP.hz;
/*
	if(DP.hzf==1)Settone(DP.hz);
	wa[waveNow].tone=DP.tone;
	if(DP.hzf==1)DP.cent=int(2*1200*log(DP.hz/tone[DP.tone])/log(2));//–{—ˆ‚Ì‚Q”{
	else DP.cent=-127*2;
	wa[waveNow].cent=DP.cent;
*/	wa[waveNow].A440=DP.A440;
	if(DP.hzf==1)Settone(DP.hz*440.0/double(DP.A440));//DP.tone ‚ðƒZƒbƒg
	wa[waveNow].tone=DP.tone;
	if(DP.hzf==1)DP.cent=int(2*1200*log(DP.hz*440.0/double(DP.A440)/tone[DP.tone])/log(2));//–{—ˆ‚Ì‚Q”{
	else DP.cent=-127*2;
	wa[waveNow].cent=DP.cent;
}
