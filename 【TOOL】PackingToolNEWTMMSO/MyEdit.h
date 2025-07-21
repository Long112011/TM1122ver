#if !defined(AFX_MYEDIT_H__23FDB529_7ADA_4F4F_AD26_305E55C62978__INCLUDED_)
#define AFX_MYEDIT_H__23FDB529_7ADA_4F4F_AD26_305E55C62978__INCLUDED_
#if _MSC_VER > 1000
#pragma once
#endif 
class CMyEdit : public CEdit
{
public:
	CMyEdit();

	CDialog m_FindStr;
public:
public:
	
	
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	
public:
	BOOL m_bFlag;
	virtual ~CMyEdit();
	void OnSize(UINT nType, int cx, int cy);
protected:
	
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDropFiles(HDROP hDropInfo);
	
	DECLARE_MESSAGE_MAP()
};
#endif 
