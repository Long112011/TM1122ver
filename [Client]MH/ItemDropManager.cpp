#include "StdAfx.h"
#include "ItemDropManager.h"
#include "mhFile.h"
#include "time.h"
#include "..\[CC]Header\GameResourceManager.h"
#include "chatmanager.h"
#include "ItemDropInfoDialog.h"
#include "gamein.h"
GLOBALTON(ItemDropManager)
ItemDropManager::ItemDropManager()
{
	m_MonsterItemDropTable.Initialize(700);//500
	m_MonsterItemDropTemp.Initialize(60);//20*3
}
ItemDropManager::~ItemDropManager()
{
	ClearMonsterDropItemList();
	ClearMonsterDropItemTempList();
}
void ItemDropManager::Init()
{
	srand((unsigned)time(NULL));
	LoadMonsterDropItemList();
}
void ItemDropManager::ClearMonsterDropItemList()
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
BOOL ItemDropManager::LoadMonsterDropItemList()
{
	ClearMonsterDropItemList();
	char filename[64];
	char MapName[MAX_MONSTER_NAME_LENGTH + 1];
	char MonsterName[MAX_MONSTER_NAME_LENGTH + 1];
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
		SafeStrCpy(MapName, file.GetString(), MAX_MONSTER_NAME_LENGTH + 1);//地图
		SafeStrCpy(MonsterName, file.GetString(), MAX_MONSTER_NAME_LENGTH + 1);//怪物
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
	//MessageBox(NULL, "loaded", "info!", MB_OK);
	file.Release();
	return TRUE;
}
MONSTER_DROP_ITEM* ItemDropManager::GetMonsterItemDrop(WORD wItemDropIdx)
{
	return m_MonsterItemDropTable.GetData(wItemDropIdx);
}






BOOL ItemDropManager::ReloadMonsterDropItem(MONSTER_DROP_ITEM* pDropItem)
{
	WORD HaveItem = pDropItem->wTotalNum;

	pDropItem->dwCurTotalRate = 0;
	for (WORD i = 0; i < HaveItem; ++i)
	{
		pDropItem->dwCurMonsterDropItemPercent[i] = pDropItem->MonsterDropItemPercent[i].dwDropPercent;
		pDropItem->dwCurTotalRate += pDropItem->dwCurMonsterDropItemPercent[i];
	}
	if (pDropItem->dwCurTotalRate == 0)
		return FALSE;

	return TRUE;
}

void ItemDropManager::DevMonsterDrop(DWORD idx)
{
	//g_Console.LOG(4, "");

	if (m_dwMobKind != idx && ITEMDROPINFODLG->IsActive())
	{
		//if (ITEMDROPINFODLG->IsActive())
		{
			return;
		}
		//goto m_Open;
	}
	else
	{
		ClearMonsterDropItemTempList();
		ClearTemp();

		MONSTEREX_LIST * pMonInfo = (MONSTEREX_LIST*)GAMERESRCMNGR->GetMonsterListInfo(idx);
		int temp1 = pMonInfo->ItemListNum1;
		int temp2 = pMonInfo->ItemListNum2;
		int temp3 = pMonInfo->ItemListNum3;

		ITEMDROPINFODLG->SetMonsterName(pMonInfo->Name);

		WORD itemdroptemp[3];

		itemdroptemp[0] = temp1;
		itemdroptemp[1] = temp2;
		itemdroptemp[2] = temp3;
		//char Column[16];
		for (int d = 0; d < 3; d++)
		{
			if (itemdroptemp[d] == 0)
				continue;

			MONSTER_DROP_ITEM* pDropItem = GetMonsterItemDrop(itemdroptemp[d]);
			if (pDropItem == NULL)
				continue;  // 不中断整个流程，只跳过当前

			if (pDropItem->dwCurTotalRate == 0)
			{
				if (ReloadMonsterDropItem(pDropItem) == FALSE)
					continue;  // 同上
			}

			WORD CurTotalRate = pDropItem->dwCurTotalRate;
			WORD TotalNum = pDropItem->wTotalNum;

			for (WORD i = 0; i < TotalNum; ++i)
			{
				WORD DropPercent = pDropItem->dwCurMonsterDropItemPercent[i];
				WORD DropItemId = pDropItem->MonsterDropItemPercent[i].wItemIdx;
				if (DropItemId != 0)
				{
					m_dwAfterCalTotalRate[d] = CurTotalRate;
					AddItemToTempList(GetIndex(), DropItemId, d, DropPercent);
				}
			}
		}
		ITEMDROPINFODLG->AddItemToGridEasy();
	}
	

//m_Open:
	
	//ITEMDROPINFODLG->AddItemToGridMedium();
	//ITEMDROPINFODLG->AddItemToGridHard();
	//CHATMGR->AddMsg(CTC_SYSMSG, "");
}










void ItemDropManager::ClearMonsterDropItemTempList()
{
	// reset data
	m_MonsterItemDropTemp.SetPositionHead();
	MONSTER_DROP_TEMP* pDropItemTemp;
	while (pDropItemTemp = m_MonsterItemDropTemp.GetData())
	{
		delete pDropItemTemp;
	}
	m_MonsterItemDropTemp.RemoveAll();
}
void ItemDropManager::AddItemToTempList(DWORD idx,DWORD itemidx, DWORD hardrate, DWORD percent)
{
	MONSTER_DROP_TEMP* pInfo = m_MonsterItemDropTemp.GetData(idx);
	if (!pInfo)
	{
		pInfo = new MONSTER_DROP_TEMP;
		pInfo->wIndex	= idx;
		pInfo->wItemIdx = itemidx;
		pInfo->wRate	= hardrate;
		pInfo->wPercent = percent;
		m_MonsterItemDropTemp.Add(pInfo, pInfo->wIndex);
	}
}
DWORD ItemDropManager::GetIndex()
{
	if (m_dwStartIndex<60)
	{
		DWORD Temp = m_dwStartIndex;
		m_dwStartIndex++;
		return Temp;
	}
	else
	{
		m_dwStartIndex = 1;
		return m_dwStartIndex;
	}
}
void ItemDropManager::GetStoredIdx()
{
	for (int i = 1; i<60; i++)
	{
		MONSTER_DROP_TEMP* pInfo = m_MonsterItemDropTemp.GetData(i);
		if (pInfo)	
		{
			
			CHATMGR->AddMsg(CTC_SYSMSG, "wIndex:%d wItemIdx:%d wRate:%d wPercent:%d"
				, pInfo->wIndex
				, pInfo->wItemIdx
				, pInfo->wRate
				, pInfo->wPercent);
		}
	}
}
void ItemDropManager::ClearTemp()
{
	ITEMDROPINFODLG->ClearIconGrid();
	m_dwStartIndex  = 1;
	for (int i = 1; i < 60; i++)
	{
		m_MonsterItemDropTemp.Remove(i);
	}	
}

MONSTER_DROP_TEMP* ItemDropManager::GetItemTempInfo(WORD Idx)
{
	return  m_MonsterItemDropTemp.GetData(Idx);
}
int ItemDropManager::GetItemTempNum()
{
	return  m_MonsterItemDropTemp.GetDataNum();
}