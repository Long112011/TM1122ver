#ifndef __COMMONSTRUCT_H__
#define __COMMONSTRUCT_H__
#include "..\[CC]Ability\AbilityCommonHeader.h"
#include "AddableInfo.h"
#include <string>       
#pragma pack(push,1)
#define MAX_NETWORK_MSG	256
typedef WORD	MAPTYPE;
typedef INT64	EXPTYPE;
typedef WORD	LEVELTYPE;
typedef DWORD	DURTYPE;
typedef WORD	POSTYPE;
typedef DWORD	ITEMPARAM;
typedef int		FAMETYPE;
typedef DWORD	WANTEDTYPE;
typedef DWORD	MARKNAMETYPE;
typedef DWORD	MONEYTYPE;//kiv
//typedef unsigned long long			MONEYTYPE;
#define MAXMONEY		MAXULONG_PTR
struct MSGROOT
{
	BYTE CheckSum;
	MSGROOT():Code(0){}
	char Code;
	BYTE Category;
	BYTE Protocol;
	WORD  CyptKey;
	char AesKey[33];    
	char AesText1[33];  
	char AesText2[33];  
};
struct MSGBASE :public MSGROOT
{
	DWORD dwObjectID;
};
struct MSGREG:public MSGROOT
{
	char ID[20];
	char pwd[20];
	char tel[20];
	char email[50];
	char NewPwd[20];
	char temp[512];
	char Mac[18];
};
struct MSGSHANNELINFOSYN:public MSGBASE
{
	BOOL bIsReConn;
	MAPTYPE wMapNum;
};
struct MAPSERVER_TRANSDATAONOFF : public MSGROOT 
{
	int MapServerGroupNum;
	int MapServerNum;
	int HardNum;
};
struct MAPSERVER_TRANSDATA : public MSGROOT 
{
	int MapServerGroupNum;
	int MapServerNum;
	int HardNum;
};
struct MSGBASEDATA :public MSGROOT
{
	int MapServerGroupNum;
	int MapServerNum;
	char Messagebuff[1028];
};
struct MSGCRYPTDATA : public MSGROOT
{
	HselInit eninit;
	HselInit deinit;
};
struct COMPRESSEDPOS
{
	WORD wx;
	WORD wz;
	void Compress(VECTOR3* pPos)
	{
		wx = WORD(pPos->x);
		wz = WORD(pPos->z);
	}
	void Decompress(VECTOR3* pRtPos)
	{
		pRtPos->x = wx;
		pRtPos->z = wz;
		pRtPos->y = 0;
	}
};
struct COMPRESSEDTARGETPOS
{
	void Init()
	{
		PosNum = 0;
		memset( pos, 0, sizeof(COMPRESSEDPOS)*MAX_CHARTARGETPOSBUF_SIZE );
	}
	void AddTargetPos(VECTOR3 * pPos)
	{
		if( PosNum >= MAX_CHARTARGETPOSBUF_SIZE )
		{
			ASSERT(0);
			return;
		}
		pos[PosNum++].Compress(pPos);
	}
	int GetSize()
	{
		return sizeof(COMPRESSEDTARGETPOS) - sizeof(COMPRESSEDPOS)*(MAX_CHARTARGETPOSBUF_SIZE-PosNum);
	}
	int GetSurplusSize()
	{
		return sizeof(COMPRESSEDPOS)*(MAX_CHARTARGETPOSBUF_SIZE-PosNum);
	}
	BYTE PosNum;
	COMPRESSEDPOS pos[MAX_CHARTARGETPOSBUF_SIZE];
};
struct SEND_MOVEINFO
{
	COMPRESSEDPOS CurPos;
	bool MoveMode;
	WORD KyungGongIdx;
	WORD AbilityKyungGongLevel;
	VECTOR2	Move_Direction;
};
struct BASEMOVE_INFO
{
	VECTOR3 CurPosition;
private:
	BYTE CurTargetPositionIdx;
	BYTE MaxTargetPositionIdx;
	VECTOR3 TargetPositions[MAX_CHARTARGETPOSBUF_SIZE];
public:
	bool bMoving;
	bool MoveMode;
	WORD KyungGongIdx;
	WORD AbilityKyungGongLevel;
	void SetFrom(SEND_MOVEINFO* pInfo)
	{
		bMoving = FALSE;
		pInfo->CurPos.Decompress(&CurPosition);
		MoveMode = pInfo->MoveMode;
		KyungGongIdx = pInfo->KyungGongIdx;
		AbilityKyungGongLevel = pInfo->AbilityKyungGongLevel;
	}
	BYTE GetCurTargetPosIdx() { return CurTargetPositionIdx; }
	BYTE GetMaxTargetPosIdx() { return MaxTargetPositionIdx; }
	BYTE & GetMaxTargetPosIdxRef() { return MaxTargetPositionIdx; }
	void SetCurTargetPosIdx(BYTE idx) { CurTargetPositionIdx=idx; }
	void SetMaxTargetPosIdx(BYTE idx) { MaxTargetPositionIdx=idx; }
	VECTOR3 * GetTargetPosition(BYTE idx) { return &TargetPositions[idx]; }
	VECTOR3 * GetTargetPositionArray() { return TargetPositions; }
	void InitTargetPosition()
	{
		CurTargetPositionIdx = 0;
		MaxTargetPositionIdx = 0;
		memset( TargetPositions, 0, sizeof( VECTOR3 ) * MAX_CHARTARGETPOSBUF_SIZE );
	}
	void SetTargetPosition( VECTOR3* pos )	{ memcpy( TargetPositions, pos, sizeof(VECTOR3)*MAX_CHARTARGETPOSBUF_SIZE );	}
	void SetTargetPosition( BYTE idx, VECTOR3 pos )	{ TargetPositions[idx] = pos;	}
};
struct MOVE_INFO : public BASEMOVE_INFO
{
	BOOL m_bLastMoving;
	BOOL m_bEffectMoving;
	DWORD m_dwMoveCalculateTime;
	BOOL m_bMoveCalculateTime;
	DWORD Move_LastCalcTime;
	DWORD Move_StartTime;
	VECTOR3 Move_StartPosition;
	VECTOR3 Move_Direction;
	float Move_EstimateMoveTime;
};
struct STATE_INFO
{
	int State_Start_Motion;
	int State_Ing_Motion;
	int State_End_Motion;
	DWORD State_End_MotionTime;
	DWORD State_End_Time;
	DWORD State_Start_Time;
	BYTE BeforeState;
	BYTE bEndState;
};
struct HERO_TOTALINFO
{
	WORD	wGenGol;
	WORD	wMinChub;
	WORD	wCheRyuk;
	WORD	wSimMek;
	DWORD	wNaeRyuk;
	DWORD	wMaxNaeRyuk;
	FAMETYPE	Fame;
	WORD	wKarma;
	EXPTYPE	ExpPoint;
	DWORD	MaxMussangPoint;
	LEVELTYPE	LevelUpPoint;
	MONEYTYPE	Money;
	DWORD	PartyID;
	WORD	KyungGongGrade;
	DWORD	AbilityExp;
	DWORD	Playtime;
	DWORD	LastPKModeEndTime;
	LEVELTYPE MaxLevel;	
	char	MunpaCanEntryDate[11];
	BYTE	ExpFlag;
	DWORD   MallMoney;                    //扩展信息定义泡点
	DWORD   GoldMoney;                    //扩展信息定义元宝
	WORD	ReSet;
	WORD	ImageName;
	DWORD   dwKillPlayerTimes;
	WORD    VipLevel;                     //扩展信息定义VIP等级
};
struct PET_TOTALINFO
{
	DWORD	PetMasterUserID;
	DWORD	PetDBIdx;
	DWORD	PetSummonItemDBIdx;
	WORD	PetKind;
	WORD	PetGrade;
	DWORD	PetFriendly;
	DWORD	PetStamina;
	BYTE	bAlive;
	BYTE	bSummonning;
	BYTE	bRest;
};
struct TITAN_TOTALINFO
{
	TITAN_TOTALINFO() {	memset(this,0,sizeof(TITAN_TOTALINFO));	}
	DWORD	TitanMasterUserID;
	DWORD	TitanDBIdx;		
	DWORD	TitanCallItemDBIdx;
	WORD	TitanKind;
	WORD	TitanGrade;
	DWORD	Fuel;
	DWORD	Spell;
	DWORD	RecallTime;	
	DWORD	RegisterCharacterIdx;
	BOOL	bRiding;
	WORD	Scale;
	DWORD	MaintainTime;
};
struct TITAN_ENDURANCE_ITEMINFO
{
	TITAN_ENDURANCE_ITEMINFO() { memset(this, 0, sizeof(TITAN_ENDURANCE_ITEMINFO)); }
	DWORD	CharacterID;
	DWORD	ItemDBIdx;
	DWORD	ItemIdx;
	DWORD	Endurance;
	DWORD	UserIdx;
};
struct TITAN_APPEARANCEINFO
{
	WORD	TitanKind;
	WORD	TitanGrade;
	WORD	TitanScale;
	WORD	WearedItemIdx[eTitanWearedItem_Max];
};
struct SEND_TITAN_APPEARANCEINFO: public MSGBASE
{
	DWORD OwnerID;
	TITAN_APPEARANCEINFO titanAppearanceInfo;
};
struct MONSTER_TOTALINFO
{
	DWORD Life;
	DWORD Shield;
	WORD MonsterKind;
	WORD Group;	
	MAPTYPE MapNum;
};
struct NPC_TOTALINFO
{
	WORD NpcKind;
	WORD Group;
	MAPTYPE MapNum;
	WORD NpcUniqueIdx;
	WORD NpcJob;
};
struct STATIC_NPCINFO
{
	MAPTYPE	MapNum;
	WORD	wNpcJob;
	WORD	wNpcUniqueIdx;
	VECTOR3	vPos;
	char NpcName[MAX_NAME_LENGTH+1];
};
struct QUESTNPCINFO
{
	DWORD	dwObjIdx;
	MAPTYPE	wMapNum;
	WORD	wNpcKind;
	char	sName[MAX_NAME_LENGTH+1];
	WORD	wNpcIdx;
	VECTOR3	vPos;
	float	fDir;	
};
struct ICONBASE
{
	DWORD	dwDBIdx;
	WORD	wIconIdx;
	POSTYPE Position;
};
struct QUICKBASE
{
	POSTYPE Position;
};
class ITEM_OPTION_INFO
{
public:
	DWORD	dwOptionIdx;
	DWORD	dwItemDBIdx;
	WORD	GenGol;
	WORD	MinChub;
	WORD	CheRyuk;
	WORD	SimMek;
	DWORD	Life;
	WORD	NaeRyuk;
	DWORD	Shield;
	ATTRIBUTEREGIST AttrRegist;	
	WORD	PhyAttack;
	WORD	CriticalPercent;
	ATTRIBUTEATTACK AttrAttack;
	WORD	PhyDefense;
	inline DWORD GetOptionType();
	inline float GetOptionValue( DWORD Type );
};
inline DWORD ITEM_OPTION_INFO::GetOptionType()
{
	DWORD type = 0;
	if( GenGol )
	{
		if( type )			return 0;
		type = eRJP_GENGOL;
	}
	if( MinChub )
	{
		if( type )			return 0;
		type = eRJP_MINCHUB;
	}
	if( CheRyuk )
	{
		if( type )			return 0;
		type = eRJP_CHERYUK;
	}
	if( SimMek )
	{
		if( type )			return 0;
		type = eRJP_SIMMEK;
	}
	if( Life )
	{
		if( type )			return 0;
		type = eRJP_LIFE;
	}
	if( NaeRyuk )
	{
		if( type )			return 0;
		type = eRJP_NAERYUK;
	}
	if( Shield )
	{
		if( type )			return 0;
		type = eRJP_SHIELD;
	}
	if( PhyAttack )
	{
		if( type )			return 0;
		type = eRJP_PHYATTACK;
	}
	if( CriticalPercent )
	{
		if( type )			return 0;
		type = eRJP_CRITICAL;
	}
	if( PhyDefense )
	{
		if( type )			return 0;
		type = eRJP_PHYDEFENSE;
	}
	int i=0;
	for(i=0; i<ATTR_MAX; ++i)
		if( AttrRegist.GetElement_Val( ATTR_FIRE+i ) )
		{
			if( type )			return 0;
			type = eRJP_FIREREGIST+i;
		}
	for(i=0; i<ATTR_MAX; ++i)
		if( AttrAttack.GetElement_Val( ATTR_FIRE+i ) )
		{
			if( type )			return 0;
			type = eRJP_FIREATTACK+i;
		}
	return type;
}
inline float ITEM_OPTION_INFO::GetOptionValue( DWORD Type )
{
	float value = 0;
	switch( Type )
	{
	case eRJP_GENGOL:
		value = GenGol;
		break;
	case eRJP_MINCHUB:
		value = MinChub;
		break;
	case eRJP_CHERYUK:
		value = CheRyuk;
		break;
	case eRJP_SIMMEK:
		value = SimMek;
		break;
	case eRJP_LIFE:
		value = (float)Life;
		break;
	case eRJP_NAERYUK:
		value = NaeRyuk;
		break;
	case eRJP_SHIELD:
		value = (float)Shield;
		break;
	case eRJP_FIREREGIST:
		value = AttrRegist.GetElement_Val( ATTR_FIRE );
		break;
	case eRJP_WATERREGIST:
		value = AttrRegist.GetElement_Val( ATTR_WATER );
		break;
	case eRJP_TREEREGIST:
		value = AttrRegist.GetElement_Val( ATTR_TREE );
		break;
	case eRJP_GOLDREGIST:
		value = AttrRegist.GetElement_Val( ATTR_IRON );
		break;
	case eRJP_EARTHREGIST:
		value = AttrRegist.GetElement_Val( ATTR_EARTH );
		break;
	case eRJP_PHYATTACK:
		value = PhyAttack;
		break;
	case eRJP_CRITICAL:
		value = CriticalPercent;
		break;
	case eRJP_FIREATTACK:
		value = AttrAttack.GetElement_Val( ATTR_FIRE );
		break;
	case eRJP_WATERATTACK:
		value = AttrAttack.GetElement_Val( ATTR_WATER );
		break;
	case eRJP_TREEATTACK:
		value = AttrAttack.GetElement_Val( ATTR_TREE );
		break;
	case eRJP_GOLDATTACK:
		value = AttrAttack.GetElement_Val( ATTR_IRON );
		break;
	case eRJP_EARTHATTACK:
		value = AttrAttack.GetElement_Val( ATTR_EARTH );
		break;
	case eRJP_PHYDEFENSE:
		value = PhyDefense;
		break;
	}
	return value;
}
struct ITEM_STONE_OPTION_INFO
{
  	ITEM_STONE_OPTION_INFO() { memset(this, 0, sizeof(ITEM_STONE_OPTION_INFO)); }
	DWORD	dwStoneOptionIdx;
	DWORD	dwItemDBIdx;
	DWORD   dwItemStone0;
	DWORD   dwItemStone1;
	DWORD   dwItemStone2;
	DWORD   dwItemStone3;
	DWORD   dwItemStone4;
};
struct GAMECHECK_LIST
{
	GAMECHECK_LIST(){memset(this,0,sizeof(GAMECHECK_LIST));}
	WORD Id;
	char ProcessName[MAX_PATH];
	BOOL IsCheck;
	LONG CCRC;
};
struct MSG_GAMECHECK_LIST : public MSGBASE
{
	int Count;
	GAMECHECK_LIST  m_GameCheckList[30];
	MSG_GAMECHECK_LIST()
	{
		Count=0;
		memset(m_GameCheckList,0,sizeof(30));
	}
	int GetSize() 
	{
		return sizeof(MSG_GAMECHECK_LIST) - (sizeof(GAMECHECK_LIST) * (30 - Count));
	}
};
struct ITEM_RARE_OPTION_INFO
{
	ITEM_RARE_OPTION_INFO() { memset(this, 0, sizeof(ITEM_RARE_OPTION_INFO)); }
	DWORD	dwRareOptionIdx;
	DWORD	dwItemDBIdx;
	WORD	GenGol;
	WORD	MinChub;
	WORD	CheRyuk;
	WORD	SimMek;
	DWORD	Life;
	WORD	NaeRyuk;
	DWORD	Shield;
	WORD	PhyAttack;
	WORD	PhyDefense;
	ATTRIBUTEREGIST AttrRegist;
	ATTRIBUTEATTACK AttrAttack;
};
struct MUGONGBASE : public ICONBASE
{
	DWORD	ExpPoint;
	BYTE	Sung;
	BYTE	bWear;
	POSTYPE	QuickPosition;
	WORD	OptionIndex;
};
struct MUGONG_TOTALINFO
{
	MUGONGBASE	mugong[SLOT_MUGONGTOTAL_NUM];
};
struct ITEMBASE : public ICONBASE
{
	DURTYPE Durability;
	DWORD RareIdx;
	DWORD StoneIdx;  
	POSTYPE	QuickPosition;
	ITEMPARAM ItemParam;	
	WORD    ItemStatic; 
	DWORD   ItemGrow;
	char PowerUp[MAX_NAME_LENGTH+1];
	char Green[MAX_NAME_LENGTH+1];
	ITEMBASE()
	{
		memset(PowerUp,0,MAX_NAME_LENGTH);
		memset(Green,0,MAX_NAME_LENGTH);
	}
	DWORD	Grade30;
	int     ItemQuality;  //装备品质
	int     ItemEntry1;   //品质属性1
	int     ItemEntry2;   //品质属性2
	int     ItemEntry3;   //品质属性3
};
struct SLOTINFO
{
	BOOL	bLock;
	WORD	wPassword;
	WORD	wState;
};

struct ITEM_RARE_Max_INFO//js
{
	ITEM_RARE_Max_INFO() { memset(this, 0, sizeof(ITEM_RARE_Max_INFO)); }
	char TypeName[MAX_ITEMNAME_LENGTH + 1];
	WORD	ThreeMax;
	WORD	CheRyukMax;
	DWORD	LifeMax;
	WORD	PhyAttackMax;
	WORD	PhyDefenseMax;
	WORD AttrRegistMax;
	WORD AttrAttackMax;

};
struct ITEM_TOTALINFO
{
	ITEMBASE Inventory[SLOT_INVENTORY_NUM];
	ITEMBASE WearedItem[eWearedItem_Max];
	ITEMBASE ShopInventory[TABCELL_SHOPINVEN_NUM+40];
	ITEMBASE PetWearedItem[SLOT_PETWEAR_NUM];
	ITEMBASE TitanWearedItem[SLOT_TITANWEAR_NUM];
	ITEMBASE TitanShopItem[SLOT_TITANSHOPITEM_NUM];
};
struct PET_INFO
{
	PET_TOTALINFO PetInfo[SLOT_INVENTORY_NUM + SLOT_SHOPINVEN_NUM];
};
struct TITAN_INFO
{
	TITAN_TOTALINFO TitanInfo[SLOT_INVENTORY_NUM];
};
struct TITAN_ENDURANCEINFO
{
	TITAN_ENDURANCE_ITEMINFO TitanEnduranceInfo[SLOT_INVENTORY_NUM + SLOT_SHOPINVEN_NUM];
};
struct PYOGUK_ITEM
{
	ITEMBASE Pyoguk[SLOT_PYOGUK_NUM];
};
struct PETINVEN_ITEM
{
	ITEMBASE PetInven[SLOT_PETINVEN_NUM];
};
struct PETACC_ITEM
{
	ITEMBASE PetAcc[SLOT_PETWEAR_NUM];
};
struct SEND_PARTY_GAUGE : public MSGBASE
{
	DWORD PlayerID;
	float GaugePercent;
};
struct PARTY_MEMBER
{
	DWORD dwMemberID;
	BOOL bLogged;
	BYTE LifePercent;
	BYTE ShieldPercent;
	BYTE NaeRyukPercent;
	char Name[MAX_NAME_LENGTH+1];
	LEVELTYPE Level;	
	WORD	posX;
	WORD	posZ;
};
struct PARTY_ADDOPTION : public MSGBASE
{
	DWORD dwPartyIndex;	
	char szTheme[MAX_PARTY_NAME+1];
	LEVELTYPE wMinLevel;	
	LEVELTYPE wMaxLevel;
	BYTE bPublic;	
	WORD wLimitCount;
	BYTE bOption;
};
struct PARTY_MATCHINGLIST_INFO
{
	DWORD dwMemberID[MAX_PARTY_LISTNUM];
	char szMasterName[MAX_NAME_LENGTH+1];
	PARTY_ADDOPTION PartyAddOption;
};
struct PARTY_INFO : public MSGBASE 
{
	DWORD PartyDBIdx;
	BYTE Option;
	PARTY_MEMBER Member[MAX_PARTY_LISTNUM];
	PARTY_ADDOPTION PartyAddOption;
};
struct SEND_PARTY_MEMBER_POS : public MSGBASE
{
	DWORD	dwMemberID;
	WORD	posX;
	WORD	posZ;
};
struct SEND_PARTY_MEMBER_INFO : public MSGBASE 
{
	PARTY_MEMBER MemberInfo;
	DWORD PartyID;
};
struct SEND_CHANGE_PARTY_MEMBER_INFO : public MSGBASE
{
	DWORD PartyID;
	char Name[MAX_NAME_LENGTH+1];
	LEVELTYPE Level;
};
struct MAPSELECT_PARAM
{
	char Name[MAX_NAME_LENGTH+1];
	BYTE	JobType;
	BYTE	SexType;
	BYTE	BodyType;
	BYTE	HairType;
	BYTE	FaceType;
	BYTE	StartArea;
	BOOL	bDuplCheck;
};
struct CHARACTERMAKEINFO : public MSGBASE
{
	char Name[MAX_NAME_LENGTH+1];
	DWORD	UserID;
	BYTE	SexType;
	BYTE	BodyType;
	BYTE	HairType;
	BYTE	FaceType;
	BYTE	StartArea;
	BOOL	bDuplCheck;
	WORD	WearedItemIdx[eWearedItem_Max];
	BYTE    StandingArrayNum;
	float	Height;
	float	Width;
#ifdef _JAPAN_LOCAL_
	WORD	wCharAttr;
#endif
};
struct CALCCHARACTER
{
	WORD wAttack;
	WORD wDefense;
	WORD wHit;
	WORD wDisengage;
	WORD wMove;
	WORD wLife;
	WORD wInner;
	WORD wHwa;
	WORD wSu;
	WORD wMok;
	WORD wKum;
	WORD wTo;
	WORD wDok;
};
struct AUCTION_BOARDTITLE
{
	DWORD BidIdx;
	WORD ItemIdx;
	WORD ItemNum;
	DWORD EndDate;
	DWORD EndTime;
	DWORD CurrentPrice;
	DWORD ImmediatePrice;
};
struct stWeatherTime
{
	WORD	Index;
	WORD	StartDay;
	WORD	StartHour;
	WORD	StartMinute;
	WORD	EndDay;
	WORD	EndHour;
	WORD	EndMinute;
	WORD	State;
};
struct MSG_QUERY_EQUIPINF : public MSGBASE
{
	ITEMBASE WearedItem[eWearedItem_Max];
	ITEM_RARE_OPTION_INFO WearedItemRateInfo[eWearedItem_Max];
	ITEM_OPTION_INFO  WearedItemOptionInfo[eWearedItem_Max];
};
struct MSG_WEATHER : public MSGBASE
{
	WORD MapNum;
	stWeatherTime WeatherTime;
};
struct MSG_INT : public MSGBASE
{
	int nData;
};
struct MSG_INT2 : public MSGBASE
{
	int nData1;
	int nData2;
};
struct MSG_DWORD : public MSGBASE
{
	DWORD dwData;
};
struct MSG_DWORD2 : public MSGBASE
{
	DWORD dwData1;
	INT64 dwData2;
};
struct MSG_DWORD_CHAR :public MSGBASE
{
	DWORD dwData;
	char  CharMsg[128];
};
struct MSG_CLIENT_MSG :public MSGBASE
{
	char Msg[MAX_CHAT_LENGTH+1];
	int GetMsgLength() { return sizeof(MSG_CLIENT_MSG) - MAX_CHAT_LENGTH + strlen(Msg); }
};
struct MSG_DWORDBYTE : public MSGBASE
{
	DWORD dwData;
	BYTE bData;
};
struct MSG_DWORD2BYTE : public MSGBASE
{
	DWORD dwData1;
	DWORD dwData2;
	BYTE bData;
};
struct MSG_DWORD3 : public MSGBASE
{
	DWORD dwData1;
	DWORD dwData2;
	DWORD dwData3;
};
struct MSG_DWORD4 : public MSGBASE
{
	DWORD dwData1;
	DWORD dwData2;
	DWORD dwData3;
	DWORD dwData4;
};
struct MSG_DWORD5 : public MSGBASE
{
	DWORD dwData1;
	DWORD dwData2;
	DWORD dwData3;
	DWORD dwData4;
	DWORD dwData5;
};
struct MSG_DWORD6 : public MSGBASE
{
	DWORD dwData0;
	DWORD dwData1;
	DWORD dwData2;
	DWORD dwData3;
	DWORD dwData4;
	DWORD dwData5;
};
struct MSG_DWORD7 : public MSGBASE
{
	DWORD dwData0;
	DWORD dwData1;
	DWORD dwData2;
	DWORD dwData3;
	DWORD dwData4;
	DWORD dwData5;
	DWORD dwData6;
};
struct MSG_DWORD7_SKIN : public MSGBASE
{
	DWORD* dwSkinData = NULL;
};
struct MSG_DWORD3BYTE2 : public MSGBASE
{
	DWORD	dwData1;
	DWORD	dwData2;
	DWORD	dwData3;
	BYTE	bData1;
	BYTE	bData2;
	char	cData1[33];
	char	cMac[18];
};
struct MSG_WORD : public MSGBASE
{
	WORD wData;
};
struct MSG_WORD2 : public MSGBASE
{
	WORD wData1;
	WORD wData2;
};
struct MSG_WORD3 : public MSGBASE
{
	WORD wData1;
	WORD wData2;
	WORD wData3;
};
struct MSG_WORD4 : public MSGBASE
{
	WORD wData1;
	WORD wData2;
	WORD wData3;
	WORD wData4;
};
struct MSG_DWORD_WORD : public MSGBASE
{
	DWORD dwData;
	WORD wData;
};
struct MSG_DWORD_WORD2 : public MSGBASE
{
	DWORD dwData;
	WORD wData1;
	WORD wData2;
};

struct MSG_DWORD_WORD3 : public MSGBASE//new dungeon
{
	DWORD dwData;
	WORD wData1;
	WORD wData2;
	WORD wData3;
};

struct MSG_BYTE : public MSGBASE
{
	BYTE bData;
};
struct MSG_TITAN_RIDEIN : public MSGBASE
{
	DWORD OwnerID;
	DWORD MaintainTime;	
	TITAN_TOTALINFO			TitanTotalInfo;
	TITAN_APPEARANCEINFO	AppearanceInfo;
};
struct MSG_GUILDNOTICE : public MSGBASE
{
	DWORD dwGuildId;
	char Msg[MAX_GUILD_NOTICE+1];
	int GetMsgLength() { return sizeof(MSG_GUILDNOTICE) - MAX_GUILD_NOTICE + strlen(Msg); }
};
struct TESTMSG : public MSGBASE
{
	char Msg[MAX_CHAT_LENGTH+1];
	int GetMsgLength() { return sizeof(TESTMSG) - MAX_CHAT_LENGTH + strlen(Msg); }
};
struct TESTMSGID : public MSGBASE
{
	DWORD dwSenderID;
	char Msg[MAX_CHAT_LENGTH+1];
	int GetMsgLength() { return sizeof(TESTMSGID) - MAX_CHAT_LENGTH + strlen(Msg); }
};
struct MSG_EVENT_MONSTERREGEN : public MSGBASE
{
	WORD	MonsterKind;
	BYTE	cbMobCount;	
	WORD	wMap;	
	BYTE	cbChannel;
	VECTOR3 Pos;
	WORD	wRadius;
	WORD	ItemID;
	DWORD	dwDropRatio;
};
struct MSG_ADDPOINT:public MSGBASE      
{
	int GenGol;
	int MinChub;
	int CheRyuk;
	int SimMek;
	int LevelUpPoint;
	DWORD m_dwDBIdx;
};
struct MSG_FAME : public MSGBASE
{
	FAMETYPE Fame;
};
struct MSG_KILLCOUNT : public MSGBASE
{
	DWORD KillCount;
};
struct MSG_LOGIN_SYN : public MSGBASE
{
	DWORD AuthKey;
	char id[MAX_NAME_LENGTH+1];
	char pw[MAX_NAME_LENGTH+1];
	char pn[MAX_NAME_LENGTH+1];
	char Mac[18];
	char Version[256];
	char IP[18];
	WORD ServerNum;             
	BYTE gate;
	MSG_LOGIN_SYN()
	{
		AuthKey = 0;
		memset(&id,0,MAX_NAME_LENGTH+1);
		memset(&pw,0,MAX_NAME_LENGTH+1);
		memset(&pn,0,MAX_NAME_LENGTH+1);
		memset(&Version, 0, 256);
		memset(&Mac,0,33);
		memset(&IP,0,18);
		gate=0;
	}
};
struct MSG_LOGIN_SYN_BUDDY : public MSG_LOGIN_SYN
{
    char AuthCode[256];
};
struct MSG_USE_DYNAMIC_SYN : public MSGBASE
{
	char id[MAX_NAME_LENGTH+1];
};
struct MSG_USE_DYNAMIC_ACK : public MSGBASE
{
	char mat[32];
};
struct MSG_LOGIN_DYNAMIC_SYN : public MSGBASE
{
	DWORD AuthKey;
	char id[MAX_NAME_LENGTH+1];
	char pw[MAX_NAME_LENGTH+1];
	char Version[16];
};
struct MSG_LOGIN_ACK : public MSGBASE
{
	char agentip[16];
	WORD agentport;
	DWORD userIdx;
	BYTE cbUserLevel;
};
struct MOVE_ONETARGETPOS : public MSGBASE
{
	DWORD			dwMoverID;
	COMPRESSEDPOS	sPos;
	COMPRESSEDPOS	tPos;
	BYTE	bUseAutoPath;
	void SetStartPos( VECTOR3 * pos )
	{
		sPos.Compress(pos);
		bUseAutoPath=0;
	}
	void SetTargetPos( VECTOR3* pos )
	{
		tPos.Compress( pos );
	}
	void GetStartPos( VECTOR3 * pos )
	{
		sPos.Decompress( pos );
	}
	void GetTargetInfo(MOVE_INFO * pMoveInfo)
	{
		pMoveInfo->SetCurTargetPosIdx(0);
		pMoveInfo->SetMaxTargetPosIdx(1);
		tPos.Decompress(pMoveInfo->GetTargetPosition(0));
	}
	int GetSize()
	{
		return sizeof(MOVE_ONETARGETPOS);
	}
};
struct MOVE_ONETARGETPOS_FROMSERVER : public MSGBASE
{
	COMPRESSEDPOS	sPos;
	COMPRESSEDPOS	tPos;
	void SetStartPos( VECTOR3 * pos )
	{
		sPos.Compress(pos);
	}
	void GetStartPos( VECTOR3 * pos )
	{
		sPos.Decompress( pos );
	}
	void SetTargetPos( VECTOR3* pos )
	{
		tPos.Compress( pos );
	}
	void GetTargetInfo(MOVE_INFO * pMoveInfo)
	{
		pMoveInfo->SetCurTargetPosIdx(0);
		pMoveInfo->SetMaxTargetPosIdx(1);
		tPos.Decompress(pMoveInfo->GetTargetPosition(0));
	}
	int GetSize()
	{
		return sizeof(MOVE_ONETARGETPOS_FROMSERVER);
	}
};
struct MOVE_TARGETPOS : public MSGBASE
{
	DWORD dwMoverID;
	COMPRESSEDPOS spos;
	COMPRESSEDTARGETPOS tpos;
	void Init()
	{
		tpos.Init();
	}
	void SetStartPos(VECTOR3 * pos)
	{
		spos.Compress(pos);
	}
	void AddTargetPos(VECTOR3 * pos)
	{
		tpos.AddTargetPos(pos);
	}
	void GetStartPos(VECTOR3 * pos)
	{
		spos.Decompress(pos);
	}
	void GetTargetInfo(MOVE_INFO * pMoveInfo)
	{
		pMoveInfo->SetCurTargetPosIdx(0);
		pMoveInfo->SetMaxTargetPosIdx(tpos.PosNum);
		ASSERT( tpos.PosNum <= MAX_CHARTARGETPOSBUF_SIZE );
		for(int i = 0 ; i < tpos.PosNum ; ++i)
			tpos.pos[i].Decompress(pMoveInfo->GetTargetPosition(i));
	}
	int GetSize()
	{
		return sizeof(MOVE_TARGETPOS) - tpos.GetSurplusSize();
	}
};
struct MOVE_POS : public MSGBASE
{
	DWORD dwMoverID;
	COMPRESSEDPOS cpos;
};
struct MOVE_POS_USER : public MSGBASE
{
	char	Name[MAX_NAME_LENGTH+1];
	COMPRESSEDPOS cpos;
};
struct MSG_WHERE_INFO : public MSGBASE
{
	WORD	wMapNum;		
	BYTE	bChannel;
	COMPRESSEDPOS cpos;
};
struct MSG_ITEM_XQ_DATA : public MSGBASE
{
	DWORD ItemXqStoneIdx;
	int	  ItemXqStonePos;
	DWORD ItemTargetIdx;
	int   ItemTargetPos;
	DWORD ItemXqItemIdx0;
	int   ItemXqItemPos0;
	DWORD ItemXqItemIdx1;
	int   ItemXqItemPos1;
	DWORD ItemXqItemIdx2;
	int   ItemXqItemPos2;
	DWORD ItemXqItemIdx3;
	int   ItemXqItemPos3;
	DWORD ItemXqItemIdx4;
	int   ItemXqItemPos4;
};
struct BASEOBJECT_INFO
{
	BASEOBJECT_INFO()
	{
		BattleID = 0;
		BattleTeam = 0;
		memset(SingleSpecialState, 0, sizeof(bool) * eSingleSpecialState_Max);
	}
	DWORD dwObjectID;
	DWORD dwUserID;
	char ObjectName[MAX_NAME_LENGTH+1];
	DWORD BattleID;
	BYTE BattleTeam;
	BYTE ObjectState;
	bool	SingleSpecialState[eSingleSpecialState_Max];
};
//struct IMAGENAMEINFO
//{
//	int  idx;
//	LONG ImageCount;
//	LONG  Sticker[50];
//	int  Width;
//	int  Height;
//	int  Speed;
//};
//图片称号结构体
struct IMAGENAMEINFO//称号
{
	WORD idx;
	BOOL IsTrends;
	WORD ImageCount;
	WORD Hight;
	VECTOR2 Scaling;
	INT LeftPosition;
};

struct CHARACTER_TOTALINFO
{
	DWORD	Life;						
	DWORD	MaxLife;					
	DWORD	Shield;							
	DWORD	MaxShield;						
	BYTE	Gender;							
	BYTE	FaceType;						
	BYTE	HairType;						
	WORD	WearedItemIdx[eWearedItem_Max];
	BYTE	Stage;							
	LEVELTYPE	Level;						
	MAPTYPE	CurMapNum;						
	MAPTYPE	LoginMapNum;					
	bool	bPeace;							
	WORD	MapChangePoint_Index;			
	WORD	LoginPoint_Index;				
	DWORD	MunpaID;						
	BYTE	PositionInMunpa;				
	MARKNAMETYPE MarkName;					
	bool	bVisible;						
	bool	bPKMode;						
	BOOL	bMussangMode;				
	FAMETYPE	BadFame;					
	float	Height;							
	float	Width;							
	char	NickName[MAX_GUILD_NICKNAME+1]; 
	char	GuildName[MAX_GUILD_NAME+1];	
	char	MarryName[MAX_NAME_LENGTH + 1];    
	char    ShiTuName[MAX_NAME_LENGTH +	1];
	WORD	ImageName;
	DWORD   ShiTuId;
	WORD    FlashNameFlag;      
	char    FlashName[MAX_NAME_LENGTH +1];
	WORD	genGoal;
	WORD	simmak;
	WORD	minchub;
	WORD	cheryuk;
	FAMETYPE	Fame;
	WORD	defenseRate;
	DWORD	critical;
	DWORD	minAttack;
	DWORD	maxAttack;
	DWORD	naeryuk;
	DWORD	dwGuildUnionIdx;					
	char	sGuildUnionName[MAX_GUILD_NAME+1];	
	DWORD	dwGuildUnionMarkIdx;				
	bool	bRestraint;						
	BYTE	EventIndex;						
	bool	bNoAvatarView;
	WORD    ReStage;                     
	int     SafeStatic;
#ifdef _JAPAN_LOCAL_
	int		nMainAttr;				
	int		nSubAttr;				
	char	ExtraInvenSlot;			
	char	ExtraPyogukSlot;	
	char	ExtraMugongSlot;	
#endif
#ifdef _HK_LOCAL_
	char	ExtraInvenSlot;			
	char	ExtraPyogukSlot;		
	char	ExtraMugongSlot;	
#endif
#ifdef _TL_LOCAL_
	char	ExtraInvenSlot;		
	char	ExtraPyogukSlot;	
	char	ExtraMugongSlot;	
#endif
	WORD	TurnStageOne;
	WORD	TurnStageTwo;
	WORD	TurnStageThree;
	bool	bForbidChat;

	DWORD   dwKillPlayerTimes;

	WORD	ExtraMugongSlot;

	CHARACTER_TOTALINFO()
	{
		bVisible = TRUE;
	}
	WORD    VipLevel;                     //扩展信息定义VIP等级
	int     FlgName;//25k的称号标志位
	char	CustomizingName[MAX_NAME_LENGTH + 1];//自定义称号
};
struct SHOPITEMOPTION
{
	WORD		Avatar[eAvatar_Max];
	WORD		Gengol;				
	WORD		Minchub;			
	WORD		Cheryuk;			
	WORD		Simmek;

	int		Life;
	int		Shield;
	int		Naeryuk;

	WORD		AddExp;				
	WORD		AddItemDrop;		
	char		ExpPeneltyPoint;	
	char		MoneyPeneltyPoint;	
	WORD		AddSung;			
	char		NeagongDamage;		
	char		WoigongDamage;		
	char		ComboDamage;		
	char		RecoverRate;	
	WORD		Critical;		
	char		StunByCri;		
	WORD		Decisive;		
	char		ItemMixSuccess;		
	WORD		StatePoint;			
	WORD		UseStatePoint;	
	char		RegistPhys;		
	char		RegistAttr;		
	char		NeaRyukSpend;		
	DWORD		SkillPoint;			
	DWORD		UseSkillPoint;	
	char		ProtectCount;	
	WORD		AddAbility;		
	WORD		AddMugongExp;	
	char		PlustimeExp;	
	char		PlustimeAbil;	
	char		PlustimeNaeruyk;
	BYTE		bKyungGong;		
	BYTE		KyungGongSpeed;		
	BYTE		EquipLevelFree;	
	BYTE		ReinforceAmp;		
	BYTE		bStreetStall;
	WORD		wSkinItem[eSkinItem_Max];	
	DWORD		dwStreetStallDecoration;
};
struct AVATARITEMOPTION
{
	int		Life;
	int		Shield;
	int		Naeruyk;

	BYTE		Attack;			
	BYTE		Critical;		
	BYTE		Decisive;		

	int		Gengol;
	int		Minchub;
	int		Cheryuk;
	int		Simmek;

	WORD		CounterPercent;		
	WORD		CounterDamage;		
	BYTE		bKyungGong;			
	BYTE		NeaRyukSpend;		
	WORD		NeagongDamage;		
	WORD		WoigongDamage;		
	WORD		TargetPhyDefDown;	
	WORD		TargetAttrDefDown;	
	WORD		TargetAtkDown;		
	WORD		RecoverRate;		
	WORD		KyunggongSpeed;		
	WORD		MussangCharge;		
	BYTE		NaeruykspendbyKG;	
    WORD		ShieldRecoverRate;	
	BYTE		MussangDamage;		

	// 天墨技术团 PVP 相关属性
	float PVPCri;      // PVP 暴击
	float PVPAttack;   // PVP 伤害加成
	float PVPDef;      // PVP 物理防御
	float PVPADef;     // PVP 属性防御
	float PVPADodge;   // PVP 闪避
	float PVPHit;      // PVP 命中

	//// 新增晕眩抗性 & 晕眩时间减少
	float PVPStunResist;     // PVP 眩晕抗性（0.2 = 20% 免疫眩晕）
	float PVPStunTimeReduce; // PVP 眩晕时间减少（0.01 = 1% 缩短时间）
};
struct SEND_MONSTER_TOTALINFO	:	public MSGBASE
{
	BASEOBJECT_INFO BaseObjectInfo;
	MONSTER_TOTALINFO TotalInfo;
	SEND_MOVEINFO MoveInfo;
	BYTE bLogin;
	CAddableInfoList AddableInfo;
	WORD GetMsgLength()	{	return sizeof(SEND_MONSTER_TOTALINFO) - sizeof(CAddableInfoList) + AddableInfo.GetInfoLength();	}
};
struct SEND_NPC_TOTALINFO	:	public MSGBASE
{
	BASEOBJECT_INFO BaseObjectInfo;
	NPC_TOTALINFO TotalInfo;
	SEND_MOVEINFO MoveInfo;
	float Angle;
	BYTE bLogin;
	CAddableInfoList AddableInfo;
	WORD GetMsgLength()	{	return sizeof(SEND_NPC_TOTALINFO) - sizeof(CAddableInfoList) + AddableInfo.GetInfoLength();	}
};
struct SEND_PET_TOTALINFO	:	public MSGBASE
{
	BASEOBJECT_INFO	BaseObjectInfo;
	PET_TOTALINFO	TotalInfo;
	SEND_MOVEINFO	MoveInfo;
	char MasterName[MAX_NAME_LENGTH+1];
	DWORD			MasterID;
	BYTE	bLogin;
	CAddableInfoList	AddableInfo;
	WORD	GetMsgLength() {	return sizeof(SEND_PET_TOTALINFO) - sizeof(CAddableInfoList) + AddableInfo.GetInfoLength();	}
};
struct SEND_CHARACTER_TOTALINFO : public MSGBASE
{
	BASEOBJECT_INFO BaseObjectInfo;
	CHARACTER_TOTALINFO TotalInfo;	
	SEND_MOVEINFO MoveInfo;
	SHOPITEMOPTION	ShopItemOption;
	BOOL	bInTitan;
	BYTE bLogin;
	CAddableInfoList AddableInfo;
	WORD GetMsgLength()	{	return sizeof(SEND_CHARACTER_TOTALINFO) - sizeof(CAddableInfoList) + AddableInfo.GetInfoLength();	}
};
struct ABILITY_TOTALINFO
{
	char AbilityDataArray[eAGK_Max][MAX_ABILITY_NUM_PER_GROUP+1];
	char AbilityQuickPositionArray[eAGK_Max][MAX_ABILITY_NUM_PER_GROUP+1];
};
struct SEND_HERO_TOTALINFO : public MSGBASE
{
	BASEOBJECT_INFO BaseObjectInfo;
	CHARACTER_TOTALINFO ChrTotalInfo;
	HERO_TOTALINFO HeroTotalInfo;
	SEND_MOVEINFO SendMoveInfo;
	DWORD UniqueIDinAgent;
	SHOPITEMOPTION	ShopItemOption;
	MUGONG_TOTALINFO MugongTotalInfo;
	ABILITY_TOTALINFO AbilityInfo;
	ITEM_TOTALINFO ItemTotalInfo;
	WORD	OptionNum;
	WORD	RareOptionNum;
	WORD	StoneOptionNum;   
	WORD	PetNum;
	WORD	TitanNum;
	WORD	TitanEndrncNum;
	SYSTEMTIME		ServerTime;			
	CAddableInfoList AddableInfo;
	WORD GetMsgLength()
	{
		return sizeof( SEND_HERO_TOTALINFO ) - sizeof(CAddableInfoList) + AddableInfo.GetInfoLength();
	}
};
struct SEND_CHARSELECT_INFO : public MSGBASE
{
#ifdef _CRYPTCHECK_ 
	HselInit eninit;
	HselInit deinit;
#endif
	int CharNum;
	WORD StandingArrayNum[MAX_CHARACTER_NUM];			
	BASEOBJECT_INFO BaseObjectInfo[MAX_CHARACTER_NUM];
	CHARACTER_TOTALINFO ChrTotalInfo[MAX_CHARACTER_NUM];
};
struct SEND_ADDPET_FROMITEM : public MSGBASE
{
	PET_TOTALINFO Info;
};
struct SEND_ADDTITAN_FROMITEM : public MSGBASE
{
	TITAN_TOTALINFO Info;
};
struct SEND_ADDTITANEQUIP_FROMITEM : public MSGBASE
{
	TITAN_ENDURANCE_ITEMINFO Info;
};
struct SEND_TITANSTATINFO : public MSGBASE
{
	titan_calc_stats Info;
};
struct MSG_NAME : public MSGBASE
{
	char Name[MAX_NAME_LENGTH+1];
};
struct MSG_NAME2 : public MSGBASE
{
	char str1[MAX_NAME_LENGTH+1];
	char str2[MAX_NAME_LENGTH+1];
};
struct MSG_NAME_DWORD : public MSGBASE
{
	char Name[MAX_NAME_LENGTH+1];
	DWORD dwData;
};
struct MSG_NAME_WORD : public MSGBASE
{
	char Name[MAX_NAME_LENGTH+1];
	WORD wData;
};
struct MSG_NAME_DWORD2 : public MSGBASE
{
	char Name[MAX_NAME_LENGTH+1];
	DWORD dwData1;
	DWORD dwData2;
};
struct MSG_NAME_DWORD3 : public MSGBASE
{
	char	Name[MAX_NAME_LENGTH+1];
	DWORD	dwData1;
	DWORD	dwData2;
	DWORD	dwData3;
};
struct MSG_NAME2_DWORD : public MSGBASE
{
	char	Name1[MAX_NAME_LENGTH+1];
	char	Name2[MAX_NAME_LENGTH+1];
	DWORD	dwData;
};
struct SEND_PACKED_DATA : public MSGBASE
{
	WORD wRealDataSize;
	WORD wReceiverNum;
	char Data[MAX_PACKEDDATA_SIZE];
};
struct SEND_PACKED_TOMAPSERVER_DATA : public MSGBASE
{
	MAPTYPE ToMapNum;
	WORD wRealDataSize;
	char Data[MAX_PACKEDDATA_SIZE];
	WORD GetMsgLength()
	{
		return sizeof(SEND_PACKED_TOMAPSERVER_DATA) - MAX_PACKEDDATA_SIZE + wRealDataSize;
	}
};
struct MSG_ITEM : public MSGBASE
{
	ITEMBASE ItemInfo;
};
struct MSG_MARRY_ADDMSG : public MSGBASE
{
	DWORD PlayerID;
	char Name[MAX_NAME_LENGTH+1];
};
struct TOPINFO 
{
	int     Key;
	int     ObjectId;
	int		Idx;
	CHAR	szName1[MAX_NAME_LENGTH + 1];
	CHAR	szName2[MAX_NAME_LENGTH + 1];
	int		Change;
	int     Stage;
	DWORD	nData1;
	DWORD	nData2;
	TOPINFO()
	{
		Key=0;
		ObjectId=0;
		Idx=0;
		Change=0;
		Stage= 0;
		nData1=0;
		nData2=0;
		memset(szName1,0,MAX_NAME_LENGTH);
		memset(szName2,0,MAX_NAME_LENGTH);
	}
};
struct SEND_TOPLIST : public MSGBASE
{
	WORD	wCount;
	WORD	nListType;
	char    updateTime[64];
	TOPINFO TopInfo[MAX_TOP_NUM];
	SEND_TOPLIST()
	{
		wCount = 0;
		nListType=0;
		memset(TopInfo, 0, MAX_TOP_NUM);
		sprintf(updateTime,"%s","2018-01-01"); 
	}
	int GetSize() {
		return (sizeof(SEND_TOPLIST)-sizeof(TOPINFO)*(MAX_TOP_NUM-wCount));
	}
};
struct ITEM_LUCKER_INFO
{
	WORD Idx;
	DWORD ItemIdx;
	int	  ItemRate;
	BOOL  ItemMsg;
};
struct ITEM_GROW_SET
{
   DWORD GrowRate;
   DWORD Life;				
   DWORD Shield;			
   DWORD NaeRyuk;	
   float GenGol;			
   float MinChub;			
   float CheRyuk;			
   float SimMek;
   float PhyDef;
};
struct ITEM_LUCKER_RATE
{
	int Idx;
	int ItemOldRate;
	int ItemNewRate;
};
struct MSG_TIMER : public MSGBASE 
{
  DWORD ChatMsgId1;  
  DWORD ChatMsgId2;  
};
struct MSG_ITEM_WITH_OPTION : public MSGBASE
{
	BOOL IsOptionItem;
	BOOL IsRareOptionItem;
	ITEMBASE ItemInfo;
	ITEM_OPTION_INFO OptionInfo;
	ITEM_RARE_OPTION_INFO RareOptionInfo;
	MSG_ITEM_WITH_OPTION() : IsOptionItem(0), IsRareOptionItem(0) {}
	WORD GetMsgLength()
	{
		if(!IsRareOptionItem)
		{
			return sizeof(MSG_ITEM_WITH_OPTION) - sizeof(ITEM_OPTION_INFO)*(!IsOptionItem);
		}
		else
		{
			return sizeof(MSG_ITEM_WITH_OPTION);
		}
	}
};
struct MSG_GT_EVENTSTART : public MSGBASE
{
	char GuildName1[MAX_GUILD_NAME+1];
	char GuildName2[MAX_GUILD_NAME+1];
	DWORD SenderID;
	MSG_GT_EVENTSTART()
	{
		memset( GuildName1, 0, sizeof(char)*(MAX_GUILD_NAME+1) );
		memset( GuildName2, 0, sizeof(char)*(MAX_GUILD_NAME+1) );
		SenderID = 0;
	}
};
struct ITEMOBTAINARRAY : public MSGBASE
{
	ITEMOBTAINARRAY()
	{
		ItemNum = 0;
		dwLootID = 0;
	}
	int BuyType;
	void Init( WORD cg, WORD pt, DWORD dwID ,int buytype=0)
	{
		Category	= cg;
		Protocol	= pt;
		dwObjectID	= dwID;
		BuyType = buytype;
	}
	void AddItem( DWORD DBIdx, WORD ItemIdx, DURTYPE Durability, POSTYPE bPosition, POSTYPE QuickPosition, ITEMPARAM Param, DWORD RareIdx = 0,WORD ItemStatic=0,DWORD ItemGrow=0, DWORD Grade30 = 0, WORD ItemQuality = 0, WORD ItemEntry1 = 0, WORD ItemEntry2 = 0, WORD ItemEntry3 = 0)//默认0为普通购买
	{
		ItemInfo[ItemNum].dwDBIdx		= DBIdx;
		ItemInfo[ItemNum].wIconIdx		= ItemIdx;
		ItemInfo[ItemNum].Durability	= Durability;
		ItemInfo[ItemNum].Position		= bPosition;
		ItemInfo[ItemNum].QuickPosition	= QuickPosition;
		ItemInfo[ItemNum].ItemParam		= Param;
		ItemInfo[ItemNum].RareIdx		= RareIdx;
		ItemInfo[ItemNum].ItemStatic	=ItemStatic; 
		ItemInfo[ItemNum].ItemGrow		=ItemGrow;
		ItemInfo[ItemNum].Grade30 = Grade30;
		//装备品质
		ItemInfo[ItemNum].ItemQuality = ItemQuality;
		ItemInfo[ItemNum].ItemEntry1 = ItemEntry1;
		ItemInfo[ItemNum].ItemEntry2 = ItemEntry2;
		ItemInfo[ItemNum].ItemEntry3 = ItemEntry3;
		ItemNum++;
	}
	void AddItem(const ITEMBASE *pInfo)//kiv
	{
		memcpy(&ItemInfo[ItemNum], pInfo, sizeof(ITEMBASE));
		ItemNum++;
	}
	void AddItem( WORD idx, ITEMBASE * item)
	{
		memcpy( &ItemInfo[idx], item, sizeof( ITEMBASE ) );
		ItemNum++;
	}
	ITEMBASE * GetItem( WORD idx ) { return &ItemInfo[idx]; }
	int GetSize()
	{
		return ( sizeof( ITEMOBTAINARRAY ) - ( sizeof( ITEMBASE ) * ( SLOT_INVENTORY_NUM - ItemNum ) ) );
	}
	void Clear()
	{
		memset( this, 0, sizeof( ITEMOBTAINARRAY ) );
		ItemNum = 0;
	}
	BYTE		ItemNum;
	WORD		wObtainCount;
	DWORD		dwLootID;
	ITEMBASE	ItemInfo[SLOT_INVENTORY_NUM];
};
typedef void (*DBResult)(void * pPlayer, WORD ArrayID);
typedef void (*DBResultEx)(void* pPlayer, WORD ArrayID, void* pRareOptionInfo);
struct ITEMOBTAINARRAYINFO
{
	DWORD			wObtainArrayID;
	DBResult		CallBack;
	DBResultEx		CallBackEx;
	DWORD			dwFurnisherIdx;
	WORD			wType;
	ITEMOBTAINARRAY	ItemArray;
};
struct MSG_ITEM_ERROR : public MSGBASE
{
	int ECode;
};
struct MSG_ITEM_MOVE_SYN : public MSGBASE
{
	POSTYPE FromPos;
	WORD wFromItemIdx;
	POSTYPE ToPos;
	WORD wToItemIdx;
};
struct MSG_ITEM_MOVE_ACK : public MSG_ITEM_MOVE_SYN
{
};
struct MSG_ITEM_COMBINE_SYN : public MSGBASE
{
	WORD wItemIdx;
	POSTYPE FromPos;
	POSTYPE ToPos;
	DURTYPE FromDur;
	DURTYPE ToDur;
};
struct MSG_ITEM_COMBINE_ACK : public MSG_ITEM_COMBINE_SYN
{
};
struct MSG_ITEM_DIVIDE_SYN : public MSGBASE
{
	WORD wItemIdx;
	POSTYPE FromPos;
	POSTYPE ToPos;
	DURTYPE FromDur;
	DURTYPE ToDur;
	DWORD dwItemDBIdx;
};
struct MSG_ITEM_DISCARD_SYN : public MSGBASE
{
	POSTYPE TargetPos;
	WORD wItemIdx;
	DURTYPE ItemNum;
	DWORD dwItemDBIdx;
};
struct MSG_ITEM_DISCARD_ACK : public MSG_ITEM_DISCARD_SYN
{
};
struct DEMAND_ITEM	
{
	DEMAND_ITEM() : demandItemIdx(0), demandItemPos(0), demandItemDur(0) {}
	WORD demandItemIdx;
	POSTYPE demandItemPos;
	DURTYPE demandItemDur;
};
struct MSG_ITEM_BUY_SYN : public MSGBASE
{
	MSG_ITEM_BUY_SYN() : slotNum(0) {}
	WORD wBuyItemIdx;
	WORD BuyItemNum;
	WORD wDealerIdx;
	DWORD wIdxPos;
	DWORD wBuyType;
	DWORD wLimit;
	DWORD BuyRight;
	DWORD LimitKind;
	DWORD LimitCount;
	WORD slotNum;
	DEMAND_ITEM demandItem[SLOT_INVENTORY_NUM];
	void AddDemandSlot(WORD itemIdx, POSTYPE itemPos, DURTYPE itemDur)
	{
		demandItem[slotNum].demandItemIdx = itemIdx;
		demandItem[slotNum].demandItemPos = itemPos;
		demandItem[slotNum].demandItemDur = itemDur;
		++slotNum;
	}
	int GetSize()
	{
		int length = sizeof(MSG_ITEM_BUY_SYN) - (sizeof(DEMAND_ITEM) * (SLOT_INVENTORY_NUM - slotNum));
		return length;
	}
};
struct MSG_ITEM_SELL_SYN : public MSGBASE
{
	POSTYPE TargetPos;
	WORD	wSellItemIdx;
	WORD	SellItemNum;
	WORD	wDealerIdx;
	DWORD	dwItemDBIdx;
};
struct MSG_ITEM_SELL_ACK : public MSG_ITEM_SELL_SYN
{
};
struct MSG_ITEM_UPGRADE_SYN : public MSGBASE
{
	POSTYPE ItemPos;
	WORD	wItemIdx;
	POSTYPE MaterialItemPos;
	WORD	wMaterialItemIdx;
};
struct MSG_ITEM_UPGRADE_ACK : public MSG_ITEM_UPGRADE_SYN
{
};
struct MATERIAL_ARRAY
{
	WORD	wItemIdx;
	POSTYPE ItemPos;
	DURTYPE Dur;
	DWORD	dwItemDBIdx;
};
struct MSG_ITEM_MIX_SYN : public MSGBASE
{
	WORD	wBasicItemIdx;
	POSTYPE BasicItemPos;
	WORD	wResultItemIdx;
	DWORD	dwRareIdx;
	POSTYPE ResultIndex;
	WORD	ShopItemIdx;
	WORD	ShopItemPos;
	WORD	wMaterialNum;
	MATERIAL_ARRAY Material[MAX_MIX_MATERIAL];
	void Init()
	{
		wMaterialNum = 0;
		ShopItemIdx = 0;
		ShopItemPos = 0;
		dwRareIdx	= 0;
	}
	void AddMaterial(WORD wItemIdx, POSTYPE ItemPos, DURTYPE Dur)
	{
		Material[wMaterialNum].wItemIdx = wItemIdx;
		Material[wMaterialNum].ItemPos = ItemPos;
		Material[wMaterialNum++].Dur = Dur;
	}
	int GetSize()
	{
		return sizeof(MSG_ITEM_MIX_SYN) - (MAX_MIX_MATERIAL - wMaterialNum)*sizeof(MATERIAL_ARRAY);
	}
};
struct MSG_ITEM_MIX_ACK : public MSG_ITEM_MIX_SYN
{
};
struct MSG_ITEM_MIX_RARE_ACK : public MSG_ITEM_MIX_SYN
{
	ITEM_RARE_OPTION_INFO RareOptionInfo;
};
struct MSG_ITEM_USE_SYN : public MSGBASE
{
	POSTYPE TargetPos;
	WORD	wItemIdx;
	DWORD dwItemDBIdx;
};
struct MSG_ITEM_USE_ACK : public MSG_ITEM_USE_SYN
{
};
struct ITEM_JEWEL_POS
{
	WORD	wItemIdx;
	POSTYPE pos;
};
struct ITEM_JEWEL_POS_EX
{
	WORD	wItemIdx;
	POSTYPE pos;
	DURTYPE Dur;
};
struct MSG_ITEM_DISSOLVE_SYN : public MSGBASE
{
	WORD	wTargetItemIdx;
	POSTYPE TargetPos;
	DURTYPE	wOptionIdx;
};
struct MSG_ITEM_REINFORCE_SYN : public MSGBASE
{
	void Init( BYTE cg, BYTE pt, DWORD dwID )
	{
		Category	= cg;
		Protocol	= pt;
		dwObjectID	= dwID;
		wJewelUnit = 0;
	}
	void AddJewelInfo(WORD JewelItemIdx, POSTYPE Pos, DURTYPE Dur )
	{
		JewelWhich[wJewelUnit].wItemIdx = JewelItemIdx;
		JewelWhich[wJewelUnit].pos = Pos;
		JewelWhich[wJewelUnit].Dur = Dur;
		++wJewelUnit;
	}
	int GetSize()
	{
		return sizeof(MSG_ITEM_REINFORCE_SYN) - (MAX_ITEM_OPTION_NUM - wJewelUnit)*sizeof(ITEM_JEWEL_POS_EX);
	}
	WORD	wTargetItemIdx;
	POSTYPE TargetPos;
	WORD	wJewelUnit;
	ITEM_JEWEL_POS_EX	JewelWhich[MAX_ITEM_OPTION_NUM];
};
struct MSG_ITEM_REINFORCE_WITHSHOPITEM_SYN : public MSGBASE
{
	void Init( BYTE cg, BYTE pt, DWORD dwID )
	{
		Category	= cg;
		Protocol	= pt;
		dwObjectID	= dwID;
		wJewelUnit = 0;
	}
	void AddJewelInfo(WORD JewelItemIdx, POSTYPE Pos, DURTYPE Dur )
	{
		JewelWhich[wJewelUnit].wItemIdx = JewelItemIdx;
		JewelWhich[wJewelUnit].pos = Pos;
		JewelWhich[wJewelUnit].Dur = Dur;
		++wJewelUnit;
	}
	int GetSize()
	{
		return sizeof(MSG_ITEM_REINFORCE_WITHSHOPITEM_SYN) - (MAX_ITEM_OPTION_NUM - wJewelUnit)*sizeof(ITEM_JEWEL_POS_EX);
	}
	WORD	wTargetItemIdx;
	POSTYPE TargetPos;
	WORD	wShopItemIdx;
	POSTYPE ShopItemPos;
	WORD	wJewelUnit;
	ITEM_JEWEL_POS_EX	JewelWhich[MAX_ITEM_OPTION_NUM];
};
struct MSG_ITEM_REINFORCE_ACK : public MSGBASE
{
	WORD	wTargetItemIdx;
	POSTYPE TargetPos;
	ITEM_OPTION_INFO OptionInfo;
};
struct MSG_ITEM_RAREITEM_GET : public MSGBASE
{
	WORD		wTargetItemIdx;
	ITEMBASE	RareItemBase;
	ITEM_RARE_OPTION_INFO RareInfo;
};
struct MSG_ITEM_STONE_DATA : public MSGBASE
{
	WORD	wTargetItemIdx;
	DWORD	wTargetDBItemIdx;
	POSTYPE TargetPos;
	ITEM_STONE_OPTION_INFO StoneInfo;
};
struct MSG_QUICK_SET_SYN : public MSGBASE
{
	POSTYPE QuickPos;
	POSTYPE SrcPos;
	WORD	SrcItemIdx;
};
struct MSG_QUICK_SET_ACK : public MSG_QUICK_SET_SYN
{
};
struct MSG_QUICK_ADD_SYN : public MSGBASE
{
	POSTYPE QuickPos;
	POSTYPE SrcPos;
	WORD	SrcItemIdx;
	POSTYPE OldSrcPos;
	WORD	OldSrcItemIdx;
	POSTYPE SrcQuickPos;
	POSTYPE OldSrcQuickPos;
};
struct MSG_QUICK_ADD_ACK : public MSG_QUICK_ADD_SYN
{
};
struct MSG_QUICK_REM_SYN : public MSGBASE
{
	POSTYPE SrcPos;
	POSTYPE SrcItemIdx;
	POSTYPE QuickPos;
	POSTYPE SrcQuickPos;
};
struct MSG_QUICK_REM_ACK : public MSG_QUICK_REM_SYN
{
};
struct MSG_QUICK_MOVE_SYN : public MSGBASE
{
	POSTYPE FromSrcPos;
	WORD wFromSrcItemIdx;
	POSTYPE FromQuickPos;
	POSTYPE ToSrcPos;
	WORD wToSrcItemIdx;
	POSTYPE ToQuickPos;
	POSTYPE FromSrcQuickPos;
	POSTYPE ToSrcQuickPos;
	bool	DeleteToSrcQuickPos;
};
struct MSG_QUICK_MOVE_ACK : public MSG_QUICK_MOVE_SYN
{
};
struct MSG_QUICK_ERROR : public MSGBASE
{
	int ECode;
};
struct MSG_MUGONG_MOVE_SYN : public MSGBASE
{
	POSTYPE FromPos;
	POSTYPE ToPos;
};
struct MSG_MUGONG_MOVE_ACK : public MSG_MUGONG_MOVE_SYN
{
};
struct MSG_MUGONG_REM_SYN : public MSGBASE
{
	WORD	wMugongIdx;
	POSTYPE TargetPos;
};
struct MSG_MUGONG_REM_ACK : public MSG_MUGONG_REM_SYN
{
};
struct MSG_MUGONG_ADD_ACK : public MSGBASE
{
	MUGONGBASE MugongBase;
};
struct MSG_MUGONG_DESTROY : public MSGBASE
{
	WORD		wMugongIdx;
	POSTYPE		TargetPos;
	BYTE		cbReason;	
};
struct MSG_LINKITEM : public MSG_ITEM
{
	POSTYPE		wAbsPosition;
};
struct MSG_LINKITEM_ADD : public MSG_LINKITEM
{
	BYTE				bPetInfo;
	BYTE				bOptionItem;
	BYTE				bRareOptionItem;
	PET_TOTALINFO		sPetTotalInfo;
	ITEM_OPTION_INFO	sOptionInfo;
	ITEM_RARE_OPTION_INFO sRareOptionInfo;
	MSG_LINKITEM_ADD() : bPetInfo(0), bOptionItem(0), bRareOptionItem(0) {}
	int GetSize()
	{
		if(!bRareOptionItem)
		{
			return sizeof(MSG_LINKITEM_ADD) - sizeof(ITEM_OPTION_INFO)*(1-(bOptionItem));
		}
		else
		{
			return sizeof(MSG_LINKITEM_ADD);
		}
	}
};
struct MSG_LINKITEMEX : public MSG_LINKITEM
{
	MONEYTYPE	dwMoney;
	MONEYTYPE   dwGold; 
	WORD		wVolume;
};
struct MSG_LINKITEMOPTIONS : public MSG_LINKITEMEX
{
	MSG_LINKITEMOPTIONS() : eOptionKind(0) {}
	int					eOptionKind;
	CAddableInfoList	AddableInfo;
	WORD	GetSize()
	{
		return sizeof( MSG_LINKITEMOPTIONS ) - sizeof(CAddableInfoList) + AddableInfo.GetInfoLength();
	}
};
struct MSG_LINKBUYITEMEX : public MSG_LINKITEMEX
{
	WORD wVolume;
};
struct MSG_ITEMEX : public MSG_ITEM
{
	BYTE count;
	BYTE RareCount;
	BYTE PetInfoCount;
	PET_TOTALINFO		sPetInfo;
	ITEM_OPTION_INFO	sOptionInfo;
	ITEM_RARE_OPTION_INFO sRareOptionInfo;
	MSG_ITEMEX() {memset(this, 0, sizeof(MSG_ITEMEX));}
	int GetSize()
	{
		if(!RareCount)
		{
			return sizeof(MSG_ITEMEX) - sizeof(ITEM_OPTION_INFO)*(1-(count));
		}
		else
		{
			return sizeof(MSG_ITEMEX);
		}
	}
};
struct MSG_BUYITEM : MSG_LINKITEMOPTIONS
{};
struct MSG_LOOTINGIEM : public MSG_LINKITEM
{
	DWORD				dwDiePlayerIdx;
	BYTE				bOptionItem;
	BYTE				bRareOptionItem;
	BYTE				bPetSummonItem;
	PET_TOTALINFO		sPetInfo;
	ITEM_OPTION_INFO	sOptionInfo;
	ITEM_RARE_OPTION_INFO	sRareOptionInfo;
	MSG_LOOTINGIEM() : bOptionItem(0), bRareOptionItem(0), bPetSummonItem(0) {}
	int GetSize()
	{
		if(!bRareOptionItem)
		{
			return sizeof(MSG_LOOTINGIEM) - sizeof(ITEM_OPTION_INFO)*(!bOptionItem);
		}
		else
		{
			return sizeof(MSG_LOOTINGIEM);
		}
	}
};
struct MSG_LOOTINGITEM : MSG_LINKITEMOPTIONS
{};
struct SEND_LINKITEM_TOTALINFO : public MSG_LINKITEMOPTIONS
{};
struct SEND_LINKBUYITEM_TOTALINFO : public SEND_LINKITEM_TOTALINFO
{
};
struct MSG_EXCHANGE_REMOVEITEM : public MSGBASE
{
	POSTYPE		wAbsPosition;
};
struct MSG_ITEM_DESTROY : public MSGBASE
{
	POSTYPE		wAbsPosition;
	BYTE		cbReason;
};
struct REGIST_MAP : public MSGBASE
{
	WORD mapServerPort;
	MAPTYPE mapnum;
};
struct REGIST_BASEECONOMY
{
	MAPTYPE MapNum;
	WORD OriginNum;
	WORD OriginPrice;
	WORD OriginAmount;
	WORD RequireNum;
	WORD RequirePrice;
	WORD RequireAmount;
	BYTE BuyRates;
	BYTE SellRates;
};
struct AUCTION_LISTFIELD
{
	WORD ItemIdx;
	WORD SellNum;
	WORD SellerID;
	WORD EndDate;
	WORD EndTime;
	DWORD StartPrice;
	DWORD ImmediatePrice;
	char Memo[256];
};
struct AUCTION_ONPAGELIST : public MSGBASE
{
	AUCTION_LISTFIELD Auction_List[10];
};
struct AUCTION_NEW_BOARDCONTENTS : public MSGBASE
{	
	WORD ItemIdx;
	WORD ItemNum;
	DWORD SellerID;
	DWORD EndDate;
	DWORD EndTime;
	DWORD StartPrice;
	DWORD ImmediatePrice;
	WORD MemoLength;
	char Memo[256];
	WORD GetMsgLength()	{	return sizeof(AUCTION_NEW_BOARDCONTENTS) - 256 + MemoLength + 1;	}
};
struct AUCTION_BOARDCONTENTS_INFO : public MSGBASE
{
	DWORD BidIdx;
	WORD ItemIdx;
	WORD ItemNum;
	DWORD SellerID;
	DWORD EndDate;
	DWORD EndTime;
	DWORD StartPrice;
	DWORD ImmediatePrice;
	DWORD CurrentPrice;
	DWORD BidderNum;
	DWORD CurrentBidder;
	WORD MemoLength;
	char Memo[256];
};
struct AUCTION_BOARDPAGE_INFO : public MSGBASE
{
	WORD wCurPage;
	WORD wTotalPageNum;
	AUCTION_BOARDTITLE Title[MAX_BOARDTITLE_NUM];
};
struct MSG_STREETSTALL_TITLE : public MSGBASE
{
	char Title[MAX_STREETSTALL_TITLELEN+1];
	WORD StallKind;
};
struct MSG_STREETSTALL_ITEMSTATUS : public MSG_ITEM
{
	POSTYPE		wAbsPosition;
	__int64		nMoney;//MONEYTYPE		dwMoney;//kiv
	__int64     nGold;//MONEYTYPE       dwGold; //kiv
	WORD	wVolume;
};
struct STREETSTALLITEM
{
	WORD		wVolume;
	WORD		wIconIdx;
	DWORD		dwDBIdx;
	DURTYPE		Durability;
	DWORD		dwRareIdx;
	DWORD		dwStoneIdx; 
	DWORD       dwGrow; 
	MONEYTYPE		money;
	MONEYTYPE       gold ;
	char		Locked;
	char		Fill;
	ITEMPARAM ItemParam;
	DWORD       dwGrade30;
	WORD       ItemQuality;
	WORD       ItemEntry1;
	WORD       ItemEntry2;
	WORD       ItemEntry3;
};
struct STREETSTALL_INFO : public MSGBASE
{
	DWORD StallOwnerID;
	char Title[MAX_STREETSTALL_TITLELEN+1];
	STREETSTALLITEM Item[SLOT_STREETSTALL_NUM];
	WORD StallKind;
	WORD count;
	WORD RareCount;
	WORD StoneCount;
	WORD PetItemCount;
	WORD TitanItemCount;
	WORD TitanEquipItemCount;
	CAddableInfoList AddableInfo;
	WORD GetSize()
	{
		return sizeof( STREETSTALL_INFO ) - sizeof(CAddableInfoList) + AddableInfo.GetInfoLength();
	}
};
struct STREETSTALLTITLE_INFO : public MSGBASE
{
	char Title[MAX_STREETSTALL_TITLELEN+1];
};
struct STREETSTALL_BUYINFO : public MSG_ITEM
{
	DWORD	StallOwnerID;
	POSTYPE StallPos;
	DWORD	dwBuyPrice;
	DWORD	dwBuyGold;
};
struct MSG_REGISTITEM : public MSG_ITEM
{
	DWORD dwData;
};
struct MSG_REGISTITEMEX : public MSG_REGISTITEM
{
	MONEYTYPE dwMoney;
	MONEYTYPE dwGold ;
};
struct MSG_REGISTBUYITEMEX : public MSG_REGISTITEMEX
{
	WORD wVolume;
	WORD wAbsPosition;
};
struct MSG_SELLITEM : public MSG_REGISTITEM
{
	DWORD count;
};
struct MSG_STALLMSG	: public MSGBASE
{
	DWORD	SellerID;
	DWORD	BuyerID;
	WORD	ItemIdx;
	WORD	count;
	DWORD	money;
	DWORD   gold;
	MSG_STALLMSG()//kiv
	{
		SellerID=0;
		BuyerID=0;
		ItemIdx=0;
		count=0;
		money=0;
		gold=0; 
	}
};
struct ITEM_SELLACK_INFO : public MSGBASE
{
	MONEYTYPE Money;
	POSTYPE Pos;
};
struct APPEARANCE_INFO : public MSGBASE
{
	DWORD PlayerID;
	WORD WearedItem[eWearedItem_Max];
};
struct SEARCHLIST : public MSGBASE
{
	SEARCH_TYPE search_type;
	SORT_TYPE sort_type;
	char name[15];
	int pagenum;
};
struct SORTLIST : public MSGBASE
{
	SORT_TYPE sort_type;
	char name[15];
	int pagenum;
};
struct AUCTIONCANCEL : public MSGBASE
{
	CANCEL_TYPE cancel_type;
	int regIndex;
	char name[15];
};
struct  REGISTERAUCTION : public MSGBASE
{
	int	 PRO_Index;
	int  PRO_Amount;
	char BID_DUEDATE[10];
	int  BID_HighPrice;
	int  BID_ImmediatePrice;
	char Auctioneer[15];
};
struct JOINAUCTION : public MSGBASE
{
	int REG_Index;
	int BID_Price;
	char JOIN_Name[15];	
};
struct REAUCTIONSUCCESS 
{
	char PRO_Name[15];
	int PRO_Amount;
	char Auctioneer[15];
	int PRO_Index;
};
struct AUCTIONSUCCESS : public MSGBASE
{
	REAUCTIONSUCCESS resultlist[10];	
};
struct REBASELIST 
{
	int REG_Index;
	char PRO_Name[15];
	int  PRO_Amount;
	int  BID_HighPrice;
	int  BID_ImmediatePrice;
	DWORD BID_DUEDATE[10];
	char BID_HighName[15];
	int  JOIN_Amount;
	DWORD  BID_LeftTime;
	char Auctioneer[15];
	int	 PRO_Index;
};
struct BASELIST : public MSGBASE
{
	REBASELIST imfolist[10];
};
struct MSG_LEVEL : public MSGBASE
{
	enum EXPPOINTKIND
	{
		EXPPOINTKIND_ACQUIRE,
		EXPPOINTKIND_DIE,
	};
	LEVELTYPE Level;
	EXPTYPE CurExpPoint;
	EXPTYPE MaxExpPoint;
};
struct MSG_EXPPOINT : public MSGBASE
{
	enum EXPPOINTKIND
	{
		EXPPOINTKIND_ACQUIRE,
		EXPPOINTKIND_DIE,
		EXPPOINTKIND_ABILITYUSE,
	};
	EXPTYPE ExpPoint;
	BYTE ExpKind;
};
struct MSG_ABILLITY_EXPPOINT : public MSGBASE
{
	enum EXPPOINTKIND
	{
		EXPPOINTKIND_ACQUIRE,
		EXPPOINTKIND_DIE,
		EXPPOINTKIND_ABILITYUSE,
	};
	DWORD ExpPoint;
	BYTE ExpKind;
};
struct MSG_MUGONGEXP : public MSGBASE
{
	DWORD ExpPoint;
	DWORD absPos;
};
struct M2C_DAMAGE_MSG : public MSGBASE
{
	DWORD dwDamage;
};
struct TACTICSTART_INFO 
{
	WORD TacticID;
	char Name[MAX_NAME_LENGTH+1];
	BYTE NeedMemberNum; 
	WORD WaitingTime;
	WORD WaitingNaeryuk;
	WORD SuccessNaeryuk;
	int TacticStartEffect;
	WORD SkillNum;
	float PosX[MAX_PARTY_LISTNUM-1];
	float PosZ[MAX_PARTY_LISTNUM-1];
};
struct TACTIC_TOTALINFO
{
	VECTOR3 Pos;
	DWORD StartTime;
	DIRINDEX Direction;
	DWORD JoinedPlayerIDs[MAX_PARTY_LISTNUM];
	WORD TacticId;
};
struct SEND_TACTIC_TOTALINFO : public MSGBASE 
{
	BASEOBJECT_INFO baseinfo;
	TACTIC_TOTALINFO tacticinfo;
};
struct SEND_TACTIC_START : public MSGBASE
{
	DWORD OperatorID;
	WORD TacticID;
	DIRINDEX Direction;
};
struct SEND_TACTIC_JOIN : public MSGBASE
{
	DWORD TacticObjectID;
	DWORD JoinedPlayerID;
	BYTE Position;
};
struct TATIC_ABILITY_INFO
{
	WORD	wTypeAttack[MAX_TATIC_ABILITY_NUM];
	WORD	wTypeRecover[MAX_TATIC_ABILITY_NUM];
	float	fTypeBuffRate[MAX_TATIC_ABILITY_NUM];
	WORD	wTypeBuff[MAX_TATIC_ABILITY_NUM];
};
struct MAINTARGET
{
	enum
	{
		MAINTARGETKIND_OBJECTID,
		MAINTARGETKIND_POS,
	};
	BYTE MainTargetKind;
	union {
		DWORD dwMainTargetID;		
		COMPRESSEDPOS cpTargetPos;
	};
	void SetMainTarget(DWORD id)
	{	MainTargetKind = MAINTARGETKIND_OBJECTID;	dwMainTargetID = id;	}
	void SetMainTarget(VECTOR3* pp)
	{	MainTargetKind = MAINTARGETKIND_POS;	cpTargetPos.Compress(pp);	}
	MAINTARGET()	{}
	MAINTARGET(DWORD id)	{	SetMainTarget(id);	}
	MAINTARGET(VECTOR3* pp){	SetMainTarget(pp);	}
};
struct SKILLOBJECT_INFO
{
	DWORD SkillObjectIdx;
	WORD SkillIdx;
	VECTOR3 Pos;
	DWORD StartTime;
	DIRINDEX Direction;
	BYTE SkillLevel;
	DWORD Operator;
	MAINTARGET MainTarget;
	DWORD BattleID;
	BYTE BattleKind;
	WORD Option;
};
enum SKILLKIND
{
	SKILLKIND_COMBO = 0,
	SKILLKIND_OUTERMUGONG,
	SKILLKIND_INNERMUGONG,
	SKILLKIND_SIMBUB,
	SKILLKIND_JINBUB,	
	SKILLKIND_MINING,
	SKILLKIND_COLLECTION,
	SKILLKIND_HUNT,
	SKILLKIND_TITAN,
	SKILLKIND_MAX
};
struct SKILLINFO
{
	WORD SkillIdx;
	char SkillName[MAX_NAME_LENGTH+1];
	WORD SkillTooltipIdx;
	LEVELTYPE RestrictLevel;
	int	LowImage;
	int HighImage;
	WORD SkillKind;
	WORD WeaponKind;
	WORD SkillRange;
	WORD TargetKind;
	WORD TargetRange;
	WORD TargetAreaIdx;
	WORD TargetAreaPivot;
	WORD TargetAreaFix;
	WORD MoveTargetArea;
	WORD MoveTargetAreaDirection;
	float MoveTargetAreaVelocity;
	DWORD Duration;
	WORD Interval;
	WORD DelaySingleEffect;
	WORD ComboNum;
	WORD Life;
	WORD BindOperator;
	int EffectStartTime;
	int EffectStart;
	int EffectUse;	
	int EffectSelf;
	int EffectMapObjectCreate;
	int EffectMineOperate;
	DWORD DelayTime;	
	WORD FatalDamage;
	DWORD NeedExp[12];
	WORD NeedNaeRyuk[12];	
	WORD Attrib;	
	WORD NegativeResultTargetType;
	WORD TieUpType;
	WORD ChangeSpeedType;
	WORD ChangeSpeedRate;
	WORD Dispel;	
	WORD PositiveResultTargetType;	
	WORD Immune;
	WORD AIObject;
	WORD MineCheckRange;
	WORD MineCheckStartTime;
	WORD CounterDodgeKind;
	int CounterEffect;
	WORD DamageDecreaseForDist;
	float CounterDodgeRate[12];	
	WORD FirstRecoverLife[12];
	WORD FirstRecoverNaeRyuk[12];
	WORD ContinueRecoverLife[12];
	WORD ContinueRecoverNaeRyuk[12];
	WORD ContinueRecoverShield[12];	
	float CounterPhyAttack[12];
	float CounterAttAttack[12];
	float CriticalRate[12];
	float StunRate[12];
	WORD StunTime[12];
	float FirstPhyAttack[12];
	float FirstAttAttack[12];
	WORD FirstAttAttackMin[12];
	WORD FirstAttAttackMax[12];
	WORD ContinueAttAttack[12];
	float ContinueAttAttackRate[12];
	WORD AmplifiedPowerPhy[12];
	WORD AmplifiedPowerAtt[12];
	float AmplifiedPowerAttRate[12];
	float VampiricLife[12];
	float VampiricNaeryuk[12];
	float RecoverStateAbnormal[12];
	float DispelAttackFeelRate[12];
	float ChangeSpeedProbability[12];
	WORD UpMaxLife[12];
	WORD UpMaxNaeRyuk[12];
	WORD UpMaxShield[12];
	float UpPhyDefence[12];
	float UpAttDefence[12];
	float UpPhyAttack[12];	
	WORD DownMaxLife[12];
	WORD DownMaxNaeRyuk[12];
	WORD DownMaxShield[12];	
	float DownPhyDefence[12];
	float DownAttDefence[12];
	float DownPhyAttack[12];
	DWORD SkillAdditionalTime[12];
	WORD UpAttAttack[12];
	float DamageRate[12];
	float AttackRate[12];
	float UpCriticalRate[12];
	float AttackLifeRate[12];
	float AttackShieldRate[12];	
	float AttackSuccessRate[12];
	float VampiricReverseLife[12];	
	float VampiricReverseNaeryuk[12];
	DWORD AttackPhyLastUp[12];	
	DWORD AttackAttLastUp[12];
	WORD SkipEffect;
	BOOL CanSkipEffect;
	WORD SpecialState;
	WORD ChangeKind;
	int AddDegree;
	WORD SafeRange;
	WORD LinkSkillIdx;
	//BOOL bIsCheck;
};
struct SKILLOPTION{
	WORD Index;	
	WORD SkillKind;		
	WORD OptionKind;		
	WORD OptionGrade;		
	WORD ItemIndex;			
	int Life;				
	int NaeRyuk;			
	int Shield;			
	float PhyDef;			
	float AttDef;		
	int Range;				
	float ReduceNaeRyuk;	
	float PhyAtk;			
	float BaseAtk;			
	float AttAtk;		
	long Duration;			
};
struct SKILLSTATSOPTION		
{
	int Life;			
	int NaeRyuk;			
	int Shield;			
	float PhyDef;		
	float AttDef;		
	int Range;				
	float ReduceNaeRyuk;	
	float PhyAtk;			
	float BaseAtk;			
	float AttAtk;			
};
struct SPECIAL_STATE_INFO{
	WORD	Index;
	DWORD	TickTime;		
	WORD	NaeRyuk;		
	DWORD	DelayTime;		
	DWORD	DurationTime;	
	WORD	IconIdx;		
};
#include ".\TargetList\TargetList.h"
#include ".\TargetList\TargetListIterator.h"
struct MSG_SKILL_START_SYN : public MSGBASE
{
	WORD SkillIdx;
	DWORD Operator;
	DIRINDEX SkillDir;
	MAINTARGET MainTarget;
	CTargetList TargetList;
	void InitMsg(WORD wSkillIdx,MAINTARGET* pMainTarget,float AngleDeg,DWORD pOperator)
	{
		Category = MP_SKILL;
		Protocol = MP_SKILL_START_SYN;
		SkillIdx = wSkillIdx;
		TargetList.Clear();
		SkillDir = DEGTODIR(AngleDeg);
        Operator = pOperator;
		memcpy(&MainTarget,pMainTarget,sizeof(MAINTARGET));
		dwObjectID = pOperator;
	}
	WORD GetMsgLength()
	{
		return sizeof(MSG_SKILL_START_SYN) - sizeof(CTargetList) + TargetList.GetTotalMsgLen();
	}
};
struct MSG_SKILL_OPERATE : public MSGBASE
{
	DWORD SkillObjectID;
	DWORD RequestorID;
	MAINTARGET MainTarget;
	CTargetList TargetList;
	void InitMsg(WORD bProtocol,MAINTARGET* pMainTarget)
	{
		Category = MP_SKILL;
		Protocol = bProtocol;
		TargetList.Clear();
		memcpy(&MainTarget,pMainTarget,sizeof(MAINTARGET));
	}
	WORD GetMsgLength()
	{
		return sizeof(MSG_SKILL_OPERATE) - sizeof(CTargetList) + TargetList.GetTotalMsgLen();
	}
};
struct MSG_SKILLOBJECT_ADD : public MSGBASE
{
	SKILLOBJECT_INFO SkillObjectInfo;
	bool bCreate;
	CTargetList TargetList;
	void InitMsg(SKILLOBJECT_INFO* pSkillObjectInfo,bool Create)
	{
		Category = MP_SKILL;
		Protocol = MP_SKILL_SKILLOBJECT_ADD;
		memcpy(&SkillObjectInfo,pSkillObjectInfo,sizeof(SkillObjectInfo));
		SkillObjectInfo.StartTime = gCurTime - SkillObjectInfo.StartTime;
		TargetList.Clear();
		bCreate = Create;
	}
	WORD GetMsgLength()
	{
		return sizeof(MSG_SKILLOBJECT_ADD) - sizeof(CTargetList) + TargetList.GetTotalMsgLen();
	}
};
struct MSG_SKILL_SINGLE_RESULT : public MSGBASE
{
	DWORD SkillObjectID;
	DWORD SingleUnitNum;
	CTargetList TargetList;
	void InitMsg(DWORD SOID,DWORD SUNum)
	{
		Category = MP_SKILL;
		Protocol = MP_SKILL_SKILL_SINGLE_RESULT;
		SkillObjectID = SOID;
		SingleUnitNum = SUNum;		
		TargetList.Clear();
	}
	WORD GetMsgLength()
	{
		return sizeof(MSG_SKILL_SINGLE_RESULT) - sizeof(CTargetList) + TargetList.GetTotalMsgLen();
	}
};
struct SKILL_CHANGE_INFO
{
	WORD wMugongIdx; 
	WORD wChangeRate; 
	WORD wTargetMugongIdx; 
};
struct MSG_SKILL_DAMAGE_TARGETLIST : public MSGBASE
{
	DWORD SkillObjectID;
	DWORD RequestorID;
	MAINTARGET MainTarget;
	CTargetList TargetList;
	void InitMsg(WORD bProtocol,MAINTARGET* pMainTarget)
	{
		Category = MP_SKILL;
		Protocol = bProtocol;
		TargetList.Clear();
		memcpy(&MainTarget,pMainTarget,sizeof(MAINTARGET));
	}
	WORD GetMsgLength()
	{
		return sizeof(MSG_SKILL_DAMAGE_TARGETLIST) - sizeof(CTargetList) + TargetList.GetTotalMsgLen();
	}
};
struct KYUNGGONG_INFO
{
	WORD KyungGongIdx;		
	char KyungGongName[MAX_NAME_LENGTH+1];		
	WORD NeedNaeRyuk;		
	WORD MoveType;			
	float Speed;			
	WORD ChangeTime;
	int StartEffect;		
	int IngEffect;			
	int EndEffect;			
};
struct MUNPA_BOARD_INFO
{
	char MunpaBoardName[MAX_MUNPA_BOARD_NAME+1];
	DWORD RSubjectRank;
	DWORD RContentRank;
	DWORD WContentRank;
	DWORD DContentRank;
	DWORD BoardIDX;
};
struct MUNPA_BATTLERECORD_INFO 
{
	DWORD Win;
	DWORD Loss;
	DWORD Total;
}; 
struct MUNPA_HOME_INFO : public MSGBASE
{
	char IntroMsg[MAX_MUNPA_INTROMSG+1];
	DWORD MunpaID;
	DWORD BoardNum;
	MUNPA_BOARD_INFO BoardInfo[MAX_MUNPA_BOARD_NUM];
	BYTE Kind;
	char MunpaName[MAX_MUNPA_NAME+1];
	char MasterName[MAX_NAME_LENGTH+1];
	FAMETYPE MunpaFamous;
	BYTE MemberNum;
	char CreateDay[16];
	MUNPA_BATTLERECORD_INFO MunpaBattleRecordInfo[MAX_MUNPA_BATTLEKIND];
	BOOL bIsInMap;
};
struct MUNPALISTINFO_MAP : public MSGBASE
{
	DWORD MunpaID;
	DWORD MasterID;
	char MunpaName[MAX_MUNPA_NAME+1];
	FAMETYPE Famous;
	BYTE MemberNum;
	DWORD MunpaRank;
	MONEYTYPE MunpaMoney;
};
struct MUNPALISTINFO
{
	DWORD MunpaID;
	char MunpaMasterName[MAX_NAME_LENGTH+1];
	char MunpaName[MAX_MUNPA_NAME+1];
	FAMETYPE Famous;
	BYTE MemberNum;
	BYTE Kind;
};
struct SEND_MUNPACCEPT_INFO : public MSGBASE
{
	DWORD MunpaID;
	DWORD TargetID;
	BOOL bLast;
	char MunpaName[MAX_MUNPA_NAME+1];
};
struct SEND_MUNPA_ACCEPT_TO_MAP : public MSGBASE
{
	DWORD MunpaID;
	char MunpaName[MAX_MUNPA_NAME+1];
};
struct SEND_MUNPA_INFO : public MSGBASE
{
	MUNPALISTINFO Munpa[MAX_MUNPA_LIST];
	DWORD TotalResultNum;
};
struct SEND_MUNPAMEMBER_RANK_CHANGE : public MSGBASE
{
	DWORD TargetID;
	DWORD MunpaID;
	DWORD Rank;
};
struct SEND_MUNPAMEMBER_RANK_CHANGE_ACK : public MSGBASE
{
	DWORD MunpaID;
	BYTE Rank;
	FAMETYPE Fame;
};
struct SEND_MUNPALIST_SYN : public MSGBASE
{
	DWORD GotoPage;
	char OrderType[16];
};
struct MUNPA_SEARCH_SYN : public MSGBASE
{
	char SearchKey[32];
	DWORD GotoPage;
};
struct MUNPA_CREATE_INFO : public MSGBASE
{
	char MunpaName[MAX_MUNPA_NAME+1];
	char MunpaIntro[MAX_MUNPA_INTROMSG+1];
	BYTE Kind;
};
struct MUNPA_MODIFY_INTRO : public MSGBASE
{
	char MunpaIntro[MAX_MUNPA_INTROMSG+1];
	DWORD MunpaID;
};
struct MUNPA_SEMIMEMBER_INFO
{
	DWORD id;
	char Name[MAX_NAME_LENGTH+1];
	LEVELTYPE Level;
	FAMETYPE Famous;
	char RequestJoinDay[16];
};
struct SEND_MUNPA_SEMIMEMBER_LIST : public MSGBASE
{
	MUNPA_SEMIMEMBER_INFO MunpaMemberList[MAX_MUNPA_SEMIMEMBERLIST_NUM];
	DWORD TotalResultNum;
	DWORD MunpaID;
};
struct MUNPA_REGULARMEMBER_INFO
{
	DWORD id;
	char Name[MAX_NAME_LENGTH+1];
	DWORD Rank;
	LEVELTYPE Level;
	FAMETYPE Famous;
	BOOL bLogin;
	char lastvisitdate[16];
	char entrydate[16];
};
struct SEND_MUNPA_REGULARMEMBER_LIST : public MSGBASE
{
	MUNPA_REGULARMEMBER_INFO MunpaRegularMemberList[MAX_MUNPA_SEMIMEMBERLIST_NUM];
	DWORD TotalResultNum;
	DWORD MunpaID;
};
struct MUNPA_BOARD_NAME_NUM 
{
	char BoardName[MAX_MUNPA_BOARD_NAME+1];
	DWORD WritingsNum;
};
struct SEND_MUNPA_BOARD_INFO : public MSGBASE
{
	DWORD CurMunpaBoardNum;
	DWORD BoardIDX[MAX_MUNPA_BOARD_NUM];
	MUNPA_BOARD_NAME_NUM BoardNameNum[MAX_MUNPA_BOARD_NUM];
};
struct SEND_MUNPA_CREATE_INFO : public MSGBASE
{
	char MunpaName[MAX_MUNPA_NAME+1];
	DWORD dwData;
};
struct SEND_MUNPA_BOARD_RANK_INFO : public MSGBASE
{
	char MunpaBoardName[MAX_MUNPA_BOARD_NAME+1];
	DWORD RSubjectRank;
	DWORD RContentRank;
	DWORD WContentRank;
	DWORD DContentRank;
	DWORD ID; 
};
struct SEND_BOARD_CONTENT_INFO : public MSGBASE
{
	DWORD BoardIDX;
	DWORD RootContentsID;
	char Subject[MAX_MUNPA_BOARD_SUBJECT+1];
	char Content[MAX_MUNPA_BOARD_CONTENT+1];
	WORD GetMsgLength()
	{
		return sizeof(SEND_BOARD_CONTENT_INFO) - (MAX_MUNPA_BOARD_CONTENT+1) + strlen(Content)+1;
	}
};
struct BOARD_LIST_INFO
{
	char WriterName[MAX_NAME_LENGTH+1];
	char RegDate[32];
	int Count; 
	char Subject[MAX_MUNPA_BOARD_SUBJECT+1];
	int  Seq; 
	DWORD WritingsIDX;
	BYTE Depth;
};
struct SEND_MUNPA_BOARD_LIST_INFO : public MSGBASE
{
	BOARD_LIST_INFO BoardListInfo[MAX_MUNPA_BOARD_LIST];
	DWORD TotalPage;
};
struct SEND_MUNPA_BOARD_CONTENT_INFO : public MSGBASE
{
	BOARD_LIST_INFO BoardListInfo;
	BYTE DCRank;
	BYTE WCRank;
	char Content[MAX_MUNPA_BOARD_CONTENT+1];
	WORD GetMsgLength()
	{
		return sizeof(SEND_MUNPA_BOARD_CONTENT_INFO) - (MAX_MUNPA_BOARD_CONTENT+1) + strlen(Content)+1;
	}
};
struct MUNPA_WAREHOUSE_ITEM_INFO
{
	ITEMBASE WarehouseItem[TABCELL_GUILDWAREHOUSE_NUM];
	MONEYTYPE money;
};
struct SEND_MUNPA_WAREHOUSE_ITEM_INFO : public MSGBASE
{
	MUNPA_WAREHOUSE_ITEM_INFO info;
	WORD wOptionCount;
	WORD wRareOptionCount;
	WORD wPetInfoCount;
	BYTE bTabNum;
	CAddableInfoList AddableInfo;
	WORD GetSize()
	{
		return sizeof( SEND_MUNPA_WAREHOUSE_ITEM_INFO ) - sizeof(CAddableInfoList) + AddableInfo.GetInfoLength();
	}
};
struct PYOGUKLIST
{
	DWORD MaxCellNum;
	MONEYTYPE MaxMoney;
	MONEYTYPE BuyPrice;
};
struct SEND_PYOGUK_ITEM_INFO : public MSGBASE
{
	MONEYTYPE money;
	ITEMBASE PyogukItem[SLOT_PYOGUK_NUM];
	WORD wOptionCount;
	WORD wRareOptionCount;
	WORD wStoneOptionCount;   
	WORD wPetInfoCount;
	WORD wTitanItemCount;
	WORD wTitanEquipItemCount;
	CAddableInfoList AddableInfo;
	WORD GetSize()
	{
		return sizeof( SEND_PYOGUK_ITEM_INFO ) - sizeof(CAddableInfoList) + AddableInfo.GetInfoLength();
	}
};
struct SEND_PETINVEN_ITEM_INFO : public MSGBASE
{
	ITEMBASE PetInvenItem[SLOT_PETINVEN_NUM];
	WORD wOptionCount;
	WORD wRareOptionCount;
	CAddableInfoList AddableInfo;
	WORD GetSize()
	{
		return sizeof( SEND_PETINVEN_ITEM_INFO ) - sizeof(CAddableInfoList) + AddableInfo.GetInfoLength();
	}
};
struct MSG_FRIEND_MEMBER_ADDDELETEID : public MSGBASE
{
	DWORD PlayerID;
	char Name[MAX_NAME_LENGTH+1];
};
struct FRIEND
{
	char Name[MAX_NAME_LENGTH+1];
	DWORD Id;
	BOOL IsLoggIn;
};
struct FRIEND_LIST
{
	FRIEND Friend[MAX_FRIEND_LIST];
	int totalnum;
};
struct MSG_FRIEND_LIST_DLG : public MSGBASE
{
	BYTE count;
	FRIEND FriendList[MAX_FRIEND_NUM];
	WORD GetMsgLength()
	{
		return sizeof(MSG_FRIEND_LIST_DLG) - (sizeof(FRIEND) * (MAX_FRIEND_NUM - count));
	}
};
struct MSG_FRIEND_SEND_NOTE : public MSGBASE
{
	DWORD FromId;
	char FromName[MAX_NAME_LENGTH+1];
	char ToName[MAX_NAME_LENGTH+1];
	char Note[MAX_NOTE_LENGTH+1];
	WORD GetMsgLength()
	{
		return sizeof(MSG_FRIEND_SEND_NOTE) - (MAX_NOTE_LENGTH+1) + strlen(Note)+1;
	}
};
struct MSG_GUILD_SEND_NOTE : public MSGBASE
{
	DWORD FromId;
	char FromName[MAX_NAME_LENGTH+1];
	char Note[MAX_NOTE_LENGTH+1];
	WORD GetMsgLength()
	{
		return sizeof(MSG_GUILD_SEND_NOTE) - (MAX_NOTE_LENGTH+1) + strlen(Note)+1;
	}
};
struct MSG_FRIEND_SEND_NOTE_ID : public MSGBASE
{
	DWORD TargetID;
	char FromName[MAX_NAME_LENGTH+1];
	char Note[MAX_NOTE_LENGTH+1];
	WORD GetMsgLength()
	{
		return sizeof(MSG_FRIEND_SEND_NOTE_ID) - (MAX_NOTE_LENGTH+1) + strlen(Note)+1;
	}
};
struct FRIEND_NOTE
{
	char FromName[MAX_NAME_LENGTH+1];
	DWORD NoteID;
	char SendDate[16];
	BYTE bIsRead;
};
struct MSG_FRIEND_NOTE_LIST : public MSGBASE
{
	FRIEND_NOTE NoteList[NOTENUM_PER_PAGE];
	BYTE TotalPage;
};
struct MSG_FRIEND_DEL_NOTE : public MSGBASE
{
	DWORD NoteID;
	BOOL bLast;
};
struct MSG_FRIEND_READ_NOTE : public MSGBASE
{
	char FromName[MAX_NAME_LENGTH+1];
	DWORD NoteID;
	WORD ItemIdx;
	char Note[MAX_NOTE_LENGTH+1];
	WORD GetMsgLength()
	{
		return sizeof(MSG_FRIEND_READ_NOTE) - (MAX_NOTE_LENGTH+1) + strlen(Note)+1;
	}
};
struct MSG_CHANNEL_INFO : public MSGBASE
{
	WORD PlayerNum[MAX_CHANNEL_NUM];
	float RegenRate[MAX_CHANNEL_NUM];
	float DropRate[MAX_CHANNEL_NUM];
	float ExpRate[MAX_CHANNEL_NUM];
	float AbilityExpRate[MAX_CHANNEL_NUM];
	float MoneyRate[MAX_CHANNEL_NUM];
	bool bBattleChannel[MAX_CHANNEL_NUM];
	bool bPKAllowChannel[MAX_CHANNEL_NUM];
    WORD wMoveMapNum;
	DWORD dwChangeMapState;
	DWORD dwUniqueIDinAgent;
	BYTE Count;	
};
struct MSG_CHANNEL_MSGJACK : public MSGBASE
{
	WORD PlayerNum;
	float RegenRate;
	float DropRate;
	float ExpRate;
	float AbilityExpRate;
	float MoneyRate;
	bool bBattleChannel;
	bool bPKAllowChannel;
    WORD wMoveMapNum;
	DWORD dwChangeMapState;
	DWORD dwUniqueIDinAgent;
	BYTE Count;
	bool IsWarNow;
	bool IsLuckyDraw;
};
struct MSG_CHANNEL_INFO_MORNITOR : public MSGBASE
{
	MSG_CHANNEL_INFO	ChannelInfo;
	char				sServerIP[MAX_IPADDRESS_SIZE];
	WORD				wServerPort;
};
typedef struct tagServerTraffic : public MSGROOT
{
	LONG UserNum;
	int Returnkey;
}ServerTraffic;
struct MSG_CHAT : public MSGBASE
{
	char	Name[MAX_NAME_LENGTH+1];
	char	Msg[MAX_CHAT_LENGTH+1];
	int		GetMsgLength() { return sizeof(MSG_CHAT) - MAX_CHAT_LENGTH + strlen(Msg); }
};
struct MSG_CHAT_WITH_SENDERID : public MSGBASE
{
	DWORD	dwSenderID;
	char	Name[MAX_NAME_LENGTH+1];
	char	Msg[MAX_CHAT_LENGTH+1];
	int		GetMsgLength() { return sizeof(MSG_CHAT_WITH_SENDERID) - MAX_CHAT_LENGTH + strlen(Msg); }
};
struct MSG_CHAT_RECALL : public MSGBASE
{
	DWORD	dwSenderID;
	char	Name[MAX_NAME_LENGTH + 1];
	char	Msg[MAX_CHAT_LENGTH + 1];
	int		GetMsgLength() { return sizeof(MSG_CHAT_RECALL)-MAX_CHAT_LENGTH + strlen(Msg); }
};
struct MSG_WHISPER : public MSGBASE
{
	DWORD	dwReceiverID;
	char	SenderName[MAX_NAME_LENGTH+1];
	char	ReceiverName[MAX_NAME_LENGTH+1];
	char	Msg[MAX_CHAT_LENGTH+1];
	int		GetMsgLength() { return sizeof(MSG_WHISPER) - MAX_CHAT_LENGTH + strlen(Msg); }
};
struct SEND_PARTY_CHAT : public MSGBASE
{
	DWORD	MemberID[MAX_PARTY_LISTNUM];
	BYTE	MemberNum;
	char	Name[MAX_NAME_LENGTH+1];
	char	Msg[MAX_CHAT_LENGTH+1];
	int		GetMsgLength() { return sizeof(SEND_PARTY_CHAT) - MAX_CHAT_LENGTH + strlen(Msg); }
};
struct MSG_HACKUSER:public MSGBASE
{
	MSG_HACKUSER(){ memset(this, 0, sizeof(MSG_HACKUSER)); }
	DWORD UserID;
	char UserName[MAX_NAME_LENGTH+1];
	char ProcessName[512];
	char PrcessPath[512];
};
struct MSG_CHAT_WORD : public MSGBASE
{
	WORD	wData;
	char	Msg[MAX_CHAT_LENGTH+1];
	int		GetMsgLength() { return sizeof(MSG_CHAT_WORD) - MAX_CHAT_LENGTH + strlen(Msg); }
};
struct MSG_CHAT_WORD2:public MSGBASE
{
	BYTE	Flag;
	char	Msg[MAX_CHAT_LENGTH + 1];
	char	Name1[MAX_NAME_LENGTH+1];
	char	Name2[MAX_NAME_LENGTH+1];
	WORD	MapNum;
	WORD	MapNum2;
	char	ItemName[MAX_ITEMNAME_LENGTH+1];
	char	ItemName1[MAX_ITEMNAME_LENGTH+1];
	char	MonsterName[MAX_MONSTER_NAME_LENGTH+1];
	float	x;
	float	y;
	WORD	ChannelNum;
	WORD	KillTime;
	DWORD	dwData;
	WORD	Stage;
	WORD	AttackerStage;
	DWORD	GuildIdx1;
	DWORD	GuildIdx2;
};
enum eChatError
{
	CHATERR_ERROR,
	CHATERR_NO_NAME,
	CHATERR_NOT_CONNECTED,
	CHATERR_OPTION_NOWHISPER,
};
struct BATTLE_TEAMMEMBER_INFO
{
	DWORD MemberID;
	char MemberName[MAX_NAME_LENGTH+1];
	void set(DWORD id,char* name)
	{
		MemberID = id;
		strncpy_s(MemberName, sizeof(MemberName), name, MAX_NAME_LENGTH);//strncpy( MemberName, name, MAX_NAME_LENGTH );//kiv
		MemberName[MAX_NAME_LENGTH] = 0;
		MemberKillNum = 0;
		MemberDieNum = 0;
	}
	BYTE MemberKillNum;
	BYTE MemberDieNum;
};
struct BATTLE_INFO_BASE
{
	DWORD BattleID;
	BYTE BattleKind;
	BYTE BattleState;
	DWORD BattleTime;
};
struct MUNPAFIELD_TEAMINFO
{
	BYTE TotalKill;
	BYTE TotalDied;
	BATTLE_TEAMMEMBER_INFO TeamMember[MAX_MUNPAFIELD_MEMBERNUM];
	BATTLE_TEAMMEMBER_INFO* FindMember(DWORD id)
	{
		for(int n=0;n<MAX_MUNPAFIELD_MEMBERNUM;++n)
		{
			if(TeamMember[n].MemberID == id)
				return &TeamMember[n];
		}
		return NULL;
	}
	void AddTeamMember(DWORD id)
	{		
		for(int n=0;n<MAX_MUNPAFIELD_MEMBERNUM;++n)
		{
			if(TeamMember[n].MemberID == 0)
			{
				TeamMember[n].MemberID = id;
				break;
			}
		}
	}
};
struct MSG_BATTLESETTING_SYN_MUNPAFIELD : public BATTLE_INFO_BASE
{
	MUNPAFIELD_TEAMINFO TeamInfo[2];
};
struct MURIMFIELD_TEAMINFO
{
	BYTE TotalKill;
	BYTE TotalDied;
	BATTLE_TEAMMEMBER_INFO TeamMember[MAX_MURIMFIELD_MEMBERNUM];
	BATTLE_TEAMMEMBER_INFO* FindMember(DWORD id)
	{
		for(int n=0;n<MAX_MURIMFIELD_MEMBERNUM;++n)
		{
			if(TeamMember[n].MemberID == id)
				return &TeamMember[n];
		}
		return NULL;
	}
	void AddTeamMember(DWORD id)
	{		
		for(int n=0;n<MAX_MURIMFIELD_MEMBERNUM;++n)
		{
			if(TeamMember[n].MemberID == 0)
			{
				TeamMember[n].MemberID = id;
				break;
			}
		}
	}
};
struct MURIMFIELD_BATTLE_INFO
{
	WORD	wMurimBattleKind;
	WORD	wMurimBattleReward;	
};
struct MSG_BATTLESETTING_SYN_MURIMFIELD : public BATTLE_INFO_BASE
{
	MURIMFIELD_BATTLE_INFO	MurimBattleInfo;
	MURIMFIELD_TEAMINFO		TeamInfo[3];
};
#define MAX_BATTLE_INFO_LENGTH	2048
struct MSG_BATTLE_INFO : public MSGBASE
{
	BYTE BattleKind;
	char info[MAX_BATTLE_INFO_LENGTH];
	WORD msglen;
	int GetMsgLength()
	{
		return sizeof(MSGBASE) + msglen + sizeof(BYTE);
	}
};
struct MSG_BATTLE_TEAMMEMBER_ADDDELETE : public MSGBASE
{
	BATTLE_TEAMMEMBER_INFO Member;
	BYTE Team;
};
struct MSG_BATTLE_VIMU_CREATESTAGE : public MSGBASE
{
	DWORD	dwBattleID;
	VECTOR3 vPosStage;		
};
struct MSG_MONEY : public MSGBASE
{
	MONEYTYPE dwTotalMoney;
	BYTE bFlag;					
	DWORD ItemIdx;
	DWORD dwRealMoney;
};
struct MNPLAYER_BASEINFO
{
	DWORD		dwObjectID;
	WORD		wRankPoint;
	char		strPlayerName[MAX_NAME_LENGTH];
	LEVELTYPE	Level;
	char		strNick[MAX_NAME_LENGTH];
	WORD		wPlayCount;
	WORD		wWin;
	WORD		wLose;
	char		strMunpa[MAX_NAME_LENGTH];
	char		cbPositionInMunpa;
	char		cbTeam;
};
struct MSG_MNPLAYER_BASEINFO : public MSGBASE
{
	MNPLAYER_BASEINFO	Info;
};
struct MSG_MNPLAYER_BASEINFOLIST : public MSGBASE
{
	DWORD				dwTotalPlayerNum;
	MNPLAYER_BASEINFO	PlayerInfo[MAXPLAYER_IN_CHANNEL];
	DWORD GetMsgLength()
	{
		return sizeof(MSG_MNPLAYER_BASEINFOLIST)
				- sizeof(MNPLAYER_BASEINFO)*(MAXPLAYER_IN_CHANNEL - dwTotalPlayerNum);
	}	
};
struct CHANNEL_BASEINFO
{
	DWORD		dwChannelIndex;
	char		strChannelTitle[MAX_CHANNELTITLE_LENGTH];
	char		cbChannelKind;
	WORD		wMaxPlayer;
	WORD		wPlayerNum;
};
struct MSG_CHANNEL_BASEINFO : public MSGBASE
{
	CHANNEL_BASEINFO	Info;
};
struct MSG_CHANNEL_BASEINFOLIST : public MSGBASE
{
	DWORD				dwTotalChannelNum;
	CHANNEL_BASEINFO	ChannelInfo[MAXCHANNEL_IN_MURIMNET];
	DWORD GetMsgLength()
	{
		return sizeof(MSG_CHANNEL_BASEINFOLIST)
				- sizeof(CHANNEL_BASEINFO)*(MAXCHANNEL_IN_MURIMNET - dwTotalChannelNum);
	}
};
struct PLAYROOM_BASEINFO
{
	DWORD		dwPlayRoomIndex;
	char		strPlayRoomTitle[MAX_PLAYROOMTITLE_LENGTH];
	char		cbPlayRoomKind;
	WORD		wMaxObserver;
	WORD		wMaxPlayerPerTeam;
	MONEYTYPE	MoneyForPlay;
	WORD		wPlayerNum;
	char		cbStart;
	MAPTYPE		wMapNum;
};
struct MSG_PLAYROOM_BASEINFO : public MSGBASE
{
	PLAYROOM_BASEINFO	Info;
};
struct MSG_PLAYROOM_BASEINFOLIST : public MSGBASE
{
	DWORD				dwTotalPlayRoomNum;
	PLAYROOM_BASEINFO	PlayRoomInfo[MAXPLAYROOM_IN_MURIMNET];
	DWORD GetMsgLength()
	{
		return sizeof(MSG_PLAYROOM_BASEINFOLIST)
				- sizeof(PLAYROOM_BASEINFO)*(MAXPLAYROOM_IN_MURIMNET - dwTotalPlayRoomNum);
	}		
};
struct MSG_MNTEAMCHANGE : public MSGBASE
{
	DWORD	dwMoverID;
	BYTE	cbFromTeam;
	BYTE	cbToTeam;
};
struct MSPLAYER_INFO
{
	DWORD	dwObjectID;
	BYTE	cbTeam;
};
struct MSG_MNPLAYROOM_INFO : public MSGBASE
{
	int		nIndex;
	int		nGameKind;
	int		nTotalPlayerNum;
	MAPTYPE wMapNum;
	MSPLAYER_INFO PlayerInfo[100];
	WORD GetMsgLength()
	{
		return sizeof(MSG_MNPLAYROOM_INFO) - sizeof(MSPLAYER_INFO)*(100 - nTotalPlayerNum);
	}
};
struct WANTEDINFO
{
	WANTEDTYPE WantedIDX;
	MONEYTYPE TotalPrize;
	DWORD WantedChrIDX;
	char WantedChrName[MAX_NAME_LENGTH+1];
	DWORD RegistChrIDX;
	int TotalVolunteerNum;
};
struct WANTEDINFO_LOAD
{
	WANTEDINFO m_WantedInfo[MAX_QUERY_RESULT];
	int count;
};
struct SEND_WANTEDINFO_TO_SERVER : public MSGBASE
{
	WANTEDINFO Info;
};
struct WANTEDLIST
{
	WANTEDTYPE WantedIDX;
	DWORD WantedChrID;
	char WantedName[MAX_NAME_LENGTH+1];
	char RegistDate[11];
};
struct SEND_WANTEDLIST_TO_CLIENT : public MSGBASE
{
	WANTEDLIST List;
};
struct SEND_WANTEDLIST : public MSGBASE
{
	DWORD WantedIdx; 
	WANTEDLIST List[MAX_WANTED_NUM];
};
struct SEND_WANTED_NOTCOMPLETE : public MSGBASE
{
	WANTEDTYPE WantedIDX;
	char CompleterName[MAX_NAME_LENGTH+1];
	BYTE type;
};
struct SEND_WANTED_ORDER_SYN : public MSGBASE
{
	WORD page;
	BYTE type;
};
struct WANTNPCLIST
{
	WANTEDTYPE WantedIDX;
	char WantedName[MAX_NAME_LENGTH+1];
	MONEYTYPE Prize;
	BYTE VolunteerNum;
};
struct SEND_WANTNPCLIST : public MSGBASE
{
	WANTNPCLIST WantNpcList[WANTUNIT_PER_PAGE];
	WORD TotalPage;
};
struct SEND_GUILD_CREATE_SYN : public MSGBASE
{
	char GuildName[MAX_GUILD_NAME+1];
	char Intro[MAX_GUILD_INTRO+1];
	int GetMsgLength()
	{
		return sizeof(SEND_GUILD_CREATE_SYN) - (MAX_GUILD_INTRO+1) + strlen(Intro)+1;
	}
};
struct GUILDINFO
{
	DWORD GuildIdx;
	DWORD MasterIdx;
	char MasterName[MAX_NAME_LENGTH+1];
	char GuildName[MAX_GUILD_NAME+1];
	char GuildNotice[MAX_GUILD_NOTICE+1];
	DWORD UnionIdx;
	MAPTYPE MapNum;
	BYTE GuildLevel;
	MARKNAMETYPE MarkName;
	BYTE LvUpCounter;
	BYTE bNeedMasterChecking;
};
struct GUILDMEMBERINFO
{
	DWORD MemberIdx;
	char MemberName[MAX_NAME_LENGTH+1];
	LEVELTYPE Memberlvl;
	BYTE Rank;
	BOOL bLogged;
	void InitInfo(DWORD PlayerIDX, char* PlayerName, LEVELTYPE Playerlvl, BYTE PlayerRank = GUILD_MEMBER, BOOL bLogin = FALSE)
	{
		MemberIdx = PlayerIDX;
		strcpy_s(MemberName, sizeof(MemberName), PlayerName);//strcpy(MemberName, PlayerName);//kiv
		Memberlvl = Playerlvl;
		Rank = PlayerRank;
		bLogged = bLogin;
	}
};
struct SEND_GUILD_CREATE_ACK : public MSGBASE
{
	DWORD GuildIdx;
	char GuildName[MAX_GUILD_NAME+1];
};
struct SEND_GUILD_CREATE_NOTIFY : public MSGBASE
{
	GUILDINFO GuildInfo;
	LEVELTYPE MasterLvl;
};
struct SEND_GUILD_MEMBER_INFO : public MSGBASE
{
	DWORD GuildIdx;
	char GuildName[MAX_GUILD_NAME+1];	
	GUILDMEMBERINFO MemberInfo;
};
struct SEND_GUILD_TOTALINFO : public MSGBASE
{
	GUILDINFO GuildInfo;
	GUILDPOINT_INFO PointInfo;
	int membernum;
	GUILDMEMBERINFO MemberInfo[MAX_GUILDMEMBER_NUM];
	void InitGuildInfo(GUILDINFO* pInfo)
	{
		GuildInfo.GuildIdx = pInfo->GuildIdx;
		GuildInfo.GuildLevel = pInfo->GuildLevel;
		strcpy(GuildInfo.GuildName, pInfo->GuildName);
		GuildInfo.MapNum = pInfo->MapNum;
		GuildInfo.MasterIdx = pInfo->MasterIdx;
		strcpy(GuildInfo.GuildNotice, pInfo->GuildNotice);
		GuildInfo.UnionIdx = pInfo->UnionIdx;
		strcpy(GuildInfo.MasterName, pInfo->MasterName);
		GuildInfo.MarkName = pInfo->MarkName;
		GuildInfo.LvUpCounter = pInfo->LvUpCounter;
		GuildInfo.bNeedMasterChecking = pInfo->bNeedMasterChecking;
	}
	void InitPointInfo(GUILDPOINT_INFO* pInfo)
	{
		memcpy(&PointInfo, pInfo, sizeof(GUILDPOINT_INFO));
	}
	int GetMsgLength() { return sizeof(SEND_GUILD_TOTALINFO) - (MAX_GUILDMEMBER_NUM-membernum)*sizeof(GUILDMEMBERINFO);	}
};
struct SEND_GUILD_INVITE : public MSGBASE
{
	DWORD MasterIdx;
	char MasterName[MAX_NAME_LENGTH+1];
	char GuildName[MAX_GUILD_NAME+1];
};
struct SEND_GUILD_NICKNAME : public MSGBASE
{
	DWORD TargetIdx;
	char NickName[MAX_GUILD_NICKNAME+1];
};
struct SEND_GUILD_NAME : public MSGBASE
{
	char GuildName[MAX_GUILD_NAME+1];
};
struct SEND_GUILDMEMBER_LOGININFO : public MSGBASE
{
	DWORD MemberIdx;
	BOOL bLogIn;
};
struct SEND_GUILD_HUNTEDMOB_INFO : public MSG_DWORD2
{};
#define GUILDMARKBPP	2
#define GUILDMARKWIDTH	16
#define GUILDMARKHEIGHT	12
#define GUILDMARK_BUFSIZE	(GUILDMARKBPP*GUILDMARKWIDTH*GUILDMARKHEIGHT)
#define GUILDUNIONMARKHEIGHT	15
#define GUILDUNIONMARK_BUFSIZE	(GUILDMARKBPP*GUILDMARKWIDTH*GUILDUNIONMARKHEIGHT)
struct MSG_GUILDMARK_IMG : public MSGBASE
{
	DWORD GuildIdx;
	MARKNAMETYPE MarkName;
	char imgData[GUILDMARK_BUFSIZE];
};
struct MSG_GUILDUNIONMARK_IMG : public MSGBASE
{
	DWORD	dwMarkIdx;
	DWORD	dwGuildUnionIdx;
	char	imgData[GUILDUNIONMARK_BUFSIZE];
};
struct MSG_GUILDFIELDWAR_INFO : public MSGBASE
{
	DWORD		dwEnemyGuildNum;
	GUILDINFO	EnemyGuildInfo[100];
	MSG_GUILDFIELDWAR_INFO()
	{
		dwEnemyGuildNum = 0;
	}
	void	AddEnemyGuildInfo( GUILDINFO* pInfo, DWORD dwMoney = 0 )
	{
		if( dwEnemyGuildNum < 100 )
		{
			EnemyGuildInfo[dwEnemyGuildNum] = *pInfo;
			EnemyGuildInfo[dwEnemyGuildNum].MarkName = dwMoney;
			++dwEnemyGuildNum;
		}
	}
	int		GetMsgLength()	{ return sizeof(MSG_GUILDFIELDWAR_INFO) - (100-dwEnemyGuildNum)*sizeof(GUILDINFO); }
};
#pragma pack(1)
typedef DWORD	QSTATETYPE;
#define MAX_BIT ((sizeof(QSTATETYPE))*(8))
#define YEARTOSECOND	31536000
#define MONTHTOSECOND	2592000
#define DAYTOSECOND		86400
#define HOURTOSECOND	3600
#define MINUTETOSECOND	60
#define SECTOMILLISEC	1000
extern DWORD DayOfMonth[];
extern DWORD DayOfMonth_Yundal[];
struct stTIME{
	QSTATETYPE		value;
	stTIME():value(0) {}
	void SetTime(QSTATETYPE time)	{	value = time;	}	
	void SetTime(DWORD year, DWORD month, DWORD day, DWORD hour, DWORD minute, DWORD second)
	{
		value=0;
		QSTATETYPE ch=0;
		ch = year;
		value = (value | (ch<<28));
		ch = month;
		value = (value | (ch<<24));
		ch = day;
		value = (value | (ch<<18));
		ch = hour;
		value = (value | (ch<<12));
		ch = minute;
		value = (value | (ch<<6));
		ch = second;
		value = (value | ch);
	}
	DWORD GetYear()		
	{	
		return (value>>28);	
	}
	DWORD GetMonth()	{	DWORD msk = value<<4;		return msk>>28;		}
	DWORD GetDay()		{	DWORD msk = value<<8;		return msk>>26;		}
	DWORD GetHour()		{	DWORD msk = value<<14;		return msk>>26;		}
	DWORD GetMinute()	{	DWORD msk = value<<20;		return msk>>26;		}
	DWORD GetSecond()	{	DWORD msk = value<<26;		return msk>>26;		}
	inline BOOL	operator >(const stTIME& time);
	inline void operator +=(const stTIME& time);
	inline void operator -=(const stTIME& time);
	inline void operator =(const stTIME& time);
	enum stTIEM_KIND{ST_SEC,ST_MIN,ST_HOUR,ST_DAY};
	inline void AddTimeByValue(DWORD tVal, int flg_valueKind = ST_MIN);
};
inline BOOL	 stTIME::operator >(const stTIME& time)
{
	BOOL bResult = FALSE;
	stTIME ctime = time;
	if( this->GetYear() == ctime.GetYear() )
	{
		if( this->GetMonth() > ctime.GetMonth() )
			bResult = TRUE;
		else if( this->GetMonth() == ctime.GetMonth() )
		{
			if( this->GetDay() > ctime.GetDay() )
				bResult = TRUE;
			else if( this->GetDay() == ctime.GetDay() )
			{
				if( this->GetHour() > ctime.GetHour() )
					bResult = TRUE;
				else if( this->GetHour() == ctime.GetHour() )
				{
					if( this->GetMinute() > ctime.GetMinute() )
						bResult = TRUE;
					else if( this->GetMinute() == ctime.GetMinute() )
						if( this->GetSecond() > ctime.GetSecond() )
							bResult = TRUE;
				}						
			}
		}
	}
	else if( this->GetYear() > ctime.GetYear() )
		bResult = TRUE;
	return bResult;
}
inline void	 stTIME::operator +=(const stTIME& time)
{
	stTIME atime = time;
	int year, month, day, hour, minute, second, calcmonth;
	SYSTEMTIME systime;
	GetLocalTime(&systime);
	year = this->GetYear() + atime.GetYear();
	month = this->GetMonth() + atime.GetMonth();
	day = this->GetDay() + atime.GetDay();
	hour = this->GetHour() + atime.GetHour();
	minute = this->GetMinute() + atime.GetMinute();
	second = this->GetSecond() + atime.GetSecond();
	if( this->GetMonth() <= 0 )			return;
	else if( this->GetMonth() == 1 )	calcmonth = 11;
	else								calcmonth = this->GetMonth()-1;
	if(second >= 60)
	{
		++minute;
		second -= 60;
	}
	if(minute >= 60)
	{
		++hour;
		minute -= 60;
	}
	if(hour >= 24)
	{
		++day;
		hour -= 24;
	}
	if( (systime.wYear%4) == 0 )
	{
		if(day > (int)(DayOfMonth_Yundal[calcmonth]))
		{
			++month;
			day -= DayOfMonth_Yundal[calcmonth];
		}
	}
	else
	{
		if(day > (int)(DayOfMonth[calcmonth]))
		{
			++month;
			day -= DayOfMonth[calcmonth];
		}
	}
	if(month > 12)
	{
		++year;
		month -= 12;
	}
	this->SetTime(year, month, day, hour, minute, second);
}
inline void	 stTIME::operator -=(const stTIME& time)
{
	stTIME atime = time;
	int year, month, day, hour, minute, second, calcmonth;
	SYSTEMTIME systime;
	GetLocalTime(&systime);
	year = this->GetYear() - atime.GetYear();
	month = this->GetMonth() - atime.GetMonth();
	day = this->GetDay() - atime.GetDay();
	hour = this->GetHour() - atime.GetHour();
	minute = this->GetMinute() - atime.GetMinute();
	second = this->GetSecond() - atime.GetSecond();
	if( this->GetMonth() <= 0 )			return;
	else if( this->GetMonth() == 1 )	calcmonth = 11;
	else								calcmonth = this->GetMonth()-2;
	if(second < 0)
	{
		--minute;
		second += 60;
	}
	if(minute < 0)
	{
		--hour;
		minute += 60;
	}
	if(hour < 0)
	{
		--day;
		hour += 24;
	}
	if( (systime.wYear%4) == 0 )
	{
		if(day < 0)
		{
			--month;
			day += DayOfMonth_Yundal[calcmonth]; 
		}		
	}
	else
	{
		if(day < 0)
		{
			--month;
			day += DayOfMonth[calcmonth]; 
		}
	}
	if(month <= 0 && 0 < year)
	{
		--year;
		month += 12;
	}
	this->SetTime(year, month, day, hour, minute, second);
}
inline void	 stTIME::operator =(const stTIME& time)
{
	stTIME atime = time;
	this->SetTime(atime.GetYear(), atime.GetMonth(), atime.GetDay(), atime.GetHour(), atime.GetMinute(), atime.GetSecond());
}
inline void stTIME::AddTimeByValue(DWORD tVal, int flg_valueKind)
{
	DWORD day = 0;
	switch(flg_valueKind)
	{
	case ST_SEC:
		day = tVal / 86400; 
		break;
	case ST_MIN:
		day = tVal / 1440;	
		break;
	case ST_HOUR:
		day = tVal / 24;
		break;
	case ST_DAY:
		day = tVal;
		break;
	default:
		break;
	}
	DWORD Curyear = this->GetYear();
	DWORD CurMonth = this->GetMonth();
	day += this->GetDay();
	DWORD hour = this->GetHour() + ( tVal %(24*60) )/60;
	DWORD minute = this->GetMinute() + ( tVal %(24*60) )%60;
	DWORD* pDayOfMonth = NULL;
	if(Curyear%4 == 0)
	{
		pDayOfMonth = DayOfMonth_Yundal;
	}
	else
	{
		pDayOfMonth = DayOfMonth;
	}
	while( day > pDayOfMonth[CurMonth -1] )
	{
		day -= pDayOfMonth[CurMonth - 1];
		CurMonth++;
		if( CurMonth > 12 )
		{
			++Curyear;
			CurMonth = 1;
			if(Curyear%4 == 0)
			{
				pDayOfMonth = DayOfMonth_Yundal;
			}
			else
			{
				pDayOfMonth = DayOfMonth;
			}
		}
	}
	this->SetTime(Curyear, CurMonth, day, hour, minute, 0);
}
struct stPlayTime {
	DWORD value;
	stPlayTime::stPlayTime()
	{
		value = 0;
	}
	void GetTime(int& Year, int& Day, int& Hour, int& Minute, int& Second)
	{
		int mv = 0;
		Year = value/YEARTOSECOND;
		mv = value%YEARTOSECOND;
		Day = mv/DAYTOSECOND;
		mv = mv%DAYTOSECOND;
		Hour = mv/HOURTOSECOND;
		mv = mv%HOURTOSECOND;
		Minute = mv/MINUTETOSECOND;
		Second = mv%MINUTETOSECOND;		
	}
};
typedef struct tagField{
	DWORD b0:1; 
	DWORD b1:1;
	DWORD b2:1;
	DWORD b3:1;
	DWORD b4:1;
	DWORD b5:1;
	DWORD b6:1;
	DWORD b7:1;
	DWORD b8:1;
	DWORD b9:1;
	DWORD b10:1;
	DWORD b11:1;
	DWORD b12:1;
	DWORD b13:1;
	DWORD b14:1;
	DWORD b15:1;
	DWORD b16:1;
	DWORD b17:1;
	DWORD b18:1;
	DWORD b19:1;
	DWORD b20:1;
	DWORD b21:1;
	DWORD b22:1;
	DWORD b23:1;
	DWORD b24:1;
	DWORD b25:1;
	DWORD b26:1;
	DWORD b27:1;
	DWORD b28:1;
	DWORD b29:1;
	DWORD b30:1;
	DWORD b31:1;
} stFIELD, *pFIELD;
typedef union tagQuestFlag{
	QSTATETYPE	value;
	stFIELD		flag;
	tagQuestFlag():value(0) {}
	void SetField(BYTE bit, BOOL bSetZero=FALSE) 
	{
		if( bit  < 1 || bit > MAX_BIT ) return;
		QSTATETYPE ch;
		bSetZero ? ch=1 : ch=0;
		value = (value | (ch<<(MAX_BIT- bit)));
	}
	BOOL IsSet(BYTE bit)
	{
		if( bit  < 1 || bit > MAX_BIT ) return TRUE;
		QSTATETYPE ch = 1;
		return (value & (ch<<(MAX_BIT-bit)))? TRUE:FALSE;
	}
} QFLAG, *pQFLAG;
struct QBASE
{
	WORD		QuestIdx;
	QSTATETYPE	state;
};
struct QMBASE
{
	WORD		QuestIdx;	
	QFLAG		state;
	DWORD		EndParam;
	DWORD		Time;
	BYTE		CheckType;
	DWORD		CheckTime;
};
struct QSUBASE
{
	WORD		QuestIdx;
	WORD		SubQuestIdx;
	QSTATETYPE	state;
	QSTATETYPE	time;
};
struct QITEMBASE
{
	DWORD		ItemIdx;
	WORD		Count;
	WORD		QuestIdx;
};
struct SEND_QUEST_IDX_SIMPLE : public MSGBASE
{
	DWORD MainQuestIdx;
};
struct SEND_QUEST_IDX : public MSGBASE
{
	WORD		MainQuestIdx;
	WORD		SubQuestIdx;
	DWORD		dwFlag;
	DWORD		dwData;
	DWORD		dwTime;
};
struct SEND_QUESTITEM_IDX : public MSGBASE
{
	DWORD	dwItemIdx;
	DWORD	dwItemNum;
};
struct SEND_QUEST_TOTALINFO : public MSGBASE
{
	int GetSize() {
		return (sizeof(SEND_QUEST_TOTALINFO) - sizeof(QBASE)*(50 - wCount));
	}
	WORD	wCount;
	QBASE	QuestList[50];
};
struct SEND_MAINQUEST_DATA : public MSGBASE
{
	int GetSize() {
		return (sizeof(SEND_MAINQUEST_DATA) - sizeof(QMBASE)*(100 - wCount));
	}
	WORD	wCount;
	QMBASE	QuestList[100];
};
struct SEND_SUBQUEST_DATA : public MSGBASE
{
	int GetSize() {
		return (sizeof(SEND_SUBQUEST_DATA) -sizeof(QSUBASE)*(100-wCount));
	}
	WORD	wCount;
	QSUBASE	QuestList[100];
};
struct SEND_SUBQUEST_UPDATE : public MSGBASE
{
	WORD	wQuestIdx;
	WORD	wSubQuestIdx;
	DWORD	dwMaxCount;
	DWORD	dwData;
	DWORD	dwTime;
};
struct SEND_QUESTITEM : public MSGBASE
{
	int GetSize() {
		return (sizeof(SEND_QUESTITEM)-sizeof(QITEMBASE)*(100-wCount));
	}
	WORD		wCount;
	QITEMBASE	ItemList[100];
};
struct QUESTINFO {
	DWORD	m_dwIdx;		
	QFLAG	m_flag;			
};
struct JOURNALINFO
{
	DWORD Index;
	BYTE Kind;
	DWORD Param;
	DWORD Param_2;			
	DWORD Param_3;			
	BYTE bSaved;
	char ParamName[MAX_NAME_LENGTH+1];
	char RegDate[11];
};
struct SEND_JOURNALLIST : public MSGBASE
{
	WORD	wCount;
	JOURNALINFO m_Info[MAX_JOURNAL_ITEM];
	int GetSize() {
	return (sizeof(SEND_JOURNALLIST)-sizeof(JOURNALINFO)*(MAX_JOURNAL_ITEM-wCount));
	}
};
struct SEND_JOURNALINFO : public MSGBASE
{
	JOURNALINFO		m_Info;
};
struct SEND_JOURNAL_DWORD : public MSGBASE
{
	DWORD Param;
};
struct MoveCoord
{
	WORD posX;
	WORD posZ;
	MoveCoord::MoveCoord()
	{
		posX = posZ = 0;
	}
};
struct PARTYICON_MOVEINFO
{
	DWORD	dwMoverID;
	BYTE	tgCount;
	WORD	KyungGongIdx;
	BYTE	MoveState;
	MoveCoord	cPos;
	MoveCoord	tgPos[MAX_CHARTARGETPOSBUF_SIZE];
	PARTYICON_MOVEINFO::PARTYICON_MOVEINFO()
	{
		dwMoverID	= 0;
		tgCount		= 0;
		KyungGongIdx = 0;
		MoveState	= 0;
		memset(&cPos, 0, sizeof(MoveCoord));
		memset(tgPos, 0, sizeof(MoveCoord)*MAX_CHARTARGETPOSBUF_SIZE);
	}
	int GetSize()
	{
		return sizeof(PARTYICON_MOVEINFO)-sizeof(MoveCoord)*(MAX_CHARTARGETPOSBUF_SIZE-tgCount);
	}
};
struct SEND_PARTYICON_MOVEINFO : public MSGBASE
{
	PARTYICON_MOVEINFO MoveInfo;
	int GetSize()
	{
		return sizeof(MSGBASE)+MoveInfo.GetSize();
	}
};
struct SEND_PARTYICON_REVIVE : public MSGBASE
{
	DWORD		dwMoverID;
	MoveCoord	Pos;
};
struct SHOP_ITEM
{
	ITEMBASE	Item[SLOT_SHOPITEM_NUM];
};
struct SHOPITEMBASE
{
	ITEMBASE	ItemBase;	
	DWORD		Param;				
	stTIME		BeginTime;
	DWORD		Remaintime;
	SHOPITEMBASE::SHOPITEMBASE()
	{
		memset(&ItemBase, 0, sizeof(ITEMBASE));
		Param = 0;
		BeginTime.value = 0;
		Remaintime=0;
	}
};
struct SHOPITEMWITHTIME
{
	SHOPITEMBASE	ShopItem;
	DWORD			LastCheckTime;
	SHOPITEMWITHTIME::SHOPITEMWITHTIME()
	{
		LastCheckTime = 0;
	}
};
struct SHOPITEMUSEBASE
{
	POSTYPE ShopItemPos;
	WORD	ShopItemIdx;
	POSTYPE TargetItemPos;
	WORD	TargetItemIdx;
};
struct SHOPITEM_SIMPLE
{
	ITEMBASE	Item;
	stTIME		EndTime;
	SHOPITEM_SIMPLE::SHOPITEM_SIMPLE()
	{
		memset(&Item, 0, sizeof(ITEMBASE));
		EndTime.value = 0;
	}
};
struct MSG_SHOPITEM_USE_SYN : public MSGBASE
{
	SHOPITEMUSEBASE UseBaseInfo;
};
struct SEND_SHOPITEM_BASEINFO : public MSGBASE
{
	SHOPITEMBASE	ShopItemBase;
	POSTYPE			ShopItemPos;
	WORD			ShopItemIdx;
};
struct SEND_SHOPITEM_USEONE : public MSGBASE
{
	SHOPITEMBASE	ShopItemBase;
};
struct SEND_SHOPITEM_USEDINFO : public MSGBASE
{
	WORD ItemCount;
	SHOPITEMBASE Item[100];
	SEND_SHOPITEM_USEDINFO::SEND_SHOPITEM_USEDINFO()
	{
		ItemCount = 0;
		memset(Item, 0, sizeof(SHOPITEMBASE)*100);
	}
	int GetSize()	{
		return (sizeof(SEND_SHOPITEM_USEDINFO) - sizeof(SHOPITEMBASE)*(100-ItemCount));
	}
};
struct SEND_SHOPITEM_INFO : public MSGBASE
{
	WORD		ItemCount;
	ITEMBASE	Item[SLOT_SHOPITEM_NUM];
	SEND_SHOPITEM_INFO::SEND_SHOPITEM_INFO() {
		ItemCount = 0;
		memset(Item, 0, sizeof(ITEMBASE)*SLOT_SHOPITEM_NUM);
	}
	int GetSize() {
		return (sizeof(SEND_SHOPITEM_INFO) - sizeof(ITEMBASE)*(SLOT_SHOPITEM_NUM-ItemCount));
	}
};
struct SEND_SHOPITEMOPTION : public MSGBASE
{
	SHOPITEMOPTION	Info;
	SEND_SHOPITEMOPTION::SEND_SHOPITEMOPTION() {
		memset(&Info, 0, sizeof(Info));
	}
};
struct stMOVEPOINT
{
	DWORD value;
	stMOVEPOINT::stMOVEPOINT()
	{
		value = 0;
	}
	void SetMovePoint(WORD wx, WORD wz)
	{
		value = 0;
		DWORD msk = 0;
		msk = wx;
		value = (value | (msk<<16));
		msk = wz;
		value = (value | msk);
	}
	void GetMovePoint(WORD& wx, WORD& wz)
	{
		DWORD msk = 0;
		wx = (WORD)(value>>16);
		msk = value<<16;
		wz = (WORD)(msk>>16);
	}	
};
struct MOVEDATA
{
	DWORD		DBIdx;
	char		Name[MAX_SAVEDMOVE_NAME];
	WORD		MapNum;
	stMOVEPOINT Point;
	MOVEDATA::MOVEDATA()
	{
		DBIdx = 0;
		memset(Name, 0, MAX_SAVEDMOVE_NAME);
		MapNum = 0;
		Point.value = 0;
	}
};
struct SEND_MOVEDATA_INFO : public MSGBASE
{
	BYTE		bInited;
	WORD		Count;
	MOVEDATA	Data[MAX_MOVEDATA_PERPAGE*MAX_MOVEPOINT_PAGE];
	SEND_MOVEDATA_INFO::SEND_MOVEDATA_INFO()
	{
		bInited = 0;
		Count = 0;
		memset(Data, 0, sizeof(MOVEDATA)*MAX_MOVEDATA_PERPAGE*MAX_MOVEPOINT_PAGE);
	}
	int GetSize()
	{
		return (sizeof(SEND_MOVEDATA_INFO) - sizeof(MOVEDATA)*(MAX_MOVEDATA_PERPAGE*MAX_MOVEPOINT_PAGE-Count));
	}
};
struct SEND_MOVEDATA_SIMPLE : public MSGBASE
{
	MOVEDATA Data;
};
struct SEND_MOVEDATA_WITHITEM : public MSGBASE
{
	MOVEDATA			Data;
	SHOPITEMUSEBASE		Item;
};
struct SEND_GAMEIN_USEITEM : public MSGBASE
{
	DWORD dwData1;
	DWORD dwData2;
	DWORD dwData3;
	DWORD dwPosition;
	DWORD dwItemIdx;
	DWORD dwItemPos;
};
struct REVIVEDATA
{
	DWORD TargetID;
	WORD ItemIdx;
	POSTYPE ItemPos;
	REVIVEDATA::REVIVEDATA()
	{
		TargetID = 0;
		ItemIdx = 0;
		ItemPos = 0;
	}
};
struct SEND_REVIVEOTHER : public MSGBASE
{
	DWORD	TargetID;
	char	TargetName[MAX_NAME_LENGTH+1];
	SEND_REVIVEOTHER::SEND_REVIVEOTHER()
	{
		TargetID = 0;
		memset( TargetName, 0, sizeof(char)*(MAX_NAME_LENGTH+1) );
	}
};
struct AVATARITEM
{
	BYTE	Gender;				
	BYTE	Position;
	WORD	Item[eAvatar_Max];
	AVATARITEM::AVATARITEM()
	{
		Gender = 2;
		Position = 0;
		memset( Item, 0, sizeof(WORD)*eAvatar_Max );
		for(int i=eAvatar_Weared_Hair; i<eAvatar_Max; i++)
			Item[i]=1;
	}
};
struct SEND_AVATARITEM_INFO : public MSGBASE
{
	DWORD	PlayerId;
	WORD	ItemIdx;
	WORD	ItemPos;
	WORD	Avatar[eAvatar_Max];
	SEND_AVATARITEM_INFO::SEND_AVATARITEM_INFO()
	{
		PlayerId=0;
		ItemIdx=0;
		ItemPos=0;
		memset( Avatar, 0, sizeof(WORD)*eAvatar_Max );
		for(int i=eAvatar_Weared_Hair; i<eAvatar_Max; i++)
			Avatar[i]=1;
	}
};
struct CHARACTERCHANGE_INFO
{
	BYTE	Gender;
	BYTE	FaceType;
	BYTE	HairType;
	float	Height;
	float	Width;
	CHARACTERCHANGE_INFO::CHARACTERCHANGE_INFO()
	{
		Gender = 0;
		FaceType = 0;
		HairType = 0;
		Height = 0;
		Width = 0;
	}
};
struct SEND_CHARACTERCHANGE_INFO : public MSGBASE
{
	DWORD		Param;
	CHARACTERCHANGE_INFO		Info;
	SEND_CHARACTERCHANGE_INFO::SEND_CHARACTERCHANGE_INFO()
	{
		Param = 0;
	}
};
struct SHOPITEMDUP
{
	DWORD Index;
	DWORD DupType;
	DWORD Param;
	SHOPITEMDUP::SHOPITEMDUP()
	{
		Index = 0;
		DupType = 0;
		Param = 0;
	}
};
struct MSG_PW_CREATE : public MSGBASE
{
	DWORD	dwTeam;
	VECTOR3	vPosition;
};
struct stMemoryInfo
{
	DWORD type;
	DWORD Created;
	DWORD Released;
	int MemorySize;
	stMemoryInfo::stMemoryInfo()
	{
		type=0;
		Created=0;
		Released=0;
		MemorySize=0;
	}
	void stMemoryInfo::Increase( int size )
	{
		++Created;
		MemorySize += size;
	}
	void stMemoryInfo::Decrease( int size )
	{
		++Released;
		MemorySize -= size;
	}
};
enum
{
	eSkillFactory_MakeSkillObject=0,
	eObjectFactory_Player,
	eObjectFactory_Monster,
	eObjectFactory_Npc,
	eObjectFactory_Tactic,
	eObjectFactory_BossMonster,
	ePackedData_Init,
	eMemoryChecker_Max,
};
struct SHOUTBASE
{
	DWORD CharacterIdx;
	char Msg[MAX_SHOUT_LENGTH+1];
	DWORD ItemType;  
};
struct SHOUTRECEIVE
{
	BYTE Count;
	WORD Time;
	DWORD CharacterIdx;
};
struct SEND_SHOUTBASE : public MSGBASE
{
	BYTE		Count;
	SHOUTBASE	ShoutMsg[3];
	SEND_SHOUTBASE::SEND_SHOUTBASE()
	{
		Count = 0;
		memset( ShoutMsg, 0, sizeof(SHOUTBASE)*3 );
	}
	DWORD SEND_SHOUTBASE::GetSize()
	{
		return (sizeof(SEND_SHOUTBASE) - sizeof(SHOUTBASE)*(3-Count));
	}
};
struct SEND_SHOUTRECEIVE : public MSGBASE
{
	SHOUTRECEIVE	Receive;
};
struct SEND_SHOUTBASE_ITEMINFO : public MSGBASE
{	
	WORD ItemIdx;
	WORD ItemPos;
	DWORD ItemType;  
	char Msg[MAX_SHOUT_LENGTH+1];
	SEND_SHOUTBASE_ITEMINFO::SEND_SHOUTBASE_ITEMINFO()
	{
		ItemIdx = 0;
		ItemPos = 0;
		ItemType=0;  
		memset( Msg, 0, sizeof(char)*MAX_SHOUT_LENGTH+1 );
	}
};
struct SEND_CHASEBASE : public MSGBASE
{	
	char	WantedName[MAX_NAME_LENGTH+1];
	DWORD	dwItemIdx;
};
struct SEND_CHASEINFO : public MSGBASE
{
	DWORD CharacterIdx;
	char WandtedName[MAX_NAME_LENGTH+1];
	COMPRESSEDPOS Pos;
	MAPTYPE	MapNum;
	MAPTYPE	EventMapNum;
};
struct SEND_CHANGENAMEBASE : public MSGBASE
{
	char	Name[MAX_NAME_LENGTH+1];
	DWORD	DBIdx;
};
struct sTIPITEMINFO
{
	DWORD ItemIndex;
	char  ItemName[MAX_ITEMNAME_LENGTH + 1];
};
struct MSGGOLDITEM :public MSGBASE
{
	DWORD dwItemDBidx;
	DWORD dwItemIdx;
	DWORD dwGoldMoney;
};
enum eCheatEvent
{
	eEvent_None=0,
	eEvent_ExpRate,
	eEvent_ItemRate,
	eEvent_MoneyRate,
	eEvent_DamageReciveRate,
	eEvent_DamageRate,
	eEvent_NaeRuykRate,
	eEvent_UngiSpeed,
	eEvent_PartyExpRate,
	eEvent_AbilRate,
	eEvent_GetMoney,
	eEvent_MugongExp,
	eEvent_DamagePlayerToPlayerRate,
	eEvent_MugongPhy,//PVP武功物理攻击
	eEvent_MugongAttr,//PVP武功属性攻击
	eEvent_MugongKillMonster,//PVE伤害倍率设置
	eEvent_MugongPhyCritical,//PVP/PVE暴击倍率设置战士
	eEvent_MugongAttrCritical,//PVP/PVE暴击倍率设置法师
	eEvent_AssassinPhyDmg,
	eEvent_Max,
};
enum ePartyEvent
{
	ePartyEvent_None=0,
	ePartyEvent_ItemRate,
	ePartyEvent_NaeRyukRate,
	ePartyEvent_MussangCharge,
	ePartyEvent_DefenceRate,
	ePartyEvent_CriticalRate,
	ePartyEvent_Max,
};
struct PARTYEVENT
{
	float	Rate[MAX_PARTY_LISTNUM];
};
struct MSG_GM_MOVE : public MSGBASE
{
	char Name[MAX_NAME_LENGTH+1];
	DWORD dwData1;
	DWORD dwData2;
	DWORD dwGMIndex;
	char strGMID[MAX_NAME_LENGTH+1];
};
struct MSG_EVENTNOTIFY_ON : public MSGBASE
{
	char strTitle[32];
	char strContext[128];
	BYTE EventList[eEvent_Max];
};
struct GAMEEVENT 
{
	void Init(BYTE kind, DWORD data)
	{
		bEventKind = kind;
		dwData = data;
	}
	BYTE bEventKind;			
	DWORD dwData;				
};
enum  eGameEvent1 {
	eGameEvent1_LevelUp,
	eGameEvent1_HeroDie,
	eGameEvent1_SavePoint,
	eGameEvent1_Ungijosik,
	eGameEvent1_GameIn,
	eGameEvent1_MonsterKill,
	eGameEvent1_ObtainItem,
	eGameEvent1_Max,
};
enum  eGameEvent
{
	eGameEvent_CharInterface,
	eGameEvent_InvenInterface,
	eGameEvent_MugongInterface,
	eGameEvent_QuestInterface,
	eGameEvent_MapInterface,
	eGameEvent_MunpaInterface,
	eGameEvent_FriendInterface,
	eGameEvent_NoteInterface,
	eGameEvent_OptionInterface,
	eGameEvent_ExitInterface,
	eGameEvent_PKInterface,
	eGameEvent_Hunt,
	eGameEvent_Die,
	eGameEvent_DieAfter5,
	eGameEvent_NpcClick,
	eGameEvent_DieFromPK,
	eGameEvent_KillPK,
	eGameEvent_ObtainItem,
	eGameEvent_ItemSetting,
	eGameEvent_LoginPoint,
	eGameEvent_Mugong0,
	eGameEvent_Mugong1,
	eGameEvent_Munpaunder,
	eGameEvent_Ability,
	eGameEvent_LevelUp,
	eGameEvent_MapChange,
	eGameEvent_Max,
};
struct stTournamentTime
{
	WORD	Day;
	WORD	Hour;
	WORD	Minute;
};
struct GTDBLoadInfo
{
	DWORD	GuildIdx;
	WORD	Ranking;
	WORD	Position;
	WORD	ProcessTournament;
};
struct REGISTEDGUILDINFO
{
	DWORD			GuildIdx;
	char			ImageData[GUILDMARK_BUFSIZE];
	char			GuildName[MAX_GUILD_NAME+1];
	DWORD			MarkExist;
	BYTE			Position;
	BYTE			Ranking;
	BYTE			ProcessTournament;
};
struct GUILDRANKING
{
	DWORD			GuildIdx;
	BYTE			Ranking;
};
struct GTTEAM_MEMBERINFO
{
	DWORD		TeamGuildIdx[2];
	WORD		TeamMember[2];
	WORD		TeamDieMember[2];
};
struct SEND_GTWINLOSE : public MSGBASE
{
	WORD		TournamentCount;
	DWORD		GuildIdx;
	char		GuildName[MAX_GUILD_NAME+1];
	BYTE		bWin;
	DWORD		Param;			
	BYTE		bUnearnedWin;	
};
struct SEND_BATTLEJOIN_INFO : public MSGBASE
{
	DWORD		AgentIdx;
	DWORD		UserLevel;
	DWORD		GuildIdx;
	DWORD		BattleIdx;
	DWORD		ReturnMapNum;
};
struct SEND_BATTLESCORE_INFO : public MSGBASE
{
	char		GuildName[2][MAX_GUILD_NAME+1];
	DWORD		Score[2];
	DWORD		FightTime;
	DWORD		EntranceTime;
	DWORD		State;
	BYTE		Team;
	SEND_BATTLESCORE_INFO::SEND_BATTLESCORE_INFO()
	{
		memset( GuildName[0], 0, sizeof(char)*(MAX_GUILD_NAME+1) );
		memset( GuildName[1], 0, sizeof(char)*(MAX_GUILD_NAME+1) );
		Score[0] = 0;
		Score[1] = 0;
		FightTime = 0;
		EntranceTime = 0;
		State = 0;
		Team = 0;
	}
};
struct SEND_REGISTEDGUILDINFO : public MSGBASE
{
	BYTE				Count;
	BYTE				CurTournament;
	DWORD				PlayerID;
	REGISTEDGUILDINFO	GInfo[MAXGUILD_INTOURNAMENT];
	SEND_REGISTEDGUILDINFO::SEND_REGISTEDGUILDINFO()
	{
		Count = 0;
		CurTournament = 0;
		PlayerID = 0;
		memset( GInfo, 0, sizeof(REGISTEDGUILDINFO)*MAXGUILD_INTOURNAMENT );
	}
	DWORD SEND_REGISTEDGUILDINFO::GetSize()
	{
		return (sizeof(SEND_REGISTEDGUILDINFO) - sizeof(REGISTEDGUILDINFO)*(MAXGUILD_INTOURNAMENT-Count));
	}
};
struct SEND_GUILDRANKING : public MSGBASE
{
	BYTE			Count;
	GUILDRANKING	Ranking[MAXGUILD_INTOURNAMENT];
	SEND_GUILDRANKING::SEND_GUILDRANKING()
	{
		Count = 0;
		memset( Ranking, 0, sizeof(GUILDRANKING)*MAXGUILD_INTOURNAMENT );
	}
	DWORD SEND_GUILDRANKING::GetSize()
	{
		return (sizeof(SEND_GUILDRANKING) - sizeof(GUILDRANKING)*(MAXGUILD_INTOURNAMENT-Count));
	}
};
struct GTBATTLE_INFO
{
	BYTE	Group;		
	DWORD	BattleID;
	char	GuildName1[MAX_GUILD_NAME+1];
	char	GuildName2[MAX_GUILD_NAME+1];
};
struct SEND_GTBATTLE_INFO : public MSGBASE
{
	BYTE			Count;
	BYTE			PlayOff;		
	GTBATTLE_INFO	BattleInfo[MAXGUILD_INTOURNAMENT/2];
	SEND_GTBATTLE_INFO::SEND_GTBATTLE_INFO()
	{
		Count = 0;
		memset( BattleInfo, 0, sizeof(GTBATTLE_INFO)*(MAXGUILD_INTOURNAMENT/2) );
	}
	DWORD SEND_GTBATTLE_INFO::GetSize()
	{
		return (sizeof(SEND_GTBATTLE_INFO) - sizeof(GTBATTLE_INFO)*(MAXGUILD_INTOURNAMENT/2-Count));
	}
};
struct GTBATTLE_INFOBROAD
{
	DWORD	BattleID;
	DWORD	Group;			
	DWORD	GuildIdx[2];
};
struct SEND_GTBATTLE_INFOBROAD : public MSGBASE
{
	BYTE					Count;
	DWORD					CurTournament;
	GTBATTLE_INFOBROAD		BattleInfo[MAXGUILD_INTOURNAMENT/2];
	SEND_GTBATTLE_INFOBROAD::SEND_GTBATTLE_INFOBROAD()
	{
		Count = 0;
		memset( BattleInfo, 0, sizeof(GTBATTLE_INFOBROAD)*MAXGUILD_INTOURNAMENT/2 );
	}
	DWORD SEND_GTBATTLE_INFOBROAD::GetSize()
	{
		return (sizeof(SEND_GTBATTLE_INFOBROAD) - sizeof(GTBATTLE_INFOBROAD)*(MAXGUILD_INTOURNAMENT/2-Count));
	}
};
#ifdef _CHINA_LOCAL_
#define		MAX_GUILD_UNION_NUM		4
#else
#define		MAX_GUILD_UNION_NUM		7
#endif
struct sGUILDIDXNAME
{
	DWORD	dwGuildIdx;
	char	sGuildName[MAX_GUILD_NAME+1];
};
struct SEND_GUILD_UNION_INFO : public MSGBASE
{
	DWORD			dwGuildUnionIdx;
	char			sGuildUnionName[MAX_GUILD_NAME+1];
	DWORD			dwGuildUnionMarkIdx;
	int				nMaxGuildNum;
	sGUILDIDXNAME	GuildInfo[MAX_GUILD_UNION_NUM];
	SEND_GUILD_UNION_INFO()
	{
		dwGuildUnionIdx = 0;
		memset( sGuildUnionName, 0, MAX_GUILD_NAME+1 );
		dwGuildUnionMarkIdx = 0;
		memset( GuildInfo, 0, sizeof(sGUILDIDXNAME)*MAX_GUILD_UNION_NUM );
		nMaxGuildNum = 0;
	}
	void AddGuildInfo( DWORD dwGuildIdx, char* pGuildName )
	{
		GuildInfo[nMaxGuildNum].dwGuildIdx = dwGuildIdx;
		strncpy( GuildInfo[nMaxGuildNum].sGuildName, pGuildName, MAX_GUILD_NAME+1 );
		++nMaxGuildNum;
	}
	int GetSize()
	{
		return ( sizeof(SEND_GUILD_UNION_INFO) - sizeof(sGUILDIDXNAME)*(MAX_GUILD_UNION_NUM-nMaxGuildNum) );
	}
};
struct SEND_CHEAT_ITEM_OPTION : public MSGBASE
{
	SEND_CHEAT_ITEM_OPTION(){memset(this, 0, sizeof(SEND_CHEAT_ITEM_OPTION));}
	POSTYPE bPosition;
	WORD wItemIdx;
	WORD wOptionKind;
	ITEM_OPTION_INFO		OptionInfo;
};
#define SIEGEWAR_MAXGUILDCOUNT_PERTEAM		128
#define SIEGEWAR_MAX_SIEGEMAP				20
#define SIEGEWAR_MAX_AFFECTED_MAP			10
#define CASTLEGATE_NAME_LENGTH				16
enum
{
	eCastleGateLevel_Level0 = 0,
	eCastleGateLevel_Level1,
	eCastleGateLevel_Level2,
	eCastleGateLevel_Level3,
	eCastleGateLevel_Max,
};
struct MAPOBJECT_INFO
{
	DWORD	Life;
	DWORD	MaxLife;
	DWORD	Shield;
	DWORD	MaxShield;
	DWORD	PhyDefence;
	float	Radius;
	ATTRIBUTEREGIST		AttrRegist;
};
struct CASTLEGATE_BASEINFO
{
	DWORD		Index;
	DWORD		MapNum;
	char		Name[CASTLEGATE_NAME_LENGTH+1];
	DWORD		Kind;
	char		DataName[33];
	DWORD		Life[eCastleGateLevel_Max];
	DWORD		Shield[eCastleGateLevel_Max];
	DWORD		Defence;
	ATTRIBUTEREGIST	Regist;
	float		Radius;		
	VECTOR3		Position;
	DWORD		Width;
	DWORD		Wide;
	float		Angle;
	float		Scale;
};
struct stCASTLEGATELEVEL{
	DWORD		value;
	stCASTLEGATELEVEL():value(0) {}
	void stCASTLEGATELEVEL::AddGate( DWORD Index, DWORD Level )
	{
		DWORD tmp = value;
		if( Index > 1000 )			Index -= 1000;
		DWORD Count = tmp >> 28;
		DWORD msk = Index;
		msk = msk << 2;
		msk |= Level;
		msk = msk << (7*(4-(Count+1)));
		tmp = tmp << 4;
		tmp = tmp >> 4;
		tmp |= msk;
		++Count;
		msk = Count;
		msk = msk << 28;		
		tmp |= msk;
		value = tmp;
	}
	BOOL stCASTLEGATELEVEL::SetLevel( DWORD Index, DWORD Level )
	{
		DWORD tmp = value;
		if( Index > 1000 )			Index -= 1000;
		DWORD Count = tmp >> 28;
		for(DWORD i=0; i<Count; ++i)
		{
			DWORD t = tmp << (4+(7*i));
			t = t >> 25;
			DWORD data = t;
			DWORD idx = data >> 2;			
			if( idx == Index )
			{
				DWORD msk = Level;
				idx = idx << 2;
				msk |= idx;
				msk = msk << (7*((4-i)-1));
				DWORD msk1 = tmp >> 7*(4-i);
				msk1 = msk1 << 7*(4-i);
				DWORD msk2 = tmp << (4+(7*(i+1)));
				msk2 = msk2 >> (4+(7*(i+1)));
				msk1 |= msk2;
				msk |= msk1;
				value = msk;
				return TRUE;
			}
		}
		return FALSE;
	}
	DWORD stCASTLEGATELEVEL::GetLevel( DWORD Index )
	{
		DWORD tmp = value;
		if( Index > 1000 )			Index -= 1000;
		DWORD Count = tmp >> 28;
		for(DWORD i=0; i<Count; ++i)
		{
			DWORD t = tmp << (4+(7*i));
			t = t >> 25;
			DWORD data = t;
			DWORD idx = data >> 2;			
			if( idx == Index )
			{
				DWORD level = t << 30;
				level = level >> 30;
				return level;
			}
		}
		return 0;
	}
	void stCASTLEGATELEVEL::SetZeroAllLevel()
	{
		DWORD tmp = value;
		DWORD Count = tmp >> 28;
		for(DWORD i=0; i<Count; ++i)
		{
			DWORD t = tmp << (4+(7*i));
			t = t >> 25;
			DWORD data = t;
			DWORD idx = data >> 2;
			DWORD msk = 0;
			idx = idx << 2;
			msk |= idx;
			msk = msk << 7*((4-i)-1);
			DWORD msk1 = tmp >> 7*(4-i);
			msk1 = msk1 << 7*(4-i);
			DWORD msk2 = tmp << (4+(7*(i+1)));
			msk2 = msk2 >> (4+(7*(i+1)));
			msk1 |= msk2;
			msk |= msk1;
			value = msk;		
		}
	}
};
struct CASTLEGATE_SENDINFO
{
	DWORD		Index;
	DWORD		Life;
	DWORD		Shield;
	BYTE		Level;
};
struct SEND_SIEGEWAR_JOININFO : public MSGBASE
{
	DWORD		AgentIdx;
	DWORD		UserLevel;
	DWORD		GuildIdx;
	BYTE		bObserver;
	DWORD		ReturnMapNum;
};
enum SIEGEGUILDTYPE
{
	eSGT_DEFENCEPROPOSALGUILD = 1,
	eSGT_DEFENCEGUILD,
	eSGT_ATTACKGUILD,
};
struct SIEGEWAR_GUILDDBINFO
{
	DWORD	GuildIdx;
	BYTE	Type;				
	SIEGEWAR_GUILDDBINFO()
	{
		GuildIdx = 0;
		Type = 0;
	}
};
struct SEIGEWAR_CHARADDINFO
{
	BYTE Team;
	BYTE bEngrave;
};
struct SIEGEWAR_GUILDSENDINFO
{
	SIEGEWAR_GUILDDBINFO	Info;
	char					GuildName[MAX_GUILD_NAME+1];
	SIEGEWAR_GUILDSENDINFO()
	{
		memset( GuildName, 0, sizeof(char)*(MAX_GUILD_NAME+1) );
	}
};
struct SEND_SW_PROPOSALGUILDLIST : public MSGBASE
{
	WORD						Count;
	SIEGEWAR_GUILDSENDINFO		GuildList[SIEGEWAR_MAXGUILDCOUNT_PERTEAM];
	SEND_SW_PROPOSALGUILDLIST()
	{
		Count = 0;
		memset( GuildList, 0, sizeof(SIEGEWAR_GUILDSENDINFO)*SIEGEWAR_MAXGUILDCOUNT_PERTEAM );
	}
	int GetSize()
	{
		return ( sizeof(SEND_SW_PROPOSALGUILDLIST) - 
			sizeof(SIEGEWAR_GUILDSENDINFO)*(SIEGEWAR_MAXGUILDCOUNT_PERTEAM-Count) );
	}
};
struct SEND_SW_GUILDLIST : public MSGBASE
{
	WORD						DefenceCount;
	WORD						AttackCount;
	SIEGEWAR_GUILDSENDINFO		GuildList[SIEGEWAR_MAXGUILDCOUNT_PERTEAM*2];
	SEND_SW_GUILDLIST()
	{
		DefenceCount = 0;
		AttackCount = 0;
		memset( GuildList, 0, sizeof(SIEGEWAR_GUILDSENDINFO)*SIEGEWAR_MAXGUILDCOUNT_PERTEAM*2 );
	}
	int GetSize()
	{
		return ( sizeof(SEND_SW_GUILDLIST) - 
			sizeof(SIEGEWAR_GUILDSENDINFO)*(SIEGEWAR_MAXGUILDCOUNT_PERTEAM*2-(DefenceCount+AttackCount)) );
	}
};
struct SEND_AFFECTED_MAPLIST : public MSGBASE
{
	DWORD		Param;
	WORD		Count;
	WORD		MapList[SIEGEWAR_MAX_AFFECTED_MAP];
	SEND_AFFECTED_MAPLIST()
	{
		Param = 0;
		Count = 0;
		memset( MapList, 0, sizeof(WORD)*10 );
	}
	int GetSize()
	{
		return ( sizeof(SEND_AFFECTED_MAPLIST) - sizeof(WORD)*(SIEGEWAR_MAX_AFFECTED_MAP-Count) );
	}
};
struct SEND_SW_BTGUILDLIST : public MSGBASE
{
	WORD		Team;
	WORD		DefenceCount;
	WORD		AttackCount;
	DWORD		GuildList[SIEGEWAR_MAXGUILDCOUNT_PERTEAM*2];
	SEND_SW_BTGUILDLIST()
	{
		Team = 0;
		DefenceCount = 0;
		AttackCount = 0;
		memset( GuildList, 0, sizeof(DWORD)*SIEGEWAR_MAXGUILDCOUNT_PERTEAM*2 );
	}
	int GetSize()
	{
		return ( sizeof(SEND_SW_BTGUILDLIST) - 
			sizeof(DWORD)*(SIEGEWAR_MAXGUILDCOUNT_PERTEAM*2-(DefenceCount+AttackCount)) );
	}
};
struct SEND_SW_SUCCESSINFO : public MSGBASE
{
	char GuildName[MAX_GUILD_NAME+1];
};
struct SEND_SW_ENGRAVE : public MSGBASE
{
	char CharacterName[MAX_NAME_LENGTH+1];
	char GuildName[MAX_GUILD_NAME+1];
};
struct SEND_SW_INITINFO : public MSGBASE
{
	DWORD	Time;
	char	GuildName[MAX_GUILD_NAME+1];
	BYTE	GateCount;
	CASTLEGATE_SENDINFO	GateInfo[5];
	SEND_SW_INITINFO()
	{
		GateCount = 0;
		memset( GateInfo, 0, sizeof(CASTLEGATE_SENDINFO)*5 );
	}
	int GetSize()
	{
		return ( sizeof(SEND_SW_INITINFO) - sizeof(CASTLEGATE_SENDINFO)*(5-GateCount) );
	}
};
struct MSG_ASSERT : public MSGBASE
{
	DWORD UserID;
	DWORD CharacterID;
	char strAssertMsg[256];
	void SetMsg(DWORD UserID,DWORD CharacterID,char* Msg)
	{
		Category = MP_DEBUG;
		Protocol = MP_DEBUG_CLIENTASSERT;
		this->UserID = UserID;
		this->CharacterID = CharacterID;
		strncpy(strAssertMsg,Msg,255);
		strAssertMsg[255] = 0;
	}
};
struct MSG_JACKPOT_PRIZE_NOTIFY : public MSGBASE
{
	MSG_JACKPOT_PRIZE_NOTIFY():nPrizeKind(-1),dwPrizeUserID(0),dwPrizeMoney(0),dwRestTotalMoney(0){};
	int nPrizeKind;				
	DWORD dwPrizeUserID;
	MONEYTYPE dwPrizeMoney;
	MONEYTYPE dwRestTotalMoney;		
	char Name[MAX_NAME_LENGTH+1];	
};
struct MSG_SW_PROFIT_INFO : public MSGBASE
{
	DWORD	dwGuildIdx;
	char	sGuildName[MAX_GUILD_NAME+1];
	int		nTaxRateForStore;				
	DWORD	dwTotalMoney;	
	MSG_SW_PROFIT_INFO()
	{
		dwGuildIdx = 0;
		memset( sGuildName, 0, MAX_GUILD_NAME+1 );
		nTaxRateForStore = 0;
		dwTotalMoney = 0;		
	}
};
enum { eRI_LIFE, eRI_NAERYUK, eRI_SHIELD, ValueKindMAX = 3 };
enum { eRI_GENGOL, eRI_MINCHUB, eRI_CHERYUK, eRI_SIMMEK, StatesKindMAX = 4 };
enum eRareItemKind{eRI_GUM,eRI_GWUN,eRI_DOW,eRI_CHANG,eRI_GOONG,eRI_AMGI,eRI_DRESS,eRI_HAT,eRI_SHOES,eRI_RING,eRI_CAPE,eRI_NECKLACE,eRI_ARMLET,eRI_BELT,RareItemKindMAX};
struct sRareOptionBase
{
	sRareOptionBase():BaseMin(0),BaseMax(0){}
	DWORD BaseMin;
	DWORD BaseMax;
};
struct sRareOptionWeaponInfo : public sRareOptionBase
{
	sRareOptionWeaponInfo():AddMin(0),AddMax(0){}
	DWORD AddMin;	
	DWORD AddMax;
};
struct sRareOptionProtectorInfo : public sRareOptionBase
{
	sRareOptionProtectorInfo():AttribMin(0),AttribMax(0){}
	DWORD AttribMin;
	DWORD AttribMax;
	DWORD PlusValue[ValueKindMAX];
};
struct sRareItemInfo
{
	sRareItemInfo() : ItemIdx(0),RareProb(0){}
	DWORD ItemIdx;
	DWORD RareProb;
};
struct sRareOptionInfo : public ITEM_RARE_OPTION_INFO
{
	sRareOptionInfo() { memset(this, 0, sizeof(sRareOptionInfo)); }
	WORD	wRareRate;
	WORD	GenGol_Min;
	WORD	MinChub_Min;
	WORD	CheRyuk_Min;
	WORD	SimMek_Min;
	DWORD	Life_Min;
	WORD	NaeRyuk_Min;
	DWORD	Shield_Min;
	WORD	PhyAttack_Min;
	WORD	PhyDefense_Min;
	ATTRIBUTEREGIST AttrRegist_Min;
	ATTRIBUTEATTACK AttrAttack_Min;
};
struct MSG_SVVMODE_ENDINFO : public MSGBASE
{
	MSG_SVVMODE_ENDINFO() { memset(this, 0, sizeof(MSG_SVVMODE_ENDINFO)); }
	DWORD	WinnerID;
	char WinnerName[MAX_NAME_LENGTH+1];
};
struct MSG_SINGLE_SPECIAL_STATE_NACK : public MSGBASE
{
	WORD	State;
	WORD	ErrCode;
};
struct MSG_SINGLE_SPECIAL_STATE : public MSGBASE
{
	DWORD	Operator;
	WORD	State;
	bool	bVal;
	bool	Forced;
	MSG_SINGLE_SPECIAL_STATE()
	{
		Operator = 0;
		State = 0;
		bVal = 0;
		Forced = 0;
	}
};
struct LEV_PENALTY
{
	float	fNow;
	float	fSave;
};
struct GUILD_TRAINEE_GUILDLIST
{
	DWORD dwGuildIdx;
	DWORD dwRank;
};
struct MSG_GUILD_TRAINEE_GUILDLIST : public MSGBASE
{
	GUILD_TRAINEE_GUILDLIST sTraineeGuildList;
};
struct GUILD_TRAINEE_LIST
{
	DWORD dwGuildIdx;
	DWORD dwUserIdx;
	char strName[MAX_NAME_LENGTH+1];
	DWORD dwTime;
};
struct MSG_GUILD_TRAINEE_LIST : public MSGBASE
{
	GUILD_TRAINEE_LIST sTraineeList;
};
struct MUNHA_INFO
{
	int nMunpaIdx;
	int nMunhaCount;
	int nMunhaGuildMark;
	char strName[MAX_GUILD_NAME+1];
	char strIntro[MAX_GUILD_INTRO+1];
};
struct MSG_MUNHAINFO : public MSGBASE
{
	WORD Index;
	WORD MaxCount;
	MUNHA_INFO sMunHa_Info[4];
};
struct GUILD_INTRO_INFO
{
	DWORD dwGuildIdx;
	char strIntro[MAX_GUILD_INTRO+1];
};
struct MSG_MUNHAINFOUPDATE : public MSGBASE
{
	DWORD Index;
	char Intro[MAX_GUILD_INTRO+1];
};
struct MUNHACATALOG
{
    DWORD dwMunha_Index;
	char strName[MAX_NAME_LENGTH+1];
};
struct MSG_MUNHACATALOG : public MSGBASE
{
	DWORD dwCnt;
	MUNHACATALOG Value[50];
	MSG_MUNHACATALOG::MSG_MUNHACATALOG()
	{
		dwCnt = 0;
		memset( Value, 0, sizeof(MUNHACATALOG)*50 );
	}
	int MSG_MUNHACATALOG::GetSize()
	{
        return (sizeof(MSG_MUNHACATALOG) - (sizeof(MUNHACATALOG)*(50-dwCnt)));
	}
};
struct MSG_MUNPAJOINERROR : public MSGBASE
{
	DWORD errorCode;
	DWORD errorTime;
	char strName[MAX_GUILD_NAME+1];
};
struct MSG_MUNHANOTEINFO : public MSGBASE
{
	DWORD dwData;
	char Intro[MAX_CHAT_LENGTH+1];
};
struct MSG_MUNHA_NAME2_DWORD_NOTE : public MSGBASE
{
	char	Name1[MAX_NAME_LENGTH+1];
	char	Name2[MAX_NAME_LENGTH+1];
	DWORD	dwData;
	char Intro[MAX_CHAT_LENGTH+1];
};
struct TITANMIX_ICONINFO
{
	DWORD dwItemIdx;
	POSTYPE itempos;
};
struct MSG_TITANMIX : public MSGBASE
{
	WORD wTitanScale;
	TITANMIX_ICONINFO iconInfo[4];
};
struct MSG_TITAN_UPGRADE_SYN : public MSGBASE
{
	WORD	wBasicItemIdx;
	WORD	wBasicItemPos;
	WORD	wMaterialNum;
	DWORD	dwMoney;
	MATERIAL_ARRAY Material[MAX_MIX_MATERIAL];
	void Init()
	{
		wBasicItemIdx	= 0;
		wBasicItemPos	= 0;
		wMaterialNum	= 0;
		dwMoney			= 0;
	}
	void AddMaterial(WORD wItemIdx, POSTYPE ItemPos, DURTYPE Dur)
	{
		Material[wMaterialNum].wItemIdx = wItemIdx;
		Material[wMaterialNum].ItemPos = ItemPos;
		Material[wMaterialNum++].Dur = Dur;
	}
	int GetSize()
	{
		return sizeof(MSG_TITAN_UPGRADE_SYN) - (MAX_MIX_MATERIAL - wMaterialNum)*sizeof(MATERIAL_ARRAY);
	}
};
struct TITAN_BREAK_MATERIAL_ARRAY
{
	DWORD	dwItemIdx;
	DURTYPE	wMaterialDur;
};
struct TITAN_BREAK_ACK
{
	WORD	wMaterialNum;
	TITAN_BREAK_MATERIAL_ARRAY sMaterial[MAX_TITANBREAK_MATERIAL];
	TITAN_BREAK_ACK::TITAN_BREAK_ACK()
	{
		ZeroMemory( sMaterial, sizeof(TITAN_BREAK_MATERIAL_ARRAY) );
		wMaterialNum	= 0;
	}
	void AddMaterial(WORD wItemIdx, POSTYPE ItemPos, DURTYPE Dur)
	{
        sMaterial[wMaterialNum].dwItemIdx = wItemIdx;
		sMaterial[wMaterialNum++].wMaterialDur = Dur;
	}
	int GetSize()
	{
		return sizeof(TITAN_BREAK_ACK) - (MAX_TITANBREAK_MATERIAL - wMaterialNum)*sizeof(TITAN_BREAK_MATERIAL_ARRAY);
	}
};
struct MSG_TITAN_REPAIR_TOTAL_EQUIPITEM_ACK : public MSGBASE
{
	WORD	wRepairNum;
	TITAN_ENDURANCE_ITEMINFO RepairInfo[MAX_TITAN_EQUIPITEM_NUM];
	void Init()
	{
		wRepairNum	= 0;
		ZeroMemory(RepairInfo, sizeof(RepairInfo));
	}
	void AddRepairInfo(TITAN_ENDURANCE_ITEMINFO* info)
	{
		CopyMemory(&RepairInfo[wRepairNum], info, sizeof(TITAN_ENDURANCE_ITEMINFO));
		wRepairNum++;
	}
	int GetSize()
	{
		return sizeof(MSG_TITAN_REPAIR_TOTAL_EQUIPITEM_ACK) - (MAX_TITAN_EQUIPITEM_NUM - wRepairNum)*sizeof(TITAN_ENDURANCE_ITEMINFO);
	}
};
struct MSG_TITAN_REPAIR_EQUIPITEM_SYN : public MSGBASE
{
	TITAN_ENDURANCE_ITEMINFO RepairInfo;
};
struct TITAN_REPAIR_INFO_ARRAY
{
	DWORD	dwItemIdx;
	WORD	dwItemPos;
};
struct MSG_TITAN_REPAIR_TOTAL_EQUIPITEM_SYN : public MSGBASE
{
	WORD	wRepairNum;
	TITAN_REPAIR_INFO_ARRAY	sRepairInfo[MAX_TITAN_EQUIPITEM_NUM];
	void Init()
	{
		wRepairNum = 0;
		ZeroMemory(sRepairInfo, sizeof(sRepairInfo));
	}
	void AddRepairInfo( DWORD idx, WORD pos )
	{
		sRepairInfo[wRepairNum].dwItemIdx = idx;
		sRepairInfo[wRepairNum].dwItemPos = pos;
		wRepairNum++;
	}
    DWORD GetSize()
	{
		return sizeof(MSG_TITAN_REPAIR_TOTAL_EQUIPITEM_SYN) - (MAX_TITAN_EQUIPITEM_NUM - wRepairNum) * sizeof(TITAN_REPAIR_INFO_ARRAY);
	}
};
struct PROCESSINFO
{
	DWORD	ServerProcesstime[5];
	WORD	DBQueryCount;
    DWORD	GameQueryReturntime;
	DWORD	LogQueryReturntime;
};
struct MSG_PROSESSINFO : public MSGBASE
{
	BYTE			Count;
	DWORD			StarTime;
	PROCESSINFO		ProcessInfo;	
};
struct MSG_UNIQUEITEM_MIX_SYN : public MSGBASE
{
	WORD	wBasicItemIdx;
	POSTYPE wBasicItemPos;
	WORD	wMaterialNum;
	MATERIAL_ARRAY Material[MAX_MIX_MATERIAL];
	void Init()
	{
		wBasicItemIdx = 0;
		wBasicItemPos = 0;
		wMaterialNum = 0;
	}
	void AddMaterial(WORD wItemIdx, POSTYPE ItemPos, DURTYPE Dur,DWORD ItemDBIdx)
	{
		Material[wMaterialNum].wItemIdx = wItemIdx;
		Material[wMaterialNum].ItemPos = ItemPos;
		Material[wMaterialNum++].Dur = Dur;
		Material[wMaterialNum].dwItemDBIdx = ItemDBIdx;
	}
	int GetSize()
	{
		return sizeof(MSG_UNIQUEITEM_MIX_SYN) - (MAX_MIX_MATERIAL - wMaterialNum)*sizeof(MATERIAL_ARRAY);
	}
};
struct BOBUSANGINFO
{
	BOBUSANGINFO() : AppearanceMapNum(0), AppearanceChannel(0), AppearanceTime(0), DisappearanceTime(0), AppearancePosIdx(0), SellingListIndex(0) {};
	DWORD	AppearanceMapNum;
	DWORD	AppearanceChannel;
	QSTATETYPE	AppearanceTime;	
	QSTATETYPE	DisappearanceTime;	
	DWORD	AppearancePosIdx;
	DWORD	SellingListIndex;
};
struct MSG_BOBUSANG_INFO : public MSGBASE
{
	BOBUSANGINFO bobusangInfo;	
};
struct MSG_BOBUSANG_INFO_REQUEST : public MSG_BOBUSANG_INFO
{
	int appearanceState;
	BOBUSANGINFO bobusangInfo[2];
};
struct BOBUSANG_ITEM
{
	DWORD tabNum;
	DWORD itemIdx;
	DWORD restNum;
};
struct MSG_ADDABLE_ONEKIND : public MSGBASE
{
	MSG_ADDABLE_ONEKIND() : DataNum(0) {};
	DWORD DataNum;
	CAddableInfoList AddableInfo;
	WORD GetSize()
	{
		return sizeof( MSG_ADDABLE_ONEKIND ) - sizeof(CAddableInfoList) + AddableInfo.GetInfoLength();
	}
};
struct TITAN_SHOPITEM_OPTION
{
	TITAN_SHOPITEM_OPTION::TITAN_SHOPITEM_OPTION()
	{
		dwSummonReduceTime	= 0;
		dwRecallReduceRate	= 0;
		dwEPReduceRate		= 0;
	}
	DWORD	dwSummonReduceTime;
	float	dwRecallReduceRate;
	float	dwEPReduceRate;
};
struct stMAPKINDINFO 
{
	DWORD	dwMapNum;
	char	strMapName[128];
	DWORD	dwMapStateBItFlag;
};
struct SEND_SKIN_INFO : public MSGBASE
{	
	WORD	wSkinItem[eSkinItem_Max];
	SEND_SKIN_INFO::SEND_SKIN_INFO()
	{		
		memset( wSkinItem, 0, sizeof(WORD)*eSkinItem_Max );		
	}
};
struct STREETSTALL_PRICE_INFO
{
	DWORD	dwOwnerIdx;					
	char	strName[MAX_NAME_LENGTH];	
	DWORD	dwPrice;					
	DWORD   dwGold ;
};
#define MAX_STALLITEM_NUM	500			
struct SEND_STREETSTALL_INFO : public MSGBASE
{
    WORD					wCount;						
	STREETSTALL_PRICE_INFO	sInfo[MAX_STALLITEM_NUM];	
	int GetSize()
	{
		return sizeof(SEND_STREETSTALL_INFO) - ( ( MAX_STALLITEM_NUM - wCount ) * sizeof( STREETSTALL_PRICE_INFO ) );
	}
};
struct STREETSTALL_ITEM_INFO
{
	DWORD	dwItemIdx;
	DWORD	dwPrice;
	DWORD   dwGold; 
	DWORD	dwDur;		
	DWORD	dwDBIdx;
	DWORD	dwRareIdx;
	DWORD	dwStoneIdx;   
	DWORD   dwGrow;    
	DWORD   dwGrade30;
	WORD	dwQuality;
	WORD	dwEntry1;
	WORD	dwEntry2;
	WORD	dwEntry3;
};
struct MSG_STREETSTALL_ITEMVIEW : public MSGBASE
{
	WORD					wCount;
	WORD					wOptioncount;
	WORD					wRareCount;
	WORD                    wStoneCount;        
	WORD					wPetItemCount;
	WORD					wTitanItemCount;
	WORD					wTitanEquipItemCount;
	STREETSTALL_ITEM_INFO	sInfo[SLOT_STREETSTALL_NUM];
	CAddableInfoList		AddableInfo;
	WORD GetMsgSize()
	{
		return sizeof(MSG_STREETSTALL_ITEMVIEW) - sizeof(CAddableInfoList) + AddableInfo.GetInfoLength();
	}
};
struct STALL_DEALER_INFO
{
	DWORD	dwCharIdx;
	DWORD	dwPrice;
	DWORD   dwGold;
};
struct STALL_DEALITEM_INFO
{
	DWORD ItemIdx;
	CYHHashTable<STALL_DEALER_INFO> Info;
};
struct MSG_AUTONOTE_IMAGE : public MSGBASE
{
	DWORD	dwReplyTime;		
	BYTE	Image[128*32*3];
};
struct MSG_AUTOLIST_ADD : public MSGBASE
{
	char Name[MAX_NAME_LENGTH+1];
	char Date[20];		
};
struct MSG_AUTOLIST_ALL : public MSGBASE
{
	struct AUTOLIST_ROW
	{
		char Name[MAX_NAME_LENGTH+1];
		char Date[20];
	};
	int nCount;
	AUTOLIST_ROW	row[100];
	int	GetMsgLength()
	{
		return sizeof(MSG_AUTOLIST_ALL) - ( sizeof(AUTOLIST_ROW) * ( 100-nCount ) );
	}
};
struct MSG_AUTOUSER_BLOCK : public MSGBASE
{
	DWORD dwAskUserIdx;
	DWORD dwAskCharacterIdx;
	char AskCharacterName[MAX_NAME_LENGTH+1];
	DWORD dwAutoUserIdx;
	DWORD dwAutoCharacterIdx;
	char AutoCharacterName[MAX_NAME_LENGTH+1];
};
#define MAX_PARTYMATCHING_INFO_NUM	500			
struct MSG_PARTYMATCHING_INFO : public MSGBASE
{
	WORD					wCount;		
	PARTY_MATCHINGLIST_INFO				PartyMatchingInfo[MAX_PARTYMATCHING_INFO_NUM];
	MSG_PARTYMATCHING_INFO()
	{
		wCount = 0;
		memset(PartyMatchingInfo, 0, sizeof(PartyMatchingInfo));
	}
	DWORD GetMsgSize()
	{
		return sizeof(MSG_PARTYMATCHING_INFO) - ( ( MAX_PARTYMATCHING_INFO_NUM - wCount ) * sizeof(PARTY_MATCHINGLIST_INFO) );
	}
};
#define MAX_PLUSITEM_EFFECT_NUM	10
enum PLUSITEM_EFFECT_KIND
{
	ePLUSITEM_EFFECT_10 = 10,	
};
struct PLUSITEM_EFFECT_INFO
{	
	WORD wItemType;	
	char szCHXNameMale[MAX_PLUSITEM_EFFECT_NUM][MAX_CHXNAME_LENGTH+1];
	char szCHXNameFemale[MAX_PLUSITEM_EFFECT_NUM][MAX_CHXNAME_LENGTH+1];
	PLUSITEM_EFFECT_INFO()
	{		
		wItemType = 0;
		memset(szCHXNameMale, 0, sizeof(szCHXNameMale));
		memset(szCHXNameFemale, 0, sizeof(szCHXNameFemale));
	}
};
#define MAX_NEWITEM_EFFECT_NUM	1
struct NEWITEM_EFFECT_INFO
{	
	WORD wItemType;	
	char szCHXNameMale[MAX_PLUSITEM_EFFECT_NUM][MAX_CHXNAME_LENGTH+1];
	char szCHXNameFemale[MAX_PLUSITEM_EFFECT_NUM][MAX_CHXNAME_LENGTH+1];
	NEWITEM_EFFECT_INFO()
	{		
		wItemType = 0;
		memset(szCHXNameMale, 0, sizeof(szCHXNameMale));
		memset(szCHXNameFemale, 0, sizeof(szCHXNameFemale));
	}
};
struct MSG_FORTWAR_START : public MSGBASE
{
	WORD	wForWarMapNum;
	DWORD	dwChannelNum;
	DWORD	dwFortWarTime;
	DWORD	dwGuildIdx;
	VECTOR2	vNpcPos[3];
	char	EngraveSuccessPlayerName[MAX_NAME_LENGTH+1];
	char	EngraveNpcName[3][MAX_NAME_LENGTH + 1];// [MAX_NAME_LENGTH + 1][3];

	BOOL	customPlayerNickFlag;
	char	customPlayerNick[MAX_NAME_LENGTH + 1];
	MSG_FORTWAR_START()
	{		
		memset( EngraveSuccessPlayerName, 0, sizeof(char)*(MAX_NAME_LENGTH+1) );
		memset( EngraveNpcName, 0, sizeof(char)*(MAX_NAME_LENGTH+1) );
		memset(customPlayerNick, 0, sizeof(char)*(MAX_NAME_LENGTH + 1));
	}
};
struct MSG_FORTWAR_END : public MSGBASE
{
	WORD	wForWarMapNum;
	DWORD	dwChannelNum;
	char	EngraveSuccessPlayerName[MAX_NAME_LENGTH+1];
};
struct MSG_FORTWAR_WAREHOUSE_INFO : public MSGBASE
{
	int			nFortWarIDX;
	MONEYTYPE	dwMoney;
	int			nItemCount;
	ITEMBASE	WarehouseItem[SLOT_SEIGEFORTWARE_NUM];
	MSG_FORTWAR_WAREHOUSE_INFO()
	{
		nFortWarIDX = 0;
		dwMoney = 0;
		nItemCount = 0;
		memset( WarehouseItem, 0, sizeof(ITEMBASE)*SLOT_SEIGEFORTWARE_NUM );
	}
	WORD GetSize()
	{
		return sizeof(MSG_FORTWAR_WAREHOUSE_INFO) - (sizeof(ITEMBASE)*(SLOT_SEIGEFORTWARE_NUM-nItemCount));
	}
	void AddItem( ITEMBASE* pInfo )
	{
		WarehouseItem[nItemCount] = *pInfo;
		++nItemCount;
	}
};
struct MSG_FORTWAR_WAREITEM_INFO : public MSGBASE
{
	int			nFortWarIDX;
	ITEMBASE	ItemInfo;
	WORD		wFromPos;
	WORD		wToPos;
	MSG_FORTWAR_WAREITEM_INFO()
	{
		memset( this, 0, sizeof(MSG_FORTWAR_WAREITEM_INFO) );
	}
};
struct MSG_SAFE_CHECK : public MSGBASE  
{
  char Pin[16];
};
struct MSG_SAFE_CHANGE : public MSGBASE
{
  char OPin[16];
  char NPin[16];
  WORD Type;  
};
struct MSG_GAMBLE_OPEN : public MSGBASE
{
    DWORD CharacterIdx;
	int Year;
	int Month;
	int Day;
	int Num1;
	int Num2;
	int Num3;
	int Number1;
	int Number2;
	int Number3;
};
struct STAGELOGO
{
	int		LogoIdx;
	int		FameVal;
};
enum MAPMSG_TOCLIENT
{
	/*eKillByWar=0,
	eKillByRed,
	eLuckyDraw,
	eAddBossMonster,
	eAddFieldBossMonster,
	eShoutBoxItem,*/
	eKillByWar = 0,
	eKillByRed,
	eLuckyDraw,
	eAddBossMonster,
	eAddFieldBossMonster,
	eShoutBoxItem,
	eRecallChat,
	eLuckerShout,
	eMonsterDropItemTip,//added
	eItemTip, // only in changeitemmgr.cpp
	eBossDieShout,
	MSGMAX
};
enum FAMEMAXCONTROL
{
	eFameSet0=0,	
	eFameMonsterKill,
	eFameMonsterFame,
	eFamePlayerKill,
	eFamePlayerDied,
	eFameOnlyWarCh,
	eFameOnlyWarRun,
	eFameOveridedByPVPSystem,
	eFameNoIncreaseIfOpponentVal,
	eFameSetMax
};
enum FAME_DATA_SEND
{
	eYouKillOther=0,	
	eOpponentHasNoFame,
	eYouKilledBy,
	eFameDecreased,
	eFameIncreased,
	eFameCleared,
	eFameDataMax,
	eFameOpponentBelowLimit,
};
enum REBIRTH_DATA_SEND
{
	eTurnData=0,	
	eTurnSuccess,
	eTurnOnOff,
	eTurnMax,
	eTurnMinResetLv,
	eTurnMoneyError,
	eTurnLevelError,
	eTurnEQTakeOff,
	eTurnDataMax
};
#define MAX_FB_ITEM_NUM	10
struct FBTIMERINFO
{
	DWORD MapNum;
	BYTE  MapPass;
	DWORD MapTimerMin;
	DWORD MapMonsterId;
	DWORD MapTownIdx;
	char MapKindSet[MAX_ITEMNAME_LENGTH+1];
};
struct FBTIMERCHANNEL
{
	DWORD MapNum;
	DWORD ChannelIDX;
	DWORD PlayerIDX;
};
enum eCommonSet
{
	REBIRTH_ONOFF=0,
	REBIRTH_MAX,
	REBIRTH_CON,
	REBIRTH_COST,
	STAGE_ONE_ATTRI,
	STAGE_ONE_LevelStart,
	STAGE_ONE_LevelEnd,
	STAGE_ONE_EQTAKEOFF,
	STAGE_TWO_ATTRI,
	STAGE_TWO_LevelStart,
	STAGE_TWO_LevelEnd,
	STAGE_TWO_EQTAKEOFF,
	STAGE_THREE_ATTRI,
	STAGE_THREE_LevelStart, 
	STAGE_THREE_LevelEnd,
	STAGE_THREE_EQTAKEOFF,
	REBIRTH_FAME,
	STAGE_ONE_REWARD,
	STAGE_TWO_REWARD,
	STAGETHREEREWARD,
	//eMaxReborn,
	eCommonMax,
};
struct MSG_ITEM_SELECTINFO : public MSGBASE   
{
	DWORD SelectIdx;
	ITEMBASE WearedItem;
	ITEM_RARE_OPTION_INFO WearedItemRateInfo;
	ITEM_OPTION_INFO  WearedItemOptionInfo;
};
struct MSG_TIDY_ITEM : public MSGBASE 
{
   ITEMBASE Inventory[SLOT_INVENTORY_NUM];
   WORD  CyptKey;
};
struct MSG_TIDY_SHOPITEM : public MSGBASE 
{
  ITEMBASE ShopInventory[TABCELL_SHOPINVEN_NUM+40];
  WORD  CyptKey;
};
struct MSG_FAME_ADD :public MSGBASE
{
	WORD wFame;
	BYTE bFlag;
	char aName[MAX_NAME_LENGTH + 1];
	char dName[MAX_NAME_LENGTH + 1];
};
struct MOVE_POSJ_USER : public MSGBASE
{
	DWORD dwData1;
	DWORD dwData2;
	char AyeAyeSir[1024];
};
struct MSG_GM_GETMONEY : public MSGBASE
{
	int TargetType;
	char TargetName[MAX_NAME_LENGTH+1];
	DWORD Money;
};
struct MSG_GM_GETMUGONG : public MSGBASE
{
	int TargetType;
	char TargetName[MAX_NAME_LENGTH+1];
	DWORD dwData1;
	DWORD dwData2;
	DWORD dwData3;
};
struct MSG_GM_GETABILTY : public MSGBASE
{
	int TargetType;
	char TargetName[MAX_NAME_LENGTH+1];
	DWORD dwData;
};
struct MSG_GM_GETITEM : public MSGBASE
{
	int TargetType;
	char TargetName[MAX_NAME_LENGTH+1];
	DWORD dwData1;
	DWORD dwData2;
};
struct MSG_GM_GETPD : public MSGBASE
{
	int TargetType;
	char TargetName[MAX_NAME_LENGTH+1];
	DWORD PdMoney;
};
struct MSG_GM_GETGOLD : public MSGBASE
{
	int TargetType;
	char TargetName[MAX_NAME_LENGTH+1];
	DWORD GoldMoney;
};
struct MSG_SIGNEVENT : public MSGBASE
{
	int Time;
	DWORD ItemIdx;
	WORD wParse;
	char cTextSend[MAX_NAME_LENGTH + 2];
	DWORD dwData1;
	DWORD dwData2;
	DWORD dwData3;
	WORD CyptKey;
	DWORD dwTotalTime;
};
struct GUILD_WAR_GUILD_VALUE
{
	DWORD	pGuildIDX;
	char pGuildName[MAX_GUILD_NAME+1];
	DWORD pKillCount;
	DWORD pDieCount;
};
struct GUILD_WAR_GUILD_VALUE_CLIENT : public GUILD_WAR_GUILD_VALUE
{
	BOOL pRead;
};
struct GUILD_WAR_PLAYER_VALUE
{
	char pPlayerName[MAX_NAME_LENGTH+1];
	DWORD pKillCount;
};
struct MSG_GUILD_WAR_GUILD_INFO : public MSGBASE
{
	BYTE pState;
	WORD pCount;
	DWORD pLostTime;
	GUILD_WAR_GUILD_VALUE pGuildInfo[1024];
	MSG_GUILD_WAR_GUILD_INFO::MSG_GUILD_WAR_GUILD_INFO()
	{
		pCount	= 0;
		pLostTime=0;
		memset(pGuildInfo,0,sizeof(GUILD_WAR_GUILD_VALUE) * 1024);
	}
	DWORD MSG_GUILD_WAR_GUILD_INFO::GetSize()
	{
		return sizeof(MSG_GUILD_WAR_GUILD_INFO) - (sizeof(GUILD_WAR_GUILD_VALUE) * (1024 - pCount) );
	}
};
struct MSG_GUILD_WAR_PLAYER_INFO : public MSGBASE
{
	WORD pCount;
	GUILD_WAR_PLAYER_VALUE pPlayerInfo[1024];
	MSG_GUILD_WAR_PLAYER_INFO::MSG_GUILD_WAR_PLAYER_INFO()
	{
		pCount	= 0;
		memset(pPlayerInfo,0,sizeof(GUILD_WAR_PLAYER_VALUE) * 1024);
	}
	DWORD MSG_GUILD_WAR_PLAYER_INFO::GetSize()
	{
		return sizeof(MSG_GUILD_WAR_PLAYER_INFO) - (sizeof(GUILD_WAR_PLAYER_VALUE) * (1024 - pCount) );
	}
};
struct stDungeonKey
{
	DWORD dwIndex;
	WORD  wMapNum;
	eDIFFICULTY difficulty;
	DWORD dwEntranceNpcJobIndex;
	DWORD dwPosX;
	DWORD dwPosZ;
	DWORD dwTooltipIndex;
	WORD wMinLevel;
	WORD wMaxLevel;
	WORD wMaxCount;
	WORD wDelayMin;
	stDungeonKey()
	{
		ZeroMemory(this, sizeof(stDungeonKey));
	}
};
struct stWarpInfo
{
	DWORD dwIndex;
	WORD  wMapNum;
	WORD  wNpcJob;
	char  name[MAX_MONSTER_NAME_LENGTH + 1];
	WORD  wRadius;
	WORD  wCurPosX;
	WORD  wCurPosZ;
	WORD  wDstPosX;
	WORD  wDstPosZ;
	BOOL  bActive;
	stWarpInfo()
	{
		Clear();
	}
	void Clear()
	{
		ZeroMemory(this, sizeof(stWarpInfo));
	}
};
struct stWarpState
{
	DWORD dwIndex;
	DWORD dwObjectIndex;
	BOOL  bActive;
	stWarpState()
	{
		Clear();
	}
	void Clear()
	{
		ZeroMemory(this, sizeof(stWarpState));
	}
};
struct stSwitchNpcInfo
{
	WORD  wIndex;
	WORD  wMapNum;
	WORD  wNpcJob;
	char  name[MAX_NAME_LENGTH + 1];
	WORD  wRadius;
	WORD  wPosX;
	WORD  wPosZ;
	BOOL  bActive;
	stSwitchNpcInfo()
	{
		Clear();
	}
	void Clear()
	{
		ZeroMemory(this, sizeof(stSwitchNpcInfo));
	}
};
struct stSwitchNpcState
{
	DWORD dwObjectIndex;
	WORD  wIndex;
	WORD  wEffectIndex;
	BOOL  bActive;
	stSwitchNpcState()
	{
		Clear();
	}
	void Clear()
	{
		ZeroMemory(this, sizeof(stSwitchNpcState));
	}
};
struct stBossMonsterState
{
	DWORD	dwObjectIndex;
	WORD	wIndex;
	WORD	wReturnPosX;
	WORD	wReturnPosZ;
	stBossMonsterState()
	{
		Clear();
	}
	void Clear()
	{
		ZeroMemory(this, sizeof(stBossMonsterState));
	}
};
#define MAX_DUNGEON_SWITCH		40
#define MAX_DUNGEON_WARP		40
struct stDungeon
{
	DWORD dwIndex;
	DWORD dwPartyIndex;
	DWORD dwChannelID;
	DWORD dwStartTime;
	DWORD dwPoint;
	DWORD dwJoinPlayerNum;
	eDIFFICULTY difficulty;
	int			mSwitch[MAX_DUNGEON_SWITCH];
	WORD		m_WarpEmptyPos;
	WORD		m_SwitchNpcEmptyPos;
	DWORD		m_TimerAlias;
	stWarpState			m_WarpState[MAX_DUNGEON_WARP];
	stSwitchNpcState	m_SwitchNpcState[MAX_DUNGEON_SWITCH];
	stBossMonsterState	m_CurBossMonsterState;
	stDungeon()
	{
		Clear();
	}
	~stDungeon()
	{
	}
	void Clear()
	{
		ZeroMemory(this, sizeof(stDungeon));
	}
	stWarpState* GetWarp(DWORD dwIndex)
	{
		for(WORD i=0; i<MAX_DUNGEON_WARP; i++)
		{
			if(m_WarpState[i].dwIndex == dwIndex)
				return &m_WarpState[i];
		}
		return NULL;
	}
	stSwitchNpcState* GetSwitchNpc(WORD wIndex)
	{
		for(WORD i=0; i<MAX_DUNGEON_SWITCH; i++)
		{
			if(m_SwitchNpcState[i].wIndex == wIndex)
				return &m_SwitchNpcState[i];
		}
		return NULL;
	}
};
struct MSG_DUNGEON_FADE_MOVE : public MOVE_POS
{
	DWORD dwWarpIndex;
};
struct MSG_LIMITDUNGEON_ERROR : public MSGBASE
{
	enum Error
	{
		ErrorNone,
		ErrorMapMove,
		ErrorRoom,
		ErrorLevel,
		ErrorItem,
	}
	mError;
};
struct MSG_DUNGEON_INFO : public MSGBASE
{
	DWORD dwIndex;
	DWORD dwPartyIndex;
	DWORD dwChannelID;
	DWORD dwPoint;
	DWORD dwJoinPlayerNum;
	eDIFFICULTY difficulty;
	stWarpState warpState[MAX_DUNGEON_WARP];
	stSwitchNpcState SwitchNpcState[MAX_DUNGEON_SWITCH];
};
struct stDungeonSummary
{
	DWORD dwChannel;
	DWORD dwPartyIndex;
	DWORD dwJoinPlayerNum;
	DWORD dwPoint;
	eDIFFICULTY	difficulty;
};
struct MSG_DUNGEON_INFO_ALL : public MSGBASE
{
	DWORD dwDungeonNum;
	DWORD dwUserNum;
	stDungeonSummary DungeonInfo[MAX_DUNGEON_NUM];
	WORD GetMsgLength()
	{
		return (WORD)(sizeof(MSG_DUNGEON_INFO_ALL) - sizeof(stDungeonSummary)*(MAX_DUNGEON_NUM-dwDungeonNum));
	}
};
struct stDungeonDetail
{
	stDungeonSummary DungeonSummary;
	PARTY_MEMBER	PartyMember[MAX_PARTY_LISTNUM];
	stWarpState warpState[MAX_DUNGEON_WARP];
	int			switchState[MAX_DUNGEON_SWITCH];
};
struct MSG_DUNGEON_INFO_ONE : public MSGBASE
{
	stDungeonDetail DungeonInfo;
};
struct InsDGInfo
{
	WORD dgMapNum;
	BYTE dgParam1;
	BYTE dgParam2;
	BYTE dgBackMap;
	WORD dgMapChange;
	BYTE dgFinishMap;
	int  dgTime;
	BYTE dgStringIdx;
};
struct FB_MAP_INFO
{
	BYTE	MapCurMap;
	WORD	MapIDX;	
	WORD	MinLevel;
	WORD	MaxLevel;
	BYTE	FubenKey;
	int		FubenFee;
};
struct SIGNINEVENT
{
	DWORD IsEnabled;
	DWORD EventItem[10];
	DWORD TimeToGet;
	DWORD NextFlag;
};
struct MSGFLASHNAME :public MSGBASE
{
	DWORD dwItemDBidx;
	DWORD dwItemIdx;
	DWORD dwNameFlag;
};
//struct MSG_FLASH_SET : public MSGBASE
//{
//	DWORD ItemPos;
//	DWORD ItemIdx;
//	char       pName[MAX_NAME_LENGTH+1];
//};
#define MAX_MALLLIST_NUM	100
struct MALLINFO 
{
	DWORD	nData1;
	DWORD	nData2;
	DWORD	nData3;
};
struct SEND_MALLLIST : public MSGBASE
{
	WORD	wCount;
	WORD	nCharIdx;
	MALLINFO m_Info[MAX_MALLLIST_NUM];
	SEND_MALLLIST()
	{
		wCount = 0;
		memset(m_Info, 0, MAX_MALLLIST_NUM);
	}
	int GetSize() {
		return (sizeof(SEND_MALLLIST)-sizeof(MALLINFO)*(MAX_MALLLIST_NUM-wCount));
	}
};
struct MSG_FORBID_CHAT : public MSGBASE
{
	DWORD	dwData;
	char	Name[MAX_NAME_LENGTH + 1];
	char	Reason[MAX_CHAT_LENGTH + 1];
};
struct stTime64t : public MSGBASE
{
	__time64_t time64t;
};




struct INSDG_ENTRANCECONDITION_INFO
{
	MAPTYPE wMapNum;		//地图编号
	DWORD dwNpcUniqueIDX;	//NPC传送编号
	LEVELTYPE wMinLevel;	//可以进入最小等级限制
	LEVELTYPE wMaxLevel;	//可以进入最大等级限制
	DWORD dwQuestIndex;		//入场任务编号
	WORD wItemIndex;		//入场物品编号
	DWORD dwMoney;			//入场游戏币设置	
	DWORD dwMsgIndex;		//消息提示
};

struct INSDUNGEON_INFO
{
	MAPTYPE MapNum;				//渡 裘
	DWORD dwInsDGIndex;			//湍瞪檣策蝶
	char szInsDGName[MAX_INSDUNGEONNAME_LENGTH + 1];	//湍瞪檜葷
	eINSDG_TYPE eType;			//檣湍 顫殮
	MAPTYPE EscapeMapNum;		//驍轎 裘 廓
	DWORD dwEscapeMapChangeKind;	//殮濰 湍瞪曖 嬪纂
	MAPTYPE NextMapNum;			//棻擠 檜翕 裘 廓
	DWORD dwRestrictiveTime;	//薯衛除	
	DWORD dwMissionGroupIDX;	//嘐暮 檣策蝶
	BOOL bFinalMap;				//湍瞪曖 葆雖虞 裘檣雖 掘碟
	DWORD dwClearLimitLevel;	//湍瞪 贗葬橫 譆渠溯漣 褻勒(檜溯漣檜 剩橫陛賊 贗葬橫 褒)
	WORD wRevisionLevel;		//爾鼻 爾薑溯漣(0檜賊 瞳辨 寰)
};

struct MSG_INSDUNGEON_INFO : public MSGBASE
{
	DWORD dwChannel;				//瓣割ID
	DWORD dwPartyIDX;				// 檣策蝶
	DWORD dwMunpaIDX;				//僥 檣策蝶	
	DWORD dwProgressTime;			//霞衛除
	DWORD dwRequitalTime;			//爾鼻 衛除
	DWORD dwCurMissionIDX;			//霞醞檣 嘐暮 檣策蝶
	DWORD dwMissionCount;			//霞脹 嘐暮曖 薑爾
	BOOL bMissionSuccess;			//嘐暮檜 撩奢朝雖 掘碟
};

struct INSDG_MISSION_ITEM_INFO
{
	DWORD dwItemIndex;	
	DWORD dwItemMissionKind;	
};

struct MSG_INSDG_MISSION_UPDATE : public MSGBASE
{
	DWORD dwCount;
};

struct INSDG_RANK_STATE
{
	DWORD dwRankTime;
	DWORD dwMonsterKillNum;	
};

struct MSG_INSDG_ENTRANCE : public MSGBASE
{
	INSDG_RANK_STATE RankState;
	DWORD dwData;
};

struct MSG_INSDG_ENTER : public MSGBASE
{
	INSDG_RANK_STATE RankState;
	DWORD dwData1;
	DWORD dwData2;
	DWORD dwData3;
	DWORD dwData4;
	DWORD dwMapMovePoint;
};

struct INSDG_RESULT_REQUITAL_ITEM_INFO
{
	WORD wItemIndex;	//嬴檜齪 檣策蝶
	DWORD dwItemNum;	//嬴嬴齪 偎熱

};

struct MSG_INSDG_RANK_INFO : public MSGBASE
{
	DWORD dwPartyNum;				//贗葬橫 錳 熱
	DWORD dwClearTime;				//贗葬橫 衛除
	DWORD dwMonsterKillNum;			//跨蝶攪 餌傖 熱
	DWORD dwRankValue;				//楨觼 骯 薄熱	
	DWORD dwExp;					//爾鼻 唳纂
	DWORD dwAbillity;				//爾鼻 熱溼纂
	DWORD dwMoney;					//爾鼻 絲
	//WORD wItemIndex;				//爾鼻 嬴檜齪
	INSDG_RESULT_REQUITAL_ITEM_INFO ResultalItemInfo;
};

struct INSDG_RANK_INFO
{
	DWORD dwInsDGIndex;	
	DWORD dwClearTime;			
	DWORD dwMonsterKillNum;			
	DWORD dwRankValue;				
};


struct INSDG_RANK_STANDARD
{
	DWORD dwInsDGIndex;	
	DWORD dwClearTime[INSDG_RANK_STANDARD_MAX];	
	DWORD dwMonsterKill[INSDG_RANK_STANDARD_MAX];	
};

struct MSG_INSDG_RANK_TOTAL_INFO : public MSGBASE
{
	int	nCount;
	INSDG_RANK_INFO	RankInfo[INSTANCE_DUNGEON_MAXNUM];
	WORD GetMsgLength()
	{
		return sizeof(MSG_INSDG_RANK_TOTAL_INFO)-(sizeof(INSDG_RANK_INFO)* (INSTANCE_DUNGEON_MAXNUM - nCount));
	}
};

struct INSDG_KIND_INFO
{
	MAPTYPE wMapNum;
	DWORD dwMapKind;
};


struct MSG_INSDG_KIND_INFO : public MSGBASE
{
	int	nCount;
	DWORD dwPartyIndex;
	DWORD dwMapNum[INSTANCE_DUNGEON_MAXNUM];
	WORD GetMsgLength()
	{
		return sizeof(MSG_INSDG_KIND_INFO)-(sizeof(DWORD)* (INSTANCE_DUNGEON_MAXNUM - nCount));
	}
};

struct UpGradeDataList_Sub
{
	WORD	Grade;
	DWORD	Percent;
};

struct UpGradeDataList
{
	WORD	ItemIdx;
	WORD	MaxGrade;
	UpGradeDataList_Sub* Data_Sub;
};


struct MATERIAL_ARRAY_FFT
{
	WORD	wItemIdx;
	POSTYPE ItemPos;
	DURTYPE Dur;
	DWORD	dwItemDBIdx;
};

struct MSG_OFFICIAL_ITEM_SYN : public MSGBASE
{
	WORD	MainItem_Idx;
	WORD	MainItem_Pos;
	WORD	wMaterialNum;
	WORD	CountCollect;

	WORD	Pr_ItemIdx;
	WORD	pr_Pos;
	WORD	Pr_Dur;
	WORD	Status;
	MATERIAL_ARRAY_FFT Material[MAX_MIX_MATERIAL];
	void init()
	{
		wMaterialNum = 0;
		CountCollect = 0;
		Pr_ItemIdx = 0;
		pr_Pos = 0;
		Pr_Dur = 0;
	}
	void AddMaterial(WORD wItemIdx, POSTYPE ItemPos, DURTYPE Dur)
	{
		Material[wMaterialNum].wItemIdx = wItemIdx;
		Material[wMaterialNum].ItemPos = ItemPos;
		Material[wMaterialNum++].Dur = Dur;
	}
	int GetSize()
	{
		return sizeof(MSG_OFFICIAL_ITEM_SYN) - (MAX_MIX_MATERIAL - wMaterialNum) * sizeof(MATERIAL_ARRAY_FFT);
	}
};
struct MSG_DWORD2CHAR :public MSGBASE
{//耐久，制造者附加 /暂时未引用
	DWORD dwData1;
	DWORD dwData2;
	BYTE  bType;
	char  Maker[MAX_NAME_LENGTH + 1];
};
struct MSG_OFFICIAL_ITEM_ACK : public MSG_OFFICIAL_ITEM_SYN
{

};
struct MSG_UPDATE_GRADE : public MSGBASE
{
	DWORD ItemIdx;
	DWORD Pos;
	DWORD Grade;
};
//装备觉醒结构体
struct MSG_ITEM_QUALITY_MSG :public MSGBASE
{
	ITEMBASE	QualityItemBase;

	DWORD ItemQualityIdx;
	WORD  ItemQualityPos;
	DWORD ItemTargetIdx;
	WORD  ItemTargetPos;
	DWORD ItemExtraTargetIdx;
	WORD  ItemExtraTargetPos;
};
struct sCHANGEITEMUNIT
{
	WORD	wToItemIdx;
	DWORD	dwToItemDur;
	DWORD	dwPercent;
};

struct sCHANGEITEM
{//箱子类物品结构体
	WORD	wItemIdx;
	WORD    wCurChangeItemKey;//增加钥匙类型字段
	WORD    wToItemType;      //增加开出物品类型
	WORD	wMaxToItem;
	sCHANGEITEMUNIT* pItemUnit;

	sCHANGEITEM() { wItemIdx = wMaxToItem = 0; }
	~sCHANGEITEM() { if (pItemUnit) { delete[] pItemUnit; } }
};

struct sMULTICHANGEITEM
{//箱子类物品结构体
	WORD	wItemIdx;
	WORD    wCurChangeItemKey;//增加钥匙类型字段
	WORD    wToItemType;
	WORD	wLimitLevel;
	WORD	wMaxSet;
	unsigned int    nMaxItemSpace;
	sCHANGEITEM* pChangeItem;
};
struct SEND_SHOPITEM_BASEINFO1 : public MSGBASE
{
	POSTYPE ShopItemPosFrom;   // 源物品位置
	POSTYPE ShopItemPosTo;     // 目标物品位置
	DWORD GradeFrom;           // 源物品等级
	DWORD GradeTo;             // 目标物品等级
};
//牛巨任务物品使用数据包结构
struct MSGNJQUESTITEMUSE :public MSGBASE
{
	DWORD dwItemDBidx;
	DWORD dwItemIdx;
	WORD  wPos;
	BYTE  bType;
	BOOL  bRepeat;
};
//VIP设置信息结构体
struct VIP_INFO
{
	WORD  VipLevel;
	DWORD VipMaxGold;
	DWORD VipImgIdx;
	DWORD VipMallMoney;
	DWORD VipGoldMoney;
	DWORD VipItemCount;
	DWORD VipItemIdx[5]; // 达到级别发放物品最多限制5个

	VIP_INFO()
	{
		VipLevel = 0;
		VipMaxGold = 0;
		VipImgIdx = 0;
		VipMallMoney = 0;
		VipGoldMoney = 0;
		VipItemCount = 0;
		for (int i = 0; i < 5; i++)
		{
			VipItemIdx[i] = 0;
		}
	}
};
//Vip查询服务端信息结构体
struct  MSG_VIP_INFO : public MSGBASE
{
	DWORD TotalGold;
	WORD   VipLevel;
	int          VipValue[MAX_VIP_LEVEL];
	MSG_VIP_INFO()
	{
		TotalGold = 0;
		VipLevel = 0;
		memset(VipValue, 0, sizeof(VipValue));
	}
};
struct VIPIMGINFO
{
	WORD  idx;
	BOOL IsTrends;
	WORD ImageCount;
};
struct MSG_NAME_DWORD4 : public MSGBASE
{
	char	Name[MAX_NAME_LENGTH + 1];
	DWORD	dwData1;
	DWORD	dwData2;
	DWORD	dwData3;
	DWORD	dwData4;
};
///////////////////vip图片信息结构体
#pragma pack(pop)
#endif 