




// StatsCalcManager.h: interface for the CStatsCalcManager class.
//
//////////////////////////////////////////////////////////////////////


#if !defined(AFX_STATSCALCMANAGER_H__1BA578DC_5092_4667_8FFC_6E3B15BF7B2E__INCLUDED_)
#define AFX_STATSCALCMANAGER_H__1BA578DC_5092_4667_8FFC_6E3B15BF7B2E__INCLUDED_

#if _MSC_VER > 1000
#pragma once

#endif // _MSC_VER > 1000

#ifdef __MAPSERVER__
#define PLAYERTYPE CPlayer
class CPlayer;
#else
#define PLAYERTYPE CHero
class CHero;
#endif

#include "..\[CC]Header\GameResourceStruct.h"


#define STATSMGR	CStatsCalcManager::GetInstance()


class CStatsCalcManager  
{
	CYHHashTable<ITEM_INFO> m_StoneItemList; // 2014-11-20 ��ʱ

	CStatsCalcManager();
public:	
	MAKESINGLETON(CStatsCalcManager);
	virtual ~CStatsCalcManager();


	/*
	void DoCalcBaseStats(PLAYERTYPE* pPlayer);
	void DoCalcItemStats(PLAYERTYPE* pPlayer);
	void DoCalcMugongStats(PLAYERTYPE* pPlayer);
	*/

	// È£Ãâ ¿ì¼±¼øÀ§ ¼ø!
	void CalcItemStats(PLAYERTYPE* pPlayer);	
	void CalcCharStats(PLAYERTYPE* pPlayer);

	void Clear(player_calc_stats * pStats);

	

	//2007. 6. 11. CBH - 세트아이�?능력�?관�?함수
	void CalcSetItemStats();

	// magi82 - UniqueItem(070626)
	void CalcUniqueItemStats();
	


	void CalcTitanStats(DWORD dwDBIdx);	// magi82 - Titan(070809)
};

#endif // !defined(AFX_STATSCALCMANAGER_H__1BA578DC_5092_4667_8FFC_6E3B15BF7B2E__INCLUDED_)


