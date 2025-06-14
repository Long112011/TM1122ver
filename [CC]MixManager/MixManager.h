#pragma once
#include "..\[CC]Header\GameResourceStruct.h"
#include <map>
#ifdef _MAPSERVER_
#define MIXMGR CMixManager::GetInstance()
#else
#define MIXMGR USINGTON(CMixManager)
#endif
class CMHFile;
class ITEM_MIX_INFO;
class ITEM_MIX_RES;
#define MAX_MIX_PERCENT	10000
#define MAX_MIX_LARGE_PERCENT	1000000
class CMixManager  
{
private:
	std::map<WORD,ITEM_MIX_INFO> m_MixItemInfoList;
#ifdef _MAPSERVER_
	CYHHashTable<HILEVEL_ITEM_MIX_RATE_INFO> m_HiLevelItemMixRateInfoList;
#endif
public:
	MAKESINGLETON(CMixManager);
	CMixManager();
	virtual ~CMixManager();
	void	Release();
	void LoadMixList();
	void LoadMixResultItemInfo(CMHFile * fp, ITEM_MIX_RES * pResInfo);
	const ITEM_MIX_INFO * GetMixItemInfo(WORD wItemIdx) const;
#ifdef _MAPSERVER_
	int MixItem(CPlayer* pPlayer, WORD wBasicItemIdx, POSTYPE BasicItemPos, POSTYPE ResultIdx, MATERIAL_ARRAY * pMaterialArray, WORD wMaterialNum, WORD ShopItemIdx, WORD ShopItemPos, BOOL bTitan = FALSE);	// magi82 - Titan(070118)
	BOOL EnoughMixMaterial(WORD needItemIdx, WORD needItemNum, MATERIAL_ARRAY * pMaterialArray, WORD wMaterialNum);
	BOOL CanMixItem(WORD wItemIndex);
	void NetworkMsgParse( DWORD dwConnectionIndex, BYTE Protocol, void* pMsg );
#endif
};
#ifndef _MAPSERVER_
EXTERNGLOBALTON(CMixManager)
#endif