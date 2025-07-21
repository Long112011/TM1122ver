// MHAutoPatchDlg.h : 头文件
//

#pragma once
#include <atlimage.h>
#include "afxwin.h"
#include "afxcmn.h"
#include "hashtable.h"
#include "explorer_gg.h"
#include <map>
#define  UPDATAURL ".\\Image\\Autopatch\\AutoPatchURL.bin"
#define	 UPDATAFILESAVEPATH ".\\Image\\Autopatch\\MyUpdata.txt"
#define RECVPACK_SIZE 2048 
struct TextColor
{
	int Red;
	int Green;
	int Blue;
};
struct DlgRect
{
	int x;
	int y;
	int cx;
	int cy;
};
struct UpdataInfo
{
	char  LunchVer[32];
	char  HomeUrl[256];
	char  PayUrl[256];
	char  QQ[256];
	char  CommandLine[64];
	char  ExeName[32];
	char  GGUrl[256];
	char  UpLuchUrl[256];
	char  LunchName[64];
};
// CMHAutoPatchDlg 对话框

struct DownLoadInfo
{
	std::string Url;
	std::string Path;
};
enum ColorSet
{
	eTip,
	eFile,
	eVer,
	eFont,
	eProf,
	eProb,
	eProdf,
	eProdb,
	eWeb,
	eMaxColor,
};
enum DlgRectSet
{
	eStart,
	eMain,
	ePay,
	eHome,
	eClose,
	eWindow,
	ePro,
	eProd,
	eFenbianlv,
	eTipMsg,
	eFileMsg,
	eVerMsg,
	eWebRect,
	eMaxRect,
};
struct FontSet
{
	int Size;
	char FontName[64];
};
class CMHAutoPatchDlg : public CDialog
{	
	TextColor sTextColor[eMaxColor];
	DlgRect sDlgRect[eMaxRect];
	FontSet sFontSet;
	CFont m_font;   //声明变量
	HBRUSH hbr;
	HBRUSH hbr1;
	HBRUSH hbr2;
public:
	CMHAutoPatchDlg(CWnd* pParent = NULL);	// 标准构造函数
	~CMHAutoPatchDlg();
// 对话框数据
	enum { IDD = IDD_MHAUTOPATCH_DIALOG };
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
	BOOL LoadServerInfo();
	void MyPaint();
	void InitComBox();
	void LoadDlgInfo();
	void LoadTipMsg();
	//void DrawMainWindow();
	 void LoadWindowSetting();
// 实现
protected:
	HICON m_hIcon;
	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	HBRUSH OnCtlColor(CDC* pDC,CWnd* pWnd,UINT nCtlColor);
	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClickedButtonStart();
	afx_msg void OnBnClickedButtonClose();
	CBitmapButton m_BtnStart;
	CBitmapButton m_BtnPay;
	CBitmapButton m_BtnHomePage;
	CBitmapButton m_BtnQQ;
	CBitmapButton m_BtnClose;
	Image* m_bgImag;
	CButton m_CheckWndMode;
	CComboBox m_ComboBox;
	CProgressCtrl m_Progress;
public:
	afx_msg void OnBnClickedButtonPay();
	afx_msg void OnBnClickedButtonHomepage();
	afx_msg void OnBnClickedButtonQq();
	afx_msg void OnCbnSelchangeComboScreen();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CStatic m_StTip;
	CStatic m_StDwnlTip;
	CStatic m_StDlSp;
	CExplorer_gg m_WebGG;
public:
	CProgressCtrl m_ProgressDone;
	CString strBook;
};
