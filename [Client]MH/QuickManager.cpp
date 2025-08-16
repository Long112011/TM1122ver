







// QuickManager.cpp: implementation of the CQuickManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "QuickManager.h"
#include "ObjectManager.h"

#include "QuickDialog.h"
#include "InventoryExDialog.h"

#include "GameIn.h"
#include "QuickItem.h"
#include "./Interface/cWindowManager.h"
#include "./Interface/cScriptManager.h"
#include "ItemManager.h"
#include "MugongManager.h"
#include "SkillManager_Client.h"
#include "./Interface/cIcon.h"
#include "./Input/UserInput.h"
#include "ChatManager.h"

#include "AbilityManager.h"
#include "SuryunDialog.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
GLOBALTON(CQuickManager);

// 06. 01 ?žåº·??- çªœç»µ???½ç?
//------------------------------------------------------
// ?žå™¨?¤è®° æ¥·é? çªƒè???
void CQuickManager::AddQuickPosition(POSTYPE& QuickPos, WORD SheetNum, WORD Pos)
{
	static WORD ADD_QUICK[4][10] = { { 0x0001, 0x0002, 0x0003, 0x0004, 0x0005, 0x0006, 0x0007, 0x0008, 0x0009, 0x000a },
		 							 { 0x0010, 0x0020, 0x0030, 0x0040, 0x0050, 0x0060, 0x0070, 0x0080, 0x0090, 0x00a0 },
									 { 0x0100, 0x0200, 0x0300, 0x0400, 0x0500, 0x0600, 0x0700, 0x0800, 0x0900, 0x0a00 },
									 { 0x1000, 0x2000, 0x3000, 0x4000, 0x5000, 0x6000, 0x7000, 0x8000, 0x9000, 0xa000 } };

	if( Pos <= 10 || Pos > 0 )
		QuickPos = QuickPos | ADD_QUICK[SheetNum][Pos - 1];
}

void CQuickManager::DeleteQuickPosition(POSTYPE& QuickPos, WORD SheetNum)
{
	static WORD DELETE_QUICK[4] = { 0xfff0, 0xff0f, 0xf0ff, 0x0fff };

	QuickPos = QuickPos & DELETE_QUICK[SheetNum];
}

WORD CQuickManager::GetQuickPosition(POSTYPE QuickPos, WORD SheetNum)
{
	static WORD	GET_QUICK[4] = { 0x000f, 0x00f0, 0x0f00, 0xf000 };

	return QuickPos & GET_QUICK[SheetNum];
}

void CQuickManager::AnalyzeQuickPosition(POSTYPE QuickPos, WORD* QuickPosList)
{
	for(WORD count = 0; count < 4; count++)
	{
		QuickPosList[count] = GetQuickPosition( QuickPos, count ) >> ( count * 4 );
	}
}

void CQuickManager::UpdateQuickPosition(POSTYPE& QuickPos, WORD SheetNum, WORD Pos)
{
	if(SheetNum >= 4 && SheetNum < 0)
		return;

	DeleteQuickPosition( QuickPos , SheetNum );

	if(Pos <= 10 && Pos > 0)
		AddQuickPosition( QuickPos, SheetNum, Pos );
}

POSTYPE CQuickManager::MergeQuickPosition(WORD* QuickPosList)
{
	POSTYPE rt = 0;

	for(WORD SheetNum = 0; SheetNum < 4; SheetNum++)
	{
		if(QuickPosList[SheetNum] == 0)
			continue;

		AddQuickPosition(rt, SheetNum, QuickPosList[SheetNum]);
	}


	return rt;

}

void CQuickManager::AddAbilityQuickPosition(BYTE Kind, BYTE Pos, POSTYPE QuickPos, ABILITY_TOTALINFO* pOutAbilityTotalInfo)
{
	WORD QuickPosList[4] = {0,};

	AnalyzeQuickPosition(QuickPos, QuickPosList);

	for(WORD SheetNum = 0; SheetNum < 4; SheetNum++)
	{
		if(QuickPosList[SheetNum] == 0)
			continue;

		WORD PosNum = (QuickPosList[SheetNum] - 1) * 2;
		pOutAbilityTotalInfo->AbilityQuickPositionArray[SheetNum][PosNum] = Kind + ABILITYQUICKPOSITION_KEY;
		pOutAbilityTotalInfo->AbilityQuickPositionArray[SheetNum][PosNum + 1] = Pos + ABILITYQUICKPOSITION_KEY;
	}
}

POSTYPE CQuickManager::GetAbilityQuickPosition(BYTE Kind, BYTE Pos, ABILITY_TOTALINFO* pOutAbilityTotalInfo)
{
	WORD QuickPosList[4] = {0,};

	for(WORD SheetNum = 0; SheetNum < 4; SheetNum++)
	{
		for(WORD Num = 0; Num < 10; Num++)
		{
			WORD PosNum = Num * 2;


			if( ( pOutAbilityTotalInfo->AbilityQuickPositionArray[SheetNum][PosNum] == Kind + ABILITYQUICKPOSITION_KEY ) &&
				( pOutAbilityTotalInfo->AbilityQuickPositionArray[SheetNum][PosNum + 1] == Pos + ABILITYQUICKPOSITION_KEY ) )
			{
				QuickPosList[SheetNum] = Num + 1;
				break;
			}
		}
	}	

	return MergeQuickPosition(QuickPosList);
}
//------------------------------------------------------


CQuickManager::CQuickManager()
{
	m_IconIndexCreator.Init(MAX_ITEMICON_NUM, IG_QUICKITEM_STARTINDEX);
	m_QuickHashForID.Initialize(100);
	//m_selectedQuickAbsPos = 0; m_selectedQuickAbsPos??æ­¹æ????¤ä¾©?‘ç?
}

CQuickManager::~CQuickManager()
{
	Release();
	m_IconIndexCreator.Release();
}
//!---------------------------------------
// å®¶èƒ¶ ?’æ? ?…å? ?æ²¥ &
// çªœç»µ???’æ? ?å?
void CQuickManager::RemoveQuickItemReal(POSTYPE SrcPos, WORD wSrcItemIdx, POSTYPE QuickPos)
{

	// ?ªæ‰¼?¢æ”«é£??’æ?è¢?QPOSITIONæ¡??½ç?
	// ?ªæ‰¼?¢æ”«é£??’æ?è¢??¬å? QPOSITIONæ¡??½ç?ç§?!
	//POSTYPE QuickPos = 0;
	POSTYPE SrcQuickPos = 0;
	WORD SheetNum = QuickPos / TABCELL_QUICK_NUM;

	switch(GetTableIdxPosition(SrcPos))
	{
	case eItemTable_Inventory:
	case eItemTable_Weared:
	case eItemTable_ShopInven:
		{
			CItem * item = GAMEIN->GetInventoryDialog()->GetItemForPos(SrcPos);
			ASSERT(item);
			// 06. 01 ?žåº·??- çªœç»µ???½ç?
			// ç§¦å¯¸ ?’æ?è¢ç‹¼ ?žå™¨?¤è®° ?¼é? ?Šå?å®¢è? ?¤å†µ?·ä? ?žå™¨?¤è®°?¼çˆ¶ ?¤å†µè¾??¸æ³¼?„ä?
			//QuickPos = item->GetQuickPosition();
			//item->SetQuickPosition(0);
			SrcQuickPos = item->GetQuickPosition();
			DeleteQuickPosition(SrcQuickPos, SheetNum);
			item->SetQuickPosition(SrcQuickPos);
		}
		break;
	case eItemTable_Mugong:
		{
			CMugongBase * pMugong = GAMEIN->GetMugongDialog()->GetMugongAbs(SrcPos);
			ASSERT(pMugong);
			// 06. 01 ?žåº·??- çªœç»µ???½ç?
			// ç§¦å¯¸ ?¬å????žå™¨?¤è®° ?¼é? ?Šå?å®¢è? ?¤å†µ?·ä? ?žå™¨?¤è®°?¼çˆ¶ ?¤å†µè¾??¸æ³¼?„ä?
			//QuickPos = pMugong->GetQuickPosition();
			//pMugong->SetQuickPosition(0);
			SrcQuickPos = pMugong->GetQuickPosition();
			DeleteQuickPosition(SrcQuickPos, SheetNum);
			pMugong->SetQuickPosition(SrcQuickPos);
		}
		break;
	case eItemTable_Ability:
		{
			CAbility* pAbility = HERO->GetAbilityGroup()->GetAbility( wSrcItemIdx - MIN_ABILITY_INDEX );
			// 06. 01 ?žåº·??- çªœç»µ???½ç?
			// ç§¦å¯¸ ç»¢å‘¼åºœè????žå™¨?¤è®° ?¼é? ?Šå?å®¢è? ?¤å†µ?·ä? ?žå™¨?¤è®°?¼çˆ¶ ?¤å†µè¾??¸æ³¼?„ä?
			//QuickPos = pAbility->GetQuickPosition();
			//ABILITYMGR->SetAbilityQuickPosition( wSrcItemIdx - MIN_ABILITY_INDEX, 0, HERO->GetAbilityGroup() );
			SrcQuickPos = pAbility->GetQuickPosition();
			DeleteQuickPosition(SrcQuickPos, SheetNum);
			ABILITYMGR->SetAbilityQuickPosition( wSrcItemIdx - MIN_ABILITY_INDEX, SrcQuickPos, HERO->GetAbilityGroup() );
		}
		break;
	default:
		{
			ASSERT(0);
		}
	}

	CQuickItem * quickOut = NULL;
	GAMEIN->GetQuickDialog()->RemQuickItem(QuickPos, (cIcon**)&quickOut);
	if(quickOut)
		ReleaseQuickItem(quickOut);

	//RemQuickItem(QuickPos);
}
//!---------------------------------------
// çªœç»µ???’æ????å? &
// ?’æ? ?‹è?åº?ç§¦å? =>?¯å?ä»?

void CQuickManager::RemQuickItem(POSTYPE QuickPos)
{
	WORD QuickPosList[4] = {0,};

	AnalyzeQuickPosition(QuickPos, QuickPosList);

	for(WORD SheetNum = 0; SheetNum < 4; SheetNum++)
	{
		if(QuickPosList[SheetNum])
		{
			WORD pos = SheetNum * TABCELL_QUICK_NUM + QuickPosList[SheetNum] - 1;
			
			CQuickItem * quickOut = NULL;
			GAMEIN->GetQuickDialog()->RemQuickItem(pos, (cIcon**)&quickOut);
			ReleaseQuickItem(quickOut);
		}
	}

	/*
	CQuickItem * quickOut = NULL;
	GAMEIN->GetQuickDialog()->RemQuickItem(QuickPos, (cIcon**)&quickOut);
	ReleaseQuickItem(quickOut);
	*/
}

void CQuickManager::OverlapCheck(POSTYPE SrcPos)
{
	POSTYPE QuickPos = 0;


	switch(GetTableIdxPosition(SrcPos))
	{
	case eItemTable_Inventory:
	case eItemTable_Weared:
	case eItemTable_ShopInven:
		{
			CItem * item = GAMEIN->GetInventoryDialog()->GetItemForPos(SrcPos);
			if( !item )
			{
				ASSERT(0);
				return;
			}

			QuickPos = item->GetQuickPosition();
			if( QuickPos == 0 ) return;
		}
		break;
	case eItemTable_Mugong:
		{
			CMugongBase * pMugong = GAMEIN->GetMugongDialog()->GetMugongAbs(SrcPos);
			if( !pMugong )
			{
				ASSERT(0);
				return;
			}

			QuickPos = pMugong->GetQuickPosition();
			if( QuickPos == 0 ) return;
		}
		break;
	case eItemTable_Ability:
		{
			CAbilityIcon* pAbilityIcon = GAMEIN->GetSuryunDialog()->GetAbilityIconAbs(SrcPos);

			if( !pAbilityIcon )
			{
				ASSERT(0);
				return;
			}


			QuickPos = pAbilityIcon->GetQuickPosition();
			if( QuickPos == 0 ) return;
		}
		break;
	default:
		{
			ASSERT(0);
		}
	}
	
	CQuickItem * quickOut = NULL;
	GAMEIN->GetQuickDialog()->RemQuickItem(QuickPos, (cIcon**)&quickOut);
	if(quickOut)
		ReleaseQuickItem(quickOut);
}

void CQuickManager::MoveQuickItemReal( POSTYPE FromSrcPos, WORD wFromSrcItemIdx, POSTYPE FromQuickPos, POSTYPE ToSrcPos, WORD wToSrcItemIdx, POSTYPE ToQuickPos, bool DeleteToQuickPos)
{
	// ä¿ŠçŸ¾ ?‰å?!!

	GAMEIN->GetQuickDialog()->MoveQuickItem(FromQuickPos, ToQuickPos, DeleteToQuickPos);
}
void CQuickManager::NetworkMsgParse(BYTE Protocol,void* pMsg)
{
	switch(Protocol)
	{
	case MP_QUICK_SET_ACK:
		{
			MSG_QUICK_SET_ACK * msg = (MSG_QUICK_SET_ACK *)pMsg;
			SetQuickItemReal(msg->QuickPos, msg->SrcPos, msg->SrcItemIdx);
		}
		break;
	case MP_QUICK_ADD_ACK:
		{
			MSG_QUICK_ADD_ACK * msg = (MSG_QUICK_ADD_ACK *)pMsg;
			//æ£µå‰¯å¦¨ç»° ?’æ?è¢ç‹¼ ?žå? ?žé??žè???ä¹æ????¤æ?
			// 06. 01 ?žåº·??- çªœç»µ???½ç?
			// ?¤ä¾©çªç˜¤ ?¼ç»°ä¿?
			//OverlapCheck(msg->SrcPos); 
			
			AddQuickItemReal(msg->QuickPos, msg->SrcPos, msg->SrcItemIdx, msg->OldSrcPos, msg->OldSrcItemIdx);
		}
		break;
	case MP_QUICK_MOVE_ACK:
		{
			MSG_QUICK_MOVE_ACK * msg = (MSG_QUICK_MOVE_ACK *)pMsg;
			MoveQuickItemReal( msg->FromSrcPos, msg->wFromSrcItemIdx, msg->FromQuickPos, msg->ToSrcPos, msg->wToSrcItemIdx, msg->ToQuickPos, msg->DeleteToSrcQuickPos);
		}
		break;
	case MP_QUICK_REM_ACK:
		{
			MSG_QUICK_REM_ACK * pmsg = (MSG_QUICK_REM_ACK *)pMsg;
			RemoveQuickItemReal(pmsg->SrcPos, pmsg->SrcItemIdx, pmsg->QuickPos);
		}
		break;
	case MP_QUICK_ADD_NACK:
		{
			MSG_WORD * msg = (MSG_WORD *)pMsg;
			//?²æ³¼?‹æ??¤è‚º è´¸å? confirm
			ASSERTMSG(0, "çªœç»µ?’ä? ?æ??»èŠ­???¤ä¾©ä¸”è? ç»ç»° ?’æ?è¢æ??ªä?.");
		}
		break;
	case MP_QUICK_REM_NACK:
		{
			ASSERT(0);
		}
		break;
	case MP_QUICK_MOVE_NACK:// not used
		{
			ASSERT(0);
		}
		break;
	}
}
#include "AdPointDlg.h"
#include "NameChangeDialog.h"
void CQuickManager::UseQuickItem(POSTYPE pos)
{
	if( !HERO ) return;
	CQuickDialog* pQuickDlg = GAMEIN->GetQuickDialog();
	if( !pQuickDlg ) return;
	pQuickDlg->OnSelectAction( pos );
	int selectedQuickAbsPos = pQuickDlg->GetSelectedPage() * TABCELL_QUICK_NUM + pos;
	CQuickItem * pQuickItem = pQuickDlg->GetQuickItem(selectedQuickAbsPos);
	if(!pQuickItem) return;
	switch(GetTableIdxPosition(pQuickItem->GetSrcPosition()))
	{
	case eItemTable_Inventory:
	case eItemTable_Weared:
	case eItemTable_ShopInven:
		{
			CItem * pItem = GAMEIN->GetInventoryDialog()->GetItemForPos(pQuickItem->GetSrcPosition());
			if(pItem)
			{
				if( pItem->GetItemIdx() == eIncantation_ChangeName || pItem->GetItemIdx() == 59707 )
				{
					ITEM_INFO * pInfo = ITEMMGR->GetItemInfo(pItem->GetItemIdx());
					GAMEIN->GetNameChangeDlg()->SetItemFlag( pInfo->SellPrice );
					GAMEIN->GetNameChangeDlg()->SetItemDBIdx( pItem->GetDBIdx() );
				}
				else if( pItem->GetItemIdx() == eIncantation_StatusFastAdd )
				{
					ADDPOINTDLG->SetItemDBIdx( pItem->GetDBIdx() );
				}
				GAMEIN->GetInventoryDialog()->UseItem(pItem);
			}
			return;
		}
		break;
	case eItemTable_Mugong:
		{			
			if( HERO->GetCharacterTotalInfo()->bVisible == FALSE ) return;	
			CActionTarget Target;
			CSkillInfo* pInfo = NULL;
			pInfo = SKILLMGR->GetSkillInfo(pQuickItem->GetSrcIdx());
			if( !pInfo )				return;
			WORD wSkillKind = pInfo->GetSkillKind();
			if( (HERO->InTitan()) && (SKILLMGR->CheckSkillKind(wSkillKind) == FALSE) )
			{
				pInfo = SKILLMGR->GetSkillInfo(pInfo->GetSkillIndex());
			}
			if( pInfo->GetSkillInfo()->TargetKind == 0)			
			{
				CObject * targetObj = OBJECTMGR->GetSelectedObject();
				if( targetObj )
				{
					if( targetObj->IsDied() || targetObj->GetState() == eObjectState_Die )
						return;
					Target.InitActionTarget(targetObj,NULL);
					if( SKILLMGR->CheckSkillKind(wSkillKind) == TRUE )
					{
						HERO->DisableAutoAttack();
					}
				}
				else
				{
					CHATMGR->AddMsgFade( CTC_SYSMSG, CHATMGR->GetChatMsg(400) );//CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(400) );
					return;
				}
			}
			else if(pInfo && pInfo->GetSkillInfo()->TargetKind == 1)		
			{
				Target.InitActionTarget(HERO,NULL);		
			}
			if(pInfo && !pInfo->GetSpecialState())
			{
				if(HERO->GetSingleSpecialState(eSingleSpecialState_Hide))
				{
					CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1329));
					return;
				}
			}
			SKILLMGR->SetSkill(pInfo);	
			SKILLMGR->OnSkillCommand( HERO, &Target, TRUE );			
		}
		break;
	case eItemTable_Ability:
		{
			CAbility* pAbility = HERO->GetAbilityGroup()->GetAbility( pQuickItem->GetSrcIdx() - MIN_ABILITY_INDEX );
			if( pAbility == NULL )
				return ;
			pAbility->Use();
		}
		break;
	default:
		{
			ASSERT(0);
		}
	}
}
void CQuickManager::UseQuickItem(int PageIdx,POSTYPE pos)
{
	if( !HERO ) return;
	CQuickDialog* pQuickDlg = GAMEIN->GetQuickDialog();
	if( !pQuickDlg ) return;
	int selectedQuickAbsPos = PageIdx * TABCELL_QUICK_NUM + pos;  
	CQuickItem * pQuickItem = pQuickDlg->GetQuickItem(selectedQuickAbsPos);
	if(!pQuickItem) return;
	switch(GetTableIdxPosition(pQuickItem->GetSrcPosition()))
	{
	case eItemTable_Inventory:
	case eItemTable_Weared:
	case eItemTable_ShopInven:
		{
			CItem * pItem = GAMEIN->GetInventoryDialog()->GetItemForPos(pQuickItem->GetSrcPosition());
			GAMEIN->GetInventoryDialog()->UseItem(pItem);
			return;
		}
		break;
	case eItemTable_Mugong:
		{			
			if( HERO->GetCharacterTotalInfo()->bVisible == FALSE ) return;	
			CActionTarget Target;
			CSkillInfo* pInfo = NULL;
			pInfo = SKILLMGR->GetSkillInfo(pQuickItem->GetSrcIdx());
			if( !pInfo )				return;
			WORD wSkillKind = pInfo->GetSkillKind();
			if( (HERO->InTitan()) && (SKILLMGR->CheckSkillKind(wSkillKind) == FALSE) )
			{
				pInfo = SKILLMGR->GetSkillInfo(pInfo->GetSkillIndex());
			}
			if( pInfo->GetSkillInfo()->TargetKind == 0)			
			{
				CObject * targetObj = OBJECTMGR->GetSelectedObject();
				if( targetObj )
				{
					if( targetObj->IsDied() || targetObj->GetState() == eObjectState_Die )
						return;
					Target.InitActionTarget(targetObj,NULL);
					if( SKILLMGR->CheckSkillKind(wSkillKind) == TRUE )
					{
						HERO->DisableAutoAttack();
					}
				}
				else
				{
					CHATMGR->AddMsgFade( CTC_SYSMSG, CHATMGR->GetChatMsg(400) );//CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(400) );
					return;
				}
			}
			else if(pInfo && pInfo->GetSkillInfo()->TargetKind == 1)		
			{
				Target.InitActionTarget(HERO,NULL);		
			}
			if(pInfo && !pInfo->GetSpecialState())
			{
				if(HERO->GetSingleSpecialState(eSingleSpecialState_Hide))
				{
					CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1329));
					return;
				}
			}
			SKILLMGR->SetSkill(pInfo);
			//if(IsMagicSkill(pInfo->GetSkillIndex()))
			//	m_UseSkillTime=1300;
			//else
				m_UseSkillTime=pInfo->GetSkillOperateAnimationTime(HERO->GetGender())+200;   
			SKILLMGR->OnSkillCommand( HERO, &Target, TRUE );			
		}
		break;
	case eItemTable_Ability:
		{
			CAbility* pAbility = HERO->GetAbilityGroup()->GetAbility( pQuickItem->GetSrcIdx() - MIN_ABILITY_INDEX );
			if( pAbility == NULL )
				return ;
			pAbility->Use();
		}
		break;
	default:
		{
			ASSERT(0);
		}
	}
}
bool CQuickManager::IsMagicSkill(int skillidx)
{
	if(skillidx>=700&&skillidx<=1012)
		return TRUE;
	else
		return FALSE;
}
//void CQuickManager::InitQuickItem(POSTYPE QuickPos, POSTYPE SrcPos, WORD SrcItemIdx)
//{

//}
void CQuickManager::SetQuickItem(POSTYPE QuickPos, CItem * pItem)
{
	if( !pItem )		return;
	if( pItem->IsLocked() || pItem->GetItemParam() & ITEM_PARAM_SEAL) return;
	
	if( pItem->GetItemKind() == eYOUNGYAK_ITEM )
	{
		if( !GAMEIN->GetInventoryDialog()->CheckItemLockForItemIdx(pItem->GetItemIdx()) )
			return;
	}

	if(TP_PYOGUK_START <= pItem->GetItemBaseInfo()->Position && pItem->GetItemBaseInfo()->Position < TP_PYOGUK_END)
		return;

	MSG_QUICK_SET_SYN msg;
	msg.Category		= MP_QUICK;
	msg.Protocol		= MP_QUICK_SET_SYN;
	msg.dwObjectID		= HEROID;
	msg.QuickPos		= QuickPos;
	msg.SrcPos			= pItem->GetPosition();

	msg.SrcItemIdx		= pItem->GetItemIdx();

	NETWORK->Send(&msg, sizeof(MSG_QUICK_SET_SYN));
}

// 06. 01 ?žåº·??- çªœç»µ???½ç?
// AddQuickItemReal??è´¸æ?ç½ç»° ?žå™¨?¤è®° ?¼ç”¸??è´¸å?çªæ? å¸®é?ç§¦è? çªå”± ?¶æƒ¦
BOOL CQuickManager::SetQuickItemReal(POSTYPE QuickPos, POSTYPE SrcPos, WORD SrcItemIdx)
{
	CBaseItem * pIcon = NULL;
	switch(GetTableIdxPosition(SrcPos))
	{
	case eItemTable_Inventory:
	case eItemTable_Weared:
	case eItemTable_ShopInven:
		{
			CItem * pItem = GAMEIN->GetInventoryDialog()->GetItemForPos(SrcPos);
			ASSERT(pItem);
			if(pItem == NULL)
				return FALSE;
			
			pItem->SetQuickPosition(QuickPos);
			pIcon = pItem;			
		}
		break;
	case eItemTable_Mugong:
		{
			CMugongBase * pMugong = GAMEIN->GetMugongDialog()->GetMugongAbs(SrcPos);
			ASSERT(pMugong);
			if(!pMugong) return FALSE;
			pMugong->SetQuickPosition(QuickPos);
			pIcon = pMugong;	
		}
		break;
	case eItemTable_Ability:
		{
			CAbilityIcon* pAbilityIcon = GAMEIN->GetSuryunDialog()->GetAbilityIconAbs(SrcPos);
			ASSERT(pAbilityIcon);
			if(!pAbilityIcon) return FALSE;
			ABILITYMGR->SetAbilityQuickPosition(SrcItemIdx - MIN_ABILITY_INDEX,QuickPos,HERO->GetAbilityGroup() );
			pIcon = pAbilityIcon;
		}
		break;
	default:
		{
			return FALSE;
		}
	}

	WORD QuickPosList[4] = {0,};
	
	AnalyzeQuickPosition(QuickPos, QuickPosList);

	for(WORD SheetNum = 0; SheetNum < 4; SheetNum++)
	{
		if(QuickPosList[SheetNum] == 0)
			continue;

		CQuickItem * pNewQItem = NewQuickItem(pIcon);

		WORD pos = SheetNum * TABCELL_QUICK_NUM + QuickPosList[SheetNum] - 1;

		pNewQItem->SetPosition(pos);
		if( !GAMEIN->GetQuickDialog()->AddQuickItem(pos, pNewQItem) )
		{
			GAMEIN->GetQuickDialog()->FakeDeleteQuickItem(pNewQItem);
			ReleaseQuickItem(pNewQItem);
		}

	}

	return TRUE;
}
BOOL CQuickManager::AddQuickItemReal(POSTYPE QuickPos, POSTYPE SrcPos, WORD SrcItemIdx, POSTYPE OldSrcPos, WORD OldSrcItemIdx)
{
//	ASSERT( QuickPos );
//	if( QuickPos == 0 ) return FALSE;

	POSTYPE SrcQuickPos = 0;
	WORD SheetNum = QuickPos / TABCELL_QUICK_NUM;
	WORD PosNum = QuickPos % TABCELL_QUICK_NUM + 1;

	CBaseItem * pIcon = NULL;
	switch(GetTableIdxPosition(SrcPos))
	{
	case eItemTable_Inventory:
	case eItemTable_Weared:
	case eItemTable_ShopInven:
		{
			CItem * pItem = GAMEIN->GetInventoryDialog()->GetItemForPos(SrcPos);
			ASSERT(pItem);
			if(pItem == NULL)
				return FALSE;

			//?ç²®??çªœç»µ?ç”« ?Šå?å®¢è? ? å?ç§¦é?ä¿?
			SrcQuickPos = pItem->GetQuickPosition();
			UpdateQuickPosition(SrcQuickPos, SheetNum, PosNum);

			pItem->SetQuickPosition(SrcQuickPos);
			//pItem->SetQuickPosition(QuickPos);
			pIcon = pItem;			
		}
		break;
	case eItemTable_Mugong:
		{
			CMugongBase * pMugong = GAMEIN->GetMugongDialog()->GetMugongAbs(SrcPos);
			ASSERT(pMugong);
			if(!pMugong) return FALSE;


			//?ç²®??çªœç»µ?ç”« ?Šå?å®¢è? ? å?ç§¦é?ä¿?
			SrcQuickPos = pMugong->GetQuickPosition();
			UpdateQuickPosition(SrcQuickPos, SheetNum, PosNum);

			pMugong->SetQuickPosition(SrcQuickPos);
			//pMugong->SetQuickPosition(QuickPos);
			pIcon = pMugong;	
		}
		break;
	case eItemTable_Ability:

		{
			CAbilityIcon* pAbilityIcon = GAMEIN->GetSuryunDialog()->GetAbilityIconAbs(SrcPos);
			ASSERT(pAbilityIcon);
			if(!pAbilityIcon) return FALSE;

			//?ç²®??çªœç»µ?ç”« ?Šå?å®¢è? ? å?ç§¦é?ä¿?
			SrcQuickPos = pAbilityIcon->GetQuickPosition();
			UpdateQuickPosition(SrcQuickPos, SheetNum, PosNum);

			ABILITYMGR->SetAbilityQuickPosition(SrcItemIdx - MIN_ABILITY_INDEX,SrcQuickPos,HERO->GetAbilityGroup() );
			pIcon = pAbilityIcon;
		}
		break;

	default:
		{
			return FALSE;
		}
	}

	if(OldSrcItemIdx)
	{
		RemoveQuickItemReal(OldSrcPos, OldSrcItemIdx, QuickPos);
	}


	CQuickItem * pNewQItem = NewQuickItem(pIcon);

	pNewQItem->SetPosition(QuickPos);
	GAMEIN->GetQuickDialog()->AddQuickItem(QuickPos, pNewQItem);

/*
	if( GAMEIN->GetQuickDialog()->AddQuickItem(QuickPos, pNewQItem) == FALSE )
	{
		WINDOWMGR->AddListDestroyWindow( pNewQItem );
	}
*/
	return TRUE;
}

CQuickItem * CQuickManager::NewQuickItem(CBaseItem * pSrcIcon)
{
	CQuickItem * pQuickItem;
	pQuickItem = new CQuickItem(pSrcIcon);
	pQuickItem->SetDurability(GAMEIN->GetInventoryDialog()->GetTotalItemDurability(pSrcIcon->GetItemIdx()));

	pQuickItem->Init(0,0,DEFAULT_ICONSIZE,DEFAULT_ICONSIZE, NULL, m_IconIndexCreator.GenerateIndex());
	GetImage(pQuickItem);
	WINDOWMGR->AddWindow(pQuickItem);	

	m_QuickHashForID.Add(pQuickItem, pQuickItem->GetID());

	return pQuickItem;

	/*
	if(pIcon->wIconIdx <= 0) return NULL;

	if(!(item = GetQuickItem(pIcon->dwDBIdx)))
	{	
		
		item = new CQuickItem;
		item->Init()
			
		item->SetIconIdx(pIcon->wIconIdx);				// taiyo modified;
		item->SetDBIdx(pIcon->dwDBIdx);					// å®¶èƒ¶??DB?¢éƒ¸??
		item->SetLinkPosition(pIcon->Position);
		if(GetIconKind(pIcon->wIconIdx) == eITEM_ICON)
		if(GetItemKind(pIcon->wIconIdx) & eYOUNGYAK_ITEM)
		{
			CItem * pitem = GAMEIN->GetInventoryDialog()->GetItemForPos(pIcon->Position);
			item->SetDurability(pitem->GetDurability());
		}
		//item->SetPosition(pIcon->Position);			// å®¶èƒ¶???°æ‘¹
		item->Init(0,0,DEFAULT_ICONSIZE,DEFAULT_ICONSIZE, NULL, m_IconIndexCreator.GenerateIndex());

		GetImage(item);

		//item->SetAlpha(80);
		WINDOWMGR->AddWindow(item);	
	}
*/
}

CQuickItem* CQuickManager::GetQuickItem(DWORD dwDBIdx)
{
	CQuickItem* pItem;
	pItem = m_QuickHashForID.GetData(dwDBIdx);
	return pItem;
}
void CQuickManager::Release()
{
	CQuickItem* pQuick;
	m_QuickHashForID.SetPositionHead();
	while(pQuick = m_QuickHashForID.GetData())
	{
		ReleaseQuickItem(pQuick);
	}
	m_QuickHashForID.RemoveAll();
}
void CQuickManager::RefreshQickItem()
{
	CQuickItem* pQuick;
	m_QuickHashForID.SetPositionHead();
	while(pQuick = m_QuickHashForID.GetData())
	{
		if(!ITEMMGR->IsOptionItem(pQuick->GetSrcIdx(), pQuick->GetSrcDurability()) && ITEMMGR->IsDupItem(pQuick->GetSrcIdx()))
		{
			pQuick->SetDurability(GAMEIN->GetInventoryDialog()->GetTotalItemDurability(pQuick->GetSrcIdx()));
		}
	}
}

void CQuickManager::ReleaseQuickItem(CQuickItem * quick)
{
	if( quick == NULL )
	{
		ASSERT(0);
		return;
	}
	m_QuickHashForID.Remove(quick->GetID());
	m_IconIndexCreator.ReleaseIndex(quick->GetID());
	WINDOWMGR->AddListDestroyWindow( quick );
}

void CQuickManager::GetImage( CQuickItem * pQuickItem )
{
	cImage lowImage;
	cImage highImage;
	
	eQUICKICON_KIND eQK = GetIconKind(pQuickItem->GetSrcIdx());
	if( eQK == eITEM_ICON)
	{
		SCRIPTMGR->GetImage( ITEMMGR->GetItemInfo(pQuickItem->GetSrcIdx())->Image2DNum, &lowImage, PFT_ITEMPATH );

		pQuickItem->SetImage2(&lowImage, NULL);
	}
	else if( eQK == eMUGONG_ICON )
	{
		CSkillInfo* pSkillInfo = SKILLMGR->GetSkillInfo(pQuickItem->GetSrcIdx());
		if(pSkillInfo == NULL)
		{
			ASSERT(0);
			return;
		}

		if( pSkillInfo->GetLowImage() != -1 )
		SCRIPTMGR->GetImage( pSkillInfo->GetLowImage(), &lowImage, PFT_MUGONGPATH );
		if( pSkillInfo->GetHighImage() != -1 )
		SCRIPTMGR->GetImage( pSkillInfo->GetHighImage(), &highImage, PFT_MUGONGPATH );

		pQuickItem->SetImage2(&lowImage, &highImage);
	}
	else if( eQK == eABILITY_ICON )
	{
		CAbility* pAbility = HERO->GetAbilityGroup()->GetAbility( pQuickItem->GetSrcIdx() - MIN_ABILITY_INDEX );
		if( pAbility == NULL )
		{
			ASSERT(0);
			return;

		}
		
		SCRIPTMGR->GetImage( pAbility->GetAbilityInfo()->GetInfo()->Ability_IconIndex, &lowImage, PFT_ABILITYPATH );
		pQuickItem->SetImage2( &lowImage, NULL );	
	}
	else
	{
		ASSERT(0);
	}
}


BOOL CQuickManager::CanEquip(cIcon * pIcon)
{
	if(pIcon->GetType() == WT_QUICKITEM)
	{
		return TRUE;
	}
	else if(pIcon->GetType() == WT_ITEM)
	{
		CItem * pItem = (CItem *)pIcon;
		eITEM_KINDBIT bits = pItem->GetItemKind();
		if(bits & eYOUNGYAK_ITEM)
		{
			return TRUE;
		}
		else if(bits & eMUGONG_ITEM)
		{
			if( ITEMMGR->CanConvertToMugong(pItem->GetItemIdx()) )
				return TRUE;

			else
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(352) );
				return FALSE;
			}
		}
		else if(bits & eEQUIP_ITEM)
		{
			if( ITEMMGR->CanEquip(pItem->GetItemIdx()) )
				return TRUE;
			else
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(352) );
				return FALSE;
			}
				
		}
		else if(bits & eEXTRA_ITEM)
		{
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(352) );
			return FALSE;
		}
		else if(bits & eSHOP_ITEM)
		{
			if(bits==eSHOP_ITEM_CHARM || bits==eSHOP_ITEM_HERB ||
				bits==eSHOP_ITEM_INCANTATION || bits==eSHOP_ITEM_SUNDRIES ||
				bits==eSHOP_ITEM_PET || bits == eSHOP_ITEM_FLGNAME)
				return TRUE;
		}
		else if(bits & eQUEST_ITEM_PET)
		{
			return TRUE;
		}
		// magi82 - Titan(070507)
		else if(bits == eTITAN_ITEM_PAPER)
		{
			return TRUE;
		}
		else if(bits == eCHANGE_ITEM)
		{
			return TRUE;
		}

		/*else if( pItem->GetItemIdx() == eIncantation_ChangeName || pItem->GetItemIdx() == 59707 )
		{
			return FALSE;
		}
		else if( pItem->GetItemIdx() == 59707 )
		{
			return FALSE;
		}
		else if( pItem->GetItemIdx() == eIncantation_StatusFastAdd )
		{
			return FALSE;
		}*/

		else if(bits == eCHANGE_ITEM_LOCK)
		{
			return TRUE;
		}
		
		return FALSE;


	}
	else if(pIcon->GetType() == WT_MUGONG ||
			pIcon->GetType() == WT_JINBUB )
	{
		CMugongBase * pMugong = (CMugongBase *)pIcon;
		CMugongManager::eMUGONG_CANNOT_REASON re;
		re = MUGONGMGR->CanUse(pMugong);
		if( re != CMugongManager::eMCUR_SungZero )
		{
			if( re == CMugongManager::eMCUR_Limit )
			{
				if( SKILLMGR->IsGeukMaSkill( pMugong->m_pSkillInfo->GetSkillIndex() ) ||
					SKILLMGR->IsHwaKungSkill( pMugong->m_pSkillInfo->GetSkillIndex() ) )
					return FALSE;
			}
			return TRUE;
		}
		else
		{
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(623) );
			return FALSE;
		}
	}
	else if(pIcon->GetType() == WT_ABILITYICON)
	{
		CAbilityIcon* pAbil = (CAbilityIcon*)pIcon;
		if(pAbil->GetState() != eAIS_Usable
			|| ( pAbil->GetAbilityInfo()->GetUseType() == eAUT_Passive
				 && pAbil->GetAbilityInfo()->GetKind() != eAGK_Job ) )
		{
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(352) );
			return FALSE;
		}

		return TRUE;
	}

	CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(352) );
	return FALSE;
}


BOOL CQuickManager::CheckQPosForItemIdx( WORD ItemIdx )
{
	CQuickItem* pQuick;
	m_QuickHashForID.SetPositionHead();
	while(pQuick = m_QuickHashForID.GetData())
	{
		if( pQuick->GetSrcIdx() == ItemIdx )
			return FALSE;
	}

	return TRUE;
}



