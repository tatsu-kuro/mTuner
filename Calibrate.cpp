// Calibrate.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "Syaku8Tuner.h"
//#include "Syaku8TunerView.h"
#include "Calibrate.h"
#include "s8header.h"
#define TONE2 110
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CCalibrate ダイアログ


CCalibrate::CCalibrate(CWnd* pParent /*=NULL*/)
	: CDialog(CCalibrate::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCalibrate)
	//}}AFX_DATA_INIT
}


void CCalibrate::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCalibrate)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCalibrate, CDialog)
	//{{AFX_MSG_MAP(CCalibrate)
	ON_BN_CLICKED(IDC_CALI_CALI, OnCaliCali)
	ON_BN_CLICKED(IDC_RESETCALI, OnResetcali)
	ON_WM_MOUSEWHEEL()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCalibrate メッセージ ハンドラ
/*
void CCalibrate::OnRokuonctr() 
{
	ShellExecute( NULL, NULL, TEXT("SNDVOL32.EXE"), TEXT("/RECORD"), NULL, SW_SHOWNORMAL );
}*/

void CCalibrate::OnCaliCali() 
{
	DP.hzcal=10000;
	SetTimer(1,200,NULL);
}

void CCalibrate::OnResetcali() 
{
	DP.hzcal=10000;
}

BOOL CCalibrate::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	if(zDelta>0)DP.hzcal+=1;
	else DP.hzcal-=1;
	return CDialog::OnMouseWheel(nFlags, zDelta, pt);
}

void CCalibrate::OnTimer(UINT nIDEvent) 
{
	if(nIDEvent==1){
		KillTimer(1);
		DP.hzcal=int(tone[DP.tone]/DP.hz*10000);
		if(DP.hzcal>11000)DP.hzcal=11000;
		else if(DP.hzcal<9000)DP.hzcal=9000;
	}
	CDialog::OnTimer(nIDEvent);
}
