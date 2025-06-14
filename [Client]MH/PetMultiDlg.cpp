#include "stdafx.h"
#include ".\petmultidlg.h"
#include "./interface/cGuagen.h"
#include "./interface/cStatic.h"
#include "./interface/cButton.h"
#include "./interface/cPushupButton.h"
#include "WindowIDEnum.h"
#include "cWindowManager.h"
#include "Pet.h"
#include "PetManager.h"
#include "chatmanager.h"
CPetMultiDlg::CPetMultiDlg(void)
{
	pDlg = NULL;
	m_pNameDlg = NULL;

	m_pDetailBtn = NULL; 
	m_pPetInvenBtn = NULL;
	
	memset(m_pSelectPet, 0, sizeof(cPushupButton*) * 3);
	memset(m_pPetFaceImg, 0, sizeof(cStatic*) * 3);	
	memset(m_pStaminaGuage, 0, sizeof(cGuagen*) * 3);

	pPetMultiTable.Initialize(1024);
}
CPetMultiDlg::~CPetMultiDlg(void)
{
	PET_MULTI_DLG* pMessageInfo = NULL;
	pPetMultiTable.SetPositionHead();
	while (pMessageInfo = pPetMultiTable.GetData())
		pMessageInfo = NULL;
	pPetMultiTable.RemoveAll();
}

void CPetMultiDlg::Linking()
{
	pDlg = WINDOWMGR->GetWindowForID(PM_DIALOG);
	m_pNameDlg = (cStatic*)GetWindowForID(PM_NAME);

	m_pDetailBtn	= (cButton*)GetWindowForID(PM_DETAIL_BTN);
	m_pPetInvenBtn	= (cButton*)GetWindowForID(PM_INVEN_BTN);

	//for (int i = 0; i<3; i++)
	//{
	//	m_pSelectPet[i] = (cPushupButton*)GetWindowForID(PM_PUB_SELPET1 + i);
	//	m_pSelectPet[i]->SetPush(false);
	//}
	m_pSelectPet[0] = (cPushupButton*)pDlg->GetWindowForID(PM_PUB_SELPET1);
	m_pSelectPet[1] = (cPushupButton*)pDlg->GetWindowForID(PM_PUB_SELPET2);
	m_pSelectPet[2] = (cPushupButton*)pDlg->GetWindowForID(PM_PUB_SELPET3);

	m_pPetFaceImg[0] = (cStatic*)pDlg->GetWindowForID(PM_PETIMG1);
	m_pPetFaceImg[1] = (cStatic*)pDlg->GetWindowForID(PM_PETIMG2);
	m_pPetFaceImg[2] = (cStatic*)pDlg->GetWindowForID(PM_PETIMG3);

	m_pStaminaGuage[0] = (cGuagen*)pDlg->GetWindowForID(PM_GUAGE_ST1);
	m_pStaminaGuage[1] = (cGuagen*)pDlg->GetWindowForID(PM_GUAGE_ST2);
	m_pStaminaGuage[2] = (cGuagen*)pDlg->GetWindowForID(PM_GUAGE_ST3);		
	for (int i = 0; i < 3; i++)
	{
		m_pStaminaGuage[i]->SetValue(0);
	}
}
void CPetMultiDlg::OnActionEvent(LONG lId, void * p, DWORD we)
{
	if( we & WE_BTNCLICK )
	{
		if (PM_INVEN_BTN == lId)
		{
			PETMGR->OpenPetInvenDlg();
		}
		else if (PM_DETAIL_BTN == lId)
		{
			//detail dialog start here
		}
	}
	/*else */if (we & WE_PUSHDOWN)
	{
		if (PM_PUB_SELPET1 <= lId && lId <= PM_PUB_SELPET3)
		{
			//m_pNameDlg->SetStaticText(" ");
			for (int i = 0; i<3; i++)
			{
				m_pSelectPet[i]->SetPush(false);
			}
			m_pSelectPet[lId - PM_PUB_SELPET1]->SetPush(true);
		}
	}
	/*else */if (we & WE_PUSHUP)
	{
		if (PM_PUB_SELPET1 <= lId && lId <= PM_PUB_SELPET3)
		{
			m_pSelectPet[lId - PM_PUB_SELPET1]->SetPush(true);

			//m_pNameDlg->SetStaticText("TESTTT");
		}
	}
}
DWORD CPetMultiDlg::InitPet(DWORD idx, DWORD ObjId)
{
	PET_MULTI_DLG* pInfo = pPetMultiTable.GetData(idx);
	if (!pInfo)
	{
		if (FALSE == CheckSeeSlot(ObjId))
		{
			pInfo = new PET_MULTI_DLG;
			pInfo->Slot = idx;
			pInfo->PetIdx = ObjId;
			pPetMultiTable.Add(pInfo, pInfo->Slot);

			m_pPetFaceImg[idx]->SetActive(TRUE);
			return idx;
		}
		else
		{
			return -1;
		}
		
	}
	return -1;
}
void CPetMultiDlg::SeeSlot(DWORD ObjId)
{
	for (int i = 0; i < 3; ++i)
	{
		PET_MULTI_DLG* pInfo = pPetMultiTable.GetData(i);
		if (pInfo)
		{
			if (pInfo->PetIdx == ObjId)
			{
				m_pPetFaceImg	[pInfo->Slot]->SetActive(FALSE);
				m_pStaminaGuage	[pInfo->Slot]->SetValue(0);
				pPetMultiTable.Remove(pInfo->Slot);
			}
		}
		else
		{
			this->SetActive(FALSE);
		}
	}
}
BOOL CPetMultiDlg::CheckSeeSlot(DWORD ObjId)
{
	for (int i = 0; i < 3; ++i)
	{
		PET_MULTI_DLG* pInfo = pPetMultiTable.GetData(i);
		if (pInfo)
		{
			if (pInfo->PetIdx == ObjId)
			{
				return TRUE;
			}
		}		
	}
	return FALSE;
}
void CPetMultiDlg::SeeALL()
{
	for (int i = 0; i < 3; ++i)
	{
		PET_MULTI_DLG* pInfo = pPetMultiTable.GetData(i);
		if (pInfo)
		{
			CHATMGR->AddMsg(CTC_SYSMSG, "PetIdx:%d Slot:%d", pInfo->PetIdx, pInfo->Slot);
		}
		else
		{
			CHATMGR->AddMsg(CTC_SYSMSG, "none");
		}
	}
}