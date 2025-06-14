#include "stdafx.h"
#include "RestPwd.h"
#include "WindowIDEnum.h"
#include "cWindowManager.h"
#include "GameResourceManager.h"
#include "cWindowManager.h"
#include "CommonStruct.h"
#include "ChatManager.h"
#include "RegID.h"
#include "Reconnect.h"
RestPwd::RestPwd()
{	
	m_type=WT_REPWD_DLG;
//	XX = YY = 0;
	SavedXX = SavedYY = 0;

	editboxId=NULL;
	editboxPwd=NULL;

	editboxNewPwd=NULL;
	editboxNewPwd1=NULL;
	editboxTel	=NULL;
	editboxEmail=NULL;
}
RestPwd::~RestPwd()
{
//	XX = YY = 0;
	SavedXX = SavedYY = 0;
	editboxId=NULL;
	editboxPwd=NULL;

	editboxNewPwd=NULL;
	editboxNewPwd1=NULL;
	editboxTel	=NULL;
	editboxEmail=NULL;
}

void RestPwd::Linking()
{	
	m_RegBtn	=(cButton*)GetWindowForID(RESTPWD_OK);
	m_RegNo		=(cButton*)GetWindowForID(RESTPWD_NO);

	editboxId		= (cEditBox *)GetWindowForID(RESTPWD_ID);
	editboxPwd		= (cEditBox *)GetWindowForID(RESTPWD_OLDPWD);

	editboxNewPwd	= (cEditBox *)GetWindowForID(RESTPWD_NEWPWD);
	editboxNewPwd1	= (cEditBox *)GetWindowForID(RESTPWD_NEWPWD1);
	editboxTel		= (cEditBox *)GetWindowForID(RESTPWD_TEL);	
	editboxEmail		= (cEditBox *)GetWindowForID(RESTPWD_EMAIL);
	/*m_ResButton[FORCHANGEPASS1]	=(cButton*)GetWindowForID(RESTPWD_OK);
	m_ResButton[FORCHANGEPASS2]	=(cButton*)GetWindowForID(RESTPWD_NO);

	m_ResButton[FORREGISTER1]	=(cButton*)GetWindowForID(REGOK);
	m_ResButton[FORREGISTER2]	=(cButton*)GetWindowForID(REGNO);*/

	//XX = this->GetAbsX();
	//YY = this->GetAbsY();

	
}

void RestPwd::Show()
{	
	SetActive( TRUE );
	if (REDISCONN)
	{
		editboxId->SetEditText(REDISCONN->GetUserName());
		editboxPwd->SetEditText(REDISCONN->GetUserPwd());

		editboxId->SetReadOnly(TRUE);
		editboxPwd->SetReadOnly(TRUE);

		editboxId->SetDisable(TRUE);
		editboxPwd->SetDisable(TRUE);
	}
}

void RestPwd::Close()
{
	SetActive( FALSE );
	editboxNewPwd->SetEditText("");
	editboxNewPwd1->SetEditText("");
}
void RestPwd::SetIDPASS(char*id, char*pass)
{
	SafeStrCpy(m_SaveID, id, 20);
	SafeStrCpy(m_SavePASS, id, 20);
}
BOOL RestPwd::OnActionEvent(LONG lId, void * p, DWORD we)
{
	MSGREG msg;
	//cEditBox * editboxId		= (cEditBox *)GetWindowForID(RESTPWD_ID);
	//cEditBox * editboxPwd		= (cEditBox *)GetWindowForID(RESTPWD_OLDPWD);	
	//cEditBox * editboxNewPwd	= (cEditBox *)GetWindowForID(RESTPWD_NEWPWD);
	//cEditBox * editboxNewPwd1	= (cEditBox *)GetWindowForID(RESTPWD_NEWPWD1);
	//cEditBox * editboxTel		= (cEditBox *)GetWindowForID(RESTPWD_TEL);	
	//cEditBox * editboxEmail		= (cEditBox *)GetWindowForID(RESTPWD_EMAIL);
	
	

	sprintf(msg.ID,		editboxId->GetEditText(),		20);
	sprintf(msg.pwd,	editboxPwd->GetEditText(),		20);
	sprintf(msg.NewPwd,	editboxNewPwd->GetEditText(),	20);
	sprintf(m_Pwd1,		editboxNewPwd1->GetEditText(),	20);
	sprintf(msg.tel,	editboxTel->GetEditText(),		20);
	sprintf(msg.email,	editboxEmail->GetEditText(),	50);

	if(strcmp(msg.ID,"")==0)
	{
		WINDOWMGR->MsgBox( MBI_NOBTNMSGBOX, 1, CHATMGR->GetChatMsg(2166) );
		return FALSE;
	}
	if(strcmp(msg.pwd,"")==0)
	{
		WINDOWMGR->MsgBox( MBI_NOBTNMSGBOX, 1, CHATMGR->GetChatMsg(2167) );
		return FALSE;
	}
	if(strcmp(msg.NewPwd,m_Pwd1)!=0)
	{
		WINDOWMGR->MsgBox( MBI_NOBTNMSGBOX, 1, CHATMGR->GetChatMsg(2718) );
		editboxNewPwd->SetEditText("");
		editboxNewPwd1->SetEditText("");
		return FALSE;
	}
	if(strcmp(msg.NewPwd,m_Pwd1)==0)
	{
		if(strcmp(msg.NewPwd,msg.pwd)==0)//if (msg.pwd == msg.NewPwd)
		{
			WINDOWMGR->MsgBox( MBI_NOBTNMSGBOX, 1, CHATMGR->GetChatMsg(2717) );
			editboxNewPwd->SetEditText("");
			editboxNewPwd1->SetEditText("");
			return FALSE;
		}		
	}
	/*if(strcmp(msg.tel,"")==0||TelIsInvalidCharInclude(msg.tel))
	{
		WINDOWMGR->MsgBox( MBI_NOBTNMSGBOX, 1, CHATMGR->GetChatMsg(2716) );//2169
		editboxTel->SetEditText("");
		return FALSE;
	}
	if(strcmp(msg.email,"")==0)
	{
		WINDOWMGR->MsgBox( MBI_NOBTNMSGBOX, 1, CHATMGR->GetChatMsg(2170) );
		editboxEmail->SetEditText("");
		return FALSE;
	}*/
	if(IsInvalidCharInclude(msg.ID)||IsInvalidCharInclude(msg.NewPwd))
	{
		WINDOWMGR->MsgBox( MBI_NOBTNMSGBOX, 1, CHATMGR->GetChatMsg(2719) );
		editboxId->SetEditText("");
		editboxNewPwd->SetEditText("");	
		editboxNewPwd1->SetEditText("");
		return FALSE;
	}
	msg.Category=MP_USERCONN;
	msg.Protocol=MP_USERCONN_RESTPWD_SYN;
	NETWORK->Send(&msg,sizeof(msg));
	return TRUE;
}
BOOL RestPwd::IsInvalidCharInclude( char* pStr ) 
{
	while( *pStr )
	{
		BOOL bOk = FALSE;

		if( IsDBCSLeadByte( *pStr ) )
		{
			++pStr;
		}
		else
		{
			if( ( *pStr >= 'A' && *pStr <= 'Z' ) || ( *pStr >= 'a' && *pStr <= 'z' ) )
				bOk = TRUE;
			else if( *pStr >= '0' && *pStr <= '9' )
				bOk = TRUE;
		}

		++pStr;

		if( bOk == FALSE )
		{
			return TRUE;
		}
	}

	return FALSE;
}
BOOL RestPwd::TelIsInvalidCharInclude( char* pStr ) 
{
	while( *pStr )
	{
		BOOL bOk = FALSE;

		if( IsDBCSLeadByte( *pStr ) )
		{
			++pStr;
		}
		else
		{
			if( *pStr >= '0' && *pStr <= '9' )
				bOk = TRUE;
		}

		++pStr;

		if( bOk == FALSE )
		{
			return TRUE;
		}
	}

	return FALSE;
}