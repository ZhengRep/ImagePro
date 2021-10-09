// ImagePro.h : main header file for the IMAGEPRO application
//

#if !defined(AFX_IMAGEPRO_H__AA1F6B75_5B42_47CA_9311_229BBD1FF491__INCLUDED_)
#define AFX_IMAGEPRO_H__AA1F6B75_5B42_47CA_9311_229BBD1FF491__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CImageProApp:
// See ImagePro.cpp for the implementation of this class
//

class CImageProApp : public CWinApp
{
public:
	CImageProApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CImageProApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CImageProApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IMAGEPRO_H__AA1F6B75_5B42_47CA_9311_229BBD1FF491__INCLUDED_)
