// RarenessManager.cpp: implementation of the RarenessManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RarenessManager.h"
#include "MHFile.h"
#include "ItemManager.h"
#include "..\[CC]Header\GameResourceStruct.h"
#include <time.h>
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

GLOBALTON(RarenessManager)

RarenessManager::RarenessManager()
{
	Init();
}

RarenessManager::~RarenessManager()
{
	Release();
}

void RarenessManager::Init()
{
	m_RareItemInfoTable.Initialize(64);
}

void RarenessManager::Release()
{
	ReleaseRareItemInfo();
}

bool RarenessManager::LoadRareItemOptionInfo()
{
	CMHFile fp;

	char szBuf[256] = { 0, };
	char line[512];

	char FileName[256];

#ifdef _FILE_BIN_
	//	sprintf(FileName, "Resource/Item_RareOptionInfo.bin");
	sprintf(FileName, "Resource/Server/Item_RareOptionInfo.bin");
	if (!fp.Init(FileName, "rb"))
		return FALSE;
#else
	sprintf(FileName, "Reaource/Item_RareOptionInfo.txt");
	if (!fp.Init(FileName, "rt"))
		return FALSE;
#endif

	DWORD dwItemKindIdx = 0;
	while (!fp.IsEOF())
	{
		if (dwItemKindIdx == RareItemKindMAX)
			break;

		fp.GetString(szBuf);

		if (szBuf[0] == '@')
		{
			fp.GetLineX(line, 512);
			continue;
		}

		if (szBuf[0] == '#')
		{
			m_RereItemInfo[dwItemKindIdx].wRareRate = fp.GetWord();//–¬‘ˆº∏¬ øÿ÷∆
			m_RereItemInfo[dwItemKindIdx].GenGol_Min = fp.GetWord();
			m_RereItemInfo[dwItemKindIdx].GenGol = fp.GetWord();
			m_RereItemInfo[dwItemKindIdx].MinChub_Min = fp.GetWord();
			m_RereItemInfo[dwItemKindIdx].MinChub = fp.GetWord();
			m_RereItemInfo[dwItemKindIdx].CheRyuk_Min = fp.GetWord();
			m_RereItemInfo[dwItemKindIdx].CheRyuk = fp.GetWord();
			m_RereItemInfo[dwItemKindIdx].SimMek_Min = fp.GetWord();
			m_RereItemInfo[dwItemKindIdx].SimMek = fp.GetWord();
			m_RereItemInfo[dwItemKindIdx].Life_Min = fp.GetDword();
			m_RereItemInfo[dwItemKindIdx].Life = fp.GetDword();
			m_RereItemInfo[dwItemKindIdx].NaeRyuk_Min = fp.GetWord();
			m_RereItemInfo[dwItemKindIdx].NaeRyuk = fp.GetWord();
			m_RereItemInfo[dwItemKindIdx].Shield_Min = fp.GetDword();
			m_RereItemInfo[dwItemKindIdx].Shield = fp.GetDword();
			m_RereItemInfo[dwItemKindIdx].PhyAttack_Min = fp.GetWord();
			m_RereItemInfo[dwItemKindIdx].PhyAttack = fp.GetWord();
			m_RereItemInfo[dwItemKindIdx].PhyDefense_Min = fp.GetWord();
			m_RereItemInfo[dwItemKindIdx].PhyDefense = fp.GetWord();
			for (int i = ATTR_FIRE; i <= ATTR_MAX; ++i)
			{
				m_RereItemInfo[dwItemKindIdx].AttrAttack_Min.SetElement_Val(i, fp.GetFloat());
				m_RereItemInfo[dwItemKindIdx].AttrAttack.SetElement_Val(i, fp.GetFloat());
			}
			for (int j = ATTR_FIRE; j <= ATTR_MAX; ++j)
			{
				m_RereItemInfo[dwItemKindIdx].AttrRegist_Min.SetElement_Val(j, fp.GetFloat());
				m_RereItemInfo[dwItemKindIdx].AttrRegist.SetElement_Val(j, fp.GetFloat());
			}

			dwItemKindIdx++;
		}

	}

	return TRUE;
}

bool RarenessManager::LoadRareItemInfo()
{
	CMHFile fp;

	char szBuf[256] = { 0, };
	char line[512];

	char FileName[256];

#ifdef _FILE_BIN_
	sprintf(FileName, "Resource/Item_RareItemInfo.bin");
	if (!fp.Init(FileName, "rb")) return FALSE;
#else
	sprintf(FileName, "Resource/Item_RareItemInfo.txt");
	if (!fp.Init(FileName, "rt")) return FALSE;
#endif

	while (!fp.IsEOF())
	{
		fp.GetString(szBuf);

		if (szBuf[0] == '@')
		{
			fp.GetLineX(line, 512);
			continue;
		}

		//		strupr(szBuf);

		if (szBuf[0] == '*')
		{
			sRareItemInfo* pRareItemInfo = new sRareItemInfo;
			pRareItemInfo->ItemIdx = fp.GetDword();
			pRareItemInfo->RareProb = fp.GetDword();

			ASSERT(!m_RareItemInfoTable.GetData(pRareItemInfo->ItemIdx));
			m_RareItemInfoTable.Add(pRareItemInfo, pRareItemInfo->ItemIdx);
		}
	}

	return TRUE;
}

bool RarenessManager::LoadRareValueProbList()
{
	CMHFile fp;

	char FileName[256];
	char szBuf[256] = { 0, };

#ifdef _FILE_BIN_
	//	sprintf(FileName, "Resource/Item_RareStatSetRate.bin");
	sprintf(FileName, "Resource/Server/Item_RareStatSetRate.bin");
	if (!fp.Init(FileName, "rb")) return FALSE;
#else
	sprintf(FileName, "Resource/Item_RareStatSetRate.txt");
	if (!fp.Init(FileName, "rt")) return FALSE;
#endif

	fp.GetLine(szBuf, 256); //@

	DWORD Index = 1;
	while (!fp.IsEOF())
	{
		if (Index == RARE_VALUE_PROB_LIST_MAX) break;

		Index = fp.GetDword();	//1~100
		m_dwRareValueProb[Index - 1] = fp.GetDword();
	}
	fp.Release();
	return TRUE;
}

void RarenessManager::ReleaseRareItemInfo()
{
	sRareItemInfo* pList = NULL;
	m_RareItemInfoTable.SetPositionHead();
	while (pList = m_RareItemInfoTable.GetData())
	{
		delete pList;
		pList = NULL;
	}
	m_RareItemInfoTable.RemoveAll();
}

bool RarenessManager::GetRare(WORD ObtainItemIdx, ITEM_RARE_OPTION_INFO* pRareOptionInfo, CPlayer* pPlayer, BOOL bRare)
{//◊∞±∏◊£∏£º∏¬ º∆À„
	if (ObtainItemIdx > BOUNDARY_INDEX_FOR_SHOPITEM) return FALSE;	//WeaponType ∏¶ òﬁ æ∆¿Ã≈€∞˙ ∞∞¿Ãæ≤∞Ì ¿÷¥Ÿ.

	ITEM_INFO* pInfo = ITEMMGR->GetItemInfo(ObtainItemIdx);
	if (!pInfo) return FALSE; //µÓ∑œµ» æ∆¿Ã≈€¿Ã æ∆¥œ∏È ∏Æ≈œ

	eITEM_KINDBIT eItemKind = GetItemKind(ObtainItemIdx);	//!¿Œ¿⁄ »Æ¿Œ
	if (!(eItemKind & eEQUIP_ITEM)) return FALSE;	//¿Â¬¯∑˘ æ∆¿Ã≈€¿Ã æ∆¥œ∏È ∏Æ≈œ


	//HK080529	//itemidex ∞° ¿ÃπÃ ¡∂«’∞¸∑√ %10¿« ∑Í¿ª ≤£¥Ÿ.
	DWORD ItemIdx = ObtainItemIdx;
	//«ÿ¥Á æ∆¿Ã≈€¿« ∑πæÓ ª˝º∫ »Æ∑¸¿ª ∞°¡ÆøÕ ∫Ò±≥.
	//¥À¥¶◊¢ Õ∑¿÷π◊È∫œ∫Ûµƒ“¬∑˛◊£∏£≤ªµΩµ»«Èøˆ   //removed by rooke
	if (eItemKind == eEQUIP_ITEM_WEAPON
		|| eItemKind == eEQUIP_ITEM_DRESS
		|| eItemKind == eEQUIP_ITEM_HAT
		|| eItemKind == eEQUIP_ITEM_SHOES)
	{
		ItemIdx /= 10;
		ItemIdx *= 10;
	}	//æ«ººªÁ∏Æ∏¶ ¡¶ø‹«— +¡∂«’ æ∆¿Ã≈€µÈ¿« ∏ÆΩ∫∆Æ ¿Œµ¶Ω∫¥¬ «œ≥™∑Œ ≈Î¿œ. ¡§∫∏ ±∏«‘.

	sRareItemInfo* RareInfo = m_RareItemInfoTable.GetData(ItemIdx);

	if (!RareInfo) return FALSE;
	//!!!+
	//	if( (rand()%10000) * 10000 + (rand()%10000 + 1) < RareInfo->RareProb )

		//BOOL baa = ( (DWORD)((rand()%10000) * 10000 + (rand()%10000 + 1)) <= (RareInfo->RareProb) );
	BOOL baa = FALSE;
	if (FALSE == bRare)
	{
		float addPob = 1.0f;	// 50%√ﬂ∞° -> 0.5f
		pPlayer->GetPetManager()->GetPetBuffResultRt(ePB_Item_RareProbUp, &addPob);
		baa = (((DWORD)((rand() % 10000) * 10000 + (rand() % 10000 + 1))) * addPob <= (RareInfo->RareProb));
		//œ¬√Ê≤‚ ‘”√£¨∑≈¥ÛµÙ¬‰◊∞±∏¬Ãº∏¬ 
		//baa = (DWORD)(rand()%10000) *addPob <= RareInfo->RareProb ;

		//DWORD rnd = ((rand()%10000) * 10000 + (rand()%10000 + 1));
		//baa = ( DWORD(rnd*addPob) <= (RareInfo->RareProb) );
	}
	if (baa || bRare)	//∑πæÓ¥Ÿ!!!
	{
		DWORD RareItemKind = 0;	//∏‚πˆ πËø≠¿« ¿Œ¿⁄∑Œ æ≤¿Œ¥Ÿ.
		if (eItemKind == eEQUIP_ITEM_WEAPON)
		{
			RareItemKind = pInfo->WeaponType - 1;
		}//0Ω£ 1»≠ 2µ∂ 3«π 4π≠ 5∞µ
		else if (eItemKind == eEQUIP_ITEM_DRESS)			RareItemKind = eRI_DRESS;//6
		else if (eItemKind == eEQUIP_ITEM_HAT)				RareItemKind = eRI_HAT;//7
		else if (eItemKind == eEQUIP_ITEM_SHOES)			RareItemKind = eRI_SHOES;//8
		else if (eItemKind == eEQUIP_ITEM_RING)			RareItemKind = eRI_RING;//9
		else if (eItemKind == eEQUIP_ITEM_CAPE)			RareItemKind = eRI_CAPE;//10
		else if (eItemKind == eEQUIP_ITEM_NECKLACE)		RareItemKind = eRI_NECKLACE;//11
		else if (eItemKind == eEQUIP_ITEM_ARMLET)			RareItemKind = eRI_ARMLET;//12
		else if (eItemKind == eEQUIP_ITEM_BELT)			RareItemKind = eRI_BELT;//13		

		if (m_RereItemInfo[RareItemKind].GenGol || m_RereItemInfo[RareItemKind].MinChub || m_RereItemInfo[RareItemKind].CheRyuk || m_RereItemInfo[RareItemKind].SimMek)	//«ˆ¿Á ¿ÂΩ≈±∏ ¿˚øÎ
		{
			switch (rand() % StatesKindMAX)
			{
			case eRI_GENGOL:
				pRareOptionInfo->GenGol = (WORD)GetRareItemValue((DWORD)m_RereItemInfo[RareItemKind].GenGol_Min, (DWORD)(m_RereItemInfo[RareItemKind].GenGol), m_RereItemInfo[RareItemKind].wRareRate);
				break;
			case eRI_MINCHUB:
				pRareOptionInfo->MinChub = (WORD)GetRareItemValue((DWORD)m_RereItemInfo[RareItemKind].MinChub_Min, (DWORD)(m_RereItemInfo[RareItemKind].MinChub), m_RereItemInfo[RareItemKind].wRareRate);
				break;
			case eRI_CHERYUK:
				pRareOptionInfo->CheRyuk = (WORD)GetRareItemValue((DWORD)m_RereItemInfo[RareItemKind].CheRyuk_Min, (DWORD)(m_RereItemInfo[RareItemKind].CheRyuk), m_RereItemInfo[RareItemKind].wRareRate);
				break;
			case eRI_SIMMEK:
				pRareOptionInfo->SimMek = (WORD)GetRareItemValue((DWORD)m_RereItemInfo[RareItemKind].SimMek_Min, (DWORD)(m_RereItemInfo[RareItemKind].SimMek), m_RereItemInfo[RareItemKind].wRareRate);
				break;
			default:
				ASSERT(0);
				break;
			}
		}
		else
		{
			//			ASSERTMSG(0, "RareItem : Not All RareOption_StatesSet on Script");

			pRareOptionInfo->GenGol = (WORD)GetRareItemValue((DWORD)m_RereItemInfo[RareItemKind].GenGol_Min, (DWORD)(m_RereItemInfo[RareItemKind].GenGol), m_RereItemInfo[RareItemKind].wRareRate);
			pRareOptionInfo->MinChub = (WORD)GetRareItemValue((DWORD)m_RereItemInfo[RareItemKind].MinChub_Min, (DWORD)(m_RereItemInfo[RareItemKind].MinChub), m_RereItemInfo[RareItemKind].wRareRate);
			pRareOptionInfo->CheRyuk = (WORD)GetRareItemValue((DWORD)m_RereItemInfo[RareItemKind].CheRyuk_Min, (DWORD)(m_RereItemInfo[RareItemKind].CheRyuk), m_RereItemInfo[RareItemKind].wRareRate);
			pRareOptionInfo->SimMek = (WORD)GetRareItemValue((DWORD)m_RereItemInfo[RareItemKind].SimMek_Min, (DWORD)(m_RereItemInfo[RareItemKind].SimMek), m_RereItemInfo[RareItemKind].wRareRate);
		}

		if (eItemKind == eEQUIP_ITEM_DRESS || eItemKind == eEQUIP_ITEM_HAT || eItemKind == eEQUIP_ITEM_SHOES)	//«ˆ¿Á πÊæÓ±∏ ¿˚øÎ
		{
			pRareOptionInfo->Life = GetRareItemValue(m_RereItemInfo[RareItemKind].Life_Min, m_RereItemInfo[RareItemKind].Life, m_RereItemInfo[RareItemKind].wRareRate);
			pRareOptionInfo->NaeRyuk = (WORD)GetRareItemValue((DWORD)m_RereItemInfo[RareItemKind].NaeRyuk_Min, (DWORD)(m_RereItemInfo[RareItemKind].NaeRyuk), m_RereItemInfo[RareItemKind].wRareRate);
			pRareOptionInfo->Shield = GetRareItemValue(m_RereItemInfo[RareItemKind].Shield_Min, m_RereItemInfo[RareItemKind].Shield, m_RereItemInfo[RareItemKind].wRareRate);
		}
		else if (m_RereItemInfo[RareItemKind].Life && m_RereItemInfo[RareItemKind].NaeRyuk && m_RereItemInfo[RareItemKind].Shield)	//«ˆ¿Á π´±‚ ¿˚øÎ
		{
			switch (rand() % ValueKindMAX)
			{
			case eRI_LIFE:
				pRareOptionInfo->Life = GetRareItemValue(m_RereItemInfo[RareItemKind].Life_Min, m_RereItemInfo[RareItemKind].Life, m_RereItemInfo[RareItemKind].wRareRate);
				break;
			case eRI_NAERYUK:
				pRareOptionInfo->NaeRyuk = (WORD)GetRareItemValue((DWORD)m_RereItemInfo[RareItemKind].NaeRyuk_Min, (DWORD)(m_RereItemInfo[RareItemKind].NaeRyuk), m_RereItemInfo[RareItemKind].wRareRate);
				break;
			case eRI_SHIELD:
				pRareOptionInfo->Shield = GetRareItemValue(m_RereItemInfo[RareItemKind].Shield_Min, m_RereItemInfo[RareItemKind].Shield, m_RereItemInfo[RareItemKind].wRareRate);
				break;
			default:
				ASSERT(0);
				break;
			}
		}
		else
		{
			//			ASSERTMSG(0, "RareItem : Not All RareOption_AddValue on Script");

			pRareOptionInfo->Life = GetRareItemValue(m_RereItemInfo[RareItemKind].Life_Min, m_RereItemInfo[RareItemKind].Life, m_RereItemInfo[RareItemKind].wRareRate);
			pRareOptionInfo->NaeRyuk = (WORD)GetRareItemValue((DWORD)m_RereItemInfo[RareItemKind].NaeRyuk_Min, (DWORD)(m_RereItemInfo[RareItemKind].NaeRyuk), m_RereItemInfo[RareItemKind].wRareRate);
			pRareOptionInfo->Shield = GetRareItemValue(m_RereItemInfo[RareItemKind].Shield_Min, m_RereItemInfo[RareItemKind].Shield, m_RereItemInfo[RareItemKind].wRareRate);
		}

		pRareOptionInfo->PhyAttack = (WORD)GetRareItemValue((DWORD)m_RereItemInfo[RareItemKind].PhyAttack_Min, (DWORD)(m_RereItemInfo[RareItemKind].PhyAttack), m_RereItemInfo[RareItemKind].wRareRate);
		pRareOptionInfo->PhyDefense = (WORD)GetRareItemValue((DWORD)m_RereItemInfo[RareItemKind].PhyDefense_Min, (DWORD)(m_RereItemInfo[RareItemKind].PhyDefense), m_RereItemInfo[RareItemKind].wRareRate);

		WORD i = rand() % ATTR_MAX + 1;

		if (rand() % 2)	//∞¯∞› or πÊæÓ
		{
			pRareOptionInfo->AttrAttack.SetElement_Val(i, (float)GetRareItemValue((DWORD)m_RereItemInfo[RareItemKind].AttrAttack_Min.GetElement_Val(i)
				, (DWORD)(m_RereItemInfo[RareItemKind].AttrAttack.GetElement_Val(i)), m_RereItemInfo[RareItemKind].wRareRate) / 100.f);
		}
		else
		{
			pRareOptionInfo->AttrRegist.SetElement_Val(i, (float)GetRareItemValue((DWORD)m_RereItemInfo[RareItemKind].AttrRegist_Min.GetElement_Val(i)
				, (DWORD)(m_RereItemInfo[RareItemKind].AttrRegist.GetElement_Val(i)), m_RereItemInfo[RareItemKind].wRareRate) / 100.f);
		}
		return TRUE;
	}
	else
		return FALSE;
}
bool RarenessManager::GetRareExt(WORD ObtainItemIdx, ITEM_RARE_OPTION_INFO* pRareOptionInfo, CPlayer* pPlayer, ITEMBASE* pBaseItem, BOOL bRare)
{//◊∞±∏◊£∏£º∏¬ º∆À„
	if (ObtainItemIdx > BOUNDARY_INDEX_FOR_SHOPITEM) return FALSE;	//WeaponType ∏¶ òﬁ æ∆¿Ã≈€∞˙ ∞∞¿Ãæ≤∞Ì ¿÷¥Ÿ.

	ITEM_INFO* pInfo = ITEMMGR->GetItemInfo(ObtainItemIdx);
	if (!pInfo) return FALSE; //µÓ∑œµ» æ∆¿Ã≈€¿Ã æ∆¥œ∏È ∏Æ≈œ

	eITEM_KINDBIT eItemKind = GetItemKind(ObtainItemIdx);	//!¿Œ¿⁄ »Æ¿Œ
	if (!(eItemKind & eEQUIP_ITEM)) return FALSE;	//¿Â¬¯∑˘ æ∆¿Ã≈€¿Ã æ∆¥œ∏È ∏Æ≈œ


	//HK080529	//itemidex ∞° ¿ÃπÃ ¡∂«’∞¸∑√ %10¿« ∑Í¿ª ≤£¥Ÿ.
	DWORD ItemIdx = ObtainItemIdx;
	//«ÿ¥Á æ∆¿Ã≈€¿« ∑πæÓ ª˝º∫ »Æ∑¸¿ª ∞°¡ÆøÕ ∫Ò±≥.
	//¥À¥¶◊¢ Õ∑¿÷π◊È∫œ∫Ûµƒ“¬∑˛◊£∏£≤ªµΩµ»«Èøˆ   //removed by rooke
	if (eItemKind == eEQUIP_ITEM_WEAPON
		|| eItemKind == eEQUIP_ITEM_DRESS
		|| eItemKind == eEQUIP_ITEM_HAT
		|| eItemKind == eEQUIP_ITEM_SHOES)
	{
		ItemIdx /= 10;
		ItemIdx *= 10;
	}	//æ«ººªÁ∏Æ∏¶ ¡¶ø‹«— +¡∂«’ æ∆¿Ã≈€µÈ¿« ∏ÆΩ∫∆Æ ¿Œµ¶Ω∫¥¬ «œ≥™∑Œ ≈Î¿œ. ¡§∫∏ ±∏«‘.

	sRareItemInfo* RareInfo = m_RareItemInfoTable.GetData(ItemIdx);

	if (!RareInfo) return FALSE;
	//!!!+
	//	if( (rand()%10000) * 10000 + (rand()%10000 + 1) < RareInfo->RareProb )

	//BOOL baa = ( (DWORD)((rand()%10000) * 10000 + (rand()%10000 + 1)) <= (RareInfo->RareProb) );

	//ªÒ»°±≥∞¸ŒÔ∆∑µƒ◊∞±∏∆∑÷ –≈œ¢µ»
	//SET_ITEMQUALITY_OPTION* pSetItemOption = NULL;	
	//pSetItemOption = ITEMMGR->GetSetItemQualityOption(pBaseItem->ItemQuality,pBaseItem->ItemEntry1,pBaseItem->ItemEntry2);

	float val = 1.0;
	if (pBaseItem->ItemQuality == 0) val = 1.0;
	else if (pBaseItem->ItemQuality == 1) val = 1.2;
	else if (pBaseItem->ItemQuality == 2) val = 1.5;
	else if (pBaseItem->ItemQuality == 3) val = 2.0;
	else if (pBaseItem->ItemQuality == 4) val = 3.0;

	BOOL baa = FALSE;
	if (FALSE == bRare)
	{
		float addPob = 1.0f;	// 50%√ﬂ∞° -> 0.5f
		pPlayer->GetPetManager()->GetPetBuffResultRt(ePB_Item_RareProbUp, &addPob);
		baa = (((DWORD)((rand() % 10000) * 10000 + (rand() % 10000 + 1))) * addPob <= (RareInfo->RareProb));
		//œ¬√Ê≤‚ ‘”√£¨∑≈¥ÛµÙ¬‰◊∞±∏¬Ãº∏¬ 
		//baa = (DWORD)(rand()%10000) *addPob <= RareInfo->RareProb ;

		//DWORD rnd = ((rand()%10000) * 10000 + (rand()%10000 + 1));
		//baa = ( DWORD(rnd*addPob) <= (RareInfo->RareProb) );
	}
	if (baa || bRare)	//∑πæÓ¥Ÿ!!!
	{
		DWORD RareItemKind = 0;	//∏‚πˆ πËø≠¿« ¿Œ¿⁄∑Œ æ≤¿Œ¥Ÿ.
		if (eItemKind == eEQUIP_ITEM_WEAPON)
		{
			RareItemKind = pInfo->WeaponType - 1;
		}//0Ω£ 1»≠ 2µ∂ 3«π 4π≠ 5∞µ
		else if (eItemKind == eEQUIP_ITEM_DRESS)			RareItemKind = eRI_DRESS;//6
		else if (eItemKind == eEQUIP_ITEM_HAT)				RareItemKind = eRI_HAT;//7
		else if (eItemKind == eEQUIP_ITEM_SHOES)			RareItemKind = eRI_SHOES;//8
		else if (eItemKind == eEQUIP_ITEM_RING)			RareItemKind = eRI_RING;//9
		else if (eItemKind == eEQUIP_ITEM_CAPE)			RareItemKind = eRI_CAPE;//10
		else if (eItemKind == eEQUIP_ITEM_NECKLACE)		RareItemKind = eRI_NECKLACE;//11
		else if (eItemKind == eEQUIP_ITEM_ARMLET)			RareItemKind = eRI_ARMLET;//12
		else if (eItemKind == eEQUIP_ITEM_BELT)			RareItemKind = eRI_BELT;//13		

		if (m_RereItemInfo[RareItemKind].GenGol || m_RereItemInfo[RareItemKind].MinChub || m_RereItemInfo[RareItemKind].CheRyuk || m_RereItemInfo[RareItemKind].SimMek)	//«ˆ¿Á ¿ÂΩ≈±∏ ¿˚øÎ
		{
			switch (rand() % StatesKindMAX)
			{
			case eRI_GENGOL:
				pRareOptionInfo->GenGol = (WORD)GetRareItemValue((DWORD)m_RereItemInfo[RareItemKind].GenGol_Min, (DWORD)(m_RereItemInfo[RareItemKind].GenGol * val), m_RereItemInfo[RareItemKind].wRareRate);
				break;
			case eRI_MINCHUB:
				pRareOptionInfo->MinChub = (WORD)GetRareItemValue((DWORD)m_RereItemInfo[RareItemKind].MinChub_Min, (DWORD)(m_RereItemInfo[RareItemKind].MinChub * val), m_RereItemInfo[RareItemKind].wRareRate);
				break;
			case eRI_CHERYUK:
				pRareOptionInfo->CheRyuk = (WORD)GetRareItemValue((DWORD)m_RereItemInfo[RareItemKind].CheRyuk_Min, (DWORD)(m_RereItemInfo[RareItemKind].CheRyuk * val), m_RereItemInfo[RareItemKind].wRareRate);
				break;
			case eRI_SIMMEK:
				pRareOptionInfo->SimMek = (WORD)GetRareItemValue((DWORD)m_RereItemInfo[RareItemKind].SimMek_Min, (DWORD)(m_RereItemInfo[RareItemKind].SimMek * val), m_RereItemInfo[RareItemKind].wRareRate);
				break;
			default:
				ASSERT(0);
				break;
			}
		}
		else
		{
			//			ASSERTMSG(0, "RareItem : Not All RareOption_StatesSet on Script");

			pRareOptionInfo->GenGol = (WORD)GetRareItemValue((DWORD)m_RereItemInfo[RareItemKind].GenGol_Min, (DWORD)(m_RereItemInfo[RareItemKind].GenGol * val), m_RereItemInfo[RareItemKind].wRareRate);
			pRareOptionInfo->MinChub = (WORD)GetRareItemValue((DWORD)m_RereItemInfo[RareItemKind].MinChub_Min, (DWORD)(m_RereItemInfo[RareItemKind].MinChub * val), m_RereItemInfo[RareItemKind].wRareRate);
			pRareOptionInfo->CheRyuk = (WORD)GetRareItemValue((DWORD)m_RereItemInfo[RareItemKind].CheRyuk_Min, (DWORD)(m_RereItemInfo[RareItemKind].CheRyuk * val), m_RereItemInfo[RareItemKind].wRareRate);
			pRareOptionInfo->SimMek = (WORD)GetRareItemValue((DWORD)m_RereItemInfo[RareItemKind].SimMek_Min, (DWORD)(m_RereItemInfo[RareItemKind].SimMek * val), m_RereItemInfo[RareItemKind].wRareRate);
		}

		if (eItemKind == eEQUIP_ITEM_DRESS || eItemKind == eEQUIP_ITEM_HAT || eItemKind == eEQUIP_ITEM_SHOES)	//«ˆ¿Á πÊæÓ±∏ ¿˚øÎ
		{
			pRareOptionInfo->Life = GetRareItemValue(m_RereItemInfo[RareItemKind].Life_Min, m_RereItemInfo[RareItemKind].Life * val, m_RereItemInfo[RareItemKind].wRareRate);
			pRareOptionInfo->NaeRyuk = (WORD)GetRareItemValue((DWORD)m_RereItemInfo[RareItemKind].NaeRyuk_Min, (DWORD)(m_RereItemInfo[RareItemKind].NaeRyuk * val), m_RereItemInfo[RareItemKind].wRareRate);
			pRareOptionInfo->Shield = GetRareItemValue(m_RereItemInfo[RareItemKind].Shield_Min, m_RereItemInfo[RareItemKind].Shield * val, m_RereItemInfo[RareItemKind].wRareRate);
		}
		else if (m_RereItemInfo[RareItemKind].Life && m_RereItemInfo[RareItemKind].NaeRyuk && m_RereItemInfo[RareItemKind].Shield)	//«ˆ¿Á π´±‚ ¿˚øÎ
		{
			switch (rand() % ValueKindMAX)
			{
			case eRI_LIFE:
				pRareOptionInfo->Life = GetRareItemValue(m_RereItemInfo[RareItemKind].Life_Min, m_RereItemInfo[RareItemKind].Life * val, m_RereItemInfo[RareItemKind].wRareRate);
				break;
			case eRI_NAERYUK:
				pRareOptionInfo->NaeRyuk = (WORD)GetRareItemValue((DWORD)m_RereItemInfo[RareItemKind].NaeRyuk_Min, (DWORD)(m_RereItemInfo[RareItemKind].NaeRyuk * val), m_RereItemInfo[RareItemKind].wRareRate);
				break;
			case eRI_SHIELD:
				pRareOptionInfo->Shield = GetRareItemValue(m_RereItemInfo[RareItemKind].Shield_Min, m_RereItemInfo[RareItemKind].Shield * val, m_RereItemInfo[RareItemKind].wRareRate);
				break;
			default:
				ASSERT(0);
				break;
			}
		}
		else
		{
			//			ASSERTMSG(0, "RareItem : Not All RareOption_AddValue on Script");

			pRareOptionInfo->Life = GetRareItemValue(m_RereItemInfo[RareItemKind].Life_Min, m_RereItemInfo[RareItemKind].Life * val, m_RereItemInfo[RareItemKind].wRareRate);
			pRareOptionInfo->NaeRyuk = (WORD)GetRareItemValue((DWORD)m_RereItemInfo[RareItemKind].NaeRyuk_Min, (DWORD)(m_RereItemInfo[RareItemKind].NaeRyuk * val), m_RereItemInfo[RareItemKind].wRareRate);
			pRareOptionInfo->Shield = GetRareItemValue(m_RereItemInfo[RareItemKind].Shield_Min, m_RereItemInfo[RareItemKind].Shield * val, m_RereItemInfo[RareItemKind].wRareRate);
		}

		pRareOptionInfo->PhyAttack = (WORD)GetRareItemValue((DWORD)m_RereItemInfo[RareItemKind].PhyAttack_Min, (DWORD)(m_RereItemInfo[RareItemKind].PhyAttack * val), m_RereItemInfo[RareItemKind].wRareRate);
		pRareOptionInfo->PhyDefense = (WORD)GetRareItemValue((DWORD)m_RereItemInfo[RareItemKind].PhyDefense_Min, (DWORD)(m_RereItemInfo[RareItemKind].PhyDefense * val), m_RereItemInfo[RareItemKind].wRareRate);

		WORD i = rand() % ATTR_MAX + 1;

		if (rand() % 2)	//∞¯∞› or πÊæÓ
		{
			pRareOptionInfo->AttrAttack.SetElement_Val(i, (float)GetRareItemValue((DWORD)m_RereItemInfo[RareItemKind].AttrAttack_Min.GetElement_Val(i)
				, (DWORD)(m_RereItemInfo[RareItemKind].AttrAttack.GetElement_Val(i) * val), m_RereItemInfo[RareItemKind].wRareRate) / 100.f);
		}
		else
		{
			pRareOptionInfo->AttrRegist.SetElement_Val(i, (float)GetRareItemValue((DWORD)m_RereItemInfo[RareItemKind].AttrRegist_Min.GetElement_Val(i)
				, (DWORD)(m_RereItemInfo[RareItemKind].AttrRegist.GetElement_Val(i) * val), m_RereItemInfo[RareItemKind].wRareRate) / 100.f);
		}
		return TRUE;
	}
	else
		return FALSE;

}

DWORD RarenessManager::GetRareItemValue(DWORD RndMin, DWORD RndMax, WORD wRareRate)
{
	if (!RndMin || !RndMax) return 0; //√÷¥Î √÷º“ ∞™¡ﬂ «œ≥™∂Ûµµ 0 ¿Ã∏È ∏Æ≈œ

	if (RndMin == RndMax) return RndMax;	//∞Ì¡§ ºˆƒ°∑Œ ∫Ÿ¥¬ ø…º«¿∫ Ω∫≈©∏≥∆Æ ªÛø°º≠ √÷º“/√÷¥Î ∞™ ∞∞∞‘ ºº∆√.

	ASSERTMSG(RndMax > RndMin, "Check RareOptionInfo File!");
	//-------------------------------------------------------------
	//◊£∏£◊∞±∏º∏¬ …Ë÷√ 
	//-------------------------------------------------------------
	DWORD RndValue = ((rand() % 10000) * 10000 + (rand() % 10000 + 1));
	DWORD Index = 0;
	while (RndValue < m_dwRareValueProb[Index])
	{
		Index++;
	}
	//!¿”Ω√
	DWORD XRndValue = (DWORD)((RndMax - RndMin) * Index / wRareRate) + RndMin;
	//DWORD XRndValue = (DWORD)((RndMax - RndMin) * Index / m_MaxRareRate) + RndMin;

	if (XRndValue > RndMax)
		XRndValue = (RndMin + rand() % (RndMax - RndMin + 1));

	return XRndValue;
	//return (DWORD)((RndMax - RndMin) * Index / RARE_VALUE_PROB_LIST_MAX ) + RndMin;
}