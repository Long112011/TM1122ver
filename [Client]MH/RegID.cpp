#include "stdafx.h"
#include "RegID.h"
#include "WindowIDEnum.h"
#include "cWindowManager.h"
#include "GameResourceManager.h"
#include "cWindowManager.h"
#include "CommonStruct.h"
#include "ChatManager.h"
#include "GameIn.h"
#include "RestPwd.h"
#include "GlobalEventFunc.h"
RegID::RegID()
{	
	m_type=WT_REG_DLG;
	//XX = YY = 0;
	SavedXX = SavedYY = 0;
	p_Dlg = NULL;
}
RegID::~RegID()
{
	//XX = YY = 0;
	SavedXX = SavedYY = 0;
	p_Dlg = NULL;
}

void RegID::Linking()
{	
	p_Dlg = WINDOWMGR->GetWindowForID( REGDLG );
	m_RegBtn=(cButton*)GetWindowForID(REGOK);
	m_RegNo=(cButton*)GetWindowForID(REGNO);
	//XX = p_Dlg->GetAbsX();
	//YY = p_Dlg->GetAbsY();
}

void RegID::Show()
{	
	SetActive( TRUE );
	LoginIDDialog_hide(1, NULL, 0);
}

void RegID::Close()
{
	SetActive( FALSE );
	LoginIDDialog_hide(0, NULL, 0);
}
/*void RegID::Render()
{
	this->SetAbsXY((LONG)GAMEIN->GetRestPwdDlg()->GetAbsX() + XX, (LONG)GAMEIN->GetRestPwdDlg()->GetAbsY() );
	cDialog::RenderWindow();
	cDialog::RenderComponent();
}*/
BOOL RegID::OnActionEvent(LONG lId, void * p, DWORD we)
{
	MSGREG msg;
	cEditBox * editboxId	= (cEditBox *)GetWindowForID(REGID);
	cEditBox * editboxPwd	= (cEditBox *)GetWindowForID(REGPWD);	
	cEditBox * editboxPwd1	= (cEditBox *)GetWindowForID(REGPWD1);
	cEditBox * editboxTel	= (cEditBox *)GetWindowForID(REGTEL);	
	cEditBox * editboxEmail = (cEditBox *)GetWindowForID(REGEMAIL);

	sprintf(msg.ID,editboxId->GetEditText(),20);
	sprintf(msg.pwd,editboxPwd->GetEditText(),20);
	sprintf(m_Pwd1,editboxPwd1->GetEditText(),20);
	sprintf(msg.tel,editboxTel->GetEditText(),20);
	sprintf(msg.email,editboxEmail->GetEditText(),50);
	if(strcmp(msg.ID,"")==0)
	{
		WINDOWMGR->MsgBox( MBI_NOBTNMSGBOX, 1, CHATMGR->GetChatMsg(2659) );//2166
		return FALSE;
	}
	if(strlen(msg.ID)<6||strlen(msg.pwd)<6||strlen(msg.ID)>19||strlen(msg.pwd)>19)
	{
		WINDOWMGR->MsgBox( MBI_NOBTNMSGBOX, 1, CHATMGR->GetChatMsg(2596) );//2310
		return FALSE;
	}
	if(strcmp(msg.pwd,"")==0)
	{
		WINDOWMGR->MsgBox( MBI_NOBTNMSGBOX, 1, CHATMGR->GetChatMsg(2600) );//2167
		return FALSE;
	}
	if(strcmp(msg.pwd,m_Pwd1)!=0)
	{
		WINDOWMGR->MsgBox( MBI_NOBTNMSGBOX, 1, CHATMGR->GetChatMsg(2204) );//2168
		editboxPwd->SetEditText("");
		editboxPwd1->SetEditText("");
		return FALSE;
	}
	if(strcmp(msg.tel,"")==0||strlen(msg.tel)<8||TelIsInvalidCharInclude( msg.tel ) )
	{
		WINDOWMGR->MsgBox( MBI_NOBTNMSGBOX, 1, CHATMGR->GetChatMsg(2222) );//2169
		editboxTel->SetEditText("");
		return FALSE;
	}
	if(strcmp(msg.email,"")==0)
	{
		WINDOWMGR->MsgBox( MBI_NOBTNMSGBOX, 1, CHATMGR->GetChatMsg(2226) );//2170
		editboxEmail->SetEditText("");
		return FALSE;
	}
	if(IsInvalidCharInclude(msg.ID)||IsInvalidCharInclude(msg.pwd))
	{
		WINDOWMGR->MsgBox( MBI_NOBTNMSGBOX, 1, CHATMGR->GetChatMsg(2228) );//2171
		editboxId->SetEditText("");
		editboxPwd->SetEditText("");	
		editboxPwd1->SetEditText("");
		return FALSE;
	}
	GAMEIN->GetMacAddress(msg.Mac);
	msg.Category=MP_USERCONN;
	msg.Protocol=MP_USERCONN_REGUSER_SYN;
	NETWORK->Send(&msg,sizeof(msg));
	return TRUE;
}
BOOL RegID::IsInvalidCharInclude( char* pStr ) 
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
BOOL RegID::TelIsInvalidCharInclude( char* pStr ) 
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
