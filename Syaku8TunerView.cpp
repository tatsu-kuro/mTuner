// Syaku8TunerView.cpp : CSyaku8TunerView クラスの動作の定義を行います。
//
#include "stdafx.h"
#include "Syaku8Tuner.h"
//#include "Calibrate.h"

#include "Syaku8TunerDoc.h"
#include "Syaku8TunerView.h"
#include "math.h"
#include <mmsystem.h>
#define S8TUNERTOP
#include "s8header.h"
extern HWAVEIN			hWave;
extern BUFTYPE			*wavebuf;
extern LPWAVEHDR		wh[MAX_BUFFERS];
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
void DispButton(CDC *pDC,CBitmap *bit);
DWORD grayRGB;
void MakeWave(double f0);
static unsigned char snddata[10000+50];
int ZUREY1=85;//五線譜
int ZUREY=171;//CENT表示
int BarY=236;

/////////////////////////////////////////////////////////////////////////////
// CSyaku8TunerView

IMPLEMENT_DYNCREATE(CSyaku8TunerView, CFormView)

BEGIN_MESSAGE_MAP(CSyaku8TunerView, CFormView)
	//{{AFX_MSG_MAP(CSyaku8TunerView)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_STOP, OnStop)
	ON_COMMAND(ID_RETURN, OnReturn)
	ON_COMMAND(ID_DOWN, OnDown)
	ON_COMMAND(ID_MIGI, OnMigi)
	ON_COMMAND(ID_UP, OnUp)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_COMMAND(ID_SHIFTRETURN, OnShiftreturn)
	ON_COMMAND(ID_HIDARI, OnHidari)
	ON_WM_SIZE()
	ON_WM_MOUSEWHEEL()
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN440, OnDeltaposSpin440)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSyaku8TunerView クラスの構築/消滅

CSyaku8TunerView::CSyaku8TunerView()
	: CFormView(CSyaku8TunerView::IDD)
{
	//{{AFX_DATA_INIT(CSyaku8TunerView)
	//}}AFX_DATA_INIT
	// TODO: この場所に構築用のコードを追加してください。
}

CSyaku8TunerView::~CSyaku8TunerView()
{
}

void CSyaku8TunerView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSyaku8TunerView)
	DDX_Control(pDX, IDC_SPIN440, m_spin440);
	DDX_Control(pDX, IDC_A440BIT, m_a440);
	DDX_Control(pDX, IDC_CNT, m_cnt);
	DDX_Control(pDX, IDC_STOP, m_stop);
	DDX_Control(pDX, IDC_WAVE, m_wave);
	DDX_Control(pDX, IDC_HERZ, m_herz);
	DDX_Control(pDX, IDC_INABC, m_inabc);
	//}}AFX_DATA_MAP
}

BOOL CSyaku8TunerView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: この位置で CREATESTRUCT cs を修正して Window クラスまたはスタイルを
	//  修正してください。

	return CFormView::PreCreateWindow(cs);
}
void DispButton(CDC *pDC,CBitmap *bit)//システムのボタンの背景色をセットする。
{
	int x,y;
	CDC MemDC;
	CBitmap *pOldBmp;
	MemDC.CreateCompatibleDC(pDC);
	pOldBmp=MemDC.SelectObject(bit);
	for(x=0;x<150;x++){
		for(y=0;y<50;y++){
			if(RGB(255,255,255)==MemDC.GetPixel(x,y))
				MemDC.SetPixel(x,y,grayRGB);
		}
	}
	MemDC.SelectObject(pOldBmp);
	DeleteDC(MemDC);
}
void CSyaku8TunerView::SetButtons(){
	if(DP.f){
		m_stop.SetBitmap( m_bitmap_stop);
		m_spin440.EnableWindow(true);
	}else{
		m_stop.SetBitmap( m_bitmap_start);
		m_spin440.EnableWindow(false);

	}
}

void CSyaku8TunerView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	//ビットマップのロード
	BarN=100;
	m_bitmap_main.LoadBitmap(IDB_BITMAP);
	m_bitmap_migi_a.LoadBitmap(IDB_MIGI_A); 
	m_bitmap_left_a.LoadBitmap(IDB_LEFT_A); 
	md_bitmap_migi_a.LoadBitmap(IDB_MIGI_A); 
	md_bitmap_left_a.LoadBitmap(IDB_LEFT_A); 
	m_bitmap_stop.LoadBitmap(IDB_STOP); 
	m_bitmap_start.LoadBitmap(IDB_START); 

	m_bitmap_migi1.LoadBitmap(IDB_MIGI1); 
	m_bitmap_migi10.LoadBitmap(IDB_MIGI10);
	m_bitmap_left1.LoadBitmap(IDB_LEFT1); 
	m_bitmap_left10.LoadBitmap(IDB_LEFT10); 
	md_bitmap_migi1.LoadBitmap(IDB_MIGI1); 
	md_bitmap_migi10.LoadBitmap(IDB_MIGI10);
	md_bitmap_left1.LoadBitmap(IDB_LEFT1); 
	md_bitmap_left10.LoadBitmap(IDB_LEFT10); 

	grayRGB=GetSysColor(COLOR_BTNFACE);//システムのボタンの背景色をセットする。
	CDC* pDC=GetDC();
	DispButton(pDC,&m_bitmap_migi_a);
	DispButton(pDC,&m_bitmap_left_a);
	DispButton(pDC,&m_bitmap_stop);
	DispButton(pDC,&m_bitmap_start);
	DispButton(pDC,&m_bitmap_migi1);
	DispButton(pDC,&m_bitmap_migi10);
	DispButton(pDC,&m_bitmap_left1);
	DispButton(pDC,&m_bitmap_left10);
	ReleaseDC(pDC);
	// ボタンにビットマップをセットします。
	DP.f=true;
	SetButtons();
	readini();
	CRect rect;
	rect.left=DP.x0;
	rect.top=DP.y0;
	rect.right=DP.x1;
	rect.bottom=DP.y1;
	CWnd* pW=GetParent( ); // 親ウィンドウ(主ウィンドウ)のポインタを得ます。
	pW->MoveWindow( &rect ); // 指定した座標のウィンドウを描きます。

/*	sscanf(ptxt[WINXY],"%d,%d,%d,%d,%d",&rect.left,&rect.top,&rect.right,&rect.bottom,&maxF);
	if(maxF==1)AfxGetMainWnd()->PostMessage(WM_SYSCOMMAND, SC_MAXIMIZE);
*/
	if(DP.max==1)AfxGetMainWnd()->PostMessage(WM_SYSCOMMAND, SC_MAXIMIZE);
	SetCtrl();
//	SetCtrl();
}

/////////////////////////////////////////////////////////////////////////////
// CSyaku8TunerView クラスの診断

#ifdef _DEBUG
void CSyaku8TunerView::AssertValid() const
{
	CFormView::AssertValid();
}

void CSyaku8TunerView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CSyaku8TunerDoc* CSyaku8TunerView::GetDocument() // 非デバッグ バージョンはインラインです。
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSyaku8TunerDoc)));
	return (CSyaku8TunerDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CSyaku8TunerView クラスのメッセージ ハンドラ
void DrawInit()
{
	int i;
	for(i=0;i<SM;i++){
		wa[i].hz=440;
		wa[i].tone=69;
	}
	initsincos();
	tone[33]=double(440)/2/2/2;
	for(i=33;i<TONE2+2;i++)tone[i+1]=R12*tone[i];
}
char *cde[14]={"C","D","D","E","E","F","G","G","A","A","B","B"};
char flat[14]={ 0,  1,  0,  1,  0,  0,  1,  0,  1,  0,  1,  0};
void DrawRect(CDC* pDC,int x,int y,int dx,int dy,short mode)
{
	DWORD rg1,rg2,rg3,rg4,rg5;
	if(mode==1){rg1=RGB(155,155,155);rg2=RGB(90,90,90);rg3=RGB(155,155,155);rg4=RGB(255,255,255);rg5=RGB(255,255,255);}
//	else {rg1=RGB(200,200,200);rg2=RGB(240,240,240);rg3=RGB(240,240,240);rg4=RGB(255,255,255);rg5=RGB(240,240,240);}
	else {rg1=RGB(150,150,150);rg2=RGB(200,200,200);rg3=RGB(150,150,150);rg4=RGB(200,200,200);rg5=RGB(250,240,240);}

	pDC->FillSolidRect(x,y,dx+1,dy+1,rg5);//back
	pDC->FillSolidRect(x,y,dx+2,1,rg1);//yoko1
	pDC->FillSolidRect(x+1,y+1,dx,1,rg2);//yoko2
	pDC->FillSolidRect(x,y+1,1,dy+1,rg1);//tate1
	pDC->FillSolidRect(x+1,y+1,1,dy,rg2);//tate2
	pDC->FillSolidRect(x+1,y+dy+1,dx+1,1,rg3);//yoko3
	pDC->FillSolidRect(x+1,y+dy+2,dx+2,1,rg4);//yoko4
	pDC->FillSolidRect(x+dx+1,y+1,1,dy,rg3);//tate3
	pDC->FillSolidRect(x+dx+2,y+1,1,dy+1,rg4);//tate4
}
void DrawBarRect(CDC* pDC,int x,int y,int dx,int dy)
{
	DWORD rg1,rg2,rg3,rg4,rg5,rg6;
	rg1=RGB(90,90,90);
	rg2=RGB(150,150,150);
	rg3=RGB(155,155,155);
	rg4=RGB(200,200,200);
	rg5=RGB(250,250,240);
	rg6=RGB(255,255,255);

	pDC->FillSolidRect(x,y,dx+1+1,dy+1+1,rg5);//back
	pDC->FillSolidRect(x,y,dx+3,1,rg1);//yoko1 soto
	pDC->FillSolidRect(x+1,y+1,dx,1,rg2);//yoko2 uti
	pDC->FillSolidRect(x,y+1-1,1,dy+3,rg1);//tate1 soto
	pDC->FillSolidRect(x+1,y+1,1,dy,rg2);//tate2 uti
	pDC->FillSolidRect(x+1,y+dy+1,dx+1,1,rg2);//yoko3 uti
	pDC->FillSolidRect(x+1,y+dy+2,dx+2,1,rg1);//yoko4 soto
	pDC->FillSolidRect(x+dx+1,y+1,1,dy,rg2);//tate3 uti
	pDC->FillSolidRect(x+dx+2,y+1,1,dy+1,rg1);//tate4 soto
}

void CSyaku8TunerView::DispIroiro(CDC *MemDC,CDC *pDC,int DN)
{
	int i;
	char buf[100];
	CDC *pDC1;
	if(DN&FRA){
		pDC->FillSolidRect(0,0,DP.winw,BarY,RGB(255,255,255));
		pDC->FillSolidRect(0,0,DP.winw,ZUREY1-90,grayRGB);
		pDC->FillSolidRect(0,ZUREY1-90,DP.winw,1,RGB(200,200,200));
		DispZureall(MemDC,pDC);
		//表示枠下のバー表示
		int x,len,Lt1,Lt2,L=BarN*7;//,redx;
		if(DP.f)Lt1=(waveCnt-BarN+SM)%SM;
		else Lt1=Lt;
		pDC->FillSolidRect(0,BarY,DP.winw+2,20+7,RGB(255,255,255));//grayRGB);
		pDC->FillSolidRect(0,BarY+16+7,DP.winw+2,1,RGB(200,200,200));
		DrawBarRect(pDC,32,BarY-2,DP.winw-37,11+4);
		if(waveCnt>BarN){
			if(waveCnt<SM){
				len=L*BarN/waveCnt;
				x=(L-len)*Lt1/(waveCnt-BarN);
			}else{
				len=L*BarN/SM;
				Lt2=(SM+Lt1-waveCnt%SM)%SM;
				x=(L-len)*Lt2/(SM-BarN);
			}
			if(waveCnt<SM-10)pDC->FillSolidRect(ZUREX+11+x,BarY+1,len,8+4,RGB(100,100,100));
			else pDC->FillSolidRect(ZUREX+11+x,BarY+1,len,8+4,RGB(100,100,100));
		}
	}
	if(DN&CTR){//wave
		pDC1=m_wave.GetDC();
		DrawRect(pDC1,0,0,75,29,1);
		if(waveNow>0)for(i=0;i<74;i++)pDC1->FillSolidRect(i+2,2+wa[waveNow-1].y[i]/10,1,1,RGB(2,2,2));
		m_wave.ReleaseDC(pDC1);
	
		pDC1=m_cnt.GetDC();//count
		DrawRect(pDC1,0,0,41+20,29,1);
		if(DP.f){
			if(waveCnt<SM){
				sprintf(buf,"%03d.%01dq",waveNow/10,waveNow%10);
				printabc(MemDC,pDC1,buf,5,10,SRCAND);
			}else{
				sprintf(buf,"%03d.%01dq",(SM-1)/10,waveNow%10);
				printabc(MemDC,pDC1,buf,5,10,SRCAND);
			}
		}else{
				sprintf(buf,"%03d.%01dq",waveNow/10,waveNow%10);
				printabc(MemDC,pDC1,buf,5,10,SRCAND);
		}
		m_cnt.ReleaseDC(pDC1);

		pDC1=m_a440.GetDC();//pitch A
		DrawRect(pDC1,0,0,47,29,1);
		if(DP.f)sprintf(buf,"A:%03d",DP.A440);
		else sprintf(buf,"A:%03d",wa[waveNow].A440);
		printabc(MemDC,pDC1,buf,6,10,SRCAND);
		m_a440.ReleaseDC(pDC1);

		pDC1=m_herz.GetDC();//Hz
		double Hz=wa[waveNow].hz;
		if(Hz>9999.0||Hz<=0.00)Hz=0;
		DrawRect(pDC1,0,0,74,29,1);
		strcpy(buf,"000");
		if(Hz>999.9)sprintf(buf,"%4.1f Hz",Hz);
		else if(Hz>99.9)sprintf(&buf[1],"%4.1f Hz",Hz);
		else if(Hz>9.9)sprintf(&buf[2],"%4.1f Hz",Hz);
		else strcpy(buf,"0000.0 Hz");
		printabc(MemDC,pDC1,buf,9,10,SRCCOPY);
		m_herz.ReleaseDC(pDC1);
	
		pDC1=m_inabc.GetDC();//CDEFGABCG b 4
		DrawRect(pDC1,0,0,39,29,1);
		printabc(MemDC,pDC1,cde[(wa[waveNow].tone)%12],11,10,SRCCOPY);
		if(wa[waveNow].tone/12<1)strcpy(buf,"0");
		else sprintf(buf,"%d",wa[waveNow].tone/12-1);
		printabc(MemDC,pDC1,buf,25,10,SRCCOPY);
		if(flat[wa[waveNow].tone%12]==1)pDC1->BitBlt(19,7,5,10,MemDC,24,0,SRCCOPY);//♭
		m_inabc.ReleaseDC(pDC1);
	}
}
void CSyaku8TunerView::OnDraw(CDC* pDC) 
{
	CDC MemDC,dcMem;
	static bool initf=false;
	CBitmap *pOldBmp,*pOldBmp1,bmpMem;
	CRect rc;
	//メモリーDC
	GetClientRect(&rc);

	dcMem.CreateCompatibleDC(pDC);
	bmpMem.CreateCompatibleBitmap(pDC,rc.right,rc.bottom);
	pOldBmp1=dcMem.SelectObject(&bmpMem);
	//ここまで
	MemDC.CreateCompatibleDC(pDC);
	pOldBmp=MemDC.SelectObject(&m_bitmap_main);
	DispIroiro(&MemDC,&dcMem,FRA);
	pDC->BitBlt(0,0,rc.Width(),BarY+17+7/*rc.Height()-50*/,&dcMem,0,0,SRCCOPY);
	DispIroiro(&MemDC,pDC,CTR);
	MemDC.SelectObject(pOldBmp);
	dcMem.SelectObject(pOldBmp1);
	if(initf==false){
		initf=true;
#ifdef English
		AfxGetMainWnd()->SetWindowText("msTuner( making shakuhachi tuner)  :  Version 1.12");
#else
		AfxGetMainWnd()->SetWindowText(" 尺八調律チューナー  :  Version 1.12 / www.shaku6.com");
#endif
	}
}

int CSyaku8TunerView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CFormView::OnCreate(lpCreateStruct) == -1)
		return -1;
	int i;
	GetCurrentDirectory(300,curdir);
	DrawInit();
	DP.f=true;
	// サンプリングバッファを確保
	wavebuf = (BUFTYPE *) GlobalAlloc( GPTR, BUFSZ * sizeof(BUFTYPE) );
	for(i=0; i<MAX_BUFFERS; i++) wh[i] = NULL;
	for(i=0;i<256;i++)DP.ynow[i]=128;
	waveCnt=0;
	// 開始
	if(!bWaveOpened) {
		if(bWaveOpened = OpenWaveInputDevice(m_hWnd))
			StartRecording();
	}
	return 0;
}
LRESULT CSyaku8TunerView::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	switch(message)
	{
		case WIM_DATA:
			MSG		 msg;
			MMRESULT rc;
			if(wavebuf != NULL && bWaveOpened) {
				int i,j;
				SYSTEMTIME st;
				int sec;
				static int oldsec=0;
				GetLocalTime(&st);
				for(i=0; i<MAX_BUFFERS; i++) {
					if(((LPWAVEHDR)lParam)->lpData == wh[i]->lpData) break;
				}
				for(j=0; j<BUFDIV; j++) {
					CopyMemory(wavebuf+BUFSZ/BUFDIV*j,
						   wh[(i-(BUFDIV-1)+j)&(MAX_BUFFERS-1)]->lpData,
						   BUFSZ/BUFDIV*sizeof(BUFTYPE));
				}
				// データバッファブロックを再利用する
				rc = waveInPrepareHeader(hWave, (LPWAVEHDR)lParam, sizeof(WAVEHDR));
				if (rc == MMSYSERR_NOERROR)
					rc = waveInAddBuffer(hWave, (LPWAVEHDR)lParam, sizeof(WAVEHDR));
				if (rc != MMSYSERR_NOERROR) {
					CloseWaveInputDevice();
				}
				// 描画処理
				sec=st.wMilliseconds/100;//DP.interval;//100でないと可笑しいところも出てくるかもしれないが・・・
				if(oldsec!=sec&&DP.f){
					waveCnt++;
					Setwavehz();
					Invalidate(false);
				}
				if(oldsec!=sec)oldsec=sec;
				// 描画処理中に溜まった描画要求を削除する
				while( PeekMessage(&msg,m_hWnd, WIM_DATA, WIM_DATA, PM_REMOVE) ) {
					// データバッファブロックを再利用する
					rc = waveInPrepareHeader(hWave, (LPWAVEHDR)msg.lParam, sizeof(WAVEHDR));
					if (rc == MMSYSERR_NOERROR)
						rc = waveInAddBuffer(hWave, (LPWAVEHDR)msg.lParam, sizeof(WAVEHDR));
					if (rc != MMSYSERR_NOERROR) {
						CloseWaveInputDevice();
						break;
					}
				}
			}
			break;
	}	
	return CFormView::WindowProc(message, wParam, lParam);
}

void CSyaku8TunerView::OnDestroy() 
{
	CFormView::OnDestroy();
	SetCurrentDirectory(curdir);
	if(bWaveOpened)CloseWaveInputDevice();
	// 確保していたメモリを開放
	GlobalFree( wavebuf );
	writeini();
}
WAVE watemp[SM+20];

void CSyaku8TunerView::OnStop() 
{
	if(!DP.f){
		DP.f=true;
		SetButtons();
	}
	else {
		int i,j;
		DP.f=false;
		SetButtons();
		////stopした時にデータがSMを超えていたら、wa[0]から並べ替える
		if(waveCnt>=SM){
			for(i=0;i<SM;i++){
				watemp[i].hz=wa[i].hz;
				watemp[i].tone=wa[i].tone;
				watemp[i].cent=wa[i].cent;
				for(j=0;j<75;j++)watemp[i].y[j]=wa[i].y[j];
			}
			for(i=0;i<SM;i++){
				wa[i].hz=watemp[(waveCnt+i)%SM].hz;
				wa[i].tone=watemp[(waveCnt+i)%SM].tone;
				wa[i].cent=watemp[(waveCnt+i)%SM].cent;
				for(j=0;j<75;j++)wa[i].y[j]=watemp[(waveCnt+i)%SM].y[j];
			}
			waveNow=waveCnt=SM-1;
		}
		waveNow=waveCnt%SM;
		if(waveCnt<BarN)Lt=0;
		else Lt=(waveNow-BarN+1+SM)%SM;
	}
	Invalidate(false);
}	

void CSyaku8TunerView::OnShiftreturn() 
{
	waveNow=waveCnt=Lt=0;
	if(DP.f)DP.f=false;
	else DP.f=true;
	OnStop();
}

void CSyaku8TunerView::OnReturn() 
{
	OnStop();	
}

void CSyaku8TunerView::OnDown() 
{
	if(!DP.f){
		HidariMigi(-BarN);
		Invalidate(false);
	}
}

void CSyaku8TunerView::OnHidari() 
{
	if(!DP.f){
		HidariMigi(-1);
		Invalidate(false);
	}
}

void CSyaku8TunerView::OnMigi() 
{
	if(!DP.f){
		HidariMigi(1);
		Invalidate(false);
	}
}

void CSyaku8TunerView::OnUp() 
{
	if(!DP.f){
		HidariMigi(BarN);
		Invalidate(false);
	}
}

/*void CSyaku8TunerView::OnSoundcard() 
{
	CCalibrate dlg;
	DP.f=true;
	SetButtons();
	DP.A440=440;
	MakeWave(440.0);
	PlaySound((char*)snddata,NULL,SND_MEMORY|SND_ASYNC|SND_LOOP);
	if(dlg.DoModal()==IDOK){

	}
	PlaySound(NULL,NULL,SND_PURGE);
}
*/
void CSyaku8TunerView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if(!DP.f&&point.y<(BarY-15)){
		int pointx;
		if(point.x<44)pointx=44;
		else if(point.x>BarN*7+45)pointx=BarN*7+45;
		else pointx=point.x;
		HidariMigi((pointx-47)/7-(waveNow-Lt));
		Invalidate(false);
	}else if(!DP.f&&point.y>=(BarY-15)&&waveCnt>BarN){
		if(point.x<44)HidariMigi(-SM);
		else if(point.x>BarN*7+45)HidariMigi(SM);
		else{
			double ctd;//waveNowからのずれ
			if(waveCnt<SM){
				ctd=double(point.x-44)/double(BarN)/7.0*double(waveCnt)-double(waveNow);
				HidariMigi((int)ctd);
			}else{
				ctd=double(point.x-44)/double(BarN)/7.0*double(SM)-double(waveNow);
				HidariMigi((int)ctd);
			}
		}
		Invalidate(false);
	}
	CFormView::OnLButtonDown(nFlags, point);
}
void CSyaku8TunerView::OnMouseMove(UINT nFlags, CPoint point) 
{
	if(!DP.f&&nFlags&&point.y<BarY-15){//223){
//		if(point.x<44)HidariMigi(-20);
//		else if(point.x>BarN*7+45)HidariMigi(20);
//		else{
		if(point.x>43&&(point.x<BarN*7+46)){
			HidariMigi((point.x-47)/7-(waveNow-Lt));
		}
		Invalidate(false);
	}else if(!DP.f&&nFlags&&point.y>=(BarY-15)&&waveCnt>BarN){
		if(point.x<44)HidariMigi(-SM);
		else if(point.x>BarN*7+45)HidariMigi(SM);
		else{
			double ctd;//waveNowからのずれ
			if(waveCnt<SM){
				ctd=double(point.x-44)/double(BarN)/7.0*double(waveCnt)-double(waveNow);
				HidariMigi((int)ctd);
			}else{
				ctd=double(point.x-44)/double(BarN)/7.0*double(SM)-double(waveNow);
				HidariMigi((int)ctd);
			}
		}
		Invalidate(false);
	}
	CFormView::OnMouseMove(nFlags, point);
}
void setA440(int a)
{
	DP.A440=a;
	if(DP.A440>470)DP.A440=470;
	else if(DP.A440<375)DP.A440=375;
}
BOOL SetClientSize(HWND hWnd, int width, int height)
{
	RECT rw, rc;
	::GetWindowRect(hWnd, &rw);
	::GetClientRect(hWnd, &rc);

	int new_width = (rw.right - rw.left) - (rc.right - rc.left) + width;
	int new_height = (rw.bottom - rw.top) - (rc.bottom - rc.top) + height;

	return ::SetWindowPos(hWnd, NULL, 0, 0, new_width, new_height, SWP_NOMOVE | SWP_NOZORDER);
}
//int ZUREY1= 85;//五線譜
//int ZUREY= 171;//CENT表示
//int BarY=236;

#define setctrl(ct,x,y,dx,dy);{rect.left=x;rect.right=x+dx,rect.top=y;rect.bottom=y+dy;ct.MoveWindow(&rect,TRUE);}
void CSyaku8TunerView::SetCtrl()
{
	CRect rect;
	int CtrlY,cx,stpt,hei;
	CtrlY=250+20;
	GetClientRect(&rect);
	cx=rect.right;
	hei=rect.bottom-rect.top;
	if(hei>CtrlY+30){
		CtrlY+=(hei-CtrlY-30)/2;
	}
	BarY=CtrlY-15-8-8;
	ZUREY1=BarY-(236-85);//CtrlY-151;
	ZUREY=BarY-(236-171);//CtrlY-57;
	stpt=cx/2+358;
	if(stpt<569)stpt=569;
	setctrl(m_a440    ,stpt-32*3-90-32-66-50-77-78-85,CtrlY,20,32);
	setctrl(m_spin440 ,stpt-32*3-90-32-66-50-77-78-35,CtrlY,16,32);
	setctrl(m_wave ,stpt-32*3-90-32-66-50-77-78-10,CtrlY,32,32);
	setctrl(m_herz ,stpt-32*3-90-32-66-50-77   -10,CtrlY,32,32);
	setctrl(m_inabc,stpt-32*3-90-32-66-50      -10,CtrlY,32,32);
	setctrl(m_stop,  stpt-80-32*3-90-30+2,CtrlY,80,32);
	setctrl(m_cnt,stpt-205,CtrlY,145,38);
	BarN=(cx-55)/7;
	DP.winw=cx;
//	CWnd* pW=GetParent( );
//	SetClientSize(HWND(pW),600,350);
//	::SetWindowPos(m_hWnd, NULL, 0, 0, 500,300, SWP_NOMOVE | SWP_NOZORDER);
//	rect={ 0, 0, 640, 480 };
//::AdjustWindowRectEx( &rect, WS_OVERLAPPEDWINDOW, FALSE, 0 );
//::SetWindowPos(NULL, NULL, 0, 0, rect.right-rect.left, rect.bottom-rect.top, SWP_NOMOVE|SWP_NOZORDER );

}
void CSyaku8TunerView::OnSize(UINT nType, int cx, int cy) 
{
	CFormView::OnSize(nType, cx, cy);
	if(::IsWindow(m_stop.m_hWnd)){
		SetCtrl();
	}
}

BOOL CSyaku8TunerView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	if(!DP.f){
		if(zDelta>0)HidariMigi(20);
		else HidariMigi(-20);
		Invalidate(false);
	}	
	return CFormView::OnMouseWheel(nFlags, zDelta, pt);
}

typedef struct
{
	int fs; // 標本化周波数 //
	int bits; // 量子化精度 //
	int length; // 音データの長さ //
	double *s; // 音データ //
} MONO_PCM;

void mwrite(void* buffer, int size, int count);
void mwrite(void* buffer, int size, int count)
//void mwrite(const void* buffer, int size, int count);
//void mwrite(const void* buffer, int size, int count)
{
	int i;
	unsigned char* ucp;
	static int pt;
	if(size==0){
		pt=0;
		return;
	}
	ucp=(unsigned char*)buffer;
	for(i=0;i<size*count;i++)snddata[pt++]=ucp[i];
}
void mono_wave_write(MONO_PCM *pcm)
{
//  FILE *fp;
	int n;
	char riff_chunk_ID[4];
	long riff_chunk_size;
	char riff_form_type[4];
	char fmt_chunk_ID[4];
	long fmt_chunk_size;
	short fmt_wave_format_type;
	short fmt_channel;
	long fmt_samples_per_sec;
	long fmt_bytes_per_sec;
	short fmt_block_size;
	short fmt_bits_per_sample;
	char data_chunk_ID[4];
	long data_chunk_size;
	short data;
	double s;
  
	riff_chunk_ID[0] = 'R';
	riff_chunk_ID[1] = 'I';
	riff_chunk_ID[2] = 'F';
	riff_chunk_ID[3] = 'F';
	riff_chunk_size = 36 + pcm->length * 2;
	riff_form_type[0] = 'W';
	riff_form_type[1] = 'A';
	riff_form_type[2] = 'V';
	riff_form_type[3] = 'E';
  
	fmt_chunk_ID[0] = 'f';
	fmt_chunk_ID[1] = 'm';
	fmt_chunk_ID[2] = 't';
	fmt_chunk_ID[3] = ' ';
	fmt_chunk_size = 16;
	fmt_wave_format_type = 1;
	fmt_channel = 1;
	fmt_samples_per_sec = pcm->fs; /* 標本化周波数 */
	fmt_bytes_per_sec = pcm->fs * pcm->bits / 8;
	fmt_block_size = pcm->bits / 8;
	fmt_bits_per_sample = pcm->bits; /* 量子化精度 */
  
	data_chunk_ID[0] = 'd';
	data_chunk_ID[1] = 'a';
	data_chunk_ID[2] = 't';
	data_chunk_ID[3] = 'a';
	data_chunk_size = pcm->length * 2;
	/*  
	fp = fopen(file_name, "wb");
  
	fwrite(riff_chunk_ID, 1, 4, fp);
	fwrite(&riff_chunk_size, 4, 1, fp);
	fwrite(riff_form_type, 1, 4, fp);
	fwrite(fmt_chunk_ID, 1, 4, fp);
	fwrite(&fmt_chunk_size, 4, 1, fp);
	fwrite(&fmt_wave_format_type, 2, 1, fp);
	fwrite(&fmt_channel, 2, 1, fp);
	fwrite(&fmt_samples_per_sec, 4, 1, fp);
	fwrite(&fmt_bytes_per_sec, 4, 1, fp);
	fwrite(&fmt_block_size, 2, 1, fp);
	fwrite(&fmt_bits_per_sample, 2, 1, fp);
	fwrite(data_chunk_ID, 1, 4, fp);
	fwrite(&data_chunk_size, 4, 1, fp);
	*/
	mwrite(riff_chunk_ID, 0, 0);//reset pointer

	mwrite(riff_chunk_ID, 1, 4);
	mwrite(&riff_chunk_size, 4, 1);
	mwrite(riff_form_type, 1, 4);
	mwrite(fmt_chunk_ID, 1, 4);
	mwrite(&fmt_chunk_size, 4, 1);
	mwrite(&fmt_wave_format_type, 2, 1);
	mwrite(&fmt_channel, 2, 1);
	mwrite(&fmt_samples_per_sec, 4, 1);
	mwrite(&fmt_bytes_per_sec, 4, 1);
	mwrite(&fmt_block_size, 2, 1);
	mwrite(&fmt_bits_per_sample, 2, 1);
	mwrite(data_chunk_ID, 1, 4);
	mwrite(&data_chunk_size, 4, 1);
  
	for (n = 0; n < pcm->length; n++)
	{
		s = (pcm->s[n] + 1.0) / 2.0 * 65536.0;
    
		if (s > 65535.0)
		{
		  s = 65535.0; /* クリッピング */
		}
		else if (s < 0.0)
		{
		  s = 0.0; /* クリッピング */
		}
    
		data = (short)(s + 0.5) - 32768; /* 四捨五入とオフセットの調節 */
	//  fwrite(&data, 2, 1, fp); /* 音データの書き出し */
		mwrite(&data, 2, 1);
	}
	//  fclose(fp);
}
void MakeWave(double f0) 
{
	MONO_PCM pcm;
	int n;
	double A;//, f0;
  
	pcm.fs = 8000;// 標本化周波数/
	pcm.bits = 16; // 量子化精度 
	pcm.length = 5000; // 音データの長さ //
	pcm.s =(double*) calloc(pcm.length, sizeof(double)); // メモリの確保 
  
	A = 0.8; // 振幅 //
//	f0 = 440.0; // 基本周波数 //
  
	// サイン波 //
	for (n = 0; n < pcm.length; n++)
	{
		pcm.s[n] = A * sin(2.0 * 3.141592653589793238 * f0 * n / pcm.fs);
	}
  
	mono_wave_write(&pcm); // WAVE配列にモノラルの音データを出力する //
  
	free(pcm.s); // メモリの解放 //
}

void CSyaku8TunerView::OnDeltaposSpin440(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	if(DP.f){
		if(pNMUpDown->iDelta > 0)setA440(DP.A440-1);
		else setA440(DP.A440+1);
	}
	*pResult = 0;
}
