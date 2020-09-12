// TestMenu20Doc.cpp : implementation of the CTestMenu20Doc class
//

#include "stdafx.h"
#include "TestMenu20.h"

#include "TestMenu20Doc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTestMenu20Doc

IMPLEMENT_DYNCREATE(CTestMenu20Doc, CDocument)

BEGIN_MESSAGE_MAP(CTestMenu20Doc, CDocument)
	//{{AFX_MSG_MAP(CTestMenu20Doc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTestMenu20Doc construction/destruction

CTestMenu20Doc::CTestMenu20Doc()
{
	// TODO: add one-time construction code here

}

CTestMenu20Doc::~CTestMenu20Doc()
{
}

BOOL CTestMenu20Doc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CTestMenu20Doc serialization

void CTestMenu20Doc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CTestMenu20Doc diagnostics

#ifdef _DEBUG
void CTestMenu20Doc::AssertValid() const
{
	CDocument::AssertValid();
}

void CTestMenu20Doc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTestMenu20Doc commands
