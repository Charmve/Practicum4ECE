// DIB.cpp: implementation of the DIB class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "picture.h"
#include "DIB.h"
#include "math.h"
#include "mydialog.h"
#include "mydialog1.h"
#include "mydialog2.h"

#define WIDTHBYTES(bits) ((bits+31)/32*4)
#define Cnumber(x,y,z) *(m_pDIBData+(x)*GetBmpRealWidth()+(y)*3+(z))
//用Cnumber(x,y,z)代表从下到上，从左到右的第x行，第y列，第z个颜色分量。

#define Cnumber1(x,y,z) *(n_pDIBData+(x)*GetBmpRealWidth()+(y)*3+(z))//用于备份真彩图 
#define gray(x,y) *(m_pDIBData+(x)*GetBmpRealWidth()+y) 
#define gray1(x,y) *(n_pDIBData+(x)*GetBmpRealWidth()+y )//用于备份灰度图 


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

DIB::DIB()
{
	m_pBMI=NULL;
    m_pDIBData=NULL;
}

DIB::~DIB()
{
	if(m_pBMI!=NULL)
		delete m_pBMI;
	if(m_pDIBData!=NULL)
		delete m_pDIBData;
}

int DIB::GetDIBWidth()
{
	return m_pBMI->bmiHeader.biWidth;
}

int DIB::GetDIBHeight()
{
	return m_pBMI->bmiHeader.biHeight;
}

DWORD DIB::GetDIBSize()
{
	return m_pBMI->bmiHeader.biSize;
}

int DIB::GetBmpRealWidth()
{
	return (bfh.bfSize-bfh.bfOffBits)/GetDIBHeight();
}

BOOL DIB::LoadFromFile(LPCTSTR lpszFileName)
{
	CFile file;
	BITMAPINFO* pBMI=NULL;
	BYTE* pDIBData=NULL;

	if(!file.Open(lpszFileName,CFile::modeRead|CFile::typeBinary))
	{
		AfxMessageBox("打不开文件");
		return FALSE;
	}
	
	if(file.Read(&bfh,sizeof(bfh))!=sizeof(bfh))
	{
		AfxMessageBox("读文件出错");
		return FALSE;
	}
	if(bfh.bfType!=0x4d42)
	{
		AfxMessageBox("不是BMP文件");
		return FALSE;
	}
	if(file.Read(&bih,sizeof(bih))!=sizeof(bih))
	{
		AfxMessageBox("读文件出错");
		return FALSE;
	}

	if(bih.biBitCount==24)
	{
		pBMI=(BITMAPINFO*)new char[sizeof(BITMAPINFOHEADER)];
	    if(!pBMI)
		{
		    AfxMessageBox("分配内存出错");
		    return FALSE;
		}
	    memcpy(pBMI,&bih,sizeof(BITMAPINFOHEADER));
	}
	else if(bih.biBitCount==8)
	{
        pBMI=(BITMAPINFO*)new char[sizeof(BITMAPINFOHEADER)+sizeof(RGBQUAD) * 256];
		memcpy(&pBMI->bmiHeader,&bih,sizeof(BITMAPINFOHEADER));
		file.Read(pBMI->bmiColors,sizeof(RGBQUAD) * 256);
	}
	else
	{
		AfxMessageBox("不是8位灰度图或24位真彩色图");
		return FALSE;
	}
	
	DWORD dataBytes=bfh.bfSize-bfh.bfOffBits;

	pDIBData=(BYTE*)new char[dataBytes];
	if(!pDIBData)
	{
		AfxMessageBox("分配内存出错");
		delete pBMI;
		return FALSE;
	}
     if(file.ReadHuge(pDIBData,dataBytes)!=dataBytes)
	{
		AfxMessageBox("分配内存出错");
		delete pBMI;
		delete pDIBData;
		return FALSE;
	}
    file.Close();
	if(m_pBMI!=NULL)
		delete m_pBMI;
	m_pBMI=pBMI;
	if(m_pDIBData!=NULL)
		delete m_pDIBData;
	m_pDIBData=pDIBData; 

	return true; 

}  


void DIB::ShowDIB(CDC *pDC, int nLeft, int nTop, int nWidth, int nHeight)
{
	int w,h;
	pDC->SetStretchBltMode(COLORONCOLOR);
    
	if(nWidth*GetDIBHeight()>=nHeight*GetDIBWidth())//为了按图像原比例显示，调整显示区域大小
	{
		w=nHeight*GetDIBWidth()/GetDIBHeight();
		h=nHeight;
	}
	else
	{
		w=nWidth;
		h=nWidth*GetDIBHeight()/GetDIBWidth();
	}
	StretchDIBits(pDC->GetSafeHdc(),
		         nLeft,nTop,w,h,
				 0,0,GetDIBWidth(),GetDIBHeight(),m_pDIBData,
				 m_pBMI,DIB_RGB_COLORS,SRCCOPY);
}

BOOL DIB::SaveFile(const CString filename)
{
	CFile file(filename,CFile::modeWrite|CFile::modeCreate);
	file.Write(&bfh,sizeof(bfh));
	if(bih.biBitCount==24)
	{
		file.Write(&bih,sizeof(bih));
	}
	else
	{
		file.Write(&m_pBMI->bmiHeader,sizeof(BITMAPINFOHEADER));
		file.Write(m_pBMI->bmiColors,sizeof(RGBQUAD)*256);
	}
	file.Write(m_pDIBData, bfh.bfSize-bfh.bfOffBits);
	return TRUE;
}

BOOL DIB::RGB2GRAY()
{
    int gray;
	int height = GetDIBHeight(); //列
    int width = GetDIBWidth(); //行

	if(bih.biBitCount!=24)
	{
		AfxMessageBox("原图不是24位真彩图");
		return false;
	}  

   else
   {
	   for (int x = 0; x < height; x++) //行
		   for (int y = 0; y < width; y++) //列
		   {
				gray=(int)(Cnumber(x,y,0)*0.299+Cnumber(x,y,1)*0.587+Cnumber(x,y,2)*0.114); //亮度方程
				Cnumber(x,y,0)=gray;Cnumber(x,y,1)=gray;Cnumber(x,y,2)=gray; //使其各个像素相等			
		   }
   }
   return true;
}

BOOL DIB::CY422() //彩色图像子采样 
{
	int x,y; 
	double Y,Cr,Cb,i,j,k; 
	if(bih.biBitCount!=24) 
	{
		AfxMessageBox("原图不是24位真彩图"); 
		return false; 
	} 
	
	else 
	{ 
		for(x=0;x<GetDIBHeight();x++) 
		{ 
			for(y=1;y<GetDIBWidth();) 
			{ 
				Y=0.299*Cnumber(x,y-1,2)+0.587*Cnumber(x,y-1,1)+0.114*Cnumber(x,y-1,0); 
						//首先将像素变换带 YCrCb, 
						//然后将相邻 
						//像素相等，再反变换到RGB 
				Cb=0.564*(Cnumber(x,y-1,0)-Y); 
				Cr=0.713*(Cnumber(x,y-1,2)-Y); 
				Y=0.299*Cnumber(x,y,2)+0.587*Cnumber(x,y,1)+0.114*Cnumber(x,y,0); 
				i=(Y+1.402*Cr); 
				j=(Y-0.344*Cb-0.714*Cr); 
				k=(Y+1.772*Cb);
				
				if(i>255) i=255; 
				else if(i<0) i=0; 
				if(j>255) j=255; 
				else if(j<0) j=0; 
				if(k>255) k=255; 
				else if(k<0) k=0; 

				Cnumber(x,y,2)=(BYTE)i; 
				Cnumber(x,y,1)=(BYTE)j; 
				Cnumber(x,y,0)=(BYTE)k; 

				y=y+2; 
			} 
		} 
	} 
	return true; 
}

BOOL DIB::Fanse()//反色变换 
{
	int x,y; 
	BYTE i; 
	if(bih.biBitCount==24)
	{ 
		for(x=0;x<GetDIBHeight();x++) 
		{ 
			for(y=0;y<GetDIBWidth();y++) 
			{ 
				//像素的彩色值变为其反色
				Cnumber(x,y,0)=255-Cnumber(x,y,0); 
				Cnumber(x,y,1)=255-Cnumber(x,y,1); 
				Cnumber(x,y,2)=255-Cnumber(x,y,2);

				if(Cnumber(x,y,0)>255) Cnumber(x,y,0)=255; 
				else if(Cnumber(x,y,0)<0) Cnumber(x,y,0)=0; 
				if(Cnumber(x,y,1)>255) Cnumber(x,y,1)=255; 
				else if(Cnumber(x,y,1)<0) Cnumber(x,y,1)=0; 
				if(Cnumber(x,y,2)>255) Cnumber(x,y,2)=255; 
				else if(Cnumber(x,y,2)<0) Cnumber(x,y,2)=0; 
			} 
		} 
		return true; 
	}

	
	else if(bih.biBitCount==8) 
	{ 
		for(x=0;x<GetDIBHeight();x++) 
		{ 
			for(y=0;y<GetDIBWidth();y++) 
			{ 
				gray(x,y)=255-gray(x,y);//像素的彩色值变为其反色 
				i = gray(x,y); 
				if(i>255) i=255; 
				if(i<0) i=0; 
			}
		} 
		return true;
	}
	

		else 
		{ 
			AfxMessageBox("原图不是真彩图或灰度图"); 
			return false;
		} 
}

BOOL DIB::ContrastINC() //增大对比度
{
	int x,y;
	double r,g,b;
	double i; 

	if(bih.biBitCount==24)
	{ 
		for(x=0;x<GetDIBHeight();x++) 
		{ 
			for(y=0;y<GetDIBWidth();y++) 
			{ 
				r=Cnumber(x,y,2); g=Cnumber(x,y,1); b=Cnumber(x,y,0); 
				
				if(r>127.5) r=r+20;//大于阈值，使其更大 
				else if(r<127.5) r=r-10;//小于阈值，使其更小
				if(r>255) r=255; 
				else if(r<0) r=0; 
				if(g>127.5) g=g+20; 
				else if(g<127.5) g=g-10; 
				if(g>255) g=255; 
				else if(g<0) g=0; 
				if(b>127.5) b=b+20; 
				else if(b<127.5) b=b-10; 
				if(b>255) b=255; 
				else if(b<0) b=0; 
				
				Cnumber(x,y,2)=(BYTE)r; Cnumber(x,y,1)=(BYTE)g; Cnumber(x,y,0)=(BYTE)b; 
			} 
		} 
		return true; 
	}
	else if(bih.biBitCount==8) 
	{ 
		for(x=0;x<GetDIBHeight();x++) 
		{ 
			for(y=0;y<GetDIBWidth();y++) 
			{  
				i=gray(x,y); 
				if (i>127.5) i=i+10; 
				else if (i<127.5) i=i-10; 
				if(i>255)i=255; 
				else if(i<0)i=0; 
				gray(x,y)=(BYTE)i; 
			} 
		} 
		return true;
	} 
	
	
	else 
	{ 
		for(x=0;x<GetDIBHeight();x++)
			AfxMessageBox("原图不是真彩图或灰度"); 
		return false; 
	} 
}

BOOL DIB::ContrastIDEC() //减小对比度
{ 
	int x,y; 
	double r,g,b;
	double i; 
	if(bih.biBitCount==24) 
	{ 
		for(x=0;x<GetDIBHeight();x++) 
		{ 
			for(y=0;y<GetDIBWidth();y++)
			{ 
				r=Cnumber(x,y,2); g=Cnumber(x,y,1); b=Cnumber(x,y,0); 

				if(r>127.5) r=r-20;//大于阈值，使其减小 
				else if(r<127.5) r=r+10; 
				if(r>255) r=255; 
				else if(r<0) r=0; 
				if(g>127.5) g=g-20; 
				else if(g<127.5) g=g+10; 
				if(g>255) g=255; else if(g<0) g=0; 
				if(b>127.5) b=b-20; else if(b<127.5) b=b+10; 
				if(b>255) b=255; else if(b<0) b=0; 
				
				Cnumber(x,y,2)=(BYTE)r; Cnumber(x,y,1)=(BYTE)g; Cnumber(x,y,0)=(BYTE)b; 
			} 
		} 
		return true; 
	}

	else if(bih.biBitCount==8) 
	{ 
		for(x=0;x<GetDIBHeight();x++) 
		{ 
			for(y=0;y<GetDIBWidth();y++) 
			{ 
				i=gray(x,y); 
				if (i>127.5) i=i-10; 
				else if (i<127.5) i=i+10; 
				if(i>255)i=255; 
				else if(i<0)i=0; 
				gray(x,y)=(BYTE)i; 
			} 
		} 
		return true;
	} 
	

	else 
	{ 
		AfxMessageBox("原图不是真彩图"); 
		return false; 
	} 
}

BOOL DIB::BrightnesINC() //增大亮度
{
	int x,y; 
	double r,g,b;
	double i; 

	if(bih.biBitCount==24)
	{ 
		for(x=0;x<GetDIBHeight();x++) 
		{ 
			for(y=0;y<GetDIBWidth();y++) 
			{ 
				r=Cnumber(x,y,2)*1.5; g=Cnumber(x,y,1)*1.5; b=Cnumber(x,y,0)*1.5;

				if(r>255) r=255; 
				else if(r<0) r=0; 
				if(g>255) g=255; 
				else if(g<0) g=0; 
				if(b>255) b=255; 
				else if(b<0) b=0; 
				
				Cnumber(x,y,2)=(BYTE)r; Cnumber(x,y,1)=(BYTE)g; Cnumber(x,y,0)=(BYTE)b; 
			} 
		} 
		return true; 
	}

	
	else if(bih.biBitCount==8) 
	{ 
		for(x=0;x<GetDIBHeight();x++) 
		{ 
			for(y=0;y<GetDIBWidth();y++) 
			{ 
				i=gray(x,y)*1.5; 
				if (i>255)	i=255; 
				if (i<0)	i=0; 
				gray(x,y)= (BYTE)i;
			}
		}
		return true;
	} 
	

	else
	{
		AfxMessageBox("原图不是真彩或灰度"); 
		return false; 
	} 
}

BOOL DIB::BrightnesDEC() //减小亮度
{
	int x,y; 
	double r,g,b;
	double i; 
	if(bih.biBitCount==24) 
	{ 
		for(x=0;x<GetDIBHeight();x++) 
		{ 
			for(y=0;y<GetDIBWidth();y++)
			{ 
				r=Cnumber(x,y,2)*0.5; g=Cnumber(x,y,1)*0.5; b=Cnumber(x,y,0)*0.5; 

				if(r>255) r=255; 
				else if(r<0) r=0; 
				if(g>255) g=255; 
				else if(g<0) g=0;
				if(b>255) b=255; 
				else if(b<0) b=0;

				Cnumber(x,y,2)=(BYTE)r; 
				Cnumber(x,y,1)=(BYTE)g; 
				Cnumber(x,y,0)=(BYTE)b; 
			} 
		} 
		return true; 
	}

	
	else if (bih.biBitCount==8) 
	{ 
		for(x=0;x<GetDIBHeight();x++) 
		{ 
			for(y=0;y<GetDIBWidth();y++) 
			{ 
				i=gray(x,y)*0.5; 
				if (i>255)	i=255; 
				if (i<0)	i=0; 
				gray(x,y)=(BYTE)i;
			}
		}
		return true;
	}
	
		else
		{
			AfxMessageBox("原图不是真彩图"); 
			return false; 
		} 
}



BOOL DIB::ExtractRED() //提取R分量 
{
	int x,y; 
	double r,g,b; 
	if(bih.biBitCount!=24) 
	{ 
		AfxMessageBox("原图不是真彩图"); 
		return false; 
	} 
	
	else 
	{ 
		for(x=0;x<GetDIBHeight();x++) 
		{ 
			for(y=0;y<GetDIBWidth();y++) 
			{ 
				r=Cnumber(x,y,2); g=0; b=0;//蓝色和绿色分量置0 
				if(r>255) r=255; 
				else if(r<0) r=0; 
				
				Cnumber(x,y,2)=(BYTE)r; 
				Cnumber(x,y,1)=(BYTE)g; 
				Cnumber(x,y,0)=(BYTE)b; 
			} 
		} 
	} 
	return true; 
} 

BOOL DIB::ExtractGREEN()//提取G分量 
{ 
	int x,y; 
	double r,g,b; 
	if(bih.biBitCount!=24) 
	{ 
		AfxMessageBox("原图不是真彩图"); 
		return false; 
	} 
	else 
	{
		for(x=0;x<GetDIBHeight();x++) 
		{ 
			for(y=0;y<GetDIBWidth();y++) 
			{ 
				g=Cnumber(x,y,1); r=0; b=0;//蓝色和红色分量置0 
				if(g>255) r=255; else if(g<0) r=0;

				Cnumber(x,y,2)=(BYTE)r; Cnumber(x,y,1)=(BYTE)g; Cnumber(x,y,0)=(BYTE)b; 
			} 
		} 
	} 
	return true;
} 

BOOL DIB::ExtractBLUE() //提取B分量
{
	int x,y; 
	double r,g,b;
	if(bih.biBitCount!=24) 
	{ 
		AfxMessageBox("原图不是真彩图"); 
		return false; 
	} 
	else 
	{ 
		for(x=0;x<GetDIBHeight();x++) 
		{ 
			for(y=0;y<GetDIBWidth();y++) 
			{ 
				b=Cnumber(x,y,0); r=0; g=0;//红色和绿色分量置0 

				if(b>255) r=255; 
				else if(b<0) r=0; 

				Cnumber(x,y,2)=(BYTE)r; 
				Cnumber(x,y,1)=(BYTE)g;
				Cnumber(x,y,0)=(BYTE)b; 
			} 
		} 
	} 
	return true;
}


BOOL DIB::MirrorUp2Down()//图像上下镜像 
{
	int x,y; 
	double r,g,b;
	double i; 

	if(bih.biBitCount==24)
	{ 
		for(x=0;x<=int(GetDIBHeight()/2);x++) 
		{ 
			for(y=0;y<GetDIBWidth();y++) 
			{ 
				if(r>255) r=255; else if(r<0) r=0; 
				if(g>255) g=255; else if(g<0) g=0; 
				if(b>255) b=255; else if(b<0) b=0; 

				//像素按上下镜像重新排列
				r=Cnumber(GetDIBHeight()-x-1,y,2); (Cnumber(GetDIBHeight()-x-1,y,2))=(BYTE)Cnumber(x,y,2); //列
				g=Cnumber(GetDIBHeight()-x-1,y,1); (Cnumber(GetDIBHeight()-x-1,y,1))=(BYTE)Cnumber(x,y,1);
				b=Cnumber(GetDIBHeight()-x-1,y,0); (Cnumber(GetDIBHeight()-x-1,y,0))=(BYTE)Cnumber(x,y,0); //列

				//像素按上下镜像重新排 
				Cnumber(x,y,2)=(BYTE)r; 
				Cnumber(x,y,1)=(BYTE)g; 
				Cnumber(x,y,0)=(BYTE)b; 
			} 
		} 
		return true; 
	}

	
	else if(bih.biBitCount==8) 
	{ 
		for(x=0;x<=int(GetDIBHeight()/2);x++) 
		{ 
			for(y=0;y<GetDIBWidth();y++) 
			{ 
				i=gray(GetDIBHeight()-x-1,y);
				gray(GetDIBHeight()-x-1,y)=(BYTE)gray(x,y); 
				gray(x,y)=(BYTE)i;
			}
		}
		return true;
	} 
	

	else 
	{ 
		AfxMessageBox("原图不是真彩图或灰度"); 
		return false; 
	} 
}

BOOL DIB::MirrorLeft2Right() //图像左右镜像
{
	int x,y; 
	double temp;
	double i; 

	if(bih.biBitCount==24)
	{ 
		for(x=0;x<GetDIBHeight();x++) 
		{ 
			for(y=0;y<=GetDIBWidth()/2;y++) 
			{ 
				if(temp>255) temp=255; else if(temp<0) temp=0; 

				for(int k = 0; k<3; k++)
				{
					//像素按左右镜像重新排列
					temp=Cnumber(x,GetDIBWidth()-y-1,k); 
					Cnumber(x,GetDIBWidth()-y-1,k)=Cnumber(x,y,k);
					Cnumber(x,y,k)=(BYTE)temp;
				}
			} 
		} 
		return true; 
	}

	
	else if(bih.biBitCount==8) 
	{ 
		for(x=0;x<=int(GetDIBHeight()/2);x++) 
		{ 
			for(y=0;y<GetDIBWidth();y++) 
			{ 
				i=gray(GetDIBHeight()-x-1,y);

				gray(GetDIBHeight()-x-1,y)=(BYTE)gray(x,y); 
				gray(x,y)=(BYTE)i;
			}
		}
		return true;
	} 	

	else { 
		AfxMessageBox("原图不是真彩图或灰度"); 
		return false; 
	} 
}

BOOL DIB::GamaCorrection() //图像伽马校正 
{ 
	int x,y; 
	double r,g,b;
	double i; 
	
	mydialog dlg; 	
	dlg.m_value=NULL; //响应并输入r值

	if(dlg.DoModal()!=IDOK) 
	{ 
		return false; 
	} 
	
	float value=dlg.m_value;//将输入的r值传送给value	
	delete dlg;
	
	if(bih.biBitCount==24) 
	{ 
		for(x=0;x<GetDIBHeight();x++) 
		{ 
			for(y=0;y<GetDIBWidth();y++) 
			{ 
				r=Cnumber(x,y,2); g=Cnumber(x,y,1); b=Cnumber(x,y,0); 

				r=255*pow(r/255,value); 
				if(r>255) r=255; else if(r<0) r=0; 
				g=255*pow(g/255,value); 
				if(g>255) g=255; else if(g<0) g=0; 
				b=255*pow(b/255,value); 
				if(b>255) b=255; else if(b<0) b=0;  
					
				Cnumber(x,y,2)=(BYTE)r; 
				Cnumber(x,y,1)=(BYTE)g; 
				Cnumber(x,y,0)=(BYTE)b; 
			} 
		}
		return true;
	}
	
	
	else if(bih.biBitCount==8)
	{
		for(x=0;x<GetDIBHeight();x++) 
		{ 
			for(y=0;y<GetDIBWidth();y++) 
			{ 
				i=gray(x,y); 
				i=255*pow(i/255,value); 
				if(i>255) i=255; else if(i<0) i=0;
				gray(x,y)=(BYTE)i;
			}
		}
		return true;
	}
	
		else 
		{
			AfxMessageBox("原图不是真彩图或灰度"); 
			return false; 
		} 
}

//将一幅图片转换角度后返回新的图片
CBitmap* DIB::BmpRotate(CBitmap* cBmp, float Angle)
{
	BITMAP bmp;
	cBmp->GetBitmap(&bmp);
	BYTE *pBits = new BYTE[bmp.bmWidthBytes*bmp.bmHeight], *TempBits = new BYTE[bmp.bmWidthBytes*bmp.bmHeight];
	cBmp->GetBitmapBits(bmp.bmWidthBytes*bmp.bmHeight, pBits);
	//#define Cnumber(x,y,z) *(m_pDIBData+(x)*GetBmpRealWidth()+(y)*3+(z))
	Angle = float (Angle*3.1415926 / 180);
	int interval=bmp.bmWidthBytes/bmp.bmWidth;
	double rx0 = bmp.bmWidth*0.5;
	double ry0 = bmp.bmHeight*0.5;

	for (int j = 0; j < bmp.bmHeight; j ++)
	{
		for (int i = 0; i < bmp.bmWidth; i++)
		{
			for (int k = 0; k < interval; k++)
			{
				TempBits[i*bmp.bmWidthBytes + j*interval + k] = 0xff;
			}
		}
	}

	for (j = 0; j < bmp.bmHeight; j++)
	{
		for (int i = 0; i< bmp.bmWidth; i++)
		{
			int tempI, tempJ;
			tempI = int ((i-rx0)*cos(Angle) + (j-ry0)*sin(Angle)+rx0);
			tempJ = int (-(i-rx0)*sin(Angle) + (j-ry0)*cos(Angle)+ry0);
			if (tempI>0&&tempI<bmp.bmWidth)
				if (tempJ>0 && tempJ < bmp.bmHeight)
				{
					for (int m = 0; m < interval;m++)
						TempBits[i*bmp.bmWidthBytes + j*interval+m] = pBits[tempI*bmp.bmWidthBytes + interval*tempJ+m];
				}
		}
	}
	CBitmap *m_bitmap;
	m_bitmap = new CBitmap;
	m_bitmap->CreateBitmapIndirect(&bmp);
	m_bitmap->SetBitmapBits(bmp.bmWidthBytes*bmp.bmHeight, TempBits);
	delete pBits;
	delete TempBits;

	return m_bitmap;
}

/*
BOOL DIB::Rotation90() //图像右旋转90度(只能对正方形图像操作) 
{ 
	int x,y; 
	double r,g,b; //三个分量
	double i,j;

	if(bih.biBitCount==24)
	{  
		for(x=0;x<GetDIBHeight();x++) 
		{ 
			for(y=x;y<GetDIBWidth();y++) 
			{ 
				r=Cnumber(x,y,2);Cnumber(x,y,2)=Cnumber(y,x,2);Cnumber(y,x,2)=(byte)r; 
				g=Cnumber(x,y,1);Cnumber(x,y,1)=Cnumber(y,x,1);Cnumber(y,x,1)=(byte)g; 
				b=Cnumber(x,y,0);Cnumber(x,y,0)=Cnumber(y,x,0);Cnumber(y,x,0)=(byte)b;
				//将图像各个像素按转置交换（相当于矩阵转置） 
			} 
		} 
		for(x=0;x<GetDIBHeight()/2;x++) 
		{ 
			for(y=0;y<GetDIBWidth();y++) 
			{ 
				r=Cnumber(x,y,2);Cnumber(x,y,2)=Cnumber(GetDIBHeight()-1-x,y,2);Cnumber(GetDIBHeight()-1-x,y,2)= (byte)r;
				g=Cnumber(x,y,1);Cnumber(x,y,1)=Cnumber(GetDIBHeight()-1-x,y,1);Cnumber(GetDIBHeight()-1-x,y,1)= (byte)g; 
				b=Cnumber(x,y,0);Cnumber(x,y,0)=Cnumber(GetDIBHeight()-1-x,y,0);Cnumber(GetDIBHeight()-1-x,y,0)= (byte)b; 
		/*for(x=0;x<GetDIBHeight();x++)
		{ 
			for(y=0;y<GetDIBWidth();y++)
			{
				for(int k = 0; k<3; k++)
					Cnumber(x,y,k) = Cnumber1(y,x,k);
					//b=Cnumber(x,y,k);Cnumber(x,y,k)=Cnumber(y,x,k);Cnumber(y,x,k)=(byte)b;

				
				#define Cnumber(x,y,z) *(m_pDIBData+(x)*GetBmpRealWidth()+(y)*3+(z))
				src[(i * desW + j) * channel + k] = tempSrc[((srcH - 1 - j) * srcW + i) * channel + k];
				src[(i * desW + j) * channel + k] = tempSrc[((srcH - 1 - i) * srcW + srcW - 1 - j) * channel + k];
				
			} 
		}
		//m_pBMI->bmiHeader.biHeight = GetDIBWidth();
		//m_pBMI->bmiHeader.biWidth = GetDIBHeight();
		AfxMessageBox("Hello"); 
		return true; 
	} 

	else if(bih.biBitCount==8)
	{ 
		for(x=0;x<GetDIBHeight();x++) 
		{ 
			for(y=0;y<GetDIBWidth();y++) 
			{ 	
 
				i=gray(y,x); 
				gray(y,x)=(BYTE)gray(x,y); 
				gray(x,y)=(BYTE)i; 
				if(i>255)	i=255; 
				else if(i<0)	i=0; 
			} 
			for(x=0;x<=int(GetDIBHeight()/2);x++) 
			{ 
				for(y=0;y<GetDIBWidth();y++) 
				{ 
					j=gray(GetDIBHeight()-x-1,y); 
					(gray(GetDIBHeight()-x-1,y))=(BYTE)gray(x,y); gray(x,y)=(BYTE)j; 
				}
			}
		}
		return true;
	} 
	

	else { 
		AfxMessageBox("原图不是真彩图或灰度"); 
		return false; 
	} 
}

*/


BOOL DIB::Rotation90() //图像右旋转90度(只能对正方形图像操作) 
{ 
	int x,y; 
	double r,g,b;
	double i,j; 
	
	if(GetDIBWidth()%GetDIBHeight())
	{
		AfxMessageBox("请给我一张正方形图片:)"); 
		return false; 
	}

	if(bih.biBitCount==24) 
	{ 
		for(x=0;x<GetDIBHeight();x++) 
		{ 
			for(y=x;y<GetDIBWidth();y++) 
			{ 
				r=Cnumber(x,y,2);Cnumber(x,y,2)=Cnumber(y,x,2);Cnumber(y,x,2)=(byte)r; 
				g=Cnumber(x,y,1);Cnumber(x,y,1)=Cnumber(y,x,1);Cnumber(y,x,1)=(byte)g; 
				b=Cnumber(x,y,0);Cnumber(x,y,0)=Cnumber(y,x,0);Cnumber(y,x,0)=(byte)b;
				//将图像各个像素按转置交换（相当于矩阵转置） 
			} 
		} 
		for(x=0;x<GetDIBHeight()/2;x++) 
		{ 
			for(y=0;y<GetDIBWidth();y++) 
			{ 
				r=Cnumber(x,y,2);Cnumber(x,y,2)=Cnumber(GetDIBHeight()-1-x,y,2);Cnumber(GetDIBHeight()-1-x,y,2)= (byte)r;
				g=Cnumber(x,y,1);Cnumber(x,y,1)=Cnumber(GetDIBHeight()-1-x,y,1);Cnumber(GetDIBHeight()-1-x,y,1)= (byte)g; 
				b=Cnumber(x,y,0);Cnumber(x,y,0)=Cnumber(GetDIBHeight()-1-x,y,0);Cnumber(GetDIBHeight()-1-x,y,0)= (byte)b; 
				//将图像像素转置后，再将图像像素值按镜像排列，综合转置与镜像，即可达到图像旋转90
			} 
		} 
		return true; 
	}  

	else if(bih.biBitCount==8) 
	{ 
		for(x=0;x<GetDIBHeight();x++) 
		{ 
			for(y=0;y<GetDIBWidth();y++) 
			{ 	
 
				i=gray(y,x); gray(y,x)=(BYTE)gray(x,y); gray(x,y)=(BYTE)i; 
				if(i>255)	i=255; 
				else if(i<0)	i=0; 
			}
		}
		for(x=0;x<GetDIBHeight()/2;x++) 
		{ 
			for(y=0;y<GetDIBWidth();y++) 
			{ 					
				j=gray(x,y); gray(x,y)=gray(GetDIBHeight()-x-1,y);gray(GetDIBHeight()-x-1,y)=(byte)j;
			}
		}
		return true;
	} 
	

	else { 
		AfxMessageBox("原图不是真彩图或灰度"); 
		return false; 
	} 
}


BOOL DIB::Resort1()//图像还原方法一 
{
	
	int x,y; 

	if(bih.biBitCount==24) 
	{ 
		for(y=0;y<GetDIBWidth();y++) 
		{ 
			for(x=0;x<GetDIBHeight();x++)
			{	 
				//将预先保存的原图重新复制给Cnumber(x,y,z)
				Cnumber(x,y,0)=Cnumber1(x,y,0); 
				Cnumber(x,y,1)=Cnumber1(x,y,1);
				Cnumber(x,y,2)=Cnumber1(x,y,2); 
			} 
		} 
		return true;
	}
	
	
	else if(bih.biBitCount==8) 
	{ 
		for(x=0;x<GetDIBHeight();x++) 
		{ 
			for(y=0;y<GetDIBWidth();y++) 
			{ 
				gray(x,y)=gray1(x,y); //将预先保存的原图重新复制给 gray(x,y)
			} 
		}
		return true;
	} 
	
	else { 
		AfxMessageBox("原图不是真彩图或灰度"); 
		return false; 
	} 
	
	return true;
} 

BOOL DIB::Convert24toGray() //24位真彩色转8位灰度图
{ 
	int i,j,n; 
	int DataByte; 
	BITMAPINFO* pBMI = NULL;//新定义位图信息头结构指针pBMI 
	BYTE* pDIBData = NULL;//新定义指向位图像素灰度值的指针pDIBData
	
	//判断打开图像是否为 24 位图 
	if(bih.biBitCount!=24) 
	{ 
		AfxMessageBox("原图不是 24 位真彩色图"); 
		return false; 
	} 

	long w=GetDIBWidth();
	if(w%4!=0)//判断宽度是不是4的倍数
	{
		n=w/4; 
		w=4*n+4;
	} 
	
	DataByte=w;//一行带有0的数据
	int supply = DataByte - GetDIBWidth(); //补0的个数
	pBMI=(BITMAPINFO*)new char[sizeof(BITMAPINFOHEADER)+sizeof(RGBQUAD)*256]; //为信息头分配内存空间 
	pDIBData=(BYTE*)new char[GetDIBHeight()*DataByte];//为新图像的像素点分配内存空间（长×宽） 
	
	//建立新的颜色表 
	for(i=0;i<256;i++)
	{ 
		pBMI->bmiColors[i].rgbRed= (unsigned char)i; 
		pBMI->bmiColors[i].rgbGreen=(unsigned char)i; 
		pBMI->bmiColors[i].rgbBlue=(unsigned char)i; 
		pBMI->bmiColors[i].rgbReserved=0;
	} 
		
	for(i=0;i<GetDIBHeight();i++) {
		for(j=0;j<GetDIBWidth();j++) //放数据
			*(pDIBData+i*(GetDIBWidth()+supply)+j)=(byte)(0.299*Cnumber(i,j,2)+0.587*Cnumber(i,j,1)+0.114*Cnumber(i,j,0));//灰度值采用亮度方程计算 
		
		for(int k = 0; k<(DataByte-GetDIBWidth()); k++)//补充0
			*(pDIBData+i*supply+(i+1)*GetDIBWidth()) = (byte) 0x00;
	}	

	bih.biBitCount=8;//更改颜色深度
	pBMI->bmiHeader.biClrUsed=256;
    pBMI->bmiHeader.biSizeImage=GetDIBHeight()*DataByte;
	bfh.bfOffBits=sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD)* 256;//实际图形数据偏移量 
	bfh.bfSize=sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD)*256 + GetDIBHeight()*DataByte;//文件总大小 
	memcpy(&pBMI->bmiHeader,&bih,sizeof(BITMAPINFOHEADER)); 
	
	if(m_pBMI!=NULL)// 判断原图信息头是否为空
	delete m_pBMI;
	m_pBMI=pBMI; 

	if(m_pDIBData!=NULL)//判断原图数据是否为空
	delete m_pDIBData; 
	m_pDIBData=pDIBData; 
	
	return true; 
}

BOOL DIB::ScalingNearest()  //图像缩放
{	
	mydialog1 dlg;
	float times = 0;
	dlg.m_scaling=NULL;//响应并输入缩放系数
	if (dlg.DoModal() != IDOK)
	{
		return false;	//返回
	}	
	times = dlg.m_scaling;//将输入的缩放系数传送给ExpScalValue	
	delete dlg;

	int width = GetDIBWidth();
	int height = GetDIBHeight();
	int MYDRAW_HEIGHT = height * times;
	int MYDRAW_WIDTH = width * times;
	int T_width = WIDTHBYTES(MYDRAW_WIDTH * bih.biBitCount);		
	int L_width = WIDTHBYTES(width * bih.biBitCount);				
	long T_size = T_width * MYDRAW_HEIGHT;							
	long L_size = L_width * height;									
	BYTE* pColorData = (BYTE*)new char[L_size];						
	memset(pColorData, 0, L_size);									
	BYTE* pColorDataMid = (BYTE*)new char[T_size];					
	memset(pColorDataMid, 0, T_size);								
	memcpy(pColorData, m_pDIBData, L_size);							
	bih.biWidth = m_pBMI->bmiHeader.biWidth = MYDRAW_WIDTH;
	bih.biHeight = m_pBMI->bmiHeader.biHeight = MYDRAW_HEIGHT;
	bih.biSizeImage = m_pBMI->bmiHeader.biSizeImage = T_size;
	bfh.bfSize = T_size + bfh.bfOffBits;
	for (int hnum = 0; hnum < MYDRAW_HEIGHT; hnum++)
		for (int wnum = 0; wnum < MYDRAW_WIDTH; wnum++)
		{
			double d_original_img_hnum = hnum * height / (double)MYDRAW_HEIGHT;
			double d_original_img_wnum = wnum * width / (double)MYDRAW_WIDTH;
			int i_original_img_hnum = (int)d_original_img_hnum;
			int i_original_img_wnum = (int)d_original_img_wnum;
			double distance_to_a_x = d_original_img_wnum - i_original_img_wnum;   
			double distance_to_a_y = d_original_img_hnum - i_original_img_hnum;
			int original_point_a = i_original_img_hnum * L_width + i_original_img_wnum * 3;
			int original_point_b = i_original_img_hnum * L_width + (i_original_img_wnum + 1) * 3;
			int original_point_c = (i_original_img_hnum + 1) * L_width + i_original_img_wnum * 3;
			int original_point_d = (i_original_img_hnum + 1) * L_width + (i_original_img_wnum + 1) * 3;
			if (i_original_img_hnum + 1 >= height - 1)
			{
				original_point_c = original_point_a;
				original_point_d = original_point_b;
			}
			if (i_original_img_wnum + 1 >= width - 1)
			{
				original_point_b = original_point_a;
				original_point_d = original_point_c;
			}
			int pixel_point = hnum * T_width + wnum * 3;
			pColorDataMid[pixel_point] =
				pColorData[original_point_a] * (1 - distance_to_a_x) * (1 - distance_to_a_y) +
				pColorData[original_point_b] * distance_to_a_x * (1 - distance_to_a_y) +
				pColorData[original_point_c] * distance_to_a_y * (1 - distance_to_a_x) +
				pColorData[original_point_c] * distance_to_a_y * distance_to_a_x;
			pColorDataMid[pixel_point + 1] =
				pColorData[original_point_a + 1] * (1 - distance_to_a_x) * (1 - distance_to_a_y) +
				pColorData[original_point_b + 1] * distance_to_a_x * (1 - distance_to_a_y) +
				pColorData[original_point_c + 1] * distance_to_a_y * (1 - distance_to_a_x) +
				pColorData[original_point_c + 1] * distance_to_a_y * distance_to_a_x;
			pColorDataMid[pixel_point + 2] =
				pColorData[original_point_a + 2] * (1 - distance_to_a_x) * (1 - distance_to_a_y) +
				pColorData[original_point_b + 2] * distance_to_a_x * (1 - distance_to_a_y) +
				pColorData[original_point_c + 2] * distance_to_a_y * (1 - distance_to_a_x) +
				pColorData[original_point_c + 2] * distance_to_a_y * distance_to_a_x;
		}
		if (m_pDIBData != NULL)
			delete m_pDIBData;
		m_pDIBData = pColorDataMid;
		delete pColorData;
}

/*///////////////////////////////////////////////////
d
////////////////////////////////////////////////////

BOOL DIB::ScalingNearest()
{	
	BYTE* pDIBData = NULL;//新定义指向位图像素灰度值的指针pDIBData
	BYTE* pDIBDataS = NULL;//动态分配存储缩放后的图片的像素信息
	
	float ExpScalValue = 0;
	int FloatToIntwidth, FloatToIntheight;//小数变成整数(float To Int)
	int RotateAngle = 90; //默认缩放旋转角度

	mydialog1 dlg;
	dlg.m_scaling=NULL;//响应并输入缩放系数
	if (dlg.DoModal() != IDOK)
	{
		return false;	// 返回
	}	
	ExpScalValue = dlg.m_scaling;//将输入的缩放系数传送给ExpScalValue	
	delete dlg;

	//如果ExpScalValue含有小数，需要整数化
	//对期望的缩放结果取整
	FloatToIntwidth = (int)(ExpScalValue*GetDIBWidth());
	FloatToIntheight = (int)(ExpScalValue*GetDIBHeight());

	//图像每一行的字节数必须是4的整数倍
	int lineByte2 = (FloatToIntwidth * bih.biBitCount / 8 + 3) / 4 * 4;
	pDIBDataS = new BYTE[lineByte2 * FloatToIntheight];//为缩放后图像分配存储空间

	int pre_x, pre_y, after_x, after_y; //缩放前后对应的像素点坐标

	if(bih.biBitCount==24) 
	{
		for (int x = 0; x<FloatToIntheight; x++)
		{
			for (int y = 0; y<FloatToIntwidth; y++)
			{
				for (int k = 0; k < 3; k++)
				{
					after_x = x;
					after_y = y;
					pre_x = (int)(after_x / ExpScalValue);//取整，插值方法为：最邻近插值（近邻取样法）
					pre_y = (int)(after_y / ExpScalValue); 
					if (pre_x >= 0 && pre_x < GetDIBHeight() && pre_y >= 0 && pre_y < GetDIBWidth())//在原图范围内
						*(pDIBDataS+x*lineByte2+y*3+k)= *(pDIBData + pre_x * GetDIBWidth()*3 + pre_y*3+k);
				}
			}
		}
		
		delete[] pDIBDataS;
		if(m_pDIBData!=NULL)//判断原图数据是否为空
			delete m_pDIBData; //释放内存
		m_pDIBData = pDIBDataS; 
		m_pBMI->bmiHeader.biHeight = GetDIBWidth();
		m_pBMI->bmiHeader.biWidth = GetDIBHeight();
		return true;
	}
	
	
	else if(bih.biBitCount==8)
	{
		
		
		
		AfxMessageBox("原图不是24位真彩图或8位灰度"); 
		return false;
	}
	
	
		else 
		{
			AfxMessageBox("原图不是真彩图或灰度"); 
			return false; 
		} 
}
*/

BOOL DIB::UserTip()
{
	mydialog2 dlg;
	if (dlg.DoModal() != IDOK)
	{
		return false;	// 返回
	}
	delete dlg;

	return true;
}
