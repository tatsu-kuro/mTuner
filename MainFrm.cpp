// MainFrm.cpp : CMainFrame クラスの動作の定義を行います。
//

#include "stdafx.h"
#include "Syaku8Tuner.h"
#include "MainFrm.h"
#include "s8header.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMainFrame クラスの構築/消滅

CMainFrame::CMainFrame()
{
	// TODO: この位置にメンバの初期化処理コードを追加してください。
	
}

CMainFrame::~CMainFrame()
{
}


/////////////////////////////////////////////////////////////////////////////
// CMainFrame クラスの診断

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame メッセージ ハンドラ

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	HICON hIcon=AfxGetApp()->LoadIcon(IDI_ICON1);
	SetIcon(hIcon,FALSE);
	return 0;
}

void CMainFrame::OnDestroy() 
{
	WINDOWPLACEMENT wndPlace;
	wndPlace.length = sizeof(WINDOWPLACEMENT);
	GetWindowPlacement(&wndPlace);
	DP.x0=wndPlace.rcNormalPosition.left;
	DP.y0=wndPlace.rcNormalPosition.top;
	DP.x1=wndPlace.rcNormalPosition.right;
	DP.y1=wndPlace.rcNormalPosition.bottom;
	DP.max=(wndPlace.showCmd==SW_SHOWMAXIMIZED ? 1:0);
	CFrameWnd::OnDestroy();
}
