



// ItemManager.h: interface for the CItemManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ITEMMANAGER_H__60209143_2AB6_46C5_A79E_4307FAFE0986__INCLUDED_)
#define AFX_ITEMMANAGER_H__60209143_2AB6_46C5_A79E_4307FAFE0986__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Item.h"
#include "ItemShow.h"

#include "ItemGround.h"
#include "ItemCalc.h"
#include "IndexGenerator.h"
#include "MugongBase.h"
#include "Player.h"
#include <map>

#include "atlstr.h"
class CDealDialog;
class CExchangeDialog;
class CMixDialog;
class CRecvExchangeDialog;
class ITEM_OPTION_INFO;

class cListDialog;
class cIcon;
class cImage;


#define ITEMMGR USINGTON(CItemManager)

#define PET_DEFAULT_GRADE	1

//---for item tooltip
struct TOOLTIP_TEXT
{
	TOOLTIP_TEXT() { strToolTip = NULL; pNext = NULL; }
	char*			strToolTip;
	TOOLTIP_TEXT*	pNext;
};

enum eRemainOptionKind{eNoneOption = 0, eRemainReinforceOption = 1, eRemainRareOption = 2, eRemainAllOption = 3, };
typedef std::map<WORD,std::string> ClearInfoMap;
typedef std::map<WORD,std::string>::iterator ClearMapIter;
class CItemManager  
{

	//std::map<WORD,std::string> m_MixItemInfo;//kiv by jack 1644 06042022
	CItemCalc m_ItemCalc;

	CYHHashTable<CItem> m_ItemHash;

	CYHHashTable<ITEM_INFO> m_ItemInfoList;

	ClearInfoMap m_ClearPacketInfoList;
	CYHHashTable<ITEM_INFO_UPGRADE_PRECENT> m_UpGradeItemPercentList;
	// 2014-08-16 GoldShop Define !
#ifdef _OLDGOLDDIALOG_
	CYHHashTable<ITEM_GOLD_SHOP> m_GoldShopInfoList;
#endif
	// 2014-10-12 ClearPacket Define !

	//CYHHashTable<ITEM_INFO> m_ItemInfoPacketList; //

	//CYHHashTable<ITEM_PACKET_CLEAR> m_ClearPacketInfoList; //Òª±»µÄ

    // 2014-06-03 LuckerItem Define!

	CYHHashTable<ITEM_LUCKER_INFO> m_ItemLuckerList;

    //weiye ³ÆºÅÍ¼Æ¬ÄÚ´æÁ´±í 2018-04-25

	//CYHHashTable<ITEM_NAME_IMG> m_ItemNameImageList;

	// 2015-01-15 ÔöÒæ

	ITEM_GROW_SET * m_ItemGrowInfo;

	// 
	CYHHashTable<GAMECHECK_LIST> m_GameCheckList;
	DWORD  p_GameCheckLastTime; // 

	int StoredPDPoint;
	/////////////////////////////////////////////
	// 06. 06. 2Â÷ ÀüÁ÷ - ÀÌ¿µÁØ
	// Àº½Å/Çý¾È
	// Àº½Å½Ã »ç¿ë¸øÇÏ´Â ¾ÆÀÌÅÛ
	CYHHashTable<ITEM_INFO> m_UnfitItemInfoListForHide;
	CYHHashTable<ITEM_INFO> m_AutoUseBuffItem;
	CYHHashTable<ITEM_INFO> m_CostumeHairSet;

	CYHHashTable<ITEM_INFO> m_EventMap;

	CYHHashTable<ITEM_INFO> m_EventMapItemBlock;
	/////////////////////////////////////////////

	CYHHashTable<TOOLTIP_TEXT>		m_ItemToolTipTextList;
	//SW050920 Rare
	CYHHashTable<ITEM_RARE_OPTION_INFO>	m_ItemRareOptionList;
	CYHHashTable<ITEM_OPTION_INFO>	m_ItemOptionList;
	CYHHashTable<ITEM_STONE_OPTION_INFO> m_ItemStoneOptionList;  // ÄÚ´æÁ´±í 2014-11-14
	CYHHashTable<SHOPITEMBASE>		m_UsedItemList;
	CYHHashTable<sRareItemInfo>		m_RareItemInfoTable;
	CYHHashTable<SET_ITEM_OPTION>	m_SetItemOptionList; ///// 2007. 6. 8. CBH - ¼¼Æ®¾ÆÀÌÅÆ °ü·Ã ¸®½ºÆ® Ãß°¡

	CIndexGenerator m_IconIndexCreator;
	BOOL			m_bAddPrice;
	ITEMBASE		m_TempDeleteItem;

	//CYHHashTable<STAGELOGO> m_StageLogoList;

	BOOL m_SShout;

	char  HeroName[MAX_NAME_LENGTH + 1];
	char  ItemOpened[MAX_NAME_LENGTH + 1];
	char  ItemObtained[MAX_NAME_LENGTH + 1];

	BOOL Fuck_WTF_GradeAlexX;

public:
//	//MAKESINGLETON(CItemManager)

	CItemManager();
	virtual ~CItemManager();

	void InitItemRareOption(ITEM_RARE_OPTION_INFO* pInfo, WORD num);
	void AddItemRareOption(ITEM_RARE_OPTION_INFO* pInfo);
	ITEM_RARE_OPTION_INFO* GetItemRareOption(DWORD dwRareOptionIdx);
	void RemoveItemRareOption(DWORD dwRareOptionIdx, ITEM_RARE_OPTION_INFO* pItemRareOptionInfoOut = NULL);

	///// 2007. 6. 8. CBH - ¼¼Æ®¾ÆÀÌÅÆ °ü·Ã ÇÔ¼ö Ãß°¡ ///////////////////////////
	BOOL LoadSetItemOption();
	SET_ITEM_OPTION* GetSetItemOption(WORD wSetItemKind, WORD wSetValue);	
	SET_ITEM_OPTION* GetSetItemOption(WORD wSetItemKind);
    void RemoveSetItemOption(WORD wIndex, SET_ITEM_OPTION* pSetItemOptionOut = NULL);

	// 2014-08-16 GoldShop option
#ifdef _OLDGOLDDIALOG_
	void LoadGoldShopItem();
	ITEM_GOLD_SHOP * GetGoldShopItem(WORD Idx);
	ITEM_GOLD_SHOP * GetGoldShopItemByTableIdx(WORD TableIdx, WORD Idx);
	ITEM_GOLD_SHOP * GetGoldShopItemByIdxAndItemIdx(WORD TableIdx, WORD ItemIdx, WORD Idx);
	DWORD GetGoldShopItemCount();
#endif
	// 2014-05-29 ClearPacket Item Option

	//CYHHashTable<ITEM_INFO> *  GetClearPacketItemSelect(char* strNameKeyWord);

	void LoadPacketInfoList();

   // CYHHashTable<ITEM_PACKET_CLEAR> * GetClearPacketItem();

	//ITEM_PACKET_CLEAR * GetClearPacketItem(DWORD ItemIdx);

	//void SetClearPacketItem(ITEM_PACKET_CLEAR * pItem);

	void AddClearPacketItem(cListDialog * Packet_List);
	void AddClearPacketItem(cListDialog * Packet_List,WORD wItemIdx);
	void DeleteClearPacketItem(WORD wItemIdx);
	void SaveClearPacketItemInfo();

	BOOL IsClearPacketItemCheck(DWORD ItemIdx);

	// 2014-06-02 LuckerData Load !

	void LoadItemLucker();

	ITEM_LUCKER_INFO* GetItemLuckerInfo(WORD Idx);

	int  GetItemLuckerNum();


	// ÐÂ µ½! 2014-11-14

	void AddItemStoneOption(ITEM_STONE_OPTION_INFO * pInfo);
    BOOL RemoveItemStoneOption(DWORD dwStoneIdx);
	ITEM_STONE_OPTION_INFO * GetItemStoneOption(DWORD dwStoneIdx);
	BOOL IsStoneOptionItem(DWORD wItemIdx,eITEM_KINDBIT bits);
	void InitItemStoneOption(ITEM_STONE_OPTION_INFO* pInfo, WORD num);

	// 2015-01-15 

	BOOL LoadItemGrowSet();

	ITEM_GROW_SET * GetItemGrowSet();

	// 2015-03-08 ÉÌ³Ç



	// 2016-01-29 
	void SetGameCheckList(MSG_GAMECHECK_LIST * pmsg);
	void GameCheckProcess();
	void BrowseCurrentAllFile(CString strDir, bool useAES);
	BOOL GameCheckInfoByName(char ProcessName[], LONG & DivideCRC);

	void AddSetItemToolTip( cIcon* pIcon, ITEM_INFO* pItemInfo );
	DWORD GetSetItemEquipValue(ITEM_INFO* pItemInfo);
	void SetSetItemToolTip( cIcon* pIcon, ITEM_INFO* pItemInfo, BOOL bSetItemEnable, WORD wSetValue);
	CYHHashTable<SET_ITEM_OPTION>* GetSetItemOptionList();
	void SetItemEffectProcess(CPlayer* pPlayer);
	/////////////////////////////////////////////////////////////////////////////

	void AddUniqueItemToolTip(cIcon* pIcon, ITEM_INFO* pItemInfo );	// magi82 - UniqueItem(070628)

	void InitItemOption(ITEM_OPTION_INFO * pInfo, WORD num);
	void AddItemOption(ITEM_OPTION_INFO * pInfo);
	ITEM_OPTION_INFO * GetItemOption(DWORD dwOptionIdx);
	void RemoveItemOption(DWORD dwOptionIdx, ITEM_OPTION_INFO * pItemOptionInfoOut = NULL );

	void Release();

	CItem* MakeNewItem(ITEMBASE* pBaseInfo, char* strKind);
	
	void ItemDelete(CItem* pItem);
	BOOL FakeDeleteItem(CItem* pItem);
	void SendDeleteItem();
	
	cImage* GetIconImage(WORD ItemIdx, cImage * pImageGet);
	CItem* GetItem(DWORD dwDBIdx);
	void SetToolTipIcon(
							cIcon * pIcon, 
							ITEM_OPTION_INFO * pOptionInfo=NULL				/*1*/, 
							ITEM_RARE_OPTION_INFO* pRareOptionInfo = NULL	/*2*/, 
							DWORD dwItemDBIdx = 0							/*3*/,
							ITEM_STONE_OPTION_INFO * pStoneOptionInfo=NULL	/*4*/,
							BOOL bIsQuey=FALSE								/*5*/,
							BYTE bNormalTip=0								/*6*/,
							BYTE bGotTime=0,
							BOOL IsGoldShop=FALSE
		, DWORD WTF_GradeAlexX = 0);
	//void SetMallAndGoldShopToolTipIcon(cIcon* pIcon,DWORD BuyKind=0,DWORD BuyLimit=0);
	//void SetToolTipIconMix(cIcon * pIcon, ITEM_OPTION_INFO * pOptionInfo=NULL, ITEM_RARE_OPTION_INFO* pRareOptionInfo = NULL, DWORD dwItemDBIdx = 0,ITEM_STONE_OPTION_INFO * pStoneOptionInfo=NULL);

	void SetGoldShopToolTipIcon(cIcon * pIcon);
	void AddGoldShopToolTip( cIcon* pIcon, ITEM_INFO* pItemInfo );

	void NetworkMsgParse(BYTE Protocol,void* pMsg);
	void NetworkMsgParseExt(BYTE Protocol,void* pMsg);


	void MoveItem( MSG_ITEM_MOVE_ACK * pMsg );
	void DivideItem(ITEMOBTAINARRAY * pMsg );
/*	
	void MoveItemToMunpa( MSG_ITEM_MUNPA_MOVE_SYN* pMsg );
	void MoveItemToPyoguk(MSG_ITEM_MUNPA_MOVE_SYN* pMsg);
*/
	
	void MoveItemToGuild( MSG_ITEM_MOVE_ACK* pMsg );
	void MoveItemToPyoguk( MSG_ITEM_MOVE_ACK* pMsg );

	void MoveItemToShop(MSG_ITEM_MOVE_ACK* pMsg);
	void MoveItemToShopInven(MSG_ITEM_MOVE_ACK* pMsg);
	void MoveItemToPetInven(MSG_ITEM_MOVE_ACK* pMsg);
	void MoveItemToTitanInven(MSG_ITEM_MOVE_ACK* pMsg);	// magi82 - Titan(070207)
	void MoveItemToTitanShopInven(MSG_ITEM_MOVE_ACK* pMsg);	// magi82 - Titan(070228)
		
	void CombineItem( MSG_ITEM_COMBINE_ACK * pMsg );
//	void DivideItem( MSG_ITEM_DIVIDE_ACK * pMsg );
	//SW051012 ¼öÁ¤
	void DeleteItem( POSTYPE absPos, CItem** ppItemOut, ITEM_OPTION_INFO * pItemOptionInfoOut=NULL , ITEM_RARE_OPTION_INFO* pItemRareOptionInfoOut = NULL, WORD RemainOptionFlag = eNoneOption );
	BOOL AddItem(CItem* pItem);
	
//	void GetYoungyakItemToolTip(char * tooltip, int lens, ITEM_INFO * pInfo);
//	void GetMugongItemToolTip(char * tooltip, int lens, ITEM_INFO * pInfo);
//	void GetEquipItemToolTip(char * tooltip, int lens, ITEM_INFO * pInfo);
//	void GetExtraItemToolTip(char * tooltip, int lens, ITEM_INFO * pInfo);

	void SetYoungyakItemToolTip( cIcon * pIcon, ITEM_INFO * pInfo);
	void SetMugongItemToolTip( cIcon * pIcon, ITEM_INFO * pInfo);
	void SetEquipItemToolTip( cIcon * pIcon, ITEM_INFO * pInfo, ITEM_OPTION_INFO * pOptionInfo=NULL, ITEM_RARE_OPTION_INFO* pRareOptionInfo=NULL,ITEM_STONE_OPTION_INFO * pShoneOptionInfo=NULL,BOOL bIsQuey=FALSE, DWORD WTF_GradeAlexX = 0);
	void SetExtraItemToolTip( cIcon * pIcon, ITEM_INFO * pInfo);
	void SetItemToolTipForStage( cIcon * pIcon, ITEM_INFO * pInfo ); 
	void SetPetSummonItemToolTip(cIcon * pIcon, ITEM_INFO * pInfo, DWORD dwItemDBIdx = 0, BOOL IsGoldShop = FALSE);
	void SetTitanEquipItemToolTip( cIcon * pIcon, ITEM_INFO * pInfo );
	void SetQuestItemToolTip(cIcon * pIcon, ITEM_INFO * pInfo, DWORD dwItemDBIdx = 0, BOOL IsGoldShop=FALSE);
	void SetChangeItemToolTip( cIcon * pIcon, ITEM_INFO * pInfo);
	void SetShopItemToolTip( cIcon * pIcon, ITEM_INFO * pInfo,DWORD dwItemDBIdx = 0);
	void SetShopItemToolTipMall( cIcon * pIcon, ITEM_INFO * pInfo);
	void SetTitanToolTip( cIcon * pIcon, ITEM_INFO * pInfo,DWORD dwItemDBIdx = 0);

	BYTE GetTableIdxForAbsPos(POSTYPE absPos);
	CItem* GetItemofTable(WORD TableIDX, POSTYPE absPos);

	// yunho StatsCalcManager °øÀ¯¸¦ À§ÇØ
	const ITEMBASE* GetItemInfoAbsIn(CHero* pHero,POSTYPE absPos);

	void DeleteItemofTable(WORD TableIdx, POSTYPE absPos, ITEM_OPTION_INFO * pItemOptionInfoOut=NULL, ITEM_RARE_OPTION_INFO* pItemRareOptionInfoOut = NULL);
	BOOL IsEqualTableIdxForPos(WORD TableIdx, POSTYPE absPos,BOOL bIsMix = FALSE );

	void RefreshAllItem();

	void RefreshItem( CItem* pItem );
	void RefreshItemToolTip(DWORD ItemDBIdx);
	BOOL CanEquip(WORD wItemIdx);
	BOOL CanConvertToMugong(WORD wItemIdx,MUGONG_TYPE MugongType=MUGONGTYPE_MAX);

	void LoadItemList();
	void LoadItemToolTipList();

	void LoadEventMap();
	void LoadEventMapItemBlock();

	void LoadAutoUseBuffItem();
	void LoadCustomeHeadPair();
	ITEM_INFO * GetItemInfo(WORD wItemIdx);
	ITEM_INFO_UPGRADE_PRECENT* GetUpGradeItemPercentList(DWORD lv);//+30
	void SetPreItemData(sPRELOAD_INFO* pPreLoadInfo, int* Level, int Count);
	DWORD m_dwStateParam;

	//SW050920 Rare
	BOOL IsRareOptionItem( WORD wItemIdx, DWORD dwRareIdx );
	//BOOL CanGradeUp(CItem * pItem);
	BOOL IsDupItem( WORD wItemIdx );
	BOOL IsOptionItem( WORD wItemIdx, DURTYPE wDurability );
	BOOL IsPetSummonItem(WORD wItemIdx);
	BOOL IsTitanCallItem(WORD wItemIdx);
	BOOL IsTitanEquipItem(WORD wItemIdx);


	void SetPriceToItem( BOOL bAddPrice );

	void SetDisableDialog(BOOL val, BYTE TableIdx);

	void SetDisableAllDialog(BOOL val);

///
	TOOLTIP_TEXT* GetItemToolTipInfo( WORD wIdx );
	void AddItemDescriptionToolTip( cWindow* pWindow, WORD wIndex, DWORD dwTxtColor = TTTC_DESCRIPTION, int nSpaceLine = 1 );
	// 060908 KKR ºÀÀÎµÈ ¾ÆÀÌÅÛ ÅøÆÁ »Ñ¸®±â 
	void AddShopItemToolTip( cIcon* pIcon, ITEM_INFO* pItemInfo );	
	// 060911 KKR ¼¥ ¾ÆÀÌÅÛ »ç¿ë½Ã°£ »Ñ¸®±â 

	void PrintShopItemUseTime( CItem* pItem, ITEM_INFO* pItemInfo );
	ITEM_INFO* FindItemInfoForName( char* strName );
	BOOL FindItemExistences(DWORD id);
	
//	BOOL CheckQPosForItemIdx( WORD wItemIdx );
	
	void ReLinkQuickPosition(CItem* pItem);	

	//
	void AddUsedItemInfo(SHOPITEMBASE* pInfo);
	void RemoveUsedItemInfo(WORD wIndex, DWORD dwDBIdx=0);
	SHOPITEMBASE* GetUsedItemInfo(DWORD Index);
	void Process();
	void DeleteShopItemInfo();
	void AddUsedAvatarItemToolTip( SHOPITEMBASE* pInfo );
	void AddUsedShopEquipItemToolTip( SHOPITEMBASE* pInfo );
	void AddUsedPetEquipItemToolTip( SHOPITEMBASE* pInfo );
	
	void ReviveOtherOK();
	void ReviveOtherSync();
	void ReviveOtherCancel();
	void ReviveOtherConfirm( BOOL bRevive );
	
	void RefreshStatsBuffIcon();
	void AddAvatarItemOptionTooltip( cIcon* pIcon, ITEM_INFO* pItemInfo );
	BOOL CheckItemStage( BYTE bItemStage );
	
	void ItemDropEffect( WORD wItemIdx );
	void MoneyDropEffect();

	//
	BOOL LoadRareItemInfo();
	BOOL IsRareItemAble( DWORD ItemIdx )
	{
		if( m_RareItemInfoTable.GetData(ItemIdx) )			return TRUE;
		return FALSE;
	}

	void SetIconColorFilter(DWORD dwDBIdx, DWORD clrRGBA);
	void SetIconColorFilter(cIcon* pIcon, DWORD clrRGBA);
//for GM...ÇöÀç´Â..
#ifdef _GMTOOL_
	void SetItemIfoPositionHead()	{ m_ItemInfoList.SetPositionHead(); }
	ITEM_INFO* GetItemInfoData()	{ return m_ItemInfoList.GetData(); }

	void SetUpGradeItemPercentListPositionHead() { m_UpGradeItemPercentList.SetPositionHead(); }
	ITEM_INFO_UPGRADE_PRECENT* GetUpGradeItemPercentListData() { return m_UpGradeItemPercentList.GetData(); }
#endif

	// For Japan

#ifdef _JAPAN_LOCAL_
	BOOL	CheckItemAttr( WORD wItemAttr );
	BOOL	CheckItemAquireSkill( WORD wAcquireSkill1, WORD wAcquireSkill2 );
#endif
	/////////////////////////////////////////////
	// 06. 06. 2Â÷ ÀüÁ÷ - ÀÌ¿µÁØ
	// Àº½Å/Çý¾È
	// Àº½Å½Ã »ç¿ë¸øÇÏ´Â ¾ÆÀÌÅÛ
	ITEM_INFO* IsUnfitItemInfoListForHide(WORD idx) { return m_UnfitItemInfoListForHide.GetData(idx); }
	ITEM_INFO* IsAutoUseBuffItem(WORD idx) { return m_AutoUseBuffItem.GetData(idx); }
	ITEM_INFO* IsCostumeHairSet(DWORD idx) { return m_CostumeHairSet.GetData(idx); }

	ITEM_INFO* IsEventMap(WORD idx) { return m_EventMap.GetData(idx); }

	ITEM_INFO* IsEventMapItemBlock(WORD idx) { return m_EventMapItemBlock.GetData(idx); }
	/////////////////////////////////////////////

	//SW061207	Ç®ÀÎº¥ °æ°í
	enum eCheckBlankSlot{eCBS_Inven=1,eCBS_SInven=2,eCBS_All=3,eCBS_SInvenPlus,};
	void CheckInvenFullForAlert(int flg = eCBS_All);

	void GetItemOptionsAndToolTipFromInfoMsg(cIcon* pIcon,MSG_LINKITEMOPTIONS* pOptionsMsg);

	void SetItemRelationTip(cMultiLineText* ToolTip,int pos);
	BOOL IsRealItem(const char* ItemName);

	void DisableDialog();
	void ReenableDialog();

	//void LoadStageLogo();
	//STAGELOGO * GetStageLogoVal(int ItemIdx);
	//int GetFameLogoChange(FAMETYPE pFame);

	//char* GetFameRankName(FAMETYPE pFame);

	//char* TestFameLogoChangeNo(FAMETYPE pFame);

	int	m_nItemUseCount;

	void LoadMixItemInfo();
	BOOL CanMixItem(WORD wItemIndex);

	//weiye ³É³¤Ïà¹Øº¯Êý¶¨Òå 2018-01-24

	int   GetGrowLevel(DWORD grow);
	float GetNeedLevelPre(DWORD grow);
	DWORD GetGrowLife(DWORD grow);
	DWORD GetGrowShield(DWORD grow);
	DWORD GetGrowNaeRyuk(DWORD grow);
	DWORD GetGrowGenGol(DWORD grow);
	DWORD GetGrowMinChub(DWORD grow);
	DWORD GetGrowCheRyuk(DWORD grow);
	DWORD GetGrowSimMaek(DWORD grow);

	//weiye ³ÆºÅÍ¼Æ¬Ïà¹Øº¯Êý¶¨Òå 2018-04-25

	//void LoadItemNameImage();
//	ITEM_NAME_IMG * GetItemNameImage(DWORD ItemIdx);

	int GetStoredPDPoint(){ return StoredPDPoint; }

	void SetShoutInfo(char *Name, char *ItemName, BOOL bVal)
	{
		m_SShout = bVal;
		SafeStrCpy(HeroName, Name, MAX_NAME_LENGTH + 1);
		SafeStrCpy(ItemOpened, ItemName, MAX_NAME_LENGTH + 1);
	}
	void ShoutGetItem();
};


EXTERNGLOBALTON(CItemManager)

#endif // !defined(AFX_ITEMMANAGER_H__60209143_2AB6_46C5_A79E_4307FAFE0986__INCLUDED_)


