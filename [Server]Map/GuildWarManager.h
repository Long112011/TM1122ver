





// MugongManager.h: interface for the CMugongManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GUILDWARMGR_H__12870C9F_2E96_4A52_9AF3_B4C8FA0A27DE__INCLUDED_)
#define AFX_GUILDWARMGR_H__12870C9F_2E96_4A52_9AF3_B4C8FA0A27DE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "MHFile.h"
#include "..\4DyuchiGRX_common\typedef.h"

class CGuildUnion;
class CGuild;
class CPlayer;

#define GUILDWARMGR CGuildWarManager::GetInstance()

enum{
	eGuildState_Wait,
	eGuildState_WarIng,
	eGuildState_End,
	eGuildState_Remove,
};

struct GUILD_WAR_TIME_INFO
{
	BOOL pOpen;//是否开启
	stTIME pStartTime;//活动开始时间
	DWORD  pWarTime;//活动持续时间
};

struct PLAYER_WAR_RAND_INFO
{
	DWORD pMoneyJB;
	DWORD pMoneyPD;
	DWORD pMoney;
};

struct GUILD_WAR_GUILD_PLAYER_RAND_INFO
{
	DWORD pMoneyJB;
	DWORD pMoneyPD;
	DWORD pMoney;
};
struct GUILD_WAR_RAND_INFO
{
	DWORD pGuildPoint;
	GUILD_WAR_GUILD_PLAYER_RAND_INFO pMaster;
	GUILD_WAR_GUILD_PLAYER_RAND_INFO pOther;
};

struct GUILD_WAR_PLAYER_INFO
{
	CPlayer* pPlayer;
	DWORD	pCharacterIDX;
	DWORD	pUniconIDX;
	char	pCharacterName[MAX_NAME_LEN+1];

	DWORD pPlayerKillCount;
	DWORD	pDieCount;
};

struct GUILD_WAR_GUILD_INFO
{
	CGuildUnion* pGuildUnion;
	DWORD	pGuildKillCount;
	DWORD	pGuildDieCount;
};

struct GUILD_WAR_MAPINFO
{
	WORD pMapNum;
	BYTE pChannnelNum;
	BYTE pLimitZhuanSheng;
	BYTE pLimitLevel;
	BYTE pClearMonster;
	BYTE pRangeLevel;
};

class CGuildWarManager  
{

	CGuildWarManager();
public:
	GETINSTANCE(CGuildWarManager);
	virtual ~CGuildWarManager();

	void Init();

	void CGuildWarManager::Process();

	void KillPlayer(CPlayer* pAttackPlayer,CPlayer* pDiePlayer);

	void CGuildWarManager::SendWarInfo(CPlayer* pPlayer);
	void CGuildWarManager::SendWarInfo();

	void AddPlayer(CPlayer* pPlayer);
	void RemovePlayer(CPlayer* pPlayer);

	void CGuildWarManager::Relase();
	void CGuildWarManager::GetSprotf(CMHFile* file);
	void CGuildWarManager::GetGuildSprotf(CMHFile* file);

	void CGuildWarManager::WarEndSpotf();

	BOOL IsWarMap(WORD pChannel = 0)
	{
		if(pChannel)
		{
			return pIsWarMap && pChannel == pWarMapInfo.pChannnelNum;
		}
		else
			return pIsWarMap;
	}

	BOOL CGuildWarManager::MoveMap(CPlayer* pPlayer,WORD pMapNum,WORD pChannelIDX,BYTE pType);

	//同步盟战状态信息
	void CGuildWarManager::ChangeWarState(BYTE pState,BOOL pSend = FALSE,WORD pMapNum = 0,WORD pChannelNum = 0);
	void CGuildWarManager::GetWarState();
	void CGuildWarManager::PushWarState();

	void CGuildWarManager::SavePlayerKillList();

	void CGuildWarManager::RemoveAllPlayer();

	void CGuildWarManager::ReturnToMap(CPlayer* pPlayer);
	//暂停刷怪
	BOOL CGuildWarManager::SuspendMonster(WORD pChannel);

	void CGuildWarManager::RemoveMonster();

	void CGuildWarManager::AutoRevive();

private:
	CYHHashTable<GUILD_WAR_GUILD_INFO> pGuildInfoTable;
	CYHHashTable<GUILD_WAR_PLAYER_INFO> pPlayerInfoTable;

	GUILD_WAR_TIME_INFO pWarTimeInfo[7];
	BOOL				pWarLast[7];
	BOOL				pLog[7];
	BOOL				pNote[3];

	DWORD pKillSpotfMoneyJB;//击杀一人奖励的元宝
	DWORD pKillSpotfMoneyPD;//击杀一人奖励的泡点
	DWORD pKillSpotfMoney;//击杀一人奖励的游戏币
	DWORD pKillSpotfGuildPoint;//击杀一人奖励的门派经验

	WORD pGuildRandSprotfCount;//门派击杀总排行奖励数量
	WORD pPlayerRandSprotfCount;//玩家击杀总排行奖励数量
	GUILD_WAR_RAND_INFO* pGuildWarRandInfo;//门派击杀排行奖励
	PLAYER_WAR_RAND_INFO* pPlayerWarRandInfo;//玩家击杀排行奖励

	DWORD pSendAllInfoTime;
	BYTE pWarState;
	DWORD pWarStartTime;//活动开始时间
	DWORD pWarTime;//活动持续时间

	DWORD pRemoveTime;

	GUILD_WAR_MAPINFO pWarMapInfo;//活动地图
	BOOL pIsWarMap;
};


#endif // !defined(AFX_GUILDWARMGR_H__12870C9F_2E96_4A52_9AF3_B4C8FA0A27DE__INCLUDED_)


