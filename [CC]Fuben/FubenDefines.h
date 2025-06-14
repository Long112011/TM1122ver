



#ifndef __FubenDEFINES_H__
#define __FubenDEFINES_H__

#pragma pack(push,1)

#ifdef _MAPSERVER_
#define	PLAYERTYPE	CPlayer
class CPlayer;
#else
#define PLAYERTYPE	CHero
class CHero;
#endif


#define MAX_Fuben_PROBABILITY		10000

#define	MAX_Fuben					1000
#define	MAX_FubenEVENT_MGR			1000
#define MAX_FubenEVENT_PLYER		100
#define	MAX_SUBFuben				32
//#define MAX_ITEMNAME_LENGTH			30
#define	MAX_ITEMDESC_LENGTH			255
#define	MAX_ITEMDESC_LINE			20
#define MAX_PROCESSFuben			100
#define MAX_COMPLETEFuben			100
#define VIEW_FubenITEM_PERPAGE		20
#define	Fuben_GIVEUP_ID				99
#define MAX_FubenITEM				100



#define TITLE_COLOR					RGB(210, 0, 255)
#define	MAINFuben_TITLE_COLOR		RGB(128, 200, 128)
#define	MAINFuben_TITLE_COLOR_SEL	RGB(128, 255, 128)
#define SUBFuben_TITLE_COLOR		RGB(0, 180, 210)


#define SUBFuben_TITLE_COLOR_SEL	RGB(0, 200, 255)
#define Fuben_GIVEUP_COLOR			RGB(80, 80, 210)
#define Fuben_GIVEUP_COLOR_SEL		RGB(100, 100, 255)
#define Fuben_DESC_COLOR			RGB(255, 255, 255)
#define Fuben_DESC_HIGHLIGHT		RGB(250, 200, 50)
#define	SELECT_COLOR				RGB(225, 200, 200)
#define	SELECT_COLOR_SEL			RGB(250, 230, 230)
#define	TITAN_MAINFuben_TITLE_COLOR		RGB(255, 200, 0)
#define	TITAN_MAINFuben_TITLE_COLOR_SEL	RGB(255, 255, 0)



#define COMBINEKEY(a, b, c)				\
	if(b<100)			c=a*100+b;		\
	else if(b<1000)		c=a*1000+b;		\
	else if(b<10000)	c=a*10000+b;	



enum eFubenEvent
{
	eFubenEvent_NpcTalk = 1,		// Npc´eE­
	eFubenEvent_Hunt,				// Monster≫c³E
	eFubenEvent_EndSub,
	eFubenEvent_Count,	
	eFubenEvent_GameEnter,	
	eFubenEvent_Level,	
	eFubenEvent_UseItem,
	eFubenEvent_MapChange,
	eFubenEvent_Die,	
	eFubenEvent_Time,
	eFubenEvent_HuntAll,
};

enum eFubenExecute
{
	eFubenExecute_EndFuben,
	eFubenExecute_StartFuben,

	eFubenExecute_EndSub,
	eFubenExecute_EndOtherSub,
	eFubenExecute_StartSub,
	
	eFubenExecute_AddCount,	
	eFubenExecute_MinusCount,

	eFubenExecute_GiveFubenItem,	// Fuben item 창에 들어가는 거
	eFubenExecute_TakeFubenItem,	
	
	eFubenExecute_GiveItem,			// inventory
	eFubenExecute_GiveMoney,
	eFubenExecute_TakeItem,		
	eFubenExecute_TakeMoney,
	eFubenExecute_TakeExp,	
	eFubenExecute_TakeSExp,

	eFubenExecute_RandomTakeItem,

	eFubenExecute_TakeFubenItemFQW,	// from 특정 무기로
	eFubenExecute_AddCountFQW,
	eFubenExecute_TakeFubenItemFW,	// from 무기류
	eFubenExecute_AddCountFW,
	
	eFubenExecute_TakeMoneyPerCount,
	
	eFubenExecute_RegenMonster,
	eFubenExecute_MapChange,
	
	eFubenExecute_ChangeStage,
	

	eFubenExecute_ChangeSubAttr,

	eFubenExecute_RegistTime,


	eFubenExecute_LevelGap,
	eFubenExecute_MonLevel,

	eFubenExecute_EndOtherFuben,

	eFubenExecute_SaveLoginPoint,
};

enum eFubenLimitKind
{
	eFubenLimitKind_Level,
	eFubenLimitKind_Money,
	eFubenLimitKind_Fuben,
	eFubenLimitKind_SubFuben,	
	eFubenLimitKind_Stage,
	eFubenLimitKind_Attr,
};

enum eFubenValue
{
	eFubenValue_Add,
	eFubenValue_Minus,
};


enum
{
	eFBView_QList,
	eFBView_QDesc,
};

enum
{
	eFBTree_NotUse,
	eFBTree_Close,
	eFBTree_Open,
};

enum
{
	eFBItem_AddCount,

	eFBItem_SetCount,
	eFBItem_GetCount,
};

struct SUBFuben
{
	DWORD	dwMaxCount;
	DWORD	dwData;
	QSTATETYPE	dwTime;	//륜麓
	SUBFuben() : dwMaxCount(0), dwData(0), dwTime(0)	{}
};

struct FubenITEM
{
	DWORD	dwFubenIdx;
	DWORD	dwItemIdx;
	DWORD	dwItemNum;

	FubenITEM() : dwFubenIdx(0), dwItemIdx(0), dwItemNum(0)	{}
};

struct Fuben_ITEM_INFO
{
	DWORD		ItemIdx;
	DWORD		FubenKey;
	char		ItemName[MAX_ITEMNAME_LENGTH+1];
	WORD		Image2DNum;
	WORD		SellPrice;
	char		ItemDesc[MAX_ITEMDESC_LENGTH+1];
};


struct FUBENMAPINFO
{
	DWORD FBMapID;
	DWORD FBMapMonster;
	DWORD FBMapKillCount;
	int	  FBMapMonsterX;
	int	  FBMapMonsterY;
	DWORD FBMapMonsterRadius;
	DWORD FBMapMessage;
	DWORD FBMapItemGet;
	DWORD FBMapNextFlag;
	bool  FBMapEnableKillAll;
	WORD  FBMapKillAllZero;
};
struct sDUNGEONNEWINIT
{
	WORD	GroupNum;
	WORD	SubMission;
	int		MonsterTarget;
	int		MonsterKillCount;
	int		NextSubMission;

	bool	EnableKillAll;
	int		KillAllZero;


	int		timer[1000];

	void Clear()
	{
		memset(this, 0, sizeof(sDUNGEONNEWINIT));
	}

	sDUNGEONNEWINIT()
	{
		Clear();
	}
};

#pragma pack(pop)
#endif


