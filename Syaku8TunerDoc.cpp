// Syaku8TunerDoc.cpp : CSyaku8TunerDoc クラスの動作の定義を行います。
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
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
		//        この位置に生成されるコードを編集しないでください。
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSyaku8TunerDoc クラスの構築/消滅

CSyaku8TunerDoc::CSyaku8TunerDoc()
{
	// TODO: この位置に１度だけ呼ばれる構築用のコードを追加してください。

}

CSyaku8TunerDoc::~CSyaku8TunerDoc()
{
}

BOOL CSyaku8TunerDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: この位置に再初期化処理を追加してください。
	// (SDI ドキュメントはこのドキュメントを再利用します。)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CSyaku8TunerDoc シリアライゼーション

void CSyaku8TunerDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: この位置に保存用のコードを追加してください。
	}
	else
	{
		// TODO: この位置に読み込み用のコードを追加してください。
	}
}

/////////////////////////////////////////////////////////////////////////////
// CSyaku8TunerDoc クラスの診断

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
// CSyaku8TunerDoc コマンド
