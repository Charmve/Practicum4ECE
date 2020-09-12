#if !defined(AFX_MYDIALOG2_H__977F0406_49D1_481F_BEFE_63BF23D9AFA0__INCLUDED_)
#define AFX_MYDIALOG2_H__977F0406_49D1_481F_BEFE_63BF23D9AFA0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// mydialog2.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// mydialog2 dialog

class mydialog2 : public CDialog
{
// Construction
public:
	mydialog2(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(mydialog2)
	enum { IDD = IDD_DIALOG3 };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(mydialog2)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(mydialog2)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYDIALOG2_H__977F0406_49D1_481F_BEFE_63BF23D9AFA0__INCLUDED_)
