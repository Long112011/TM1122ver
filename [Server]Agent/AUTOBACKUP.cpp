#include "StdAfx.h"
#include "AUTOBACKUP.h"
#include "mmSystem.h"
#include "ServerSystem.h"
#include "Network.h"
GLOBALTON(AUTOBACKUP)
AUTOBACKUP::AUTOBACKUP()
{
	m_CurTime=timeGetTime();

	//CreateDirectory("C:\AUTOBACKUP", NULL);
}
AUTOBACKUP::~AUTOBACKUP()
{
	m_CurTime=0;
}
void AUTOBACKUP::Process()
{
	DWORD Currtime=timeGetTime();

	if(Currtime-m_CurTime>15000)
	{
		//BackupNow();

		m_CurTime=timeGetTime();
	}
}
void AUTOBACKUP::BackupNow()
{
	SYSTEMTIME sysTime;
	GetLocalTime(&sysTime);
	g_Console.LOG(4, "backup : [%04d-%02d-%02d %02d:%02d:%02d]",
		sysTime.wYear, sysTime.wMonth, sysTime.wDay, sysTime.wHour, sysTime.wMinute, sysTime.wSecond);

	char txt[256];
	sprintf(txt, "EXEC JACK_AUTOBACKUP");
	g_DB.LoginQuery(eQueryType_FreeQuery, 0, 0, txt);
}
