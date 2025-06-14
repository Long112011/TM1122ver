#pragma once
#include "resource.h"
#include "colorstatic.h"
#include "FontPreviewCombo.h"
#include "OXColorPickerButton.h"
#include "C_Button.h"
#include "C_Frame.h"
#include "colourHook.h"
//#include "ScintillaWnd.h"






enum{ STYLELIST = 0, FONTCOMBO, FONTSIZECOMBO };
enum{ BackColor = 0, TextColor };

class CStylerDlg : public CDialog
{
	
	DECLARE_DYNAMIC(CStylerDlg)
private:
	//CScintillaWnd       m_ScinCtrlStyler;
	long _higherY = 0;
	long _lowerY = 0;
	//COXColorPickerButton* _pLinkColorPicker = nullptr;
	COXColorPickerButton	m_btnTextColor;
	COXColorPickerButton	m_btnBackColor;

	CMFCButton		m_TestDefaultBtn;

	ColourStaticTextHooker _colourHooker;
	HWND _hStyleInfoStaticText = nullptr;
	HBRUSH darkHBR;
	//CToolTipCtrl *pTool;
	void enableLineNumberCB();
	void enableEOLflagCB();
	void enableLexerForeStyleCB();
	void enableLexerBackStyleCB();

	void onSelectBackColor();
	void onSelectTextColor();

	CString m_faceName;
	CString m_fontSize;
	CString	m_sliderValue;
	CString m_sliderBorderWidthValue;
	CString m_sliderAlphaPercent;
public:
	CWnd* m_pParent;
	CRect	m_editorRect;
	CC_Frame	m_staticBtnColor[2];
	CStylerDlg(CWnd* pParent = NULL);
	//virtual ~CStylerDlg();
	enum { IDD = IDJ_STYLER_DLG };
	
	CColorStatic m_styleDescStatic;
	CSliderCtrl m_slider, m_sliderBorderWidth;
	
	CScrollBar	m_ctrlScroll;
	//CButton* pDarkModeCB;
	virtual void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	virtual BOOL OnInitDialog();
	virtual void InitFontComboBox();
	virtual void InitFontSizeComboBox();
	virtual void InitCheckBox();
	virtual void InitAllDarkButton();
	//virtual void InitDarkButton1(HWND hButton);
	virtual void InitButton();
	virtual void InitStyleListBox();
	virtual void InitColorButton();
	virtual void InitGroupBox();
	virtual void InitSliderControl();
	virtual void InitStaticControl();
	virtual int SelectFont(LPCTSTR lpszFontName);
	virtual int SelectFontSize(LPCTSTR lpszFontSize);
	virtual void SelectDefaultFocus(int id = IDJ_STYLES_LIST);
/*************modeless dialog*****************/
	virtual void PostNcDestroy();
	virtual void OnCancel();
	virtual void OnOK();

	virtual void ShowWindow(int nCmdShow);


	virtual void SetSelectThemeEnable(BOOL bFlag);
protected:

	HACCEL			m_hAccel;
	ACCEL*			m_pAccelTable;
	int				m_nAccelSize;
	BOOL FindAccelerator(UINT uiCmd, CString& str) const;


	static COLORREF _focus;
	static COLORREF _frost;
	HICON m_hIcon;
	CFontPreviewCombo m_fontCombo;
	CListBox  m_styleListBox;
	CComboBox  m_fontSizeCombo;
	virtual void DoDataExchange(CDataExchange* pDX);
	
	afx_msg void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void drawPreviewZone(DRAWITEMSTRUCT *pdis);
	virtual BOOL InitButton(CMFCButton& btn, UINT nID, HINSTANCE hResInst = nullptr, int nImgSize = 16) const;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd *pWnd, UINT nCtlColor);
	afx_msg void OnDropDownFontSelchange();
	afx_msg void OnDropDownFontSizeSelchange();
	afx_msg void OnSelchangeStyleListBox();
	afx_msg void OnButtonBackcolor();
	afx_msg void OnButtonTextcolor();
	afx_msg void OnCBNoBorder();
	afx_msg void OnCBTransparent();
	afx_msg int GetBorderWidth();
	afx_msg int GetStylerAlphaPercent();
	afx_msg void OnCBBorderWidth();
	afx_msg void OnCBDarkMode();
	afx_msg void OnButtonDefault();
	afx_msg void UpdateColorPickerButton_temp();
	afx_msg void UpdateColorPickerButton();
	afx_msg void OnBnClickedSavecloseButton();
	DECLARE_MESSAGE_MAP()
};
