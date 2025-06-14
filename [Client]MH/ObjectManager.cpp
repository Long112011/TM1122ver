#include "stdafx.h"
#include "ObjectManager.h"
#include "Hero.h"
#include "Object.h"
#include "AppearanceManager.h"
#include "MoveManager.h"
#include "ItemManager.h"
#include "MugongManager.h"
#include "StatsCalcManager.h"
#include "ObjectStateManager.h"
#include "PeaceWarModeManager.h"
#include "PartyManager.h"
#include "Effect\EffectManager.h"
#include "AbilityManager.h"
#include "PetManager.h"
#include "TitanManager.h"
#include "ChatManager.h"
#include "SkillArea.h"
#include "SkillObject_Client.h"
#include "GameIn.h"
#include "ObjectBalloon.h"
#include "MonsterGuageDlg.h"
#include "WantedManager.h"
#include "GuildFieldWar.h"
#include "BattleSystem_Client.h"
#include "..\[CC]BattleSystem\SiegeWar\Battle_SiegeWar.h"
#include "CheatMsgParser.h"
#include "MainGame.h"
#include "MHCamera.h"
#include "SkillManager_client.h"
#include "StatusIconDlg.h"
#include "SiegeWarMgr.h"
#include "PathManager.h"
#include "BillBoardManager.h"
DWORD gHeroID = 0;
DWORD gUserID = 0;
GLOBALTON(CObjectManager);
CObjectManager::CObjectManager()
{
	m_pHero = NULL;
	m_pHeroPet = NULL;
	m_pCharCalcMgr = CHARCALCMGR;
	m_ObjectTable.Initialize(1024);
	m_PlayerPool.Init(MAX_PLAYEROBJECT_NUM, 50, "CPlayer");
	m_MonsterPool.Init(MAX_MONSTEROBJECT_NUM, 50, "CMonster");
	m_NpcPool.Init(MAX_NPCOBJECT_NUM, 50, "CNpc");
	m_BossMonsterPool.Init(MAX_MONSTEROBJECT_NUM, 50, "CBossMonster");
	m_TacticObjectPool.Init(MAX_TACTICOBJECT_NUM, 50, "CTactic");
	m_PetPool.Init(MAX_PETOBJECT_NUM, 50, "Pet");
	m_MapObjectPool.Init(MAX_MAPOBJECT_NUM, 10, "CMapObject");
	m_SelectedObjectID = 0;
	m_pDPObjectName = NULL;
	m_dwOverInfoOption = eOO_NAME_CLICKEDONLY;
	m_bAllNameShow = FALSE;
	for (int i = 0; i < eBOSSTYPE_MAX; ++i)
		m_pBoss[i] = NULL;
	m_dwEffectOption = eOO_EFFECT_DEFAULT;
	m_pCurGTFlg = NULL;
	m_pCurSGFlg = NULL;
	m_dwWeekData = 0;
}
CObjectManager::~CObjectManager()
{
}
void CObjectManager::AddShadow(CObject* pObject)
{
	if (pObject->GetObjectKind() == eObjectKind_Player)
	{
		if (((CPlayer*)pObject)->m_CharacterInfo.bVisible == FALSE)
		{
			RemoveShadow(pObject);
			return;
		}
	}
	BYTE bShadow = GAMERESRCMNGR->m_GameDesc.bShadow;
	if (bShadow == MHSHADOW_CIRCLE || bShadow == MHSHADOW_DETAIL)
	{
		if (pObject == HERO && bShadow == MHSHADOW_DETAIL)
			pObject->GetEngineObject()->EnableShadow(TRUE);
		else if (pObject->GetSingleSpecialState(eSingleSpecialState_Hide))
		{
			pObject->GetEngineObject()->EnableShadow(FALSE);
			pObject->m_ShadowObj.Init("hide.mod", NULL, eEngineObjectType_Shadow, 0, -1);
			pObject->m_ShadowObj.ApplyHeightField(TRUE);
		}
		else
		{
			pObject->GetEngineObject()->EnableShadow(FALSE);
			pObject->m_ShadowObj.Init("sh.mod", NULL, eEngineObjectType_Shadow, 0, -1);
			pObject->m_ShadowObj.ApplyHeightField(TRUE);
		}
	}
	else
	{
		pObject->GetEngineObject()->EnableShadow(FALSE);
	}
}
void CObjectManager::RemoveShadow(CObject* pObject)
{
	if (pObject->m_ShadowObj.IsInited())
	{
		pObject->m_ShadowObj.Release();
	}
	pObject->GetEngineObject()->EnableShadow(FALSE);
}
void CObjectManager::RegisterHero(SEND_HERO_TOTALINFO * sendHeroTotalInfo)
{
	m_pHero = new CHero;
	m_pHero->Init(eObjectKind_Player, &sendHeroTotalInfo->BaseObjectInfo);
	m_pHero->InitPlayer(&sendHeroTotalInfo->ChrTotalInfo);
	m_pHero->InitHero(&sendHeroTotalInfo->HeroTotalInfo);
	gHeroID = m_pHero->GetID();
	m_pHero->InitObjectBalloon(ObjectBalloon_Name | ObjectBalloon_Chat | ObjectBalloon_Title | ObjectBalloon_MunpaMark);
	if (m_pHero->IsMarry())
	{
		CHAR balloon[MAX_MASTERNAME_LENGTH + 1];
		if (m_pHero->GetGender() == 0)
		{
			sprintf(balloon, CHATMGR->GetChatMsg(2011), m_pHero->GetMarryName());
		}
		else
		{
			sprintf(balloon, CHATMGR->GetChatMsg(2012), m_pHero->GetMarryName());
		}
		m_pHero->SetMarryNameBalloon(balloon);
	}
	if (m_pHero->HasShiFu())
	{
		CHAR balloon[MAX_MASTERNAME_LENGTH + 1];
		sprintf(balloon, CHATMGR->GetChatMsg(2089), m_pHero->GetShiFuName());
		m_pHero->SetShiTuNameBalloon(balloon);
	}
	m_pHero->SetShopItemOptionInfo(&sendHeroTotalInfo->ShopItemOption);
	m_pHero->CalcAvatarOption();
	ABILITYMGR->InitAbility(m_pHero, &sendHeroTotalInfo->AbilityInfo, m_pHero->GetAbilityGroup());
	ABILITYMGR->InitAbilityExp(m_pHero->GetAbilityGroup(), sendHeroTotalInfo->HeroTotalInfo.AbilityExp);
	PEACEWARMGR->InitPeace_WarMode(m_pHero);
	APPEARANCEMGR->InitAppearance(m_pHero);
	BASEMOVE_INFO moveInfo;
	moveInfo.SetFrom(&sendHeroTotalInfo->SendMoveInfo);
	MOVEMGR->InitMove(m_pHero, &moveInfo);
	OBJECTSTATEMGR->InitObjectState(m_pHero);
	sBOARDMGR->SetHeroTopListFlg(m_pHero);
	CAddableInfoIterator iter(&sendHeroTotalInfo->AddableInfo);
	BYTE AddInfoKind;
	while ((AddInfoKind = iter.GetInfoKind()) != CAddableInfoList::None)
	{
		switch (AddInfoKind)
		{
		case (CAddableInfoList::ItemOption) :
		{
												ITEM_OPTION_INFO OptionInfo[MAX_ITEM_OPTION_NUM + SLOT_PYOGUK_NUM];
												iter.GetInfoData(&OptionInfo);
												ITEMMGR->InitItemOption(OptionInfo, sendHeroTotalInfo->OptionNum);
		}
											break;
		case (CAddableInfoList::ItemRareOption) :
		{
													ITEM_RARE_OPTION_INFO RareOptionInfo[MAX_ITEM_OPTION_NUM + SLOT_PYOGUK_NUM];
													iter.GetInfoData(&RareOptionInfo);
													ITEMMGR->InitItemRareOption(RareOptionInfo, sendHeroTotalInfo->RareOptionNum);
		}
												break;
		case (CAddableInfoList::ItemStoneOption) :
		{
													 ITEM_STONE_OPTION_INFO StoneOptionInfo[MAX_ITEM_OPTION_NUM + SLOT_PYOGUK_NUM];
													 iter.GetInfoData(&StoneOptionInfo);
													 ITEMMGR->InitItemStoneOption(StoneOptionInfo, sendHeroTotalInfo->StoneOptionNum);
		}
												 break;
		case (CAddableInfoList::PetTotalInfo) :
		{
												  PET_TOTALINFO PetInfo[SLOT_INVENTORY_NUM + SLOT_SHOPINVEN_NUM];
												  iter.GetInfoData(PetInfo);
												  PETMGR->InitPetInfo(PetInfo, sendHeroTotalInfo->PetNum);
		}
											  break;
		case (CAddableInfoList::TitanEndrncInfo) :
		{
													 TITAN_ENDURANCE_ITEMINFO TitanEnduranceInfo[SLOT_INVENTORY_NUM + SLOT_SHOPINVEN_NUM];
													 iter.GetInfoData(TitanEnduranceInfo);
													 TITANMGR->InitTitanEquipItemEnduranceInfo(TitanEnduranceInfo, sendHeroTotalInfo->TitanEndrncNum);
		}
												 break;
		case (CAddableInfoList::TitanTotalInfo) :
		{
													TITAN_TOTALINFO TitanInfo[SLOT_INVENTORY_NUM];
													iter.GetInfoData(TitanInfo);
													TITANMGR->InitTitanInfo(TitanInfo, sendHeroTotalInfo->TitanNum);
		}
												break;
		}
		iter.ShiftToNextData();
	}
	ITEMMGR->NetworkMsgParse(MP_ITEM_TOTALINFO_LOCAL, &sendHeroTotalInfo->ItemTotalInfo);
	MUGONGMGR->NetworkMsgParse(MP_MUGONG_TOTALINFO_LOCAL, &sendHeroTotalInfo->MugongTotalInfo);
	TITANMGR->SetTitanPaperDelayTime();
	TITANMGR->SetTitanTotalEnduranceView();
	STATSMGR->CalcTitanStats(TITANMGR->GetRegistedTitanItemDBIdx());
	m_ObjectTable.Add(m_pHero, m_pHero->GetID());
	m_pHero->ApplyInterface();
	ABILITYMGR->UpdateAbilityState(ABILITYUPDATE_ABILITYLEVEL_CHANGED, 0, m_pHero->GetAbilityGroup());
	ApplyOverInfoOption(m_pHero);
	ApplyShadowOption(m_pHero);
	m_pHero->GetEngineObject()->DisablePick();
	STATSMGR->CalcItemStats(HERO);
	return;
}
CPlayer* CObjectManager::AddPlayerThroughCache(BASEOBJECT_INFO* pBaseObjectInfo, BASEMOVE_INFO* pMoveInfo, CHARACTER_TOTALINFO* pTotalInfo, SHOPITEMOPTION* pShopItemOption, SEND_CHARACTER_TOTALINFO* pMsg)
{
	CObject* pPreObj = GetObject(pBaseObjectInfo->dwObjectID);
	ASSERT(pPreObj == NULL);
	if (pPreObj)
	{
		RemoveObject(&pPreObj);
	}
	CPlayer* pPlayer = m_PlayerPool.Alloc();
	pPlayer->Init(eObjectKind_Player, pBaseObjectInfo);
	pPlayer->InitPlayer(pTotalInfo);
	pPlayer->InitObjectBalloon(ObjectBalloon_Name | ObjectBalloon_Chat | ObjectBalloon_Title | ObjectBalloon_MunpaMark);
	pPlayer->SetShopItemOptionInfo(pShopItemOption);
	pPlayer->CalcAvatarOption();
	PEACEWARMGR->InitPeace_WarMode(pPlayer);
	APPEARANCEMGR->ReserveToAppearObject(pPlayer, (void*)pMsg);
	MOVEMGR->InitMove(pPlayer, pMoveInfo);
	m_ObjectTable.Add(pPlayer, pPlayer->GetID());


	return pPlayer;
}
CPlayer* CObjectManager::AddPlayer(BASEOBJECT_INFO* pBaseObjectInfo, BASEMOVE_INFO* pMoveInfo, CHARACTER_TOTALINFO* pTotalInfo, SHOPITEMOPTION* pShopItemOption, TITAN_APPEARANCEINFO* pTitanAppearanceInfo, BOOL bInTitan)
{
	CObject* pPreObj = GetObject(pBaseObjectInfo->dwObjectID);
	ASSERT(pPreObj == NULL);
	if (pPreObj)
	{
		RemoveObject(&pPreObj);
	}
	CPlayer* pPlayer = m_PlayerPool.Alloc();
	pPlayer->Init(eObjectKind_Player, pBaseObjectInfo);
	pPlayer->InitPlayer(pTotalInfo);
	pPlayer->InitObjectBalloon(ObjectBalloon_Name | ObjectBalloon_Chat | ObjectBalloon_Title | ObjectBalloon_MunpaMark);
	pPlayer->SetShopItemOptionInfo(pShopItemOption);
	pPlayer->CalcAvatarOption();
	pPlayer->RidingTitan(bInTitan);
	pPlayer->SetTitanAppearanceInfo(pTitanAppearanceInfo);
	pPlayer->SetFullMoonEventIndex(eFULLMOONEVENT_NONE);
	PEACEWARMGR->InitPeace_WarMode(pPlayer);
	APPEARANCEMGR->InitAppearance(pPlayer);
	MOVEMGR->InitMove(pPlayer, pMoveInfo);
	OBJECTSTATEMGR->InitObjectState(pPlayer);
	m_ObjectTable.Add(pPlayer, pPlayer->GetID());

	ApplyOverInfoOption(pPlayer);
	ApplyShadowOption(pPlayer);
	return pPlayer;
}
CMonster* CObjectManager::AddMonster(BASEOBJECT_INFO* pBaseObjectInfo, BASEMOVE_INFO* pMoveInfo, MONSTER_TOTALINFO* pTotalInfo)
{
	CObject* pPreObj = GetObject(pBaseObjectInfo->dwObjectID);
	ASSERT(pPreObj == NULL);
	if (pPreObj)
	{
		RemoveObject(&pPreObj);
	}
	ASSERT(pTotalInfo->MonsterKind != 0);
	CMonster* pMonster = m_MonsterPool.Alloc();
	WORD MonsterKind = GAMERESRCMNGR->GetMonsterListInfo(pTotalInfo->MonsterKind)->ObjectKind;
	BOOL IsFieldBoss = false;
	BOOL IsSpecialMonster = false;
	BOOL IsToghterPlayMonster = false;
	BOOL IsMining = false;
	BOOL IsCollection = false;
	BOOL IsHunt = false;
	BOOL IsTitanMonster = false;
	switch (MonsterKind)
	{
	case eObjectKind_FieldBossMonster: IsFieldBoss = true;           break;
	case eObjectKind_SpecialMonster:   IsSpecialMonster = true;      break;
	case eObjectKind_ToghterPlayMonster:IsToghterPlayMonster = true; break;
	case eObjectKind_Mining: IsMining = true;						 break;
	case eObjectKind_Collection: IsCollection = true;				 break;
	case eObjectKind_Hunt: IsHunt = true;							 break;
	case eObjectKind_TitanMonster: IsTitanMonster = true;			 break;
	default: break;
	}
	pMonster->Init(eObjectKind_Monster, pBaseObjectInfo);
	pMonster->InitMonster(pTotalInfo);
	pMonster->InitObjectBalloon(ObjectBalloon_Name | ObjectBalloon_Chat);
	APPEARANCEMGR->InitAppearance(pMonster);
	MOVEMGR->InitMove(pMonster, pMoveInfo);
	OBJECTSTATEMGR->InitObjectState(pMonster);
	m_ObjectTable.Add(pMonster, pMonster->GetID());
	ApplyOverInfoOption(pMonster);
	ApplyShadowOption(pMonster);
	if (IsFieldBoss) pMonster->SetObjectKind(eObjectKind_FieldBossMonster);
	else if (IsSpecialMonster) pMonster->SetObjectKind(eObjectKind_SpecialMonster);
	else if (IsToghterPlayMonster) pMonster->SetObjectKind(eObjectKind_ToghterPlayMonster);
	else if (IsMining) pMonster->SetObjectKind(eObjectKind_Mining);
	else if (IsCollection) pMonster->SetObjectKind(eObjectKind_Collection);
	else if (IsHunt) pMonster->SetObjectKind(eObjectKind_Hunt);
	else if (IsTitanMonster) pMonster->SetObjectKind(eObjectKind_TitanMonster);
	return pMonster;
}
CBossMonster* CObjectManager::AddBossMonster(BASEOBJECT_INFO* pBaseObjectInfo, BASEMOVE_INFO* pMoveInfo, MONSTER_TOTALINFO* pTotalInfo)
{
	CBossMonster * pBoss = NULL;
	CObject* pPreObj = GetObject(pBaseObjectInfo->dwObjectID);
	ASSERT(pPreObj == NULL);
	if (pPreObj)
	{
		RemoveObject(&pPreObj);
	}
	switch (pTotalInfo->MonsterKind)
	{
	case eBOSSKIND_EMPERIORTOMB_EVENT:
	case eBOSSKIND_EMPERIORTOMB:
	{
								   pBoss = m_pBoss[eBOSSTYPE_EMPERIORTOMB] = m_BossMonsterPool.Alloc();
	}
	break;
	case eBOSSKIND_GENERAL:
	{
							  pBoss = m_pBoss[eBOSSTYPE_GENERAL] = m_BossMonsterPool.Alloc();
	}
	break;
	case eBOSSTYPE_YUKJI:
	{
							pBoss = m_pBoss[eBOSSTYPE_YUKJI] = m_BossMonsterPool.Alloc();
	}
	break;
	case eBOSSKIND_MOOUN:
	{
							pBoss = m_pBoss[eBOSSTYPE_MOOUN] = m_BossMonsterPool.Alloc();
	}
	break;
	case eBOSSKIND_COWF:
	{
						   pBoss = m_pBoss[eBOSSTYPE_COWF] = m_BossMonsterPool.Alloc();
	}
	break;
	case eBOSSKIND_COWL:
	{
						   pBoss = m_pBoss[eBOSSTYPE_COWL] = m_BossMonsterPool.Alloc();
	}
	break;
	case eBOSSKIND_COWKING:
	{
							  pBoss = m_pBoss[eBOSSTYPE_COWKING] = m_BossMonsterPool.Alloc();
	}
	break;
#ifdef _NAGA_
	case eBOSSKIND_REDDRADON:
	{
								pBoss = m_pBoss[eBOSSTYPE_REDDRADON] = m_BossMonsterPool.Alloc();
	}
	break;
#endif
	default:
	ASSERT(0);
	}
	pBoss->Init(eObjectKind_BossMonster, pBaseObjectInfo);
	pBoss->InitMonster(pTotalInfo);
	pBoss->InitObjectBalloon(ObjectBalloon_Name | ObjectBalloon_Chat);
	APPEARANCEMGR->InitAppearance(pBoss);
	MOVEMGR->InitMove(pBoss, pMoveInfo);
	OBJECTSTATEMGR->InitObjectState(pBoss);
	m_ObjectTable.Add(pBoss, pBoss->GetID());
	ApplyOverInfoOption(pBoss);
	ApplyShadowOption(pBoss);
	return pBoss;
}
CNpc* CObjectManager::AddNpc(BASEOBJECT_INFO* pBaseObjectInfo, BASEMOVE_INFO* pMoveInfo, NPC_TOTALINFO* pTotalInfo)
{
	CNpc* pNpc = m_NpcPool.Alloc();
	pNpc->Init(eObjectKind_Npc, pBaseObjectInfo);
	pNpc->InitNpc(pTotalInfo);
	pNpc->InitObjectBalloon(ObjectBalloon_Name);
	pNpc->m_MoveInfo.CurPosition.x = 0;
	pNpc->m_MoveInfo.CurPosition.y = 0;
	pNpc->m_MoveInfo.CurPosition.z = 0;
	APPEARANCEMGR->InitAppearance(pNpc);
	MOVEMGR->InitMove(pNpc, pMoveInfo);
	OBJECTSTATEMGR->InitObjectState(pNpc);
	m_ObjectTable.Add(pNpc, pNpc->GetID());
	ApplyOverInfoOption(pNpc);
	ApplyShadowOption(pNpc);
	return pNpc;
}
CTacticObject* CObjectManager::AddTacticObject(BASEOBJECT_INFO* pBaseObjectInfo, TACTIC_TOTALINFO* pTotalInfo)
{
	CObject* pPreObj = GetObject(pBaseObjectInfo->dwObjectID);
	ASSERT(pPreObj == NULL);
	CTacticObject* pTObj = m_TacticObjectPool.Alloc();
	pTObj->Init(eObjectKind_Tactic, pBaseObjectInfo);
	pTObj->InitTacticObject(pTotalInfo);
	BASEMOVE_INFO moveinfo;
	moveinfo.bMoving = FALSE;
	moveinfo.CurPosition.x = pTotalInfo->Pos.x;
	moveinfo.CurPosition.y = pTotalInfo->Pos.y;
	moveinfo.CurPosition.z = pTotalInfo->Pos.z;
	MOVEMGR->InitMove(pTObj, &moveinfo, DIRTODEG(pTotalInfo->Direction));
	OBJECTSTATEMGR->InitObjectState(pTObj);
	m_ObjectTable.Add(pTObj, pTObj->GetID());
	return pTObj;
}
void CObjectManager::AddSkillObject(CSkillObject* pSObj)
{
	CObject* pPreObj = GetObject(pSObj->GetID());
	ASSERT(pPreObj == NULL);
	if (pPreObj)
	{
		RemoveObject(&pPreObj);
	}
	m_ObjectTable.Add(pSObj, pSObj->GetID());
}
void CObjectManager::RemoveAllObject()
{
	m_ObjectTable.SetPositionHead();
	CObject* pObject;
	while (pObject = m_ObjectTable.GetData())
	{
		RemoveObject(&pObject);
	}
	m_ObjectTable.RemoveAll();
	SetSelectedObjectID(0);
	m_bAllNameShow = FALSE;
	if (GAMEIN->GetMonsterGuageDlg())
		GAMEIN->GetMonsterGuageDlg()->SetActive(FALSE);
	m_pCurGTFlg = NULL;
	m_pCurSGFlg = NULL;
	m_dwWeekData = 0;
}
void CObjectManager::RemoveObject(CObject** ppObject)
{
	if (*ppObject)
	{
		m_ObjectTable.Remove((*ppObject)->GetID());
		if (m_SelectedObjectID == (*ppObject)->GetID())
		{
			SetSelectedObjectID(0);
		}
		int kind = (*ppObject)->GetObjectKind();
		(*ppObject)->Release();
		if ((*ppObject) == HERO)
		{
			delete (*ppObject);
			m_pHero = NULL;
		}
		else
		{
			switch (kind)
			{
			case eObjectKind_Player:
			m_PlayerPool.Free((CPlayer*)*ppObject);
			break;
			case eObjectKind_Monster:
			case eObjectKind_FieldBossMonster:
			case eObjectKind_FieldSubMonster:
			m_MonsterPool.Free((CMonster*)*ppObject);
			break;
			case eObjectKind_SpecialMonster:
			m_MonsterPool.Free((CMonster*)*ppObject);
			break;
			case eObjectKind_ToghterPlayMonster:
			m_MonsterPool.Free((CMonster*)*ppObject);
			break;
			case eObjectKind_Npc:
			m_NpcPool.Free((CNpc*)*ppObject);
			break;
			case eObjectKind_Tactic:
			m_TacticObjectPool.Free((CTacticObject*)*ppObject);
			break;
			case eObjectKind_SkillObject:
			break;
			case eObjectKind_BossMonster:
			m_BossMonsterPool.Free((CBossMonster*)*ppObject);
			break;
			case eObjectKind_Pet:
			m_PetPool.Free((CPet*)*ppObject);
			break;
			case eObjectKind_MapObject:
			case eObjectKind_CastleGate:
			m_MapObjectPool.Free((CMapObject*)*ppObject);
			break;
			case eObjectKind_Mining:
			m_MonsterPool.Free((CMonster*)*ppObject);
			break;
			case eObjectKind_Collection:
			m_MonsterPool.Free((CMonster*)*ppObject);
			break;
			case eObjectKind_Hunt:
			m_MonsterPool.Free((CMonster*)*ppObject);
			break;
			case eObjectKind_TitanMonster:
			m_MonsterPool.Free((CMonster*)*ppObject);
			break;
			default:
			{
					   ASSERT(0);
			}
			}
		}
		*ppObject = NULL;
	}
}
void CObjectManager::AddGarbageObject(CObject* pObject)
{
	RemoveObject(&pObject);
}
void CObjectManager::ProcessGarbage()
{
	CObject* pObject;
	while (pObject = (CObject*)m_GarbageObjectList.RemoveTail())
	{
		RemoveObject(&pObject);
	}
}
CObject* CObjectManager::GetObject(DWORD dwObjectID)
{
	return m_ObjectTable.GetData(dwObjectID);
}
void CObjectManager::GetTargetInRange(VECTOR3* pPivotPos, float Radius, ObjectArray<LPOBJECT>* pObjectArray, BOOL bPK, BOOL pBehindWall, BOOL pIgnoreBosses)
{
	pObjectArray->Init();
	m_ObjectTable.SetPositionHead();
	CObject* pObject;
	VECTOR3 ObjectPos;
	float dist;
	float dx, dz;
	while (pObject = m_ObjectTable.GetData())
	{
		BYTE wJobMonsterKind = pObject->GetObjectKind();
		if (
			(wJobMonsterKind == 33 ||
			wJobMonsterKind == 34 ||
			wJobMonsterKind == 35 ||
			wJobMonsterKind == 36 ||
			wJobMonsterKind == 37 ||
			GetObjectKindGroup(wJobMonsterKind) == eOBJECTKINDGROUP_JOB) && pIgnoreBosses)
			continue;
		if (
			wJobMonsterKind == eObjectKind_Npc ||
			wJobMonsterKind == eObjectKind_Tactic ||
			GetObjectKindGroup(wJobMonsterKind) == eOBJECTKINDGROUP_JOB)
			continue;
		if (pObject->IsDied() == TRUE)
			continue;
		if (wJobMonsterKind == eObjectKind_SkillObject)
		{
			if (((CSkillObject*)pObject)->IsLifeObject() == FALSE)
				continue;
		}
		if (pBehindWall == TRUE)
		{
			VECTOR3 TempPos;
			pObject->GetPosition(&TempPos);
			if (PATHMGR->CheckCollisionLine(pPivotPos, &TempPos, HERO))
			{
				continue;
			}
		}
		pObject->GetPosition(&ObjectPos);
		dx = (pPivotPos->x - ObjectPos.x);
		dz = (pPivotPos->z - ObjectPos.z);
		dist = sqrtf(dx*dx + dz*dz) - pObject->GetRadius();
		if (dist <= Radius)
			pObjectArray->AddObject(pObject);
	}
}
void CObjectManager::GetTargetInRangeForAI(VECTOR3* pPivotPos, float Radius, ObjectArray<LPOBJECT>* pObjectArray, BOOL bPK)
{
	pObjectArray->Init();
	m_ObjectTable.SetPositionHead();
	CObject* pObject;
	VECTOR3 ObjectPos;
	float dist;
	float dx, dz;
	while (pObject = m_ObjectTable.GetData())
	{
		BYTE wJobMonsterKind = pObject->GetObjectKind();
		if (wJobMonsterKind == eObjectKind_Npc ||
			wJobMonsterKind == eObjectKind_Tactic ||
			GetObjectKindGroup(wJobMonsterKind) == eOBJECTKINDGROUP_JOB)
			continue;
		if (pObject->IsDied() == TRUE)
			continue;
		if (wJobMonsterKind == eObjectKind_SkillObject)
		{
			if (((CSkillObject*)pObject)->IsLifeObject() == FALSE)
				continue;
		}
		VECTOR3 TempPos;
		pObject->GetPosition(&TempPos);
		if (PATHMGR->CheckCollisionLine(pPivotPos, &TempPos, HERO))
		{
			continue;
		}
		pObject->GetPosition(&ObjectPos);
		dx = (pPivotPos->x - ObjectPos.x);
		dz = (pPivotPos->z - ObjectPos.z);
		dist = sqrtf(dx*dx + dz*dz) - pObject->GetRadius();
		if (dist <= Radius)
			pObjectArray->AddObject(pObject);
	}
}
void CObjectManager::GetTargetInArea(CHero* pHero, CSkillArea* pSkillArea, ObjectArray<LPOBJECT>* pObjectArray, BOOL bPK)
{
	CObject* pObject;
	VECTOR3 ObjectPos;
	pObjectArray->Init();
	m_ObjectTable.SetPositionHead();
	while (pObject = m_ObjectTable.GetData())
	{
		BYTE wJobMonsterKind = pObject->GetObjectKind();
		if (wJobMonsterKind == eObjectKind_Npc ||
			wJobMonsterKind == eObjectKind_Tactic ||
			GetObjectKindGroup(wJobMonsterKind) == eOBJECTKINDGROUP_JOB)
			continue;
		if (pObject->IsDied() == TRUE)
			continue;
		pObject->GetPosition(&ObjectPos);
		if (pSkillArea->IsInDamageTile(&ObjectPos) == TRUE)
			pObjectArray->AddObject(pObject);
	}
}
CObject* CObjectManager::GetSelectedObject()
{
	if (m_SelectedObjectID == 0)
		return NULL;
	CObject* pObject = GetObject(m_SelectedObjectID);
	return pObject;
}
void CObjectManager::SetSelectedObject(CObject* pObject)
{
	CObject* pLastSelectedObject = GetSelectedObject();
	if (pLastSelectedObject == pObject) return;
	if (!(MAINGAME->GetUserLevel() <= eUSERLEVEL_GM && CAMERA->GetCameraMode() != eCM_EyeView))
	{
		if (pObject)
		if (pObject->GetSingleSpecialState(eSingleSpecialState_Hide) && !HERO->IsDetect())
			return;
	}
#ifdef _CHEATENABLE_
	{
		CHEATMGR->SetCheatTargetObject(pObject);
	}
#endif
	if (pLastSelectedObject && pLastSelectedObject->IsInited())
	{
		if ((CanOverInfoChange(pLastSelectedObject) && !m_bAllNameShow) || (pLastSelectedObject->GetObjectKind() & eObjectKind_Monster))
			pLastSelectedObject->ShowObjectName(FALSE);
		else
			pLastSelectedObject->ShowObjectName(TRUE, GetObjectNameColor(pLastSelectedObject));
		pLastSelectedObject->OnDeselected();
	}
	if (pObject)
	{
		m_SelectedObjectID = pObject->GetID();
		pObject->OnSelected();
		BYTE objectKind = pObject->GetObjectKind();
		GAMEIN->GetMonsterGuageDlg()->SetMonsterNameColor(NAMECOLOR_WHITE);
		if (objectKind == eObjectKind_Player)
		{
			if (!GUILDFIELDWAR->IsEnemy((CPlayer*)pObject))
				pObject->ShowObjectName(TRUE, NAMECOLOR_SELECTED);
			if (GAMEIN->GetMonsterGuageDlg())
			{
				GAMEIN->GetMonsterGuageDlg()->SetActive(TRUE);
				GAMEIN->GetMonsterGuageDlg()->SetMonsterName(pObject->GetObjectName());
				GAMEIN->GetMonsterGuageDlg()->SetGuildUnionName(((CPlayer*)pObject)->GetGuildName(), ((CPlayer*)pObject)->GetGuildUnionName());
				GAMEIN->GetMonsterGuageDlg()->ShowMonsterGuageMode(eMonGuage_Character);
			}
		}
		else if (objectKind & eObjectKind_Pet)
		{
			pObject->ShowObjectName(TRUE, NAMECOLOR_PARTY);
			if (GAMEIN->GetMonsterGuageDlg())
			{
				GAMEIN->GetMonsterGuageDlg()->SetActive(TRUE);
				GAMEIN->GetMonsterGuageDlg()->SetMonsterName(pObject->GetObjectName());
				char szBuf[MAX_MASTERNAME_LENGTH + 1] = { 0, };
				sprintf(szBuf, "%s%s", ((CPet*)pObject)->m_MasterName, CHATMGR->GetChatMsg(1248));
				GAMEIN->GetMonsterGuageDlg()->SetNpcName(szBuf);
				GAMEIN->GetMonsterGuageDlg()->ShowMonsterGuageMode(eMonGuage_Npc);
			}
		}
		else if (objectKind & eObjectKind_Monster)
		{
			DWORD dwNameColor = NAMECOLOR_SELECTED;
			if (objectKind == eObjectKind_TitanMonster)
			{
				dwNameColor = NAMECOLOR_TITANMONSTER_SELECT;
			}
			pObject->ShowObjectName(TRUE, dwNameColor);
			if (GAMEIN->GetMonsterGuageDlg())
			{
				GAMEIN->GetMonsterGuageDlg()->SetActive(TRUE);
				char s[128];
				WORD kind = ((CMonster*)pObject)->GetObjectKind();
				char arg[256] = { 0, };
				switch (kind)
				{
				case eObjectKind_Monster:
				{
											sprintf(arg, "%s", CHATMGR->GetChatMsg(1311));
											sprintf(s, arg, ((CMonster*)pObject)->GetSInfo()->Level, ((CMonster*)pObject)->GetSInfo()->Name);
				}
				break;
				case eObjectKind_BossMonster:
				{
												sprintf(arg, "%s", CHATMGR->GetChatMsg(1309));
												sprintf(s, arg, ((CMonster*)pObject)->GetSInfo()->Name);
				}
				break;
				case eObjectKind_SpecialMonster:
				{
												   sprintf(arg, "%s", CHATMGR->GetChatMsg(1352));
												   sprintf(s, arg, ((CMonster*)pObject)->GetSInfo()->Name);
				}
				break;
				case eObjectKind_FieldBossMonster:
				{
													 sprintf(arg, "%s", CHATMGR->GetChatMsg(1310));
													 sprintf(s, arg, ((CMonster*)pObject)->GetSInfo()->Name);
				}
				break;
				case eObjectKind_FieldSubMonster:
				{
													sprintf(arg, "%s", CHATMGR->GetChatMsg(1311));
													sprintf(s, arg, ((CMonster*)pObject)->GetSInfo()->Level, ((CMonster*)pObject)->GetSInfo()->Name);
				}
				break;
				case eObjectKind_ToghterPlayMonster:
				{
													   sprintf(arg, "%s", CHATMGR->GetChatMsg(1351));
													   sprintf(s, arg, ((CMonster*)pObject)->GetSInfo()->Name);
				}
				break;
				case eObjectKind_Mining:
				{
										   sprintf(s, CHATMGR->GetChatMsg(1670));
				}
				break;
				case eObjectKind_Collection:
				{
											   sprintf(s, CHATMGR->GetChatMsg(1671));
				}
				break;
				case eObjectKind_Hunt:
				{
										 sprintf(s, CHATMGR->GetChatMsg(1672));
				}
				break;
				case eObjectKind_TitanMonster:
				{
												 sprintf(arg, "%s", CHATMGR->GetChatMsg(1311));
												 sprintf(s, arg, ((CMonster*)pObject)->GetSInfo()->Level, ((CMonster*)pObject)->GetSInfo()->Name);
				}
				break;
				default:
				break;
				}
				if (GetObjectKindGroup(kind) == eOBJECTKINDGROUP_NONE)
				{
					GAMEIN->GetMonsterGuageDlg()->SetMonsterName(s, ((CMonster*)pObject)->GetSInfo()->MonsterKind, objectKind);
					GAMEIN->GetMonsterGuageDlg()->SetMonsterLife((CMonster *)pObject);
					GAMEIN->GetMonsterGuageDlg()->SetMonsterShield((CMonster *)pObject);
					GAMEIN->GetMonsterGuageDlg()->ShowMonsterGuageMode(eMonGuage_Monster);
				}
				else
				{
					GAMEIN->GetMonsterGuageDlg()->SetActive(TRUE);
					GAMEIN->GetMonsterGuageDlg()->SetMonsterName(s);
					GAMEIN->GetMonsterGuageDlg()->SetNpcName(((CMonster*)pObject)->GetSInfo()->Name);
					GAMEIN->GetMonsterGuageDlg()->ShowMonsterGuageMode(eMonGuage_Npc);
				}
			}
		}
		else if (objectKind & eObjectKind_MapObject)
		{
			CMapObject* pMObject = (CMapObject*)pObject;
			pMObject->ShowObjectName(TRUE, NAMECOLOR_SELECTED);
			if (GAMEIN->GetMonsterGuageDlg())
			{
				GAMEIN->GetMonsterGuageDlg()->SetActive(TRUE);
				GAMEIN->GetMonsterGuageDlg()->SetMonsterName(pMObject->GetObjectName());
				GAMEIN->GetMonsterGuageDlg()->SetObjectType(eObjectKind_MapObject);
				GAMEIN->GetMonsterGuageDlg()->SetMonsterLife(pMObject->GetLife(), pMObject->GetMaxLife());
				GAMEIN->GetMonsterGuageDlg()->SetMonsterShield(pMObject->GetShield(), pMObject->GetMaxShield());
				GAMEIN->GetMonsterGuageDlg()->ShowMonsterGuageMode(eMonGuage_Monster);
			}
		}
		else if (objectKind == eObjectKind_Npc)
		{
			pObject->ShowObjectName(TRUE, NAMECOLOR_SELECTED);
			WORD npcKind = ((CNpc*)pObject)->GetNpcKind();
			if (npcKind == 30 || npcKind == 31 || npcKind == 0)
			{
				if (GAMEIN->GetMonsterGuageDlg())
					GAMEIN->GetMonsterGuageDlg()->SetActive(FALSE);
				return;
			}
			if (GAMEIN->GetMonsterGuageDlg())
			{
				GAMEIN->GetMonsterGuageDlg()->SetActive(TRUE);
				GAMEIN->GetMonsterGuageDlg()->SetMonsterName(pObject->GetObjectName());
				GAMEIN->GetMonsterGuageDlg()->SetNpcName(((CNpc*)pObject)->GetSInfo()->Name);
				GAMEIN->GetMonsterGuageDlg()->ShowMonsterGuageMode(eMonGuage_Npc);
			}
		}
	}
	else
	{
		SetSelectedObjectID(0);
		GAMEIN->GetPartyDialog()->SetClickedMemberID(0);
	}
}
void CObjectManager::SetOverInfoOption(DWORD dwOption)
{
	m_dwOverInfoOption = dwOption;
	ApplyOverInfoOptionToAll();
}
void CObjectManager::ApplyOverInfoOptionToAll()
{
	CObject* pObject;
	m_ObjectTable.SetPositionHead();
	while (pObject = m_ObjectTable.GetData())
	{
		ApplyOverInfoOption(pObject);
	}
}
extern WORD mapNum;
void CObjectManager::ApplyOverInfoOption(CObject* pObject)
{
	if (pObject == NULL) return;
	pObject->SetOverInfoOption(1);
	if (pObject->GetObjectKind() == eObjectKind_Player)
	{
#ifndef _GMTOOL_
		if (((CPlayer*)pObject)->GetCharacterTotalInfo()->bVisible == FALSE)
		{
			pObject->ShowObjectName(FALSE);
			return;
		}
#endif
		if (!(MAINGAME->GetUserLevel() <= eUSERLEVEL_GM  && CAMERA->GetCameraMode() != eCM_EyeView))
		{
			if (HERO)
			if (HERO != pObject)
			if (!HERO->IsDetect() && ((CPlayer*)pObject)->IsHide())
			{
				pObject->SetOverInfoOption(0);
				pObject->ShowObjectName(FALSE);
				return;
			}
		}
		if (GUILDFIELDWAR->IsEnemy((CPlayer*)pObject))
		{
			pObject->ShowObjectName(TRUE, NAMECOLOR_PKMODE);
			return;
		}
	}
	if (GetSelectedObject() != pObject)
	{
		if (pObject->GetObjectKind() == eObjectKind_Player)
		{
			if (TRUE)
			{
				if (WANTEDMGR->IsChrIDInList(pObject->GetID()))
				{
					pObject->ShowObjectName(TRUE, NAMECOLOR_WANTED);
					return;
				}
				if (((CPlayer*)pObject)->IsPKMode() || ((CPlayer*)pObject)->IsRestraintMode())
				{
					pObject->ShowObjectName(TRUE, NAMECOLOR_PKMODE);
					return;
				}
				if (m_dwOverInfoOption & eOO_NAME_PARTY || m_bAllNameShow)
				{
					if (PARTYMGR->IsPartyMember(pObject->GetID()))
					{
						pObject->ShowObjectName(TRUE, NAMECOLOR_PARTY);
						return;
					}
				}
				if (m_dwOverInfoOption & eOO_NAME_MUNPA || m_bAllNameShow)
				{
					if (HERO)
					if (HERO->GetGuildIdx() != 0)
					if (HERO->GetGuildIdx() == ((CPlayer*)pObject)->GetGuildIdx())
					{
						pObject->ShowObjectName(TRUE, RGB_HALF(102, 153, 204));
						if (((CPlayer*)pObject)->GetGuildMarkName() > 0)
							pObject->SetGuildMark();
						return;
					}
				}
				if (m_dwOverInfoOption & eOO_NAME_WHOLE || m_bAllNameShow)
				{
					if (MAINGAME->GetCurStateNum() == eGAMESTATE_GAMEIN)
					if (CalcDistanceXZ(&pObject->GetCurPosition(), &HERO->GetCurPosition()) > 2000.f)
					{
						pObject->ShowObjectName(FALSE);
						return;

					}

					pObject->ShowObjectName(TRUE);
					return;

				}
			}
			pObject->ShowObjectName(FALSE);
		}
		else if (pObject->GetObjectKind() == eObjectKind_Npc && ((CNpc*)pObject)->GetNpcUniqueIdx() == mapNum)
		{
			if (m_bAllNameShow)
				pObject->ShowObjectName(TRUE);
			else
				pObject->ShowObjectName(FALSE);
		}
	}
	else
	{
		pObject->ShowObjectName(TRUE, NAMECOLOR_SELECTED);
	}
}
void CObjectManager::SetHideWingOption(DWORD dwOption)
{
	//m_dwShadowOption = dwOption;
	ApplyHideWingOptionToAll();
}
void CObjectManager::ApplyHideWingOptionToAll()
{
	CObject* pObject;
	m_ObjectTable.SetPositionHead();
	while (pObject = m_ObjectTable.GetData())
	{
		//ApplyShadowOption(pObject);
		if (pObject->GetObjectKind() == eObjectKind_Player)
		{
			APPEARANCEMGR->InitAppearance(pObject);
			//break;
		}
	}
}
#include "FortWarManager.h"

void CObjectManager::ApplyFortWarCustomName()
{
	CObject* pObject;
	m_ObjectTable.SetPositionHead();
	while (pObject = m_ObjectTable.GetData())
	{
		//ApplyShadowOption(pObject);
		if (pObject->GetObjectKind() == eObjectKind_Player)
		{
			pObject->SetFortName(FORTWARMGR->GetObjectName());
		//	APPEARANCEMGR->InitAppearance(pObject);
			//break;
		}
	}
}
void CObjectManager::SetAllNameShow(BOOL bShow)
{
	if (m_bAllNameShow == bShow)
		return;
	m_bAllNameShow = bShow;
	ApplyOverInfoOptionToAll();
}
void CObjectManager::OnMouseOver(CObject* pObject)
{
	if (pObject->GetID() != m_SelectedObjectID)
	{
		BYTE objectKind = pObject->GetObjectKind();
		if (!m_bAllNameShow || objectKind & eObjectKind_Monster)
		{
			if (CanOverInfoChange(pObject))
			{
				DWORD dwColor = NAMECOLOR_DEFAULT;
				if (objectKind == eObjectKind_TitanMonster)
				{
					dwColor = NAMECOLOR_TITANMONSTER;
				}
				pObject->ShowObjectName(TRUE, dwColor);
			}
		}
		if (MAINGAME->GetCurStateNum() == eGAMESTATE_GAMEIN)
		if (objectKind == eObjectKind_Player)
		{
			if (CanOverInfoChange(pObject))
			{
				DWORD dwColor = NAMECOLOR_DEFAULT;
				pObject->ShowObjectName(TRUE, dwColor);
			}
		}
	}
}
void CObjectManager::OnMouseLeave(CObject* pObject)
{
	if (pObject->GetID() != m_SelectedObjectID)
	if (!m_bAllNameShow || pObject->GetObjectKind() & eObjectKind_Monster)
	if (CanOverInfoChange(pObject))
	{
		pObject->ShowObjectName(FALSE);
	}
	if (MAINGAME->GetCurStateNum() == eGAMESTATE_GAMEIN)
	if (pObject->GetID() != m_SelectedObjectID)
	if (pObject->GetObjectKind() == eObjectKind_Player)
	if (CanOverInfoChange(pObject))
	{
		if (CalcDistanceXZ(&pObject->GetCurPosition(), &HERO->GetCurPosition()) >  2000.f)
			pObject->ShowObjectName(FALSE);
		else
			pObject->ShowObjectName(TRUE);
	}
}
BOOL CObjectManager::CanOverInfoChange(CObject* pObject)
{
	if (pObject->GetObjectKind() == eObjectKind_Player)
	{
		if (WANTEDMGR->IsChrIDInList(pObject->GetID())) return FALSE;
		if (((CPlayer*)pObject)->IsPKMode())		return FALSE;
		if (((CPlayer*)pObject)->IsRestraintMode())	return FALSE;
		if (m_dwOverInfoOption & eOO_NAME_WHOLE)	return FALSE;
		if (m_dwOverInfoOption & eOO_NAME_PARTY)
		if (PARTYMGR->IsPartyMember(pObject->GetID()))
			return FALSE;
		if (m_dwOverInfoOption & eOO_NAME_MUNPA)
		if (HERO)
		if (HERO->GetMunpaIdx() != 0)
		if (HERO->GetMunpaMemberRank() > MUNPA_SEMIMEMBER)
		if (HERO->GetMunpaIdx() == ((CPlayer*)pObject)->GetMunpaIdx() && ((CPlayer*)pObject)->GetMunpaMemberRank() > MUNPA_SEMIMEMBER)
			return FALSE;
		if (GUILDFIELDWAR->IsEnemy((CPlayer*)pObject))
			return FALSE;
	}
	return TRUE;
}
DWORD CObjectManager::GetObjectNameColor(CObject* pObject)
{
	if (pObject->GetObjectKind() == eObjectKind_Player)
	{
		if (strncmp(pObject->GetObjectName(), "GM", 2) == 0)
			return NAMECOLOR_GM;
		if (WANTEDMGR->IsChrIDInList(pObject->GetID()))
			return NAMECOLOR_WANTED;
		if (((CPlayer*)pObject)->IsPKMode() || ((CPlayer*)pObject)->IsRestraintMode())
			return NAMECOLOR_PKMODE;
		if (m_dwOverInfoOption & eOO_NAME_PARTY)
		if (PARTYMGR->IsPartyMember(pObject->GetID()))
			return NAMECOLOR_PARTY;
		if (m_dwOverInfoOption & eOO_NAME_MUNPA)
		if (HERO)
		if (HERO->GetGuildIdx())
		if (HERO->GetGuildIdx() == ((CPlayer*)pObject)->GetGuildIdx())
			return NAMECOLOR_MUNPA;
		if (GUILDFIELDWAR->IsEnemy((CPlayer*)pObject))
			return NAMECOLOR_PKMODE;
		return NAMECOLOR_PLAYER;
	}
	return NAMECOLOR_DEFAULT;
}
													  
 
							   
							
 
											   
 
				  
								 
										  
  
							   
										 
  
 
void CObjectManager::SetShadowOption(DWORD dwOption)
{
	m_dwShadowOption = dwOption;
	ApplyShadowOptionToAll();
}
void CObjectManager::ApplyShadowOptionToAll()
{
	CObject* pObject;
	m_ObjectTable.SetPositionHead();
	while (pObject = m_ObjectTable.GetData())
	{
		ApplyShadowOption(pObject);
	}
}
void CObjectManager::ApplyShadowOption(CObject* pObject)
{
	if (pObject == NULL) return;
	if (pObject->GetObjectKind() == eObjectKind_Player)
	{
		if (pObject != HERO)
		if (((CPlayer*)pObject)->IsHide())
		{
			AddShadow(pObject);
			return;
		}
	}
	if (m_dwShadowOption & eOO_SHADOW_OTHERCHARACTER)
	{
		if (pObject->GetObjectKind() == eObjectKind_Player)
		if (pObject != HERO)
		{
			AddShadow(pObject);
			return;
		}
	}
	if (m_dwShadowOption & eOO_SHADOW_ONLYHERO)
	{
		if (pObject == HERO)
		{
			AddShadow(pObject);
			return;
		}
	}
	if (m_dwShadowOption & eOO_SHADOW_MONSTER)
	{
		if ((pObject->GetObjectKind() & eObjectKind_Monster) || (pObject->GetObjectKind() == eObjectKind_Npc) || (pObject->GetObjectKind() == eObjectKind_Pet))
		{
#ifdef _JAPAN_LOCAL_
			if (pObject->GetObjectKind() & eObjectKind_Monster)
			{
				if (((CMonster*)pObject)->GetMonsterKind() == 168 ||
					((CMonster*)pObject)->GetMonsterKind() == 169)
				{
					RemoveShadow(pObject);
				}
				else
				{
					AddShadow(pObject);
				}
			}
			else
				AddShadow(pObject);
			return;
#else
#ifdef _GMTOOL_
			if (MAINGAME->GetUserLevel() <= eUSERLEVEL_GM  && CAMERA->GetCameraMode() != eCM_EyeView)
			{
				if (1.0f > pObject->GetEngineObject()->GetAlpha())
				{
					RemoveShadow(pObject);
				}
				else
				{
					AddShadow(pObject);
				}
			}
			else
#endif
				AddShadow(pObject);
			return;
#endif
		}
	}
	RemoveShadow(pObject);
}
void CObjectManager::ShowChatBalloon(CObject* pObject, char* pStrChat, DWORD dwColor, DWORD dwAliveTime)
{
	if (m_dwOverInfoOption & eOO_CHAT_NOBALLOON)
		return;
	if (pObject)
		pObject->ShowChatBalloon(TRUE, pStrChat, dwColor, dwAliveTime);
}
void CObjectManager::HideChatBalloonAll()
{
	CObject* pObject;
	m_ObjectTable.SetPositionHead();
	while (pObject = m_ObjectTable.GetData())
	{
		pObject->ShowChatBalloon(FALSE, NULL);
	}
}
void CObjectManager::UpdateTargetList_Monster(CSkillObject* pSkillObject)
{
	CObject* pObject;
	m_ObjectTable.SetPositionHead();
	while (pObject = m_ObjectTable.GetData())
	{
		if ((pObject->GetObjectKind() & eObjectKind_Monster) == FALSE)
			continue;
		pSkillObject->UpdateTargetList(pObject);
	}
}
void CObjectManager::NetworkMsgParse(BYTE Protocol, void* pMsg)
{
	switch (Protocol)
	{
	case MP_BOSS_REST_START_NOTIFY:
	{
									  MSG_DWORD * pmsg = (MSG_DWORD *)pMsg;
									  switch (pmsg->dwData)
									  {
									  case eBOSSKIND_EMPERIORTOMB:
									  case eBOSSKIND_EMPERIORTOMB_EVENT:
									  {
																		   OBJECTSTATEMGR->StartObjectState(m_pBoss[eBOSSTYPE_EMPERIORTOMB], eObjectState_Rest);
																		   TARGETSET set;
																		   set.pTarget = m_pBoss[eBOSSTYPE_EMPERIORTOMB];
																		   m_hRestHandle[eBOSSTYPE_EMPERIORTOMB] = EFFECTMGR->StartEffectProcess(
																			   FindEffectNum("Boss001_summon.beff"),
																			   m_pBoss[eBOSSTYPE_EMPERIORTOMB], NULL, 0, m_pBoss[eBOSSTYPE_EMPERIORTOMB]->GetID());
									  }
									  break;
									  case eBOSSKIND_MOOUN:
									  {
									  }
									  break;
									  case eBOSSKIND_COWF:
									  {
									  }
									  break;
									  case eBOSSKIND_COWL:
									  {
									  }
									  break;
									  case eBOSSKIND_COWKING:
									  {
																OBJECTSTATEMGR->StartObjectState(m_pBoss[eBOSSTYPE_COWKING], eObjectState_Rest);
																TARGETSET set;
																set.pTarget = m_pBoss[eBOSSTYPE_COWKING];
																m_hRestHandle[eBOSSTYPE_COWKING] = EFFECTMGR->StartEffectProcess(
																	FindEffectNum("M184_attack9.beff"),
																	m_pBoss[eBOSSTYPE_COWKING], NULL, 0, m_pBoss[eBOSSTYPE_COWKING]->GetID());
									  }
									  break;
#ifdef _NAGA_
									  case eBOSSKIND_REDDRADON:
									  {
																  OBJECTSTATEMGR->StartObjectState(m_pBoss[eBOSSTYPE_REDDRADON], eObjectState_Rest);
																  TARGETSET set;
																  set.pTarget = m_pBoss[eBOSSTYPE_REDDRADON];
																  m_hRestHandle[eBOSSTYPE_REDDRADON] = EFFECTMGR->StartEffectProcess(
																	  FindEffectNum("M184_attack9.beff"),
																	  m_pBoss[eBOSSTYPE_REDDRADON], NULL, 0, m_pBoss[eBOSSTYPE_REDDRADON]->GetID());
									  }
									  break;
#endif
									  }
	}
	break;
	case MP_BOSS_RECALL_NOTIFY:
	{
								  MSG_DWORD * pmsg = (MSG_DWORD *)pMsg;
								  switch (pmsg->dwData)
								  {
								  case eBOSSKIND_EMPERIORTOMB:
								  case eBOSSKIND_EMPERIORTOMB_EVENT:
								  {
																	   OBJECTSTATEMGR->StartObjectState(m_pBoss[eBOSSTYPE_EMPERIORTOMB], eObjectState_Rest);
																	   TARGETSET set;
																	   set.pTarget = m_pBoss[eBOSSTYPE_EMPERIORTOMB];
																	   EFFECTMGR->StartEffectProcess(
																		   FindEffectNum("Boss001_summon.beff")
																		   , m_pBoss[eBOSSTYPE_EMPERIORTOMB], &set, 0, m_pBoss[eBOSSTYPE_EMPERIORTOMB]->GetID());
								  }
								  break;
								  case eBOSSKIND_MOOUN:
								  {
								  }
								  break;
								  case eBOSSKIND_COWF:
								  {
								  }
								  break;
								  case eBOSSKIND_COWL:
								  {
								  }
								  break;
								  case eBOSSKIND_COWKING:
								  {
															OBJECTSTATEMGR->StartObjectState(m_pBoss[eBOSSTYPE_COWKING], eObjectState_Rest);
															TARGETSET set;
															set.pTarget = m_pBoss[eBOSSTYPE_COWKING];
															EFFECTMGR->StartEffectProcess(
																FindEffectNum("M184_attack10.beff")
																, m_pBoss[eBOSSTYPE_COWKING], &set, 0, m_pBoss[eBOSSTYPE_COWKING]->GetID());
								  }
								  break;
#ifdef _NAGA_
								  case eBOSSKIND_REDDRADON:
								  {
															  OBJECTSTATEMGR->StartObjectState(m_pBoss[eBOSSTYPE_REDDRADON], eObjectState_Rest);
															  TARGETSET set;
															  set.pTarget = m_pBoss[eBOSSTYPE_REDDRADON];
															  EFFECTMGR->StartEffectProcess(
																  FindEffectNum("M184_attack10.beff")
																  , m_pBoss[eBOSSTYPE_REDDRADON], &set, 0, m_pBoss[eBOSSTYPE_REDDRADON]->GetID());
								  }
								  break;
#endif
								  }
	}
	break;
	case MP_BOSS_LIFE_NOTIFY:
	{
								MSG_DWORD2 * pmsg = (MSG_DWORD2 *)pMsg;
								switch (pmsg->dwData2)
								{
								case eBOSSKIND_EMPERIORTOMB:
								case eBOSSKIND_EMPERIORTOMB_EVENT:
								{
																	 m_pBoss[eBOSSTYPE_EMPERIORTOMB]->SetLife(pmsg->dwData1);
								}
								break;
								case eBOSSKIND_MOOUN:
								{
														m_pBoss[eBOSSTYPE_MOOUN]->SetLife(pmsg->dwData1);
								}
								break;
								case eBOSSKIND_COWF:
								{
													   m_pBoss[eBOSSTYPE_COWF]->SetLife(pmsg->dwData1);
								}
								break;
								case eBOSSKIND_COWL:
								{
													   m_pBoss[eBOSSTYPE_COWL]->SetLife(pmsg->dwData1);
								}
								break;
								case eBOSSKIND_COWKING:
								{
														  m_pBoss[eBOSSTYPE_COWKING]->SetLife(pmsg->dwData1);
								}
								break;
#ifdef _NAGA_
								case eBOSSKIND_REDDRADON:
								{
															m_pBoss[eBOSSTYPE_REDDRADON]->SetLife(pmsg->dwData1);
								}
								break;
#endif
								}
	}
	break;
	case MP_BOSS_SHIELD_NOTIFY:
	{
								  MSG_DWORD2 * pmsg = (MSG_DWORD2 *)pMsg;
								  switch (pmsg->dwData2)
								  {
								  case eBOSSKIND_EMPERIORTOMB:
								  case eBOSSKIND_EMPERIORTOMB_EVENT:
								  {
																	   m_pBoss[eBOSSTYPE_EMPERIORTOMB]->SetShield(pmsg->dwData1);
								  }
								  break;
								  case eBOSSKIND_MOOUN:
								  {
														  m_pBoss[eBOSSTYPE_MOOUN]->SetShield(pmsg->dwData1);
								  }
								  break;
								  case eBOSSKIND_COWF:
								  {
														 m_pBoss[eBOSSTYPE_COWF]->SetShield(pmsg->dwData1);
								  }
								  break;
								  case eBOSSKIND_COWL:
								  {
														 m_pBoss[eBOSSTYPE_COWL]->SetShield(pmsg->dwData1);
								  }
								  break;
								  case eBOSSKIND_COWKING:
								  {
															m_pBoss[eBOSSTYPE_COWKING]->SetShield(pmsg->dwData1);
								  }
								  break;
#ifdef _NAGA_
								  case eBOSSKIND_REDDRADON:
								  {
															  m_pBoss[eBOSSTYPE_REDDRADON]->SetShield(pmsg->dwData1);
								  }
								  break;
#endif
								  }
	}
	case MP_BOSS_STAND_NOTIFY:
	{
								 MSG_DWORD * pmsg = (MSG_DWORD *)pMsg;
								 switch (pmsg->dwData)
								 {
								 case eBOSSKIND_EMPERIORTOMB:
								 case eBOSSKIND_EMPERIORTOMB_EVENT:
								 {
																	  TARGETSET set;
																	  set.pTarget = m_pBoss[eBOSSTYPE_EMPERIORTOMB];
																	  EFFECTMGR->StartEffectProcess(
																		  FindEffectNum("Boss001_Stand.beff")
																		  , m_pBoss[eBOSSTYPE_EMPERIORTOMB], &set, 0, m_pBoss[eBOSSTYPE_EMPERIORTOMB]->GetID());
								 }
								 break;
								 case eBOSSKIND_MOOUN:
								 {
								 }
								 break;
								 case eBOSSKIND_COWF:
								 {
								 }
								 break;
								 case eBOSSKIND_COWL:
								 {
								 }
								 break;
								 case eBOSSKIND_COWKING:
								 {
								 }
								 break;
#ifdef _NAGA_
								 case eBOSSKIND_REDDRADON:
								 {
								 }
								 break;
#endif
								 }
	}
	break;
	case MP_BOSS_STAND_END_NOTIFY:
	{
									 MSG_DWORD * pmsg = (MSG_DWORD *)pMsg;
									 switch (pmsg->dwData)
									 {
									 case eBOSSKIND_EMPERIORTOMB:
									 case eBOSSKIND_EMPERIORTOMB_EVENT:
									 {
																		  if (m_hRestHandle[eBOSSTYPE_EMPERIORTOMB])
																			  EFFECTMGR->ForcedEndEffect(m_hRestHandle[eBOSSTYPE_EMPERIORTOMB]);
									 }
									 break;
									 case eBOSSKIND_MOOUN:
									 {
															 if (m_hRestHandle[eBOSSTYPE_MOOUN])
																 EFFECTMGR->ForcedEndEffect(m_hRestHandle[eBOSSTYPE_MOOUN]);
									 }
									 break;
									 case eBOSSKIND_COWF:
									 {
															if (m_hRestHandle[eBOSSTYPE_COWF])
																EFFECTMGR->ForcedEndEffect(m_hRestHandle[eBOSSTYPE_COWF]);
									 }
									 break;
									 case eBOSSKIND_COWL:
									 {
															if (m_hRestHandle[eBOSSTYPE_COWL])
																EFFECTMGR->ForcedEndEffect(m_hRestHandle[eBOSSTYPE_COWL]);
									 }
									 break;
									 case eBOSSKIND_COWKING:
									 {
															   if (m_hRestHandle[eBOSSTYPE_COWKING])
																   EFFECTMGR->ForcedEndEffect(m_hRestHandle[eBOSSTYPE_COWKING]);
									 }
									 break;
#ifdef _NAGA_
									 case eBOSSKIND_REDDRADON:
									 {
																 if (m_hRestHandle[eBOSSTYPE_REDDRADON])
																	 EFFECTMGR->ForcedEndEffect(m_hRestHandle[eBOSSTYPE_REDDRADON]);
									 }
									 break;
#endif
									 }
	}
	break;
	case MP_FIELD_LIFE_NOTIFY:
	{
								 MSG_DWORD3* pmsg = (MSG_DWORD3*)pMsg;
								 CMonster* pMonster = (CMonster*)GetObject(pmsg->dwData3);
								 if (!pMonster)
									 return;
								 if (pMonster->GetMonsterKind() != pmsg->dwData2)
									 return;
								 pMonster->SetLife(pmsg->dwData1);
	}
	break;
	case MP_FIELD_SHIELD_NOTIFY:
	{
								   MSG_DWORD3* pmsg = (MSG_DWORD3*)pMsg;
								   CMonster* pMonster = (CMonster*)GetObject(pmsg->dwData3);
								   if (!pMonster)
									   return;
								   if (pMonster->GetMonsterKind() != pmsg->dwData2)
									   return;
								   pMonster->SetShield(pmsg->dwData1);
	}
	break;
	break;
	}
}
void CObjectManager::AllPlayerPKOff()
{
	m_ObjectTable.SetPositionHead();
	CObject* pObject = NULL;
	while (pObject = m_ObjectTable.GetData())
	{
		if (pObject->GetObjectKind() != eObjectKind_Player) continue;
		CPlayer* pPlayer = (CPlayer*)pObject;
		if (pPlayer->IsPKMode() == FALSE) continue;
		pPlayer->SetPKMode(FALSE);
		if (HERO)
		if (pPlayer != HERO)
		{
			HERO->ClearTarget(pPlayer);
		}
		pPlayer->RemoveObjectEffect(PK_EFFECT_ID);
	}
	ApplyOverInfoOptionToAll();
}
void CObjectManager::SetSelectedObjectID(DWORD dwId)
{
	m_SelectedObjectID = dwId;
	if (m_SelectedObjectID == 0)
	{
		if (GAMEIN->GetMonsterGuageDlg())
			GAMEIN->GetMonsterGuageDlg()->SetActive(FALSE);
	}
}
CPet* CObjectManager::AddPet(BASEOBJECT_INFO* pBaseObjectInfo, BASEMOVE_INFO* pMoveInfo, PET_TOTALINFO* pTotalInfo, char* pMasterName)
{
	CObject* pPreObj = GetObject(pBaseObjectInfo->dwObjectID);
	ASSERT(pPreObj == NULL);
	if (pPreObj)
	{
		RemoveObject(&pPreObj);
	}
	CPet* pPet = m_PetPool.Alloc();
	pPet->Init(eObjectKind_Pet, pBaseObjectInfo);
	pPet->InitPet(pTotalInfo);
	pPet->InitObjectBalloon(ObjectBalloon_Name | ObjectBalloon_Chat);
	SafeStrCpy(pPet->m_MasterName, pMasterName, MAX_NAME_LENGTH + 1);
	pPet->SetPetMasterName(pMasterName);
	if (pTotalInfo->PetMasterUserID == gUserID)
	{
		SetHeroPet(pPet);
	}
	APPEARANCEMGR->InitAppearance(pPet);
	pMoveInfo->SetMaxTargetPosIdx(0);
	MOVEMGR->InitMove(pPet, pMoveInfo);
	OBJECTSTATEMGR->InitObjectState(pPet);
	m_ObjectTable.Add(pPet, pPet->GetID());
	ApplyOverInfoOption(pPet);
	ApplyShadowOption(pPet);
	return pPet;
}
CMapObject* CObjectManager::AddMapObject(BASEOBJECT_INFO* pBaseObjectInfo, BASEMOVE_INFO* pMoveInfo, char* strData, float fScale, float fAngle)
{
	CObject* pPreObj = GetObject(pBaseObjectInfo->dwObjectID);
	if (pPreObj)
	{
		RemoveObject(&pPreObj);
	}
	CMapObject* pMapObject = m_MapObjectPool.Alloc();
	pMapObject->Init(eObjectKind_MapObject, pBaseObjectInfo);
	pMapObject->m_RotateInfo.Angle.SetAngleDeg(fAngle);
	APPEARANCEMGR->SetMapObjectAppearance(pMapObject, strData, fScale);
	MOVEMGR->InitMove(pMapObject, pMoveInfo, fAngle);
	OBJECTSTATEMGR->InitObjectState(pMapObject);
	m_ObjectTable.Add(pMapObject, pMapObject->GetID());
	ApplyOverInfoOption(pMapObject);
	ApplyShadowOption(pMapObject);
	return pMapObject;
}
void CObjectManager::RefreshSiegeNamePannel()
{
	if (BATTLESYSTEM->GetBattle(HERO)->GetBattleKind() != eBATTLE_KIND_SIEGEWAR)
		return;
	CObject* pObject = NULL;
	m_ObjectTable.SetPositionHead();
	while (pObject = m_ObjectTable.GetData())
	{
		if (pObject->GetObjectKind() == eObjectKind_Player)
		{
			CPlayer* pPlayer = (CPlayer*)pObject;
			((CBattle_SiegeWar*)BATTLESYSTEM->GetBattle(HERO))->SetSiegeWarName(
				pPlayer, pPlayer->GetBattleTeam(), pPlayer->GetGuildIdx());
		}
	}
}
BOOL CObjectManager::TargetByName(const char* name)
{
	CObject* pObject = NULL;
	m_ObjectTable.SetPositionHead();
	while (pObject = m_ObjectTable.GetData())
	{
		if ((pObject->GetObjectKind() == eObjectKind_Player) ||
			(pObject->GetObjectKind() == eObjectKind_Npc))
		{
			if (strcmp(pObject->GetObjectName(), name) == 0)
			{
				if (pObject->GetID() == HERO->GetID())
					return FALSE;
				if (pObject->GetSingleSpecialState(eSingleSpecialState_Hide) && !HERO->IsDetect())
					return FALSE;
				SetSelectedObject(pObject);
				return TRUE;
			}
		}
	}
	return FALSE;
}
CPet* CObjectManager::FindPlayerOwnPet(DWORD CharacterIdx)
{
	CObject* pObject = NULL;
	m_ObjectTable.SetPositionHead();
	while (pObject = m_ObjectTable.GetData())
	{
		if ((pObject->GetObjectKind() == eObjectKind_Pet))
		{
			if (((CPet*)pObject)->m_dwMasterID == CharacterIdx)
				return (CPet*)pObject;
		}
	}
	return NULL;
}
void CObjectManager::HideNpc(WORD UniqueIdx, BOOL bOn)
{
	CObject* pObject = NULL;
	m_ObjectTable.SetPositionHead();
	while (pObject = m_ObjectTable.GetData())
	{
		if ((pObject->GetObjectKind() == eObjectKind_Npc))
		{
			if (((CNpc*)pObject)->GetNpcUniqueIdx() == UniqueIdx)
			{
				if (bOn)
				{
#ifdef _GMTOOL_
					if (MAINGAME->GetUserLevel() <= eUSERLEVEL_GM  && CAMERA->GetCameraMode() != eCM_EyeView)
						pObject->GetEngineObject()->SetAlpha(1.0f);
					else
#endif
						pObject->GetEngineObject()->Show();
					AddShadow(pObject);
				}
				else
				{
#ifdef _GMTOOL_
					if (MAINGAME->GetUserLevel() <= eUSERLEVEL_GM  && CAMERA->GetCameraMode() != eCM_EyeView)
						pObject->GetEngineObject()->SetAlpha(0.3f);
					else
#endif
						pObject->GetEngineObject()->HideWithScheduling();
					if (pObject->GetID() == GetSelectedObjectID())
						SetSelectedObject(NULL);
					RemoveShadow(pObject);
				}
			}
		}
	}
}
void CObjectManager::ApplySingleSpecialState(CObject* pObject, WORD State, BOOL bVal)
{
	if (pObject == HERO)
	{
		SPECIAL_STATE_INFO* pInfo = SKILLMGR->GetSpecialStateInfo(State);
		if (!pInfo)
			return;
		DWORD time = 0;
		if (pInfo->DurationTime != -1)
			time = pInfo->DurationTime;
		STATUSICONDLG->RemoveIcon(HERO, pInfo->IconIdx);
		if (bVal)	STATUSICONDLG->AddIcon(HERO, pInfo->IconIdx, 0, time);
	}
	if (bVal && pObject->GetSingleSpecialState(State))
		return;
	pObject->SetSingleSpecialState(State, bVal);
	switch (State)
	{
	case eSingleSpecialState_Hide:
	SetHide(pObject, bVal);
	break;
	case eSingleSpecialState_Detect:
	case eSingleSpecialState_DetectItem:
	SetDetect(pObject, bVal);
	break;
	}
	pObject->SetSingleSpecialState(State, bVal);
	if (!bVal)
		pObject->SetSingleSpecialStateUsedTime(State, gCurTime);
}
void CObjectManager::SetHide(CObject* pObject, BOOL bVal)
{
	BOOL bShow = FALSE;
	if (pObject->GetID() == HEROID)
		bShow = TRUE;
	if (MAINGAME->GetUserLevel() <= eUSERLEVEL_GM  && CAMERA->GetCameraMode() != eCM_EyeView)
		bShow = TRUE;
	if (HERO->IsDetect())
		bShow = TRUE;
	if (!bShow)
	{
		if (bVal)
		{
			pObject->GetEngineObject()->HideWithScheduling();
			pObject->RemoveAllObjectEffect();
			if (pObject->GetID() == GetSelectedObjectID())
				SetSelectedObject(NULL);
		}
		else
			pObject->GetEngineObject()->Show();
	}
	else
	{
		if (bVal)
		{
			pObject->GetEngineObject()->SetAlpha(0.3f);
			if (pObject->GetID() == GetSelectedObjectID())
				SetSelectedObject(NULL);
		}
		else
		{
			pObject->GetEngineObject()->SetAlpha(1.0f);
		}
	}
	ApplyShadowOption(pObject);
	ApplyOverInfoOption(pObject);
}
void CObjectManager::SetDetect(CObject* pObject, BOOL bVal)
{
	if (bVal)
	{
		if ((((CPlayer*)pObject)->GetCharacterTotalInfo()->bVisible == FALSE)
			|| (!pObject->GetSingleSpecialState(eSingleSpecialState_Hide)))
		{
			OBJECTEFFECTDESC desc(FindEffectNum("m_ba_084_R.beff"));
			pObject->AddObjectEffect(DETECT_EFFECT_ID, &desc, 1, pObject);
		}
	}
	else
	{
		pObject->RemoveObjectEffect(DETECT_EFFECT_ID);
	}
	if (pObject->GetID() != HEROID)
		return;
	if (MAINGAME->GetUserLevel() <= eUSERLEVEL_GM  && CAMERA->GetCameraMode() != eCM_EyeView)
		return;
	CObject* pObj = NULL;
	m_ObjectTable.SetPositionHead();
	while (pObj = m_ObjectTable.GetData())
	{
		if ((pObj->GetObjectKind() == eObjectKind_Player))
		{
			if (((CPlayer*)pObj)->IsHide())
			{
				if (bVal)
				{
					pObj->GetEngineObject()->SetAlpha(0.3f);
					pObj->GetEngineObject()->Show();
				}
				else
				{
					if (pObj->GetID() == GetSelectedObjectID())
						SetSelectedObject(NULL);
					pObj->GetEngineObject()->HideWithScheduling();
				}
				ApplyShadowOption(pObj);
				ApplyOverInfoOption(pObj);
			}
		}
	}
}
void CObjectManager::ChangeFlag(BYTE Category, WORD NpcKind)
{
	CObject* pObject = NULL;
	m_ObjectTable.SetPositionHead();
	while (pObject = m_ObjectTable.GetData())
	{
		if ((pObject->GetObjectKind() == eObjectKind_Npc))
		{
			if (((CNpc*)pObject)->m_pSInfo->NpcKind == NpcKind)
			{
				switch (Category)
				{
				case eSWState_Before:
				{
										if (NpcKind == 68)
										{
											pObject->GetEngineObject()->Show();
										}
										else if (NpcKind == 69 || NpcKind == 70)
										{
											pObject->GetEngineObject()->Hide();
										}
				}
				break;
				case eSWState_Proclamation:
				{
											  if (NpcKind == 68)
											  {
												  pObject->GetEngineObject()->Show();
											  }
											  else if (NpcKind == 69 || NpcKind == 70)
											  {
												  pObject->GetEngineObject()->Hide();
											  }
				}
				break;
				case eSWState_Acceptance:
				{
											if (NpcKind == 69)
											{
												pObject->GetEngineObject()->Show();
											}
											else if (NpcKind == 68 || NpcKind == 70)
											{
												pObject->GetEngineObject()->Hide();
											}
				}
				break;
				case eSWState_BeforeSiegeWar:
				{
												if (NpcKind == 69)
												{
													pObject->GetEngineObject()->Show();
												}
												else if (NpcKind == 68 || NpcKind == 70)
												{
													pObject->GetEngineObject()->Hide();
												}
				}
				break;
				case eSWState_SiegeWar:
				{
										  if (NpcKind == 70)
										  {
											  pObject->GetEngineObject()->Show();
										  }
										  else if (NpcKind == 68 || NpcKind == 69)
										  {
											  pObject->GetEngineObject()->Hide();
										  }
				}
				break;
				case eSWState_EndSiegeWar:
				{
											 if (NpcKind == 70)
											 {
												 pObject->GetEngineObject()->Show();
											 }
											 else if (NpcKind == 68 || NpcKind == 69)
											 {
												 pObject->GetEngineObject()->Hide();
											 }
				}
				}
			}
		}
	}
}
void CObjectManager::ChangeFlagNPC(int flagKind, DWORD flagIdx)
{
	if (flagKind == eGTFlg && Weekfor(eWeekforGT))
	{
		BYTE flagDeck[] = { 0, 65, 66, 67, };
		if (m_pCurGTFlg)
		{
			m_pCurGTFlg->GetEngineObject()->Hide();
			RemoveShadow(m_pCurGTFlg);
		}
		CObject* pObj = NULL;
		m_ObjectTable.SetPositionHead();
		while (pObj = m_ObjectTable.GetData())
		{
			if (pObj->GetObjectKind() == eObjectKind_Npc)
			{
				if (((CNpc*)pObj)->GetSInfo()->NpcKind == flagDeck[flagIdx])
				{
					pObj->GetEngineObject()->Show();
					AddShadow(pObj);
					m_pCurGTFlg = (CNpc*)pObj;
					break;
				}
			}
		}
	}
	else if (flagKind == eSGFlg && Weekfor(eWeekforSG))
	{
		BYTE flagDeck[] = { 0, 68, 69, 70, };
		if (m_pCurSGFlg)
		{
			m_pCurSGFlg->GetEngineObject()->Hide();
			RemoveShadow(m_pCurSGFlg);
		}
		CObject* pObj = NULL;
		m_ObjectTable.SetPositionHead();
		while (pObj = m_ObjectTable.GetData())
		{
			if (pObj->GetObjectKind() == eObjectKind_Npc)
			{
				if (((CNpc*)pObj)->GetSInfo()->NpcKind == flagDeck[flagIdx])
				{
					pObj->GetEngineObject()->Show();
					AddShadow(pObj);
					m_pCurSGFlg = (CNpc*)pObj;
					break;
				}
			}
		}
	}
}
CMapObject* CObjectManager::AddDecoration(CPlayer* pPlayer, DWORD dwStreetStallKind)
{
	CMapObject* pObject = pPlayer->GetDecorationAddress();
	if (pObject)
	{
		return NULL;
	}
	CMapObject* pMapObject = m_MapObjectPool.Alloc();
	pPlayer->SetDecorationAddress(pMapObject);
	pMapObject->SetObjectKind(eObjectKind_MapObject);
	APPEARANCEMGR->SetDecorationAppearance(pMapObject, pPlayer, dwStreetStallKind);
	pMapObject->GetEngineObject()->DisablePick();
	pPlayer->SetDecorationInfo(gCurTime, TRUE);
	ApplyOverInfoOption(pMapObject);
	ApplyShadowOption(pMapObject);
	return pMapObject;
}
void CObjectManager::SetRemoveDecorationInfo(CPlayer* pPlayer)
{
	CObject* pObject = pPlayer->GetDecorationAddress();
	if (pObject)
	{
		pPlayer->SetDecorationInfo(gCurTime, FALSE);
	}
}
void CObjectManager::RemoveDecoration(CPlayer* pPlayer)
{
	CObject* pObject = pPlayer->GetDecorationAddress();
	if (pObject)
	{
		OBJECTMGR->RemoveObject(&pObject);
		pPlayer->SetDecorationAddress(NULL);
	}
}
void CObjectManager::HideAllPet(BOOL bValue)
{
	CObject* pObject = NULL;
	m_ObjectTable.SetPositionHead();
	while (pObject = m_ObjectTable.GetData())
	{
		if ((pObject->GetObjectKind() == eObjectKind_Pet))
		{
			if (!bValue)
			{
				pObject->GetEngineObject()->HideWithScheduling();
				pObject->GetEngineObject()->Hide();
				if (pObject->GetID() == GetSelectedObjectID())
				{
					SetSelectedObject(NULL);
				}
				RemoveShadow(pObject);
				m_bHidePetFlg = TRUE;
			}
			else
			{
				pObject->GetEngineObject()->Show();
				AddShadow(pObject);
				m_bHidePetFlg = FALSE;
			}
		}
	}
}
void CObjectManager::HideAllNpc(BOOL bValue)
{
	CObject* pObject = NULL;
	m_ObjectTable.SetPositionHead();
	while (pObject = m_ObjectTable.GetData())
	{
		if (pObject->GetObjectKind() == eObjectKind_Npc)
		{
			if ((((CNpc*)pObject)->m_pSInfo->NpcKind != 65
				&& ((CNpc*)pObject)->m_pSInfo->NpcKind != 66
				&& ((CNpc*)pObject)->m_pSInfo->NpcKind != 67
				&& ((CNpc*)pObject)->m_pSInfo->NpcKind != 68
				&& ((CNpc*)pObject)->m_pSInfo->NpcKind != 69
				&& ((CNpc*)pObject)->m_pSInfo->NpcKind != 70))
			{
				if (bValue)
				{
					pObject->GetEngineObject()->HideWithScheduling();
					pObject->GetEngineObject()->Hide();
					if (pObject->GetID() == GetSelectedObjectID())
					{
						SetSelectedObject(NULL);
					}
					RemoveShadow(pObject);
					m_bHideNpcFlg = TRUE;//kiv
				}
				else
				{
					pObject->GetEngineObject()->Show();
					AddShadow(pObject);
					m_bHideNpcFlg = FALSE;//kiv
				}
			}
			else
			{
				if (bValue)
				{
					if (CAMERA->GetCameraMode() != eCM_EyeView)
					{
						pObject->GetEngineObject()->SetAlpha(0.0f);
					}
					else
					{
						pObject->GetEngineObject()->HideWithScheduling();
					}
					if (pObject->GetID() == GetSelectedObjectID())
					{
						SetSelectedObject(NULL);
					}
					RemoveShadow(pObject);
					m_bHideNpcFlg = TRUE;
				}
				else
				{
					if (CAMERA->GetCameraMode() != eCM_EyeView)
					{
						pObject->GetEngineObject()->SetAlpha(1.0f);
					}
					else
					{
						pObject->GetEngineObject()->Show();
					}
					AddShadow(pObject);
					m_bHideNpcFlg = FALSE;
				}
			}
		}
	}
}
void CObjectManager::HideAllMonster(BOOL bValue)
{
	CObject* pObject = NULL;
	m_ObjectTable.SetPositionHead();
	while (pObject = m_ObjectTable.GetData())
	{
		if ((pObject->GetObjectKind() == eObjectKind_Monster))
		{
			if (bValue)
			{
				pObject->GetEngineObject()->Hide();
				if (pObject->GetID() == GetSelectedObjectID())
				{
					SetSelectedObject(NULL);
				}
				RemoveShadow(pObject);
				m_bHideMonsterFlg = TRUE;
			}
			else
			{
				pObject->GetEngineObject()->Show();
				AddShadow(pObject);
				m_bHideMonsterFlg = FALSE;
			}
		}
	}
}
DWORD CObjectManager::GetNearMonsterByName(const char* name)
{
	DWORD dwObjectID = 0;
	float fNearDistance = 2000.0f;
	float fDistance = 0.0f;
	CObject* pObject = NULL;
	m_ObjectTable.SetPositionHead();
	char* pName = NULL;
	char* pCur = NULL;
	char* pNext = NULL;
	int nTotalLen = 0;
	int nCurLen = 0;
	char nameBuf[128];
	char tempBuf[4];
	BYTE byObjectKind = 0;
	while (pObject = m_ObjectTable.GetData())
	{
		if (pObject->GetObjectKind() <= 2) continue;
		fDistance = CalcDistanceXZ(&HERO->GetCurPosition(), &pObject->GetCurPosition());
		if (fNearDistance < fDistance) continue;
		fNearDistance = fDistance;
		dwObjectID = pObject->GetID();
		memset(nameBuf, 0, sizeof(nameBuf));
		pName = pObject->GetObjectName();
		nTotalLen = strlen(pName);
		pCur = pObject->GetObjectName();
		byObjectKind = pObject->GetObjectKind();
		for (int count = 0; count < nTotalLen;)
		{
			if (byObjectKind != 2)
			{
				if (*pCur == '^')
				{
					pNext = pCur + 1;
					if (*pNext == 's')
					{
						strcat(nameBuf, " ");
						pCur += 2;
						count += 2;
						continue;
					}
				}
			}
			else
			{
				if (*pCur == ' ')
				{
					strcat(nameBuf, " ");
					++pCur;
					++count;
					continue;
				}
			}
			if (pCur + 1 != CharNext(CharPrev(pCur, pCur + 1)))
			{
				memset(tempBuf, 0, sizeof(tempBuf));
				memcpy(tempBuf, pCur, sizeof(char)* 2);
				strcat(nameBuf, tempBuf);
				pCur += 2;
				count += 2;
			}
			else
			{
				memcpy(nameBuf, pCur, sizeof(char));
				++pCur;
				++count;
			}
		}
		if (strcmp(nameBuf, name) == 0)
		{
			dwObjectID = pObject->GetID();
			break;
		}
	}
	return dwObjectID;
}
#include "AiManager.h"
void CObjectManager::UpdateAutoTarget(float fDistances)
{
	ClearAutoTarget();
	m_ObjectTable.SetPositionHead();
	while (CObject* l_pObject = m_ObjectTable.GetData())
	{
		if (l_pObject->IsDied())
		{
			continue;
		}
		CBattle* l_pBattle = BATTLESYSTEM->GetBattle(HERO);
		if (l_pBattle->IsEnemy(HERO, l_pObject) == FALSE)
		{
			continue;
		}
		PTRLISTPOS l_posAutoTarget = m_listpAutoTarget.GetHeadPosition();
		while (1)
		{
			if (l_posAutoTarget == NULL)
			{
				m_listpAutoTarget.AddTail(l_pObject);
				break;
			}
			CObject* l_pObjectInList = (CObject*)m_listpAutoTarget.GetAt(l_posAutoTarget);
			m_listpAutoTarget.GetNext(l_posAutoTarget);
		}
	}
	return;
}
#include "AISetManager.h"
void CObjectManager::ClearAutoTarget()
{
	m_listpAutoTarget.RemoveAll();
	return;
}
void CObjectManager::UpdateAutoTarget()
{
	ClearAutoTarget();
	m_ObjectTable.SetPositionHead();
	while (CObject* l_pObject = m_ObjectTable.GetData())
	{
		if (l_pObject->IsDied())
		{
			continue;
		}
		CBattle* l_pBattle = BATTLESYSTEM->GetBattle(HERO);
		if (l_pBattle->IsEnemy(HERO, l_pObject) == FALSE)
		{
			continue;
		}
		if (GetDistance(l_pObject) >= AISETMGR->GetGameAI()->sPage5.spRange * 100)
		{
			continue;
		}
		if (AISETMGR->GetGameAI()->sPage5.cbNoMonsterWall)
		{
			VECTOR3 TempPos;
			l_pObject->GetPosition(&TempPos);
			if (PATHMGR->CheckCollisionLine(&sCPlayerAI.GetvPos(), &TempPos, HERO))
			{
				continue;
			}
		}
		PTRLISTPOS l_posAutoTarget = m_listpAutoTarget.GetHeadPosition();
		while (1)
		{
			if (l_posAutoTarget == NULL)
			{
				m_listpAutoTarget.AddTail(l_pObject);
				break;
			}
			CObject* l_pObjectInList = (CObject*)m_listpAutoTarget.GetAt(l_posAutoTarget);
			if (CompareAutoObject(l_pObjectInList, l_pObject))
			{
				m_listpAutoTarget.InsertBefore(l_posAutoTarget, l_pObject);
				break;
			}
			m_listpAutoTarget.GetNext(l_posAutoTarget);
		}
	}
	return;
}
void CObjectManager::ClearAutoFriend(void)
{
	m_listpAutoFriend.RemoveAll();
	return;
}
void CObjectManager::UpdateAutoFriend(void)
{
	ClearAutoFriend();
	m_ObjectTable.SetPositionHead();
	while (CObject* l_pObject = m_ObjectTable.GetData())
	{
		if (l_pObject->GetObjectKind() != eObjectKind_Player)
		{
			continue;
		}
		if (l_pObject == HERO)
		{
			continue;
		}
		if (l_pObject->IsDied())
		{
			continue;
		}
		CBattle* l_pBattle = BATTLESYSTEM->GetBattle(HERO);
		if (l_pBattle->IsFriend(HERO, l_pObject) == FALSE)
		{
			continue;
		}
		if (GetDistance(HERO, l_pObject) >= AUTOTARGETRANGE)
		{
			continue;
		}
		PTRLISTPOS l_posAutoFriend = m_listpAutoFriend.GetHeadPosition();
		while (1)
		{
			if (l_posAutoFriend == NULL)
			{
				m_listpAutoFriend.AddTail(l_pObject);
				break;
			}
			CObject* l_pObjectInList = (CObject*)m_listpAutoFriend.GetAt(l_posAutoFriend);
			if (CompareAutoObject(l_pObjectInList, l_pObject))
			{
				m_listpAutoFriend.InsertBefore(l_posAutoFriend, l_pObject);
				break;
			}
			m_listpAutoFriend.GetNext(l_posAutoFriend);
		}
	}
	return;
}
BOOL CObjectManager::CompareAutoObject(CObject* p_pObject1, CObject* p_pObject2)
{
	if (p_pObject1 == NULL || p_pObject2 == NULL || p_pObject1 == p_pObject2)
	{
		return FALSE;
	}
	if (GetDistance(HERO, p_pObject1) < GetDistance(HERO, p_pObject2))
	{
		return FALSE;
	}
	return TRUE;
}
float CObjectManager::GetDistance(CObject* a, CObject* b)
{
	if (a == NULL || b == NULL)
	{
		assert(0);
		return 0;
	}
	const VECTOR3 locationVector = a->GetPosition() - b->GetPosition();
	return sqrt(pow(locationVector.x, 2.0f) + pow(locationVector.y, 2.0f) + pow(locationVector.z, 2.0f));
}
float CObjectManager::GetDistance(CObject* b)
{
	if (b == NULL)
	{
		assert(0);
		return 0;
	}
	const VECTOR3 locationVector = sCPlayerAI.GetvPos() - b->GetPosition();
	return sqrt(pow(locationVector.x, 2.0f) + pow(locationVector.y, 2.0f) + pow(locationVector.z, 2.0f));
}