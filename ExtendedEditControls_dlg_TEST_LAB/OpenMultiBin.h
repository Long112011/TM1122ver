#pragma once
#include "resource.h"
#include "OXEdit.h"
class COpenMultiBin
:public CDialog
{
public:
	COpenMultiBin();
	enum { IDD = IDD_DIALOG_POPUP_EXT };
	CString	m_sExt;
	CString	m_sInfo;
	COXEdit				m_sExtEdit;
	afx_msg void OnBnClickedOk();
	virtual int GetFormatFromExtension(LPCSTR szExtension);
	void SetContextMenuId(UINT nId) { m_nContextMenuId = nId; }
protected:
	UINT m_nContextMenuId;
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    
	DECLARE_MESSAGE_MAP()	
};