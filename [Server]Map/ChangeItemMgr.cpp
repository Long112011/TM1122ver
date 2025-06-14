#include "stdafx.h"
#include "ChangeItemMgr.h"
#include "MHFile.h"
#include "../[CC]Header/GameResourceStruct.h"
#include "ItemManager.h"
#include "Player.h"
#include "ObjectStateManager.h"
#include "MapDBMsgParser.h"
#include "CheckRoutine.h"
CChangeItemMgr::CChangeItemMgr()
{
}
CChangeItemMgr::~CChangeItemMgr()
{
	Release();
}
void CChangeItemMgr::Init()
{
	m_ChangeItemList.Initialize(300);
	m_MultiChangeItemList.Initialize(100);
	LoadItemChange();
}
void CChangeItemMgr::Release()
{
	sCHANGEITEM* pData = NULL;
	m_ChangeItemList.SetPositionHead();
	while (pData = m_ChangeItemList.GetData())
	{
		delete pData;
	}
	m_ChangeItemList.RemoveAll();
	sMULTICHANGEITEM* pData1 = NULL;
	m_MultiChangeItemList.SetPositionHead();
	while (pData1 = m_MultiChangeItemList.GetData())
	{
		delete[] pData1->pChangeItem;
		delete pData1;
	}
	m_MultiChangeItemList.RemoveAll();
}
BOOL CChangeItemMgr::LoadItemChange()
{
	CMHFile file;
	if (!file.Init("./Resource/Server/ItemChangeList.bin", "rb"))
		return FALSE;
	while (1)
	{
		if (file.IsEOF())
			break;
		sCHANGEITEM* pData = new sCHANGEITEM;
		pData->wItemIdx = file.GetWord();
		pData->wMaxToItem = file.GetWord();
		pData->pItemUnit = new sCHANGEITEMUNIT[pData->wMaxToItem];
		for (WORD i = 0; i < pData->wMaxToItem; ++i)
		{
			pData->pItemUnit[i].wToItemIdx = file.GetWord();
			pData->pItemUnit[i].dwToItemDur = file.GetDword();
			pData->pItemUnit[i].dwPercent = file.GetDword();
		}
		m_ChangeItemList.Add(pData, pData->wItemIdx);
	}
	file.Release();
	if (!file.Init("./Resource/Server/ItemChangeListMulti.bin", "rb"))
		return FALSE;
	while (1)
	{
		if (file.IsEOF())
			break;
		sMULTICHANGEITEM* pData = new sMULTICHANGEITEM;
		pData->wItemIdx = file.GetWord();
		pData->wLimitLevel = file.GetWord();
		pData->wMaxSet = file.GetWord();
		pData->pChangeItem = new sCHANGEITEM[pData->wMaxSet];
		for (WORD i = 0; i < pData->wMaxSet; ++i)
		{
			pData->pChangeItem[i].wItemIdx = pData->wItemIdx;
			pData->pChangeItem[i].wMaxToItem = file.GetWord();
			pData->pChangeItem[i].pItemUnit = new sCHANGEITEMUNIT[pData->pChangeItem[i].wMaxToItem];
			for (WORD j = 0; j < pData->pChangeItem[i].wMaxToItem; ++j)
			{
				pData->pChangeItem[i].pItemUnit[j].wToItemIdx = file.GetWord();
				pData->pChangeItem[i].pItemUnit[j].dwToItemDur = file.GetDword();
				pData->pChangeItem[i].pItemUnit[j].dwPercent = file.GetDword();
			}
		}
		pData->nMaxItemSpace = ChangedTotalItemNum(pData, pData->wMaxSet);
		m_MultiChangeItemList.Add(pData, pData->wItemIdx);
	}
	file.Release();
	return TRUE;
}
int CChangeItemMgr::UseChangeItem(CPlayer* pPlayer, WORD TargetPos, WORD wItemIdx)
{
	if (pPlayer->GetState() == eObjectState_Die)
		return 0;
	if (!CHKRT->ItemOf(pPlayer, TargetPos, wItemIdx, 0, 0, CB_EXIST | CB_ICONIDX))
		return 0;
	if (pPlayer->GetState() == eObjectState_Immortal)
		OBJECTSTATEMGR_OBJ->EndObjectState(pPlayer, eObjectState_Immortal);
	CItemSlot* pSlot = pPlayer->GetSlot(TargetPos);
	const ITEMBASE* pItemBase = pSlot->GetItemInfoAbs(TargetPos);
	ITEMBASE Item = *pItemBase;
	ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo(wItemIdx);
	if (pItemInfo)
		if (pItemInfo->ItemKind & eCHANGE_ITEM)
		{
			if (m_MultiChangeItemList.GetData(wItemIdx))
			{
				//if (ITEMMGR->IsUnfitItemInfoListForShout(wItemIdx))
				//	SetShoutBoxItem(pPlayer, pItemInfo->ItemIdx);
				return UseMultiChangeItem(pPlayer, TargetPos, wItemIdx, &Item);
			}
			else
			{
				//if (ITEMMGR->IsUnfitItemInfoListForShout(wItemIdx))
				//	SetShoutBoxItem(pPlayer, pItemInfo->ItemIdx);
				return UseNormalChangeItem(pPlayer, TargetPos, wItemIdx, &Item);
			}
		}
		else
			return 0;
	return 1;
}
void CChangeItemMgr::SetShoutBoxItem(CPlayer* pPlayer,DWORD wItemid)
{
	MSG_CHAT_WORD2 msg;
	msg.Category = MP_CLIENT;
	msg.Protocol = MP_CLIENT_SECOND_AGENT;
	msg.Flag = eShoutBoxItem;
	SafeStrCpy(msg.Name1, pPlayer->GetObjectName(), MAX_NAME_LENGTH + 1);
	msg.dwData = (WORD)wItemid;
	pPlayer->SendMsg(&msg, sizeof(msg));
}
int CChangeItemMgr::UseNormalChangeItem(CPlayer* pPlayer, WORD TargetPos, WORD wItemIdx, ITEMBASE* pItem)
{
	CItemSlot* pSlot = pPlayer->GetSlot(eItemTable_Inventory);
	//if (NULL == pSlot) ASSERTMSG(0, "棵官弗 酒捞袍 浇吩阑 掘阑 荐 绝嚼聪促.");
	unsigned int nNum = ITEMMGR->GetTotalEmptySlotNum(pSlot);
	sCHANGEITEMUNIT* pItemUnit = GetItemUnitFromCalPercent(wItemIdx);
	if (!pItemUnit)	return 0;
	unsigned int haveSpaceItemtotalcount = 0;
	if (pItemUnit->wToItemIdx == 7999)
	{
	}
	else if (pItemUnit->wToItemIdx == 7997)
	{
	}
	else
	{
		if (ITEMMGR->IsDupItem(pItemUnit->wToItemIdx))
		{
			unsigned int devidecount = pItemUnit->dwToItemDur / MAX_YOUNGYAKITEM_DUPNUM;
			DWORD pluscount = pItemUnit->dwToItemDur%MAX_YOUNGYAKITEM_DUPNUM;
			haveSpaceItemtotalcount += devidecount;
			if (pluscount)
				haveSpaceItemtotalcount += 1;
		}
		else
		{
			haveSpaceItemtotalcount += pItemUnit->dwToItemDur;
		}
	}
	if ((nNum + 1)  < haveSpaceItemtotalcount)
	{
		MSG_WORD msg;
		msg.Category = MP_ITEM;
		msg.Protocol = MP_ITEM_CHANGEITEM_NACK;
		msg.wData = NOT_SPACE;
		pPlayer->SendMsg(&msg, sizeof(msg));
		return 0;
	}
	if (EI_TRUE != ITEMMGR->DiscardItem(pPlayer, TargetPos, wItemIdx, 1))
		return 0;

	//---------------------------------------------------------
	/*sTIPITEMINFO * GetItem = NULL;
	GetItem = ITEMMGR->GetTipItem(pItemUnit->wToItemIdx);
	if (GetItem)
	{
		ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo(wItemIdx);
		MSG_CHAT_WORD2 msg;
		msg.Category = MP_CLIENT;
		msg.Protocol = MP_CLIENT_SECOND_AGENT;
		msg.Flag = eShoutBoxItem;
		SafeStrCpy(msg.Name1, pPlayer->GetObjectName(), MAX_NAME_LENGTH + 1);
		SafeStrCpy(msg.ItemName, pItemInfo->ItemName, MAX_ITEMNAME_LENGTH + 1);
		SafeStrCpy(msg.ItemName1, GetItem->ItemName, MAX_ITEMNAME_LENGTH + 1);
		pPlayer->SendMsg(&msg, sizeof(msg));
	}*/
	sTIPITEMINFO * GetItem = NULL;
	GetItem = ITEMMGR->GetTipItem(pItemUnit->wToItemIdx);
	if (GetItem)
	{

		ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo(wItemIdx);
		MSG_CHAT_WORD2 msg;
		msg.Category = MP_CLIENT;
		msg.Protocol = MP_CLIENT_SECOND_AGENT;
		msg.Flag = eItemTip;
		SafeStrCpy(msg.Name1, pPlayer->GetObjectName(), MAX_NAME_LENGTH + 1);
		SafeStrCpy(msg.ItemName, pItemInfo->ItemName, MAX_ITEMNAME_LENGTH + 1);
		SafeStrCpy(msg.ItemName1, GetItem->ItemName, MAX_ITEMNAME_LENGTH + 1);

		//g_Console.LOC(0xFFFF66, 4, "UseNormalChangeItem %s %s %s %d", msg.Name1, msg.ItemName, msg.ItemName1, pItemUnit->wToItemIdx);


		pPlayer->SendMsg(&msg, sizeof(msg));
		//g_Network.Broadcast2AgentServer((char*)&msg, sizeof(msg));
	}
	//---------------------------------------------------------
	LogItemMoney(pPlayer->GetID(), pPlayer->GetObjectName(), 0, "",
		eLog_ItemDestroybyChangeItem, pPlayer->GetMoney(eItemTable_Inventory), 0, 0,
		pItem->wIconIdx, pItem->dwDBIdx, pItem->Position, 0, pItem->Durability, pPlayer->GetPlayerExpPoint());
	MSG_ITEM_USE_ACK msg;
	msg.Category = MP_ITEM;
	msg.Protocol = MP_ITEM_USE_ACK;
	msg.dwObjectID = pPlayer->GetID();
	msg.TargetPos = TargetPos;
	msg.wItemIdx = wItemIdx;
	ITEMMGR->SendAckMsg(pPlayer, &msg, sizeof(msg));
	if (pItemUnit->wToItemIdx == 7999)
	{
		pPlayer->SetMoney(pItemUnit->dwToItemDur, MONEY_ADDITION, 64);
	}
	else if (pItemUnit->wToItemIdx == 7997)
	{
		pPlayer->AddAbilityExp(pItemUnit->dwToItemDur);
	}
	else if (pItemUnit->wToItemIdx == 7998)
	{
		MSG_NAME msg;
		msg.Category = MP_USERCONN;
		msg.Protocol = MP_USERCONN_EVENTITEM_USE;
		SafeStrCpy(msg.Name, pPlayer->GetObjectName(), MAX_NAME_LENGTH + 1);
		pPlayer->SendMsg(&msg, sizeof(msg));
	}
	else if (pItemUnit->wToItemIdx == 7996)
	{
		MSG_NAME msg;
		msg.Category = MP_USERCONN;
		msg.Protocol = MP_USERCONN_EVENTITEM_USE2;
		SafeStrCpy(msg.Name, pPlayer->GetObjectName(), MAX_NAME_LENGTH + 1);
		pPlayer->SendMsg(&msg, sizeof(msg));
	}
	else
		ITEMMGR->ObtainItemFromChangeItem(pPlayer, pItemUnit->wToItemIdx, (WORD)pItemUnit->dwToItemDur);
	return 1;
}
unsigned int CChangeItemMgr::ChangedTotalItemNum(sMULTICHANGEITEM* pMultiChangeItem, WORD wMaxSet)
{
	unsigned int haveSpaceItemtotalcount = 0;
	sCHANGEITEMUNIT* pItemUnit = NULL;
	for (WORD i = 0; i < pMultiChangeItem->wMaxSet; ++i)
	{
		DWORD nMin = 0;
		if (pMultiChangeItem->pChangeItem[i].wMaxToItem == 1)
		{
			haveSpaceItemtotalcount++;
		}
		else
			pItemUnit = GetMaxSpaceItemRef(&pMultiChangeItem->pChangeItem[i], nMin);
		if (pItemUnit)
		{
			if (pItemUnit->wToItemIdx == 7999)
			{
			}
			else if (pItemUnit->wToItemIdx == 7997)
			{
			}
			else
			{
				if (ITEMMGR->IsDupItem(pItemUnit->wToItemIdx))
				{
					if (nMin > (pItemUnit->dwToItemDur / MAX_YOUNGYAKITEM_DUPNUM))
					{
						haveSpaceItemtotalcount += nMin;
					}
					else
					{
						haveSpaceItemtotalcount += pItemUnit->dwToItemDur / MAX_YOUNGYAKITEM_DUPNUM;
						DWORD pluscount = pItemUnit->dwToItemDur%MAX_YOUNGYAKITEM_DUPNUM;
						if (pluscount)
							haveSpaceItemtotalcount += 1;
					}
				}
				else
				{
					haveSpaceItemtotalcount += pItemUnit->dwToItemDur;
				}
			}
		}
	}
	return haveSpaceItemtotalcount;
}
int CChangeItemMgr::UseMultiChangeItem(CPlayer* pPlayer, WORD TargetPos, WORD wItemIdx, ITEMBASE* pItem)
{
	CItemSlot* pSlot = pPlayer->GetSlot(eItemTable_Inventory);
	//if (NULL == pSlot) ASSERTMSG(0, "棵官弗 酒捞袍 浇吩阑 掘阑 荐 绝嚼聪促.");
	unsigned int nNum = ITEMMGR->GetTotalEmptySlotNum(pSlot);
	sMULTICHANGEITEM* pMultiChangeItem = m_MultiChangeItemList.GetData(wItemIdx);
	unsigned int haveSpaceItemtotalcount = pMultiChangeItem->nMaxItemSpace;
	if ((nNum + 1)  < haveSpaceItemtotalcount)
	{
		MSG_WORD msg;
		msg.Category = MP_ITEM;
		msg.Protocol = MP_ITEM_CHANGEITEM_NACK;
		msg.wData = NOT_SPACE;
		pPlayer->SendMsg(&msg, sizeof(msg));
		return 0;
	}
	if (pPlayer->GetMaxLevel() < pMultiChangeItem->wLimitLevel)	return 0;
	if (EI_TRUE != ITEMMGR->DiscardItem(pPlayer, TargetPos, wItemIdx, 1))	return 0;
	LogItemMoney(pPlayer->GetID(), pPlayer->GetObjectName(), 0, "",
		eLog_ItemDestroybyChangeItem, pPlayer->GetMoney(eItemTable_Inventory), 0, 0,
		pItem->wIconIdx, pItem->dwDBIdx, pItem->Position, 0, pItem->Durability, pPlayer->GetPlayerExpPoint());
	MSG_ITEM_USE_ACK msg;
	msg.Category = MP_ITEM;
	msg.Protocol = MP_ITEM_USE_ACK;
	msg.dwObjectID = pPlayer->GetID();
	msg.TargetPos = TargetPos;
	msg.wItemIdx = wItemIdx;
	ITEMMGR->SendAckMsg(pPlayer, &msg, sizeof(msg));

	BOOL FristShowTip = FALSE;

	sCHANGEITEMUNIT* pItemUnit = NULL;
	for (WORD i = 0; i < pMultiChangeItem->wMaxSet; ++i)
	{
		if (pMultiChangeItem->pChangeItem[i].wMaxToItem == 1)
			pItemUnit = &pMultiChangeItem->pChangeItem[i].pItemUnit[0];
		else
			pItemUnit = GetMultiItemUnitFromCalPercent(&pMultiChangeItem->pChangeItem[i]);
		if (pItemUnit)
		{
			if (pItemUnit->wToItemIdx == 7999)
			{
				pPlayer->SetMoney(pItemUnit->dwToItemDur, MONEY_ADDITION, 64);
			}
			else if (pItemUnit->wToItemIdx == 7997)
			{
				pPlayer->AddAbilityExp(pItemUnit->dwToItemDur);
			}
			else
			{
				if (!FristShowTip)
				{
					//Daily 속흙역芎路狼膠틔瓊刻
					sTIPITEMINFO * GetItem = NULL;
					GetItem = ITEMMGR->GetTipItem(pItemUnit->wToItemIdx);
					if (GetItem)
					{

						ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo(wItemIdx);
						MSG_CHAT_WORD2 msg;
						msg.Category = MP_CLIENT;
						msg.Protocol = MP_CLIENT_SECOND_AGENT;
						msg.Flag = eItemTip;
						SafeStrCpy(msg.Name1, pPlayer->GetObjectName(), MAX_NAME_LENGTH + 1);
						SafeStrCpy(msg.ItemName, pItemInfo->ItemName, MAX_ITEMNAME_LENGTH + 1);
						SafeStrCpy(msg.ItemName1, GetItem->ItemName, MAX_ITEMNAME_LENGTH + 1);
						//g_Console.LOC(0xFFFF66, 4, "test_2 %s %s %s %d", msg.Name1, msg.ItemName, msg.ItemName1, pItemUnit->wToItemIdx);
						pPlayer->SendMsg(&msg, sizeof(msg));
						//g_Network.Broadcast2AgentServer((char*)&msg, sizeof(msg));
						FristShowTip = TRUE;
					}
				}
				ITEMMGR->ObtainItemFromChangeItem(pPlayer, pItemUnit->wToItemIdx, (WORD)pItemUnit->dwToItemDur);
			}
		}
	}
	return 1;
}
sCHANGEITEMUNIT* CChangeItemMgr::GetItemUnitFromCalPercent(WORD wItemIdx)
{
	sCHANGEITEM* pData = m_ChangeItemList.GetData(wItemIdx);
	if (pData == NULL) return NULL;
#ifdef _HK_LOCAL_
	DWORD RandRateHigh = rand() % 1000;
	DWORD RandRateLow = rand() % 1000;
	DWORD RandRate = RandRateHigh * 1000 + RandRateLow;
#else
	DWORD RandRate = rand() % MAX_CHANGE_RATE;
#endif
	DWORD FromPercent = 0;
	DWORD ToPercent = 0;
	for (WORD i = 0; i < pData->wMaxToItem; ++i)
	{
		FromPercent = ToPercent;
		ToPercent += pData->pItemUnit[i].dwPercent;
		if (FromPercent <= RandRate && RandRate < ToPercent)
		{
			return &pData->pItemUnit[i];
		}
	}
	return NULL;
}
sCHANGEITEMUNIT* CChangeItemMgr::GetMaxSpaceItemRef(sCHANGEITEM* pSet, DWORD& nMin)
{
	DWORD MaxSpaceCount = 0;
	WORD MaxSpaceItemIdx = 0;
	for (WORD i = 0; i < pSet->wMaxToItem; ++i)
	{
		if (MaxSpaceCount < pSet->pItemUnit[i].dwToItemDur)
		{
			MaxSpaceCount = pSet->pItemUnit[i].dwToItemDur;
			MaxSpaceItemIdx = i;
		}
		if (FALSE == ITEMMGR->IsDupItem(pSet->pItemUnit[i].wToItemIdx))
		{
			if (nMin < pSet->pItemUnit[i].dwToItemDur)
				nMin = pSet->pItemUnit[i].dwToItemDur;
		}
	}
	return &pSet->pItemUnit[MaxSpaceItemIdx];
}
sCHANGEITEMUNIT* CChangeItemMgr::GetMultiItemUnitFromCalPercent(sCHANGEITEM* pSet)
{
#ifdef _HK_LOCAL_
	DWORD RandRateHigh = rand() % 1000;
	DWORD RandRateLow = rand() % 1000;
	DWORD RandRate = RandRateHigh * 1000 + RandRateLow;
#else
	DWORD RandRate = rand() % MAX_CHANGE_RATE;
#endif
	DWORD FromPercent = 0;
	DWORD ToPercent = 0;
	for (WORD i = 0; i < pSet->wMaxToItem; ++i)
	{
		FromPercent = ToPercent;
		ToPercent += pSet->pItemUnit[i].dwPercent;
		if (FromPercent <= RandRate && RandRate < ToPercent)
		{
			return &pSet->pItemUnit[i];
		}
	}
	return NULL;
}
int	CChangeItemMgr::UseChangeItemFromShopItem(CPlayer* pPlayer, WORD wItemIdx)
{
	if (pPlayer->GetState() == eObjectState_Die)
		return 0;
	if (pPlayer->GetState() == eObjectState_Immortal)
		OBJECTSTATEMGR_OBJ->EndObjectState(pPlayer, eObjectState_Immortal);
	ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo(wItemIdx);
	if (pItemInfo && pItemInfo->ItemKind == eSHOP_ITEM_SUNDRIES)
	{
		if (m_MultiChangeItemList.GetData(wItemIdx))
		{
			return UseMultiChangeItemFromShopItem(pPlayer, wItemIdx);
		}
		else
		{
			return UseNormalChangeItemFromShopItem(pPlayer, wItemIdx);
		}
	}
	return 1;
}
int CChangeItemMgr::UseNormalChangeItemFromShopItem(CPlayer* pPlayer, WORD wItemIdx)
{
	sCHANGEITEMUNIT* pItemUnit = GetItemUnitFromCalPercent(wItemIdx);
	if (!pItemUnit)	return 1;
	ITEM_INFO* pResultItemInfo = NULL;
	CItemSlot* pSlot = NULL;
	unsigned int nNum = 0;
	if (pItemUnit->wToItemIdx != 7996 && pItemUnit->wToItemIdx != 7997 &&
		pItemUnit->wToItemIdx != 7998 && pItemUnit->wToItemIdx != 7999)
	{
		pResultItemInfo = ITEMMGR->GetItemInfo(pItemUnit->wToItemIdx);
		if (!pResultItemInfo)
			return 1;
		if (pResultItemInfo->ItemKind & eSHOP_ITEM)
			pSlot = pPlayer->GetSlot(eItemTable_ShopInven);
		else
			pSlot = pPlayer->GetSlot(eItemTable_Inventory);
		//if (NULL == pSlot) ASSERTMSG(0, "棵官弗 酒捞袍 浇吩阑 掘阑 荐 绝嚼聪促.");
		nNum = ITEMMGR->GetTotalEmptySlotNum(pSlot);
	}
	unsigned int haveSpaceItemtotalcount = 0;
	if (pItemUnit->wToItemIdx == 7999)
	{
	}
	else if (pItemUnit->wToItemIdx == 7997)
	{
	}
	else
	{
		if (ITEMMGR->IsDupItem(pItemUnit->wToItemIdx))
		{
			unsigned int devidecount = pItemUnit->dwToItemDur / MAX_YOUNGYAKITEM_DUPNUM;
			DWORD pluscount = pItemUnit->dwToItemDur%MAX_YOUNGYAKITEM_DUPNUM;
			haveSpaceItemtotalcount += devidecount;
			if (pluscount)
				haveSpaceItemtotalcount += 1;
		}
		else
		{
			haveSpaceItemtotalcount += pItemUnit->dwToItemDur;
		}
	}
	if ((nNum + 1)  < haveSpaceItemtotalcount)
		return 2;
	if (pItemUnit->wToItemIdx == 7999)
	{
		pPlayer->SetMoney(pItemUnit->dwToItemDur, MONEY_ADDITION, MF_OBTAIN);
	}
	else if (pItemUnit->wToItemIdx == 7997)
	{
		pPlayer->AddAbilityExp(pItemUnit->dwToItemDur);
	}
	else if (pItemUnit->wToItemIdx == 7998)
	{
		MSG_NAME msg;
		msg.Category = MP_USERCONN;
		msg.Protocol = MP_USERCONN_EVENTITEM_USE;
		SafeStrCpy(msg.Name, pPlayer->GetObjectName(), MAX_NAME_LENGTH + 1);
		pPlayer->SendMsg(&msg, sizeof(msg));
	}
	else if (pItemUnit->wToItemIdx == 7996)
	{
		MSG_NAME msg;
		msg.Category = MP_USERCONN;
		msg.Protocol = MP_USERCONN_EVENTITEM_USE2;
		SafeStrCpy(msg.Name, pPlayer->GetObjectName(), MAX_NAME_LENGTH + 1);
		pPlayer->SendMsg(&msg, sizeof(msg));
	}
	else
		return ITEMMGR->ObtainItemFromChangeItem(pPlayer, pItemUnit->wToItemIdx, (WORD)pItemUnit->dwToItemDur);
	return 0;
}
int CChangeItemMgr::UseMultiChangeItemFromShopItem(CPlayer* pPlayer, WORD wItemIdx)
{
	CItemSlot* pSlot = pPlayer->GetSlot(eItemTable_Inventory);
	//if (NULL == pSlot) ASSERTMSG(0, "棵官弗 酒捞袍 浇吩阑 掘阑 荐 绝嚼聪促.");
	unsigned int nNum = ITEMMGR->GetTotalEmptySlotNum(pSlot);
	sMULTICHANGEITEM* pMultiChangeItem = m_MultiChangeItemList.GetData(wItemIdx);
	unsigned int haveSpaceItemtotalcount = pMultiChangeItem->nMaxItemSpace;
	if ((nNum + 1)  < haveSpaceItemtotalcount)
		return 2;
	if (pPlayer->GetMaxLevel() < pMultiChangeItem->wLimitLevel)	return 0;
	sCHANGEITEMUNIT* pItemUnit = NULL;
	for (WORD i = 0; i < pMultiChangeItem->wMaxSet; ++i)
	{
		if (pMultiChangeItem->pChangeItem[i].wMaxToItem == 1)
			pItemUnit = &pMultiChangeItem->pChangeItem[i].pItemUnit[0];
		else
			pItemUnit = GetMultiItemUnitFromCalPercent(&pMultiChangeItem->pChangeItem[i]);
		if (pItemUnit)
		{
			if (pItemUnit->wToItemIdx == 7999)
			{
				pPlayer->SetMoney(pItemUnit->dwToItemDur, MONEY_ADDITION, MF_OBTAIN);
			}
			else if (pItemUnit->wToItemIdx == 7997)
			{
				pPlayer->AddAbilityExp(pItemUnit->dwToItemDur);
			}
			else
			{
				ITEMMGR->ObtainItemFromChangeItem(pPlayer, pItemUnit->wToItemIdx, (WORD)pItemUnit->dwToItemDur);
			}
		}
	}
	return 0;
}