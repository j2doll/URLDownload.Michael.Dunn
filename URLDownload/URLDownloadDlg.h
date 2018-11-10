//////////////////////////////////////////////////////////////////////
//
// Written by Michael Dunn (mdunn at inreach dot com).  Copyright and all
// that stuff.  Use however you like but give me credit where it's due.  I'll
// know if you don't. bwa ha ha ha ha!
// 
// Release history:
//   December 24, 1999: Version 1.0.  First release.
//
//////////////////////////////////////////////////////////////////////

// URLDownloadDlg.h : header file
//

#if !defined(AFX_URLDOWNLOADDLG_H__6B469EC6_B785_11D3_8D3B_D5CFB868D41C__INCLUDED_)
#define AFX_URLDOWNLOADDLG_H__6B469EC6_B785_11D3_8D3B_D5CFB868D41C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CURLDownloadDlg dialog

class CURLDownloadDlg : public CDialog
{
// Construction
public:
	CURLDownloadDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CURLDownloadDlg)
	enum { IDD = IDD_URLDOWNLOAD_DIALOG };
	CEdit	m_editTimeout;
	CButton	m_btnTimeout;
	CSpinButtonCtrl	m_spinner;
	CEdit	m_editFile;
	CButton	m_btnAbout;
	CEdit	m_editURL;
	CButton	m_btnExit;
	CButton	m_btnStop;
	CButton	m_btnStart;
	CProgressCtrl	m_progress;
	CStatic	m_stIEMsg;
	CStatic	m_stCustomMsg;
	UINT	m_uTimeout;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CURLDownloadDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;
    void UpdateTimeoutCtrls();

public:
    void WorkerThreadProc();
    void ProgressUpdate ( LPCTSTR szIEMsg, LPCTSTR szCustomMsg,
                          const int nPercentDone );

	// Generated message map functions
	//{{AFX_MSG(CURLDownloadDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	virtual void OnOK();
	afx_msg void OnStart();
	afx_msg void OnStop();
	afx_msg void OnAbout();
	afx_msg void OnClickedTimeout();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_URLDOWNLOADDLG_H__6B469EC6_B785_11D3_8D3B_D5CFB868D41C__INCLUDED_)
