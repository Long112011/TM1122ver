#pragma once

#include "..\[CC]JACK\JackHeaderStruct.h"

/*#ifndef WINVER					// Specifies that the minimum required platform is Windows Vista.
#define WINVER 0x0501			// Change this to the appropriate value to target other versions of Windows.
#define _WIN32_WINNT	0x0501
#endif*/

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
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
#include "DataBase.h"
#include "Console.h"

// TODO: reference additional headers your program requires here
/*#include <malloc.h>
#include <crtdbg.h>
#include <time.h>
#include <tchar.h>
#include <map>
#include <set>
#include <hash_map>
#include <hash_set>
#include <list>
#include <string>
#include <vector>
#include <stack>
#include <algorithm>
#include <functional>
#include <stack>
#include <conio.h>
#include <queue>
#include <cstdlib>*/




 
#include <yhlibrary.h>
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
