#include "stdafx.h"
#include "ShoutDialog.h"
#include "./Interface/cEditBox.h"
#include "./Interface/cListDialog.h"
#include "ChatManager.h"
#include "ObjectManager.h"
#include "WindowIDEnum.h"
#include "FilteringTable.h"
#include "cScriptManager.h"                
CShoutDialog::CShoutDialog()
{
	m_type = WT_SHOUT_DLG;
	m_dwItemIdx = m_dwItemPos = 0;
	m_dwItemType=0;
}
CShoutDialog::~CShoutDialog()
{
}
void CShoutDialog::Linking()
{
	m_pMsgBox = (cEditBox*)GetWindowForID( CHA_MSG );
	m_ImageGrid =(cIconGridDialog*)GetWindowForID(CHA_ICONGRID_IMAGE); 
	m_pMsgBox->SetRenderFaceImg(TRUE);  
	LoadFaceList(); 
}
void CShoutDialog::LoadFaceList()
{
	for(int i=0;i<CHARFACELIST; i++)
	{
		if(SCRIPTMGR->GetEmoji(i))
		{
			SCRIPTMGR->GetImage(SCRIPTMGR->GetEmoji(i)->ImageIdx,&m_ImageFace[i],&SCRIPTMGR->GetEmoji(i)->rect[0]);
		}
		cIcon * pIcon;
		pIcon=new cIcon;
		pIcon->Init(0,0,16,16,&m_ImageFace[i] ,IG_DEALITEM_START+8000+i);				
		pIcon->SetData(i+FACELISTSTARTIDX);
		pIcon->SetDisable(TRUE);
		pIcon->SetDepend(FALSE); 
		pIcon->SetMovable(false);
		m_ImageGrid->AddIcon(i,pIcon);
	}
	m_ImageGrid->SetActive(FALSE);
}
void CShoutDialog::OnActionEvent(LONG lId, void * p, DWORD we)
{
  	if( we & WE_BTNCLICK )
	{
		switch( lId )
		{
			case CHA_BTN_IMAGE:  
				{
					if(m_ImageGrid->IsActive())
					{
						m_ImageGrid->SetActive(FALSE);
					}
					else
					{
						m_ImageGrid->SetActive(TRUE);
					}
				}
				break;
		}
	}
	else if( we & WE_LBTNCLICK)  
	{
        switch(lId)
		{
		case CHA_ICONGRID_IMAGE:
			{
				int Pos=m_ImageGrid->GetCurSelCellPos();
				if(Pos>=0)
				{
					char ImgCode[10];
					sprintf(ImgCode,"&%02d",Pos);
					m_pMsgBox->SetEditText(strcat(m_pMsgBox->GetEditText(),ImgCode));
				}
			}
		}
	}
}
BOOL CShoutDialog::SendShoutMsgSyn()
{
	char buf[MAX_SHOUT_LENGTH+1] = { 0, };
	strcpy( buf, m_pMsgBox->GetEditText() );
	if( strlen( buf ) == 0 )
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(903) );
		return FALSE;
	}
	m_pMsgBox->SetEditText("");
	if( FILTERTABLE->FilterChat( buf ) )
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(27) );
		return FALSE;
	}
	SEND_SHOUTBASE_ITEMINFO msg;	
	wsprintf( msg.Msg, "%s", buf );
	msg.Category = MP_ITEM;
	msg.Protocol = MP_ITEM_SHOPITEM_SHOUT_SYN;
	msg.dwObjectID = HERO->GetID();
	msg.ItemIdx = (DWORD)m_dwItemIdx;
	msg.ItemPos = (WORD)m_dwItemPos;
	msg.ItemType= (DWORD)m_dwItemType;
	NETWORK->Send( &msg, sizeof(msg) );
	SetActive( FALSE );
	m_dwItemIdx = m_dwItemPos = 0;
	m_dwItemType=0;
	return TRUE;
}
void CShoutDialog::SetShouMsg(char * pMsg)
{
	m_pMsgBox->SetEditText(pMsg);
}