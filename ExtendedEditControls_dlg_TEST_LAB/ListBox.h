// ListBoxTutorialDlg.h : header file
//

#if !defined(AFX_LISTBOXTUTORIALDLG_H__30AAEAC7_9009_11D4_BBCF_0000B44EED3A__INCLUDED_)
#define AFX_LISTBOXTUTORIALDLG_H__30AAEAC7_9009_11D4_BBCF_0000B44EED3A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CListBoxTutorialDlg dialog

#define	NumOfLists	6

class CListBoxTutorialDlg : public CDialog
{
// Construction
public:
	CListBoxTutorialDlg(CWnd* pParent = NULL);	// standard constructor
	virtual ~CListBoxTutorialDlg();

// Dialog Data
	//{{AFX_DATA(CListBoxTutorialDlg)
	enum { IDD = IDD_LISTBOXTUTORIAL_DIALOG };
	CStatic	m_Template;
	BOOL	m_bLoadList;
	CString	m_strReport;
	BOOL	m_InsertRemove;
	//}}AFX_DATA
	CListBox	m_List[NumOfLists];
	CListBox*	m_pFlyList;
	CString	m_strSelection[NumOfLists];

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CListBoxTutorialDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;
	BOOL m_bTabbed;
	DWORD m_ListStyle;

	// Generated message map functions
	//{{AFX_MSG(CListBoxTutorialDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnLoadList();
	afx_msg void OnToggleListStyle();
	afx_msg void OnInsertRemove();
	afx_msg void OnSelect();
	//}}AFX_MSG
	afx_msg void OnSelchangeList(UINT nID);
	afx_msg void OnDblclkList(UINT nID);

	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LISTBOXTUTORIALDLG_H__30AAEAC7_9009_11D4_BBCF_0000B44EED3A__INCLUDED_)
