







// Console.cpp: implementation of the CConsole class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Console.h"
#include <stdio.H>


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CConsole g_Console;

CConsole::CConsole()
{
	m_pIConsole = NULL;


//	CoInitialize(NULL);
}


CConsole::~CConsole()
{
//	CoUninitialize();

}

BOOL CConsole::Init(char * txt, int MaxButton, MENU_CUSTOM_INFO* pMenuInfo, cbRetrnFunc commandFunc)
{
	HRESULT hr;
	
	hr = CoCreateInstance(
           CLSID_ULTRA_TCONSOLE,
           NULL,
           CLSCTX_INPROC_SERVER,
           IID_ITConsole,
           (void**)&m_pIConsole);

	

	if(FAILED(hr))
		return FALSE;

	LOGFONT logFont;
	logFont.lfHeight = 16;// 16;//17 

	logFont.lfWidth			= 0; 
	logFont.lfEscapement	= 0; 
	logFont.lfOrientation	= 0; 
	logFont.lfWeight = FW_NORMAL;// FW_NORMAL;//
	logFont.lfItalic		= 0; 
	logFont.lfUnderline		= 0; 

	logFont.lfStrikeOut		= 0; 
	logFont.lfCharSet = DEFAULT_CHARSET;//GB2312_CHARSET; //
	logFont.lfOutPrecision	= 0; 
	logFont.lfClipPrecision	= 0; 
	logFont.lfQuality = PROOF_QUALITY;
	logFont.lfPitchAndFamily	= 0; 
	//strcpy(logFont.lfFaceName, "ÐÂËÎÌå");//ËÎÌå
	strcpy(logFont.lfFaceName, "Consolas");// MingLiU

	HWND hWnd;
	MHTCONSOLE_DESC	desc;

	desc.szConsoleName = PARSEKEY(txt);
	desc.dwRefreshRate = 1000;
	desc.wLogFileType = LFILE_DESTROYLOGFILEOUT;//0;//LFILE_LOGOVERFLOWFILEOUT;
	desc.szLogFileName = "./Log/ConsoleLog.txt";
	desc.dwFlushFileBufferSize = 10000;
	desc.dwDrawTimeOut	= 1000*60*3;
	desc.wMaxLineNum = 65535;//1000
	desc.dwListStyle = TLO_NOTMESSAGECLOSE|TLO_LINENUMBER|TLO_SCROLLTUMBTRACKUPDATE;
	desc.Width	= 800;
	desc.Height = 400;
	desc.pFont = &logFont;

	desc.nCustomMunuNum = MaxButton;
	desc.cbReturnFunc = commandFunc;

	desc.pCustomMenu = pMenuInfo;
	m_pIConsole->CreateConsole(&desc, &hWnd);

	return TRUE;
}

void CConsole::Release()
{
	if(m_pIConsole)
	{

		m_pIConsole->Release();
		m_pIConsole = NULL;

	}
	
}

void CConsole::Log(int LogType,int MsgLevel,char* LogMsg,...)
{
	static char   va_Temp_Buff[1024];
	
	va_list vl;

	va_start(vl, LogMsg);
	vsprintf(va_Temp_Buff, LogMsg, vl);
	va_end(vl);


	switch(LogType) 
	{
	case eLogDisplay:
		m_pIConsole->OutputFile(va_Temp_Buff,strlen(va_Temp_Buff));
		break;
	case eLogFile:
		{
			//m_pIConsole->WriteFile(va_Temp_Buff,strlen(va_Temp_Buff),MsgLevel);
		}
		break;
	default:
		MessageBox(NULL,"Not Defined LogType",0,0);
	}
}

void CConsole::LOG(int MsgLevel,char* LogMsg,...)
{
	static char   va_Temp_Buff[1024];
	
	va_list vl;


	va_start(vl, LogMsg);
	vsprintf(va_Temp_Buff, LogMsg, vl);
	va_end(vl);

	if(m_pIConsole)
	m_pIConsole->OutputDisplay(va_Temp_Buff,strlen(va_Temp_Buff));
}
void CConsole::LOGANDFILE(char* FilePath, DWORD color, char* strMsg, ...)
{
	static char   va_Temp_Buff[4096];

	va_list vl;

	va_start(vl, strMsg);
	vsprintf(va_Temp_Buff, strMsg, vl);
	va_end(vl);

	if (m_pIConsole)
		m_pIConsole->OutputDisplayEx(va_Temp_Buff, strlen(va_Temp_Buff), color);

	FILE* OutFile;
	fopen_s(&OutFile, FilePath, "a+");
	if (OutFile)
	{
		fprintf(OutFile,"%s\t%s\n", va_Temp_Buff,GetCurTimeToString());
		fclose(OutFile);
	}
}
/*void CConsole::LOGEX(int LogLevel,DWORD color,char* LogMsg,...)
{
	static char   va_Temp_Buff[1024];

	va_list vl;


	va_start(vl, LogMsg);
	vsprintf(va_Temp_Buff, LogMsg, vl);
	va_end(vl);

	if(m_pIConsole)
		m_pIConsole->OutputDisplayEx(va_Temp_Buff,strlen(va_Temp_Buff),color);
}*/
void CConsole::WaitMessage()
{
	m_pIConsole->MessageLoop();
}

void CConsole::LOC(DWORD color,int MsgLevel,char* LogMsg,...)
{
	static char   va_Temp_Buff[1024];
	
	va_list vl;


	va_start(vl, LogMsg);
	vsprintf(va_Temp_Buff, LogMsg, vl);
	va_end(vl);

	if(m_pIConsole)
	m_pIConsole->OutputDisplayEx(va_Temp_Buff,strlen(va_Temp_Buff),color);
}