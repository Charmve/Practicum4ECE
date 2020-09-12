// DIB.h: interface for the DIB class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DIB_H__AE681D90_C41D_4075_BFAF_22AF658F1676__INCLUDED_)
#define AFX_DIB_H__AE681D90_C41D_4075_BFAF_22AF658F1676__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class DIB : public CObject  
{
public:
	BOOL UserTip();
	CBitmap* BmpRotate(CBitmap* cBmp, float Angle);
	DWORD GetDIBSize();
	BOOL ScalingNearest();
	BOOL Resort1();
	BOOL Convert24toGray();
	BOOL GamaCorrection();
	BOOL Rotation90();
	BOOL MirrorUp2Down();
	BOOL MirrorLeft2Right();
	BOOL ExtractBLUE();
	BOOL ExtractGREEN();
	BOOL ExtractRED();
	BOOL Fanse();
	BOOL BrightnesDEC();
	BOOL BrightnesINC();
	BOOL ContrastIDEC();
	BOOL ContrastINC();
	BOOL CY422();
	BOOL RGB2GRAY();
	BOOL SaveFile(const CString filename);
	virtual void ShowDIB(CDC *pDC,int nLeft,int nTop,int nWidth,int nHeight);
	virtual BOOL LoadFromFile(LPCTSTR lpszFileName);
	int GetBmpRealWidth();
	int GetDIBHeight();
	int GetDIBWidth();
	BITMAPINFO * m_pBMI;
	BITMAPINFOHEADER bih;
	BITMAPFILEHEADER bfh;
	BYTE * m_pDIBData;
	BYTE * n_pDIBData;
	DIB();
	virtual ~DIB();

};

#endif // !defined(AFX_DIB_H__AE681D90_C41D_4075_BFAF_22AF658F1676__INCLUDED_)
