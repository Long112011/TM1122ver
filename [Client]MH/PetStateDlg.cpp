




#include "stdafx.h"

#ifdef _MUTIPET_




#include ".\petstatedlg.h"
#include "./interface/cStatic.h"
#include "./interface/cPushupButton.h"
#include "cScriptManager.h"
#include "./Interface/cGuagen.h"
#include "cImageSelf.h"
#include "WindowIDEnum.h"
#include "PetManager.h"
#include "Pet.h"

CPetStateDlg::CPetStateDlg(void)
{
	m_pNameDlg = NULL;
	m_pGradeDlg = NULL;
	//	m_pStateDlg			= NULL;
	m_pFriend = NULL;
	m_pPetFaceImg = NULL;
	memset(m_pSkill, 0, sizeof(cStatic*) * 3);
	memset(m_pSkillPuls, 0, sizeof(cStatic*) * 9);
	m_pFriendGuage = NULL;
	XingJi = 1;
}

CPetStateDlg::~CPetStateDlg(void)
{
}


// void CPetStateDlg::Add( cWindow* window )
// 
// {
// 	if(window->GetType() == WT_PUSHUPBUTTON)
// 		AddTabBtn(curIdx1++, (cPushupButton * )window);
// 	else if(window->GetType() == WT_DIALOG)
// 		AddTabSheet(curIdx2++, window);
// 	else 
// 		cTabDialog::Add(window);
// }

void CPetStateDlg::Linking()
{
	m_pNameDlg = (cStatic*)GetWindowForID(PS_NAME);

	//	m_pPetSealBtn	= (cButton*)GetWindowForID(PS_LOCK_BTN);
	//	m_pPetUseRestBtn	= (cButton*)GetWindowForID(PS_USEREST_BTN);
	//	m_pPetInvenBtn	= (cButton*)GetWindowForID(PS_INVEN_BTN);

	//	m_pDlgToggleBtn	= (cButton*)GetWindowForID(PS_TOGGLE_S_BTN);



	m_pGradeDlg = (cStatic*)GetWindowForID(PS_GRADE);
	//	m_pStateDlg		= (cStatic*)GetWindowForID(PS_STATE);
	m_pFriend = (cStatic*)GetWindowForID(PS_GUAGETEXT1);

	//	m_pStamina		= (cStatic*)pDlg->GetWindowForID(PS_GUAGETEXT2);
	m_pPetFaceImg = (cStatic*)GetWindowForID(PS_IMAGE);	//!!!Selfimage 찾아서..메모리풀로..이미지들 등록해서 쓰자.

	m_pFriendGuage = (cGuagen*)GetWindowForID(PS_GUAGE1);
	//	m_pStaminaGuage	= (cGuagen*)pDlg->GetWindowForID(PS_GUAGE2);

	//	m_pInvenNum		= (cStatic*)pDlg->GetWindowForID(PS_INVENNUM);
	m_pSkill[0] = (cStatic*)GetWindowForID(PS_SKILL1);
	m_pSkill[1] = (cStatic*)GetWindowForID(PS_SKILL2);
	m_pSkill[2] = (cStatic*)GetWindowForID(PS_SKILL3);
	m_pSkill[3] = (cStatic*)GetWindowForID(PS_SKILL4);
	m_pSkill[4] = (cStatic*)GetWindowForID(PS_SKILL5);
	m_pSkill[5] = (cStatic*)GetWindowForID(PS_SKILL6);
	for (int i = 0; i < 16; ++i)
	{
		m_pSkillPuls[i] = (cStatic*)GetWindowForID(PSP_SKILL1 + i);
	}

}
void CPetStateDlg::Render()
{
	if (!IsActive())
		return;
	cDialog::Render();
	cImage im;
	SCRIPTMGR->GetImage(418, &im);//槿섬暠튬鞫刻
	VECTOR2 pos;
	pos.x = m_pPetFaceImg->GetAbsX();
	pos.y = m_pPetFaceImg->GetAbsY();
	VECTOR2 scale = { 0.5, 0.5 };
	for (int i = 0; i < XingJi; ++i)
	{
		im.RenderSprite(&scale, NULL, 0, &pos, RGBA_MERGE(0xffffff, m_alpha * m_dwOptionAlpha / 100));
		pos.x += 6;
	}
}
void CPetStateDlg::OnActionEvent(LONG lId, void* p, DWORD we)
{

	if (we & WE_BTNCLICK)
	{

		// 		if( PS_LOCK_BTN == lId )
		// 		{
		// 			PETMGR->SendPetSealMsg();
		// 		}
		// 		else if( PS_USEREST_BTN == lId )
		// 		{
		// 			if (FALSE == PETMGR->IsSummonPet())	return;
		// 		//	PETMGR->SendPetRestMsg(!PETMGR->GetCurSummonPet()->IsPetRest());
		// 		}
		// 		else if( PS_INVEN_BTN == lId )
		// 		{
		// 			PETMGR->OpenPetInvenDlg();
		// 
		// 		}
		// 		else if( PS_TOGGLE_S_BTN == lId )
		// 		{
		// 			PETMGR->TogglePetStateDlg();
		// 		}

	}
}

void CPetStateDlg::SetBtnClick(int btnKind)

{/*
	if( ePetRestBtn == btnKind )
	{
		m_pPetUseBtn->SetPush(FALSE);

		m_pPetRestBtn->SetPush(TRUE);
	}

	else if( ePetUseBtn == btnKind )
	{
		m_pPetUseBtn->SetPush(TRUE);
		m_pPetRestBtn->SetPush(FALSE);
	}*/
}

#else



#include ".\petstatedlg.h"
#include "./interface/cStatic.h"
#include "./interface/cPushupButton.h"

#include "./Interface/cGuagen.h"
#include "cImageSelf.h"
#include "WindowIDEnum.h"
#include "PetManager.h"
#include "Pet.h"

CPetStateDlg::CPetStateDlg(void)
{
	m_pNameDlg			= NULL;
	m_pGradeDlg			= NULL;
	m_pStateDlg			= NULL;
	m_pFriend			= NULL;
	m_pStamina			= NULL;


	m_pPetFaceImg		= NULL;

	m_pInvenNum			= NULL;
	memset(m_pSkill, 0, sizeof(cStatic*)*3);


	m_pFriendGuage		= NULL;
	m_pStaminaGuage		= NULL;

	m_pPetSealBtn		= NULL;
	m_pPetUseRestBtn	= NULL;
	m_pPetInvenBtn		= NULL;

	m_pDlgToggleBtn		= NULL;
}

CPetStateDlg::~CPetStateDlg(void)
{
}


void CPetStateDlg::Add( cWindow* window )

{
	if(window->GetType() == WT_PUSHUPBUTTON)
		AddTabBtn(curIdx1++, (cPushupButton * )window);
	else if(window->GetType() == WT_DIALOG)
		AddTabSheet(curIdx2++, window);
	else 
		cTabDialog::Add(window);
}

void CPetStateDlg::Linking()
{
	m_pNameDlg		= (cStatic*)GetWindowForID(PS_NAME);

	m_pPetSealBtn	= (cButton*)GetWindowForID(PS_LOCK_BTN);
	m_pPetUseRestBtn	= (cButton*)GetWindowForID(PS_USEREST_BTN);
	m_pPetInvenBtn	= (cButton*)GetWindowForID(PS_INVEN_BTN);

	m_pDlgToggleBtn	= (cButton*)GetWindowForID(PS_TOGGLE_S_BTN);


	cDialog* pDlg = (cDialog*)GetTabSheet(0);

	m_pGradeDlg		= (cStatic*)pDlg->GetWindowForID(PS_GRADE);
	m_pStateDlg		= (cStatic*)pDlg->GetWindowForID(PS_STATE);
	m_pFriend		= (cStatic*)pDlg->GetWindowForID(PS_GUAGETEXT1);

	m_pStamina		= (cStatic*)pDlg->GetWindowForID(PS_GUAGETEXT2);
	m_pPetFaceImg	= (cStatic*)pDlg->GetWindowForID(PS_IMAGE);	//!!!Selfimage 찾아서..메모리풀로..이미지들 등록해서 쓰자.

	m_pFriendGuage	= (cGuagen*)pDlg->GetWindowForID(PS_GUAGE1);
	m_pStaminaGuage	= (cGuagen*)pDlg->GetWindowForID(PS_GUAGE2);

	pDlg = (cDialog*)GetTabSheet(1);
	m_pInvenNum		= (cStatic*)pDlg->GetWindowForID(PS_INVENNUM);
	m_pSkill[0]		= (cStatic*)pDlg->GetWindowForID(PS_SKILL1);	
	m_pSkill[1]		= (cStatic*)pDlg->GetWindowForID(PS_SKILL2);	
	m_pSkill[2]		= (cStatic*)pDlg->GetWindowForID(PS_SKILL3);	
}

void CPetStateDlg::OnActionEvent(LONG lId, void * p, DWORD we)
{

	if( we & WE_BTNCLICK )
	{

		if( PS_LOCK_BTN == lId )
		{
			PETMGR->SendPetSealMsg();
		}
		else if( PS_USEREST_BTN == lId )
		{
			if(NULL == PETMGR->GetCurSummonPet())	return;
			PETMGR->SendPetRestMsg(!PETMGR->GetCurSummonPet()->IsPetRest());
		}
		else if( PS_INVEN_BTN == lId )
		{
			PETMGR->OpenPetInvenDlg();

		}


		else if( PS_TOGGLE_S_BTN == lId )

		{
			PETMGR->TogglePetStateDlg();

		}

	}
}

void CPetStateDlg::SetBtnClick(int btnKind)

{/*
	if( ePetRestBtn == btnKind )
	{
		m_pPetUseBtn->SetPush(FALSE);

		m_pPetRestBtn->SetPush(TRUE);
	}

	else if( ePetUseBtn == btnKind )
	{
		m_pPetUseBtn->SetPush(TRUE);
		m_pPetRestBtn->SetPush(FALSE);
	}*/
}



#endif // _MUTIPET_
