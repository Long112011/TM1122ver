#include "stdafx.h"
#include "MixManager.h"
#include "MHFile.h"
#ifdef _MAPSERVER_
#include "MapDBMsgParser.h"
#include "Player.h"
#include "..\[CC]Header\GameResourceManager.h"
#include "PackedData.h"
#include "ItemManager.h"
#include "CheckRoutine.h"
#include "AbilityManager.h"
#include "TitanItemManager.h"
#include "BattleSystem_Server.h"
#include "UserTable.h"
#endif
#ifndef _MAPSERVER_
GLOBALTON(CMixManager)
#endif
CMixManager::CMixManager()
{
#ifdef _MAPSERVER_
	m_HiLevelItemMixRateInfoList.Initialize(50);
#endif
}
void CMixManager::Release()
{
	m_MixItemInfoList.clear();
}
CMixManager::~CMixManager()
{
	Release();
}
void CMixManager::LoadMixResultItemInfo(CMHFile * fp, ITEM_MIX_RES * pResInfo)
{
	pResInfo->wResItemIdx = fp->GetWord();
	SafeStrCpy(pResInfo->szResItemName, fp->GetString(), MAX_ITEMNAME_LENGTH + 1);
	pResInfo->wMixKind = fp->GetWord();
	pResInfo->Money = fp->GetDword();
	pResInfo->SuccessRatio = fp->GetWord();
	pResInfo->wMaterialItemNum = fp->GetWord();
	if (pResInfo->wMaterialItemNum != 0)
	{
		pResInfo->psMaterialItemInfo = new ITEM_MIX_MATERIAL[pResInfo->wMaterialItemNum];
		for (int i = 0; i < pResInfo->wMaterialItemNum; ++i)
		{
			pResInfo->psMaterialItemInfo[i].wMatItemIdx = fp->GetWord();
			SafeStrCpy(pResInfo->psMaterialItemInfo[i].szMatItemName, fp->GetString(), MAX_ITEMNAME_LENGTH + 1);
			pResInfo->psMaterialItemInfo[i].wMatItemNum = fp->GetWord();
		}
	}
}
void CMixManager::LoadMixList()
{
	Release();
	CMHFile file;
	if (!file.Init("Resource/ItemMixList.bin", "rb"))
		return;
	WORD wTmpItemIdx = 0;
	WORD wTmpRestIdx = 0;
	WORD wTmpResultNum = 0;
	std::map<WORD, ITEM_MIX_INFO>::iterator Iter;
	while (1)
	{
		if (file.IsEOF())
			break;
		wTmpItemIdx = file.GetWord();
		Iter = m_MixItemInfoList.find(wTmpItemIdx);
		if (Iter != m_MixItemInfoList.end())
		{
			SafeStrCpy(Iter->second.szItemName, file.GetString(), MAX_ITEMNAME_LENGTH + 1);
			wTmpResultNum = file.GetWord();
			Iter->second.wResultItemNum = wTmpResultNum;
			for (wTmpRestIdx = 0; wTmpRestIdx < Iter->second.wResultItemNum; ++wTmpRestIdx)
			{
				if (Iter->second.psResultItemInfo[wTmpRestIdx].wResItemIdx == 0)
				{
					LoadMixResultItemInfo(&file, &Iter->second.psResultItemInfo[wTmpRestIdx]);
					break;
				}
			}
		}
		else
		{
			ITEM_MIX_INFO  pInfo;
			memset(&pInfo, 0, sizeof(ITEM_MIX_INFO));
			pInfo.wItemIdx = wTmpItemIdx;
			SafeStrCpy(pInfo.szItemName, file.GetString(), MAX_ITEMNAME_LENGTH + 1);
			wTmpResultNum = file.GetWord();
			pInfo.wResultItemNum = wTmpResultNum;
			if (!pInfo.psResultItemInfo)
			{
				pInfo.psResultItemInfo = new ITEM_MIX_RES[pInfo.wResultItemNum];
				memset(pInfo.psResultItemInfo, 0, sizeof(ITEM_MIX_RES)*pInfo.wResultItemNum);
			}
			for (wTmpRestIdx = 0; wTmpRestIdx < pInfo.wResultItemNum; ++wTmpRestIdx)
			{
				if (pInfo.psResultItemInfo[wTmpRestIdx].wResItemIdx == 0)
				{
					LoadMixResultItemInfo(&file, &pInfo.psResultItemInfo[wTmpRestIdx]);
					break;
				}
			}
			m_MixItemInfoList[pInfo.wItemIdx] = pInfo;
		}
	}
	file.Release();
#ifdef _MAPSERVER_
	if (!file.Init("Resource/Server/HiLevelItemMixRate.bin", "rb"))
		return;

	while (1)
	{
		if (file.IsEOF())
			break;
		HILEVEL_ITEM_MIX_RATE_INFO*	pRateInfo = NULL;
		pRateInfo = new HILEVEL_ITEM_MIX_RATE_INFO;
		ZeroMemory(pRateInfo, sizeof(HILEVEL_ITEM_MIX_RATE_INFO));
		pRateInfo->wItemLevel = file.GetWord();
		pRateInfo->dwBaseRate = file.GetDword();
		pRateInfo->dwModulate = file.GetWord();
		m_HiLevelItemMixRateInfoList.Add(pRateInfo, pRateInfo->wItemLevel);
	}
	file.Release();
#endif
}
const ITEM_MIX_INFO * CMixManager::GetMixItemInfo(WORD wItemIdx) const
{
	std::map<WORD, ITEM_MIX_INFO>::const_iterator Iter = m_MixItemInfoList.find(wItemIdx);
	return m_MixItemInfoList.end() == Iter ? 0 : &(Iter->second);
}
#ifdef _MAPSERVER_
BOOL CMixManager::EnoughMixMaterial(WORD needItemIdx, WORD needItemNum, MATERIAL_ARRAY * pMaterialArray, WORD wMaterialNum)
{
	DURTYPE deNeedItemNum = needItemNum;
	for (int i = 0; i < wMaterialNum; ++i)
	{
		if (deNeedItemNum == 0) break;
		if (pMaterialArray[i].wItemIdx == needItemIdx)
		{
			if (pMaterialArray[i].Dur < deNeedItemNum)
			{
				if (!ITEMMGR->IsDupItem(pMaterialArray[i].wItemIdx))
					deNeedItemNum = deNeedItemNum - 1;
				else
					deNeedItemNum -= pMaterialArray[i].Dur;
			}
			else
			{
				deNeedItemNum = 0;
			}
		}
	}
	if (deNeedItemNum == 0)
		return TRUE;
	else
		return FALSE;
}
int CMixManager::MixItem(CPlayer* pPlayer, WORD wBasicItemIdx, POSTYPE BasicItemPos, POSTYPE ResultIdx, MATERIAL_ARRAY * pMaterialArray, WORD wMaterialNum, WORD ShopItemIdx, WORD ShopItemPos, BOOL bTitan)
{
	if (!bTitan)
	{
		if (!CHKRT->AbilityOf(pPlayer, eAUKJOB_Mix))
			return 1;
	}
	if (!CHKRT->ItemOf(pPlayer, BasicItemPos, wBasicItemIdx, 0, 0, CB_EXIST | CB_ICONIDX))
		return 2;
	eITEM_KINDBIT eBit = GetItemKind(wBasicItemIdx);
	if (eBit == eEQUIP_ITEM_UNIQUE && GAMERESRCMNGR->GetUniqueItemOptionList(wBasicItemIdx)->MixFlag == 0)
	{
		return 2;
	}
	ITEM_INFO* pShopItem = NULL;
	if (ShopItemIdx)
	{
		if (!CHKRT->ItemOf(pPlayer, ShopItemPos, ShopItemIdx, 0, 0, CB_EXIST | CB_ICONIDX))
			return 20;
		pShopItem = ITEMMGR->GetItemInfo(ShopItemIdx);
		if (!pShopItem)		return 2;
	}
	int i = 0;
	for (i = 0; i < wMaterialNum; ++i)
	{
		if (BasicItemPos == pMaterialArray[i].ItemPos)
			return 3;
		if (!CHKRT->ItemOf(pPlayer, pMaterialArray[i].ItemPos, pMaterialArray[i].wItemIdx, pMaterialArray[i].Dur, 0, CB_EXIST | CB_ICONIDX | CB_ENOUGHDURA))
			return 3;
	}
	const ITEM_MIX_INFO * pMixInfo = NULL;
	if (bTitan)
		pMixInfo = TITANITEMMGR->GetTitanMixItemInfo(wBasicItemIdx);
	else
		pMixInfo = GetMixItemInfo(wBasicItemIdx);
	if (!pMixInfo)
		return 4;
	if (pMixInfo->wResultItemNum <= ResultIdx)	return 4;
	const ITEMBASE * pBasicItemBase = ITEMMGR->GetItemInfoAbsIn(pPlayer, BasicItemPos);
	BOOL bIsCanMix = FALSE;
	bIsCanMix = CanMixItem(pBasicItemBase->wIconIdx);
	if (GetItemKind(pBasicItemBase->wIconIdx) != eYOUNGYAK_ITEM &&
		GetItemKind(pBasicItemBase->wIconIdx) != eEXTRA_ITEM_JEWEL &&
		GetItemKind(pBasicItemBase->wIconIdx) != eEXTRA_ITEM_FUSE &&
		!bIsCanMix &&
		(pBasicItemBase->Durability != 1 && !IsEquipItem(GetItemKind(pBasicItemBase->wIconIdx))))
	{
		return 7;
	}
	MONEYTYPE money;
	if (ITEMMGR->IsDupItem(wBasicItemIdx))
		money = pMixInfo->psResultItemInfo[ResultIdx].Money * pBasicItemBase->Durability;
	else
		money = pMixInfo->psResultItemInfo[ResultIdx].Money;
	if (pPlayer->GetMoney() < money)
		return 6;
	WORD needItemIdx = 0;
	WORD needItemNum = 0;
	for (i = 0; i < pMixInfo->psResultItemInfo[ResultIdx].wMaterialItemNum; ++i)
	{
		needItemIdx = pMixInfo->psResultItemInfo[ResultIdx].psMaterialItemInfo[i].wMatItemIdx;
		needItemNum = pMixInfo->psResultItemInfo[ResultIdx].psMaterialItemInfo[i].wMatItemNum;
		if (ITEMMGR->IsDupItem(wBasicItemIdx))
			needItemNum *= (WORD)pBasicItemBase->Durability;
		if (!EnoughMixMaterial(needItemIdx, needItemNum, pMaterialArray, wMaterialNum))
			return 5;
	}
	ITEM_INFO* pNewItemInfo = ITEMMGR->GetItemInfo(pMixInfo->psResultItemInfo[ResultIdx].wResItemIdx);
	if (ShopItemIdx)
	{
		if (!pNewItemInfo)			return 2;
		if (pNewItemInfo->LimitLevel < pShopItem->GenGol || pNewItemInfo->LimitLevel > pShopItem->MinChub)
			return 23;
	}
	DWORD ratio = 0;
	WORD mixGrade = ABILITYMGR->GetAbilityLevel(ABILITYINDEX_ITEMMIX, pPlayer->GetAbilityGroup());
	WORD per = MAX_MIX_PERCENT / 100;
	ITEM_INFO* pResItemInfo = ITEMMGR->GetItemInfo(pMixInfo->psResultItemInfo[ResultIdx].wResItemIdx);
	if (pResItemInfo->ItemGrade >= 19)
	{
		HILEVEL_ITEM_MIX_RATE_INFO* pRateInfo = m_HiLevelItemMixRateInfoList.GetData(pResItemInfo->ItemGrade);
		ratio = pRateInfo->dwBaseRate + (mixGrade * 10000 / pRateInfo->dwModulate);
	}
	else
		ratio = (pMixInfo->psResultItemInfo[ResultIdx].SuccessRatio + mixGrade*per);
	DWORD Key = ITEMMGR->MakeNewKey();
	ITEMBASE ItemBase;
	ITEMBASE Item;
	memset(&Item, 0, sizeof(ITEMBASE));
	char NameKey[MAX_NAME_LENGTH + 1];
	sprintf(NameKey, "%d", Key);
	CItemSlot * pSlot = pPlayer->GetSlot(0);
	BOOL bUseMixUp = FALSE;
	BOOL bSuccess = FALSE;
	BOOL bProtection = FALSE;
	DWORD Seed;
	if (pResItemInfo->ItemGrade >= 19)
		Seed = LargeRandom(0, MAX_MIX_LARGE_PERCENT);
	else
		Seed = random(0, MAX_MIX_PERCENT);
	if (pPlayer->GetShopItemStats()->ItemMixSuccess)
	{
		SHOPITEMWITHTIME* ShopInfo = pPlayer->GetShopItemManager()->GetUsingItemInfo(55322);
		ratio += (WORD)(MAX_MIX_PERCENT*0.1f);
		pPlayer->GetShopItemManager()->CalcShopItemOption(eIncantation_MixUp, FALSE);
		pPlayer->GetShopItemManager()->SendMsgDwordToPlayer(MP_ITEM_SHOPITEM_MIXUP, eIncantation_MixUp);
		pPlayer->GetShopItemManager()->DeleteUsingShopItem(eIncantation_MixUp);
		bUseMixUp = TRUE;
	}
	if (Seed < ratio)
		bSuccess = TRUE;
	if (pResItemInfo->ItemGrade >= 19)
	{
		bSuccess = TRUE;
	}
	if (!bSuccess && ShopItemIdx)
	{
		bProtection = TRUE;
	}
	else
	{
		for (i = 0; i < wMaterialNum; ++i)
		{
			Item = *pSlot->GetItemInfoAbs(pMaterialArray[i].ItemPos);
			if (EI_TRUE != ITEMMGR->DiscardItem(pPlayer, pMaterialArray[i].ItemPos, pMaterialArray[i].wItemIdx, pMaterialArray[i].Dur))
				return 10;
			LogItemMoney(pPlayer->GetID(), NameKey, pPlayer->GetID(), pPlayer->GetObjectName(), eLog_ItemDestroyMix,
				Item.Durability, Item.Durability - pMaterialArray[i].Dur, pMaterialArray[i].Dur, Item.wIconIdx, Item.dwDBIdx,
				Item.Position, 0, Item.Durability, pPlayer->GetPlayerExpPoint());
		}
		pPlayer->SetMoney(money, MONEY_SUBTRACTION);
	}
	if (ShopItemIdx)
	{
		const ITEMBASE* pShop = ITEMMGR->GetItemInfoAbsIn(pPlayer, ShopItemPos);
		if (pShopItem)
		{
			if (EI_TRUE != ITEMMGR->DiscardItem(pPlayer, ShopItemPos, ShopItemIdx, 1))
				ASSERT(0);
			SEND_SHOPITEM_BASEINFO msg;
			msg.Category = MP_ITEM;
			msg.Protocol = MP_ITEM_SHOPITEM_USE_ACK;
			msg.ShopItemPos = ShopItemPos;
			msg.ShopItemIdx = ShopItemIdx;
			ITEMMGR->SendAckMsg(pPlayer, &msg, sizeof(msg));
			LogItemMoney(pPlayer->GetID(), NameKey, pPlayer->GetID(), pPlayer->GetObjectName(), eLog_ShopItemUse,
				pPlayer->GetMoney(), pPlayer->GetMoney(), 0, ShopItemIdx, pShop->dwDBIdx, ShopItemPos, 0,
				Item.Position, pPlayer->GetPlayerExpPoint());
		}
	}
	if (bSuccess)
	{
		ITEM_INFO* pinfo = ITEMMGR->GetItemInfo(pMixInfo->psResultItemInfo[ResultIdx].wResItemIdx);
		if (!pinfo)
			return 2;
		CItemSlot * pSlot = pPlayer->GetSlot(BasicItemPos);
		if (pinfo->ItemKind & eSHOP_ITEM)
		{
			if (EI_TRUE != ITEMMGR->DiscardItem(pPlayer, BasicItemPos, wBasicItemIdx, 1))
				return 2;
			ITEMMGR->ObtainItemFromChangeItem(pPlayer, pMixInfo->psResultItemInfo[ResultIdx].wResItemIdx, 1);
			LogItemMoney(pPlayer->GetID(), NameKey, pPlayer->GetID(), pPlayer->GetObjectName(), eLog_ItemMixSuccess,
				pPlayer->GetMoney(), ResultIdx, money, pinfo->ItemIdx, 0, 0, 0, 1,
				pPlayer->GetPlayerExpPoint());
		}
		else
		{
			WORD flag = UB_ICONIDX;
			if (!ITEMMGR->IsDupItem(pMixInfo->psResultItemInfo[ResultIdx].wResItemIdx))
				flag |= UB_DURA;
			if (EI_TRUE != pSlot->UpdateItemAbs(pPlayer, BasicItemPos, 0, pMixInfo->psResultItemInfo[ResultIdx].wResItemIdx, 0, 0, 0, flag, SS_LOCKOMIT))
				return 6;
			const ITEMBASE * pItemBase = pSlot->GetItemInfoAbs(BasicItemPos);
			if (pItemBase->RareIdx < 0)
			{
				ItemUpdateToDB(pPlayer->GetID(), pItemBase->dwDBIdx, pItemBase->wIconIdx,
					pItemBase->Durability, pItemBase->Position, pItemBase->QuickPosition, 0);
				LogItemMoney(pPlayer->GetID(), NameKey, pPlayer->GetID(), pPlayer->GetObjectName(), eLog_ItemMixSuccess,
					pPlayer->GetMoney(), ResultIdx, money, pItemBase->wIconIdx, pItemBase->dwDBIdx, pItemBase->Position, pItemBase->Position, pItemBase->Durability,
					pPlayer->GetPlayerExpPoint());
			}
			else
			{
				ItemUpdateToDB(pPlayer->GetID(), pItemBase->dwDBIdx, pItemBase->wIconIdx,
					pItemBase->Durability, pItemBase->Position, pItemBase->QuickPosition, pItemBase->RareIdx);
				LogItemMoney(pPlayer->GetID(), NameKey, pPlayer->GetID(), pPlayer->GetObjectName(), eLog_ItemMixSuccess,
					pPlayer->GetMoney(), ResultIdx, money, pItemBase->wIconIdx, pItemBase->dwDBIdx, pItemBase->Position, pItemBase->Position, pItemBase->Durability,
					pPlayer->GetPlayerExpPoint());
			}
		}
		return 0;
	}
	else
	{
		Seed = random(0, MAX_MIX_PERCENT);
		ItemBase = *pSlot->GetItemInfoAbs(BasicItemPos);
		float fBicFail_Prob;
		int eBigFail_Kind;
		int eFail_Kind;
		DWORD dwOptionIdx;
		if (!ItemBase.RareIdx)
		{
			fBicFail_Prob = 0.2f;
			eBigFail_Kind = eLog_ItemMixBigFail;
			eFail_Kind = eLog_ItemMixFail;
			dwOptionIdx = ItemBase.Durability;
		}
		else
		{
			fBicFail_Prob = 0.3f;
			eBigFail_Kind = eLog_ItemRareMixBigFail;
			eFail_Kind = eLog_ItemRareMixFail;
			dwOptionIdx = ItemBase.RareIdx;
		}
		if (!bUseMixUp && Seed <= MAX_MIX_PERCENT*fBicFail_Prob)
		{
			if (EI_TRUE != ITEMMGR->DiscardItem(pPlayer, BasicItemPos, wBasicItemIdx, 9999))
				return 8;
			LogItemMoney(pPlayer->GetID(), NameKey, pPlayer->GetID(), pPlayer->GetObjectName(), eBigFail_Kind,
				pPlayer->GetMoney(), ResultIdx, money, ItemBase.wIconIdx, ItemBase.dwDBIdx, ItemBase.Position, 0,
				dwOptionIdx, pPlayer->GetPlayerExpPoint());
			if (bProtection)
				return 21;
			else
				return 1000;
		}
		else
		{
			LogItemMoney(pPlayer->GetID(), NameKey, pPlayer->GetID(), pPlayer->GetObjectName(), eFail_Kind,
				pPlayer->GetMoney(), ResultIdx, money, ItemBase.wIconIdx, ItemBase.dwDBIdx, ItemBase.Position,
				ItemBase.Position, dwOptionIdx, pPlayer->GetPlayerExpPoint());
			if (bProtection)
				return 22;
			else
				return 1001;
		}
	}
	return 11;
}

BOOL CMixManager::CanMixItem(WORD wItemIndex)
{
	const ITEM_MIX_INFO * pMixItemInfo = GetMixItemInfo(wItemIndex);
	if (pMixItemInfo)
		return true;
	else
		return false;
}
void CMixManager::NetworkMsgParse(DWORD dwConnectionIndex, BYTE Protocol, void* pMsg)
{
	MSGBASE * pmsg = (MSGBASE *)pMsg;
	CPlayer * pPlayer = (CPlayer *)g_pUserTable->FindUser(pmsg->dwObjectID);
	if (pPlayer)
	if (BATTLESYSTEM->GetBattle(pPlayer)->GetBattleKind() == eBATTLE_KIND_SURYUN)
	{
		return;
	}
	switch (Protocol)
	{
		case MP_ITEM_MIX_ADDITEM_SYN:
		{
			MSG_ITEM* pmsg = (MSG_ITEM*)pMsg;
			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
			if (pPlayer == NULL) return;
			WORD wErrorCode = 0;
			const ITEM_MIX_INFO* pMixItemInfo = NULL;
			CItemSlot * pSlot = NULL;
			MSG_ITEM msgAck;
			BOOL bIsCanMix = FALSE;
			bIsCanMix = MIXMGR->CanMixItem(pmsg->ItemInfo.wIconIdx);
			if (GetTableIdxPosition(pmsg->ItemInfo.Position) != eItemTable_Inventory&&
				!bIsCanMix)
			{
				wErrorCode = 1;
				goto go_MIX_ADDITEM_NACK;
			}
			if (!CHKRT->ItemOf(pPlayer, pmsg->ItemInfo.Position, pmsg->ItemInfo.wIconIdx, 0, 0, CB_EXIST | CB_ICONIDX))
			{
				wErrorCode = 2;
				goto go_MIX_ADDITEM_NACK;
			}
			pSlot = pPlayer->GetSlot(eItemTable_Inventory);
			if (pSlot)
			if (pSlot->IsLock(pmsg->ItemInfo.Position))
			{
				wErrorCode = 3;
				goto go_MIX_ADDITEM_NACK;
			}
			if (ITEMMGR->IsOptionItem(pmsg->ItemInfo.wIconIdx, pmsg->ItemInfo.Durability))
			{
				wErrorCode = 4;
				goto go_MIX_ADDITEM_NACK;
			}
			pMixItemInfo = MIXMGR->GetMixItemInfo(pmsg->ItemInfo.wIconIdx);
			if (!pMixItemInfo)
			{
				wErrorCode = 5;
				goto go_MIX_ADDITEM_NACK;
			}
			if (GetItemKind(pmsg->ItemInfo.wIconIdx) != eYOUNGYAK_ITEM &&
				GetItemKind(pmsg->ItemInfo.wIconIdx) != eEXTRA_ITEM_JEWEL &&
				GetItemKind(pmsg->ItemInfo.wIconIdx) != eEXTRA_ITEM_FUSE &&

				pmsg->ItemInfo.Durability > 1 &&
				!bIsCanMix)
			{
				wErrorCode = 6;
				goto go_MIX_ADDITEM_NACK;
			}
			pSlot->SetLock(pmsg->ItemInfo.Position, TRUE);
			msgAck.Category = MP_ITEM;
			msgAck.Protocol = MP_ITEM_MIX_ADDITEM_ACK;
			msgAck.ItemInfo = pmsg->ItemInfo;
			pPlayer->SendMsg(&msgAck, sizeof(msgAck));
			break;
		go_MIX_ADDITEM_NACK:
			MSG_WORD2 msgNack;
			msgNack.Category = MP_ITEM;
			msgNack.Protocol = MP_ITEM_MIX_ADDITEM_NACK;
			msgNack.wData1 = pmsg->ItemInfo.Position;
			msgNack.wData2 = wErrorCode;
			pPlayer->SendMsg(&msgNack, sizeof(msgNack));
		}
		break;
	case MP_ITEM_MIX_RELEASEITEM:
		{
			MSG_WORD* pmsg = (MSG_WORD*)pMsg;
			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
			if (pPlayer == NULL) return;
			CItemSlot* pSlot = pPlayer->GetSlot(pmsg->wData);
			if (pSlot)
				pSlot->SetLock(pmsg->wData, FALSE);
		}
		break;
	case MP_ITEM_MIX_SYN:
		{
			MSG_ITEM_MIX_SYN * pmsg = (MSG_ITEM_MIX_SYN *)pMsg;
			CPlayer * pPlayer = (CPlayer *)g_pUserTable->FindUser(pmsg->dwObjectID);
			if (pPlayer == NULL) return;
			int rt = 0;
			if (EI_TRUE == (rt = MIXMGR->MixItem(pPlayer, pmsg->wBasicItemIdx, pmsg->BasicItemPos, pmsg->ResultIndex, pmsg->Material, pmsg->wMaterialNum, pmsg->ShopItemIdx, pmsg->ShopItemPos)))
			{
				MSG_ITEM_MIX_ACK msg;
				memcpy(&msg, pmsg, sizeof(MSG_ITEM_MIX_SYN));
				msg.Protocol = MP_ITEM_MIX_SUCCESS_ACK;
				ITEMMGR->SendAckMsg(pPlayer, &msg, msg.GetSize());
			}
			else
			{
				switch (rt)
				{
				case 1000:
					{
						MSG_ITEM_MIX_ACK msg;
						memcpy(&msg, pmsg, sizeof(MSG_ITEM_MIX_SYN));
						msg.Protocol = MP_ITEM_MIX_BIGFAILED_ACK;
						ITEMMGR->SendAckMsg(pPlayer, &msg, msg.GetSize());
					}
					break;
				case 1001:
					{
						MSG_ITEM_MIX_ACK msg;
						memcpy(&msg, pmsg, sizeof(MSG_ITEM_MIX_SYN));
						msg.Protocol = MP_ITEM_MIX_FAILED_ACK;
						ITEMMGR->SendAckMsg(pPlayer, &msg, msg.GetSize());
					}
					break;
				case 20:
				case 21:
				case 22:
				case 23:
				case 256:
					{
						MSG_DWORD2 msg;
						msg.Category = MP_ITEM;
						msg.Protocol = MP_ITEM_MIX_MSG;
						msg.dwObjectID = pPlayer->GetID();
						msg.dwData1 = rt;
						msg.dwData2 = pmsg->BasicItemPos;
						pPlayer->SendMsg(&msg, sizeof(msg));
					}
					break;
				default:
					{
						if (rt != 2)
						{
						}
						MSG_ITEM_ERROR msg;
						msg.Category = MP_ITEM;
						msg.Protocol = MP_ITEM_ERROR_NACK;
						msg.dwObjectID = pPlayer->GetID();
						msg.ECode = eItemUseErr_Mix;
						ITEMMGR->SendErrorMsg(pPlayer, &msg, sizeof(msg), rt);
					}
					break;
				}
			}
		}
		break;
		//default:
		//break;
	}
}
#endif