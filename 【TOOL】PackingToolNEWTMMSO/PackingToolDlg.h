#pragma once
#define RGBA_MAKE(r, g, b, a)   ((DWORD) (((a) << 24) | ((r) << 16) | ((g) << 8) | (b)))
#define RGB_HALF(r, g, b)		((DWORD) (((r) << 16) | ((g) << 8) | (b)))
#define RGBA_MERGE(rgb, a)		((DWORD) (((a) << 24) | (rgb)))
//#include "NewDlg.h"	
#include "MHFileMng.h"	
#include "MHFileEx.h"	
#include "ExtDlg.h"	
#include "SaveAsDlg.h"	
#include "SelectFontDlg.h"	
//#include "MyEdit.h"
#include <vector>
//#include "SkinScrollWnd.h"
//#include "SkinScrollBar.h"

//#include "OXFontPickerButton.h"
//#include "OXSeparator.h"
//#include "OXFontComboBox.h"
/*class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

protected:
	DECLARE_MESSAGE_MAP()
};*/
enum eTLSTYLEOUT
{
	TLO_NAIVEFONT				= 0x00000001,		/* 扁夯 迄飘 荤侩 */
	TLO_MULTILINE				= 0x00000002,		// not implemented
	TLO_DISABLENOSCROLL			= 0x00000004,		/* 临 荐啊 府胶农甫 逞绢哎锭鳖瘤 胶农费 牧飘费 救焊烙 荤侩*/

	TLO_LINENUMBER				= 0x00000008,		/* 府胶飘 临 锅龋 免仿 荤侩 */
	TLO_DISABLESCROLLTUMBTRACK	= 0x00000010,		/* 胶农费 滚瓢 UP矫 胶农费 函拳 荤侩 */
	TLO_SCROLLTUMBTRACKUPDATE	= 0x00000020,		/* 胶农费 滚瓢 捞悼矫 官肺 拳搁 诀单捞飘 荤侩 */
	TLO_SIMPLEFRAME				= 0x00000040,		/* Window Simple Frame 荤侩 */
	TLO_NOTMESSAGECLOSE			= 0x00000080,		/* WM_CLOSE啊 吭阑 锭 拱绢 焊瘤 臼绊 官肺 辆丰 */
};
class CTList;

class CPackingToolDlg : public CDialog
{
	void OpenFile(char* pFileName);
	void OpenFileTwo(char* pFileName);
	void UpdateList();

	/*char* BIG5ToGB2312(const char* szBIG5String);
	char* UnicodeToGB2312(const wchar_t* szUnicodeString);
	wchar_t* BIG5ToUnicode(const char* szBIG5String);
	char* GB2312ToBIG5(const char* szGBString);
	wchar_t* GB2312ToUnicode(const char* szGBString);
	char* UnicodeToBIG5(const wchar_t* szUnicodeString);*/
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	void SetCommandLine(LPCTSTR aCommand);
	CPackingToolDlg(CWnd* pParent = NULL);	
	HBITMAP	m_hBmpScrollBar;
	enum { IDD = IDD_PACKINGTOOL_DIALOG };
	CButton	m_BtnSave;
	CButton	m_BtnSaveAsBin;
	CEdit	m_Edit;
	CButton	m_BtnSaveAllToTxt;
	CButton	m_BtnSaveAllToBin;
	CButton	m_BtnOpenBin;
	CButton	m_BtnOpen;
	CButton	m_BtnNew;
	CButton	m_BtnDelAll;
	//COXFontPickerButton	m_BtnSelFont;
	CListBox m_FileList;
	//CListBox m_LineNumber;
	CString	m_sFileNum;
	CString	m_sCurFile;

	void CreateList(LOGFONT * pFont);
	void DestroyList();
	CTList * GetTList();
	CTList  * pCombox1;
	BOOL IsStyle(DWORD dwStyle);
//	CSkinScrollBar	m_ctrlScroll;

	//COXSeparator	m_sepCombo;
	//COXSeparator	m_sepButton;
	//COXFontComboBox	m_cmbFontPicker;
	//COXFontPickerButton	m_btnFontPicker;//
	DWORD BigenChar;

	DWORD stHeight;
	DWORD stWidth;

	CEdit m_FindStr;

	//CGr
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButtonFindstr();
	afx_msg void OnEnChangeEditFindstr();
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd *pWnd, UINT nCtlColor);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedSelectFont();
	afx_msg void OnClose();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	WORD    GetOperationSystemType();
	void OnEnChangeEdit();
	afx_msg void OnBnClickedSelectFontTest();
    //bool m_bShowGripper;         // ignored if not WS_THICKFRAME

	RECT m_Gap;
	WINDOWPLACEMENT m_EditPl;
	int m_nOldIndex;
	void SaveData( int index );
	void TestMsgFromAnotherFile();
	//CNewDlg m_NewDlg;
	CSaveAsDlg	m_SaveAsDlg;
	CSelectFontDlg m_SelectFontDlg;
	CExtDlg m_ExtDlg;
	CMHFileMng	m_FileMng;
	int			m_nFileNum;
	
	RECT JackRect;

	int			m_nWheelMsg;

	BOOL		m_ctrlFocused;

	BOOL		m_fontLoaded;
	DWORD		m_fontLoadedKind; 
	CFont *m_pFont;

	CStatic m_static1;

	CFont m_font2;

	WORD		m_nlinePerScreen;
	WORD		m_nScrollMaxPos;
	int			m_nScrollPos;
protected:

	CFont m_font;
	COLORREF rgbColor;
	COLORREF m_rgbColor;

	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();

	HICON m_hIcon;
	HCURSOR OnQueryDragIcon();
	
	void AutoMove(int iID, double dXMovePct, double dYMovePct, double dXSizePct, double dYSizePct);

	

	RECT GetJackRect(){return JackRect;}

	void OnSysCommand(UINT nID, LPARAM lParam);
	void OnPaint();
	
	void OnButtonOpen();
	void OnButtonSaveToBin();
	void OnButtonOpenBin();
	void OnButtonSaveToTxt();
	void OnButtonDelall();
	void OnSelchangeListFile();
	void OnButtonNew();
	void OnButtonSaveAsBin();
	void OnDropFiles(HDROP hDropInfo);
	void OnButtonSaveBin();
	void OnFileSearch();
	void OnSearch();
	void OnButtonSavedsof();

	void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
    void OnSize(UINT nType, int cx, int cy);
	void OnSizeJack(int cx, int cy);
	void UpdateDialogTitle(CString Text);

	void LoadFont(BYTE bFlag=0);
	void DlgMsgBox(CString c);
	void DlgMsgBox(BYTE b);

	CButton*GetButton(int ID);
	CString IntToStr(int i);
	string GetCurrentWindowTitle();

	void AddLineNumber();

	
	DECLARE_MESSAGE_MAP()

	
private:
    struct SMovingChild
    {
        HWND        m_hWnd;
        double      m_dXMoveFrac;
        double      m_dYMoveFrac;
        double      m_dXSizeFrac;
        double      m_dYSizeFrac;
        CRect       m_rcInitial;
    };
    typedef std::vector<SMovingChild>   MovingChildren;

    MovingChildren  m_MovingChildren;
    CSize           m_szInitial;
    CSize           m_szMinimum;
    //HWND            m_hGripper;


	struct SMovingChildTest
	{
		HWND        m_hWnd;
		double      m_dXMoveFrac;
		double      m_dYMoveFrac;
		double      m_dXSizeFrac;
		double      m_dYSizeFrac;
		CRect       m_rcInitial;
	};
	typedef std::vector<SMovingChildTest>   MovingChildrenTest;

	MovingChildrenTest  m_MovingChildrenTest;

	CTList * m_pTList;
};

extern CPackingToolDlg theApp2;