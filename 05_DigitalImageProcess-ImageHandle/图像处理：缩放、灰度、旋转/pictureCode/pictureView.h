// pictureView.h : interface of the CPictureView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PICTUREVIEW_H__1A25E3B5_9762_4238_BFC0_E369C55B1F84__INCLUDED_)
#define AFX_PICTUREVIEW_H__1A25E3B5_9762_4238_BFC0_E369C55B1F84__INCLUDED_

#include "DIB.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CPictureView : public CView
{
protected: // create from serialization only
	CPictureView();
	DECLARE_DYNCREATE(CPictureView)

// Attributes
public:
	CPictureDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPictureView)
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
	float value;
	DIB m_dib;
	virtual ~CPictureView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CPictureView)
	afx_msg void OnFileOpen();
	afx_msg void OnFileSave();
	afx_msg void OnRgb2gray();
	afx_msg void OnAppAuthor();
	afx_msg void OnCY422();
	afx_msg void OnContrastIDEC();
	afx_msg void OnBrightnesDEC();
	afx_msg void OnBrightnesINC();
	afx_msg void OnContrastINC();
	afx_msg void OnFanse();
	afx_msg void OnExtractRED();
	afx_msg void OnExtractGREEN();
	afx_msg void OnExtractBLUE();
	afx_msg void OnMirrorUp2Down();
	afx_msg void OnMirrorLeft2Right();
	afx_msg void OnGamaCorrection();
	afx_msg void OnRotation90();
	afx_msg void OnConvert24toGray();
	afx_msg void OnResort1();
	afx_msg void OnResort2();
	afx_msg void OnReset();
	afx_msg void OnScalingNearest();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in pictureView.cpp
inline CPictureDoc* CPictureView::GetDocument()
   { return (CPictureDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PICTUREVIEW_H__1A25E3B5_9762_4238_BFC0_E369C55B1F84__INCLUDED_)
