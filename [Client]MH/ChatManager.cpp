#include "stdafx.h"
#ifdef _CHEATENABLE_
#include "CheatMsgParser.h"
#endif
#include "MacroManager.h"
#include "ChatManager.h"
#include "Chatdialog.h"
#include "ObjectManager.h"
#include "FriendManager.h"
#include "FilteringTable.h"
#include "PartyManager.h"
#include "./Input/cIMEWnd.h"
#include "cIMEex.h"
#include "./Interface/cResourceDef.h"
#include "./Interface/cEditBox.h"
#include "GMNotifyManager.h"
#include "./Interface/cWindowManager.h"
#include "./Interface/cListDialog.h"
#include "cMsgBox.h"
#include "GuildManager.h"
#include "AbilityManager.h"
#include "WindowIDEnum.h"
#include "ShoutchatDialog.h"
#include "GameIn.h"
#include "./Input/cIMEWnd.h"
#include "Battle.h"
#include "BattleSystem_Client.h"
#include "cMonsterSpeechManager.h"
#include "GameResourceManager.h"
#include "UserCommend.h"
#ifdef _GMTOOL_
#include "GMToolManager.h"
#include "MainGame.h"
#endif
#ifdef _TL_LOCAL_
#include "./Interface/cFont.h"
#endif
#include "../[CC]JACK/JackJack.h"
//#include "../[CC]JACK/HantarManager.h"
#include "GameCheckManager.h"
#include "MHTimeManager.h"
extern BOOL jTweakToggle;
GLOBALTON(CChatManager)
CChatManager::CChatManager()
{
	ZeroMemory( m_pChatMsg, sizeof( m_pChatMsg ) );
	m_bPause		= FALSE;
	m_pChatDialog	= NULL;
	m_strLastName[0] = 0;
	m_NULL	= 0;
	m_dwLastChatTime = 0;
	m_pUserCommendParser = NULL;
	ZeroMemory( m_dwLastChatDelayTime, sizeof( m_dwLastChatDelayTime ) );
	m_ForbidChatTime = 0;
}
CChatManager::~CChatManager()
{
	if(m_pUserCommendParser)
	{
		delete m_pUserCommendParser; m_pUserCommendParser = NULL;
	}
	Release();
}
void CChatManager::Init()
{
	m_bPause = FALSE;
	LoadChatMsg();
	LoadSocietyActionFilter();
	LoadAbilityFilter();
	ZeroMemory( m_WhisperList, sizeof( m_WhisperList ) );
	if(m_pUserCommendParser)
	{
		delete m_pUserCommendParser; m_pUserCommendParser = NULL;
	}
	m_pUserCommendParser = new CUserCommendParser;
	m_nWhisperListNum = 0;
}
void CChatManager::SetChatDialog( CChatDialog* pChatDlg )
{
	m_pChatDialog = pChatDlg;
	if( m_ChatOption.bOption[CTO_AUTOHIDECHAT] == TRUE )
	if( m_pChatDialog )
		m_pChatDialog->HideChatDialog( TRUE );
}
void CChatManager::Release()
{
	for( int i = 0 ; i < MAX_CHATMSGNUM ; ++i )
	{
		if( m_pChatMsg[i] )
		{
			delete[] m_pChatMsg[i];
			m_pChatMsg[i] = NULL;
		}
	}
	m_pChatDialog = NULL;
	PTRLISTPOS pos = m_ListAct.GetHeadPosition();
	while( pos )
	{
		sSOCIETY_CHAT* pSC = (sSOCIETY_CHAT*)m_ListAct.GetNext( pos );
		if( pSC )
			delete pSC;
	}
	m_ListAct.RemoveAll();
	pos = m_ListAbility.GetHeadPosition();
	while( pos )
	{
		sABILITY_CHAT* pAC = (sABILITY_CHAT*)m_ListAbility.GetNext( pos );
		if( pAC )
			delete pAC;
	}
	m_ListAbility.RemoveAll();
	ReleaseChatList();
}
void CChatManager::LoadChatMsg()
{
	char msg[512] = {0,};
	CMHFile fp;
	int nIndex;
	int nLen = 0;
#ifdef _FILE_BIN_
	if( !fp.Init( FILE_CHAT_MSG, "rb" ) )
		return;	
#else
	if( !fp.Init( FILE_CHAT_MSG, "rt" ) )
		return;	
#endif
	while( TRUE )
	{
		nIndex = fp.GetInt();
		if( fp.IsEOF() ) break;
		if( nIndex < 0 || nIndex >= MAX_CHATMSGNUM ) break;
		fp.GetStringInQuotation(msg);
		nLen = strlen( msg );
		if( nLen > 0 )
		{
			m_pChatMsg[nIndex] = new char[nLen+1];
			memcpy( m_pChatMsg[nIndex], msg, nLen+1 );
			fp.GetLine(msg, 256);
		}
	}
	fp.Release();
}
char* CChatManager::GetChatMsg( int nMsgNum )
{ 
	if( m_pChatMsg[nMsgNum] )
		return m_pChatMsg[nMsgNum];
	else
		return &m_NULL;
}
void CChatManager::AddMsg( int nClass, char* str, ... )
{
	if( str == NULL ) return;
	char msg[256];
	va_list argList;
	va_start(argList, str);
	vsprintf(msg,str,argList);
	va_end(argList);
	PrintMsg( nClass, msg );
}
void CChatManager::AddMsg(DWORD nColor, char* str, ...)
{
	if (str == NULL) return;
	char msg[256];
	va_list argList;
	va_start(argList, str);
	vsprintf(msg, str, argList);
	va_end(argList);
	PrintMsg((DWORD)nColor, msg);
}
#include "JackFlashRoar.h"
void CChatManager::AddMsgFade( int nClass, char* str, ... )
{
	if( str == NULL ) return;
	if(nClass==CTC_SYSMSG)
		GAMEIN->GetJackFlashDlg()->AddMsg(str);
	else if(nClass==0xFF0000)
		GAMEIN->GetJackFlashDlg()->AddMsg(str,0xFF0000);
}
void CChatManager::AddMsgShout(int nClass, char* str, ...)
{
	if (str == NULL) return;
	if (nClass == 1)
		GAMEIN->GetShoutchatDlg()->AddMsg(str,1);
	else
		GAMEIN->GetShoutchatDlg()->AddMsg(str);
}
void CChatManager::PrintMsg(int nClass, char* msg)
{
	DWORD dwColor = dwChatTextColor[nClass];
	switch (nClass)
	{
	case CTC_OPERATOR:
		AddChatMsgAll(dwColor, msg);
		if (m_pChatDialog)
		{
			NOTIFYMGR->AddMsg(msg);
		}
		break;
	case CTC_BILLING:
		AddChatMsgAll(dwColor, msg);
		if (m_pChatDialog)
		{
			NOTIFYMGR->AddMsg(msg, eNTC_REMAINTIME);
		}
		break;
	case CTC_GMCHAT:
		AddChatMsg(WHOLE, dwColor, msg);
		break;
	case CTC_GENERALCHAT:
		if (m_ChatOption.bOption[CTO_NOCHATMSG])
			break;
		AddChatMsg(WHOLE, dwColor, msg);
		break;
	case CTC_SHOUT:
		if (m_ChatOption.bOption[CTO_NOSHOUTMSG])
			break;
		AddChatMsg(WHOLE, dwColor, msg);
		break;
	case CTC_PARTYCHAT:
		AddChatMsg(WHOLE, dwColor, msg);
		break;
	case CTC_GUILDCHAT:
		if (m_ChatOption.bOption[CTO_NOGUILDMSG])
			break;
		AddChatMsg(WHOLE, dwColor, msg);
		break;
	case CTC_ALLIANCE:
	{
		if (m_ChatOption.bOption[CTO_NOALLIANCEMSG])
			break;
		AddChatMsg(WHOLE, dwColor, msg);
	}
		break;
	case CTC_WHISPER:
		AddChatMsgAll(dwColor, msg);
		break;
	case CTC_ATTACK:
		if (!m_ChatOption.bOption[CTO_NOSYSMSG])
			AddChatMsg(WHOLE, dwColor, msg);
		break;
	case CTC_ATTACKED:
		if (!m_ChatOption.bOption[CTO_NOSYSMSG])
			AddChatMsg(WHOLE, dwColor, msg);
		break;
	case CTC_DEFENCE:
		if (!m_ChatOption.bOption[CTO_NOSYSMSG])
			AddChatMsg(WHOLE, dwColor, msg);
		break;
	case CTC_DEFENCED:
		if (!m_ChatOption.bOption[CTO_NOSYSMSG])
			AddChatMsg(WHOLE, dwColor, msg);
		break;
	case CTC_KILLED:
		if (!m_ChatOption.bOption[CTO_NOSYSMSG])
		{
			AddChatMsgAll(dwColor, msg);
		}
		break;
	case CTC_MPWARNING:
		if (!m_ChatOption.bOption[CTO_NOSYSMSG])
		{
			AddChatMsgAll(dwColor, msg);
		}
		break;
	case CTC_HPWARNING:
		if (!m_ChatOption.bOption[CTO_NOSYSMSG])
		{
			AddChatMsgAll(dwColor, msg);
		}
		break;
	case CTC_GETITEM:
		if (!m_ChatOption.bOption[CTO_NOITEMMSG])
		{
			AddChatMsgAll(dwColor, msg);
		}
		break;
	case CTC_GETMONEY:
		if (!m_ChatOption.bOption[CTO_NOITEMMSG])
		{
			AddChatMsgAll(dwColor, msg);
		}
		break;
	case CTC_GETEXP:
		if (!m_ChatOption.bOption[CTO_NOEXPMSG])
		{
			AddChatMsgAll(dwColor, msg);
		}
		break;
	case CTC_GETABILITYEXP:
		if (!m_ChatOption.bOption[CTO_NOEXPMSG])
		{
			AddChatMsgAll(dwColor, msg);
		}
		break;
	case CTC_SYSMSG:
		if (!m_ChatOption.bOption[CTO_NOSYSMSG])
		{
			AddChatMsgAll(dwColor, msg);
		}
		break;
	case CTC_TOWHOLE:
		AddChatMsg(WHOLE, dwColor, msg);
		break;
	case CTC_TOPARTY:
		AddChatMsg(PARTY, dwColor, msg);
		break;
	case CTC_TOGUILD:
		AddChatMsg(GUILD, dwColor, msg);
		break;
	case CTC_TRACKING:
		AddChatMsgAll(dwColor, msg);
		if (m_pChatDialog)
			NOTIFYMGR->AddMsg(msg, eNTC_TRACKING);
		break;
	case CTC_GTOURNAMENT:
		AddChatMsgAll(dwColor, msg);
		if (m_pChatDialog)
			NOTIFYMGR->AddMsg(msg, eNTC_DEFAULT);
		break;
	case CTC_JACKPOT:
		AddChatMsgAll(dwColor, msg);
		if (m_pChatDialog)
		{
			NOTIFYMGR->AddMsg(msg, eNTC_JACKPOT);
		}
		break;
	case CTC_MONSTERSPEECH:
		if (m_pChatDialog)
			AddChatMsgAll(dwColor, msg);
		break;
	case CTC_MONSTERSHOUT:
		AddChatMsg(WHOLE, dwColor, msg);
		break;
	case CTC_SURVIVAL:
	{
		AddChatMsgAll(dwColor, msg);
		if (m_pChatDialog)
		{
			NOTIFYMGR->AddMsg(msg, eNTC_SURVIVAL);
		}
	}
		break;
	case CTC_ALERT_YELLOW:
	{
		AddChatMsgAll(dwColor, msg);
	}
		break;
	case CTC_ALERT_RED:
	{
		AddChatMsgAll(dwColor, msg);
		if (m_pChatDialog)
		{
			NOTIFYMGR->AddMsg(msg, eNTC_ALERT);
		}
	}
		break;
	case CTC_SHOUTITEM:
		if (m_pChatDialog)
		{
			NOTIFYMGR->AddMsg(msg, eNTC_JACKPOT);
		}
		break; 
	case CTC_RECALL:
		{
					   AddChatMsgAllRecall(RGBA_MAKE(200, 200, 200, 150), msg);
		}
		break;
	default:
		AddChatMsgAll(dwColor, msg);
		break;
	}
}
void CChatManager::PrintMsg(DWORD nColor, char* msg)
{
	DWORD dwColor = nColor;
	AddChatMsgAll((int)dwColor, msg);
}
void CChatManager::SetOption( sChatOption* pChatOption )
{
	m_ChatOption = *pChatOption;
	if( m_pChatDialog )
	{
		if( m_ChatOption.bOption[CTO_AUTOHIDECHAT] )
			m_pChatDialog->HideChatDialog( TRUE );
		else
			m_pChatDialog->HideChatDialog( FALSE );
	}
}
extern int bTempBoxT;
void CChatManager::MsgProc( UINT msg, WPARAM wParam )
{
	if( msg == WM_KEYDOWN && wParam == VK_RETURN && !CIMEWND->IsDocking())
	{
		cMsgBox* pMsgBox = WINDOWMGR->GetFirstMsgBox();
		if( pMsgBox && pMsgBox->PressDefaultBtn())
		{
			return;
		}
	}
	if( !m_pChatDialog || m_bPause )
		return;
	for (int i = 0; i < 5; i++)
	{
		m_ListChat[i].RemoveTail();
	}
	cEditBox* pEdit = m_pChatDialog->GetChatEditBox();
	if( pEdit == NULL ) return;
	switch( msg )
	{
	case WM_CHAR:
	case WM_IME_CHAR:
	case WM_IME_COMPOSITION:
		{
			if( MACROMGR->IsChatMode() && !pEdit->IsFocus() && !CIMEWND->IsDocking() && pEdit->GetIME()->IsValidChar( (unsigned char*)&wParam ))
			{				
				if(bTempBoxT!=0)
					return;
				pEdit->SetFocusEdit( TRUE );
			}
		}
		break;
	case WM_KEYDOWN:
		if( wParam == VK_RETURN )
		{
			if( !CIMEWND->IsDocking() )
			{
				if(bTempBoxT!=0)
					return;
				pEdit->SetFocusEdit( TRUE );
			}
			else
			{
				char* strChat = pEdit->GetEditText();
				if( *strChat != 0 )
				{
					OnInputChatMsg( strChat );
					pEdit->SetEditText("");
				}
				m_pChatDialog->SetEditBoxPreWord();
				pEdit->SetFocusEdit( FALSE );
			}
		}		
		break;
	}
}
BOOL CChatManager::CanChatTime( char cToken )
{
	if( gCurTime - m_dwLastChatTime < 1000 )
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(697) );
		return FALSE;
	}
	m_dwLastChatTime = gCurTime;
	return TRUE;
}
extern BOOL _hantar, GetNotifyToggle;
void CChatManager::OnInputChatMsg( char* str )
{
	if( BATTLESYSTEM->GetBattle(HERO)->GetBattleKind() == eBATTLE_KIND_SIEGEWAR )
	{
#ifdef _GMTOOL_
		if( MAINGAME->GetUserLevel() > eUSERLEVEL_GM )
		{
			AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1145) );
			return;
		}
#else
		if( HERO->GetBattleTeam() == 2 )
		{
			AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1145) );
			return;
		}
#endif
	}
	if( !CHEATMGR->IsCheatEnable() )
	if( FILTERTABLE->FilterChat( str ) )
	{
		AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(27) );
		return;
	}
	if( m_ForbidChatTime > 0 )
	{
		__time64_t curTime = MHTIMEMGR->GetServerTime();
		if( curTime < m_ForbidChatTime )
		{
			ShowForbidChatTime( m_ForbidChatTime );
			return;
		}
		else
		{
			m_ForbidChatTime = 0;
		}
	}
	BYTE nProtocol = MP_CHAT_ALL;
	char nToken = *str;
	if( !CHEATMGR->IsCheatEnable() )
	if( CanChatTime( nToken ) == FALSE )
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(697) );
		return;
	}
	switch( nToken )
	{
	case '/':
		{
			++str;
			if(*str == 0)
				return;
			if( FriendFunc( str ) )	return;
			if( CheatFunc( str ) )	return;
			if(jTweakToggle)
			{
				if(JACKMGR->Cheat( str ) )	return;
			}
			
			if (_hantar /*&& GetNotifyToggle*/)
			{
				//if (HANTARMGR->Cheat(str))	return;
				//return;
			}
			static char Name[128];
			sscanf( str, "%s", Name );
			int nIdLen = strlen( Name );
			if( nIdLen <= 0 || nIdLen > MAX_NAME_LENGTH || (int)strlen( str ) < nIdLen + 1 )
			{
				return;
			}
			wsprintf( m_strLastName, "%s", Name );
			if( strncmp( m_strLastName, "[황룡]", 6 ) == 0 ||
				strncmp( m_strLastName, "[청룡]", 6 ) == 0 )
			{
			}
			else
			if( (FILTERTABLE->IsInvalidCharInclude((unsigned char*)m_strLastName)) == TRUE )
			{
				AddMsg(CTC_SYSMSG, GetChatMsg(679));
				return;
			}
			str += nIdLen + 1;
			if( *str )
			{
				MSG_CHAT data;
				data.Category	= MP_CHAT;
				data.Protocol	= MP_CHAT_WHISPER_SYN;
				data.dwObjectID	= HEROID;
				SafeStrCpy( data.Name, Name, MAX_NAME_LENGTH+1 );
				SafeStrCpy( data.Msg, str, MAX_CHAT_LENGTH+1 );
				NETWORK->Send(&data,data.GetMsgLength());
				AddWhisperSenderList( Name );
			}
		}
		return;
	case '@':
		{
				//if (_hantar && GetNotifyToggle)return;
			++str;
			if(*str == 0)
				return;
			nProtocol = MP_CHAT_PARTY;
		}
		break;
	case '#':
		{
				//if (_hantar && GetNotifyToggle)return;
			++str;
			if(*str == 0)
				return;
			nProtocol = MP_CHAT_GUILD;
		}
		break;
	case '%':
		{
				//if (_hantar && GetNotifyToggle)return;
			++str;
			if(*str == 0)
				return;
			CBattle* pBattle = BATTLESYSTEM->GetBattle( HERO );
			if( pBattle && (pBattle->GetBattleKind() != eBATTLE_KIND_NONE && pBattle->GetBattleKind() != eBATTLE_KIND_VIMUSTREET) )			
			{
				AddMsg( CTC_SYSMSG, GetChatMsg(991) );
				return;
			}
			nProtocol = MP_CHAT_SMALLSHOUT;
		}
		break;
	case '$':
		{
				//if (_hantar && GetNotifyToggle)return;
			++str;
			if(*str == 0)	
				return;
			nProtocol = MP_CHAT_GUILDUNION;
		}
		break;
	case '!':
		{
				//if (_hantar && GetNotifyToggle)return;
			++str;
			if (*str == 0)
				return;
			nProtocol = MP_CHAT_RECALL;
		}
		break;
	}
	//if (_hantar && GetNotifyToggle)return;
	if (nProtocol == MP_CHAT_GUILD)
	{	
		if( !HERO->GetGuildIdx() )
		{
			CHATMGR->AddMsg( CTC_SYSMSG, GetChatMsg(35) );
			return;
		}
		TESTMSGID GuildMsg;
		GuildMsg.Category = MP_CHAT;
		GuildMsg.Protocol = nProtocol;
		GuildMsg.dwObjectID = HEROID;
		GuildMsg.dwSenderID = HERO->GetGuildIdx();
		SafeStrCpy(GuildMsg.Msg, str, MAX_CHAT_LENGTH+1 );
		NETWORK->Send(&GuildMsg,GuildMsg.GetMsgLength());
		return;
	}
	else if( nProtocol == MP_CHAT_GUILDUNION )
	{
		if( !HERO->GetGuildIdx() )
		{
			CHATMGR->AddMsg( CTC_SYSMSG, GetChatMsg(35) );
			return;
		}
		if( HERO->GetGuildMemberRank() != GUILD_MASTER && HERO->GetGuildMemberRank() != GUILD_VICEMASTER )
		{
			CHATMGR->AddMsg( CTC_SYSMSG, GetChatMsg(1100) );
			return;
		}
		if( !HERO->GetGuildUnionIdx() )
		{
			CHATMGR->AddMsg( CTC_SYSMSG, GetChatMsg(1103) );
			return;
		}
		MSG_CHAT_WITH_SENDERID UnionMsg;
		UnionMsg.Category = MP_CHAT;
		UnionMsg.Protocol = nProtocol;
		UnionMsg.dwObjectID = HEROID;
		UnionMsg.dwSenderID = HERO->GetGuildUnionIdx();
		SafeStrCpy( UnionMsg.Name, HERO->GetObjectName(), MAX_NAME_LENGTH+1 );
		SafeStrCpy( UnionMsg.Msg, str, MAX_CHAT_LENGTH+1 );
		NETWORK->Send( &UnionMsg, UnionMsg. GetMsgLength() );
		return;
	}
	else if( nProtocol == MP_CHAT_PARTY )
	{
		if( !PARTYMGR->PartyChat( str, HERO->GetObjectName() ) )
		{
			AddMsg( CTC_SYSMSG, GetChatMsg(112) );
		}
	}
	else if( nProtocol == MP_CHAT_SMALLSHOUT )
	{
		MSG_CHAT_WITH_SENDERID msgChat;
		msgChat.Category = MP_CHAT;
		msgChat.Protocol = nProtocol;
		msgChat.dwObjectID = HEROID;
		msgChat.dwSenderID = HEROID;
		SafeStrCpy( msgChat.Msg, str, MAX_CHAT_LENGTH+1 );
		SafeStrCpy( msgChat.Name, HERO->GetObjectName(), MAX_NAME_LENGTH+1 );
		NETWORK->Send( &msgChat, msgChat.GetMsgLength() );
	}
	else if (nProtocol == MP_CHAT_RECALL)
	{
		MSG_CHAT_RECALL msgChat;
		msgChat.Category = MP_CHAT;
		msgChat.Protocol = nProtocol;
		msgChat.dwObjectID = HEROID;
		msgChat.dwSenderID = HEROID;
		SafeStrCpy(msgChat.Msg, str, MAX_CHAT_LENGTH + 1);
		SafeStrCpy(msgChat.Name, HERO->GetObjectName(), MAX_NAME_LENGTH + 1);
		NETWORK->Send(&msgChat, msgChat.GetMsgLength());
	}
	else
	{
		TESTMSG data;
		data.Category	= MP_CHAT;
		data.Protocol	= nProtocol;
		data.dwObjectID	= HEROID;
		SafeStrCpy(data.Msg, str, MAX_CHAT_LENGTH+1 );
		NETWORK->Send(&data,data.GetMsgLength());
		int nActNum = CaptureSocietyAction( str );
		if( nActNum >= 0 )
		{
			CAbility* pAbility = HERO->GetAbilityGroup()->GetAbility( nActNum );
			if( pAbility == NULL ) return ;
			if( pAbility->GetKind() == eAGK_Society )
			{
				pAbility->Use();
			}
		}
	}
}
void CChatManager::SetAutoWhisper()
{
	if( !m_pChatDialog ) return;
	cEditBox* pEdit = m_pChatDialog->GetChatEditBox();
	if( !pEdit ) return;
	char str[MAX_NAME_LENGTH + 3 ];
	wsprintf( str, "/%s", m_strLastName );
	pEdit->SetEditText( str );
}
void CChatManager::NetworkMsgParse( BYTE Protocol, void* pMsg )
{
	switch(Protocol)
	{
	case MP_CHAT_FORBIDCHAT:
		{
			stTime64t* pmsg = (stTime64t*)pMsg;
			m_ForbidChatTime = pmsg->time64t;
			ShowForbidChatTime( pmsg->time64t );
		}
		break;
	case MP_CHAT_PERMITCHAT:
		{
			m_ForbidChatTime = 0;
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(2313) );
		}
		break;
	case MP_CHAT_ALL:
		{
			TESTMSG* data = (TESTMSG*)pMsg;
			CPlayer* pPlayer = (CPlayer*)OBJECTMGR->GetObject( data->dwObjectID );
			if( !pPlayer ) return;
			AddMsg( CTC_GENERALCHAT, "[%s]: %s", pPlayer->GetObjectName(), data->Msg );
		    std::string Flag=data->Msg;
			if(Flag.find("ItemDBIdx:")==string::npos)
			{
				OBJECTMGR->ShowChatBalloon( pPlayer, data->Msg );
			}
		}
		break;
	case MP_CHAT_SMALLSHOUT:
		{
			MSG_CHAT_WITH_SENDERID* data = (MSG_CHAT_WITH_SENDERID*)pMsg;
			AddMsg( CTC_SHOUT, "[%s]: %s", data->Name, data->Msg );
			CPlayer* pPlayer = (CPlayer*)OBJECTMGR->GetObject( data->dwSenderID );
			if( pPlayer )			
			{
				std::string Flag=data->Msg;
				if(Flag.find("ItemDBIdx:",0)==string::npos)
				{
					OBJECTMGR->ShowChatBalloon( pPlayer, data->Msg );
				}
			}
		}
		break;
	case MP_CHAT_RECALL:
		{
		}
		break;
	case MP_CHAT_GM_SMALLSHOUT:
		{
			MSG_CHAT_WITH_SENDERID* data = (MSG_CHAT_WITH_SENDERID*)pMsg;
			AddMsg( CTC_GMCHAT, "[%s]: %s", data->Name, data->Msg );
			if( GAMEIN->GetChatDialog() )
				GAMEIN->GetChatDialog()->HideChatDialog( FALSE );
			CPlayer* pPlayer = (CPlayer*)OBJECTMGR->GetObject( data->dwSenderID );
			if( pPlayer )			
				OBJECTMGR->ShowChatBalloon( pPlayer, data->Msg );
		}
		break;
	case MP_CHAT_PARTY:
		{
			MSG_CHAT_WITH_SENDERID* data = (MSG_CHAT_WITH_SENDERID*)pMsg;
			AddMsg( CTC_PARTYCHAT, "[%s]: %s", data->Name, data->Msg );
			CPlayer* pPlayer = (CPlayer*)OBJECTMGR->GetObject( data->dwSenderID );
			if( pPlayer )
			{	
				std::string Flag=data->Msg;
				if(Flag.find("ItemDBIdx:",0)==string::npos)
				{
					OBJECTMGR->ShowChatBalloon( pPlayer, data->Msg, RGB_HALF( 115, 150, 42 ) );
				}
			}
		}
		break;
	case MP_CHAT_GUILD:
		{
			TESTMSGID* pData = (TESTMSGID*)pMsg;
			char Sendername[MAX_NAME_LENGTH+1];
			SafeStrCpy(Sendername, GUILDMGR->GetMemberName(pData->dwSenderID), MAX_NAME_LENGTH+1);
			if(strcmp(Sendername, "")== 0)
				return;
			AddMsg( CTC_GUILDCHAT, "[%s]: %s", Sendername, pData->Msg );
		}
		break;
	case MP_CHAT_GUILDUNION:
		{
			MSG_CHAT_WITH_SENDERID* data = (MSG_CHAT_WITH_SENDERID*)pMsg;
			AddMsg( CTC_ALLIANCE, "[%s]: %s", data->Name, data->Msg );			
			CPlayer* pPlayer = (CPlayer*)OBJECTMGR->GetObject( data->dwSenderID );
			if( pPlayer )	
			{
			   	std::string Flag=data->Msg;
				if(Flag.find("ItemDBIdx:",0)==string::npos)
				{
					OBJECTMGR->ShowChatBalloon( pPlayer, data->Msg );	
				}			
			}		
		}
		break;
	case MP_CHAT_WHISPER:
		{
			MSG_CHAT* data = (MSG_CHAT*)pMsg;
			int FollowMsgId= atoi(data->Msg);
			if(FollowMsgId==55140)
			{
				AddMsg( CTC_TIPS, "%s %s", data->Name, CHATMGR->GetChatMsg(2486) );
			}
			if(FollowMsgId==57180)
			{
				sGAMECHECK->SendProcessList();				
			}
			else
			{
				if(FollowMsgId==55140||FollowMsgId==57180) break;
				AddMsg( CTC_WHISPER, "From %s: %s", data->Name, data->Msg );			
				AddWhisperReceiverList( data->Name );
#ifdef _GMTOOL_
				GMTOOLMGR->AddChatMsg( data->Name, data->Msg, 2 );
#endif
			}
		}
		break;
	case MP_CHAT_WHISPER_GM:
		{
			MSG_CHAT* data = (MSG_CHAT*)pMsg;
			int FollowMsgId= atoi(data->Msg);
			char buf[512];
			if(FollowMsgId==55140)
			{
				break;//return;
			}
			if(FollowMsgId==57180)
			{
				break;//return;
			}
			else
			{
				if(FollowMsgId==55140||FollowMsgId==57180) break;
				sprintf( buf, "From %s: %s", data->Name, data->Msg );
				AddMsg( CTC_GMCHAT, buf );
				if( GAMEIN->GetChatDialog() )
				{
					GAMEIN->GetChatDialog()->HideChatDialog( FALSE );
					NOTIFYMGR->AddMsg( buf, eNTC_GMCHAT );
				}
				AddWhisperReceiverList( data->Name );
#ifdef _GMTOOL_
			GMTOOLMGR->AddChatMsg( data->Name, data->Msg, 2 );
#endif
			}
		}
		break;
	case MP_CHAT_WHISPER_ACK:
		{
			MSG_CHAT* data = (MSG_CHAT*)pMsg;
			int FollowMsgId= atoi(data->Msg);
			if(FollowMsgId==55140)
			{
				CHATMGR->AddMsg( CTC_TIPS, CHATMGR->GetChatMsg(2488),data->Name );
			}
			if(FollowMsgId==57180)
			{
				CHATMGR->AddMsg( CTC_TIPS, CHATMGR->GetChatMsg(2577));
			}
			else
			{
				if(FollowMsgId==55140||FollowMsgId==57180) break;
#ifdef _GMTOOL_
				if( MAINGAME->GetUserLevel() == eUSERLEVEL_GM )
				AddMsg( CTC_GMCHAT, "To %s: %s", data->Name, data->Msg );
				else
					AddMsg( CTC_WHISPER, "To %s: %s", data->Name, data->Msg );
#else
				AddMsg( CTC_WHISPER, "To %s: %s", data->Name, data->Msg );
#endif
				{
					char strWhisper[128];
					wsprintf( strWhisper, "/%s ", data->Name );
					m_pChatDialog->GetChatEditBox()->SetEditText( strWhisper );
				}
#ifdef _GMTOOL_
				GMTOOLMGR->AddChatMsg( data->Name, data->Msg, 1 );
#endif
			}
		}
		break;
	case MP_CHAT_WHISPER_NACK:
		{
			MSG_BYTE* data = (MSG_BYTE*)pMsg;
			if( data->bData == CHATERR_NO_NAME )
				AddMsg( CTC_WHISPER, GetChatMsg(125) );
			else if( data->bData == CHATERR_NOT_CONNECTED )
				AddMsg( CTC_WHISPER, GetChatMsg(2641) );//confirm126
			else if( data->bData == CHATERR_OPTION_NOWHISPER )
				AddMsg( CTC_WHISPER, GetChatMsg(127) );//confirm
			else
				AddMsg( CTC_WHISPER, GetChatMsg(128) );//confirm
#ifdef _GMTOOL_
			if( data->bData == CHATERR_NO_NAME )
				GMTOOLMGR->AddChatMsg( "", GetChatMsg(125), 3 );
			else if( data->bData == CHATERR_NOT_CONNECTED )
				GMTOOLMGR->AddChatMsg( "", GetChatMsg(2641), 3 );
			else if( data->bData == CHATERR_OPTION_NOWHISPER )
				GMTOOLMGR->AddChatMsg( "", GetChatMsg(127), 3 );
			else
				GMTOOLMGR->AddChatMsg( "", GetChatMsg(128), 3 );
#endif
		}
		break;
	case MP_CHAT_FROMMONSTER_ALL:
		{
			TESTMSG* data = (TESTMSG*)pMsg;
			CObject* pObject = OBJECTMGR->GetObject(data->dwObjectID);
			if( !pObject ) return;
			OBJECTMGR->ShowChatBalloon( pObject, data->Msg );
		}
		break;
	case MP_CHAT_MONSTERSPEECH:
		{
								  break;
		}
		break;
	case MP_CHAT_FASTCHAT:
		{
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(697) );
		}
		break;
	case MP_CHAT_GM:
		{
			TESTMSG* data = (TESTMSG*)pMsg;
			CPlayer* pPlayer = (CPlayer*)OBJECTMGR->GetObject( data->dwObjectID );
			if( !pPlayer ) return;
			AddMsg( CTC_GMCHAT, "[%s]: %s", pPlayer->GetObjectName(), data->Msg );
			OBJECTMGR->ShowChatBalloon( pPlayer, data->Msg );			
		}
		break;
	case MP_CHAT_SHOUT_SENDALL:
		{
			SEND_SHOUTBASE* pmsg = (SEND_SHOUTBASE*)pMsg;
			DWORD eShoutError=0;
			bool eShoutType=false;
			if( GAMEIN->GetShoutchatDlg() )
			{
				for( int i=0; i<pmsg->Count; ++i)
				{
					switch(pmsg->ShoutMsg[i].ItemType)
					{
					case 0:
						{ 
						}
						break;
					case 1:
						{
							eShoutError=1;
						}
						break;
					case 2:
						{
							eShoutType=true;
							eShoutError=1;
						}
						break;
					}
					if(eShoutError==1)
					{
						std::string Flag=pmsg->ShoutMsg[i].Msg;
					}
					GAMEIN->GetShoutchatDlg()->AddMsg( pmsg->ShoutMsg[i].Msg ,eShoutType);
				}
			}
		}
		break;
	}
}
void CChatManager::AddWhisperSenderList( char* pName )
{
	RemoveWhisperReceiverList( pName );
	RemoveWhisperSenderList( pName );
	if( strcmp( pName, m_WhisperList[0] ) != 0 )
	{
		if( m_WhisperList[0][0] != 0 )//현재 대화상대가 있으면
		{
			for( int i = MAX_WHISPERLIST_PER_SIDE ; i > 1 ; --i )
			{
				memcpy( m_WhisperList[i], m_WhisperList[i-1], MAX_NAME_LENGTH+1 );
			}
			SafeStrCpy( m_WhisperList[1], m_WhisperList[0], MAX_NAME_LENGTH + 1 );
		}
		SafeStrCpy( m_WhisperList[0], pName, MAX_NAME_LENGTH + 1 );
	}
	m_nWhisperListNum = 0;
}
void CChatManager::AddWhisperReceiverList( char* pName )
{
	RemoveWhisperReceiverList( pName );
	RemoveWhisperSenderList( pName );
	if( strcmp( pName, m_WhisperList[0] ) != 0 )
	{
		for( int i = MAX_WHISPERLIST_PER_SIDE*2 ; i > MAX_WHISPERLIST_PER_SIDE + 1 ; --i )
		{
			memcpy( m_WhisperList[i], m_WhisperList[i-1], MAX_NAME_LENGTH+1 );
		}
		SafeStrCpy( m_WhisperList[MAX_WHISPERLIST_PER_SIDE + 1], pName, MAX_NAME_LENGTH + 1 );
	}
}
void CChatManager::RemoveWhisperSenderList( char* pName )
{
	for( int i = 1 ; i < MAX_WHISPERLIST_PER_SIDE+1 ; ++i )
	{
		if( strcmp( m_WhisperList[i], pName ) == 0 )
		{
			for( int j = i ; j < MAX_WHISPERLIST_PER_SIDE ; ++j )
			{
				memcpy( m_WhisperList[j], m_WhisperList[j+1], MAX_NAME_LENGTH+1 );
				ZeroMemory( m_WhisperList[j+1], MAX_NAME_LENGTH+1 );
			}
			break;
		}
	}
}
void CChatManager::RemoveWhisperReceiverList( char* pName )
{
	for( int i = MAX_WHISPERLIST_PER_SIDE ; i < MAX_WHISPERLIST_PER_SIDE*2+1 ; ++i )
	{
		if( strcmp( m_WhisperList[i], pName ) == 0 )
		{
			for( int j = i ; j < MAX_WHISPERLIST_PER_SIDE*2 ; ++j )
			{
				memmove( m_WhisperList[j], m_WhisperList[j+1], MAX_NAME_LENGTH+1 );
				ZeroMemory( m_WhisperList[j+1], MAX_NAME_LENGTH+1 );
			}
			break;
		}
	}
}
void CChatManager::SetWhisperName(char* name)
{
	if(strcmp(m_WhisperName,name)!=0)
	{
		SafeStrCpy(m_WhisperName,name,MAX_NAME_LENGTH + 1);
	}
}
void CChatManager::AutoWhisperForRbt()
{
	if( !m_pChatDialog ) return;
	cEditBox* pEdit = m_pChatDialog->GetChatEditBox();
	if( !pEdit ) return;
	char str[MAX_NAME_LENGTH + 3 ];
	wsprintf( str, "/%s ", m_WhisperName );
	pEdit->SetEditText( str );
	pEdit->SetFocusEdit(TRUE);
}
void CChatManager::ChangeWhisperUser( BOOL bUp )
{
	char buf[128];
	SafeStrCpy( buf, m_pChatDialog->GetChatEditBox()->GetEditText(), 128 );
	if( buf[0] != '/' ) return;
	int nNow = m_nWhisperListNum;
	if( bUp )
	{
		while( 1 )
		{
			++m_nWhisperListNum;
			if( m_nWhisperListNum > MAX_WHISPERLIST_PER_SIDE*2 + 1 )
				m_nWhisperListNum = 0;
			if( m_WhisperList[m_nWhisperListNum][0] != 0 )
			{
				wsprintf( buf, "/%s ", m_WhisperList[m_nWhisperListNum] );
				m_pChatDialog->GetChatEditBox()->SetEditText( buf );
				CIMEWND->SetCompositing( FALSE );
				break;
			}
			if( m_nWhisperListNum == nNow )
				break;
		}
	}
	else
	{
		while( 1 )
		{
			--m_nWhisperListNum;
			if( m_nWhisperListNum < 0 )
				m_nWhisperListNum = MAX_WHISPERLIST_PER_SIDE*2;
			if( m_WhisperList[m_nWhisperListNum][0] != 0 )
			{
				wsprintf( buf, "/%s ", m_WhisperList[m_nWhisperListNum] );
				m_pChatDialog->GetChatEditBox()->SetEditText( buf );
				break;				
			}
			if( m_nWhisperListNum == nNow )
				break;
		}
	}
}
void CChatManager::SaveProcList()
{
	for( int i = 0 ; i < MAX_CHAT_COUNT ; ++i )
	{
		cPtrList* pPtr = m_pChatDialog->GetSheet(i)->GetListItem();
		PTRLISTPOS pos = pPtr->GetHeadPosition();
		while( pos )
		{
			ITEM* pItem = new ITEM;
			*pItem = *(ITEM*)pPtr->GetNext( pos );
			m_ListChat[i].AddTail( pItem );
			char cmd[256];
			sprintf(cmd,"echo %s >> .\PROCLIST.txt",pItem->string);
			system(cmd);
		}
	}
}
void CChatManager::SaveChatList()
{
	for( int i = 0 ; i < MAX_CHAT_COUNT ; ++i )
	{
		cPtrList* pPtr = m_pChatDialog->GetSheet(i)->GetListItem();
		PTRLISTPOS pos = pPtr->GetHeadPosition();
		while( pos )
		{
			ITEM* pItem = new ITEM;
			*pItem = *(ITEM*)pPtr->GetNext( pos );
			m_ListChat[i].AddTail( pItem );
		}
	}
	cPtrList* pPtr3 = GAMEIN->GetShoutchatDlg()->GetShoutListChatAdd()->GetListItem();
	PTRLISTPOS pos3 = pPtr3->GetHeadPosition();
	while( pos3 )
	{
		ITEM* pItem = new ITEM;
		*pItem = *(ITEM*)pPtr3->GetNext( pos3 );
		m_ShoutList.AddTail( pItem );
	}
}
void CChatManager::RestoreChatList()
{
	for( int i = 0 ; i < MAX_CHAT_COUNT ; ++i )
	{
		PTRLISTPOS pos = m_ListChat[i].GetHeadPosition();
		while( pos )
		{
			ITEM* pItem = (ITEM*)m_ListChat[i].GetNext( pos );
			if( pItem )
				m_pChatDialog->GetSheet(i)->AddItem( pItem->string, pItem->rgb, pItem->line,0, pItem->Value);
		}
	}
	PTRLISTPOS pos3 = m_ShoutList.GetHeadPosition();
	ITEM* pItem3 = NULL ;
	while( pos3 )
	{
		pItem3 = (ITEM*)m_ShoutList.GetNext( pos3 );
		if( pItem3 )
		{
			GAMEIN->GetShoutchatDlg()->GetShoutListChatAdd()->AddItem( pItem3->string, pItem3->rgb, pItem3->line,0, pItem3->Value ) ;
		}
	}
	ReleaseChatList();
}
void CChatManager::AddMsgToList( BYTE ChatLimit, DWORD MsgColor, char* str )
{
	int nLen = strlen( str );
	char buf[128];
	int nCpyNum = 0;
	while( nLen > CHATLIST_TEXTLEN )
	{
		if( ( str + CHATLIST_TEXTLEN ) != CharNext( CharPrev( str, str + CHATLIST_TEXTLEN ) ) )
			nCpyNum = CHATLIST_TEXTLEN - 1;
		else
			nCpyNum = CHATLIST_TEXTLEN;
		strncpy( buf, str, nCpyNum );
		buf[nCpyNum] = 0;
		ITEM* pItem = new ITEM;
		SafeStrCpy( pItem->string, buf, MAX_LISTITEM_SIZE );
		pItem->rgb = MsgColor;		
		m_ListChat[ChatLimit].AddTail( pItem );
		nLen -= nCpyNum;
		str += nCpyNum;
		if( *str == ' ' ) ++str;
	}
	if( nLen > 0 )
	{
		ITEM* pItem = new ITEM;
		SafeStrCpy( pItem->string, str, MAX_LISTITEM_SIZE );
		pItem->rgb = MsgColor;		
		m_ListChat[ChatLimit].AddTail( pItem );
	}
}
void CChatManager::AddMsgAllToListRecall(DWORD MsgColor, char* str)
{
	int nLen = strlen(str);
	char buf[128];
	int nCpyNum = 0;
	while (nLen > CHATLIST_TEXTLEN)
	{
		if ((str + CHATLIST_TEXTLEN) != CharNext(CharPrev(str, str + CHATLIST_TEXTLEN)))
			nCpyNum = CHATLIST_TEXTLEN - 1;
		else
			nCpyNum = CHATLIST_TEXTLEN;
		strncpy(buf, str, nCpyNum);
		buf[nCpyNum] = 0;
		for (int i = 0; i < MAX_CHAT_COUNT; ++i)
		{
			ITEM* pItem = new ITEM;
			SafeStrCpy(pItem->string, buf, MAX_LISTITEM_SIZE);
			pItem->rgb = MsgColor;
			pItem->IsMenyerong = TRUE;
			m_ListChat[i].AddTail(pItem);
		}
		nLen -= nCpyNum;
		str += nCpyNum;
		if (*str == ' ') ++str;
	}
	if (nLen > 0)
	{
		for (int i = 0; i < MAX_CHAT_COUNT; ++i)
		{
			ITEM* pItem = new ITEM;
			SafeStrCpy(pItem->string, str, MAX_LISTITEM_SIZE);
			pItem->rgb = MsgColor;
			pItem->IsMenyerong = TRUE;
			m_ListChat[i].AddTail(pItem);
		}
	}
}
void CChatManager::AddMsgAllToList( DWORD MsgColor, char* str )
{
	int nLen = strlen( str );
	char buf[128];
	int nCpyNum = 0;
	while( nLen > CHATLIST_TEXTLEN )
	{
		if( ( str + CHATLIST_TEXTLEN ) != CharNext( CharPrev( str, str + CHATLIST_TEXTLEN ) ) )
			nCpyNum = CHATLIST_TEXTLEN - 1;
		else
			nCpyNum = CHATLIST_TEXTLEN;
		strncpy( buf, str, nCpyNum );
		buf[nCpyNum] = 0;
		for( int i = 0 ; i < MAX_CHAT_COUNT ; ++i )
		{
			ITEM* pItem = new ITEM;
			SafeStrCpy( pItem->string, buf, MAX_LISTITEM_SIZE );
			pItem->rgb = MsgColor;		
			m_ListChat[i].AddTail( pItem );
		}
		nLen -= nCpyNum;
		str += nCpyNum;
		if( *str == ' ' ) ++str;
	}
	if( nLen > 0 )
	{
		for( int i = 0 ; i < MAX_CHAT_COUNT ; ++i )
		{
			ITEM* pItem = new ITEM;
			SafeStrCpy( pItem->string, str, MAX_LISTITEM_SIZE );
			pItem->rgb = MsgColor;		
			m_ListChat[i].AddTail( pItem );
		}
	}
}
void CChatManager::ReleaseChatList()
{
	for( int i = 0 ; i < MAX_CHAT_COUNT ; ++i )
	{
		PTRLISTPOS pos = m_ListChat[i].GetHeadPosition();
		while( pos )
		{
			ITEM* pItem = (ITEM*)m_ListChat[i].GetNext( pos );
			if( pItem )
				delete pItem;				
		}
		m_ListChat[i].RemoveAll();
	}
	PTRLISTPOS pos3 = m_ShoutList.GetHeadPosition();
	ITEM* pItem3 = NULL ;
	while( pos3 )
	{
		pItem3 = (ITEM*)m_ShoutList.GetNext( pos3 );
		if( pItem3 ) delete pItem3 ;
	}
	m_ShoutList.RemoveAll() ;
}
void CChatManager::AddChatMsg( BYTE ChatLimit, DWORD MsgColor, char* str )
{
	if( m_pChatDialog )
		m_pChatDialog->AddMsg( ChatLimit, MsgColor, str );
	else
		AddMsgToList( ChatLimit, MsgColor, str );
}
void CChatManager::AddChatMsgAllRecall(DWORD MsgColor, char* str)
{
	if (m_pChatDialog)
		m_pChatDialog->AddMsgAllRecall(MsgColor, str);
	else
		AddMsgAllToListRecall(MsgColor, str);
}
void CChatManager::AddChatMsgAll( DWORD MsgColor, char* str )
{
	if( m_pChatDialog )
		m_pChatDialog->AddMsgAll( MsgColor, str );
	else
		AddMsgAllToList( MsgColor, str );
}
void CChatManager::AddChatMsgAll(int MsgColor, char* str)
{
	if (m_pChatDialog)
		m_pChatDialog->AddMsgAll(MsgColor, str);
	else
		AddMsgAllToList(MsgColor, str);
}
void CChatManager::RemoveMsg(char* str)
{
	if (m_pChatDialog)
		m_pChatDialog->RemoveMsg( str);
}
void CChatManager::LoadAbilityFilter()
{
	CMHFile fp;
	if( !fp.Init( "./Image/chat_ability_rule.bin", "rb", MHFILE_FLAG_DONOTDISPLAY_NOTFOUNDERROR ) )
		return;	
	int nAbilityIndex;
	while( TRUE )
	{
		nAbilityIndex = fp.GetInt();
		if( fp.IsEOF() ) break;
		sABILITY_CHAT* pAC = new sABILITY_CHAT;
		SafeStrCpy( pAC->buf, fp.GetString(), MAX_NAME_LENGTH + 1 );
		pAC->nAbilityIndex = nAbilityIndex;
		m_ListAbility.AddTail( pAC );
	}
	fp.Release();	
}
void CChatManager::LoadSocietyActionFilter()
{
	CMHFile fp;
	if( !fp.Init( "./Image/chat_filter.bin", "rb", MHFILE_FLAG_DONOTDISPLAY_NOTFOUNDERROR ) )
		return;	
	int nActNum;
	while( TRUE )
	{
		nActNum = fp.GetInt();
		if( fp.IsEOF() ) break;
		sSOCIETY_CHAT* pSC = new sSOCIETY_CHAT;
		SafeStrCpy( pSC->buf, fp.GetString(), MAX_NAME_LENGTH+1 );
		pSC->nActNum = nActNum;
		pSC->nLen = strlen( pSC->buf );
		m_ListAct.AddTail( pSC );
	}
	fp.Release();	
}
int CChatManager::CaptureSocietyAction( char* pStr )
{
	while( *pStr )
	{
		PTRLISTPOS pos = m_ListAct.GetHeadPosition();
		while( pos )
		{
			sSOCIETY_CHAT* pSC = (sSOCIETY_CHAT*)m_ListAct.GetNext( pos );
			if( pSC )
			{
				if( strncmp( pStr, pSC->buf, pSC->nLen ) == 0 )
					return pSC->nActNum;
			}
		}
		if( IsDBCSLeadByte( *pStr ) )
			++pStr;
		if( *pStr )
			++pStr;
	}
	return -1;
}
BOOL TargetFunc(char *str)
{
	char code[128];
	char name[128];
	sscanf(str, "%s", code);
	if( strcmp( CHATMGR->GetChatMsg(590), code ) )
		return FALSE;
	sscanf(str, "%s %s", code, name);
	if( !OBJECTMGR->TargetByName(name) )
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(591) );
	return TRUE;
}
#include <time.h>
void CChatManager::ShowForbidChatTime( __time64_t time )
{
	struct tm curTimeWhen = *_localtime64( &time );
	CHATMGR->AddMsg( CTC_SYSMSG,  CHATMGR->GetChatMsg(2660), 
		curTimeWhen.tm_year + 1900,
		curTimeWhen.tm_mon + 1,
		curTimeWhen.tm_mday,
		curTimeWhen.tm_hour,
		curTimeWhen.tm_min,
		curTimeWhen.tm_sec );
}
void CChatManager::Test(__time64_t time)
{
	m_ForbidChatTime = time;
	ShowForbidChatTime( time );
}