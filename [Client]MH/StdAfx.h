#pragma once


//

#ifndef WINVER					// Specifies that the minimum required platform is Windows Vista.
#define WINVER 0x0501			// Change this to the appropriate value to target other versions of Windows.
#define _WIN32_WINNT	0x0501
#endif

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers

#define _FOR_MUKHYANG
#define _CLIENTVERSIONCHK_
#define _FILE_BIN_
#define _MHCLIENT_
#define _CLIENTJ_
#define _CRYPTCHECK_
//#define _KOR_LOCAL_
#define _CHEATENABLE_
#define _ASSERTENABLE_

#include "..\[CC]JACK\JackHeaderStruct.h"

#pragma warning(disable:4100)
#pragma warning(disable:4127)
#pragma warning(disable:4201)
#pragma warning(disable:4512)
#pragma warning(disable: 4504)
#pragma warning(disable:4786)

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_WARNINGS 1
#pragma comment(lib, "ws2_32.lib")
#endif

#define	VERSION	"1.0"
#define	AUTHOR	"JACK Trinity"
#define YEAR	((((__DATE__ [7] - '0') * 10 + (__DATE__ [8] - '0')) * 10 + (__DATE__ [9] - '0')) * 10 + (__DATE__ [10] - '0'))

#ifdef WIN32
#define close	closesocket
#define snprintf _snprintf
#endif
#ifdef WIN32	// Windows specific
#include <Windows.h>
#include <WinSock2.h>
#include <time.h>

#else			// Linux specific

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/time.h>

#endif


#include <windows.h>

// Windows Header Files:
#include <ole2.h>
#include <initguid.h>



// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <stdio.h>

#include <assert.h>

#include <intrin.h>
// TODO: reference additional headers your program requires here

#include <iostream>
#include <deque>
#include <vector>

/*#include <math.h>
#include <crtdbg.h>
#include <vfw.h>
#include <dbghelp.h>
#include <time.h>
#include <shellapi.h.>
#include <direct.h>
#include <conio.h>
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
#include <queue>*/

#include "resource.h"

#include "yhlibrary.h"
#include <map>

#include "..\4DyuchiGXGFunc\global.h"
#include "..\[CC]Header\protocol.h"
#include "..\[CC]Header\CommonDefine.h"
#include "..\[CC]Header\CommonGameDefine.h"
#include "..\[CC]Header\ClientGameDefine.h"
#include "..\[CC]Header\CommonGameStruct.h"
#include "..\[CC]Header\CommonStruct.h"
#include "..\[CC]Header\ClientGameStruct.h"
#include "..\[CC]Header\CommonGameFunc.h"

#include "Motion.h"
#include "MHNetwork.h"
#include "ErrorMsg.h"
#include "DirectoryManager.h"


#include "../[CC]JACK/gettimeofday.h"
// Error codes
#define	SUCCESS						0
#define	ERROR_POUTOFMEMORY			100
#define	ERROR_SOCKET_CANNOTRESOLVE	101
#define	ERROR_SOCKET_TIMEOUT		102
#define	ERROR_SOCKET_GENERALFAILURE	103
#define	ERROR_INVALIDARGUMENTS		200


#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <stdio.h>



#include <sys/types.h>
#include <fcntl.h>

// Types
typedef	const		wchar_t*	pcw_t;
typedef	const		char*		pcc_t;
typedef				wchar_t*	pwc_t;
typedef				char*		pc_t;
typedef	unsigned	short		ushort_t;

#include "../[CC]JACK/host.h"
#include "../[CC]JACK/timer.h"


#include "../[CC]JACK/socket.h"

/*#include "print.h"
#include "i18n.h"
#include "stats.h"

#include "arguments.h"


#include "ping.h"*/
extern BOOL g_bUsingEnginePack;