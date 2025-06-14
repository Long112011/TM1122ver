#pragma once
enum eUSERINFO_KIND
{
	eUIK_NONE		= 0,
	eUIK_INTERFACE	= 1,
	eUIK_USERSTATE	= 2,
	eUIK_USERAI		= 3,
	eUIK_RECONNECT	= 4,
	eUIK_ALL		= 0xFFFFFFFF,
};
enum eSAVE_DIALOG
{
	eSD_HeroGuage,			
	eSD_MonsterGuage,		
	eSD_MiniMap,			
	eSD_Quick,				
	eSD_MainBar,			
	eSD_Chat,				
	eSD_Inventory,			
	eSD_Character,			
	eSD_Skill,				
	eSD_Option,				
	eSD_Quest,				
	eSD_EventNotify,		
	eSD_Party,				
	eSD_PetInfo,			
	eSD_Reconnect,
	eSD_Shout,
	eSD_ItemLink,
	eSD_FPSCounter,
	eSD_Max,
};
enum eUSERINFO_FLAG
{
	eUIF_NONACTIVE	= 0,
	eUIF_ACTIVE		= 1,
	eUIF_SECONDFORM	= 2,
	eUIF_TABNUM2	= 4,
	eUIF_TABNUM3	= 8,
	eUIF_TABNUM4	= 16,
	eUIF_SUBTABNUM2	= 32,
	eUIF_SUBTABNUM3	= 64,
	eUIF_SUBTABNUM4	= 128,
	eUIF_SUBACTIVE	= 256,
};
struct sDIALOG_INFO
{
	LONG	lPosX;
	LONG	lPosY;
	DWORD	dwFlag;
	DWORD	dwParam;
};
struct sUSERINFO_INTERFACE
{
	sDIALOG_INFO	DIALOG_INFO[eSD_Max];
};
struct sUSERINFO_USERSTATE
{
	WORD wKyungGongIdx;
	BOOL bIsKyungGongMode;
	float fZoomDistance;
};
#define USERINFOMGR USINGTON(CUserInfoManager)
class CUserInfoManager  
{
protected:
	char m_strUserID[MAX_NAME_LENGTH+1];
	DWORD m_dwSaveFolderName;
	BOOL m_bMapChange;
	BOOL m_bMinimapActive;
	BOOL m_bReconnectActive;
	BOOL m_bShoutActive;
	//BOOL m_bIsReconnect;
public:
	CUserInfoManager();
	virtual ~CUserInfoManager();
	void LoadUserInfo( DWORD dwFlag );
	void SaveUserInfo( DWORD dwFlag );
	void LoadInterfaceInfo( char* strFilePath );
	void SaveInterfaceInfo( char* strFilePath );
	void LoadReconnectInterface(char* strFilePath);
	void SaveReconnectInterface(char* strFilePath);
	void LoadUserState( char* strFilePath );
	void SaveUserState( char* strFilePath );
	void SetUserID( char* strID );
	char* GetUserID() { return m_strUserID; }
	BOOL IsValidInterfacePos( RECT* prcCaption, LONG lX, LONG lY );
	void SetMapChage(BOOL bMapChange) { m_bMapChange = bMapChange; }
	//void SetReconnect(BOOL bFlag) { m_bIsReconnect = bFlag; }
	void SetSaveFolderName( DWORD dwUserID );
	DWORD GetSaveFolderName(){return m_dwSaveFolderName;}
};
EXTERNGLOBALTON(CUserInfoManager)