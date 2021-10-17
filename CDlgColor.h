#pragma once


// CDlgColor 对话框

class CDlgColor : public CDialog
{
	DECLARE_DYNAMIC(CDlgColor)

public:
	CDlgColor(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDlgColor();
public:

	// 颜色名称字符串长度
	int m_nNameLen;

	// 颜色名称字符串数组指针
	LPSTR m_lpColorName;

	// 颜色数目
	int m_nColorCount;

	// 当前选择的伪彩色编码表
	int m_nColor;

	CListBox m_lstColor;
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_Color };

#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	// Generated message map functions
	//{{AFX_MSG(CDlgColor)
	afx_msg void OnDblclkColorList();
	virtual BOOL OnInitDialog();
	virtual void OnOK();

	DECLARE_MESSAGE_MAP()
};
