// Syaku8TunerDoc.cpp : CSyaku8TunerDoc �N���X�̓���̒�`���s���܂��B
//

#include "stdafx.h"
#include "Syaku8Tuner.h"

#include "Syaku8TunerDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSyaku8TunerDoc

IMPLEMENT_DYNCREATE(CSyaku8TunerDoc, CDocument)

BEGIN_MESSAGE_MAP(CSyaku8TunerDoc, CDocument)
	//{{AFX_MSG_MAP(CSyaku8TunerDoc)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ}�b�s���O�p�̃}�N����ǉ��܂��͍폜���܂��B
		//        ���̈ʒu�ɐ��������R�[�h��ҏW���Ȃ��ł��������B
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSyaku8TunerDoc �N���X�̍\�z/����

CSyaku8TunerDoc::CSyaku8TunerDoc()
{
	// TODO: ���̈ʒu�ɂP�x�����Ă΂��\�z�p�̃R�[�h��ǉ����Ă��������B

}

CSyaku8TunerDoc::~CSyaku8TunerDoc()
{
}

BOOL CSyaku8TunerDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: ���̈ʒu�ɍď�����������ǉ����Ă��������B
	// (SDI �h�L�������g�͂��̃h�L�������g���ė��p���܂��B)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CSyaku8TunerDoc �V���A���C�[�[�V����

void CSyaku8TunerDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: ���̈ʒu�ɕۑ��p�̃R�[�h��ǉ����Ă��������B
	}
	else
	{
		// TODO: ���̈ʒu�ɓǂݍ��ݗp�̃R�[�h��ǉ����Ă��������B
	}
}

/////////////////////////////////////////////////////////////////////////////
// CSyaku8TunerDoc �N���X�̐f�f

#ifdef _DEBUG
void CSyaku8TunerDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CSyaku8TunerDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CSyaku8TunerDoc �R�}���h
