// CDlgColor.cpp: 实现文件
//

#include "StdAfx.h"
#include "ImagePro.h"
#include "CDlgColor.h"
#include "afxdialogex.h"


// CDlgColor 对话框

IMPLEMENT_DYNAMIC(CDlgColor, CDialog)

CDlgColor::CDlgColor(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DLG_Color, pParent)
{

}

CDlgColor::~CDlgColor()
{
}

BOOL CDlgColor::OnInitDialog()
{
	// 循环变量
	int		i;

	// 调用默认OnInitDialog函数
	CDialog::OnInitDialog();

	// 添加伪彩色编码
	for (i = 0; i < m_nColorCount; i++)
	{
		m_lstColor.AddString(m_lpColorName + i * m_nNameLen);
	}

	// 选中初始编码表
	m_lstColor.SetCurSel(m_nColor);

	// 返回TRUE
	return TRUE;
}

void CDlgColor::OnDblclkColorList()
{
	// 双击事件，直接调用OnOK()成员函数
	OnOK();

}

void CDlgColor::OnOK()
{
	// 用户单击确定按钮
	m_nColor = m_lstColor.GetCurSel();

	// 调用默认的OnOK()函数
	CDialog::OnOK();
}

void CDlgColor::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgColor, CDialog)
	ON_LBN_DBLCLK(IDC_COLOR_LIST, OnDblclkColorList)//左击和OnDblclkColorList绑定
END_MESSAGE_MAP()


// CDlgColor 消息处理程序
