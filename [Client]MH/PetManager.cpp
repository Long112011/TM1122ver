#include "stdafx.h"
#include ".\petmanager.h"
#include "Pet.h"
#include "GameIn.h"
#include "ObjectManager.h"
#include "MoveManager.h"
#include "PetSpeechManager.h"
#include "ChatManager.h"
#include "ItemManager.h"
#include "CheatMsgParser.h"
#include "StatsCalcManager.h"
#include "WindowIdEnum.h"
#include "./Interface/cScriptManager.h"
#include "./interface/cDialog.h"
#include "ObjectGuagen.h"
#include "cStatic.h"
#include "PetStateDlg.h"
#include "PetStateMiniDlg.h"
#include "PetInventoryDlg.h"
#include "PetUpgradeDialog.h"
#include "PetRevivalDialog.h"
#include "CharacterDialog.h"
#include "..\[CC]Header\CommonCalcFunc.h"
#include ".\petmultidlg.h"
GLOBALTON(CPetManager)
CPetManager::CPetManager(void)
{
	m_PetInfoList.Initialize(10);
	m_PetImageList.Initialize(20);
	PetMgrInit();
}
CPetManager::~CPetManager(void)
{
}
void CPetManager::PetMgrInit()
{
	m_dwStateCheckTime = gCurTime;
	m_dwStaminaDecrease = 0;
	m_dwOldFriendShipForToolTipChange = PET_DEFAULT_FRIENDLY;
	m_dwDistCheckTime = gCurTime;
	m_bValidDistance = FALSE;
	m_bReadytoMove = FALSE;
	m_dwSkillRechargeCheckTime = gCurTime;
	m_dwSkillRechargeAmount = 0;
	m_bSkillGuageFull = FALSE;
	m_bReadyToSendSkillMsg = FALSE;
	m_dwResummonDelayTime = 0;
	m_pCurSummonPet = NULL;
	m_bIsPetStateDlgToggle = FALSE;
	m_pCurPetStateDlg = NULL;
	m_pStateDlg = NULL;
	m_pStateMiniDlg = NULL;
	m_pInvenDlg = NULL;
	m_pMultiDlg = NULL;
	m_bReadyToSendRestMsg = TRUE;
}
void CPetManager::PetMgrRelease()
{
	ReleasePetInfoList();
	PetMgrInit();
	RemovePetImage();
}
void CPetManager::InitPetInfo(PET_TOTALINFO* pPetInfo, int num)
{
	for (int i = 0; i < num; ++i)
	{
		AddPetInfo(&pPetInfo[i]);
	}
}
void CPetManager::AddPetInfo(PET_TOTALINFO* pPetInfo)
{
	PET_TOTALINFO* pInfo = m_PetInfoList.GetData(pPetInfo->PetSummonItemDBIdx);
	if (pInfo)
	{
		memcpy(pInfo, pPetInfo, sizeof(PET_TOTALINFO));
	}
	else
	{
		PET_TOTALINFO* pNewInfo = new PET_TOTALINFO;
		memcpy(pNewInfo, pPetInfo, sizeof(PET_TOTALINFO));
		m_PetInfoList.Add(pNewInfo, pNewInfo->PetSummonItemDBIdx);
	}
}
void CPetManager::RemovePetInfo(DWORD dwSummonItemDBIdx)
{
	PET_TOTALINFO* pInfo = m_PetInfoList.GetData(dwSummonItemDBIdx);
	if (!pInfo)	return;
	SAFE_DELETE(pInfo);
	m_PetInfoList.Remove(dwSummonItemDBIdx);
}
void CPetManager::ReleasePetInfoList()
{
	PET_TOTALINFO* pInfo = NULL;
	m_PetInfoList.SetPositionHead();
	while (pInfo = m_PetInfoList.GetData())
	{
		SAFE_DELETE(pInfo);
	}
	m_PetInfoList.RemoveAll();
}
PET_TOTALINFO* CPetManager::GetPetInfo(DWORD dwItemDBIdx)
{
	PET_TOTALINFO* pInfo = NULL;
	pInfo = m_PetInfoList.GetData(dwItemDBIdx);
	return pInfo;
}
void CPetManager::UpdateCurPetInfo(CPet* pPet)
{
	DWORD ItemDBIdx = pPet->m_PetTotalInfo.PetSummonItemDBIdx;
	PET_TOTALINFO* pInfo = GetPetInfo(ItemDBIdx);
	if (pInfo)
		memcpy(pInfo, &pPet->m_PetTotalInfo, sizeof(PET_TOTALINFO));
}
void CPetManager::OnPetRemove(CPet* pPet)
{
	if (!pPet->GetMaster())	return;
	else
	{
		pPet->GetMaster()->SetPet(NULL);
	}
	if (HEROID != pPet->GetMaster()->GetID())
	{
		pPet->SetMaster(NULL);
		return;
	}
	else
	{
		if (!m_pCurSummonPet)	return;
		if (!CheckCurSummonPetKindIs(ePK_EventPet))
		if (TRUE == CheckPetAlive(m_pCurSummonPet->m_PetTotalInfo.PetSummonItemDBIdx))
			UpdateCurPetInfo(pPet);
		pPet->SetMaster(NULL);
		m_pCurSummonPet = NULL;
		RefleshPetMaintainBuff();
	}
}
void CPetManager::SetResummonCheckTime()
{
	m_dwResummonDelayTime = gCurTime;
}
BOOL CPetManager::CheckResummonAvailable()
{
#ifdef _CHEATENABLE_
	if (CHEATMGR->IsCheatEnable())
		return TRUE;
#endif
	return (gCurTime - m_dwResummonDelayTime >= GAMERESRCMNGR->GetPetRule()->ResummonVilidTime);
}
DWORD CPetManager::GetPetResummonRestTime()
{
	return (GAMERESRCMNGR->GetPetRule()->ResummonVilidTime - (gCurTime - m_dwResummonDelayTime));
}
BOOL CPetManager::CheckDefaultFriendship(DWORD dwItemDBIdx)
{
	PET_TOTALINFO* pInfo = GetPetInfo(dwItemDBIdx);
	if (pInfo)
	{
		if (pInfo->PetFriendly < GAMERESRCMNGR->GetPetRule()->DefaultFriendship)
			return FALSE;
		else
			return TRUE;
	}
	return FALSE;
}
DWORD CPetManager::GetPetFriendship(DWORD dwItemDBIdx)
{
	PET_TOTALINFO* pInfo = GetPetInfo(dwItemDBIdx);
	if (!pInfo)
	{
		ASSERTMSG(0, "SummonItem have No PetInfo!");
		return 0;
	}
	return pInfo->PetFriendly;
}
BOOL CPetManager::IsCurPetStaminaFull()
{
	if (m_pCurSummonPet)
	{
		return m_pCurSummonPet->IsPetStaminaFull();
	}
	else
	{
		return FALSE;
	}
}
BOOL CPetManager::IsCurPetSummonItem(DWORD dwItemDBIdx)
{
	if (!m_pCurSummonPet)	return FALSE;
	if (m_pCurSummonPet->m_PetTotalInfo.PetSummonItemDBIdx == dwItemDBIdx)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
DWORD CPetManager::GetHeroPetID()
{
	if (GetCurSummonPet())
	{
		return GetCurSummonPet()->GetID();
	}
	else
	{
		return 0;
	}
}
BOOL CPetManager::CheckPetSummoned()
{
	if (!GetCurSummonPet())
	{
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1240));
		return FALSE;
	}
	else
		return TRUE;
}
BOOL CPetManager::CheckPetAlive(DWORD dwSummonItemDBIdx)
{
	PET_TOTALINFO* pInfo = GetPetInfo(dwSummonItemDBIdx);
	if (!pInfo)
	{
		ASSERT(0);
		return FALSE;
	}
	return pInfo->bAlive;
}
void CPetManager::NetworkMsgParse(BYTE Protocol, void* pMsg)
{
	switch (Protocol)
	{
	case MP_PET_VALUEINFO:
	{
							 MSG_DWORD4* pmsg = (MSG_DWORD4*)pMsg;
							 if (!m_pCurSummonPet) return;
							 if (pmsg->dwData4 != m_pCurSummonPet->GetID())
							 {
								 ASSERT(0);
								 return;
							 }
							 if (FALSE == CheckPetAlive(m_pCurSummonPet->m_PetTotalInfo.PetSummonItemDBIdx))
								 return;
							 m_pCurSummonPet->m_PetTotalInfo.PetStamina = pmsg->dwData1;
							 m_pCurSummonPet->m_PetTotalInfo.PetFriendly = pmsg->dwData2;
							 static DWORD Stamina = 0;
							 if (Stamina && (0 == pmsg->dwData1))
							 {
								 CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1264));
							 }
							 DWORD Friendship = pmsg->dwData2 / 1000;
							 if (m_dwOldFriendShipForToolTipChange != Friendship)
							 {
								 UpdateCurPetInfo(m_pCurSummonPet);
								 ITEMMGR->RefreshItemToolTip(m_pCurSummonPet->m_PetTotalInfo.PetSummonItemDBIdx);
								 m_dwOldFriendShipForToolTipChange = Friendship;
							 }
							 Stamina = pmsg->dwData1;
							 SetPetGuageText(pmsg->dwData1, pmsg->dwData2);
	}
		break;
	case MP_PET_STATEINFO:
	{
							 MSG_DWORD4* pmsg = (MSG_DWORD4*)pMsg;
							 CPlayer* pPlayer = (CPlayer*)OBJECTMGR->GetObject(pmsg->dwObjectID);
							 if (NULL == pPlayer)	return;
							 CPet* pPet = pPlayer->GetPet();
							 if (NULL == pPet)	return;
							 CPet* pMPet = (CPet*)OBJECTMGR->GetObject(pmsg->dwData1);
							 if (NULL == pMPet)	return;
							 if (pPet->GetID() != pmsg->dwData1)
							 {
								 ASSERTMSG(0, "Error! : Lost Pet");
							 }
							 if (pmsg->dwData2)
							 {
								 if (ePM_SKILL != pmsg->dwData2 || ePM_STAMINA_ZERO != pmsg->dwData2)
								 {
									 pPet->ChangeMotion(pmsg->dwData2, FALSE);
								 }
							 }
							 if (pmsg->dwData3)
							 {
								 char* temp = PET_SPEECHMGR->GetPetSpeech(pmsg->dwData3);
								 if (temp)
								 {
									 OBJECTMGR->ShowChatBalloon(pPet, temp);
								 }
							 }
	}
		break;
	case MP_PET_REST_ON_ACK:
	{
							   if (!m_pCurSummonPet)	return;
							   m_pCurSummonPet->SetPetRest(TRUE);
							   SetReadyToSendRestMsg(TRUE);
							   SetPetStateDlgUseRestInfo(m_pCurSummonPet);
							   RefleshPetMaintainBuff();
	}
		break;
	case MP_PET_REST_OFF_ACK:
	{
								if (!m_pCurSummonPet)	return;
								m_pCurSummonPet->SetPetRest(FALSE);
								SetReadyToSendRestMsg(TRUE);
								SetPetStateDlgUseRestInfo(m_pCurSummonPet);
								RefleshPetMaintainBuff();
	}
		break;
	case MP_PET_REST_ON_NACK:
	{
								CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1272));
								SetReadyToSendRestMsg(TRUE);
	}
		break;
	case MP_PET_REST_NACK:
	{
							 ASSERT(0);
							 SetReadyToSendRestMsg(TRUE);
	}
		break;
	case MP_PET_SUMMON_ACK:
	{
							  MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
							  CPet* pPet = (CPet*)OBJECTMGR->GetObject(pmsg->dwData);
							  if (!pPet)	return;
							  TARGETSET set;
							  set.pTarget = pPet;
							  EFFECTMGR->StartEffectProcess(eEffect_PetSummon, pPet, &set, 0, pPet->GetID());
							  if (pPet->GetMaster())
							  {
								  if (pPet->GetMaster()->GetID() == HEROID)
									  CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1259));
							  }
	}
		break;
	case MP_PET_SEAL_ACK:
	{
							MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
							CPet* pPet = (CPet*)OBJECTMGR->GetObject(pmsg->dwData);
							if (!pPet)	return;
							TARGETSET set;
							set.pTarget = pPet;
							EFFECTMGR->StartEffectProcess(eEffect_PetSeal, pPet, &set, 0, pPet->GetID());
							if (HEROID == pmsg->dwObjectID)
							{
								CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1260));
								SetPetGuageText(0, 0);
								if (!CheckCurSummonPetKindIs(ePK_EventPet))
									SetResummonCheckTime();
								ClosePetAllDlg();
								//m_pMultiDlg->SeeSlot(pPet->GetID());
							}
	}
		break;
	case MP_PET_USE_SKILL_ACK:
	{
								 SetSkillRechargeAmount(0);
								 m_bSkillGuageFull = FALSE;
								 SetReadyToSendSkillMsg(TRUE);
	}
		break;
	case MP_PET_USE_SKILL_NACK:
	{
								  SetReadyToSendSkillMsg(TRUE);
	}
		break;
	case MP_PET_ADDNEW_FROMITEM:
	{
								   SEND_ADDPET_FROMITEM* pmsg = (SEND_ADDPET_FROMITEM*)pMsg;
								   AddPetInfo(&pmsg->Info);
								   ITEMMGR->RefreshItemToolTip(pmsg->Info.PetSummonItemDBIdx);
	}
		break;
	case MP_PET_UPDATE_FRIENDSHIP:
	{
									 MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
									 PET_TOTALINFO* pInfo = GetPetInfo(pmsg->dwData1);
									 pInfo->PetFriendly = pmsg->dwData2;
	}
		break;
	case MP_PET_UPGRADE_ACK:
	{
							   GAMEIN->GetPetUpgradeDialog()->PetUpgradeAck();
							   CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1273));
	}
		break;
	case MP_PET_UPGRADE_NACK:
	{
								GAMEIN->GetPetUpgradeDialog()->PetUpgradeNack();
								CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1274));
	}
		break;
	case MP_PET_REVIVAL_ACK:
	{
							   MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
							   GAMEIN->GetPetRevivalDialog()->PetRevivalAck(pmsg->dwData);
							   CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1262));
	}
		break;
	case MP_PET_REVIVAL_NACK:
	{
								GAMEIN->GetPetRevivalDialog()->PetRevivalNack();
	}
		break;
	case MP_PET_SUMMON_RESULT:
	{
								 CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1255));
	}
		break;
	case MP_PET_MAXSTAMINA_NOTIFY:
	{
									 if (!m_pCurSummonPet)	return;
									 MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
									 m_pCurSummonPet->SetMaxStamina(pmsg->dwData);
	}
		break;
	case MP_PET_ADDINFO:
	{
						   SEND_ADDPET_FROMITEM* pmsg = (SEND_ADDPET_FROMITEM*)pMsg;
						   AddPetInfo(&pmsg->Info);
	}
		break;
	default:
		break;
	}
}
void CPetManager::PetMgrProcess()
{
	if (m_pCurSummonPet == NULL) return;
	CheckDistWithMaster();
}
void CPetManager::CalcSkillRecharge()
{
	if (!m_pCurSummonPet)	return;
	if (m_pCurSummonPet->IsPetRest())
	{
		return;
	}
	if (gCurTime - m_dwSkillRechargeCheckTime < PET_SKILLCHARGE_CHECKTIME) return;
	if (0 != m_pCurSummonPet->m_PetTotalInfo.PetStamina)
	{
		m_dwSkillRechargeAmount += m_pCurSummonPet->m_pSInfo->SkillRecharge;
	}
	else
	{
		m_dwSkillRechargeAmount = 0;
	}
	if (m_dwSkillRechargeAmount > PET_MAX_SKILL_CHARGE)
	{
		m_dwSkillRechargeAmount = PET_MAX_SKILL_CHARGE;
		m_bSkillGuageFull = TRUE;
		SetReadyToSendSkillMsg(TRUE);
	}
	SetSkillRechargeAmount(m_dwSkillRechargeAmount);
	m_dwSkillRechargeCheckTime = gCurTime;
}
void CPetManager::SetSkillRechargeAmount(DWORD amount)
{
	m_dwSkillRechargeAmount = amount;
	float SkillCharge = 0.f;
	DWORD effectTime = 0;
	if (amount)
	{
		SkillCharge = (float)m_dwSkillRechargeAmount / (float)PET_MAX_SKILL_CHARGE;
		effectTime = 1600 * amount / PET_MAX_SKILL_CHARGE;
	}
}
void CPetManager::CheckDistWithMaster()
{
	if (gCurTime - m_dwDistCheckTime < PET_DIST_CHECKTIME) return;
	float dist = CalcDistanceXZ(&m_pCurSummonPet->GetCurPosition(), &HERO->GetCurPosition());
	m_bValidDistance = (DEFAULT_PET_FOLLOW_DIST < dist);
	if ((m_bValidDistance || MOVEMGR->IsMoving(m_pCurSummonPet)) && m_bReadytoMove)
	{
		MOVEMGR->SendPetMoveMsg();
	}
	m_dwDistCheckTime = gCurTime;
}
void CPetManager::OpenPetStateDlg()
{
	if (!CheckPetSummoned())
		return;
	if (m_pCurPetStateDlg && m_pCurPetStateDlg->IsActive())
		return;
	if (!m_bIsPetStateDlgToggle)
	{
		//if (m_pMultiDlg)	m_pMultiDlg->SetActive(FALSE);
		if (m_pStateMiniDlg)	m_pStateMiniDlg->SetActive(FALSE);
		m_pCurPetStateDlg = (cDialog*)m_pStateDlg;
	}
	else
	{
		if (m_pStateDlg)	m_pStateDlg->SetActive(FALSE);
		m_pCurPetStateDlg = (cDialog*)m_pStateMiniDlg;
		//m_pCurPetStateDlg = (cDialog*)m_pMultiDlg;
	}
	if (m_pCurPetStateDlg)
		m_pCurPetStateDlg->SetActive(TRUE);
}
void CPetManager::OpenPetInvenDlg()
{
	if (!CheckPetSummoned())
		return;
	if (!m_pInvenDlg)	return;
	if (m_pInvenDlg->IsItemInfoInit())
	{
		if (m_pInvenDlg)
		{
			if (!m_pInvenDlg->IsActive())
			{
				SetPetValidInvenTab();
				m_pInvenDlg->SetActive(TRUE);
			}
			else
				m_pInvenDlg->SetActive(FALSE);
		}
	}
	else
	{
		MSG_DWORD msg;
		msg.Category = MP_ITEM;
		msg.Protocol = MP_ITEM_PETINVEN_INFO_SYN;
		msg.dwObjectID = HEROID;
		msg.dwData = GetHeroPetID();
		NETWORK->Send(&msg, sizeof(msg));
	}
}
void CPetManager::TogglePetStateDlg()
{
	if (!m_pStateDlg || !m_pStateMiniDlg)	return;
	LONG absX = LONG(m_pStateDlg->GetAbsX());
	LONG absY = LONG(m_pStateDlg->GetAbsY());
	if (m_pCurPetStateDlg)
	{
		absX = LONG(m_pCurPetStateDlg->GetAbsX());
		absY = LONG(m_pCurPetStateDlg->GetAbsY());
		m_pCurPetStateDlg->SetActive(FALSE);
	}
	if (!m_bIsPetStateDlgToggle)
	{
		m_pCurPetStateDlg = (cDialog*)m_pStateMiniDlg;
	}
	else
	{
		m_pCurPetStateDlg = (cDialog*)m_pStateDlg;
	}
	m_pCurPetStateDlg->SetAbsXY(absX, absY);
	m_pCurPetStateDlg->SetActive(TRUE);
	m_bIsPetStateDlgToggle = !m_bIsPetStateDlgToggle;
	return;
}
void CPetManager::SetPetStateDlgInfo(CPet* pPet)
{
	if (!pPet)	return;
	BASE_PET_LIST* pList = pPet->m_pSInfo;
	PET_TOTALINFO* pInfo = &pPet->m_PetTotalInfo;
	if (!pList || !pInfo)	return;
	if (!m_pStateDlg)	return;
	cStatic* pName = (cStatic*)m_pStateDlg->GetNameTextWin();
	cStatic* pNameMini = (cStatic*)m_pStateMiniDlg->GetNameTextWin();
	if (pName && pNameMini)
	{
		pName->SetStaticText(pList->Name);
		pNameMini->SetStaticText(pList->Name);
	}
	cStatic* pGrade = (cStatic*)m_pStateDlg->GetGradeTextWin();
	if (pGrade)
	{
		char szValue[32];
		sprintf(szValue, "%d", pInfo->PetGrade);
		pGrade->SetStaticText(szValue);
	}
	cStatic* p2DImage = (cStatic*)m_pStateDlg->Get2DImageWin();
	if (p2DImage)
	{
		p2DImage->SetBasicImage(GetPetImage(pList->PetKind));
	}
	if (pList->BuffList)
	{
		PET_BUFF_LIST* pBuffList = NULL;
		char BuffName[32] = { 0, };
		for (int i = 0; i < MAX_PET_BUFF_NUM; ++i)
		{
			cStatic** pText = (cStatic**)m_pStateDlg->GetPetBuffTextWin();
			WORD BuffIdx = pList->BuffList[pInfo->PetGrade - 1][i];
			if (0 == BuffIdx)
			{
				if (pText[i])
				{
					pText[i]->SetStaticText("");
				}
				continue;
			}
			pBuffList = GAMERESRCMNGR->GetPetBuffInfo(BuffIdx);
			if (pBuffList && pBuffList->BuffValueData)
			{
				if (pBuffList->BuffKind == ePB_Demage_Percent)
				{
					wsprintf(BuffName, CHATMGR->GetChatMsg(1270), pBuffList->BuffValueData);
				}
				else if (pBuffList->BuffKind == ePB_Dodge)
				{
					wsprintf(BuffName, CHATMGR->GetChatMsg(1271), pBuffList->BuffSuccessProb);
				}
				else if (pBuffList->BuffKind == ePB_MasterAllStatUp)
				{
					wsprintf(BuffName, CHATMGR->GetChatMsg(1286), pBuffList->BuffValueData);
				}
				else if (pBuffList->BuffKind == ePB_Item_DoubleChance)
				{
					wsprintf(BuffName, CHATMGR->GetChatMsg(1305), pBuffList->BuffSuccessProb);
				}
				else if (pBuffList->BuffKind == ePB_NoForeAtkMonster)
				{
					wsprintf(BuffName, CHATMGR->GetChatMsg(1350));
				}
				else if (pBuffList->BuffKind == ePB_ReduceCriticalDmg)
				{
					wsprintf(BuffName, CHATMGR->GetChatMsg(1388), pBuffList->BuffValueData);
				}
				else if (pBuffList->BuffKind == ePB_MasterAllStatRound)
				{
					wsprintf(BuffName, CHATMGR->GetChatMsg(1389));
				}
				else if (pBuffList->BuffKind == ePB_Item_RareProbUp)
				{
					wsprintf(BuffName, CHATMGR->GetChatMsg(1476));
				}
				else if (pBuffList->BuffKind == ePB_MussangTimeIncrease)
				{
					wsprintf(BuffName, CHATMGR->GetChatMsg(1477));
				}
			}
			if (pText[i])
			{
				pText[i]->SetStaticText(BuffName);
			}
		}
	}
}
void CPetManager::SetPetStateDlgUseRestInfo(CPet* pPet)
{
	if (!pPet)	return;
	cStatic* pState = m_pStateDlg->GetUseRestTextWin();
	cStatic* pStateMini = m_pStateMiniDlg->GetUseRestTextWin();
	if (pState && pStateMini)
	{
		if (pPet->IsPetRest())
		{
			pState->SetStaticText(CHATMGR->GetChatMsg(1257));
			pStateMini->SetStaticText(CHATMGR->GetChatMsg(1257));
		}
		else
		{
			pState->SetStaticText(CHATMGR->GetChatMsg(1256));
			pStateMini->SetStaticText(CHATMGR->GetChatMsg(1256));
		}
	}
}
void CPetManager::ClosePetAllDlg()
{
	m_pStateDlg->SetActive(FALSE);
	m_pStateMiniDlg->SetActive(FALSE);
	m_pInvenDlg->SetActive(FALSE);
}
void CPetManager::SetPetGuageText(DWORD dwStamina, DWORD dwFriendShip)
{
	if (!m_pCurSummonPet)	return;
	char szValue[32];
	if (0 < dwFriendShip && dwFriendShip <= 100000)
	{
		sprintf(szValue, "%d%%", 1);
	}
	else
	{
		sprintf(szValue, "%d%%", dwFriendShip / 100000);
	}
#ifdef _CHEATENABLE_
	if (CHEATMGR->IsCheatEnable())
	{
		sprintf(szValue, "%.3f", (float)dwFriendShip / 100000);
	}
#endif
	((cStatic*)m_pStateDlg->GetFriendShipTextWin())->SetStaticText(szValue);
	((cStatic*)m_pStateMiniDlg->GetFriendShipTextWin())->SetStaticText(szValue);
	sprintf(szValue, "%d / %d", dwStamina, m_pCurSummonPet->GetMaxStamina());
	((cStatic*)m_pStateDlg->GetStaminaTextWin())->SetStaticText(szValue);
	((cStatic*)m_pStateMiniDlg->GetStaminaTextWin())->SetStaticText(szValue);
	float stamina = float(m_pCurSummonPet->m_PetTotalInfo.PetStamina)
		/ float(m_pCurSummonPet->GetMaxStamina());
	float friendShip = float(m_pCurSummonPet->m_PetTotalInfo.PetFriendly)
		/ float(GAMERESRCMNGR->GetPetRule()->MaxFriendship);
	((CObjectGuagen*)m_pStateDlg->GetFriendShipGuage())->SetValue(friendShip, 0);
	((CObjectGuagen*)m_pStateMiniDlg->GetFriendShipGuage())->SetValue(friendShip, 0);
	((CObjectGuagen*)m_pStateDlg->GetStaminaGuage())->SetValue(stamina, 0);
	((CObjectGuagen*)m_pStateMiniDlg->GetStaminaGuage())->SetValue(stamina, 0);
}
void CPetManager::SetPetStateMiniDlgInfo(CPet* pPet)
{
	if (!pPet)	return;
	cStatic* pInvenNum = (cStatic*)m_pStateDlg->GetInvenNumTextWin();
	PET_TOTALINFO* pInfo = &pPet->m_PetTotalInfo;
	BASE_PET_LIST* pList = GAMERESRCMNGR->GetPetListInfo(pPet->GetPetIdx());
	/*for (int i = 0; i < MAX_PET_BUFF_NUM; ++i)
	{
		if (i == m_pMultiDlg->InitPet(i, pPet->GetID()))
		{
			float stamina = float(m_pCurSummonPet->m_PetTotalInfo.PetStamina)
				/ float(m_pCurSummonPet->GetMaxStamina());
			cStatic* pNameMini = (cStatic*)m_pMultiDlg->GetNameTextWin();
			if (pNameMini)
			{
				pNameMini->SetStaticText(pList->Name);
			}
			cStatic** p2D = (cStatic**)m_pMultiDlg->Get2DImageWin();
			cGuagen** pGuage = (cGuagen**)m_pMultiDlg->GetStaminaGuage();
			p2D[i]->SetBasicImage(GetPetImage(pList->PetKind));
			pGuage[i]->SetValue(stamina);
			break;
		}
	}*/
	if (pInvenNum)
	{
		char szValue[32];
		DWORD count = pList->InventoryTapNum[pInfo->PetGrade - 1];
		if (count)
		{
			wsprintf(szValue, "%d", count);
		}
		else
		{
			wsprintf(szValue, "%s", CHATMGR->GetChatMsg(249));
		}
		pInvenNum->SetStaticText(szValue);
	}
}
DWORD CPetManager::GetPetValidInvenMaxTabNum()
{
	if (!m_pCurSummonPet)	return 0;
	BASE_PET_LIST* pPetList = NULL;
	pPetList = m_pCurSummonPet->GetSInfo();
	return pPetList->InventoryTapNum[m_pCurSummonPet->GetPetCurGrade() - 1];
}
void CPetManager::SetPetValidInvenTab()
{
	if (!m_pInvenDlg)	return;
	DWORD ValidTabNum = GetPetValidInvenMaxTabNum();
	m_pInvenDlg->ApplyValidInvenTab(ValidTabNum);
}
void CPetManager::SetCurPetRest(BOOL bRest)
{
	if (!CheckPetSummoned())
	{
		ASSERT(0);
		return;
	}
	m_pCurSummonPet->SetPetRest(bRest);
}
void CPetManager::SendPetRestMsg(BOOL bRest)
{
	if (!CheckPetSummoned())
		return;
	if (!IsReadyToSendRestMsg())
		return;
	if (0 == m_pCurSummonPet->m_PetTotalInfo.PetStamina)
	{
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1272));
		return;
	}
	if (m_pCurSummonPet->IsPetRest() == bRest) return;
	MSGBASE msg;
	msg.Category = MP_PET;
	if (bRest)
		msg.Protocol = MP_PET_REST_ON;
	else
		msg.Protocol = MP_PET_REST_OFF;
	msg.dwObjectID = HERO->GetID();
	NETWORK->Send(&msg, sizeof(msg));
	SetReadyToSendRestMsg(FALSE);
}
void CPetManager::SendPetSealMsg()
{
	if (!CheckPetSummoned())
	{
		return;
	}
	MSGBASE msg;
	msg.Category = MP_PET;
	msg.Protocol = MP_PET_SEAL_SYN;
	msg.dwObjectID = HERO->GetID();
	NETWORK->Send(&msg, sizeof(msg));
}
void CPetManager::CheckRestNSkillUse()
{
	if (!CheckPetSummoned())
		return;
	if (m_pCurSummonPet->IsPetRest())
	{
		SendPetRestMsg(FALSE);
		UseCurPetSkill();
	}
	else
	{
		UseCurPetSkill();
	}
}
void CPetManager::InitPetSkillGuage()
{
	SetSkillRechargeAmount(0);
	SetSkillGuageFull(FALSE);
}
void CPetManager::UseCurPetSkill()
{
	if (!CheckPetSummoned())
		return;
	if (!IsSkillGuageFull())
	{
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1241));
	}
	if (!IsReadyToSendSkillMsg())
		return;
	MSGBASE msg;
	msg.Category = MP_PET;
	msg.Protocol = MP_PET_USE_SKILL_SYN;
	msg.dwObjectID = HERO->GetID();
	NETWORK->Send(&msg, sizeof(msg));
	SetReadyToSendSkillMsg(FALSE);
}
void CPetManager::AddMasterStatFromPetBuff(player_calc_stats* pMasterAdditionalStat)
{
	CPet* pCurPet = GetCurSummonPet();
	if (NULL == pCurPet)	return;
	if (pCurPet->IsPetRest())	return;
	BASE_PET_LIST* pInfo = pCurPet->GetSInfo();
	for (int i = 0; i < 3; ++i)
	{
		WORD BuffIdx = pInfo->BuffList[m_pCurSummonPet->GetPetCurGrade() - 1][i];
		PET_BUFF_LIST* pList = GAMERESRCMNGR->GetPetBuffInfo(BuffIdx);
		if (!pList)	continue;
		if (pList->BuffKind == ePB_MasterAllStatUp)
		{
			WORD AddStat = (WORD)pList->BuffValueData;
			pMasterAdditionalStat->GenGol += AddStat;
			pMasterAdditionalStat->MinChub += AddStat;
			pMasterAdditionalStat->CheRyuk += AddStat;
			pMasterAdditionalStat->SimMaek += AddStat;
		}
		if (pList->BuffKind == ePB_MasterAllStatRound)
		{
			pMasterAdditionalStat->GenGol += GetAddStatFromRound(HERO->GetGenGol());
			pMasterAdditionalStat->MinChub += GetAddStatFromRound(HERO->GetMinChub());
			pMasterAdditionalStat->CheRyuk += GetAddStatFromRound(HERO->GetCheRyuk());
			pMasterAdditionalStat->SimMaek += GetAddStatFromRound(HERO->GetSimMek());
		}
	}
}
void CPetManager::RefleshPetMaintainBuff()
{
	STATSMGR->CalcItemStats(HERO);
	GAMEIN->GetCharacterDialog()->RefreshInfo();
}
WORD CPetManager::GetPetEquipItemMax(DWORD dwItemDBIdx)
{
	PET_TOTALINFO* pInfo = GetPetInfo(dwItemDBIdx);
	if (!pInfo)	return 0;
	return pInfo->PetGrade;
}
void CPetManager::LoadPetImage()
{
	BASE_PET_LIST* pPetList = NULL;
	int count = 1;
	while (pPetList = GAMERESRCMNGR->GetPetListInfo(count))
	{
		cImage* pImage = new cImage;
		SCRIPTMGR->GetImage(pPetList->Pet2DIdx, pImage, PFT_ITEMPATH);
		ASSERT(!m_PetImageList.GetData(pPetList->PetKind));
		m_PetImageList.Add(pImage, pPetList->PetKind);
		++count;
	}
}
cImage* CPetManager::GetPetImage(DWORD dwPetIdx)
{
	return m_PetImageList.GetData(dwPetIdx);
}
void CPetManager::RemovePetImage()
{
	cImage* pImage = NULL;
	m_PetImageList.SetPositionHead();
	while (pImage = m_PetImageList.GetData())
	{
		delete pImage;
		pImage = NULL;
	}
	m_PetImageList.RemoveAll();
}
void CPetManager::SetCurSummonPetKind(CPet* pPet)
{
	WORD PetListIdx = pPet->GetPetKind();
	if (PetListIdx == CRISTMAS_EVENTPET)
	{
		m_wPetKind = ePK_EventPet;
	}
	else
	{
		m_wPetKind = ePK_None;
	}
}
BOOL CPetManager::CheckCurSummonPetKindIs(int kind)
{
	BOOL rt = (m_wPetKind & kind);
	return rt;
}