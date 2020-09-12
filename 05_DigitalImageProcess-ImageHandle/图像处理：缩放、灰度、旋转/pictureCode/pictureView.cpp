// pictureView.cpp : implementation of the CPictureView class
//

#include "stdafx.h"
#include "picture.h"

#include "pictureDoc.h"
#include "pictureView.h"
#include "mydialog.h"
#include "mydialog1.h"
#include "mydialog2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPictureView

CString fname;

IMPLEMENT_DYNCREATE(CPictureView, CView)

BEGIN_MESSAGE_MAP(CPictureView, CView)
	//{{AFX_MSG_MAP(CPictureView)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_COMMAND(ID_FILE_SAVE, OnFileSave)
	ON_COMMAND(ID_RGB2GRAY, OnRgb2gray)
	ON_COMMAND(ID_APP_AUTHOR, OnAppAuthor)
	ON_COMMAND(ID_CY422, OnCY422)
	ON_COMMAND(ID_ContrastIDEC, OnContrastIDEC)
	ON_COMMAND(ID_BrightnesDEC, OnBrightnesDEC)
	ON_COMMAND(ID_BrightnesINC, OnBrightnesINC)
	ON_COMMAND(ID_ContrastINC, OnContrastINC)
	ON_COMMAND(ID_FANSE, OnFanse)
	ON_COMMAND(ID_ExtractRED, OnExtractRED)
	ON_COMMAND(ID_ExtractGREEN, OnExtractGREEN)
	ON_COMMAND(ID_ExtractBLUE, OnExtractBLUE)
	ON_COMMAND(ID_MirrorUp2Down, OnMirrorUp2Down)
	ON_COMMAND(ID_MirrorLeft2Right, OnMirrorLeft2Right)
	ON_COMMAND(ID_GamaCorrection, OnGamaCorrection)
	ON_COMMAND(ID_Rotation90, OnRotation90)
	ON_COMMAND(ID_Convert24toGray, OnConvert24toGray)
	ON_COMMAND(ID_Resort2, OnResort2)
	ON_COMMAND(ID_RESET, OnReset)
	ON_COMMAND(ID_ScalingNearest, OnScalingNearest)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPictureView construction/destruction

CPictureView::CPictureView()
{
	// TODO: add construction code here
	//MessageBox("This is my first VC++ program!","软件用户使用提示");
	m_dib.UserTip();
	//if(m_dib.UserTip())
     //Invalidate(TRUE);
}

CPictureView::~CPictureView()
{
}

BOOL CPictureView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CPictureView drawing

void CPictureView::OnDraw(CDC* pDC)
{
	CPictureDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
	if(m_dib.m_pBMI==NULL||m_dib.m_pDIBData==NULL)
		return;
	CRect rect;
	GetClientRect(rect);
	m_dib.ShowDIB(pDC,0,0,rect.Width(),rect.Height());
}

/////////////////////////////////////////////////////////////////////////////
// CPictureView printing

BOOL CPictureView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CPictureView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CPictureView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CPictureView diagnostics

#ifdef _DEBUG
void CPictureView::AssertValid() const
{
	CView::AssertValid();
}

void CPictureView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CPictureDoc* CPictureView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CPictureDoc)));
	return (CPictureDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CPictureView message handlers

void CPictureView::OnFileOpen() 
{
	// TODO: Add your command handler code here
	CString filename;
	static char szFilter[]="BMP文件(*.bmp)|*.bmp|所有文件(*.*)|*.*||";
	CFileDialog dlg(TRUE,"bmp",NULL,
		             OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,szFilter);
	if(dlg.DoModal()==IDOK)
	{
		filename=dlg.GetPathName();
		fname = filename;
		if(!m_dib.LoadFromFile(filename))
			return;
		Invalidate(TRUE);
	}
}

void CPictureView::OnFileSave() 
{
	// TODO: Add your command handler code here
	if(m_dib.m_pBMI==NULL||m_dib.m_pDIBData==NULL)
	{
		AfxMessageBox("没有图像资源");
		return;
	}
	CString filename;
	static char szFilter[]="BMP文件(*.bmp)|*.bmp|所有文件(*.*)|*.*||";
	CFileDialog dlg(TRUE,"bmp",NULL,
		             OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,szFilter);
	if(dlg.DoModal()==IDOK)
	{
		filename=dlg.GetPathName();
		if(!m_dib.SaveFile(filename))
			return;
	}
}


void CPictureView::OnRgb2gray() 
{
	// TODO: Add your command handler code here
	if(m_dib.m_pBMI==NULL||m_dib.m_pDIBData==NULL)
	{
		AfxMessageBox("没有图像资源");return;
	}
	if(m_dib.RGB2GRAY())
     Invalidate(TRUE); 
}

void CPictureView::OnAppAuthor()
{
	// TODO: Add your command handler code here

	// MessageBox("This is my first VC++ program!","hello");
	AfxMessageBox(" 姓名：张伟\n 学号：160901137 \n\n 春节快乐！！！", MB_OK|MB_ICONINFORMATION);   //用法参考链接：https://blog.csdn.net/qq1041256623/article/details/17321853
}

void CPictureView::OnCY422() 
{
	// TODO: Add your command handler code here
	if(m_dib.m_pBMI==NULL||m_dib.m_pDIBData==NULL)
	{
		AfxMessageBox("没有图像资源");return;
	}
	if(m_dib.CY422())
     Invalidate(TRUE);
}

void CPictureView::OnFanse()
{
	// TODO: Add your command handler code here
	if(m_dib.m_pBMI==NULL||m_dib.m_pDIBData==NULL)
	{
		AfxMessageBox("没有图像资源");return;
	}
	if(m_dib.Fanse())
     Invalidate(TRUE);
}

void CPictureView::OnBrightnesDEC() 
{
	// TODO: Add your command handler code here
	if(m_dib.m_pBMI==NULL||m_dib.m_pDIBData==NULL)
	{
		AfxMessageBox("没有图像资源");return;
	}
	if(m_dib.BrightnesDEC())
     Invalidate(TRUE);
}

void CPictureView::OnBrightnesINC() 
{
	// TODO: Add your command handler code here
	if(m_dib.m_pBMI==NULL||m_dib.m_pDIBData==NULL)
	{
		AfxMessageBox("没有图像资源");return;
	}
	if(m_dib.BrightnesINC())
     Invalidate(TRUE);
}

void CPictureView::OnContrastIDEC() 
{
	// TODO: Add your command handler code here
	if(m_dib.m_pBMI==NULL||m_dib.m_pDIBData==NULL)
	{
		AfxMessageBox("没有图像资源");return;
	}
	if(m_dib.ContrastIDEC())
     Invalidate(TRUE);
}

void CPictureView::OnContrastINC() 
{
	// TODO: Add your command handler code here
	if(m_dib.m_pBMI==NULL||m_dib.m_pDIBData==NULL)
	{
		AfxMessageBox("没有图像资源");return;
	}
	if(m_dib.ContrastINC())
     Invalidate(TRUE);
}



void CPictureView::OnExtractRED() 
{
	// TODO: Add your command handler code here
	if(m_dib.m_pBMI==NULL||m_dib.m_pDIBData==NULL)
	{
		AfxMessageBox("没有图像资源");return;
	}
	if(m_dib.ExtractRED())
     Invalidate(TRUE);
}

void CPictureView::OnExtractGREEN() 
{
	// TODO: Add your command handler code here
	if(m_dib.m_pBMI==NULL||m_dib.m_pDIBData==NULL)
	{
		AfxMessageBox("没有图像资源");return;
	}
	if(m_dib.ExtractGREEN())
     Invalidate(TRUE);
}

void CPictureView::OnExtractBLUE() 
{
	// TODO: Add your command handler code here
	if(m_dib.m_pBMI==NULL||m_dib.m_pDIBData==NULL)
	{
		AfxMessageBox("没有图像资源");return;
	}
	if(m_dib.ExtractBLUE())
     Invalidate(TRUE);
}

void CPictureView::OnMirrorUp2Down() 
{
	// TODO: Add your command handler code here
	if(m_dib.m_pBMI==NULL||m_dib.m_pDIBData==NULL)
	{
		AfxMessageBox("没有图像资源");return;
	}
	if(m_dib.MirrorUp2Down())
     Invalidate(TRUE);	
}

void CPictureView::OnMirrorLeft2Right() 
{
	// TODO: Add your command handler code here
	if(m_dib.m_pBMI==NULL||m_dib.m_pDIBData==NULL)
	{
		AfxMessageBox("没有图像资源");return;
	}
	if(m_dib.MirrorLeft2Right())
     Invalidate(TRUE);	
}

void CPictureView::OnRotation90() //图像右旋转90度
{
	// TODO: Add your command handler code here
	if(m_dib.m_pBMI==NULL||m_dib.m_pDIBData==NULL)
	{
		AfxMessageBox("没有图像资源");return;
	}
	if(m_dib.Rotation90())
     Invalidate(TRUE);
	//m_dib.BmpRotate(CBitmap* cBmp, 45.0);
}

void CPictureView::OnGamaCorrection() //图像伽马校正
{
	// TODO: Add your command handler code here
	if(m_dib.m_pBMI==NULL||m_dib.m_pDIBData==NULL)
	{
		AfxMessageBox("没有图像资源");return;
	}

	if(m_dib.GamaCorrection())
     Invalidate(TRUE);	
}

void CPictureView::OnConvert24toGray() //同时可以用保存按钮存盘 ?
{
	// TODO: Add your command handler code here
	if(m_dib.m_pBMI==NULL||m_dib.m_pDIBData==NULL)
	{
		AfxMessageBox("没有图像资源");return;
	}
	if(m_dib.Convert24toGray())
     Invalidate(TRUE);	
}

void CPictureView::OnResort2() //图像还原方法二
{
	// TODO: Add your command handler code here
	 CString filename; 
	 static char szFilter[]="BMP 文件(*.bmp)|*.bmp|所有文件(*.*)|*.*||"; 
	 CFileDialog dlg(TRUE,"bmp",NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,szFilter); 
	 if(dlg.DoModal()==IDOK) 
	 { 
		 filename=dlg.GetPathName(); 
		 fname=dlg.GetPathName();//原始图像备份 
		 if(!m_dib.LoadFromFile(filename)) 
			 return; 
		 Invalidate(TRUE); 
	} 	
}


void CPictureView::OnReset() //还原
{
	// TODO: Add your command handler code here
	if(m_dib.m_pBMI==NULL||m_dib.m_pDIBData==NULL) 
	{ 
		AfxMessageBox("没有图像资源"); 
		return; 
	} 
	if(!m_dib.LoadFromFile(fname))//载入内存中的原始图片 
		return; 
	Invalidate(TRUE);
}

void CPictureView::OnScalingNearest() 
{
	// TODO: Add your command handler code here
	if(m_dib.m_pBMI==NULL||m_dib.m_pDIBData==NULL) 
	{ 
		AfxMessageBox("没有图像资源"); 
		return; 
	} 

	if(!m_dib.ScalingNearest()) 
		return; 
	Invalidate(TRUE);
}
