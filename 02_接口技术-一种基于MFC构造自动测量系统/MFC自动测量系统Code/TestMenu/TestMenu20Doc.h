// TestMenu20Doc.h : interface of the CTestMenu20Doc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_TESTMENU20DOC_H__47EF31F6_09BE_4876_BC05_267BF53B7FD9__INCLUDED_)
#define AFX_TESTMENU20DOC_H__47EF31F6_09BE_4876_BC05_267BF53B7FD9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CTestMenu20Doc : public CDocument
{
protected: // create from serialization only
	CTestMenu20Doc();
	DECLARE_DYNCREATE(CTestMenu20Doc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTestMenu20Doc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTestMenu20Doc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CTestMenu20Doc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTMENU20DOC_H__47EF31F6_09BE_4876_BC05_267BF53B7FD9__INCLUDED_)
