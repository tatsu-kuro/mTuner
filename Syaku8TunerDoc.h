// Syaku8TunerDoc.h : CSyaku8TunerDoc クラスの宣言およびインターフェイスの定義をします。
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_SYAKU8TUNERDOC_H__AF89283D_D9AC_42FF_9722_D7A2D77B6A4D__INCLUDED_)
#define AFX_SYAKU8TUNERDOC_H__AF89283D_D9AC_42FF_9722_D7A2D77B6A4D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CSyaku8TunerDoc : public CDocument
{
protected: // シリアライズ機能のみから作成します。
	CSyaku8TunerDoc();
	DECLARE_DYNCREATE(CSyaku8TunerDoc)

// アトリビュート
public:

// オペレーション
public:

//オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CSyaku8TunerDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	virtual ~CSyaku8TunerDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成されたメッセージ マップ関数
protected:
	//{{AFX_MSG(CSyaku8TunerDoc)
		// メモ - ClassWizard はこの位置にメンバ関数を追加または削除します。
		//        この位置に生成されるコードを編集しないでください。
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_SYAKU8TUNERDOC_H__AF89283D_D9AC_42FF_9722_D7A2D77B6A4D__INCLUDED_)
