; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CURLDownloadDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "URLDownload.h"

ClassCount=3
Class1=CURLDownloadApp
Class2=CURLDownloadDlg
Class3=CAboutDlg

ResourceCount=3
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDD_URLDOWNLOAD_DIALOG

[CLS:CURLDownloadApp]
Type=0
HeaderFile=URLDownload.h
ImplementationFile=URLDownload.cpp
Filter=N

[CLS:CURLDownloadDlg]
Type=0
HeaderFile=URLDownloadDlg.h
ImplementationFile=URLDownloadDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=IDC_TIMEOUT

[CLS:CAboutDlg]
Type=0
HeaderFile=URLDownloadDlg.h
ImplementationFile=URLDownloadDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_URLDOWNLOAD_DIALOG]
Type=1
Class=CURLDownloadDlg
ControlCount=18
Control1=IDC_STATIC,static,1342308352
Control2=IDC_URL,edit,1350762624
Control3=IDC_STATIC,static,1342308352
Control4=IDC_FILE,edit,1350762624
Control5=IDC_TIMEOUT,button,1342373891
Control6=IDC_TIMEOUT_SECS,edit,1484988544
Control7=IDC_SPIN,msctls_updown32,1476526134
Control8=IDC_STATIC,static,1342308352
Control9=IDC_STATIC,button,1342308359
Control10=IDC_STATIC,static,1342308352
Control11=IDC_IE_STATUS,static,1342308352
Control12=IDC_STATIC,static,1342308352
Control13=IDC_CUSTOM_STATUS,static,1342308352
Control14=IDC_PROGRESS,msctls_progress32,1350696961
Control15=IDC_START,button,1342373889
Control16=IDC_STOP,button,1476460544
Control17=IDCANCEL,button,1342242816
Control18=IDC_ABOUT,button,1342242816

