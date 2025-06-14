#include "Stdafx.h"
#include "SafeNumberIcon.h"
//#include "SafeNumberDialog.h"
#include "SafeNumberImg.h"
//#include "SafeNumberLockDialog.h"
//#include "SafeNumberChangeDialog.h"
#include "WindowIDEnum.h"
#include "./Interface/cWindowManager.h" 
#include "ObjectManager.h"
#include "GameIn.h"
#include "ChatManager.h"
#include "cResourceManager.h"
#include "Reconnect.h"
extern BOOL  m_SafeIconShow;

CSafeNumberIcon::CSafeNumberIcon()
{
    m_Dlg=NULL;
	bDialogState=0;

	m_EditBox1=NULL;
	m_EditBox2=NULL;

	m_Btn_Confirm=NULL;
	for(int i=0;i<eBtnMax;i++)
	{
		m_Btn_Icon[i]=NULL;
	}

	//for(int i=0;i<3;i++)
	//{
   //     m_Static_on[i]=NULL;
	//	m_Static_off[i]=NULL;
	//}
}

CSafeNumberIcon::~CSafeNumberIcon()
{
    m_Dlg=NULL;
	bDialogState=0;
	
	m_EditBox1=NULL;
	m_EditBox2=NULL;

	m_Btn_Confirm=NULL;	
	for(int i=0;i<eBtnMax;i++)
	{
		m_Btn_Icon[i]=NULL;
	}

	//for(int i=0;i<3;i++)
	//{
    //    m_Static_on[i]=NULL;
	//	m_Static_off[i]=NULL;
	//}
}

void CSafeNumberIcon::Linking()
{
    m_Dlg= WINDOWMGR->GetWindowForID(SAFE_NUMBER_ICON);

	m_Btn_Icon[0]=(cButton*)GetWindowForID(SAFE_NUMBER_BTN);
	m_Btn_Icon[1]=(cButton*)GetWindowForID(SAFE_RESET_BTN);
	m_Btn_Icon[2]=(cButton*)GetWindowForID(SAFE_CHANGE_BTN);

	m_Btn_Confirm=(cButton*)GetWindowForID(SAFELOCK_CONFIRM_BTN);

	m_StaticTitle = (cStatic*)GetWindowForID(SAFELOCK_TITLE);

	m_EditBox1=(cEditBox*)GetWindowForID(SAFELOCK_EDITBOX1);
	m_EditBox2=(cEditBox*)GetWindowForID(SAFELOCK_EDITBOX2);

	m_Static = (cStatic*)GetWindowForID(SAFE_STATIC_LAB);

//	for(int i=0;i<3;i++)
//	{
//       m_Static_on[i]=(cStatic*)GetWindowForID(SAFE_ICON_PACKET_ON1+i);
//		m_Static_off[i]=(cStatic*)GetWindowForID(SAFE_ICON_PACKET_OFF1+i);
//	}

	//m_Dlg->SetAlwaysTop(TRUE);
}

BOOL CSafeNumberIcon::OnActionEvent(LONG lId, void * p, DWORD we)
{
	if(we & WE_BTNCLICK)
	{
		switch(lId)
		{
		case SAFE_NUMBER_BTN:
			{
				ClearPwdStr();
				if(GetSafeStatic())
				{
					//GAMEIN->GetSafeNumberLockDialog()->ClearPwdStr();
					LockSafeCode();
				}
				else
				{
					SetLock();
					//GAMEIN->GetSafeNumberLockDialog()->ClearPwdStr();
					//GAMEIN->GetSafeNumberLockDialog()->SetActive(TRUE);
				}
			}break;
		case SAFE_CHANGE_BTN:
			{
				SetUnLock();
				//if(!GAMEIN->GetSafeNumberChangeDialog()->IsActive())
				//{
				//	GAMEIN->GetSafeNumberChangeDialog()->ClearPwdStr();
				//	GAMEIN->GetSafeNumberChangeDialog()->SetActive(TRUE);
				//}
			}break;
		case SAFE_RESET_BTN:
			{
				SetReset();
				//if(!GAMEIN->GetSafeNumberDialog()->IsActive())
				//{
				//	GAMEIN->GetSafeNumberDialog()->SetActive(TRUE);
				//}
			}break;
		case SAFELOCK_CONFIRM_BTN:
			{
				if(bDialogState==0)//
				{
					if(strlen(m_EditBox1->GetEditText())>16 || strlen(m_EditBox1->GetEditText())<6 )
					{
						CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(2596));
						m_EditBox1->SetFocusEdit(TRUE);
						return FALSE;
					}
					else
					{
						  CheckSafeCode();
					}
				}

				if(bDialogState==1)//
				{
					if( strlen(m_EditBox1->GetEditText())<6 )
					{
						CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(2389));
						return FALSE;
					}
					else if(strlen(m_EditBox1->GetEditText())>16 )
					{
						CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(2586));
						return FALSE;
					}
					else if(strcmp(m_EditBox1->GetEditText(),m_EditBox2->GetEditText())!=0)
					{
						CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(2204));
						return FALSE;
					}
					else
					{
						SetPwdMsg();
					}
				}
				if(bDialogState==2)
				{
					if(strlen(m_EditBox1->GetEditText())>16 || strlen(m_EditBox1->GetEditText())<6)
					{
						CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(2596));
						return FALSE;
					}
					else
					{
						ChangePwdMsg();  
					}
				}
			}break;
		}
	}
	if(we & WE_CLOSEWINDOW)
	{
		SetActiveStatic(FALSE);
	}

	return TRUE;
}

void CSafeNumberIcon::SetActiveStatic(BOOL Val)
{
	 m_SafeIconShow=Val;
}

void CSafeNumberIcon::SetSafeIconShow()
{
	if(HERO)
	{
		ClearPwdStr();
		switch(HERO->GetSafeStatic())
		{
		case 1:		SetUnLock();	break;
		case -1:	SetReset();		break;
		case -5:	Disable();		break;
		default:	SetLock();		break;
		}
	}
}

BOOL CSafeNumberIcon::GetSafeStatic()
{
	if(HERO)
	{
		return HERO->GetSafeStatic();
	}
	else
	{
		return FALSE;
	}
}
void CSafeNumberIcon::Disable()
{
	m_SafeIconShow=false;
	GAMEIN->GetSafeNumberIcon()->SetActive(FALSE);
	GAMEIN->GetSafeNumberIcon()->SetActiveStatic(FALSE);
	GAMEIN->GetSafeNumberIcon()->SetDisable(true);

	GAMEIN->GetSafeNumberImg()->SetActive(FALSE);

	GAMEIN->GetSafeNumberImg()->SetDisable(true);
}
void CSafeNumberIcon::SetLock()
{//SafeNumberLockDialog.bin
	Button(0);

	bDialogState=0;

	//m_EditBox1->SetActive(true);

	//GAMEIN->GetSafeNumberLockDialog()->SetActive(TRUE);

	//GAMEIN->GetSafeNumberImg()->SetLock();

	m_StaticTitle->SetStaticText(RESRCMGR->GetMsg(1333));
}


void  CSafeNumberIcon::SetReset()
{//SafeRegisterDlg.bin
	Button(1);

	bDialogState=1;

	//m_EditBox1->SetActive(true);
	//m_EditBox2->SetActive(true);
	//GAMEIN->GetSafeNumberDialog()->SetActive(TRUE);

	m_StaticTitle->SetStaticText(RESRCMGR->GetMsg(1478));
}
void CSafeNumberIcon::SetUnLock()
{
	Button(2);

	bDialogState=2;

	//m_EditBox1->SetActive(true);
	//m_EditBox2->SetActive(true);

	m_StaticTitle->SetStaticText(RESRCMGR->GetMsg(1476));
	//GAMEIN->GetSafeNumberImg()->SetUnLock();
}
void CSafeNumberIcon::ButtonLong(WORD bFlag)
{
	ClearAll();
	m_Btn_Icon[bFlag]->SetActive(true);
}

void CSafeNumberIcon::Button(WORD bFlag)
{
	ClearPwdStr();
	
	for(int i=0;i<eBtnMax;i++)
	{
		m_Btn_Icon[i]->SetActive(false);
	}
	m_EditBox1->SetActive(false);
	m_EditBox2->SetActive(false);
	

	if(bFlag==0)
	{
		m_EditBox1->SetActive(true);
	}
	else
	{
		m_EditBox1->SetActive(true);
		m_EditBox2->SetActive(true);
	}
	//m_Btn_Icon[bFlag]->SetActive(true);

	m_Static->SetActive(true);
	m_Static->SetStaticText(CHATMGR->GetChatMsg(2587+bFlag));
	m_Btn_Confirm->SetActive(true);
	
	if(bFlag!=1)
	{
		BYTE bTemp=0;

		if(bFlag==0)
			bTemp=2205;
		if(bFlag==2)
			bTemp=2206;

		m_Btn_Icon[bFlag]->SetText(CHATMGR->GetChatMsg(bTemp)  ,RGBA_MAKE(225, 225, 225, 255));
	}
}



void CSafeNumberIcon::ClearPwdStr()
{
	if(m_EditBox1)
	{
		m_EditBox1->SetEditText("");
	}
	if(m_EditBox2)
	{
		m_EditBox1->SetEditText("");
	}
}
void CSafeNumberIcon::ClearAll()
{
	ClearPwdStr();
	for(int i=0;i<eBtnMax;i++)
	{
		m_Btn_Icon[i]->SetActive(false);
	}
	m_EditBox1->SetActive(false);
	m_EditBox2->SetActive(false);
	m_Static->SetActive(false);
	m_Btn_Confirm->SetActive(false);

	m_StaticTitle->SetStaticText("");
}



void CSafeNumberIcon::LockSafeCode()
{
	MSG_DWORD msg;
	msg.Category=MP_ITEM;
	msg.Protocol= MP_ITEM_SAFE_LOCK_SYN;
	msg.dwObjectID= HEROID;
	msg.dwData=1986;
	NETWORK->Send( &msg,sizeof(msg));
}

//-----------------for state 0
void CSafeNumberIcon::CheckSafeCode()
{
	MSG_SAFE_CHECK msg;
	SafeStrCpy(msg.Pin,m_EditBox1->GetEditText(),16);
	msg.Category=MP_ITEM;
	msg.Protocol=MP_ITEM_SAFE_CHECK_SYN;
	msg.dwObjectID= HEROID;
	NETWORK->Send( &msg,sizeof(msg));

	char * userID = m_EditBox1->GetEditText();
	REDISCONN->SetSafePwd(userID);
}

//-----------------for state 1
void CSafeNumberIcon::SetPwdMsg()
{
	MSG_SAFE_CHANGE msg;
	SafeStrCpy(msg.OPin,m_EditBox1->GetEditText(),16);
	SafeStrCpy(msg.NPin,m_EditBox2->GetEditText(),16);
	msg.Category=MP_ITEM;
	msg.Protocol=MP_ITEM_SAFE_CHANGE_SYN;
	msg.dwObjectID= HEROID;
	msg.Type=0;
	NETWORK->Send( &msg,sizeof(msg));
}

//-----------------for state 2
void CSafeNumberIcon::ChangePwdMsg()
{
	MSG_SAFE_CHANGE msg;
	SafeStrCpy(msg.OPin,m_EditBox1->GetEditText(),16);
	SafeStrCpy(msg.NPin,m_EditBox2->GetEditText(),16);
	msg.Category=MP_ITEM;
	msg.Protocol=MP_ITEM_SAFE_CHANGE_SYN;
	msg.dwObjectID= HEROID;
	msg.Type=1;
	NETWORK->Send( &msg,sizeof(msg));
}

