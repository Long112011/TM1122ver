







// GameState.cpp: implementation of the CGameState class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GameState.h"
#include "MainGame.h"
#include "..\interface\cwindowmanager.h"
#include "windowidenum.h"
#include "cmsgbox.h"
#include "chatmanager.h"
#include "CharSelect.h"
#include "ReconnectDialog.h"
#ifdef _HACK_SHIELD_
#include "HackShieldManager.h"
#endif

#ifdef _NPROTECT_
#include "NProtectManager.h"
#endif
#include "GameIn.h"
#include "RegID.h"
#include "RestPwd.h"
#include "Reconnect.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
extern HWND _g_hWnd;

CGameState::CGameState()
{
	m_bBlockScreen	= FALSE;
	m_bDisconnected = FALSE;
}

CGameState::~CGameState()
{

}
BOOL CGameState::CommonNetworkParser(BYTE Category,BYTE Protocol,void* pMsg)
{
	BOOL rt = FALSE;
	switch(Category) 
	{
	case MP_USERCONN:
		{
			switch(Protocol) 
			{
			case MP_USERCONN_OTHERUSER_CONNECTTRY_NOTIFY:
				{
					WINDOWMGR->MsgBox( MBI_OTHERUSER_CONNECTTRY, MBT_OK, CHATMGR->GetChatMsg(2) );
					rt = TRUE;
				}
				break;
			case MP_USERCONN_CONNECTION_CHECK:
				{
					MSGBASE msg;
					msg.Category = MP_USERCONN;
					msg.Protocol = MP_USERCONN_CONNECTION_CHECK_OK;


					NETWORK->Send(&msg,sizeof(msg));
					rt = TRUE;
				}
				break;
			case MP_USERCONN_DISCONNECTED_BY_OVERLAPLOGIN:
				{
					WINDOWMGR->MsgBox( MBI_DISCONNECTED_OVERLAPPEDLOGIN, 
						MBT_OK, CHATMGR->GetChatMsg(3) );

					rt = TRUE;
				}
				break;
			case MP_USERCONN_CHARACTERSLOT:
				{
					MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;


					rt = TRUE;
				}
				break;
			case MP_USERCONN_REGUSER_ACK://账号注册消息解析  
				{
					cDialog* pIDDlg = WINDOWMGR->GetWindowForID( MT_LOGINDLG );
					cEditBox* UserName=(cEditBox*)pIDDlg->GetWindowForID( MT_IDEDITBOX );
					cEditBox* PassWord=(cEditBox*)pIDDlg->GetWindowForID( MT_PWDEDITBOX );
					cDialog* pREDlg = WINDOWMGR->GetWindowForID( REGDLG );
					cEditBox * editboxId = (cEditBox *)pREDlg->GetWindowForID(REGID);
					cEditBox * editboxPwd = (cEditBox *)pREDlg->GetWindowForID(REGPWD);	
					cEditBox * editboxPwd1 = (cEditBox *)pREDlg->GetWindowForID(REGPWD1);
					cEditBox * editboxTel = (cEditBox *)pREDlg->GetWindowForID(REGTEL);	
					cEditBox * editboxEmail = (cEditBox *)pREDlg->GetWindowForID(REGEMAIL);
						MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
					if(pmsg->dwData1==1)	//账号密码非法字符	
					{
						WINDOWMGR->MsgBox( MBI_NOBTNMSGBOX, 1, CHATMGR->GetChatMsg(2218) );
						editboxId->SetEditText("");
						editboxPwd->SetEditText("");
						editboxPwd1->SetEditText("");
						return FALSE;
					}
					if(pmsg->dwData1==2)	//账号已存在	
					{
						WINDOWMGR->MsgBox( MBI_NOBTNMSGBOX, 1, CHATMGR->GetChatMsg(2219) );
						editboxId->SetEditText("");
						return FALSE;
					}
					if(pmsg->dwData1==3)	//注册成功	
					{
						WINDOWMGR->MsgBox( MBI_NOBTNMSGBOX, 1, CHATMGR->GetChatMsg(2220) );
						UserName->SetEditText(editboxId->GetEditText());
						PassWord->SetEditText(editboxPwd->GetEditText());
						editboxId->SetEditText("");
						editboxPwd->SetEditText("");
						editboxPwd1->SetEditText("");
						editboxTel->SetEditText("");
						editboxEmail->SetEditText("");
						GAMEIN->GetRegID()->Close();
						return TRUE;
					}
					if(pmsg->dwData1==4)	//数据库错误	
					{
						WINDOWMGR->MsgBox( MBI_NOBTNMSGBOX, 1, CHATMGR->GetChatMsg(2221) );
						editboxId->SetEditText("");
						editboxPwd->SetEditText("");
						editboxPwd1->SetEditText("");
						editboxTel->SetEditText("");
						editboxEmail->SetEditText("");
						GAMEIN->GetRegID()->Close();
						return FALSE;
					}
					if(pmsg->dwData1==5)	//手机邮箱非法字符		
					{
						WINDOWMGR->MsgBox( MBI_NOBTNMSGBOX, 1, CHATMGR->GetChatMsg(2222) );
						editboxTel->SetEditText("");
						editboxEmail->SetEditText("");
						return FALSE;
					}
					if(pmsg->dwData1==6)	//黑名单用户	
					{
						WINDOWMGR->MsgBox( MBI_NOBTNMSGBOX, 1, CHATMGR->GetChatMsg(2223) );
						editboxId->SetEditText("");
						editboxPwd->SetEditText("");
						editboxPwd1->SetEditText("");
						editboxTel->SetEditText("");
						editboxEmail->SetEditText("");
						GAMEIN->GetRegID()->Close();
						return FALSE;
					}
					if(pmsg->dwData1 ==7)		//注册关闭	
					{
						WINDOWMGR->MsgBox( MBI_NOBTNMSGBOX, 1, CHATMGR->GetChatMsg(2282) );
						editboxId->SetEditText("");
						editboxPwd->SetEditText("");
						editboxPwd1->SetEditText("");
						editboxTel->SetEditText("");
						editboxEmail->SetEditText("");
						GAMEIN->GetRegID()->Close();
						return FALSE;
					}
				}
				break;
			case MP_USERCONN_RESTPWD_ACK:   //Change password response protocol processing  
				{
					MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
					if(pmsg->dwData1==1)	//Account does not exist	
					{
						WINDOWMGR->MsgBox( MBI_NOBTNMSGBOX, 1, CHATMGR->GetChatMsg(2224) );
						return FALSE;
					}
					if(pmsg->dwData1==2)	//The original password is wrong	
					{
						WINDOWMGR->MsgBox( MBI_NOBTNMSGBOX, 1, CHATMGR->GetChatMsg(2225) );
						return FALSE;
					}
					if(pmsg->dwData1==3)	//Security mailbox error	
					{
						WINDOWMGR->MsgBox( MBI_NOBTNMSGBOX, 1, CHATMGR->GetChatMsg(2226) );
						return FALSE;
					}
					if(pmsg->dwData1==4)	//Security phone error	
					{
						WINDOWMGR->MsgBox( MBI_NOBTNMSGBOX, 1, CHATMGR->GetChatMsg(2227) );
						return FALSE;
					}
					if(pmsg->dwData1==5)	//Illegal characters in new password	
					{
						WINDOWMGR->MsgBox( MBI_NOBTNMSGBOX, 1, CHATMGR->GetChatMsg(2228) );
						return FALSE;
					}
					if(pmsg->dwData1==6)	//Database query error	
					{
						WINDOWMGR->MsgBox( MBI_NOBTNMSGBOX, 1, CHATMGR->GetChatMsg(2229) );
						return FALSE;
					}
					if(pmsg->dwData1==7)	//Successfully modified	
					{
						WINDOWMGR->MsgBox( MBI_NOBTNMSGBOX, 1, CHATMGR->GetChatMsg(2230) );
						cDialog* pREDlg = WINDOWMGR->GetWindowForID( RESTPWD_GDLG );
						cEditBox * editboxId = (cEditBox *)pREDlg->GetWindowForID(RESTPWD_NEWPWD);
						REDISCONN->SetUserPwd(editboxId->GetEditText());
						GAMEIN->GetRestPwdDlg()->Close();
						return TRUE;
					}
				}
				break;
			}
		}
		break;


#ifdef _HACK_SHIELD_
	case MP_HACKSHIELD:
		{

			HACKSHIELDMGR->NetworkMsgParse(Protocol, pMsg);
			rt = TRUE;
		}
		break;
#endif
#ifdef _NPROTECT_
	case MP_NPROTECT:
		{
			NPROTECTMGR->NetworkMsgParse(Protocol, pMsg);
			rt = TRUE;
		}


		break;

#endif
	}


	return rt;
}

void CGameState::SetBlock(BOOL val)
{
	m_bBlockScreen = val;
}
BOOL  CGameState::IsDisconnect()
{
	REDISCONN->SetDisConnected();
	return m_bDisconnected;
}
void CGameState::OnDisconnect()
{
	m_bDisconnected = TRUE;
	REDISCONN->SetDisConnected();
	if( cMsgBox::IsInit() )
	{
		cMsgBox* pMsgBox = WINDOWMGR->MsgBox( MBI_SERVERDISCONNECT, MBT_OK, CHATMGR->GetChatMsg( 448 ) );

		//REDISCONN->Init();
		//REDISCONN->StartProcess();
		//REDISCONN->SetStart(true);
		//REDISCONN->Init();
		//REDISCONN->StartProcess();
		//REDISCONN->GoToGameLoginDialog();
	}

//	else
//	{
//		MessageBox( NULL, "Disconnected To Server.\nThe Game is Closing.", 0, 0 );
//		MAINGAME->SetGameState(eGAMESTATE_END);
//		PostMessage( _g_hWnd, WM_CLOSE, NULL, NULL );
//	}
}




