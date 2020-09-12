// pictureDoc.h : interface of the CPictureDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PICTUREDOC_H__CD397409_8F8F_4F16_90F6_E17C6FED34AC__INCLUDED_)
#define AFX_PICTUREDOC_H__CD397409_8F8F_4F16_90F6_E17C6FED34AC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CPictureDoc : public CDocument
{
protected: // create from serialization only
	CPictureDoc();
	DECLARE_DYNCREATE(CPictureDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPictureDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPictureDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CPictureDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PICTUREDOC_H__CD397409_8F8F_4F16_90F6_E17C6FED34AC__INCLUDED_)
