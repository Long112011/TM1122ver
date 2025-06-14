
#pragma once
#include "..\[CC]JACK\JackHeaderStruct.h"

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#define _WIN32_WINNT	0x0500

#define _FILE_BIN_
#define _CRYPTCHECK_
//#define _KOR_LOCAL_

#pragma warning(disable : 4786)
//#include <afxwin.h>
#include <windows.h>
#include <winsock2.h>
#include <ole2.h>
#include <initguid.h>
#include <stdio.h>

#include <assert.h>
#include <intrin.h>
// TODO: reference additional headers your program requires here

#include "DataBase.h"
#include "Console.h"
 
#include <yhlibrary.h>
//#include "CommonHeader.h"
#include "vector.h"
#include "protocol.h"

#include "CommonDefine.h"
#include "CommonGameDefine.h"
#include "ServerGameDefine.h"
#include "CommonGameStruct.h"
#include "CommonStruct.h"

#include "ServerGameStruct.h"

#include "CommonGameFunc.h"



#include ".\ServerSystem.h"


