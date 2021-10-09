// ImageProView.h : interface of the CImageProView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_IMAGEPROVIEW_H__865C1405_220C_4A82_B81A_12C7244B7308__INCLUDED_)
#define AFX_IMAGEPROVIEW_H__865C1405_220C_4A82_B81A_12C7244B7308__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CImageProView : public CView
{
protected: // create from serialization only
	CImageProView();
	DECLARE_DYNCREATE(CImageProView)

// Attributes
public:
	CImageProDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CImageProView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CImageProView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CImageProView)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in ImageProView.cpp
inline CImageProDoc* CImageProView::GetDocument()
   { return (CImageProDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IMAGEPROVIEW_H__865C1405_220C_4A82_B81A_12C7244B7308__INCLUDED_)
