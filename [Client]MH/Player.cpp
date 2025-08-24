// Player.cpp: implementation of the CPlayer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Player.h"
#include "GameResourceManager.h"
#include "MoveManager.h "
#include "./interface/cWindowManager.h"
#include "AppearanceManager.h"
#include "PartyManager.h"
#include "ItemManager.h"
#include "ObjectManager.h"
#include "KyungGongManager.h"
#include "StreetStallTitleTip.h"
#include "StreetStallManager.h"
#include "GameIn.h"
#include "PeaceWarModeManager.h"
#include "ObjectBalloon.h"
#include "MonsterGuageDlg.h"
#include "AbilityManager.h"

#include "MainGame.h"
#include "WindowIdEnum.h"
#include "MapChange.h"
#include "cMsgBox.h"

#include "ObjectStateManager.h"
#include "PKManager.h"
#include "ChatManager.h"
#include "GuildFieldWar.h"

#include "OptionManager.h"

#include "Pet.h"
#include "PetBase.h"
#include "UngijosikManager.h"

#include "CharacterDialog.h"
#include "WeatherManager.h"

#include "Titan.h"
#include "TitanManager.h"

#include "cScriptManager.h"
//#include "GameAIManager.h"   //weiye ¶¨ÒåÄÚ¹ÒÍ·ÎÄ¼þ 2017-12-05
#include "AIManager.h"
//#include "QuestGroup.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//int CPlayerAI::m_MonsterAttrTime;

CPlayer::CPlayer()
{
	m_ItemUseEffect = -1;
	m_DecorationAddress = NULL;	
	InitSkinDelayTime();
	m_NameImg=0;                //weiye Ä¬ÈÏÃ»ÓÐ³ÆºÅÍ¼Æ¬ 2018-04-21
}

CPlayer::~CPlayer()
{
//	if( m_pQuestGroup )
//		delete m_pQuestGroup;
}

void CPlayer::InitPlayer(CHARACTER_TOTALINFO* pTotalInfo)
{
	memcpy(&m_CharacterInfo,pTotalInfo,sizeof(CHARACTER_TOTALINFO));
	memset(&m_MoveInfo,0,sizeof(MOVE_INFO));
	memset(&m_ShopItemOption, 0, sizeof(SHOPITEMOPTION));
	memset(&m_AvatarOption, 0, sizeof(AVATARITEMOPTION));	

	//////////////////////////////////////////////////////////////////////////
	// 06. 06. 2Â÷ ÀüÁ÷ - ÀÌ¿µÁØ
	// ¹«°ø º¯È¯ Ãß°¡
	memset(&m_SkillStatsOption, 0, sizeof(SKILLSTATSOPTION));
	//////////////////////////////////////////////////////////////////////////

	for(int i=eAvatar_Weared_Hair; i<eAvatar_Max; i++)	
		m_ShopItemOption.Avatar[i] = 1;
	
	m_pPet = NULL;
	m_pTitan = NULL;
	m_bInTitan = FALSE;
	m_TitanMoveSpeed = 0;
	m_bTitanPreView = FALSE;

	SetBaseMotion();

	m_dwSetItemEffectID = 0;

	memset(&m_TitanAppearanceInfo,0,sizeof(TITAN_APPEARANCEINFO));
	memset(&m_TitanPreViewAppInfo,0,sizeof(TITAN_APPEARANCEINFO));

	OBJECTMGR->RemoveDecoration(this);	// magi82(44) - ³ëÁ¡»ó ¾÷µ¥ÀÌÆ®

	m_NameImg=0;            //weiye Ä¬ÈÏÃ»ÓÐ³ÆºÅÍ¼Æ¬ 2018-04-21
}

void CPlayer::GetCharacterTotalInfo(CHARACTER_TOTALINFO* pRtInfo)
{
	memcpy(pRtInfo,&m_CharacterInfo,sizeof(CHARACTER_TOTALINFO));
}

#define DecorationTime	1500

void CPlayer::Process()
{
	if( GetState() == eObjectState_Die )
	if( GAMEIN->GetMonsterGuageDlg() )
	{
		CObject * pSelectedObj = GAMEIN->GetMonsterGuageDlg()->GetCurMonster();
		if( pSelectedObj == this )
		{
//			OBJECTMGR->SetSelectedObject(NULL);
			GAMEIN->GetMonsterGuageDlg()->SetActive(FALSE);
		}
	}

	CObject::Process();

	// magi82(44) - ³ëÁ¡»ó ²Ù¹Ì±â ¾÷µ¥ÀÌÆ®
	// ³ëÁ¡»ó ²Ù¹Ì±â °´Ã¼´Â ¼­¼­È÷ »ç¶óÁö¹Ç·Î ÇÃ·¹ÀÌ¾îÀÇ ÇÁ·Î¼¼½º¿¡¼­ ¾ËÆÄ°ªÀ» ¼­¼­È÷ »©ÁÖ´Ù°¡ Áö¿öÁØ´Ù.
	CObject* pObject = GetDecorationAddress();
	if(pObject &&  m_dwDecorationStartTime > 0)
	{
		float alp = GetAlphaValue(DecorationTime, m_dwDecorationStartTime, m_bDecorationFlag);
		
		if(pObject)
            pObject->GetEngineObject()->SetAlpha(alp);

		if( gCurTime - m_dwDecorationStartTime > DecorationTime && m_bDecorationFlag == FALSE )	// m_bDecorationFlag °¡ FALSE ÀÌ¸é µ¥ÄÚ·¹ÀÌ¼Ç ÇØÁ¦
		{
			SetDecorationInfo(0, FALSE);
			alp = 0.0f;
			OBJECTMGR->RemoveDecoration(this);
		}
	}

	//2008. 1. 21. CBH - ½ºÅ² ·¹ÀÌ Ã³¸® Ãß°¡
	if(m_bSkinDelayResult == TRUE)
	{
		DWORD dwCurrentTime = gCurTime;
		if(dwCurrentTime > m_dwSkinDelayTime)
		{
			InitSkinDelayTime();
		}
	}
}


void CPlayer::SetWearedItemIdx(DWORD WearedPosition,WORD ItemIdx)
{
	if(WearedPosition == eWearedItem_Weapon)
	{
		// ¹«±â¸¦ ¹Ù²Ù¸é ÁõÆøÀº »ç¶óÁø´Ù.
		GetSpecialStateParam1(eSpecialState_AmplifiedPowerPhy);
	}

	m_CharacterInfo.WearedItemIdx[WearedPosition] = ItemIdx;
	SetBaseMotion();			// ¹«±â°¡ ¹Ù²î¸é ±âº»µ¿ÀÛÀÌ ¹Ù²ï´Ù
}


WORD CPlayer::GetWeaponEquipType()//»ñÈ¡µ±Ç°ÎäÆ÷ÊÇÊ²Ã´ÖÖÀà
{
	WORD WeaponItemIdx = GetWearedItemIdx(eWearedItem_Weapon);
	if(WeaponItemIdx == 0)
		return WP_GWUN;
	ITEM_INFO * pItemInfo = ITEMMGR->GetItemInfo(WeaponItemIdx);
	ASSERT(pItemInfo);
	if(pItemInfo && pItemInfo->WeaponType != 0)
		return pItemInfo->WeaponType;
	else
		return 0;
}

WORD CPlayer::GetTitanWeaponEquipType()
{
	WORD WeaponItemIdx = 0;
	// magi82 - Titan(070912) Å¸ÀÌÅº ¹«°ø¾÷µ¥ÀÌÆ®
//	if( InTitan() )
		WeaponItemIdx = m_TitanAppearanceInfo.WearedItemIdx[eTitanWearedItem_Weapon];
//	else if( IsTitanPreView() )
//		WeaponItemIdx = m_TitanPreViewAppInfo.WearedItemIdx[eTitanWearedItem_Weapon];

	if(WeaponItemIdx == 0)
		return WP_GWUN;
	ITEM_INFO * pItemInfo = ITEMMGR->GetItemInfo(WeaponItemIdx);
	ASSERT(pItemInfo);
	if(pItemInfo && pItemInfo->WeaponType != 0)
		return pItemInfo->WeaponType;
	else
		return 0;
}

WORD CPlayer::GetTitanWearedItemIdx(DWORD WearedPosition)
{	
	if( InTitan() )
		return m_TitanAppearanceInfo.WearedItemIdx[WearedPosition];
	else if( IsTitanPreView() )
		return m_TitanPreViewAppInfo.WearedItemIdx[WearedPosition];

	return 0;
}

void CPlayer::SetBaseMotion()
{
	WORD WeaponEquipType = 0;
	if( InTitan() )
		WeaponEquipType = GetTitanWeaponEquipType();
	else
		WeaponEquipType = GetWeaponEquipType();
	
	//	2005 Å©¸®½º¸¶½º ÀÌº¥Æ® ÄÚµå
	//////////////////////////////////////////////////////////////////////////
	// ÀÌº¥Æ®¹«±â Å¸ÀÔÀº º¡¾î¸® Àå°©! ±Ç ¹«±â ¸ð¼Ç »ç¿ë
	if(WeaponEquipType == WP_EVENT)
		WeaponEquipType = WP_GWUN;
	else if( WeaponEquipType == WP_EVENT_HAMMER )
		WeaponEquipType = WP_GUM;
	//////////////////////////////////////////////////////////////////////////
	
	if(m_MoveInfo.KyungGongIdx)
	{	// °æ°øÁßÀÌ¸é
		// °æ°ø Á¾·ù¿¡ µû¶ó °æ°øµ¿ÀÛ 1,2,3Áß °áÁ¤ÇÑ´Ù.
		CKyungGongInfo* pKGInfo = KYUNGGONGMGR->GetKyungGongInfo(m_MoveInfo.KyungGongIdx);
		ASSERT(pKGInfo);
		int MoveType;
		if(pKGInfo != NULL)
			MoveType = pKGInfo->GetMoveType() - 1;		// -1 Àº 1ºÎÅÍ ½ÃÀÛÇÏ±â ¶§¹®¿¡ 0À¸·Î ½ÃÀÛÀ» ¸ÂÃã
		else
			MoveType = 0;
		
		// magi82(9) - Titan(071024) Å¸ÀÌÅº ÀÏ¶© µÎ¹øÂ° °æ°ø¶§ ½ÃÀÛµ¿ÀÛÀ» »©Áö¾Ê´Â´Ù.
		if(MoveType == 1 && this->InTitan() == FALSE)	// µÎ¹øÂ° °æ°øÀº ½ÃÀÛµ¿ÀÛ »­ by SSAMI
			m_Move_Start_Motion = eMotion_KyungGong1_Move_NoWeapon + MoveType*3;
		else
			m_Move_Start_Motion = eMotion_KyungGong1_Start_NoWeapon + MoveType*3;
		m_Move_Ing_Motion = eMotion_KyungGong1_Move_NoWeapon + MoveType*3;

		// RaMa - 05.09.05  - °æ°øÁ¾·á·¹ÀÌ ¾ø¾Ú
		if( GetAvatarOption()->bKyungGong || GetShopItemStats()->bKyungGong )
			m_Move_End_Motion = m_Move_End_Motion;
		// 2005 Å©¸®½º¸¶½º ÀÌº¥Æ® ÄÚµå
		else if( (WEATHERMGR->GetWeatherState() == eWS_Snow ) &&
				 ((GetShopItemStats()->Avatar[eAvatar_Dress] == EVENT_SHOPITEM_SNOWMAN_DRESS && GetShopItemStats()->Avatar[eAvatar_Hat] == EVENT_SHOPITEM_SNOWMAN_HAT) ||
				  (GetShopItemStats()->Avatar[eAvatar_Dress] ==  EVENT_SHOPITEM_RUDOLP_DRESS && GetShopItemStats()->Avatar[eAvatar_Hat] ==  EVENT_SHOPITEM_RUDOLP_HAT) ||
				  (GetShopItemStats()->Avatar[eAvatar_Dress] == EVENT_SHOPITEM_SNOWMAN_DRESS2 && GetShopItemStats()->Avatar[eAvatar_Hat] == EVENT_SHOPITEM_SNOWMAN_HAT2) ||
				  (GetShopItemStats()->Avatar[eAvatar_Dress] ==  EVENT_SHOPITEM_RUDOLP_DRESS2 && GetShopItemStats()->Avatar[eAvatar_Hat] ==  EVENT_SHOPITEM_RUDOLP_HAT2) || 
				  (GetShopItemStats()->Avatar[eAvatar_Dress] == EVENT_SHOPITEM_SNOWMAN_DRESS && GetShopItemStats()->Avatar[eAvatar_Hat] == EVENT_SHOPITEM_SNOWMAN_HAT2) ||
				  (GetShopItemStats()->Avatar[eAvatar_Dress] ==  EVENT_SHOPITEM_RUDOLP_DRESS && GetShopItemStats()->Avatar[eAvatar_Hat] ==  EVENT_SHOPITEM_RUDOLP_HAT2) ||
				  (GetShopItemStats()->Avatar[eAvatar_Dress] == EVENT_SHOPITEM_SNOWMAN_DRESS2 && GetShopItemStats()->Avatar[eAvatar_Hat] == EVENT_SHOPITEM_SNOWMAN_HAT) ||
				  (GetShopItemStats()->Avatar[eAvatar_Dress] ==  EVENT_SHOPITEM_RUDOLP_DRESS2 && GetShopItemStats()->Avatar[eAvatar_Hat] ==  EVENT_SHOPITEM_RUDOLP_HAT) ||
				  GetShopItemStats()->Avatar[eAvatar_Dress] ==  EVENT_SHOPITEM_SNOWMAN1_HK || GetShopItemStats()->Avatar[eAvatar_Dress] ==  EVENT_SHOPITEM_SNOWMAN2_HK ||
				  GetShopItemStats()->Avatar[eAvatar_Dress] ==  EVENT_SHOPITEM_SNOWMAN3_HK || GetShopItemStats()->Avatar[eAvatar_Dress] ==  EVENT_SHOPITEM_RUDOLP1_HK ||
				  GetShopItemStats()->Avatar[eAvatar_Dress] ==  EVENT_SHOPITEM_RUDOLP2_HK || GetShopItemStats()->Avatar[eAvatar_Dress] ==  EVENT_SHOPITEM_RUDOLP3_HK ))
					m_Move_End_Motion = m_Move_End_Motion;
		else
		{
			#ifdef TAIWAN_LOCAL
				m_Move_End_Motion = m_Move_End_Motion;
			#else
				m_Move_End_Motion = eMotion_KyungGong1_End_NoWeapon;// + MoveType*3;
			#endif
		}
		
		if(m_CharacterInfo.bPeace == TRUE)		// ÆòÈ­¸ðµå ÀÌ³Ä
		{	// ÆòÈ­¸ðµå
			if(WeaponEquipType == WP_DO)
				m_StandardMotion = eMotion_Peace_Standard_Do;
			else if(WeaponEquipType == WP_CHANG)
				m_StandardMotion = eMotion_Peace_Standard_Change;
			else if (WeaponEquipType == WP_AXE)
				m_StandardMotion = eMotion_Peace_Standard_Change;
			else
				m_StandardMotion = eMotion_Peace_Standard;
		}
		else
		{
			WORD WeaponEquipType = GetWeaponEquipType();

			//////////////////////////////////////////////////////////////////////////
			// ÀÌº¥Æ®¹«±â Å¸ÀÔÀº º¡¾î¸® Àå°©! ±Ç ¹«±â ¸ð¼Ç »ç¿ë
			if(WeaponEquipType == WP_EVENT)
				WeaponEquipType = WP_GWUN;
			else if( WeaponEquipType == WP_EVENT_HAMMER )
				WeaponEquipType = WP_GUM;
			//////////////////////////////////////////////////////////////////////////
			else if (WeaponEquipType == WP_AXE)
				m_StandardMotion = eMotion_Battle_Do_Standard;
			else if (WeaponEquipType == WP_DAGGER)
				m_StandardMotion = eMotion_Battle_Amgi_Standard;
			else
			m_StandardMotion = eMotion_Battle_Gum_Standard + WeaponEquipType -1;			
		}
		return;
	}

	if(m_CharacterInfo.bPeace == TRUE)		// ÆòÈ­¸ðµå ÀÌ³Ä
	{	// ÆòÈ­¸ðµå
		if(WeaponEquipType == WP_DO)
			m_StandardMotion = eMotion_Peace_Standard_Do;
		else if(WeaponEquipType == WP_CHANG)
			m_StandardMotion = eMotion_Peace_Standard_Change;
		else if (WeaponEquipType == WP_AXE)
			m_StandardMotion = eMotion_Peace_Standard_Do;
		else if (WeaponEquipType == WP_DAGGER)
			m_StandardMotion = eMotion_Peace_Standard;

		else
			m_StandardMotion = eMotion_Peace_Standard;

		if(m_MoveInfo.MoveMode == eMoveMode_Walk)
		{	// °È±â
			if(WeaponEquipType == WP_DO)
				m_Move_Start_Motion = eMotion_Peace_Walk_Do;
			else if(WeaponEquipType == WP_CHANG)
				m_Move_Start_Motion = eMotion_Peace_Walk_Change;
			else if (WeaponEquipType == WP_AXE)
				m_Move_Start_Motion = eMotion_Peace_Walk_Change;
			else if (WeaponEquipType == WP_DAGGER)
				m_Move_Start_Motion = eMotion_Peace_Walk;
			else
				m_Move_Start_Motion = eMotion_Peace_Walk;
			m_Move_Ing_Motion = m_Move_Start_Motion;
			m_Move_End_Motion = -1;
		}
		else
		{	// ¶Ù±â	
			if(WeaponEquipType == WP_DO)
				m_Move_Start_Motion = eMotion_Peace_Run_Do;
			else if(WeaponEquipType == WP_CHANG)
				m_Move_Start_Motion = eMotion_Peace_Run_Change;
			else if (WeaponEquipType == WP_AXE)
				m_Move_Start_Motion = eMotion_Peace_Run_Change;
			else if (WeaponEquipType == WP_DAGGER)
				m_Move_Start_Motion = eMotion_Peace_Run;

			else
				m_Move_Start_Motion = eMotion_Peace_Run;
			m_Move_Ing_Motion = m_Move_Start_Motion;
			m_Move_End_Motion = -1;
		}
	}

	else
	{
		// ÅÐ¶Ï×Ô¶¨ÒåÎäÆ÷ÀàÐÍ
		if (WeaponEquipType == WP_AXE)
			m_StandardMotion = eMotion_Battle_Do_Standard;  // ¼ÙÉè900ÊÇ¸«Í·µÄ±ê×¼´ý»ú¶¯×÷
		else if (WeaponEquipType == WP_DAGGER)
			m_StandardMotion = eMotion_Battle_Amgi_Standard;  // ¼ÙÉè910ÊÇ´Ì¿ÍË«µ¶µÄ±ê×¼´ý»ú¶¯×÷
		else
			m_StandardMotion = eMotion_Battle_Gum_Standard + WeaponEquipType - 1;

		// ²½ÐÐ×´Ì¬
		if (m_MoveInfo.MoveMode == eMoveMode_Walk)
		{
			if (WeaponEquipType == WP_AXE)
			{
				m_Move_Start_Motion = eMotion_Battle_Chang_Walk; // ¸«Í·²½ÐÐ¶¯×÷
				m_Move_Ing_Motion = eMotion_Battle_Chang_Walk;
			}
			else if (WeaponEquipType == WP_DAGGER)
			{
				m_Move_Start_Motion = eMotion_Battle_Amgi_Walk; // Ø°Ê×²½ÐÐ¶¯×÷
				m_Move_Ing_Motion = eMotion_Battle_Amgi_Walk;
			}
			else
			{
				m_Move_Start_Motion = eMotion_Battle_Gum_Walk + WeaponEquipType - 1;
				m_Move_Ing_Motion = eMotion_Battle_Gum_Walk + WeaponEquipType - 1;
			}
			m_Move_End_Motion = -1;
		}
		// ÅÜ²½×´Ì¬
		else
		{
			if (WeaponEquipType == WP_AXE)
			{
				m_Move_Start_Motion = eMotion_Battle_Chang_Run; // ¸«Í·ÅÜ¶¯
				m_Move_Ing_Motion = eMotion_Battle_Chang_Run;
			}
			else if (WeaponEquipType == WP_DAGGER)
			{
				m_Move_Start_Motion = eMotion_Battle_Amgi_Run; // Ø°Ê×ÅÜ¶¯
				m_Move_Ing_Motion = eMotion_Battle_Amgi_Run;
			}
			else
			{
				m_Move_Start_Motion = eMotion_Battle_Gum_Run + WeaponEquipType - 1;
				m_Move_Ing_Motion = eMotion_Battle_Gum_Run + WeaponEquipType - 1;
			}
			m_Move_End_Motion = -1;
		}
	}

}

BOOL CPlayer::StartSocietyAct( int nStartMotion, int nIngMotion, int nEndMotion, BOOL bHideWeapon )
{
	//Æò»ó »óÅÂ°¡ ¾Æ´Ï¸é ¾ÈµÈ´Ù.
	if( GetState() != eObjectState_None && GetState() != eObjectState_Society
		&& GetState() != eObjectState_Immortal ) return FALSE;

	OBJECTSTATEMGR->EndObjectState( this, GetState(), 0 );	//ÇöÀç »óÅÂ¸¦ ³¡³½´Ù.

	m_ObjectState.State_Start_Motion	= (short int)nStartMotion;
	m_ObjectState.State_Ing_Motion		= (short int)nIngMotion;
	m_ObjectState.State_End_Motion		= (short int)nEndMotion;

	if( nEndMotion != -1 )
		m_ObjectState.State_End_MotionTime	= GetEngineObject()->GetAnimationTime(nEndMotion);
	else
		m_ObjectState.State_End_MotionTime = 0;


	if(m_ObjectState.State_Start_Motion != -1)
	{
		ChangeMotion( m_ObjectState.State_Start_Motion, FALSE );

		if( m_ObjectState.State_Ing_Motion != -1 )
			ChangeBaseMotion( m_ObjectState.State_Ing_Motion );
	}

	if(m_ObjectState.State_End_MotionTime)
	m_ObjectState.BeforeState		= m_BaseObjectInfo.ObjectState;
	m_BaseObjectInfo.ObjectState	= eObjectState_Society;
	m_ObjectState.State_Start_Time	= gCurTime;
	m_ObjectState.bEndState			= FALSE;

	if( bHideWeapon )
		APPEARANCEMGR->HideWeapon( this );

	m_ObjectState.BeforeState		= m_BaseObjectInfo.ObjectState;
	m_BaseObjectInfo.ObjectState	= eObjectState_Society;
	m_ObjectState.State_Start_Time	= gCurTime;
	m_ObjectState.bEndState			= FALSE;
	
	if( m_ObjectState.State_Ing_Motion == -1 )
		OBJECTSTATEMGR->EndObjectState( this, eObjectState_Society, GetEngineObject()->GetAnimationTime( m_ObjectState.State_Start_Motion ) );

	return TRUE;
}


void CPlayer::SetMotionInState(BYTE State)
{
	switch(State)
	{
	case eObjectState_StreetStall_Owner:
		{
			m_ObjectState.State_Start_Motion = eMotion_StreetStall_Start;
			m_ObjectState.State_Ing_Motion = eMotion_StreetStall_Ing;
			m_ObjectState.State_End_Motion = eMotion_StreetStall_End;
			m_ObjectState.State_End_MotionTime = GetEngineObject()->GetAnimationTime(eMotion_StreetStall_End);
		}
		break;
	case eObjectState_Move:
		{
			m_ObjectState.State_Start_Motion = m_Move_Start_Motion;
			m_ObjectState.State_Ing_Motion = m_Move_Ing_Motion;
			m_ObjectState.State_End_Motion = m_Move_End_Motion;
			m_ObjectState.State_End_MotionTime = GetEngineObject()->GetAnimationTime(m_Move_End_Motion);
		}
		break;
	case eObjectState_None:
	case eObjectState_Exchange:
	case eObjectState_Deal:
	case eObjectState_Munpa:
	case eObjectState_PrivateWarehouse:
	case eObjectState_StreetStall_Guest:
		{
			m_ObjectState.State_Start_Motion = m_StandardMotion;
			
			//YH2DO
			if( GetState() == eObjectState_SkillSyn ||
				GetState() == eObjectState_SkillUsing )
				m_ObjectState.State_Start_Motion = -1;

			m_ObjectState.State_Ing_Motion = m_StandardMotion;
			m_ObjectState.State_End_Motion = -1;
			m_ObjectState.State_End_MotionTime = 0;
		}
		break;
	case eObjectState_Ungijosik:
		{
			m_ObjectState.State_Start_Motion = eMotion_Ungijosik_Start;
			m_ObjectState.State_Ing_Motion = eMotion_Ungijosik_Ing;
			m_ObjectState.State_End_Motion = eMotion_Ungijosik_End;
			m_ObjectState.State_End_MotionTime = GetEngineObject()->GetAnimationTime(eMotion_Ungijosik_End);
		}
		break;
		
	case eObjectState_Tactic:
		{
			m_ObjectState.State_Start_Motion = eMotion_Tactic_Start;
			m_ObjectState.State_Ing_Motion = eMotion_Tactic_Ing;
			m_ObjectState.State_End_Motion = -1;
			m_ObjectState.State_End_MotionTime = 0;
		}
		break;
	case eObjectState_Engrave:
		{
			m_ObjectState.State_Start_Motion = eMotion_Engrave_End;
			m_ObjectState.State_Ing_Motion = eMotion_Engrave_Ing;
			m_ObjectState.State_End_Motion = -1;
			m_ObjectState.State_End_MotionTime = GetEngineObject()->GetAnimationTime(eMotion_Ungijosik_End);
		}
		break;
	case eObjectState_TitanRecall:
		{
			m_ObjectState.State_Start_Motion = eMotion_Engrave_Start;
			m_ObjectState.State_Ing_Motion = eMotion_Engrave_Ing;
			m_ObjectState.State_End_Motion = -1;
			m_ObjectState.State_End_MotionTime = 0;
		}
		break;
	default:
		{
			m_ObjectState.State_Start_Motion = -1;
			m_ObjectState.State_Ing_Motion = -1;
			m_ObjectState.State_End_Motion = -1;
			m_ObjectState.State_End_MotionTime = 0;
		}
		break;
	}
}

BYTE CPlayer::GetLifePercent()
{
	return (BYTE)((m_CharacterInfo.Life*100) / GetMaxLife());
}

BYTE CPlayer::GetShieldPercent()
{
	return (BYTE)((m_CharacterInfo.Shield*100) / GetMaxShield());
}

//////////////////////////////////////////////////////////////////////////
// ObjectStateManager¿¡¼­ StateÀÇ ½ÃÀÛ°ú ³¡¿¡¼­ È£ÃâÇØÁÖ´Â ÇÔ¼öµé
void CPlayer::OnStartObjectState(BYTE State)
{
	switch(State)
	{
	case eObjectState_Move:
		{
			// magi82(4) - Titan(071023) Å¸ÀÌÅºÀÏ¶§ ÀÌÆåÆ® Ã³¸®(°È±â & ¶Ù±â)
			// magi82(11) - Effect(071025) °æ°ø ÀÌÆåÆ® ID ºÎ¿©
			if( this == HERO )
			{
				WORD KyungGongIdx = ((CObject*)this)->GetBaseMoveInfo()->KyungGongIdx;

				if( KyungGongIdx > 0 )
				{
					KyungGongIdx = KyungGongIdx - 2600;	// °æ°øÀº 2600ºÎÅÍÀÌ¹Ç·Î »©¼­ ZeroBase È­ ½ÃÄÑ¾ßÇÔ
					
					switch(KyungGongIdx)
					{
					case 0:	// ½ÅÇà¹éº¯
						{
							if(this->InTitan() == TRUE)
							{
								if( GetAvatarOption()->bKyungGong || GetShopItemStats()->bKyungGong )
								{
									OBJECTEFFECTDESC desc(eEffect_Titan_Shop_KyungGong1);
									AddObjectEffect(KYUNGGONG_EFFECTID,&desc,1);				
								}
								else
								{
									OBJECTEFFECTDESC desc(eEffect_Titan_KyungGong1);
									AddObjectEffect(KYUNGGONG_EFFECTID,&desc,1);				
								}
							}
							else
							{
								OBJECTEFFECTDESC desc(eEffect_KyungGong1);
								AddObjectEffect(KYUNGGONG_EFFECTID,&desc,1);
							}
						}
						break;
					case 2:	// ÃÊ»óºñ
						{
							if(this->InTitan() == TRUE)
							{
 								if( GetAvatarOption()->bKyungGong || GetShopItemStats()->bKyungGong )
								{
									OBJECTEFFECTDESC desc(eEffect_Titan_Shop_KyungGong2);
									AddObjectEffect(KYUNGGONG_EFFECTID,&desc,1);				
								}
								else
								{
									OBJECTEFFECTDESC desc(eEffect_Titan_KyungGong2);
									AddObjectEffect(KYUNGGONG_EFFECTID,&desc,1);				
								}
							}
							else
							{
								OBJECTEFFECTDESC desc(eEffect_KyungGong2);
								AddObjectEffect(KYUNGGONG_EFFECTID,&desc,1);
							}
						}
						break;
					case 4:	// ´É°øÇãµµ
						{
							if(this->InTitan() == TRUE)
							{
								if( GetAvatarOption()->bKyungGong || GetShopItemStats()->bKyungGong )
								{
									OBJECTEFFECTDESC desc(eEffect_Titan_Shop_KyungGong3);
									AddObjectEffect(KYUNGGONG_EFFECTID,&desc,1);				
								}
								else
								{
									OBJECTEFFECTDESC desc(eEffect_Titan_KyungGong3);
									AddObjectEffect(KYUNGGONG_EFFECTID,&desc,1);				
								}
							}
							else
							{
								OBJECTEFFECTDESC desc(eEffect_KyungGong3);
								AddObjectEffect(KYUNGGONG_EFFECTID,&desc,1);
							}
						}
						break;
					}
				}
				else
				{
					if( ((CObject*)this)->GetBaseMoveInfo()->MoveMode == eMoveMode_Run )
					{
						if(this->InTitan() == TRUE)
						{
							OBJECTEFFECTDESC desc(FindEffectNum("t_ba_004.beff"));
							AddObjectEffect(KYUNGGONG_EFFECTID,&desc,1);
						}
						else
						{
							OBJECTEFFECTDESC desc(eEffect_Run);
							AddObjectEffect(KYUNGGONG_EFFECTID,&desc,1);
						}
					}
					else if( ((CObject*)this)->GetBaseMoveInfo()->MoveMode == eMoveMode_Walk )
					{
						if(this->InTitan() == TRUE)
						{
							OBJECTEFFECTDESC desc(FindEffectNum("t_ba_003.beff"));
							AddObjectEffect(KYUNGGONG_EFFECTID,&desc,1);
						}
						else
						{
							OBJECTEFFECTDESC desc(eEffect_Walk);
							AddObjectEffect(KYUNGGONG_EFFECTID,&desc,1);
						}
					}
				}
			}
		}
		break;
	case eObjectState_Exit:
		{
			PEACEWARMGR->Peace_WarMode( this, TRUE );

			// magi82 - UniqueItem(071010) ±ÇÀÌ ¾Æ´Ò¶§¸¸ ¹«±â¸¦ ¼û±ä´Ù.
			if(GetWeaponEquipType() != WP_GWUN)
				APPEARANCEMGR->HideWeapon(this);
		}
		break;
	case eObjectState_Ungijosik:
		{
			// magi82 - UniqueItem(071010) ±ÇÀÌ ¾Æ´Ò¶§¸¸ ¹«±â¸¦ ¼û±ä´Ù.
			if(GetWeaponEquipType() != WP_GWUN)
				APPEARANCEMGR->HideWeapon(this);

			OBJECTEFFECTDESC desc(eEffect_Ungijosik);
			AddObjectEffect(STATE_EFFECT_ID,&desc,1);
		}
		break;
	case eObjectState_Tactic:
		{
		}
		break;
	case eObjectState_StreetStall_Owner:
		{
			// magi82 - UniqueItem(071010) ±ÇÀÌ ¾Æ´Ò¶§¸¸ ¹«±â¸¦ ¼û±ä´Ù.
			if(GetWeaponEquipType() != WP_GWUN)
				APPEARANCEMGR->HideWeapon(this);
		}
		break;
	case eObjectState_ItemUse:
		{
			if( m_ItemUseEffect == -1 )
				return;

			EFFECTMGR->StartPlayerEffectProcess( HERO, m_ItemUseEffect );
			m_ItemUseEffect = -1;
		}
		break;
	case eObjectState_TitanRecall:
		{
			OBJECTEFFECTDESC desc(FindEffectNum("m_Summon01.beff"));			
			AddObjectEffect( TITAN_RECALL_EFFECTID, &desc, 1, this );
		}
		break;
	}
}

void CPlayer::OnEndObjectState(BYTE State)
{
	switch(State)
	{
	case eObjectState_StreetStall_Owner:
	case eObjectState_Ungijosik:
	case eObjectState_Society:
		{
			WORD weaponType = GetWeaponEquipType();
			/*
			if(weaponType == WP_AMGI)
			{
				if(PEACEWARMGR->IsPeaceMode(this) == FALSE)
					APPEARANCEMGR->ShowWeapon(this);
			}
			//else if(weaponType == WP_GWUN)
			//{
			//	WORD WeaponItemIdx = GetWearedItemIdx(eWearedItem_Weapon);
			//	ITEM_INFO* pInfo = ITEMMGR->GetItemInfo(WeaponItemIdx);

			//	if(pInfo->ItemKind == eEQUIP_ITEM_UNIQUE)
			//	{
			//		APPEARANCEMGR->ShowWeapon(this);
			//	}
			//}
			else if(weaponType != WP_GWUN)
			{
				APPEARANCEMGR->ShowWeapon(this);
			}
			*/
			if(weaponType != WP_GWUN)
			{
				APPEARANCEMGR->ShowWeapon(this);
			}
		}
		break;
	case eObjectState_Move:
		{
			// magi82(11) - Effect(071025) °æ°ø ÀÌÆåÆ® ID ºÎ¿©
			RemoveObjectEffect(KYUNGGONG_EFFECTID);
		}
		break;
	case eObjectState_Die:
		{
			if(GetID() != HEROID)
				GetEngineObject()->EnablePick();
		}
		break;
	case eObjectState_Tactic:
		{
		}
		break;
	case eObjectState_ItemUse:
		{
			if( m_ItemUseMotion==eMotion_Item_ChangeHair_1 || m_ItemUseMotion==eMotion_Item_ChangeHair_2 || m_ItemUseMotion==eMotion_Item_ChangeHair_3 )
			{
				APPEARANCEMGR->InitAppearance( this );
			}

			//
			if(GetID() != HEROID )
				goto MotionReset;
			if( m_ItemUseMotion==eMotion_Item_Teleport_1 || m_ItemUseMotion==eMotion_Item_Teleport_2 || m_ItemUseMotion==eMotion_Item_Teleport_3 )

			{
				DWORD map = GAMEIN->GetMoveMap();
				WINDOWMGR->MsgBox( MBI_NOBTNMSGBOX, MBT_NOBTN, CHATMGR->GetChatMsg( 160 ) );
				MAPCHANGE->SetGameInInitKind(eGameInInitKind_MovePos);				
				MAINGAME->SetGameState(eGAMESTATE_MAPCHANGE,&map,4);
			}

MotionReset:
			m_ItemUseMotion = -1;
			m_ItemUseEffect = -1;
		}
		break;
	case eObjectState_TitanRecall:
		{
			RemoveObjectEffect( TITAN_RECALL_EFFECTID );
		}
		break;
	}
	
	RemoveObjectEffect(STATE_EFFECT_ID);	//????
}
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
//#include "FortWarDialog.h"
void CPlayer::Die(CObject* pAttacker,BOOL bFatalDamage,BOOL bCritical, BOOL bDecisive) 
{	
	/*if(HERO )
	{//kiv jack		
		if(GetState() == eObjectState_Engrave)
		{
			HERO->KillEngravingDlg();
		}
	}*/
	if(pAttacker)
	{
		MOVEMGR->SetLookatPos(this,&pAttacker->GetCurPosition(),0,gCurTime);

		if( HERO )
		if( pAttacker == HERO && HERO->IsPKMode() )
		{
			PKMGR->AddPKContinueTime( 60000 );
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(698), 60 );
		}
	}	
	int DieEffect;
	if(bFatalDamage)
		DieEffect = eEffect_Die2;
	else
		DieEffect = eEffect_Die1;
		
	TARGETSET set;
	set.pTarget = this;
	set.ResultInfo.bCritical = bCritical;
	set.ResultInfo.bDecisive = bDecisive;
	
	EFFECTMGR->StartEffectProcess(DieEffect,pAttacker,&set,1,GetID(),
							EFFECT_FLAG_DIE|EFFECT_FLAG_GENDERBASISTARGET);

	CObject::Die(pAttacker,bFatalDamage,bCritical,bDecisive);
}

void CPlayer::Damage(CObject* pAttacker,BYTE DamageKind,DWORD Damage,DWORD ShieldDamage,BOOL bCritical, BOOL bDecisive, DWORD titanObserbDamage)
{
	BOOL bEvent = FALSE;

	if (pAttacker)
	{
		if (pAttacker->GetObjectKind() & eObjectKind_Monster)
		{
			if (HERO->GetState() == eObjectState_Ungijosik && sCPlayerAI.IsOpen())
			{
				sCPlayerAI.HitByMonsterCount();
			}
		}
		if (pAttacker->GetObjectKind() == eObjectKind_Player)
		if (((CPlayer*)pAttacker)->GetWeaponEquipType() == WP_EVENT ||
			((CPlayer*)pAttacker)->GetWeaponEquipType() == WP_EVENT_HAMMER)
			bEvent = TRUE;
	}

	if (Damage != 0 || bEvent)
	{
		if (GetState() == eObjectState_None)
		{
			if (pAttacker)
				MOVEMGR->SetLookatPos(this, &pAttacker->GetCurPosition(), 0, gCurTime);

			if (this == HERO)
			{

				if (m_CharacterInfo.bPeace)
				{
					PEACEWARMGR->ToggleHeroPeace_WarMode();
				}
				else
				{
					PEACEWARMGR->SetCheckTime(gCurTime);
				}
			}
		}

		if (GetState() == eObjectState_Ungijosik && m_ObjectState.bEndState == FALSE &&
			GetEngineObject()->GetCurMotion() != eMotion_Ungijosik_Start)
		{
			ChangeMotion(eMotion_Ungijosik_Damage, FALSE);

			//#ifdef _JAPAN_LOCAL_
			//			if( HERO == this )
			//				UNGIMGR->ToggleHero_UngiMode();
			//#endif
		}
		else if (GetState() == eObjectState_None)
		{
			switch (DamageKind) {
			case eDamageKind_Front:
				ChangeMotion(eMotion_Damage_Front, FALSE);
				break;
			case eDamageKind_Left:
				ChangeMotion(eMotion_Damage_Left, FALSE);
				break;
			case eDamageKind_Right:
				ChangeMotion(eMotion_Damage_Right,FALSE);
				break;
			}
		}

		int DamageEffect = -1;
		switch (DamageKind) {
		case eDamageKind_Front:
		case eDamageKind_Counter:
		case eDamageKind_ContinueDamage:
		{
										   if (InTitan() == TRUE)
											   DamageEffect = eEffect_Titan_Damage1;
										   else
											   DamageEffect = eEffect_Damage1;
		}
			break;
		case eDamageKind_Left:
		{
								 if (InTitan() == TRUE)
									 DamageEffect = eEffect_Titan_Damage2;
								 else
									 DamageEffect = eEffect_Damage2;
		}
			break;
		case eDamageKind_Right:
		{
								  if (InTitan() == TRUE)
									  DamageEffect = eEffect_Titan_Damage3;
								  else
									  DamageEffect = eEffect_Damage3;
		}
			break;
		}
		if (DamageEffect != -1)
		{
			TARGETSET set;
			set.pTarget = this;
			set.ResultInfo.bCritical = bCritical;
			set.ResultInfo.bDecisive = bDecisive;

			EFFECTMGR->StartEffectProcess(DamageEffect, pAttacker, &set, 1, GetID(),
				EFFECT_FLAG_GENDERBASISTARGET);
		}
	}

	if (pAttacker == HERO || this == HERO)
	{
		if (this->InTitan() && titanObserbDamage)
			EFFECTMGR->AddDamageNumber(titanObserbDamage, pAttacker, this, DamageKind, bCritical, bDecisive);
		else
			EFFECTMGR->AddDamageNumber(Damage, pAttacker, this, DamageKind, bCritical, bDecisive);
	}
	else if (pAttacker)
	{
		if (OPTIONMGR->IsShowMemberDamage())
		if (PARTYMGR->IsPartyMember(pAttacker->GetID()))
		{
			EFFECTMGR->AddDamageNumber(Damage, pAttacker, this, DamageKind, bCritical, bDecisive);
		}
	}

	DWORD life = GetLife();
	if (life < Damage)
		life = 0;
	else
		life = life - Damage;
	SetLife(life);

	DWORD Shield = GetShield();
	if (Shield < ShieldDamage)
		Shield = 0;
	else

		Shield = Shield - ShieldDamage;
	SetShield(Shield, FALSE);

	if (this == HERO && InTitan())
	{
		if (titanObserbDamage)
		{
			DWORD fuel = GetCurTitan()->GetLife();
			if (fuel < titanObserbDamage)
				fuel = 0;
			else
				fuel = fuel - titanObserbDamage;

			GetCurTitan()->SetLife(fuel);
		}
	}
}
void CPlayer::SetLife(DWORD life, BYTE type)
{
	m_CharacterInfo.Life = life; 

	
	if(GetState() == eObjectState_Die)
		life = 0;

	if( GAMEIN->GetMonsterGuageDlg())
	{
		if( GAMEIN->GetMonsterGuageDlg()->GetCurMonster() == (CObject*)this )
		{
			GAMEIN->GetMonsterGuageDlg()->SetMonsterLife( life, GetMaxLife() );
		}
	}
}
void CPlayer::SetShield(DWORD Shield, BYTE type)
{ 
	m_CharacterInfo.Shield = Shield; 

	
	if(GetState() == eObjectState_Die)
		Shield = 0;

	if( GAMEIN->GetMonsterGuageDlg() )
	{
		if( GAMEIN->GetMonsterGuageDlg()->GetCurMonster() == (CObject*)this )
		{
			GAMEIN->GetMonsterGuageDlg()->SetMonsterShield( Shield, GetMaxShield());
		}
	}
}

void CPlayer::Heal(CObject* pHealer,BYTE HealKind,DWORD HealVal)
{
	DWORD life = GetLife() + HealVal;
	SetLife(life);
}

void CPlayer::Recharge(CObject* pHealer,BYTE RechargeKind,DWORD RechargeVal)
{
	DWORD mana = GetNaeRyuk() + RechargeVal;
	SetNaeRyuk(mana);
}

void CPlayer::ShowStreetStallTitle( BOOL bShow, char* strTitle, DWORD dwColor )
{
	if( !m_pObjectBalloon )									return;
	if( !(m_bObjectBalloonBits & ObjectBalloon_Title ) )	return;

	m_pObjectBalloon->ShowStreeStallTitle( bShow, strTitle, dwColor );
}

void CPlayer::SetStreetStallTitle( char* strTitle )
{
	if( m_pObjectBalloon )
	if( m_pObjectBalloon->GetSSTitleTip() )
		m_pObjectBalloon->GetSSTitleTip()->SetTitle( strTitle );
	
}

void CPlayer::ShowStreetBuyStallTitle( BOOL bShow, char* strTitle, DWORD dwColor )
{
	if( !m_pObjectBalloon )									return;
	if( !(m_bObjectBalloonBits & ObjectBalloon_Title ) )	return;
	
	m_pObjectBalloon->ShowStreeBuyStallTitle( bShow, strTitle, dwColor );
}

void CPlayer::SetStreetBuyStallTitle( char* strTitle )
{
	if( m_pObjectBalloon )
		if( m_pObjectBalloon->GetSBSTitleTip() )
			m_pObjectBalloon->GetSBSTitleTip()->SetTitle( strTitle );
		
}

float CPlayer::DoGetMoveSpeed()
{
	float Speed = 0;
	/*
	if(m_MoveInfo.KyungGongIdx)
	{
		if( this == HERO )
		{
			Speed = MOVEMGR->GetKyungGongSpeed(m_MoveInfo.KyungGongIdx) 
				  + GetAbilityStats()->Kyunggong;
		}
		else
		{
			Speed = MOVEMGR->GetKyungGongSpeed(m_MoveInfo.KyungGongIdx) 
				  + ABILITYMGR->GetAbilityKyungGongSpeed( m_MoveInfo.AbilityKyungGongLevel );

		}
		
		// ¾Æ¹ÙÅ¸ ¾ÆÀÌÅÛ °æ°ø ½ºÇÇµå 
		Speed += GetAvatarOption()->KyunggongSpeed;
		// ºÎÀû °æ°ø µµ »ó½Â
		Speed += GetShopItemStats()->KyungGongSpeed;
	}
	else
	{
		if(m_MoveInfo.MoveMode == eMoveMode_Run)
			Speed = RUNSPEED;
		else
			Speed = WALKSPEED;
	}*/
	//SW070127 Å¸ÀÌÅº
	if( InTitan() )
	{
		if( m_MoveInfo.KyungGongIdx )
		{
			TITANINFO_GRADE* pGradeInfo = GAMERESRCMNGR->GetTitanGradeInfo(m_TitanAppearanceInfo.TitanKind, m_TitanAppearanceInfo.TitanGrade);
			if( !pGradeInfo )
				return TITAN_WALKSPEED;

			WORD klev = 0;		// ½ÅÇà¹éº¯
			if( m_MoveInfo.KyungGongIdx == 2602 )
				klev = 1;		// ÃÊ»óºñ
			else if( m_MoveInfo.KyungGongIdx == 2604 )
				klev = 2;		// 
			Speed = pGradeInfo->KyungGongSpeed[klev];

			// ¾Æ¹ÙÅ¸ ¾ÆÀÌÅÛ °æ°ø ½ºÇÇµå 
			Speed += GetAvatarOption()->KyunggongSpeed;
			// ·ûÖä¼Ó³É
			Speed += GetShopItemStats()->KyungGongSpeed;
			//×°±¸¾õÐÑÇá¹¦¼Ó³É
			Speed += HERO->GetSetItemQualityStats()->KyunggongSpeed;
		}
		else
		{
			if(m_MoveInfo.MoveMode == eMoveMode_Run)
				Speed = GetTitanMoveSpeed();
			else
				Speed =TITAN_WALKSPEED;
		}
	}

	else
	{
			if(m_MoveInfo.KyungGongIdx)
			{
				if( this == HERO )
				{
					Speed = MOVEMGR->GetKyungGongSpeed(m_MoveInfo.KyungGongIdx) 
						+ GetAbilityStats()->Kyunggong;
				}
				else
				{
					Speed = MOVEMGR->GetKyungGongSpeed(m_MoveInfo.KyungGongIdx) 
						+ ABILITYMGR->GetAbilityKyungGongSpeed( m_MoveInfo.AbilityKyungGongLevel );
				}

				// ¾Æ¹ÙÅ¸ ¾ÆÀÌÅÛ °æ°ø ½ºÇÇµå 
				Speed += GetAvatarOption()->KyunggongSpeed;

				// ´Ì¿Í£¨WeaponType == 11£©Ãô½ÝÂß¼­£ºÃ¿3µãMinChub¼Ó1µãÒÆ¶¯ËÙ¶È
				WORD weaponType = GetWeaponEquipType();

				if (weaponType == 11) // ½öµ±´Ì¿ÍÎäÆ÷Ê±ÉúÐ§
				{
					int minChub = HERO->GetMinChub();  // »ñÈ¡´©´Ì£¨Ãô½Ý£©Öµ
					Speed += (float)(minChub / 3);     // Ã¿3µã¼Ó1µãËÙ¶È
				}

		

				if( WEATHERMGR->GetWeatherState() == eWS_Snow )
				{
					if( (GetShopItemStats()->Avatar[eAvatar_Dress] ==  SHOPITEM_COS_CAT_DRESS) ||
						(GetShopItemStats()->Avatar[eAvatar_Dress] ==  SHOPITEM_COS_WEDDING_MAN) ||
						(GetShopItemStats()->Avatar[eAvatar_Dress] ==  SHOPITEM_COS_WEDDING_WOMAN) )
						Speed += 50;
				}
				// ºÎÀû °æ°ø µµ »ó½Â
				Speed += GetShopItemStats()->KyungGongSpeed;
			}
			else
			{
				if(m_MoveInfo.MoveMode == eMoveMode_Run)
					Speed = RUNSPEED;
				else
					Speed = WALKSPEED;
			}
	}
	return Speed;
}

void CPlayer::SetPKMode( BOOL bPKMode )
{
	m_CharacterInfo.bPKMode = bPKMode ? true : false;	
}

DWORD CPlayer::DoGetMaxLife()
{
	return m_CharacterInfo.MaxLife;
}

DWORD CPlayer::DoGetMaxShield()
{
	return m_CharacterInfo.MaxShield;
}

void CPlayer::SetBadFame(FAMETYPE val)
{
	m_CharacterInfo.BadFame = val;
}

void CPlayer::SetShopItemOptionInfo(SHOPITEMOPTION* pInfo)
{
	if(!pInfo)			return;
	memcpy(&m_ShopItemOption, pInfo, sizeof(SHOPITEMOPTION));
}

void CPlayer::SetGuildIdxRank(DWORD GuildIdx, BYTE Rank)
{ 
	m_CharacterInfo.MunpaID = GuildIdx;
	m_CharacterInfo.PositionInMunpa = Rank;
}

void CPlayer::SetGuildMarkName(MARKNAMETYPE MarkName)
{
	m_CharacterInfo.MarkName = MarkName;
}

void CPlayer::SetNickName(char* pNickName)
{
	SafeStrCpy(m_CharacterInfo.NickName, pNickName, MAX_GUILD_NICKNAME+1);
}

MARKNAMETYPE CPlayer::GetGuildMarkName()
{
	return m_CharacterInfo.MarkName;
}

void CPlayer::SetGuildIdx(DWORD GuildIdx)
{
	m_CharacterInfo.MunpaID = GuildIdx;
}

void CPlayer::ClearGuildInfo()
{
	m_CharacterInfo.MunpaID = 0;

	m_CharacterInfo.PositionInMunpa = GUILD_NOTMEMBER;
	m_CharacterInfo.MarkName = 0;
	SafeStrCpy(m_CharacterInfo.NickName, "", MAX_GUILD_NICKNAME+1);
	SafeStrCpy(m_CharacterInfo.GuildName, "", MAX_GUILD_NAME+1);
}

char* CPlayer::GetNickName()
{
	return m_CharacterInfo.NickName;
}

void CPlayer::SetGuildName(char* GuildName)
{
	SafeStrCpy(m_CharacterInfo.GuildName, GuildName, MAX_GUILD_NAME+1);
}

char* CPlayer::GetGuildName()
{
	return m_CharacterInfo.GuildName;
}

void CPlayer::SetPet( CPet* pet )
{
	m_pPet = pet;

//	if(!pet)		return;
//	pet->SetMaster(this);
}

void CPlayer::SetStage( BYTE Stage )
{
	m_CharacterInfo.Stage = Stage;
}
void CPlayer::SetCharChangeInfo( CHARACTERCHANGE_INFO* pInfo )
{
	m_CharacterInfo.Gender = pInfo->Gender;
	m_CharacterInfo.HairType = pInfo->HairType;
	m_CharacterInfo.FaceType = pInfo->FaceType;
	m_CharacterInfo.Height = pInfo->Height;
	m_CharacterInfo.Width = pInfo->Width;

	APPEARANCEMGR->AddCharacterPartChange( GetID() );

}

void CPlayer::SetSiegeName( DWORD NameType )
{
	if( m_pObjectBalloon )
		m_pObjectBalloon->SetSiegeName( NameType, strlen(GetObjectName()) );
}

void CPlayer::CalcAvatarOption()
{
	memset( &m_AvatarOption, 0, sizeof(AVATARITEMOPTION) );

	for(int i=0; i<eAvatar_Max; i++)
	{
		if( m_ShopItemOption.Avatar[i] < 2 )					continue;

		ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo( m_ShopItemOption.Avatar[i] );
		if( !pItemInfo )		continue;

		
		// ±Ù°ñ
		if( pItemInfo->GenGol > 0 )
			m_AvatarOption.Gengol += pItemInfo->GenGol;
		// ¹ÎÃ¸
		if( pItemInfo->MinChub > 0 )
			m_AvatarOption.Minchub += pItemInfo->MinChub;
		// Ã¼·Â
		if( pItemInfo->CheRyuk > 0 )
			m_AvatarOption.Cheryuk += pItemInfo->CheRyuk;
		// ½É¸Æ
		if( pItemInfo->SimMek > 0 )
			m_AvatarOption.Simmek += pItemInfo->SimMek;
		// ÃÖ´ë»ý¸í·Â
		if( pItemInfo->Life > 0 )
			m_AvatarOption.Life += (WORD)pItemInfo->Life;
		// ÃÖ´ëÈ£½Å°­±â
		if( pItemInfo->Shield > 0 )
			m_AvatarOption.Shield += (WORD)pItemInfo->Shield;
		// ÃÖ´ë³»·Â
		if( pItemInfo->NaeRyuk > 0 )
			m_AvatarOption.Naeruyk += pItemInfo->NaeRyuk;
		// ¹«±â°ø°Ý·Â
		if( pItemInfo->MeleeAttackMin > 0 )
			m_AvatarOption.Attack += pItemInfo->MeleeAttackMin;
		// Å©¸®Æ¼ÄÃ¼öÄ¡
		if( pItemInfo->CriticalPercent > 0 )
			m_AvatarOption.Critical += pItemInfo->CriticalPercent;
		// ¹Ý°ÝÈ®À²
		if( pItemInfo->Plus_MugongIdx > 0 )
			m_AvatarOption.CounterPercent += pItemInfo->Plus_MugongIdx;
		// ¹Ý°Ý½Ã µ¥¹ÌÁöÀ²
		if( pItemInfo->Plus_Value > 0 )
			m_AvatarOption.CounterDamage += pItemInfo->Plus_Value;
		// °æ°ø·¹ÀÌ
		if( pItemInfo->AllPlus_Kind == 1 )
			m_AvatarOption.bKyungGong = 1;
		// ³»·Â¼Ò¸ð°¨¼Ò
		if( pItemInfo->LimitCheRyuk > 0 )
			m_AvatarOption.NeaRyukSpend += pItemInfo->LimitCheRyuk;
		// ³»°ø µ¥¹ÌÁö

		if( pItemInfo->LimitJob > 0 )
			m_AvatarOption.NeagongDamage += pItemInfo->LimitJob;
		// ¿Ü°ø µ¥¹ÌÁö
		if( pItemInfo->LimitGender > 0 )
			m_AvatarOption.WoigongDamage += pItemInfo->LimitGender;		
		// ¸ó½ºÅÍ ¹°¸®¹æ¾î·Â °¨¼Ò
		if( pItemInfo->LimitLevel > 0 )
			m_AvatarOption.TargetPhyDefDown += pItemInfo->LimitLevel;
		// ¸ó½ºÅÍ ¼ºÀúÇ×·Â °¨¼Ò
		if( pItemInfo->LimitGenGol > 0 )

			m_AvatarOption.TargetAttrDefDown += pItemInfo->LimitGenGol;
		// ¸ó½ºÅÍ °ø°Ý·Â °¨¼Ò
		if( pItemInfo->LimitMinChub > 0 )
		{
			m_AvatarOption.TargetAtkDown += pItemInfo->LimitMinChub;
			if( m_AvatarOption.TargetAtkDown > 99 )
			{				
				//g_Console.LOG(4, "---%d---%d",pAvatarOption->TargetAtkDown,pItemInfo->LimitMinChub);
				m_AvatarOption.TargetAtkDown = 99;
			}
			//else
				
		}
		// È¸º¹·® Ãß°¡ Áõ°¡Ä¡
		if( pItemInfo->LimitSimMek )
			m_AvatarOption.RecoverRate += pItemInfo->LimitSimMek;
		// °æ°øµµ »ó½Â
		if( pItemInfo->ItemGrade )
			m_AvatarOption.KyunggongSpeed += pItemInfo->ItemGrade;
		// ¹¬È¥°ÔÀÌÁö ÃæÀüµµ
		if( pItemInfo->RangeType )
			m_AvatarOption.MussangCharge += pItemInfo->RangeType;
		// °æ°ø»ç¿ë½Ã ³»·Â¼Ò¸ð¿©ºÎ
		if( pItemInfo->EquipKind == 1 )
			m_AvatarOption.NaeruykspendbyKG = 1;
		//////////////////////////////////////////////////////
		// 06. 07 ³»°ø ÀûÁß(ÀÏ°Ý) - ÀÌ¿µÁØ
		if( pItemInfo->NaeRyukRecover )
			m_AvatarOption.Decisive += pItemInfo->NaeRyukRecover;
		// ¹¬È¥¸ðµå¹ßµ¿½Ã µ¥¹ÌÁöÁõ°¡
        if( pItemInfo->RangeAttackMax > 0 )
			m_AvatarOption.MussangDamage += pItemInfo->RangeAttackMax;
		if (pItemInfo->PVPCri > 0)
			m_AvatarOption.PVPCri += pItemInfo->PVPCri * 100;

		if (pItemInfo->PVPAttack > 0)
			m_AvatarOption.PVPAttack += pItemInfo->PVPAttack * 100;

		if (pItemInfo->PVPDef > 0)
			m_AvatarOption.PVPDef += pItemInfo->PVPDef * 100;

		if (pItemInfo->PVPADef > 0)
			m_AvatarOption.PVPADef += pItemInfo->PVPADef * 100;

		if (pItemInfo->PVPADodge > 0)
			m_AvatarOption.PVPADodge += pItemInfo->PVPADodge * 100;

		if (pItemInfo->PVPHit > 0)
			m_AvatarOption.PVPHit += pItemInfo->PVPHit * 100;

		if (pItemInfo->PVPStunResist > 0)
			m_AvatarOption.PVPStunResist += pItemInfo->PVPStunResist * 100;

		if (pItemInfo->PVPStunTimeReduce > 0)
			m_AvatarOption.PVPStunTimeReduce += pItemInfo->PVPStunTimeReduce * 100;
	}

	// Character Dialog
	if( GetID() == HEROID )
		GAMEIN->GetCharacterDialog()->RefreshInfo();
}

#ifdef _JAPAN_LOCAL_
int CPlayer::WhatIsAttrRelation( int nOtherMainAttr )
{
	if( m_CharacterInfo.nMainAttr == eCA_None || nOtherMainAttr == eCA_None )	return eCAR_None;

	int nRelation = nOtherMainAttr - GetMainCharAttr();
	if( nRelation < 0 )	nRelation += 5;

	return nRelation;
}
#endif // _JAPAN_LOCAL_

//////////////////////////////////////////////////////////////////////////
// 06. 06. 2Â÷ ÀüÁ÷ - ÀÌ¿µÁØ
// ¹«°ø º¯È¯ Ãß°¡
void CPlayer::AddSkillStatsOption(SKILLOPTION *pSkillOption)
{
	m_SkillStatsOption.Life += pSkillOption->Life;
	m_SkillStatsOption.NaeRyuk += pSkillOption->NaeRyuk;
	m_SkillStatsOption.Shield += pSkillOption->Shield;
	m_SkillStatsOption.PhyDef += pSkillOption->PhyDef;
	m_SkillStatsOption.AttDef += pSkillOption->AttDef;

	m_SkillStatsOption.Range += pSkillOption->Range;
	m_SkillStatsOption.ReduceNaeRyuk += pSkillOption->ReduceNaeRyuk;
	m_SkillStatsOption.PhyAtk += pSkillOption->PhyAtk;
	m_SkillStatsOption.BaseAtk += pSkillOption->BaseAtk;
	m_SkillStatsOption.AttAtk += pSkillOption->AttAtk;
}

void CPlayer::RemoveSkillStatsOption(SKILLOPTION *pSkillOption)
{
	m_SkillStatsOption.Life -= pSkillOption->Life;
	m_SkillStatsOption.NaeRyuk -= pSkillOption->NaeRyuk;
	m_SkillStatsOption.Shield -= pSkillOption->Shield;
	m_SkillStatsOption.PhyDef -= pSkillOption->PhyDef;
	m_SkillStatsOption.AttDef -= pSkillOption->AttDef;

	m_SkillStatsOption.Range -= pSkillOption->Range;
	m_SkillStatsOption.ReduceNaeRyuk -= pSkillOption->ReduceNaeRyuk;
	m_SkillStatsOption.PhyAtk -= pSkillOption->PhyAtk;
	m_SkillStatsOption.BaseAtk -= pSkillOption->BaseAtk;
	m_SkillStatsOption.AttAtk -= pSkillOption->AttAtk;
}
//////////////////////////////////////////////////////////////////////////

//SW070127 Å¸ÀÌÅº
void CPlayer::SetTitanAppearanceInfo( TITAN_APPEARANCEINFO* pInfo )
{
	if(pInfo)
	{
		m_TitanAppearanceInfo = *pInfo;

		SetTitanMoveSpeed();
	}
	else
	{
		memset(&m_TitanAppearanceInfo,0,sizeof(TITAN_APPEARANCEINFO));
		memset(&m_TitanPreViewAppInfo,0,sizeof(TITAN_APPEARANCEINFO));		
	}
}

void CPlayer::SetTitanMoveSpeed()
{
	m_TitanMoveSpeed = 0;

	// Å¸ÀÌÅº ÀÌµ¿ µµ ¼³Á¤
	TITAN_APPEARANCEINFO* pAprInfo = NULL;
	if( InTitan() )
		pAprInfo = GetTitanAppearInfo();	//pAprInfo not NULL.member
	else if( IsTitanPreView() )
		pAprInfo = GetTitanPreViewInfo();	//pAprInfo not NULL.member

	if( !pAprInfo )
		return;

	TITANINFO_GRADE* pGradeInfo = GAMERESRCMNGR->GetTitanGradeInfo(pAprInfo->TitanKind, pAprInfo->TitanGrade);
	if(pGradeInfo)
        m_TitanMoveSpeed = (float)pGradeInfo->Speed;

	// ÀåÂøÅÛ ¿É¼ÇºÎºÐ
	for( int i = 0; i < eTitanWearedItem_Max; ++i )
	{
		if(pAprInfo->WearedItemIdx[i])
		{
			ITEM_INFO* pInfo = ITEMMGR->GetItemInfo(pAprInfo->WearedItemIdx[i]);
			if(pInfo)	// magi82 - Titan(070705)
			{
				m_TitanMoveSpeed += pInfo->AllPlus_Value;	//	¹«°ø À¯Çüº° ¹«°ø+ Á¤µµ
			}
		}
	}
}

void CPlayer::SetCurTitan( CTitan* pTitan )
{
	if(this == HERO)
	{
		TITANMGR->SetCurRidingTitan(pTitan);
	}
	
	m_pTitan = pTitan;

	if( pTitan )
		RidingTitan(TRUE);
	else
		RidingTitan(FALSE);
}

void CPlayer::SetTitanPreView( TITAN_APPEARANCEINFO* pInfo )
{
	m_bTitanPreView = TRUE;

	m_TitanPreViewAppInfo = *pInfo;
	APPEARANCEMGR->InitAppearance( this );
}

void CPlayer::ReleaseTitanPreView()
{
	if( m_bTitanPreView )
	{
		m_bTitanPreView = FALSE;
		APPEARANCEMGR->InitAppearance( this );
	}
}

//2007. 8. 8. CBH - ¼¼Æ®¾ÆÀÌÅÆ ÀÌÆåÆ® °ü·Ã Ãß°¡
DWORD CPlayer::GetSetItemEffectID()
{
	return m_dwSetItemEffectID;
}
void CPlayer::SetSetItemEffectID(DWORD dwEffectID)
{
	m_dwSetItemEffectID = dwEffectID;
}

//2008. 1. 21. CBH - ½ºÅ² ·¹ÀÌ °ü·Ã ÇÔ¼ö
void CPlayer::InitSkinDelayTime()
{
	m_dwSkinDelayTime = 0;	//·¹ÀÌ ÃÑ ½Ã°£ º¯¼ö
	m_bSkinDelayResult = FALSE;	//·¹ÀÌÁßÀÎÁö Ã¼Å©ÇÏ´Â º¯¼ö
}

void CPlayer::StartSkinDelayTime()
{
	m_dwSkinDelayTime = gCurTime + 10000;
	m_bSkinDelayResult = TRUE;
}

BOOL CPlayer::CheckSkinDelay()
{
	//·¹ÀÌ Ã³¸®
	if(m_bSkinDelayResult == TRUE)
	{
		DWORD dwTime = m_dwSkinDelayTime - gCurTime;
		if(gCurTime > m_dwSkinDelayTime)
		{
			dwTime = 0;
		}		
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1355), dwTime/1000+1);

		return FALSE;
	}

	return TRUE;
}
//////////////////////////////////////////////////


// magi82(47)
void CPlayer::SetStreetStallBalloonImage( DWORD eKind, BOOL bFlag )
{
	CStreetStallTitleTip* pStallTitle = NULL;

	cImage left1;
	cImage left2;
	cImage left3;
	cImage center1;
	cImage center2;
	cImage center3;
	cImage right1;
	cImage right2;
	cImage right3;

	if( eSK_SELL == eKind )
		pStallTitle = m_pObjectBalloon->GetSSTitleTip();
	else if( eSK_BUY == eKind )
		pStallTitle = m_pObjectBalloon->GetSBSTitleTip();

	if( TRUE == bFlag )
	{
		SCRIPTMGR->GetImage(138, &left1);
		SCRIPTMGR->GetImage(139, &center1);
		SCRIPTMGR->GetImage(140, &right1);
		SCRIPTMGR->GetImage(141, &left2);
		SCRIPTMGR->GetImage(142, &center2);
		SCRIPTMGR->GetImage(143, &right2);
		SCRIPTMGR->GetImage(144, &left3);
		SCRIPTMGR->GetImage(145, &center3);
		SCRIPTMGR->GetImage(146, &right3);	
	}
	else
	{
		if( eSK_SELL == eKind )
		{
			SCRIPTMGR->GetImage(117, &left1);
			SCRIPTMGR->GetImage(118, &center1);
			SCRIPTMGR->GetImage(119, &right1);
			SCRIPTMGR->GetImage(120, &left2);
			SCRIPTMGR->GetImage(121, &center2);
			SCRIPTMGR->GetImage(122, &right2);
			SCRIPTMGR->GetImage(123, &left3);
			SCRIPTMGR->GetImage(124, &center3);
			SCRIPTMGR->GetImage(125, &right3);

		}
		else if( eSK_BUY == eKind )
		{
			SCRIPTMGR->GetImage(128, &left1);
			SCRIPTMGR->GetImage(129, &center1);
			SCRIPTMGR->GetImage(130, &right1);
			SCRIPTMGR->GetImage(131, &left2);
			SCRIPTMGR->GetImage(132, &center2);
			SCRIPTMGR->GetImage(133, &right2);
			SCRIPTMGR->GetImage(134, &left3);
			SCRIPTMGR->GetImage(135, &center3);
			SCRIPTMGR->GetImage(136, &right3);
		}
	}

	pStallTitle->InitBalloonForChat(&left1, &center1, &right1, &left2, &center2, &right2, &left3, &center3, &right3, 8);
}

// 2014-05-09 marry name set

void CPlayer::SetMarryName(char* pName)
{
	if (pName == NULL)

		return;

	SafeStrCpy( m_CharacterInfo.MarryName, pName, MAX_NAME_LENGTH + 1 );
	
	if (IsMarry())
	{
		CHAR balloon[MAX_MASTERNAME_LENGTH + 1];

		if (GetGender() == 0)
		{
			sprintf(balloon, CHATMGR->GetChatMsg(2011), GetMarryName());
		}
		else
		{
			sprintf(balloon, CHATMGR->GetChatMsg(2012), GetMarryName());

		}

		SetMarryNameBalloon(balloon);
	}
	else
	{
		SetMarryNameBalloon(pName);
	}
	
}

char* CPlayer::GetMarryName()
{
	return m_CharacterInfo.MarryName;
}
// 2014-05-15 ShiTu Set 
void CPlayer::SetShiFuName(char* pName)
{
	if (pName == NULL)
		return;

	SafeStrCpy( m_CharacterInfo.ShiTuName, pName, MAX_NAME_LENGTH + 1 );

	if (HasShiFu())
	{
		CHAR balloon[MAX_MASTERNAME_LENGTH + 1];
		sprintf(balloon, CHATMGR->GetChatMsg(2089), GetShiFuName());
		SetShiTuNameBalloon(balloon);
	}
	else
	{
		SetShiTuNameBalloon("");
	}
}
char* CPlayer::GetShiFuName()
{
	return  m_CharacterInfo.ShiTuName;
}
void CPlayer::ClearShiFu(DWORD ShiFuId)
{
	m_CharacterInfo.ShiTuId=ShiFuId;

    ZeroMemory(m_CharacterInfo.ShiTuName,sizeof(m_CharacterInfo.ShiTuName));

	SetShiTuNameBalloon("");
}
#include "FameManager.h"
void CPlayer::SetFame(FAMETYPE val)
{
	m_CharacterInfo.Fame = val;
	if (CPlayer::GetFame() != 0)
	{
		CHAR balloon[MAX_MASTERNAME_LENGTH + 1];
		sprintf(balloon, "%d", CPlayer::GetFame());
		SetFameBalloon(balloon);

		// ¹Ø¼ü¸Ä¶¯£ºÎÞÌõ¼þÉèÖÃ³ÆºÅ£¬²»ÔÙÆÁ±Î¡°½­ºþÐ¡Ïº¡±
		const char* title = FAMEMGR->GetFameNameString(CPlayer::GetFame());
		SetFameRankBalloon(const_cast<char*>(title));
	}
}

void CPlayer::SetKillCount(DWORD val)
{
	m_CharacterInfo.dwKillPlayerTimes = val;

	std::string s = std::to_string(val);
	char* c = strcpy(new char[s.length() + 1], s.c_str());

	SetKillCountBalloon(c);
}

char* CPlayer::TestFameLogoChangeNo(FAMETYPE pFame)
{
	//FAMETYPE pFame=pPlayer->GetFame();
	/*int Val=ITEMMGR->GetFameLogoChange(CPlayer::GetFame());
	if( pFame < ITEMMGR->GetStageLogoVal(Val)->Fame5 )
		return " V";
	else if( pFame < ITEMMGR->GetStageLogoVal(Val)->Fame4 )
		return " IV";
	else if( pFame < ITEMMGR->GetStageLogoVal(Val)->Fame3 )
		return " III";
	else if( pFame < ITEMMGR->GetStageLogoVal(Val)->Fame2 )
		return " II";
	else if( pFame < ITEMMGR->GetStageLogoVal(Val)->Fame1 )
		return " I";*/
	return "";
}
/*void CPlayer::SetFameRank(char* fame)
{
	/*CHAR famerank[MAX_MASTERNAME_LENGTH + 1];
	ZeroMemory(&famerank, sizeof(famerank));
	sprintf(famerank,"%s%s",ITEMMGR->GetFameRankName(this->GetFame()),((CPlayer*)this)->TestFameLogoChangeNo(this->GetFame()));

	SetFameRankBalloon(famerank);
}*/
void CPlayer::SetCustomizingName(char* pName)
{
	if (pName == NULL)
		return;

	SafeStrCpy(m_CharacterInfo.CustomizingName, pName, MAX_NAME_LENGTH + 1);
	SetObjectCustomizingName(pName);
}

void CPlayer::SetVipLevel(WORD VipLevel)
{
	m_CharacterInfo.VipLevel = VipLevel;

	SetObjectVIPImage(VipLevel);
}