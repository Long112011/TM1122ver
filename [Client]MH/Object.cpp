







// Object.cpp: implementation of the CObject class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Object.h"
#include "MoveManager.h"
#include "ObjectManager.h"
#include "GameIn.h"
#include "ObjectStateManager.h"
#include "./interface/cWindowManager.h"
#include "./Interface/cScriptManager.h"
#include "ObjectBalloon.h"
#include "OBalloonChat.h"
#include "OBalloonName.h"
#include "StreetStallTitleTip.h"
#include "StunState.h"
#include "AmplifiedPower.h"

#include "ObjectGuagen.h"
#include "GameResourceManager.h"
#include "Status.h"

#include "MOTIONDESC.h"

#include "Hero.h"
#include "BattleSystem_Client.h"

#include "DetectState.h"
#include "BillBoardManager.h"

#include "GameState.h"
#include "MainGame.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CObject::CObject()
{
	m_pSpecialState[eSpecialState_Stun] = new CStunState(this);
	m_pSpecialState[eSpecialState_AmplifiedPowerPhy] = new CAmplifiedPowerPhy(this);
	m_pSpecialState[eSpecialState_AmplifiedPowerAtt] = new CAmplifiedPowerAtt(this);
	m_pObjectBalloon		= NULL;
	m_bObjectBalloonBits	= 0;
	m_StateEffectList.Initialize(3);
	m_pMotionDESC			= NULL;
	m_bSelected				= FALSE;

	m_ObjectKind			= 0;
}

CObject::~CObject()
{
	Release();

	for(int n=0;n<eSpecialState_Max;++n)

		delete m_pSpecialState[n];
}

BOOL CObject::Init(EObjectKind kind,BASEOBJECT_INFO* pBaseObjectInfo)
{
	for(int n=0;n<eSpecialState_Max;++n)
		m_pSpecialState[n]->Init();

	for(int i = 0; i < eSingleSpecialState_Max; i++)
		m_SingleSpecialStateUsedTime[i] = 0;


	m_bMoveSkipCount = 0;
	m_bIsYMoving = FALSE;

	memcpy(&m_BaseObjectInfo,pBaseObjectInfo,sizeof(m_BaseObjectInfo));

	m_ObjectKind = kind;
	m_ObjectState.bEndState = FALSE;

	SetFlag(FALSE);
//m_bDieFlag = FALSE;
	m_bInited = TRUE;

	return TRUE;
}

#include "ItemManager.h"
#include "famemanager.h"
#include "FortWarManager.h"
void CObject::InitObjectBalloon(BYTE bitFlag)
{
	m_pObjectBalloon = new CObjectBalloon;
	m_bObjectBalloonBits = bitFlag;

	this->InitTopListFlg();

	SetTopListFlg(0);



	/*switch( GetObjectKind() )
	{
	case eObjectKind_BossMonster:
	case eObjectKind_FieldBossMonster:
	case eObjectKind_Monster:
		{
			const BASE_MONSTER_LIST* monsterList = ( ( CMonster* )this )->GetSInfo();
			if( monsterList )
			{
				m_pObjectBalloon->InitBalloon( this, monsterList->Tall, bitFlag );
			}
			break;
		}
	case eObjectKind_Player:
		{
			m_pObjectBalloon->InitBalloon(this, 180, bitFlag);
			break;
		}
	case eObjectKind_Npc:
		{
			CNpc* npc = ( CNpc* )this;
			m_pObjectBalloon->InitBalloon( npc, npc->GetSInfo()->Tall, bitFlag );
			break;
		}
	case eObjectKind_Pet:
		{
			CPet* pPet = (CPet*)this;
			m_pObjectBalloon->InitBalloon(this, pPet->GetSInfo()->Tall[(pPet->GetPetCurGrade())-1], bitFlag);
			break;
		}
	default:
		{
			m_pObjectBalloon->InitBalloon(this, 180, bitFlag);
			break;
		}
	}*/

	/*if(! ( bitFlag & ObjectBalloon_Name ) )
	{
		return;
	}

	COBalloonName* balloonName =  m_pObjectBalloon->GetOBalloonName();
	if( ! balloonName )
	{
		return;
	}*/



	
	BYTE objectKind = GetObjectKind();
	if(objectKind == eObjectKind_Player)
	{
		m_pObjectBalloon->InitBalloon(this, /*170*/180, bitFlag);		
	}
	else if(objectKind & eObjectKind_Pet)
	{
		CPet* pPet = (CPet*)this;
		m_pObjectBalloon->InitBalloon(this, pPet->GetSInfo()->Tall[(pPet->GetPetCurGrade())-1], bitFlag);
	}
	else if(objectKind & eObjectKind_Monster)
	{
		BASE_MONSTER_LIST* pData = ((CMonster*)this)->GetSInfo();
		m_pObjectBalloon->InitBalloon(this, (LONG)(pData->Tall*pData->Scale), bitFlag);	
	}
	else if(objectKind & eObjectKind_Npc)
	{
		m_pObjectBalloon->InitBalloon(this, ((CNpc *)this)->GetSInfo()->Tall, bitFlag);	
	}
	else
	{
		m_pObjectBalloon->InitBalloon(this, 180, bitFlag);
	}

	/*switch( GetObjectKind() )
	{
	case eObjectKind_Player:
		{	
			balloonName->InitName(m_BaseObjectInfo.ObjectName);
			CPlayer*pPlayer=(CPlayer*)this;

			//SetGuildMark();
			//SetNickName();

			if( pPlayer->GetGuildMarkName() > 0 ) 
			{
				SetGuildMark();
			}
			m_pObjectBalloon->SetNickName(pPlayer->GetNickName());

			if(pPlayer->IsMarry())
			{
				pPlayer->SetMarryName(pPlayer->GetMarryName());
			}
			if(pPlayer->HasShiFu())
			{
				pPlayer->SetShiFuName(pPlayer->GetShiFuName());
			}
			if(pPlayer->IsFlashNameFlag())
			{
				pPlayer->SetFlashNameFlag(pPlayer->GetFlashNameFlag());
			}
			if(pPlayer->IsImageName())
			{
				m_pObjectBalloon->SetImageName(pPlayer->GetImageName());
			}
			if(pPlayer->IsFlashName())
			{
			    pPlayer->SetFlashName(pPlayer->GetFlashName());
			}
			if(pPlayer->GetFame() > 0)
			{
				pPlayer->SetFame(pPlayer->GetFame());

				CHAR balloon[MAX_MASTERNAME_LENGTH + 1];
				ZeroMemory(&balloon, sizeof(balloon));
				
				if( pPlayer->GetStage() == 64||pPlayer->GetStage() == 65 )
					sprintf(balloon, "@%d", ITEMMGR->GetFameLogoChange(pPlayer->GetFame()));
				if( pPlayer->GetStage() == 128||pPlayer->GetStage() == 129 )
					sprintf(balloon, "&%d", ITEMMGR->GetFameLogoChange(pPlayer->GetFame()));
				if(pPlayer->GetStage() == 0)
					sprintf(balloon, "");

				pPlayer->SetStageLogoBalloon(balloon);				
			}
			sBOARDMGR->SetHeroTopListFlg(pPlayer);
			break;
		}
	case eObjectKind_Npc:
		{				
			//CNpc* npc = ( CNpc* )this;
			//if( npc->GetSInfo()->ShowJob )
			//	m_pObjectBalloon->SetNickName(npc->GetSInfo()->Name);
			//{
			//	m_pObjectBalloon->SetNickName( npc->GetSInfo()->Name );
			//}
			//MAPCHANGE_INFO* info  = GAMERESRCMNGR->GetMapChangeInfo( npc->GetNpcUniqueIdx() );
			//balloonName->InitName( info ? info->ObjectName : m_BaseObjectInfo.ObjectName );
			if( ((CNpc*)this)->GetSInfo()->ShowJob )
				m_pObjectBalloon->SetNickName(((CNpc*)this)->GetSInfo()->Name);
			break;
		}
	case eObjectKind_Monster:
	case eObjectKind_TitanMonster:
		{
			m_pObjectBalloon->GetOBalloonName()->InitName(((CMonster *)this)->GetSInfo()->Name);
		}
	default:
		{
			//balloonName->InitName(
			//	GetObjectName());
			m_pObjectBalloon->GetOBalloonName()->InitName(m_BaseObjectInfo.ObjectName);
			break;
		}
	}*/




	if(bitFlag & ObjectBalloon_Name)
	{
		if ((objectKind == eObjectKind_Monster) || (objectKind == eObjectKind_TitanMonster))
		{
			m_pObjectBalloon->GetOBalloonName()->InitName(((CMonster *)this)->GetSInfo()->Name);
		}
		else
		{
			
			m_pObjectBalloon->GetOBalloonName()->InitName(m_BaseObjectInfo.ObjectName);
		}


		CPlayer* pPlayer=((CPlayer*)this);
		if(objectKind == eObjectKind_Player)
		{
			//if (!FORTWARMGR->IsWarStart())

			if (FORTWARMGR->IsWarStart())
			{
				m_pObjectBalloon->SetFortName(FORTWARMGR->GetObjectName());
			}
			{
				if (pPlayer->GetGuildMarkName() > 0)
				{
					SetGuildMark();
				}

				m_pObjectBalloon->SetNickName(pPlayer->GetNickName());

				if (pPlayer->IsMarry())
				{
					pPlayer->SetMarryName(pPlayer->GetMarryName());
				}
				if (pPlayer->HasShiFu())
				{
					pPlayer->SetShiFuName(pPlayer->GetShiFuName());
				}
				if (pPlayer->IsFlashNameFlag())
				{
					pPlayer->SetFlashNameFlag(pPlayer->GetFlashNameFlag());
				}
				if (pPlayer->IsImageName())
				{
					m_pObjectBalloon->SetImageName(pPlayer->GetImageName());
				}
				if (pPlayer->IsFlashName())
				{
					pPlayer->SetFlashName(pPlayer->GetFlashName());
				}
				if (pPlayer->GetFame() > 0)
				{
					pPlayer->SetFame(pPlayer->GetFame());

					CHAR balloon[MAX_MASTERNAME_LENGTH + 1];
					ZeroMemory(&balloon, sizeof(balloon));

					if (pPlayer->GetStage() == 64 || pPlayer->GetStage() == 65)
						sprintf(balloon, "@%d", FAMEMGR->GetFameLogoChange(pPlayer->GetFame()));
					if (pPlayer->GetStage() == 128 || pPlayer->GetStage() == 129)
						sprintf(balloon, "&%d", FAMEMGR->GetFameLogoChange(pPlayer->GetFame()));
					if (pPlayer->GetStage() == 0)
						sprintf(balloon, "");

					pPlayer->SetStageLogoBalloon(balloon);
				}
				//if (pPlayer->GetKillCount() > -1)
				{
					std::string s = std::to_string(pPlayer->GetKillCount());
					char* c = strcpy(new char[s.length() + 1], s.c_str());

					m_pObjectBalloon->SetKillCount(c);
					//SetKillCountBalloon
					//pPlayer->SetFame(pPlayer->GetFame());
				}
				sBOARDMGR->SetHeroTopListFlg(pPlayer);
				//sBOARDMGR->SetHeroTopListGameIn(pPlayer->GetID());
			}
		}
		if(objectKind == eObjectKind_Npc)
		{
			if( ((CNpc*)this)->GetSInfo()->ShowJob )
				m_pObjectBalloon->SetNickName(((CNpc*)this)->GetSInfo()->Name);
		}
	}
	// chat
	cImage top1, top2, top3, middle1, middle2, middle3, bottom1, bottom2, bottom3, tail; 
	SCRIPTMGR->GetImage(66, &top1);
	SCRIPTMGR->GetImage(67, &top2);
	SCRIPTMGR->GetImage(68, &top3);
	SCRIPTMGR->GetImage(69, &middle1);
	SCRIPTMGR->GetImage(70, &middle2);
	SCRIPTMGR->GetImage(71, &middle3);
	SCRIPTMGR->GetImage(72, &bottom1);
	SCRIPTMGR->GetImage(73, &bottom2);
	SCRIPTMGR->GetImage(74, &bottom3);
	SCRIPTMGR->GetImage(75, &tail);


	if( bitFlag & ObjectBalloon_Chat )
	{
		m_pObjectBalloon->GetOBalloonChat()->InitBalloon(&top1, &top3, &bottom1, &bottom3, &middle1, &middle3, &top2, &bottom2, &middle2, &tail, 8);
		cImage left1;
		cImage left2;
		cImage left3;
		cImage center1;
		cImage center2;
		cImage center3;
		cImage right1;
		cImage right2;
		cImage right3;
		
		SCRIPTMGR->GetImage(99, &left1);
		SCRIPTMGR->GetImage(100, &center1);
		SCRIPTMGR->GetImage(101, &right1);
		SCRIPTMGR->GetImage(102, &left2);
		SCRIPTMGR->GetImage(103, &center2);
		SCRIPTMGR->GetImage(104, &right2);
		SCRIPTMGR->GetImage(105, &left3);
		SCRIPTMGR->GetImage(106, &center3);
		SCRIPTMGR->GetImage(107, &right3);

		m_pObjectBalloon->GetOBalloonChat()->InitBalloonForChat(&left1, &center1, &right1,
			&left2, &center2, &right2, &left3, &center3, &right3, 8);
	}

	if( bitFlag & ObjectBalloon_Title)
	{
		m_pObjectBalloon->GetSSTitleTip()->InitBalloon(&top1, &top3, &bottom1, &bottom3, &middle1, &middle3, &top2, &bottom2, &middle2, NULL, 8);
		m_pObjectBalloon->GetSBSTitleTip()->InitBalloon(&top1, &top3, &bottom1, &bottom3, &middle1, &middle3, &top2, &bottom2, &middle2, NULL, 8);

		// StreetStall Title
		{						
			cImage left1;
			cImage left2;
			cImage left3;
			cImage center1;
			cImage center2;
			cImage center3;
			cImage right1;
			cImage right2;
			cImage right3;
			
			SCRIPTMGR->GetImage(117, &left1);
			SCRIPTMGR->GetImage(118, &center1);
			SCRIPTMGR->GetImage(119, &right1);
			SCRIPTMGR->GetImage(120, &left2);
			SCRIPTMGR->GetImage(121, &center2);
			SCRIPTMGR->GetImage(122, &right2);
			SCRIPTMGR->GetImage(123, &left3);
			SCRIPTMGR->GetImage(124, &center3);
			SCRIPTMGR->GetImage(125, &right3);		
			
			m_pObjectBalloon->GetSSTitleTip()->InitBalloonForChat(&left1, &center1, &right1,&left2, &center2, &right2, &left3, &center3, &right3, 8);

				

		}
		{						
			cImage left1;
			cImage left2;
			cImage left3;
			cImage center1;

			cImage center2;
			cImage center3;
			cImage right1;
			cImage right2;
			cImage right3;
			
			SCRIPTMGR->GetImage(128, &left1);
			SCRIPTMGR->GetImage(129, &center1);
			SCRIPTMGR->GetImage(130, &right1);
			SCRIPTMGR->GetImage(131, &left2);
			SCRIPTMGR->GetImage(132, &center2);
			SCRIPTMGR->GetImage(133, &right2);
			SCRIPTMGR->GetImage(134, &left3);

			SCRIPTMGR->GetImage(135, &center3);
			SCRIPTMGR->GetImage(136, &right3);		
			
			m_pObjectBalloon->GetSBSTitleTip()->InitBalloonForChat(&left1, &center1, &right1,
				&left2, &center2, &right2, &left3, &center3, &right3, 8);
		}
	}	
}

//void CObject::AddChatBalloon( char * chatMsg )
//{
//	ShowChatBalloon( TRUE, chatMsg );
//}

//-----
void CObject::SetFortName(char* name)
{
	if (m_pObjectBalloon)
		m_pObjectBalloon->SetFortName(name);
}
void CObject::SetName(char* name)
{
	if( m_pObjectBalloon )	
		m_pObjectBalloon->SetName( name );
}
void CObject::SetOverInfoOption( DWORD dwOption )
{
	if( m_pObjectBalloon )	
		m_pObjectBalloon->SetOverInfoOption( dwOption );
}

void CObject::SetGuildMark()
{
	if( m_pObjectBalloon )
		m_pObjectBalloon->SetGuildMark( ((CPlayer*)this)->GetGuildMarkName() );
}

void CObject::SetNickName()
{
	if( m_pObjectBalloon )
		m_pObjectBalloon->SetNickName( ((CPlayer*)this)->GetNickName() );
}

void CObject::ShowObjectName( BOOL bShow, DWORD dwColor )//show/hide
{
	if( !m_pObjectBalloon )								return;
	if( !(m_bObjectBalloonBits & ObjectBalloon_Name) )	return;

	m_pObjectBalloon->ShowObjectName( bShow, dwColor );
}

void CObject::ShowChatBalloon( BOOL bShow, char* chatMsg, DWORD dwColor, DWORD dwAliveTime )
{
	if( !m_pObjectBalloon )								return;
	if( !(m_bObjectBalloonBits & ObjectBalloon_Chat) )	return;

	m_pObjectBalloon->ShowChatBalloon( bShow, chatMsg, dwColor, dwAliveTime );
}

//-----

void CObject::InitMove(BASEMOVE_INFO* pMoveInfo)
{
	memset(&m_MoveInfo,0,sizeof(MOVE_INFO));
	memcpy(&m_MoveInfo,pMoveInfo,sizeof(BASEMOVE_INFO));
}

void CObject::Release()
{
	m_StatusList.RemoveAll();
	
	for(int n=0;n<eSpecialState_Max;++n)
		m_pSpecialState[n]->EndState();

	m_bMoveSkipCount = 0;
	m_bInited = FALSE;

	if(m_ShadowObj.IsInited())
	{
		m_ShadowObj.Release();
	}
	RemoveAllObjectEffect();

	m_ObjectKind = 0;
	m_BaseObjectInfo.ObjectState = -1;
	m_EngineObject.Release();
	
	SAFE_DELETE( m_pObjectBalloon );
	m_bObjectBalloonBits = 0;
}

void CObject::GetBaseObjectInfo(BASEOBJECT_INFO* pRtInfo)
{
	memcpy(pRtInfo,&m_BaseObjectInfo,sizeof(BASEOBJECT_INFO));
}

void CObject::GetBaseMoveInfo(BASEMOVE_INFO* pRtInfo)
{
	memcpy(pRtInfo,&m_MoveInfo,sizeof(BASEMOVE_INFO));
}

void CObject::Process()
{
	SpecialStateProcess(gTickTime);
	OBJECTSTATEMGR->StateProcess(this);
	MOVEMGR->MoveProcess(this);

	if(m_ShadowObj.IsInited())
		m_ShadowObj.SetEngObjPosition(&GetCurPosition());
	
	/*
	HEFFPROC hEff;
	m_StateEffectList.SetPositionHead();
	while(hEff = m_StateEffectList.GetData())
		EFFECTMGR->Process(gCurTime,hEff);
	*/
}

void CObject::SetFlag(BOOL val)
{
	m_bDieFlag = val; 
}

void CObject::SetMarryNameBalloon(char * name)
{
	m_pObjectBalloon->SetMarryName(name);
}

void CObject::SetDieFlag()
{
	m_DiedTime = gCurTime;
	SetFlag(TRUE);
	MOVEMGR->MoveStop(this);
	HERO->ClearTarget(this);

	if( OBJECTMGR->GetSelectedObjectID() == GetID() )
	{
		OBJECTMGR->SetSelectedObjectID(0);
		OnDeselected();
		if( OBJECTMGR->CanOverInfoChange( this ) )
			ShowObjectName( FALSE );
		else
			ShowObjectName( TRUE, OBJECTMGR->GetObjectNameColor(this) );
	}
	if( m_ObjectKind == eObjectKind_Player )
		m_EngineObject.EnablePick();
	else
		m_EngineObject.DisablePick();
}
void CObject::Revive(VECTOR3* pRevivePos)
{
	SetFlag(FALSE);
	MOVEMGR->SetPosition(this,pRevivePos);
}

void CObject::SetState(BYTE State)
{
	SetMotionInState(State);
	
	if(m_ObjectState.State_Start_Motion != -1)
	{
		ChangeMotion(m_ObjectState.State_Start_Motion,FALSE);

		if(m_ObjectState.State_Ing_Motion != -1)
			ChangeBaseMotion(m_ObjectState.State_Ing_Motion);
	}
	//m_ObjectState.BeforeState	= m_BaseObjectInfo.ObjectState;
	m_BaseObjectInfo.ObjectState		= State;
	m_ObjectState.State_Start_Time = gCurTime;
	m_ObjectState.bEndState		= FALSE;
}

BYTE CObject::GetState()
{
	return m_BaseObjectInfo.ObjectState;
}
void CObject::SetPosition(VECTOR3* pPos)
{
	GetEngineObject()->SetEngObjPosition(pPos);
}
void CObject::SetAngle(float AngleRad)
{
	GetEngineObject()->SetEngObjAngle(AngleRad);
}
void CObject::GetPosition(VECTOR3* pPos)
{
	GetEngineObject()->GetEngObjPosition(pPos);
}
VECTOR3 CObject::GetPosition(void)
{
	VECTOR3 l_v3Pos;
	GetEngineObject()->GetEngObjPosition(&l_v3Pos);

	return l_v3Pos;
}
float CObject::GetAngle()
{
	return GetEngineObject()->GetEngObjAngle();
}
DIRINDEX CObject::GetDirectionIndex()
{
	float angleRad = GetAngle();
	DIRINDEX dir = RADTODIR(angleRad);
	while(dir >= MAX_DIRECTION_INDEX)
		dir -= MAX_DIRECTION_INDEX;
	return dir;
}

void CObject::AddStatus(CStatus* pStatus)
{
	m_StatusList.AddTail(pStatus);
}
void CObject::RemoveStatus(CStatus* pStatus)
{
	BOOL rt = m_StatusList.Remove(pStatus);
}

void CObject::ChangeMotion(int motion,BOOL bRoop)
{
	GetEngineObject()->ChangeMotion(motion,bRoop);
}

void CObject::ChangeBaseMotion(int motion)
{
	GetEngineObject()->ChangeBaseMotion(motion);
}


float CObject::GetAttDefense(WORD Attrib)
{
	if(Attrib == ATTR_NOATTR)

		return 0;
	
	float Ori = DoGetAttDefense(Attrib);
	float Up = 0,Down = 0;
	PTRLISTSEARCHSTART(m_StatusList,CStatus*,pSL)
		pSL->GetAttDefense(Attrib,Ori,Up,Down);
	PTRLISTSEARCHEND;
	float res = STATUSCALC(Ori,Up,Down);
#ifndef _JAPAN_LOCAL_	
	if(res > 1)
		res = 1;
#endif
	return res;
}

void CObject::OnSelected()
{
	GetEngineObject()->EnableSelfIllumin();
	m_bSelected = TRUE;
}

void CObject::OnDeselected()
{
	GetEngineObject()->DisableSelfIllumin();
	m_bSelected = FALSE;
}

void CObject::SetBattle(DWORD BattleID,BYTE Team)
{
	m_BaseObjectInfo.BattleID = BattleID;
	m_BaseObjectInfo.BattleTeam = Team;
}

void CObject::AddObjectEffect(DWORD KeyValue,OBJECTEFFECTDESC* EffectNumArray,WORD NumOfEffect,CObject* pSkillOperator)
{
	// test
//	if(GetState() == eObjectState_Die)
//	{
//		int a=0;
//	}

	
	HEFFPROC heff;

	// ?¶Ë?Á∏?Ê∞àÊ? Ê™úÔõµ?æË? ?äÊ?Ê£?
	BOOL bFound = FALSE;
	m_StateEffectList.StartGetMultiData(KeyValue);
	while(heff = (HEFFPROC)m_StateEffectList.GetMultiData())
	{
		EFFECTMGR->IncEffectRefCount(heff);
		bFound = TRUE;
	}

	// Ê∞àÊ≠∑?≤Ë? ?¨Ê?		//kes***
	if(bFound == TRUE)
		return;

	// Ê©àÊ≠∑?≤Ë? ?ÑÁ? ?úË?Ê£?
	for(int n=0;n<NumOfEffect;++n)
	{
		if(EffectNumArray[n].Effect == 0)
			continue;
		heff = EFFECTMGR->StartSkillMainTainEffect(
			EffectNumArray[n].Effect,this,EffectNumArray[n].Flag,&EffectNumArray[n].Position,pSkillOperator);
		if(heff == NULL)
			continue;
		m_StateEffectList.Add((void*)heff,KeyValue);
	}
}

void CObject::RemoveObjectEffect(DWORD KeyValue)
{
    if( m_bInited != 1 )
	{
		ASSERT(0);
		return;
	}

	HEFFPROC heff;
	// ?¶Ë?Á∏?Ê∞àÊ? Ê™úÔõµ?æË? ?äÊ?Ê£?
	BOOL bFound = FALSE;
	m_StateEffectList.StartGetMultiData(KeyValue);
	
	cPtrList list;

	while(heff = (HEFFPROC)m_StateEffectList.GetMultiData())
	{
		if(EFFECTMGR->ForcedEndEffect(heff) == TRUE)
		{
//			m_StateEffectList.RemoveCurMultiData();
			list.AddTail((void*)KeyValue);
		}
	}

	PTRLISTPOS pos = list.GetHeadPosition();
	while( pos )
	{
		DWORD k = (HEFFPROC)list.GetNext(pos);
		m_StateEffectList.Remove(k);
	}
	list.RemoveAll();

}
void CObject::RemoveAllObjectEffect()
{
	HEFFPROC heff;
	m_StateEffectList.SetPositionHead();
	while(heff = (HEFFPROC)m_StateEffectList.GetData())
	{
		while(EFFECTMGR->ForcedEndEffect(heff) == FALSE)
		{}
	}
	m_StateEffectList.RemoveAll();	//KES ?¨Ëù∂?æÁ????ñÈå∂Á¥??éÈá≠? forceEnd??ÂØ∞ËÑπ?≥Ëæ¶??.confirm
}
int	CObject::GetMotionIDX(int mainMotion, int subMotion)
{
	return m_pMotionDESC->GetMotionIDX(mainMotion, subMotion);
}

void CObject::Die(CObject* pAttacker,BOOL bFatalDamage,BOOL bCritical, BOOL bDecisive)
{
	for(int n=0;n<eSpecialState_Max;++n)
		m_pSpecialState[n]->EndState();

//	RemoveAllObjectEffect();	
//	m_StatusList.RemoveAll();
}

void CObject::StartSpecialState(DWORD SpecialStateKind,DWORD Time,
								WORD wParam1,WORD wParam2,float fParam3,
								WORD EffectNum,WORD StateIcon,BOOL bHeroOper)
{
	m_pSpecialState[SpecialStateKind]->StartState(Time,wParam1,wParam2,fParam3,EffectNum,StateIcon,bHeroOper);
}
void CObject::EndSpecialState(DWORD SpecialStateKind)
{
	m_pSpecialState[SpecialStateKind]->EndState();
}
BOOL CObject::IsInSpecialState(DWORD SpecialStateKind)
{
	return m_pSpecialState[SpecialStateKind]->IsInState();
}
void CObject::SpecialStateProcess(DWORD TickTime)
{
	for(int n=0;n<eSpecialState_Max;++n)
	{
		m_pSpecialState[n]->Process(TickTime);
	}
}
WORD CObject::GetSpecialStateParam1(DWORD SpecialStateKind)
{
	return m_pSpecialState[SpecialStateKind]->GetParam1();
}

WORD CObject::GetSpecialStateParam2(DWORD SpecialStateKind)
{
	return m_pSpecialState[SpecialStateKind]->GetParam2();
}

BOOL CObject::IsNpcMark( DWORD dwValue )
{
	HEFFPROC heff = (HEFFPROC)m_StateEffectList.GetData( dwValue );
	if( heff )
		return TRUE;
	return FALSE;
}

void CObject::SetKyungGongLevel( WORD wLevel )
{
	m_MoveInfo.AbilityKyungGongLevel = wLevel;
}

void CObject::SetObjectBalloonTall(LONG Tall)
{
	m_pObjectBalloon->SetTall(Tall);
}
void CObject::SetShiTuNameBalloon(char* name)
{
	m_pObjectBalloon->SetShiTuName(name);
}



void CObject::SetImageNameBalloon(WORD ImageName)
{
	m_pObjectBalloon->SetImageName(ImageName);
}
void CObject::SetFameBalloon(char * fame)
{
	m_pObjectBalloon->SetFame(fame);
}
void CObject::SetStageLogoBalloon(char * StageLogo)
{
	m_pObjectBalloon->SetStageLogo(StageLogo);
}
void CObject::SetTopListFlg(WORD val)
{
	m_pObjectBalloon->SetTopListFlg(val);
}
void CObject::InitTopListFlg()
{
	m_pObjectBalloon->InitTopListFlg();
}
void CObject::SetFameRankBalloon(char* fame)
{
	m_pObjectBalloon->SetFameRank(fame);
}

void CObject::Stop( VECTOR3* p_pStopPos )
{
	/*if ( p_pStopPos == NULL )
	{
		SetPosition(p_pStopPos);

		if (GetState() == eObjectState_Move)
		{
			// ?¥Î?Ï§ëÏ? ?πÏ? ?êÌ?Î™®Î?Í∞Ä ?úÏÑ±?????àÎã§Î©?Î™®Î???Î∞îÍæ∏ÏßÄ ?äÎ???
			CPlayer* t_Player = dynamic_cast<CPlayer*>(this);
			if ( t_Player && (t_Player->GetJumpMode() != eJumpMode_None) ) 
			{
				return;
			}

			// ObjectState??ObjectState_None???úÎã§
			OBJECTSTATEMGR->EndObjectState(this, eObjectState_Move);
		}
		//// ?§ÌÇ¨ ?¨Ïö©Ï§ëÏùº?åÎ? Î≤†Ïù¥??Î™®Ï?Îß?Î∞îÍ?Ï§Ä??(?àÍ∑∏?¨Î©¥ ?¥Ì??∏Î? ?òÍ?, Î≤†Ïù¥??Î™®Ï? ?àÎ?ÍøîÏ£ºÎ©??§ÌÇ¨ ?¨Ïö©???¨Î¶º)
		else if (GetObjectKind() == eObjectKind_Player && GetState() == eObjectState_SkillSyn)
		{
			ChangeBaseMotion( ((CPlayer*)this)->GetStandardMotion());
		}
		//////////////////////////////////////////////////////////////////////////
	}
	else
	{
		//// ?Ä?îÍ±∏ ÎßâÍ∏∞?ÑÌï¥ ?¥Î??ºÎ? Ï≤òÎ¶¨
		IMove* l_pMove = GetMove();
		if(l_pMove == NULL)
		{
			return;
		}
		l_pMove->SetMove( &GetPosition(), p_pStopPos, gCurTime, GetMoveSpeed(), 1.0f);
		//////////////////////////////////////////////////////////////////////////
	}*/

}
void CObject::SetFlashNameBalloonFlag(int Flag)
{
	m_pObjectBalloon->SetFlashNameFlag(Flag);
}


void CObject::SetFlashNameBalloon(char * FlashName)
{
	m_pObjectBalloon->SetFlashName(FlashName);
}

void CObject::SetKillCountBalloon(char * kill)
{
	m_pObjectBalloon->SetKillCount(kill);
}