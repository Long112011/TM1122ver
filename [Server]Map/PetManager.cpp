#include "StdAfx.h"
#include ".\petmanager.h"
#include "PackedData.h"
#include "Pet.h"
#include "Player.h"
#include "ObjectFactory.h"
#include "CharMove.h"
#include "UserTable.h"
#include "MapDBMsgParser.h"
#include "ItemSlot.h"
#include "ItemManager.h"
#include "CheckRoutine.h"
#include "../[CC]Header/GameResourceManager.h"
#include "../[CC]Header/CommonCalcFunc.h"
#include "CharMove.h"
#include "Battle.h"
DWORD CPetManager::m_dwPetObjectID = PET_ID_START;
GLOBALTON(CPetManager);
CPetManager::CPetManager(void)
{
	m_PetInfoList.Initialize(5);
	m_pCurSummonPet		= NULL;
	m_dwSkillRechargeCheckTime	= 0;
	m_dwSkillRechargeAmount	= 0;
	m_bSkillGuageFull	= FALSE;
	m_bPetStaminaZero	= FALSE;
	m_dwReleaseDelayTime = 0;
	m_dwResummonDelayTime = 0;
	m_BuffFlag = ePB_None;
	m_wPetKind = 0;
	m_iFriendshipReduceAmount = 0;
	m_dwEventPetSummonRemainTime = 0;
	m_dwPetValidDistPosCheckTime = gCurTime;
}
CPetManager::~CPetManager(void)
{
}
void CPetManager::Init(CPlayer* pPlayer)
{
	m_pPlayer = pPlayer;
	m_pCurSummonPet = NULL;
	m_dwSkillRechargeCheckTime	= 0;
	m_dwSkillRechargeAmount	= 0;
	m_bSkillGuageFull	= FALSE;
	m_bPetStaminaZero	= FALSE;
	m_dwReleaseDelayTime = 0;
	m_dwResummonDelayTime = 0;
	m_BuffFlag = ePB_None;
	m_dwPetValidDistPosCheckTime = gCurTime;
	m_wPetKind = 0;
	m_iFriendshipReduceAmount = 0;
	m_dwEventPetSummonRemainTime = 0;
	m_dwEventPetCheckTime = 0;
	memset(&m_PetEquipOption, 0, sizeof(PETEQUIP_ITEMOPTION));
}
void CPetManager::Release()
{	
	m_PetInfoList.SetPositionHead();
	PET_TOTALINFO* pInfo = NULL;
	while( pInfo = m_PetInfoList.GetData() )
	{
		delete pInfo;
		pInfo = NULL;
	}
	m_PetInfoList.RemoveAll();
	if(m_pCurSummonPet)
	{
		RemovePet();
	}
}
void CPetManager::ReleaseCurPetMove()
{
	if(m_pCurSummonPet)
		CCharMove::ReleaseMove(m_pCurSummonPet);
}
void CPetManager::DeletePet( DWORD ItemDBIdx )
{
	PET_TOTALINFO* pInfo = m_PetInfoList.GetData(ItemDBIdx);
	if(!pInfo)
	{
		//ASSERT(0);
		return;
	}
	PetDeleteToDB(pInfo->PetDBIdx);
	SAFE_DELETE(pInfo);
	m_PetInfoList.Remove(ItemDBIdx);
	if(m_pCurSummonPet)
	{
		if(m_pCurSummonPet->GetPetSummonItemDBIdx() == ItemDBIdx)
		{
			RemovePet();
		}
	}
}
void CPetManager::AddPetTotalInfo( PET_TOTALINFO* pPetInfo, int flagSendMsgTo )
{
	if(!pPetInfo)
	{
		return;
	}
	if(m_PetInfoList.GetData(pPetInfo->PetSummonItemDBIdx))
	{
		return;
	}
	PET_TOTALINFO* pNewInfo = new PET_TOTALINFO;
	memcpy(pNewInfo, pPetInfo, sizeof(PET_TOTALINFO));
	m_PetInfoList.Add(pNewInfo, pNewInfo->PetSummonItemDBIdx);
	if(eServerNClient == flagSendMsgTo)
	{
		SEND_ADDPET_FROMITEM msg;
		memcpy(&msg.Info, pPetInfo, sizeof(PET_TOTALINFO));
		msg.Category = MP_PET;
		msg.Protocol = MP_PET_ADDNEW_FROMITEM;
		msg.dwObjectID = m_pPlayer->GetID();
		m_pPlayer->SendMsg(&msg, sizeof(SEND_ADDPET_FROMITEM));
	}
}
void CPetManager::RemovePetTotalInfo( DWORD dwSummonItemDBIdx )
{
	PET_TOTALINFO* pInfo = m_PetInfoList.GetData(dwSummonItemDBIdx);
	//ASSERT(pInfo);
	SAFE_DELETE(pInfo);
	m_PetInfoList.Remove(dwSummonItemDBIdx);
}
BOOL CPetManager::UpGradePet( DWORD dwSummonItemDBIdx, BOOL bCheckProb  )
{
	PET_TOTALINFO* pInfo = GetPetTotalInfo(dwSummonItemDBIdx);
	if(!pInfo)	return eUpgradeFailforEtc;
	BASE_PET_LIST* pList = GAMERESRCMNGR->GetPetListInfo(pInfo->PetKind);
	if(!pList)	return eUpgradeFailforEtc;
	BOOL rt = eUpgradeSucess;
	if(m_pCurSummonPet)
	{
		if(m_pCurSummonPet->GetPetSummonItemDBIdx() == dwSummonItemDBIdx)
			return eUpgradeFailforSamePetSummoned;
	}
	if(bCheckProb)
	{
		BYTE Rnd = random(0,100);
		switch(pInfo->PetGrade)
		{
		case 1:
			if( Rnd > GAMERESRCMNGR->GetPetRule()->GradeUpProb1to2 )	
				rt = eUpgradeFailforProb;
			break;
		case 2:
			if( Rnd > GAMERESRCMNGR->GetPetRule()->GradeUpProb2to3 )	
				rt = eUpgradeFailforProb;
			break;
		case 3:
			return eUpgradeFailfor3rdUp;
		default:
			break;
		}
	}
	if(rt == eUpgradeFailforProb)
	{
		int Petfriendly = GAMERESRCMNGR->GetPetRule()->FriendShipPerUpgradeFail;
		if( pInfo->PetFriendly - Petfriendly < 0 )
		{
			pInfo->PetFriendly = 0;
		}
		else
		{
			pInfo->PetFriendly += Petfriendly;
		}
		MSG_DWORD2 msg;
		msg.Category = MP_PET;
		msg.Protocol = MP_PET_UPDATE_FRIENDSHIP;
		msg.dwObjectID = m_pPlayer->GetID();
		msg.dwData1 = pInfo->PetSummonItemDBIdx;
		msg.dwData2 = pInfo->PetFriendly;
		m_pPlayer->SendMsg(&msg, sizeof(msg));
		LogPet(m_pPlayer->GetID(),m_pPlayer->GetUserID(),ePetLog_UpGradeFailed, pInfo->PetDBIdx, pInfo->PetSummonItemDBIdx,
			pInfo->PetGrade,pInfo->PetStamina,pInfo->PetFriendly,pInfo->bAlive);
	}
	else if(rt == eUpgradeSucess)
	{
		if( pInfo->PetGrade == PET_MAX_LEVEL )	return eUpgradeFailforEtc;
		else
			++pInfo->PetGrade;
		pInfo->PetFriendly = GAMERESRCMNGR->GetPetRule()->DefaultFriendship;
		pInfo->PetStamina = pList->StaminaMax[pInfo->PetGrade-1];
		LogPet(m_pPlayer->GetID(),m_pPlayer->GetUserID(),ePetLog_GradeUp, pInfo->PetDBIdx, pInfo->PetSummonItemDBIdx,
			pInfo->PetGrade,pInfo->PetStamina,pInfo->PetFriendly,pInfo->bAlive);
	}
	PetUpdateToDB(m_pPlayer->GetUserID(), pInfo);
	return rt;
}
BOOL CPetManager::RevivalPet( DWORD dwSummonItemDBIdx, int iGrade )
{
	PET_TOTALINFO* pInfo = GetPetTotalInfo(dwSummonItemDBIdx);
	if(!pInfo)	return FALSE;
	BASE_PET_LIST* pList = GAMERESRCMNGR->GetPetListInfo(pInfo->PetKind);
	if(!pList)	return FALSE;
	if(iGrade)
	if(pInfo->PetGrade != iGrade)
		return FALSE;
	//ASSERT(FALSE==pInfo->bAlive);
	pInfo->bAlive = TRUE;
	if(iGrade)
		pInfo->PetFriendly = GAMERESRCMNGR->GetPetRule()->RevivalFriendship;
	else
		pInfo->PetFriendly = GAMERESRCMNGR->GetPetRule()->RevivalFriendship_Shop;
	pInfo->PetStamina = pList->StaminaMax[pInfo->PetGrade-1];
	PetUpdateToDB(m_pPlayer->GetUserID(), pInfo);
	LogPet(m_pPlayer->GetID(),m_pPlayer->GetUserID(),ePetLog_PetRevival, pInfo->PetDBIdx, pInfo->PetSummonItemDBIdx,
		pInfo->PetGrade,pInfo->PetStamina,pInfo->PetFriendly,pInfo->bAlive);
	return TRUE;
}
void CPetManager::RemovePet()
{
	if(!m_pCurSummonPet)	return;
	UpdateCurPetInfo();
	g_pServerSystem->RemovePet(m_pCurSummonPet->GetID());
	m_pCurSummonPet = NULL;
	m_BuffFlag = ePB_None;
	m_dwEventPetSummonRemainTime = 0;
	m_dwEventPetCheckTime = 0;
}
void CPetManager::AddPetInfoList( PET_TOTALINFO* pPetInfo )
{
	if(!pPetInfo)
	{
		return;
	}
	if(m_PetInfoList.GetData(pPetInfo->PetSummonItemDBIdx))
	{
		//ASSERT(0);
		return;
	}
	m_PetInfoList.Add(pPetInfo, pPetInfo->PetSummonItemDBIdx);
}
void CPetManager::RemovePetInfoList( DWORD dwSummonItemDBIdx )
{
	PET_TOTALINFO* pInfo = m_PetInfoList.GetData(dwSummonItemDBIdx);
	//ASSERT(pInfo);
	m_PetInfoList.Remove(dwSummonItemDBIdx);
}
WORD CPetManager::GetPetInfoList(PET_TOTALINFO* RtInfo)
{
	WORD PetCount = 0;
	m_PetInfoList.SetPositionHead();
	PET_TOTALINFO* pInfo = NULL;
	while(pInfo = m_PetInfoList.GetData())
	{
		memcpy(&RtInfo[PetCount], pInfo, sizeof(PET_TOTALINFO));
		++PetCount;
	}
	return PetCount;
}
PET_TOTALINFO* CPetManager::GetPetTotalInfo( DWORD dwItemDBIdx, int flg  )
{
	PET_TOTALINFO* pInfo = m_PetInfoList.GetData(dwItemDBIdx);
	if(pInfo)
		return pInfo;
	if( flg == eWithNULL )
	{
		return NULL;
	}
	else
	{
		static PET_TOTALINFO NULLINFO;
		memset(&NULLINFO,0,sizeof(PET_TOTALINFO));
		return &NULLINFO;
	}
}
BOOL CPetManager::CheckPetDefaultFriendship(DWORD dwItemDBIdx)
{
	PET_TOTALINFO* pPetInfo = GetPetTotalInfo(dwItemDBIdx);
	if( pPetInfo->PetFriendly < GAMERESRCMNGR->GetPetRule()->DefaultFriendship )
	{
		return FALSE;
	}
	else
		return TRUE;
}
void CPetManager::SummonPet( DWORD dwItemDBIdx, BOOL bSummonFromUser  )
{
	if( TRUE == CheckBeforeSummonPet( dwItemDBIdx ) )
	{
		PET_TOTALINFO* pInfo = GetPetTotalInfo(dwItemDBIdx);
		if(!pInfo)
		{
		//	char szBuf[128] = {0,};
		//	sprintf(szBuf, "Player Have No PetData with Item! PlayerID :%d\t ItemDBIdx:%d\n"
		//		,m_pPlayer->GetID(),dwItemDBIdx);
		//	ASSERTMSG(0,szBuf);
			return;
		}
		if( !pInfo->bAlive )
		{
			return;
		}
		if( m_pPlayer->GetUserLevel() > eUSERLEVEL_GM )
		if( FALSE == CheckResummonAvailable() )
			return;
		VECTOR3 PetPos = m_pPlayer->GetMoveInfo()->CurPosition;
		int	Basic_distance = random(100,300);
		int temp = rand() % 200 - 100;
		PetPos.x += Basic_distance + temp;
		temp = rand() % 200 - 100;
		PetPos.y += Basic_distance + temp;
		int MapNum = g_pServerSystem->GetMapNum();
		if( g_pServerSystem->GetMap()->CollisionTilePos(PetPos.x,PetPos.z,MapNum) == TRUE )
		{	
			PetPos = m_pPlayer->GetMoveInfo()->CurPosition;
		}
		CPet* pPet = g_pServerSystem->AddPet(m_pPlayer, pInfo->PetDBIdx+m_dwPetObjectID, pInfo, &PetPos);
		//m_dwSkillRechargeAmount = 0;
		if(pPet)
		{
			//g_Console.LOG(4, "AddPet");
			m_pCurSummonPet = pPet;
		}
		else
		{
			//g_Console.LOG(4, "NULL");
			//ASSERT(0);
			m_pCurSummonPet = NULL;
			return;
		}
		SetPetBuffInfo();
		RefleshPetMaintainBuff();
		m_pCurSummonPet->CalcPetMaxStamina();
		m_pCurSummonPet->SendPetInfoMsg();
		if(TRUE == bSummonFromUser)
		{
			MSG_DWORD msg;
			msg.Category = MP_PET;
			msg.Protocol = MP_PET_SUMMON_ACK;
			msg.dwObjectID = m_pPlayer->GetID();
			msg.dwData = m_pCurSummonPet->GetID();
			PACKEDDATA_OBJ->QuickSend( m_pPlayer, &msg, sizeof(msg) );
		}
	}
}
BOOL CPetManager::CheckBeforeSummonPet(DWORD dwItemDBIdx)
{
	if(m_pCurSummonPet)
	{
		if(m_pCurSummonPet->GetPetSummonItemDBIdx() == dwItemDBIdx)
		{
			SealPet();
			return FALSE;
		}
		else if(m_pCurSummonPet->GetPetKind() == CRISTMAS_EVENTPET)
		{
			SealPet();
			return TRUE;
		}
		else
		{		
		//	MSGBASE msg;
		//	msg.Category = MP_PET;
		//	msg.Protocol = MP_PET_SUMMON_RESULT;
		//	msg.dwObjectID = m_pPlayer->GetID();
		//	m_pPlayer->SendMsg(&msg,sizeof(msg));
			return TRUE;
		}
	}
	else
		return TRUE;
}
void CPetManager::SummonEventPet()
{
	if(m_pCurSummonPet)
	{
		return;
	}
	BASE_PET_LIST*	pPetListInfo;
	pPetListInfo = GAMERESRCMNGR->GetPetListInfo(CRISTMAS_EVENTPET);
	if( NULL == pPetListInfo )
		return;
	PET_TOTALINFO TotalInfo;
	TotalInfo.PetSummonItemDBIdx = 0;			
	TotalInfo.PetKind		= CRISTMAS_EVENTPET;	
	TotalInfo.PetStamina	= pPetListInfo->StaminaMax[0];	
	TotalInfo.PetFriendly = GAMERESRCMNGR->GetPetRule()->DefaultFriendship;		
	TotalInfo.PetGrade	= 1;	
	TotalInfo.PetMasterUserID	= m_pPlayer->GetUserID();	
	TotalInfo.bAlive		= TRUE;
	TotalInfo.bSummonning	= FALSE;
	TotalInfo.bRest			= FALSE;
	VECTOR3 PetPos = m_pPlayer->GetMoveInfo()->CurPosition;
	int	Basic_distance = random(100,300);
	int temp = rand() % 200 - 100;
	PetPos.x += Basic_distance + temp;
	temp = rand() % 200 - 100;
	PetPos.y += Basic_distance + temp;
	int MapNum = g_pServerSystem->GetMapNum();
	if( g_pServerSystem->GetMap()->CollisionTilePos(PetPos.x,PetPos.z,MapNum) == TRUE )
	{	
		PetPos = m_pPlayer->GetMoveInfo()->CurPosition;
	}
	CPet* pPet = g_pServerSystem->AddPet(m_pPlayer, EVENT_PET_ID_START + m_pPlayer->GetID(), &TotalInfo, &PetPos);
	if(pPet)
	{
		m_pCurSummonPet = pPet;
	}
	else
	{
		//ASSERT(0);
		m_pCurSummonPet = NULL;
		return;
	}
	SetPetBuffInfo();
	RefleshPetMaintainBuff();
	m_dwEventPetSummonRemainTime = gCurTime + CRISTMAS_EVENTPET_SUMMONNING_TIME;
	m_dwEventPetCheckTime = gCurTime;
	SetCurSummonPetKind(ePK_EventPet);
	MSG_DWORD msg;
	msg.Category = MP_PET;
	msg.Protocol = MP_PET_SUMMON_ACK;
	msg.dwObjectID = m_pPlayer->GetID();
	msg.dwData = m_pCurSummonPet->GetID();
	PACKEDDATA_OBJ->QuickSend( m_pPlayer, &msg, sizeof(msg) );
}
BOOL CPetManager::CheckCurSummonPetKindIs( int kind )
{
	BOOL rt = (m_wPetKind & kind);
	return rt;
}
void CPetManager::CheckEventPetSummonRemainTime()
{
#define EVENTPET_SUMMONNING_CHECK_TIME_MSEC 30000
	if( gCurTime - m_dwEventPetCheckTime < EVENTPET_SUMMONNING_CHECK_TIME_MSEC )
	{
		return;
	}
	else
	{
		m_dwEventPetCheckTime = gCurTime;
	}
	if( m_dwEventPetSummonRemainTime < gCurTime )
	{
		SealPet();
	}
}
void CPetManager::SealPet()
{
	if(!m_pCurSummonPet)	return;
	if( m_pPlayer->GetState() == eObjectState_Die )
		return;
	if( !CheckCurSummonPetKindIs(ePK_EventPet) )
		UpdateCurPetInfo();
	MSG_DWORD msg;
	msg.Category = MP_PET;
	msg.Protocol = MP_PET_SEAL_ACK;
	msg.dwObjectID = m_pPlayer->GetID();
	msg.dwData = m_pCurSummonPet->GetID();
	PACKEDDATA_OBJ->QuickSend( m_pPlayer, &msg, sizeof(msg) );
	g_pServerSystem->RemovePet(m_pCurSummonPet->GetID());
	SetResummonCheckTime();
	m_pCurSummonPet = NULL;
	RefleshPetMaintainBuff();
	m_BuffFlag = ePB_None;
	m_dwEventPetSummonRemainTime = 0;
	m_dwEventPetCheckTime = 0;
	SetCurSummonPetKind(ePK_None);
	memset(m_BuffData, 0, sizeof(BuffData)*ePB_Kind_Max);
}
void CPetManager::ExchangePet( DWORD dwItemDBIdx, CPlayer* pNewMaster, BOOL bReduceFriendship )
{
	PET_TOTALINFO* pPetInfo = GetPetTotalInfo(dwItemDBIdx);
	if(NULL == pPetInfo)	
	{
		return;
	}
	pPetInfo->PetMasterUserID = pNewMaster->GetUserID();
	//ASSERT((int)pPetInfo->PetFriendly - GAMERESRCMNGR->GetPetRule()->DefaultFriendship >= 0);
	if( TRUE == bReduceFriendship )
	{
		int Petfriendly = GAMERESRCMNGR->GetPetRule()->FriendShipPerTrade;
		pPetInfo->PetFriendly += Petfriendly;
	}
	PetUpdateToDB(pNewMaster->GetUserID(), pPetInfo);
	LogPet(pNewMaster->GetID(),pNewMaster->GetUserID(),ePetLog_Exchange, pPetInfo->PetDBIdx, pPetInfo->PetSummonItemDBIdx,
		pPetInfo->PetGrade,pPetInfo->PetStamina,pPetInfo->PetFriendly,pPetInfo->bAlive);
	pNewMaster->GetPetManager()->AddPetInfoList(pPetInfo);
	MSG_DWORD2 msg;
	msg.Category = MP_PET;
	msg.Protocol = MP_PET_UPDATE_FRIENDSHIP;
	msg.dwObjectID = pNewMaster->GetID();
	msg.dwData1 = pPetInfo->PetSummonItemDBIdx;
	msg.dwData2 = pPetInfo->PetFriendly;
	pNewMaster->SendMsg(&msg, sizeof(msg));
	RemovePetInfoList(dwItemDBIdx);
	return;
}
BOOL CPetManager::IsCurPetSummonItem( DWORD dwItemDBIdx )
{
	if(!m_pCurSummonPet)	return FALSE;
	const PET_TOTALINFO* pInfo = m_pCurSummonPet->GetPetTotalInfo();
	if( pInfo->PetSummonItemDBIdx == dwItemDBIdx )
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
void CPetManager::FeedUpPet( DWORD dwFeedAmount )
{
	BYTE rt = ePFR_Sucess;
	if(!m_pCurSummonPet)
	{
		rt = ePFR_Unsummoned;
		return;
	}
	if(m_pCurSummonPet->IsPetStaminaFull())
	{
		rt = ePFR_StaminaFull;
		return;
	}
	if(m_pCurSummonPet && rt == ePFR_Sucess)
	{
		DWORD AddAmount = dwFeedAmount;
		if(m_PetEquipOption.iPetStaminaRecoverateIncrease)
		{
			AddAmount += AddAmount * m_PetEquipOption.iPetStaminaRecoverateIncrease / 100;
		}
		if(m_PetEquipOption.iPetStaminaRecoverateAmount)
		{
			AddAmount += m_PetEquipOption.iPetStaminaRecoverateAmount;
		}
		m_pCurSummonPet->AddStamina(AddAmount, TRUE);
	}
}
void CPetManager::PetProcess()
{
	if(m_pCurSummonPet)
	{
		CheckPosValidDistWithMaster();
		m_pCurSummonPet->Process();
		CCharMove::MoveProcess(m_pCurSummonPet);
		CheckStaminaZero();
		CheckDelayRelease();
		if( CheckCurSummonPetKindIs(ePK_EventPet) )
		{
			CheckEventPetSummonRemainTime();
		}
	}
}
void CPetManager::CheckPosValidDistWithMaster()
{
	if(!m_pCurSummonPet)	return;
#define PET_VALID_DISTANCE_CHECK_TIME_MSEC	3000
	if( gCurTime - m_dwPetValidDistPosCheckTime < PET_VALID_DISTANCE_CHECK_TIME_MSEC )	return;
	VECTOR3* pPetCurPos = CCharMove::GetPosition(m_pCurSummonPet);
	VECTOR3* pPlayerCurPos = CCharMove::GetPosition(m_pPlayer);
	float Dist = CalcDistanceXZ(pPetCurPos,pPlayerCurPos);
	if(Dist > 1600)
	{
		CCharMove::SetPosition(m_pCurSummonPet,pPlayerCurPos);
		CCharMove::EndMove(m_pCurSummonPet,gCurTime,pPlayerCurPos);
		MOVE_POS	Msg;
		Msg.Category = MP_MOVE;
		Msg.Protocol = MP_MOVE_PET_WARP_ACK;
		Msg.dwObjectID = m_pPlayer->GetID();
		Msg.dwMoverID = m_pCurSummonPet->GetID();
		Msg.cpos.Compress(pPlayerCurPos);
		PACKEDDATA_OBJ->QuickSend(m_pPlayer,&Msg,sizeof(Msg));
	}
	m_dwPetValidDistPosCheckTime = gCurTime;
}
void CPetManager::CheckStaminaZero()
{
	if(!m_pCurSummonPet)	return;
	const PET_TOTALINFO* pPetInfo = m_pCurSummonPet->GetPetTotalInfo();
	if( FALSE == m_bPetStaminaZero && TRUE == m_pCurSummonPet->IsPetStaminaZero() )
	{
		if(m_pCurSummonPet->IsPetRest())
			return;
		else
		{
			m_pCurSummonPet->SetPetRest(TRUE);
			MSGBASE msg;
			msg.Category = MP_PET;
			msg.dwObjectID = m_pPlayer->GetID();
			msg.Protocol = MP_PET_REST_ON_ACK;
			m_pPlayer->SendMsg(&msg, sizeof(msg));
		}
		LogPet(m_pPlayer->GetID(),m_pPlayer->GetUserID(),ePetLog_StaminaZero, pPetInfo->PetDBIdx, pPetInfo->PetSummonItemDBIdx,
			pPetInfo->PetGrade,pPetInfo->PetStamina,pPetInfo->PetFriendly,pPetInfo->bAlive);
	}
	else if( TRUE == m_bPetStaminaZero && FALSE == m_pCurSummonPet->IsPetStaminaZero() )
	{
		LogPet(m_pPlayer->GetID(),m_pPlayer->GetUserID(),ePetLog_StaminaFromZero, pPetInfo->PetDBIdx, pPetInfo->PetSummonItemDBIdx,
			pPetInfo->PetGrade,pPetInfo->PetStamina,pPetInfo->PetFriendly,pPetInfo->bAlive);
	}
	m_bPetStaminaZero = m_pCurSummonPet->IsPetStaminaZero();
}
void CPetManager::CalcPetSkillRecharge()
{
	if(m_pCurSummonPet->IsPetRest())
		return;
	if(gCurTime - m_dwSkillRechargeCheckTime < PET_SKILLCHARGE_CHECKTIME)	return;
	const BASE_PET_LIST* pPetBaseInfo = m_pCurSummonPet->GetPetBaseInfo();
	if( FALSE == m_pCurSummonPet->IsPetStaminaZero() )
	{
		m_dwSkillRechargeAmount += pPetBaseInfo->SkillRecharge;
	}
	else
	{
		m_dwSkillRechargeAmount = 0;
	}
	if( m_dwSkillRechargeAmount >= PET_MAX_SKILL_CHARGE )
	{
		m_dwSkillRechargeAmount = PET_MAX_SKILL_CHARGE;
		m_bSkillGuageFull = TRUE;
	}
	m_dwSkillRechargeCheckTime = gCurTime;
}
void CPetManager::ReleaseMoveWithDelay(DWORD delayTime)
{
	m_dwReleaseDelayTime = gCurTime + delayTime;
}
void CPetManager::CheckDelayRelease()	
{
	if(m_dwReleaseDelayTime)
	{
		if(gCurTime < m_dwReleaseDelayTime)
			return;
		else
		{
			RemovePet();
			m_dwReleaseDelayTime = 0;
		}
	}
}
void CPetManager::NetworkMsgParse( BYTE Protocol, void* pMsg )
{
	MSG_DWORD msg;
	msg.Category = MP_PET;
	msg.dwObjectID = m_pPlayer->GetID();
	switch( Protocol )
	{
	case MP_PET_REST_ON:
		{
			if( m_pCurSummonPet && FALSE == m_pCurSummonPet->IsPetRest() )
			{
				m_pCurSummonPet->SetPetRest(TRUE);
				RefleshPetMaintainBuff();
				msg.Protocol = MP_PET_REST_ON_ACK;
			}
			else
			{
				msg.Protocol = MP_PET_REST_NACK;
			}
		}
		break;
	case MP_PET_REST_OFF:
		{
			if( m_pCurSummonPet && TRUE == m_pCurSummonPet->IsPetRest() )
			{
				if(m_pCurSummonPet->IsPetStaminaZero())
					msg.Protocol = MP_PET_REST_ON_NACK;
				else
				{
					m_pCurSummonPet->SetPetRest(FALSE);
					RefleshPetMaintainBuff();
					msg.Protocol = MP_PET_REST_OFF_ACK;
				}
			}
			else
			{
				msg.Protocol = MP_PET_REST_NACK;
			}
		}
		break;
	case MP_PET_USE_SKILL_SYN:
		{
			if(FALSE == IsSkillGuageFull())
			{
				msg.Protocol = MP_PET_USE_SKILL_NACK;
				break;
			}
			if( m_pCurSummonPet && m_pCurSummonPet->UsePetSkill())
			{
				m_dwSkillRechargeAmount = 0;
				m_bSkillGuageFull = FALSE;
				msg.Protocol = MP_PET_USE_SKILL_ACK;
			}
			else
			{
				msg.Protocol = MP_PET_USE_SKILL_NACK;
			}
		}
		break;
	case MP_PET_SEAL_SYN:
		{
			if(m_pCurSummonPet)
			{
				SealPet();
			}
			else
			{
				msg.Protocol = MP_PET_SEAL_NACK;
			}
		}
		break;
	case MP_PET_UPGRADE_SYN:
		{
			MSG_DWORD3* pmsg = (MSG_DWORD3*)pMsg;
			BOOL rt = UpGradePet(pmsg->dwData1);
			if(!CHKRT->ItemOf(m_pPlayer, (POSTYPE)pmsg->dwData2, (WORD)pmsg->dwData3,0,0,CB_EXIST|CB_ICONIDX))
				rt = eUpgradeFailforEtc;
			switch( rt )
			{
			case eUpgradeSucess:
				{
					msg.Protocol = MP_PET_UPGRADE_ACK;
				}
				break;
			case eUpgradeFailforProb:
				{
					msg.Protocol = MP_PET_UPGRADE_NACK;
				}
				break;
			case eUpgradeFailforEtc:
				{
					//char szBuf[128] = {0,};
					//sprintf(szBuf, "PetDataErrorAtPetUpgrade! PlayerID:%d\tItemDBIdx:%d\n", m_pPlayer->GetID(), pmsg->dwData1);
					//ASSERTMSG(0, szBuf);
					return;
				}
			case eUpgradeFailfor3rdUp:
				{
					msg.Protocol = MP_PET_UPGRADE_NACK;
					//char szBuf[128] = {0,};
					//sprintf(szBuf, "PetProcessErrorAtPetUpgrade! Attempt 3rd Upgrade PlayerID:%d\tItemDBIdx:%d\n", m_pPlayer->GetID(), pmsg->dwData1);
					//ASSERTMSG(0, szBuf);
				}
				break;
			case eUpgradeFailforSamePetSummoned:
				{
					msg.Protocol = MP_PET_UPGRADE_NACK;
					//char szBuf[128] = {0,};
					//sprintf(szBuf, "PetProcessErrorAtPetUpgrade! Attempt SamePet PlayerID:%d\tItemDBIdx:%d\n", m_pPlayer->GetID(), pmsg->dwData1);
					//ASSERTMSG(0, szBuf);
				}
				break;
			default:
				break;
			}
			if( eUpgradeSucess == rt || eUpgradeFailforProb == rt )
			{
				if( EI_TRUE != ITEMMGR->DiscardItem( m_pPlayer, (POSTYPE)pmsg->dwData2, (WORD)pmsg->dwData3, 1 ) )
				{
					//ASSERTMSG(0,"DeleteItemFailedError! PetUpgradeItem");
					return;
				}
				MSG_ITEM_USE_ACK Msg;
				Msg.Category = MP_ITEM;
				Msg.Protocol = MP_ITEM_USE_ACK;
				Msg.TargetPos = (POSTYPE)pmsg->dwData2;
				Msg.wItemIdx = (WORD)pmsg->dwData3;
				m_pPlayer->SendMsg(&Msg,sizeof(Msg));
			}
		}
		break;
	case MP_PET_REVIVAL_SYN:
		{
			MSG_DWORD4* pmsg = (MSG_DWORD4*)pMsg;
			if(!CHKRT->ItemOf(m_pPlayer, (POSTYPE)pmsg->dwData2, (WORD)pmsg->dwData3,0,0,CB_EXIST|CB_ICONIDX))
			{
				//ASSERTMSG(0,"TargetItemNotExist! PetRevivalItem");
				return;
			}
			if( RevivalPet(pmsg->dwData1,pmsg->dwData4) )
			{
				msg.Protocol = MP_PET_REVIVAL_ACK;
				msg.dwData = pmsg->dwData4;
				if( EI_TRUE != ITEMMGR->DiscardItem( m_pPlayer, (POSTYPE)pmsg->dwData2, (WORD)pmsg->dwData3, 1 ) )
				{
				//	ASSERTMSG(0,"DeleteItemFailedError! PetRevivalItem");
					return;
				}
				MSG_ITEM_USE_ACK Msg;
				Msg.Category = MP_ITEM;
				Msg.Protocol = MP_ITEM_USE_ACK;
				Msg.TargetPos = (POSTYPE)pmsg->dwData2;
				Msg.wItemIdx = (WORD)pmsg->dwData3;
				m_pPlayer->SendMsg(&Msg,sizeof(Msg));
			}
			else
			{
				//char szBuf[128] = {0,};
				//sprintf(szBuf, "PetDataErrorAtPetRevival! PlayerID:%d\tItemDBIdx:%d\n", m_pPlayer->GetID(), pmsg->dwData1);
				//ASSERTMSG(0, szBuf);
				msg.Protocol = MP_PET_REVIVAL_NACK;
			}
		}
		break;
	default:
		//ASSERT(0);
		break;
	}
	m_pPlayer->SendMsg(&msg, sizeof(msg));
}
void CPetManager::UpdateCurPetInfo()
{
	if( !m_pCurSummonPet ) return;
	const PET_TOTALINFO* pCurPetInfo = m_pCurSummonPet->GetPetTotalInfo();
	DWORD dwItemDBIdx = pCurPetInfo->PetSummonItemDBIdx;
	PET_TOTALINFO* TInfo = GetPetTotalInfo(dwItemDBIdx);
	if(!TInfo)
	{
		//ASSERT(0);
		return;
	}
	*TInfo = *pCurPetInfo;
	PetUpdateToDB(m_pPlayer->GetUserID(), TInfo);
}
void CPetManager::UpdateLogoutToDB()
{
}
void CPetManager::SetSommonPetStamina( BYTE bFlag )
{
	if( !m_pCurSummonPet )	return;
	DWORD stamina = 0;
	if(1 == bFlag)
	{
		const BASE_PET_LIST* pList = m_pCurSummonPet->GetPetBaseInfo();
		stamina = pList->StaminaMax[m_pCurSummonPet->GetPetCurGrade()-1];
	}
	else if(0 == bFlag)
	{
		stamina = 100;		
	}
	else
		return;
	m_pCurSummonPet->SetPetStamina(stamina);
	UpdateCurPetInfo();
}
int CPetManager::SetSommonPetFriendship( DWORD dwFriendship )
{
	if( !m_pCurSummonPet )	return 1;
	DWORD max_friendly = GAMERESRCMNGR->GetPetRule()->MaxFriendship;
	if( dwFriendship > max_friendly )
		return 2;
	else
	{
		m_pCurSummonPet->SetPetFriendShip(dwFriendship);
		return 0;
	}
	UpdateCurPetInfo();
}
void CPetManager::SetSommonPetSkillReady()
{
	m_dwSkillRechargeAmount = PET_MAX_SKILL_CHARGE;
	m_bSkillGuageFull = TRUE;
	MSGBASE msg;
	msg.Category = MP_CHEAT;
	msg.Protocol = MP_CHEAT_PET_SKILLREADY_ACK;
	msg.dwObjectID = m_pPlayer->GetID();
	m_pPlayer->SendMsg(&msg, sizeof(msg));
}
void CPetManager::SetPetBuffInfo()
{
	if(!m_pCurSummonPet)	return;
	const BASE_PET_LIST* pBaseInfo = m_pCurSummonPet->GetPetBaseInfo();
	m_BuffFlag = 0;
	for( int i = 0; i < 3; ++i )	
	{
		WORD BuffIdx = pBaseInfo->BuffList[m_pCurSummonPet->GetPetCurGrade()-1][i];
		PET_BUFF_LIST* pList = GAMERESRCMNGR->GetPetBuffInfo(BuffIdx);
		if(!pList)	continue;
		WORD BuffKind = pList->BuffKind;
		m_BuffFlag |= (1<<BuffKind);
		m_BuffData[BuffKind].Prob = pList->BuffSuccessProb;
		m_BuffData[BuffKind].BuffValueData = pList->BuffValueData;
		m_BuffData[BuffKind].BuffAdditionalData = pList->BuffAdditionalData;
	}
}
void CPetManager::GetPetBuffResultRt( int BuffKind, void* Data )
{
	if(!m_pCurSummonPet)	return;
	if(m_pCurSummonPet->IsPetRest())	return;
	if(m_pCurSummonPet->IsPetStaminaZero())	return;
	if( !(m_BuffFlag & (1<<BuffKind)) )	return;
	DWORD RandV = random(0,100);
	switch(BuffKind)
	{
	case ePB_Demage_Percent:
		{
			if(RandV > m_BuffData[ePB_Demage_Percent].Prob)	return;
			double* pDamage = (double*)Data;
			double Damg = *pDamage;
			Damg = double(Damg + Damg*(m_BuffData[ePB_Demage_Percent].BuffValueData * 0.01));
			*pDamage = Damg;
		}
		break;
	case ePB_Dodge:
		{
			if(RandV > m_BuffData[ePB_Dodge].Prob)	return;
			BOOL* pDodge = (BOOL*)Data;
			*pDodge = TRUE;
		}
		break;
	case ePB_MasterAllStatUp:	
		{
			WORD PlusStat = (WORD)m_BuffData[ePB_MasterAllStatUp].BuffValueData;
			player_calc_stats* pMasterAdditionalStats = (player_calc_stats*)Data;
			pMasterAdditionalStats->GenGol += PlusStat;
			pMasterAdditionalStats->MinChub += PlusStat;
			pMasterAdditionalStats->CheRyuk += PlusStat;
			pMasterAdditionalStats->SimMaek += PlusStat;
		}
		break;
	case ePB_Item_DoubleChance:
		{
			if(RandV > m_BuffData[ePB_Item_DoubleChance].Prob)	return;
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
	case ePB_MasterAllStatRound:
		{
			player_calc_stats* pMasterAddStats = (player_calc_stats*)Data;
			pMasterAddStats->GenGol += GetAddStatFromRound(m_pPlayer->GetGenGol());
			pMasterAddStats->MinChub += GetAddStatFromRound(m_pPlayer->GetMinChub());
			pMasterAddStats->CheRyuk += GetAddStatFromRound(m_pPlayer->GetCheRyuk());
			pMasterAddStats->SimMaek += GetAddStatFromRound(m_pPlayer->GetSimMek());
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


	case ePB_ReduceCriticalRate:
		{
			if (RandV > m_BuffData[ePB_ReduceCriticalRate].Prob)	return;
			WORD* pAdd = (WORD*)Data;
			WORD Rate = *pAdd;
			Rate += m_BuffData[ePB_ReduceCriticalRate].BuffValueData;
			*pAdd = Rate;
		}
		break;
	case ePB_ReduceDemageRate:
		{
			if (RandV > m_BuffData[ePB_ReduceDemageRate].Prob)	return;
			
			DWORD* pDamage = (DWORD*)Data;
			DWORD Damg = *pDamage;
			Damg = Damg * (100 - m_BuffData[ePB_ReduceDemageRate].BuffValueData) * 0.01f;
			*pDamage = Damg;
		}
		break;
	case ePB_ReduceCriticalDmgPlayer:
		{
			if (RandV > m_BuffData[ePB_ReduceCriticalDmgPlayer].Prob)	return;
			double* pDamage = (double*)Data;
			double Damg = *pDamage;
			Damg = Damg * (100 - m_BuffData[ePB_ReduceCriticalDmgPlayer].BuffValueData) * 0.01f;
			*pDamage = Damg;
		}
		break;

	default:
		break;
	}
}
void CPetManager::RefleshPetMaintainBuff()
{
	if( (m_BuffFlag & (1<<ePB_MasterAllStatUp)) ||
		(m_BuffFlag & (1<<ePB_MasterAllStatRound)) )
		m_pPlayer->CalcState();
}
void CPetManager::SetPetEquipOption( DWORD ItemIdx, BOOL bAddOpt )
{
	ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo((WORD)ItemIdx);
	if(!pItemInfo)
	{
		//ASSERT(0);
		return;
	}
	int symbol = 1;
	if(FALSE == bAddOpt)
		symbol = -1;
	if(pItemInfo->LimitGenGol)	
	{
		m_PetEquipOption.iPetStaminaReductionDecrease += pItemInfo->LimitGenGol * symbol;
		if(m_PetEquipOption.iPetStaminaReductionDecrease < 0)
			m_PetEquipOption.iPetStaminaReductionDecrease = 0;
	}
	if(pItemInfo->LimitMinChub)
	{
		m_PetEquipOption.iPetStaminaMaxIncreaseAmount += pItemInfo->LimitMinChub * symbol;
		if(m_PetEquipOption.iPetStaminaMaxIncreaseAmount < 0)
			m_PetEquipOption.iPetStaminaMaxIncreaseAmount = 0;
		if(m_pCurSummonPet)
		{
			m_pCurSummonPet->CalcPetMaxStamina();
		}
	}
	if(pItemInfo->LimitCheRyuk)
	{
		m_PetEquipOption.iPetStaminaRecoverateIncrease += pItemInfo->LimitCheRyuk * symbol;
		if(m_PetEquipOption.iPetStaminaRecoverateIncrease < 0)
			m_PetEquipOption.iPetStaminaRecoverateIncrease = 0;
	}
	if(pItemInfo->LimitSimMek)
	{
		m_PetEquipOption.iPetStaminaRecoverateAmount += pItemInfo->LimitSimMek * symbol;
		if(m_PetEquipOption.iPetStaminaRecoverateAmount < 0)
			m_PetEquipOption.iPetStaminaRecoverateAmount = 0;
	}
	if(pItemInfo->ItemGrade)
	{
		m_PetEquipOption.iPetFriendshipIncrease += pItemInfo->ItemGrade * symbol;
		if(m_PetEquipOption.iPetFriendshipIncrease < 0)
			m_PetEquipOption.iPetFriendshipIncrease = 0;
	}
	if(pItemInfo->RangeType)	
	{
		m_PetEquipOption.fPetFriendshipProtectionRate += (float)pItemInfo->RangeType * 0.01f * symbol;
		if(m_PetEquipOption.fPetFriendshipProtectionRate < 0)
			m_PetEquipOption.fPetFriendshipProtectionRate = 0.f;
	}
}
void CPetManager::SetPetSummonning( int flag )	
{
	if(!m_pCurSummonPet)	return;
	if(ePSS_ReleaseSummon == flag)
	{
		m_pCurSummonPet->SetPetSummonning(FALSE);
	}
	else	
	{
		m_pCurSummonPet->SetPetSummonning(TRUE);
	}
}
void CPetManager::CheckSummonningPet()
{
	PET_TOTALINFO* pInfo = NULL;
	m_PetInfoList.SetPositionHead();
	while(pInfo = m_PetInfoList.GetData())
	{
		if( TRUE == pInfo->bSummonning )
		{
			SummonPet(pInfo->PetSummonItemDBIdx,FALSE);	
			SetPetSummonning(ePSS_ReleaseSummon);
			return;
		}
	}
}
void CPetManager::SetResummonCheckTime()
{
	m_dwResummonDelayTime = gCurTime;
}
BOOL CPetManager::CheckResummonAvailable()
{
	return ( gCurTime-m_dwResummonDelayTime > PET_RESUMMON_VALID_TIME );
}
void CPetManager::SendPetInfo( PET_TOTALINFO* pPetInfo )
{
	if(!pPetInfo)
	{
		return;
	}
	if(m_PetInfoList.GetData(pPetInfo->PetSummonItemDBIdx))
	{
		return;
	}
	SEND_ADDPET_FROMITEM msg;
	memcpy(&msg.Info, pPetInfo, sizeof(PET_TOTALINFO));
	msg.Category = MP_PET;
	msg.Protocol = MP_PET_ADDINFO;
	msg.dwObjectID = m_pPlayer->GetID();
	m_pPlayer->SendMsg(&msg, sizeof(SEND_ADDPET_FROMITEM));
}
void CPetManager::SetReduceAmountPetFriendship( CObject* pAttacker )
{
	if(!m_pCurSummonPet)	return;
	m_iFriendshipReduceAmount = GAMERESRCMNGR->GetPetRule()->FriendShipPerMasterDie;
	float applyRate = 1.0f;
	DWORD dwCheckBit = eTournament | eQuestRoom;
	if( (TRUE == g_pServerSystem->GetMap()->IsMapKind(dwCheckBit)) || (g_pServerSystem->GetMapNum() == 95) )
	{
		applyRate = 0;	
	}
	if(pAttacker->GetObjectKind() & eObjectKind_Monster )
	{
		if( m_pPlayer->GetBattle()->GetBattleKind() == eBATTLE_KIND_SURYUN )
		{
			applyRate = 0;
		}
	}
	else if( pAttacker->GetObjectKind() == eObjectKind_Player )
	{
		applyRate = 0;
	}
	if( m_PetEquipOption.fPetFriendshipProtectionRate > 1.f )
	{
		 m_PetEquipOption.fPetFriendshipProtectionRate = 1;
		// ASSERT(0);
	}
	if( m_PetEquipOption.fPetFriendshipProtectionRate > 0 )
	{
		applyRate *= (1 - m_PetEquipOption.fPetFriendshipProtectionRate);
	}
	m_iFriendshipReduceAmount = int(m_iFriendshipReduceAmount * applyRate);
}
void CPetManager::ReducePetFriendshipWithMastersRevive()
{
	if(!m_pCurSummonPet)	return;
	if( 0 == m_iFriendshipReduceAmount )	return;
	m_pCurSummonPet->AddFriendship(m_iFriendshipReduceAmount, TRUE);
	const PET_TOTALINFO* pInfo = m_pCurSummonPet->GetPetTotalInfo();
	LogPet(m_pPlayer->GetID(),m_pPlayer->GetUserID(),ePetLog_MasterDie, pInfo->PetDBIdx, pInfo->PetSummonItemDBIdx,
		pInfo->PetGrade,pInfo->PetStamina,pInfo->PetFriendly,pInfo->bAlive);
}