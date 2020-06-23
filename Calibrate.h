#if !defined(AFX_CALIBRATE_H__75821A63_E437_4397_ACE7_9C1CB14CB03A__INCLUDED_)
#define AFX_CALIBRATE_H__75821A63_E437_4397_ACE7_9C1CB14CB03A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Calibrate.h : ヘッダー ファイル
//
/////////////////////////////////////////////////////////////////////////////
// CCalibrate ダイアログ

class CCalibrate : public CDialog
{
// コンストラクション
public:
	CCalibrate(CWnd* pParent = NULL);   // 標準のコンストラクタ
//	CBitmap m_bmap_calup;
//	CBitmap m_bmap_caldown;
// ダイアログ データ
	//{{AFX_DATA(CCalibrate)
#ifdef English
	enum { IDD = IDD_CALIBRATE_E };
#else
	enum { IDD = IDD_CALIBRATE_J };
#endif
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CCalibrate)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CCalibrate)
	afx_msg void OnRokuonctr();
	afx_msg void OnCaliCali();
	afx_msg void OnResetcali();
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_CALIBRATE_H__75821A63_E437_4397_ACE7_9C1CB14CB03A__INCLUDED_)
