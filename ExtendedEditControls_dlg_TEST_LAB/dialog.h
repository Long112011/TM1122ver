
#pragma once
#include "OXEdit.h"
#include "SaveAsDlg.h"	
#include "MHFileMng.h"	
#include "ScintillaWnd.h"
#include "tabctrl.hpp"
#include "StylerDlg.h"
#include "OpenMultiBin.h"
#include "XListBox.h"
#include "Button_Input.h"
#include "ScintillaStruct.h"
#include "AutoEdit.h"
#include "DayValidate.h"
#include "PreviewEditDlg.h"

#include "QuickFindWnd.h"
#include "SMovingChildStruct.h"
#include "Modeless.h"

class theDlg : public CDialog
{
protected:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	CStatusBar		m_wndStatusBar;
public:
	//modeless
	afx_msg void OnBnClickedButton1();
	CModeless* m_pmodeless;
	CString m_text;

	///////////////
	CWnd* m_pParent;
	enum { IDD = ID_DESCRIPTION_FILE };
	CQuickFindWnd		*m_QuickFind;
	CStylerDlg			*m_StylerDlg;

	CMHFileMng			MHFileManager;
	
	CScintillaWnd       m_ScinCtrl;		
	COXEdit				m_FindStr;
	CXListBox			m_FileList;

	COpenMultiBin		m_MultiBinaryLoadDlg;
	CSaveAsDlg			*m_SaveAsDlg;
	//CPreviewEditDlg		m_PreviewEditDlg;
	
	CAutoEdit			m_Edit1;
	/*************************tab control***********************/
	TabControl			m_TabControl;
	CStatic				m_StaticListCount;

	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	
	CString	m_sExt;
	void SetCommandLine_TEST(char* argv[]);
	void SetCommandLine(LPCTSTR aCommand);
	void CommandLine();
	void CommandLine_N();
private:
	HFONT hFont;

	void undo();
	void redo();

	void Cut();
	void Copy();
	void Paste();
	void SelectAll();


	void FixCurrentFileEOL();

	
	
	BOOL Size();
	void InitialiseEditor();
	
	void InitialiseOutput();
	void UpdateDialogTitle(CString Text);
	HWND				m_hwndOutput;

	HICON m_hIcon;
	
	void OnButtonDelall();
	
	typedef std::vector<SMovingChildTest>   MovingChildrenTest;
	typedef std::vector<SMovingChild>		MovingChildren;

	MovingChildren  m_MovingChildren;
	CSize           m_szInitial;
	CSize           m_szMinimum;

	MovingChildrenTest  m_MovingChildrenTest;

	DWORD stHeight;
	DWORD stWidth;
	DWORD SizeX;
	DWORD SizeY;
public:

	bool doStreamComment();
	void OndefaultButtonTestingButton(CString gStr);
	void OndefaultButtonTesting(BOOL bFlag = FALSE, char*stR = NULL);
	afx_msg void RefreshDarkButton();
	afx_msg void OnCBBorderWidth();
	virtual void DoDefaultFolding(int nMargin, long lPosition);
	virtual void Refresh();


	

	DWORD BigenChar;
	CString	m_sCurrentEOL;
	CString	m_sFileErrorNum;
	CString	m_sFileNum;
	
	HBRUSH darkHBR;

	theDlg(CWnd* pParent = NULL);
	CStatic		m_staticGroupBox;	
	COXColorPickerButton	m_btnTextColor;
	CMyButton	m_btnOpenSetting, OpenFileBtn, SaveAs, pakManBtn[5];
	COLORREF m_clrBack;
	COLORREF m_clrText;
	int m_nFileErrorNum;
	int m_nFileNum;
	int m_nOldIndex;
	void RefreshButtonStates(BOOL bImmediate);
	LRESULT OnControlPlusF(WPARAM /*wp*/, LPARAM /*lp*/);
	LRESULT OnClearButtonStates(WPARAM /*wp*/, LPARAM /*lp*/);
	LRESULT OnRefreshButtonStatesEOL(WPARAM wparam, LPARAM /*lp*/);
	LRESULT OnRefreshButtonStates(WPARAM /*wp*/, LPARAM /*lp*/);
	CButton*GetCurButton(int ID);
	afx_msg void OnRClicked(NMHDR * pNotifyStruct, LRESULT * result);
	afx_msg void OnButtonTestBackcolor(COLORREF clr);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnLoadAllBinaryFile();
	void BrowseCurrentAllFile(CString strDir);
	void InitFont();
	void InitSaveGroupBoxRect();
	void OnSize(UINT nType, int cx, int cy);
	void OnSizeJack(int cx, int cy, int percentage=0);
	void OnCenterWindow();
	
	void SaveData(int index);

	afx_msg void OnButtonCloseSettings();
	afx_msg CRect GetEditorPositionDialog();
	afx_msg void SetEditorPositionDialog();
protected:
	
	//afx_msg LONG OnPaste(UINT wParam, LONG lParam);
	CRect	m_editorRect;
	CDayValidate* m_dayValidate;
	afx_msg void StartTimer_bak();
	afx_msg void StopTimer_bak();



	afx_msg void OnTimer_Bak(UINT nTimer);
	afx_msg void OnFeature_bak();
	virtual void DoDataExchange(CDataExchange* pDX);
	
	void InitFindStringEditControl();
	void SetGroupBoxGangNewPos(int x, int y, int w, int h);
	void OnClickTabCtrl(NMHDR* pNotifyStruct, LRESULT* pResult);
	void OnDropFiles(HDROP hDropInfo);
	void ShowError(char * stRing); 
	void BukaFileDynamic(char* pFileName);
	void BukaFileStatic(char* pFileName);
	
	void UpdateList();
	afx_msg void AddTab(int index, char*str);
	afx_msg void OnSelchangeListFile();
	//afx_msg void OnCancel();
	afx_msg void OnButtonExit();
	afx_msg void OnSingleFileOpen();
	afx_msg void OnMultiFileOpen();
	//afx_msg void LoadMultiFile();
	afx_msg void UpdateCurrentSelection();
	afx_msg void OnFileSaveas();
	afx_msg void FixLineEndings();
	afx_msg void OnButtonOpenPreviewDlg();
	afx_msg void OnButtonDecryptAS();
	afx_msg void OnButtonEncryptAS();
	afx_msg void OnButtonSaveBin();
	afx_msg void OnEnChangeEditFindstr();
	void OnEnChangeEdit();
	afx_msg void randomGUID();
	afx_msg void OnBnClickedButtonFindstr();
	virtual void OnQuickReplace();
	virtual void OnQuickFind();
	virtual void CheckCloseOwnedFindWindow();
	virtual void GetFontHeight();
	//virtual BOOL PreTranslateMessage(MSG* pMsg);
	BOOL PreTranslateMessage(MSG* pMsg) override;
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnDestroy();
	virtual void PostNcDestroy();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd *pWnd, UINT nCtlColor);
	
	afx_msg void InitStyler();
	afx_msg void OnButtonOpenSettings();
	afx_msg void OnButtonTextcolor();
	afx_msg void OnButtonFont();
	afx_msg void OnButtonTextcolorNegative();
	afx_msg void OnCheckDecimalDigitCountIndefinite();
	afx_msg void OnCheckNoGroup();
	afx_msg void OnEditchangeComboMask();
	afx_msg void OnSelchangeComboMask();
	afx_msg void OnCheckLeadingZero();
	afx_msg void OnSelchangeComboDecimalDigitCount();
	afx_msg void OnEditchangeComboDecimalSeparator();
	afx_msg void OnSelchangeComboDecimalSeparator();
	afx_msg void OnSelchangeComboFractionalDigitCount();
	afx_msg void OnSelchangeComboGroupLength();
	afx_msg void OnEditchangeComboGroupSeparator();
	afx_msg void OnSelchangeComboGroupSeparator();
	afx_msg void OnEditchangeComboNegativeValueFormat();
	afx_msg void OnSelchangeComboNegativeValueFormat();
	afx_msg void OnEditchangeComboCurrencyName();
	afx_msg void OnSelchangeComboCurrencyName();
	afx_msg void OnEditchangeComboPositiveValueFormat();
	afx_msg void OnSelchangeComboPositiveValueFormat();
	DECLARE_MESSAGE_MAP()
};