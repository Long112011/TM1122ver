







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
				
				// magi82 - Titan(070611) 鸥捞藕 公傍函券 林籍贸府
				//m_pMugongDlg->AddMugong(&msg->Titanmugong[i]);
				if(msg->mugong[i].QuickPosition != 0)
				{
					// 06. 01 捞康霖 - 窜绵芒 函版
					// 檬扁 汲沥 窃荐 AddQuickItemReal -> SetQuickItemReal
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

			// magi82 - Titan(070611) 鸥捞藕 公傍函券 林籍贸府
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
			// 公傍氓 昏力
//			ITEMMGR->DeleteItem( msg->MugongBase.Position );
			// 公傍 积己, 殿废
			m_pMugongDlg->AddMugong(&msg->MugongBase);
			
			//msg免仿
			CMugongBase * icon = GetMugong(msg->MugongBase.wIconIdx);
			if( !icon ) return;
			CSkillInfo* pCommSkill = icon->m_pSkillInfo;
			if(!pCommSkill)	return;
			// 例檬搁 楷免
			if(SKILLMGR->IsDeadlyMugong(pCommSkill->GetSkillIndex()))
			{
				// magi82(4) - Titan(071023) 鸥捞藕老锭 捞棋飘 贸府(例檬)
				if(HERO->InTitan() == TRUE)
                    EFFECTMGR->StartHeroEffectProcess(eEffect_Titan_ChangeMugong);
				else
					EFFECTMGR->StartHeroEffectProcess(eEffect_Mugong_ChangeMugong);

				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(791), pCommSkill->GetSkillName() );
			}
			else if(msg->MugongBase.Sung == 0) // 公傍裙垫
			{
				// magi82(4) - Titan(071023) 鸥捞藕老锭 捞棋飘 贸府(公傍裙垫)
				//if(HERO->InTitan() == TRUE)
				//	EFFECTMGR->StartHeroEffectProcess(eEffect_Titan_GetMugong);
				//else
				//	EFFECTMGR->StartHeroEffectProcess(eEffect_Mugong_GetMugong);


				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(189), pCommSkill->GetSkillName() );
				GAMEIN->GetMugongDialog()->SetDisable(FALSE);				
			}
			else if(SKILLMGR->CheckSkillKind(pCommSkill->GetSkillKind()) == TRUE) //傈巩胶懦 嚼垫
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
			// target : MUGONG DEL -> GROUND ADD(HERO困摹)
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
			// 俊矾 皋矫瘤 
			LOG(EC_MUGONG_ADDNITEMDELETE_FAILED);
		}
		break;
	case MP_MUGONG_DELETEINVENTORYADD_NACK:
		{
			// invalid target
			// 俊矾 皋矫瘤 
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
				else if(pmsg->wData3 == eLog_MugongLevelDownByBadFame)	//厩疙摹 臭酒 颇鲍
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 677 ),pInfo->GetSkillName() );
				else if(pmsg->wData3 == eLog_MugongLevelDownByWanted)	//泅惑裹捞骨肺 颇鲍
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 687 ),pInfo->GetSkillName() );
			}

			/*
			CMugongBase * mugongOut = NULL;
			mugongOut = (CMugongBase *)m_pMugongDlg->GetIconForIdx(pmsg->wData1);
			if(mugongOut->GetDBIdx() > 0) mugongOut->SetSung(pmsg->wData2);
			// 牢磐其捞胶
			// 公傍 芒狼 己钎矫 
			// 公傍 芒狼 版氰摹 max 霸捞瘤 setting

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

			// magi82(4) - Titan(071023) 鸥捞藕老锭 捞棋飘 贸府(公傍饭骇诀)
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

	// magi82 - Titan(070910) 鸥捞藕 公傍诀单捞飘
//	if(mugongBase->wIconIdx < SKILLNUM_TO_TITAN)
//	{
		if(!(icon = GetMugong(mugongBase->wIconIdx)))
		{
			cImage skillLowImage;
			cImage skillHighImage;
			GetIconImage(mugongBase->wIconIdx, &skillLowImage, &skillHighImage);

			//////////////////////////////////////////////////////////////////////////
			// YH2DO Kind 沥府 鞘夸
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

//			SetToolTipIcon(icon);	// 葛电 公傍阑 眠啊 矫挪第 老褒利栏肺 砒屏 利侩窍妨绊 林籍贸府


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
			// YH2DO Kind 沥府 鞘夸
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

	switch (pCommSkill->GetWeaponType())
	{
	case WP_GUM: //剑
	{
		sprintf(line, "%s", CHATMGR->GetChatMsg(2776 ));
		pMugong->AddToolTipLine(line, TTTC_FREELIMIT);
	}
	break;
	case WP_GWUN: //拳
	{
		sprintf(line, "%s", CHATMGR->GetChatMsg(2777 ));
		pMugong->AddToolTipLine(line, TTTC_FREELIMIT);
	}
	break;
	case WP_DO: //刀
	{
		sprintf(line, "%s", CHATMGR->GetChatMsg(2778));
		pMugong->AddToolTipLine(line, TTTC_FREELIMIT);
	}
	break;
	case WP_CHANG: //枪
	{
		sprintf(line, "%s", CHATMGR->GetChatMsg(2779));
		pMugong->AddToolTipLine(line, TTTC_FREELIMIT);
	}
	break;
	case WP_GUNG: //弓
	{
		sprintf(line, "%s", CHATMGR->GetChatMsg(2780));
		pMugong->AddToolTipLine(line, TTTC_FREELIMIT);
	}
	break;
	case WP_AMGI: //暗
	{
		sprintf(line, "%s", CHATMGR->GetChatMsg(2781));
		pMugong->AddToolTipLine(line, TTTC_FREELIMIT);
	}
	break;
	case WP_AXE: //斧
	{
		sprintf(line, "%s", CHATMGR->GetChatMsg(2832));
		pMugong->AddToolTipLine(line, TTTC_FREELIMIT);
	}
	break;
	case WP_DAGGER: //刺客
	{
		sprintf(line, "%s", CHATMGR->GetChatMsg(2833));
		pMugong->AddToolTipLine(line, TTTC_FREELIMIT);
	}
	break;
	}
	switch (pCommSkill->GetAttrib())
	{
	case ATTR_FIRE: //火
	{
		sprintf(line, "%s", CHATMGR->GetChatMsg(2782));
		pMugong->AddToolTipLine(line, TTTC_FREELIMIT);
	}
	break;
	case ATTR_WATER: //水
	{
		sprintf(line, "%s", CHATMGR->GetChatMsg(2783));
		pMugong->AddToolTipLine(line, TTTC_FREELIMIT);
	}
	break;
	case ATTR_TREE: //木
	{
		sprintf(line, "%s", CHATMGR->GetChatMsg(2784));
		pMugong->AddToolTipLine(line, TTTC_FREELIMIT);
	}
	break;
	case ATTR_IRON: //金
	{
		sprintf(line, "%s", CHATMGR->GetChatMsg(2785));
		pMugong->AddToolTipLine(line, TTTC_FREELIMIT);
	}
	break;
	case ATTR_EARTH: //土
	{
		sprintf(line, "%s", CHATMGR->GetChatMsg(2786));
		pMugong->AddToolTipLine(line, TTTC_FREELIMIT);
	}
	break;
	case ATTR_NONE://无属性
	{
		sprintf(line, "%s", CHATMGR->GetChatMsg(2787));
		pMugong->AddToolTipLine(line, TTTC_FREELIMIT);
	}
	break;
	case ATTR_ALLATTR://全属性
	{
		sprintf(line, "%s", CHATMGR->GetChatMsg(2788));
		pMugong->AddToolTipLine(line, TTTC_FREELIMIT);
	}
	break;
	}
	switch (pCommSkill->GetSkillKind())
	{
	case SKILLKIND_JINBUB: //阵法
	{
		sprintf(line, "%s", CHATMGR->GetChatMsg(2789));
		pMugong->AddToolTipLine(line, TTTC_FREELIMIT);
	}
	break;
	case SKILLKIND_SIMBUB: //心法
	{
		sprintf(line, "%s", CHATMGR->GetChatMsg(2790));
		pMugong->AddToolTipLine(line, TTTC_FREELIMIT);
	}
	break;
	}
	wsprintf(line, "          %s", line2);
	sprintf(line2,CHATMGR->GetChatMsg(472),pCommSkill->GetNeedNaeRyuk(mLevel));
	wsprintf( line3, "         %s",line2);

	if( HERO->GetCharacterTotalInfo()->Level < pCommSkill->GetSkillInfo()->RestrictLevel )
		pMugong->AddToolTipLine2( line, line3, TTTC_LIMIT,TTTC_BUYPRICE,NULL,0 );
	else
		pMugong->AddToolTipLine2( line, line3, TTTC_FREELIMIT,TTTC_BUYPRICE,NULL,0 );


	pMugong->AddToolTipLine( "" );


	int NeedExp = pCommSkill->GetNeedExp(mLevel);
	float DelayRate = pCommSkill->GetDelayTime();
	if (NeedExp != 0)
	{
		if (mLevel <= 12 && mLevel != 12)
		{
			float percent = pMugong->GetExpPoint() * 100.0f / NeedExp;
			if (pMugong->GetExpPoint() != NeedExp - 1)
				if (percent > 99.99f)
				{
					percent = 99.99f;
				}
			sprintf(line2, CHATMGR->GetChatMsg(2480), mLevel, percent);
		}
		else
		{
			if (SKILLMGR->IsDeadlyMugong(pMugong->GetItemIdx()) == FALSE)
				sprintf(line2, CHATMGR->GetChatMsg(2481), mLevel);
			else
				sprintf(line2, CHATMGR->GetChatMsg(2482), mLevel);
		}
		float delay = DelayRate;
		sprintf(line3, CHATMGR->GetChatMsg(2462), delay / 1000);
		wsprintf(line, "                          %s", line3);
		pMugong->AddToolTipLine2(line2, line, RGB_HALF(255, 255, 200), TTTC_RAREITEM, NULL, 0, 1);
		//pMugong->AddToolTipLine( "" );
	}
	else
	{
		//pMugong->AddToolTipLine( "" );
		pMugong->AddToolTipLine("");
	}

	pMugong->AddToolTipLine("");



	switch (pCommSkill->GetWeaponType())
	{
	case WP_GUM: //剑
	{
		if (pCommSkill->GetSkillInfo()->SkillRange)
		{
			sprintf(line, CHATMGR->GetChatMsg(2791), pCommSkill->GetSkillInfo()->SkillRange, pMugong->GetSung() > 0);	//攻击距离
			pMugong->AddToolTipLine(line, TTTC_EXTRAATTR);

			if (pCommSkill->GetSkillInfo()->TargetRange)
			{
				sprintf(line, CHATMGR->GetChatMsg(2792), pCommSkill->GetSkillInfo()->TargetRange, pMugong->GetSung() > 0);	//攻击目标︰半径 内的所有目标 (目标为中心)
				pMugong->AddToolTipLine(line, TTTC_EXTRAATTR);
			}
			else
			{
				sprintf(line, CHATMGR->GetChatMsg(2793), pCommSkill->GetSkillInfo()->TargetRange);	//攻击目标︰单一目标
				pMugong->AddToolTipLine(line, TTTC_EXTRAATTR);
			}
		}
		else
		{
			sprintf(line, CHATMGR->GetChatMsg(2794), pCommSkill->GetSkillInfo()->TargetRange);	//攻击目标︰单一目标
			pMugong->AddToolTipLine(line, TTTC_EXTRAATTR);
		}
	}
	break;
	case WP_GWUN: //拳
	{
		if (pCommSkill->GetSkillInfo()->SkillRange)
		{
			sprintf(line, CHATMGR->GetChatMsg(2792), pCommSkill->GetSkillInfo()->SkillRange, pMugong->GetSung() > 0);	//攻击距离
			pMugong->AddToolTipLine(line, TTTC_EXTRAATTR);

			if (pCommSkill->GetSkillInfo()->TargetRange)
			{
				sprintf(line, CHATMGR->GetChatMsg(2792), pCommSkill->GetSkillInfo()->TargetRange, pMugong->GetSung() > 0);	//攻击目标︰半径 内的所有目标 (目标为中心)
				pMugong->AddToolTipLine(line, TTTC_EXTRAATTR);
			}
			else
			{
				sprintf(line, CHATMGR->GetChatMsg(2793), pCommSkill->GetSkillInfo()->TargetRange);	//攻击目标︰单一目标
				pMugong->AddToolTipLine(line, TTTC_EXTRAATTR);
			}
		}
		else
		{
			sprintf(line, CHATMGR->GetChatMsg(2794), pCommSkill->GetSkillInfo()->TargetRange);	//攻击目标︰单一目标
			pMugong->AddToolTipLine(line, TTTC_EXTRAATTR);
		}
	}
	break;
	case WP_DO: //刀
	{
		if (pCommSkill->GetSkillInfo()->SkillRange)
		{
			sprintf(line, CHATMGR->GetChatMsg(2792), pCommSkill->GetSkillInfo()->SkillRange, pMugong->GetSung() > 0);	//攻击距离
			pMugong->AddToolTipLine(line, TTTC_EXTRAATTR);

			if (pCommSkill->GetSkillInfo()->TargetRange)
			{
				sprintf(line, CHATMGR->GetChatMsg(2792), pCommSkill->GetSkillInfo()->TargetRange, pMugong->GetSung() > 0);	//攻击目标︰半径 内的所有目标 (目标为中心)
				pMugong->AddToolTipLine(line, TTTC_EXTRAATTR);
			}
			else
			{
				sprintf(line, CHATMGR->GetChatMsg(2793), pCommSkill->GetSkillInfo()->TargetRange);	//攻击目标︰单一目标
				pMugong->AddToolTipLine(line, TTTC_EXTRAATTR);
			}
		}
		else
		{
			sprintf(line, CHATMGR->GetChatMsg(2794), pCommSkill->GetSkillInfo()->TargetRange);	//攻击目标︰单一目标
			pMugong->AddToolTipLine(line, TTTC_EXTRAATTR);
		}
	}
	break;
	case WP_CHANG: //枪
	{
		if (pCommSkill->GetSkillInfo()->SkillRange)
		{
			sprintf(line, CHATMGR->GetChatMsg(2792), pCommSkill->GetSkillInfo()->SkillRange, pMugong->GetSung() > 0);	//攻击距离
			pMugong->AddToolTipLine(line, TTTC_EXTRAATTR);

			if (pCommSkill->GetSkillInfo()->TargetRange)
			{
				sprintf(line, CHATMGR->GetChatMsg(2792), pCommSkill->GetSkillInfo()->TargetRange, pMugong->GetSung() > 0);	//攻击目标︰半径 内的所有目标 (目标为中心)
				pMugong->AddToolTipLine(line, TTTC_EXTRAATTR);
			}
			else
			{
				sprintf(line, CHATMGR->GetChatMsg(2793), pCommSkill->GetSkillInfo()->TargetRange);	//攻击目标︰单一目标
				pMugong->AddToolTipLine(line, TTTC_EXTRAATTR);
			}
		}
		else
		{
			sprintf(line, CHATMGR->GetChatMsg(2794), pCommSkill->GetSkillInfo()->TargetRange);	//攻击目标︰单一目标
			pMugong->AddToolTipLine(line, TTTC_EXTRAATTR);
		}
	}
	break;
	case WP_GUNG: //弓
	{
		if (pCommSkill->GetSkillInfo()->SkillRange)
		{
			sprintf(line, CHATMGR->GetChatMsg(2792), pCommSkill->GetSkillInfo()->SkillRange, pMugong->GetSung() > 0);	//攻击距离
			pMugong->AddToolTipLine(line, TTTC_EXTRAATTR);

			if (pCommSkill->GetSkillInfo()->TargetRange)
			{
				sprintf(line, CHATMGR->GetChatMsg(2792), pCommSkill->GetSkillInfo()->TargetRange, pMugong->GetSung() > 0);	//攻击目标︰半径 内的所有目标 (目标为中心)
				pMugong->AddToolTipLine(line, TTTC_EXTRAATTR);
			}
			else
			{
				sprintf(line, CHATMGR->GetChatMsg(2793), pCommSkill->GetSkillInfo()->TargetRange);	//攻击目标︰单一目标
				pMugong->AddToolTipLine(line, TTTC_EXTRAATTR);
			}
		}
		else
		{
			sprintf(line, CHATMGR->GetChatMsg(2794), pCommSkill->GetSkillInfo()->TargetRange);	//攻击目标︰单一目标
			pMugong->AddToolTipLine(line, TTTC_EXTRAATTR);
		}
	}
	break;
	case WP_AMGI: //暗
	{
		if (pCommSkill->GetSkillInfo()->SkillRange)
		{
			sprintf(line, CHATMGR->GetChatMsg(2792), pCommSkill->GetSkillInfo()->SkillRange, pMugong->GetSung() > 0);	//攻击距离
			pMugong->AddToolTipLine(line, TTTC_EXTRAATTR);

			if (pCommSkill->GetSkillInfo()->TargetRange)
			{
				sprintf(line, CHATMGR->GetChatMsg(2792), pCommSkill->GetSkillInfo()->TargetRange, pMugong->GetSung() > 0);	//攻击目标︰半径 内的所有目标 (目标为中心)
				pMugong->AddToolTipLine(line, TTTC_EXTRAATTR);
			}
			else
			{
				sprintf(line, CHATMGR->GetChatMsg(2793), pCommSkill->GetSkillInfo()->TargetRange);	//攻击目标︰单一目标
				pMugong->AddToolTipLine(line, TTTC_EXTRAATTR);
			}
		}
		else
		{
			sprintf(line, CHATMGR->GetChatMsg(2794), pCommSkill->GetSkillInfo()->TargetRange);	//攻击目标︰单一目标
			pMugong->AddToolTipLine(line, TTTC_EXTRAATTR);
		}
	}
	break;
	case WP_AXE: //斧
	{
		if (pCommSkill->GetSkillInfo()->SkillRange)
		{
			sprintf(line, CHATMGR->GetChatMsg(2792), pCommSkill->GetSkillInfo()->SkillRange, pMugong->GetSung() > 0);	//攻击距离
			pMugong->AddToolTipLine(line, TTTC_EXTRAATTR);

			if (pCommSkill->GetSkillInfo()->TargetRange)
			{
				sprintf(line, CHATMGR->GetChatMsg(2792), pCommSkill->GetSkillInfo()->TargetRange, pMugong->GetSung() > 0);	//攻击目标︰半径 内的所有目标 (目标为中心)
				pMugong->AddToolTipLine(line, TTTC_EXTRAATTR);
			}
			else
			{
				sprintf(line, CHATMGR->GetChatMsg(2793), pCommSkill->GetSkillInfo()->TargetRange);	//攻击目标︰单一目标
				pMugong->AddToolTipLine(line, TTTC_EXTRAATTR);
			}
		}
		else
		{
			sprintf(line, CHATMGR->GetChatMsg(2794), pCommSkill->GetSkillInfo()->TargetRange);	//攻击目标︰单一目标
			pMugong->AddToolTipLine(line, TTTC_EXTRAATTR);
		}
	}
	break;
	case WP_DAGGER: //刺客
	{
		if (pCommSkill->GetSkillInfo()->SkillRange)
		{
			sprintf(line, CHATMGR->GetChatMsg(2792), pCommSkill->GetSkillInfo()->SkillRange, pMugong->GetSung() > 0);	//攻击距离
			pMugong->AddToolTipLine(line, TTTC_EXTRAATTR);

			if (pCommSkill->GetSkillInfo()->TargetRange)
			{
				sprintf(line, CHATMGR->GetChatMsg(2792), pCommSkill->GetSkillInfo()->TargetRange, pMugong->GetSung() > 0);	//攻击目标︰半径 内的所有目标 (目标为中心)
				pMugong->AddToolTipLine(line, TTTC_EXTRAATTR);
			}
			else
			{
				sprintf(line, CHATMGR->GetChatMsg(2793), pCommSkill->GetSkillInfo()->TargetRange);	//攻击目标︰单一目标
				pMugong->AddToolTipLine(line, TTTC_EXTRAATTR);
			}
		}
		else
		{
			sprintf(line, CHATMGR->GetChatMsg(2794), pCommSkill->GetSkillInfo()->TargetRange);	//攻击目标︰单一目标
			pMugong->AddToolTipLine(line, TTTC_EXTRAATTR);
		}
	}
	break;
	}
	switch (pCommSkill->GetAttrib())
	{
	case ATTR_FIRE: //火
	{
		if (pCommSkill->GetSkillInfo()->SkillRange)
		{
			sprintf(line, CHATMGR->GetChatMsg(2792), pCommSkill->GetSkillInfo()->SkillRange, pMugong->GetSung() > 0);	//攻击距离
			pMugong->AddToolTipLine(line, TTTC_EXTRAATTR);

			if (pCommSkill->GetSkillInfo()->TargetRange)
			{
				sprintf(line, CHATMGR->GetChatMsg(2792), pCommSkill->GetSkillInfo()->TargetRange, pMugong->GetSung() > 0);	//攻击目标︰半径 内的所有目标 (目标为中心)
				pMugong->AddToolTipLine(line, TTTC_EXTRAATTR);
			}
			else
			{
				sprintf(line, CHATMGR->GetChatMsg(2793), pCommSkill->GetSkillInfo()->TargetRange);	//攻击目标︰单一目标
				pMugong->AddToolTipLine(line, TTTC_EXTRAATTR);
			}
		}
		else
		{
			sprintf(line, CHATMGR->GetChatMsg(2794), pCommSkill->GetSkillInfo()->TargetRange);	//攻击目标︰单一目标
			pMugong->AddToolTipLine(line, TTTC_EXTRAATTR);
		}

	}
	break;
	case ATTR_WATER: //水
	{
		if (pCommSkill->GetSkillInfo()->SkillRange)
		{
			sprintf(line, CHATMGR->GetChatMsg(2792), pCommSkill->GetSkillInfo()->SkillRange, pMugong->GetSung() > 0);	//攻击距离
			pMugong->AddToolTipLine(line, TTTC_EXTRAATTR);

			if (pCommSkill->GetSkillInfo()->TargetRange)
			{
				sprintf(line, CHATMGR->GetChatMsg(2792), pCommSkill->GetSkillInfo()->TargetRange, pMugong->GetSung() > 0);	//攻击目标︰半径 内的所有目标 (目标为中心)
				pMugong->AddToolTipLine(line, TTTC_EXTRAATTR);
			}
			else
			{
				sprintf(line, CHATMGR->GetChatMsg(2793), pCommSkill->GetSkillInfo()->TargetRange);	//攻击目标︰单一目标
				pMugong->AddToolTipLine(line, TTTC_EXTRAATTR);
			}
		}
		else
		{
			sprintf(line, CHATMGR->GetChatMsg(2794), pCommSkill->GetSkillInfo()->TargetRange);	//攻击目标︰单一目标
			pMugong->AddToolTipLine(line, TTTC_EXTRAATTR);
		}

	}
	break;
	case ATTR_TREE: //木
	{
		if (pCommSkill->GetSkillInfo()->SkillRange)
		{
			sprintf(line, CHATMGR->GetChatMsg(2792), pCommSkill->GetSkillInfo()->SkillRange, pMugong->GetSung() > 0);	//攻击距离
			pMugong->AddToolTipLine(line, TTTC_EXTRAATTR);

			if (pCommSkill->GetSkillInfo()->TargetRange)
			{
				sprintf(line, CHATMGR->GetChatMsg(2792), pCommSkill->GetSkillInfo()->TargetRange, pMugong->GetSung() > 0);	//攻击目标︰半径 内的所有目标 (目标为中心)
				pMugong->AddToolTipLine(line, TTTC_EXTRAATTR);
			}
			else
			{
				sprintf(line, CHATMGR->GetChatMsg(2793), pCommSkill->GetSkillInfo()->TargetRange);	//攻击目标︰单一目标
				pMugong->AddToolTipLine(line, TTTC_EXTRAATTR);
			}
		}
		else
		{
			sprintf(line, CHATMGR->GetChatMsg(2794), pCommSkill->GetSkillInfo()->TargetRange);	//攻击目标︰单一目标
			pMugong->AddToolTipLine(line, TTTC_EXTRAATTR);
		}

	}
	break;
	case ATTR_IRON: //金
	{
		if (pCommSkill->GetSkillInfo()->SkillRange)
		{
			sprintf(line, CHATMGR->GetChatMsg(2792), pCommSkill->GetSkillInfo()->SkillRange, pMugong->GetSung() > 0);	//攻击距离
			pMugong->AddToolTipLine(line, TTTC_EXTRAATTR);

			if (pCommSkill->GetSkillInfo()->TargetRange)
			{
				sprintf(line, CHATMGR->GetChatMsg(2792), pCommSkill->GetSkillInfo()->TargetRange, pMugong->GetSung() > 0);	//攻击目标︰半径 内的所有目标 (目标为中心)
				pMugong->AddToolTipLine(line, TTTC_EXTRAATTR);
			}
			else
			{
				sprintf(line, CHATMGR->GetChatMsg(2793), pCommSkill->GetSkillInfo()->TargetRange);	//攻击目标︰单一目标
				pMugong->AddToolTipLine(line, TTTC_EXTRAATTR);
			}
		}
		else
		{
			sprintf(line, CHATMGR->GetChatMsg(2794), pCommSkill->GetSkillInfo()->TargetRange);	//攻击目标︰单一目标
			pMugong->AddToolTipLine(line, TTTC_EXTRAATTR);
		}

	}
	break;
	case ATTR_EARTH: //土
	{
		if (pCommSkill->GetSkillInfo()->SkillRange)
		{
			sprintf(line, CHATMGR->GetChatMsg(2792), pCommSkill->GetSkillInfo()->SkillRange, pMugong->GetSung() > 0);	//攻击距离
			pMugong->AddToolTipLine(line, TTTC_EXTRAATTR);

			if (pCommSkill->GetSkillInfo()->TargetRange)
			{
				sprintf(line, CHATMGR->GetChatMsg(2792), pCommSkill->GetSkillInfo()->TargetRange, pMugong->GetSung() > 0);	//攻击目标︰半径 内的所有目标 (目标为中心)
				pMugong->AddToolTipLine(line, TTTC_EXTRAATTR);
			}
			else
			{
				sprintf(line, CHATMGR->GetChatMsg(2793), pCommSkill->GetSkillInfo()->TargetRange);	//攻击目标︰单一目标
				pMugong->AddToolTipLine(line, TTTC_EXTRAATTR);
			}
		}
		else
		{
			sprintf(line, CHATMGR->GetChatMsg(2794), pCommSkill->GetSkillInfo()->TargetRange);	//攻击目标︰单一目标
			pMugong->AddToolTipLine(line, TTTC_EXTRAATTR);
		}

	}
	break;
	case ATTR_NONE://无属性
	{
		if (pCommSkill->GetSkillInfo()->SkillRange)
		{
			sprintf(line, CHATMGR->GetChatMsg(2792), pCommSkill->GetSkillInfo()->SkillRange, pMugong->GetSung() > 0);	//攻击距离
			pMugong->AddToolTipLine(line, TTTC_EXTRAATTR);

			if (pCommSkill->GetSkillInfo()->TargetRange)
			{
				sprintf(line, CHATMGR->GetChatMsg(2792), pCommSkill->GetSkillInfo()->TargetRange, pMugong->GetSung() > 0);	//攻击目标︰半径 内的所有目标 (目标为中心)
				pMugong->AddToolTipLine(line, TTTC_EXTRAATTR);
			}
			else
			{
				sprintf(line, CHATMGR->GetChatMsg(2793), pCommSkill->GetSkillInfo()->TargetRange);	//攻击目标︰单一目标
				pMugong->AddToolTipLine(line, TTTC_EXTRAATTR);
			}
		}
		else
		{
			sprintf(line, CHATMGR->GetChatMsg(2794), pCommSkill->GetSkillInfo()->TargetRange);	//攻击目标︰单一目标
			pMugong->AddToolTipLine(line, TTTC_EXTRAATTR);
		}
	}
	break;
	case ATTR_ALLATTR://全属性
	{
		if (pCommSkill->GetSkillInfo()->SkillRange)
		{
			sprintf(line, CHATMGR->GetChatMsg(2792), pCommSkill->GetSkillInfo()->SkillRange, pMugong->GetSung() > 0);	//攻击距离
			pMugong->AddToolTipLine(line, TTTC_EXTRAATTR);

			if (pCommSkill->GetSkillInfo()->TargetRange)
			{
				sprintf(line, CHATMGR->GetChatMsg(2792), pCommSkill->GetSkillInfo()->TargetRange, pMugong->GetSung() > 0);	//攻击目标︰半径 内的所有目标 (目标为中心)
				pMugong->AddToolTipLine(line, TTTC_EXTRAATTR);
			}
			else
			{
				sprintf(line, CHATMGR->GetChatMsg(2793), pCommSkill->GetSkillInfo()->TargetRange);	//攻击目标︰单一目标
				pMugong->AddToolTipLine(line, TTTC_EXTRAATTR);
			}
		}
		else
		{
			sprintf(line, CHATMGR->GetChatMsg(2794), pCommSkill->GetSkillInfo()->TargetRange);	//攻击目标︰单一目标
			pMugong->AddToolTipLine(line, TTTC_EXTRAATTR);
		}
	}
	break;
	}
	switch (pCommSkill->GetSkillKind())
	{
	case SKILLKIND_JINBUB: //阵法
	{
		pMugong->AddToolTipLine(CHATMGR->GetChatMsg(2794), TTTC_EXTRAATTR); break;
	}
	break;
	case SKILLKIND_SIMBUB: //心法
	{
		pMugong->AddToolTipLine(CHATMGR->GetChatMsg(2794), TTTC_EXTRAATTR); break;
	}
	break;
	}

	pMugong->AddToolTipLine("");   // 添一空行
	if (pCommSkill->GetFirstPhyAttack(pMugong->GetSung() > 0))
	{
		sprintf(line, CHATMGR->GetChatMsg(2795), pCommSkill->GetFirstPhyAttack(pMugong->GetSung()));
		pMugong->AddToolTipLine(line, RGB_HALF(255, 255, 0));
	}
	if (pCommSkill->GetFirstAttAttackMin(pMugong->GetSung() > 0))
	{
		sprintf(line, CHATMGR->GetChatMsg(2796), pCommSkill->GetFirstAttAttackMin(pMugong->GetSung()), pCommSkill->GetFirstAttAttackMax(pMugong->GetSung()));
		pMugong->AddToolTipLine(line, RGB_HALF(255, 255, 0));
	}

	if (pCommSkill->GetContinueAttAttack(pMugong->GetSung() > 0))
	{
		sprintf(line, CHATMGR->GetChatMsg(2797), pCommSkill->GetContinueAttAttack(pMugong->GetSung()));
		pMugong->AddToolTipLine(line, RGB_HALF(255, 255, 0));
	}
	if (pCommSkill->GetContinueAttAttackRate(pMugong->GetSung() > 0))
	{
		sprintf(line, CHATMGR->GetChatMsg(2798), pCommSkill->GetContinueAttAttackRate(pMugong->GetSung()));
		pMugong->AddToolTipLine(line, RGB_HALF(255, 255, 0));
	}
	if (pCommSkill->GetAttackRate(pMugong->GetSung() > 0))
	{
		sprintf(line, CHATMGR->GetChatMsg(2799), pCommSkill->GetAttackRate(pMugong->GetSung()));
		pMugong->AddToolTipLine(line, RGB_HALF(255, 255, 0));
	}
	if (pCommSkill->GetCounterDodgeRate(pMugong->GetSung() > 0))
	{
		sprintf(line, CHATMGR->GetChatMsg(2800), pCommSkill->GetCounterDodgeRate(pMugong->GetSung()));
		pMugong->AddToolTipLine(line, RGB_HALF(255, 255, 0));
	}
	if (pCommSkill->GetCriticalRate(pMugong->GetSung() > 0))
	{
		sprintf(line, CHATMGR->GetChatMsg(2801), pCommSkill->GetCriticalRate(pMugong->GetSung()));
		pMugong->AddToolTipLine(line, RGB_HALF(255, 255, 0));
	}
	if (pCommSkill->GetStunRate(pMugong->GetSung() > 0))
	{
		sprintf(line, CHATMGR->GetChatMsg(2802), pCommSkill->GetStunRate(pMugong->GetSung()));
		pMugong->AddToolTipLine(line, RGB_HALF(255, 255, 0));
	}
	if (pCommSkill->GetDamageRate(pMugong->GetSung() > 0))
	{
		sprintf(line, CHATMGR->GetChatMsg(2803), pCommSkill->GetDamageRate(pMugong->GetSung()));
		pMugong->AddToolTipLine(line, RGB_HALF(255, 255, 0));
	}
	if (pCommSkill->GetContinueRecoverLife(pMugong->GetSung() > 0))
	{
		sprintf(line, CHATMGR->GetChatMsg(2804), pCommSkill->GetContinueRecoverLife(pMugong->GetSung()));
		pMugong->AddToolTipLine(line, RGB_HALF(255, 255, 0));
	}
	if (pCommSkill->GetContinueRecoverShield(pMugong->GetSung() > 0))
	{
		sprintf(line, CHATMGR->GetChatMsg(2805), pCommSkill->GetContinueRecoverShield(pMugong->GetSung()));
		pMugong->AddToolTipLine(line, RGB_HALF(255, 255, 0));
	}
	if (pCommSkill->GetReverseVampiricNaeryuk(pMugong->GetSung() > 0))
	{
		sprintf(line, CHATMGR->GetChatMsg(2806), pCommSkill->GetReverseVampiricNaeryuk(pMugong->GetSung()));
		pMugong->AddToolTipLine(line, RGB_HALF(255, 255, 0));
	}
	if (pCommSkill->GetReverseVampiricLife(pMugong->GetSung() > 0))
	{
		sprintf(line, CHATMGR->GetChatMsg(2807), pCommSkill->GetReverseVampiricLife(pMugong->GetSung()));
		pMugong->AddToolTipLine(line, RGB_HALF(255, 255, 0));
	}
	if (pCommSkill->GetAttackPhyLastUp(pMugong->GetSung() > 0))
	{
		sprintf(line, CHATMGR->GetChatMsg(2808), pCommSkill->GetAttackPhyLastUp(pMugong->GetSung()));
		pMugong->AddToolTipLine(line, RGB_HALF(255, 255, 0));
	}
	if (pCommSkill->GetAttackAttLastUp(pMugong->GetSung() > 0))
	{
		sprintf(line, CHATMGR->GetChatMsg(2809), pCommSkill->GetAttackAttLastUp(pMugong->GetSung()));
		pMugong->AddToolTipLine(line, RGB_HALF(255, 255, 0));
	}
	if (pCommSkill->GetAttackLifeRate(pMugong->GetSung() > 0))
	{
		sprintf(line, CHATMGR->GetChatMsg(2810), pCommSkill->GetAttackLifeRate(pMugong->GetSung()));
		pMugong->AddToolTipLine(line, RGB_HALF(255, 255, 0));
	}
	if (pCommSkill->GetAttackShieldRate(pMugong->GetSung() > 0))
	{
		sprintf(line, CHATMGR->GetChatMsg(2811), pCommSkill->GetAttackShieldRate(pMugong->GetSung()));
		pMugong->AddToolTipLine(line, RGB_HALF(255, 255, 0));
	}
	if (pCommSkill->GetUpMaxShield(pMugong->GetSung() > 0))
	{
		sprintf(line, CHATMGR->GetChatMsg(2812), pCommSkill->GetUpMaxShield(pMugong->GetSung()));
		pMugong->AddToolTipLine(line, RGB_HALF(255, 255, 0));
	}
	if (pCommSkill->GetUpPhyAttack(pMugong->GetSung() > 0))
	{
		sprintf(line, CHATMGR->GetChatMsg(2813), pCommSkill->GetUpPhyAttack(pMugong->GetSung()));
		pMugong->AddToolTipLine(line, RGB_HALF(255, 255, 0));
	}
	if (pCommSkill->GetUpMaxLife(pMugong->GetSung() > 0))
	{
		sprintf(line, CHATMGR->GetChatMsg(2814), pCommSkill->GetUpMaxLife(pMugong->GetSung()));
		pMugong->AddToolTipLine(line, RGB_HALF(255, 255, 0));
	}
	if (pCommSkill->GetUpPhyDefense(pMugong->GetSung() > 0))
	{
		sprintf(line, CHATMGR->GetChatMsg(2815), pCommSkill->GetUpPhyDefense(pMugong->GetSung()));
		pMugong->AddToolTipLine(line, RGB_HALF(255, 255, 0));
	}
	if (pCommSkill->GetUpAttDefense(pMugong->GetSung() > 0))
	{
		sprintf(line, CHATMGR->GetChatMsg(2816), pCommSkill->GetUpAttDefense(pMugong->GetSung()));
		pMugong->AddToolTipLine(line, RGB_HALF(255, 255, 0));
	}
	if (pCommSkill->GetUpMaxNaeRyuk(pMugong->GetSung() > 0))
	{
		sprintf(line, CHATMGR->GetChatMsg(2817), pCommSkill->GetUpMaxNaeRyuk(pMugong->GetSung()));
		pMugong->AddToolTipLine(line, RGB_HALF(255, 255, 0));
	}
	if (pCommSkill->GetContinueRecoverNaeRyuk(pMugong->GetSung() > 0))
	{
		sprintf(line, CHATMGR->GetChatMsg(2818), pCommSkill->GetContinueRecoverNaeRyuk(pMugong->GetSung()));
		pMugong->AddToolTipLine(line, RGB_HALF(255, 255, 0));
	}
	if (pCommSkill->GetAmplifiedPowerPhy(pMugong->GetSung() > 0))
	{
		sprintf(line, CHATMGR->GetChatMsg(2819), pCommSkill->GetAmplifiedPowerPhy(pMugong->GetSung()));
		pMugong->AddToolTipLine(line, RGB_HALF(255, 255, 0));
	}
	if (pCommSkill->GetDispelAttackFeelRate(pMugong->GetSung() > 0))
	{
		sprintf(line, CHATMGR->GetChatMsg(2820), pCommSkill->GetDispelAttackFeelRate(pMugong->GetSung()));
		pMugong->AddToolTipLine(line, RGB_HALF(255, 255, 0));
	}
	if (pCommSkill->GetChangeSpeedProbability(pMugong->GetSung() > 0))
	{
		sprintf(line, CHATMGR->GetChatMsg(2821), pCommSkill->GetChangeSpeedProbability(pMugong->GetSung()));
		pMugong->AddToolTipLine(line, RGB_HALF(255, 255, 0));
	}
	if (pCommSkill->GetDownMaxLife(pMugong->GetSung() > 0))
	{
		sprintf(line, CHATMGR->GetChatMsg(2822), pCommSkill->GetDownMaxLife(pMugong->GetSung()));
		pMugong->AddToolTipLine(line, RGB_HALF(255, 255, 0));
	}
	if (pCommSkill->GetDownMaxNaeRyuk(pMugong->GetSung() > 0))
	{
		sprintf(line, CHATMGR->GetChatMsg(2823), pCommSkill->GetDownMaxNaeRyuk(pMugong->GetSung()));
		pMugong->AddToolTipLine(line, RGB_HALF(255, 255, 0));
	}
	if (pCommSkill->GetDownMaxShield(pMugong->GetSung() > 0))
	{
		sprintf(line, CHATMGR->GetChatMsg(2824), pCommSkill->GetDownMaxShield(pMugong->GetSung()));
		pMugong->AddToolTipLine(line, RGB_HALF(255, 255, 0));
	}
	if (pCommSkill->GetDownPhyDefense(pMugong->GetSung() > 0))
	{
		sprintf(line, CHATMGR->GetChatMsg(2825), pCommSkill->GetDownPhyDefense(pMugong->GetSung()));
		pMugong->AddToolTipLine(line, RGB_HALF(255, 255, 0));
	}
	if (pCommSkill->GetDownAttDefense(pMugong->GetSung() > 0))
	{
		sprintf(line, CHATMGR->GetChatMsg(2826), pCommSkill->GetDownAttDefense(pMugong->GetSung()));
		pMugong->AddToolTipLine(line, RGB_HALF(255, 255, 0));
	}
	if (pCommSkill->GetDownPhyAttack(pMugong->GetSung() > 0))
	{
		sprintf(line, CHATMGR->GetChatMsg(2827), pCommSkill->GetDownPhyAttack(pMugong->GetSung()));
		pMugong->AddToolTipLine(line, RGB_HALF(255, 255, 0));
	}
	if (pCommSkill->GetAmplifiedPowerAtt(pMugong->GetSung() > 0))
	{
		sprintf(line, CHATMGR->GetChatMsg(2828), pCommSkill->GetAmplifiedPowerAtt(pMugong->GetSung()));
		pMugong->AddToolTipLine(line, RGB_HALF(255, 255, 0));
	}
	if (pCommSkill->GetAmplifiedPowerAttRate(pMugong->GetSung() > 0))
	{
		sprintf(line, CHATMGR->GetChatMsg(2829), pCommSkill->GetAmplifiedPowerAttRate(pMugong->GetSung()));
		pMugong->AddToolTipLine(line, RGB_HALF(255, 255, 0));
	}


	if (pMugong->GetLastSecond())
	{
		char ToolTipText[32];
		wsprintf(ToolTipText, CHATMGR->GetChatMsg(582), pMugong->GetLastSecond());
		pMugong->AddToolTipLine(ToolTipText, TTTC_QUESTITEM);
	}

	if (pCommSkill->GetSpecialState())
	{
		pMugong->AddToolTipLine(CHATMGR->GetChatMsg(1359), TTTC_MUGONGNEEDMANA);

		SPECIAL_STATE_INFO* pInfo = SKILLMGR->GetSpecialStateInfo(pCommSkill->GetSpecialState());
		if (pInfo)
		{
			char ToolTipText[32];
			wsprintf(ToolTipText, CHATMGR->GetChatMsg(1360), pInfo->NaeRyuk);
			pMugong->AddToolTipLine(ToolTipText, TTTC_MUGONGNEEDMANA);
		}
	}

	SKILLOPTION* pOption = SKILLMGR->GetSkillOption(pMugong->GetOption());

	if (pOption)
	{
		pMugong->AddToolTipLine("");

		if (pOption->Range > 0)
		{
			char ToolTipText[32];
			wsprintf(ToolTipText, CHATMGR->GetChatMsg(1316), pOption->Range);
			pMugong->AddToolTipLine(ToolTipText, TTTC_TRANSMUGONGOPTION);
		}
		if (pOption->Range < 0)
		{
			char ToolTipText[32];
			wsprintf(ToolTipText, CHATMGR->GetChatMsg(1320), -pOption->Range);
			pMugong->AddToolTipLine(ToolTipText, TTTC_TRANSMUGONGOPTION);
		}

		if (pOption->ReduceNaeRyuk > 0)
		{
			char ToolTipText[32];
			wsprintf(ToolTipText, CHATMGR->GetChatMsg(1317), (int)(pOption->ReduceNaeRyuk * 100 + 0.5));
			pMugong->AddToolTipLine(ToolTipText, TTTC_TRANSMUGONGOPTION);
		}
		if (pOption->ReduceNaeRyuk < 0)
		{
			char ToolTipText[32];
			wsprintf(ToolTipText, CHATMGR->GetChatMsg(1321), (int)(-pOption->ReduceNaeRyuk * 100 + 0.5));
			pMugong->AddToolTipLine(ToolTipText, TTTC_TRANSMUGONGOPTION);
		}

		if (pOption->PhyAtk > 0)
		{
			char ToolTipText[32];
			wsprintf(ToolTipText, CHATMGR->GetChatMsg(1318), (int)(pOption->PhyAtk * 100 + 0.5));
			pMugong->AddToolTipLine(ToolTipText, TTTC_TRANSMUGONGOPTION);
		}
		if (pOption->PhyAtk < 0)
		{
			char ToolTipText[32];
			wsprintf(ToolTipText, CHATMGR->GetChatMsg(1322), (int)(-pOption->PhyAtk * 100 + 0.5));
			pMugong->AddToolTipLine(ToolTipText, TTTC_TRANSMUGONGOPTION);
		}

		if (pOption->BaseAtk)
		{
			char ToolTipText[32];
			wsprintf(ToolTipText, CHATMGR->GetChatMsg(1740), (int)(pOption->BaseAtk * 100));
			//wsprintf(ToolTipText, "老馆 傍拜仿 %d%% %s", (int)(pOption->BaseAtk * 100), CheckMugongOption(pOption->BaseAtk));
			pMugong->AddToolTipLine(ToolTipText, TTTC_TRANSMUGONGOPTION);
		}

		if (pOption->AttAtk > 0)
		{
			char ToolTipText[32];
			wsprintf(ToolTipText, CHATMGR->GetChatMsg(1319), (int)(pOption->AttAtk * 100 + 0.5));
			pMugong->AddToolTipLine(ToolTipText, TTTC_TRANSMUGONGOPTION);
		}
		if (pOption->AttAtk < 0)
		{
			char ToolTipText[32];
			wsprintf(ToolTipText, CHATMGR->GetChatMsg(1323), (int)(-pOption->AttAtk * 100 + 0.5));
			pMugong->AddToolTipLine(ToolTipText, TTTC_TRANSMUGONGOPTION);
		}

		if (pOption->Life > 0)
		{
			char ToolTipText[32];
			wsprintf(ToolTipText, CHATMGR->GetChatMsg(1340), pOption->Life);
			pMugong->AddToolTipLine(ToolTipText, TTTC_TRANSMUGONGOPTION);

		}
		if (pOption->Life < 0)
		{
			char ToolTipText[32];
			wsprintf(ToolTipText, CHATMGR->GetChatMsg(1341), -pOption->Life);
			pMugong->AddToolTipLine(ToolTipText, TTTC_TRANSMUGONGOPTION);
		}

		if (pOption->NaeRyuk > 0)
		{
			char ToolTipText[32];
			wsprintf(ToolTipText, CHATMGR->GetChatMsg(1342), pOption->NaeRyuk);
			pMugong->AddToolTipLine(ToolTipText, TTTC_TRANSMUGONGOPTION);
		}
		if (pOption->NaeRyuk < 0)
		{
			char ToolTipText[32];
			wsprintf(ToolTipText, CHATMGR->GetChatMsg(1343), -pOption->NaeRyuk);
			pMugong->AddToolTipLine(ToolTipText, TTTC_TRANSMUGONGOPTION);
		}

		if (pOption->Shield > 0)
		{
			char ToolTipText[32];
			wsprintf(ToolTipText, CHATMGR->GetChatMsg(1344), pOption->Shield);
			pMugong->AddToolTipLine(ToolTipText, TTTC_TRANSMUGONGOPTION);
		}
		if (pOption->Shield < 0)
		{
			char ToolTipText[32];
			wsprintf(ToolTipText, CHATMGR->GetChatMsg(1345), -pOption->Shield);
			pMugong->AddToolTipLine(ToolTipText, TTTC_TRANSMUGONGOPTION);
		}

		if (pOption->PhyDef > 0)
		{
			char ToolTipText[32];
			wsprintf(ToolTipText, CHATMGR->GetChatMsg(1348), (int)(pOption->PhyDef * 100 + 0.5));
			pMugong->AddToolTipLine(ToolTipText, TTTC_TRANSMUGONGOPTION);
		}
		if (pOption->PhyDef < 0)
		{
			char ToolTipText[32];
			wsprintf(ToolTipText, CHATMGR->GetChatMsg(1349), (int)(-pOption->PhyDef * 100 + 0.5));
			pMugong->AddToolTipLine(ToolTipText, TTTC_TRANSMUGONGOPTION);
		}

		if (pOption->AttDef > 0)
		{
			char ToolTipText[32];
			wsprintf(ToolTipText, CHATMGR->GetChatMsg(1324), (int)(pOption->AttDef * 100 + 0.5));
			pMugong->AddToolTipLine(ToolTipText, TTTC_TRANSMUGONGOPTION);
		}
		if (pOption->AttDef < 0)
		{
			char ToolTipText[32];
			wsprintf(ToolTipText, CHATMGR->GetChatMsg(1325), (int)(-pOption->AttDef * 100 + 0.5));
			pMugong->AddToolTipLine(ToolTipText, TTTC_TRANSMUGONGOPTION);
		}

		if (pOption->Duration > 0)
		{
			char ToolTipText[32];
			wsprintf(ToolTipText, CHATMGR->GetChatMsg(1346), pOption->Duration);
			pMugong->AddToolTipLine(ToolTipText, TTTC_TRANSMUGONGOPTION);
		}
		if (pOption->Duration < 0)
		{
			char ToolTipText[32];
			wsprintf(ToolTipText, CHATMGR->GetChatMsg(1347), -pOption->Duration);
			pMugong->AddToolTipLine(ToolTipText, TTTC_TRANSMUGONGOPTION);
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
			// magi82 - Titan(070911) 鸥捞藕 公傍诀单捞飘

			// 鸥捞藕 啪铰惑怕啊 酒聪搁 公傍阑 给静霸 秦具窃.
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
	
	//饭骇眉农殿 救窍唱?
	if( HERO && pMugong->m_pSkillInfo )
	if( pMugong->m_pSkillInfo->GetSkillInfo() )
	if( pMugong->m_pSkillInfo->GetSkillInfo()->RestrictLevel > HERO->GetLevel() )
		return eMCUR_Limit;
		
	if( pMugong->GetSung() == 0 )
		return eMCUR_SungZero;

	if( pMugong->m_pSkillInfo == NULL )
		return eMCUR_Disable;

	// magi82 - Titan(070911) 鸥捞藕 公傍诀单捞飘
	// 鸥捞藕 啪铰惑怕啊 酒聪搁 公傍阑 给静霸 秦具窃.
	if( HERO->InTitan() == FALSE && pMugong->m_pSkillInfo->GetSkillKind() == SKILLKIND_TITAN )
		return eMCUR_Titan;
	if( HERO->InTitan() == TRUE && (pMugong->m_pSkillInfo->GetSkillKind() == SKILLKIND_OUTERMUGONG || pMugong->m_pSkillInfo->GetSkillKind() == SKILLKIND_INNERMUGONG) )
		return eMCUR_Titan;

	if( pMugong->m_pSkillInfo->GetSkillKind() == SKILLKIND_OUTERMUGONG &&
		pMugong->m_pSkillInfo->GetWeaponType() != HERO->GetWeaponEquipType() )

		return eMCUR_Weapon;

	// magi82 - Titan(071011) 鸥捞藕 公傍诀单捞飘
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

	// 拳版, 必付傈流饶俊 惑己包拌狼 公傍 给静霸
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

	// 例檬 八祸 hs
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

//////// 2007. 7. 2. CBH - 傈巩胶懦 八祸 ///////////////
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




