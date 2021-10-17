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

//外部变量声明
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
			AfxMessageBox("无法载入图像文件！");
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

		//是JPEG格式图像，判断其是否为24位真彩色图像
		if (nIndex == 5)
		{
			if (m_pDibObject->GetNumBits() != 24)
			{
				AfxMessageBox("只有24位真彩色图像才能存为JPEG格式！");
				return;
			}
		}

		//CDibObject类中有图像才能保存
		if (m_pDibObject != NULL)
		{
			//获取指定的文件名（含路径名）
			CString strPathName = DialogSaveAs.GetPathName();
			int nFindIndex = strPathName.Find(".");

			if (nFindIndex != -1)
				strPathName = strPathName.Left(nFindIndex);

			//加上指定的扩展名以构成完整文件名
			strPathName += CDibObject::szExtensions[nIndex - 1];//加扩展名
			//调用CDibObject类成员函数Save()保存图像
			m_pDibObject->Save(strPathName);

			//设置窗口标题
			CString strFileName = DialogSaveAs.GetFileName();
			nFindIndex = strFileName.Find(".");

			if (nFindIndex != -1)
				strFileName = strFileName.Left(nFindIndex);

			strFileName += CDibObject::szExtensions[nIndex - 1];
			pChildFrame->SetWindowText(strFileName);

			SetPathName(strPathName);

			//若保存为JPEG图像，重新将图像载入并更新视图的显示
			if (nIndex == 5)
			{
				m_pDibObject->Load(strPathName);
				pImageProView->InvalidateRect(NULL, FALSE);
				pImageProView->UpdateWindow();
			}
		}
	}
}

//将图像文件读入CDibObject类对象中
BOOL CImageProDoc::ReadImgToDoc()
{
	CString strPathName = GetPathName();

	//设置等待光标
	BeginWaitCursor();
	m_pDibObject = new CDibObject(strPathName.GetBuffer(3));//CString object allocate 3 bytes memory
	//取消等待光标
	EndWaitCursor();

	//读入图像文件失败
	if (m_pDibObject == NULL)
	{
		AfxMessageBox("无法创建图像类对象！");
		//返回FALSE
		return(FALSE);
	}

	//读入图像文件成功，设置相应变量
	m_bImageLoaded = TRUE;

	//返回TRUE
	return(TRUE);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CImageProDoc commands

//////////////////////////////////////////////////////////////////////////
//PointPro
void CImageProDoc::OnPointEqua()//图像均衡化
{
	//创建点处理CPointPro类对象
	CPointPro PointOperation(m_pDibObject);

	// 更改光标形状
	BeginWaitCursor();

	// 调用LinerTrans()函数进行线性变换
	PointOperation.InteEqualize(m_pDibObject);

	// 设置脏标记
	//SetModifiedFlag(TRUE);

	// 更新视图
	UpdateAllViews(NULL);

	// 恢复光标
	EndWaitCursor();
}

void CImageProDoc::OnPointLiner()
{
	// 创建对话框
	CDlgLinerPara dlgPara;

	// 线性变换的斜率
	float fa;
	// 线性变换的截距
	float fb;

	// 初始化变量值
	dlgPara.m_fA = 1.0;
	dlgPara.m_fB = 0.0;

	// 显示对话框，提示用户设定平移量
	if (dlgPara.DoModal() != IDOK)
	{
		// 返回
		return;
	}

	// 获取用户设定的平移量
	fa = dlgPara.m_fA;
	fb = dlgPara.m_fB;

	// 删除对话框
	delete dlgPara;

	//创建点处理CPointPro类对象
	CPointPro PointOperation(m_pDibObject);

	// 更改光标形状
	BeginWaitCursor();
	// 调用LinerTrans()函数进行线性变换
	PointOperation.LinerTran(fa, fb);

	// 设置脏标记
	//SetModifiedFlag(TRUE);

	// 更新视图
	UpdateAllViews(NULL);

	// 恢复光标
	EndWaitCursor();
}

void CImageProDoc::OnPointStre()
{
	// 创建对话框
	CDlgPointStre dlgPara;

	// 点1坐标
	BYTE	bX1;
	BYTE	bY1;

	// 点2坐标
	BYTE	bX2;
	BYTE	bY2;

	// 初始化变量值
	dlgPara.m_bX1 = 50;
	dlgPara.m_bY1 = 30;
	dlgPara.m_bX2 = 200;
	dlgPara.m_bY2 = 220;

	// 显示对话框，提示用户设定拉伸位置
	if (dlgPara.DoModal() != IDOK)
	{
		// 返回
		return;
	}

	// 获取用户的设定
	bX1 = dlgPara.m_bX1;
	bY1 = dlgPara.m_bY1;
	bX2 = dlgPara.m_bX2;
	bY2 = dlgPara.m_bY2;

	// 删除对话框
	delete dlgPara;

	//创建点处理CPointPro类对象
	CPointPro PointOperation(m_pDibObject);

	// 更改光标形状
	BeginWaitCursor();
	// 调用LinerTrans()函数进行线性变换
	PointOperation.GrayStretch(bX1, bY1, bX2, bY2);

	// 设置脏标记
	//SetModifiedFlag(TRUE);

	// 更新视图
	UpdateAllViews(NULL);

	// 恢复光标
	EndWaitCursor();
}

void CImageProDoc::OnPointContauto()
{
	//判断当前是否有图像对象
	if (m_pDibObject == NULL) return;

	//在点处理CPointPro类中创建用来绘制直方图的数据
	CPointPro PointOperation(m_pDibObject);

	int *pHistogram = PointOperation.GetHistogram();

	int i = 0;
	int nGrayMax = 0, nGrayMin = 0;

	//查找最小灰度值
	while (nGrayMin == 0 && i <= 255)
	{
		if (pHistogram[i] >= 100)
		{
			nGrayMin = i;
		}
		i++;
	}

	//查找最大灰度值
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

	// 更改光标形状
	BeginWaitCursor();
	// 调用LinerTrans()函数进行线性变换
	PointOperation.GrayStretch(nGrayMin, 0, nGrayMax, 255);

	// 设置脏标记
	//SetModifiedFlag(TRUE);

	// 更新视图
	UpdateAllViews(NULL);

	// 恢复光标
	EndWaitCursor();
}

void CImageProDoc::OnPointThre()
{
	//判断当前是否有图像对象
	if (m_pDibObject == NULL) return;

	//在点处理CPointPro类中创建用来绘制直方图的数据
	CPointPro PointOperation(m_pDibObject);

	int *pHistogram = PointOperation.GetHistogram();

	//生成一个对话框CHistDlg类的实例
	CDlgThreshold ThreshDlg;

	//将绘制直方图的数据传递给CHistDlg对话框类的公有成员变量m_pnHistogram
	if (pHistogram != NULL)
	{
		ThreshDlg.m_pnHistogram = pHistogram;
		//设置当前像素值为0的像素数
		//HistDlg.m_nCurrentPiexsNum = pHistogram[0];

	}

	// 更改光标形状
	BeginWaitCursor();

	ThreshDlg.m_nThresholdAutoAdjust = PointOperation.FindThresh_Discriminant();

	ThreshDlg.m_nThresholdDiscriminant = PointOperation.FindThresh_Auto();

	ThreshDlg.m_nCurrentThreshold = ThreshDlg.m_nThresholdAutoAdjust;

	//显示对话框
	if (ThreshDlg.DoModal() != IDOK)
		return;

	delete[] pHistogram;



	// 调用Threshold()函数进行阈值化操作
	PointOperation.Threshold(ThreshDlg.m_nCurrentThreshold,
		ThreshDlg.m_nThresholdType, TRUE);

	m_bBinary = TRUE;

	// 设置脏标记
	//SetModifiedFlag(TRUE);

	// 更新视图
	UpdateAllViews(NULL);

	// 恢复光标
	EndWaitCursor();
}

void CImageProDoc::OnPointInvert()
{
	//创建点处理CPointPro类对象
	CPointPro PointOperation(m_pDibObject);

	BeginWaitCursor();
	//反色：fa=-1.0，fb=255
	PointOperation.LinerTran(-1.0, 255);
	EndWaitCursor();

	//SetModifiedFlag(TRUE);

	UpdateAllViews(NULL);
}

void CImageProDoc::OnChannelSplit()
{
	//获取应用程序类指针 modify
	CImageProApp *pApp = (CImageProApp*)AfxGetApp();
	//获取主框架窗口指针
	CMainFrame *pFrame = (CMainFrame*)AfxGetMainWnd();

	CDibObject *red = new CDibObject(m_pDibObject);
	CDibObject *green = new CDibObject(m_pDibObject);
	CDibObject *blue = new CDibObject(m_pDibObject);

	//创建点处理CPointPro类对象
	CPointPro PointOperation(m_pDibObject);

	// 更改光标形状
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

		//获取子框架窗口指针
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

		//获取子框架窗口指针
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

		//获取子框架窗口指针
		pChild = (CChildFrame *)pFrame->MDIGetActive();
		pChild->m_nWidth = m_pDibObject->GetWidth();
		pChild->m_nHeight = m_pDibObject->GetHeight();
		pChild->SetWindowPos(NULL, 0, 0, pChild->m_nWidth + 12,
			pChild->m_nHeight + 38, SWP_NOZORDER | SWP_NOMOVE);

		pDocument->UpdateAllViews(NULL);

	}
	else
		AfxMessageBox(IDS_FAIL_SPLIT);

	// 恢复光标
	EndWaitCursor();

}

void CImageProDoc::OnChannelBlue()
{
	//获取应用程序类指针
	CImageProApp *pApp = (CImageProApp*)AfxGetApp();
	//获取主框架窗口指针
	CMainFrame *pFrame = (CMainFrame*)AfxGetMainWnd();

	CDibObject *blue = new CDibObject(m_pDibObject);

	//创建点处理CPointPro类对象
	CPointPro PointOperation(m_pDibObject);

	// 更改光标形状
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

		//获取子框架窗口指针
		CChildFrame *pChild = (CChildFrame *)pFrame->MDIGetActive();
		pChild->m_nWidth = m_pDibObject->GetWidth();
		pChild->m_nHeight = m_pDibObject->GetHeight();
		pChild->SetWindowPos(NULL, 0, 0, pChild->m_nWidth + 12,
			pChild->m_nHeight + 38, SWP_NOZORDER | SWP_NOMOVE);

		pDocument->UpdateAllViews(NULL);

	}
	else
		AfxMessageBox(IDS_FAIL_SPLIT);

	// 恢复光标
	EndWaitCursor();
}

void CImageProDoc::OnChannelGreen()
{
	CImageProApp *pApp = (CImageProApp*)AfxGetApp();
	//获取主框架窗口指针
	CMainFrame *pFrame = (CMainFrame*)AfxGetMainWnd();

	CDibObject *green = new CDibObject(m_pDibObject);

	//创建点处理CPointPro类对象
	CPointPro PointOperation(m_pDibObject);

	// 更改光标形状
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

		//获取子框架窗口指针
		CChildFrame *pChild = (CChildFrame *)pFrame->MDIGetActive();
		pChild->m_nWidth = m_pDibObject->GetWidth();
		pChild->m_nHeight = m_pDibObject->GetHeight();
		pChild->SetWindowPos(NULL, 0, 0, pChild->m_nWidth + 12,
			pChild->m_nHeight + 38, SWP_NOZORDER | SWP_NOMOVE);

		pDocument->UpdateAllViews(NULL);

	}
	else
		AfxMessageBox(IDS_FAIL_SPLIT);

	// 恢复光标
	EndWaitCursor();
}

void CImageProDoc::OnChannelMixed()
{
	CImageProApp *pApp = (CImageProApp*)AfxGetApp();
	//获取主框架窗口指针
	CMainFrame *pFrame = (CMainFrame*)AfxGetMainWnd();

	CDibObject *mixed = new CDibObject(m_pDibObject);

	//创建点处理CPointPro类对象
	CPointPro PointOperation(m_pDibObject);

	// 更改光标形状
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

		//获取子框架窗口指针
		CChildFrame *pChild = (CChildFrame *)pFrame->MDIGetActive();
		pChild->m_nWidth = m_pDibObject->GetWidth();
		pChild->m_nHeight = m_pDibObject->GetHeight();
		pChild->SetWindowPos(NULL, 0, 0, pChild->m_nWidth + 12,
			pChild->m_nHeight + 38, SWP_NOZORDER | SWP_NOMOVE);

		pDocument->UpdateAllViews(NULL);

	}
	else
		AfxMessageBox(IDS_FAIL_SPLIT);

	// 恢复光标
	EndWaitCursor();
}

void CImageProDoc::OnChannelRed()
{
	//获取应用程序类指针
	CImageProApp *pApp = (CImageProApp*)AfxGetApp();
	//获取主框架窗口指针
	CMainFrame *pFrame = (CMainFrame*)AfxGetMainWnd();

	CDibObject *red = new CDibObject(m_pDibObject);

	//创建点处理CPointPro类对象
	CPointPro PointOperation(m_pDibObject);

	// 更改光标形状
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

		//获取子框架窗口指针
		CChildFrame *pChild = (CChildFrame *)pFrame->MDIGetActive();
		pChild->m_nWidth = m_pDibObject->GetWidth();
		pChild->m_nHeight = m_pDibObject->GetHeight();
		pChild->SetWindowPos(NULL, 0, 0, pChild->m_nWidth + 12,
			pChild->m_nHeight + 38, SWP_NOZORDER | SWP_NOMOVE);

		pDocument->UpdateAllViews(NULL);

	}
	else
		AfxMessageBox(IDS_FAIL_SPLIT);

	// 恢复光标
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
	//判断当前是否有图像对象
	if (m_pDibObject == NULL) return;

	//在点处理CPointPro类中创建用来绘制直方图的数据
	CPointPro PointOperation(m_pDibObject);

	//判断是否为256色灰度图像
	pCmdUI->Enable(PointOperation.IsGray256());
}

void CImageProDoc::OnUpdatePointPseudocolor(CCmdUI* pCmdUI)
{
	//判断当前是否有图像对象
	if (m_pDibObject == NULL) return;

	//在点处理CPointPro类中创建用来绘制直方图的数据
	CPointPro PointOperation(m_pDibObject);

	//判断是否为256色灰度图像
	pCmdUI->Enable(PointOperation.IsGray256());
}

//////////////////////////////////////////////////////////////////////////
//AreaPro
void CImageProDoc::OnEnhaSmooth()
{
	// 模板高度
	int	nTempH;

	// 模板宽度
	int nTempW;

	// 模板系数
	float fTempC;

	// 模板中心元素X坐标
	int nTempXc;

	// 模板中心元素Y坐标
	int nTempYc;

	// 模板元素数组
	float aValue[25];

	// 创建对话框
	CDlgSmooth dlgPara;

	// 给模板数组赋初值（为平均模板）
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

	// 初始化对话框变量值
	dlgPara.m_intType = 0;
	dlgPara.m_iTempH = 3;
	dlgPara.m_iTempW = 3;
	dlgPara.m_iTempMX = 1;
	dlgPara.m_iTempMY = 1;
	dlgPara.m_fTempC = 9.0;
	dlgPara.m_fpArray = aValue;

	// 显示对话框，提示用户设定平移量
	if (dlgPara.DoModal() != IDOK)
	{
		// 返回
		return;
	}

	// 获取用户设定的平移量
	nTempH = dlgPara.m_iTempH;
	nTempW = dlgPara.m_iTempW;
	nTempXc = dlgPara.m_iTempMX;
	nTempYc = dlgPara.m_iTempMY;
	fTempC = dlgPara.m_fTempC;

	// 删除对话框
	delete dlgPara;

	//创建点处理CPointPro类对象
	CAreaPro AreaPro(m_pDibObject);
	// 更改光标形状
	BeginWaitCursor();

	AreaPro.Smoothing(aValue, fTempC, nTempW, nTempH, nTempXc, nTempYc);

	// 恢复光标
	EndWaitCursor();

	// 更新视图
	UpdateAllViews(NULL);

}

//void CImageProDoc::OnPointPseudocolor()
//{
//	CDlgColor dlgPara;
//
//	// 初始化变量值
//	if (m_nColorIndex >= 0)
//	{
//		// 初始选中当前的伪彩色
//		dlgPara.m_nColor = m_nColorIndex;
//	}
//	else
//	{
//		// 初始选中灰度伪彩色编码表
//		dlgPara.m_nColor = 0;
//	}
//
//	// 指向名称数组的指针
//	dlgPara.m_lpColorName = (LPSTR)ColorScaleName;
//
//	// 伪彩色编码数目
//	dlgPara.m_nColorCount = COLOR_SCALE_COUNT;
//
//	// 名称字符串长度
//	dlgPara.m_nNameLen = sizeof(ColorScaleName) / COLOR_SCALE_COUNT;
//
//	// 显示对话框，提示用户设定平移量
//	if (dlgPara.DoModal() != IDOK)
//	{
//		// 返回
//		return;
//	}
//
//	// 获取用户的设定
//	int nColor = dlgPara.m_nColor;
//
//	// 删除对话框
//	delete dlgPara;
//
//	CPointPro PointOperation(m_pDibObject);
//
//	// 更改光标形状
//	BeginWaitCursor();
//
//	// 调用ReplaceColorPal()函数变换调色板
//	PointOperation.ReplaceColorPal(m_pDibObject, (BYTE*)ColorsTable[nColor]);
//
//	// 设置脏标记
//	//SetModifiedFlag(TRUE);
//
//	// 更新视图
//	UpdateAllViews(NULL);
//
//	// 恢复光标
//	EndWaitCursor();
//
//}

void CImageProDoc::OnEnhaGradsharp()
{
	//创建点处理CPointPro类对象
	CAreaPro AreaPro(m_pDibObject);
	// 更改光标形状
	BeginWaitCursor();

	AreaPro.GradSharping(10);

	// 恢复光标
	EndWaitCursor();

	// 更新视图
	UpdateAllViews(NULL);
}


void CImageProDoc::OnEnhaMidianF()
{
	// 窗口高度
	int	nTempH;

	// 窗口宽度
	int nTempW;

	// 窗口中心元素X坐标
	int nTempXc;

	// 窗口中心元素Y坐标
	int nTempYc;

	// 排序方式
	int nSortType;


	// 创建对话框
	CDlgMedianFilter dlgPara;

	// 初始化对话框变量值
	dlgPara.m_nWindowSize = 0;
	dlgPara.m_iTempH = 3;
	dlgPara.m_iTempW = 3;
	dlgPara.m_iTempMX = 1;
	dlgPara.m_iTempMY = 1;
	dlgPara.m_nSortType = 0;

	// 显示对话框，提示用户设定平移量
	if (dlgPara.DoModal() != IDOK)
	{
		// 返回
		return;
	}

	// 获取用户设定的平移量
	nTempH = dlgPara.m_iTempH;
	nTempW = dlgPara.m_iTempW;
	nTempXc = dlgPara.m_iTempMX;
	nTempYc = dlgPara.m_iTempMY;
	nSortType = dlgPara.m_nSortType;


	// 删除对话框
	delete dlgPara;

	//创建点处理CPointPro类对象
	CAreaPro AreaPro(m_pDibObject);
	// 更改光标形状
	BeginWaitCursor();

	AreaPro.MedianFilter(nSortType, nTempW, nTempH, nTempXc, nTempYc);

	// 恢复光标
	EndWaitCursor();

	// 更新视图
	UpdateAllViews(NULL);
}

void CImageProDoc::OnEnhaTempsharp()
{
	// 模板高度
	int	nTempH;

	// 模板宽度
	int nTempW;

	// 模板系数
	float fTempC;

	// 模板中心元素X坐标
	int nTempXc;

	// 模板中心元素Y坐标
	int nTempYc;

	// 模板元素数组
	float aValue[25];

	// 方向元素数组
	int nDirection[8];

	// 创建对话框
	CEdgeDetDlg dlgPara;

	// 给模板数组赋初值（为Roberts模板1）
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

	// 初始化对话框变量值
	dlgPara.m_intType = 0;
	dlgPara.m_iTempH = 3;
	dlgPara.m_iTempW = 3;
	dlgPara.m_iTempMX = 1;
	dlgPara.m_iTempMY = 1;
	dlgPara.m_fTempC = 1.0;
	dlgPara.m_fpArray = aValue;
	dlgPara.m_nDirection = nDirection;

	// 显示对话框，提示用户设定平移量
	if (dlgPara.DoModal() != IDOK)
	{
		// 返回
		return;
	}

	// 获取用户设定的平移量
	nTempH = dlgPara.m_iTempH;
	nTempW = dlgPara.m_iTempW;
	nTempXc = dlgPara.m_iTempMX;
	nTempYc = dlgPara.m_iTempMY;
	fTempC = dlgPara.m_fTempC;


	// 删除对话框
	delete dlgPara;

	//创建点处理CPointPro类对象
	CAreaPro AreaPro(m_pDibObject);
	// 更改光标形状
	BeginWaitCursor();

	AreaPro.Sharping(aValue, nDirection);

	// 恢复光标
	EndWaitCursor();

	// 更新视图
	UpdateAllViews(NULL);

}

void CImageProDoc::OnEnhaLapsharp()
{
	// 模板高度
	int	nTempH;

	// 模板宽度
	int nTempW;

	// 模板系数
	float fTempC;

	// 模板中心元素X坐标
	int nTempXc;

	// 模板中心元素Y坐标
	int nTempYc;

	// 模板元素数组
	float aValue[25];

	// 创建对话框
	CLapDlg dlgPara;

	// 给模板数组赋初值（为平均模板）
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

	// 初始化对话框变量值
	dlgPara.m_intType = 0;
	dlgPara.m_iTempH = 3;
	dlgPara.m_iTempW = 3;
	dlgPara.m_iTempMX = 1;
	dlgPara.m_iTempMY = 1;
	dlgPara.m_fTempC = 1.0;
	dlgPara.m_fpArray = aValue;

	// 显示对话框，提示用户设定平移量
	if (dlgPara.DoModal() != IDOK)
	{
		// 返回
		return;
	}

	// 获取用户设定的平移量
	nTempH = dlgPara.m_iTempH;
	nTempW = dlgPara.m_iTempW;
	nTempXc = dlgPara.m_iTempMX;
	nTempYc = dlgPara.m_iTempMY;
	fTempC = dlgPara.m_fTempC;

	// 删除对话框
	delete dlgPara;

	//创建点处理CPointPro类对象
	CAreaPro AreaPro(m_pDibObject);
	// 更改光标形状
	BeginWaitCursor();

	AreaPro.LapSharping(aValue, fTempC, nTempW, nTempH, nTempXc, nTempYc);

	// 恢复光标
	EndWaitCursor();

	// 更新视图
	UpdateAllViews(NULL);

}



