#include "stdafx.h"
#include "ItemGambleManager.h"
#include "WindowIDEnum.h"
#include "cScriptManager.h"     // 2
#include "ObjectManager.h"
#include "ChatManager.h"
#include "./Interface/cIconDialog.h"
#include "./Interface/cWindowManager.h"
#include "./Interface/cStatic.h"
#include "./Interface/cIcon.h"
#include "cSpin.h"


CItemGambleDialog::CItemGambleDialog()
{
   pDlg=NULL;

   for(int i=0;i<6;i++)
   {
	   m_pIconDlg[i]=NULL;

	   m_icon[i]=NULL;
   }
   
}
CItemGambleDialog::~CItemGambleDialog()
{
   pDlg=NULL;

   for(int i=0;i<6;i++)
   {
	   m_pIconDlg[i]=NULL;

	   m_icon[i]=NULL;
   }
}

void  CItemGambleDialog::Linking()
{
	LoadNumberListImage();  // 载入用到的数字

    pDlg = WINDOWMGR->GetWindowForID( ITEMGAMBLE_DLG );

	for(int i=0;i<6;i++)
	{ 
		m_pIconDlg[i] = (cIconDialog*)GetWindowForID(ITEMGAMBLE_ICON1+i);

		m_icon[i] = new cIcon;

		m_icon[i]->Init(0,0,32,32,&m_NumberList[0],IG_DEALITEM_START+6000+i);   // 

		m_icon[i]->SetDisable(FALSE);

		m_icon[i]->SetData(0);

		m_pIconDlg[i]->AddIcon(0,m_icon[i]);
	}
}

void CItemGambleDialog::LoadNumberListImage()
{
	for( int i = 0 ; i < NUMBERLISTIMGMAX ; ++i )
	{
		SCRIPTMGR->GetImage( 49 + i, &m_NumberList[i], PFT_HARDPATH );
	}
}

void CItemGambleDialog::SetGamblePublic(int Number[3])
{
	for(int i=0;i<3;i++)
	{

		m_pIconDlg[i]->DeleteIconAll();

		m_icon[i] = new cIcon;

		m_icon[i]->Init(0,0,32,32,&m_NumberList[Number[i]],IG_DEALITEM_START+6000+i);   // 

		m_icon[i]->SetDisable(FALSE);

		m_icon[i]->SetData(0);

		m_pIconDlg[i]->AddIcon(0,m_icon[i]);

	}
	delete Number;
}

void CItemGambleDialog::SetGamblePrivate(int Number[3])
{
	for(int i=3;i<6;i++)
	{
		m_pIconDlg[i]->DeleteIconAll();

		m_icon[i] = new cIcon;

		m_icon[i]->Init(0,0,32,32,&m_NumberList[Number[i-3]],IG_DEALITEM_START+6000+i);   // 

		m_icon[i]->SetDisable(FALSE);

		m_icon[i]->SetData(0);

		m_pIconDlg[i]->AddIcon(0,m_icon[i]);

	}
	delete Number;
}

void CItemGambleDialog::SendGambleInfo()
{
	 int Number1,Number2,Number3;

	 Number1= atoi( ((cSpin*)pDlg->GetWindowForID( ITEMGAMBLE_SPIN1 ))->GetEditText() );

	 Number2= atoi( ((cSpin*)pDlg->GetWindowForID( ITEMGAMBLE_SPIN2 ))->GetEditText() );

	 Number3= atoi( ((cSpin*)pDlg->GetWindowForID( ITEMGAMBLE_SPIN3 ))->GetEditText() );

	 if(CheckGambleNumber(Number1) && CheckGambleNumber(Number2) && CheckGambleNumber(Number3))
	 {
		 MSG_DWORD3 msg;
		 msg.Category=MP_ITEM;
		 msg.Protocol=MP_ITEM_GAMBLE_SEND_SYN;
		 msg.dwObjectID=HERO->GetID();
		 msg.dwData1= Number1;
		 msg.dwData2= Number2;
		 msg.dwData3= Number3;
		 NETWORK->Send(&msg, sizeof(msg));
	 }
	 else
	 {
		  CHATMGR->AddMsg(CTC_SYSMSG,CHATMGR->GetChatMsg(2131));  // 2015-
	 }
}

BOOL CItemGambleDialog::CheckGambleNumber(int Number)
{
	if(Number<=6 && Number>0)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}