#include "stdafx.h"
#include "RankButtonDialog.h"
#include "cWindowManager.h"
#include "cScriptManager.h"
#include "WindowIDEnum.h"
#include "TopIcon.h"
#include "TopDialog.h"
#include "TopMunpDialog.h"
#include "GameIn.h"
#include "LuckerDialog.h"
#include "FameGoldDialog.h"
#include "chatmanager.h"
#include "VipDialog.h"
#include "KeySettingTipDlg.h" // 记得在 RankButtonDialog.cpp 顶部 include

CRankButtonDialog::CRankButtonDialog()
{
	pDlg=NULL;
	pHeroBtn=NULL;
	pMunpBtn=NULL;
	pMapInfo=NULL;
	pVipDlgBtn = NULL;//vip dialog button
	pEventBtn = NULL;//event button
	SCRIPTMGR->GetImage( 45, &m_TooltipImage, PFT_JACKPATH );

	MapExp=1.f;
	MapSkillPoint=1.f;
	MapGoldRate=1.f;
	MapRegenRate=1.f;
	MapDropRate=1.f;

	NotifyIcon[HRANK]=NULL;
	NotifyIcon[GRANK]=NULL;

	SCRIPTMGR->GetImage(34, &m_ScrollImg[0], PFT_JACKPATH);//60
	SCRIPTMGR->GetImage(35, &m_ScrollImg[1], PFT_JACKPATH);//61
	SCRIPTMGR->GetImage(34, &m_ScrollImg[2], PFT_JACKPATH);//62

	m_CurScrollImage[0] = 0;
	m_CurScrollImage[1] = 0;

	m_bCounter[0]	= 0;
	m_bFlicker[0]	= FALSE;
	m_bFlActive[0]	= FALSE;
	m_bSwapTime[0]	= 0;

	m_bCounter[1]	= 0;
	m_bFlicker[1]	= FALSE;
	m_bFlActive[1]	= FALSE;
	m_bSwapTime[1]	= 0;

	mScale.x = 1 ;
	mScale.y = 1 ;

	dwColor = RGBA_MERGE(0x00ffffff, 100);
	//RGBA_MERGE(0x00ffffff, 255 )
}
CRankButtonDialog::~CRankButtonDialog()
{
	pDlg=NULL;
	pHeroBtn=NULL;
	pMunpBtn=NULL;
	pMapInfo=NULL;
	pVipDlgBtn = NULL;//vip dialog button
	pEventBtn = NULL;//event button

	MapExp=1.f;
	MapSkillPoint=1.f;
	MapGoldRate=1.f;
	MapRegenRate=1.f;
	MapDropRate=1.f;
}
void CRankButtonDialog::Linking()
{
	cImage imgToolTip;
	SCRIPTMGR->GetImage(63, &imgToolTip, PFT_HARDPATH);

	pDlg = WINDOWMGR->GetWindowForID(RANKBUTTON_DIALOG);
	pMunpBtn =(cButton *)GetWindowForID(TOP_MUNP_BTN);
	pHeroBtn =(cButton *)GetWindowForID(TOP_HERO_BTN);
	pMapInfo =(cStatic *)GetWindowForID(TOP_MAP_INFO);

	pVipDlgBtn = (cButton*)GetWindowForID(CONTROL_VIPBTNONLINE);
	pEventBtn = (cButton*)GetWindowForID(CONTROL_EVENT);
//	pVipDlgBtn->SetToolTip("VIP系统", RGBA_MAKE(255, 255, 255, 255), &imgToolTip, TTCLR_ITEM_CANEQUIP);

	NotifyIcon[HRANK] = new cStatic;
	if( NotifyIcon[HRANK] )
	{
		NotifyIcon[HRANK]->Init( pHeroBtn->GetRelX()-5, pHeroBtn->GetRelY()-1, 0, 0, NULL, -1 );		
		NotifyIcon[HRANK]->SetImageRGB(dwColor);
		NotifyIcon[HRANK]->SetScale(&mScale);
		Add( NotifyIcon[HRANK] );
	}

	NotifyIcon[GRANK] = new cStatic;
	if( NotifyIcon[GRANK] )
	{
		NotifyIcon[GRANK]->Init( pMunpBtn->GetRelX(), pMunpBtn->GetRelY(), 0, 0, NULL, -1 );		
		NotifyIcon[GRANK]->SetImageRGB(dwColor);
		NotifyIcon[GRANK]->SetScale(&mScale);
		Add( NotifyIcon[GRANK] );
	}


//	pVipDlgBtn->SetToolTip("VIPϵͳ", RGBA_MAKE(255, 255, 255, 255), &imgToolTip, TTCLR_ITEM_CANEQUIP);

	//SetFlicker(true);

	
}
void CRankButtonDialog::CurrentChannelRateMsg(MSG_CHANNEL_MSGJACK* pInfo)
{
	MapExp			=MapExp			*pInfo->ExpRate;
	MapSkillPoint	=MapSkillPoint	*pInfo->AbilityExpRate;
	MapGoldRate		=MapGoldRate	*pInfo->MoneyRate;
	MapRegenRate	=MapRegenRate	*pInfo->RegenRate;
	MapDropRate		=MapDropRate	*pInfo->DropRate;
}

void CRankButtonDialog::SetShow(bool p)
{
	cDialog::SetActive(p);
}

void CRankButtonDialog::Render()
{
	cDialog::Render();
	HRankRendering();
	GRankRendering();
}
void CRankButtonDialog::SetFlicker( BOOL bFlicker , BYTE bFlag)
{
	switch(bFlag)
	{
	case HRANK: 
		NotifyIcon[HRANK]->SetActive(bFlicker);
		m_bFlicker[0] = bFlicker;
		m_bSwapTime[0] = gCurTime;
	break;
	case GRANK: 
		NotifyIcon[GRANK]->SetActive(bFlicker);
		m_bFlicker[1] = bFlicker;
		m_bSwapTime[1] = gCurTime;
	break;
	}
}
void CRankButtonDialog::HRankRendering()
{
	if(!m_bFlicker[0])	return;

	static int TICK_PER_IMAGE = 350;
	m_CurScrollImage[0]+=gTickTime;
	int curImage = m_CurScrollImage[0] % (TICK_PER_IMAGE*3);
	curImage /= TICK_PER_IMAGE;

	if( gCurTime - m_bSwapTime[0] > 50 )
	{	
		if(m_bCounter[0]>=254)
		{
			m_bCounter[0]=0;
		}
		else
		{
			m_bCounter[0]=m_bCounter[0]+25;
			NotifyIcon[HRANK]->SetBasicImage(&m_ScrollImg[curImage]);
			NotifyIcon[HRANK]->SetAlpha(m_bCounter[0]);
		}
		m_bFlActive[0] = !m_bFlActive[0];
		m_bSwapTime[0] = gCurTime;
	}
}
void CRankButtonDialog::GRankRendering()
{
	if(!m_bFlicker[1])	return;

	static int TICK_PER_IMAGE = 350;
	m_CurScrollImage[1]+=gTickTime;
	int curImage = m_CurScrollImage[1] % (TICK_PER_IMAGE*3);
	curImage /= TICK_PER_IMAGE;

	if( gCurTime - m_bSwapTime[1] > 50 )
	{	
		if(m_bCounter[1]>=254)
		{
			m_bCounter[1]=0;
		}
		else
		{
			m_bCounter[1]=m_bCounter[1]+25;
			NotifyIcon[GRANK]->SetBasicImage(&m_ScrollImg[curImage]);
			NotifyIcon[GRANK]->SetAlpha(m_bCounter[1]);
		}
		m_bFlActive[1] = !m_bFlActive[1];
		m_bSwapTime[1] = gCurTime;
	}
}









DWORD CRankButtonDialog::ActionEvent( CMouse* mouseInfo )
{
	DWORD we = cDialog::ActionEvent( mouseInfo );
	if( !m_bActive ) return we;

	if( we & WE_MOUSEOVER )
	{
		DWORD dwColor=RGB_HALF( 255, 255, 0 );

		char bTemp[64] = {0,};

		ZeroMemory(&bTemp, sizeof(bTemp));
		pMapInfo->SetToolTip(CHATMGR->GetChatMsg(2608),dwColor,&m_TooltipImage);
		pMapInfo->AddToolTipLine("");

		pMapInfo->AddToolTipLine(CHATMGR->GetChatMsg(2609),dwColor);

		sprintf(bTemp,CHATMGR->GetChatMsg(2610),MapExp);
		pMapInfo->AddToolTipLine(bTemp,dwColor);

		sprintf(bTemp,CHATMGR->GetChatMsg(2611),MapSkillPoint);
		pMapInfo->AddToolTipLine(bTemp,dwColor);

		sprintf(bTemp,CHATMGR->GetChatMsg(2612),MapGoldRate);
		pMapInfo->AddToolTipLine(bTemp,dwColor);

		//sprintf(bTemp,CHATMGR->GetChatMsg(2613),MapRegenRate);
		//pMapInfo->AddToolTipLine(bTemp,dwColor);

		sprintf(bTemp,CHATMGR->GetChatMsg(2614),MapDropRate);
		pMapInfo->AddToolTipLine(bTemp,dwColor);
	}
	return we;
}
void CRankButtonDialog::OnActionEvent(LONG lId, void * p, DWORD we)
{
	if( GAMEIN->GetGameInInitKind() == eGameInInitKind_SuryunEnter || 
		GAMEIN->GetGameInInitKind() == eGameInInitKind_EventMapEnter )
		return;
	if(we & WE_BTNCLICK)
	{
		switch(lId)
		{
		case TOP_HERO_BTN:
			{
				CTopDialog * pHandler = (CTopDialog*)GAMEIN->GetTopDialog();

				if( !pHandler ) return;

				if( pHandler->IsActive() )
				{
					pHandler->SetActive( FALSE );
				}
				else
				{
					pHandler->InitHeroList();
					pHandler->ShowAllList();
					pHandler->SetActive( TRUE );
					pHandler->OpenLastTab();
				}
				
			}
			break;
		case TOP_MUNP_BTN:
			{
				CTopMunpDialog* pHandler = (CTopMunpDialog*)GAMEIN->GetTopMunpDialog();

				if( !pHandler ) return;

				if( pHandler->IsActive() )
				{
					pHandler->SetActive( FALSE );
				}
				else
				{
					pHandler->InitMunpList();
					pHandler->ShowMunpList();
					pHandler->SetActive( TRUE );
					pHandler->OpenLastTab();
				}
			}
			break;
		case CONTROL_EVENT:   // ← 活动按钮
		{
			auto* tip = GAMEIN->GetKeySettingTipDlg();
			if (!tip) break;

			// 想“按一次开，再按一次关”→ toggle：
			bool nextActive = !tip->IsActive();
			tip->SetActive(nextActive);
			if (nextActive) {
				tip->SetMode(1);         // 0=KeySetting1.tga, 1=KeySetting2.tga
				// 或者如果你实现了 ShowTip： tip->ShowTip(1, true);
				// 确保不被遮挡（如需）
				// WINDOWMGR->MoveToTop(tip);
			//	CHATMGR->AddMsg(CTC_SYSMSG, "活动图开启"); // 可选：聊天提示
			}
			else {
			//	CHATMGR->AddMsg(CTC_SYSMSG, "活动图关闭"); // 可选
			}
			break;
		}
		case CONTROL_VIPBTNONLINE:
		{
			VipDialog* pHandler = (VipDialog*)GAMEIN->GetVipDialog();

			if (!pHandler)
			{
				//OutputDebugStringA("[VIP Button] Failed to open: VipDialog handler not found.\n");
				//CHATMGR->AddMsg(CTC_SYSMSG, "VIP open failed");
				return;
			}

			if (pHandler->IsActive())
			{
				pHandler->SetActive(FALSE);
//				OutputDebugStringA("[VIP Button] VIP dialog closed.\n");
	//			CHATMGR->AddMsg(CTC_SYSMSG, "VIP dialog closed");
			}
			else
			{
				pHandler->SetActive(TRUE);
				GAMEIN->UpdataGoldMoney();   // 在线充值刷新元宝
				pHandler->SetVipItem();      // 设置 VIP 物品信息

		//		OutputDebugStringA("[VIP Button] VIP dialog opened successfully.\n");
			//	CHATMGR->AddMsg(CTC_SYSMSG, "VIP dialog opened");
			}

			//this->SetActive(FALSE);
		}
		break;


		
		case TOP_WANT_BTN:
			{
				CFameGoldDialog * pHandler = (CFameGoldDialog*)GAMEIN->GetFameGoldDialog();
				if( !pHandler ) return;

#ifdef _JSKEN_
				pHandler->JustForKen();
#else				
				if( pHandler->IsActive() )
				{
					pHandler->SetActive( FALSE );
				}
				else
				{
					pHandler->SetFame();
					pHandler->SetActive( TRUE );
				}
#endif
			}
			break;
		}
	}
}