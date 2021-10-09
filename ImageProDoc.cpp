// ImageProDoc.cpp : implementation of the CImageProDoc class
//

#include "stdafx.h"
#include "ImagePro.h"

#include "ImageProDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CImageProDoc

IMPLEMENT_DYNCREATE(CImageProDoc, CDocument)

BEGIN_MESSAGE_MAP(CImageProDoc, CDocument)
	//{{AFX_MSG_MAP(CImageProDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CImageProDoc construction/destruction

CImageProDoc::CImageProDoc()
{
	// TODO: add one-time construction code here

}

CImageProDoc::~CImageProDoc()
{
}

BOOL CImageProDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CImageProDoc serialization

void CImageProDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CImageProDoc diagnostics

#ifdef _DEBUG
void CImageProDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CImageProDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CImageProDoc commands
