// ImageProDoc.h : interface of the CImageProDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_IMAGEPRODOC_H__30E23BD8_044A_4C59_B28F_1F86F7550B94__INCLUDED_)
#define AFX_IMAGEPRODOC_H__30E23BD8_044A_4C59_B28F_1F86F7550B94__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CImageProDoc : public CDocument
{
protected: // create from serialization only
	CImageProDoc();
	DECLARE_DYNCREATE(CImageProDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CImageProDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);


	BOOL ReadImgToDoc();

// Implementation
public:
	virtual ~CImageProDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:
	BOOL m_bImageLoaded;
	CDibObject* m_pDibObject;
protected:
	//{{AFX_MSG(CImageProDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	afx_msg void OnFileOpen();
	afx_msg void OnFileNew();
	afx_msg void OnFileSaveAs();


	DECLARE_MESSAGE_MAP()

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IMAGEPRODOC_H__30E23BD8_044A_4C59_B28F_1F86F7550B94__INCLUDED_)
