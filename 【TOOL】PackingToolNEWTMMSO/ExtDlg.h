#if !defined(AFX_EXTDLG_H__13579D67_6576_44EA_BE40_674E12370293__INCLUDED_)
#define AFX_EXTDLG_H__13579D67_6576_44EA_BE40_674E12370293__INCLUDED_
#if _MSC_VER > 1000
#pragma once
#endif 
class CExtDlg : public CDialog
{
public:
	CExtDlg(CWnd* pParent = NULL);   
	
	enum { IDD = IDD_DIALOG_EXT };
	CString	m_sExt;
	CString	m_sExtT;
	
	
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	
protected:
	
	
	
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void TestClick();
};
#endif 
