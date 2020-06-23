#define ZUREX 32
#define TONE1 37//51;//�\���Œ���g��
#define TONE2 110////103;//98;//�ō����g��
#define NYQUIST	11025
#define BUFSZ 8192//DRAWWIDTH �����\���ɑ傫�� 2^n
#define BUFDIV (BUFSZ/1024)//�T���v�����O�̍ۂ�buf������
const double R12=1.05946309;
const double PAI=3.141592653589793238;
#define FRA 1
#define CTR 2
#define SNDSIZE 40000
#define SM 5000
#define DMAX 512
#define MAX_BUFFERS	8// BUFDIV �����\���ɑ傫�� 2^n
typedef	SHORT BUFTYPE;// �ʎq���r�b�g��=16bits/sample
struct WAVE{
	double  hz;
	short  tone;
	short cent;
	short A440;
	unsigned char y[75];
};
struct DATADP{
	short hzcal;
	bool hzf;//Hz disp f
	short cent;
	double hz;
	short tone;//���͉���
	bool f;//disp flag
	short peaklevel;
	unsigned char ynow[256];
	short x0;
	short y0;
	short x1;
	short y1;
	short max;
	short winw;
	short cntx;
	short A440;
};
#ifdef S8TUNERTOP
DATADP DP;
char curdir[300];
double tone[TONE2+2+2];
int waveNow,waveCnt,Lt,BarN;
WAVE wa[SM+20];
bool bWaveOpened = false;
#else
extern DATADP DP;
extern char curdir[];
extern double tone[];
extern int waveNow,waveCnt,Lt,BarN;
extern WAVE wa[SM+20];
extern bool	bWaveOpened;// = false;
#endif
int Chehz(double d[]);
void CloseWaveInputDevice(void);
void DispMeterData(CDC *MemDC,CDC *pDC);
void DispZureall(CDC* MemDC,CDC* pDC);
void HidariMigi(int n);
void initsincos();
bool OpenWaveInputDevice(HWND);
void printabc(CDC *MemDC,CDC *pDC,char *p,int x,int y,int f,int mode=0);
bool readini();
double Setf(double a,double b);
double Setsoukan();
void Setwavehz();
void StartRecording(void);
void writeini();

