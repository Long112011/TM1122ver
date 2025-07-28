#include "stdafx.h"
#include "LoginEventManager.h"
#include "MapDBMsgParser.h"
#include "Player.h"
#include "MHFile.h"
CLoginEventManager::CLoginEventManager()
{
	m_LoginEventListInfo.Initialize(15);

	m_Master=false;
	m_MasterExt=false;
}
CLoginEventManager::~CLoginEventManager()
{
	LOGINEVENTINFO * pInfo = NULL;
	m_LoginEventListInfo.SetPositionHead();
	while(pInfo=m_LoginEventListInfo.GetData())
	{
		delete pInfo;
	}
	m_LoginEventListInfo.RemoveAll();
}

void CLoginEventManager::LoadLoginEventItemList()
{

	CMHFile file;
	
	if(!file.Init("Resource/Server/LoginEvent.bin", "rb"))
		return;


	SYSTEMTIME st;
	GetLocalTime( &st );


	if( strcmp( file.GetString(), "#EVENT_START") == 0 )
	{
		DWORD year = file.GetDword();
		DWORD month = file.GetDword();
		DWORD day = file.GetDword();

		DWORD hour = file.GetDword();
		DWORD minute = file.GetDword();
		//DWORD second = file.GetDword();

		m_FirstStartTime.SetTime( year, month, day, hour, minute, 0 );

	//	g_Console.LOG( 4, "year:%d month:%d day:%d ,hour:%d min:%d" ,year, month, day,hour,minute);
	}
	if( strcmp( file.GetString(), "#EVENT_END") == 0 )
	{
		DWORD year = file.GetDword();
		DWORD month = file.GetDword();
		DWORD day = file.GetDword();

		DWORD hour = file.GetDword();
		DWORD minute = file.GetDword();
		//DWORD second = file.GetDword();

		m_EndEventTime.SetTime( year, month, day, hour, minute, 0 );



	//	g_Console.LOG( 4, "year:%d month:%d day:%d ,hour:%d min:%d" ,year, month, day,hour,minute);
	}






	LOGINEVENTINFO * pInfo = NULL;
	while( 1 )
	{
		char buf[32] = { 0, };
		strcpy( buf, file.GetString() );

		if( strcmp( buf, "#EVENT" ) == 0 )
		{
			ASSERT(!pInfo);
			LOGINEVENTINFO * pInfo = new LOGINEVENTINFO;
			if(pInfo)
			{
				pInfo->Key				= file.GetInt();
				pInfo->dwItemIdx		= file.GetDword();
				pInfo->dwTiming			= file.GetInt();
				pInfo->dwQuantity		= file.GetInt();		
				pInfo->dwNextLine		= file.GetDword();
				memset(pInfo->cTextSend, 0, sizeof(char)*(MAX_GUILD_NAME+2));
				SafeStrCpy(pInfo->cTextSend,file.GetStringInQuotation(),MAX_GUILD_NAME+2);
				ASSERT(m_LoginEventListInfo.GetData(pInfo->Key) == FALSE);
				m_LoginEventListInfo.Add(pInfo,pInfo->Key);
			}
		}
		else if( strcmp( buf, "#END" ) == 0 )
			break;
	}

	//CheckEvent();
	AddLastLine(99);
	file.Release();
}
void CLoginEventManager::AddLastLine(DWORD idx)
{
	LOGINEVENTINFO* pInfo = m_LoginEventListInfo.GetData(idx);
	if (!pInfo)
	{
		LOGINEVENTINFO* pBase = GetLoginEventItemInfo(1);
		if (!pBase || pBase->dwItemIdx == 0) return; // 防止插入无效物品

		pInfo = new LOGINEVENTINFO;
		pInfo->Key = idx;
		pInfo->dwItemIdx = pBase->dwItemIdx;
		pInfo->dwTiming = pBase->dwTiming;
		pInfo->dwQuantity = 1;
		pInfo->dwNextLine = 1;
		memset(pInfo->cTextSend, 0, sizeof(char) * (MAX_GUILD_NAME + 2));
		SafeStrCpy(pInfo->cTextSend, "GG", MAX_GUILD_NAME + 2);
		m_LoginEventListInfo.Add(pInfo, pInfo->Key);
	}
}

QSTATETYPE CLoginEventManager::GetCurTimeYear()
{
	SYSTEMTIME systime;
	GetLocalTime( &systime );
	stTIME time;
	time.SetTime( systime.wYear, systime.wMonth, systime.wDay, systime.wHour, systime.wMinute, 0 );

	return time.value;
}


void CLoginEventManager::Process(CPlayer* pPlayer)
{
	
	if(CheckEvent())
		pPlayer->LoginEventProcess();
}
char * CLoginEventManager::GetStartTimeInString()
{
	char temp2[256];
	sprintf(temp2,"%d%02d%02d",
		m_FirstStartTime.GetYear(),
		m_FirstStartTime.GetMonth(),
		m_FirstStartTime.GetDay());

	return temp2;
}
char * CLoginEventManager::GetEndTimeInString()
{
	char temp2[256];
	sprintf(temp2,"%d%02d%02d",
		m_EndEventTime.GetYear(),
		m_EndEventTime.GetMonth(),
		m_EndEventTime.GetDay());
	return temp2;
}
char * CLoginEventManager::GetTotalTimeInString()
{
	char temp1[256];
	sprintf(temp1,"%d.%02d.%02d",
		m_FirstStartTime.GetYear()+16,
		m_FirstStartTime.GetMonth(),
		m_FirstStartTime.GetDay());
		

	char temp2[256];
	sprintf(temp2,"%d.%02d.%02d",
		m_EndEventTime.GetYear()+16,
		m_EndEventTime.GetMonth(),
		m_EndEventTime.GetDay());

	char temp3[256];
	sprintf(temp3,"%s~%s",temp1,temp2);

	return temp3;
}
bool CLoginEventManager::CheckEvent()
{
	//g_Console.LOG( 4, "GetStartTimeInString() %s",GetStartTimeInString());
	//g_Console.LOG( 4, "GetEndTimeInString() %s",GetEndTimeInString());
	//g_Console.LOG( 4, "GetTotalTimeInString() %s",GetTotalTimeInString());
	if( GetCurTimeYear() < m_FirstStartTime.value )
	{
		return false;
	}
	if( GetCurTimeYear() >= m_EndEventTime.value )
	{
		return false;
	}

	return true;
}
void CLoginEventManager::LoginEventSend(CPlayer* pPlayer,DWORD dwKind,DWORD dwTiming,WORD dwItemIdx,char* TextMsg,int dwQty,int dwTotalTime)
{
	//if(m_Master /*&& m_MasterExt*/)
	//{
	//	m_MasterExt=false;
	//}
	//char dwTempMsg[256];
	//strcpy(dwTempMsg,GetTotalTimeInString());
	if(dwTotalTime==0 || dwQty==0 || dwItemIdx==0 || dwTiming==0) 
	{
	//	g_Console.LOG( 4, "oiii");
		return;
	}

	
	MSG_SIGNEVENT	msg;
	msg.Category	=MP_CHAR;
	msg.Protocol	=MP_CHAR_LOGIN_EVENT_ACK;
	msg.dwObjectID	=pPlayer->GetID();
	msg.wParse		=dwKind;	
	msg.Time		=dwTiming/1000;
	msg.ItemIdx		=dwItemIdx;
	msg.CyptKey		=dwQty;

	msg.dwTotalTime	=dwTotalTime/1000;

	strcpy(TextMsg,GetTotalTimeInString());
	SafeStrCpy(msg.cTextSend,TextMsg,MAX_GUILD_NAME+2);

	pPlayer->SendMsg( &msg,	sizeof(MSG_SIGNEVENT));

	memset( &msg,		0, sizeof(MSG_SIGNEVENT) );
	memset( &dwKind,	0, sizeof(MSG_SIGNEVENT) );
	memset( &dwTiming,	0, sizeof(MSG_SIGNEVENT) );
	memset( &dwItemIdx,	0, sizeof(MSG_SIGNEVENT) );
	memset( &TextMsg,	0, sizeof(MSG_SIGNEVENT) );
	memset( &dwQty,		0, sizeof(MSG_SIGNEVENT) );
	memset( &dwTotalTime,0, sizeof(MSG_SIGNEVENT) );

	CharacterLoginEventUpdate(pPlayer);
}