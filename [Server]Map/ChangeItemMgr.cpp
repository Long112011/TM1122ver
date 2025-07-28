// ChangeItemMgr.cpp: implementation of the CChangeItemMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ChangeItemMgr.h"
#include "MHFile.h"
#include "../[CC]Header/GameResourceStruct.h"
#include "ItemManager.h"
#include "Player.h"
#include "ObjectStateManager.h"
#include "MapDBMsgParser.h"
#include "CheckRoutine.h"
#include "GuildManager.h"
#include "Guild.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

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
		//		delete[] pData->pItemUnit;
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
#ifdef _FILE_BIN_
	//	if( !file.Init( "./Resource/ItemChangeList.bin", "rb" ) )
	if (!file.Init("./Resource/Server/ItemChangeList.bin", "rb"))
		return FALSE;
#else
	if (!file.Init("./Resource/ItemChangeList.txt", "rt"))
		return FALSE;
#endif
	while (1)
	{
		if (file.IsEOF())
			break;
		sCHANGEITEM* pData = new sCHANGEITEM;
		pData->wItemIdx = file.GetWord();
		pData->wCurChangeItemKey = file.GetWord(); //增加开箱所需钥匙类型
		pData->wToItemType = file.GetWord(); //添加物品状态
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

#ifdef _FILE_BIN_
	//	if( !file.Init( "./Resource/ItemChangeListMulti.bin", "rb" ) )
	if (!file.Init("./Resource/Server/ItemChangeListMulti.bin", "rb"))
		return FALSE;
#else
	if (!file.Init("./Resource/ItemChangeListMulti.txt", "rt"))
		return FALSE;
#endif
	while (1)
	{
		if (file.IsEOF())
			break;
		sMULTICHANGEITEM* pData = new sMULTICHANGEITEM;
		pData->wItemIdx = file.GetWord();
		pData->wCurChangeItemKey = file.GetWord(); //增加开箱所需钥匙类型
		pData->wToItemType = file.GetWord(); //添加物品状态
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
	//所有箱子必须百宝栏、背包都至少有一格
	if (ITEMMGR->GetTotalEmptySlotNum(pPlayer->GetSlot(eItemTable_ShopInven)) < 1
		|| ITEMMGR->GetTotalEmptySlotNum(pPlayer->GetSlot(eItemTable_Inventory)) < 1)
	{
		MSG_WORD msg;
		msg.Category = MP_ITEM;
		msg.Protocol = MP_ITEM_CHANGEITEM_NACK;
		msg.wData = NOT_SPACE;
		pPlayer->SendMsg(&msg, sizeof(msg));
		return 0;
	}
	CItemSlot* pSlot = pPlayer->GetSlot(TargetPos);
	const ITEMBASE* pItemBase = pSlot->GetItemInfoAbs(TargetPos);
	ITEMBASE Item = *pItemBase;
	ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo(wItemIdx);
	sMULTICHANGEITEM* pMultiChangeItem = m_MultiChangeItemList.GetData(wItemIdx);
	sCHANGEITEM* pData = m_ChangeItemList.GetData(wItemIdx);
	WORD wKeyItemPos;
	DWORD dwKeyItemIdx;

	if (pItemInfo)
		if (pItemInfo->ItemKind & eCHANGE_ITEM)
		{
			if (m_ChangeItemList.GetData(wItemIdx) && pData->wCurChangeItemKey != 0)//有需求钥匙在此处理钥匙不足
			{
				if (m_ChangeItemList.GetData(wItemIdx) && !CheckBoxKey(pData->wCurChangeItemKey, pPlayer, wKeyItemPos, dwKeyItemIdx))
					return 0;//钥匙不足打开箱子失败在此处理
			}
			if (m_MultiChangeItemList.GetData(wItemIdx) && pMultiChangeItem->wCurChangeItemKey != 0)//有需求钥匙在此处理钥匙不足
			{
				if (m_MultiChangeItemList.GetData(wItemIdx) && !CheckBoxKey(pMultiChangeItem->wCurChangeItemKey, pPlayer, wKeyItemPos, dwKeyItemIdx))
					return 0;//钥匙不足打开箱子失败在次处理
			}
			//没有钥匙需求或钥匙足够执行以下
			if (m_MultiChangeItemList.GetData(wItemIdx))
			{
				return UseMultiChangeItem(pPlayer, TargetPos, wItemIdx, &Item);
			}
			else
			{
				return UseNormalChangeItem(pPlayer, TargetPos, wItemIdx, &Item);
			}
		}
		else
			return 0;

	return 1;
}
//添加箱子需求钥匙检查
BOOL CChangeItemMgr::CheckBoxKey(WORD wCurNeedBoxKey, CPlayer* pPlayer, WORD& wKeyItemPos, DWORD& dwKeyItemIdx)
{
	if (!pPlayer) return FALSE;
	ITEM_TOTALINFO ItemTotalInfo;
	pPlayer->GetItemtotalInfo(&ItemTotalInfo, GETITEM_FLAG_INVENTORY);//查询背包信息
	BOOL bfind = FALSE;
	for (int i = 0; i < SLOT_INVENTORY_NUM; ++i) //遍历背包
	{
		ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo(ItemTotalInfo.Inventory[i].wIconIdx);//获取物品信息
		if (!pItemInfo) continue;//没有找到或找到物品跳出循环
		if (pItemInfo->ItemKind == eCHANGE_ITEM_KEY)//判断是否为钥匙类型32770，查找是否存在钥匙
		{
			if (pItemInfo->BuyPrice == wCurNeedBoxKey)//开箱所需类型是否和钥匙一致
			{
				wKeyItemPos = i;
				dwKeyItemIdx = ItemTotalInfo.Inventory[i].wIconIdx;
				bfind = TRUE;
				return TRUE;
			}
		}
	}
	if (!bfind)//没有找到钥匙发送客户端提示
	{
		MSG_ITEM_ERROR msg;
		msg.Category = MP_ITEM;
		msg.Protocol = MP_ITEM_USE_NACK;
		msg.dwObjectID = pPlayer->GetID();
		msg.ECode = 999;
		ITEMMGR->SendErrorMsg(pPlayer, &msg, sizeof(msg), 999);
	}
	return FALSE;
}
int CChangeItemMgr::UseNormalChangeItem(CPlayer* pPlayer, WORD TargetPos, WORD wItemIdx, ITEMBASE* pItem)
{
	CItemSlot* pSlot = pPlayer->GetSlot(eItemTable_Inventory);
	if (NULL == pSlot) ASSERTMSG(0, "角色背包数据获取失败");
	WORD ItemStatic = GetToItemTypeForNormal(wItemIdx);

	WORD ItemQuality = ITEMMGR->GetItemQuality();
	WORD ItemEntry1 = 0;
	WORD ItemEntry2 = 0;

	if (ItemQuality >= 1 && ItemQuality <= 4)
	{
		ItemEntry1 = ITEMMGR->GetItemEntry1(); // 1~11 随机值
		ItemEntry2 = 0;
	}
	else
	{
		ItemEntry1 = 0;
		ItemEntry2 = 0;
	}
	unsigned int nNum = ITEMMGR->GetTotalEmptySlotNum(pSlot);

	sCHANGEITEMUNIT* pItemUnit = GetItemUnitFromCalPercent(wItemIdx);
	if (!pItemUnit)	return 0;

	unsigned int haveSpaceItemtotalcount = 0;
	//if( pItemUnit->wToItemIdx == 7999 ) //money
	//{
	//	
	//}
	//else if( pItemUnit->wToItemIdx == 7997 )	// 荐访摹
	//{
	//	
	//}
	if (pItemUnit->wToItemIdx == eChangeMoney || pItemUnit->wToItemIdx == ChangeToSuryun ||
		pItemUnit->wToItemIdx == eChangeGoldMoney || pItemUnit->wToItemIdx == ChangeToMonster ||
		pItemUnit->wToItemIdx == eChangeMallMoney || pItemUnit->wToItemIdx == eChangeGuildExp)
	{

	}
	else
	{
		if (ITEMMGR->IsDupItem(pItemUnit->wToItemIdx))
		{
			unsigned int devidecount = pItemUnit->dwToItemDur / MAX_YOUNGYAKITEM_DUPNUM;
			DWORD pluscount = pItemUnit->dwToItemDur % MAX_YOUNGYAKITEM_DUPNUM;

			haveSpaceItemtotalcount += devidecount;
			if (pluscount)
				haveSpaceItemtotalcount += 1;
		}
		else
		{
			haveSpaceItemtotalcount += pItemUnit->dwToItemDur;
		}
	}

	//if( (nNum+1)  < haveSpaceItemtotalcount)
	if ((pItemUnit->wToItemIdx != eChangeMoney) && (pItemUnit->wToItemIdx != ChangeToSuryun)
		&& (pItemUnit->wToItemIdx != ChangeToMonster))
	{
		if (IsNormalChangeItemSlot(pPlayer, pItemUnit) == FALSE)
		{
			MSG_WORD msg;
			msg.Category = MP_ITEM;
			msg.Protocol = MP_ITEM_CHANGEITEM_NACK;
			msg.wData = NOT_SPACE;
			pPlayer->SendMsg(&msg, sizeof(msg));
			return 0;
		}
	}

	if (EI_TRUE != ITEMMGR->DiscardItem(pPlayer, TargetPos, wItemIdx, 1))
		return 0;
	//钥匙物品检查
	sCHANGEITEM* pData = m_ChangeItemList.GetData(wItemIdx);
	WORD wKeyItemPos;
	DWORD dwKeyItemIdx;
	if (pData->wCurChangeItemKey != 0)
	{//检查箱子是否需要钥匙
		if (CheckBoxKey(pData->wCurChangeItemKey, pPlayer, wKeyItemPos, dwKeyItemIdx)) //需要钥匙，检查钥匙
		{
			ITEMMGR->DiscardItem(pPlayer, wKeyItemPos, dwKeyItemIdx, 1);//使用箱子成功服务端删除钥匙
			MSG_ITEM_USE_ACK msg;
			msg.Category = MP_ITEM;
			msg.Protocol = MP_ITEM_USE_ACK;//箱子使用成功通知客户端删除钥匙
			msg.dwObjectID = pPlayer->GetID();
			msg.TargetPos = wKeyItemPos;//钥匙位置
			msg.wItemIdx = dwKeyItemIdx;//钥匙编号
			ITEMMGR->SendAckMsg(pPlayer, &msg, sizeof(msg));
		}
	}

	LogItemMoney(pPlayer->GetID(), pPlayer->GetObjectName(), 0, "",
		eLog_ItemDestroybyChangeItem, pPlayer->GetMoney(eItemTable_Inventory), 0, 0,
		pItem->wIconIdx, pItem->dwDBIdx, pItem->Position, 0, pItem->Durability, pPlayer->GetPlayerExpPoint());

	// send item use
	MSG_ITEM_USE_ACK msg;
	msg.Category = MP_ITEM;
	msg.Protocol = MP_ITEM_USE_ACK;
	msg.dwObjectID = pPlayer->GetID();
	msg.TargetPos = TargetPos;
	msg.wItemIdx = wItemIdx;
	ITEMMGR->SendAckMsg(pPlayer, &msg, sizeof(msg));

	if (pItemUnit->wToItemIdx == eChangeMoney)		//游戏币
	{
		pPlayer->SetMoney(pItemUnit->dwToItemDur, MONEY_ADDITION, 64);	//烙矫 64
	}
	else if (pItemUnit->wToItemIdx == ChangeToSuryun)	// 技能点
	{
		pPlayer->AddAbilityExp(pItemUnit->dwToItemDur);
	}
	else if (pItemUnit->wToItemIdx == eChangeGoldMoney)//元宝
	{
		pPlayer->SetGoldMoney(pItemUnit->dwToItemDur, eGetGoldMoney);
		//ItemShopUseLog(eLog_ChangeItemGetGoldMoney, pPlayer->GetID(), pPlayer->GetObjectName(),
		//	pItemUnit->wToItemIdx, ITEMMGR->GetItemInfo(wItemIdx)->ItemName, 1,
		//	0, 0, 0,
		//	pPlayer->GetGoldMoney(), pItemUnit->dwToItemDur, pPlayer->GetGoldMoney() + pItemUnit->dwToItemDur,
		//	0, 0, 0);
	}
	else if (pItemUnit->wToItemIdx == ChangeToMonster)//角色经验
	{
		pPlayer->AddPlayerExpPoint(pItemUnit->dwToItemDur);
	}
	else if (pItemUnit->wToItemIdx == eChangeMallMoney)//泡点
	{
		//ItemShopUseLog(eLog_ChangeItemGetMallMoney, pPlayer->GetID(), pPlayer->GetObjectName(),
		//	pItemUnit->wToItemIdx, ITEMMGR->GetItemInfo(wItemIdx)->ItemName, 1,
		//	pPlayer->GetMallMoney(), pItemUnit->dwToItemDur, pPlayer->GetMallMoney() + pItemUnit->dwToItemDur,
		//	0, 0, 0,
		//	0, 0, 0);
		pPlayer->SetMallMoney(pItemUnit->dwToItemDur, 2);

	}
	else if (pItemUnit->wToItemIdx == eChangeGuildExp)
	{
		DWORD GuildIdx = pPlayer->GetGuildIdx();
		if (0 == GuildIdx) return 0;

		CGuild* pGuild = GUILDMGR->GetGuild(GuildIdx);

		if (NULL == pGuild) return 0;

		LogGuildPoint(GuildIdx, pGuild->GetGuildPoint(), TRUE, eGuildUseChangeItem, pItemUnit->dwToItemDur, pPlayer->GetMaxLevel(), pPlayer->GetID());

		GuildAddGuildPoint(GuildIdx, pItemUnit->dwToItemDur, eGuildUseChangeItem, pPlayer->GetID());
	}
	else if (pItemUnit->wToItemIdx == ChangeToEvent1)
	{
		// event 050203
		MSG_NAME msg;
		msg.Category = MP_USERCONN;
		msg.Protocol = MP_USERCONN_EVENTITEM_USE;
		SafeStrCpy(msg.Name, pPlayer->GetObjectName(), MAX_NAME_LENGTH + 1);
		//		strcpy( msg.Name, pPlayer->GetObjectName() );
		pPlayer->SendMsg(&msg, sizeof(msg));
	}
	else if (pItemUnit->wToItemIdx == ChangeToEvent2)
	{
		MSG_NAME msg;
		msg.Category = MP_USERCONN;
		msg.Protocol = MP_USERCONN_EVENTITEM_USE2;
		SafeStrCpy(msg.Name, pPlayer->GetObjectName(), MAX_NAME_LENGTH + 1);
		pPlayer->SendMsg(&msg, sizeof(msg));
	}
	else
	{
		ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo(pItemUnit->wToItemIdx);
		if (pItemInfo->ItemKind & eEQUIP_ITEM || pItemInfo->ItemKind == eEQUIP_ITEM_UNIQUE)
			ITEMMGR->ObtainItemFromChangeItem(pPlayer, pItemUnit->wToItemIdx, (WORD)pItemUnit->dwToItemDur, ItemStatic, ItemQuality, ItemEntry1, ItemEntry2, 0);//箱子开出物品增加状态
		else
			ITEMMGR->ObtainItemFromChangeItem(pPlayer, pItemUnit->wToItemIdx, (WORD)pItemUnit->dwToItemDur, ItemStatic, 0, 0, 0, 0);//箱子开出物品增加状态

		//ITEMMGR->SetItemValue(1);
		ITEM_INFO* pInfo = ITEMMGR->GetItemInfo(wItemIdx);
		ITEMMGR->SetChangeItemName(pInfo->ItemName);
	}
	//箱子物品提示发送
	//sTIPITEMINFO * GetItem = NULL;
	//GetItem = ITEMMGR->GetTipItem(pItemUnit->wToItemIdx);
	//if(GetItem)
	//{
	//	ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo( wItemIdx );

	//	MSG_TIP msg;
	//	msg.Category = MP_CLIENT;
	//	msg.Protocol = MP_CLIENT_MSG_AGENT;
	//	msg.Flag = eItemTip;
	//	SafeStrCpy(msg.Name1,pPlayer->GetObjectName(),MAX_NAME_LENGTH+1);
	//	SafeStrCpy(msg.ItemName,pItemInfo->ItemName,MAX_ITEMNAME_LENGTH+1);
	//	SafeStrCpy(msg.ItemName1,GetItem->ItemName,MAX_ITEMNAME_LENGTH+1);
	//	pPlayer->SendMsgToAllAgent(&msg, sizeof(msg));
	//}

	return 1;
}

unsigned int CChangeItemMgr::ChangedTotalItemNum(sMULTICHANGEITEM* pMultiChangeItem, WORD wMaxSet)//060613 Add by wonju 
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
			//if( pItemUnit->wToItemIdx == 7999 ) //money
			//{
			//	
			//}
			//else if( pItemUnit->wToItemIdx == 7997 )	// 荐访摹
			//{
			//	
			//}
			if (pItemUnit->wToItemIdx == eChangeMoney || pItemUnit->wToItemIdx == ChangeToSuryun ||
				pItemUnit->wToItemIdx == eChangeGoldMoney || pItemUnit->wToItemIdx == ChangeToMonster ||
				pItemUnit->wToItemIdx == eChangeMallMoney || pItemUnit->wToItemIdx == eChangeGuildExp)
			{

			}
			else
			{
				if (ITEMMGR->IsDupItem(pItemUnit->wToItemIdx)) //盒秦啊 登绰 巴捞搁
				{
					if (nMin > (pItemUnit->dwToItemDur / MAX_YOUNGYAKITEM_DUPNUM)) //盒秦啊 救登绰 酒捞袍 蔼狼 弥措蔼焊促 累促搁
					{
						haveSpaceItemtotalcount += nMin; //盒秦啊 救登绰 酒捞袍 蔼狼 弥措蔼阑 持绰促.
					}
					else
					{
						haveSpaceItemtotalcount += pItemUnit->dwToItemDur / MAX_YOUNGYAKITEM_DUPNUM;
						DWORD pluscount = pItemUnit->dwToItemDur % MAX_YOUNGYAKITEM_DUPNUM;
						if (pluscount)
							haveSpaceItemtotalcount += 1;
					}
				}
				else//般媚龙 荐 绝促搁 弊措肺 历厘
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
	if (NULL == pSlot) ASSERTMSG(0, "棵官弗 酒捞袍 浇吩阑 掘阑 荐 绝嚼聪促.");

	unsigned int nNum = ITEMMGR->GetTotalEmptySlotNum(pSlot);

	sMULTICHANGEITEM* pMultiChangeItem = m_MultiChangeItemList.GetData(wItemIdx);

	int ItemQuality = ITEMMGR->GetItemQuality();
	int ItemEntry1 = 0;
	int ItemEntry2 = 0;

	if (ItemQuality >= 1 && ItemQuality <= 4)
	{
		ItemEntry1 = ITEMMGR->GetItemEntry1(); // 1~11 随机值
		ItemEntry2 = 0;
	}
	else
	{
		ItemEntry1 = 0;
		ItemEntry2 = 0;
	}

	//unsigned int haveSpaceItemtotalcount = pMultiChangeItem->nMaxItemSpace;

	//if( (nNum+1)  < haveSpaceItemtotalcount)
	if (IsMultiChangeItemSlot(pPlayer, pMultiChangeItem) == FALSE)
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
	//钥匙物品检查
	WORD wKeyItemPos;
	DWORD dwKeyItemIdx;
	if (pMultiChangeItem->wCurChangeItemKey != 0)
	{//检查开箱是否需要钥匙
		if (CheckBoxKey(pMultiChangeItem->wCurChangeItemKey, pPlayer, wKeyItemPos, dwKeyItemIdx)) //需要钥匙，检查钥匙
		{
			ITEMMGR->DiscardItem(pPlayer, wKeyItemPos, dwKeyItemIdx, 1);//箱子使用成功服务端删除钥匙
			MSG_ITEM_USE_ACK msg;
			msg.Category = MP_ITEM;
			msg.Protocol = MP_ITEM_USE_ACK;//箱子使用成功通知客户端删除钥匙
			msg.dwObjectID = pPlayer->GetID();
			msg.TargetPos = wKeyItemPos;//钥匙位置
			msg.wItemIdx = dwKeyItemIdx;//钥匙编号
			ITEMMGR->SendAckMsg(pPlayer, &msg, sizeof(msg));
		}
	}
	// log
	LogItemMoney(pPlayer->GetID(), pPlayer->GetObjectName(), 0, "",
		eLog_ItemDestroybyChangeItem, pPlayer->GetMoney(eItemTable_Inventory), 0, 0,
		pItem->wIconIdx, pItem->dwDBIdx, pItem->Position, 0, pItem->Durability, pPlayer->GetPlayerExpPoint());

	// send item use
	MSG_ITEM_USE_ACK msg;
	msg.Category = MP_ITEM;
	msg.Protocol = MP_ITEM_USE_ACK;
	msg.dwObjectID = pPlayer->GetID();
	msg.TargetPos = TargetPos;
	msg.wItemIdx = wItemIdx;
	ITEMMGR->SendAckMsg(pPlayer, &msg, sizeof(msg));

	sCHANGEITEMUNIT* pItemUnit = NULL;
	for (WORD i = 0; i < pMultiChangeItem->wMaxSet; ++i)
	{
		if (pMultiChangeItem->pChangeItem[i].wMaxToItem == 1)
			pItemUnit = &pMultiChangeItem->pChangeItem[i].pItemUnit[0];
		else
			pItemUnit = GetMultiItemUnitFromCalPercent(&pMultiChangeItem->pChangeItem[i]);

		if (pItemUnit)
		{
			if (pItemUnit->wToItemIdx == eChangeMoney)//游戏币
			{
				pPlayer->SetMoney(pItemUnit->dwToItemDur, MONEY_ADDITION, 64);
			}
			else if (pItemUnit->wToItemIdx == ChangeToSuryun)	// 技能点
			{
				pPlayer->AddAbilityExp(pItemUnit->dwToItemDur);
			}
			else if (pItemUnit->wToItemIdx == eChangeGoldMoney)//元宝
			{
				pPlayer->SetGoldMoney(pItemUnit->dwToItemDur, eGetGoldMoney);
				//ItemShopUseLog(eLog_ChangeItemGetGoldMoney, pPlayer->GetID(), pPlayer->GetObjectName(),
				//	pItemUnit->wToItemIdx, ITEMMGR->GetItemInfo(wItemIdx)->ItemName, 1,
				//	0, 0, 0,
				//	pPlayer->GetGoldMoney(), pItemUnit->dwToItemDur, pPlayer->GetGoldMoney() + pItemUnit->dwToItemDur,
				//	0, 0, 0);
			}
			else if (pItemUnit->wToItemIdx == ChangeToMonster)//角色经验
			{
				pPlayer->AddPlayerExpPoint(pItemUnit->dwToItemDur);
			}
			else if (pItemUnit->wToItemIdx == eChangeMallMoney)//泡点
			{
				//ItemShopUseLog(eLog_ChangeItemGetMallMoney, pPlayer->GetID(), pPlayer->GetObjectName(),
				//	pItemUnit->wToItemIdx, ITEMMGR->GetItemInfo(wItemIdx)->ItemName, 1,
				//	pPlayer->GetMallMoney(), pItemUnit->dwToItemDur, pPlayer->GetMallMoney() + pItemUnit->dwToItemDur,
				//	0, 0, 0,
				//	0, 0, 0);
				pPlayer->SetMallMoney(pItemUnit->dwToItemDur, 2);

			}
			else if (pItemUnit->wToItemIdx == eChangeGuildExp)//帮会经验
			{
				DWORD GuildIdx = pPlayer->GetGuildIdx();
				if (0 == GuildIdx) return 0;

				CGuild* pGuild = GUILDMGR->GetGuild(GuildIdx);

				if (NULL == pGuild) return 0;

				LogGuildPoint(GuildIdx, pGuild->GetGuildPoint(), TRUE, eGuildUseChangeItem, pItemUnit->dwToItemDur, pPlayer->GetMaxLevel(), pPlayer->GetID());

				GuildAddGuildPoint(GuildIdx, pItemUnit->dwToItemDur, eGuildUseChangeItem, pPlayer->GetID());
			}
			else
			{
				ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo(pItemUnit->wToItemIdx);
				if (pItemInfo->ItemKind & eEQUIP_ITEM || pItemInfo->ItemKind == eEQUIP_ITEM_UNIQUE)
					ITEMMGR->ObtainItemFromChangeItem(pPlayer, pItemUnit->wToItemIdx, (WORD)pItemUnit->dwToItemDur, pMultiChangeItem->wToItemType, ItemQuality, ItemEntry1, ItemEntry2, 0);//箱子开出物品增加状态
				else
					ITEMMGR->ObtainItemFromChangeItem(pPlayer, pItemUnit->wToItemIdx, (WORD)pItemUnit->dwToItemDur, pMultiChangeItem->wToItemType, 0, 0, 0, 0);

				//ITEMMGR->SetItemValue(1);
				ITEM_INFO* pInfo = ITEMMGR->GetItemInfo(wItemIdx);
				ITEMMGR->SetChangeItemName(pInfo->ItemName);
			}
			//MultiChangeItem箱子物品提示发送
			//sTIPITEMINFO * GetItem = NULL;
			//GetItem = ITEMMGR->GetTipItem(pItemUnit->wToItemIdx);
			//if(GetItem)
			//{
			//	ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo( wItemIdx );
			//	MSG_TIP msg;
			//	msg.Category = MP_CLIENT;
			//	msg.Protocol = MP_CLIENT_MSG_AGENT;
			//	msg.Flag = eItemTip;
			//	SafeStrCpy(msg.Name1,pPlayer->GetObjectName(),MAX_NAME_LENGTH+1);
			//	SafeStrCpy(msg.ItemName,pItemInfo->ItemName,MAX_ITEMNAME_LENGTH+1);
			//	SafeStrCpy(msg.ItemName1,GetItem->ItemName,MAX_ITEMNAME_LENGTH+1);
			//	pPlayer->SendMsgToAllAgent(&msg, sizeof(msg));
			//}
		}
	}

	return 1;
}
//箱子物品是否加锁
WORD CChangeItemMgr::GetToItemTypeForNormal(WORD wItemIdx)
{
	sCHANGEITEM* pData = m_ChangeItemList.GetData(wItemIdx);
	if (pData == NULL)
		return 2;
	else
		return pData->wToItemType;
}
sCHANGEITEMUNIT* CChangeItemMgr::GetItemUnitFromCalPercent(WORD wItemIdx)
{
	sCHANGEITEM* pData = m_ChangeItemList.GetData(wItemIdx);
	if (pData == NULL) return NULL;

#ifdef _HK_LOCAL_
	DWORD RandRateHigh = rand() % 1000;
	DWORD RandRateLow = rand() % 1000;
	DWORD RandRate = RandRateHigh * 1000 + RandRateLow;	//(0 ~ 999,999)归父盒狼老
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
	//啊厘 腹篮 傍埃阑 瞒瘤窍绰 酒捞袍狼 器牢磐甫 倒妨霖促.
	DWORD MaxSpaceCount = 0;
	WORD MaxSpaceItemIdx = 0;
	for (WORD i = 0; i < pSet->wMaxToItem; ++i)
	{
		if (MaxSpaceCount < pSet->pItemUnit[i].dwToItemDur)
		{
			MaxSpaceCount = pSet->pItemUnit[i].dwToItemDur;
			MaxSpaceItemIdx = i;
		}

		//般媚瘤瘤 臼绰 酒捞袍狼 弥措蔼阑 备茄促.
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
	DWORD RandRate = RandRateHigh * 1000 + RandRateLow;	//(0 ~ 999,999)归父盒狼老
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

BOOL CChangeItemMgr::IsMultiChangeItemSlot(CPlayer* pPlayer, sMULTICHANGEITEM* pMultiChangeItem)
{
	CItemSlot* pInventorySlot = pPlayer->GetSlot(eItemTable_Inventory);
	if (NULL == pInventorySlot) ASSERTMSG(0, "棵官弗 老馆 酒捞袍 浇吩阑 掘阑 荐 绝嚼聪促.");

	CItemSlot* pShopInventorySlot = pPlayer->GetSlot(eItemTable_ShopInven);
	if (NULL == pShopInventorySlot) ASSERTMSG(0, "棵官弗 某浆 酒捞袍 浇吩阑 掘阑 荐 绝嚼聪促.");

	unsigned int nInvenNum = ITEMMGR->GetTotalEmptySlotNum(pInventorySlot);
	unsigned int nShopInvenNum = ITEMMGR->GetTotalEmptySlotNum(pShopInventorySlot);

	ITEM_INFO* pItemInfo = NULL;
	unsigned int dwItemCount = 0;
	unsigned int dwShopItemCount = 0;

	for (int i = 0; i < pMultiChangeItem->wMaxSet; i++)
	{
		pItemInfo = ITEMMGR->GetItemInfo(pMultiChangeItem->pChangeItem[i].pItemUnit[0].wToItemIdx);
		if (pItemInfo == NULL)
			continue;

		if (pItemInfo->ItemKind & eSHOP_ITEM)
			dwShopItemCount++;
		else
			dwItemCount++;
	}

	if (nInvenNum < dwItemCount)
		return FALSE;

	if (nShopInvenNum < dwShopItemCount)
		return FALSE;


	return TRUE;
}

BOOL CChangeItemMgr::IsNormalChangeItemSlot(CPlayer* pPlayer, sCHANGEITEMUNIT* pNormalChangeItem)
{
	CItemSlot* pInventorySlot = pPlayer->GetSlot(eItemTable_Inventory);
	if (NULL == pInventorySlot) ASSERTMSG(0, "棵官弗 老馆 酒捞袍 浇吩阑 掘阑 荐 绝嚼聪促.");

	CItemSlot* pShopInventorySlot = pPlayer->GetSlot(eItemTable_ShopInven);
	if (NULL == pShopInventorySlot) ASSERTMSG(0, "棵官弗 某浆 酒捞袍 浇吩阑 掘阑 荐 绝嚼聪促.");

	unsigned int nInvenNum = ITEMMGR->GetTotalEmptySlotNum(pInventorySlot);
	unsigned int nShopInvenNum = ITEMMGR->GetTotalEmptySlotNum(pShopInventorySlot);

	ITEM_INFO* pItemInfo = NULL;
	unsigned int dwItemCount = 0;
	unsigned int dwShopItemCount = 0;
	unsigned int haveSpaceItemtotalcount = 0;

	pItemInfo = ITEMMGR->GetItemInfo(pNormalChangeItem->wToItemIdx);
	if (pItemInfo == NULL)
		return FALSE;


	//酒捞牌隔 酒捞牌
	if (pItemInfo->ItemKind & eSHOP_ITEM)
		dwShopItemCount++;
	else	//老馆 酒捞牌
	{
		if (ITEMMGR->IsDupItem(pItemInfo->ItemIdx))
		{
			//角力 俺荐甫 舅酒郴绢辑 MAX_YOUNGYAKITEM_DUPNUM肺唱传促.
			unsigned int devidecount = pNormalChangeItem->dwToItemDur / MAX_YOUNGYAKITEM_DUPNUM;
			DWORD pluscount = pNormalChangeItem->dwToItemDur % MAX_YOUNGYAKITEM_DUPNUM;

			dwItemCount += devidecount;
			if (pluscount)
				dwItemCount += 1;
		}
		else
		{
			dwItemCount += pNormalChangeItem->dwToItemDur;
		}
	}

	if (nInvenNum < dwItemCount)
		return FALSE;

	if (nShopInvenNum < dwShopItemCount)
		return FALSE;

	return TRUE;
}


//////////////////////////////////////////////////////////////////////////
// [START] RaMa - 06.04.19  Shop酒捞袍栏肺 ChangeItem荤侩
//////////////////////////////////////////////////////////////////////////
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
	//敲饭捞绢狼 巢酒乐绰 浇吩 俺荐甫 舅酒辰促.
	sCHANGEITEMUNIT* pItemUnit = GetItemUnitFromCalPercent(wItemIdx);
	if (!pItemUnit)	return 1;

	ITEM_INFO* pResultItemInfo = NULL;
	CItemSlot* pSlot = NULL;
	unsigned int nNum = 0;

	//if( pItemUnit->wToItemIdx != 7996 && pItemUnit->wToItemIdx != 7997 && 
	//	pItemUnit->wToItemIdx != 7998 && pItemUnit->wToItemIdx != 7999 )
	if (pItemUnit->wToItemIdx != ChangeToEvent2 && pItemUnit->wToItemIdx != ChangeToSuryun &&
		pItemUnit->wToItemIdx != ChangeToEvent1 && pItemUnit->wToItemIdx != eChangeMoney &&
		pItemUnit->wToItemIdx != ChangeToMonster && pItemUnit->wToItemIdx != eChangeGoldMoney &&
		pItemUnit->wToItemIdx != eChangeMallMoney && pItemUnit->wToItemIdx != eChangeGuildExp)

	{
		pResultItemInfo = ITEMMGR->GetItemInfo(pItemUnit->wToItemIdx);
		if (!pResultItemInfo)
			return 1;

		if (pResultItemInfo->ItemKind & eSHOP_ITEM)
			pSlot = pPlayer->GetSlot(eItemTable_ShopInven);
		else
			pSlot = pPlayer->GetSlot(eItemTable_Inventory);

		if (NULL == pSlot) ASSERTMSG(0, "棵官弗 酒捞袍 浇吩阑 掘阑 荐 绝嚼聪促.");

		nNum = ITEMMGR->GetTotalEmptySlotNum(pSlot);
	}

	unsigned int haveSpaceItemtotalcount = 0;

	//if( pItemUnit->wToItemIdx == 7999 ) //money
	//{

	//}
	//else if( pItemUnit->wToItemIdx == 7997 )	// 荐访摹
	//{

	//}
	if (pItemUnit->wToItemIdx == eChangeMoney || pItemUnit->wToItemIdx == ChangeToSuryun ||
		pItemUnit->wToItemIdx == eChangeGoldMoney || pItemUnit->wToItemIdx == ChangeToMonster ||
		pItemUnit->wToItemIdx == eChangeMallMoney || pItemUnit->wToItemIdx == eChangeGuildExp)
	{

	}
	else
	{
		if (ITEMMGR->IsDupItem(pItemUnit->wToItemIdx))
		{
			//角力 俺荐甫 舅酒郴绢辑 5肺唱传促.
			unsigned int devidecount = pItemUnit->dwToItemDur / MAX_YOUNGYAKITEM_DUPNUM;
			DWORD pluscount = pItemUnit->dwToItemDur % MAX_YOUNGYAKITEM_DUPNUM;

			haveSpaceItemtotalcount += devidecount;
			if (pluscount)
				haveSpaceItemtotalcount += 1;
		}
		else
		{
			haveSpaceItemtotalcount += pItemUnit->dwToItemDur;
		}
	}

	if ((nNum + 1) < haveSpaceItemtotalcount)
		return 2;

	if (pItemUnit->wToItemIdx == eChangeMoney)		// money
	{
		pPlayer->SetMoney(pItemUnit->dwToItemDur, MONEY_ADDITION, MF_OBTAIN);
	}
	else if (pItemUnit->wToItemIdx == ChangeToSuryun)	// 荐访摹
	{
		pPlayer->AddAbilityExp(pItemUnit->dwToItemDur);
	}
	else if (pItemUnit->wToItemIdx == eChangeGoldMoney)//元宝
	{
		pPlayer->SetGoldMoney(pItemUnit->dwToItemDur, eGetGoldMoney);
		//ItemShopUseLog(eLog_ChangeItemGetGoldMoney, pPlayer->GetID(), pPlayer->GetObjectName(),
		//	pItemUnit->wToItemIdx, ITEMMGR->GetItemInfo(wItemIdx)->ItemName, 1,
		//	0, 0, 0,
		//	pPlayer->GetGoldMoney(), pItemUnit->dwToItemDur, pPlayer->GetGoldMoney() + pItemUnit->dwToItemDur,
		//	0, 0, 0);
	}
	else if (pItemUnit->wToItemIdx == ChangeToMonster)//角色经验
	{
		pPlayer->AddPlayerExpPoint(pItemUnit->dwToItemDur);
	}
	else if (pItemUnit->wToItemIdx == eChangeMallMoney)//泡点
	{
		//ItemShopUseLog(eLog_ChangeItemGetMallMoney, pPlayer->GetID(), pPlayer->GetObjectName(),
		//	pItemUnit->wToItemIdx, ITEMMGR->GetItemInfo(wItemIdx)->ItemName, 1,
		//	pPlayer->GetMallMoney(), pItemUnit->dwToItemDur, pPlayer->GetMallMoney() + pItemUnit->dwToItemDur,
		//	0, 0, 0,
		//	0, 0, 0);
		pPlayer->SetMallMoney(pItemUnit->dwToItemDur, 2);

	}
	else if (pItemUnit->wToItemIdx == eChangeGuildExp)//帮会经验
	{
		DWORD GuildIdx = pPlayer->GetGuildIdx();
		if (0 == GuildIdx) return 0;

		CGuild* pGuild = GUILDMGR->GetGuild(GuildIdx);

		if (NULL == pGuild) return 0;

		LogGuildPoint(GuildIdx, pGuild->GetGuildPoint(), TRUE, eGuildUseChangeItem, pItemUnit->dwToItemDur, pPlayer->GetMaxLevel(), pPlayer->GetID());

		GuildAddGuildPoint(GuildIdx, pItemUnit->dwToItemDur, eGuildUseChangeItem, pPlayer->GetID());
	}

	else if (pItemUnit->wToItemIdx == ChangeToEvent1)
	{
		// event 050203
		MSG_NAME msg;
		msg.Category = MP_USERCONN;
		msg.Protocol = MP_USERCONN_EVENTITEM_USE;
		SafeStrCpy(msg.Name, pPlayer->GetObjectName(), MAX_NAME_LENGTH + 1);
		//		strcpy( msg.Name, pPlayer->GetObjectName() );
		pPlayer->SendMsg(&msg, sizeof(msg));
	}
	else if (pItemUnit->wToItemIdx == ChangeToEvent2)
	{
		MSG_NAME msg;
		msg.Category = MP_USERCONN;
		msg.Protocol = MP_USERCONN_EVENTITEM_USE2;
		SafeStrCpy(msg.Name, pPlayer->GetObjectName(), MAX_NAME_LENGTH + 1);
		pPlayer->SendMsg(&msg, sizeof(msg));
	}
	else
		return ITEMMGR->ObtainItemFromChangeItem(pPlayer, pItemUnit->wToItemIdx, (WORD)pItemUnit->dwToItemDur, 2, 0, 0, 0, 0);

	return 0;
}
int CChangeItemMgr::UseMultiChangeItemFromShopItem(CPlayer* pPlayer, WORD wItemIdx)
{
	CItemSlot* pSlot = pPlayer->GetSlot(eItemTable_Inventory);
	if (NULL == pSlot) ASSERTMSG(0, "棵官弗 酒捞袍 浇吩阑 掘阑 荐 绝嚼聪促.");

	unsigned int nNum = ITEMMGR->GetTotalEmptySlotNum(pSlot);

	sMULTICHANGEITEM* pMultiChangeItem = m_MultiChangeItemList.GetData(wItemIdx);

	unsigned int haveSpaceItemtotalcount = pMultiChangeItem->nMaxItemSpace;

	if ((nNum + 1) < haveSpaceItemtotalcount)
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
			if (pItemUnit->wToItemIdx == eChangeMoney)
			{
				pPlayer->SetMoney(pItemUnit->dwToItemDur, MONEY_ADDITION, MF_OBTAIN);
			}
			else if (pItemUnit->wToItemIdx == ChangeToSuryun)	// 荐访摹
			{
				pPlayer->AddAbilityExp(pItemUnit->dwToItemDur);
			}
			else
			{
				ITEMMGR->ObtainItemFromChangeItem(pPlayer, pItemUnit->wToItemIdx, (WORD)pItemUnit->dwToItemDur, 2, 0, 0, 0, 0);
			}
		}
	}

	return 0;
}
//////////////////////////////////////////////////////////////////////////
// [END] RaMa - 06.04.19  Shop酒捞袍栏肺 ChangeItem荤侩
//////////////////////////////////////////////////////////////////////////


/*
int CChangeItemMgr::UseChangeItem( CPlayer* pPlayer, WORD TargetPos, WORD wItemIdx )
{
	if( pPlayer->GetState() == eObjectState_Die )
		return 0;
	if( !CHKRT->ItemOf( pPlayer, TargetPos, wItemIdx, 0, 0, CB_EXIST|CB_ICONIDX) )
		return 0;
	if( pPlayer->GetState() == eObjectState_Immortal )
		OBJECTSTATEMGR_OBJ->EndObjectState( pPlayer, eObjectState_Immortal );

	CItemSlot* pSlot = pPlayer->GetSlot( TargetPos );
	const ITEMBASE* pItemBase = pSlot->GetItemInfoAbs( TargetPos );
	ITEMBASE Item = *pItemBase;
	ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo( wItemIdx );

	if( pItemInfo )
	if( pItemInfo->ItemKind == eCHANGE_ITEM )
	{
		sCHANGEITEMUNIT* pItemUnit = GetItemUnitFromCalPercent( wItemIdx );
		if( pItemUnit )
		{
			if( EI_TRUE != ITEMMGR->DiscardItem( pPlayer, TargetPos, wItemIdx, 1 ) )
				return 0;

			LogItemMoney(pPlayer->GetID(), pPlayer->GetObjectName(), 0, "",
			eLog_ItemDestroybyChangeItem, pPlayer->GetMoney(eItemTable_Inventory), 0, 0,
			Item.wIconIdx, Item.dwDBIdx, Item.Position, 0,
			Item.Durability, pPlayer->GetPlayerExpPoint());

			// send item use
			MSG_ITEM_USE_ACK msg;
			msg.Category = MP_ITEM;
			msg.Protocol = MP_ITEM_USE_ACK;
			msg.dwObjectID = pPlayer->GetID();
			msg.TargetPos = TargetPos;
			msg.wItemIdx = wItemIdx;
			ITEMMGR->SendAckMsg( pPlayer, &msg, sizeof(msg) );

			if( pItemUnit->wToItemIdx == 7999 )		// money
			{
				pPlayer->SetMoney( pItemUnit->wToItemDur, MONEY_ADDITION, MF_OBTAIN );
			}
			else if( pItemUnit->wToItemIdx == 7998 )
			{
//				// event 050106
//				MSGBASE msg;
//				msg.Category = MP_USERCONN;
//				msg.Protocol = MP_USERCONN_EVENTITEM_USE;
//				pPlayer->SendMsg( &msg, sizeof(msg) );
				// event 050203
				MSG_NAME msg;
				msg.Category = MP_USERCONN;
				msg.Protocol = MP_USERCONN_EVENTITEM_USE;
				strcpy( msg.Name, pPlayer->GetObjectName() );
				pPlayer->SendMsg( &msg, sizeof(msg) );
			}
			else
				ITEMMGR->ObtainItemFromChangeItem( pPlayer, pItemUnit->wToItemIdx, pItemUnit->wToItemDur );
		}
		else
			return 0;
	}
	else
		return 0;

	return 1;
}
*/


/*
// ChangeItemMgr.cpp: implementation of the CChangeItemMgr class.
CChangeItemMgr::CChangeItemMgr()
{

}

CChangeItemMgr::~CChangeItemMgr()
{
	Release();
}

void CChangeItemMgr::Init()
{
	m_ChangeItemList.Initialize( 300 );
	m_MultiChangeItemList.Initialize( 100 );
	LoadItemChange();
}

void CChangeItemMgr::Release()
{
	sCHANGEITEM* pData = NULL;
	m_ChangeItemList.SetPositionHead();
	while( pData = m_ChangeItemList.GetData() )
	{
		delete[] pData->pItemUnit;
		delete pData;
	}
	m_ChangeItemList.RemoveAll();

	sMULTICHANGEITEM* pData1 = NULL;
	m_MultiChangeItemList.SetPositionHead();
	while( pData1 = m_MultiChangeItemList.GetData() )
	{
		delete[] pData1->pSingleItemUnit;
		delete[] pData1->pMultiItemUnit;
		delete pData1;
	}
	m_MultiChangeItemList.RemoveAll();
}

BOOL CChangeItemMgr::LoadItemChange()
{
	CMHFile file;
#ifdef _FILE_BIN_
	if( !file.Init( "./Resource/ItemChangeList.bin", "rb" ) )
		return FALSE;
#else
	if( !file.Init( "./Resource/ItemChangeList.txt", "rt" ) )
		return FALSE;
#endif
	while( 1 )
	{
		if( file.IsEOF() )
			break;
		sCHANGEITEM* pData = new sCHANGEITEM;
		pData->wItemIdx = file.GetWord();
		pData->wMaxToItem = file.GetWord();
		pData->pItemUnit = new sCHANGEITEMUNIT[pData->wMaxToItem];
		for( WORD i = 0; i < pData->wMaxToItem; ++i )
		{
			pData->pItemUnit[i].wToItemIdx = file.GetWord();
			pData->pItemUnit[i].wToItemDur = file.GetWord();
			pData->pItemUnit[i].wPercent = file.GetWord();
		}

		m_ChangeItemList.Add( pData, pData->wItemIdx );
	}
	file.Release();

#ifdef _FILE_BIN_
	if( !file.Init( "./Resource/ItemChangeListMulti.bin", "rb" ) )
		return FALSE;
#else
	if( !file.Init( "./Resource/ItemChangeListMulti.txt", "rt" ) )
		return FALSE;
#endif
	while( 1 )
	{
		if( file.IsEOF() )
			break;
		sMULTICHANGEITEM* pData = new sMULTICHANGEITEM;
		pData->wItemIdx = file.GetWord();
		pData->wLimitLevel = file.GetWord();
		pData->wMaxSingle = file.GetWord();
		pData->wMaxMulti = file.GetWord();
		pData->pSingleItemUnit = new sCHANGEITEMUNIT[pData->wMaxSingle];
		pData->pMultiItemUnit = new sCHANGEITEMUNIT[pData->wMaxMulti];
		WORD s = 0;
		WORD m = 0;
		for( WORD i = 0; i < pData->wMaxSingle+pData->wMaxMulti; ++i )
		{
			sCHANGEITEMUNIT unit;
			unit.wToItemIdx = file.GetWord();
			unit.wToItemDur = file.GetWord();
			unit.wPercent = file.GetWord();
			if( unit.wPercent == 0 )
			{
				pData->pSingleItemUnit[s] = unit;
				++s;
			}
			else
			{
				pData->pMultiItemUnit[m] = unit;
				++m;
			}
		}

		m_MultiChangeItemList.Add( pData, pData->wItemIdx );
	}
	file.Release();

	return TRUE;
}

int CChangeItemMgr::UseChangeItem( CPlayer* pPlayer, WORD TargetPos, WORD wItemIdx )
{
	if( pPlayer->GetState() == eObjectState_Die )
		return 0;
	if( !CHKRT->ItemOf( pPlayer, TargetPos, wItemIdx, 0, 0, CB_EXIST|CB_ICONIDX) )
		return 0;
	if( pPlayer->GetState() == eObjectState_Immortal )
		OBJECTSTATEMGR_OBJ->EndObjectState( pPlayer, eObjectState_Immortal );

	CItemSlot* pSlot = pPlayer->GetSlot( TargetPos );
	const ITEMBASE* pItemBase = pSlot->GetItemInfoAbs( TargetPos );
	ITEMBASE Item = *pItemBase;
	ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo( wItemIdx );

	if( pItemInfo )
	if( pItemInfo->ItemKind == eCHANGE_ITEM )
	{
		if( m_MultiChangeItemList.GetData( wItemIdx ) )
		{
			return UseMultiChangeItem( pPlayer, TargetPos, wItemIdx, &Item );
		}
		else
		{
			return UseNormalChangeItem( pPlayer, TargetPos, wItemIdx, &Item );
		}
	}
	else
		return 0;

	return 1;
}

int CChangeItemMgr::UseNormalChangeItem( CPlayer* pPlayer, WORD TargetPos, WORD wItemIdx, ITEMBASE* pItem )
{
	sCHANGEITEMUNIT* pItemUnit = GetItemUnitFromCalPercent( wItemIdx );
	if( !pItemUnit )	return 0;

	if( EI_TRUE != ITEMMGR->DiscardItem( pPlayer, TargetPos, wItemIdx, 1 ) )
		return 0;
	LogItemMoney( pPlayer->GetID(), pPlayer->GetObjectName(), 0, "",
				  eLog_ItemDestroybyChangeItem, pPlayer->GetMoney(eItemTable_Inventory), 0, 0,
				  pItem->wIconIdx, pItem->dwDBIdx, pItem->Position, 0, pItem->Durability, pPlayer->GetPlayerExpPoint() );

	// send item use
	MSG_ITEM_USE_ACK msg;
	msg.Category = MP_ITEM;
	msg.Protocol = MP_ITEM_USE_ACK;
	msg.dwObjectID = pPlayer->GetID();
	msg.TargetPos = TargetPos;
	msg.wItemIdx = wItemIdx;
	ITEMMGR->SendAckMsg( pPlayer, &msg, sizeof(msg) );

	if( pItemUnit->wToItemIdx == 7999 )		// money
	{
		pPlayer->SetMoney( pItemUnit->wToItemDur, MONEY_ADDITION, MF_OBTAIN );
	}
	else if( pItemUnit->wToItemIdx == 7998 )
	{
		// event 050203
		MSG_NAME msg;
		msg.Category = MP_USERCONN;
		msg.Protocol = MP_USERCONN_EVENTITEM_USE;
		strcpy( msg.Name, pPlayer->GetObjectName() );
		pPlayer->SendMsg( &msg, sizeof(msg) );
	}
	else
		ITEMMGR->ObtainItemFromChangeItem( pPlayer, pItemUnit->wToItemIdx, pItemUnit->wToItemDur );

	return 1;
}

int CChangeItemMgr::UseMultiChangeItem( CPlayer* pPlayer, WORD TargetPos, WORD wItemIdx, ITEMBASE* pItem )
{
	sMULTICHANGEITEM* pMultiChangeItem = m_MultiChangeItemList.GetData( wItemIdx );

	if( pPlayer->GetMaxLevel() < pMultiChangeItem->wLimitLevel )	return 0;
	if( EI_TRUE != ITEMMGR->DiscardItem( pPlayer, TargetPos, wItemIdx, 1 ) )	return 0;
	// log
	LogItemMoney( pPlayer->GetID(), pPlayer->GetObjectName(), 0, "",
				  eLog_ItemDestroybyChangeItem, pPlayer->GetMoney(eItemTable_Inventory), 0, 0,
				  pItem->wIconIdx, pItem->dwDBIdx, pItem->Position, 0, pItem->Durability, pPlayer->GetPlayerExpPoint() );

	// send item use
	MSG_ITEM_USE_ACK msg;
	msg.Category = MP_ITEM;
	msg.Protocol = MP_ITEM_USE_ACK;
	msg.dwObjectID = pPlayer->GetID();
	msg.TargetPos = TargetPos;
	msg.wItemIdx = wItemIdx;
	ITEMMGR->SendAckMsg( pPlayer, &msg, sizeof(msg) );

	for( WORD i = 0; i < pMultiChangeItem->wMaxSingle; ++i )
	{
		if( pMultiChangeItem->pSingleItemUnit[i].wToItemIdx == 7999 )
		{
			pPlayer->SetMoney( pMultiChangeItem->pSingleItemUnit[i].wToItemDur, MONEY_ADDITION, MF_OBTAIN );
		}
		else
		{
			ITEMMGR->ObtainItemFromChangeItem( pPlayer, pMultiChangeItem->pSingleItemUnit[i].wToItemIdx, pMultiChangeItem->pSingleItemUnit[i].wToItemDur );
		}
	}

	sCHANGEITEMUNIT* pItemUnit = GetMultiItemUnitFromCalPercent( pMultiChangeItem );
	if( !pItemUnit )	return 0;

	ITEMMGR->ObtainItemFromChangeItem( pPlayer, pItemUnit->wToItemIdx, pItemUnit->wToItemDur );

	return 1;
}

sCHANGEITEMUNIT* CChangeItemMgr::GetItemUnitFromCalPercent( WORD wItemIdx )
{
	sCHANGEITEM* pData = m_ChangeItemList.GetData( wItemIdx );

	WORD RandRate = rand()%10001;
	WORD FromPercent = 0;
	WORD ToPercent = 0;

	for( WORD i = 0; i < pData->wMaxToItem; ++i )
	{
		FromPercent = ToPercent;
		ToPercent += pData->pItemUnit[i].wPercent;
		if( FromPercent <= RandRate && RandRate < ToPercent )
		{
			return &pData->pItemUnit[i];
		}
	}

	return NULL;
}

sCHANGEITEMUNIT* CChangeItemMgr::GetMultiItemUnitFromCalPercent( sMULTICHANGEITEM* pMultiChangeItem )
{
	WORD RandRate = rand()%10001;
	WORD FromPercent = 0;
	WORD ToPercent = 0;

	for( WORD i = 0; i < pMultiChangeItem->wMaxMulti; ++i )
	{
		FromPercent = ToPercent;
		ToPercent += pMultiChangeItem->pMultiItemUnit[i].wPercent;
		if( FromPercent <= RandRate && RandRate < ToPercent )
		{
			return &pMultiChangeItem->pMultiItemUnit[i];
		}
	}

	return NULL;
}

int CChangeItemMgr::UseChangeItem( CPlayer* pPlayer, WORD TargetPos, WORD wItemIdx )
{
	if( pPlayer->GetState() == eObjectState_Die )
		return 0;
	if( !CHKRT->ItemOf( pPlayer, TargetPos, wItemIdx, 0, 0, CB_EXIST|CB_ICONIDX) )
		return 0;
	if( pPlayer->GetState() == eObjectState_Immortal )
		OBJECTSTATEMGR_OBJ->EndObjectState( pPlayer, eObjectState_Immortal );

	CItemSlot* pSlot = pPlayer->GetSlot( TargetPos );
	const ITEMBASE* pItemBase = pSlot->GetItemInfoAbs( TargetPos );
	ITEMBASE Item = *pItemBase;
	ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo( wItemIdx );

	if( pItemInfo )
	if( pItemInfo->ItemKind == eCHANGE_ITEM )
	{
		sCHANGEITEMUNIT* pItemUnit = GetItemUnitFromCalPercent( wItemIdx );
		if( pItemUnit )
		{
			if( EI_TRUE != ITEMMGR->DiscardItem( pPlayer, TargetPos, wItemIdx, 1 ) )
				return 0;

			LogItemMoney(pPlayer->GetID(), pPlayer->GetObjectName(), 0, "",
			eLog_ItemDestroybyChangeItem, pPlayer->GetMoney(eItemTable_Inventory), 0, 0,
			Item.wIconIdx, Item.dwDBIdx, Item.Position, 0,
			Item.Durability, pPlayer->GetPlayerExpPoint());

			// send item use
			MSG_ITEM_USE_ACK msg;
			msg.Category = MP_ITEM;
			msg.Protocol = MP_ITEM_USE_ACK;
			msg.dwObjectID = pPlayer->GetID();
			msg.TargetPos = TargetPos;
			msg.wItemIdx = wItemIdx;
			ITEMMGR->SendAckMsg( pPlayer, &msg, sizeof(msg) );

			if( pItemUnit->wToItemIdx == 7999 )		// money
			{
				pPlayer->SetMoney( pItemUnit->wToItemDur, MONEY_ADDITION, MF_OBTAIN );
			}
			else if( pItemUnit->wToItemIdx == 7998 )
			{
//				// event 050106
//				MSGBASE msg;
//				msg.Category = MP_USERCONN;
//				msg.Protocol = MP_USERCONN_EVENTITEM_USE;
//				pPlayer->SendMsg( &msg, sizeof(msg) );
				// event 050203
				MSG_NAME msg;
				msg.Category = MP_USERCONN;
				msg.Protocol = MP_USERCONN_EVENTITEM_USE;
				strcpy( msg.Name, pPlayer->GetObjectName() );
				pPlayer->SendMsg( &msg, sizeof(msg) );
			}
			else
				ITEMMGR->ObtainItemFromChangeItem( pPlayer, pItemUnit->wToItemIdx, pItemUnit->wToItemDur );
		}
		else
			return 0;
	}
	else
		return 0;

	return 1;
}
*/
