



// ItemManager.h: interface for the CItemManager class.
// created by taiyo
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ITEMMANAGER_H__B48BE6A4_2998_4459_B958_995D81EAAF1E__INCLUDED_)

#define AFX_ITEMMANAGER_H__B48BE6A4_2998_4459_B958_995D81EAAF1E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "IndexGenerator.h"
#include "cLinkedList.h"
#include "HashTable.h"
#include "..\[CC]Header\GameResourceStruct.h"
#include "IndexManager.h"
#include <map>
#include "..\[CC]MixManager\MixManager.h"
class CMHFile;
struct ITEM_INFO;
//class ITEM_MIX_INFO;
struct MATERIAL_ARRAY;
//class ITEM_MIX_RES;
class ITEM_REINFORCE_INFO;

//#define MAX_MIX_PERCENT	10000
//#define MAX_MIX_LARGE_PERCENT	1000000
#define ITEMMGR CItemManager::GetInstance()

#define EVENTITEM_SHIELD_RECOVER 53031
#define EVENTITEM_ALL_RECOVER 53032

class CItemSlot;

struct DealerItem
{
	BYTE Tab;
	BYTE Pos;
	WORD ItemIdx;
	int	ItemCount;
};
struct DealerData
{

public:
	BOOL FindItem(WORD ItemIdx)
	{
		POS pos = m_DealItemList.GetFirstPos();
		while(DealerItem* pItem = m_DealItemList.GetNextPos(pos) )
		{
			if(pItem->ItemIdx == ItemIdx && pItem->ItemCount != 0)
				return TRUE;
		}

		return FALSE;
	}

	DealerItem*	GetItem(WORD ItemIdx)
	{
		POS pos = m_DealItemList.GetFirstPos();
		while(DealerItem* pItem = m_DealItemList.GetNextPos(pos) )
		{
			if(pItem->ItemIdx == ItemIdx && pItem->ItemCount != 0)
				return pItem;
		}

		return NULL;
	}

	void MakeSellingList(cPtrList* pList, DWORD tabNum)
	{
		POS pos = m_DealItemList.GetFirstPos();
		while(DealerItem* pItem = m_DealItemList.GetNextPos(pos) )
		{
			if(pItem->Tab == tabNum)
			{
				DealerItem* pTmp = new DealerItem;
				*pTmp = *pItem;
				pList->AddTail(pTmp);
			}
		}
	}

	cLinkedList<DealerItem> m_DealItemList;
};

class CItemManager  
{
	CYHHashTable<DealerData>	m_DealerTable;
	DWORD m_Key;

	

	



public:
	DWORD MakeNewKey();
	CItemManager();
	virtual ~CItemManager();
	GETINSTANCE(CItemManager);
	void LoadDealerItem();
	DealerData * GetDealer(WORD wDealerIdx);
	int DivideItem( CPlayer* pPlayer, WORD ItemIdx,  POSTYPE FromPos, POSTYPE ToPos, DURTYPE FromDur, DURTYPE ToDur);
	int CombineItem( CPlayer* pPlayer, WORD ItemIdx, POSTYPE FromPos, POSTYPE ToPos, DURTYPE & FromDur, DURTYPE & ToDur);
	int MoveItem( CPlayer * pPlayer, WORD FromItemIdx, POSTYPE FromPos, WORD ToItemIdx, POSTYPE& ToPos);

	void CheckAvataChange(CPlayer * pPlayer,WORD ItemIdx,WORD wWhatPos);
	int DiscardItem(CPlayer * pPlayer, POSTYPE whatPos, WORD whatItemIdx, DURTYPE whatItemNum);
	
	void SendErrorMsg( CPlayer * pPlayer, MSG_ITEM_ERROR * msg, int msgSize, int ECode);
	void SendAckMsg( CPlayer * pPlayer, MSGBASE * msg, int msgSize);
	void SendGuildErrorMsg( CPlayer * pPlayer, BYTE Protocol, int ECode );

	int ObtainItemEx(CPlayer * pPlayer, ITEMOBTAINARRAYINFO * pArrayInfo, WORD whatItemIdx, WORD whatItemNum, WORD * EmptyCellPos, WORD EmptyCellNum, WORD ArrayInfoUnitNum, WORD bSeal=0,WORD BuyType=0);  // 2014-12-02  ?áÔ?0Ê°∂Â∞®?•Á±µNPC?ÉÈ?! 1Ê°∂Â∞®?êËê∏?ÉÈ? 2Ê°∂Â∞®?ÉÈ?
	int ObtainRareItem(CPlayer* pPlayer, ITEMOBTAINARRAYINFO* pArrayInfo, WORD whatItemIdx, WORD whatItemNum, WORD * EmptyCellPos, WORD ArrayInfoUnitNum, ITEM_RARE_OPTION_INFO* pRareOptionInfo );

	int SellItem( CPlayer* pPlayer, POSTYPE whatPos, WORD wSellItemIdx, DURTYPE sellItemDur, DWORD DealerIdx );
	//int BuyItem( CPlayer* pPlayer, WORD buyItemIdx, WORD buyItemNum, WORD DealerIdx,DWORD ItemPos=0,WORD buyType=0); // 2014-12-02 ?áÔ?0Ê°∂Â∞®?•Á±µNPC?ÉÈ?!

	int BuyItem( CPlayer* pPlayer, DWORD buyItemIdx, WORD buyItemNum, WORD DealerIdx,DWORD ItemPos=0,WORD buyType=0); // 2014-12-02 ÈªòËÆ§0Ë°®Á§∫?ÆÈÄöNPCË¥≠‰π∞!

#ifdef _GOLDDLGONEBTN_
	int BuyItemForGoldShop(CPlayer* pPlayer, DWORD buyItemIdx, WORD buyItemNum, WORD DealerIdx, DWORD ItemPos = 0, WORD buyType = 0, DWORD buyRight=0);
#else
	int BuyItemForGoldShop(CPlayer* pPlayer, DWORD buyItemIdx, WORD buyItemNum, WORD DealerIdx, DWORD ItemPos = 0, WORD buyType = 0);
#endif
	 
	void MonsterObtainItem(CPlayer * pPlayer, WORD obtainItemIdx, DWORD dwFurnisherIdx, WORD ItemNum = 1);

	int LootObtainItem( CPlayer* pPlayer, WORD obtainItemIdx, WORD ItemNum,WORD bSel=0,WORD BuyType=0 );
	//static void LootObtainItemDBResult(CPlayer * pPlayer, WORD ArrayID);

	int CheatObtainItem( CPlayer* pPlayer, WORD obtainItemIdx, WORD ItemNum );
	int CheatObtainRareItem(CPlayer* pPlayer, WORD obtainItemIdx, WORD ItemNum, ITEM_OPTION_INFO* pOptionInfo);

	static void ObtainItemDBResult(CPlayer * pPlayer, WORD ArrayID);
	static void BuyItemDBResult(CPlayer * pPlayer, WORD ArrayID );	

	static void DivideItemDBResult(CPlayer* pPlayer, WORD ArrayID);
	static void RareItemDBResult(CPlayer* pPlayer, WORD ArrayID, ITEM_RARE_OPTION_INFO* pRareOptionInfo);
	static void TitanUpgradeItemDBResult(CPlayer* pPlayer, WORD ArrayID);

	void NetworkMsgParse( DWORD dwConnectionIndex, BYTE Protocol, void* pMsg );
	void NetworkMsgParseExt( DWORD dwConnectionIndex, BYTE Protocol, void* pMsg );
	//void LoadMixList();
	//void LoadMixResultItemInfo(CMHFile * fp, ITEM_MIX_RES * pResInfo);
	void LoadReinforceItemInfo();

	void LoadItemList();
	void ReloadItemList();


	void LoadGoldShopItem();

   // ITEM_GOLD_SHOP * GetGoldShopItem(WORD ItemIdx,WORD Idx);
#ifdef _GOLDDLGONEBTN_
	ITEM_GOLD_SHOP * GetGoldShopItem(DWORD ItemIdx, DWORD Key, DWORD buyRight);
#else
	ITEM_GOLD_SHOP * GetGoldShopItem(DWORD ItemIdx,DWORD Key);
#endif


	void LoadItemLucker();

	ITEM_LUCKER_INFO* GetItemLuckerInfo(WORD Idx);

	int  GetItemLuckerNum();

	ITEM_LUCKER_INFO * GetRandLuckerItem();

    int  GetItemLuckerIndexForRate(int ItemRate); 


	int SetItemLock(CPlayer * pPlayer,DWORD ItemIdx,WORD ItemPos, DWORD StoneIdx,WORD StonePos);

	void ItemLockDBResult(CPlayer * pPlayer,DWORD wItemDBIdx,POSTYPE TargetPos);


	int SetItemUnLock(CPlayer * pPlayer,DWORD ItemIdx,WORD ItemPos, DWORD StoneIdx,WORD StonePos);

	void ItemUnLockDBResult(CPlayer * pPlayer,DWORD wItemDBIdx,POSTYPE TargetPos);

	void SetItemInfo(WORD ItemIdx,ITEM_INFO* pInfo,CMHFile* pFile);
	ITEM_INFO * GetItemInfo( WORD wItemIdx );
	//const ITEM_MIX_INFO * GetMixItemInfo(WORD wItemIdx) const;
	ITEM_REINFORCE_INFO * GetReinforceItemInfo(WORD wItemIdx);


	BOOL IsStoneOptionItem(DWORD wItemIdx,eITEM_KINDBIT bits);

	DWORD  ShoneItem(CPlayer* pPlayer, DWORD wTargetItemIdx, POSTYPE TargetPos,DWORD wStoneItemIdx,POSTYPE StoneItemPos); 
	
    int  GetItemShoneCount(ITEM_STONE_OPTION_INFO * pStoneInfo);

    void StoneItemDBResult(CPlayer * pPlayer, DWORD wItemDBIdx,POSTYPE TargetPos, ITEM_STONE_OPTION_INFO * pStoneInfo);

	void StoneItemDBResultEx(CPlayer * pPlayer, DWORD wItemDBIdx,POSTYPE TargetPos, ITEM_STONE_OPTION_INFO * pStoneInfo);
   
	int   XqItem(CPlayer * pPlayer,MSG_ITEM_XQ_DATA * pMsg);


	BOOL LoadItemGrowSet();

	ITEM_GROW_SET * GetItemGrowSet();

	void GrowItemDBResult(CPlayer* pPlayer, DWORD wTargetItemDBIdx,POSTYPE wTargetItemPos, DWORD dwItemGrow);
	
	void LoadGameCheckList();
	void SendGameCheckMsg(CPlayer* pPlayer);

	int UseItem( CPlayer* pPlayer, WORD TargetPos, WORD wItemIdx );
	int UpgradeItem(CPlayer* pPlayer, WORD & TargetItemIdx, POSTYPE TargetPos, WORD MatItemIdx, POSTYPE MatItemPos);

	//int MixItem(CPlayer* pPlayer, WORD wBasicItemIdx, POSTYPE BasicItemPos, POSTYPE ResultIdx, MATERIAL_ARRAY * pMaterialArray, WORD wMaterialNum, WORD ShopItemIdx, WORD ShopItemPos, BOOL bTitan = FALSE);	// magi82 - Titan(070118)
	int ReinforceItem(CPlayer* pPlayer, WORD wTargetItemIdx, POSTYPE TargetPos, ITEM_JEWEL_POS_EX * pJewelInfo, WORD wJewelNum);
	BOOL IsSameUseItem(ITEM_JEWEL_POS_EX * pJewelInfo, WORD wJewelNum);
	void ReinforceItemDBResult(CPlayer * pPlayer, WORD wItemIdx, POSTYPE TargetPos, ITEM_OPTION_INFO * pOptionInfo);

	BOOL EnoughDissolveSpace(CPlayer * pPlayer, WORD wOptionIdx, LEVELTYPE ItemLevel, WORD wAbilityGrade, MATERIAL_ARRAY * pJewelArray, WORD & wJewelNum);
	BOOL EnoughDissolveInvSpace(CPlayer * pPlayer, WORD wJewelNum);
	
	//BOOL EnoughMixMaterial(WORD needItemIdx, WORD needItemNum, MATERIAL_ARRAY * pMaterialArray, WORD wMaterialNum);

	ITEMOBTAINARRAYINFO *	Alloc(CPlayer * pPlayer, WORD c, WORD p, DWORD dwObjectID, DWORD dwFurnisherIdx, WORD wType, WORD ObtainNum, DBResult CallBackFunc, DBResultEx CallBackFuncEx = NULL,int BuyType = 0);
	void Free(CPlayer * pPlayer, ITEMOBTAINARRAYINFO * info);
	BOOL IsDupItem( WORD wItemIdx );
	BOOL IsRareOptionItem( WORD wItemIdx, DWORD dwRareDBIdx );
	BOOL IsOptionItem( WORD wItemIdx, DURTYPE wDurability );
	BOOL IsPetSummonItem( WORD wItemIdx );
	BOOL IsTitanCallItem( WORD wItemIdx );
	BOOL IsTitanEquipItem( WORD wItemIdx );
	
	BOOL ItemMoveUpdateToDBbyTable(CPlayer* pPlayer, DWORD dwfromDBIdx, WORD dwfromIconIdx,  POSTYPE frompos, DWORD dwtoDBIdx, POSTYPE topos);
	void ItemUpdatebyTable(CPlayer* pPlayer, DWORD dwDBIdx, WORD wIconIdx, DURTYPE FromDur, POSTYPE Position, POSTYPE QuickPosition);
	const ITEMBASE * GetItemInfoAbsIn(CPlayer* pPlayer, POSTYPE Pos);

	void ItemMoveLog(POSTYPE FromPos, POSTYPE ToPos, CPlayer* pPlayer, ITEMBASE* pItem);
	void AddGuildItemOption(POSTYPE FromPos, POSTYPE ToPos, CPlayer* pPlayer);
	void RemoveGuildItemOption(POSTYPE ToPos, const ITEMBASE* pItem, CPlayer* pPlayer);

	unsigned int GetTotalEmptySlotNum(CItemSlot* pSlot); //060612 Add by wonju
	WORD GetCanBuyNumInSpace(CPlayer * pPlayer, CItemSlot * pSlot, DWORD whatItemIdx, DURTYPE whatItemNum, WORD * EmptyCellPos, WORD & EmptyCellNum,WORD buyType=0);

	int NewUpGrareItem(CPlayer* pPlayer, MSG_NEWYPGRARE_ALEXX* pMsg);
	ITEM_INFO_UPGRADE_PRECENT* GetUpGradeItemPercentList(DWORD lv);

	//
	void GradeAlexXItemDBResult(CPlayer* pPlayer, DWORD wTargetItemDBIdx, POSTYPE wTargetItemPos, WORD wItemGradeAlexX);
	//

private:

	WORD GetCanBuyNumInMoney(CPlayer * pPlayer, WORD butNum, MONEYTYPE Price);
	BOOL CanMovetoGuildWare(POSTYPE FromPos, POSTYPE ToPos, CPlayer* pPlayer);
	BOOL CanbeMoved(WORD wIconIdx,POSTYPE pos, CPlayer* pPlayer);
	BOOL CanEquip(ITEM_INFO* pInfo, CPlayer* pPlayer);
	void ClearQuickPos(ITEMBASE* pItem, POSTYPE ToPos);

	CYHHashTable<ITEM_INFO> m_ItemInfoList;


	CYHHashTable<ITEM_GOLD_SHOP> m_GoldShopInfoList;


	CYHHashTable<ITEM_LUCKER_INFO> m_ItemLuckerList;


	ITEM_GROW_SET * m_ItemGrowInfo;

	CYHHashTable<GAMECHECK_LIST> m_GameCheckList;
	//---------------------------------------------------------------------------
	//std::map<WORD,std::string> m_MixItemInfo;//kiv by jack 1644 06042022
	//CYHHashTable<ITEM_MIX_INFO> m_MixItemInfoList;
	//---------------------------------------------------------------------------

	//std::map<WORD,ITEM_MIX_INFO> m_MixItemInfoList;
	//CYHHashTable<HILEVEL_ITEM_MIX_RATE_INFO> m_HiLevelItemMixRateInfoList;
	//CYHHashTable<ITEM_INFO> m_IsCanMixItem;
	//---------------------------------------------------------------------------

	CYHHashTable<ITEM_REINFORCE_INFO> m_ReinforceItemInfoList;
	CYHHashTable<SET_ITEM_OPTION>	m_SetItemOptionList;
	CMemoryPoolTempl<ITEMOBTAINARRAYINFO> m_ItemArrayPool;
	CIndexGenerator		m_ArrayIndexCreator;
	//IndexManager m_ArrayIndexCreator;
	CYHHashTable<AVATARITEM>		m_AvatarEquipTable;

	CYHHashTable<ITEM_INFO> m_UnfitItemInfoListForHide;

	

	CYHHashTable<ITEM_INFO> m_UnfitItemInfoListForShout;
	CYHHashTable<sTIPITEMINFO>		m_TipItemList;
	
	CYHHashTable<ITEM_INFO_UPGRADE_PRECENT> m_UpGradeItemPercentList;

public:
	int		ObtainItemFromQuest( CPlayer* pPlayer, WORD wItemKind, DWORD dwItemNum );
	WORD	GetWeaponKind( WORD wWeapon );
	void	GetItemKindType( WORD wItemIdx, WORD* wKind, WORD* wType );

	int		ObtainItemFromChangeItem( CPlayer* pPlayer, WORD wItemKind, WORD wItemNum );
	BOOL	CheckHackNpc( CPlayer* pPlayer, WORD wNpcIdx, WORD wParam=0 );	
	int UseShopItem(CPlayer* pPlayer, SHOPITEMUSEBASE pInfo, SHOPITEMBASE* pShopItem);
	BOOL IsUseAbleShopItem( CPlayer* pPlayer, WORD ItemIdx, POSTYPE ItemPos );
	void ShopItemUseUpgrade(ITEM_INFO* pShopItemInfo, CPlayer* pPlayer, POSTYPE ItemPos, WORD wItemIdx);
	int ReinforceItemWithShopItem(CPlayer* pPlayer, WORD wTargetItemIdx, POSTYPE TargetPos, WORD wShopItemIdx, POSTYPE ShopItemPos, ITEM_JEWEL_POS_EX * pJewelInfo, WORD wJewelNum);

	BOOL LoadAvatarEquipList();
	BOOL ItemUnsealing(CPlayer* pPlayer, POSTYPE absPos);
	
	int CheatObtainShopItem( CPlayer* pPlayer, WORD obtainItemIdx, DWORD ItemNum );
	
	BOOL	CheckHackItemMove( CPlayer* pPlayer, CItemSlot* pFromSlot, CItemSlot* pToSlot );

	ITEM_INFO* IsUnfitItemInfoListForHide(WORD idx) { return m_UnfitItemInfoListForHide.GetData(idx); }

	//ITEM_INFO* IsCanMixItem(WORD idx) { return m_IsCanMixItem.GetData(idx); }

	ITEM_INFO* IsUnfitItemInfoListForShout(WORD idx) { return m_UnfitItemInfoListForShout.GetData(idx); }
	/////////////////////////////////////////////

	//SW070308 Â¨¥Ê??±Áù°?õË§´?ÆË? ?ëËë¨
	void	SetItemOptionsInfoMsg(CPlayer* pItemOwner, ITEMBASE* pItemBase, MSG_LINKITEMOPTIONS* rtMsg);

	///// 2007. 6. 8. CBH - ?ÆÔöæÂ¨¥Ê?ÈΩ?Â©¶Ê∫º ?ÅÁÜ± Ëπ∫È? ///////////////////////////
	BOOL LoadSetItemOption();
	SET_ITEM_OPTION* GetSetItemOption(WORD wSetItemKind, WORD wSetValue);
	void RemoveSetItemOption(WORD wIndex, SET_ITEM_OPTION* pSetItemOptionOut = NULL);
	CYHHashTable<SET_ITEM_OPTION>* GetSetItemOptionList();
	/////////////////////////////////////////////////////////////////////////////

	BOOL	CheckDemandItem(CPlayer* pPlayer, MSG_ITEM_BUY_SYN* pmsg);
	BOOL	PayForDeal(CPlayer* pPlayer, DWORD DealerIdx, DWORD dwPayItemIdx, POSTYPE PayItemPos = 99, DWORD dwPayItemNum = 1);

	BOOL CheckWeaponToShopItem(CPlayer* pPlayer, WORD wItemIndex); //2007. 10. 15. CBH - Èº†Êô¶ Â¨¥Â§•È°?Â¨¥Ê?ÈΩ?Êø∞È? Â©¶Ê∫º ?ÅÁÜ± Ëπ∫È?

	// Ê™úÊº∏?æË? È±îÔ???
	WORD ObtainItemFromGameEvent( CPlayer* pPlayer, DWORD dwItemIdx, WORD wItemNum );

	
	void LoadTipItem();
	sTIPITEMINFO * GetTipItem(WORD ItemIdx);

	//void LoadMixItemInfo();
	//BOOL CanMixItem(WORD wItemIndex);
	void LoadAlexXUpGradeItemPercent();		
};


#endif // !defined(AFX_ITEMMANAGER_H__B48BE6A4_2998_4459_B958_995D81EAAF1E__INCLUDED_)


