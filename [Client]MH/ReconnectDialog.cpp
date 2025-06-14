#include "stdafx.h"
#include "ReconnectDialog.h"
#include "Reconnect.h"
#include "WindowIDEnum.h"
#include "cWindowManager.h"
#include "cResourceManager.h"
#include "ChatManager.h"
#include "./Input/UserInput.h"

#include "maingame.h"
#include "charselect.h"

#include "GameResourceManager.h"
#include "ServerListDialog.h"

#include "UserInfoManager.h"
CReconnectDialog::CReconnectDialog()
{	
	m_type=WT_RECONNECT_DLG;
}
CReconnectDialog::~CReconnectDialog()
{
}
void CReconnectDialog::Clear()
{
//	LONG x=this->GetAbsX();
//	LONG y=this->GetAbsY();
//	REDISCONN->SetRecDlgAbsXY(x,y);

	WINDOWMGR->DisableAllWindow( false );
	//this->SetMovable(false);
	m_RegNo->SetActive( false );
	m_StaticText[0]->SetActive( false );
	m_StaticText[1]->SetActive( false );

	//if(REDISCONN->GetStart())
	//	SetAbsXY(REDISCONN->GetRecDlgAbsX(),REDISCONN->GetRecDlgAbsY());
}
void CReconnectDialog::Linking()
{	
	m_RegNo=(cButton*)GetWindowForID(RECONNECT_BTN_CANCEL);

	m_StaticText[0] = (cStatic*)GetWindowForID(RECONNECT_STC_TEXT_WITH_CANCEL);
	m_StaticText[1] = (cStatic*)GetWindowForID(RECONNECT_STC_TEXT_WITHOUT_CANCEL);
	if( m_StaticText[0] )
		m_StaticText[0]->SetActive( false );
	if( m_StaticText[1] )
		m_StaticText[1]->SetActive( false );

	//ReloadDlgPosition();

	//SetAbsXY(REDISCONN->GetRecDlgAbsX(),REDISCONN->GetRecDlgAbsY());
}
void CReconnectDialog::ReloadDlgPosition()
{
	//USERINFOMGR->SetReconnect(TRUE);
	USERINFOMGR->SetSaveFolderName(REDISCONN->GetAccID());
	USERINFOMGR->SaveUserInfo(eUIK_RECONNECT);
	//LONG x=GetAbsX()+((float)(GET_MAINWIN_W/2)-(GetWidth()/2));
	//LONG y=GetAbsY()+((float)(GET_MAINWIN_H/2)-(GetHeight()/2));
	//REDISCONN->SetRecDlgAbsXY(x,y);
}
void CReconnectDialog::OnActionEvent(LONG lId, void * p, DWORD we)
{
	if(we & WE_BTNCLICK /*&& lId == RECONNECT_BTN_CANCEL*/)
	{
		switch(lId)
		{
		case RECONNECT_BTN_CANCEL:
			{
				REDISCONN->StopReconnect();

				if( MAINGAME->GetCurStateNum() == eGAMESTATE_CHARSELECT  )
				{
					cDialog* m_CharSelectDlg = WINDOWMGR->GetWindowForID(CS_CHARSELECTDLG);  
					if(m_CharSelectDlg)
					{
						if(m_CharSelectDlg->IsActive())
						{
							CHARSELECT->SetDisablePick( false );
						}
					}
				}
				break;
			}
		default: REDISCONN->StopReconnect(); break;			
		}
	}
}

void CReconnectDialog::AddMessage(int nChatFlag, int nResChat, eReconnectDlg nButton)
{
	char str[512]={0};

	if(nChatFlag==IMAGEMSG) wsprintf(str,RESRCMGR->GetMsg(nResChat));
		
	if(nChatFlag==CHATMSG) wsprintf(str,CHATMGR->GetChatMsg(nResChat));		

	AddMessage(nButton,str);	
}
void CReconnectDialog::AddMessage(int nButton, char* str, ...)
{
	Clear();

	m_RegNo->SetActive( !nButton );
	m_StaticText[nButton]->SetStaticText( str );
	m_StaticText[nButton]->SetActive( true );
	
	SetActive( true );
	SetDisable( false );

	USERINFOMGR->LoadUserInfo(eUIK_RECONNECT);

	DisableSet();	
}
void CReconnectDialog::DisableSet()
{
	CServerListDialog* m_ServerListDlg = (CServerListDialog*)WINDOWMGR->GetWindowForID(SL_SERVERLISTDLG);
	//cDialog* m_ServerListDlg = WINDOWMGR->GetWindowForID(SL_SERVERLISTDLG); 
	if(m_ServerListDlg)
	{
		if(m_ServerListDlg->IsActive())
		{
			m_ServerListDlg->SetDisable( TRUE );
		}
	}
}
//void CReconnectDialog::SetActive(BOOL bFlag)
//{
	//cDialog::SetActive(bFlag);
//}