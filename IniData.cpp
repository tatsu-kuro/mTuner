#include "stdafx.h"
#include "Syaku8Tuner.h"
#include "Syaku8TunerDoc.h"
#include "Syaku8TunerView.h"
#include "s8header.h"
//extern DATADP DP;

int paran=8;
char *ptext[]={
	"FFT peak cutting Level(2-90):",
	"temporizing Hz amending fraction(0.9000-1.1000):",
	"WindowRect(left):",
	"WindowRect(top):",
	"WindowRect(right):",
	"WindowRect(bottom):",
	"WindowMax(0-1):",
	"A:"
};
short *ppara[]={&DP.peaklevel,&DP.hzcal,&DP.x0,&DP.y0,&DP.x1,&DP.y1,&DP.max,&DP.A440};

short defau[]={20,0,10,10,1000,410,0,440};


void SetIniStr(char *str,char *buf,int n){
	int i;
	strcpy(str,&buf[n]);
	for(i=0;str[i]!='\n';)i++;
	str[i]=0;
}

bool readini()
{
	int i;
	char buf[300],buf1[300];
	CStdioFile ff;
	SetCurrentDirectory(curdir);
	if(!ff.Open("mTuner.ini",CFile::modeRead|CFile::typeText))
	{
		ff.Open("mTuner.ini",CFile::modeCreate|
				CFile::modeWrite|CFile::modeRead|CFile::typeText);
		for(i=0;i<paran;i++){//writeini
			*ppara[i]=defau[i];
			strcpy(buf,ptext[i]);
			if(strstr(ptext[i],ptext[1])){
				strcpy(buf1,"1.0\n");
				*ppara[i]=10000;
			}
			else sprintf(buf1,"%d\n",*ppara[i]);
			strcat(buf,buf1);
			ff.WriteString(buf);
		}
		ff.Abort();
		return false;
	}
	else{
		while(ff.ReadString(buf,299)){
			for(i=0;i<paran;i++){
				if(strstr(buf,ptext[i])){
					if(strstr(ptext[i],ptext[1])){
						sscanf(&buf[strlen(ptext[i])+2],"%d\n",ppara[i]);
						if(*ppara[i]<1000)*ppara[i]+=10000;
						else if(*ppara[i]<=9000)*ppara[i]=10000;
					}
					else sscanf(&buf[strlen(ptext[i])],"%d\n",ppara[i]);
				}
			}
		}
		ff.Abort();
		return true;
	}
}
void writeini()
{
	CStdioFile ff;
	char buf[300],buf1[300];
	int i;
	ff.Open("mTuner.ini",CFile::modeCreate|
				CFile::modeWrite|CFile::modeRead|CFile::typeText);
	for(i=0;i<paran;i++){//writeini
		strcpy(buf,ptext[i]);
		if(strstr(buf,ptext[1])){
			if(*ppara[i]>=10000)sprintf(buf1,"1.%04d\n",*ppara[i]-10000);
			else sprintf(buf1,"0.%04d\n",*ppara[i]);
		}
		else sprintf(buf1,"%d\n",*ppara[i]);
		strcat(buf,buf1);
		ff.WriteString(buf);
	}
	ff.Abort();
}