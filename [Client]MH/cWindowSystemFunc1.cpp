#include "stdafx.h"
#include "cWindowSystemFunc.h"
#include "WindowIDEnum.h"
#include "interface/cWindowManager.h"
#include "globaleventfunc.h"
#include "GameIn.h"
#include "CharMake.h"
#include "./Audio/MHAudioManager.h"
#include "ChatManager.h"
#include "ObjectManager.h"
#include "InventoryExDialog.h"
#include "PyogukDialog.h"
#include "QuickDialog.h"
#include "MugongDialog.h"
#include "ExchangeDialog.h"
#include "StallKindSelectDlg.h"
#include "StreetStall.h"
#include "MonsterGuageDlg.h"
#include "DebugDlg.h"
#include "ChannelDialog.h"
#include "QuickManager.h"
#include "ItemManager.h"
#include "Item.h"
#include "MugongBase.h"
#include "ExchangeItem.h"
#include "cMsgBox.h"
#include "cImeEx.h"
#include "CharMakeDialog.h"
#include "MainTitle.h"
#include "ServerListDialog.h"
#include "MenuSlotDialog.h"
#include "PKManager.h"
#include "SafeNumberIcon.h"
#ifdef TAIWAN_LOCAL
#include "ChinaAdviceDlg.h"
#endif
#ifdef _KOR_LOCAL_
#include "OptionManager.h"
#endif
#include "ReconnectDialog.h"
#include "RegID.h"
#include "RestPwd.h"
void cbDragDropProcess(LONG curDragX, LONG curDragY, LONG id, LONG beforeDragX, LONG beforeDragY)
{
	cDialog * srcDlg = WINDOWMGR->GetWindowForID(id);
	if(!srcDlg) return;
	ASSERT(srcDlg);
	cDialog* ptDlg = dynamic_cast<cDialog*>(WINDOWMGR->GetWindowForXYExceptIcon(beforeDragX, beforeDragY));
	cDialog* ptardlg = dynamic_cast<cDialog*>(WINDOWMGR->GetWindowForXYExceptIcon(curDragX, curDragY));
	if(ptDlg && !ptardlg)
	{
		if( ptDlg->GetType() == WT_ITEMSHOPDIALOG )
		{
			srcDlg->SetAbsXY((LONG)beforeDragX, (LONG)beforeDragY);
			if(!srcDlg->GetParent())
				return;
			if(!srcDlg->GetParent()->IsActive())
				srcDlg->SetActive(FALSE);
			srcDlg->SetDepend(TRUE);
			WINDOWMGR->SetMouseInputProcessed();
			return;
		}
	}
	if(srcDlg->GetType() & WT_ICON)
	{
		WORD wtype = srcDlg->GetType();
		if( !(wtype == WT_ITEM ||
			wtype == WT_MUGONG ||
			wtype == WT_JINBUB ||
			wtype == WT_QUICKITEM ||
			wtype == WT_EXCHANGEITEM ||
			wtype == WT_STALLITEM ||
			wtype == WT_DEALITEM ||
			wtype == WT_ABILITYICON) )
		{
			ASSERT(0);
		}
		cIcon* pIcon = (cIcon*)srcDlg;
		cDialog * targetdlg = WINDOWMGR->GetWindowForXYExceptIcon(curDragX,curDragY);
		if(targetdlg)
		{
			if( pIcon->CanMoveToDialog(targetdlg) == FALSE )
			{
				WINDOWMGR->MsgBox(MBI_QUEST_NONE, MBT_OK, CHATMGR->GetChatMsg(652));
				srcDlg->SetAbsXY((LONG)beforeDragX, (LONG)beforeDragY);
			}
			else
			{				
				if(!targetdlg->FakeMoveIcon(curDragX,curDragY,(cIcon*)srcDlg))
				{
					srcDlg->SetAbsXY((LONG)beforeDragX, (LONG)beforeDragY);
				}
				switch(targetdlg->GetType())
				{
				case WT_QUICKDIALOG:
					{
						AUDIOMGR->Play(61, HERO);
					}
					break;
				case WT_INVENTORYDIALOG:
					break;
				default:
					{
						AUDIOMGR->Play(58, HERO);
					}
				}				
			}
		}
		else
		{
			if( pIcon->CanDelete() )
			{
				if(GAMEIN->GetGameInInitKind() != eGameInInitKind_SuryunEnter)
				{
					if(!GAMEIN->GetSafeNumberIcon()->GetSafeStatic())
					{
						CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(2599) );
					}
					else
					{
						SendDeleteIconMsg((cIcon*)srcDlg);
					}
				}
			}
			else
				WINDOWMGR->MsgBox(MBI_QUEST_NONE, MBT_OK, CHATMGR->GetChatMsg(652));
			srcDlg->SetAbsXY((LONG)beforeDragX, (LONG)beforeDragY);
		}
		ASSERT(srcDlg->GetParent());
		if(srcDlg->GetParent())
		{
			if(!srcDlg->GetParent()->IsActive())
				srcDlg->SetActive(FALSE);
			srcDlg->SetDepend(TRUE);
		}
		else if( srcDlg->GetType() == WT_QUICKITEM )
		{
			QUICKMGR->ReleaseQuickItem( (CQuickItem*)srcDlg );
		}
		WINDOWMGR->SetMouseInputProcessed();
	}
}
void SendDeleteIconMsg(cIcon * pIcon)
{
	if(pIcon->GetType() == WT_ITEM)
	{
		if( PKMGR->IsPKLooted() ) return;
        if( ITEMMGR->FakeDeleteItem((CItem *)pIcon) == FALSE )
			return;
		if( (((CItem*)pIcon)->GetItemBaseInfo()->ItemStatic == ITEM_STATIC_LUCK))
		{
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(2124) );
			return;
		}
		WINDOWMGR->MsgBox( MBI_DELETEICONCHECK, MBT_YESNO, CHATMGR->GetChatMsg(363), ((CItem *)pIcon)->GetItemInfo()->ItemName );
		GAMEIN->GetInventoryDialog()->SetDisable( TRUE );
		GAMEIN->GetPyogukDialog()->SetDisable( TRUE );
	}
	else if(pIcon->GetType() == WT_QUICKITEM)
	{
		GAMEIN->GetQuickDialog()->FakeDeleteQuickItem( (CQuickItem *)pIcon );
		AUDIOMGR->Play(67, HERO);
	}
	else if(pIcon->GetType() == WT_MUGONG || pIcon->GetType() == WT_JINBUB )
	{
		GAMEIN->GetMugongDialog()->FakeDeleteMugong(((CMugongBase*)pIcon));
		WINDOWMGR->MsgBox(MBI_MUGONGDELETE, MBT_YESNO, CHATMGR->GetChatMsg(190));
		GAMEIN->GetMugongDialog()->SetDisable( TRUE );
	}
	else if( pIcon->GetType() == WT_EXCHANGEITEM )
	{
		GAMEIN->GetExchangeDialog()->FakeDeleteItem(((CExchangeItem*)pIcon)->GetPosition());
		AUDIOMGR->Play(67, HERO);
	}
	else if( pIcon->GetType() == WT_STALLITEM )
	{
		GAMEIN->GetStreetStallDialog()->FakeDeleteItem( ((CExchangeItem*)pIcon)->GetPosition() );
		AUDIOMGR->Play(67, HERO);
	}
	else if( pIcon->GetType() == WT_MENUICON )
	{
		GAMEIN->GetMenuSlotDialog()->FakeDeleteIcon( (CMenuIcon*)pIcon );
		AUDIOMGR->Play(67, HERO);
	}
}
void CreateMainTitle_m()
{
#ifdef _FILE_BIN_
	cWindow* window = WINDOWMGR->GetDlgInfoFromFile("./image/InterfaceScript/IDDlg.bin", "rb");
#else
	cWindow* window = WINDOWMGR->GetDlgInfoFromFile("./image/IDDlg.txt");
#endif
	WINDOWMGR->AddWindow(window);
	cEditBox* pEdit;
	pEdit = (cEditBox*)((cDialog*)window)->GetWindowForID( MT_IDEDITBOX );
	if(GAMERESRCMNGR->m_GameDesc.LimitDay != 0 && GAMERESRCMNGR->m_GameDesc.LimitDay >= GetCurTimeValue())
		pEdit->SetEditText(GAMERESRCMNGR->m_GameDesc.LimitID);
#ifdef _KOR_LOCAL_
	pEdit->SetValidCheck( VCM_DEFAULT );
#else
	pEdit->SetValidCheck( VCM_ID );
#endif
	pEdit = (cEditBox*)((cDialog*)window)->GetWindowForID( MT_PWDEDITBOX );
	if(GAMERESRCMNGR->m_GameDesc.LimitDay != 0 && GAMERESRCMNGR->m_GameDesc.LimitDay >= GetCurTimeValue())
		pEdit->SetEditText(GAMERESRCMNGR->m_GameDesc.LimitPWD);
	pEdit->SetValidCheck( VCM_PASSWORD );
#ifdef _KOR_LOCAL_
	cComboBox* pCombo = (cComboBox*)WINDOWMGR->GetWindowForIDEx(MT_LISTCOMBOBOX);
	pCombo->SelectComboText(OPTIONMGR->GetGameOption()->nLoginCombo);
	pCombo->SetCurSelectedIdx(OPTIONMGR->GetGameOption()->nLoginCombo);	
#endif
#ifndef TAIWAN_LOCAL
#ifdef _FILE_BIN_
	cWindow* window1 = WINDOWMGR->GetDlgInfoFromFile("./image/InterfaceScript/copyright.bin", "rb");
#else
	cWindow* window1 = WINDOWMGR->GetDlgInfoFromFile("./image/copyright.txt");
#endif
	WINDOWMGR->AddWindow(window1);
#else
#ifdef	_FILE_BIN_
	cWindow* window2 = WINDOWMGR->GetDlgInfoFromFile( "./image/CNAdviceDlg.bin", "rb" );
#else
	cWindow* window2 = WINDOWMGR->GetDlgInfoFromFile( "./image/CNAdviceDlg.txt" );
#endif
	((CChinaAdviceDlg*)window2)->Linking();
	WINDOWMGR->AddWindow( window2 );
#endif
#ifdef	_FILE_BIN_
	cWindow* window3 = WINDOWMGR->GetDlgInfoFromFile( "./image/InterfaceScript/ServerListDlg.bin", "rb" );
#else
	cWindow* window3 = WINDOWMGR->GetDlgInfoFromFile( "./image/ServerListDlg.txt" );
#endif
	WINDOWMGR->AddWindow( window3 );
	cWindow* window5 = WINDOWMGR->GetDlgInfoFromFile( "./image/InterfaceScript/IntroReplay.bin", "rb" );
	WINDOWMGR->AddWindow( window5 );
	WINDOWMGR->CreateNumberPadDlg();
	cWindow* window6 = NULL;
	window6 = WINDOWMGR->GetDlgInfoFromFile("./image/InterfaceScript/RegisterDlg.bin", "rb");
	VECTOR2 Pos3;
	//Pos3.x =/*window6->GetAbsX()+*/((float)GAMERESRCMNGR->m_GameDesc.dispInfo.dwWidth-MIDRSLTN_W)/2;
	//Pos3.y =/*window6->GetAbsY()+*/((float)GAMERESRCMNGR->m_GameDesc.dispInfo.dwHeight-MIDRSLTN_H)/2; 
	Pos3.x = ((float)GET_MAINWIN_W / 2 - window->GetWidth() / 2);
	Pos3.y = ((float)GET_MAINWIN_H / 2 - window->GetHeight() / 2);
	window6->SetAbsXY(Pos3.x,Pos3.y);
	WINDOWMGR->AddWindow(window6);
	RegID* pDlg = (RegID*)window6;
	pDlg->Linking();
	GAMEIN->SetRegID( pDlg );
	
}
void NewCreateCharSelect_m()
{
#ifdef _FILE_BIN_
	cWindow * window = WINDOWMGR->GetDlgInfoFromFile("./image/InterfaceScript/CharSelectDlg.bin", "rb");
#else
	cWindow * window = WINDOWMGR->GetDlgInfoFromFile("./image/CharSelectDlg.txt");
#endif
	WINDOWMGR->AddWindow(window);

	cWindow* window7 = NULL;
	window7 = WINDOWMGR->GetDlgInfoFromFile("./image/InterfaceScript/ResetDlg.bin", "rb");
	VECTOR2 Pos5;
	//Pos5.x =/*window7->GetAbsX()+*/((float)GAMERESRCMNGR->m_GameDesc.dispInfo.dwWidth-MIDRSLTN_W)/2;
	//Pos5.y =/*window7->GetAbsY()+*/((float)GAMERESRCMNGR->m_GameDesc.dispInfo.dwHeight-MIDRSLTN_H)/2; 
	Pos5.x = ((float)GET_MAINWIN_W / 2 - window->GetWidth() / 2);
	Pos5.y = ((float)GET_MAINWIN_H / 2 - window->GetHeight() / 2);
	window7->SetAbsXY(Pos5.x,Pos5.y);
	WINDOWMGR->AddWindow(window7);
	RestPwd* pDlg1 = (RestPwd*)window7;
	pDlg1->Linking();
	GAMEIN->SetRestPwdDlg( pDlg1 );
}
void CreateCharMakeDlg()
{
#ifdef _FILE_BIN_
	cWindow * window = WINDOWMGR->GetDlgInfoFromFile("./image/InterfaceScript/CharMakeNewDlg.bin", "rb");
#else
	cWindow * window = WINDOWMGR->GetDlgInfoFromFile("./image/CharMakeNewDlg.txt");
#endif
	VECTOR2 Pos1;
	Pos1.x =window->GetAbsX()+((float)GAMERESRCMNGR->m_GameDesc.dispInfo.dwWidth-MIDRSLTN_W)+5;
	Pos1.y =window->GetAbsY();
	window->SetAbsXY(Pos1.x,Pos1.y);
	WINDOWMGR->AddWindow(window);
	CHARMAKE->SetCharMakeDialog((cCharMakeDlg*)window);
	CHARMAKE->GetCharMakeDialog()->Linking();
	cEditBox* pEdit = (cEditBox*)CHARMAKE->GetCharMakeDialog()->GetWindowForID( CMID_IDEDITBOX );
	pEdit->SetValidCheck( VCM_CHARNAME );
}
void CharMessageDlg_m()
{
}
cWindow * CreateGameLoading_m()
{
#ifdef _FILE_BIN_
	cWindow * window = WINDOWMGR->GetDlgInfoFromFile("./image/InterfaceScript/NewLoadDlg.bin", "rb");
#else
	cWindow * window = WINDOWMGR->GetDlgInfoFromFile("./image/NewLoadDlg.txt");
#endif
	VECTOR2 Pos1;
	Pos1.x =((float)GAMERESRCMNGR->m_GameDesc.dispInfo.dwWidth-window->GetWidth())-2;
	Pos1.y =window->GetAbsY();
	window->SetAbsXY(Pos1.x,Pos1.y);
	WINDOWMGR->AddWindow(window);
	return window;
}
void CreateInitLogo_m()
{
	cWindow * window = WINDOWMGR->GetDlgInfoFromFile("./image/InterfaceScript/InitDlg.bin", "rb");
	if (WINDOWMGR->GetScreenX() != 800 && WINDOWMGR->GetScreenY() != 600)
	{
		VECTOR2 Pos1;
		Pos1.x = WINDOWMGR->GetScreenX() / 2 - window->GetWidth() / 2;
		Pos1.y = WINDOWMGR->GetScreenY() / 2 - window->GetHeight() / 2;
		window->SetAbsXY(Pos1.x, Pos1.y);
	}
	else
	{
		cImageScale scale;
		scale.x = (float)GAMERESRCMNGR->m_GameDesc.dispInfo.dwWidth / MIDRSLTN_W;
		scale.y = (float)GAMERESRCMNGR->m_GameDesc.dispInfo.dwHeight / MIDRSLTN_H;
		window->SetScale(&scale);
	}
	WINDOWMGR->AddWindow(window);
}
void CreateMoneyDlg()
{
#ifdef _FILE_BIN_
	cWindow * window = WINDOWMGR->GetDlgInfoFromFile("./image/moneyDlg.bin", "rb");
#else
	cWindow * window = WINDOWMGR->GetDlgInfoFromFile("./image/moneyDlg.txt");
#endif
	WINDOWMGR->AddWindow(window);
}
void CreateMonsterGuageDlg()
{
#ifdef _FILE_BIN_
	cWindow * window = WINDOWMGR->GetDlgInfoFromFile("./image/InterfaceScript/51.bin", "rb");
#else
	cWindow * window = WINDOWMGR->GetDlgInfoFromFile("./image/51.txt");
#endif
	VECTOR2 Pos1;
	Pos1.x =window->GetAbsX()+((float)GAMERESRCMNGR->m_GameDesc.dispInfo.dwWidth-MIDRSLTN_W)/2;
	Pos1.y =window->GetAbsY();
	window->SetAbsXY(Pos1.x,Pos1.y);
	WINDOWMGR->AddWindow(window);
	GAMEIN->SetMonsterGuageDlg((CMonsterGuageDlg * )window);
	GAMEIN->GetMonsterGuageDlg()->Linking();
}
void CreateChannelDlg()
{
	cWindow * window = WINDOWMGR->GetDlgInfoFromFile("./image/InterfaceScript/Channel.bin", "rb");
	WINDOWMGR->AddWindow(window);
	CChannelDialog* pChannelDlg = (CChannelDialog*)WINDOWMGR->GetWindowForID( CHA_CHANNELDLG );
	if(pChannelDlg)
	{
		pChannelDlg->Linking();
	}
}
void CreateReconnectDlg()
{
	cWindow* window6 = NULL;
	window6 = WINDOWMGR->GetDlgInfoFromFile("./image/InterfaceScript/Reconnect.bin", "rb");
	VECTOR2 Pos3;
	Pos3.x = ((float)GET_MAINWIN_W / 2 - window6->GetWidth() / 2);
	Pos3.y = ((float)GET_MAINWIN_H / 2 - window6->GetHeight() / 2);
	window6->SetAbsXY(Pos3.x, Pos3.y);
	WINDOWMGR->AddWindow(window6);
	CReconnectDialog* pDlg = (CReconnectDialog*)window6;
	pDlg->Linking();
	GAMEIN->SetReconnectDlg(pDlg);
}