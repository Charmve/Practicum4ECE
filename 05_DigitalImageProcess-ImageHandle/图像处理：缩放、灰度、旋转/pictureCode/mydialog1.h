#if !defined(AFX_MYDIALOG1_H__A152FC0F_3B10_4E75_A906_CC35523151D4__INCLUDED_)
#define AFX_MYDIALOG1_H__A152FC0F_3B10_4E75_A906_CC35523151D4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// mydialog1.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// mydialog1 dialog

class mydialog1 : public CDialog
{
// Construction
public:
	mydialog1(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(mydialog1)
	enum { IDD = IDD_DIALOG2 };
	float	m_scaling;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(mydialog1)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(mydialog1)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYDIALOG1_H__A152FC0F_3B10_4E75_A906_CC35523151D4__INCLUDED_)
