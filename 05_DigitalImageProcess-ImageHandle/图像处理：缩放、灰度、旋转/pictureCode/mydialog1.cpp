// mydialog1.cpp : implementation file
//

#include "stdafx.h"
#include "picture.h"
#include "mydialog1.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// mydialog1 dialog


mydialog1::mydialog1(CWnd* pParent /*=NULL*/)
	: CDialog(mydialog1::IDD, pParent)
{
	//{{AFX_DATA_INIT(mydialog1)
	m_scaling = 0.0f;
	//}}AFX_DATA_INIT
}


void mydialog1::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(mydialog1)
	DDX_Text(pDX, IDC_EDIT2, m_scaling);
	DDV_MinMaxFloat(pDX, m_scaling, 0.f, 10.f);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(mydialog1, CDialog)
	//{{AFX_MSG_MAP(mydialog1)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// mydialog1 message handlers
