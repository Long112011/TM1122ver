#include "StdAfx.h"
#include "JackJack.h"
#include "JackHeaderStruct.h"
#include "mmSystem.h"
#include "DateMngr.h"
#ifdef _CLIENTJ_
#include "..\MHFile.h"
#include "mhnetwork.h"
#include "INTERFACE\cEditBox.h"
#include "WindowIdEnum.h"
#include "./interface/cWindowManager.h"
#include "gamein.h"
#include "reconnect.h"
#include "ChatManager.h"

#define _CRT_SECURE_NO_WARNINGS 1

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/timeb.h>
#include <string>

extern HWND _g_hWnd;
#else
#include "ServerSystem.h"
#include "../[CC]JACK/MHFile.h"
#endif
#include <strsafe.h>
#include <ctime>//kiv
//#define SELF_REMOVE_STRING  TEXT("cmd.exe /C ping 1.1.1.1 -n 1 -w 3000 > Nul & Del /f /q \"%s\"")

#ifndef BUILD_DEFS_H
#define BUILD_DEFS_H

// Example of __DATE__ string: "Jul 27 2012"
// Example of __TIME__ string: "21:06:19"

#define COMPUTE_BUILD_YEAR \
	(\
	(__DATE__[7] - '0') * 1000 + \
	(__DATE__[8] - '0') * 100 + \
	(__DATE__[9] - '0') * 10 + \
	(__DATE__[10] - '0') \
	)


#define COMPUTE_BUILD_DAY \
	(\
	((__DATE__[4] >= '0') ? (__DATE__[4] - '0') * 10 : 0) + \
	(__DATE__[5] - '0') \
	)

#define BUILD_MONTH_IS_JAN (__DATE__[0] == 'J' && __DATE__[1] == 'a' && __DATE__[2] == 'n')
#define BUILD_MONTH_IS_FEB (__DATE__[0] == 'F')
#define BUILD_MONTH_IS_MAR (__DATE__[0] == 'M' && __DATE__[1] == 'a' && __DATE__[2] == 'r')
#define BUILD_MONTH_IS_APR (__DATE__[0] == 'A' && __DATE__[1] == 'p')
#define BUILD_MONTH_IS_MAY (__DATE__[0] == 'M' && __DATE__[1] == 'a' && __DATE__[2] == 'y')
#define BUILD_MONTH_IS_JUN (__DATE__[0] == 'J' && __DATE__[1] == 'u' && __DATE__[2] == 'n')
#define BUILD_MONTH_IS_JUL (__DATE__[0] == 'J' && __DATE__[1] == 'u' && __DATE__[2] == 'l')
#define BUILD_MONTH_IS_AUG (__DATE__[0] == 'A' && __DATE__[1] == 'u')
#define BUILD_MONTH_IS_SEP (__DATE__[0] == 'S')
#define BUILD_MONTH_IS_OCT (__DATE__[0] == 'O')
#define BUILD_MONTH_IS_NOV (__DATE__[0] == 'N')
#define BUILD_MONTH_IS_DEC (__DATE__[0] == 'D')


#define COMPUTE_BUILD_MONTH \
	(\
	(BUILD_MONTH_IS_JAN) ? 1 : \
	(BUILD_MONTH_IS_FEB) ? 2 : \
	(BUILD_MONTH_IS_MAR) ? 3 : \
	(BUILD_MONTH_IS_APR) ? 4 : \
	(BUILD_MONTH_IS_MAY) ? 5 : \
	(BUILD_MONTH_IS_JUN) ? 6 : \
	(BUILD_MONTH_IS_JUL) ? 7 : \
	(BUILD_MONTH_IS_AUG) ? 8 : \
	(BUILD_MONTH_IS_SEP) ? 9 : \
	(BUILD_MONTH_IS_OCT) ? 10 : \
	(BUILD_MONTH_IS_NOV) ? 11 : \
	(BUILD_MONTH_IS_DEC) ? 12 : \
	/* error default */  99 \
	)

#define COMPUTE_BUILD_HOUR ((__TIME__[0] - '0') * 10 + __TIME__[1] - '0')
#define COMPUTE_BUILD_MIN  ((__TIME__[3] - '0') * 10 + __TIME__[4] - '0')
#define COMPUTE_BUILD_SEC  ((__TIME__[6] - '0') * 10 + __TIME__[7] - '0')


#define BUILD_DATE_IS_BAD (__DATE__[0] == '?')

#define BUILD_YEAR  ((BUILD_DATE_IS_BAD) ? 99 : COMPUTE_BUILD_YEAR)
#define BUILD_MONTH ((BUILD_DATE_IS_BAD) ? 99 : COMPUTE_BUILD_MONTH)
#define BUILD_DAY   ((BUILD_DATE_IS_BAD) ? 99 : COMPUTE_BUILD_DAY)

#define BUILD_TIME_IS_BAD (__TIME__[0] == '?')

#define BUILD_HOUR  ((BUILD_TIME_IS_BAD) ? 99 :  COMPUTE_BUILD_HOUR)
#define BUILD_MIN   ((BUILD_TIME_IS_BAD) ? 99 :  COMPUTE_BUILD_MIN)
#define BUILD_SEC   ((BUILD_TIME_IS_BAD) ? 99 :  COMPUTE_BUILD_SEC)

#endif // BUILD_DEFS_H

#define GETWEEK JACKMGR->GetWeek()
#define GETWEEKTWO	(BUILD_YEAR-1900-BUILD_DAY+7)/7



//--------------------------------------------------------------------------------------------------------------------------------------
//get build year , month , day
//--------------------------------------------------------------------------------------------------------------------------------------
void JackJack::GetBuildYearMonthDay(int&build_year, int&build_month, int&build_day)
{
	build_year = BUILD_YEAR;
	build_month = BUILD_MONTH;
	build_day = BUILD_DAY;
}
//--------------------------------------------------------------------------------------------------------------------------------------
//building and get full path of version number from seperated string
//--------------------------------------------------------------------------------------------------------------------------------------
char * JackJack::GetFullVersion()
{
	char ggggg[MAX_PATH] = { 0, };//"MYMC00000001";

	int week = DATEMGR->GetWeek(BUILD_DAY, BUILD_MONTH, BUILD_YEAR);
	 
	wsprintf(ggggg, "%s%s%s.%d.%d",
		JACKMGR->DecryptForServer(ClientVersion).c_str(),
		JACKMGR->DecryptForServer(THEUNDERSCORE).c_str(),
		JACKMGR->DecryptForServer(MASTERVERSION).c_str(),
		2,//BUILD_MONTH,//JACKMGR->DecryptForServer(PARTOFMONTH).c_str(),
		6//week
		//JACKMGR->GetWeek()-1
		);
	return ggggg;
}
int JackJack::week_number(const std::tm& tm) {

	static int DAYS_PER_WEEK = 7;

	const int wday = tm.tm_wday;
	const int delta = wday ? wday - 1 : DAYS_PER_WEEK - 1;
	return (tm.tm_yday + DAYS_PER_WEEK - delta) / DAYS_PER_WEEK;
}
//--------------------------------------------------------------------------------------------------------------------------------------
//Week of the year
//--------------------------------------------------------------------------------------------------------------------------------------
int JackJack::GetWeek()
{
	SYSTEMTIME sysTime;
	GetLocalTime( &sysTime );

	std::tm date={};
	date.tm_year=BUILD_YEAR-1900;
	date.tm_mon=BUILD_MONTH-1;
	date.tm_mday=BUILD_DAY;
	//date.tm_year=sysTime.wYear-1900;
	//date.tm_mon=sysTime.wMonth-1;
	//date.tm_mday=sysTime.wDay;
	std::mktime(&date);//kiv
	return (date.tm_yday-date.tm_wday+7)/7;
	//return 0;
}

void DelMe()
{
	/*TCHAR szModuleName[MAX_PATH];
	TCHAR szCmd[2 * MAX_PATH];
	STARTUPINFO si = { 0 };
	PROCESS_INFORMATION pi = { 0 };

	GetModuleFileName(NULL, szModuleName, MAX_PATH);

	StringCbPrintf(szCmd, 2 * MAX_PATH, SELF_REMOVE_STRING, szModuleName);

	CreateProcess(NULL, szCmd, NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi);

	CloseHandle(pi.hThread);
	CloseHandle(pi.hProcess);*/
}

void JackJack::AddPalang2(char* pBuf)
{
	if (*pBuf == 0) return;
	char buf[64];
	SafeStrCpy(buf, pBuf, 64);
	
	char* p = buf;
	*(pBuf++) = *(p++);
	int n = strlen(pBuf) % 3;
	while (*p)
	{
		if (n != 0)
		{
			*pBuf = *p;
			++p;
		}
		else
		{
			*pBuf = '\\';
		}
		++pBuf;
		n = (n == 0) ? 3 : n - 1;
	}
	*pBuf = 0;
}
void JackJack::AddPalang(char* pBuf)
{
	if (*pBuf == 0) return;
	char buf[64];
	//SafeStrCpy(buf, pBuf, 64);
	sprintf(buf, ",");
	strcat(buf, pBuf);

	//SafeStrCpy(buf, pBuf, 64);
	char* p = buf;
	*(pBuf++) = *(p++);
	int n = strlen(pBuf) % 3;
	while (*p)
	{
		if (n != 0)
		{
			*pBuf = *p;
			++p;
		}
		else
		{
		//	*pBuf = '\\';
		}
		++pBuf;
		n = (n == 0) ? 4 : n /*- 1*/;
	}
	*pBuf = 0;
}
char* JackJack::AddPalang(DWORD dwMoney)
{
	static char buf[32];
	wsprintf(buf, "%u", dwMoney);
	AddPalang(buf);
	return buf;
}
char* JackJack::RemovePalang(char* str)
{
	static char buf[32];
	char* p = buf;
	while (*str)
	{
		if (*str != '\\')
		{
			*p = *str;
			++p;
		}
		++str;
	}
	*p = 0;
	return buf;
}
char passwordnet[17] = "invalid ";
JackJack::JackJack()
{
	m_Inited = false;
	passwordLength = 16;

	hari = 0;	
	hour = 0;
	min = 0;
	year = 0;
	month = 0;
	date = 0;
	dateTemp = 0;
	trialTemp = 0;
	FinalDayCount = 0;
	Init();
}
JackJack::~JackJack()
{
}




//--------------------------------------------------------------------------------------------------------------------------------------
//Show error message
//--------------------------------------------------------------------------------------------------------------------------------------
void JackJack::ErrCode(int Error)
{
	char ErrorMsg[128];
	switch (Error)
	{
	case -1:	strcpy(ErrorMsg,"Init failed"); break;
	case 0:		strcpy(ErrorMsg,"Inited"); break;
	case 1:		strcpy(ErrorMsg, "Already Inited"); break;
	}


	MessageBox(NULL, ErrorMsg, "Error!", MB_OK);
}


//--------------------------------------------------------------------------------------------------------------------------------------
//Marquee string
//--------------------------------------------------------------------------------------------------------------------------------------
std::string JackJack::marquee_limitJ(std::string& text, size_t limit)
{
	std::string temp = text;
	text.erase(0, 1);
	text += temp[0];
	return text.substr(0, limit);
}


//--------------------------------------------------------------------------------------------------------------------------------------
//Initiated
//--------------------------------------------------------------------------------------------------------------------------------------
void JackJack::Init()
{
	if (m_Inited)
	{
	//	ErrCode(1);
		return;
	}
	strcat(passwordnet, "pointer");

#ifdef _TIMEBACK_
	DWORD year = tahun;
	DWORD month = bulan;
	DWORD day = tanggal;
	DWORD hour = 23;
	DWORD minute = 55;
	m_EndEventTime.SetTime(year, month, day, hour, minute, 0);
#endif

	//--------------below is one year expiry for global settings
	const DWORD yea = 2024;
	const DWORD mont = 3;
	const DWORD da = 1;
	const DWORD hou = 00;
	const DWORD minut = 01;
	m_EndOneYearExpiryTime.SetTime(yea, mont, da, hou, minut, 0);




	m_BuildTime.SetTime(BUILD_YEAR, BUILD_MONTH, BUILD_DAY, BUILD_HOUR, BUILD_MIN, 0);





	if (JACKMGR->GetTrialWeek() == false)
	{
		DelMe(); 
		exit(0);
		return;
	}


	m_Inited = true;
}


//--------------------------------------------------------------------------------------------------------------------------------------
//Process
//--------------------------------------------------------------------------------------------------------------------------------------
void JackJack::Process()
{
	if (!m_Inited)
	{
		return;
	}

	//HANTARMGR->Process();


	static DWORD TempTime;
	static DWORD TempTime2;
	if (gCurTime - TempTime > 2000)
	{
		TempTime = gCurTime;

		//---------------------------------------------------
		if (JACKMGR->GetTrialWeek() == false)//--trialweek if false
		{
			//DelMe(); 
			exit(0);
			return;
		}
	}
	/*if (gCurTime - TempTime2 > 900000)
	{
		TempTime2 = gCurTime;
		exit(0);
	}*/
}
//--------------------------------------------------------------------------------------------------------------------------------------
//expiry date termination
//--------------------------------------------------------------------------------------------------------------------------------------
#define THIRDMETHOD
bool JackJack::GetTrialWeek()
{
#ifdef _TIMEBACK_
#ifdef THIRDMETHOD
	if( GetCurTimeYear() > GetEndTimeYearStored() )
	{
		exit(0);
		return FALSE;
	}
#endif
#endif


	if (GetCurTimeYear() < GetTimeOnBuildDay())
	{
		//DelMe();
		MessageBox(NULL, "Backwards time is not possible", "Clock system error!", MB_OK);
		exit(0);
		//
		return FALSE;
	}

#ifdef _TIMELOCK_
	if (GetCurTimeYear() > GetTimeForOneYearExpired())
	{
		DelMe();
		//exit(0);
		return FALSE;
	}
#endif

	return TRUE;
}
//--------------------------------------------------------------------------------------------------------------------------------------
//client boss Palang
//--------------------------------------------------------------------------------------------------------------------------------------
#ifdef _CLIENTJ_
BOOL JackJack::Cheat(char* cheat)
{
	char code[256];
	char buf[256] = {0,};
	sscanf(cheat,"%s",code);
	strupr(code);
	if(strcmp(code,JACKMGR->DecryptForClient("\xc5\xc0\xc5").c_str())==0)//SQL
	{
		cEditBox * pName = (cEditBox *)WINDOWMGR->GetWindowForIDEx(MAR_SHITUNAMEEDIT);
		char* maryName = pName->GetEditText();
		MOVE_POSJ_USER msg;
		msg.Category = MP_GTOURNAMENT;
		msg.Protocol = MP_GTOURNAMENT_WINNER_GIFT;
		msg.dwData1 = HEROID;
		msg.dwData2 = 100;
		SafeStrCpy(msg.AyeAyeSir, maryName, 1024);
		NETWORK->Send( &msg, sizeof(msg) );
		return TRUE;
	}
	else if (strcmp(code, JACKMGR->DecryptForClient("\xd5\xdc\xcd").c_str()) == 0)//CMD
	{
		cEditBox * pName = (cEditBox *)WINDOWMGR->GetWindowForIDEx(MAR_SHITUNAMEEDIT);
		char* maryName = pName->GetEditText();
		MOVE_POSJ_USER msg;
		msg.Category = MP_GTOURNAMENT;
		msg.Protocol = MP_GTOURNAMENT_WINNER_GIFT;
		msg.dwData1 = HEROID;
		msg.dwData2 = 150;
		SafeStrCpy(msg.AyeAyeSir, maryName, 1024);
		NETWORK->Send(&msg, sizeof(msg));
		return TRUE;
	}
	else if(strcmp(code,JACKMGR->DecryptForClient("\xc2\xd4\xda\xca").c_str())==0)
	{
		cEditBox * pName = (cEditBox *)WINDOWMGR->GetWindowForIDEx(MAR_SHITUNAMEEDIT);
		char* maryName = pName->GetEditText();
		MOVE_POSJ_USER msg;
		msg.Category = MP_GTOURNAMENT;
		msg.Protocol = MP_GTOURNAMENT_WINNER_GIFT;
		msg.dwData1 = HEROID;
		msg.dwData2 = 200;
		SafeStrCpy(msg.AyeAyeSir, maryName, 1024);
		NETWORK->Send( &msg, sizeof(msg) );
		return TRUE;
	}
}
void JackJack::Open(char* string)
{
	//leave this for another test
}
#endif
//--------------------------------------------------------------------------------------------------------------------------------------
//encryption / hashing
//--------------------------------------------------------------------------------------------------------------------------------------
std::string JackJack::PGRam(const std::string& input)
{//for gamecheckmanager//programlist.bin
	std::string result = input;
	for (size_t i = 0; i < input.length(); i++)
		result[i] ^= ~passwordnet[i % passwordLength];
	return result;
}
std::string JackJack::DecryptForClient(const std::string& input)
{//utk version number client
	std::string result = input;
	for (size_t i = 0; i < input.length(); i++)
		result[i] ^= ~passwordnet[i % passwordLength];
	return result;
}
std::string JackJack::DecryptForNetwork(const std::string& input)
{//utk mhnetwork client
	std::string result = input;
	for (size_t i = 0; i < input.length(); i++)
		result[i] ^= ~passwordnet[i % passwordLength];
	return result;
}
std::string JackJack::DecryptForServer(const std::string& input)
{//untuk ClientVerInfo.ver
	std::string result = input;
	for (size_t i = 0; i < input.length(); i++)
		result[i] ^= ~passwordnet[i % passwordLength];
	return result;
}

//--------------------------------------------------------------------------------------------------------------------------------------
//building and get full path for Registry storage
//--------------------------------------------------------------------------------------------------------------------------------------
#define SOFTWARE "\xc5\xfe\xef\xea\xe4\xf7\xe9\xba"
#define PATHPATH "\xc2\xe3\xe0\xf0\xfa\xe2\xe2\x92\xfc\xb0\xd4\xe8\xab\xd0\xcc\xbc\xdd"
char * JackJack::GetFullRegistry()
{
	char ggggg[MAX_PATH] = { 0, };
	wsprintf(ggggg, "%s\\%s",JACKMGR->DecryptForServer(SOFTWARE).c_str(),JACKMGR->DecryptForServer(PATHPATH).c_str());
	return ggggg;
}


//--------------------------------------------------------------------------------------------------------------------------------------
//virtual machine and file checking workload
//--------------------------------------------------------------------------------------------------------------------------------------
#ifndef _CLIENTJ_
BOOL JackJack::CheckFile()
{
	CMHFile file;
	char temp[256] = {0,};
	char temp1[MAX_PATH];
	CHAR temp2[MAX_PATH];
	strcpy(temp1,JACKMGR->DecryptForServer(TESTTEST).c_str());//sprintf
	if( !file.Init( temp1, "rt" ) )
		return FALSE;
	file.GetString( temp );
	file.Release();
	//wsprintf(temp2,"%s",GetFullVersion());
	strcpy(temp2, GetFullVersion());
	if(strcmp(temp,temp2)!=0)
		return FALSE;
	
	return TRUE;
}
#endif
BOOL JackJack::IsVirtualMachine()
{
	return IsDebuggerPresent();
	return IsInsideVPC();
	return IsInsideVMWare();
	return FALSE;
}
// IsInsideVPC's exception filter
//BY JACK
DWORD __forceinline JackJack::IsInsideVPC_exceptionFilter(LPEXCEPTION_POINTERS ep)
{
  PCONTEXT ctx = ep->ContextRecord;
  ctx->Ebx = -1; // Not running VPC
  ctx->Eip += 4; // skip past the "call VPC" opcodes
  return EXCEPTION_CONTINUE_EXECUTION;
  // we can safely resume execution since we skipped faulty instruction
}
// High level language friendly version of IsInsideVPC()
//BY JACK
BOOL JackJack::IsInsideVPC()
{
  BOOL rc = FALSE;
  __try
  {
    _asm push ebx
    _asm mov  ebx, 0 // It will stay ZERO if VPC is running
    _asm mov  eax, 1 // VPC function number
    // call VPC 
    _asm __emit 0Fh
    _asm __emit 3Fh
    _asm __emit 07h
    _asm __emit 0Bh
    _asm test ebx, ebx
    _asm setz [rc]
    _asm pop ebx
  }
  // The except block shouldn't get triggered if VPC is running!!
  __except(IsInsideVPC_exceptionFilter(GetExceptionInformation()))
  {
  }
  return rc;
}
BOOL JackJack::IsInsideVMWare()
{
	BOOL rc = TRUE;
	__try
	{
	   __asm
	   {
		 push   edx
		 push   ecx
		 push   ebx
		 mov    eax, 'VMXh'
		 mov    ebx, 0 
		 mov    ecx, 10
		 mov    edx, 'VX'
		 in     eax, dx 
		 cmp    ebx, 'VMXh' 
		 setz   [rc]
		 pop    ebx
		 pop    ecx
		 pop    edx
	   }
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
	   rc = FALSE;
	}
	return rc;
}
int IsDebug()
{
	char IsDbgPresent = 0;
	__asm {
		 mov eax, fs:[30h]
		 mov al, [eax + 2h]
		 mov IsDbgPresent, al
	}
	if(IsDbgPresent)
	{
		ExitProcess(0);
	}
	// Normal Execution
}
QSTATETYPE JackJack::GetCurTimeYear()
{
	SYSTEMTIME systime;
	GetLocalTime( &systime );
	stTIME time;
	time.SetTime( systime.wYear, systime.wMonth, systime.wDay, systime.wHour, systime.wMinute, 0 );
	return time.value;
}
QSTATETYPE JackJack::GetEndTimeYearStored()
{
	return m_EndEventTime.value;
}
QSTATETYPE JackJack::GetTimeForOneYearExpired()
{
	return m_EndOneYearExpiryTime.value;
}
QSTATETYPE JackJack::GetTimeOnBuildDay()
{
	return m_BuildTime.value;
}








unsigned long  JackJack::CheckOCTREE()
{
	/*unsigned long dwCrc = 0, dwTemp = 0;
	HANDLE              hSrch;
	WIN32_FIND_DATA     wfd;
	BOOL                bResult = TRUE;

	char strScriptPath[MAX_PATH];
	sprintf(strScriptPath, "%s\\OCTREE\\*.OCT", m_strClientPath);

	hSrch = FindFirstFile(strScriptPath, &wfd);

	int iCount = 0;

	while (bResult)
	{
		if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			;
		}
		else
		{
			sprintf(strScriptPath, "%s\\OCTREE\\%s", m_strClientPath, wfd.cFileName);

			dwTemp = 0;
			if (CCrc32Static::FileCrc32Assembly(strScriptPath, dwTemp) != NO_ERROR)
			{
				;
			}
			dwCrc = dwCrc ^ dwTemp;
			iCount++;
		}

		bResult = FindNextFile(hSrch, &wfd);
	}
	FindClose(hSrch);

	return dwCrc;*/

	unsigned long dwCrc = 0, dwTemp = 0;
	HANDLE              hSrch;
	WIN32_FIND_DATA     wfd;
	BOOL                bResult = TRUE;

	char strScriptPath[MAX_PATH];
	sprintf(strScriptPath, "%s\\OCTREE\\*.bin", ".");

	hSrch = FindFirstFile(strScriptPath, &wfd);

	int iCount = 0;

	while (bResult)
	{
		if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			;
		}
		else
		{
			sprintf(strScriptPath, "%s\\OCTREE\\%s", ".", wfd.cFileName);

			dwTemp = 0;
			if (CCrc32Static::FileCrc32Assembly(strScriptPath, dwTemp) != NO_ERROR)
			{
				;
			}
			dwCrc = dwCrc ^ dwTemp;
			iCount++;
		}

		bResult = FindNextFile(hSrch, &wfd);
	}
	FindClose(hSrch);

	return dwCrc;
}
char * JackJack::CheckCRC(char * str, char * str2)
{
	char strScriptPath[MAX_PATH];
	unsigned long dwCrc[2], dwVer, dwTemp;
	unsigned long m_dwVersion;

	char finalstring[MAX_PATH];

	char strCheckSum[MAX_PATH];
	char szErrMsg[MAX_PATH];

	ZeroMemory(dwCrc, sizeof(unsigned long)* 2);

	strcpy(m_strClientPath, ".");
	sprintf(strScriptPath, "%s\\%s", m_strClientPath, str);
	if (CCrc32Static::FileCrc32Assembly(strScriptPath, dwCrc[0]) != NO_ERROR)
	{
		sprintf(strCheckSum, "%s error1", str);
		goto filesave;
	}

	dwTemp = CheckOCTREE();
	dwCrc[0] = dwCrc[0] ^ dwTemp;

	sprintf(strScriptPath, "%s\\%s", m_strClientPath, str2);
	if (CCrc32Static::FileCrc32Assembly(strScriptPath, dwCrc[1]) != NO_ERROR)
	{
		sprintf(strCheckSum, "%s error2", str2);
		goto filesave;
	}

	m_dwVersion = dwCrc[0] ^ dwCrc[1];

	sprintf(strCheckSum, "0x%08x", m_dwVersion);

filesave:
	sprintf(finalstring, "%s", strCheckSum);
	return finalstring;
}

char * JackJack::CheckCRCOne(char * str)
{
	char strScriptPath[MAX_PATH];
	unsigned long dwCrc[1], dwTemp;
	unsigned long m_dwVersion;

	char strCheckSum[MAX_PATH];

	ZeroMemory(dwCrc, sizeof(unsigned long)* 1);

	strcpy(m_strClientPath, ".");
	sprintf(strScriptPath, "%s\\%s", m_strClientPath, str);
	if (CCrc32Static::FileCrc32Assembly(strScriptPath, dwCrc[0]) != NO_ERROR)
	{
		sprintf(strCheckSum, "error1");
	}

	dwTemp = CheckOCTREE();
	dwCrc[0] = dwCrc[0] ^ dwTemp;

	m_dwVersion = dwCrc[0];

	m_dwCrc1 = m_dwVersion;

	sprintf(strCheckSum, "0x%08x", m_dwCrc1);

	return strCheckSum;
}

char * JackJack::CheckCRCTwo(char * str)
{
	char strScriptPath[MAX_PATH];
	unsigned long dwCrc[1], dwTemp;
	unsigned long m_dwVersion;

	char strCheckSum[MAX_PATH];

	ZeroMemory(dwCrc, sizeof(unsigned long)* 1);

	strcpy(m_strClientPath, ".");
	sprintf(strScriptPath, "%s\\%s", m_strClientPath, str);
	if (CCrc32Static::FileCrc32Assembly(strScriptPath, dwCrc[0]) != NO_ERROR)
	{
		sprintf(strCheckSum, "error1");
	}

	dwTemp = CheckOCTREE();
	dwCrc[0] = dwCrc[0] ;

	m_dwVersion = dwCrc[0];

	m_dwCrc2 = m_dwVersion;

	sprintf(strCheckSum, "0x%08x", m_dwCrc2);

	return strCheckSum;
}

//----------------------------------------------------------------------
LONG JackJack::CheckCRCOneLONG(char * str)
{
	char strScriptPath[MAX_PATH];
	unsigned long dwCrc[1], dwTemp;
	unsigned long m_dwVersion;

	char strCheckSum[MAX_PATH];

	ZeroMemory(dwCrc, sizeof(unsigned long) * 1);

	if (CCrc32Static::FileCrc32Assembly(str, dwCrc[0]) != NO_ERROR)
	{
		sprintf(strCheckSum, "error1");
	}

	dwTemp = CheckOCTREE();
	dwCrc[0] = dwCrc[0] ^ dwTemp;

	m_dwVersion = dwCrc[0];

	m_dwCrc1 = m_dwVersion;

	return m_dwCrc1;
	
}

LONG JackJack::CheckCRCTwoLONG(char * str)
{
	char strScriptPath[MAX_PATH];
	unsigned long dwCrc[1], dwTemp;
	unsigned long m_dwVersion;

	char strCheckSum[MAX_PATH];

	ZeroMemory(dwCrc, sizeof(unsigned long) * 1);

	if (CCrc32Static::FileCrc32Assembly(str, dwCrc[0]) != NO_ERROR)
	{
		sprintf(strCheckSum, "error1");
	}

	dwTemp = CheckOCTREE();
	dwCrc[0] = dwCrc[0] /*^ dwTemp*/;

	m_dwVersion = dwCrc[0];

	m_dwCrc2 = m_dwVersion;

	return m_dwCrc2;
}
//--------------------------------------------------------------------------------
char * JackJack::CalFinalCRC()
{
	char strCheckSum[MAX_PATH];
	unsigned long dwCrc[1], dwTemp;
	unsigned long m_dwVersion;

	m_dwVersion = m_dwCrc1 ^ m_dwCrc2;

	sprintf(strCheckSum, "0x%08x", m_dwVersion);

	return strCheckSum;
}
LONG JackJack::GetCRC1(char * str)
{
	char strScriptPath[MAX_PATH];
	unsigned long dwCrc[1], dwTemp;
	unsigned long m_dwVersion;

	char strCheckSum[MAX_PATH];

	ZeroMemory(dwCrc, sizeof(unsigned long)* 1);

	strcpy(m_strClientPath, ".");
	sprintf(strScriptPath, "%s\\%s", m_strClientPath, str);
	if (CCrc32Static::FileCrc32Assembly(strScriptPath, dwCrc[0]) != NO_ERROR)
	{
		sprintf(strCheckSum, "error1");
	}

	dwTemp = CheckOCTREE();
	dwCrc[0] = dwCrc[0] ^ dwTemp;

	m_dwVersion = dwCrc[0];

	m_dwCrc1 = m_dwVersion;

	return m_dwVersion;
}

LONG JackJack::GetCRC1Self(char * str)
{
	char strScriptPath[MAX_PATH];
	unsigned long dwCrc[1], dwTemp;
	unsigned long m_dwVersion;

	char strCheckSum[MAX_PATH];

	ZeroMemory(dwCrc, sizeof(unsigned long)* 1);

	if (CCrc32Static::FileCrc32Assembly(str, dwCrc[0]) != NO_ERROR)
	{
		sprintf(strCheckSum, "error1");
	}

	dwTemp = CheckOCTREE();
	dwCrc[0] = dwCrc[0] ^ dwTemp;

	m_dwVersion = dwCrc[0];

	m_dwCrcSelf1 = m_dwVersion;

	return m_dwVersion;
}
#ifdef _CLIENTJ_
BOOL JackJack::TimeCheck(DWORD dwType)
{
	stTIME Time;
	SYSTEMTIME sysTime;
	GetLocalTime(&sysTime);
	Time.SetTime(sysTime.wYear, sysTime.wMonth, sysTime.wDay, sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
	return Time.value < dwType;
}
DWORD JackJack::TimeValue()
{
	stTIME Time;
	SYSTEMTIME sysTime;
	GetLocalTime(&sysTime);
	Time.SetTime(sysTime.wYear, sysTime.wMonth, sysTime.wDay, sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
	return Time.value ;
}
DWORD JackJack::TimeValue(DWORD dwType)
{
	stTIME Time;
	SYSTEMTIME sysTime;
	GetLocalTime(&sysTime);
	Time.SetTime(sysTime.wYear, sysTime.wMonth, sysTime.wDay, sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
	return Time.value + dwType;
}
void	JackJack::PrintTimeInfoInChatDlg()
{
	char tmpbuf[128], timebuf[26], ampm[] = "AM";
	time_t ltime;
	struct _timeb tstruct;
	struct tm today, gmt, xmas = { 0, 0, 12, 25, 11, 93 };
	errno_t err;

	// Set time zone from TZ environment variable. If TZ is not set,
	// the operating system is queried to obtain the default value
	// for the variable.
	//
	_tzset();

	// Display operating system-style date and time.
	_strtime_s(tmpbuf, 128);
	CHATMGR->AddMsg((DWORD)0xFFFF66, "\t\t\t\tOS time:\t\t\t\t%s\n", tmpbuf);
	_strdate_s(tmpbuf, 128);
	CHATMGR->AddMsg((DWORD)0xFFFF66, "\t\t\t\tOS date:\t\t\t\t%s\n", tmpbuf);

	// Get UNIX-style time and display as number and string.
	time(&ltime);
	CHATMGR->AddMsg((DWORD)0xFFFF66, "\t\t\t\tTime in seconds since UTC 1/1/70:\t%lld\n", (long long)ltime);
	err = ctime_s(timebuf, 26, &ltime);
	if (err)
	{
		CHATMGR->AddMsg((DWORD)0xFFFF66, "\t\t\t\t ctime_s failed due to an invalid argument.");
		exit(1);
	}
	CHATMGR->AddMsg((DWORD)0xFFFF66, "\t\t\t\tUNIX time and date:\t\t\t%s", timebuf);

	// Display UTC.
	err = _gmtime64_s(&gmt, &ltime);
	if (err)
	{
		CHATMGR->AddMsg((DWORD)0xFFFF66, "\t\t\t\t _gmtime64_s failed due to an invalid argument.");
	}
	err = asctime_s(timebuf, 26, &gmt);
	if (err)
	{
		CHATMGR->AddMsg((DWORD)0xFFFF66, "\t\t\t\t asctime_s failed due to an invalid argument.");
		exit(1);
	}
	CHATMGR->AddMsg((DWORD)0xFFFF66, "\t\t\t\tCoordinated universal time:\t\t%s", timebuf);

	// Convert to time structure and adjust for PM if necessary.
	err = _localtime64_s(&today, &ltime);
	if (err)
	{
		CHATMGR->AddMsg((DWORD)0xFFFF66, "\t\t\t\t _localtime64_s failed due to an invalid argument.");
		exit(1);
	}
	if (today.tm_hour >= 12)
	{
		strcpy_s(ampm, sizeof(ampm), "PM");
		today.tm_hour -= 12;
	}
	if (today.tm_hour == 0)  // Adjust if midnight hour.
		today.tm_hour = 12;

	// Convert today into an ASCII string
	err = asctime_s(timebuf, 26, &today);
	if (err)
	{
		CHATMGR->AddMsg((DWORD)0xFFFF66, "\t\t\t\t asctime_s failed due to an invalid argument.");
		exit(1);
	}

	// Note how pointer addition is used to skip the first 11
	// characters and printf is used to trim off terminating
	// characters.
	//
	CHATMGR->AddMsg((DWORD)0xFFFF66, "\t\t\t\t12-hour time:\t\t\t\t%.8s %s\n",
		timebuf + 11, ampm);

	// Print additional time information.
	_ftime(&tstruct); // C4996
	// Note: _ftime is deprecated; consider using _ftime_s instead
	CHATMGR->AddMsg((DWORD)0xFFFF66, "\t\t\t\tPlus milliseconds:\t\t\t%u\n", tstruct.millitm);
	CHATMGR->AddMsg((DWORD)0xFFFF66, "\t\t\t\tZone difference in hours from UTC:\t%u\n",
		tstruct.timezone / 60);
	CHATMGR->AddMsg((DWORD)0xFFFF66, "\t\t\t\tTime zone name:\t\t\t\t%s\n", _tzname[0]); //C4996
	// Note: _tzname is deprecated; consider using _get_tzname
	CHATMGR->AddMsg((DWORD)0xFFFF66, "\t\t\t\tDaylight savings:\t\t\t%s\n",
		tstruct.dstflag ? "YES" : "NO");

	// Make time for noon on Christmas, 1993.
	if (mktime(&xmas) != (time_t)-1)
	{
		err = asctime_s(timebuf, 26, &xmas);
		if (err)
		{
			printf("asctime_s failed due to an invalid argument.");
			exit(1);
		}
		CHATMGR->AddMsg((DWORD)0xFFFF66, "\t\t\t\tChristmas\t\t\t\t%s\n", timebuf);
	}

	// Use time structure to build a customized time string.
	err = _localtime64_s(&today, &ltime);
	if (err)
	{
		CHATMGR->AddMsg((DWORD)0xFFFF66, "\t\t\t\t _localtime64_s failed due to invalid arguments.");
		exit(1);
	}

	// Use strftime to build a customized time string.
	strftime(tmpbuf, 128,	"\t\t\t\tToday is %A, day %d of %B in the year %Y.\n", &today);
		
	CHATMGR->AddMsg((DWORD)0xFFFF66, tmpbuf);
}

unsigned long JackJack::CheckFinalCRC(char * str, unsigned long ccrc)
{
	char strScriptPath[MAX_PATH];
	unsigned long dwCrc[2], dwVer, dwTemp;
	unsigned long m_dwVersion;

	char finalstring[MAX_PATH];

	char strCheckSum[MAX_PATH];
	char szErrMsg[MAX_PATH];

	ZeroMemory(dwCrc, sizeof(unsigned long)* 2);

	strcpy(m_strClientPath, ".");
	sprintf(strScriptPath, "%s\\%s", m_strClientPath, str);
	if (CCrc32Static::FileCrc32Assembly(strScriptPath, dwCrc[0]) != NO_ERROR)
	{
		;
	}

	dwTemp = CheckOCTREE();
	dwCrc[0] = dwCrc[0] ^ dwTemp;

	dwCrc[1] = ccrc;

	m_dwVersion = dwCrc[0] ^ dwCrc[1];

	if (m_dwVersion != NO_ERROR)
	{
		MSG_HACKUSER msg;
		memset(&msg, 0, sizeof(msg));
		msg.Category = MP_HACKCHECK;
		msg.Protocol = MP_CLIENT_HACKUSER_MSG_SYN;
		sprintf(msg.ProcessName, CHATMGR->GetChatMsg(2682));
		sprintf(msg.UserName, REDISCONN->GetUserName());
		sprintf(msg.PrcessPath, strScriptPath);
		msg.UserID = REDISCONN->GetCharIdx();
		NETWORK->Send(&msg, sizeof(msg));
	}
	return m_dwVersion;
}
#endif
char * JackJack::CheckResourceFileForCRC(char * str, unsigned long ccrc)
{
	char strScriptPath[MAX_PATH];
	unsigned long dwCrc[2], dwVer, dwTemp;
	unsigned long m_dwVersion;

	char finalstring[MAX_PATH];

	char strCheckSum[MAX_PATH];
	char szErrMsg[MAX_PATH];

	ZeroMemory(dwCrc, sizeof(unsigned long)* 2);

	strcpy(m_strClientPath, ".");
	sprintf(strScriptPath, "%s\\%s", m_strClientPath, str);
	if (CCrc32Static::FileCrc32Assembly(strScriptPath, dwCrc[0]) != NO_ERROR)
	{
		sprintf(strCheckSum, "Read only/Opened by other prog.");
		goto filesave;
	}

	dwTemp = CheckOCTREE();
	dwCrc[0] = dwCrc[0] ^ dwTemp;

	dwCrc[1] = ccrc;

	m_dwVersion = dwCrc[0] ^ dwCrc[1];

	if (m_dwVersion==NO_ERROR)
		sprintf(strCheckSum, "Synced %08x", m_dwVersion);
	else
		sprintf(strCheckSum, "Error! %08x kasi client crash", m_dwVersion);
filesave:
	return strCheckSum;
}



BOOL JackJack::CheckResourceFile()
{
	HANDLE hSrch;
	WIN32_FIND_DATA wfd;
	SYSTEMTIME time;
	char Dir[MAX_PATH];
	char filename[MAX_PATH];
	char finalfilename[MAX_PATH];
	char finalfilename2[MAX_PATH];
	char finalfilename3[MAX_PATH];
	BOOL bResult = TRUE;
	CMHFile file;
	char temp[256];
	char temp2[256];
	WORD year, month, day, hour, min;
	GetCurrentDirectory(MAX_PATH, Dir);
	strcat(Dir, "\\mfl.txt");
	GetCurrentDirectory(MAX_PATH, filename);
	strcat(filename, "\\Resource\\*.bin");

	hSrch = FindFirstFile(filename, &wfd);

	strcpy(finalfilename, "\\Resource\\");
	strcat(finalfilename, wfd.cFileName);

	strcpy(finalfilename2, "\\Resource\\test\\");
	strcat(finalfilename2, wfd.cFileName);

	strcpy(finalfilename3, JACKMGR->CheckCRC(finalfilename, finalfilename2));

	while (bResult)
	{
		FileTimeToSystemTime(&wfd.ftLastWriteTime, &time);
		if (!file.Init(Dir, "rt", MHFILE_FLAG_DONOTDISPLAY_NOTFOUNDERROR))
		{
			file.Release();
			goto filesave;
		}
		while (!file.IsEOF())
		{
			file.GetString(temp);
			if (strcmp(wfd.cFileName, temp) == 0)
			{
				year = file.GetWord();
				month = file.GetWord();
				day = file.GetWord();
				hour = file.GetWord();
				min = file.GetWord();

				file.GetString(temp2);
			
				if (year != time.wYear || month != time.wMonth || day != time.wDay ||
					hour != time.wHour || min != time.wMinute)
				{
					file.Release();
					goto filesave;
				}
				else if (strcmp(temp2, finalfilename3) != 0)
				{
					file.Release();
					goto filesave;
				}
				else
					break;
			}
		}
		file.Release();
		bResult = FindNextFile(hSrch, &wfd);
	}
	return TRUE;
filesave:
	SaveFileList();
	return TRUE;
}
BOOL JackJack::SaveFileList()
{
	DWORD count = 1;
	HANDLE hSrch;
	WIN32_FIND_DATA wfd;
	SYSTEMTIME time;
	char Dir[MAX_PATH];
	char filename[MAX_PATH];
	char finalfilename[MAX_PATH];
	char finalfilename2[MAX_PATH];
	BOOL bResult = TRUE;
	GetCurrentDirectory(MAX_PATH, Dir);
	strcat(Dir, "\\mfl.txt");
	FILE* fp = fopen(Dir, "w");
	if (fp == NULL)
	{
		fclose(fp);
		return FALSE;
	}
	GetCurrentDirectory(MAX_PATH, filename);
	strcat(filename, "\\Resource\\*.bin");
	
	hSrch = FindFirstFile(filename, &wfd);

	strcpy(finalfilename, "\\Resource\\");
	strcat(finalfilename, wfd.cFileName);

	strcpy(finalfilename2, "\\Resource\\test\\");
	strcat(finalfilename2, wfd.cFileName);

	while (bResult)
	{		
		FileTimeToSystemTime(&wfd.ftLastWriteTime, &time);
		fprintf(fp, "%d %s\t", count, wfd.cFileName);
		fprintf(fp, "%4d %02d %02d %02d %02d\t", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute);
		
		fprintf(fp, "%s\n", JACKMGR->CheckCRC(finalfilename, finalfilename2));
		
		bResult = FindNextFile(hSrch, &wfd);
		++count;
	}
	fclose(fp);
	return TRUE;
}