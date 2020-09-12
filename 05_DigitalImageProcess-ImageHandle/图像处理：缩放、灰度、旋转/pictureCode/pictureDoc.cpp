// pictureDoc.cpp : implementation of the CPictureDoc class
//

#include "stdafx.h"
#include "picture.h"

#include "pictureDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPictureDoc

IMPLEMENT_DYNCREATE(CPictureDoc, CDocument)

BEGIN_MESSAGE_MAP(CPictureDoc, CDocument)
	//{{AFX_MSG_MAP(CPictureDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPictureDoc construction/destruction

CPictureDoc::CPictureDoc()
{
	// TODO: add one-time construction code here

}

CPictureDoc::~CPictureDoc()
{
}

BOOL CPictureDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CPictureDoc serialization

void CPictureDoc::Serialize(CArchive& ar)
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
// CPictureDoc diagnostics

#ifdef _DEBUG
void CPictureDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CPictureDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CPictureDoc commands
