#include "stdafx.h"

#include "ControlDialog.h"
#include "WindowIDEnum.h"
#include "TopMunpDialog.h"
#include "TopDialog.h"
#include "LuckerDialog.h"
//#include "SafeNumberDialog.h"
#include "cWindowManager.h"
#include "FameGoldDialog.h"
#include "ChatManager.h"
#include "Itemmanager.h"
#include "Gamein.h"
#include "GoldShopDialog.h"
#include "ItemGambleManager.h"
#include "Objectmanager.h"
#include "Mhmap.h"
// the bar control class 2014-05-23

CControlDialog::CControlDialog()
{

}
CControlDialog::~CControlDialog()
{

}
void CControlDialog::Linking()
{

}
void CControlDialog::OnActionEvent(LONG lId, void * p, DWORD we)
{
	/*if(we & WE_BTNCLICK)
	{
		CControlDialog* pDlg=(CControlDialog*)p;
		switch(lId)
		{
		case BAR_PH_BTN_OPEN:  
			{
				CTopDialog* pHandler = (CTopDialog*)GAMEIN->GetTopDialog();  
				if( !pHandler ) return;
				if( pHandler->IsActive() )
				{
					pHandler->SetActive( FALSE );
				}
				else
				{
					pHandler->InitHeroList();
					pHandler->ShowAllList();
					pHandler->SetActive( TRUE );
				}
				pDlg->SetActive(FALSE);
			}
			break;
		case BAR_PH2_BTN_OPEN:  
			{
				CTopMunpDialog* pHandler = (CTopMunpDialog*)GAMEIN->GetTopMunpDialog();  
				if( !pHandler ) return;
				if( pHandler->IsActive() )
				{
					pHandler->SetActive( FALSE );
				}
				else
				{
					pHandler->InitMunpList();
					pHandler->ShowMunpList();
					pHandler->SetActive( TRUE );
				}
				pDlg->SetActive(FALSE);
			}
			break;
		case BAR_LUCKER_BTN_OPEN:
			{
				CLuckerDialog* pHandler = (CLuckerDialog*)GAMEIN->GetLuckerDialog();  
				if( !pHandler ) return;
				if( pHandler->IsActive() )
				{
					pHandler->SetActive( FALSE );
				}
				else
				{
					pHandler->SetActive(TRUE);
				}
				pDlg->SetActive(FALSE);
			}
			break;
		case BAR_MALL_BTN_OPEN:
			{
			}
			break;
	   case BAR_GOLD_BTN_OPEN:
			{
				CGoldShopDialog* pHandlerGold = (CGoldShopDialog*)GAMEIN->GetGoldShopDialog();
				if( !pHandlerGold ) return;
				pHandlerGold->Linking();
				if( pHandlerGold->IsActive() )
				{
					pHandlerGold->SetShopOpen(TRUE);
					pHandlerGold->HideDealer();
					pHandlerGold->SetActive( FALSE );
				}
				else
				{
					pHandlerGold->SetShopOpen(FALSE);
					pHandlerGold->ShowDealer();
					pHandlerGold->SetActive( TRUE );
				}
				pDlg->SetActive(FALSE);
			}
			break;
		case BAR_PAY_BTN_OPEN:
			{
				CItemGambleDialog* pHandlerGamble= (CItemGambleDialog*)GAMEIN->GetItemGambleDialog();
				if( !pHandlerGamble ) return;
				if(pHandlerGamble->IsActive())
				{
					pHandlerGamble->SetActive(FALSE);
				}
				else
				{
					pHandlerGamble->SetActive(TRUE);
				}
				pDlg->SetActive(FALSE);
			}
			break;
		case  BAR_SAFE_BTN_OPEN:
			{
//			  		CSafeNumberDialog * pHandler = (CSafeNumberDialog*)GAMEIN->GetSafeNumberDialog();  
//					if( !pHandler ) return ;
//					if( pHandler->IsActive() )
//					{
//						pHandler->SetActive( FALSE );
//					}
//					else
//					{
//						pHandler->SetActive( TRUE );
//					}
//					pDlg->SetActive(FALSE); 
			}
			break;
		case BAR_Dle_BTN_OPEN:
			{
				DWORD dwCheckBit = eBossMap  | eSiegeWarMap | eSurvivalMap | eTournament;
				if (HERO->GetState() != eObjectState_Die)
				{
					CHATMGR->AddMsg(CTC_SYSMSG,"?ƒï²?³ç’½ç«»ç??—è¦²?¾é???");
					return ;
				} 	
				if( MAP->IsMapKind(dwCheckBit) )
				{
					CHATMGR->AddMsg(CTC_SYSMSG,"å»’åƒ©æ¶·å¡¹æ¯ˆé?å¼…è?ç¹¹â???è­œç?å¡??§éƒ¯?¨éƒªå«?¹§");
					return ;
				} 	
				else
				{
					if( IsBattleChannel[gChannelNum] )
					{
							if (HERO->IsDied())	
							{
								MSGBASE msg;
								msg.Category = MP_CHAR_REVIVE;
								msg.Protocol = MP_CHAR_REVIVE_LOGINSPOT_SYN;
								msg.dwObjectID = HEROID;
								NETWORK->Send(&msg, sizeof(msg));
								cDialog* pDlg = WINDOWMGR->GetWindowForID(MBI_REVIVECONFIRM);
								if (pDlg)
								{
									WINDOWMGR->AddListDestroyWindow(pDlg);
									ITEMMGR->ReviveOtherConfirm(FALSE);
								}
							}
					}
					else
					{
						CHATMGR->AddMsg(CTC_SYSMSG,"å»’åƒ©æ¶·æ??ŠèŠ±?‰é¾?æ™¼?Ÿç«»");
						return ;
					}
				}
			}
			break;
		}
	}*/
}