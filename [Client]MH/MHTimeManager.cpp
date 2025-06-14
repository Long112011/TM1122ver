








// MHTimeManager.cpp: implementation of the CMHTimeManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MHTimeManager.h"

#include "mmsystem.h"
#include <time.h>

DWORD gCurTime = 0;		//0 초기화 추가...
DWORD gTickTime;
float gAntiGravity = 1.0f;

//#define TICK_PER_DAY 86400000		// 24 * 60 * 60 * 1000
using namespace std;													// Include so we don't need to complicate our C++ 11 code
using namespace chrono;													// Include so we don't need to complicate our C++ 11 code

extern HWND _g_hWnd;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
GLOBALTON(CMHTimeManager)
CMHTimeManager::CMHTimeManager()
: m_TotalFrames(0)
, m_Count_OneSec(0)
{
	m_MHDate = 0;
	m_MHTime = 0;

	m_lasttime = 0;

	m_ServerTimeTick = 0;
	_time64( &m_ServerTime );
}

CMHTimeManager::~CMHTimeManager()
{


}
LONG CMHTimeManager::GetCountOneSec()
{
	return m_Count_OneSec;
}
void CMHTimeManager::Init(DWORD mhDate,DWORD mhTime)
{
	m_MHDate = mhDate;
	m_MHTime = mhTime;
}

void CMHTimeManager::Process()
{
	static bool bFirst = true;
	static DWORD curtime = 0;
	static int tempDay = 0;

	if( bFirst )
	{

		curtime = m_lasttime = GetTickCount();
		bFirst = false;
	}
	else
	{


		curtime = GetTickCount();		

		if( curtime < m_lasttime )		// DWORD 형의 한계를 넘어갔다

			gTickTime = curtime - m_lasttime + 4294967295; //( 2^32 - 1 )
		else

			gTickTime = curtime - m_lasttime;
		m_lasttime = curtime;
		
		gCurTime += gTickTime;

		
		//////////////////////////////////////////////////////////////////////////

		// 묵향력-_-a; 
		// 묵향력 사용하지 않아 현실과 같은 시계로 변경

		m_MHTime += gTickTime;
		if(m_MHTime >= TICK_PER_DAY)
		{
			++m_MHDate;

			m_MHTime -= TICK_PER_DAY;
		}
	}

	m_ServerTimeTick += (int)gTickTime;
	int sec = int( m_ServerTimeTick / 1000 );
	m_ServerTimeTick = m_ServerTimeTick % 1000;
	m_ServerTime += sec;
}

DWORD CMHTimeManager::GetNewCalcCurTime()	//cur타임 새로 받아오기 기존 gCurTime, gTickTime에 영향없다.
{
	DWORD lcurtime = GetTickCount();
	DWORD lTickTime;
		

	if( lcurtime < m_lasttime )		// DWORD 형의 한계를 넘어갔다
		lTickTime = lcurtime - m_lasttime + 4294967295; //( 2^32 - 1 )
	else

		lTickTime = lcurtime - m_lasttime;

	
	return gCurTime + lTickTime;
}

DWORD CMHTimeManager::GetMHDate()
{
	return m_MHDate;
}
DWORD CMHTimeManager::GetMHTime()
{
	return m_MHTime;
}



void CMHTimeManager::GetMHDate(BYTE& year,BYTE& month,BYTE& day)
{
	year = (BYTE)(m_MHDate / 360) + 1;
	month = (BYTE)((m_MHDate - year) / 30) + 1;
	day = (BYTE)m_MHDate % 30 + 1;  // -_-a; 묵향에선 모든 달은 30일까지다 -_-	
}

void CMHTimeManager::GetMHTime(BYTE& hour,BYTE& minute)
{

	hour = (BYTE)m_MHTime / 3600000;
	minute = (BYTE)(m_MHTime - hour) / 60000;
}




//---------------------------------------------DELAY------------
bool CMHTimeManager::CheckTimer12000()
{
	static DWORD nTick = gCurTime;
	if (gCurTime-nTick < 12000) return FALSE;
	nTick = gCurTime;
	return TRUE;
}

bool CMHTimeManager::CheckTimer5000()
{
	static DWORD nTick = gCurTime;
	if (gCurTime-nTick < 5000) return FALSE;
	nTick = gCurTime;
	return TRUE;
}


bool CMHTimeManager::CheckTimer2000()
{
	static DWORD nTick = gCurTime;
	if (gCurTime-nTick < 2000) return FALSE;
	nTick = gCurTime;
	return TRUE;
}

bool CMHTimeManager::CheckTimer1000()
{
	static DWORD nTick = gCurTime;
	if (gCurTime-nTick < 1000) return FALSE;
	nTick = gCurTime;
	return TRUE;
}


bool CMHTimeManager::CheckTimer400()
{
	static DWORD nTick = gCurTime;
	if (gCurTime-nTick < 400) return FALSE;
	nTick = gCurTime;
	return TRUE;
}

bool CMHTimeManager::CheckTimer100()
{
	static DWORD nTick = gCurTime;
	if (gCurTime - nTick < 100) return FALSE;
	nTick = gCurTime;
	return TRUE;
}
//--------------------------------------------------------------
void CMHTimeManager::SetServerTime( __time64_t serverTime )
{
	m_ServerTimeTick = 0;
	m_ServerTime = serverTime;

//	QUESTMGR->InitDateTime();
}





//----------------------------------------------------------------
// NEW FPS
//----------------------------------------------------------------
#include "VideoCaptureIcon.h"
// This calculates our current scene's frames per second and displays it in the console
double CMHTimeManager::CalculateFrameRate()
{
	static double framesPerSecond = 0.0f;								// This will store our fps
	static double startTime = GetTime();								// This will hold the time per second to test against
	static char strFrameRate[50] = { 0 };								// We will store the string here for the window title
	static double currentFPS = 0.0f;									// This stores the current frames per second

	CurrentTime = GetTime();
	++framesPerSecond;

	if (CurrentTime - startTime > 1.0f)
	{
		startTime = CurrentTime;
		currentFPS = framesPerSecond;
		framesPerSecond = 0;

		//sprintf(strFrameRate, "%1.0f", currentFPS);
		//SetWindowTextA(_g_hWnd, strFrameRate);
	}
	return currentFPS;
}
// This calculates our current scene's frames per second and displays it in the console
double CMHTimeManager::CalculateFrameRate2()
{
	static double framesPerSecond = 0.0f;								// This will store our fps
	static double startTime = GetTime();								// This will hold the time per second to test against
	static char strFrameRate[50] = { 0 };								// We will store the string here for the window title
	static double currentFPS = 0.0f;									// This stores the current frames per second

	CurrentTime = GetTime();
	++framesPerSecond;

	if (CurrentTime - startTime > 1.0f)
	{
		startTime = CurrentTime;

		currentFPS = framesPerSecond;

		framesPerSecond = 0;

		sprintf(strFrameRate, "%1.0f", currentFPS);
		SetWindowTextA(_g_hWnd, strFrameRate);
	}
	return currentFPS;
}


// This returns the current time in seconds (uses C++ 11 system_clock)
double CMHTimeManager::GetTime()
{
	// Grab the current system time since 1/1/1970, otherwise know as the Unix Timestamp or Epoch
	auto beginningOfTime = system_clock::now().time_since_epoch();

	// Convert the system time to milliseconds
	auto ms = duration_cast<milliseconds>(beginningOfTime).count();

	// Return the time in seconds and give us a fractional value (important!)
	return ms * 0.001;
}


// This pauses the current thread for an amount of time in milliseconds
void CMHTimeManager::Sleep(int ms)
{
	// Use the C++ 11 sleep_for() function to pause the current thread
	this_thread::sleep_for(milliseconds(ms));
}