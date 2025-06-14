
#pragma once



#include <AntiCpSvrFunc.h>
#include "Crypt.h"


#ifdef _NPROTECT_
class CCSAuth2;
#endif

BOOL DisconnectUser(DWORD dwConnectionIndex);

struct aGAMEOPTION
{
	BYTE bNoFriend;
	BYTE bNoWhisper;

	aGAMEOPTION():bNoWhisper(0), bNoFriend(0) {}
};

struct CHARSELECTINFO
{
	DWORD dwCharacterID;
	LEVELTYPE Level;
	MAPTYPE MapNum;
	BYTE Gender;
	char CharacterName[MAX_NAME_LENGTH+1];
};

struct USERINFO
{
	DWORD	dwConnectionIndex;
	DWORD	dwCharacterID;
	DWORD	dwUserID;
	BYTE	UserLevel;
	DWORD	dwMapServerConnectionIndex;
	WORD	wUserMapNum;
	CHARSELECTINFO SelectInfoArray[MAX_CHARACTER_NUM];
	char Mac[18];
	BOOL m_IsSiegeWarMaster; //쭤捺냘寮깃街	
	WORD m_MaxPack;
	DWORD	DistAuthKey;			// Distribute 서버에서 보내온 AuthKey 보관
//KES 연속 채팅 금지
	DWORD	dwLastChatTime;	
//---KES CheckSum 071003
	BYTE	cbCheckSum;
//----------------------

	WORD	wChannel;
	DWORD	dwUniqueConnectIdx;	// 서버에서의 고유 연결 번호
								// dwConnectionIdx와는 다름. 계속 증가함

	CCrypt crypto;

	char   reUserID[20];
	char   rePwd[20];
	char   reTel[20];
	char   reEmail[50];
	char   NewPwd[20];
//	char   Mac[18];
	char   IP[18];

	aGAMEOPTION GameOption;

	DWORD	dwLastConnectionCheckTime;
	BOOL	bConnectionCheckFailed;
	
#ifdef _JAPAN_LOCAL_
	DWORD	dwExtraUserSlot;
#endif	// _JAPAN_LOCAL_
#ifdef _HK_LOCAL_
	DWORD	dwExtraUserSlot;
#endif	// _HK_LOCAL_
#ifdef _TL_LOCAL_
	DWORD	dwExtraUserSlot;
#endif	// _TL_LOCAL_
#ifdef _HACK_SHIELD_
	BYTE m_GUIDInfo[SIZEOF_GUIDREQINFO];
	BYTE m_Info[SIZEOF_REQINFO];
	unsigned long* m_pCrcInfo;
	BOOL m_bHSCheck;
#endif
#ifdef _NPROTECT_
	CCSAuth2 *m_pCSA;
	BOOL m_bCSA;

	int m_nCSAInit;
	DWORD	dwLastNProtectCheck;
	DWORD	m_dwHUC;
	BOOL bHackToolUser;
#endif
};//USERINFO;

typedef void (__stdcall *USERDisconnectProc)(USERINFO* pInfo);

void	__stdcall OnRemoveObjProc( USERINFO* pInfo);


class CUserTable : public CYHHashTable<USERINFO>
{
	DWORD m_MaxUserCount;
	DWORD m_dwUserCount;
	DWORD m_addCount;
	DWORD m_removeCount;

	USERDisconnectProc m_pOnDisconnectProc;

public:

	CUserTable();
	virtual ~CUserTable();
	void SetPositionUserHead() {  CYHHashTable<USERINFO>::SetPositionHead(); }
	USERINFO * GetUserData() { return CYHHashTable<USERINFO>::GetData(); }
	void Init(DWORD dwBucket, USERDisconnectProc pProc);
	USERINFO * FindUser(DWORD dwKey);
	BOOL AddUser(USERINFO* pObject,DWORD dwKey);
	USERINFO * RemoveUser(DWORD dwKey);
	void RemoveAllUser();

	DWORD GetUserCount(){ return m_dwUserCount; }

	DWORD GetUserMaxCount(){return m_MaxUserCount;}
	void SetCalcMaxCount(DWORD CurCount);
	
	BOOL SendToUser(DWORD dwKey,DWORD dwUniqueConnectIdx,MSGBASE* pmsg,DWORD size);
	BOOL OnDisconnectUser(DWORD dwKey);

	DWORD GetAddCount() { return m_addCount; }
	DWORD GetRemoveCount() { return m_removeCount; }
};
  



extern CUserTable * g_pUserTable;							// key : connectionidx
extern CUserTable * g_pUserTableForUserID;					// key : UserID
extern CUserTable * g_pUserTableForObjectID;				// key : dwObjectID
