



// ItemDrop.cpp: implementation of the CItemDrop class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ItemDrop.h"
#include "Player.h"
#include "time.h"
#include "ItemManager.h"
#include "Monster.h"
#include "..\[CC]Header\GameResourceManager.h"
#include "MHFile.h"
#include "PartyManager.h"
#include "Distribute_Random.h"
#include "Party.h"
#include "ChannelSystem.h"

//////////////////////////////////////////////////////////////////////

// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CItemDrop::CItemDrop()
{
	m_MonsterItemDropTable.Initialize(700);//500
}

CItemDrop::~CItemDrop()
{
	ClearMonsterDropItemList();
}

void CItemDrop::Init()
{
	srand((unsigned)time(NULL));
	LoadMonsterDropItemList();
}

int CItemDrop::DropItemKind(MONSTEREX_LIST * pMonsterExList, CPlayer* pPlayer)
{
	DWORD CurTotalRate = 0;
	DWORD DropRate[eDROPITEMKIND_MAX];

	int i=0;
	for(i=0; i<eDROPITEMKIND_MAX; i++)
	{
		DropRate[i] = pMonsterExList->CurDropRate[i];

		if(i == eMONEY)
		{
			//DropRate[eMONEY] *= gMoneyRate;
			DropRate[eMONEY] = (DWORD)(DropRate[eMONEY]*gEventRate[eEvent_MoneyRate]);
		}
		else if(i >= eITEM1 && i <= eITEM3)
		{
			// DropRate[i] *= gItemRate;
			DropRate[i] = (DWORD)(DropRate[i]*gEventRate[eEvent_ItemRate]);

			// 
			if( pPlayer->GetPartyIdx() )
			{
				CParty* pParty = PARTYMGR->GetParty( pPlayer->GetPartyIdx() );
				if( pParty )
				{
					int count = pParty->GetMemberCountofMap( pPlayer->GetID() );
					if( count && gPartyEvent[ePartyEvent_ItemRate].Rate[count-1] )
						DropRate[i] = (DWORD)(DropRate[i]*gPartyEvent[ePartyEvent_ItemRate].Rate[count-1]);
				}
			}
		}

		CurTotalRate += DropRate[i];	
	}

	// RaMa - 06.02.08 ->사냥꾼의부적
	if( pPlayer->GetShopItemStats()->AddItemDrop )
	{
		for(i=eITEM1; i<=eITEM3; ++i)
		{
			CurTotalRate -= DropRate[i];
			DropRate[i] *= (DWORD)(1+(pPlayer->GetShopItemStats()->AddItemDrop*0.01f) + 0.001f);
			CurTotalRate += DropRate[i];
		}
	}

	//SW080515 채널별 세팅
	float fDropRate = CHANNELSYSTEM->GetDropRate( pPlayer->GetChannelID());
	if( fDropRate != 1.0f )

	{
		for(i=eITEM1; i<= eITEM3; ++i)
		{
			CurTotalRate -= DropRate[i];
			DropRate[i] = DWORD(fDropRate * DropRate[i]);
			CurTotalRate += DropRate[i];
		}
	}

	if( CurTotalRate > MAX_DROP_ITEM_PERCENT )
	{
		int dwTemp = DropRate[eMONEY]+DropRate[eITEM1]+DropRate[eITEM2]+DropRate[eITEM3];

		if( dwTemp < MAX_DROP_ITEM_PERCENT )
		{
			DropRate[eNOITEM] = MAX_DROP_ITEM_PERCENT - dwTemp;
			CurTotalRate = MAX_DROP_ITEM_PERCENT;
		}
		else
		{
			DropRate[eNOITEM] = 0;
			CurTotalRate = dwTemp;
		}
	}

	//CurTotalRate == 0이면 안된다.
	if( CurTotalRate == 0 )
	{
//		char buf[128];

//		wsprintf( buf, "MonsterIdx:%d", pMonsterExList->MonsterKind );
//		ASSERTMSG(0, buf);
		ASSERT(0);
		return -1;
	}

	DWORD RandRate = random(1, CurTotalRate);
	DWORD CurMinRate=0,CurMaxRate=0;

	for( i=0; i<eDROPITEMKIND_MAX; i++)
	{
		CurMinRate = CurMaxRate;
		CurMaxRate += DropRate[i];

		if(RandRate > CurMinRate && RandRate <= CurMaxRate)
		{
			return i;
		}
	}
	//ASSERT(0);

	//char buf[128];
	//wsprintf( buf, "MonsterIdx:%d, CurTotalRate:%d, RandRate:%d, CurMinRate:%d, CurMaxRate:%d,",
	//	pMonsterExList->MonsterKind, CurTotalRate, RandRate, CurMinRate, CurMaxRate );
	//ASSERTMSG(0, buf);

	return -1;

}
MONEYTYPE CItemDrop::MoneyItemNoItemPercentCalculator(MONSTEREX_LIST * pMonInfo, CPlayer* pPlayer, WORD MonsterKind)
{

//	ASSERT(pMob->GetObjectKind() & eObjectKind_Monster );
	
	// RaMa - 04.11.24   ->ShopItemOption의 ItemDrop비율 추가

	MONEYTYPE TotalMoney = 0; 

	for( int i = 0 ; i < pMonInfo->MaxDropItemNum ; ++i )
	{
		int eDropItemKind = DropItemKind(pMonInfo, pPlayer);

		switch(eDropItemKind)
		{
		case eNOITEM:
			{

			}
			break;
		case eMONEY:
			{
				//if( g_pServerSystem->GetMapNum() > TITANMAP_START_INDEX )
				if( g_pServerSystem->GetMap()->IsMapKind(eTitanMap) )
				{
					MONSTERDROP_TIMAP* pMonsterDropInfo = GAMERESRCMNGR->GetTitanMapMonsterDropInfo( pMonInfo->Level );
					if( pMonsterDropInfo )
						TotalMoney = random( pMonsterDropInfo->stDropInfo[0].wMoneyMin, pMonsterDropInfo->stDropInfo[0].wMoneyMax );
				}
				else
					TotalMoney += MonsterDropMoney(pMonInfo);
			}
			break;
		case eITEM1:
			{
				MonsterDropItem(MonsterKind, pMonInfo->ItemListNum1, pPlayer);
			}
			break;
		case eITEM2:
			{
				MonsterDropItem(MonsterKind, pMonInfo->ItemListNum2, pPlayer);
			}
			break;
		case eITEM3:
			{
				MonsterDropItem(MonsterKind, pMonInfo->ItemListNum3, pPlayer);
			}
			break;
		default:
			{
				ASSERT(0);
			}
		}
	}

//	return 0;
	return TotalMoney;
}

MONEYTYPE CItemDrop::MonsterDropMoney(MONSTEREX_LIST * pMonsterExList)
{
	MONEYTYPE MinMoney = pMonsterExList->MinDropMoney;
	MONEYTYPE MaxMoney = pMonsterExList->MaxDropMoney;
//	ASSERT(MaxMoney >= MinMoney);

	MONEYTYPE ResultMoney = 0;
/*
	if(MinMoney == MaxMoney)
	{
		ResultMoney = MinMoney;
	}
	else
	{

		ResultMoney = (rand() % (MaxMoney - MinMoney)) + MinMoney;
	}
*/	
	if( MaxMoney > MinMoney )
	{
		ResultMoney = random( MinMoney, MaxMoney );
	}
	else
	{
		ResultMoney = MinMoney;
	}

	// RaMa -04.11.24   ->돈 얻는 액수
	// ResultMoney *= gGetMoney;
	ResultMoney = (MONEYTYPE)(ResultMoney*gEventRate[eEvent_GetMoney]);

	return ResultMoney;
}
void CItemDrop::DevMonsterDrop(DWORD idx)
{
	g_Console.LOG(4, "");

	MONSTEREX_LIST * pMonInfo = (MONSTEREX_LIST*)GAMERESRCMNGR->GetMonsterListInfo(idx);
	int temp1 = pMonInfo->ItemListNum1;
	int temp2 = pMonInfo->ItemListNum2;
	int temp3 = pMonInfo->ItemListNum3;

	g_Console.LOC(0x757575,4, "[ %d ]  [ BD:%d ] [ BE:%d ] [ BF:%d ]   %s "
					 , idx					 
					 , temp1
					 , temp2
					 , temp3
					 , pMonInfo->Name);


	WORD itemdroptemp[3];

	itemdroptemp[0] = temp1;
	itemdroptemp[1] = temp2;
	itemdroptemp[2] = temp3;
	char Column[16];
	for (int d = 0; d < 3; d++)
	{
		if (itemdroptemp[d] == 0)
			continue;

		MONSTER_DROP_ITEM * pDropItem = GetMonsterItemDrop(itemdroptemp[d]);
		if (pDropItem == NULL)
			return;

		if (pDropItem->dwCurTotalRate == 0)
		{
			if (ReloadMonsterDropItem(pDropItem) == FALSE)
				return;
		}


		WORD CurTotalRate = pDropItem->dwCurTotalRate;
		WORD TotalNum = pDropItem->wTotalNum;


		if (d == 0)
			strcpy(Column, "BD");
		if (d == 1)
			strcpy(Column, "BE");
		if (d == 2)
			strcpy(Column, "BF");



		g_Console.LOC(0x424242, 4, "[ %s: %d  rate: %d%%  count: %d ]"
																		 , Column
																		 , itemdroptemp[d]
																		 , CurTotalRate
																		 , TotalNum);

		for (WORD i = 0; i < pDropItem->wTotalNum; ++i)
		{
			WORD DropPercent = pDropItem->dwCurMonsterDropItemPercent[i];
			WORD DropItemId = pDropItem->MonsterDropItemPercent[i].wItemIdx;

			g_Console.LOC(0x9E9E9E, 4, "[ ItemIdx: %d ] %d%%", DropItemId, DropPercent);
		}
	}

	g_Console.LOG(4, "");
}
void CItemDrop::MonsterDropItem(WORD MonsterKind, WORD wDropItemIdx, CPlayer* pPlayer)
{
	MONSTER_DROP_ITEM * pDropItem = GetMonsterItemDrop(wDropItemIdx);
	ASSERT(pDropItem);
	if(pDropItem == NULL)
		return;
	
	if( pDropItem->dwCurTotalRate == 0 )
	{
		if(ReloadMonsterDropItem(pDropItem) == FALSE)
			return;
	}

	DWORD RandRate = random( 1, pDropItem->dwCurTotalRate );
	
	DWORD FromPercent	= 0;
	DWORD ToPercent		= 0;

	for(WORD i = 0 ; i < pDropItem->wTotalNum ; ++i)
	{
		FromPercent = ToPercent;
		ToPercent += pDropItem->dwCurMonsterDropItemPercent[i];
		if(FromPercent < RandRate && RandRate <= ToPercent)
		{
			--pDropItem->dwCurMonsterDropItemPercent[i];
			--pDropItem->dwCurTotalRate;

			if(pDropItem->MonsterDropItemPercent[i].wItemIdx)
			{
				WORD DropItemId = pDropItem->MonsterDropItemPercent[i].wItemIdx;

					DISTRIBUTERAND->ItemChangeAtLv(DropItemId);

				ITEMMGR->MonsterObtainItem(pPlayer, DropItemId, MonsterKind);
			}
			break;
		}
	}
}


BOOL CItemDrop::ReloadMonsterDropItem(MONSTER_DROP_ITEM* pDropItem)
{
	WORD HaveItem = pDropItem->wTotalNum;

	pDropItem->dwCurTotalRate = 0;
	for(WORD i = 0; i < HaveItem; ++i)
	{
		pDropItem->dwCurMonsterDropItemPercent[i] = pDropItem->MonsterDropItemPercent[i].dwDropPercent;
		pDropItem->dwCurTotalRate += pDropItem->dwCurMonsterDropItemPercent[i];
	}
	if(pDropItem->dwCurTotalRate == 0)
		return FALSE;

	return TRUE;
}


void CItemDrop::ClearMonsterDropItemList()
{
	// reset data
	m_MonsterItemDropTable.SetPositionHead();
	MONSTER_DROP_ITEM* pDropItem;
	while(pDropItem = m_MonsterItemDropTable.GetData())
	{
		delete pDropItem;
	}
	m_MonsterItemDropTable.RemoveAll();
}

BOOL CItemDrop::LoadMonsterDropItemList()
{
	ClearMonsterDropItemList();

	char filename[64];
	CMHFile file;
	sprintf(filename, DROPINFOPATH);
	if(!file.Init(filename, "rb"))
		return FALSE;


	while(1)
	{
		if(file.IsEOF())
			break;
		
		MONSTER_DROP_ITEM* pDropItem = new MONSTER_DROP_ITEM;
		memset(pDropItem, 0, sizeof(MONSTER_DROP_ITEM));
		pDropItem->wItemDropIndex = file.GetWord();

		WORD totalnum = 0;
		for(int idx=0; idx<MAX_DROPITEM_NUM; ++idx)
		{
			char szTempName[256];
			file.GetString(szTempName);
			pDropItem->MonsterDropItemPercent[idx].wItemIdx = file.GetWord();		
			pDropItem->MonsterDropItemPercent[idx].dwDropPercent = file.GetDword();
			if(pDropItem->MonsterDropItemPercent[idx].dwDropPercent)
				++totalnum;
		}
		
		pDropItem->wTotalNum = totalnum;
		pDropItem->dwCurTotalRate = 0;

		ASSERT(!m_MonsterItemDropTable.GetData(pDropItem->wItemDropIndex));
		m_MonsterItemDropTable.Add(pDropItem,pDropItem->wItemDropIndex);

		ReloadMonsterDropItem(pDropItem);
	}	
	
	file.Release();
	
	return TRUE;
}

MONSTER_DROP_ITEM* CItemDrop::GetMonsterItemDrop(WORD wItemDropIdx)
{
	return m_MonsterItemDropTable.GetData(wItemDropIdx);
}