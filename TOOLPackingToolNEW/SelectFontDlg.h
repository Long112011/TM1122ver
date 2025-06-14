
#pragma once
class CSelectFontDlg : public CDialog
{
public:
	CSelectFontDlg(CWnd* pParent = NULL);   
	
	enum { IDD = IDD_DIALOG_SELECTFONT };
	CString	m_sExt;
	CString	m_sExtT;
	
	
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//void OnPaint();
protected:
	
	
	
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};
