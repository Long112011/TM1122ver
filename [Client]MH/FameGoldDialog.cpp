#include "Stdafx.h"
#include "FameGoldDialog.h"
#include "WindowIDEnum.h"
#include "cWindowManager.h"
#include "ObjectManager.h"
#include "ChatManager.h"


CFameGoldDialog::CFameGoldDialog()
{
	 m_Dlg=NULL;
	 m_FrameSet=NULL;
     m_FameValue=NULL;
}

CFameGoldDialog::~CFameGoldDialog()
{
	 m_Dlg=NULL;
	 m_FrameSet=NULL;
     m_FameValue=NULL;
}

void  CFameGoldDialog::Linking()
{
    m_Dlg = WINDOWMGR->GetWindowForID( FAMEGOLD_DLG );
    m_FameValue =(cStatic *)GetWindowForID(FAME_STATIC_LAB); 
	m_FrameSet    =(cSpin*)GetWindowForID(FAME_STATIC_SPIN); 
}

void CFameGoldDialog::SetFame()
{
	if(HERO)
	{
	    m_FameValue->SetStaticValue(HERO->GetBadFame());
	}
}

BOOL CFameGoldDialog::OnActionEvent(LONG lId, void * p, DWORD we)
{
	if(we & WE_BTNCLICK)
	{
         if(lId == FAME_FLASH_OK)
		 {
              DWORD m_FameValue= atoi(m_FrameSet->GetEditText());
			  if(m_FameValue>0)
			  {
                     if(HERO->GetBadFame()< m_FameValue)
					 {
							CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(2185));
						    return FALSE;
					 }
					 else if(HERO->GetGoldMoney()< m_FameValue*2)  //weiye Ôª±¦²»¹»Ï´¶ñ
					 {
                            CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(2186));
						    return FALSE;
					 }
					 else
					 {
                            MSG_DWORD msg;
							msg.Category=MP_CHAR;
							msg.Protocol= MP_CHAR_BADFAME_GOLD_SYN;
							msg.dwObjectID= HEROID;
							msg.dwData=m_FameValue;
							NETWORK->Send( &msg,sizeof(msg));
					 }
			  }

			  else
			  {
                    CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(2189));

					return FALSE;
			  }
		 }

		 if(lId ==FAME_FLASH_EXIT)
		 {
			  if(m_Dlg)
			  {
				  m_Dlg->SetActive(FALSE);
			  }
		 }
	}

	return TRUE;
}
void CFameGoldDialog::JustForKen()
{
	if(HERO->GetBadFame()>0)
	{
		MSG_DWORD msg;
		msg.Category=MP_CHAR;
		msg.Protocol= MP_CHAR_BADFAME_GOLD_SYN;
		msg.dwObjectID= HEROID;
		msg.dwData=HERO->GetBadFame();
		NETWORK->Send( &msg,sizeof(msg));
	}
	else
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(2189));
	}
}