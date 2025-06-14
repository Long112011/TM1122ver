
#pragma once

#include "resource.h"
#include "SettingStruct.h"
#include "splasher.h"
#include "singleinstance.h"
#include "StylerDlg.h"
#include "dialog.h"
class CMainApp : public CWinApp, public CSingleInstance
{
	theDlg		*m_StylerDlg;
	BOOL			m_SplashInit;
	
protected:

	BOOL			m_isStylerEnable;

	BOOL			m_scintillaLoaded;

	CSplashThread* pSplashThread;
	sGAMEAI			m_GameAI;
	HMODULE LoadLibraryFromApplicationDirectory(LPCTSTR lpFileName);

	HINSTANCE m_hScintilla;
	HINSTANCE m_hLexilla;
	HINSTANCE   m_hDll;
	int SplashInit(int timer);
public:
	void SetScintillaEnable(bool bFlag){ m_scintillaLoaded = bFlag; }
	BOOL GetScintillaEnable(){ return m_scintillaLoaded; }

	/******************for styler******************/
	void SetStylerEnable(bool bFlag){ m_isStylerEnable = bFlag; }
	BOOL GetStylerEnable(){ return m_isStylerEnable; }


	virtual void WakeUp(LPCTSTR aCommandLine) const;

	HMODULE m_hSciDLL;
	HMODULE m_hLexDLL;
	sGAMEAI* GetGameAI() { return &m_GameAI; }
	void defaultConfig();
	void WriteConfig();
	void SplashQuit();
	CMainApp();

	//Lexilla::CreateLexerFn m_pCreateLexer;
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	
	DECLARE_MESSAGE_MAP()
	void LoadConfig_OLD();
	void ReadConfig();
	

	char  UserName[MAX_NAME_LENGTH + 1];
	char  UserPwd[MAX_NAME_LENGTH + 1];
};
//extern bool Scintilla_RegisterClasses(void *hInstance);
//extern bool Scintilla_ReleaseResources();
extern CMainApp theApp;
extern COLORREF black, white, green, red, blue, yellow, magenta, cyan, jackfgClr, jackClr, jackLineNumfgClr, jackLineNumClr;