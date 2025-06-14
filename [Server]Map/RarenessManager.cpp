



// RarenessManager.cpp: implementation of the RarenessManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RarenessManager.h"
#include "MHFile.h"
#include "ItemManager.h"
#include "..\[CC]Header\GameResourceStruct.h"

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
	m_Count = 0;
	m_MaxRareRndRatio = 0;
}

void RarenessManager::Release()
{
	ReleaseRareItemInfo();
}
bool RarenessManager::LoadRareItemOptionInfo()
{
	CMHFile fp;

	char szBuf[256] = {0,};
	char line[512];

	char FileName[256];

#ifdef _FILE_BIN_
//	sprintf(FileName, "Resource/Item_RareOptionInfo.bin");
	sprintf(FileName, "Resource/Server/Item_RareOptionInfo.bin");
	if( !fp.Init( FileName, "rb" ) )
		return FALSE;
#else
	sprintf(FileName, "Reaource/Item_RareOptionInfo.txt");
	if( !fp.Init( FileName, "rt" ) )
		return FALSE;
#endif

	DWORD dwItemKindIdx = 0;
	while( !fp.IsEOF() )
	{
		if(dwItemKindIdx == RareItemKindMAX)
			break;

		fp.GetString(szBuf);

		if( szBuf[0] == '@' )
		{
			fp.GetLineX(line, 512);
			continue;
		}
		
		if( szBuf[0] == '#' )
		{
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
			for(int i = ATTR_FIRE; i <= ATTR_MAX; ++i )
			{
			m_RereItemInfo[dwItemKindIdx].AttrAttack_Min.SetElement_Val( i, fp.GetFloat() );
			m_RereItemInfo[dwItemKindIdx].AttrAttack.SetElement_Val( i, fp.GetFloat() );
			}
			for(int j = ATTR_FIRE; j <= ATTR_MAX; ++j )
			{
			m_RereItemInfo[dwItemKindIdx].AttrRegist_Min.SetElement_Val( j, fp.GetFloat() );
			m_RereItemInfo[dwItemKindIdx].AttrRegist.SetElement_Val( j, fp.GetFloat() );
			}

			dwItemKindIdx++;
		}

	}
	fp.Release();
	return TRUE;
}
/*
bool RarenessManager::LoadRareItemOptionInfo()
{

	CMHFile fp;

	char szBuf[256] = {0,};
	char line[512];

	char FileName[256];

#ifdef _FILE_BIN_
	sprintf(FileName,"Resource/Item_RareOptionInfo.bin");
	if( !fp.Init( FileName, "rb" ) )
		return FALSE;
#else
	sprintf(FileName,"Resource/Item_RareOptionInfo.txt");
	if( !fp.Init( FileName, "rt" ) )
		return FALSE;
#endif		

	while( !fp.IsEOF() )
	{
		fp.GetString(szBuf);

		if( szBuf[0] == '@' )
		{
			fp.GetLineX(line,512);
			continue;
		}
		strupr(szBuf);

		if( strcmp(szBuf, "$WEAPONS") == 0 )
		{
			//fp.GetLineX(line,512);
			for( int i = 0; i < WeaponKindMAX; ++i )
			{
				fp.GetString(szBuf);	//Name

				m_Weapon[i].BaseMin = fp.GetDword();
				m_Weapon[i].BaseMax = fp.GetDword();
				m_Weapon[i].AddMin = fp.GetDword();
				m_Weapon[i].AddMax = fp.GetDword();

			}
		}
		else if( strcmp(szBuf, "$PROTECTORS") == 0 )
		{
			for( int i = 0; i < ProtectorkindMAX; ++i )
			{
				fp.GetString(szBuf);
				m_Protector[i].BaseMin = fp.GetDword();
				m_Protector[i].BaseMax = fp.GetDword();
				m_Protector[i].AttribMin = fp.GetDword();
				m_Protector[i].AttribMax = fp.GetDword();
				m_Protector[i].PlusValue[LIFE] = fp.GetDword();
				m_Protector[i].PlusValue[NAERYUK] = fp.GetDword();
				m_Protector[i].PlusValue[SHIELD] = fp.GetDword();
			}
		}
		else if( strcmp(szBuf, "$ACCESSARIES") == 0 )
		{
			for( int i = 0; i < AccessaryKindMAX; ++i )
			{
				fp.GetString(szBuf);
				m_Accessary[i].BaseMin = fp.GetDword();
				m_Accessary[i].BaseMax = fp.GetDword();
			}
		}
	}

	return TRUE;
}*/

bool RarenessManager::LoadRareItemInfo()
{
	CMHFile fp;

	char szBuf[256] = {0,};
	char line[512];

	char FileName[256];

#ifdef _FILE_BIN_
	sprintf(FileName, "Resource/Item_RareItemInfo.bin");
	if( !fp.Init(FileName,"rb") ) return FALSE;
#else
	sprintf(FileName, "Resource/Item_RareItemInfo.txt");
	if( !fp.Init(FileName,"rt") ) return FALSE;
#endif

	while( !fp.IsEOF() )
	{
		fp.GetString(szBuf);

		if( szBuf[0] == '@' )
		{
			fp.GetLineX(line,512);
			continue;
		}

//		strupr(szBuf);

		if( szBuf[0] == '*' )
		{
			sRareItemInfo* pRareItemInfo = new sRareItemInfo;
			pRareItemInfo->ItemIdx = fp.GetDword();
			pRareItemInfo->RareProb = fp.GetDword();

			ASSERT(!m_RareItemInfoTable.GetData(pRareItemInfo->ItemIdx));
			m_RareItemInfoTable.Add(pRareItemInfo, pRareItemInfo->ItemIdx);
		}
	}
	fp.Release();
	return TRUE;
}

bool RarenessManager::LoadRareValueProbList()
{
	CMHFile fp;

	char FileName[256];
	char szBuf[256] = {0,};

#ifdef _FILE_BIN_
	sprintf(FileName, "Resource/Server/Item_RareStatSetRate.bin");
	if(!fp.Init(FileName,"rb")) return FALSE;
#else
	sprintf(FileName, "Resource/Item_RareStatSetRate.txt");
	if(!fp.Init(FileName,"rt")) return FALSE;
#endif

	fp.GetLine(szBuf,256); //@

	DWORD Index = 1;
	while( !fp.IsEOF() )
	{
		if(Index == RARE_VALUE_PROB_LIST_MAX) break;
		

		Index	= fp.GetDword();	//1~100
		m_dwRareValueProb[Index - 1] = fp.GetDword();
	}
	fp.Release();
	LoadMaxRareRatio();
	return TRUE;
}
void RarenessManager::LoadMaxRareRatio()
{
	CMHFile fp;
#ifdef _FILE_BIN_
	if(!fp.Init("./Resource/Server/Item_MaxRareRndRatio.bin","rb")) return ;
#else
	if(!fp.Init("Resource/Server/Item_MaxRareRndRatio.txt","rt")) return FALSE;
#endif
	m_MaxRareRndRatio = fp.GetDword();
	if(m_MaxRareRndRatio==0) m_MaxRareRndRatio=5000;
	fp.Release();
}
void RarenessManager::ReleaseRareItemInfo()
{
	sRareItemInfo* pList = NULL;
	m_RareItemInfoTable.SetPositionHead();
	while( pList = m_RareItemInfoTable.GetData() )
	{
		delete pList;
		pList = NULL;
	}
	m_RareItemInfoTable.RemoveAll();
}
/*bool RarenessManager::GetRareSpecialStone(DWORD ObtainItemIdx, ITEM_RARE_OPTION_INFO* pRareOptionInfo, CPlayer* pPlayer, BOOL bRare, DWORD StoneIdx )
{
	if (ObtainItemIdx > BOUNDARY_INDEX_FOR_SHOPITEM) return FALSE;
	ITEM_INFO* pInfo = ITEMMGR->GetItemInfo(ObtainItemIdx);
	if (!pInfo) return FALSE;
	eITEM_KINDBIT eItemKind = GetItemKind(ObtainItemIdx);
	if (!(eItemKind & eEQUIP_ITEM)) return FALSE;
	DWORD ItemIdx = ObtainItemIdx;
	if (eItemKind == eEQUIP_ITEM_WEAPON
		|| eItemKind == eEQUIP_ITEM_DRESS
		|| eItemKind == eEQUIP_ITEM_HAT
		|| eItemKind == eEQUIP_ITEM_SHOES)
	{
		ItemIdx /= 10;
		ItemIdx *= 10;
	}
	sRareItemInfo* RareInfo = m_RareItemInfoTable.GetData(ItemIdx);
	if (!RareInfo) return FALSE;
	BOOL baa = FALSE;
	if (FALSE == bRare)
	{
		float addPob = 1.0f;
		pPlayer->GetPetManager()->GetPetBuffResultRt(ePB_Item_RareProbUp, &addPob);
		baa = (((DWORD)((rand() % 10000) * 10000 + (rand() % 10000 + 1)))*addPob <= (RareInfo->RareProb));
	}

	
	if (baa || bRare)
	{
		DWORD RareItemKind = 0;
		if (eItemKind == eEQUIP_ITEM_WEAPON)
		{
			RareItemKind = pInfo->WeaponType - 1;
		}
		else if (eItemKind == eEQUIP_ITEM_DRESS)		RareItemKind = eRI_DRESS;
		else if (eItemKind == eEQUIP_ITEM_HAT)			RareItemKind = eRI_HAT;
		else if (eItemKind == eEQUIP_ITEM_SHOES)		RareItemKind = eRI_SHOES;
		else if (eItemKind == eEQUIP_ITEM_RING)			RareItemKind = eRI_RING;
		else if (eItemKind == eEQUIP_ITEM_CAPE)			RareItemKind = eRI_CAPE;
		else if (eItemKind == eEQUIP_ITEM_NECKLACE)		RareItemKind = eRI_NECKLACE;
		else if (eItemKind == eEQUIP_ITEM_ARMLET)		RareItemKind = eRI_ARMLET;
		else if (eItemKind == eEQUIP_ITEM_BELT)			RareItemKind = eRI_BELT;

		DWORD SpecialStoneKind = -1;
		if (StoneIdx == eSpecialBlessingStoneSTR)
		{
			SpecialStoneKind = 0;
		}
		if (StoneIdx == eSpecialBlessingStoneAGI)
		{
			SpecialStoneKind = 1;
		}
		if (StoneIdx == eSpecialBlessingStoneCON)
		{
			SpecialStoneKind = 2;
		}
		if (StoneIdx == eSpecialBlessingStoneINT)
		{
			SpecialStoneKind = 3;
		}
		if (m_RereItemInfo[RareItemKind].GenGol || m_RereItemInfo[RareItemKind].MinChub || m_RereItemInfo[RareItemKind].CheRyuk || m_RereItemInfo[RareItemKind].SimMek)
		{
			switch (SpecialStoneKind)
			{
			case eRI_GENGOL:
				pRareOptionInfo->GenGol = (WORD)GetRareItemValue((DWORD)m_RereItemInfo[RareItemKind].GenGol_Min, (DWORD)m_RereItemInfo[RareItemKind].GenGol);
				break;
			case eRI_MINCHUB:
				pRareOptionInfo->MinChub = (WORD)GetRareItemValue((DWORD)m_RereItemInfo[RareItemKind].MinChub_Min, (DWORD)m_RereItemInfo[RareItemKind].MinChub);
				break;
			case eRI_CHERYUK:
				pRareOptionInfo->CheRyuk = (WORD)GetRareItemValue((DWORD)m_RereItemInfo[RareItemKind].CheRyuk_Min, (DWORD)m_RereItemInfo[RareItemKind].CheRyuk);
				break;
			case eRI_SIMMEK:
				pRareOptionInfo->SimMek = (WORD)GetRareItemValue((DWORD)m_RereItemInfo[RareItemKind].SimMek_Min, (DWORD)m_RereItemInfo[RareItemKind].SimMek);
				break;
			default:
				ASSERT(0);
				break;
			}
		}
		else
		{
			pRareOptionInfo->GenGol = (WORD)GetRareItemValue((DWORD)m_RereItemInfo[RareItemKind].GenGol_Min, (DWORD)m_RereItemInfo[RareItemKind].GenGol);
			pRareOptionInfo->MinChub = (WORD)GetRareItemValue((DWORD)m_RereItemInfo[RareItemKind].MinChub_Min, (DWORD)m_RereItemInfo[RareItemKind].MinChub);
			pRareOptionInfo->CheRyuk = (WORD)GetRareItemValue((DWORD)m_RereItemInfo[RareItemKind].CheRyuk_Min, (DWORD)m_RereItemInfo[RareItemKind].CheRyuk);
			pRareOptionInfo->SimMek = (WORD)GetRareItemValue((DWORD)m_RereItemInfo[RareItemKind].SimMek_Min, (DWORD)m_RereItemInfo[RareItemKind].SimMek);
		}
		if (eItemKind == eEQUIP_ITEM_DRESS || eItemKind == eEQUIP_ITEM_HAT || eItemKind == eEQUIP_ITEM_SHOES)
		{
			pRareOptionInfo->Life = GetRareItemValue(m_RereItemInfo[RareItemKind].Life_Min, m_RereItemInfo[RareItemKind].Life);
			pRareOptionInfo->NaeRyuk = (WORD)GetRareItemValue((DWORD)m_RereItemInfo[RareItemKind].NaeRyuk_Min, (DWORD)m_RereItemInfo[RareItemKind].NaeRyuk);
			pRareOptionInfo->Shield = GetRareItemValue(m_RereItemInfo[RareItemKind].Shield_Min, m_RereItemInfo[RareItemKind].Shield);
		}
		else if (m_RereItemInfo[RareItemKind].Life && m_RereItemInfo[RareItemKind].NaeRyuk && m_RereItemInfo[RareItemKind].Shield)
		{
			switch (rand() % ValueKindMAX)
			{
			case eRI_LIFE:
				pRareOptionInfo->Life = GetRareItemValue(m_RereItemInfo[RareItemKind].Life_Min, m_RereItemInfo[RareItemKind].Life);
				break;
			case eRI_NAERYUK:
				pRareOptionInfo->NaeRyuk = (WORD)GetRareItemValue((DWORD)m_RereItemInfo[RareItemKind].NaeRyuk_Min, (DWORD)m_RereItemInfo[RareItemKind].NaeRyuk);
				break;
			case eRI_SHIELD:
				pRareOptionInfo->Shield = GetRareItemValue(m_RereItemInfo[RareItemKind].Shield_Min, m_RereItemInfo[RareItemKind].Shield);
				break;
			default:
				ASSERT(0);
				break;
			}
		}
		else
		{
			pRareOptionInfo->Life = GetRareItemValue(m_RereItemInfo[RareItemKind].Life_Min, m_RereItemInfo[RareItemKind].Life);
			pRareOptionInfo->NaeRyuk = (WORD)GetRareItemValue((DWORD)m_RereItemInfo[RareItemKind].NaeRyuk_Min, (DWORD)m_RereItemInfo[RareItemKind].NaeRyuk);
			pRareOptionInfo->Shield = GetRareItemValue(m_RereItemInfo[RareItemKind].Shield_Min, m_RereItemInfo[RareItemKind].Shield);
		}
		pRareOptionInfo->PhyAttack = (WORD)GetRareItemValue((DWORD)m_RereItemInfo[RareItemKind].PhyAttack_Min, (DWORD)m_RereItemInfo[RareItemKind].PhyAttack);
		pRareOptionInfo->PhyDefense = (WORD)GetRareItemValue((DWORD)m_RereItemInfo[RareItemKind].PhyDefense_Min, (DWORD)m_RereItemInfo[RareItemKind].PhyDefense);
		WORD i = rand() % ATTR_MAX + 1;
		if (rand() % 2)
		{
			pRareOptionInfo->AttrAttack.SetElement_Val(i, (float)GetRareItemValue((DWORD)m_RereItemInfo[RareItemKind].AttrAttack_Min.GetElement_Val(i)
				, (DWORD)(m_RereItemInfo[RareItemKind].AttrAttack.GetElement_Val(i))) / 100.f);
		}
		else
		{
			pRareOptionInfo->AttrRegist.SetElement_Val(i, (float)GetRareItemValue((DWORD)m_RereItemInfo[RareItemKind].AttrRegist_Min.GetElement_Val(i)
				, (DWORD)(m_RereItemInfo[RareItemKind].AttrRegist.GetElement_Val(i))) / 100.f);
		}
		return TRUE;
	}
	else
		return FALSE;
}*/
bool RarenessManager::GetRare(DWORD ObtainItemIdx, ITEM_RARE_OPTION_INFO* pRareOptionInfo, CPlayer* pPlayer, BOOL bRare,DWORD StoneIdx)
{
	if (ObtainItemIdx > BOUNDARY_INDEX_FOR_SHOPITEM) return FALSE;
	ITEM_INFO* pInfo = ITEMMGR->GetItemInfo(ObtainItemIdx);
	if (!pInfo) return FALSE;
	eITEM_KINDBIT eItemKind = GetItemKind(ObtainItemIdx);
	if (!(eItemKind & eEQUIP_ITEM)) return FALSE;
	DWORD ItemIdx = ObtainItemIdx;
	if (eItemKind == eEQUIP_ITEM_WEAPON
		|| eItemKind == eEQUIP_ITEM_DRESS
		|| eItemKind == eEQUIP_ITEM_HAT
		|| eItemKind == eEQUIP_ITEM_SHOES)
	{
	//	ItemIdx /= 10;
		//ItemIdx *= 10;
	}
	sRareItemInfo* RareInfo = m_RareItemInfoTable.GetData(ItemIdx);
	if (!RareInfo) return FALSE;
	BOOL baa = FALSE;
	if (FALSE == bRare)
	{
		float addPob = 1.0f;
		pPlayer->GetPetManager()->GetPetBuffResultRt(ePB_Item_RareProbUp, &addPob);
		baa = (((DWORD)((rand() % 10000) * 10000 + (rand() % 10000 + 1)))*addPob <= (RareInfo->RareProb));
	}
	if (baa || bRare)
	{
		DWORD RareItemKind = 0;
		if (eItemKind == eEQUIP_ITEM_WEAPON)
		{
			RareItemKind = pInfo->WeaponType - 1;
		}
		else if (eItemKind == eEQUIP_ITEM_DRESS)			RareItemKind = eRI_DRESS;
		else if (eItemKind == eEQUIP_ITEM_HAT)				RareItemKind = eRI_HAT;
		else if (eItemKind == eEQUIP_ITEM_SHOES)			RareItemKind = eRI_SHOES;
		else if (eItemKind == eEQUIP_ITEM_RING)			RareItemKind = eRI_RING;
		else if (eItemKind == eEQUIP_ITEM_CAPE)			RareItemKind = eRI_CAPE;
		else if (eItemKind == eEQUIP_ITEM_NECKLACE)		RareItemKind = eRI_NECKLACE;
		else if (eItemKind == eEQUIP_ITEM_ARMLET)			RareItemKind = eRI_ARMLET;
		else if (eItemKind == eEQUIP_ITEM_BELT)			RareItemKind = eRI_BELT;
		if (m_RereItemInfo[RareItemKind].GenGol || m_RereItemInfo[RareItemKind].MinChub || m_RereItemInfo[RareItemKind].CheRyuk || m_RereItemInfo[RareItemKind].SimMek)
		{
			if (StoneIdx == eSpecialBlessingStoneSTR ||
				StoneIdx == eSpecialBlessingStoneAGI ||
				StoneIdx == eSpecialBlessingStoneINT ||
				StoneIdx == eSpecialBlessingStoneCON)
			{
				DWORD SpecialStoneKind = -1;
				if (StoneIdx == eSpecialBlessingStoneSTR)
				{
					SpecialStoneKind = 0;
				}
				if (StoneIdx == eSpecialBlessingStoneAGI)
				{
					SpecialStoneKind = 1;
				}
				if (StoneIdx == eSpecialBlessingStoneCON)
				{
					SpecialStoneKind = 2;
				}
				if (StoneIdx == eSpecialBlessingStoneINT)
				{
					SpecialStoneKind = 3;
				}
				switch (SpecialStoneKind)
				{
				case eRI_GENGOL:
					pRareOptionInfo->GenGol = (WORD)GetRareItemValue((DWORD)m_RereItemInfo[RareItemKind].GenGol_Min, (DWORD)m_RereItemInfo[RareItemKind].GenGol);
					break;
				case eRI_MINCHUB:
					pRareOptionInfo->MinChub = (WORD)GetRareItemValue((DWORD)m_RereItemInfo[RareItemKind].MinChub_Min, (DWORD)m_RereItemInfo[RareItemKind].MinChub);
					break;
				case eRI_CHERYUK:
					pRareOptionInfo->CheRyuk = (WORD)GetRareItemValue((DWORD)m_RereItemInfo[RareItemKind].CheRyuk_Min, (DWORD)m_RereItemInfo[RareItemKind].CheRyuk);
					break;
				case eRI_SIMMEK:
					pRareOptionInfo->SimMek = (WORD)GetRareItemValue((DWORD)m_RereItemInfo[RareItemKind].SimMek_Min, (DWORD)m_RereItemInfo[RareItemKind].SimMek);
					break;
				default:
					ASSERT(0);
					break;
				}
			}
			else
			{
				switch (rand() % StatesKindMAX)
				{
				case eRI_GENGOL:
					pRareOptionInfo->GenGol = (WORD)GetRareItemValue((DWORD)m_RereItemInfo[RareItemKind].GenGol_Min, (DWORD)m_RereItemInfo[RareItemKind].GenGol);
					break;
				case eRI_MINCHUB:
					pRareOptionInfo->MinChub = (WORD)GetRareItemValue((DWORD)m_RereItemInfo[RareItemKind].MinChub_Min, (DWORD)m_RereItemInfo[RareItemKind].MinChub);
					break;
				case eRI_CHERYUK:
					pRareOptionInfo->CheRyuk = (WORD)GetRareItemValue((DWORD)m_RereItemInfo[RareItemKind].CheRyuk_Min, (DWORD)m_RereItemInfo[RareItemKind].CheRyuk);
					break;
				case eRI_SIMMEK:
					pRareOptionInfo->SimMek = (WORD)GetRareItemValue((DWORD)m_RereItemInfo[RareItemKind].SimMek_Min, (DWORD)m_RereItemInfo[RareItemKind].SimMek);
					break;
				default:
					ASSERT(0);
					break;
				}
			}


			
		}
		else
		{
			pRareOptionInfo->GenGol = (WORD)GetRareItemValue((DWORD)m_RereItemInfo[RareItemKind].GenGol_Min, (DWORD)m_RereItemInfo[RareItemKind].GenGol);
			pRareOptionInfo->MinChub = (WORD)GetRareItemValue((DWORD)m_RereItemInfo[RareItemKind].MinChub_Min, (DWORD)m_RereItemInfo[RareItemKind].MinChub);
			pRareOptionInfo->CheRyuk = (WORD)GetRareItemValue((DWORD)m_RereItemInfo[RareItemKind].CheRyuk_Min, (DWORD)m_RereItemInfo[RareItemKind].CheRyuk);
			pRareOptionInfo->SimMek = (WORD)GetRareItemValue((DWORD)m_RereItemInfo[RareItemKind].SimMek_Min, (DWORD)m_RereItemInfo[RareItemKind].SimMek);
		}
		if (eItemKind == eEQUIP_ITEM_DRESS || eItemKind == eEQUIP_ITEM_HAT || eItemKind == eEQUIP_ITEM_SHOES)
		{
			pRareOptionInfo->Life = GetRareItemValue(m_RereItemInfo[RareItemKind].Life_Min, m_RereItemInfo[RareItemKind].Life);
			pRareOptionInfo->NaeRyuk = (WORD)GetRareItemValue((DWORD)m_RereItemInfo[RareItemKind].NaeRyuk_Min, (DWORD)m_RereItemInfo[RareItemKind].NaeRyuk);
			pRareOptionInfo->Shield = GetRareItemValue(m_RereItemInfo[RareItemKind].Shield_Min, m_RereItemInfo[RareItemKind].Shield);
		}
		else if (m_RereItemInfo[RareItemKind].Life && m_RereItemInfo[RareItemKind].NaeRyuk && m_RereItemInfo[RareItemKind].Shield)
		{
			switch (rand() % ValueKindMAX)
			{
			case eRI_LIFE:
				pRareOptionInfo->Life = GetRareItemValue(m_RereItemInfo[RareItemKind].Life_Min, m_RereItemInfo[RareItemKind].Life);
				break;
			case eRI_NAERYUK:
				pRareOptionInfo->NaeRyuk = (WORD)GetRareItemValue((DWORD)m_RereItemInfo[RareItemKind].NaeRyuk_Min, (DWORD)m_RereItemInfo[RareItemKind].NaeRyuk);
				break;
			case eRI_SHIELD:
				pRareOptionInfo->Shield = GetRareItemValue(m_RereItemInfo[RareItemKind].Shield_Min, m_RereItemInfo[RareItemKind].Shield);
				break;
			default:
				ASSERT(0);
				break;
			}
		}
		else
		{
			pRareOptionInfo->Life = GetRareItemValue(m_RereItemInfo[RareItemKind].Life_Min, m_RereItemInfo[RareItemKind].Life);
			pRareOptionInfo->NaeRyuk = (WORD)GetRareItemValue((DWORD)m_RereItemInfo[RareItemKind].NaeRyuk_Min, (DWORD)m_RereItemInfo[RareItemKind].NaeRyuk);
			pRareOptionInfo->Shield = GetRareItemValue(m_RereItemInfo[RareItemKind].Shield_Min, m_RereItemInfo[RareItemKind].Shield);
		}
		pRareOptionInfo->PhyAttack = (WORD)GetRareItemValue((DWORD)m_RereItemInfo[RareItemKind].PhyAttack_Min, (DWORD)m_RereItemInfo[RareItemKind].PhyAttack);
		pRareOptionInfo->PhyDefense = (WORD)GetRareItemValue((DWORD)m_RereItemInfo[RareItemKind].PhyDefense_Min, (DWORD)m_RereItemInfo[RareItemKind].PhyDefense);
		WORD i = rand() % ATTR_MAX + 1;
		if (rand() % 2)
		{
			pRareOptionInfo->AttrAttack.SetElement_Val(i, (float)GetRareItemValue((DWORD)m_RereItemInfo[RareItemKind].AttrAttack_Min.GetElement_Val(i)
				, (DWORD)(m_RereItemInfo[RareItemKind].AttrAttack.GetElement_Val(i))) / 100.f);
		}
		else
		{
			pRareOptionInfo->AttrRegist.SetElement_Val(i, (float)GetRareItemValue((DWORD)m_RereItemInfo[RareItemKind].AttrRegist_Min.GetElement_Val(i)
				, (DWORD)(m_RereItemInfo[RareItemKind].AttrRegist.GetElement_Val(i))) / 100.f);
		}
		return TRUE;
	}
	else
		return FALSE;
}

DWORD RarenessManager::GetRareItemValue(DWORD RndMin, DWORD RndMax)
{
	if( !RndMin || !RndMax ) return 0; //최대 최소 값중 하나라도 0 이면 리턴

	if( RndMin == RndMax ) return RndMax;	//고정 수치로 붙는 옵션은 스크립트 상에서 최소/최대 값 같게 세팅.

	if(m_Count>=m_MaxRareRndRatio)
	{
		m_Count = 0;
		return RndMax;
	}

	ASSERTMSG(RndMax>RndMin, "Check RareOptionInfo File!");

	DWORD RndValue = ((rand()%10000) * 10000 + (rand()%10000 + 1));

	DWORD Index = 0;
	while(RndValue < m_dwRareValueProb[Index])
	{
		Index++;
	}
	DWORD XRndValue = (DWORD)((RndMax - RndMin) * Index / RARE_VALUE_PROB_LIST_MAX ) + RndMin;
	if(XRndValue == RndMax)
		m_Count = 0;
	else
		m_Count +=1;
	return XRndValue;
}


