// MHAutoPatchDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MHAutoPatch.h"
#include "MHAutoPatchDlg.h"
#include "MonitorAdapter.h"
#include "MHFile.h"
#include <afxinet.h>
#include "CheckResourceFile.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//登录器版本号
char AutoPatchVer[] = "APv20210926";
// CMHAutoPatchDlg 对话框
//服务器信息结构
UpdataInfo ServerInfo;
//更新地址
char UpdataUrl[256];
//当前运行目录
char pCurPath[MAX_PATH];
//需下载文件数量
int m_FileCount; 
//进程退出控制变量
BOOL StopTheard = FALSE;
//需下载文件名向量
std::vector<std::string> m_WaitDownlaodFileInfo;
//提示文字信息
std::map<std::string,std::string> TipMsgInfo;
//迭代器
std::map<std::string,std::string>::iterator MapIter;
//字符串解析类
cTextParsing TextPars;
//文件校验类
CheckResourceFile CheckFile;
//下载线程
UINT DownloadFile(LPVOID pParam);
//下载线程管理
UINT StartDownload(LPVOID pParam);
//文件校验
UINT StartCheckFile(LPVOID pParam);
//文件保存路径生成
std::string  MakePath(std::string& str);
//补丁下载地址生成
std::string  MakeUrl(std::string& str);
//下载提示信息生成
void MakeDownloadInfo(std::string &str);
//编码转换
std::wstring Ansi2WChar(LPCSTR pszSrc, int nLen) ; 
//主程运行参数生成
void MakeLauchData();
//登录器更新器启动
void UpDataLunch();
//获取下载的文件名
std::string  GetFileName(std::string& str);

CMHAutoPatchDlg::CMHAutoPatchDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMHAutoPatchDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMHAutoPatchDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK_WDNMODE, m_CheckWndMode);
	DDX_Control(pDX, IDC_COMBO_SCREEN, m_ComboBox);
	DDX_Control(pDX, IDC_PROGRESS, m_Progress);
	DDX_Control(pDX, IDC_STATIC_Tip, m_StTip);
	DDX_Control(pDX, IDC_STATIC_FILESIZETIP, m_StDwnlTip);
	DDX_Control(pDX, IDC_EXPLORER_GG, m_WebGG);
	DDX_Control(pDX,IDC_STATIC_VER,m_StDlSp);
	m_FileCount=0;
	DDX_Control(pDX, IDC_PROGRESS_DONE, m_ProgressDone);
}
CMHAutoPatchDlg::~CMHAutoPatchDlg()
{

}
BEGIN_MESSAGE_MAP(CMHAutoPatchDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_CTLCOLOR()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_START, &CMHAutoPatchDlg::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, &CMHAutoPatchDlg::OnBnClickedButtonClose)
	ON_BN_CLICKED(IDC_BUTTON_PAY, &CMHAutoPatchDlg::OnBnClickedButtonPay)
	ON_BN_CLICKED(IDC_BUTTON_HOMEPAGE, &CMHAutoPatchDlg::OnBnClickedButtonHomepage)
	ON_BN_CLICKED(IDC_BUTTON_QQ, &CMHAutoPatchDlg::OnBnClickedButtonQq)
	ON_CBN_SELCHANGE(IDC_COMBO_SCREEN, &CMHAutoPatchDlg::OnCbnSelchangeComboScreen)
	ON_WM_CREATE()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CMHAutoPatchDlg 消息处理程序

BOOL CMHAutoPatchDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	LoadDlgInfo();
	LoadTipMsg();
	MyPaint();
	m_font.CreatePointFont(sFontSet.Size,sFontSet.FontName); //设置字体大小和类型
	hbr  = CreateSolidBrush( RGB(sTextColor[eTip].Red,sTextColor[eTip].Green,sTextColor[eTip].Blue) );
	hbr2 = CreateSolidBrush( RGB(sTextColor[eVer].Red,sTextColor[eVer].Green,sTextColor[eVer].Blue) );
	hbr1 = CreateSolidBrush( RGB(sTextColor[eFile].Red,sTextColor[eFile].Green,sTextColor[eFile].Blue) );
	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标
	// TODO: 在此添加额外的初始化代码
	CWnd::SetWindowPos(NULL,sDlgRect[eMain].x,sDlgRect[eMain].y,sDlgRect[eMain].cx,sDlgRect[eMain].cy,0); //设置初试窗口大小
	int nPos = GetCurrentDirectory( MAX_PATH, pCurPath);
	this->SetDlgItemText(IDC_STATIC_VER,AutoPatchVer);
	InitComBox();
	LoadWindowSetting(); // ← 加在这
	GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
	SetTimer(1,2000,NULL);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}


void CMHAutoPatchDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		//背景图片
		CPaintDC dc(this); // 用于绘制的设备上下文
		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);
		m_bgImag=m_bgImag->FromFile(L".\\Image\\Autopatch\\bg.png",FALSE);
		CRect rect;
		GetClientRect(&rect);
		Graphics g(dc);
		g.DrawImage(m_bgImag,rect.left,rect.top,rect.right,rect.bottom);
		CDialog::OnPaint();
	}
}

void CMHAutoPatchDlg::LoadDlgInfo()
{
	char buf[512];
	memset(&buf,0,sizeof(buf));
	CMHFile file;
	//file.Init(".\\Image\\Autopatch\\AutoPatchDlg_N.bin", "rb");
	file.Init(".\\Image\\Autopatch\\AutoPatchDlg.bin", "rb");
	while(1)
	{
		if(file.IsEOF())
			break;
		file.GetString(buf);
		if(strcmp(buf,"#主界面")==0)
		{
			sDlgRect[eMain].x = file.GetWord();
			sDlgRect[eMain].y = file.GetWord();
			sDlgRect[eMain].cx = file.GetWord();
			sDlgRect[eMain].cy = file.GetWord();
			continue;
		}
		if(strcmp(buf,"#开始按钮")==0)
		{
			sDlgRect[eStart].x = file.GetWord();
			sDlgRect[eStart].y = file.GetWord();
			sDlgRect[eStart].cx = file.GetWord();
			sDlgRect[eStart].cy = file.GetWord();
			continue;
		}
/*		if(strcmp(buf,"#充值按钮")==0)
		{
			sDlgRect[ePay].x = file.GetWord();
			sDlgRect[ePay].y = file.GetWord();
			sDlgRect[ePay].cx = file.GetWord();
			sDlgRect[ePay].cy = file.GetWord();
			continue;
		}
		if(strcmp(buf,"#官网按钮")==0)
		{
			sDlgRect[eHome].x = file.GetWord();
			sDlgRect[eHome].y = file.GetWord();
			sDlgRect[eHome].cx = file.GetWord();
			sDlgRect[eHome].cy = file.GetWord();
			continue;
		}*/
		if(strcmp(buf,"#关闭按钮")==0)
		{
			sDlgRect[eClose].x = file.GetWord();
			sDlgRect[eClose].y = file.GetWord();
			sDlgRect[eClose].cx = file.GetWord();
			sDlgRect[eClose].cy = file.GetWord();
			continue;
		}
		if(strcmp(buf,"#窗口模式")==0)
		{
			sDlgRect[eWindow].x = file.GetWord();
			sDlgRect[eWindow].y = file.GetWord();
			sDlgRect[eWindow].cx = file.GetWord();
			sDlgRect[eWindow].cy = file.GetWord();
			continue;
		}
		/*if(strcmp(buf,"#当前进度条")==0)
		{
			sDlgRect[ePro].x = file.GetWord();
			sDlgRect[ePro].y = file.GetWord();
			sDlgRect[ePro].cx = file.GetWord();
			sDlgRect[ePro].cy = file.GetWord();
			continue;
		}
		if(strcmp(buf,"#当前进度条背景色")==0)
		{
			sTextColor[eProb].Red = file.GetWord();
			sTextColor[eProb].Green = file.GetWord();
			sTextColor[eProb].Blue = file.GetWord();
			continue;
		}
		if(strcmp(buf,"#当前进度条前景色")==0)
		{
			sTextColor[eProf].Red = file.GetWord();
			sTextColor[eProf].Green = file.GetWord();
			sTextColor[eProf].Blue = file.GetWord();
			continue;
		}*/
		if(strcmp(buf,"#完成进度条")==0)
		{
			sDlgRect[eProd].x = file.GetWord();
			sDlgRect[eProd].y = file.GetWord();
			sDlgRect[eProd].cx = file.GetWord();
			sDlgRect[eProd].cy = file.GetWord();
			continue;
		}
		if(strcmp(buf,"#完成进度条背景色")==0)
		{
			sTextColor[eProdb].Red = file.GetWord();
			sTextColor[eProdb].Green = file.GetWord();
			sTextColor[eProdb].Blue = file.GetWord();
			continue;
		}
		if(strcmp(buf,"#完成进度条前景色")==0)
		{
			sTextColor[eProdf].Red = file.GetWord();
			sTextColor[eProdf].Green = file.GetWord();
			sTextColor[eProdf].Blue = file.GetWord();
			continue;
		}
		if(strcmp(buf,"#分辨率选择")==0)
		{
			sDlgRect[eFenbianlv].x = file.GetWord();
			sDlgRect[eFenbianlv].y = file.GetWord();
			sDlgRect[eFenbianlv].cx = file.GetWord();
			sDlgRect[eFenbianlv].cy = file.GetWord();
			continue;
		}
		if(strcmp(buf,"#提示消息")==0)
		{
			sDlgRect[eTipMsg].x = file.GetWord();
			sDlgRect[eTipMsg].y = file.GetWord();
			sDlgRect[eTipMsg].cx = file.GetWord();
			sDlgRect[eTipMsg].cy = file.GetWord();
			continue;
		}
		if(strcmp(buf,"#下载提示")==0)
		{
			sDlgRect[eFileMsg].x = file.GetWord();
			sDlgRect[eFileMsg].y = file.GetWord();
			sDlgRect[eFileMsg].cx = file.GetWord();
			sDlgRect[eFileMsg].cy = file.GetWord();
			continue;
		}
		if(strcmp(buf,"#版本号")==0)
		{
			sDlgRect[eVerMsg].x = file.GetWord();
			sDlgRect[eVerMsg].y = file.GetWord();
			sDlgRect[eVerMsg].cx = file.GetWord();
			sDlgRect[eVerMsg].cy = file.GetWord();
			continue;
		}
		if(strcmp(buf,"#公告")==0)
		{
			sDlgRect[eWebRect].x = file.GetWord();
			sDlgRect[eWebRect].y = file.GetWord();
			sDlgRect[eWebRect].cx = file.GetWord();
			sDlgRect[eWebRect].cy = file.GetWord();
			continue;
		}
		if(strcmp(buf,"#公告背景色")==0)
		{
			sTextColor[eWeb].Red = file.GetWord();
			sTextColor[eWeb].Green = file.GetWord();
			sTextColor[eWeb].Blue = file.GetWord();
			continue;
		}
		if(strcmp(buf,"#字体颜色")==0)
		{
			sTextColor[eFont].Red = file.GetWord();
			sTextColor[eFont].Green = file.GetWord();
			sTextColor[eFont].Blue = file.GetWord();
			continue;
		}
		if(strcmp(buf,"#版本号颜色")==0)
		{
			sTextColor[eVer].Red = file.GetWord();
			sTextColor[eVer].Green = file.GetWord();
			sTextColor[eVer].Blue = file.GetWord();
			continue;
		}
		if(strcmp(buf,"#提示消息颜色")==0)
		{
			sTextColor[eTip].Red = file.GetWord();
			sTextColor[eTip].Green = file.GetWord();
			sTextColor[eTip].Blue = file.GetWord();
			continue;
		}
		if(strcmp(buf,"#下载提示颜色")==0)
		{
			sTextColor[eFile].Red = file.GetWord();
			sTextColor[eFile].Green = file.GetWord();
			sTextColor[eFile].Blue = file.GetWord();
			continue;
		}
		if(strcmp(buf,"#字体")==0)
		{
			sFontSet.Size = file.GetWord();
			file.GetString(sFontSet.FontName);
			continue;
		}
	}
	file.Release();
}
//当用户拖动最小化窗口时系统调用此函数取得光标显示。
//--------------------------------事件重载函数定义--------------------------------//
HCURSOR CMHAutoPatchDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
//当用户拖动最小化窗口时系统调用此函数取得光标显示。
void CMHAutoPatchDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// 调用父类处理函数完成基本操作
	CDialog::OnLButtonDown(nFlags, point);

	// 发送WM_NCLBUTTONDOWN消息
	// 使Windows认为鼠标在标题条上
	PostMessage(WM_NCLBUTTONDOWN,
		HTCAPTION,
		MAKELPARAM(point.x, point.y));  
}
HBRUSH CMHAutoPatchDlg::OnCtlColor(CDC* pDC,CWnd* pWnd,UINT nCtlColor)
{
	pDC->SelectObject(&m_font);       //设置字体 
	pDC->SetTextColor(RGB(sTextColor[eFont].Red,sTextColor[eFont].Green,sTextColor[eFont].Blue));  //设置字体颜色
	pDC->SetBkMode(TRANSPARENT);      //属性设置为透明
	switch(pWnd->GetDlgCtrlID())
	{
		case IDC_STATIC_Tip:
				return hbr;
		case IDC_STATIC_VER:
				return hbr2;
		case IDC_STATIC_FILESIZETIP:
				return hbr1;
	}
	
}
//--------------------------------对话框类成员函数定义--------------------------------//
void CMHAutoPatchDlg::MyPaint()
{
	//开始按钮
	m_BtnStart.LoadBitmaps(IDB_BITMAP_START_U,IDB_BITMAP_START_D,IDB_BITMAP_START_U,IDB_BITMAP_START_X);
	m_BtnStart.SubclassDlgItem(IDC_BUTTON_START,this);
	m_BtnStart.SizeToContent();
	m_BtnStart.SetWindowPos(NULL,sDlgRect[eStart].x,sDlgRect[eStart].y,sDlgRect[eStart].cx,sDlgRect[eStart].cy,SWP_SHOWWINDOW);
	//充值按钮
	//m_BtnPay.LoadBitmaps(IDB_BITMAP_PAY_U,IDB_BITMAP_PAY_D,IDB_BITMAP_PAY_U);
	//m_BtnPay.SubclassDlgItem(IDC_BUTTON_PAY,this);
	//m_BtnPay.SizeToContent();
	//m_BtnPay.SetWindowPos(NULL,sDlgRect[ePay].x,sDlgRect[ePay].y,sDlgRect[ePay].cx,sDlgRect[ePay].cy,SWP_SHOWWINDOW);
	//官网按钮
	//m_BtnHomePage.LoadBitmaps(IDB_BITMAP_HOME_U,IDB_BITMAP_HOME_D,IDB_BITMAP_HOME_U);
	//m_BtnHomePage.SubclassDlgItem(IDC_BUTTON_HOMEPAGE,this);
	//m_BtnHomePage.SizeToContent();
	//m_BtnHomePage.SetWindowPos(NULL,sDlgRect[eHome].x,sDlgRect[eHome].y,sDlgRect[eHome].cx,sDlgRect[eHome].cy,SWP_SHOWWINDOW);
	//客服按钮
	//关闭按钮
	m_BtnClose.LoadBitmaps(IDB_BITMAP_CLOSE_U,IDB_BITMAP_CLOSE_D);
	m_BtnClose.SubclassDlgItem(IDC_BUTTON_CLOSE,this);
	m_BtnClose.SizeToContent();
	m_BtnClose.SetWindowPos(NULL,sDlgRect[eClose].x,sDlgRect[eClose].y,sDlgRect[eClose].cx,sDlgRect[eClose].cy,SWP_SHOWWINDOW);
	//窗口模式
	m_CheckWndMode.SetWindowPos(NULL,sDlgRect[eWindow].x,sDlgRect[eWindow].y,sDlgRect[eWindow].cx,sDlgRect[eWindow].cy,SWP_SHOWWINDOW);
	m_CheckWndMode.SetCheck(TRUE);
	//当前进度条
	//m_Progress.SetWindowPos(NULL,sDlgRect[ePro].x,sDlgRect[ePro].y,sDlgRect[ePro].cx,sDlgRect[ePro].cy,SWP_SHOWWINDOW);
	//m_Progress.SendMessage(PBM_SETBKCOLOR, 0, RGB(sTextColor[eProb].Red, sTextColor[eProb].Green, sTextColor[eProb].Blue));
	//m_Progress.SendMessage(PBM_SETBARCOLOR, 0, RGB(sTextColor[eProf].Red, sTextColor[eProf].Green, sTextColor[eProf].Blue));
	//m_Progress.ShowWindow(0);
	//完成进度条
	m_ProgressDone.SetWindowPos(NULL,sDlgRect[eProd].x,sDlgRect[eProd].y,sDlgRect[eProd].cx,sDlgRect[eProd].cy,SWP_SHOWWINDOW);
	m_ProgressDone.SendMessage(PBM_SETBKCOLOR, 0, RGB(sTextColor[eProdb].Red, sTextColor[eProdb].Green, sTextColor[eProdb].Blue));
	m_ProgressDone.SendMessage(PBM_SETBARCOLOR, 0, RGB(sTextColor[eProdf].Red, sTextColor[eProdf].Green, sTextColor[eProdf].Blue));
	//分辨率选择
	m_ComboBox.SetWindowPos(NULL,sDlgRect[eFenbianlv].x,sDlgRect[eFenbianlv].y,sDlgRect[eFenbianlv].cx,sDlgRect[eFenbianlv].cy,SWP_SHOWWINDOW);
	//提示消息
	m_StTip.SetWindowPos(NULL,sDlgRect[eTipMsg].x,sDlgRect[eTipMsg].y,sDlgRect[eTipMsg].cx,sDlgRect[eTipMsg].cy,SWP_SHOWWINDOW);
	m_StTip.RedrawWindow();
	//下载提示
	m_StDwnlTip.SetWindowPos(NULL,sDlgRect[eFileMsg].x,sDlgRect[eFileMsg].y,sDlgRect[eFileMsg].cx,sDlgRect[eFileMsg].cy,SWP_SHOWWINDOW);
	m_StDwnlTip.RedrawWindow();
	//版本号
	m_StDlSp.SetWindowPos(NULL,sDlgRect[eVerMsg].x,sDlgRect[eVerMsg].y,sDlgRect[eVerMsg].cx,sDlgRect[eVerMsg].cy,SWP_SHOWWINDOW);
	m_StDlSp.RedrawWindow();
	m_WebGG.SetWindowPos(NULL,sDlgRect[eWebRect].x,sDlgRect[eWebRect].y,sDlgRect[eWebRect].cx,sDlgRect[eWebRect].cy,SWP_SHOWWINDOW);
	m_WebGG.SendMessage(PBM_SETBKCOLOR, 0, RGB(sTextColor[eWeb].Red, sTextColor[eWeb].Green, sTextColor[eWeb].Blue));
}
void CMHAutoPatchDlg::OnBnClickedButtonStart()
{
	OnCbnSelchangeComboScreen();
	MakeLauchData();

	// 容错：防止 ExeName 为空
	if (strlen(ServerInfo.ExeName) == 0)
		strcpy(ServerInfo.ExeName, "MHClient-Connect.exe");

	std::string exePath = pCurPath;
	exePath += "\\";
	exePath += ServerInfo.ExeName;

	// 启动前检查文件是否存在
	if (GetFileAttributesA(exePath.c_str()) == INVALID_FILE_ATTRIBUTES)
	{
		CString err;
		err.Format("找不到执行档案：\n%s", exePath.c_str());
		AfxMessageBox(err);
		return;
	}

	std::string cmdLine = "\"" + exePath + "\"";
	if (ServerInfo.CommandLine[0] != '\0')
	{
		cmdLine += " ";
		cmdLine += ServerInfo.CommandLine;
	}

	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si, sizeof(si));
	ZeroMemory(&pi, sizeof(pi));
	si.cb = sizeof(si);

	if (CreateProcess(NULL, (LPSTR)cmdLine.c_str(), NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
	{
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
		OnBnClickedButtonClose();
	}
	else
	{
		DWORD err = GetLastError();
		CString msg;
		msg.Format("运行失败\n错误码: %lu", err);
		AfxMessageBox(msg);

		HANDLE hProcess = GetCurrentProcess();
		TerminateProcess(hProcess, 0);
	}
}




void CMHAutoPatchDlg::OnBnClickedButtonClose()
{
	StopTheard = TRUE;
	Sleep(1000);//主线程暂停1秒等待其他线程结束
	AfxGetMainWnd()->SendMessage(WM_CLOSE);
}
BOOL CMHAutoPatchDlg::LoadServerInfo()
{
	time_t nRand = time(NULL);
	std::string str;
	char buf[256] = {0,};
	CMHFile file;
	if(!file.Init(UPDATAURL, "rb"))
	{
		::MessageBox(NULL,"更新信息加载失败","错误",MB_OK);
		return FALSE;
	}
	while(!file.IsEOF())
	{
		file.GetString(buf);
		if (strcmp(buf,"*UpUrl")==0)
		{
			file.GetString(UpdataUrl);
			continue;
		}
		if(strcmp(buf,"*HomePage")==0)
		{
			file.GetString(ServerInfo.HomeUrl);
			continue;
		}
		if(strcmp(buf,"*PayUrl")==0)
		{
			file.GetString(ServerInfo.PayUrl);
			continue;
		}
		if(strcmp(buf,"*QQ")==0)
		{
			file.GetString(ServerInfo.QQ);
			continue;
		}
		if(strcmp(buf,"*Line")==0)
		{
			file.GetString(ServerInfo.CommandLine);
			continue;
		}
		if(strcmp(buf,"*ExeName")==0)
		{
			file.GetString(ServerInfo.ExeName);
			continue;
		}
		if(strcmp(buf,"*LunchVer")==0)
		{
			file.GetString(ServerInfo.LunchVer);
			continue;
		}
		if(strcmp(buf,"*GGUrl")==0)
		{
			file.GetString(ServerInfo.GGUrl);
			continue;
		}
		if(strcmp(buf,"*UpLuchUrl")==0)
		{
			file.GetString(ServerInfo.UpLuchUrl);
			continue;
		}
		if(strcmp(buf,"*LunchName")==0)
		{
			file.GetString(ServerInfo.LunchName);
			continue;
		}
	}
	file.Release();
	COleVariant sLoc(ServerInfo.GGUrl);
	m_WebGG.Navigate2(&sLoc,NULL,NULL,NULL,NULL);
	if(strcmp(ServerInfo.LunchVer,AutoPatchVer)!=0)
	{
		MessageBox("登录器需要更新，请稍等...","提示",MB_OK);
		UpDataLunch();
		return FALSE;
	}
	sprintf_s(buf,"/MyUpdata.txt?abc=%d",nRand);
	str = UpdataUrl;
	str += buf;
	if(S_OK!=URLDownloadToFile(NULL,str.c_str(),UPDATAFILESAVEPATH,NULL,NULL))
		return FALSE;
	else
	{
		MapIter = TipMsgInfo.find("3");
		CString str;
		str.Format(MapIter->second.c_str());
		AfxGetMainWnd()->SetDlgItemText(IDC_STATIC_Tip,str.GetBuffer());
		CWinThread* pThread = AfxBeginThread(StartCheckFile,NULL,THREAD_PRIORITY_NORMAL);
		return TRUE;
	}
	return FALSE;
}

void CMHAutoPatchDlg::OnBnClickedButtonPay()
{
	ShellExecute( NULL, "open", ServerInfo.PayUrl,  NULL, NULL, SW_SHOWNORMAL );
}

void CMHAutoPatchDlg::OnBnClickedButtonHomepage()
{
	ShellExecute( NULL, "open", ServerInfo.HomeUrl,NULL, NULL, SW_SHOWNORMAL );
}

void CMHAutoPatchDlg::OnBnClickedButtonQq()
{
	ShellExecute( NULL, "open",   ServerInfo.QQ, NULL,NULL, SW_SHOWNORMAL );
}
void CMHAutoPatchDlg::InitComBox()
{
	char buf[64],buf2[64];
	//std::ofstream ofn("./PrintScreenList.txt");
	MonitorAdapter          m_monitorAdapter;        
	VEC_MONITORMODE_INFO vecMointorListInfo;  
	std::vector<info>* a;
	m_monitorAdapter.GetAllDisplayMode(vecMointorListInfo);  
	int nWidth = 0, nHeight = 0, nFreq = 0, nBits = 0;  
	VEC_MONITORMODE_INFO::iterator itBeg = vecMointorListInfo.begin();  
	a=m_monitorAdapter.GetInfo();
	info n;
	int i = 0;
	for(vector<info>::iterator iter1=a->begin();iter1!=a->end();iter1++)
	{
		n=*iter1;
		sprintf_s(buf,"%d %d",n.m_nWidth,n.m_nHeight);
		m_ComboBox.InsertString(i,buf);

		//if(n.m_nWidth<800 ||n.m_nHeight<600)
		//{//舍弃800*600以下分辨率
		//	continue;
		//}
		//sprintf_s(buf2,"%u\t%u\t%u",i+1,n.m_nWidth,n.m_nHeight);
		//ofn<<buf2<<std::endl;
		i++;
	}
	//ofn.close();
	//m_ComboBox.SetCurSel(7);//4 //默认显示
}
void CMHAutoPatchDlg::OnCbnSelchangeComboScreen()
{
	std::ofstream outFile;
	CString str;
	int col;
	int WndMode;
	outFile.open("./AutopatchOption_N.opt");
	col = m_ComboBox.GetCurSel();
	WndMode = m_CheckWndMode.GetCheck();
	//str.Format("%d\n%d",WndMode,col+1);
	if(col != LB_ERR)
	{//add by rookie
		m_ComboBox.GetLBText(col,strBook);//选中的分辨率保存到strBook中
	}
	str.Format("#WINDOWMODE %u\n#RESOLUTION %s",WndMode,strBook);

	outFile<<str;
	outFile.close();
}
void CMHAutoPatchDlg::LoadTipMsg()
{
	std::string Temp,Temp1;
	ifstream InFile;
	InFile.open(".\\Image\\Autopatch\\TipMsg.txt",std::ios::in);
	while(!InFile.eof())
	{
		InFile>>Temp;
		InFile>>Temp1;
		TipMsgInfo[Temp]=Temp1;
	}
}
//--------------------------------全局函数定义--------------------------------//
std::string  MakePath(std::string& str)
{
	std::string tmp = pCurPath;
	tmp +=str;
	return tmp;
}
std::string  MakeUrl(std::string& str)
{
	std::string tmp = UpdataUrl;
	char buf[32];
	time_t nRand = time(NULL);
	sprintf_s(buf,"?abc=%d",nRand);//生成随机参数，防止读取cookie
	TextPars.replace_all(str,"\\","/");
	tmp += (str+buf);
	return tmp;
}
std::string  GetFileName(std::string& str)
{
	std::string result = str;
	int Pos = result.find_last_of("\\");
	result.replace(0,Pos+1,"");
	return result;
}
std::wstring Ansi2WChar(LPCSTR pszSrc, int nLen)  
{  
	int nSize = MultiByteToWideChar(CP_ACP, 0, (LPCSTR)pszSrc, nLen, 0, 0);  
	if(nSize <= 0) return NULL;  

	WCHAR *pwszDst = new WCHAR[nSize+1];  
	if( NULL == pwszDst) return NULL;  

	MultiByteToWideChar(CP_ACP, 0,(LPCSTR)pszSrc, nLen, pwszDst, nSize);  
	pwszDst[nSize] = 0;  

	if( pwszDst[0] == 0xFEFF) 
		for(int i = 0; i < nSize; i ++)   
			pwszDst[i] = pwszDst[i+1];  

	wstring wcharString(pwszDst);  
	delete pwszDst;  

	return wcharString;  
}  
UINT StartDownload(LPVOID pParam)
{
	CString str;
	((CProgressCtrl*)AfxGetMainWnd()->GetDlgItem(IDC_PROGRESS_DONE))->SetRange32(0,m_FileCount);
	int FileNum = m_FileCount;
	
	while(m_WaitDownlaodFileInfo.size())
	{
		MapIter = TipMsgInfo.find("4");
		//提示信息输出
		if(StopTheard)return 0;
		str.Format(MapIter->second.c_str(),m_FileCount);
		AfxGetMainWnd()->SetDlgItemText(IDC_STATIC_Tip,str.GetBuffer());
		//下载信息生成
		DownLoadInfo info;
		info.Path = MakePath(m_WaitDownlaodFileInfo.back());
		info.Url  = MakeUrl(m_WaitDownlaodFileInfo.back());
		//去掉已下载元素
		if(m_WaitDownlaodFileInfo.size())
			m_WaitDownlaodFileInfo.pop_back();
		//总进度条设置
		m_FileCount--;
		((CProgressCtrl*)AfxGetMainWnd()->GetDlgItem(IDC_PROGRESS_DONE))->SetPos(FileNum-m_FileCount);
		
		//开始下载文件
		CWinThread* pThread = AfxBeginThread(DownloadFile,&info,THREAD_PRIORITY_NORMAL);
		WaitForSingleObject(*pThread, INFINITE ); //等待线程结束
	}
	MapIter = TipMsgInfo.find("1");
	str.Format(MapIter->second.c_str());
	AfxGetMainWnd()->SetDlgItemText(IDC_STATIC_Tip,str.GetBuffer());
	AfxGetMainWnd()->SetDlgItemText(IDC_STATIC_FILESIZETIP,str.GetBuffer());
	AfxGetMainWnd()->GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
	return 0;
}
void MakeDownloadInfo(std::string &str)
{
	std::string FileSize;
	MapIter = TipMsgInfo.find("8");//KB
	FileSize = MapIter->second;
	MapIter = TipMsgInfo.find("5");//文件名
	str += MapIter->second;
	str += " ";

	MapIter = TipMsgInfo.find("6");
	str += MapIter->second;
	str += " ";
	str += FileSize;
	str += " ";

	MapIter = TipMsgInfo.find("7");
	str += MapIter->second;
	str += " ";
	str += FileSize;
	str += " ";
}
UINT DownloadFile(LPVOID pParam)
{
	std::string DownLoadFileInfo;
	
	MakeDownloadInfo(DownLoadFileInfo);
	DownLoadInfo* Info = (DownLoadInfo*)pParam;
	std::string FileName = GetFileName(Info->Path);
	CWnd*			pwnd = AfxGetMainWnd();
	CProgressCtrl*	m_Prog = (CProgressCtrl*)pwnd->GetDlgItem(IDC_PROGRESS);
	bool ret=false;  
	CInternetSession Sess("lpload");  
	Sess.SetOption(INTERNET_OPTION_CONNECT_TIMEOUT     , 20000); //20秒的连接超时  
	Sess.SetOption(INTERNET_OPTION_SEND_TIMEOUT        , 20000); //20秒的发送超时  
	Sess.SetOption(INTERNET_OPTION_RECEIVE_TIMEOUT     , 20000); //20秒的接收超时  
	Sess.SetOption(INTERNET_OPTION_DATA_SEND_TIMEOUT   , 20000); //20秒的发送超时  
	Sess.SetOption(INTERNET_OPTION_DATA_RECEIVE_TIMEOUT, 20000); //20秒的接收超时  
	DWORD dwFlag = INTERNET_FLAG_TRANSFER_BINARY|INTERNET_FLAG_DONT_CACHE|INTERNET_FLAG_RELOAD ;  
	CHttpFile* cFile   = NULL;  
	char      *pBuf    = NULL;  
	int        nBufLen = 0   ;  
	do {  
		try{  
			cFile = (CHttpFile*)Sess.OpenURL(Info->Url.c_str(),1,dwFlag);  
			DWORD dwStatusCode;  
			cFile->QueryInfoStatusCode(dwStatusCode);  
			if (dwStatusCode == HTTP_STATUS_OK) {  
				//查询文件长度  
				DWORD nLen=0;  
				cFile->QueryInfo(HTTP_QUERY_CONTENT_LENGTH, nLen);  

				m_Prog->SetRange32(0,nLen); 
				nBufLen=nLen;  
				CString str;
				str.Format(DownLoadFileInfo.c_str(),FileName.c_str(),nBufLen/1024,nLen/1024);
				pwnd->SetDlgItemText(IDC_STATIC_FILESIZETIP,str.GetBuffer());
				if (nLen <= 0) break;

				//分配接收数据缓存  
				pBuf = new char[nLen+8];
				ZeroMemory(pBuf,nLen+8);  

				char *p=pBuf;  
				while (nLen>0) {  
					if (StopTheard)
					{
						//释放缓存  
						if (pBuf) {  
							delete []pBuf;  
							pBuf=NULL;  
							nBufLen = 0 ;  
						}  

						//关闭下载连接  
						if (cFile) {  
							cFile->Close();  
							Sess.Close();  
							delete cFile;  
						}  
						return 0;
					}
					//每次下载8K  
					int n = cFile->Read(p,(nLen<RECVPACK_SIZE)?nLen:RECVPACK_SIZE);  
					//接收完成退出循环  
					if (n <= 0) break;//  
					//接收缓存后移  
					p+= n ;  
					//剩余长度递减  
					nLen -= n ;  
					m_Prog->SetPos(nBufLen-nLen);
					str.Format(DownLoadFileInfo.c_str(),FileName.c_str(),nBufLen/1024,nLen/1024);
					pwnd->SetDlgItemText(IDC_STATIC_FILESIZETIP,str.GetBuffer());
				}  
				//如果未接收完中断退出  
				if (nLen != 0) break;  
				//接收成功保存到文件  

				CFile file(Info->Path.c_str(), CFile::modeCreate | CFile::modeWrite);  
				file.Write(pBuf,nBufLen);  
				file.Close();  
				ret = true;  
			}  
		} catch(...) {  
			//cout << "异常" << endl;  
			break;//  
		}  
	} while(0);  

	//释放缓存  
	if (pBuf) {  
		delete []pBuf;  
		pBuf=NULL;  
		nBufLen = 0 ;  
	}  

	//关闭下载连接  
	if (cFile) {  
		cFile->Close();  
		Sess.Close();  
		delete cFile;  
	}  
	return ret;  
}


UINT StartCheckFile(LPVOID pParam)
{
	MapIter = TipMsgInfo.find("2");
	CString str;
	CheckFile.CheckFile(m_WaitDownlaodFileInfo);//文件校验
	if(m_WaitDownlaodFileInfo.size()>0)
	{
		m_FileCount  = m_WaitDownlaodFileInfo.size();
		str.Format(MapIter->second.c_str());
		AfxGetMainWnd()->SetDlgItemText(IDC_STATIC_Tip,str.GetBuffer());
		CWinThread* pThread = AfxBeginThread(StartDownload,NULL,THREAD_PRIORITY_NORMAL);
	}
	else
	{
		MapIter = TipMsgInfo.find("1");
		((CProgressCtrl*)AfxGetMainWnd()->GetDlgItem(IDC_PROGRESS_DONE))->SetRange32(0,100);
		((CProgressCtrl*)AfxGetMainWnd()->GetDlgItem(IDC_PROGRESS_DONE))->SetPos(100);
		((CProgressCtrl*)AfxGetMainWnd()->GetDlgItem(IDC_PROGRESS))->SetRange32(0,100);
		((CProgressCtrl*)AfxGetMainWnd()->GetDlgItem(IDC_PROGRESS))->SetPos(100);

		str.Format(MapIter->second.c_str());
		AfxGetMainWnd()->SetDlgItemText(IDC_STATIC_Tip,str.GetBuffer());
		AfxGetMainWnd()->GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
	}
	return 0;
}
void MakeLauchData()
{
	char buf[64]={0,};
	sprintf_s(buf,ServerInfo.CommandLine);
	std::ofstream outFile;
	outFile.open("./Log/Data.dat");
	if(strlen(buf)!=0)
	{
		for(int i = 0;i<strlen(buf);i++)
			buf[i] += i%2; //+=10
	}
	outFile<<buf;
	outFile.close();
}
void UpDataLunch()
{
	std::string Patch,Commandl;

	Patch = pCurPath;
	Patch += "\\Image\\Autopatch\\UpDataLuch.exe";

	Commandl = ServerInfo.UpLuchUrl;
	Commandl += "/";
	Commandl +=ServerInfo.LunchName;
	Commandl += "|";
	Commandl +=pCurPath;
	Commandl +="\\";
	Commandl +=ServerInfo.LunchName;

	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	ZeroMemory( &pi, sizeof(pi) );
	ZeroMemory( &si, sizeof(si) );
	si.cb = sizeof(si);
	if(CreateProcess((LPSTR)Patch.c_str(),(LPSTR)Commandl.c_str(), NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
	{
		CloseHandle( pi.hProcess );
		CloseHandle( pi.hThread );
	}
	else 
	{
		AfxMessageBox("运行失败");
		HANDLE hProcess = GetCurrentProcess();//get current process
		TerminateProcess(hProcess,0);         //close process
	}
	//flogFile.close();
	AfxGetMainWnd()->SendMessage(WM_CLOSE);
}

void CMHAutoPatchDlg::LoadWindowSetting()
{
	std::ifstream inFile("./AutopatchOption_N.opt");
	if (!inFile.is_open()) return;

	std::string line;
	int wndMode = 1;
	CString resolutionStr;

	while (getline(inFile, line))
	{
		if (line.find("#WINDOWMODE") != std::string::npos)
		{
			sscanf(line.c_str(), "#WINDOWMODE %d", &wndMode);
		}
		else if (line.find("#RESOLUTION") != std::string::npos)
		{
			// 提取整个分辨率字符串
			size_t pos = line.find("#RESOLUTION ");
			if (pos != std::string::npos)
			{
				std::string resStr = line.substr(pos + strlen("#RESOLUTION "));
				resolutionStr = resStr.c_str();
			}
		}
	}
	inFile.close();

	// 设置窗口模式勾选
	m_CheckWndMode.SetCheck(wndMode);

	// 设置分辨率 ComboBox 默认项
	int count = m_ComboBox.GetCount();
	for (int i = 0; i < count; ++i)
	{
		CString temp;
		m_ComboBox.GetLBText(i, temp);
		if (temp == resolutionStr)
		{
			m_ComboBox.SetCurSel(i);
			return;
		}
	}

	// 如果找不到匹配项，就选第一个
	m_ComboBox.SetCurSel(0);
}


void CMHAutoPatchDlg::OnTimer(UINT_PTR nIDEvent)
{
	switch(nIDEvent)
	{
	case 1:
		{
			KillTimer(1);
			LoadServerInfo();			
		}
		break;
	}
//	CDialog::OnTimer(nIDEvent);
}
