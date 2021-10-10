// ImageProDoc.cpp : implementation of the CImageProDoc class
//

#include "stdafx.h"
#include "ImagePro.h"
#include "MainFrm.h"
#include "ImageProDoc.h"
#include "ChildFrm.h"
#include "ImageProView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//�ⲿ��������
extern char szFilter[];
/////////////////////////////////////////////////////////////////////////////
// CImageProDoc

IMPLEMENT_DYNCREATE(CImageProDoc, CDocument)

BEGIN_MESSAGE_MAP(CImageProDoc, CDocument)
	ON_COMMAND(ID_FILE_NEW, OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_COMMAND(ID_FILE_SAVE_AS, OnFileSaveAs)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CImageProDoc construction/destruction

CImageProDoc::CImageProDoc()
{
	// TODO: add one-time construction code here
	m_pDibObject = NULL;
	m_bImageLoaded = FALSE;
}

CImageProDoc::~CImageProDoc()
{
	if (m_pDibObject != NULL)
	{
		delete m_pDibObject;
		m_pDibObject = NULL;
	}
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
void CImageProDoc::OnFileOpen()
{
	static int nIndex = 1;

	CFileDialog FileDlg(TRUE, NULL, NULL, OFN_HIDEREADONLY, szFilter);
	FileDlg.m_ofn.nFilterIndex = (DWORD)nIndex;

	if (FileDlg.DoModal() == IDOK)
	{
		CString strPathName = FileDlg.GetPathName();
		AfxGetApp()->OpenDocumentFile(strPathName);
		nIndex = (int)FileDlg.m_ofn.nFilterIndex;
		if (!ReadImgToDoc())
		{
			AfxMessageBox("�޷�����ͼ���ļ���");
			return;
		}
	}
}
void CImageProDoc::OnFileNew()
{
}
void CImageProDoc::OnFileSaveAs()
{
	static int nIndex = 1;

	CFileDialog DialogSaveAs(FALSE, NULL, m_pDibObject->GetImageName(),OFN_HIDEREADONLY, szFilter);

	DialogSaveAs.m_ofn.nFilterIndex = (DWORD)nIndex;

	if (DialogSaveAs.DoModal() == IDOK)
	{
		CMainFrame *pMainFrame = (CMainFrame *)AfxGetMainWnd();
		CChildFrame *pChildFrame = (CChildFrame *)pMainFrame->MDIGetActive();
		CImageProView *pImageProView = (CImageProView *)pChildFrame->GetActiveView();

		nIndex = (int)DialogSaveAs.m_ofn.nFilterIndex;

		//��JPEG��ʽͼ���ж����Ƿ�Ϊ24λ���ɫͼ��
		if (nIndex == 5)
		{
			if (m_pDibObject->GetNumBits() != 24)
			{
				AfxMessageBox("ֻ��24λ���ɫͼ����ܴ�ΪJPEG��ʽ��");
				return;
			}
		}

		//CDibObject������ͼ����ܱ���
		if (m_pDibObject != NULL)
		{
			//��ȡָ�����ļ�������·������
			CString strPathName = DialogSaveAs.GetPathName();
			int nFindIndex = strPathName.Find(".");

			if (nFindIndex != -1)
				strPathName = strPathName.Left(nFindIndex);

			//����ָ������չ���Թ��������ļ���
			strPathName += CDibObject::szExtensions[nIndex - 1];//����չ��
			//����CDibObject���Ա����Save()����ͼ��
			m_pDibObject->Save(strPathName);

			//���ô��ڱ���
			CString strFileName = DialogSaveAs.GetFileName();
			nFindIndex = strFileName.Find(".");

			if (nFindIndex != -1)
				strFileName = strFileName.Left(nFindIndex);

			strFileName += CDibObject::szExtensions[nIndex - 1];
			pChildFrame->SetWindowText(strFileName);

			SetPathName(strPathName);

			//������ΪJPEGͼ�����½�ͼ�����벢������ͼ����ʾ
			if (nIndex == 5)
			{
				m_pDibObject->Load(strPathName);
				pImageProView->InvalidateRect(NULL, FALSE);
				pImageProView->UpdateWindow();
			}
		}
	}
}

//��ͼ���ļ�����CDibObject�������
BOOL CImageProDoc::ReadImgToDoc()
{
	CString strPathName = GetPathName();

	//���õȴ����
	BeginWaitCursor();
	m_pDibObject = new CDibObject(strPathName.GetBuffer(3));//CString object allocate 3 bytes memory
	//ȡ���ȴ����
	EndWaitCursor();

	//����ͼ���ļ�ʧ��
	if (m_pDibObject == NULL)
	{
		AfxMessageBox("�޷�����ͼ�������");
		//����FALSE
		return(FALSE);
	}

	//����ͼ���ļ��ɹ���������Ӧ����
	m_bImageLoaded = TRUE;

	//����TRUE
	return(TRUE);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CImageProDoc commands
