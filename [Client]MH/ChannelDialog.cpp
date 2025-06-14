#include "stdafx.h"
#include "ChannelDialog.h"
#include "WindowIDEnum.h"
#include "CharSelect.h"
#include "cRItemEx.h"
#include "ChatManager.h"
#include "MainGame.h"
#include "ObjectManager.h"
#include "./Input/UserInput.h"
#include "MoveDialog.h"
#include "GameIn.h"
#include "ObjectStateManager.h"
#include "ItemManager.h"
#include "MHTimeManager.h"
#include "aimanager.h"
#include "JackFlashRoar.h"
#include "reconnect.h"
#include "./Interface/cWindowManager.h"

#include "GameResourceManager.h"
int gChannelNum = -1;
bool IsBattleChannel[MAX_CHANNEL_NUM] = { 0, };
bool IsPKAllowChannel[MAX_CHANNEL_NUM] = { 0, };
//extern int m_ChannelIndex;
GLOBALTON(CChannelDialog)

CChannelDialog::CChannelDialog()
{
	IsMove = FALSE;
	m_bInit = FALSE;
	m_SelectRowIdx = 0;
	m_BaseChannelIndex = 0;
	m_wMoveMapNum = 0;
	m_dwChangeMapState = 0;

	m_SelectedTab = 0;
	m_bExtend = FALSE;
	m_wMoveMapNumEx = 0;

}

CChannelDialog::~CChannelDialog()
{
	if (m_pChannelLCtrl)
	{
		m_pChannelLCtrl->DeleteAllItems();
	}
}
void CChannelDialog::Linking()
{
	m_pChannelLCtrl = (cListCtrl*)GetWindowForID(CHA_CHANNELLIST);

	m_pPageBtn[0] = (cPushupButton*)GetWindowForID(CHA_CHANNELPAGE01);
	m_pPageBtn[1] = (cPushupButton*)GetWindowForID(CHA_CHANNELPAGE02);
	if (m_pPageBtn[0])
		m_pPageBtn[0]->SetActive(false);
	if (m_pPageBtn[1])
		m_pPageBtn[1]->SetActive(false);

	if (m_pChannelLCtrl)
	{
		m_pChannelLCtrl->SetRenderTooltip(TRUE);
	}
}
DWORD CChannelDialog::ActionKeyboardEvent(CKeyboard * keyInfo)
{
	if (!m_bActive) return WE_NULL;

	for (int i = 0; i < 9; i++)
	{
		if (keyInfo->GetKeyDown(KEY_PAD1+i))
		{
			SelectChannel(i);
			return WE_NULL;
		}
	}
	/*if (keyInfo->GetKeyDown(KEY_PAD1))
	{
		SelectChannel(0);
		return WE_NULL;
	}
	if (keyInfo->GetKeyDown(KEY_PAD2))
	{
		SelectChannel(1);
		return WE_NULL;
	}
	if (keyInfo->GetKeyDown(KEY_PAD3))
	{
		SelectChannel(2);
		return WE_NULL;
	}
	if (keyInfo->GetKeyDown(KEY_PAD4))
	{
		SelectChannel(3);
		return WE_NULL;
	}
	if (keyInfo->GetKeyDown(KEY_PAD5))
	{
		SelectChannel(4);
		return WE_NULL;
	}
	if (keyInfo->GetKeyDown(KEY_PAD6))
	{
		SelectChannel(5);
		return WE_NULL;
	}
	if (keyInfo->GetKeyDown(KEY_PAD7))
	{
		SelectChannel(6);
		return WE_NULL;
	}
	if (keyInfo->GetKeyDown(KEY_PAD8))
	{
		SelectChannel(7);
		return WE_NULL;
	}
	if (keyInfo->GetKeyDown(KEY_PAD9))
	{
		SelectChannel(8);
		return WE_NULL;
	}
	if (keyInfo->GetKeyDown(KEY_PAD0))
	{
		SelectChannel(9);
		return WE_NULL;
	}
	else */if (keyInfo->GetKeyDown(KEY_PADENTER))
	{
		if (MAINGAME->GetCurStateNum() == eGAMESTATE_CHARSELECT)
			OnConnect();
		else if (MAINGAME->GetCurStateNum() == eGAMESTATE_GAMEIN)
			MapChange();
		return WE_NULL;
	}

	/*if( MAINGAME->GetCurStateNum() == eGAMESTATE_GAMEIN  )
	{
	CChatDialog * dlg = GAMEIN->GetChatDialog();
	if(dlg->IsChatEditBoxActive())
	return WE_NULL;
	else
	{
	if( MBT_YESNO == m_nMBType )
	{
	for( int i = 0 ; i < 2 ; ++ i )
	{
	if( !m_pButton[i] ) continue ;

	if( (keyInfo->GetKeyDown( KEY_RETURN )||keyInfo->GetKeyDown( KEY_PADENTER )) && (MBI_YES == m_pButton[i]->GetID()) )
	{
	ForcePressButton( MBI_YES ) ;
	break;
	}
	else if( keyInfo->GetKeyDown( KEY_ESCAPE ) && (MBI_NO == m_pButton[i]->GetID()) )
	{
	ForcePressButton( MBI_NO ) ;
	break;
	}
	}
	}
	else if( MBT_OK == m_nMBType )
	{
	if( keyInfo->GetKeyDown( KEY_RETURN )||keyInfo->GetKeyDown( KEY_PADENTER ))
	{
	ForcePressButton( MBT_OK ) ;
	}
	}
	}
	}*/


	return WE_NULL;
}

DWORD CChannelDialog::ActionEvent(CMouse* mouseInfo)
{
	DWORD we = WE_NULL;
	if (!m_bActive) return we;

	CReconnectDialog* m_RecDlg = (CReconnectDialog*)WINDOWMGR->GetWindowForID(RECONNECT_DLG);
	if (m_RecDlg && m_RecDlg->IsActive())
	{
		return we;
	}

	we = cDialog::ActionEvent(mouseInfo);




	int rowidx = m_pChannelLCtrl->GetSelectedRowIdx();
	if (we & WE_ROWCLICK)
	{
		//if(m_pPageBtn[0]->IsPushed())
		SelectChannel(rowidx);
		//if(m_pPageBtn[1]->IsPushed())
		//	SelectChannel(rowidx+10);
	}
	else if (we & WE_ROWDBLCLICK)
	{
		if (MAINGAME->GetCurStateNum() == eGAMESTATE_CHARSELECT)
			OnConnect();
		else if (MAINGAME->GetCurStateNum() == eGAMESTATE_GAMEIN)
			MapChange();
	}
	return we;

	////////////////////
	/*DWORD we = WE_NULL;

	if( !m_bActive ) return we;
	we = cDialog::ActionEvent(mouseInfo);

	int rowidx = m_pMoveLCtrl->GetSelectedRowIdx();
	if(we & WE_ROWCLICK)
	{
	SelectMoveIdx(rowidx);
	}
	else if( we & WE_ROWDBLCLICK )
	{
	//OnMove();
	}
	return we;*/
}


void CChannelDialog::SetChannelList(MSG_CHANNEL_INFO* pInfo)
{
	pInfoSaved = pInfo;
	m_pChannelLCtrl->DeleteAllItems();

	char temp[MAX_CHANNEL_NAME + 4] = { 0, };
	int len = strlen(temp);
	char num[2] = { 0, };
	BYTE Count = 0;
	WORD LowCrowd = 1000;
	int rowidx = 0;

	m_BaseChannelIndex = 0;
	for (int i = 0; i<pInfo->Count; ++i)
	{
		IsBattleChannel[i] = pInfo->bBattleChannel[i];
		IsPKAllowChannel[i] = pInfo->bPKAllowChannel[i];
		cRITEMEx *ritem = new cRITEMEx(2);
		sprintf(temp, CHATMGR->GetChatMsg(2604), i + 1);//1701
		strcpy(ritem->pString[0], temp);

		if(pInfo->Count>10)
		SetExtend(TRUE);

		/*if(pInfo->PlayerNum[i] < 30)
		strcpy(ritem->pString[1], CHATMGR->GetChatMsg(211) );
		else if(pInfo->PlayerNum[i] < 50)
		strcpy(ritem->pString[1], CHATMGR->GetChatMsg(212) );
		else
		{
		strcpy(ritem->pString[1], CHATMGR->GetChatMsg(213) );
		ritem->rgb[1] = RGBA_MAKE(255,0,0,255);
		}*/

		// 070122 LYW --- Setting color.
		//ritem->rgb[0] = RGBA_MAKE( 10, 10, 10, 255 ) ;
		//ritem->rgb[1] = RGBA_MAKE( 10, 10, 10, 255 ) ;

		//Crowd Level
		if (pInfo->PlayerNum[i] < 30)
			// 070122 LYW --- Modified this line.
			//strcpy(ritem->pString[1], CHATMGR->GetChatMsg(211) );
			strcpy(ritem->pString[1], CHATMGR->GetChatMsg(211));
		else if (pInfo->PlayerNum[i] < 100)	//MAX USER PER CHANNEL
			// 070122 LYW --- Modified this line.
			//strcpy(ritem->pString[1], CHATMGR->GetChatMsg(212) );
			strcpy(ritem->pString[1], CHATMGR->GetChatMsg(212));
		else
		{
			// 070122 LYW --- Modified this line.
			//strcpy(ritem->pString[1], CHATMGR->GetChatMsg(213) );
			strcpy(ritem->pString[1], CHATMGR->GetChatMsg(213));
			ritem->rgb[1] = RGBA_MAKE(255, 0, 0, 255);
		}


		if (pInfo->bPKAllowChannel[i])
		{
			//strcpy(ritem->pString[0], CHATMGR->GetChatMsg(2603) );
			char temp[64] = { 0, };
			sprintf(temp, " %s", CHATMGR->GetChatMsg(2603));//1702
			strcat(ritem->pString[0], temp);
			ritem->rgb[0] = RGBA_MAKE(255, 0, 0, 255);

			//char TooltipInfo[128]={0,};
			//sprintf( TooltipInfo, CHATMGR->GetChatMsg(2605));
			//ritem->AddToolTipLine(TooltipInfo);

			DWORD dwColor = RGB_HALF(255, 255, 0);

			char bTemp[64] = { 0, };

			ZeroMemory(&bTemp, sizeof(bTemp));

			//sprintf(bTemp,CHATMGR->GetChatMsg(2610),pInfo->ExpRate[i]);
			ritem->AddToolTipLine(/*CHATMGR->GetChatMsg(2608)*/ritem->pString[0], ritem->rgb[0]);
			//ritem->SetToolTip(CHATMGR->GetChatMsg(2608),dwColor,&m_TooltipImage);
			ritem->AddToolTipLine("");

			ritem->AddToolTipLine(CHATMGR->GetChatMsg(2609), dwColor);

			sprintf(bTemp, CHATMGR->GetChatMsg(2610), pInfo->ExpRate[i]);
			ritem->AddToolTipLine(bTemp, dwColor);

			sprintf(bTemp, CHATMGR->GetChatMsg(2611), pInfo->AbilityExpRate[i]);
			ritem->AddToolTipLine(bTemp, dwColor);

			sprintf(bTemp, CHATMGR->GetChatMsg(2612), pInfo->MoneyRate[i]);
			ritem->AddToolTipLine(bTemp, dwColor);

			sprintf(bTemp, CHATMGR->GetChatMsg(2613), pInfo->RegenRate[i]);
			ritem->AddToolTipLine(bTemp, dwColor);

			sprintf(bTemp, CHATMGR->GetChatMsg(2614), pInfo->DropRate[i]);
			ritem->AddToolTipLine(bTemp, dwColor);

		}
		else if (pInfo->bBattleChannel[i])
		{
			//strcpy(ritem->pString[0], CHATMGR->GetChatMsg(1702) );
			char temp[64] = { 0, };
			sprintf(temp, " %s", CHATMGR->GetChatMsg(1702));//1702
			strcat(ritem->pString[0], temp);
			ritem->rgb[0] = RGBA_MAKE(255, 0, 0, 255);

			//char TooltipInfo[128]={0,};
			//sprintf( TooltipInfo, CHATMGR->GetChatMsg(2606));
			//ritem->AddToolTipLine(TooltipInfo);
			DWORD dwColor = RGB_HALF(255, 255, 0);

			char bTemp[64] = { 0, };

			ZeroMemory(&bTemp, sizeof(bTemp));
			ritem->AddToolTipLine(/*CHATMGR->GetChatMsg(2608)*/ritem->pString[0], ritem->rgb[0]);
			//ritem->SetToolTip(CHATMGR->GetChatMsg(2608),dwColor,&m_TooltipImage);
			ritem->AddToolTipLine("");

			ritem->AddToolTipLine(CHATMGR->GetChatMsg(2609), dwColor);

			sprintf(bTemp, CHATMGR->GetChatMsg(2610), pInfo->ExpRate[i]);
			ritem->AddToolTipLine(bTemp, dwColor);

			sprintf(bTemp, CHATMGR->GetChatMsg(2611), pInfo->AbilityExpRate[i]);
			ritem->AddToolTipLine(bTemp, dwColor);

			sprintf(bTemp, CHATMGR->GetChatMsg(2612), pInfo->MoneyRate[i]);
			ritem->AddToolTipLine(bTemp, dwColor);

			sprintf(bTemp, CHATMGR->GetChatMsg(2613), pInfo->RegenRate[i]);
			ritem->AddToolTipLine(bTemp, dwColor);

			sprintf(bTemp, CHATMGR->GetChatMsg(2614), pInfo->DropRate[i]);
			ritem->AddToolTipLine(bTemp, dwColor);
		}
		else
		{
			//strcpy(ritem->pString[0], CHATMGR->GetChatMsg(2605) );
			char temp[64] = { 0, };
			sprintf(temp, " %s", CHATMGR->GetChatMsg(2605));//1702
			strcat(ritem->pString[0], temp);
			ritem->rgb[0] = RGBA_MAKE(255, 255, 255, 255);

			//char TooltipInfo[128]={0,};
			//sprintf( TooltipInfo, CHATMGR->GetChatMsg(2607));
			/*sprintf( TooltipInfo, CHATMGR->GetChatMsg(2240),
			pInfo->ExpRate[i]	,
			pInfo->AbilityExpRate[i]	,
			pInfo->MoneyRate[i]	,
			pInfo->RegenRate[i]	,
			pInfo->DropRate[i]);*/

			DWORD dwColor = RGB_HALF(255, 255, 0);

			char bTemp[64] = { 0, };

			ZeroMemory(&bTemp, sizeof(bTemp));
			ritem->AddToolTipLine(/*CHATMGR->GetChatMsg(2608)*/ritem->pString[0], ritem->rgb[0]);
			//ritem->SetToolTip(CHATMGR->GetChatMsg(2608),dwColor,&m_TooltipImage);
			ritem->AddToolTipLine("");

			ritem->AddToolTipLine(CHATMGR->GetChatMsg(2609), dwColor);

			sprintf(bTemp, CHATMGR->GetChatMsg(2610), pInfo->ExpRate[i]);
			ritem->AddToolTipLine(bTemp, dwColor);

			sprintf(bTemp, CHATMGR->GetChatMsg(2611), pInfo->AbilityExpRate[i]);
			ritem->AddToolTipLine(bTemp, dwColor);

			sprintf(bTemp, CHATMGR->GetChatMsg(2612), pInfo->MoneyRate[i]);
			ritem->AddToolTipLine(bTemp, dwColor);

			sprintf(bTemp, CHATMGR->GetChatMsg(2613), pInfo->RegenRate[i]);
			ritem->AddToolTipLine(bTemp, dwColor);

			sprintf(bTemp, CHATMGR->GetChatMsg(2614), pInfo->DropRate[i]);
			ritem->AddToolTipLine(bTemp, dwColor);

			//ritem->AddToolTipLine(TooltipInfo);
			//ritem->rgb[0] = RGBA_MAKE(255,255,255,255);			
		}

		/*if( pInfo->bPKAllowChannel[i] )
		{
		char temp[64] = {0,};
		sprintf( temp, " %s", CHATMGR->GetChatMsg(2603) );//1702
		strcat(ritem->pString[0], temp);
		ritem->rgb[0] = RGBA_MAKE(255,0,0,255);

		char TooltipInfo[128]={0,};
		sprintf( TooltipInfo, CHATMGR->GetChatMsg(2606));
		ritem->AddToolTipLine(TooltipInfo);
		}
		else if( pInfo->bBattleChannel[i] )
		{
		char temp[64] = {0,};
		sprintf( temp, " %s", CHATMGR->GetChatMsg(1702) );//1702
		strcat(ritem->pString[0], temp);
		ritem->rgb[0] = RGBA_MAKE(255,0,0,255);
		}
		else// if( !pInfo->bPKAllowChannel[i] && !pInfo->bBattleChannel[i] )
		{
		char temp[64] = {0,};
		sprintf( temp, " %s", CHATMGR->GetChatMsg(2605) );//1702
		strcat(ritem->pString[0], temp);
		ritem->rgb[0] = RGBA_MAKE(255,255,255,255);
		}*/
		ritem->dwID = pInfo->PlayerNum[i];
		m_pChannelLCtrl->InsertItem(i, ritem);

		Count++;

		if (LowCrowd == 0)
			continue;
		if (LowCrowd > pInfo->PlayerNum[i])
		{
			LowCrowd = pInfo->PlayerNum[i];
			rowidx = i;
		}
	}
	rowidx = 0;

	/*cRITEMEx* pRItem = (cRITEMEx*)m_pChannelLCtrl->GetRItem(rowidx);
	// 070122 LYW --- Modified this line.
	//pRItem->rgb[0] = RGBA_MAKE(255,234,0,255);
	if (pRItem)
	{
	//pRItem->rgb[0] = RGBA_MAKE(255, 255, 255, 255);
	//pRItem->rgb[1] = RGBA_MAKE(255, 255, 255, 255);
	if( IsBattleChannel[rowidx] )
	pRItem->rgb[0] = RGBA_MAKE(255,0,0,255);
	else if( IsPKAllowChannel[rowidx] )
	pRItem->rgb[0] = RGBA_MAKE(255,0,0,255);
	else
	{
	//strcpy(pRItem->pString[0], CHATMGR->GetChatMsg(2605) );
	pRItem->rgb[0] = RGBA_MAKE(255,255,255,255);
	}
	}*/
	/*else //if (pRItem)
	{
	pRItem->rgb[0] = RGBA_MAKE(255, 0, 0, 255);
	pRItem->rgb[1] = RGBA_MAKE(255, 0, 0, 255);
	}*/

	//pRItem->rgb[0] = RGBA_MAKE(255,234,0,255);
	//pRItem->rgb[1] = RGBA_MAKE(255,255,255,255);
	gChannelNum = rowidx + m_BaseChannelIndex;
	m_SelectRowIdx = rowidx;
	SetActive(TRUE);

	m_wMoveMapNum = pInfo->wMoveMapNum;
	m_dwChangeMapState = pInfo->dwChangeMapState;
}
void CChannelDialog::SelectChannel(int rowidx)
{
	int Temp = 0;

	if (m_pPageBtn[0]->IsPushed())
		Temp = 0;

	if (m_pPageBtn[1]->IsPushed())
		Temp = 10;

	cRITEMEx * pRItem = (cRITEMEx *)m_pChannelLCtrl->GetRItem(rowidx);
	if (pRItem != NULL)
	{
		if (m_SelectRowIdx != rowidx)
		{
			pRItem->rgb[0] = RGBA_MAKE(0, 255, 0, 255);
			//pRItem->rgb[1] = RGBA_MAKE(255, 255, 255, 255);
			pRItem = (cRITEMEx*)m_pChannelLCtrl->GetRItem(m_SelectRowIdx);
			

			if (IsBattleChannel[m_SelectRowIdx] || IsPKAllowChannel[m_SelectRowIdx])
				pRItem->rgb[0] = RGBA_MAKE(255, 0, 0, 255);
			else
			{
				pRItem->rgb[0] = RGBA_MAKE(255, 255, 255, 255);
			}

			


			cRITEMEx* pRItem = (cRITEMEx*)m_pChannelLCtrl->GetRItem(rowidx);
			if (pRItem->dwID >= 300)
				gChannelNum = -1;
			else
			{
				gChannelNum = rowidx + m_BaseChannelIndex + Temp;
			}

			m_SelectRowIdx = rowidx;

			REDISCONN->SetChannelIdx(rowidx + m_BaseChannelIndex + Temp);
		}
	}
}
void CChannelDialog::SetActive(BOOL val)
{
	cDialog::SetActive(val);

	if (val)
	{
		m_SelectedTab = 0;

		if (m_bExtend)
		{
			if (m_pPageBtn[0])
			{
				m_pPageBtn[0]->SetActive(TRUE);
				m_pPageBtn[0]->SetPush(TRUE);
			}
			if (m_pPageBtn[1])
			{
				m_pPageBtn[1]->SetActive(TRUE);
				m_pPageBtn[1]->SetPush(FALSE);
			}
		}
		else
		{
			if (m_pPageBtn[0])
				m_pPageBtn[0]->SetActive(FALSE);
			if (m_pPageBtn[1])
				m_pPageBtn[1]->SetActive(FALSE);
		}
	}
	else
	{
		ITEMMGR->SetDisableDialog(FALSE, eItemTable_Inventory);
		ITEMMGR->SetDisableDialog(FALSE, eItemTable_Pyoguk);
		ITEMMGR->SetDisableDialog(FALSE, eItemTable_MunpaWarehouse);
		ITEMMGR->SetDisableDialog(FALSE, eItemTable_Shop);

		OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);
	}
}

void CChannelDialog::OnConnect()
{
	if (m_pPageBtn[0]->IsPushed())
		m_SelectRowIdx = m_SelectRowIdx;

	if (m_pPageBtn[1]->IsPushed())
		m_SelectRowIdx = m_SelectRowIdx + 10;

	cRITEMEx * pRItem = (cRITEMEx*)m_pChannelLCtrl->GetRItem(m_SelectRowIdx);
	if (pRItem)
	{
		if (gChannelNum == -1)
		{
			CHARSELECT->DisplayNotice(279);
		}
		else if (CHARSELECT->EnterGame() == FALSE)
		{
			CHARSELECT->DisplayNotice(18);
		}
	}
}


void CChannelDialog::SendMapChannelInfoSYN(WORD wMapNum, DWORD dwState)
{
	MSG_DWORD2 msg;
	msg.Category = MP_USERCONN;
	msg.Protocol = MP_USERCONN_CHANGEMAP_CHANNELINFO_SYN;
	msg.dwObjectID = HERO->GetID();
	msg.dwData1 = wMapNum;
	msg.dwData2 = dwState;

	NETWORK->Send(&msg, sizeof(msg));

}
void CChannelDialog::MapChange()
{
	if (m_wMoveMapNum != 0)
	{
		if (m_dwChangeMapState == eMapChange_General)
			g_UserInput.GetHeroMove()->MapChange(m_wMoveMapNum);

		else if (m_dwChangeMapState == eMapChange_Item)
		{
			CMoveDialog* pMoveDlg = GAMEIN->GetMoveDialog();
			if (pMoveDlg != NULL)
			{
				pMoveDlg->MapChange(m_wMoveMapNum);
			}
		}
	}
	else
	{
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1699));
		if (HERO->GetState() == eObjectState_Deal)
			OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);
	}

	SetActive(FALSE);

	m_wMoveMapNum = 0;
	m_dwChangeMapState = 0;
}





void CChannelDialog::SetExtend(BOOL bExtend)
{
	m_bExtend = bExtend;

}
void CChannelDialog::SetButton(DWORD Idx)
{
	if (m_SelectedTab == Idx || Idx > MAX_MOVEPOINT_PAGE)		return;

	if (m_pPageBtn[m_SelectedTab])
		m_pPageBtn[m_SelectedTab]->SetPush(FALSE);
	if (m_pPageBtn[Idx])
		m_pPageBtn[Idx]->SetPush(TRUE);
	m_SelectedTab = Idx;

	RefreshMoveInfo(Idx);
}
void CChannelDialog::RefreshMoveInfo(DWORD Idx)
{
	m_pChannelLCtrl->DeleteAllItems();
	char temp[MAX_CHANNEL_NAME + 4] = { 0, };
	int len = strlen(temp);
	char num[2] = { 0, };
	BYTE Count = 0;
	WORD LowCrowd = 1000;
	int rowidx = 0;

	m_BaseChannelIndex = 0;
	int i = 0;
	if (Idx == 0)
		i = 0;
	if (Idx == 1)
		i = 10;

	for (i; i<pInfoSaved->Count; ++i)
	{
		IsBattleChannel[i] = pInfoSaved->bBattleChannel[i];
		IsPKAllowChannel[i] = pInfoSaved->bPKAllowChannel[i];
		cRITEMEx *ritem = new cRITEMEx(2);
		sprintf(temp, CHATMGR->GetChatMsg(2604), i + 1);//1701
		strcpy(ritem->pString[0], temp);

		if (pInfoSaved->Count>10)
			SetExtend(TRUE);
		if (pInfoSaved->PlayerNum[i] < 30)
			strcpy(ritem->pString[1], CHATMGR->GetChatMsg(211));
		else if (pInfoSaved->PlayerNum[i] < 50)
			strcpy(ritem->pString[1], CHATMGR->GetChatMsg(212));
		else
		{
			strcpy(ritem->pString[1], CHATMGR->GetChatMsg(213));
			ritem->rgb[1] = RGBA_MAKE(255, 0, 0, 255);
		}
		if (pInfoSaved->bPKAllowChannel[i])
		{
			char temp[64] = { 0, };
			sprintf(temp, " %s", CHATMGR->GetChatMsg(2603));//1702
			strcat(ritem->pString[0], temp);
			ritem->rgb[0] = RGBA_MAKE(255, 0, 0, 255);

			char TooltipInfo[128] = { 0, };
			sprintf(TooltipInfo, CHATMGR->GetChatMsg(2606));
			ritem->AddToolTipLine(TooltipInfo);
		}
		else if (pInfoSaved->bBattleChannel[i])
		{
			char temp[64] = { 0, };
			sprintf(temp, " %s", CHATMGR->GetChatMsg(1702));//1702
			strcat(ritem->pString[0], temp);
			ritem->rgb[0] = RGBA_MAKE(255, 0, 0, 255);
		}
		else// if( !pInfoSaved->bPKAllowChannel[i] && !pInfoSaved->bBattleChannel[i] )
		{
			char temp[64] = { 0, };
			sprintf(temp, " %s", CHATMGR->GetChatMsg(2605));//1702
			strcat(ritem->pString[0], temp);
			ritem->rgb[0] = RGBA_MAKE(255, 255, 255, 255);
		}
		ritem->dwID = pInfoSaved->PlayerNum[i];
		m_pChannelLCtrl->InsertItem(i, ritem);

		Count++;

		if (LowCrowd == 0)
			continue;
		if (LowCrowd > pInfoSaved->PlayerNum[i])
		{
			LowCrowd = pInfoSaved->PlayerNum[i];
			rowidx = i;
		}
	}
}


void CChannelDialog::InitDmove()
{
	nMcount = GAMERESRCMNGR->GetPortalCountDown();
	CurTime = MHTIMEMGR->GetNewCalcCurTime();
	MoveTime = CurTime - 1000;

}
void CChannelDialog::DmoveMap()
{
	if (gCurTime - MoveTime >= 1000)
	{
		MoveTime += 1000;
		if (nMcount <= 0)
		{
			NETWORK->Send(&moMsg, sizeof(moMsg));
			g_UserInput.SetInputFocus(FALSE);
			g_UserInput.SetAllowMouseClick(FALSE);
			IsMove = FALSE;

			GAMEIN->GetJackFlashDlg()->SetMapChange(true);
		}
		else
		{
			sCPlayerAI.IsRun = false;
			g_UserInput.SetAllowMouseClick(FALSE);
			GAMEIN->GetJackFlashDlg()->SetMapChange(true);
			/*if (moMsg.dwData2 == 0 && moMsg.dwData3 == 0 && moMsg.dwData4==0)
			{
			CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(2163), nMcount);
			}
			if (moMsg.dwData2 == 55301 || moMsg.dwData2 == 55303 || moMsg.dwData2 == 57508
			|| moMsg.dwData2==57509)
			{
			CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(2164), nMcount);
			}
			if (moMsg.dwData2 == 55302 || moMsg.dwData2 == 55304 || moMsg.dwData2==55357
			|| moMsg.dwData2 == 55362 || moMsg.dwData2 == 55365 || moMsg.dwData2==55371
			|| moMsg.dwData2 == 55390 || moMsg.dwData2 == 57510 || moMsg.dwData2==57511)
			{
			CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(2165), nMcount);
			}*/

			CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(2720), nMcount);
		}
		--nMcount;
	}
}
void CChannelDialog::SelectChannelGamePadInc()
{
	//static BYTE bTemp = bSel + m_SelectRowIdx;
	SelectChannel(m_SelectRowIdx+1);
}
void CChannelDialog::SelectChannelGamePadDec()
{
	//static BYTE bTemp = bSel + m_SelectRowIdx;
	SelectChannel(m_SelectRowIdx-1);
}