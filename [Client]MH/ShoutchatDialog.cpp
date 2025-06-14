#include "stdafx.h"
#include "ShoutchatDialog.h"
#include "WindowIDEnum.h"
#include "./Interface/cListDialog.h"
#include "./Interface/cStatic.h"
#include "GameIn.h"
#include "ChatDialog.h"
#include "GameResourceManager.h"
#include "ItemManager.h"
#include "cScriptManager.h"
#include "input/UserInput.h"
#include "CharacterRBTN.h"
#include "cWindowManager.h"
#include "chatmanager.h"
CShoutchatDialog::CShoutchatDialog()
{
	m_type = WT_SHOUTCHAT_DLG;
	m_LastMsgTime = 0;
	m_glistenCount2 = 0;
	m_bFlicker = FALSE;
	m_bFlActive = FALSE;
	m_dwFlickerSwapTime = 0;
	SCRIPTMGR->GetImage(90, &iConOne, PFT_JACKPATH);
	SCRIPTMGR->GetImage(90, &iConTwo, PFT_JACKPATH);
	m_ChatActive = TRUE;

	m_SelectedItemIdx = 0;
}
CShoutchatDialog::~CShoutchatDialog()
{
}
void CShoutchatDialog::Linking()
{
	pDlg = WINDOWMGR->GetWindowForID(CHA_DLG);
	m_pMsgStatic = (cStatic*)GetWindowForID(CHA_STC_STATIC);

	m_pMsgListDlg = (cListDialog*)GetWindowForID(CHA_LIST);
	m_pMsgShout = (cListDialog*)GetWindowForID(CHA_STC_LASTMSG);

	m_pMsgListDlg->SetRenderFaceImg(TRUE);
	m_pMsgShout->SetRenderFaceImg(TRUE);

	if (GAMERESRCMNGR->IsLowResolution() == 0)
	{
		GAMEIN->GetShoutchatDlg()->RefreshPosition();
	}
}
DWORD CShoutchatDialog::ActionEvent(CMouse * mouseInfo)
{
	DWORD we = WE_NULL;
	DWORD dwLebar = 420;
	if (!IsActive()) return WE_NULL;
	if (!mouseInfo) return WE_NULL;
	we |= cDialog::ActionEvent(mouseInfo);

	LONG mouseX = (LONG)mouseInfo->GetMouseX();
	LONG mouseY = (LONG)mouseInfo->GetMouseY();

	WORD height = pDlg->cWindow::GetHeight();
	ITEM* pItem1 = m_pMsgListDlg->GetSelectedITEM(mouseInfo);
	ITEM* pItem2 = m_pMsgShout->GetSelectedITEM(mouseInfo);

	if (pItem2)
	if (mouseInfo->LButtonUp())
	if (we && pDlg->PtInWindow(mouseX, mouseY))
	{
		CHARRBTN->RequestMsgForItemLink(pItem2->Value);
		return we;
	}

	if (pItem1)
	if (mouseInfo->LButtonDown() )
	if (we && pDlg->PtInWindow(mouseX, mouseY))
	{
		m_SelectedItemIdx = pItem1->Value;
		return we;
	}
	if (pItem1)
	if (mouseInfo->LButtonUp())
	if (we && pDlg->PtInWindow(mouseX, mouseY))
	{
		if (m_SelectedItemIdx == pItem1->Value)
		{
			CHARRBTN->RequestMsgForItemLink(pItem1->Value);
			return we;
		}
	}

	int Wheel = mouseInfo->GetWheel();
	if (Wheel)
	if (we && pDlg->PtInWindow(mouseX, mouseY))
	{
		if (Wheel > 0)
		{
			m_pMsgListDlg->OnUpwardItem();
		}
		else
		{
			m_pMsgListDlg->OnDownwardItem();
		}
	}

	if (pItem1 || pItem2)
	{
		dwLebar = 420;
	}
	else
	{
		dwLebar = 14;
	}


	if (mouseInfo->LButtonDown() ||
		mouseInfo->LButtonPressed() ||
		mouseInfo->RButtonUp() ||
		mouseInfo->RButtonDown() ||
		mouseInfo->RButtonPressed())
	{
		dwLebar = 420;
	}

	pDlg->SetWH(dwLebar, height);
	return we;
}
void CShoutchatDialog::Process()
{
	if (gCurTime - m_LastMsgTime < 5000)
		return;
	m_LastMsgTime = gCurTime;
}
BOOL CShoutchatDialog::CanActive()
{
	return m_ChatActive;
}
void CShoutchatDialog::SetActive(BOOL val)
{
	SetActiveChat(val);
}
void CShoutchatDialog::SetActiveChat(BOOL val)
{
	GAMEIN->GetShoutchatDlg()->GetWindowForID(CHA_LIST)->SetActive(val);
	if (val)
	{
		GAMEIN->GetShoutchatDlg()->SetHeight(100);
	}
	else
	{
		GAMEIN->GetShoutchatDlg()->SetHeight(20);
	}

	m_ChatActive = val;
	RefreshPosition();
}
void CShoutchatDialog::AddMsg(char* pstr, BOOL Isglisten)
{
	char buf[61] = { 0, };
	DWORD dwItemDBIdx = 0;
	if (strlen(pstr)>0)
	{
		std::string Buffer = pstr;
		DWORD dwItemIdx = 0;
		DWORD dwHeroIdx = 0;
		int Pos = Buffer.find("ItemDBIdx:", 0);
		if (Pos >= 0)
		{
			int PosName = Buffer.find(":", 0);
			std::string CharName = Buffer.substr(0, PosName);
			std::string ItemInfo = Buffer.substr(Pos + 10, Buffer.length());
			int ItemPos = ItemInfo.find('¡ú', 0);
			if (ItemPos >= 0)
			{
				std::string ItemDBIdx = ItemInfo.substr(0, ItemPos);
				std::string ItemIdx = ItemInfo.substr(ItemPos + 1, ItemInfo.length());
				dwItemDBIdx = atoi(ItemDBIdx.c_str());
				dwItemIdx = atoi(ItemIdx.c_str());
				if (dwItemDBIdx>0 && dwItemIdx>0)
				{
					ITEM_INFO * pItem = ITEMMGR->GetItemInfo(dwItemIdx);
					if (pItem)
					{
						if (CharName.length() + strlen(pItem->ItemName)<60)
						{
							memset(pstr, 0, strlen(pstr));
							sprintf(pstr, "%s: [%s]", CharName.c_str(), pItem->ItemName);
						}
					}
				}
			}
		}
	}
	strncpy(buf, pstr, 60);
	if (m_pMsgListDlg && !Isglisten)
	{
		m_pMsgListDlg->AddItem(buf, RGBA_MAKE(217, 206, 247, 255), -1, FALSE, dwItemDBIdx);
	}
	if (m_pMsgShout && Isglisten)
	{
		m_pMsgShout->AddItem(buf, RGBA_MAKE(217, 206, 247, 255), -1, Isglisten, dwItemDBIdx);
		m_pMsgShout->SetFlickerRoar(TRUE);
		m_pMsgListDlg->AddItem(buf, RGBA_MAKE(255, 255, 180, 255), -1, FALSE, dwItemDBIdx);
		SetFlicker(TRUE);
	}
}
void CShoutchatDialog::RefreshPosition()
{
	float absX = GAMEIN->GetChatDialog()->GetAbsX();
	float absY = GAMEIN->GetChatDialog()->GetSheetPosY();
	SetAbsXY((LONG)absX, (LONG)absY - GetHeight());
}
cPtrList * CShoutchatDialog::GetShoutListChat()
{
	return m_pMsgListDlg->GetListItem();
}
cListDialog * CShoutchatDialog::GetShoutListChatAdd()
{
	return m_pMsgListDlg;
}
void CShoutchatDialog::SetFlicker(BOOL bFlicker)
{
	m_bFlicker = bFlicker;
	m_dwFlickerSwapTime = gCurTime;
}
void CShoutchatDialog::FlickerMussangGuage()
{
	if (!m_bFlicker)	return;
	if (gCurTime - m_dwFlickerSwapTime > 20)
	{
		if (m_glistenCount2 >= 450)
		{
			SetFlicker(FALSE);
			m_glistenCount2 = 0;
		}
		else
		{
			m_glistenCount2 = m_glistenCount2 + 50;
			float absX = GAMEIN->GetChatDialog()->GetAbsX();
			float absY = GAMEIN->GetChatDialog()->GetSheetPosY();
			m_pMsgStatic->SetAbsX((LONG)absX - 450 + m_glistenCount2);
			m_pMsgShout->SetAbsXY((LONG)absX - 450 + m_glistenCount2, (LONG)absY - GetHeight());
		}
		m_bFlActive = !m_bFlActive;
		m_dwFlickerSwapTime = gCurTime;
	}
}
void CShoutchatDialog::Render()
{
	VECTOR2 scaleRate, Pos1;
	scaleRate.x = 1;
	scaleRate.y = 4;
	if (m_ChatActive)
	{
		Pos1.x = GAMEIN->GetChatDialog()->GetAbsX();
		Pos1.y = GAMEIN->GetChatDialog()->GetSheetPosY() - (GetHeight() - 20);
		iConOne.RenderSprite(&scaleRate, NULL, 0, &Pos1, 0xffffffff);
		iConTwo.RenderSprite(&scaleRate, NULL, 0, &Pos1, 0xffffffff);
	}
	cDialog::Render();
	FlickerMussangGuage();
}
