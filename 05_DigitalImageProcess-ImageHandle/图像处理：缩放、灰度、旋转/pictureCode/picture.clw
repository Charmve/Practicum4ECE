; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=mydialog2
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "picture.h"
LastPage=0

ClassCount=9
Class1=CChildFrame
Class2=CMainFrame
Class3=mydialog
Class4=mydialog1
Class5=CPictureApp
Class6=CAboutDlg
Class7=CPictureDoc
Class8=CPictureView

ResourceCount=6
Resource1=IDR_PICTURTYPE
Resource2=IDR_MAINFRAME
Resource3=IDD_DIALOG1
Resource4=IDD_ABOUTBOX
Resource5=IDD_DIALOG2
Class9=mydialog2
Resource6=IDD_DIALOG3

[CLS:CChildFrame]
Type=0
BaseClass=CMDIChildWnd
HeaderFile=ChildFrm.h
ImplementationFile=ChildFrm.cpp

[CLS:CMainFrame]
Type=0
BaseClass=CMDIFrameWnd
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp

[CLS:mydialog]
Type=0
BaseClass=CDialog
HeaderFile=mydialog.h
ImplementationFile=mydialog.cpp
LastObject=IDC_EDIT1

[CLS:mydialog1]
Type=0
BaseClass=CDialog
HeaderFile=mydialog1.h
ImplementationFile=mydialog1.cpp
LastObject=ID_FILE_SAVE

[CLS:CPictureApp]
Type=0
BaseClass=CWinApp
HeaderFile=picture.h
ImplementationFile=picture.cpp

[CLS:CAboutDlg]
Type=0
BaseClass=CDialog
HeaderFile=picture.cpp
ImplementationFile=picture.cpp
LastObject=ID_ScalingNearest

[CLS:CPictureDoc]
Type=0
BaseClass=CDocument
HeaderFile=pictureDoc.h
ImplementationFile=pictureDoc.cpp

[CLS:CPictureView]
Type=0
BaseClass=CView
HeaderFile=pictureView.h
ImplementationFile=pictureView.cpp
LastObject=CPictureView

[DLG:IDD_DIALOG1]
Type=1
Class=mydialog
ControlCount=5
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Control4=IDC_EDIT1,edit,1350631552
Control5=IDC_STATIC,button,1342177287

[DLG:IDD_DIALOG2]
Type=1
Class=mydialog1
ControlCount=6
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,button,1342177287
Control4=IDC_STATIC,static,1342308352
Control5=IDC_EDIT2,edit,1350631552
Control6=IDC_STATIC,static,1342308352

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[MNU:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_APP_EXIT
Command3=ID_VIEW_TOOLBAR
Command4=ID_VIEW_STATUS_BAR
Command5=ID_APP_ABOUT
CommandCount=5

[MNU:IDR_PICTURTYPE]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_CLOSE
Command4=ID_FILE_SAVE
Command5=ID_APP_EXIT
Command6=ID_VIEW_TOOLBAR
Command7=ID_VIEW_STATUS_BAR
Command8=ID_WINDOW_NEW
Command9=ID_WINDOW_CASCADE
Command10=ID_WINDOW_TILE_HORZ
Command11=ID_WINDOW_ARRANGE
Command12=ID_RGB2GRAY
Command13=ID_CY422
Command14=ID_FANSE
Command15=ID_Convert24toGray
Command16=ID_BrightnesINC
Command17=ID_BrightnesDEC
Command18=ID_ContrastINC
Command19=ID_ContrastIDEC
Command20=ID_ExtractRED
Command21=ID_ExtractGREEN
Command22=ID_ExtractBLUE
Command23=ID_MirrorUp2Down
Command24=ID_MirrorLeft2Right
Command25=ID_RESET
Command26=ID_Resort2
Command27=ID_ScalingNearest
Command28=ID_GamaCorrection
Command29=ID_Rotation90
Command30=ID_APP_ABOUT
Command31=ID_APP_AUTHOR
CommandCount=31

[TB:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_EDIT_CUT
Command5=ID_EDIT_COPY
Command6=ID_EDIT_PASTE
Command7=ID_FILE_PRINT
Command8=ID_APP_ABOUT
CommandCount=8

[ACL:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_FILE_PRINT
Command5=ID_EDIT_UNDO
Command6=ID_EDIT_CUT
Command7=ID_EDIT_COPY
Command8=ID_EDIT_PASTE
Command9=ID_EDIT_UNDO
Command10=ID_EDIT_CUT
Command11=ID_EDIT_COPY
Command12=ID_EDIT_PASTE
Command13=ID_NEXT_PANE
Command14=ID_PREV_PANE
CommandCount=14

[DLG:IDD_DIALOG3]
Type=1
Class=mydialog2
ControlCount=3
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308480

[CLS:mydialog2]
Type=0
HeaderFile=mydialog2.h
ImplementationFile=mydialog2.cpp
BaseClass=CDialog
Filter=D
LastObject=ID_APP_ABOUT

