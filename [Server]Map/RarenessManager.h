// RarenessManager.h: interface for the RarenessManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RARENESSMANAGER_H__98F09FB6_6D0C_45D2_A95A_192AA6ECDEDE__INCLUDED_)
#define AFX_RARENESSMANAGER_H__98F09FB6_6D0C_45D2_A95A_192AA6ECDEDE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Player.h"

#define RAREITEMMGR	USINGTON(RarenessManager)
//普通物品和百宝物品的分界线  55101
#define BOUNDARY_INDEX_FOR_SHOPITEM	65535	//"itemlist.bin" 55101//changed by rooke
#define RARE_VALUE_PROB_LIST_MAX	100

//enum WeaponKind{ GUM, DO, CHANG, AMGI, GOONG, WeaponKindMAX = 5 };
//enum ProtectorKind{ DRESS, HAT, SHOES, ProtectorkindMAX = 3 };
//enum AccessaryKind{ RING, CAPE, NECKLACE, ARMLET, BELT, AccessaryKindMAX = 5 };

//struct 
class RarenessManager
{
	CYHHashTable<sRareItemInfo>	m_RareItemInfoTable;

	sRareOptionInfo m_RereItemInfo[RareItemKindMAX];

	DWORD	m_dwRareValueProb[RARE_VALUE_PROB_LIST_MAX];
	//RereItem's Additional Opt Info
//	sRareOptionWeaponInfo		m_Weapon[WeaponKindMAX];
//	sRareOptionProtectorInfo	m_Protector[ProtectorkindMAX];
//	sRareOptionBase				m_Accessary[AccessaryKindMAX];

public:
	RarenessManager();
	virtual ~RarenessManager();

	void Init();
	void Release();

	bool LoadRareItemOptionInfo();	//技何 可记 蔼 汲沥//窃荐疙篮 捞抚栏肺 扁瓷阑 且 荐 乐阑 沥档肺!
	bool LoadRareItemInfo();	//酒捞袍 牢郸胶(辆幅) 喊 靛酚犬伏
	bool LoadRareValueProbList();
	void ReleaseRareItemInfo();	//饭绢牢器 抛捞喉 秦力
	//饭绢酒捞袍 犬伏 魄喊(饭绢啊 登蠢衬 付蠢衬)
	bool GetRare(WORD ObtainItemIdx, ITEM_RARE_OPTION_INFO* pRareOptionInfo, CPlayer* pPlayer, BOOL bRare = FALSE);
	//(饭绢捞搁)秦寸 酒捞袍 辆幅俊 嘎绰 饭绢 蔼 技泼
//	void MakeRareWeapon(ITEM_RARE_OPTION_INFO* pRareInfo);
//	void MakeRareProtector(ITEM_RARE_OPTION_INFO* pRareInfo);
//	void MakeRareAccessary(ITEM_RARE_OPTION_INFO* pRareInfo);
	DWORD GetRareItemValue(DWORD RndMin, DWORD RndMax, WORD wRareRate);

	//DB俊 饭绢 沥焊 殿废
	BOOL IsRareItemAble(DWORD ItemIdx)
	{
		if (m_RareItemInfoTable.GetData(ItemIdx))			return TRUE;
		return FALSE;
	}
	bool GetRareExt(WORD ObtainItemIdx, ITEM_RARE_OPTION_INFO* pRareOptionInfo, CPlayer* pPlayer, ITEMBASE* pBaseItem, BOOL bRare);
};

EXTERNGLOBALTON(RarenessManager)

#endif // !defined(AFX_RARENESSMANAGER_H__98F09FB6_6D0C_45D2_A95A_192AA6ECDEDE__INCLUDED_)
