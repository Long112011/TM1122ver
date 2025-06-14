
#pragma once
#include "..\[CC]JACK\JackHeaderStruct.h"

#define WIN32_LEAN_AND_MEAN		// 

#define _WIN32_WINNT	0x0500

#define _FILE_BIN_
#define _CRYPTCHECK_
//#define _KOR_LOCAL_

#pragma warning(disable : 4786)

#include <intrin.h>
#include <windows.h>
#include <winsock2.h>
#include <ole2.h>
#include <initguid.h>
#include <stdio.h>
#include <assert.h>
// TODO: reference additional headers your program requires here

#include "DataBase.h"

#include "Console.h"
 
#include <yhlibrary.h>
//#include "CommonHeader.h"
#include "..\[CC]Header\vector.h"
#include "..\[CC]Header\protocol.h"
#include "..\[CC]Header\CommonDefine.h"
#include "..\[CC]Header\CommonGameDefine.h"
#include "..\[CC]Header\ServerGameDefine.h"
#include "..\[CC]Header\CommonGameStruct.h"
#include "..\[CC]Header\CommonStruct.h"
#include "..\[CC]Header\ServerGameStruct.h"
#include "..\[CC]Header\CommonGameFunc.h"


#include ".\ServerSystem.h"


