// ChangeItemMgr.h: interface for the CChangeItemMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHANGEITEMMGR_H__94A4A82B_1B66_44EE_9965_C0BAFC2B306C__INCLUDED_)
#define AFX_CHANGEITEMMGR_H__94A4A82B_1B66_44EE_9965_C0BAFC2B306C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define CHANGEITEMMGR CChangeItemMgr::GetInstance()


#define MAX_CHANGE_RATE	30001
/*
struct sCHANGEITEMUNIT
{
	WORD	wToItemIdx;
	DWORD	dwToItemDur;
	DWORD	dwPercent;
};

struct sCHANGEITEM
{
	WORD	wItemIdx;
	WORD	wMaxToItem;
	sCHANGEITEMUNIT*	pItemUnit;

	sCHANGEITEM()	{ wItemIdx = wMaxToItem = 0; }
	~sCHANGEITEM()	{ if( pItemUnit ) { delete [] pItemUnit; } }
};

struct sMULTICHANGEITEM
{
	WORD	wItemIdx;
	WORD	wLimitLevel;
	WORD	wMaxSet;
	unsigned int    nMaxItemSpace;
	sCHANGEITEM*	pChangeItem;
};*/
class CChangeItemMgr
{
	//enum ChangeType
	//{
	//	ChangeToMonster	= 7995,
	//	ChangeToEvent2	= 7996,
	//	ChangeToSuryun	= 7997,
	//	ChangeToEvent1	= 7998,
	//	ChangeToMoney	= 7999,

	//	MaxChangeType	= 8000,
	//};

	CYHHashTable<sCHANGEITEM>		m_ChangeItemList;
	CYHHashTable<sMULTICHANGEITEM>	m_MultiChangeItemList;

public:
	MAKESINGLETON(CChangeItemMgr);
	CChangeItemMgr();
	virtual ~CChangeItemMgr();

	void	Init();
	void	Release();
	BOOL	LoadItemChange();

	int			 UseChangeItem(CPlayer* pPlayer, WORD TargetPos, WORD wItemIdx);
	int			 UseNormalChangeItem(CPlayer* pPlayer, WORD TargetPos, WORD wItemIdx, ITEMBASE* pItem);
	int			 UseMultiChangeItem(CPlayer* pPlayer, WORD TargetPos, WORD wItemIdx, ITEMBASE* pItem);
	unsigned int ChangedTotalItemNum(sMULTICHANGEITEM* pMultiChangeItem, WORD wMaxSet);//060613 Add by wonju

	// ShopItem
	int		UseChangeItemFromShopItem(CPlayer* pPlayer, WORD wItemIdx);
	int		UseNormalChangeItemFromShopItem(CPlayer* pPlayer, WORD wItemIdx);
	int		UseMultiChangeItemFromShopItem(CPlayer* pPlayer, WORD wItemIdx);
	//

	sCHANGEITEMUNIT* GetItemUnitFromCalPercent(WORD wItemIdx);
	sCHANGEITEMUNIT* GetMultiItemUnitFromCalPercent(sCHANGEITEM* pSet);
	sCHANGEITEMUNIT* GetMaxSpaceItemRef(sCHANGEITEM* pSet, DWORD& nMin);

	//2009. 1. 14. CBH - 牢亥 浇吩 八荤
	BOOL IsMultiChangeItemSlot(CPlayer* pPlayer, sMULTICHANGEITEM* pMultiChangeItem);
	BOOL IsNormalChangeItemSlot(CPlayer* pPlayer, sCHANGEITEMUNIT* pNormalChangeItem);
	//箱子物品是否加锁
	WORD GetToItemTypeForNormal(WORD wItemIdx);
	//开箱要是类型检查
	BOOL CheckBoxKey(WORD wCurNeedBoxKey, CPlayer* pPlayer, WORD& wKeyItemPos, DWORD& dwKeyItemIdx);
};

#endif // !defined(AFX_CHANGEITEMMGR_H__94A4A82B_1B66_44EE_9965_C0BAFC2B306C__INCLUDED_)
