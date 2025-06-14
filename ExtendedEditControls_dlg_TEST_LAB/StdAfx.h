
#pragma once
#include <sdkddkver.h>
#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers
//#define _AFXDLL 
#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
//#include <afxdisp.h>        // MFC Automation classes
#if _MFC_VER > 0x0421
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#endif // _MFC_VER > 0x0421
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#include <afxcontrolbars.h>
#endif // _AFX_NO_AFXCMN_SUPPORT
#include "afxbutton.h"

#include <JackHeaderStruct.h>
//#include "include\OXAdvancedAssert.h"
#include "OPini.h"
//#include <size>
#include <map>
#include <vector>
#include <math.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <locale.h>

#include <uxtheme.h>
#include "Common.h"
#include "CommonDefine.h"
#include "DarkMode.h"
#include "UAHMenuBar.h"
//#include "scintilla/include/SciLexer.h"
//#include "scintilla/include/Scintilla.h"

#include <dwmapi.h>
#include <afxdlgs.h>
#ifndef DWMWA_USE_IMMERSIVE_DARK_MODE
#define DWMWA_USE_IMMERSIVE_DARK_MODE 20
#endif
auto YES = TRUE;
#include ".\Server_CMD.h"
using namespace std;
//namespace fs = std::filesystem;