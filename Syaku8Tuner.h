// Syaku8Tuner.h : SYAKU8TUNER �A�v���P�[�V�����̃��C�� �w�b�_�[ �t�@�C��
//

#if !defined(AFX_SYAKU8TUNER_H__95109D7F_8303_494D_B27A_4556929E1B09__INCLUDED_)
#define AFX_SYAKU8TUNER_H__95109D7F_8303_494D_B27A_4556929E1B09__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // ���C�� �V���{��

/////////////////////////////////////////////////////////////////////////////
// CSyaku8TunerApp:
// ���̃N���X�̓���̒�`�Ɋւ��Ă� Syaku8Tuner.cpp �t�@�C�����Q�Ƃ��Ă��������B
//

class CSyaku8TunerApp : public CWinApp
{
public:
	CSyaku8TunerApp();

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CSyaku8TunerApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
	//{{AFX_MSG(CSyaku8TunerApp)
	afx_msg void OnAppAbout();
		// ���� - ClassWizard �͂��̈ʒu�Ƀ����o�֐���ǉ��܂��͍폜���܂��B
		//        ���̈ʒu�ɐ��������R�[�h��ҏW���Ȃ��ł��������B
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_SYAKU8TUNER_H__95109D7F_8303_494D_B27A_4556929E1B09__INCLUDED_)
