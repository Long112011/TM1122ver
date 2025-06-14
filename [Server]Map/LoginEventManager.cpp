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
//---------------------------------------------------below is v1
	/*CMHFile file;
	if(!file.Init("./Resource/Server/LoginEvent.bin", "rb"))
		return;

	LOGINEVENTINFO * pInfo = NULL;
	while(1)
	{		
		if(file.IsEOF())
			break;

		if(m_LoginEventListInfo.GetDataNum() >= 15)//max item per tab
		{
			g_Console.LOG( 4, "LoginEvent.bin lines over than 15!" );
			return;
		}
		ASSERT(!pInfo);
		pInfo				= new LOGINEVENTINFO;
		pInfo->Key			= file.GetInt();
		if( pInfo->Key == 0 )
		{
			file.Release();
			return;
		}		
		pInfo->dwItemIdx	= file.GetDword();
		pInfo->dwTiming		= file.GetInt();
		pInfo->dwQuantity	= file.GetInt();		
		pInfo->dwNextLine	= file.GetDword();
		memset(pInfo->cTextSend, 0, sizeof(char)*(MAX_GUILD_NAME+1));
		SafeStrCpy(pInfo->cTextSend,file.GetStringInQuotation(),MAX_GUILD_NAME+1);
		ASSERT(m_LoginEventListInfo.GetData(pInfo->Key) == FALSE);
		if( pInfo->dwQuantity == 0 )
		{
			pInfo->dwQuantity = 1;
		}
		if( pInfo->dwTiming == 0 || pInfo->dwTiming < 0 )
		{
			g_Console.LOG( 4, "dwTiming 0" );
			pInfo->dwTiming = 1;
		}
		m_LoginEventListInfo.Add(pInfo,pInfo->Key);
		pInfo=NULL;
	}
	file.Release();*/
//------------------------------------------------------------------below is v2

	/*CMHFile file;
	
	if(!file.Init("Resource/Server/LoginEvent.bin", "rb"))
		return;



	if( strcmp( file.GetString(), "TIME_START") == 0 )
	{
		DWORD year = file.GetDword();
		DWORD month = file.GetDword();
		DWORD day = file.GetDword();
		m_FirstStartTime.SetTime( year, month, day, 0, 0, 0 );

		g_Console.LOG( 4, "year:%d month:%d day:%d" ,year, month, day);
	}

	char TabName[17] = {0,};
	char StrBuf[256] = {0,};

	while(1)
	{
		if(file.IsEOF())
			break;

		file.GetString(StrBuf);

		if( StrBuf[0] != '#' )
		{
			file.GetLine(StrBuf,256);
			continue;
		}
		else if( strcmp( StrBuf , "#TAB" ) == 0 )
		{			
			file.GetWord();
			SafeStrCpy( TabName, file.GetString(), 17 );
			
			
			char StrBuf[256] = {0,};
			LOGINEVENTINFO * pInfo = NULL;
			while(1)
			{
				if(file.IsEOF())
					break;

				file.GetString(StrBuf);
				if( StrBuf[0] != '#' )
				{
					file.GetLine(StrBuf,256);
					continue;
				}
				else if( strcmp( StrBuf , "#CASHITEM" ) == 0 )
				{
					if(m_LoginEventListInfo.GetDataNum() >= 15)//max item per tab
					{
						return;
					}

					ASSERT(!pInfo);
					LOGINEVENTINFO * pInfo = new LOGINEVENTINFO;
					if(pInfo)
					{
						pInfo->Key				= file.GetInt();
						pInfo->dwItemIdx		= file.GetDword();
						pInfo->dwTiming			= file.GetInt();
						pInfo->dwQuantity		= file.GetInt();		
						pInfo->dwNextLine		= file.GetDword();
						memset(pInfo->cTextSend, 0, sizeof(char)*(MAX_GUILD_NAME+1));
						SafeStrCpy(pInfo->cTextSend,file.GetStringInQuotation(),MAX_GUILD_NAME+1);
						ASSERT(m_LoginEventListInfo.GetData(pInfo->Key) == FALSE);
						m_LoginEventListInfo.Add(pInfo,pInfo->Key);
					}
				}
				else if( strcmp( StrBuf , "#TAB_END" ) == 0 )
				{
					break;
				}
				pInfo=NULL;
			}
		}
	}
	file.Release();*/
//---------------------------------------------------------below is v3

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


	//g_Console.LOG( 4, "%d %d %d" ,m_EndEventTime.GetYear()+16, m_EndEventTime.GetMonth(), m_EndEventTime.GetDay());

	//g_Console.LOG( 4, "[st.wDayOfWeek:%d] [st.wHour:%d] [st.wMinute:%d]" ,st.wDayOfWeek, st.wHour, st.wMinute);

	//g_Console.LOG( 4, "[st.wYear:%d] [st.wMonth:%d] [st.wDay:%d]" ,st.wYear, st.wMonth, st.wDay);





	//stTIME ct;
	//ct.value = GetCurTime();

	//g_Console.LOG( 4, "[GetCurTime:%d] [ct.value:%d]",GetCurTime(),ct.value);



	//g_Console.LOG( 4, "");
	//g_Console.LOG( 4, "[m_FirstStartTime:%d] [m_EndEventTime.value:%d]",m_FirstStartTime.value,m_EndEventTime.value);

	//g_Console.LOG( 4, "[GetCurTime:%d] [this->GetCurTime():%d]",GetCurTime(),this->GetCurTimeYear());


	//g_Console.LOG( 4, "");

	/*LOGINEVENTINFO * pInfo = NULL;
	while(1)
	{		
		if(file.IsEOF())
			break;

		
		ASSERT(!pInfo);
		pInfo				= new LOGINEVENTINFO;
		pInfo->Key			= file.GetDword();
		
		pInfo->dwItemIdx	= file.GetDword();
		pInfo->dwTiming		= file.GetDword();
		pInfo->dwQuantity	= file.GetDword();		
		pInfo->dwNextLine	= file.GetDword();

		memset(pInfo->cTextSend, 0, sizeof(char)*(MAX_GUILD_NAME+2));
		SafeStrCpy(pInfo->cTextSend,file.GetStringInQuotation(),MAX_GUILD_NAME+2);

		ASSERT(m_LoginEventListInfo.GetData(pInfo->Key) == FALSE);
		
		m_LoginEventListInfo.Add(pInfo,pInfo->Key);
		pInfo=NULL;
	}
	file.Release();*/

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
	if(!pInfo)
	{
		pInfo = new LOGINEVENTINFO;
		pInfo->Key				= idx;
		pInfo->dwItemIdx		= 0;
		pInfo->dwTiming			= GetLoginEventItemInfo(1)->dwTiming;
		pInfo->dwQuantity		= 1;		
		pInfo->dwNextLine		= 1;
		memset(pInfo->cTextSend, 0, sizeof(char)*(MAX_GUILD_NAME+2));
		SafeStrCpy(pInfo->cTextSend,"GG",MAX_GUILD_NAME+2);
		m_LoginEventListInfo.Add(pInfo,pInfo->Key);
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