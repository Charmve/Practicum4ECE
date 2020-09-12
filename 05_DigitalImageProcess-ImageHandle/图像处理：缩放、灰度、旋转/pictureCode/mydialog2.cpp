// mydialog2.cpp : implementation file
//

#include "stdafx.h"
#include "picture.h"
#include "mydialog2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// mydialog2 dialog


mydialog2::mydialog2(CWnd* pParent /*=NULL*/)
	: CDialog(mydialog2::IDD, pParent)
{
	//{{AFX_DATA_INIT(mydialog2)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void mydialog2::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(mydialog2)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(mydialog2, CDialog)
	//{{AFX_MSG_MAP(mydialog2)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// mydialog2 message handlers
