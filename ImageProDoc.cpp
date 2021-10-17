// ImageProDoc.cpp : implementation of the CImageProDoc class
//

#include "stdafx.h"
#include "ImagePro.h"
#include "MainFrm.h"
#include "ImageProDoc.h"
#include "ChildFrm.h"
#include "ImageProView.h"
#include "resource.h"
#include "PointPro.h"
#include "AreaPro.h"

#include "DlgLinerPara.h"
#include "DlgPointStre.h"
#include "DlgThreshold.h"
#include "DlgSmooth.h"
#include "EdgeDetDlg.h"
#include "LapDlg.h"
#include "CDlgColor.h"
#include "DlgMedianFilter.h"

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

	//PointPro
	ON_COMMAND(ID_POINT_EQUA, OnPointEqua)
	ON_COMMAND(ID_POINT_LINER, OnPointLiner)
	ON_COMMAND(ID_POINT_STRE, OnPointStre)
	ON_COMMAND(ID_POINT_CONTAUTO, OnPointContauto)
	ON_COMMAND(ID_POINT_THRE, OnPointThre)
	ON_COMMAND(ID_POINT_INVERT, OnPointInvert)
	ON_COMMAND(ID_CHANNEL_SPLIT, OnChannelSplit)
	ON_UPDATE_COMMAND_UI(ID_CHANNEL_SPLIT, OnUpdateChannelSplit)
	ON_COMMAND(ID_CHANNEL_BLUE, OnChannelBlue)
	ON_UPDATE_COMMAND_UI(ID_CHANNEL_BLUE, OnUpdateChannelBlue)
	ON_COMMAND(ID_CHANNEL_GREEN, OnChannelGreen)
	ON_COMMAND(ID_CHANNEL_MIXED, OnChannelMixed)
	ON_COMMAND(ID_CHANNEL_RED, OnChannelRed)
	ON_UPDATE_COMMAND_UI(ID_CHANNEL_GREEN, OnUpdateChannelGreen)
	ON_UPDATE_COMMAND_UI(ID_CHANNEL_MIXED, OnUpdateChannelMixed)
	ON_UPDATE_COMMAND_UI(ID_CHANNEL_RED, OnUpdateChannelRed)
	ON_UPDATE_COMMAND_UI(ID_POINT_THRE, OnUpdatePointThre)
	ON_UPDATE_COMMAND_UI(ID_POINT_PSEUDOCOLOR, OnUpdatePointPseudocolor)
	
	//AreaPro
	ON_COMMAND(ID_ENHA_SMOOTH, OnEnhaSmooth)
	//ON_COMMAND(ID_POINT_PSEUDOCOLOR, OnPointPseudocolor)
	ON_COMMAND(ID_ENHA_GRADSHARP, OnEnhaGradsharp)
	ON_COMMAND(ID_ENHA_TEMPSHARP, OnEnhaTempsharp)
	ON_COMMAND(ID_ENHA_LAPSHARP, OnEnhaLapsharp)
	ON_COMMAND(ID_ENHA_MidianF, OnEnhaMidianF)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CImageProDoc construction/destruction

CImageProDoc::CImageProDoc()
{
	// TODO: add one-time construction code here
	m_pDibObject = NULL;
	m_bImageLoaded = FALSE;
	m_nColorIndex = 0;
	m_bBinary = FALSE;
	m_lHeight = 0;
	m_lWidth = 0;
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

//////////////////////////////////////////////////////////////////////////
//PointPro
void CImageProDoc::OnPointEqua()//ͼ����⻯
{
	//�����㴦��CPointPro�����
	CPointPro PointOperation(m_pDibObject);

	// ���Ĺ����״
	BeginWaitCursor();

	// ����LinerTrans()�����������Ա任
	PointOperation.InteEqualize(m_pDibObject);

	// ��������
	//SetModifiedFlag(TRUE);

	// ������ͼ
	UpdateAllViews(NULL);

	// �ָ����
	EndWaitCursor();
}

void CImageProDoc::OnPointLiner()
{
	// �����Ի���
	CDlgLinerPara dlgPara;

	// ���Ա任��б��
	float fa;
	// ���Ա任�Ľؾ�
	float fb;

	// ��ʼ������ֵ
	dlgPara.m_fA = 1.0;
	dlgPara.m_fB = 0.0;

	// ��ʾ�Ի�����ʾ�û��趨ƽ����
	if (dlgPara.DoModal() != IDOK)
	{
		// ����
		return;
	}

	// ��ȡ�û��趨��ƽ����
	fa = dlgPara.m_fA;
	fb = dlgPara.m_fB;

	// ɾ���Ի���
	delete dlgPara;

	//�����㴦��CPointPro�����
	CPointPro PointOperation(m_pDibObject);

	// ���Ĺ����״
	BeginWaitCursor();
	// ����LinerTrans()�����������Ա任
	PointOperation.LinerTran(fa, fb);

	// ��������
	//SetModifiedFlag(TRUE);

	// ������ͼ
	UpdateAllViews(NULL);

	// �ָ����
	EndWaitCursor();
}

void CImageProDoc::OnPointStre()
{
	// �����Ի���
	CDlgPointStre dlgPara;

	// ��1����
	BYTE	bX1;
	BYTE	bY1;

	// ��2����
	BYTE	bX2;
	BYTE	bY2;

	// ��ʼ������ֵ
	dlgPara.m_bX1 = 50;
	dlgPara.m_bY1 = 30;
	dlgPara.m_bX2 = 200;
	dlgPara.m_bY2 = 220;

	// ��ʾ�Ի�����ʾ�û��趨����λ��
	if (dlgPara.DoModal() != IDOK)
	{
		// ����
		return;
	}

	// ��ȡ�û����趨
	bX1 = dlgPara.m_bX1;
	bY1 = dlgPara.m_bY1;
	bX2 = dlgPara.m_bX2;
	bY2 = dlgPara.m_bY2;

	// ɾ���Ի���
	delete dlgPara;

	//�����㴦��CPointPro�����
	CPointPro PointOperation(m_pDibObject);

	// ���Ĺ����״
	BeginWaitCursor();
	// ����LinerTrans()�����������Ա任
	PointOperation.GrayStretch(bX1, bY1, bX2, bY2);

	// ��������
	//SetModifiedFlag(TRUE);

	// ������ͼ
	UpdateAllViews(NULL);

	// �ָ����
	EndWaitCursor();
}

void CImageProDoc::OnPointContauto()
{
	//�жϵ�ǰ�Ƿ���ͼ�����
	if (m_pDibObject == NULL) return;

	//�ڵ㴦��CPointPro���д�����������ֱ��ͼ������
	CPointPro PointOperation(m_pDibObject);

	int *pHistogram = PointOperation.GetHistogram();

	int i = 0;
	int nGrayMax = 0, nGrayMin = 0;

	//������С�Ҷ�ֵ
	while (nGrayMin == 0 && i <= 255)
	{
		if (pHistogram[i] >= 100)
		{
			nGrayMin = i;
		}
		i++;
	}

	//�������Ҷ�ֵ
	i = 255;
	while (nGrayMax == 0 && i >= 0)
	{
		if (pHistogram[i] >= 100)
		{
			nGrayMax = i;
		}
		i--;
	}

	delete[] pHistogram;

	// ���Ĺ����״
	BeginWaitCursor();
	// ����LinerTrans()�����������Ա任
	PointOperation.GrayStretch(nGrayMin, 0, nGrayMax, 255);

	// ��������
	//SetModifiedFlag(TRUE);

	// ������ͼ
	UpdateAllViews(NULL);

	// �ָ����
	EndWaitCursor();
}

void CImageProDoc::OnPointThre()
{
	//�жϵ�ǰ�Ƿ���ͼ�����
	if (m_pDibObject == NULL) return;

	//�ڵ㴦��CPointPro���д�����������ֱ��ͼ������
	CPointPro PointOperation(m_pDibObject);

	int *pHistogram = PointOperation.GetHistogram();

	//����һ���Ի���CHistDlg���ʵ��
	CDlgThreshold ThreshDlg;

	//������ֱ��ͼ�����ݴ��ݸ�CHistDlg�Ի�����Ĺ��г�Ա����m_pnHistogram
	if (pHistogram != NULL)
	{
		ThreshDlg.m_pnHistogram = pHistogram;
		//���õ�ǰ����ֵΪ0��������
		//HistDlg.m_nCurrentPiexsNum = pHistogram[0];

	}

	// ���Ĺ����״
	BeginWaitCursor();

	ThreshDlg.m_nThresholdAutoAdjust = PointOperation.FindThresh_Discriminant();

	ThreshDlg.m_nThresholdDiscriminant = PointOperation.FindThresh_Auto();

	ThreshDlg.m_nCurrentThreshold = ThreshDlg.m_nThresholdAutoAdjust;

	//��ʾ�Ի���
	if (ThreshDlg.DoModal() != IDOK)
		return;

	delete[] pHistogram;



	// ����Threshold()����������ֵ������
	PointOperation.Threshold(ThreshDlg.m_nCurrentThreshold,
		ThreshDlg.m_nThresholdType, TRUE);

	m_bBinary = TRUE;

	// ��������
	//SetModifiedFlag(TRUE);

	// ������ͼ
	UpdateAllViews(NULL);

	// �ָ����
	EndWaitCursor();
}

void CImageProDoc::OnPointInvert()
{
	//�����㴦��CPointPro�����
	CPointPro PointOperation(m_pDibObject);

	BeginWaitCursor();
	//��ɫ��fa=-1.0��fb=255
	PointOperation.LinerTran(-1.0, 255);
	EndWaitCursor();

	//SetModifiedFlag(TRUE);

	UpdateAllViews(NULL);
}

void CImageProDoc::OnChannelSplit()
{
	//��ȡӦ�ó�����ָ�� modify
	CImageProApp *pApp = (CImageProApp*)AfxGetApp();
	//��ȡ����ܴ���ָ��
	CMainFrame *pFrame = (CMainFrame*)AfxGetMainWnd();

	CDibObject *red = new CDibObject(m_pDibObject);
	CDibObject *green = new CDibObject(m_pDibObject);
	CDibObject *blue = new CDibObject(m_pDibObject);

	//�����㴦��CPointPro�����
	CPointPro PointOperation(m_pDibObject);

	// ���Ĺ����״
	BeginWaitCursor();

	if (PointOperation.SplitChannels(red, green, blue))
	{
		POSITION posTemplate = pApp->GetFirstDocTemplatePosition();
		CDocTemplate* pDocTemplate = pApp->GetNextDocTemplate(posTemplate);

		CImageProDoc* pDocument = (CImageProDoc*)pDocTemplate->OpenDocumentFile(NULL);
		pDocument->m_pDibObject = red;
		pDocument->m_bImageLoaded = TRUE;
		CString sSplitCaption;
		AfxFormatString1(sSplitCaption, IDS_SPLIT_RED_CAPTION, GetTitle());
		pDocument->SetTitle(sSplitCaption);

		//��ȡ�ӿ�ܴ���ָ��
		CChildFrame *pChild = (CChildFrame *)pFrame->MDIGetActive();
		pChild->m_nWidth = m_pDibObject->GetWidth();
		pChild->m_nHeight = m_pDibObject->GetHeight();
		pChild->SetWindowPos(NULL, 0, 0, pChild->m_nWidth + 12,
			pChild->m_nHeight + 38, SWP_NOZORDER | SWP_NOMOVE);

		pDocument->UpdateAllViews(NULL);

		pDocument = (CImageProDoc*)pDocTemplate->OpenDocumentFile(NULL);
		pDocument->m_pDibObject = green;
		pDocument->m_bImageLoaded = TRUE;
		AfxFormatString1(sSplitCaption, IDS_SPLIT_GREEN_CAPTION, GetTitle());
		pDocument->SetTitle(sSplitCaption);

		//��ȡ�ӿ�ܴ���ָ��
		pChild = (CChildFrame *)pFrame->MDIGetActive();
		pChild->m_nWidth = m_pDibObject->GetWidth();
		pChild->m_nHeight = m_pDibObject->GetHeight();
		pChild->SetWindowPos(NULL, 0, 0, pChild->m_nWidth + 12,
			pChild->m_nHeight + 38, SWP_NOZORDER | SWP_NOMOVE);

		pDocument->UpdateAllViews(NULL);

		pDocument = (CImageProDoc*)pDocTemplate->OpenDocumentFile(NULL);
		pDocument->m_pDibObject = blue;
		pDocument->m_bImageLoaded = TRUE;
		AfxFormatString1(sSplitCaption, IDS_SPLIT_BLUE_CAPTION, GetTitle());
		pDocument->SetTitle(sSplitCaption);

		//��ȡ�ӿ�ܴ���ָ��
		pChild = (CChildFrame *)pFrame->MDIGetActive();
		pChild->m_nWidth = m_pDibObject->GetWidth();
		pChild->m_nHeight = m_pDibObject->GetHeight();
		pChild->SetWindowPos(NULL, 0, 0, pChild->m_nWidth + 12,
			pChild->m_nHeight + 38, SWP_NOZORDER | SWP_NOMOVE);

		pDocument->UpdateAllViews(NULL);

	}
	else
		AfxMessageBox(IDS_FAIL_SPLIT);

	// �ָ����
	EndWaitCursor();

}

void CImageProDoc::OnChannelBlue()
{
	//��ȡӦ�ó�����ָ��
	CImageProApp *pApp = (CImageProApp*)AfxGetApp();
	//��ȡ����ܴ���ָ��
	CMainFrame *pFrame = (CMainFrame*)AfxGetMainWnd();

	CDibObject *blue = new CDibObject(m_pDibObject);

	//�����㴦��CPointPro�����
	CPointPro PointOperation(m_pDibObject);

	// ���Ĺ����״
	BeginWaitCursor();

	if (PointOperation.GetMixedChannel(blue))
	{
		POSITION posTemplate = pApp->GetFirstDocTemplatePosition();
		CDocTemplate* pDocTemplate = pApp->GetNextDocTemplate(posTemplate);

		CImageProDoc* pDocument = (CImageProDoc*)pDocTemplate->OpenDocumentFile(NULL);
		pDocument->m_pDibObject = blue;
		pDocument->m_bImageLoaded = TRUE;
		CString sSplitCaption;
		AfxFormatString1(sSplitCaption, IDS_SPLIT_BLUE_CAPTION, GetTitle());
		pDocument->SetTitle(sSplitCaption);

		//��ȡ�ӿ�ܴ���ָ��
		CChildFrame *pChild = (CChildFrame *)pFrame->MDIGetActive();
		pChild->m_nWidth = m_pDibObject->GetWidth();
		pChild->m_nHeight = m_pDibObject->GetHeight();
		pChild->SetWindowPos(NULL, 0, 0, pChild->m_nWidth + 12,
			pChild->m_nHeight + 38, SWP_NOZORDER | SWP_NOMOVE);

		pDocument->UpdateAllViews(NULL);

	}
	else
		AfxMessageBox(IDS_FAIL_SPLIT);

	// �ָ����
	EndWaitCursor();
}

void CImageProDoc::OnChannelGreen()
{
	CImageProApp *pApp = (CImageProApp*)AfxGetApp();
	//��ȡ����ܴ���ָ��
	CMainFrame *pFrame = (CMainFrame*)AfxGetMainWnd();

	CDibObject *green = new CDibObject(m_pDibObject);

	//�����㴦��CPointPro�����
	CPointPro PointOperation(m_pDibObject);

	// ���Ĺ����״
	BeginWaitCursor();

	if (PointOperation.GetMixedChannel(green))
	{
		POSITION posTemplate = pApp->GetFirstDocTemplatePosition();
		CDocTemplate* pDocTemplate = pApp->GetNextDocTemplate(posTemplate);

		CImageProDoc* pDocument = (CImageProDoc*)pDocTemplate->OpenDocumentFile(NULL);
		pDocument->m_pDibObject = green;
		pDocument->m_bImageLoaded = TRUE;
		CString sSplitCaption;
		AfxFormatString1(sSplitCaption, IDS_SPLIT_GREEN_CAPTION, GetTitle());
		pDocument->SetTitle(sSplitCaption);

		//��ȡ�ӿ�ܴ���ָ��
		CChildFrame *pChild = (CChildFrame *)pFrame->MDIGetActive();
		pChild->m_nWidth = m_pDibObject->GetWidth();
		pChild->m_nHeight = m_pDibObject->GetHeight();
		pChild->SetWindowPos(NULL, 0, 0, pChild->m_nWidth + 12,
			pChild->m_nHeight + 38, SWP_NOZORDER | SWP_NOMOVE);

		pDocument->UpdateAllViews(NULL);

	}
	else
		AfxMessageBox(IDS_FAIL_SPLIT);

	// �ָ����
	EndWaitCursor();
}

void CImageProDoc::OnChannelMixed()
{
	CImageProApp *pApp = (CImageProApp*)AfxGetApp();
	//��ȡ����ܴ���ָ��
	CMainFrame *pFrame = (CMainFrame*)AfxGetMainWnd();

	CDibObject *mixed = new CDibObject(m_pDibObject);

	//�����㴦��CPointPro�����
	CPointPro PointOperation(m_pDibObject);

	// ���Ĺ����״
	BeginWaitCursor();

	if (PointOperation.GetMixedChannel(mixed))
	{
		POSITION posTemplate = pApp->GetFirstDocTemplatePosition();
		CDocTemplate* pDocTemplate = pApp->GetNextDocTemplate(posTemplate);

		CImageProDoc* pDocument = (CImageProDoc*)pDocTemplate->OpenDocumentFile(NULL);
		pDocument->m_pDibObject = mixed;
		pDocument->m_bImageLoaded = TRUE;
		CString sSplitCaption;
		AfxFormatString1(sSplitCaption, IDS_SPLIT_MIXED_CAPTION, GetTitle());
		pDocument->SetTitle(sSplitCaption);

		//��ȡ�ӿ�ܴ���ָ��
		CChildFrame *pChild = (CChildFrame *)pFrame->MDIGetActive();
		pChild->m_nWidth = m_pDibObject->GetWidth();
		pChild->m_nHeight = m_pDibObject->GetHeight();
		pChild->SetWindowPos(NULL, 0, 0, pChild->m_nWidth + 12,
			pChild->m_nHeight + 38, SWP_NOZORDER | SWP_NOMOVE);

		pDocument->UpdateAllViews(NULL);

	}
	else
		AfxMessageBox(IDS_FAIL_SPLIT);

	// �ָ����
	EndWaitCursor();
}

void CImageProDoc::OnChannelRed()
{
	//��ȡӦ�ó�����ָ��
	CImageProApp *pApp = (CImageProApp*)AfxGetApp();
	//��ȡ����ܴ���ָ��
	CMainFrame *pFrame = (CMainFrame*)AfxGetMainWnd();

	CDibObject *red = new CDibObject(m_pDibObject);

	//�����㴦��CPointPro�����
	CPointPro PointOperation(m_pDibObject);

	// ���Ĺ����״
	BeginWaitCursor();

	if (PointOperation.GetMixedChannel(red))
	{
		POSITION posTemplate = pApp->GetFirstDocTemplatePosition();
		CDocTemplate* pDocTemplate = pApp->GetNextDocTemplate(posTemplate);

		CImageProDoc* pDocument = (CImageProDoc*)pDocTemplate->OpenDocumentFile(NULL);
		pDocument->m_pDibObject = red;
		pDocument->m_bImageLoaded = TRUE;
		CString sSplitCaption;
		AfxFormatString1(sSplitCaption, IDS_SPLIT_RED_CAPTION, GetTitle());
		pDocument->SetTitle(sSplitCaption);

		//��ȡ�ӿ�ܴ���ָ��
		CChildFrame *pChild = (CChildFrame *)pFrame->MDIGetActive();
		pChild->m_nWidth = m_pDibObject->GetWidth();
		pChild->m_nHeight = m_pDibObject->GetHeight();
		pChild->SetWindowPos(NULL, 0, 0, pChild->m_nWidth + 12,
			pChild->m_nHeight + 38, SWP_NOZORDER | SWP_NOMOVE);

		pDocument->UpdateAllViews(NULL);

	}
	else
		AfxMessageBox(IDS_FAIL_SPLIT);

	// �ָ����
	EndWaitCursor();
}

void CImageProDoc::OnUpdateChannelGreen(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_pDibObject->GetNumBits() >= 8);
}

void CImageProDoc::OnUpdateChannelMixed(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_pDibObject->GetNumBits() >= 8);
}

void CImageProDoc::OnUpdateChannelRed(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_pDibObject->GetNumBits() >= 8);
}

void CImageProDoc::OnUpdateChannelBlue(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_pDibObject->GetNumBits() >= 8);
}

void CImageProDoc::OnUpdateChannelSplit(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_pDibObject->GetNumBits() >= 8);
}

void CImageProDoc::OnUpdatePointThre(CCmdUI* pCmdUI)
{
	//�жϵ�ǰ�Ƿ���ͼ�����
	if (m_pDibObject == NULL) return;

	//�ڵ㴦��CPointPro���д�����������ֱ��ͼ������
	CPointPro PointOperation(m_pDibObject);

	//�ж��Ƿ�Ϊ256ɫ�Ҷ�ͼ��
	pCmdUI->Enable(PointOperation.IsGray256());
}

void CImageProDoc::OnUpdatePointPseudocolor(CCmdUI* pCmdUI)
{
	//�жϵ�ǰ�Ƿ���ͼ�����
	if (m_pDibObject == NULL) return;

	//�ڵ㴦��CPointPro���д�����������ֱ��ͼ������
	CPointPro PointOperation(m_pDibObject);

	//�ж��Ƿ�Ϊ256ɫ�Ҷ�ͼ��
	pCmdUI->Enable(PointOperation.IsGray256());
}

//////////////////////////////////////////////////////////////////////////
//AreaPro
void CImageProDoc::OnEnhaSmooth()
{
	// ģ��߶�
	int	nTempH;

	// ģ����
	int nTempW;

	// ģ��ϵ��
	float fTempC;

	// ģ������Ԫ��X����
	int nTempXc;

	// ģ������Ԫ��Y����
	int nTempYc;

	// ģ��Ԫ������
	float aValue[25];

	// �����Ի���
	CDlgSmooth dlgPara;

	// ��ģ�����鸳��ֵ��Ϊƽ��ģ�壩
	aValue[0] = 1.0;
	aValue[1] = 1.0;
	aValue[2] = 1.0;
	aValue[3] = 1.0;
	aValue[4] = 1.0;
	aValue[5] = 1.0;
	aValue[6] = 1.0;
	aValue[7] = 1.0;
	aValue[8] = 1.0;
	aValue[9] = 1.0;
	aValue[10] = 1.0;
	aValue[11] = 1.0;
	aValue[12] = 1.0;
	aValue[13] = 1.0;
	aValue[14] = 1.0;
	aValue[15] = 1.0;
	aValue[16] = 1.0;
	aValue[17] = 1.0;
	aValue[18] = 1.0;
	aValue[19] = 1.0;
	aValue[20] = 1.0;
	aValue[21] = 1.0;
	aValue[22] = 1.0;
	aValue[23] = 1.0;
	aValue[24] = 1.0;

	// ��ʼ���Ի������ֵ
	dlgPara.m_intType = 0;
	dlgPara.m_iTempH = 3;
	dlgPara.m_iTempW = 3;
	dlgPara.m_iTempMX = 1;
	dlgPara.m_iTempMY = 1;
	dlgPara.m_fTempC = 9.0;
	dlgPara.m_fpArray = aValue;

	// ��ʾ�Ի�����ʾ�û��趨ƽ����
	if (dlgPara.DoModal() != IDOK)
	{
		// ����
		return;
	}

	// ��ȡ�û��趨��ƽ����
	nTempH = dlgPara.m_iTempH;
	nTempW = dlgPara.m_iTempW;
	nTempXc = dlgPara.m_iTempMX;
	nTempYc = dlgPara.m_iTempMY;
	fTempC = dlgPara.m_fTempC;

	// ɾ���Ի���
	delete dlgPara;

	//�����㴦��CPointPro�����
	CAreaPro AreaPro(m_pDibObject);
	// ���Ĺ����״
	BeginWaitCursor();

	AreaPro.Smoothing(aValue, fTempC, nTempW, nTempH, nTempXc, nTempYc);

	// �ָ����
	EndWaitCursor();

	// ������ͼ
	UpdateAllViews(NULL);

}

//void CImageProDoc::OnPointPseudocolor()
//{
//	CDlgColor dlgPara;
//
//	// ��ʼ������ֵ
//	if (m_nColorIndex >= 0)
//	{
//		// ��ʼѡ�е�ǰ��α��ɫ
//		dlgPara.m_nColor = m_nColorIndex;
//	}
//	else
//	{
//		// ��ʼѡ�лҶ�α��ɫ�����
//		dlgPara.m_nColor = 0;
//	}
//
//	// ָ�����������ָ��
//	dlgPara.m_lpColorName = (LPSTR)ColorScaleName;
//
//	// α��ɫ������Ŀ
//	dlgPara.m_nColorCount = COLOR_SCALE_COUNT;
//
//	// �����ַ�������
//	dlgPara.m_nNameLen = sizeof(ColorScaleName) / COLOR_SCALE_COUNT;
//
//	// ��ʾ�Ի�����ʾ�û��趨ƽ����
//	if (dlgPara.DoModal() != IDOK)
//	{
//		// ����
//		return;
//	}
//
//	// ��ȡ�û����趨
//	int nColor = dlgPara.m_nColor;
//
//	// ɾ���Ի���
//	delete dlgPara;
//
//	CPointPro PointOperation(m_pDibObject);
//
//	// ���Ĺ����״
//	BeginWaitCursor();
//
//	// ����ReplaceColorPal()�����任��ɫ��
//	PointOperation.ReplaceColorPal(m_pDibObject, (BYTE*)ColorsTable[nColor]);
//
//	// ��������
//	//SetModifiedFlag(TRUE);
//
//	// ������ͼ
//	UpdateAllViews(NULL);
//
//	// �ָ����
//	EndWaitCursor();
//
//}

void CImageProDoc::OnEnhaGradsharp()
{
	//�����㴦��CPointPro�����
	CAreaPro AreaPro(m_pDibObject);
	// ���Ĺ����״
	BeginWaitCursor();

	AreaPro.GradSharping(10);

	// �ָ����
	EndWaitCursor();

	// ������ͼ
	UpdateAllViews(NULL);
}


void CImageProDoc::OnEnhaMidianF()
{
	// ���ڸ߶�
	int	nTempH;

	// ���ڿ��
	int nTempW;

	// ��������Ԫ��X����
	int nTempXc;

	// ��������Ԫ��Y����
	int nTempYc;

	// ����ʽ
	int nSortType;


	// �����Ի���
	CDlgMedianFilter dlgPara;

	// ��ʼ���Ի������ֵ
	dlgPara.m_nWindowSize = 0;
	dlgPara.m_iTempH = 3;
	dlgPara.m_iTempW = 3;
	dlgPara.m_iTempMX = 1;
	dlgPara.m_iTempMY = 1;
	dlgPara.m_nSortType = 0;

	// ��ʾ�Ի�����ʾ�û��趨ƽ����
	if (dlgPara.DoModal() != IDOK)
	{
		// ����
		return;
	}

	// ��ȡ�û��趨��ƽ����
	nTempH = dlgPara.m_iTempH;
	nTempW = dlgPara.m_iTempW;
	nTempXc = dlgPara.m_iTempMX;
	nTempYc = dlgPara.m_iTempMY;
	nSortType = dlgPara.m_nSortType;


	// ɾ���Ի���
	delete dlgPara;

	//�����㴦��CPointPro�����
	CAreaPro AreaPro(m_pDibObject);
	// ���Ĺ����״
	BeginWaitCursor();

	AreaPro.MedianFilter(nSortType, nTempW, nTempH, nTempXc, nTempYc);

	// �ָ����
	EndWaitCursor();

	// ������ͼ
	UpdateAllViews(NULL);
}

void CImageProDoc::OnEnhaTempsharp()
{
	// ģ��߶�
	int	nTempH;

	// ģ����
	int nTempW;

	// ģ��ϵ��
	float fTempC;

	// ģ������Ԫ��X����
	int nTempXc;

	// ģ������Ԫ��Y����
	int nTempYc;

	// ģ��Ԫ������
	float aValue[25];

	// ����Ԫ������
	int nDirection[8];

	// �����Ի���
	CEdgeDetDlg dlgPara;

	// ��ģ�����鸳��ֵ��ΪRobertsģ��1��
	aValue[0] = -1.0;
	aValue[1] = 0.0;
	aValue[2] = 0.0;
	aValue[3] = 0.0;
	aValue[4] = 0.0;
	aValue[5] = 0.0;
	aValue[6] = 1.0;
	aValue[7] = 0.0;
	aValue[8] = 0.0;
	aValue[9] = 0.0;
	aValue[10] = 0.0;
	aValue[11] = 0.0;
	aValue[12] = 0.0;
	aValue[13] = 0.0;
	aValue[14] = 0.0;
	aValue[15] = 0.0;
	aValue[16] = 0.0;
	aValue[17] = 0.0;
	aValue[18] = 0.0;
	aValue[19] = 0.0;
	aValue[20] = 0.0;
	aValue[21] = 0.0;
	aValue[22] = 0.0;
	aValue[23] = 0.0;
	aValue[24] = 0.0;

	// ��ʼ���Ի������ֵ
	dlgPara.m_intType = 0;
	dlgPara.m_iTempH = 3;
	dlgPara.m_iTempW = 3;
	dlgPara.m_iTempMX = 1;
	dlgPara.m_iTempMY = 1;
	dlgPara.m_fTempC = 1.0;
	dlgPara.m_fpArray = aValue;
	dlgPara.m_nDirection = nDirection;

	// ��ʾ�Ի�����ʾ�û��趨ƽ����
	if (dlgPara.DoModal() != IDOK)
	{
		// ����
		return;
	}

	// ��ȡ�û��趨��ƽ����
	nTempH = dlgPara.m_iTempH;
	nTempW = dlgPara.m_iTempW;
	nTempXc = dlgPara.m_iTempMX;
	nTempYc = dlgPara.m_iTempMY;
	fTempC = dlgPara.m_fTempC;


	// ɾ���Ի���
	delete dlgPara;

	//�����㴦��CPointPro�����
	CAreaPro AreaPro(m_pDibObject);
	// ���Ĺ����״
	BeginWaitCursor();

	AreaPro.Sharping(aValue, nDirection);

	// �ָ����
	EndWaitCursor();

	// ������ͼ
	UpdateAllViews(NULL);

}

void CImageProDoc::OnEnhaLapsharp()
{
	// ģ��߶�
	int	nTempH;

	// ģ����
	int nTempW;

	// ģ��ϵ��
	float fTempC;

	// ģ������Ԫ��X����
	int nTempXc;

	// ģ������Ԫ��Y����
	int nTempYc;

	// ģ��Ԫ������
	float aValue[25];

	// �����Ի���
	CLapDlg dlgPara;

	// ��ģ�����鸳��ֵ��Ϊƽ��ģ�壩
	aValue[0] = -1.0;
	aValue[1] = -1.0;
	aValue[2] = -1.0;
	aValue[3] = 0.0;
	aValue[4] = 0.0;
	aValue[5] = -1.0;
	aValue[6] = 9.0;
	aValue[7] = -1.0;
	aValue[8] = 0.0;
	aValue[9] = 0.0;
	aValue[10] = -1.0;
	aValue[11] = -1.0;
	aValue[12] = -1.0;
	aValue[13] = 0.0;
	aValue[14] = 0.0;
	aValue[15] = 0.0;
	aValue[16] = 0.0;
	aValue[17] = 0.0;
	aValue[18] = 0.0;
	aValue[19] = 0.0;
	aValue[20] = 0.0;
	aValue[21] = 0.0;
	aValue[22] = 0.0;
	aValue[23] = 0.0;
	aValue[24] = 0.0;

	// ��ʼ���Ի������ֵ
	dlgPara.m_intType = 0;
	dlgPara.m_iTempH = 3;
	dlgPara.m_iTempW = 3;
	dlgPara.m_iTempMX = 1;
	dlgPara.m_iTempMY = 1;
	dlgPara.m_fTempC = 1.0;
	dlgPara.m_fpArray = aValue;

	// ��ʾ�Ի�����ʾ�û��趨ƽ����
	if (dlgPara.DoModal() != IDOK)
	{
		// ����
		return;
	}

	// ��ȡ�û��趨��ƽ����
	nTempH = dlgPara.m_iTempH;
	nTempW = dlgPara.m_iTempW;
	nTempXc = dlgPara.m_iTempMX;
	nTempYc = dlgPara.m_iTempMY;
	fTempC = dlgPara.m_fTempC;

	// ɾ���Ի���
	delete dlgPara;

	//�����㴦��CPointPro�����
	CAreaPro AreaPro(m_pDibObject);
	// ���Ĺ����״
	BeginWaitCursor();

	AreaPro.LapSharping(aValue, fTempC, nTempW, nTempH, nTempXc, nTempYc);

	// �ָ����
	EndWaitCursor();

	// ������ͼ
	UpdateAllViews(NULL);

}



