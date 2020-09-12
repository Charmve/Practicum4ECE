// TestMenu20.h : main header file for the TESTMENU20 application
//

#if !defined(AFX_TESTMENU20_H__3BF549F6_25B7_495A_AD26_CADAFCEC5D44__INCLUDED_)
#define AFX_TESTMENU20_H__3BF549F6_25B7_495A_AD26_CADAFCEC5D44__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CTestMenu20App:
// See TestMenu20.cpp for the implementation of this class
//

class CTestMenu20App : public CWinApp
{
public:
	CTestMenu20App();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTestMenu20App)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CTestMenu20App)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTMENU20_H__3BF549F6_25B7_495A_AD26_CADAFCEC5D44__INCLUDED_)
