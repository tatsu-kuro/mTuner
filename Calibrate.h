#if !defined(AFX_CALIBRATE_H__75821A63_E437_4397_ACE7_9C1CB14CB03A__INCLUDED_)
#define AFX_CALIBRATE_H__75821A63_E437_4397_ACE7_9C1CB14CB03A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Calibrate.h : �w�b�_�[ �t�@�C��
//
/////////////////////////////////////////////////////////////////////////////
// CCalibrate �_�C�A���O

class CCalibrate : public CDialog
{
// �R���X�g���N�V����
public:
	CCalibrate(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^
//	CBitmap m_bmap_calup;
//	CBitmap m_bmap_caldown;
// �_�C�A���O �f�[�^
	//{{AFX_DATA(CCalibrate)
#ifdef English
	enum { IDD = IDD_CALIBRATE_E };
#else
	enum { IDD = IDD_CALIBRATE_J };
#endif
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CCalibrate)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
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
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_CALIBRATE_H__75821A63_E437_4397_ACE7_9C1CB14CB03A__INCLUDED_)
