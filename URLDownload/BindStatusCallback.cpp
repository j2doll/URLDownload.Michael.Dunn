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

// BindStatusCallback.cpp: implementation of the CBindStatusCallback class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "URLDownload.h"
#include "BindStatusCallback.h"
#include <shlwapi.h>                    // for StrFormatByteSize()

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCallback::CCallback() : m_bUseTimeout(FALSE), m_pDlg(NULL)
{
}

CCallback::~CCallback()
{
}

HRESULT CCallback::OnProgress ( ULONG ulProgress,   ULONG ulProgressMax,
                                ULONG ulStatusCode, LPCWSTR wszStatusText )
{
// Local variables are declared static so they don't have to be reallocated on
// the stack every time.  This is safe in this app since I know I'll only have
// one thread downloading.
static CString sIEStatusMsg;
static TCHAR   szCustomStatusMsg [256];
static TCHAR   szAmtDownloaded [256], szTotalSize [256];

    UNREFERENCED_PARAMETER(ulStatusCode);

    // Did the user hit the Stop button?
    if ( 0 != g_fAbortDownload )
        return E_ABORT;

    // Has the timeout period elapsed?
    if ( m_bUseTimeout  &&  CTime::GetCurrentTime() > m_timeToStop )
        return E_ABORT;

    // Use CString to convert IE's status message to a TCHAR string.
    if ( NULL != wszStatusText )
        {
        sIEStatusMsg = wszStatusText;
        }
    else
        {
        sIEStatusMsg.Empty();
        }

    // Make our own progress message - we'll show the amount downloaded and
    // the total file size (if known).

    StrFormatByteSize ( ulProgress, szAmtDownloaded, 256 );
    StrFormatByteSize ( ulProgressMax, szTotalSize, 256 );
    
    if ( 0 != ulProgressMax )
        {
        wsprintf ( szCustomStatusMsg, _T("Downloaded %s of %s"),
                   szAmtDownloaded, szTotalSize );
        }
    else
        {
        wsprintf ( szCustomStatusMsg, _T("Downloaded %s (total size unknown)"),
                   szAmtDownloaded );
        }

    // Report the progress back to the main window.

    if ( 0 != ulProgressMax )
        {
        m_pDlg->ProgressUpdate ( sIEStatusMsg, szCustomStatusMsg,
                                 int( 100.0 * ulProgress / ulProgressMax) );
        }
    else
        {
        m_pDlg->ProgressUpdate ( sIEStatusMsg, szCustomStatusMsg, 0 );
        }

    return S_OK;
}
