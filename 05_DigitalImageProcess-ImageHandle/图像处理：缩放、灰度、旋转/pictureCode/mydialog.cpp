// mydialog.cpp : implementation file
//

#include "stdafx.h"
#include "picture.h"
#include "mydialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// mydialog dialog


mydialog::mydialog(CWnd* pParent /*=NULL*/)
	: CDialog(mydialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(mydialog)
	m_value = 0.0f;
	//}}AFX_DATA_INIT
}


void mydialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(mydialog)
	DDX_Text(pDX, IDC_EDIT1, m_value);
	DDV_MinMaxFloat(pDX, m_value, 0.5f, 1.5f);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(mydialog, CDialog)
	//{{AFX_MSG_MAP(mydialog)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// mydialog message handlers
