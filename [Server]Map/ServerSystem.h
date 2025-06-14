



// ServerSystem.h: interface for the CServerSystem class.
//

//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SERVERSYSTEM_H__FD3EBFC3_EE3D_4505_A5A1_24DA471D20AB__INCLUDED_)
#define AFX_SERVERSYSTEM_H__FD3EBFC3_EE3D_4505_A5A1_24DA471D20AB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "MHMap.h"
#include "IndexGenerator.h"

//#include "PtrList.h"
class CObject;
class CGridSystem;

class CPlayer;
class CTitan;
class CPet;
class CMonster;
class CNpc;
class CTacticObject;
class CTacticStartInfo;
class CSkillObject;
class CBossMonster;
class CMapObject;


// �ʵ庸�� - 05.12 �̿���
class CFieldBossMonster;
class CFieldSubMonster;

enum eNATION
{
	eNATION_KOREA,
	eNATION_CHINA,
};

enum{ePET_FROM_DB, ePET_FROM_ITEM};		//DB�κ��� �����, ��ȯ������ ù������� �ֻ���

enum eMapKind
{
	eNormalMap,
	eSurvival,

	//eRunningMap,
	eGTMap,	//��������
	eSGMap, //��������
};

struct WAIT_EXIT_PLAYER_INFO
{
	DWORD dwAgentConnectionIndex;
	DWORD dwPlayerID;
	DWORD dwStartTime;
};


//----------------------relog lastmap
typedef struct _RELOADWANTEDMAP_INFO
{
	WORD MoveMapNum;
	char ObjectName[MAX_NAME_LENGTH];
	
}RELOADWANTEDMAP_INFO;
//-----------------------------------


class CServerSystem  
{

//	CYHHashTable<CItemObject> m_ItemObjectTable;

	RELOADWANTEDMAP_INFO	m_ReloadWantedMap[MAX_POINT_NUM];

	CGridSystem* m_pGridSystem;
	CMHMap m_Map;
	
	friend class CCharMove;

	friend void GameProcess();

	BOOL m_start;
//	CIndexGenerator m_objectIdxGen;
	
	WORD	m_wMapNum;

	//SW061019 �ʼ���
	WORD	m_iMapKind;
	
//����
	int		m_Nation;

	cPtrList									m_listWaitExitPlayer;

	CMemoryPoolTempl<WAIT_EXIT_PLAYER_INFO>*	m_mpWaitExitPlayer;
	
	DWORD	m_dwQuestTime;
	BOOL	m_bQuestTime;

	BOOL	m_bCompletionChrUpdate;
//--check process time
	DWORD	m_dwMainProcessTime;


	BOOL			m_bCheckProcessTime;
	LARGE_INTEGER	m_freq;
	LARGE_INTEGER	m_ObjLoopProc[2];
	LARGE_INTEGER	m_TriggerProc[2];
	LARGE_INTEGER	m_DungeonProc[2];
	double			m_fTimeObjLoopProc;
	double			m_fTimeTriggerProc;
	double			m_fTimeDungeonProc;
	double			m_fAvrObjLoopProc;
	double			m_fAvrTriggerProc;
	double			m_fAvrDungeonProc;
	DWORD			m_dwProcessCount;

public:
	void ToggleCheckProcessTime();
	DWORD GetMainProcessTime() { return m_dwMainProcessTime; }



	void SetReloadWantedMapInfo();
	BOOL GetReloadWantedMapInfo(DWORD Index);

	// RaMa - ����Ȯ�ο�
	MSG_PROSESSINFO		m_ProcessInfo;
	BOOL				m_bCheckInfo;
	void CheckServerInfo( DWORD ObjectID );

	static bool ReloadWantedEnable;

    static int MaxReSetTime;

	static int ReSetPower;

	static int MinReSetLevel;

	static DWORD ReSetMoney;

	static WORD  ReLevelPoint;

	static WORD  ReCheRyukPoint;

	static DWORD FirstStageMoney;

	static DWORD SecondStageMoney;

	static int  ReStagePower;   // 2015-02-15 ����

	static WORD MarryLvSet;

	static DWORD MarryMoney;

	static DWORD MarryGold;

	static WORD ShiFuLvSet;

	static DWORD ShiTuMoney;

	static DWORD ShiTuGold;

	static WORD m_MonsterCount;

	static WORD PaoDianLevel;
    static WORD PaoDianReSet;
    static WORD PaoDianTime;
    static DWORD PaoDianMap;

	static BOOL GrowSwitch;
	static WORD m_Grow;

	static WORD CanBadFame;
	static WORD LootItemNum;
	static DWORD BolehRobKarma;

	static MONEYTYPE LuckerIngot;
	static MONEYTYPE LuckerBubble;
	static MONEYTYPE LuckerGold;
	static WORD LuckerMode;
	static BOOL LuckerEnable;

	static WORD GrowRate;
	static BOOL gControlLucker;

	static BOOL gBossShout;
	static BOOL gBossSubShout;

	static WORD TidyItemTime;
	static WORD TidyMallTime;


	static BOOL RobExp;
	static float RobExpPecent;

	static BOOL gBloodStorm;


	///DEVELOPER TEST AREA

	static DWORD DevMaxGradeUpPoint;
	static DWORD DevMaxAbilityPoint;

	static BOOL gIgnoreLatestUpdate;

public:

#ifdef _CHINA_LOCAL_
//���
 	SYSTEMTIME		m_NoPV;
 	int				m_NoPCode;
 	char			m_NoP[128];
//---
#endif


	CServerSystem();

	virtual ~CServerSystem();

	void AddWaitExitPlayer( DWORD dwAgentConnetionIndex, DWORD dwPlayerID );
	void ProcessWaitExitPlayer();
//����
	void SetNation();
	int GetNation() { return m_Nation; }

	void ReadServerConfig(WORD MapNum);

	void _5minProcess();
	void Process();
	void Start(WORD ServerNum);
	void End();		

	void ReloadBloodStorm();

	CMHMap* GetMap()	{ return &m_Map;	}
	WORD	GetMapNum()	{ return m_wMapNum;	}
	
	BOOL GetStart()		{ return m_start; 	}
	void SetStart(BOOL state);

	void	SetMapKind( WORD wMapNum );
	BOOL	CheckMapKindIs( int eMapkind );
	/*
	CObject* GetObject(DWORD dwObjectID);
	CPlayer* GetPlayer(DWORD dwPlayerID);
	CMonster* GetMonster(DWORD dwMonsterID);
	CNpc* GetNpc(DWORD dwNpcID);
	*/
//	CItemObject* GetItemObject(DWORD dwItemID);
	
	CPlayer* AddPlayer(DWORD dwPlayerID, DWORD dwAgentNum,DWORD UniqueIDinAgent,int ChannelNum, eUSERLEVEL=eUSERLEVEL_USER);
	CPlayer* InitPlayerInfo(BASEOBJECT_INFO* pBaseObjectInfo,CHARACTER_TOTALINFO* pTotalInfo,HERO_TOTALINFO* pHeroInfo);

	CTitan*	MakeTitan(CPlayer* pOwner);
	TITAN_TOTALINFO* MakeTitanInfo(TITAN_TOTALINFO* pInfo);
	void	RemoveTitanInfo(TITAN_TOTALINFO* pInfo);

	void CreateNewPet(CPlayer* pMaster, DWORD dwItemIdx, DWORD SummonItemDBIdx, WORD wPetKind, WORD PetGrade = 1);
	CPet*	AddPet(CPlayer* pMaster, DWORD dwPetObjID, PET_TOTALINFO* pTotalInfo,VECTOR3* pPos);
//	CPet*	AddPet(DWORD dwPetObjID, CPlayer* pMaster, BASEOBJECT_INFO* pBaseObjectInfo, PET_TOTALINFO* pTotalInfo, WORD wPetKind, int flag = ePET_FROM_DB);
	CMonster* AddMonster(DWORD dwSubID, BASEOBJECT_INFO* pBaseObjectInfo,MONSTER_TOTALINFO* pTotalInfo,VECTOR3* pPos,WORD wObjectKind = 32);
	CBossMonster* AddBossMonster(DWORD dwSubID, BASEOBJECT_INFO* pBaseObjectInfo,MONSTER_TOTALINFO* pTotalInfo,VECTOR3* pPos);
	

	// �ʵ庸�� - 05.12 �̿���
	CFieldBossMonster* AddFieldBossMonster(DWORD dwSubID, BASEOBJECT_INFO* pBaseObjectInfo,MONSTER_TOTALINFO* pTotalInfo,VECTOR3* pPos);
	CFieldSubMonster* AddFieldSubMonster(DWORD dwSubID, BASEOBJECT_INFO* pBaseObjectInfo,MONSTER_TOTALINFO* pTotalInfo,VECTOR3* pPos);
	
	//CNpc* AddNpc(BASEOBJECT_INFO* pBaseObjectInfo,NPC_TOTALINFO* pTotalInfo,VECTOR3* pPos);
	//SW070626 ���λ�NPC
	CNpc* AddNpc(BASEOBJECT_INFO* pBaseObjectInfo,NPC_TOTALINFO* pTotalInfo,VECTOR3* pPos,float angle = 0);

	CTacticObject* AddTacticObject(BASEOBJECT_INFO* pBaseObjectInfo,CTacticStartInfo* pInfo,CPlayer* pOperator,DIRINDEX Direction);
	CSkillObject* AddSkillObject(CSkillObject* pSkillObj,VECTOR3* pPos);

	CMapObject* AddMapObject(DWORD Kind, BASEOBJECT_INFO* pBaseObjectInfo, MAPOBJECT_INFO* pMOInfo, VECTOR3* pPos);

	void RemovePlayer(DWORD dwPlayerID, BOOL bRemoveFromUserTable = TRUE);
	void RemoveTitan(CTitan* pTitan);
	void RemovePet(DWORD dwPetObjectID);
	void RemoveMonster(DWORD dwPlayerID);
	void RemoveBossMonster(DWORD dwMonster, BOOL bDisappear=FALSE);
	void RemoveNpc(DWORD dwPlayerID);
	void RemoveTacticObject(DWORD dwTacticObjID);
	void RemoveItemObject(DWORD ItemID);
	void RemoveSkillObject(DWORD SkillObjectID);
	void RemoveMapObject( DWORD MapObjID );

	
	inline CGridSystem* GetGridSystem()	{	return m_pGridSystem;	}


	void SendToOne(CObject* pObject,void* pMsg,int MsgLen);
	
	void ReloadResourceData();

	void ButtonToggleLoadFBInfo();
	
    // 2015-01-25 �Ĳ�����

	void GambleProcess();


	void	SetCharUpdateCompletion(BOOL bVal)	{ m_bCompletionChrUpdate = bVal;	}
	BOOL	GetCharUpdateCompletion()	{	return m_bCompletionChrUpdate;	}
	void	RemoveServerForKind( WORD wSrvKind );
	void	HandlingBeforeServerEND();

	void	LoadHackCheck();
};

//void GameProcess();
//void _5minGameProcess();

void __stdcall  GameProcess(DWORD dwEventIndex);
void __stdcall _5minGameProcess(DWORD dwEventIndex);

extern CServerSystem * g_pServerSystem;

void __stdcall  OnConnectServerSuccess(DWORD dwIndex, void* pVoid);
void __stdcall  OnConnectServerFail(void* pVoid);

//weiye kernel thread

/*void  _stdcall  ProcessDBMessage(DWORD dwEventIndex);
void  _stdcall ReceivedMsgFromServer(DWORD dwConnectionIndex,char* pMsg,DWORD dwLength);
void _stdcall ReceivedMsgFromUser(DWORD dwConnectionIndex,char* pMsg,DWORD dwLength);
void _stdcall OnAcceptServer(DWORD dwConnectionIndex);
void _stdcall OnDisconnectServer(DWORD dwConnectionIndex);
void _stdcall OnAcceptUser(DWORD dwConnectionIndex);
void _stdcall OnDisconnectUser(DWORD dwConnectionIndex);*/
//void _stdcall GameProcess(DWORD dwEventIndex);

//void _stdcall _5minGameProcess(DWORD dwEventIndex);
//void _stdcall OnConnectServerSuccess(DWORD dwIndex, void* pVoid);
//void _stdcall OnConnectServerFail(void* pVoid);

BOOL LoadEventRate(char* strFileName);
BOOL LoadPartyPlustime(char* strFileName);
BOOL LoadFameConfig(char* strFileName);


#endif // !defined(AFX_SERVERSYSTEM_H__FD3EBFC3_EE3D_4505_A5A1_24DA471D20AB__INCLUDED_)


