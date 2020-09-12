// TestMenu20View.h : interface of the CTestMenu20View class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_TESTMENU20VIEW_H__6BF9ADAD_9C12_4970_BD86_0BF47D22DAC9__INCLUDED_)
#define AFX_TESTMENU20VIEW_H__6BF9ADAD_9C12_4970_BD86_0BF47D22DAC9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



class CTestMenu20View : public CView
{
protected: // create from serialization only
	CTestMenu20View();
	DECLARE_DYNCREATE(CTestMenu20View)

// Attributes
public:
	CTestMenu20Doc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTestMenu20View)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTestMenu20View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	void Redraw(CDC* pDC);
	bool m_bUnderline;
	bool m_bItaly;
	bool m_bBold;
	UINT m_nSize;


	CString m_strFontName;

	//{{AFX_MSG(CTestMenu20View)
	afx_msg void OnFontNameAerial();
	afx_msg void OnFontNameRoman();
	afx_msg void OnFontPatternBold();
	afx_msg void OnFontPatternItaly();
	afx_msg void OnFontPatternUnderline();
	afx_msg void OnUpdateFontNameAerial(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFontNameRoman(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFontPatternBold(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFontPatternItaly(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFontPatternUnderline(CCmdUI* pCmdUI);
	afx_msg void OnFontSizeBase(WPARAM wParam,LPARAM lParam);

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in TestMenu20View.cpp
inline CTestMenu20Doc* CTestMenu20View::GetDocument()
   { return (CTestMenu20Doc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTMENU20VIEW_H__6BF9ADAD_9C12_4970_BD86_0BF47D22DAC9__INCLUDED_)
