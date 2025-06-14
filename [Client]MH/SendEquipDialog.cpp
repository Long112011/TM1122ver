
#include "StdAfx.h"
#include "SendEquipDialog.h"
#include "cWindowManager.h"
#include "cScriptManager.h"
#include "WindowIDEnum.h"
#include "ChatManager.h"
#include "ObjectManager.h"
#include "GameIn.h"
#include "ItemManager.h"
#include "./Interface/cWindowManager.h" 
#include "./Interface/cButton.h"
#include "InventoryExDialog.h"
#include "ShoutchatDialog.h"
#include "ShoutDialog.h"

CSendEquipDialog::CSendEquipDialog()
{
	  pDlg=NULL;
	  pItemName=NULL;
}

CSendEquipDialog::~CSendEquipDialog()
{
	  pDlg=NULL;
	  pItemName=NULL;
}

void CSendEquipDialog::Linking()
{
   pDlg=WINDOWMGR->GetWindowForID(SENDEQUIP_DLG);
   pItemName=((cButton*)pDlg->GetWindowForID(SENDEQUIP_ITEM_NAME));
}

void CSendEquipDialog::SetItemName(DWORD ItemIdx)
{
	ITEM_INFO * pItem = ITEMMGR->GetItemInfo(ItemIdx); 

	if(pItem)
	{
		 if(pItemName)
		 {
			 char name[MAX_ITEMNAME_LENGTH+1];
			 SafeStrCpy(name,pItem->ItemName,MAX_ITEMNAME_LENGTH+1);
			 pItemName->SetText(name,RGBA_MAKE(255,255,214,255));
		 }
	}
}

void  CSendEquipDialog::OnActionEvent(LONG lId, void * p, DWORD we)
{
	switch( lId )
	{
	case SENDEQUIP_ITEM_LINK:
		{			
              CItem *  pItem =  GAMEIN->GetInventoryDialog()->GetCurSelectedItem(WEAR);
 
			  char Tempmsg[128];
			
			  if(pItem)
			  {   
				    ITEM_INFO * pInfo = ITEMMGR->GetItemInfo(pItem->GetItemIdx());

					if(!pInfo) return;

				    if(pItem->GetPosition()<80 || pItem->GetPosition()>=90)
				    {
					   return ; //weiye 只有穿戴的装备可以展示
				    }
	
					eITEM_KINDBIT bits = GetItemKind(pInfo->ItemIdx);

					if(bits & eEQUIP_ITEM)
					{
						if(GAMEIN->GetChatDialog()->GetCurSheetNum()==0)
						{
							sprintf(Tempmsg,"ItemDBIdx:%d→%d",pItem->GetDBIdx(),pItem->GetItemIdx());

							CHATMGR->OnInputChatMsg(Tempmsg);
						}
						else if(GAMEIN->GetChatDialog()->GetCurSheetNum()==1)
						{
							sprintf(Tempmsg,"@ItemDBIdx:%d→%d",pItem->GetDBIdx(),pItem->GetItemIdx());

							CHATMGR->OnInputChatMsg(Tempmsg);
						}
						else if(GAMEIN->GetChatDialog()->GetCurSheetNum()==2)
						{
							sprintf(Tempmsg,"#ItemDBIdx:%d→%d",pItem->GetDBIdx(),pItem->GetItemIdx());

							CHATMGR->OnInputChatMsg(Tempmsg);
						}
						else if(GAMEIN->GetChatDialog()->GetCurSheetNum()==3)
						{
							sprintf(Tempmsg,"$ItemDBIdx:%d→%d",pItem->GetDBIdx(),pItem->GetItemIdx());

							CHATMGR->OnInputChatMsg(Tempmsg);
						}
						else if(GAMEIN->GetChatDialog()->GetCurSheetNum()==4)
						{
							sprintf(Tempmsg,"%ItemDBIdx:%d→%d",pItem->GetDBIdx(),pItem->GetItemIdx());

							CHATMGR->OnInputChatMsg(Tempmsg);
						}
			    	}
			 }

			  this->SetActive(FALSE);
		}
		break;
	case SENDEQUIP_ITEM_NAME:
		{
			 this->SetActive(FALSE);
		}
		break;
	case SENDEQUIP_ITEM_SHOUT:
		{
			   CShoutDialog * mDlg = GAMEIN->GetShoutDlg();

			  if(mDlg && mDlg->IsActive())
			  {
				   CItem *  pItem =  GAMEIN->GetInventoryDialog()->GetCurSelectedItem(WEAR);
 
					char Tempmsg[128];
					
					if(pItem)
					{   
						ITEM_INFO * pInfo = ITEMMGR->GetItemInfo(pItem->GetItemIdx());

						if(!pInfo) return;

						if(pItem->GetPosition()<0 || pItem->GetPosition()>=90)
						{
						  return ; //weiye 只有普通物品可以展示
						}
		
						eITEM_KINDBIT bits = GetItemKind(pInfo->ItemIdx);

						if(bits & eEQUIP_ITEM)
						{
							sprintf(Tempmsg,"ItemDBIdx:%d→%d",pItem->GetDBIdx(),pItem->GetItemIdx());

							mDlg->SetShouMsg(Tempmsg);

							mDlg->SendShoutMsgSyn();
						}

					}
			  }
			  else
			  {
                  CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(2197));
			  }
		}
		break;
	}
}


void CSendEquipDialog::SendEquipMsg(DWORD ItemDBIdx)
{
	MSG_DWORD	msg;
	msg.Category = MP_ITEMEXT;
	msg.Protocol =  MP_ITEMEXT_SELECT_SYN;
	msg.dwObjectID = HEROID;
	msg.dwData=ItemDBIdx;
	NETWORK->Send( &msg, sizeof(msg) );
}
