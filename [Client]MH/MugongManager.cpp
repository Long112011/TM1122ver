







// MugongManager.cpp: implementation of the CMugongManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MugongManager.h"
#include "GameIn.h"
#include "./Interface/cWindowManager.h"
#include "./Interface/cScriptManager.h"

#include "ItemManager.h"
#include "chatmanager.h"
#include "GameDef.h"
#include "QuickManager.h"
#include "SkillManager_Client.h"
#include "JinBub.h"
#include "SimBub.h"
#include "ObjectManager.h"
#include "CheatMsgParser.h"
#include "GameEventManager.h"
#include "QuickDialog.h"
extern char JACKJACK[64];
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
GLOBALTON(CMugongManager)
CMugongManager::CMugongManager()
{
	m_pMugongDlg = NULL;
	m_MugongHash.Initialize(100);
	m_IconIndexCreator.Init(MAX_ITEMICON_NUM, IG_MUGONG_STARTINDEX);		
	
}

CMugongManager::~CMugongManager()
{
	Release();

	m_IconIndexCreator.Release();
}

void CMugongManager::Release()
{
	CMugongBase * pMugong = NULL;
	m_MugongHash.SetPositionHead();
	while(pMugong = m_MugongHash.GetData())
	{
		DeleteMugongIcon(pMugong);
		//m_IconIndexCreator.ReleaseIndex(pMugong->GetID());		
	}
	m_MugongHash.RemoveAll();
}
void CMugongManager::Init()
{
}

void CMugongManager::NetworkMsgParse(BYTE Protocol,void* pMsg)
{
	switch(Protocol) 
	{
	case MP_MUGONG_TOTALINFO_LOCAL:
		{
			MUGONG_TOTALINFO * msg = (MUGONG_TOTALINFO *)pMsg;
			for(int i = 0 ; i < SLOT_MUGONGTOTAL_NUM ; i++)
			{
#ifdef _SKILLSLOT_
				m_pMugongDlg->RefreshSlot();
#endif
				m_pMugongDlg->AddMugong(&msg->mugong[i]);
				
				// magi82 - Titan(070611) 타이탄 무공변환 주석처리
				//m_pMugongDlg->AddMugong(&msg->Titanmugong[i]);
				if(msg->mugong[i].QuickPosition != 0)
				{
					// 06. 01 이영준 - 단축창 변경
					// 초기 설정 함수 AddQuickItemReal -> SetQuickItemReal
					QUICKMGR->SetQuickItemReal(msg->mugong[i].QuickPosition,msg->mugong[i].Position, msg->mugong[i].wIconIdx);
				}
			}

			// magi82 - Titan(070319)
			for(int i = 0 ; i < SLOT_MUGONGTOTAL_NUM ; i++)
			{
				char line[64] = { 0, };
				CMugongBase* pMugong = GetMugong(msg->mugong[i].wIconIdx);

				if( pMugong )
					SetToolTipIcon(pMugong);
			}
		}
		break;
	case MP_MUGONG_MOVE_ACK:
		{
			MSG_MUGONG_MOVE_ACK * msg = (MSG_MUGONG_MOVE_ACK *)pMsg;
			m_pMugongDlg->MoveMugong(msg->ToPos, msg->FromPos);

			GAMEIN->GetMugongDialog()->SetDisable( FALSE );
			WINDOWMGR->DragWindowNull();
		}
		break;
	case MP_MUGONG_MOVE_NACK:
		{
			GAMEIN->GetMugongDialog()->SetDisable( FALSE );
			WINDOWMGR->BackDragWindow();
		}
		break;
	case MP_MUGONG_REM_ACK:
		{
			MSG_MUGONG_REM_ACK * pmsg = (MSG_MUGONG_REM_ACK *)pMsg;

			CMugongBase * mugongOut;
			m_pMugongDlg->DeleteMugong(pmsg->TargetPos, &mugongOut);
			
			DeleteMugongIcon(mugongOut);

			// magi82 - Titan(070611) 타이탄 무공변환 주석처리
			/*
			CSkillInfo*	pSInfo = SKILLMGR->GetSkillInfo(mugongOut->GetMugongBase()->wIconIdx);
			CMugongBase* pTitanMugong = GetMugong(pSInfo->GetSkillIndex() + SKILLNUM_TO_TITAN);
			if( pTitanMugong )
			{
				m_MugongHash.Remove(pSInfo->GetSkillIndex() + SKILLNUM_TO_TITAN);
			}
			*/

			GAMEIN->GetMugongDialog()->SetDisable( FALSE );

			ITEMMGR->RefreshAllItem();
/*
			MSG_MUGONG_REM_ACK * pmsg = (MSG_MUGONG_REM_ACK *)pMsg;
			switch(pmsg->wTableIdx)
			{
			case eItemTable_Mugong:
				{
					CMugongBase * mugongOut;
					m_pMugongDlg->DeleteMugong(pmsg->AbsPosition, &mugongOut);
					DeleteMugongIcon(mugongOut);
				}
				break;
			default:
				{
//					ASSERT(0);
				}
			}
			
			GAMEIN->GetMugongDialog()->SetDisable( FALSE );
*/
		}
		break;
	case MP_MUGONG_REM_NACK:
		{
			ASSERT(0);
			GAMEIN->GetMugongDialog()->SetDisable( FALSE );
			WINDOWMGR->BackDragWindow();
		}
		break;
/*	
	case MP_MUGONG_DEL_ACK:
		{
			MSG_MUGONG_DELINFO * msg = (MSG_MUGONG_DELINFO *)pMsg;
			CMugongBase * mugongOut;
			m_pMugongDlg->DeleteMugong(msg->wPosition, &mugongOut);
			DeleteMugongIcon(mugongOut);
		}
		break;
	case MP_MUGONG_DEL_NACK:
		{
			ASSERT(0);
		}
		break;
*/
	case MP_MUGONG_ADD_ACK:
		{
			MSG_MUGONG_ADD_ACK * msg = (MSG_MUGONG_ADD_ACK *)pMsg;
			// 무공책 삭제
//			ITEMMGR->DeleteItem( msg->MugongBase.Position );
			// 무공 생성, 등록
			m_pMugongDlg->AddMugong(&msg->MugongBase);
			
			//msg출력
			CMugongBase * icon = GetMugong(msg->MugongBase.wIconIdx);
			if( !icon ) return;
			CSkillInfo* pCommSkill = icon->m_pSkillInfo;
			if(!pCommSkill)	return;
			// 절초면 연출
			if(SKILLMGR->IsDeadlyMugong(pCommSkill->GetSkillIndex()))
			{
				// magi82(4) - Titan(071023) 타이탄일때 이펙트 처리(절초)
				if(HERO->InTitan() == TRUE)
                    EFFECTMGR->StartHeroEffectProcess(eEffect_Titan_ChangeMugong);
				else
					EFFECTMGR->StartHeroEffectProcess(eEffect_Mugong_ChangeMugong);

				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(791), pCommSkill->GetSkillName() );
			}
			else if(msg->MugongBase.Sung == 0) // 무공획득
			{
				// magi82(4) - Titan(071023) 타이탄일때 이펙트 처리(무공획득)
				//if(HERO->InTitan() == TRUE)
				//	EFFECTMGR->StartHeroEffectProcess(eEffect_Titan_GetMugong);
				//else
				//	EFFECTMGR->StartHeroEffectProcess(eEffect_Mugong_GetMugong);


				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(189), pCommSkill->GetSkillName() );
				GAMEIN->GetMugongDialog()->SetDisable(FALSE);				
			}
			else if(SKILLMGR->CheckSkillKind(pCommSkill->GetSkillKind()) == TRUE) //전문스킬 습득
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 184 ),pCommSkill->GetSkillName() );
			}
			else 
			{
				if( SKILLMGR->GetSkillChangeInfo( msg->MugongBase.wIconIdx ) )
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 677 ),pCommSkill->GetSkillName() );
			}
			// event
			GAMEEVENTMGR->AddEvent(eGameEvent_Mugong0);

			ITEMMGR->RefreshAllItem();
		}
		break;
	case MP_MUGONG_ADD_NACK:
		{
//			ASSERT(0);
			GAMEIN->GetMugongDialog()->SetDisable(FALSE);
		}
		break;
/*		
	case MP_MUGONG_DELETEGROUNDADD_ACK:
		{
			// target : MUGONG DEL -> GROUND ADD(HERO위치)
//			MSG_MUGONG_DELGRDADD_ACK * msg  = (MSG_MUGONG_DELGRDADD_ACK *)pMsg;
		//	m_pMugongDlg->DeleteMugong(msg->bMugongType);
			
		}
		break;
	case MP_MUGONG_DELETEINVENTORYADD_ACK:
		{
			// target : MUGONG DEL -> Inventory ADD
//			MSG_MUGONG_DELINVADD_ACK * msg  = (MSG_MUGONG_DELINVADD_ACK * )pMsg;
		}
		break;	
	case MP_MUGONG_DELETEGROUNDADD_NACK:
		{
			// invalid target
			// 에러 메시지 
			LOG(EC_MUGONG_ADDNITEMDELETE_FAILED);
		}
		break;
	case MP_MUGONG_DELETEINVENTORYADD_NACK:
		{
			// invalid target
			// 에러 메시지 
			LOG(EC_MUGONG_ADDNITEMDELETE_FAILED);
		}
		break;
*/
	case MP_MUGONG_SUNG_NOTIFY:
		{
			MSG_WORD3 * pmsg = (MSG_WORD3 * )pMsg;
			m_pMugongDlg->SetMugongSung(pmsg->wData1, (BYTE)pmsg->wData2);

			CMugongBase* mugong = (CMugongBase*)m_pMugongDlg->GetMugongAbs(pmsg->wData1);
			if( !mugong ) return;

			CSkillInfo* pInfo = mugong->m_pSkillInfo;


			if( pInfo )
			{
				if(pmsg->wData3 == eLog_MugongLevelup)
				{
					if( pmsg->wData2 < 12 )
						CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(182), pInfo->GetSkillName(), pmsg->wData2 );
					else
						CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(183), pInfo->GetSkillName() );
					
					if( pmsg->wData2 == 1 )
						GAMEEVENTMGR->AddEvent( eGameEvent_Mugong1 );
				}
				else if(pmsg->wData3 == eLog_MugongLevelDownByBadFame)	//악명치 높아 파괴
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 677 ),pInfo->GetSkillName() );
				else if(pmsg->wData3 == eLog_MugongLevelDownByWanted)	//현상범이므로 파괴
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 687 ),pInfo->GetSkillName() );
			}

			/*
			CMugongBase * mugongOut = NULL;
			mugongOut = (CMugongBase *)m_pMugongDlg->GetIconForIdx(pmsg->wData1);
			if(mugongOut->GetDBIdx() > 0) mugongOut->SetSung(pmsg->wData2);
			// 인터페이스
			// 무공 창의 성표시 
			// 무공 창의 경험치 max 게이지 setting

			*/
			RefreshMugong();
			ITEMMGR->RefreshAllItem();
		}
		break;
	case MP_MUGONG_EXPPOINT_NOTIFY:
		{
			MSG_MUGONGEXP* pmsg = (MSG_MUGONGEXP*)pMsg;
			m_pMugongDlg->SetMugongExpPoint((POSTYPE)pmsg->absPos,pmsg->ExpPoint);
			GAMEIN->GetQuickDialog()->RefreshIcon(m_pMugongDlg->GetMugongAbs((POSTYPE)pmsg->absPos)->GetQuickPosition());
		}
		break;
	case MP_MUGONG_SUNG_LEVELUP:
		{
			MSGBASE* pmsg = (MSGBASE*)pMsg;

			CPlayer* pPlayer = (CPlayer*)OBJECTMGR->GetObject( pmsg->dwObjectID );
			if( !pPlayer )			return;

			// magi82(4) - Titan(071023) 타이탄일때 이펙트 처리(무공레벨업)
			if(pPlayer->InTitan() == TRUE)
                EFFECTMGR->StartEffectProcess(eEffect_TitanLevelUpSentence,pPlayer,NULL,0,0);
			else
				EFFECTMGR->StartEffectProcess(eEffect_LevelUpSentence,pPlayer,NULL,0,0);
		}
		break;
	case MP_MUGONG_OPTION_ACK:
		{
			MSG_WORD4* pmsg = (MSG_WORD4*)pMsg;

			SKILLOPTION* pOption = SKILLMGR->GetSkillOptionByItemIndex(pmsg->wData3);
			if(!pOption) return;

			CMugongBase* pMugong = GetMugongByMugongIdx(pmsg->wData1);
			if(!pMugong) return;

			CItem* pOutItem = NULL;
			ITEMMGR->DeleteItem( pmsg->wData4, &pOutItem);
			

			pMugong->GetMugongBase()->OptionIndex = pOption->Index;

			SetToolTipIcon(pMugong);
			GAMEIN->GetQuickDialog()->RefreshIcon(pMugong->GetQuickPosition());

			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1312 ) );
			
		}
		break;
	case MP_MUGONG_OPTION_NACK:
		{
		}
		break;
	case MP_MUGONG_OPTION_CLEAR_ACK:
		{

			MSG_WORD4* pmsg = (MSG_WORD4*)pMsg;

			CMugongBase* pMugong = GetMugongByMugongIdx(pmsg->wData1);
			if(!pMugong) return;

			CItem* pOutItem = NULL;
			ITEMMGR->DeleteItem( pmsg->wData4, &pOutItem);
			
			pMugong->GetMugongBase()->OptionIndex = 0;

			SetToolTipIcon(pMugong);
			GAMEIN->GetQuickDialog()->RefreshIcon(pMugong->GetQuickPosition());

			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1326 ) );
		}
		break;
	case MP_MUGONG_OPTION_CLEAR_NACK:
		{
			MSG_WORD4* pmsg = (MSG_WORD4*)pMsg;

			const ITEMBASE* pitembase = ITEMMGR->GetItemInfoAbsIn( HERO, pmsg->wData4 );
			if( !pitembase )		return;

			CItem* pitem = ITEMMGR->GetItem( pitembase->dwDBIdx );
			if( pitem && pitem->GetItemKind() & eSHOP_ITEM )
				pitem->SetLock( FALSE );
		}
		break;
	}
}
CMugongBase * CMugongManager::NewMugongIcon(MUGONGBASE * mugongBase)
{
	CMugongBase * icon = NULL;

	// magi82 - Titan(070910) 타이탄 무공업데이트
//	if(mugongBase->wIconIdx < SKILLNUM_TO_TITAN)
//	{
		if(!(icon = GetMugong(mugongBase->wIconIdx)))
		{
			cImage skillLowImage;
			cImage skillHighImage;
			GetIconImage(mugongBase->wIconIdx, &skillLowImage, &skillHighImage);

			//////////////////////////////////////////////////////////////////////////
			// YH2DO Kind 정리 필요
			CSkillInfo* pSkillInfo = SKILLMGR->GetSkillInfo(mugongBase->wIconIdx);
			if(pSkillInfo == NULL)
			{
				icon = new CSimBub;
			}
			else if(pSkillInfo->GetSkillKind() == SKILLKIND_JINBUB)
			{
				icon = new CJinBub;
			}
			else
			{
				icon = new CMugongBase;
			}
			//////////////////////////////////////////////////////////////////////////


			icon->Init(0,0, DEFAULT_ICONSIZE, DEFAULT_ICONSIZE, &skillLowImage, &skillHighImage, m_IconIndexCreator.GenerateIndex());
			icon->SetData(mugongBase->wIconIdx);
			icon->SetMugongBase(mugongBase);

			icon->m_pSkillInfo = pSkillInfo;

//			SetToolTipIcon(icon);	// 모든 무공을 추가 시킨뒤 일괄적으로 툴팁 적용하려고 주석처리


			m_MugongHash.Add(icon, mugongBase->wIconIdx);
			WINDOWMGR->AddWindow(icon);
		}
/*
	}
	else
	{
		if(!(icon = GetMugong(mugongBase->wIconIdx)))
		{
			//////////////////////////////////////////////////////////////////////////
			// YH2DO Kind 정리 필요
			CSkillInfo* pSkillInfo = SKILLMGR->GetSkillInfo(mugongBase->wIconIdx);
			if(pSkillInfo == NULL)
			{
				icon = new CSimBub;

			}
			else if(pSkillInfo->GetSkillKind() == SKILLKIND_JINBUB)
			{
				icon = new CJinBub;
			}
			else
			{
				icon = new CMugongBase;
			}
			//////////////////////////////////////////////////////////////////////////

			icon->SetData(mugongBase->wIconIdx);
			icon->SetMugongBase(mugongBase);
			icon->m_pSkillInfo = pSkillInfo;

			m_MugongHash.Add(icon, mugongBase->wIconIdx);
		}
	}
*/
	MUGONGMGR->RefreshMugong();
	return icon;
}
CMugongBase * CMugongManager::GetMugong(DWORD dwIdx)
{
	CMugongBase * pMugong;
	pMugong = m_MugongHash.GetData(dwIdx);
	return pMugong;
}
CMugongBase * CMugongManager::GetMugongByMugongIdx(WORD MugongIdx)
{
	CMugongBase * pMugong;
	m_MugongHash.SetPositionHead();
	while(pMugong = m_MugongHash.GetData())
	{
		if(pMugong->GetItemIdx() == MugongIdx)
			return pMugong;
	}
	return NULL;
}

void CMugongManager::SetToolTipIcon(CMugongBase *pMugong)
{
	CSkillInfo* pCommSkill = pMugong->m_pSkillInfo;
	if(!pCommSkill)	return;

	cImage imgHeader;
	SCRIPTMGR->GetImage(25, &imgHeader, PFT_JACKPATH );
	pMugong->SetHeaderImage(&imgHeader);
	cImage imgToolTip;
	SCRIPTMGR->GetImage(26, &imgToolTip, PFT_JACKPATH );
	pMugong->SetToolModel(1);
	//pMugong->SetToolTip( "", RGBA_MAKE(255, 255, 255, 255), &imgToolTip, TTCLR_ITEM_CANEQUIP );
	
	
	char line[64];
	char line2[64];
	char line3[64];
	ZeroMemory(&line, sizeof(line));
	ZeroMemory(&line2, sizeof(line2));
	cImage skillLowImage;
	cImage skillHighImage;
	GetIconImage(pMugong->GetMugongBase()->wIconIdx, &skillLowImage, &skillHighImage);

	BYTE mLevel = pMugong->GetSung();

	pMugong->SetAutoLineToolTip("",RGBA_MAKE(255, 255, 255, 255),&imgToolTip,46,&skillLowImage,&skillHighImage);
	wsprintf(line, "[%s]", pCommSkill->GetSkillName() );
	if(pMugong->GetOption() == eSkillOption_None)
		pMugong->AddToolTipLine( line, TTTC_MUGONGNAME,NULL,6 );
	else
		pMugong->AddToolTipLine( line, TTTC_TRANSMUGONGNAME,NULL,6 );
	pMugong->AddToolTipLine( JACKJACK );
	switch( pCommSkill->GetAttrib())
	{
	case ATTR_FIRE:	 sprintf(line2,"%s",CHATMGR->GetChatMsg(473)); break;
	case ATTR_WATER: sprintf(line2,"%s",CHATMGR->GetChatMsg(474)); break;
	case ATTR_TREE:	 sprintf(line2,"%s",CHATMGR->GetChatMsg(475)); break;
	case ATTR_IRON:	 sprintf(line2,"%s",CHATMGR->GetChatMsg(476)); break;
	case ATTR_EARTH: sprintf(line2,"%s",CHATMGR->GetChatMsg(477)); break;
	case ATTR_NONE:	 sprintf(line2,"%s",CHATMGR->GetChatMsg(478)); break;
	case ATTR_NOATTR:	 sprintf(line2,"%s",CHATMGR->GetChatMsg(2459)); break;
	case ATTR_ALLATTR:sprintf(line2,"%s",CHATMGR->GetChatMsg(479)); break;
	}
	wsprintf(line, "          %s%s",CHATMGR->GetChatMsg(460), line2);
	sprintf(line2,CHATMGR->GetChatMsg(472),pCommSkill->GetNeedNaeRyuk(mLevel));
	wsprintf( line3, "                               %s",line2);

	if( HERO->GetCharacterTotalInfo()->Level < pCommSkill->GetSkillInfo()->RestrictLevel )
		pMugong->AddToolTipLine2( line, line3, TTTC_LIMIT,TTTC_BUYPRICE,NULL,0 );
	else
		pMugong->AddToolTipLine2( line, line3, TTTC_FREELIMIT,TTTC_BUYPRICE,NULL,0 );


	pMugong->AddToolTipLine( "" );


	int NeedExp = pCommSkill->GetNeedExp( mLevel );
	float DelayRate=pCommSkill->GetDelayTime();
	if( NeedExp != 0)
	{
		if( mLevel <= 12 && mLevel != 12 )
		{
			float percent = pMugong->GetExpPoint() * 100.0f / NeedExp;
			if( pMugong->GetExpPoint() != NeedExp-1 )
			if( percent > 99.99f )
			{
				percent = 99.99f;
			}
			sprintf( line2, CHATMGR->GetChatMsg(2480), mLevel, percent );
		}
		else
		{
			if(SKILLMGR->IsDeadlyMugong(pMugong->GetItemIdx()) == FALSE)
				sprintf( line2, CHATMGR->GetChatMsg(2481), mLevel );
			else
				sprintf( line2, CHATMGR->GetChatMsg(2482), mLevel );
		}
		float delay = DelayRate;
			sprintf(line3,CHATMGR->GetChatMsg(2462),delay/1000);
			wsprintf( line, "                          %s",line3);
			pMugong->AddToolTipLine2( line2, line, RGB_HALF(255,255,200),TTTC_RAREITEM,NULL,0,1 );
			//pMugong->AddToolTipLine( "" );
	}
	else
	{
		//pMugong->AddToolTipLine( "" );
		pMugong->AddToolTipLine( "" );
	}



/*-------------------------------------------------------------------
Attack Range:
---------------------------------------------------------------------*/
	pMugong->AddToolTipLine( "" );
	int AttRange = pCommSkill->GetSkillRange();
	if( AttRange != 0 )
	{
		sprintf(line,CHATMGR->GetChatMsg(2463),AttRange);
		pMugong->AddToolTipLine( line, TTTC_BUYPRICE );
	}
	/*else if(AttRange==0)
	{
		sprintf(line,CHATMGR->GetChatMsg(2464),CHATMGR->GetChatMsg(2465));
		pMugong->AddToolTipLine( line, TTTC_BUYPRICE );
	}*/



/*-------------------------------------------------------------------
Target:
---------------------------------------------------------------------*/
	bool TargetKind=pCommSkill->IsTargetAreaFitToHero();
	if(TargetKind/*==TRUE*/)
	{
		char range5[64];
		wsprintf(range5, CHATMGR->GetChatMsg(2668),pCommSkill->GetSkillInfo()->TargetRange);
		sprintf(line,CHATMGR->GetChatMsg(2464),/*CHATMGR->GetChatMsg(2668)*/range5);//All targets within the radius
		pMugong->AddToolTipLine2( line,"", TTTC_BUYPRICE );
	}
	else if(AttRange==0)
	{
		sprintf(line,CHATMGR->GetChatMsg(2464),CHATMGR->GetChatMsg(2465));//Self
		pMugong->AddToolTipLine( line, TTTC_BUYPRICE );
	}
	else if(TargetKind == false)
	{
		sprintf(line,CHATMGR->GetChatMsg(2464),CHATMGR->GetChatMsg(2667));//Single target
		pMugong->AddToolTipLine( line, TTTC_BUYPRICE );
	}
	




/*-------------------------------------------------------------------
Skill rate
---------------------------------------------------------------------*/
	

	float SkillDmg=pCommSkill->GetDamageRate(mLevel);
	float SkillAtt=pCommSkill->GetAttackRate(mLevel);
	float Stun=pCommSkill->GetStunRate(mLevel);
	float Critical=pCommSkill->GetCriticalRate(mLevel);
	WORD LifeRecovery=pCommSkill->GetContinueRecoverLife(mLevel);

	if( SkillDmg||SkillAtt||Stun||Critical||LifeRecovery )
	{
		pMugong->AddToolTipLine( "" );
	}


	if( SkillDmg/*!=0*/ )
	{
		sprintf(line,CHATMGR->GetChatMsg(2469),(int)SkillDmg);
		pMugong->AddToolTipLine( line, RGB_HALF(255,255,200) );
	}	
	
	if( SkillAtt/*!=0*/ )
	{
		sprintf(line,CHATMGR->GetChatMsg(2469),(int)SkillAtt);
		pMugong->AddToolTipLine( line, RGB_HALF(255,255,200) );
	}	
	
	if( Stun/*!=0*/ )
	{
		sprintf(line,CHATMGR->GetChatMsg(2468),Stun*100);
		pMugong->AddToolTipLine( line, RGB_HALF(255,255,200) );
	}
	
	if( Critical /*!= 0*/ )
	{
		sprintf(line,CHATMGR->GetChatMsg(2467),Critical*100);
		pMugong->AddToolTipLine( line, RGB_HALF(255,255,200) );
	}	
	
	if( LifeRecovery /*!= 0*/ )
	{
		sprintf(line,CHATMGR->GetChatMsg(1409),LifeRecovery);
		pMugong->AddToolTipLine( line, RGB_HALF(255,255,200) );
	}
	


/*-------------------------------------------------------------------
Skill Option / Meta Stone
---------------------------------------------------------------------*/
	SKILLOPTION* pOption = SKILLMGR->GetSkillOption(pMugong->GetOption());
	if(pOption)
	{
		pMugong->AddToolTipLine( "" );
		if(pOption->Range > 0)
		{
			char ToolTipText[32];
			wsprintf(ToolTipText, CHATMGR->GetChatMsg(1316), pOption->Range);
			pMugong->AddToolTipLine( ToolTipText, TTTC_TRANSMUGONGOPTION );
		}
		if(pOption->Range < 0)
		{
			char ToolTipText[32];
			wsprintf(ToolTipText, CHATMGR->GetChatMsg(1320), -pOption->Range);
			pMugong->AddToolTipLine( ToolTipText, TTTC_TRANSMUGONGOPTION );
		}
		if(pOption->ReduceNaeRyuk > 0)
		{
			char ToolTipText[32];
			wsprintf(ToolTipText, CHATMGR->GetChatMsg(1317), (int)(pOption->ReduceNaeRyuk * 100 + 0.5));
			pMugong->AddToolTipLine( ToolTipText, TTTC_TRANSMUGONGOPTION );
		}
		if(pOption->ReduceNaeRyuk < 0)
		{
			char ToolTipText[32];
			wsprintf(ToolTipText, CHATMGR->GetChatMsg(1321), (int)(-pOption->ReduceNaeRyuk * 100 + 0.5));
			pMugong->AddToolTipLine( ToolTipText, TTTC_TRANSMUGONGOPTION );
		}
		if(pOption->PhyAtk > 0)
		{
			char ToolTipText[32];
			wsprintf(ToolTipText, CHATMGR->GetChatMsg(1318), (int)(pOption->PhyAtk * 100 + 0.5));
			pMugong->AddToolTipLine( ToolTipText, TTTC_TRANSMUGONGOPTION );
		}
		if(pOption->PhyAtk < 0)
		{
			char ToolTipText[32];
			wsprintf(ToolTipText, CHATMGR->GetChatMsg(1322), (int)(-pOption->PhyAtk * 100 + 0.5));
			pMugong->AddToolTipLine( ToolTipText, TTTC_TRANSMUGONGOPTION );
		}
		if(pOption->BaseAtk)
		{
			char ToolTipText[32];			
			wsprintf(ToolTipText, CHATMGR->GetChatMsg(1740), (int)(pOption->BaseAtk * 100));
			pMugong->AddToolTipLine( ToolTipText, TTTC_TRANSMUGONGOPTION );
		}
		if(pOption->AttAtk > 0)
		{
			char ToolTipText[32];
			wsprintf(ToolTipText, CHATMGR->GetChatMsg(1319), (int)(pOption->AttAtk * 100 + 0.5));
			pMugong->AddToolTipLine( ToolTipText, TTTC_TRANSMUGONGOPTION );
		}
		if(pOption->AttAtk < 0)
		{
			char ToolTipText[32];
			wsprintf(ToolTipText, CHATMGR->GetChatMsg(1323), (int)(-pOption->AttAtk * 100 + 0.5));
			pMugong->AddToolTipLine( ToolTipText, TTTC_TRANSMUGONGOPTION );
		}
		if(pOption->Life > 0)	
		{
			char ToolTipText[32];
			wsprintf(ToolTipText, CHATMGR->GetChatMsg(1340), pOption->Life);
			pMugong->AddToolTipLine( ToolTipText, TTTC_TRANSMUGONGOPTION );
		}
		if(pOption->Life < 0)	
		{
			char ToolTipText[32];
			wsprintf(ToolTipText, CHATMGR->GetChatMsg(1341), -pOption->Life);
			pMugong->AddToolTipLine( ToolTipText, TTTC_TRANSMUGONGOPTION );
		}
		if(pOption->NaeRyuk > 0)
		{
			char ToolTipText[32];
			wsprintf(ToolTipText, CHATMGR->GetChatMsg(1342), pOption->NaeRyuk);
			pMugong->AddToolTipLine( ToolTipText, TTTC_TRANSMUGONGOPTION );
		}
		if(pOption->NaeRyuk < 0)
		{
			char ToolTipText[32];
			wsprintf(ToolTipText, CHATMGR->GetChatMsg(1343), -pOption->NaeRyuk);
			pMugong->AddToolTipLine( ToolTipText, TTTC_TRANSMUGONGOPTION );
		}
		if(pOption->Shield > 0)
		{
			char ToolTipText[32];
			wsprintf(ToolTipText, CHATMGR->GetChatMsg(1344), pOption->Shield);
			pMugong->AddToolTipLine( ToolTipText, TTTC_TRANSMUGONGOPTION );
		}
		if(pOption->Shield < 0)
		{
			char ToolTipText[32];
			wsprintf(ToolTipText, CHATMGR->GetChatMsg(1345), -pOption->Shield);
			pMugong->AddToolTipLine( ToolTipText, TTTC_TRANSMUGONGOPTION );
		}
		if(pOption->PhyDef > 0)
		{
			char ToolTipText[32];
			wsprintf(ToolTipText, CHATMGR->GetChatMsg(1348), (int)(pOption->PhyDef * 100 + 0.5));
			pMugong->AddToolTipLine( ToolTipText, TTTC_TRANSMUGONGOPTION );
		}
		if(pOption->PhyDef < 0)
		{
			char ToolTipText[32];
			wsprintf(ToolTipText, CHATMGR->GetChatMsg(1349), (int)(-pOption->PhyDef * 100 + 0.5));
			pMugong->AddToolTipLine( ToolTipText, TTTC_TRANSMUGONGOPTION );
		}
		if(pOption->AttDef > 0)
		{
			char ToolTipText[32];
			wsprintf(ToolTipText, CHATMGR->GetChatMsg(1324), (int)(pOption->AttDef * 100 + 0.5));
			pMugong->AddToolTipLine( ToolTipText, TTTC_TRANSMUGONGOPTION );
		}
		if(pOption->AttDef < 0)
		{
			char ToolTipText[32];
			wsprintf(ToolTipText, CHATMGR->GetChatMsg(1325), (int)(-pOption->AttDef * 100 + 0.5));
			pMugong->AddToolTipLine( ToolTipText, TTTC_TRANSMUGONGOPTION );
		}
		if(pOption->Duration > 0)
		{
			char ToolTipText[32];
			wsprintf(ToolTipText, CHATMGR->GetChatMsg(1346), pOption->Duration);
			pMugong->AddToolTipLine( ToolTipText, TTTC_TRANSMUGONGOPTION );
		}
		if(pOption->Duration < 0)
		{
			char ToolTipText[32];
			wsprintf(ToolTipText, CHATMGR->GetChatMsg(1347), -pOption->Duration);
			pMugong->AddToolTipLine( ToolTipText, TTTC_TRANSMUGONGOPTION );
		}
	}


/*-------------------------------------------------------------------
Grey line next level information
---------------------------------------------------------------------*/
	if( mLevel+1 > 0 && mLevel != 12 )
	{
		pMugong->AddToolTipLine( "" );
		wsprintf( line, CHATMGR->GetChatMsg(2460) );
		pMugong->AddToolTipLine( line, ICONTEXT_DISABLE_BYZEROSUNG );
		wsprintf( line, CHATMGR->GetChatMsg(472), pCommSkill->GetNeedNaeRyuk(mLevel+1) );
		pMugong->AddToolTipLine( line, ICONTEXT_DISABLE_BYZEROSUNG );
		float Stun=pCommSkill->GetStunRate(mLevel+1);
		if(Stun!=0)
		{
			sprintf(line,CHATMGR->GetChatMsg(2468),Stun*100);
			pMugong->AddToolTipLine( line, ICONTEXT_DISABLE_BYZEROSUNG );
		}
		float Critical=pCommSkill->GetCriticalRate(mLevel+1);
		if( Critical != 0)
		{
			sprintf(line,CHATMGR->GetChatMsg(2467),Critical*100);
			pMugong->AddToolTipLine( line, ICONTEXT_DISABLE_BYZEROSUNG );
		}
	}
	if(pMugong->GetLastSecond())
	{
		char ToolTipText[32];
		wsprintf(ToolTipText, CHATMGR->GetChatMsg(582), pMugong->GetLastSecond() );
		pMugong->AddToolTipLine( ToolTipText, TTTC_QUESTITEM );
	}
	if(pCommSkill->GetSpecialState())
	{
		pMugong->AddToolTipLine( CHATMGR->GetChatMsg(1359), TTTC_MUGONGNEEDMANA );
		SPECIAL_STATE_INFO* pInfo = SKILLMGR->GetSpecialStateInfo(pCommSkill->GetSpecialState());
		if(pInfo)
		{
			char ToolTipText[32];
			wsprintf(ToolTipText, CHATMGR->GetChatMsg(1360), pInfo->NaeRyuk);
			pMugong->AddToolTipLine( ToolTipText, TTTC_MUGONGNEEDMANA );
		}
	}
	ITEMMGR->AddItemDescriptionToolTip( pMugong, pCommSkill->GetSkillInfo()->SkillTooltipIdx );
	pMugong->AddToolTipLine( "" );
	//pMugong->AddToolTipLine( "" );
}

void CMugongManager::GetIconImage(WORD ItemIdx, cImage * pLowImage , cImage * pHighImage)
{
	
//	char buff[128];
	CSkillInfo* pSkillInfo = SKILLMGR->GetSkillInfo(ItemIdx);
	if(pSkillInfo == NULL)
	{
//		ASSERT(0);
		return;
	}

	if( pSkillInfo->GetLowImage() != -1 )
	SCRIPTMGR->GetImage( pSkillInfo->GetLowImage(), pLowImage, PFT_MUGONGPATH );
	if( pSkillInfo->GetHighImage() != -1 )
	SCRIPTMGR->GetImage( pSkillInfo->GetHighImage(), pHighImage, PFT_MUGONGPATH );
}
void CMugongManager::DeleteMugongIcon(CMugongBase * mugong)
{	
	if(mugong->GetQuickPosition())
	{
		QUICKMGR->RemQuickItem(mugong->GetQuickPosition());
	}

	// magi82 - Titan(070320)
	m_MugongHash.Remove(mugong->GetMugongBase()->wIconIdx);
	m_IconIndexCreator.ReleaseIndex(mugong->GetID());
	WINDOWMGR->AddListDestroyWindow( mugong );
}



void CMugongManager::RefreshMugong()
{
	CMugongBase * pMugong = NULL;
	m_MugongHash.SetPositionHead();
	eMUGONG_CANNOT_REASON reason;
	while(pMugong = m_MugongHash.GetData())
	{
		reason = CanUse(pMugong);
		ySWITCH(reason)
			yCASE(eMCUR_Usable)
				pMugong->SetImageRGB( ICONCLR_USABLE );
				pMugong->SetAlpha( ICONALPHA_USABLE );

				pMugong->SetFontColor( ICONTEXT_USABLE );
				pMugong->SetToolTipImageRGB( TTCLR_MUGONG_CANUSE );
			yCASE(eMCUR_SungZero)

				pMugong->SetImageRGB( ICONCLR_DISABLE_BYZEROSUNG );
				pMugong->SetAlpha( ICONALPHA_DISABLE_BYZEROSUNG );
				pMugong->SetFontColor( ICONTEXT_DISABLE_BYZEROSUNG );
				pMugong->SetToolTipImageRGB( TTCLR_MUGONG_CANNOTUSE );
			yCASE(eMCUR_Weapon)
				pMugong->SetImageRGB( ICONCLR_DISABLE );
				pMugong->SetAlpha( ICONALPHA_DISABLE );
				pMugong->SetFontColor( ICONTEXT_DISABLE );
				pMugong->SetToolTipImageRGB( TTCLR_MUGONG_CANNOTUSE );
			yCASE(eMCUR_Limit)
				pMugong->SetImageRGB( ICONCLR_DISABLE );
				pMugong->SetAlpha( ICONALPHA_DISABLE );
				pMugong->SetFontColor( ICONTEXT_DISABLE );
				pMugong->SetToolTipImageRGB( TTCLR_MUGONG_CANNOTUSE );
			yCASE(eMCUR_Disable)
				pMugong->SetActive(FALSE);
			// magi82 - Titan(070911) 타이탄 무공업데이트

			// 타이탄 탑승상태가 아니면 무공을 못쓰게 해야함.
			yCASE(eMCUR_Titan)
				pMugong->SetImageRGB( ICONCLR_DISABLE );
				pMugong->SetAlpha( ICONALPHA_DISABLE );
				pMugong->SetFontColor( ICONTEXT_DISABLE );
				pMugong->SetToolTipImageRGB( TTCLR_MUGONG_CANNOTUSE );
		yENDSWITCH
		
		SetToolTipIcon( pMugong );
	}
}
CMugongManager::eMUGONG_CANNOT_REASON CMugongManager::CanUse(CMugongBase * pMugong)
{
	if( pMugong == NULL ) return eMCUR_Disable;
	
	//레벨체크등 안하나?
	if( HERO && pMugong->m_pSkillInfo )
	if( pMugong->m_pSkillInfo->GetSkillInfo() )
	if( pMugong->m_pSkillInfo->GetSkillInfo()->RestrictLevel > HERO->GetLevel() )
		return eMCUR_Limit;
		
	if( pMugong->GetSung() == 0 )
		return eMCUR_SungZero;

	if( pMugong->m_pSkillInfo == NULL )
		return eMCUR_Disable;

	// magi82 - Titan(070911) 타이탄 무공업데이트
	// 타이탄 탑승상태가 아니면 무공을 못쓰게 해야함.
	if( HERO->InTitan() == FALSE && pMugong->m_pSkillInfo->GetSkillKind() == SKILLKIND_TITAN )
		return eMCUR_Titan;
	if( HERO->InTitan() == TRUE && (pMugong->m_pSkillInfo->GetSkillKind() == SKILLKIND_OUTERMUGONG || pMugong->m_pSkillInfo->GetSkillKind() == SKILLKIND_INNERMUGONG) )
		return eMCUR_Titan;

	if( pMugong->m_pSkillInfo->GetSkillKind() == SKILLKIND_OUTERMUGONG &&
		pMugong->m_pSkillInfo->GetWeaponType() != HERO->GetWeaponEquipType() )

		return eMCUR_Weapon;

	// magi82 - Titan(071011) 타이탄 무공업데이트
	if( pMugong->m_pSkillInfo->GetSkillKind() == SKILLKIND_TITAN)
	{
		WORD wSkillWeaponType = pMugong->m_pSkillInfo->GetWeaponType();


		if( wSkillWeaponType != WP_WITHOUT )
		{
			if( wSkillWeaponType != HERO->GetTitanWeaponEquipType() )
				return eMCUR_Titan;

			if( HERO->GetWeaponEquipType() != HERO->GetTitanWeaponEquipType() )
				return eMCUR_Titan;
		}
	}

	// 화경, 극마전직후에 상성관계의 무공 못쓰게
	if( HERO->GetStage() & eStage_Hwa )
	{
		if( SKILLMGR->IsGeukMaSkill( pMugong->m_pSkillInfo->GetSkillIndex() ) )
			return eMCUR_Limit;
	}
	else if( HERO->GetStage() & eStage_Geuk )
	{
		if( SKILLMGR->IsHwaKungSkill( pMugong->m_pSkillInfo->GetSkillIndex() ) )
			return eMCUR_Limit;
	}

	return eMCUR_Usable;
}


BOOL CMugongManager::CheckMugongTree(WORD MugongIdx)
{
	CSkillInfo* pInfo = SKILLMGR->GetSkillInfo(MugongIdx);
	if(pInfo == NULL)
		return FALSE;
	
	if(pInfo->GetBeforeSkill())
	{
		CMugongBase* pMugong = GetMugongByMugongIdx(pInfo->GetBeforeSkill());
		
		if(pMugong == NULL)
			return FALSE;
		
		if(pMugong->GetSung() < 1)
			return FALSE;
	}

	// 절초 검색 hs
/*	SKILL_CHANGE_INFO * pChangeInfo = SKILLMGR->GetSkillChangeInfo(MugongIdx);

	if(pChangeInfo)
	{
		if(GetMugongByMugongIdx(pChangeInfo->wTargetMugongIdx))
			return FALSE;
	}
*/
	return TRUE;
}

int CMugongManager::GetMugongNum()
{
	int count = 0;
	m_MugongHash.SetPositionHead();
	CMugongBase* pMugongBase;
	while(pMugongBase = m_MugongHash.GetData())
	{
		if(pMugongBase->GetSung() > 0)
			++count;
	}
	return count;
}

//////// 2007. 7. 2. CBH - 전문스킬 검색 ///////////////
BOOL CMugongManager::SearchJobSkill()
{
	m_MugongHash.SetPositionHead();
	CMugongBase* pMugongBase = NULL;
	while(pMugongBase = m_MugongHash.GetData())
	{		
		CSkillInfo* pSkillInfo = pMugongBase->m_pSkillInfo;
		if(pSkillInfo == NULL)
		{
			return FALSE;
		}

		WORD wSkillKind = pSkillInfo->GetSkillKind();
		if( SKILLMGR->CheckSkillKind(wSkillKind) == TRUE )
		{
			return FALSE;
		}
	}

	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////




