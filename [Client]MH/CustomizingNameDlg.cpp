









// NameChangeDialog.cpp: implementation of the CNameChangeDialog class.
//
//////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "CustomizingNameDlg.h"
#include "WindowIDEnum.h"
#include "./Interface/cEditBox.h"
#include "ObjectManager.h"

#include "../cIMEex.h"

#include "FilteringTable.h"
#include "ChatManager.h"
#include "InventoryExDialog.h"
#include "GameIn.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCustomizingNameDialog::CCustomizingNameDialog()
{
	m_DBIdx = 0;
}

CCustomizingNameDialog::~CCustomizingNameDialog()
{

}

void CCustomizingNameDialog::Linking()
{
	m_pNameBox = (cEditBox*)GetWindowForID(CH_CONTOMIZINGNAME_EDITBOX);
}

void CCustomizingNameDialog::SetActive(BOOL val)
{
	cDialog::SetActive(val);

	if (val)
		m_pNameBox->SetEditText("");
}


void CCustomizingNameDialog::CreatCustomizingNameSyn()
{
	DWORD len = 0;
	char buf[20];
	strcpy(buf, m_pNameBox->GetEditText());

	len = strlen(buf);

	if (len == 0)
	{
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(11));
		return;
	}
	else if (len < 4)
	{
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(19));
		return;
	}
	else if (len > MAX_NAME_LENGTH)
		return;

	if (strcmp(buf, "0") == 0)
		return;

	if (FILTERTABLE->IsInvalidCharInclude((unsigned char*)buf))
		//	if( FILTERTABLE->IsInvalidCharacterName((unsigned char*)buf) )
	{
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(14));
		return;
	}
	if (!FILTERTABLE->IsUsableName(buf))
	{
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(14));
		return;
	}
	CItem* pItem = GAMEIN->GetInventoryDialog()->GetCurSelectedItem(SHOPIVN);
	if (pItem)
	{
		m_DBIdx = pItem->GetDBIdx();

		SEND_CHANGENAMEBASE msg;
		msg.Category = MP_ITEMEXT;
		msg.Protocol = MP_ITEMEXT_SHOPITEM_CUSTOMIZING_SYN;
		msg.dwObjectID = HERO->GetID();
		msg.DBIdx = pItem->GetDBIdx();
		strncpy(msg.Name, buf, MAX_NAME_LENGTH + 1);
		NETWORK->Send(&msg, sizeof(msg));

		SetActive(FALSE);
	}
}




