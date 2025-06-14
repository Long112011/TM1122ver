#pragma once
#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		
#endif
#ifndef WINVER				
#define WINVER 0x0501		
#endif
#ifndef _WIN32_WINNT		
#define _WIN32_WINNT 0x0501		
#endif						
#ifndef _WIN32_WINDOWS		
#define _WIN32_WINDOWS 0x0510 
#endif
#ifndef _WIN32_IE			
#define _WIN32_IE 0x0501	
#endif
#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	
#define _AFX_ALL_WARNINGS
#include "..\[CC]Jack\JackHeaderStruct.h"
#include <afxwin.h>
#include <afxext.h>
#include <afxdisp.h>
#include <afxdtctl.h>
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>
#endif
#pragma warning(disable:4786)
#include <map>
#include <set>
#include <strsafe.h>
#include "cmdline.h"
#define TT AfxMessageBox("test");
char* RemoveBracket(char* str);
class CLog
{
public:
	static void Put(const TCHAR*, ...);
	static void SetEnable(bool);

protected:
	CLog();
	~CLog();

	static bool mIsEnable;
};

using namespace std;


