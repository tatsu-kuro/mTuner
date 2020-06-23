// Syaku8TunerView.h : CSyaku8TunerView �N���X�̐錾����уC���^�[�t�F�C�X�̒�`�����܂��B
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_SYAKU8TUNERVIEW_H__9187C898_12FD_4AF0_9638_BC2B0AE96CE2__INCLUDED_)
#define AFX_SYAKU8TUNERVIEW_H__9187C898_12FD_4AF0_9638_BC2B0AE96CE2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
class CSyaku8TunerView : public CFormView
{
protected: // �V���A���C�Y�@�\�݂̂���쐬���܂��B
	CSyaku8TunerView();
	DECLARE_DYNCREATE(CSyaku8TunerView)

public:
	//{{AFX_DATA(CSyaku8TunerView)
	enum { IDD = IDD_SYAKU8TUNER_FORM };
	CSpinButtonCtrl	m_spin440;
	CStatic	m_a440;
	CStatic	m_cnt;
	CButton	m_stop;
	CStatic	m_wave;
	CStatic	m_herz;
	CStatic	m_inabc;
	//}}AFX_DATA

// �A�g���r���[�g
public:
	CSyaku8TunerDoc* GetDocument();
// �I�y���[�V����
public:
void SetCtrl();
void DispIroiro(CDC *MemDC,CDC *pDC,int DN);
void SetButtons();
// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CSyaku8TunerView)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �̃T�|�[�g
	virtual void OnInitialUpdate(); // �\�z��̍ŏ��̂P�x�����Ăяo����܂��B
	virtual void OnDraw(CDC* pDC);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
public:
	CBitmap m_bitmap_main;
	CBitmap m_bitmap_left10;
	CBitmap m_bitmap_left1;
	CBitmap m_bitmap_migi10;
	CBitmap m_bitmap_migi1;
	CBitmap m_bitmap_start;
	CBitmap m_bitmap_stop;
	CBitmap m_bitmap_migi_a;
	CBitmap m_bitmap_left_a;
	CBitmap md_bitmap_left10;
	CBitmap md_bitmap_left1;
	CBitmap md_bitmap_migi10;
	CBitmap md_bitmap_migi1;
	CBitmap md_bitmap_start;
	CBitmap md_bitmap_stop;
	CBitmap md_bitmap_migi_a;
	CBitmap md_bitmap_left_a;
	virtual ~CSyaku8TunerView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// �������ꂽ���b�Z�[�W �}�b�v�֐�
protected:
	//{{AFX_MSG(CSyaku8TunerView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnStop();
	afx_msg void OnReturn();
	afx_msg void OnDown();
	afx_msg void OnMigi();
	afx_msg void OnUp();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnShiftreturn();
	afx_msg void OnHidari();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnDeltaposSpin440(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG
inline CSyaku8TunerDoc* CSyaku8TunerView::GetDocument()
   { return (CSyaku8TunerDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_SYAKU8TUNERVIEW_H__9187C898_12FD_4AF0_9638_BC2B0AE96CE2__INCLUDED_)
