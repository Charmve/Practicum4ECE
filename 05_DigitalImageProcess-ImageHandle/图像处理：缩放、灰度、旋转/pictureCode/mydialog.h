#if !defined(AFX_MYDIALOG_H__E1FCDEE3_0664_404C_8B95_3B8C9E1818CF__INCLUDED_)
#define AFX_MYDIALOG_H__E1FCDEE3_0664_404C_8B95_3B8C9E1818CF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// mydialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// mydialog dialog

class mydialog : public CDialog
{
// Construction
public:
	mydialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(mydialog)
	enum { IDD = IDD_DIALOG1 };
	float	m_value;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(mydialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(mydialog)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYDIALOG_H__E1FCDEE3_0664_404C_8B95_3B8C9E1818CF__INCLUDED_)
