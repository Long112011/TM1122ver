




#include "stdafx.h"



#ifdef _MUTIPET_











#include "stdafx.h"
#include ".\petstateminidlg.h"
#include "./interface/cGuagen.h"
#include "./interface/cStatic.h"
#include "./interface/cButton.h"
#include "WindowIDEnum.h"
#include "ObjectGuagen.h"
#include "Pet.h"
#include "PetManager.h"
#include "GameIn.h"
#include "ItemManager.h"
CPetStateMiniDlg::CPetStateMiniDlg(void)
{

	m_pNameDlg = NULL;
	m_pStateDlg = NULL;
	m_pFriend = NULL;
	m_pStamina = NULL;

	//	m_pFriendGuage		= NULL;
	//	m_pStaminaGuage		= NULL;
	for (int i = 0; i < 3; ++i)
	{
		m_pFriendGuage[i] = NULL;
	}
	m_pPetUseRestBtn = NULL;
	m_pPetInvenBtn = NULL;


	m_pDlgToggleBtn = NULL;
}


CPetStateMiniDlg::~CPetStateMiniDlg(void)
{
}

void CPetStateMiniDlg::Linking()
{
	m_pNameDlg = (cStatic*)GetWindowForID(PSMN_NAME);
	m_pStateDlg = (cStatic*)GetWindowForID(PSMN_STATE);
	m_pFriend = (cStatic*)GetWindowForID(PSMN_GUAGETEXT1);
	m_pStamina = (cStatic*)GetWindowForID(PSMN_GUAGETEXT2);



	//	m_pFriendGuage	= (cGuagen*)GetWindowForID(PSMN_GUAGE1);
	//	m_pStaminaGuage = (cGuagen*)GetWindowForID(PSMN_GUAGE2);



	PetIcon = (cIconGridDialog*)GetWindowForID(PSMN_lIcon);
	m_pPetUseRestBtn = (cButton*)GetWindowForID(PSMN_USEREST_BTN);
	m_pPetInvenBtn = (cButton*)GetWindowForID(PSMN_INVEN_BTN);

	m_pDlgToggleBtn = (cButton*)GetWindowForID(PSMN_TOGGLE_B_BTN);
	for (int i = 0; i < 3; ++i)
	{
		m_pFriendGuage[i] = (cGuagen*)GetWindowForID(PSMN_GUAGE1 + i);
		((CObjectGuagen*)m_pFriendGuage[i])->SetValue(0, 0);
		//m_Pet[i] = NULL;
		m_PetIcon[i].Init(0, 0, 0, 0, 0);
	}
}

void CPetStateMiniDlg::OnActionEvent(LONG lId, void* p, DWORD we)
{
	if (we & WE_BTNCLICK)
	{
		if (PSMN_TOGGLE_B_BTN == lId)
		{

			PETMGR->TogglePetStateDlg();
		}
		else if (PSMN_USEREST_BTN == lId)
		{
			//if(NULL == PETMGR->GetCurSummonPet())	return;
			//PETMGR->SendPetRestMsg(!PETMGR->GetCurSummonPet()->IsPetRest());

		}
		else if (PSMN_INVEN_BTN == lId)
		{
			PETMGR->OpenPetInvenDlg();
		}
	}
	if (we & WE_LBTNDBLCLICK || we & WE_LBTNCLICK)
	{
		if (PSMN_lIcon == lId)
		{
			WORD index = (WORD)PetIcon->GetCurSelCellPos();
			PETMGR->SetSelectPet(index);
		}
	}
}

void CPetStateMiniDlg::AddPetIcon(CPet** pPet)
{
	if (!pPet)
		return;
	if (PetIcon)
	{
		for (int i = 0; i < 3; ++i)
		{
			RemovePetIcon(i);
			if (pPet[i])
			{
				if (PetIcon->IsAddable(i) == TRUE)
				{
					cImage  image;
					ITEM_INFO* pInfo = (ITEM_INFO*)ITEMMGR->GetItem(pPet[i]->GetPetSommonItemDBIdx())->GetItemInfo();
					ITEMMGR->GetIconImage(pInfo->ItemIdx, &image);
					m_PetIcon[i].Init(0, 0, 0, 0, &image, 0);
					PetIcon->AddIcon(i, &m_PetIcon[i]);
				}
			}
		}
	}
}
void CPetStateMiniDlg::Render()
{
	if (!IsActive())
		return;
	cDialog::Render();
	for (int i = 0; i < 3; ++i)
	{
		if (!PetIcon->IsAddable(i))
			m_PetIcon[i].Render();
	}
}
void CPetStateMiniDlg::RemovePetIcon(BYTE i)
{
	cIcon* picon = NULL;
	PetIcon->DeleteIcon(i, (cIcon**)&picon);
	m_PetIcon[i].Init(0, 0, 0, 0, 0, 0);
}
void CPetStateMiniDlg::SetPetFriendShipGuage(BYTE i, float friendShip)
{

	((CObjectGuagen*)m_pFriendGuage[i])->SetValue(friendShip, 0);

}

#endif //  _MUTIPET_

#ifndef  _MUTIPET_




#include ".\petstateminidlg.h"
#include "./interface/cGuagen.h"
#include "./interface/cStatic.h"
#include "./interface/cButton.h"
#include "WindowIDEnum.h"

#include "Pet.h"
#include "PetManager.h"

CPetStateMiniDlg::CPetStateMiniDlg(void)
{

	m_pNameDlg			= NULL;
	m_pStateDlg			= NULL;
	m_pFriend			= NULL;
	m_pStamina			= NULL;

	m_pFriendGuage		= NULL;
	m_pStaminaGuage		= NULL;

	m_pPetUseRestBtn	= NULL;
	m_pPetInvenBtn		= NULL;


	m_pDlgToggleBtn		= NULL;
}


CPetStateMiniDlg::~CPetStateMiniDlg(void)
{
}

void CPetStateMiniDlg::Linking()
{
	m_pNameDlg		= (cStatic*)GetWindowForID(PSMN_NAME);
	m_pStateDlg		= (cStatic*)GetWindowForID(PSMN_STATE);
	m_pFriend		= (cStatic*)GetWindowForID(PSMN_GUAGETEXT1);
	m_pStamina		= (cStatic*)GetWindowForID(PSMN_GUAGETEXT2);



	m_pFriendGuage	= (cGuagen*)GetWindowForID(PSMN_GUAGE1);
	m_pStaminaGuage = (cGuagen*)GetWindowForID(PSMN_GUAGE2);




	m_pPetUseRestBtn	= (cButton*)GetWindowForID(PSMN_USEREST_BTN);
	m_pPetInvenBtn	= (cButton*)GetWindowForID(PSMN_INVEN_BTN);

	m_pDlgToggleBtn = (cButton*)GetWindowForID(PSMN_TOGGLE_B_BTN);
}

void CPetStateMiniDlg::OnActionEvent(LONG lId, void *p, DWORD we)
{
	if( we & WE_BTNCLICK )
	{
		if( PSMN_TOGGLE_B_BTN == lId )
		{

			PETMGR->TogglePetStateDlg();
		}
		else if( PSMN_USEREST_BTN == lId )


		{
			if(NULL == PETMGR->GetCurSummonPet())	return;
			PETMGR->SendPetRestMsg(!PETMGR->GetCurSummonPet()->IsPetRest());

		}
		else if( PSMN_INVEN_BTN == lId )
		{

			PETMGR->OpenPetInvenDlg();
		}


	}		

}





#endif // _MUTIPET_