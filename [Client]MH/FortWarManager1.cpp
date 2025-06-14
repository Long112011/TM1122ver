#include "stdafx.h"
#include "FortWarManager.h"
#include "ChatManager.h"
#include "ObjectManager.h"
#include "GameIn.h"
#include "MiniMapDlg.h"
#include "ObjectStateManager.h"
#include "FortWarDialog.h"
#include "MHMap.h"
#include "ItemManager.h"
#include "InventoryExDialog.h"
#include "cWindowManager.h"
#include "BloodStormIcon.h"
#include "cMsgBox.h"
#include "WindowIdEnum.h"
#include "MoveManager.h"
#include "autopath.h"
GLOBALTON(CFortWarManager)
CFortWarManager::CFortWarManager(void)
{
	m_nRelationFortWarIDX = 0;
	m_nTaxRate = 0;
	dw_MoveMapNum = 0;
	dw_MoveMapChannel = 0;
	m_WarStart = false;
	ZeroMemory(cNpcName, sizeof(cNpcName));
	ZeroMemory(m_MasterName, sizeof(m_MasterName));
}
CFortWarManager::~CFortWarManager(void)
{
}
void CFortWarManager::Init()
{
	m_nRelationFortWarIDX = 0;
	m_nTaxRate = 0;
	dw_MoveMapNum = 0;
	dw_MoveMapChannel = 0;
	ZeroMemory(cNpcName, sizeof(cNpcName));
	m_WarStart = false;
}
void CFortWarManager::AddSymbolNpc(float x, float z, DWORD dwStaticNpcIdx, DWORD dwStaticNpcStartId, char* pNpcName, DWORD dwRandomPick)
{
	CObject* pObject = OBJECTMGR->GetObject(STATICNPC_ID_START + 10000 + dwStaticNpcStartId);
	if (pObject)
		return;
	m_WarStart = true;
	NPC_TOTALINFO tinfo;
	BASEOBJECT_INFO binfo;
	BASEMOVE_INFO minfo;
	sprintf(cNpcName, "%s", pNpcName);
	binfo.dwObjectID = STATICNPC_ID_START + 10000 + dwStaticNpcStartId;
	binfo.ObjectState = 0;
	SafeStrCpy(binfo.ObjectName, cNpcName, MAX_NAME_LENGTH + 1);
	tinfo.MapNum = MAP->GetMapNum();
	tinfo.NpcKind = 17;
	tinfo.NpcUniqueIdx = dwStaticNpcIdx;
	tinfo.NpcJob = FORTWAR_SYMBOL_ROLE;
	minfo.bMoving = FALSE;
	minfo.KyungGongIdx = 0;
	minfo.MoveMode = 0;
	minfo.CurPosition.x = x;
	minfo.CurPosition.y = 0;
	minfo.CurPosition.z = z;
	CNpc* pNpc = OBJECTMGR->AddNpc(&binfo, &minfo, &tinfo);
	if (pNpc == NULL)
		return;
	float fDir = 0;
	pNpc->SetAngle(DEGTORAD(fDir));
	BYTE bTemp = dwStaticNpcStartId;
	//if (tinfo.NpcUniqueIdx == dwRandomPick && strcmp(HERO->GetObjectName(), m_MasterName) == 0)
	if (HERO->GetGuildIdx()!=0&&tinfo.NpcUniqueIdx == dwRandomPick && HERO->GetGuildIdx() == dwGuildIdx/*strcmp(HERO->GetObjectName(), m_MasterName) == 0*/)
		GAMEIN->GetMiniMapDialog()->AddStaticNpcIcon(pNpc, 2);
	else
		GAMEIN->GetMiniMapDialog()->AddStaticNpcIcon(pNpc, 1);
	TARGETSET set;
	set.pTarget = pNpc;
	EFFECTMGR->StartEffectProcess(eEffect_MonRegen, pNpc, &set, 0, pNpc->GetID());
}
void CFortWarManager::RemoveSymbolNpc(DWORD dwStaticNpcStartId)
{
	CObject* pObject = OBJECTMGR->GetObject(STATICNPC_ID_START + 10000 + dwStaticNpcStartId);
	if (pObject == NULL)
		return;
	OBJECTMGR->AddGarbageObject(pObject);
	GAMEIN->GetMiniMapDialog()->RemoveIcon(pObject->GetID());
	m_WarStart = false;
}
DWORD CFortWarManager::CalTexRateForSell(DWORD dwMoney)
{
	if (m_nRelationFortWarIDX == 0 ||
		m_nTaxRate == 0)
		return dwMoney;
	return (dwMoney - (DWORD)(((float)dwMoney * m_nTaxRate) / 10000));
}
DWORD CFortWarManager::CalTexRateForBuy(DWORD dwMoney)
{
	if (m_nRelationFortWarIDX == 0 ||
		m_nTaxRate == 0)
		return dwMoney;
	return (dwMoney + (DWORD)(((float)dwMoney * m_nTaxRate) / 10000));
}
void CFortWarManager::NetworkMsgParse(BYTE Protocol, void* pMsg)
{
	switch (Protocol)
	{
	case MP_FORTWAR_INFO:
	{
							MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
							m_nRelationFortWarIDX = pmsg->dwData1;
							m_nTaxRate = pmsg->dwData2;
							CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1745), m_nTaxRate / 100);
	}
	break;
	case MP_FORTWAR_START_BEFORE10MIN:
	{
										 MSG_FORTWAR_START* pmsg = (MSG_FORTWAR_START*)pMsg;
										 CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1747), MAP->GetMapName(pmsg->wForWarMapNum));
	}
	break;
	case MP_FORTWAR_START:
	{
		MSG_FORTWAR_START* pmsg = (MSG_FORTWAR_START*)pMsg;
		{
			if (MAP->GetMapNum() == pmsg->wForWarMapNum)
			if ((gChannelNum + 1) == pmsg->dwChannelNum)
			{
				if (GAMEIN->GetFWTimeDlg())
				{
					GAMEIN->GetFWTimeDlg()->SetActiveWithTimeName(TRUE, pmsg->dwFortWarTime, pmsg->EngraveSuccessPlayerName);
					SafeStrCpy(m_MasterName, pmsg->EngraveSuccessPlayerName, MAX_NAME_LENGTH + 1);
				}			

				/*for (int i = 0; i < 3; i++)
				{
					AddSymbolNpc(pmsg->vNpcPos[0+i].x, pmsg->vNpcPos[0+i].y, 15001+i, 0+i, pmsg->EngraveNpcName[0+i], pmsg->CyptKey);
				}*/
				AddSymbolNpc(pmsg->vNpcPos[0].x, pmsg->vNpcPos[0].y, 15001, 0, pmsg->EngraveNpcName[0], pmsg->CyptKey);
				AddSymbolNpc(pmsg->vNpcPos[1].x, pmsg->vNpcPos[1].y, 15002, 1, pmsg->EngraveNpcName[1], pmsg->CyptKey);
				AddSymbolNpc(pmsg->vNpcPos[2].x, pmsg->vNpcPos[2].y, 15003, 2, pmsg->EngraveNpcName[2], pmsg->CyptKey);
				dwGuildIdx = pmsg->dwGuildIdx;
			}
			dw_MoveMapNum = pmsg->wForWarMapNum;
			dw_MoveMapChannel = pmsg->dwChannelNum;
			
		}
		WINDOWMGR->MsgBox(MSBOX_JOIN_FORTWAR, MBT_YESNO, CHATMGR->GetChatMsg(1749), MAP->GetMapName(pmsg->wForWarMapNum));
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1749), MAP->GetMapName(pmsg->wForWarMapNum));
		GAMEIN->GetBloodStormIcon()->SetActive(true);
	}
	break;
	case MP_FORTWAR_NEWRANDOM:
	{
		MSG_FORTWAR_START* pmsg = (MSG_FORTWAR_START*)pMsg;
		/*for (int i = 0; i < 3; i++)
		{
			RemoveSymbolNpc(i);
		}*/

		RemoveSymbolNpc(0);
		RemoveSymbolNpc(1);
		RemoveSymbolNpc(2);

		
		//if (GAMEIN->GetFWTimeDlg())
		//{
		//	GAMEIN->GetFWTimeDlg()->SetActiveWithTimeName(TRUE, pmsg->dwFortWarTime, pmsg->EngraveSuccessPlayerName);
		//	SafeStrCpy(m_MasterName, pmsg->EngraveSuccessPlayerName, MAX_NAME_LENGTH + 1);
		//}

		/*for (int i = 0; i < 3; i++)
		{
			AddSymbolNpc(pmsg->vNpcPos[0 + i].x, pmsg->vNpcPos[0 + i].y, 15001 + i, 0, pmsg->EngraveNpcName[0 + i], pmsg->CyptKey);
		}*/
		AddSymbolNpc(pmsg->vNpcPos[0].x, pmsg->vNpcPos[0].y, 15001, 0, pmsg->EngraveNpcName[0], pmsg->CyptKey);
		AddSymbolNpc(pmsg->vNpcPos[1].x, pmsg->vNpcPos[1].y, 15002, 1, pmsg->EngraveNpcName[1], pmsg->CyptKey);
		AddSymbolNpc(pmsg->vNpcPos[2].x, pmsg->vNpcPos[2].y, 15003, 2, pmsg->EngraveNpcName[2], pmsg->CyptKey);

		dwGuildIdx = pmsg->dwGuildIdx;
		//CHATMGR->AddMsg(CTC_SYSMSG, "NEWRANDOM");
	}
	break;
	case MP_FORTWAR_ING:
	{
		MSG_FORTWAR_START* pmsg = (MSG_FORTWAR_START*)pMsg;
		if (GAMEIN->GetFWTimeDlg())
		{
			GAMEIN->GetFWTimeDlg()->SetActiveWithTimeName(TRUE, pmsg->dwFortWarTime, pmsg->EngraveSuccessPlayerName);
			SafeStrCpy(m_MasterName, pmsg->EngraveSuccessPlayerName, MAX_NAME_LENGTH + 1);
		}
		/*for (int i = 0; i < 3; i++)
		{
			AddSymbolNpc(pmsg->vNpcPos[0+i].x, pmsg->vNpcPos[0+i].y, 15001+i, 0, pmsg->EngraveNpcName[0+i], pmsg->CyptKey);
		}*/
		AddSymbolNpc(pmsg->vNpcPos[0].x, pmsg->vNpcPos[0].y, 15001, 0, pmsg->EngraveNpcName[0], pmsg->CyptKey);
		AddSymbolNpc(pmsg->vNpcPos[1].x, pmsg->vNpcPos[1].y, 15002, 1, pmsg->EngraveNpcName[1], pmsg->CyptKey);
		AddSymbolNpc(pmsg->vNpcPos[2].x, pmsg->vNpcPos[2].y, 15003, 2, pmsg->EngraveNpcName[2], pmsg->CyptKey);

		dwGuildIdx = pmsg->dwGuildIdx;

		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1750));
	}
	break;
	case MP_FORTWAR_END:
	{
						   MSG_FORTWAR_END* pmsg = (MSG_FORTWAR_END*)pMsg;
						   if (GAMEIN->GetFWTimeDlg())
						   {
							   GAMEIN->GetFWTimeDlg()->SetActive(FALSE);
						   }
						   /*for (int i = 0; i < 3; i++)
						   {
							   RemoveSymbolNpc(i);
						   }*/
						   RemoveSymbolNpc(0);
							RemoveSymbolNpc(1);
							RemoveSymbolNpc(2);
						   CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1751), MAP->GetMapName(pmsg->wForWarMapNum));
						   GAMEIN->GetBloodStormIcon()->SetActive(false);
	}
	break;
	case MP_FORTWAR_ENGRAVE_START_ACK:
	{
										 MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
										 CPlayer* pPlayer = (CPlayer*)OBJECTMGR->GetObject(pmsg->dwData1);
										 if (!pPlayer)	return;
										 if (pPlayer->GetID() == HEROID)
										 {
											 if (GAMEIN->GetFWEngraveDlg())
												 GAMEIN->GetFWEngraveDlg()->SetActiveWithTime(TRUE, pmsg->dwData2);
										 }
										 if (pPlayer->GetState() == eObjectState_Deal)
											 OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);
										 OBJECTSTATEMGR->StartObjectState(pPlayer, eObjectState_Engrave);
										 OBJECTEFFECTDESC desc(FindEffectNum("eff_gakin_s.beff"));
										 pPlayer->AddObjectEffect(ENGRAVE_EFFECTID, &desc, 1, HERO);
	}
	break;
	case MP_FORTWAR_ENGRAVE_START_NACK:
	{
		MSG_NAME_DWORD* pmsg1 = (MSG_NAME_DWORD*)pMsg;
		CPlayer* pPlayer = (CPlayer*)OBJECTMGR->GetObject(pmsg1->dwData);
		if (pPlayer != NULL)
		{
			if (pPlayer == HERO)
			{
				if (GAMEIN->GetFWEngraveDlg())
					GAMEIN->GetFWEngraveDlg()->SetActive(FALSE);
			}
			pPlayer->RemoveObjectEffect(ENGRAVE_EFFECTID);
			if (OBJECTSTATEMGR->GetObjectState(pPlayer) == eObjectState_Engrave)
				OBJECTSTATEMGR->EndObjectState(pPlayer, eObjectState_Engrave);
			pPlayer->RemoveObjectEffect(ENGRAVE_ACK_EFFECTID);
			OBJECTEFFECTDESC desc(FindEffectNum("eff_gakin_fail.beff"));
			pPlayer->AddObjectEffect(ENGRAVE_ACK_EFFECTID, &desc, 1, HERO);
		}
		MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
		if (HERO->GetState() == eObjectState_Deal)
			OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);
		if (HERO->GetState() == eObjectState_Engrave)
			OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Engrave);
										  
		//if (pPlayer == HERO)
		{
			if (GAMEIN->GetFWEngraveDlg())
				GAMEIN->GetFWEngraveDlg()->SetActive(FALSE);
		}

		switch (pmsg->dwData)
		{
		case eFortWarEngraveError_None:				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(2648));	break;
		case eFortWarEngraveError_NotMap:			CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(2649));	break;
		case eFortWarEngraveError_NotIng:			CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(2650));	break;
		case eFortWarEngraveError_AlreadyPlayer:	CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(2651));	break;
		case eFortWarEngraveError_NotGuildMaster:	CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(2652));	break;
		case eFortWarEngraveError_AlreadyMaster:	CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(2653));	break;
		case 6:	CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(2685));	break;
		}
	}
	break;
	case MP_FORTWAR_ENGRAVE_CANCEL_ACK:
	{
										  MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
										  CPlayer* pPlayer = (CPlayer*)OBJECTMGR->GetObject(pmsg->dwData);
										  if (!pPlayer)	return;
										  if (pPlayer == HERO)
										  {
											  if (GAMEIN->GetFWEngraveDlg())
												  GAMEIN->GetFWEngraveDlg()->SetActive(FALSE);
										  }
										  pPlayer->RemoveObjectEffect(ENGRAVE_EFFECTID);
										  if (pPlayer->GetState() == eObjectState_Engrave)
											  OBJECTSTATEMGR->EndObjectState(pPlayer, eObjectState_Engrave);
										  pPlayer->RemoveObjectEffect(ENGRAVE_NACK_EFFECTID);
										  OBJECTEFFECTDESC desc(FindEffectNum("eff_gakin_fail.beff"));
										  pPlayer->AddObjectEffect(ENGRAVE_NACK_EFFECTID, &desc, 1, HERO);
	}
	break;
	case MP_FORTWAR_ENGRAVE_CANCEL_NACK:
	{
	}
	break;
	case MP_FORTWAR_ENGRAVE_SUCESS:
	{
		MSG_NAME_DWORD* pmsg = (MSG_NAME_DWORD*)pMsg;
		CPlayer* pPlayer = (CPlayer*)OBJECTMGR->GetObject(pmsg->dwData);
		if (pPlayer != NULL)
		{
			if (pPlayer == HERO)
			{
				if (GAMEIN->GetFWEngraveDlg())
					GAMEIN->GetFWEngraveDlg()->SetActive(FALSE);
			}
			pPlayer->RemoveObjectEffect(ENGRAVE_EFFECTID);
			if (OBJECTSTATEMGR->GetObjectState(pPlayer) == eObjectState_Engrave)
				OBJECTSTATEMGR->EndObjectState(pPlayer, eObjectState_Engrave);
			pPlayer->RemoveObjectEffect(ENGRAVE_ACK_EFFECTID);
			OBJECTEFFECTDESC desc(FindEffectNum("eff_gakin_suc.beff"));
			pPlayer->AddObjectEffect(ENGRAVE_ACK_EFFECTID, &desc, 1, HERO);
		}
		if (GAMEIN->GetFWTimeDlg())
			GAMEIN->GetFWTimeDlg()->SetCharacterName(pmsg->Name);
		SafeStrCpy(m_MasterName, pmsg->Name, MAX_NAME_LENGTH + 1);

		dwGuildIdx = pmsg->CyptKey;//KIV

		CNpc* pNpc = (CNpc*)OBJECTMGR->GetObject(dwStoredNpcID);
		if (pNpc)
		{
			GAMEIN->GetMiniMapDialog()->RemoveIcon(dwStoredNpcID);
			//if (strcmp(HERO->GetObjectName(), pmsg->Name) == 0)//kiv
			if (HERO->GetGuildIdx() !=0&&HERO->GetGuildIdx() == dwGuildIdx)
				GAMEIN->GetMiniMapDialog()->AddStaticNpcIcon(pNpc, 2);
			else
				GAMEIN->GetMiniMapDialog()->AddStaticNpcIcon(pNpc, 1);
		}
		AUTOPATH->SetTargetPos(HERO->GetCurPosition());
		MOVEMGR->HeroMoveStop();
	}
	break;
	case MP_FORTWAR_ENGRAVE_FAIL:
	{
		MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
		CPlayer* pPlayer = (CPlayer*)OBJECTMGR->GetObject(pmsg->dwData);
		if (!pPlayer)	return;
		if (pPlayer == HERO)
		{
			if (GAMEIN->GetFWEngraveDlg())
				GAMEIN->GetFWEngraveDlg()->SetActive(FALSE);
		}
		pPlayer->RemoveObjectEffect(ENGRAVE_EFFECTID);
		if (OBJECTSTATEMGR->GetObjectState(pPlayer) == eObjectState_Engrave)
			OBJECTSTATEMGR->EndObjectState(pPlayer, eObjectState_Engrave);
		pPlayer->RemoveObjectEffect(ENGRAVE_ACK_EFFECTID);
		OBJECTEFFECTDESC desc(FindEffectNum("eff_gakin_fail.beff"));
		pPlayer->AddObjectEffect(ENGRAVE_ACK_EFFECTID, &desc, 1, HERO);
	}
	break;
	case MP_FORTWAR_WAREHOUSE_INFO_ACK:
	{
		MSG_FORTWAR_WAREHOUSE_INFO* pmsg = (MSG_FORTWAR_WAREHOUSE_INFO*)pMsg;
		GAMEIN->GetFWWareHouseDlg()->SetFortWarWareHouseInfo(pmsg, eFortWareHouseDlg);
		GAMEIN->GetFWWareHouseDlg()->SetActive(TRUE);
	}
	break;
	case MP_FORTWAR_WAREHOUSE_INFO_NACK:
	{
	}
	break;
	case MP_FORTWAR_SIEGEWAREHOUSE_INFO_ACK:
	{
		MSG_FORTWAR_WAREHOUSE_INFO* pmsg = (MSG_FORTWAR_WAREHOUSE_INFO*)pMsg;
		GAMEIN->GetFWWareHouseDlg()->SetFortWarWareHouseInfo(pmsg, eSeigeWareHouseDlg);
		GAMEIN->GetFWWareHouseDlg()->SetActive(TRUE);
	}
	break;
	case MP_FORTWAR_SIEGEWAREHOUSE_INFO_NACK:
	{
	}
	break;
	case MP_FORTWAR_WAREHOUSE_ITEM_MOVE_ACK:
	{
		MSG_ITEM_MOVE_ACK* pmsg = (MSG_ITEM_MOVE_ACK*)pMsg;
		CFWWareHouseDialog* pFortWarWareHouseDlg = GAMEIN->GetFWWareHouseDlg();
		CInventoryExDialog* pInvenDlg = GAMEIN->GetInventoryDialog();
		CItem* FromItem = NULL;
		pFortWarWareHouseDlg->DeleteItem(pmsg->FromPos, &FromItem);
		FromItem->SetPosition(pmsg->ToPos);
		pInvenDlg->AddItem(FromItem);
		ITEMMGR->SetDisableDialog(FALSE, eItemTable_Inventory);
		pFortWarWareHouseDlg->SetDisable(FALSE);
	}
	break;
	case MP_FORTWAR_WAREHOUSE_ITEM_MOVE_NACK:
	{
		MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
		switch (pmsg->dwData)
		{
		case eFortWarWareError_None:		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1753));	break;
		case eFortWarWareError_NotMap:		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1754));	break;
		case eFortWarWareError_NotTime:		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1755));	break;
		case eFortWarWareError_NotRight:	CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1756));	break;
		case eFortWarWareError_NotUse:		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1753));	break;
		}
		WINDOWMGR->BackDragWindow();
		ITEMMGR->SetDisableDialog(FALSE, eItemTable_Inventory);
		GAMEIN->GetFWWareHouseDlg()->SetDisable(FALSE);
	}
	break;
	case MP_FORTWAR_WAREHOUSE_ITEM_MOVE_TO_SIEGEHOUSE_SYN:
	{
		MSG_ITEM_MOVE_ACK* pmsg = (MSG_ITEM_MOVE_ACK*)pMsg;
		CFWWareHouseDialog* pFortWarWareHouseDlg = GAMEIN->GetFWWareHouseDlg();
		CItem* pFromItem = NULL;
		pFortWarWareHouseDlg->DeleteItem(pmsg->FromPos, &pFromItem);
		ITEMMGR->ItemDelete(pFromItem);
		ITEMMGR->SetDisableDialog(FALSE, eItemTable_Inventory);
		pFortWarWareHouseDlg->SetDisable(FALSE);
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1757));
	}
	break;
	case MP_FORTWAR_SIEGEWAREHOUSE_ITEM_MOVE_ACK:
	{
		MSG_ITEM_MOVE_ACK* pmsg = (MSG_ITEM_MOVE_ACK*)pMsg;
		CFWWareHouseDialog* pFortWarWareHouseDlg = GAMEIN->GetFWWareHouseDlg();
		CInventoryExDialog* pInvenDlg = GAMEIN->GetInventoryDialog();
		CItem* FromItem = NULL;
		pFortWarWareHouseDlg->DeleteItem(pmsg->FromPos, &FromItem);
		FromItem->SetPosition(pmsg->ToPos);
		pInvenDlg->AddItem(FromItem);
		ITEMMGR->SetDisableDialog(FALSE, eItemTable_Inventory);
		pFortWarWareHouseDlg->SetDisable(FALSE);
	}
	break;
	case MP_FORTWAR_SIEGEWAREHOUSE_ITEM_MOVE_NACK:
	{
													 MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
													 switch (pmsg->dwData)
													 {
													 case eFortWarWareError_None:		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1753));	break;
													 case eFortWarWareError_NotMap:		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1754));	break;
													 case eFortWarWareError_NotTime:		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1755));	break;
													 case eFortWarWareError_NotRight:	CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1756));	break;
													 case eFortWarWareError_NotUse:		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1753));	break;
													 }
													 WINDOWMGR->BackDragWindow();
													 ITEMMGR->SetDisableDialog(FALSE, eItemTable_Inventory);
													 GAMEIN->GetFWWareHouseDlg()->SetDisable(FALSE);
	}
	break;
	case MP_FORTWAR_WAREHOUSE_ITEM_INSERT:
	{
											 MSG_FORTWAR_WAREITEM_INFO* pmsg = (MSG_FORTWAR_WAREITEM_INFO*)pMsg;
											 CFWWareHouseDialog* pFortWarWareHouseDlg = GAMEIN->GetFWWareHouseDlg();
											 if (pFortWarWareHouseDlg == NULL)
												 return;
											 if (pFortWarWareHouseDlg->IsActive())
											 {
												 if (pFortWarWareHouseDlg->GetDialogKind() == eFortWareHouseDlg)
													 pFortWarWareHouseDlg->AddItem(&pmsg->ItemInfo);
											 }
	}
	break;
	case MP_FORTWAR_WAREHOUSE_ITEM_MOVE_TO_INVEN:
	{
													MSG_FORTWAR_WAREITEM_INFO* pmsg = (MSG_FORTWAR_WAREITEM_INFO*)pMsg;
													CFWWareHouseDialog* pFortWarWareHouseDlg = GAMEIN->GetFWWareHouseDlg();
													if (pFortWarWareHouseDlg == NULL)
														return;
													if (pFortWarWareHouseDlg->IsActive())
													{
														if (pFortWarWareHouseDlg->GetDialogKind() == eFortWareHouseDlg)
														{
															CItem* pFromItem = NULL;
															pFortWarWareHouseDlg->DeleteItem(pmsg->wFromPos, &pFromItem);
															ITEMMGR->ItemDelete(pFromItem);
														}
													}
	}
	break;
	case MP_FORTWAR_WAREHOUSE_ITEM_MOVE_TO_SIEGEHOUSE:
	{
														 MSG_FORTWAR_WAREITEM_INFO* pmsg = (MSG_FORTWAR_WAREITEM_INFO*)pMsg;
														 CFWWareHouseDialog* pFortWarWareHouseDlg = GAMEIN->GetFWWareHouseDlg();
														 if (pFortWarWareHouseDlg == NULL)
															 return;
														 if (pFortWarWareHouseDlg->IsActive())
														 {
															 if (pFortWarWareHouseDlg->GetDialogKind() == eFortWareHouseDlg)
															 {
																 CItem* pFromItem = NULL;
																 pFortWarWareHouseDlg->DeleteItem(pmsg->wFromPos, &pFromItem);
																 ITEMMGR->ItemDelete(pFromItem);
															 }
															 else if (pFortWarWareHouseDlg->GetDialogKind() == eSeigeWareHouseDlg)
															 {
																 pFortWarWareHouseDlg->AddItem(&pmsg->ItemInfo);
															 }
														 }
	}
	break;
	case MP_FORTWAR_SIEGEWAREHOUSE_ITEM_MOVE_TO_INVEN:
	{
														 MSG_FORTWAR_WAREITEM_INFO* pmsg = (MSG_FORTWAR_WAREITEM_INFO*)pMsg;
														 CFWWareHouseDialog* pFortWarWareHouseDlg = GAMEIN->GetFWWareHouseDlg();
														 if (pFortWarWareHouseDlg == NULL)
															 return;
														 if (pFortWarWareHouseDlg->IsActive())
														 {
															 if (pFortWarWareHouseDlg->GetDialogKind() == eSeigeWareHouseDlg)
															 {
																 CItem* pFromItem = NULL;
																 pFortWarWareHouseDlg->DeleteItem(pmsg->wFromPos, &pFromItem);
																 ITEMMGR->ItemDelete(pFromItem);
															 }
														 }
	}
	break;
	case MP_FORTWAR_WAREHOUSE_PUTOUT_MONEY_ACK:
	{
												  MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
												  CFWWareHouseDialog* pFortWarWareHouseDlg = GAMEIN->GetFWWareHouseDlg();
												  if (pFortWarWareHouseDlg == NULL)
													  return;
												  if (pFortWarWareHouseDlg->IsActive())
												  {
													  if (pFortWarWareHouseDlg->GetDialogKind() == eFortWareHouseDlg)
													  {
														  pFortWarWareHouseDlg->SetMoney(pmsg->dwData);
													  }
												  }
	}
	break;
	case MP_FORTWAR_WAREHOUSE_PUTOUT_MONEY_NACK:
	{
												   MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
												   switch (pmsg->dwData)
												   {
												   case eFortWarWareError_None:		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1753));	break;
												   case eFortWarWareError_NotMap:		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1754));	break;
												   case eFortWarWareError_NotTime:		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1755));	break;
												   case eFortWarWareError_NotRight:	CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1756));	break;
												   case eFortWarWareError_NotUse:		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1753));	break;
												   }
	}
	break;
	case MP_FORTWAR_DROPITEM_TO_FORTWAREHOUSE:
	{
												 MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
												 ITEM_INFO* pInfo = ITEMMGR->GetItemInfo(pmsg->dwData);
												 if (pInfo)
												 {
													 CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1760), pInfo->ItemName);
												 }
	}
	break;
	}
}
