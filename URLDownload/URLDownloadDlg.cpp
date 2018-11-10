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

// URLDownloadDlg.cpp : implementation file
//

#include "stdafx.h"
#include "URLDownload.h"
#include "URLDownloadDlg.h"
#include "BindStatusCallback.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static UINT gThreadProc ( void* pv );

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CURLDownloadDlg dialog

CURLDownloadDlg::CURLDownloadDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CURLDownloadDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CURLDownloadDlg)
	m_uTimeout = 0;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CURLDownloadDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CURLDownloadDlg)
	DDX_Control(pDX, IDC_TIMEOUT_SECS, m_editTimeout);
	DDX_Control(pDX, IDC_TIMEOUT, m_btnTimeout);
	DDX_Control(pDX, IDC_SPIN, m_spinner);
	DDX_Control(pDX, IDC_FILE, m_editFile);
	DDX_Control(pDX, IDC_ABOUT, m_btnAbout);
	DDX_Control(pDX, IDC_URL, m_editURL);
	DDX_Control(pDX, IDCANCEL, m_btnExit);
	DDX_Control(pDX, IDC_STOP, m_btnStop);
	DDX_Control(pDX, IDC_START, m_btnStart);
	DDX_Control(pDX, IDC_PROGRESS, m_progress);
	DDX_Control(pDX, IDC_IE_STATUS, m_stIEMsg);
	DDX_Control(pDX, IDC_CUSTOM_STATUS, m_stCustomMsg);
	DDX_Text(pDX, IDC_TIMEOUT_SECS, m_uTimeout);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CURLDownloadDlg, CDialog)
	//{{AFX_MSG_MAP(CURLDownloadDlg)
	ON_WM_SYSCOMMAND()
	ON_BN_CLICKED(IDC_START, OnStart)
	ON_BN_CLICKED(IDC_STOP, OnStop)
	ON_BN_CLICKED(IDC_ABOUT, OnAbout)
	ON_BN_CLICKED(IDC_TIMEOUT, OnClickedTimeout)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CURLDownloadDlg message handlers

BOOL CURLDownloadDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

    // Init & pre-fill some of the dlg controls.

    m_spinner.SetRange ( 1, 120 );
    m_progress.SetRange ( 0, 100 );
    
    // Fill the URL editbox with "http://"
    m_editURL.SetWindowText ( _T("http://") );

    // Fill the file editbox with the user's MyDocs directory.
LPITEMIDLIST pidl;
LPMALLOC     pMalloc;
TCHAR        szDocsDir [MAX_PATH];

    if ( SUCCEEDED( SHGetSpecialFolderLocation ( NULL, CSIDL_PERSONAL,
                                                 &pidl )))
        {
        if ( SHGetPathFromIDList ( pidl, szDocsDir ))
            {
            lstrcat ( szDocsDir, _T("\\") );
            m_editFile.SetWindowText ( szDocsDir );
            }

        if ( SUCCEEDED( SHGetMalloc ( &pMalloc )))
            {
            pMalloc->Free ( pidl );
            pMalloc->Release();
            }
        }

    // Set the focus to the URL edit box.
    GotoDlgCtrl ( &m_editURL );
    m_editURL.SetSel ( -1, -1 );

    return FALSE;  // return TRUE  unless you set the focus to a control
}

void CURLDownloadDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}


/////////////////////////////////////////////////////////////////////////////
// CURLDownloadDlg command handlers

void CURLDownloadDlg::OnOK() 
{
    // Do nothing - this is just here to prevent the dlg from closing if
    // the user hits the Enter key.
}

void CURLDownloadDlg::OnStart() 
{
CWinThread* pWorkerThread;

    UpdateData();

    if ( m_editURL.GetWindowTextLength() == 0 )
        {
        AfxMessageBox ( _T("Please enter a URL.") );
        return;
        }

    if ( m_editURL.GetWindowTextLength() == 0 )
        {
        AfxMessageBox ( _T("Please enter a filename.") );
        return;
        }

    // Create our worker thread.  It won't start until we disable all of the
    // dialog controls.

    pWorkerThread = AfxBeginThread ( gThreadProc, this,
                                     THREAD_PRIORITY_NORMAL, 0, 
                                     CREATE_SUSPENDED );

    // Note: Yeah I know that it's BAD to pass a CWnd object between threads.
    // I forgot about that rule when I wrote the above call, and it turns out
    // that I got lucky and it works.  All the stuff the worker thread does
    // involves just sending messages to window handles.  Since accessing the
    // data member CWnd::m_hWnd is safe to do across threads, nothing ever
    // asserted to alert me of this mistake.
    // Since I have finished this sample app and just now noticed this mistake,
    // I;m gonna be lazy and not fix it. :D

    if ( NULL != pWorkerThread )
        {
        g_fAbortDownload = 0;

        m_editURL.    EnableWindow ( FALSE );
        m_editFile.   EnableWindow ( FALSE );
        m_editTimeout.EnableWindow ( FALSE );
        m_btnStart.   EnableWindow ( FALSE );
        m_btnStop.    EnableWindow ();
        m_btnExit.    EnableWindow ( FALSE );
        m_btnAbout.   EnableWindow ( FALSE );
        m_btnTimeout. EnableWindow ( FALSE );
        m_spinner.    EnableWindow ( FALSE );

        GotoDlgCtrl ( &m_btnStop );

        // Kick off the download!
        pWorkerThread->ResumeThread();
        }
    else
        {
        AfxMessageBox ( _T("Couldn't create worker thread!"), MB_ICONERROR );
        }
}

void CURLDownloadDlg::OnStop() 
{
    InterlockedExchange ( &g_fAbortDownload, 1 );
}

void CURLDownloadDlg::OnAbout() 
{
CAboutDlg dlgAbout;

    dlgAbout.DoModal();
}


/////////////////////////////////////////////////////////////////////////////
// CURLDownloadDlg message handlers

// Handler for BN_CLICKED from the "Use time limit" checkbox.
void CURLDownloadDlg::OnClickedTimeout() 
{
    UpdateTimeoutCtrls();
}

// UpdateTimeoutCtrls(): Enables/disabled the timeout-related controls
// (edit box and spin button) according to the state of the "Use time limit"
// checkbox.
void CURLDownloadDlg::UpdateTimeoutCtrls()
{
BOOL bEnable = m_btnTimeout.GetCheck();

    m_editTimeout.EnableWindow ( bEnable );
    m_spinner.EnableWindow ( bEnable );
}


/////////////////////////////////////////////////////////////////////////////
// CURLDownloadDlg other functions

// gThreadProd(): Function for our worker thread.  It just calls 
// CURLDownloadDlg::WorkerThreadProc().
UINT gThreadProc ( void* pv )
{
CURLDownloadDlg* pDlg = (CURLDownloadDlg*) pv;

    pDlg->WorkerThreadProc();

    return 0;
}

// WorkerThreadProc(): Function that does all the work of downloading
// and error reporting.
void CURLDownloadDlg::WorkerThreadProc()
{
CCallback callback;
HRESULT   hr;
CString   sURL, sFile;

    callback.m_pDlg = this;

    m_editURL.GetWindowText ( sURL );
    m_editFile.GetWindowText ( sFile );

    // If the user wants a timeout, calculate the time when the download
    // should abort.
    if ( m_btnTimeout.GetCheck() )
        {
        callback.m_bUseTimeout = TRUE;

        callback.m_timeToStop = CTime::GetCurrentTime() +
                                  CTimeSpan( 0, 0, 0, m_uTimeout );
        }

    hr = URLDownloadToFile ( NULL,      // ptr to ActiveX container
                             sURL,      // URL to get
                             sFile,     // file to store data in
                             0,         // reserved
                             &callback  // ptr to IBindStatusCallback
                           );

    if ( SUCCEEDED(hr) )
        {
        AfxMessageBox ( _T("Download completed successfully!"), 
                        MB_ICONINFORMATION );
        }
    else
        {
        LPTSTR lpszErrorMessage;
        CString sMsg;

        if ( FormatMessage ( FORMAT_MESSAGE_ALLOCATE_BUFFER | 
                              FORMAT_MESSAGE_FROM_SYSTEM | 
                              FORMAT_MESSAGE_IGNORE_INSERTS,
                            NULL, hr, 
                            MAKELANGID ( LANG_NEUTRAL, SUBLANG_DEFAULT ),
                            (LPTSTR) &lpszErrorMessage, 0, NULL ))
            {
            sMsg.Format ( _T("Download failed.  Error = 0x%08lX\n\n%s"),
                          (DWORD) hr, lpszErrorMessage );

            LocalFree ( lpszErrorMessage );
            }
        else
            {
            sMsg.Format ( _T("Download failed.  Error = 0x%08lX\n\nNo message available."),
                          (DWORD) hr );
            }

        AfxMessageBox ( sMsg );
        }

    // At this point, the thread's about to end, so re-enable the dialog
    // controls.

    m_editURL.   EnableWindow();
    m_editFile.  EnableWindow();
    m_btnStart.  EnableWindow();
    m_btnStop.   EnableWindow ( FALSE );
    m_btnExit.   EnableWindow();
    m_btnAbout.  EnableWindow();
    m_btnTimeout.EnableWindow();
    UpdateTimeoutCtrls();

    GotoDlgCtrl ( &m_editURL );
}

// ProgressUpdate(): Called by the worker thread to update the status indicators
// in the main window.
void CURLDownloadDlg::ProgressUpdate ( LPCTSTR szIEMsg,
                                       LPCTSTR szCustomMsg,
                                       const int nPercentDone )
{
    ASSERT ( AfxIsValidString ( szIEMsg ));
    ASSERT ( AfxIsValidString ( szCustomMsg ));
    ASSERT ( nPercentDone >= 0  &&  nPercentDone <= 100 );

    m_stIEMsg.SetWindowText ( szIEMsg );
    m_stCustomMsg.SetWindowText ( szCustomMsg );
    m_progress.SetPos ( nPercentDone );
}
