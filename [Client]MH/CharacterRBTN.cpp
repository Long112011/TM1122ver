#include "stdafx.h"
#include "CharacterRBTN.h"
#include "ObjectManager.h"
#include "ChatManager.h"
#include "GameIn.h"
#include "CharacterDialog.h"
#include "InventoryExDialog.h"
#include "cMsgBox.h"
#include "cWindowManager.h"
#include "cButton.h"
#include "Input/UserInput.h"

#include "ItemManager.h"
#include "OtherCharacterEquip.h"
#include "MiniFriendDialog.h"
#include "ExchangeManager.h"
#include "ShoutDialog.h"
#include "MiniNoteDialog.h"
#include "MoveManager.h"
#include "VimuManager.h"
#include "OtherCharacterInfo.h"
#include "PartyWar.h"
#include "PartyManager.h"
#include "GuildManager.h"
#include "AbilityManager.h"
#include "FriendManager.h"
#include "OtherCharacterAmuletDlg.h"
#include "MHTimeManager.h"
#include "ObjectStateManager.h"
#include "cResourceManager.h"
#include "MiniMarryDialog.h"

#include "SendEquipSelectDialog.h"
#include "MixDialog.h"
#include "..\[CC]MixManager\MixManager.h"

#include "PartyCreateDlg.h"

#include "SafeNumberIcon.h"

#include "input\cIMEWnd.h"

#include "MainGame.h"
#include "DealDialog.h"
#include "GlobalEventFunc.h"
#include ".\rarecreatedialog.h"
#include "ReinforceDlg.h"

#include "DissloveDlg.h"

#include "PyogukDialog.h"
#include "GuildDialog.h"
const DWORD c1 = RGB( 250, 250, 250 );
const DWORD c2 = RGB( 250, 250, 250 );
const DWORD c3 = RGB( 250, 250, 250 );

#define COLOR_MERGE c1,c2,c3
#define BTN_SPACES	19

char LinkFirst[128] = "ItemDBIdx:";
char LinkSecond[8] = "¡ú";
char * CharacterRBTN::ReqSmallChar(int i)
{	
	char Tempmsg[128];
	char buf[8] = {0,};
	SafeStrCpy( buf, &m_cPreWordT[1], 128 );
	wsprintf(Tempmsg,"%s",buf);
	return Tempmsg;
}
CharacterRBTN::CharacterRBTN(void)
{
	//m_type=WT_CHARRBTN_DLG;

	pDlg=NULL;
	bStatic=NULL;
	m_pSelectedObj = NULL;
	SetSelf=-1;
	StoredItemIdx = 0;
	m_cPreWord[0] = 0;
	m_cPreWord[1] = '@';
	m_cPreWord[2] = '#';
	m_cPreWord[3] = '$';
	m_cPreWord[4] = '%';

	m_Height=20;
	m_pSelecItem = NULL;

	//ZeroMemory(RecallMsg, sizeof(RecallMsg));
	//ZeroMemory(bufName, sizeof(bufName));
}
CharacterRBTN::~CharacterRBTN(void)
{
	pDlg=NULL;
	bStatic=NULL;
	SetSelf=-1;
	m_Height=0;
}

void CharacterRBTN::Linking()
{
	pDlg	= WINDOWMGR->GetWindowForID( RCM_DLG );
	bStatic	=( cStatic* )GetWindowForID( RCM_STC_NAME );

	for(int i=0;i<13;++i)
	{
		bButton[i] = ( cButton* )GetWindowForID( RCM_BTN_MENU01+i );
		bButton[i] -> SetActive(false);
	}

	SetSelf=-1;
	if(pDlg->IsActive())
	{
		ITEMMGR->DisableDialog();
		pDlg->SetAlwaysTop(TRUE);
	}
}
void CharacterRBTN::SetCharName(char * buf, DWORD color)
{	
	char Name[MAX_NAME_LENGTH+1] = {0};
	SafeStrCpy(Name, buf, MAX_NAME_LENGTH+1);
	bStatic->SetStaticText( Name );
	bStatic->SetAlign( TXT_CENTER );
	bStatic->SetActive(TRUE);
	bStatic->SetFGColor(color);
}
void CharacterRBTN::Buka()
{	
	if(cDialog* const dialog = WINDOWMGR->GetWindowForID(RCM_DLG))
	{
		SHORT_RECT screenRect = {0};
		WORD value = 0;
		g_pExecutive->GetRenderer()->GetClientRect(&screenRect,&value,&value);			
		cDialog::SetAbsXY(	screenRect.right < MOUSE->GetMouseX() + dialog->GetWidth() ? screenRect.right - dialog->GetWidth() : MOUSE->GetMouseX(),
							screenRect.bottom < MOUSE->GetMouseY() + dialog->GetHeight()+m_Height ? screenRect.bottom - dialog->GetHeight()-m_Height : MOUSE->GetMouseY());
		cDialog::SetActive(TRUE);
	}
	RefreshPosition();
}


DWORD CharacterRBTN::ActionEvent(CMouse *mouseInfo)
{
	DWORD rt = cDialog::ActionEvent(mouseInfo) ;
	if( !rt && ( mouseInfo->LButtonDown() || mouseInfo->RButtonDown() ))
	{
		this->SetActive(FALSE);//Close();
	}
	else if( rt && ( mouseInfo->LButtonUp() || mouseInfo->RButtonDown() ))
	{
		this->SetActive(FALSE);//Close();
	}
	return rt;
}
void CharacterRBTN::OnActionEvent(LONG lId, void * p, DWORD we)
{
	switch( lId )
	{
	case RCM_BTN_MENU01:	ButtonAction01();	break;
	case RCM_BTN_MENU02:	ButtonAction02();	break;
	case RCM_BTN_MENU03:	ButtonAction03();	break;
	case RCM_BTN_MENU04:	ButtonAction04();	break;
	case RCM_BTN_MENU05:	ButtonAction05();	break;
	case RCM_BTN_MENU06:	ButtonAction06();	break;
	case RCM_BTN_MENU07:	ButtonAction07();	break;
	case RCM_BTN_MENU08:	ButtonAction08();	break;
	case RCM_BTN_MENU09:	ButtonAction09();	break;
	case RCM_BTN_MENU10:	ButtonAction10();	break;
	case RCM_BTN_MENU11:	ButtonAction11();	break;
	case RCM_BTN_MENU12:	ButtonAction12();	break;
	case RCM_BTN_MENU13:	ButtonAction13();	break;		
	}
	this->SetActive(FALSE);
}
void CharacterRBTN::RefreshPosition()
{
	float absY=pDlg->GetAbsY();
	int line = 0;
	if(bStatic->IsActive()){bStatic->SetAbsY((LONG)absY+line);line=line+BTN_SPACES;}
	if(bButton[0]->IsActive()){bButton[0]->SetAbsY((LONG)absY+line);line=line+BTN_SPACES;}
	if(bButton[1]->IsActive()){bButton[1]->SetAbsY((LONG)absY+line);line=line+BTN_SPACES;}
	if(bButton[2]->IsActive()){bButton[2]->SetAbsY((LONG)absY+line);line=line+BTN_SPACES;}
	if(bButton[3]->IsActive()){bButton[3]->SetAbsY((LONG)absY+line);line=line+BTN_SPACES;}
	if(bButton[4]->IsActive()){bButton[4]->SetAbsY((LONG)absY+line);line=line+BTN_SPACES;}
	if(bButton[5]->IsActive()){bButton[5]->SetAbsY((LONG)absY+line);line=line+BTN_SPACES;}
	if(bButton[6]->IsActive()){bButton[6]->SetAbsY((LONG)absY+line);line=line+BTN_SPACES;}
	if(bButton[7]->IsActive()){bButton[7]->SetAbsY((LONG)absY+line);line=line+BTN_SPACES;}
	if(bButton[8]->IsActive()){bButton[8]->SetAbsY((LONG)absY+line);line=line+BTN_SPACES;}
	if(bButton[9]->IsActive()){bButton[9]->SetAbsY((LONG)absY+line);line=line+BTN_SPACES;}
	if(bButton[10]->IsActive()){bButton[10]->SetAbsY((LONG)absY+line);line=line+BTN_SPACES;}
	if(bButton[11]->IsActive()){bButton[11]->SetAbsY((LONG)absY+line);line=line+BTN_SPACES;}
	if(bButton[12]->IsActive()){bButton[12]->SetAbsY((LONG)absY+line);line=line+BTN_SPACES;}

	SetTinggi(line-BTN_SPACES);
}
void CharacterRBTN::ShowOtherPlayerMenu( CObject* pObj )
{
	Hide();
	SetSelf=eSelfOtherPlayer;
	m_pSelectedObj = pObj;

	SetCharName(m_pSelectedObj->GetObjectName());

	bButton[0]	->SetText(RESRCMGR->GetMsg(1141),	COLOR_MERGE);bButton[0]->SetActive(TRUE);	//View equipment
	bButton[1]	->SetText(RESRCMGR->GetMsg(1142),	COLOR_MERGE);bButton[1]->SetActive(TRUE);	//Properties
	bButton[2]	->SetText(RESRCMGR->GetMsg(71),		COLOR_MERGE);bButton[2]->SetActive(TRUE);	//Trade request
	bButton[3]	->SetText(RESRCMGR->GetMsg(371),	COLOR_MERGE);bButton[3]->SetActive(TRUE);	//Party invite
	bButton[4]	->SetText(RESRCMGR->GetMsg(392),	COLOR_MERGE);bButton[4]->SetActive(TRUE);	//Guild invite
	bButton[5]	->SetText(RESRCMGR->GetMsg(70),		COLOR_MERGE);bButton[5]->SetActive(TRUE);	//Duel request
	bButton[6]	->SetText(RESRCMGR->GetMsg(342),	COLOR_MERGE);bButton[6]->SetActive(TRUE);	//Friend request
	bButton[7]	->SetText(RESRCMGR->GetMsg(248),	COLOR_MERGE);bButton[7]->SetActive(TRUE);	//Messaging
	bButton[8]	->SetText(RESRCMGR->GetMsg(504),	COLOR_MERGE);bButton[8]->SetActive(TRUE);	//Whisper
	bButton[9]	->SetText(RESRCMGR->GetMsg(1451),	COLOR_MERGE);bButton[9]->SetActive(TRUE);	//Follow Player
	bButton[10]	->SetText(RESRCMGR->GetMsg(1237),	COLOR_MERGE);bButton[10]->SetActive(TRUE);	//Copy Name
	bButton[11]	->SetText(RESRCMGR->GetMsg(1323),	COLOR_MERGE);bButton[11]->SetActive(TRUE);	//Item mall
#ifndef _JSKEN2_//if not jsken //selain daripada jsken gunakan code bawah ni
	if(HERO->GetGender() == 0)
	{
		if(!HERO->IsMarry())
			bButton[12]->SetText(RESRCMGR->GetMsg(1467),COLOR_MERGE);//Proposed Her
		else
			bButton[12]->SetText(RESRCMGR->GetMsg(1468),COLOR_MERGE);//Divorce
		
		bButton[12]->SetActive(TRUE);
	}
#endif//_JSKEN2_
	Buka();
}
void CharacterRBTN::Hide()
{
	m_pSelectedObj = NULL;
	bStatic->SetActive(FALSE);
	bButton[0]->SetActive(FALSE);
	bButton[1]->SetActive(FALSE);
	bButton[2]->SetActive(FALSE);
	bButton[3]->SetActive(FALSE);
	if(bButton[4]->IsDisable())
		bButton[4]->SetDisable(FALSE);
	bButton[4]->SetActive(FALSE);
	bButton[5]->SetActive(FALSE);
	bButton[6]->SetActive(FALSE);
	bButton[7]->SetActive(FALSE);
	bButton[8]->SetActive(FALSE);
	bButton[9]->SetActive(FALSE);
	bButton[10]->SetActive(FALSE);
	bButton[11]->SetActive(FALSE);
	bButton[12]->SetActive(FALSE);
	this->SetActive(false);
}
void CharacterRBTN::SetItemNameExt(DWORD flag)
{
	Hide();
	SetSelf=eSelfItemDelelteSell;
	m_InventoryFlag=flag;

	SetCharName("selected items",0xFFFFC8);

	switch(flag)
	{
	case INV:
		{
			bButton[0]->SetText(CHATMGR->GetChatMsg(632),COLOR_MERGE);
			bButton[0]->SetActive(TRUE);

			CDealDialog* pDlg = NULL ;
			pDlg = GAMEIN->GetDealDialog() ;
			if( !pDlg ) break ;
			if( !pDlg->IsActive() ) break ;

			bButton[1]->SetText(CHATMGR->GetChatMsg(2656),COLOR_MERGE);
			bButton[1]->SetActive(TRUE);
		}
	}
	Buka();
}
void CharacterRBTN::SetItemName(DWORD ItemIdx,DWORD flag)
{	
	Hide();
	SetSelf=eSelfItemName;
	StoredItemIdx=ItemIdx;
	m_InventoryFlag=flag;

	CShoutDialog * mShoutDlg = GAMEIN->GetShoutDlg();	
	
	char emptyline[24] = "--------------------";

	ITEM_INFO * pItem = ITEMMGR->GetItemInfo(StoredItemIdx);
	CItem* Item = GAMEIN->GetInventoryDialog()->GetCurSelectedItem(GetInventoryFlag());
	Item = GAMEIN->GetInventoryDialog()->GetItemForPos((WORD)Item->GetPosition());

	SetCharName(pItem->ItemName,0xFFFFC8);

	//for(int n=INV; n<= SHOPIVN; n++)
	//{
		switch(flag/*==n*/)
		{
		case INV:
		case SHOPIVN:
			{
				bButton[0]->SetText(CHATMGR->GetChatMsg(2560),COLOR_MERGE);
				bButton[0]->SetActive(TRUE);

				const ITEM_MIX_INFO * pMixItemInfo = MIXMGR->GetMixItemInfo(Item->GetItemIdx());
				if( pMixItemInfo )
				{
					bButton[1]->SetText(CHATMGR->GetChatMsg(2561),COLOR_MERGE);
					bButton[1]->SetActive(TRUE);
				}
				else
				{
					bButton[1]->SetActive(false);
				}

				bButton[3]->SetText(CHATMGR->GetChatMsg(2562),COLOR_MERGE);bButton[3]->SetActive(TRUE);
				if(mShoutDlg->IsActive())
				{
					bButton[2]->SetText(CHATMGR->GetChatMsg(2563),COLOR_MERGE);bButton[2]->SetActive(TRUE);
				}				
				else
				{
					bButton[4]->SetText(emptyline,COLOR_MERGE);bButton[4]->SetActive(TRUE);bButton[4]->SetDisable(TRUE);
				}

			
				bButton[5]->SetText(CHATMGR->GetChatMsg(2564),COLOR_MERGE);bButton[5]->SetActive(TRUE);
			

				bButton[6]->SetText(CHATMGR->GetChatMsg(2654),COLOR_MERGE);bButton[6]->SetActive(TRUE);

				Buka();
				break;
			}
		case WEAR:
			{
				bButton[3]->SetText(CHATMGR->GetChatMsg(2562),COLOR_MERGE);bButton[3]->SetActive(TRUE);bButton[3]->SetActive(TRUE);
				if(mShoutDlg->IsActive())
				{
					bButton[2]->SetText(CHATMGR->GetChatMsg(2563),COLOR_MERGE);bButton[2]->SetActive(TRUE);
				}
				Buka();
				break;
			}
		
		case 3:
			{
				//if(flag==3)
				//{
				//	CHATMGR->AddMsg(CTC_SYSMSG,"check RBTN case 3 setitemname flag is %d",flag);
				//	break;
				//}
				bButton[0]->SetText(CHATMGR->GetChatMsg(2560),COLOR_MERGE);
				bButton[0]->SetActive(TRUE);

				const ITEM_MIX_INFO * pMixItemInfo = MIXMGR->GetMixItemInfo(Item->GetItemIdx());
				if( pMixItemInfo )
				{
					bButton[1]->SetText(CHATMGR->GetChatMsg(2561),COLOR_MERGE);
					bButton[1]->SetActive(TRUE);
				}
				else
				{
					bButton[1]->SetActive(false);
				}

				bButton[3]->SetText(CHATMGR->GetChatMsg(2562),COLOR_MERGE);bButton[3]->SetActive(TRUE);
				if(mShoutDlg->IsActive())
				{
					bButton[2]->SetText(CHATMGR->GetChatMsg(2563),COLOR_MERGE);bButton[2]->SetActive(TRUE);
				}				
				else
				{
					bButton[4]->SetText(emptyline,COLOR_MERGE);bButton[4]->SetActive(TRUE);bButton[4]->SetDisable(TRUE);
				}

			
				bButton[5]->SetText(CHATMGR->GetChatMsg(2564),COLOR_MERGE);bButton[5]->SetActive(TRUE);
			

				bButton[6]->SetText(CHATMGR->GetChatMsg(2654),COLOR_MERGE);bButton[6]->SetActive(TRUE);

				Buka();
				break;
			}
		
		}
	//}
}
void CharacterRBTN::RequestMsgForItemLink(DWORD ItemDBIdx)
{
	MSG_DWORD		msg;
	msg.Category	= MP_ITEMEXT;
	msg.Protocol	= MP_ITEMEXT_SELECT_SYN;
	msg.dwObjectID	= HEROID;
	msg.dwData		= ItemDBIdx;
	NETWORK->		Send( &msg, sizeof(msg) );
}
void CharacterRBTN::ShowSelfMenu( CObject* pObj )
{
	Hide();
	SetSelf=eSelfMenu;
	m_pSelectedObj = pObj;

	SetCharName(m_pSelectedObj->GetObjectName());

	bButton[0]->SetText(RESRCMGR->GetMsg(1457),COLOR_MERGE);bButton[0]->SetActive(TRUE);
	bButton[1]->SetText(RESRCMGR->GetMsg(1458),COLOR_MERGE);bButton[1]->SetActive(TRUE);
	bButton[2]->SetText(RESRCMGR->GetMsg(1459),COLOR_MERGE);bButton[2]->SetActive(TRUE);
	bButton[3]->SetText(RESRCMGR->GetMsg(1460),COLOR_MERGE);bButton[3]->SetActive(TRUE);
	bButton[4]->SetText(RESRCMGR->GetMsg(1461),COLOR_MERGE);bButton[4]->SetActive(TRUE);

	if(HERO->IsMarry()&&HERO->GetGender() == 0)
	{
		bButton[12]->SetText(RESRCMGR->GetMsg(1468),COLOR_MERGE);bButton[12]->SetActive(TRUE);
	}
	Buka();
}

void CharacterRBTN::ShowChatMenu(char* buf, BOOL bRecallBtn)
{
	Hide();
	SetSelf=eSelfChatMenu;
	//bufName=buf;
	strcpy(bufName,buf);

	SetCharName(buf);

	
	if (bRecallBtn)
	{
		bButton[4]->SetText("Recall Message", COLOR_MERGE); bButton[4]->SetActive(TRUE);//Recall button
	}
	else
	{
		bButton[0]->SetText(RESRCMGR->GetMsg(504), COLOR_MERGE); bButton[0]->SetActive(TRUE);//whishper
		bButton[1]->SetText(RESRCMGR->GetMsg(342), COLOR_MERGE); bButton[1]->SetActive(TRUE);//friend request
		bButton[2]->SetText(RESRCMGR->GetMsg(248), COLOR_MERGE); bButton[2]->SetActive(TRUE);//messanging
		bButton[3]->SetText(RESRCMGR->GetMsg(1237), COLOR_MERGE); bButton[3]->SetActive(TRUE);//copy name
	}
	Buka();
}




bool CharacterRBTN::ItemCheckExt()
{
	bool eError=true;


	CMixDialog * pMixDlg = (CMixDialog *)GAMEIN->GetMixDialog();
	if(pMixDlg && pMixDlg->IsActive())
	{
		GAMEIN->GetMixDialog()->MixCancelBtn(pMixDlg);
	}

	CDissloveDlg * pDissDlg = (CDissloveDlg *)GAMEIN->GetDissolveDialog();
	if(pDissDlg && pDissDlg->IsActive())
	{
		GAMEIN->GetDissolveDialog()->DissolveCancelBtn(pDissDlg);
	}

	CReinforceDlg * pReinforceDlg = (CReinforceDlg *)GAMEIN->GetReinforceDialog();
	if(pReinforceDlg && pReinforceDlg->IsActive())
	{
		GAMEIN->GetReinforceDialog()->ReinforceCancelBtn(pReinforceDlg);
	}

	CRareCreateDialog * pRareDlg = (CRareCreateDialog *)GAMEIN->GetRareCreateDlg();
	if(pRareDlg && pRareDlg->IsActive())
	{
		GAMEIN->GetRareCreateDlg()->ReleaseItem();
		//GAMEIN->GetRareCreateDlg()->ItemRareCreateNAck();
	}



	return eError;
}


//-------------------------START BUTTON ACTION
void CharacterRBTN::ButtonAction01()
{
	switch(SetSelf)
	{
	case eSelfOtherPlayer:
		{
			cDialog *pDlg = (cDialog*)GAMEIN->GetCharacterEquipInfoDlg();
			CQuipInfoDlg *pDlg2 = (CQuipInfoDlg*)GAMEIN->GetCharacterEquipInfoDlg();
			if (!pDlg)
				return;

			pDlg2->SetChr(m_pSelectedObj->GetObjectName());

			if (pDlg->IsActive())
			{
				pDlg->SetActive(FALSE);
			}
			else
			{
				MSG_DWORD msg;
				msg.Category = MP_ITEM;
				msg.Protocol = MP_ITEM_QUERY_EQUI_SYN;
				msg.dwObjectID = HERO->GetID();
				msg.dwData = m_pSelectedObj->GetID();
				NETWORK->Send(&msg, sizeof(msg));
			}
			break;
		}
	
	
	case eSelfMenu:
		{
			CAbility* pAbility = HERO->GetAbilityGroup()->GetAbility( 401 );
			pAbility->Use();
		}
	break;
	
	
	case eSelfItemName:
		{
			CItem* Item = GAMEIN->GetInventoryDialog()->GetCurSelectedItem(GetInventoryFlag());
			Item = GAMEIN->GetInventoryDialog()->GetItemForPos((WORD)Item->GetPosition());
			if(Item)
			{
				GAMEIN->GetInventoryDialog()->UseItem(Item);
				ITEMMGR->ReenableDialog();
			}
			break;
		}
	
	case eSelfChatMenu:
		{
			CHATMGR->SetWhisperName(GetCharNameForChat());
			CHATMGR->AutoWhisperForRbt();
			break;
		}
	
	case eSelfItemDelelteSell://delete selected item
		{//KEEP IN VIEW //PENTING
			if (GAMEIN->GetInventoryDialog()->GetSeletedItemRbtn() != 0)
			{
				GAMEIN->GetInventoryDialog()->SetDisable(true);
				GAMEIN->GetPyogukDialog()->SetDisable(true);
				GAMEIN->GetGuildDlg()->SetDisable(true);
				WINDOWMGR->MsgBox(MSBOX_CLEARITEM, MBT_YESNO, CHATMGR->GetChatMsg(2315));
			}
			/*CItem* Item = NULL;

			if(!CHARRBTN->ItemCheckExt())
				return;

			for(int i=0;i<=79;i++)
			{
				Item = GAMEIN->GetInventoryDialog()->GetItemForPos(i);
				if(Item && Item->IsLocked())
				{
					MSG_ITEM_DISCARD_SYN msg;
					msg.Category = MP_ITEM;
					msg.Protocol = MP_ITEM_DISCARD_SYN;
					msg.dwObjectID = HEROID;
					msg.TargetPos =Item->GetPosition();
					msg.wItemIdx  =Item->GetItemIdx();
					msg.ItemNum   =Item->GetDurability();
					NETWORK->Send(&msg,sizeof(msg));
				}
			}*/
			//CHATMGR->AddMsg( CTC_SYSMSG, "test rbtn" );
			break;
		}
	}
}
void CharacterRBTN::ButtonAction02()
{
	switch(SetSelf)
	{
	case eSelfOtherPlayer:
		{
			CCharactorInfoDlg * pDlg = (CCharactorInfoDlg *)GAMEIN->GetCharacterInfoDlg();
			if( !pDlg )	return;
			if( pDlg->IsActive() )
			{
				pDlg->SetActive( FALSE );
			}
			else 
			{
				pDlg->ShowDialog((CPlayer*)m_pSelectedObj);
			}
		}
	break;
	case eSelfMenu:
		{
			CAbility* pAbility = HERO->GetAbilityGroup()->GetAbility( 403 );
			pAbility->Use();
			break;
		}
	case eSelfItemName:
		{			
			CItem* Item = GAMEIN->GetInventoryDialog()->GetCurSelectedItem(GetInventoryFlag());
			const ITEM_MIX_INFO * pMixItemInfo = MIXMGR->GetMixItemInfo(Item->GetItemIdx());
			Item = GAMEIN->GetInventoryDialog()->GetItemForPos((WORD)Item->GetPosition());
			if(Item)
			{

				MIXDIALOG->FakeMoveIcon(0,0,Item);
				MIXDIALOG->SetActiveRecursive( TRUE );
				/*BOOL bIsCanMix = FALSE;
				bIsCanMix= ITEMMGR->CanMixItem(Item->GetItemIdx());

				if( Item->IsLocked() ) return;

				if(!ITEMMGR->IsEqualTableIdxForPos(eItemTable_Inventory, Item->GetPosition()))
				{
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 612 ) );
					return ;
				}
				if(ITEMMGR->IsOptionItem(Item->GetItemIdx(), Item->GetDurability()))
				{
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(202) );
					return ;
				}

				//eITEM_KINDBIT bits = Item->GetItemKind();
				//if( !(bits & eYOUNGYAK_ITEM) && !(bits & eMUGONG_ITEM)
				//	&& !(bits & eEQUIP_ITEM) && !(bits & eEXTRA_ITEM) 
				//	&& !(bits & eCHANGE_ITEM) && !(bits & eQUEST_ITEM_START) )
				eITEM_KINDBIT bits = Item->GetItemKind();
				if( !(bits & eYOUNGYAK_ITEM) && !(bits & eMUGONG_ITEM)
					&& !(bits & eEQUIP_ITEM) && !(bits & eEXTRA_ITEM) 
					&& !(bits & eCHANGE_ITEM) && !(bits & eQUEST_ITEM_START)&&
					!bIsCanMix)
				{
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(202) );
					return ;
				}
				if(bits == eEQUIP_ITEM_UNIQUE && GAMERESRCMNGR->GetUniqueItemOptionList(Item->GetItemIdx())->MixFlag == 0)
				{
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1625) );
					return ;
				}
				ITEM_MIX_INFO * pMixItemInfo = MIXMGR->GetMixItemInfo(Item->GetItemIdx());
				if(!pMixItemInfo)
				{
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(202) );
					return ;
				}
				if( GetItemKind(Item->GetItemIdx() ) != eYOUNGYAK_ITEM && 
					GetItemKind(Item->GetItemIdx() ) != eEXTRA_ITEM_JEWEL && 
					Item->GetDurability() > 1 )
				{
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(629) );
					return ;
				}

				Item->SetLock(TRUE);

				MSG_ITEM	msgItem;
				msgItem.Category	= MP_ITEM;
				msgItem.Protocol	= MP_ITEM_MIX_ADDITEM_SYN;
				msgItem.dwObjectID	= HEROID;

				msgItem.ItemInfo	= *Item->GetItemBaseInfo();
				NETWORK->Send( &msgItem, sizeof(msgItem) );

				ITEMMGR->SetDisableDialog(TRUE, eItemTable_Inventory);
				ITEMMGR->SetDisableDialog(TRUE, eItemTable_Pyoguk);
				ITEMMGR->SetDisableDialog(TRUE, eItemTable_MunpaWarehouse);


				if(!GAMEIN->GetMixDialog()->IsEmpty(eMixPos_Basic))
				{
					GAMEIN->GetMixDialog()->Release(eMixBasicRelease);
				}
				GAMEIN->GetMixDialog()->Release(eMixResultRelease);

				GAMEIN->GetMixDialog()->AddVirtualItemWrap(eMixPos_Basic, Item);

				GAMEIN->GetMixDialog()->ViewResultItemList(pMixItemInfo);
				GAMEIN->GetMixDialog()->SetActiveRecursive( TRUE );	*/					
			}
			break;
		}
	
	case eSelfChatMenu:
		{
			GAMEIN->GetMiniFriendDialog()->SetActive(TRUE);
			char Name[MAX_NAME_LENGTH+1] = {0};
			SafeStrCpy(Name, GetCharNameForChat(), MAX_NAME_LENGTH+1);
			GAMEIN->GetMiniFriendDialog()->SetName(Name);
			break;
		}
	
	case eSelfItemDelelteSell:
		{
			CItem* pItem = NULL;
			for(int i=0;i<=79;i++)
			{
				pItem = GAMEIN->GetInventoryDialog()->GetItemForPos(i);
				if(pItem && pItem->IsLocked())
				{
					MSG_ITEM_SELL_SYN			m_sellMsg;
					m_sellMsg.Category			= MP_ITEM;
					m_sellMsg.Protocol			= MP_ITEM_SELL_SYN;
					m_sellMsg.dwObjectID		= HEROID;
					m_sellMsg.TargetPos			= pItem->GetPosition();
					m_sellMsg.SellItemNum		= (WORD)pItem->GetDurability();
					m_sellMsg.wSellItemIdx		= pItem->GetItemIdx();
					m_sellMsg.wDealerIdx		= GAMEIN->GetDealDialog()->GetDealerIdx();
					NETWORK->Send(&m_sellMsg,sizeof(m_sellMsg));				
				}
			}
			break;
		}
	
	}
}
void CharacterRBTN::ButtonAction03()
{
	switch(SetSelf)
	{
	case eSelfOtherPlayer:
		{
			if(!GAMEIN->GetSafeNumberIcon()->GetSafeStatic())
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(2599) );
				return;
			}
			EXCHANGEMGR->ApplyExchange();
		}
	break;
	case eSelfMenu:
		{
			CAbility* pAbility = HERO->GetAbilityGroup()->GetAbility( 406 );
			pAbility->Use();
		}
	break;			
	case eSelfItemName:
		{
			CShoutDialog * mDlg = GAMEIN->GetShoutDlg();
			if(mDlg && mDlg->IsActive())
			{
				CItem *  pItem =  GAMEIN->GetInventoryDialog()->GetCurSelectedItem(GetInventoryFlag());
				if(pItem)
				{   
					ITEM_INFO * pInfo = ITEMMGR->GetItemInfo(StoredItemIdx);
					if(!pInfo) return;

					char Tempmsg[128];
					wsprintf(Tempmsg,"%s%d%s%d",LinkFirst,pItem->GetDBIdx(),LinkSecond,pItem->GetItemIdx());
					mDlg->SetShouMsg(Tempmsg);
					mDlg->SendShoutMsgSyn();
					SetActive(FALSE);
					ITEMMGR->ReenableDialog();
					break;
				}
			}
			else
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(2197));SetActive(FALSE);
				ITEMMGR->ReenableDialog();
				break;
			}
		}
	break;
	case eSelfChatMenu:
		{
			GAMEIN->GetMiniNoteDialog()->SetActive(TRUE);
			char Name[MAX_NAME_LENGTH+1] = {0};
			GAMEIN->GetMiniNoteDialog()->SetMiniNote(GetCharNameForChat(), "", 0);
			GAMEIN->GetMiniNoteDialog()->ShowMiniNoteMode(eMinNoteMode_Write);
		}
	break;
	}
}
void CharacterRBTN::ButtonAction04()
{
	switch(SetSelf)
	{
	case eSelfOtherPlayer://party
		{
			if( HEROID != PARTYMGR->GetMasterID() )
			{
				//CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1725) );
				CPartyCreateDlg* pParty=GAMEIN->GetPartyCreateDialog();
				if(pParty)
				{
					pParty->SetActive(TRUE);
				}
				return;
			}
			if(PARTYMGR->IsProcessing() == TRUE)
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1726) );
				return;
			}
			if( PARTYWAR->IsPartyWar() )
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(885));
				return;
			}
			CObject* pObject = OBJECTMGR->GetSelectedObject();
			if( pObject )
			{
				if( pObject->GetSingleSpecialState(eSingleSpecialState_Hide) )
				{
					CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1329));
					return;
				}
			}
			if(!pObject)
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(18));
				return;
			}
			else 
			{
				if(pObject->GetObjectKind() == eObjectKind_Player)
				{
					PARTYMGR->AddPartyMemberSyn(m_pSelectedObj->GetID());
				}
				else
				{
					CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(18));
				}
			}
			break;
		}
	
	case eSelfMenu:
		{
			MOVEMGR->ToggleHeroMoveMode();
			break;
		}
	
	case eSelfItemName:
		{//ITEMLINK
			//for(int n=INV; n<= SHOPIVN; n++)
			//{
				CItem *  pItem =  GAMEIN->GetInventoryDialog()->GetCurSelectedItem(GetInventoryFlag());
				if(pItem)
				{   
					ITEM_INFO * pInfo = ITEMMGR->GetItemInfo(StoredItemIdx);
					if(!pInfo) return;
					//int i=GAMEIN->GetChatDialog()->GetCurSheetNum();	

					char Tempmsg[128];
					char buf[16];
					strcpy(buf, &m_cPreWord[GAMEIN->GetChatDialog()->GetCurSheetNum()]);
					//char buf[8] = {0,};
					//SafeStrCpy( buf, &m_cPreWord[i], 128 );
					wsprintf(Tempmsg,"%s%s%d%s%d",buf,LinkFirst,pItem->GetDBIdx(),LinkSecond,pItem->GetItemIdx());
					CHATMGR->OnInputChatMsg(Tempmsg);
					SetActive(FALSE);
					ITEMMGR->ReenableDialog();
					break;
				}
			//}
			//break;
						  CHATMGR->AddMsg(CTC_SYSMSG, "test rbtn");
		}
		break;
	
	case eSelfChatMenu:
		{
			if(GAMEIN->CopyToClipboard(GetCharNameForChat(),MAX_NAME_LENGTH))
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(2302) );
			}
			break;
		}
	
	case eSelfItemDelelteSell:
		{
			if( !CIMEWND->IsDocking())
				return;

				//CIMEWND->PasteJack();
			//if(GAMEIN->CopyToClipboard(GetCharNameForChat(),MAX_NAME_LENGTH))
				//CHATMGR->AddMsg( CTC_SYSMSG, "test" );
			break;
		}
	
	}
}
void CharacterRBTN::ButtonAction05()
{
	switch(SetSelf)
	{
	case eSelfOtherPlayer:
		{
			if(HERO->GetGuildMemberRank() < GUILD_VICEMASTER)
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(297) );
				return;
			}
			CObject * targetObj = (CPlayer*)m_pSelectedObj;//OBJECTMGR->GetSelectedObject();
			if(targetObj)
			{
				if(targetObj->GetObjectKind()  == eObjectKind_Player)
				{
					if(((CPlayer*)targetObj)->GetGuildIdx())
					{
						CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(38));
						return;
					}
					else
					{
						GUILDMGR->AddMemberSyn(targetObj->GetID());
					}
				}
			}
			break;
		}
	
	case eSelfMenu:
		{
			CAbility* pAbility = HERO->GetAbilityGroup()->GetAbility( 306 );
			pAbility->Use();
			break;
		}
	
	case eSelfItemName:
		{
			ITEMMGR->ReenableDialog();
			break;
		}	
	case eSelfChatMenu:
		{
			MSG_CHAT_RECALL msgChat;
			msgChat.Category = MP_CHAT;
			msgChat.Protocol = MP_CHAT_RECALL;
			msgChat.dwObjectID = HEROID;
			msgChat.dwSenderID = HEROID;
			SafeStrCpy(msgChat.Msg, RecallMsg, MAX_CHAT_LENGTH + 1);
			SafeStrCpy(msgChat.Name, HERO->GetObjectName(), MAX_NAME_LENGTH + 1);
			NETWORK->Send(&msgChat, msgChat.GetMsgLength());
			//CHATMGR->RemoveMsg(RecallMsg);
			//ZeroMemory(RecallMsg, sizeof(RecallMsg));
			ZeroMemory(RecallMsg, sizeof(RecallMsg));
			break;
		}
	}
}
void CharacterRBTN::ButtonAction06()
{
	switch(SetSelf)
	{
	case eSelfOtherPlayer:
		{
			if(GAMEIN->GetGameInInitKind() == eGameInInitKind_SuryunEnter || GAMEIN->GetGameInInitKind() == eGameInInitKind_EventMapEnter)
				return;
			if(ABILITYMGR->CheckAbilityLevel(ABILITYINDEX_VIMU,HERO->GetAbilityGroup(),1) == FALSE)
			{
				CHATMGR->AddMsg( CTC_ABILITYUPGRADE_FAILED, CHATMGR->GetChatMsg( 139 ));
			}
			else
			{
				VIMUMGR->ApplyVimu();
			}
		}
	break;
	case eSelfItemName:
		{
			CItem *  pItem =  GAMEIN->GetInventoryDialog()->GetCurSelectedItem(GetInventoryFlag());
			
			WINDOWMGR->MsgBox( MSBOX_CLEARITEMRBTN, MBT_YESNO, CHATMGR->GetChatMsg(363), ((CItem *)pItem)->GetItemInfo()->ItemName );
			ITEMMGR->ReenableDialog();
		}
	break;
	}
}
void CharacterRBTN::ButtonAction07()
{
	switch(SetSelf)
	{
	case eSelfItemName:
		{
			for(int i=0;i<=79;i++)
			{
				CItem* pItem = GAMEIN->GetInventoryDialog()->GetItemForPos(i);
				if(pItem)
				{
					pItem->SetLock(TRUE);
				}
			}
		}
		break;
	case eSelfOtherPlayer:
		{
			char Name[MAX_NAME_LENGTH+1] = {0};
			char code[MAX_NAME_LENGTH+1] = {0,};	
			CObject* pTarget = OBJECTMGR->GetSelectedObject();
			if(pTarget && (pTarget->GetObjectKind() == eObjectKind_Player))
			{
				SafeStrCpy(Name, OBJECTMGR->GetSelectedObject()->GetObjectName(), MAX_NAME_LENGTH+1);
				if(strcmp("",Name) == 0)return;							
				char heroname[MAX_NAME_LENGTH+1] = {0,};
				sscanf(HERO->GetObjectName(), "%s", heroname);
				strupr(heroname);
				if(strcmp(heroname, code) == 0)
				{
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(676) );
					return;
				}				
				FRIENDMGR->AddDelFriendSyn(Name, MP_FRIEND_ADD_SYN);
			}
			else
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(18));
				return;
			}
		}
		break;	
	}
}
void CharacterRBTN::ButtonAction08()
{
	char Name[MAX_NAME_LENGTH+1] = {0};
	CObject* pTarget = OBJECTMGR->GetSelectedObject();
	if(pTarget && (pTarget->GetObjectKind() == eObjectKind_Player))
	{	   
		if(GAMEIN->GetMiniNoteDialog()->IsActive())
		{
			if(GAMEIN->GetMiniNoteDialog()->GetCurMode() == eMinNoteMode_Read)
			{
				GAMEIN->GetMiniNoteDialog()->ShowMiniNoteMode(eMinNoteMode_Write);
			}
			else
			{
				GAMEIN->GetMiniNoteDialog()->SetActive(FALSE);
			}
		}
		else
		{
			GAMEIN->GetMiniNoteDialog()->SetActive(TRUE);
			GAMEIN->GetMiniNoteDialog()->ShowMiniNoteMode(eMinNoteMode_Write);	
			cEditBox * pReceiver = (cEditBox *)WINDOWMGR->GetWindowForIDEx(NOTE_MRECEIVEREDIT);
			if(pReceiver)
			{
				char pname[MAX_NAME_LENGTH+1] = {0};
				SafeStrCpy(pname, pTarget->GetObjectName() , MAX_NAME_LENGTH+1);
				if(strlen(pname) == 0)
				return;
				pReceiver->SetEditText(pname);
			}
		}
	}
	else
	{
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(18));
		return;
	}
}
void CharacterRBTN::ButtonAction09()
{
	CObject* pTarget = OBJECTMGR->GetSelectedObject();
	if(pTarget && (pTarget->GetObjectKind() == eObjectKind_Player))
	{
			char temp[MAX_NAME_LENGTH+1];
			if(strcmp(pTarget->GetObjectName(), "") == 0)
				return;
			sprintf(temp,"/%s ",pTarget->GetObjectName());
			cEditBox* pChatBox = GAMEIN->GetChatDialog()->GetChatEditBox();
			pChatBox->SetFocusEdit(TRUE);
			pChatBox->SetEditText(temp);
	}
	else
	{
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(18));
		return;
	}
}
void CharacterRBTN::ButtonAction10()
{
	CObject* pObject = (CPlayer*)m_pSelectedObj;//OBJECTMGR->GetSelectedObject();
	if( pObject )
	if( pObject->GetObjectKind() == eObjectKind_Player )
	{
		if( HERO )
		{
			VECTOR3 vMyPos, vTargetPos;
			HERO->GetPosition( &vMyPos );
			pObject->GetPosition( &vTargetPos );
			float dist = CalcDistanceXZ( &vMyPos, &vTargetPos );
			if( dist > 1800.0f )
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(789));
			}
			else
			{
				HERO->SetFollowPlayer( pObject->GetID() );
				static char Name[128];
				sprintf(Name,pObject->GetObjectName());
				char str[128];
				sprintf(str,"55140");
				MSG_CHAT data;
				data.Category	= MP_CHAT;
				data.Protocol	= MP_CHAT_WHISPER_SYN;
				data.dwObjectID	= HEROID;
				SafeStrCpy( data.Name, Name, MAX_NAME_LENGTH+1 );
				SafeStrCpy( data.Msg, str, MAX_CHAT_LENGTH+1 );
				NETWORK->Send(&data,data.GetMsgLength());
			}
		}
	}
}
void CharacterRBTN::ButtonAction11()
{
	CObject* pTarget = OBJECTMGR->GetSelectedObject();
	if(GAMEIN->CopyToClipboard(pTarget->GetObjectName(),MAX_NAME_LENGTH))
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(2302) );
}
void CharacterRBTN::ButtonAction12()
{
	switch(SetSelf)
	{
	case eSelfOtherPlayer:
		{
			COtherCharacterAmuletDlg *pDlg = (COtherCharacterAmuletDlg*)GAMEIN->GetOtherCharacterAmuletDlg();
			if (!pDlg)
				return;
			if (pDlg->IsActive())
			{
				pDlg->SetActive(FALSE);
			}
			else
			{
				DWORD curtime =  MHTIMEMGR->GetNewCalcCurTime();
				if(curtime-pDlg->GetLastTime()>10000)
				{
					MSG_DWORD msg;
					msg.Category = MP_ITEM;
					msg.Protocol = MP_ITEM_QUERY_CHARACTERAMULET_SYN;
					msg.dwObjectID = HERO->GetID();
					msg.dwData = m_pSelectedObj->GetID();
					NETWORK->Send(&msg, sizeof(msg));
					pDlg->SetLastTime(curtime);
					pDlg->SetActive(TRUE);
				}
				else
				{
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1950));
				}
			}
		}
	break;
	}
}
void CharacterRBTN::ButtonAction13()
{
	switch(SetSelf)
	{
	case eSelfOtherPlayer:
	case eSelfMenu:
		{
			//!((CPlayer*)pObj)->IsMarry()&&
				
			MOVEMGR->HeroMoveStop();
			//OBJECTSTATEMGR->StartObjectState( HERO, eObjectState_Deal );
			if(!HERO->IsMarry())
			{
				//GAMEIN->GetMiniMaryDialog()->SetActive(TRUE);
				char Name[MAX_NAME_LENGTH+1] = {0};
				CObject* pTarget = OBJECTMGR->GetSelectedObject();
				if(pTarget && (pTarget->GetObjectKind() == eObjectKind_Player))
				{
					SafeStrCpy(Name, OBJECTMGR->GetSelectedObject()->GetObjectName(), MAX_NAME_LENGTH+1);
					//GAMEIN->GetMiniMaryDialog()->SetName(Name);
					MSG_NAME msg;
					msg.Category = MP_MARRY;
					msg.Protocol = MP_MARRY_ADD_SYN;
					SafeStrCpy(msg.Name, Name, MAX_NAME_LENGTH + 1);
					msg.dwObjectID = HEROID;
					NETWORK->Send(&msg, sizeof(msg));
				}
				//WINDOWMGR->MsgBox( MBI_MARRYPROPOSE, MBT_YESNO, CHATMGR->GetChatMsg(2552) );
			}
			else
			{
				//WINDOWMGR->MsgBox( MBI_SHOUT_NACK, MBT_OK, CHATMGR->GetChatMsg(904) );
				WINDOWMGR->MsgBox( MBI_MARRYDIVORCE, MBT_YESNO, CHATMGR->GetChatMsg(2553) );
			}
		}
	break;
	}
}
//-------------------------------------BUTTON ACTION END