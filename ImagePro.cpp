// ImagePro.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "ImagePro.h"

#include "MainFrm.h"
#include "ImageProDoc.h"
#include "ImageProView.h"
#include "ChildFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
//过滤器字符数组
char szFilter[] = "位图文件(*.BMP)|*.BMP|图形交换格式文件(*.GIF)|*.GIF|PCX文件(*.PCX)|*.PCX|TGA文件(*.TGA)|*.TGA|JPEG文件(*.JPG)|*.JPG|标记图像文件(*.TIF)|*.TIF|所有支持图片|*.BMP,*.GIF,*.PCX,*.TGA,*.JPG,*.TIF|所有文件(*.*)|*.*||";


/////////////////////////////////////////////////////////////////////////////
// CImageProApp

BEGIN_MESSAGE_MAP(CImageProApp, CWinApp)
	//{{AFX_MSG_MAP(CImageProApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CImageProApp::OnFileOpen)
	/*ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)*/
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CImageProApp construction

CImageProApp::CImageProApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CImageProApp object

CImageProApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CImageProApp initialization

BOOL CImageProApp::InitInstance()
{
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	LoadStdProfileSettings();  // Load standard INI file options (including MRU)

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(
		IDR_IMAGEPTYPE,
		RUNTIME_CLASS(CImageProDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CImageProView));
	AddDocTemplate(pDocTemplate);

	// create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;
	m_pMainWnd = pMainFrame;

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The main window has been initialized, so show and update it.
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

	return TRUE;
}


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
		// No message handlers
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

// App command to run the dialog
void CImageProApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

void CImageProApp::OnFileNew()
{
}

void CImageProApp::OnFileOpen()
{
	static int nIndex = 1;

	CFileDialog FileDlg(TRUE, NULL, NULL, OFN_HIDEREADONLY, szFilter);//文件对话框
	FileDlg.m_ofn.nFilterIndex = (DWORD)nIndex;//openfilename

	if (FileDlg.DoModal() == IDOK)
	{
		CString PathName = FileDlg.GetPathName();
		PathName.MakeUpper();
		OpenDocumentFile(PathName);
		nIndex = (int)FileDlg.m_ofn.nFilterIndex;
	}
}


/////////////////////////////////////////////////////////////////////////////
// CImageProApp message handlers

