#ifndef _CHATMANAGER_H
#define _CHATMANAGER_H
#define CHATMGR USINGTON(CChatManager)
#include "gamein.h"
#include "shoutchatdialog.h"
#define MAX_CHATMSGNUM	3000
class CKeyboard;
#include "ChatDialog.h"
enum eTextClass
{
	CTC_OPERATOR,
	CTC_GENERALCHAT,
	CTC_PARTYCHAT,
	CTC_GUILDCHAT,
	CTC_ALLIANCE,
	CTC_SHOUT,
	CTC_TIPS,
	CTC_WHISPER,
	CTC_MONSTERSPEECH,
	CTC_MONSTERSHOUT,
	CTC_ATTACK,	
	CTC_ATTACKED,
	CTC_DEFENCE,
	CTC_DEFENCED,
	CTC_KILLED,	
	CTC_MPWARNING,
	CTC_HPWARNING,	
	CTC_GETITEM,
	CTC_GETMONEY,
	CTC_GETEXP,
	CTC_GETABILITYEXP,
	CTC_ABILITYUPGRADE_SUCCESS,
	CTC_ABILITYUPGRADE_FAILED,
	CTC_SYSMSG,	
	CTC_TOGM,	
	CTC_TOWHOLE,
	CTC_TOPARTY,
	CTC_TOGUILD,
	CTC_CHEAT_1,
	CTC_CHEAT_2,
	CTC_CHEAT_3,
	CTC_CHEAT_4,
	CTC_CHEAT_5,
	CTC_QUEST,
	CTC_GMCHAT,
	CTC_BILLING,	
	CTC_TRACKING,
	CTC_GTOURNAMENT,
	CTC_JACKPOT,
	CTC_GUILD_NOTICE,
	CTC_SURVIVAL,
	CTC_ALERT_YELLOW,
	CTC_ALERT_RED,
	CTC_LEVELMAX,
	CTC_PRINT_BOSS,
	CTC_PRINT_KILL,
	CTC_PRINT_MIN5,
	CTC_DUNGEON,
	CTC_SHOUTITEM,
	CTC_RECALL,
	CTC_COUNT,
};
const DWORD dwChatTextColor[CTC_COUNT] = {
	RGBA_MAKE(0, 255, 60, 255),
	RGBA_MAKE(255, 255, 255, 255),
	RGBA_MAKE(185, 241, 69, 255),
	RGBA_MAKE(1, 209, 247, 255),
	RGBA_MAKE(240, 255, 0, 255),
	RGBA_MAKE(247, 182, 24, 255),
	RGBA_MAKE(255, 255, 150, 255),
	RGBA_MAKE(235, 195, 206, 255),
	RGBA_MAKE(255, 255, 255, 255),
	RGBA_MAKE(247, 182, 24, 255),
	RGBA_MAKE(255, 97, 19, 255),
	RGBA_MAKE(255, 97, 19, 255),
	RGBA_MAKE(255, 97, 19, 255),
	RGBA_MAKE(255, 97, 19, 255),
	RGBA_MAKE(255, 97, 19, 255),
	RGBA_MAKE(255, 52, 19, 255),
	RGBA_MAKE(255, 52, 19, 255),
	RGBA_MAKE(2, 255, 163, 255),
	RGBA_MAKE(200, 200, 200, 255),
	RGBA_MAKE(200, 200, 200, 255),
	RGBA_MAKE(200, 200, 200, 255),
	RGBA_MAKE(82, 255, 204, 255),
	RGBA_MAKE(82, 255, 204, 255),
	RGBA_MAKE(82, 255, 204, 255),
	RGBA_MAKE(82, 255, 204, 255),
	RGBA_MAKE(171, 195, 255, 255),
	RGBA_MAKE(255, 158, 6, 255),
	RGBA_MAKE(240, 255, 0, 255),
	RGBA_MAKE(255, 0, 0, 255),
	RGBA_MAKE(255, 255, 0, 255),
	RGBA_MAKE(255, 0, 255, 255),
	RGBA_MAKE(0, 255, 255, 255),
	RGBA_MAKE(100, 50, 100, 255),
	RGBA_MAKE( 0, 255, 255, 255 ),
	RGBA_MAKE( 28, 233, 151, 255 ),
	RGBA_MAKE(0, 255, 60, 255),
	RGBA_MAKE(255, 20, 20, 255),
	RGBA_MAKE(20, 255, 60, 255),
	RGBA_MAKE(144, 252, 222, 255),
	RGBA_MAKE(255, 214, 0, 255),
	RGBA_MAKE(144, 252, 222, 255),
	RGBA_MAKE(255, 252, 0, 255),
	RGBA_MAKE(255, 0, 0, 255),
	RGBA_MAKE(255, 0, 0, 255),
	RGBA_MAKE(255, 255, 0,255),
	RGBA_MAKE(255, 255,  0,255),
	RGBA_MAKE(0, 255, 0,255),
	RGBA_MAKE(1, 209, 247, 255),
	RGBA_MAKE(144, 252, 222, 255),
	RGBA_MAKE(60, 60, 60, 150),
};
enum eChatOption
{
	CTO_NOSYSMSG,
	CTO_NOITEMMSG,
	CTO_NOEXPMSG,
	CTO_NOCHATMSG,
	CTO_AUTOHIDECHAT,
	CTO_NOSHOUTMSG,
	CTO_NOGUILDMSG,
	CTO_NOALLIANCEMSG,	
	CTO_COUNT,
};
struct sChatOption
{
	BOOL bOption[CTO_COUNT];
};
struct sSOCIETY_CHAT
{
	int nActNum;
	int nLen;
	char buf[MAX_NAME_LENGTH+1];
};
struct sABILITY_CHAT
{
	int nAbilityIndex;
	char buf[MAX_NAME_LENGTH+1];
};
#define MAX_WHISPERLIST_PER_SIDE 2
enum eCHAT_DELAY
{
	eCHATDELAY_PARTY,
	eCHATDELAY_GUILD,
	eCHATDELAY_UNION,
	eCHATDELAY_SHOUT,
	eCHATDELAY_WHISPER,
	eCHATDELAY_MAX,
};
class CUserCommendParser;
class CChatManager
{
protected:
	BOOL			m_bPause;
	sChatOption		m_ChatOption;
	CChatDialog*	m_pChatDialog;
	CUserCommendParser* m_pUserCommendParser;
	char*			m_pChatMsg[MAX_CHATMSGNUM];
	char			m_NULL;
	char			m_strLastName[MAX_NAME_LENGTH+2];
	cPtrList		m_ListChat[MAX_CHAT_COUNT];
	DWORD			m_dwLastChatTime;
	DWORD			m_dwLastChatDelayTime[eCHATDELAY_MAX];
	cPtrList		m_ListAct;
	cPtrList		m_ListAbility;
	cPtrList		m_ShoutList;
	char			m_WhisperName[MAX_NAME_LENGTH+1];
	char			m_WhisperList[MAX_WHISPERLIST_PER_SIDE*2+1][MAX_NAME_LENGTH+1];
	int				m_nWhisperListNum;
	__time64_t  m_ForbidChatTime;
protected:
	void LoadSocietyActionFilter();
	int CaptureSocietyAction( char* pStr );
	void LoadAbilityFilter();
	void LoadChatMsg();
	void PrintMsg( int nClass, char* msg );
	void PrintMsg(DWORD nColor, char* msg);
	void AddWhisperSenderList( char* pName );
	void AddWhisperReceiverList( char* pName );
	void RemoveWhisperSenderList( char* pName );
	void RemoveWhisperReceiverList( char* pName );
public:
	void SetWhisperName(char* name);
	void AutoWhisperForRbt();
	void ChangeWhisperUser( BOOL bUp );
	CChatManager();
	virtual ~CChatManager();
	void Init();
	void Release();
	void MsgProc( UINT msg, WPARAM wParam );
	void SetOption( sChatOption* pChatOption );
	sChatOption* GetOption()					{ return &m_ChatOption; }
	void AddMsg( int nClass, char* str, ... );
	void AddMsg(DWORD nColor, char* str, ...);
	void AddMsgFade( int nClass, char* str, ... );
	void AddMsgShout(int nClass, char* str, ...);
	void RemoveMsg(char* str);
	char* GetChatMsg( int nMsgNum );
	void NetworkMsgParse( BYTE Protocol, void* pMsg );
	void SetPause( BOOL bPause ) { m_bPause = bPause; }
	void SetChatDialog( CChatDialog* pChatDlg );
	void OnInputChatMsg( char* str );
	void SetAutoWhisper();
	void AddChatMsg( BYTE ChatLimit, DWORD MsgColor, char* str );
	void AddChatMsgAllRecall(DWORD MsgColor, char* str);
	void AddChatMsgAll( DWORD MsgColor, char* str );
	void AddChatMsgAll(int MsgColor, char* str);
	BOOL CanChatTime( char cToken );
	void SaveProcList();
	void SaveChatList();
	void RestoreChatList();
	void ReleaseChatList();
	void AddMsgToList( BYTE ChatLimit, DWORD MsgColor, char* str );
	void AddMsgAllToListRecall(DWORD MsgColor, char* str);
	void AddMsgAllToList( DWORD MsgColor, char* str );
	void ShowForbidChatTime( __time64_t time );
	void Test(__time64_t time);
};
EXTERNGLOBALTON(CChatManager)
BOOL TargetFunc(char *str);
#endif
