#include "stdafx.h"
#if defined(_MUTIPET_)


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
#include "PetMixDlg.h"
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
#include "AIManager.h"

GLOBALTON(CPetManager)

CPetManager::CPetManager(void)
{
	//	m_HeroOwnPetInfo.Initialize(5);
	m_PetInfoList.Initialize(10);
	//m_PetImageList.Initialize(20);

	PetMgrInit();
}

CPetManager::~CPetManager(void)
{
}

void CPetManager::PetMgrInit()
{
	m_dwStateCheckTime = gCurTime;
	m_dwStaminaDecrease = 0;
	m_dwOldFriendShipForToolTipChange[0] = PET_DEFAULT_FRIENDLY;
	m_dwOldFriendShipForToolTipChange[1] = PET_DEFAULT_FRIENDLY;
	m_dwOldFriendShipForToolTipChange[3] = PET_DEFAULT_FRIENDLY;
	m_dwDistCheckTime = gCurTime;
	m_bValidDistance = FALSE;
	m_dwSkillRechargeCheckTime = gCurTime;
	m_dwSkillRechargeAmount = 0;
	m_bSkillGuageFull = FALSE;
	m_bReadyToSendSkillMsg = FALSE;
	m_dwResummonDelayTime = 0;
	SelectPet = NULL;
	m_pCurSummonPet[0] = NULL;
	m_pCurSummonPet[1] = NULL;
	m_pCurSummonPet[2] = NULL;
	m_pStateDlg = NULL;
	m_pStateMiniDlg = NULL;
	m_pInvenDlg = NULL;
	m_bReadyToSendRestMsg = TRUE;
}


void CPetManager::PetMgrRelease()
{
	ReleasePetInfoList();
	PetMgrInit();
	//RemovePetImage();
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
		pPet->GetMaster()->RemovePet(pPet);
	}

	if (HEROID != pPet->GetMaster()->GetID())
	{
		pPet->SetMaster(NULL);
		return;
	}
	else
	{
		CPet* Pet = NULL;
		int i = 0;
		for (; i < 3; ++i)
		{
			if (m_pCurSummonPet[i] == pPet)
			{
				Pet = m_pCurSummonPet[i];
				break;
			}
		}
		if (!Pet)	return;
		if (!CheckCurSummonPetKindIs(Pet, ePK_EventPet))
			if (TRUE == CheckPetAlive(Pet->m_PetTotalInfo.PetSummonItemDBIdx))
				UpdateCurPetInfo(pPet);

		pPet->SetMaster(NULL);

		m_pCurSummonPet[i] = NULL;

		m_pStateMiniDlg->RemovePetIcon(i);
		for (int i = 0; i < 3; ++i)
		{
			if (m_pCurSummonPet[i])
			{
				SetSelectPet(i);
				break;
			}
		}
		if (!IsSummonPet())
		{
			ClosePetAllDlg();
		}
		STATSMGR->CalcItemStats(HERO);
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

BOOL CPetManager::IsCurPetSummonItem(DWORD dwItemDBIdx)
{
	BOOL b = FALSE;
	for (int i = 0; i < 3; ++i)
	{
		if (m_pCurSummonPet[i])
		{
			if (m_pCurSummonPet[i]->m_PetTotalInfo.PetSummonItemDBIdx == dwItemDBIdx)
			{
				b = TRUE;
			}
			else
			{
				continue;
			}
		}
	}
	return b;


}

void CPetManager::SetSelectPet(WORD i)
{
	if (m_pCurSummonPet[i])
	{
		SelectPet = m_pCurSummonPet[i];
	}
	if (SelectPet)
	{
		SetPetStateDlgInfo(SelectPet);
	}
}
BOOL CPetManager::IsCurPetFull()
{
	for (int i = 0; i < 3; ++i)
	{
		if (m_pCurSummonPet[i] == NULL)
		{
			return FALSE;
		}
	}
	return TRUE;
}

BOOL CPetManager::IsSummonPet(CPet* pPet)
{
	if (pPet)
	{
		for (int i = 0; i < 3; ++i)
		{
			if (m_pCurSummonPet[i] == pPet)
			{
				return TRUE;
			}
		}
		return FALSE;
	}
	for (int i = 0; i < 3; ++i)
	{
		if (m_pCurSummonPet[i])
		{
			return TRUE;
		}
	}
	return FALSE;
}
DWORD CPetManager::GetHeroPetID()
{
	for (int i = 0; i < 3; ++i)
	{
		if (m_pCurSummonPet[i])
		{
			return m_pCurSummonPet[i]->GetID();
		}
	}
	return 0;
}

BOOL CPetManager::CheckPetSummoned()
{
	CPet* Pet = NULL;
	for (int i = 0; i < 3; ++i)
	{
		if (m_pCurSummonPet[i])
		{
			Pet = m_pCurSummonPet[i];
			break;
		}
	}
	if (!Pet)
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

void CPetManager::NetworkMsgParse(WORD Protocol, void* pMsg)
{
	switch (Protocol)
	{
	case MP_PET_VALUEINFO:
	{
		MSG_DWORD4* pmsg = (MSG_DWORD4*)pMsg;
		CPet* Pet = NULL;
		int i = 0;
		PET_TOTALINFO* pInfo = GetPetInfo(pmsg->dwData1);
		if (!pInfo)
		{
			return;
		}
		for (; i < 3; ++i)
		{
			if (m_pCurSummonPet[i])
				if (m_pCurSummonPet[i]->GetID() == pmsg->dwData4)
				{
					Pet = m_pCurSummonPet[i];
					break;
				}
		}
		if (!Pet)
		{
			if (pInfo)
			{
				pInfo->PetFriendly = pmsg->dwData2;
				float friendShip = float(pInfo->PetFriendly)
					/ float(GAMERESRCMNGR->GetPetRule()->MaxFriendship);
				ITEMMGR->RefreshItemToolTip(pmsg->dwData1);
			}
			return;
		}

		if (FALSE == CheckPetAlive(Pet->m_PetTotalInfo.PetSummonItemDBIdx))
			return;

		Pet->m_PetTotalInfo.PetFriendly = pmsg->dwData2;

		DWORD Friendship = pmsg->dwData2 / 1000;
		if (m_dwOldFriendShipForToolTipChange[i] != Friendship)
		{
			UpdateCurPetInfo(Pet);

			ITEMMGR->RefreshItemToolTip(Pet->m_PetTotalInfo.PetSummonItemDBIdx);
			m_dwOldFriendShipForToolTipChange[i] = Friendship;
		}
		if (pInfo->Pos > 4 && pInfo->Pos < 6)
		{
			float friendShip = float(Pet->m_PetTotalInfo.PetFriendly)
				/ float(GAMERESRCMNGR->GetPetRule()->MaxFriendship);
		}
		SetPetGuageText(Pet, pmsg->dwData1, pmsg->dwData2);

	}
	break;
	case MP_PET_STATEINFO:
	{
		MSG_DWORD4* pmsg = (MSG_DWORD4*)pMsg;

		CPlayer* pPlayer = (CPlayer*)OBJECTMGR->GetObject(pmsg->dwObjectID);
		if (NULL == pPlayer)	return;

		CPet* pMPet = (CPet*)OBJECTMGR->GetObject(pmsg->dwData1);
		if (NULL == pMPet)	return;
		CPet* pPet = NULL;
		if (pPlayer->IsSummonPet(pMPet))
			pPet = pMPet;
		if (NULL == pPet)	return;
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
			//to do
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
		// 			if(!m_pCurSummonPet)	return;
		// 			m_pCurSummonPet->SetPetRest(TRUE);
		// 			SetReadyToSendRestMsg(TRUE);
		// 			//!!!¹öÆ° ¼³Á¤ È®ÀÎ (´ÜÃàÅ°·Î ÈÞ½Ä¼³Á¤ÀÌ °¡´É)
		// 			//m_pStateDlg->SetBtnClick(ePetRestBtn);
		// 			SetPetStateDlgUseRestInfo(m_pCurSummonPet);
		// 
		// 			//Æê Áö¹öÇÁ °ü·Ã Ãß°¡ ½ºÅÈ °»½Å
		// 			RefleshPetMaintainBuff();
	}
	break;
	case MP_PET_REST_OFF_ACK:
	{
		// 			if(!m_pCurSummonPet)	return;
		// 			m_pCurSummonPet->SetPetRest(FALSE);
		// 			SetReadyToSendRestMsg(TRUE);
		// 			//m_pStateDlg->SetBtnClick(ePetUseBtn);
		// 			SetPetStateDlgUseRestInfo(m_pCurSummonPet);
		// 
		// 			//Æê Áö¹öÇÁ °ü·Ã Ãß°¡ ½ºÅÈ °»½Å
		// 			RefleshPetMaintainBuff();
	}
	break;
	case MP_PET_REST_ON_NACK:
	{
		// 			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1272) );
		// 			SetReadyToSendRestMsg(TRUE);
	}
	break;
	case MP_PET_REST_NACK:
	{
		//sync error
		// 			ASSERT(0);
		// 			SetReadyToSendRestMsg(TRUE);
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
		STATSMGR->CalcItemStats(HERO);
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
			SetPetGuageText(pPet, 0, 0);

			if (!CheckCurSummonPetKindIs(pPet, ePK_EventPet))
				SetResummonCheckTime();

		}
		STATSMGR->CalcItemStats(HERO);
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

		//ITEMMGR->RefreshItemToolTip(pInfo->PetSummonItemDBIdx);
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
		//pmsg->dwData 0Àº ¾ÆÀÌÅÛ¸ôºÎÈ°, 1,2,3Àº °ÔÀÓ³» ´Ü°èº°
		GAMEIN->GetPetRevivalDialog()->PetRevivalAck(pmsg->dwData);
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1262));
		//Æê ½ºÅ³°ÔÀÌÁö ÃÊ±âÈ­
		//Æê ÈÞ½Ä»óÅÂ
		// 			m_dwSkillRechargeAmount = 0;
		// 			m_bSkillGuageFull = FALSE;
		// 			SetReadyToSendSkillMsg(FALSE);
	}
	break;
	case MP_PET_REVIVAL_NACK:
	{
		GAMEIN->GetPetRevivalDialog()->PetRevivalNack();
	}
	break;
	// 	case MP_PET_FEED_FAILED:
	// 		{
	// 			MSG_BYTE* pmsg = (MSG_BYTE*)pMsg;
	// 			switch(pmsg->bData)
	// 			{
	// 			case ePFR_Unsummoned:
	// 				{
	// 					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1253) );
	// 				}
	// 				break;
	// 			case ePFR_StaminaFull:
	// 				{
	// 				}
	// 				break;
	// 			default:
	// 				break;
	// 			}
	// 		}
	// 		break;
	case MP_PET_SUMMON_RESULT:
	{
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1255));
	}
	break;
	case MP_PET_MAXSTAMINA_NOTIFY:
	{
		// 			//½ºÅÂ¹Ì³ª ÃÖ´ë°ª º¯°æ.
		// 			if(!m_pCurSummonPet)	return;
		// 			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
		// 			m_pCurSummonPet->SetMaxStamina(pmsg->dwData);
	}
	break;
	case MP_PET_ADDINFO:
	{
		SEND_ADDPET_FROMITEM* pmsg = (SEND_ADDPET_FROMITEM*)pMsg;
		AddPetInfo(&pmsg->Info);
	}
	break;
	case MP_PET_MixAck:
	{
		MSG_DWORD3* pmsg = (MSG_DWORD3*)pMsg;
		GAMEIN->GetPetMixDlg()->MixAck(pmsg);
	}
	break;
	default:
		break;
	}

}


void CPetManager::PetMgrProcess()
{
	CPet* Pet = NULL;
	for (int i = 0; i < 3; ++i)
	{
		if (m_pCurSummonPet[i])
		{
			Pet = m_pCurSummonPet[i];
		}
	}
	if (!Pet)	return;
	CheckDistWithMaster();

	//m_pCurSummonPet->CalcStamina();
	//CalcSkillRecharge();
}

void CPetManager::CalcSkillRecharge()
{
	// 	if(!m_pCurSummonPet)	return;
	// 
	// 	if(m_pCurSummonPet->IsPetRest())
	// 	{
	// 		//m_dwSkillRechargeCheckTime = gCurTime;
	// 		return;
	// 	}
	// 
	//     if( gCurTime - m_dwSkillRechargeCheckTime < PET_SKILLCHARGE_CHECKTIME ) return;
	// 
	// 	if( 0 != m_pCurSummonPet->m_PetTotalInfo.PetStamina )
	// 	{	
	// 		m_dwSkillRechargeAmount += m_pCurSummonPet->m_pSInfo->SkillRecharge;
	// 	}
	// 	else
	// 	{
	// 		m_dwSkillRechargeAmount = 0;
	// 	}
	// 
	// 	if( m_dwSkillRechargeAmount > PET_MAX_SKILL_CHARGE )
	// 	{
	// 		m_dwSkillRechargeAmount = PET_MAX_SKILL_CHARGE;
	// 
	// 		m_bSkillGuageFull = TRUE;
	// 		SetReadyToSendSkillMsg(TRUE);
	// 	}
	// 
	// 	SetSkillRechargeAmount(m_dwSkillRechargeAmount);

	//	float SkillCharge = (float)m_dwSkillRechargeAmount / (float)PET_MAX_SKILL_CHARGE;
	//	((CObjectGuagen*)m_pStateDlg->GetWindowForID(PS_GUAGE3))->SetValue( SkillCharge, 0 );
	//	((CObjectGuagen*)m_pStateMiniDlg->GetWindowForID(PSMN_GUAGE3))->SetValue( SkillCharge, 0 );

	//	m_dwSkillRechargeCheckTime = gCurTime;
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

	//((CObjectGuagen*)m_pStateDlg->GetWindowForID(PS_GUAGE3))->SetValue( SkillCharge, 0 );
	//((CObjectGuagen*)m_pStateMiniDlg->GetWindowForID(PSMN_GUAGE3))->SetValue( SkillCharge, 0 );
	//	((CObjectGuagen*)m_pStateDlg->GetWindowForID(PS_GUAGE3))->SetValue( SkillCharge, effectTime );
	//	((CObjectGuagen*)m_pStateMiniDlg->GetWindowForID(PSMN_GUAGE3))->SetValue( SkillCharge, effectTime );
}
void CPetManager::CheckDistWithMaster()
{
	//	if( !m_pCurSummonPet ) return;
	//1ÃÊ¸¶´Ù È÷¾î·ÎÀÇ ÇöÀç À§Ä¡¸¦ ÆÖ À§Ä¡¿Í ºñ±³ÇÏ°í
	//À¯È¿ °Å¸® ÀÌ»óÀÌ¸é
	//È÷¾î·Î ³ëµå·Î ºÎÅÍ Æê ³ëµå¸¦ ±¸ÇØ
	//¼­¹ö·Î Á¤º¸ º¸³»°í
	//Æê ÀÌµ¿½ÃÀÛÇÑ´Ù.
	if (gCurTime - m_dwDistCheckTime < PET_DIST_CHECKTIME) return;

	for (int i = 0; i < 3; ++i)
	{
		if (m_pCurSummonPet[i])
		{
			float dist = CalcDistanceXZ(&m_pCurSummonPet[i]->GetCurPosition(), &HERO->GetCurPosition());
			m_bValidDistance = (DEFAULT_PET_FOLLOW_DIST < dist);

			if ((m_bValidDistance || MOVEMGR->IsMoving(m_pCurSummonPet[i])) && m_pCurSummonPet[i]->IsReadytoMove()/* && m_bReadytoMove*/)
			{
				VECTOR3* v;
				WORD count = m_pCurSummonPet[i]->m_MoveInfo.GetMaxTargetPosIdx();
				if (0 == count) continue;
				else if (1 == count)
				{
					v = m_pCurSummonPet[i]->m_MoveInfo.GetTargetPosition(0);
					v->z += i * 200;

				}
				else
				{
					v = m_pCurSummonPet[i]->m_MoveInfo.GetTargetPosition(count - 1);
					v->z += i * 200;
				}
				MOVEMGR->SendPetMoveMsg(m_pCurSummonPet[i]);

			}
			if (!HERO->GetBaseMoveInfo()->bMoving && MOVEMGR->IsMoving(m_pCurSummonPet[i]))
				MOVEMGR->MoveStop(m_pCurSummonPet[i]);


		}
	}


	m_dwDistCheckTime = gCurTime;
	/*
	if( MAX_LIMIT_PET_DIST < dist )
	{
	//¿öÇÁ¸Þ½ÃÁö. ¼­¹ö¿¡¼­ ¼ø°£ÀÌµ¿ Ã³¸®.
	MOVE_POS	msg;
	msg.Category = MP_MOVE;
	msg.Protocol = MP_MOVE_PET_WARP_SYN;
	msg.dwObjectID = HEROID;
	msg.dwMoverID = m_pCurSummonPet->GetID();
	MOVE_INFO* pMoveInfo = HERO->GetBaseMoveInfo();
	msg.cpos.Compress(&pMoveInfo->CurPosition);
	NETWORK->SetRAM();NETWORK->Send(&msg,sizeof(msg));

	//SetPosition(pPet,&pHeroMoveInfo->CurPosition);
	}*/


}

void CPetManager::OpenPetStateDlg()
{
	if (!CheckPetSummoned())
		return;
	if (!m_pStateMiniDlg->IsActive())
		m_pStateMiniDlg->SetActive(TRUE);

	// 		if(!m_pStateDlg->IsActive())
	// 			m_pStateDlg->SetActive(TRUE);



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
	if (!CheckPetSummoned())
		return;
	if (!m_pStateDlg)	return;
	m_pStateDlg->SetActive(!m_pStateDlg->IsActive());
}

void CPetManager::SetPetStateDlgInfo(CPet* pPet)
{
	if (!pPet)	return;
	BASE_PET_LIST* pList = pPet->m_pSInfo;
	PET_TOTALINFO* pInfo = &pPet->m_PetTotalInfo;
	if (!pList || !pInfo)	return;

	if (!m_pStateDlg)	return;
	SetPetGuageText(pPet, 0, pInfo->PetFriendly);
	m_pStateMiniDlg->AddPetIcon(m_pCurSummonPet);
	cStatic* pName = (cStatic*)m_pStateDlg->GetNameTextWin();
	cStatic* pNameMini = (cStatic*)m_pStateMiniDlg->GetNameTextWin();
	ITEM_INFO* piInfo = (ITEM_INFO*)ITEMMGR->GetItem(pPet->GetPetSommonItemDBIdx())->GetItemInfo();
	if (piInfo)
	{
		m_pStateDlg->SetXing(piInfo->CheRyuk);
	}
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
		cImage  image;
		ITEMMGR->GetIconImage(ITEMMGR->GetItem(pPet->GetPetSommonItemDBIdx())->GetItemIdx(), &image);
		p2DImage->SetBasicImage(&image);
	}
	if (pList->BuffList)
	{
		PET_BUFF_LIST* pBuffList = NULL;
		char BuffName[512] = { 0, };
		DWORD Grade = pPet->GetPetCurGrade() - 1;//½×¶Î
		WORD BuffIdx = 0;
		float val = 0;
		int x = 0;
		cStatic** pText = pText = (cStatic**)m_pStateDlg->GetPetBuffTextWin();
		DWORD co = RGB_HALF(0, 255, 0);

		for (int i = 0; i < MAX_PET_BUFF_NUM; ++i)
		{
			BuffIdx = pList->BuffList[Grade][i];
			pBuffList = GAMERESRCMNGR->GetPetBuffInfo(BuffIdx);
			if (0 == BuffIdx || pBuffList == 0)
			{
				if (pText[x])
				{
					pText[x]->SetStaticText("");
				}
				continue;
			}
			if (!pBuffList->IsAdd)
			{
				continue;
			}
			if (pBuffList && pBuffList->BuffValueData)
			{
				val = pBuffList->BuffValueData;
				GetPetBuffName(pBuffList->BuffKind, val, BuffName, pBuffList->BuffSuccessProb);
			}

			if (pText[x])
			{
				pText[x]->SetFGColor(co);
				pText[x]->SetStaticText(BuffName);
			}
			x++;
		}
		co = TTTC_MACROKEY;
		for (int i = 0; i < MAX_PET_BUFF_NUM; ++i)
		{
			BuffIdx = pList->BuffList[Grade][i];
			pBuffList = GAMERESRCMNGR->GetPetBuffInfo(BuffIdx);
			if (0 == BuffIdx || pBuffList == 0)
			{
				if (pText[x])
				{
					pText[x]->SetStaticText("");
				}
				continue;
			}
			if (pBuffList->IsAdd)
			{
				continue;
			}
			if (pBuffList && pBuffList->BuffValueData)
			{
				val = pBuffList->BuffValueData;
				GetPetBuffName(pBuffList->BuffKind, val, BuffName, pBuffList->BuffSuccessProb);
			}

			if (pText[x])
			{
				pText[x]->SetFGColor(co);
				pText[x]->SetStaticText(BuffName);
			}
			x++;
		}
	}

}

void CPetManager::SetPetStateDlgInfoAll()
{
	char BuffName[512] = { 0, };
	cStatic** pText = (cStatic**)m_pStateDlg->GetPetBuffPulsTextWin();
	int x = 0;
	DWORD co = RGB_HALF(0, 255, 0);
	for (int i = 1; i < ePB_Kind_Max; ++i)
	{
		if (i < 16)
			pText[i]->SetStaticText("");
		if (x >= 16)
		{
			return;
		}



		if (!m_BuffData[i].IsAdd)
			continue;;
		if (m_BuffData[i].BuffValueData == 0)
			continue;
		GetPetBuffName(i, m_BuffData[i].BuffValueData, BuffName, m_BuffData[i].Prob);
		if (pText[x])
		{
			pText[x]->SetFGColor(co);
			pText[x]->SetStaticText(BuffName);
		}
		x++;
	}
	co = TTTC_QUESTITEM;
	for (int i = 1; i < ePB_Kind_Max; ++i)
	{
		if (x >= 16)
		{
			return;
		}

		if (m_BuffData[i].IsAdd)
			continue;
		if (m_BuffData[i].BuffValueData == 0)
			continue;
		GetPetBuffName(i, m_BuffData[i].BuffValueData, BuffName, m_BuffData[i].Prob);
		if (pText[x])
		{
			pText[x]->SetFGColor(co);
			pText[x]->SetStaticText(BuffName);
		}
		x++;
	}
}

void   CPetManager::GetPetBuffName(BYTE BuffKind, float BuffValueData, char* BuffName, float Rate)
{
	if (BuffKind == ePB_Demage_Percent)
	{
		sprintf(BuffName, "%.1f%%¸ÅÂÊÔö¼Ó %.1f%%¹¥»÷Á¦", Rate, BuffValueData);
	}
	else if (BuffKind == ePB_Dodge)
	{
		sprintf(BuffName, CHATMGR->GetChatMsg(1271), (DWORD)BuffValueData);
	}
	else if (BuffKind == ePB_MasterAllStatUp)
	{
		sprintf(BuffName, "ËùÓÐÊôÐÔÔö¼Ó %d", (DWORD)BuffValueData);
	}
	else if (BuffKind == ePB_Item_DoubleChance)
	{
		sprintf(BuffName, CHATMGR->GetChatMsg(1305), BuffValueData);
	}
	else if (BuffKind == ePB_NoForeAtkMonster)
	{
		sprintf(BuffName, CHATMGR->GetChatMsg(1350));
	}
	else if (BuffKind == ePB_ReduceCriticalDmg)
	{
		sprintf(BuffName, "¹ÖÎï·ÜÁ¦Ò»»÷ÉËº¦¼õÉÙ %d", BuffValueData);
	}
	else if (ePB_MasterAllStatRound == BuffKind)
	{
		sprintf(BuffName, "ÊôÐÔµ÷Õû");
	}
	else if (BuffKind == ePB_MaxLifeNaeRyukShield)
	{
		sprintf(BuffName, "ÉúÃü/ÄÚÁ¦/»¤ÌåÔö¼Ó %.1f%%", BuffValueData);
	}


	else if (BuffKind == ePB_Item_RareProbUp)
	{
		sprintf(BuffName, CHATMGR->GetChatMsg(1476), BuffValueData);
	}
	else if (BuffKind == ePB_MussangTimeIncrease)
	{
		sprintf(BuffName, CHATMGR->GetChatMsg(1477), (DWORD)BuffValueData);
	}
	else if (BuffKind == ePB_Exp)
	{
		sprintf(BuffName, "½ÇÉ«¾­Ñé¼Ó³É %.1f%%"/*CHATMGR->GetChatMsg(2331)*/, BuffValueData);
	}
	else if (BuffKind == ePB_PVPDmgSub)
	{
		sprintf(BuffName, "PVPÉËº¦¼õÉÙ %.1f%%", BuffValueData);
	}
	else if (BuffKind == ePB_PVPDmgUp)
	{
		sprintf(BuffName, "PVPÉËº¦Ôö¼Ó %.1f%%", BuffValueData);
	}
	else if (BuffKind == ePB_MoneyDropUp)
	{
		sprintf(BuffName, "µôÂäÓÎÏ·±Ò¼Ó³É %.1f%%", BuffValueData);
	}


}
void CPetManager::ClosePetAllDlg()
{
	m_pStateDlg->SetActive(FALSE);
	m_pStateMiniDlg->SetActive(FALSE);
	m_pInvenDlg->SetActive(FALSE);
	SelectPet = NULL;
}

void CPetManager::SetPetGuageText(CPet* pPet, DWORD dwStamina, DWORD dwFriendShip)
{
	CPet* Pet = NULL;
	int i = 0;
	for (; i < 3; ++i)
	{
		if (m_pCurSummonPet[i] == pPet)
		{
			Pet = m_pCurSummonPet[i];
			break;
		}
	}
	if (!Pet) return;

	char szValue[32];

	if (dwFriendShip >= 10000000)
	{
		sprintf(szValue, "%d%%", 100);
	}
	else
	{
		sprintf(szValue, "%.3f%%"/*"%d%%"*/, (float)dwFriendShip / 100000);
	}
#ifdef _CHEATENABLE_
	if (CHEATMGR->IsCheatEnable())
	{
		sprintf(szValue, "%.3f", (float)dwFriendShip / 100000);
	}
#endif



	//sprintf(szValue, "%d / %d", dwStamina, pPet->GetMaxStamina());
	//((cStatic*)m_pStateDlg->GetStaminaTextWin())->SetStaticText(szValue);

	float friendShip = float(pPet->m_PetTotalInfo.PetFriendly)
		/ float(GAMERESRCMNGR->GetPetRule()->MaxFriendship);
	if (pPet == SelectPet)
	{
		((cStatic*)m_pStateDlg->GetFriendShipTextWin())->SetStaticText(szValue);
		if (m_pStateDlg->IsActive())
		{
			((CObjectGuagen*)m_pStateDlg->GetFriendShipGuage())->SetValue(friendShip, 0);
			// 			CObjectGuagen* dlg = (CObjectGuagen*)m_pStateDlg->GetFriendShipGuage();
			// 			if (dlg)
			// 			{
			// 				dlg->SetValue(friendShip, 0);
			// 			}
		}

	}

	//((CObjectGuagen*)m_pStateMiniDlg->GetFriendShipGuage())->SetValue( friendShip, 0 );
	m_pStateMiniDlg->SetPetFriendShipGuage(i, friendShip);

}

void CPetManager::SetPetStateMiniDlgInfo(CPet* pPet)
{
	// 	if(!pPet)	return;
	// 	
	// 	cStatic* pInvenNum = (cStatic*)m_pStateDlg->GetInvenNumTextWin();
	// 
	// 	PET_TOTALINFO* pInfo = &pPet->m_PetTotalInfo;
	// 	BASE_PET_LIST* pList = GAMERESRCMNGR->GetPetListInfo(pPet->GetPetIdx());
	// 	//Æê À¯È¿ ÀÎº¥Åä¸® °¹¼ö
	// 	if( pInvenNum )
	// 	{
	// 		char szValue[32];
	// 		DWORD count = pList->InventoryTapNum[pInfo->PetGrade-1];
	// 
	// 		if(count)
	// 		{
	// 			wsprintf( szValue, "%d", count);
	// 		}
	// 		else
	// 		{
	// 			wsprintf( szValue, "%s", CHATMGR->GetChatMsg(249));
	// 		}
	// 
	// 		pInvenNum->SetStaticText(szValue);
	// 	}

	//Æê ½ºÅ³ Á¤º¸

	/*if(pList->BuffList)
	{
	PET_BUFF_LIST* pBuffList = NULL;
	char BuffName[32] = {0,};

	for( int i = 0; i < MAX_PET_BUFF_NUM; ++i )
	{
	cStatic** pText = (cStatic**)m_pStateDlg->GetPetBuffTextWin();

	WORD BuffIdx = pList->BuffList[pPetInfo->PetGrade-1][i];
	if(0 == BuffIdx)
	{
	if(pText[i])
	{
	pText[i]->SetStaticText("");
	}
	continue;
	}

	pBuffList = GAMERESRCMNGR->GetPetBuffInfo(BuffIdx);

	if(pBuffList && pBuffList->BuffValueData)
	{
	if( pBuffList->BuffKind == ePB_Demage_Percent )
	{
	wsprintf( BuffName, CHATMGR->GetChatMsg(1270), pBuffList->BuffValueData );
	}
	else if( pBuffList->BuffKind == ePB_Dodge )
	{
	wsprintf( BuffName, CHATMGR->GetChatMsg(1271), pBuffList->BuffSuccessProb );
	}

	}

	if(pText[i])
	{
	pText[i]->SetStaticText(BuffName);
	}
	}
	}*/
}

DWORD CPetManager::GetPetValidInvenMaxTabNum()
{
	// 	//if(!m_pCurSummonPet)	return 0;
	// 	CPet* Pet = NULL;
	// 	for (int i = 0; i < 3; ++i)
	// 	{
	// 		if (m_pCurSummonPet[i])
	// 		{
	// 			Pet = m_pCurSummonPet[i];
	// 		}
	// 	}
	// 	if (!Pet) return 0;
	// 	BASE_PET_LIST* pPetList = NULL;
	// 
	// 	pPetList = Pet->GetSInfo();

	return  3; // pPetList->InventoryTapNum[MAX_PET_GRADE];
}

void CPetManager::SetPetValidInvenTab()
{
	if (!m_pInvenDlg)	return;

	DWORD ValidTabNum = GetPetValidInvenMaxTabNum();	//0 ~ 3

	//ÀÎº¥Ã¢ ÅÇ ²¨ÁÖ±â ¶Ç´Â ÅÇ ½½·Ôµé ¶ôÇØÁÖ±â
	m_pInvenDlg->ApplyValidInvenTab(ValidTabNum);
}

void CPetManager::SetCurPetRest(BOOL bRest)
{
	// 	if(!CheckPetSummoned())
	// 	{
	// 		ASSERT(0);
	// 		return;
	// 	}
	// 
	// 	m_pCurSummonPet->SetPetRest(bRest);
	//m_pStateDlg->SetBtnClick(!bRest);
}

void CPetManager::SendPetRestMsg(BOOL bRest)//Æê »ç¿ë/ÈÞ½Ä Á¤º¸ º¸³»±â
{
	if (!CheckPetSummoned())
		return;

	if (!IsReadyToSendRestMsg())
		return;

	// 	if( 0 == m_pCurSummonPet->m_PetTotalInfo.PetStamina )
	// 	{
	// 		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1272) );
	// 		return;
	// 	}
	// 
	// 	if(m_pCurSummonPet->IsPetRest() == bRest) return;

	//SetCurPetRest(bRest); //¸®ÅÏ¹Þ°í Ã³¸®.

	MSGBASE msg;
	msg.Category = MP_PET;

	//	if(m_pCurSummonPet->IsPetRest())
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

	MSG_DWORD msg;
	msg.Category = MP_PET;
	msg.Protocol = MP_PET_SEAL_SYN;
	msg.dwObjectID = HERO->GetID();
	msg.dwData = SelectPet->GetPetSommonItemDBIdx();
	NETWORK->Send(&msg, sizeof(msg));
}

void CPetManager::CheckRestNSkillUse()	//¹«°øÃ¢ ½ºÅ³¹öÆ° ±â´É. ÈÞ½Ä»óÅÂ¸é ÇØÁ¦½ÃÅ°°í °ÔÀÌÁöÇ®ÀÌ¸é ½ºÅ³ »ç¿ë. ±âÈ¹ÀÚ¿¡°Ô ¹®ÀÇÈÄ ¾Ë°ÔµÊ.
{
	// 	if(!CheckPetSummoned())
	// 		return;
	// 
	// 	if(m_pCurSummonPet->IsPetRest())
	// 	{
	// 		SendPetRestMsg(FALSE);
	// 		UseCurPetSkill();
	// 	}
	// 	else
	// 	{
	// 		UseCurPetSkill();
	// 	}
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

	//½ºÅ³ °ÔÀÌÁö Ç® »óÅÂ Ã¼Å©
	if (!IsSkillGuageFull())
	{
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1241));
	}

	if (!IsReadyToSendSkillMsg())
		return;
	//	MP_PET_USE_SKILL_SYN,
	//	MP_PET_USE_SKILL_ACK,	//Ä³¸¯ÀÌ »ç¿ëÁßÀÎ ½ºÅ³°ú Áßº¹ÀÌ ¾Æ´Ï°í, Æê ½ºÅ³ °ÔÀÌÁö°¡ Ç® »óÅÂ.
	//	MP_PET_USE_SKILL_NACK,

	MSGBASE msg;
	msg.Category = MP_PET;
	msg.Protocol = MP_PET_USE_SKILL_SYN;
	msg.dwObjectID = HERO->GetID();
	NETWORK->Send(&msg, sizeof(msg));

	SetReadyToSendSkillMsg(FALSE);
}

// void CPetManager::AddMasterStatFromPetBuff( player_calc_stats* pMasterAdditionalStat )
// {
// 	for (int i = 0; i < 3; ++i)
// 	{
// 		CPet* pCurPet = GetCurSummonPet(i);
// 		if (NULL == pCurPet)	continue;;
// 
// 		//if (pCurPet->IsPetRest())	return;
// 
// 		BASE_PET_LIST* pInfo = pCurPet->GetSInfo();
// 
// 		for (int i = 0; i < 3; ++i)
// 		{
// 			WORD BuffIdx = pInfo->BuffList[pCurPet->GetPetCurGrade() - 1][i];
// 			PET_BUFF_LIST* pList = GAMERESRCMNGR->GetPetBuffInfo(BuffIdx);
// 
// 			if (!pList)	continue;
// 
// 			if (pList->BuffKind == ePB_MasterAllStatUp)
// 			{
// 				WORD AddStat = (WORD)((float)pList->BuffValueData *(1 + HERO->GetJingLongRate()));
// 				pMasterAdditionalStat->GenGol += AddStat;
// 				pMasterAdditionalStat->MinChub += AddStat;
// 				pMasterAdditionalStat->CheRyuk += AddStat;
// 				pMasterAdditionalStat->SimMaek += AddStat;
// 
// 			}
// 		}
// 	}
// }
void CPetManager::GetPetBuffResultRt(int BuffKind, void* Data)
{
	DWORD RandV = random(0, 100);

	switch (BuffKind)
	{
	case ePB_Demage_Percent:
	{
		if (RandV > m_BuffData[ePB_Demage_Percent].Prob)	return;

		double* pDamage = (double*)Data;
		double Damg = *pDamage;
		Damg = double(Damg + Damg * (m_BuffData[ePB_Demage_Percent].BuffValueData * 0.01));
		*pDamage = Damg;
	}
	break;
	case ePB_Dodge:
	{
		BYTE* pDodge = (BYTE*)Data;
		*pDodge = m_BuffData[BuffKind].BuffValueData;
	}
	break;
	case ePB_MasterAllStatUp:
	{
		WORD PlusStat = (WORD)((float)m_BuffData[ePB_MasterAllStatUp].BuffValueData);
		player_calc_stats* pMasterAdditionalStats = (player_calc_stats*)Data;
		pMasterAdditionalStats->GenGol += PlusStat;
		pMasterAdditionalStats->MinChub += PlusStat;
		pMasterAdditionalStats->CheRyuk += PlusStat;
		pMasterAdditionalStats->SimMaek += PlusStat;
	}
	break;
	case ePB_Item_DoubleChance:
	{
		if (RandV > m_BuffData[ePB_Item_DoubleChance].Prob)	return;
		BOOL* pDouble = (BOOL*)Data;
		*pDouble = TRUE;
	}
	break;
	case ePB_NoForeAtkMonster:
	{
		BOOL* pNoForeAtk = (BOOL*)Data;
		*pNoForeAtk = TRUE;
	}
	break;
	case ePB_ReduceCriticalDmg:
	{
		double* pDamage = (double*)Data;
		double Damg = *pDamage;
		Damg = Damg * (100 - m_BuffData[ePB_ReduceCriticalDmg].BuffValueData) * 0.01f;
		*pDamage = Damg;
	}
	break;
	case ePB_Item_RareProbUp:
	{
		float* pAddprob = (float*)Data;

		float addprob = 1.0f - (float)m_BuffData[ePB_Item_RareProbUp].BuffValueData * 0.01f;
		*pAddprob = addprob;
	}
	break;
	case ePB_MussangTimeIncrease:
	{
		DWORD* pAddtime = (DWORD*)Data;

		DWORD addtime = (DWORD)m_BuffData[ePB_MussangTimeIncrease].BuffValueData * 1000;
		*pAddtime = addtime;
	}
	break;
	case ePB_MaxLifeNaeRyukShield:
	{
		float Rate = m_BuffData[ePB_MaxLifeNaeRyukShield].BuffValueData * 0.01f;
		DWORD* pMaxVal = (DWORD*)Data;
		DWORD PlusStat = *pMaxVal * Rate;
		*pMaxVal += PlusStat;
	}
	break;
	case ePB_MoneyDropUp:
	case ePB_Exp:
	{
		float* pval = (float*)Data;
		*pval += m_BuffData[BuffKind].BuffValueData * 0.01f;
	}
	break;
	case ePB_PVPDmgSub:
	{
		float* pval = (float*)Data;
		*pval += m_BuffData[BuffKind].BuffValueData * 0.01f;
	}
	break;
	case ePB_PVPDmgUp:
	{
		float* pval = (float*)Data;
		*pval += m_BuffData[BuffKind].BuffValueData * 0.01f;
	}
	break;
	default:
		break;
	}
}


WORD CPetManager::GetPetEquipItemMax(DWORD dwItemDBIdx)
{//Æê ÀåÂø ¾ÆÀÌÅÛ ÃÖ´ë°¹¼ö
	PET_TOTALINFO* pInfo = GetPetInfo(dwItemDBIdx);

	if (!pInfo)	return 0;

	return pInfo->PetGrade;
}

// void CPetManager::LoadPetImage()
// {
// 	BASE_PET_LIST* pPetList = NULL;
// 	int count = 1;
// 
// 	while (pPetList = GAMERESRCMNGR->GetPetListInfo(count))
// 	{
// 		cImage* pImage = new cImage;
// 		SCRIPTMGR->GetImage(pPetList->Pet2DIdx, pImage, PFT_ITEMPATH);
// 		ASSERT(!m_PetImageList.GetData(pPetList->PetKind));
// 		m_PetImageList.Add(pImage, pPetList->PetKind);
// 		++count;
// 	}
// }

// cImage* CPetManager::GetPetImage( DWORD dwPetIdx )
// {
// 	return m_PetImageList.GetData(dwPetIdx);
// }

// void CPetManager::RemovePetImage()
// {
// 	cImage* pImage = NULL;
// 
// 	m_PetImageList.SetPositionHead();
// 	while(pImage = m_PetImageList.GetData())
// 	{
// 		delete pImage;
// 		pImage = NULL;
// 	}
// 	m_PetImageList.RemoveAll();
// }

void CPetManager::SetCurSummonPet(CPet* pPet)
{
	int i = 0;
	for (; i < 3; ++i)
	{
		if (m_pCurSummonPet[i] == NULL)
		{
			m_pCurSummonPet[i] = pPet;
			SetSelectPet(i);
			WORD PetListIdx = pPet->GetPetKind();
			if (PetListIdx == CRISTMAS_EVENTPET)
			{
				m_wPetKind[i] = ePK_EventPet;
			}
			else
			{
				m_wPetKind[i] = ePK_None;
			}
			break;
		}
	}
	SetSelectPet(i);
}

void CPetManager::SetPetBuffInfo()
{

	memset(m_BuffData, 0, sizeof(BuffData) * ePB_Kind_Max);
	const BASE_PET_LIST* pBaseInfo = NULL;
	WORD BuffIdx = 0;
	WORD BuffKind = 0;
	PET_BUFF_LIST* pList = NULL;
	for (int j = 0; j < 3; ++j)
	{
		if (m_pCurSummonPet[j])
		{
			pBaseInfo = m_pCurSummonPet[j]->GetSInfo();


			for (int i = 0; i < 6; ++i)
			{
				BuffIdx = pBaseInfo->BuffList[m_pCurSummonPet[j]->GetPetGrade() - 1][i];

				pList = GAMERESRCMNGR->GetPetBuffInfo(BuffIdx);
				if (!pList)	continue;

				BuffKind = pList->BuffKind;

				if (m_BuffData[BuffKind].Prob < pList->BuffSuccessProb)
				{
					m_BuffData[BuffKind].Prob = pList->BuffSuccessProb;
				}

				if (pList->IsAdd)
				{
					DWORD BuffVal = pList->BuffValueData;
					m_BuffData[BuffKind].BuffValueData += BuffVal;
					m_BuffData[BuffKind].IsAdd = pList->IsAdd;
				}
				else
				{
					DWORD BuffVal = pList->BuffValueData;
					if (m_BuffData[BuffKind].BuffValueData < BuffVal)
					{
						m_BuffData[BuffKind].BuffValueData = BuffVal;
					}
				}
			}
		}
	}
}


BOOL CPetManager::CheckCurSummonPetKindIs(CPet* pPet, int kind)
{
	int id = 0;
	for (int i = 0; i < 3; ++i)
	{
		if (m_pCurSummonPet[i] == pPet)
		{
			id = i;
			break;
		}
	}
	BOOL rt = (m_wPetKind[id] & kind);

	return rt;
}



void CPetManager::RefleshPetMaintainBuff(player_calc_stats* item_stats)
{//Æê Áö¹öÇÁ °ü·Ã Ãß°¡ ½ºÅÈ °»½Å	//ÆêÈ°¼º/ÈÞ½Ä/¼ÒÈ¯/ºÀÀÎ½Ã¿¡ ÇÑ´Ù.	//¼ÒÈ¯,ºÀÀÎ½Ã´Â °³³äÀûÀÌ ¾Æ´Ñ ¹°¸®Àû Ãß°¡/Á¦°Å½Ã. 
	SetPetBuffInfo();
	SetPetStateDlgInfo(SelectPet);
	SetPetStateDlgInfoAll();
	GetPetBuffResultRt(ePB_MasterAllStatUp, item_stats);
	GAMEIN->GetCharacterDialog()->RefreshInfo();
}


#elif !defined(_MUTIPET_)


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
#else
#error "Preprocessor mismatch around _MUTIPET_: outer block got closed early."
#endif