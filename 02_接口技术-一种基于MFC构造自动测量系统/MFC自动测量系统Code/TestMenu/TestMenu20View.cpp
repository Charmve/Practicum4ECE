// TestMenu20View.cpp : implementation of the CTestMenu20View class
//

#include "stdafx.h"
#include "TestMenu20.h"

#include "TestMenu20Doc.h"
#include "TestMenu20View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTestMenu20View


IMPLEMENT_DYNCREATE(CTestMenu20View, CView)

BEGIN_MESSAGE_MAP(CTestMenu20View, CView)
	//{{AFX_MSG_MAP(CTestMenu20View)   	
	ON_COMMAND(ID_FONT_NAME_AERIAL, OnFontNameAerial)
	ON_COMMAND(ID_FONT_NAME_ROMAN, OnFontNameRoman)
	ON_COMMAND(ID_FONT_PATTERN_BOLD, OnFontPatternBold)
	ON_COMMAND(ID_FONT_PATTERN_ITALY, OnFontPatternItaly)
	ON_COMMAND(ID_FONT_PATTERN_UNDERLINE, OnFontPatternUnderline)
	ON_UPDATE_COMMAND_UI(ID_FONT_NAME_AERIAL, OnUpdateFontNameAerial)
	ON_UPDATE_COMMAND_UI(ID_FONT_NAME_ROMAN, OnUpdateFontNameRoman)
	ON_UPDATE_COMMAND_UI(ID_FONT_PATTERN_BOLD, OnUpdateFontPatternBold)
	ON_UPDATE_COMMAND_UI(ID_FONT_PATTERN_ITALY, OnUpdateFontPatternItaly)
	ON_UPDATE_COMMAND_UI(ID_FONT_PATTERN_UNDERLINE, OnUpdateFontPatternUnderline)
	//ON_COMMAND(ID_FONT_SIZE_BASE, OnFontSizeBase)
	ON_COMMAND_RANGE(ID_FONT_SIZE_BASE,ID_FONT_SIZE_BASE+6,OnFontSizeBase)

	//}}AFX_MSG_MAP
	// Standard printing commands	

	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)

END_MESSAGE_MAP()

    
/////////////////////////////////////////////////////////////////////////////
// CTestMenu20View construction/destruction

CTestMenu20View::CTestMenu20View()
{
	// TODO: add construction code here
	
	m_nSize=20;
	m_strFontName="Aerial";
	m_bBold=false;
	m_bItaly=false;
	m_bUnderline=false;

}

CTestMenu20View::~CTestMenu20View()
{
}

BOOL CTestMenu20View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CTestMenu20View drawing

void CTestMenu20View::OnDraw(CDC* pDC)
{
	CTestMenu20Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
		Redraw(pDC);
}

/////////////////////////////////////////////////////////////////////////////
// CTestMenu20View printing

BOOL CTestMenu20View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CTestMenu20View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CTestMenu20View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CTestMenu20View diagnostics

#ifdef _DEBUG
void CTestMenu20View::AssertValid() const
{
	CView::AssertValid();
}

void CTestMenu20View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CTestMenu20Doc* CTestMenu20View::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTestMenu20Doc)));
	return (CTestMenu20Doc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTestMenu20View message handlers

void CTestMenu20View::OnFontNameAerial() 
{
	// TODO: Add your command handler code here
		// TODO: Add your command handler code here
	if(m_strFontName!="Aerial")
	{
		m_strFontName="Aerial";
		Invalidate(TRUE);
	}	
	
}

void CTestMenu20View::OnFontNameRoman() 
{
	// TODO: Add your command handler code here
		if(m_strFontName!="Times New Roman")
	{
		m_strFontName="Times New Roman";
		Invalidate(TRUE);
	}	
	
}

void CTestMenu20View::OnFontPatternBold() 
{
	// TODO: Add your command handler code here
		m_bBold=!m_bBold;
	Invalidate(TRUE);
}

void CTestMenu20View::OnFontPatternItaly() 
{
	// TODO: Add your command handler code here
		m_bItaly=!m_bItaly;
	Invalidate(TRUE);
}

void CTestMenu20View::OnFontPatternUnderline() 
{
	// TODO: Add your command handler code here
		m_bUnderline=!m_bUnderline;
	Invalidate(TRUE);
}

void CTestMenu20View::OnFontSizeBase(WPARAM wParam,LPARAM lParam)
{
	if(m_nSize!=((wParam-ID_FONT_SIZE_BASE)*4+16))
	{
		m_nSize=(wParam-ID_FONT_SIZE_BASE)*4+16;
		Invalidate(TRUE);
	}
}
void CTestMenu20View::OnUpdateFontNameAerial(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
		if(m_strFontName=="Aerial")
		pCmdUI->SetRadio(1);
	else
		pCmdUI->SetRadio(0);
	
}

void CTestMenu20View::OnUpdateFontNameRoman(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
		if(m_strFontName=="Times New Roman")
		pCmdUI->SetRadio(1);
	else
		pCmdUI->SetRadio(0);
}

void CTestMenu20View::OnUpdateFontPatternBold(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
		if(m_bBold==true)
		pCmdUI->SetCheck(1);
	else
		pCmdUI->SetCheck(0);
}

void CTestMenu20View::OnUpdateFontPatternItaly(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
		if(m_bItaly==true)
		pCmdUI->SetCheck(1);
	else
		pCmdUI->SetCheck(0);
}

void CTestMenu20View::OnUpdateFontPatternUnderline(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
		if(m_bUnderline==true)
		pCmdUI->SetCheck(1);
	else
		pCmdUI->SetCheck(0);
}



void CTestMenu20View::Redraw(CDC *pDC)
{
	CFont* pOldFont;
	CFont* pNewFont=new CFont;
	pNewFont->CreateFont(m_nSize,0,0,0,(m_bBold==true?FW_BOLD:FW_NORMAL),
						m_bItaly,m_bUnderline,0,
						ANSI_CHARSET,OUT_DEFAULT_PRECIS,
						CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,
						DEFAULT_PITCH&FF_SWISS,	m_strFontName);
	pOldFont=(CFont*)pDC->SelectObject(pNewFont);
	pDC->TextOut(40,40,"单文档用户界面和菜单的使用");
	pDC->SelectObject(pOldFont);

	delete pNewFont;
	return;	
}

