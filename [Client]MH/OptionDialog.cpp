#include "stdafx.h"
#include "OptionDialog.h"
#include "WindowIDEnum.h"
#include "./Interface/cWindowManager.h" 
#include "./Interface/cPushupButton.h"
#include "./Interface/cGuageBar.h"
#include "./Interface/cCheckBox.h"
#include "./interface/cStatic.h"
#include "./interface/cCheckbox.h"
#include "./Interface/cListDialog.h"
#include "MacroManager.h"
#include "GameIn.h"
#include "cMsgBox.h"
#include "ObjectManager.h"
#include "./interface/cScriptManager.h"
#include "ChatManager.h"
#include "MacroDialog.h"
#include "cSpin.h"
#include "MainBarDialog.h"
#include "MHMap.h"
#include "AiManager.h"

#include "ItemManager.h" 
#include "./Input/UserInput.h"

#include "movemanager.h" 
#include "VSyncDialog.h"
cImage ToolTipImage;
GLOBALTON(COptionDialog)
COptionDialog::COptionDialog()
{
	m_BtnPushstartTime = 0;
	m_BtnPushDelayTime = 500;
	m_bPushTabWithMouseOver	= FALSE;
}
COptionDialog::~COptionDialog()
{
}
void COptionDialog::Add(cWindow * window)
{
	if(window->GetType() == WT_PUSHUPBUTTON)
		AddTabBtn(curIdx1++, (cPushupButton * )window);
	else if(window->GetType() == WT_DIALOG)
		AddTabSheet(curIdx2++, window);
	else 
		cTabDialog::Add(window);
}
void COptionDialog::Linking()
{
	cWindow* pWnd = ((cWindow*)((cDialog*)GetTabSheet(2))->GetWindowForID( OTI_CB_AUTOCONTROL ));
	//cImage ToolTipImage;
	SCRIPTMGR->GetImage( 63, &ToolTipImage, PFT_HARDPATH );
	pWnd->SetToolTip( CHATMGR->GetChatMsg(1098), RGBA_MAKE(255, 255, 255, 255), &ToolTipImage, TTCLR_DEFAULT );

	cDialog* pDlg = (cDialog*)GetTabSheet( 8 );
	Packet_List =((cListDialog*)pDlg->GetWindowForID(OTI_LIST_PACKET_SELECT));
	if(Packet_List)
	{
		ITEMMGR->AddClearPacketItem(Packet_List);
	}
}
void COptionDialog::SetActive(BOOL val)
{
	if( !m_bDisable )
	{
		if( val )
		{
			m_GameOption = *( OPTIONMGR->GetGameOption() );

			//m_GameAiOption = *( AISETMGR->GetGameAI() );

			m_GameAI = *( AISETMGR->GetGameAI() );

			UpdateData( FALSE );
		}
		cTabDialog::SetActive(val);
	}
	CMainBarDialog* pDlg = GAMEIN->GetMainInterfaceDialog();
	if( pDlg )
		pDlg->SetPushBarIcon( OPT_OPTIONDLGICON, m_bActive );
}
void COptionDialog::UpdateData( BOOL bSave )
{
	cDialog* pDlg;
	if( bSave )
	{
		pDlg = (cDialog*)GetTabSheet( 0 );	
		m_GameOption.bNoDeal		= ((cCheckBox*)pDlg->GetWindowForID( OTI_CB_NODEAL ))->IsChecked();
		m_GameOption.bNoParty		= ((cCheckBox*)pDlg->GetWindowForID( OTI_CB_NOPARTY ))->IsChecked();
		m_GameOption.bNoFriend		= ((cCheckBox*)pDlg->GetWindowForID( OTI_CB_NOFRIEND ))->IsChecked();
		m_GameOption.bNoVimu		= ((cCheckBox*)pDlg->GetWindowForID( OTI_CB_NOFIGHT ))->IsChecked();

		m_GameOption.bNameOthers	= ((cCheckBox*)pDlg->GetWindowForID( OTI_CB_OTHERSNAME ))->IsChecked();
		m_GameOption.bNameMunpa		= m_GameOption.bNameOthers;//((cCheckBox*)pDlg->GetWindowForID( OTI_CB_MUNPANAME ))->IsChecked();
		m_GameOption.bNameParty		= m_GameOption.bNameOthers;//((cCheckBox*)pDlg->GetWindowForID( OTI_CB_PARTYNAME ))->IsChecked();
		
		m_GameOption.bNoMemberDamage	= ((cCheckBox*)pDlg->GetWindowForID( OTI_CB_MEMBERDAMAGE ))->IsChecked();
		m_GameOption.bNoGameTip		= ((cCheckBox*)pDlg->GetWindowForID( OTI_CB_GAMETIP ))->IsChecked();
		m_GameOption.bMunpaIntro	= ((cCheckBox*)pDlg->GetWindowForID( OTI_CB_MUNPAPR ))->IsChecked();
		m_GameOption.nMacroMode		= 1;//((cPushupButton*)pDlg->GetWindowForID( OTI_PB_MACROMODE ))->IsPushed();
		pDlg = (cDialog*)GetTabSheet( 1 );
		m_GameOption.bNoWhisper		= ((cCheckBox*)pDlg->GetWindowForID( OTI_CB_NOWHISPER ))->IsChecked();
		m_GameOption.bNoChatting	= ((cCheckBox*)pDlg->GetWindowForID( OTI_CB_NOCHATTING ))->IsChecked();
		m_GameOption.bNoBalloon		= ((cCheckBox*)pDlg->GetWindowForID( OTI_CB_NOBALLOON ))->IsChecked();
		m_GameOption.bAutoHide		= ((cCheckBox*)pDlg->GetWindowForID( OTI_CB_AUTOHIDE ))->IsChecked();
		m_GameOption.bNoShoutChat	= ((cCheckBox*)pDlg->GetWindowForID( OTI_CB_WORLDHIDE ))->IsChecked();
		m_GameOption.bNoGuildChat	= ((cCheckBox*)pDlg->GetWindowForID( OTI_CB_MUNPAHIDE ))->IsChecked();
		m_GameOption.bNoAllianceChat= ((cCheckBox*)pDlg->GetWindowForID( OTI_CB_ALLMUNPAHIDE ))->IsChecked();
		m_GameOption.bNoSystemMsg	= ((cCheckBox*)pDlg->GetWindowForID( OTI_CB_NOSYSTEMMSG ))->IsChecked();
		m_GameOption.bNoExpMsg		= ((cCheckBox*)pDlg->GetWindowForID( OTI_CB_NOACQUIREMSG ))->IsChecked();
		m_GameOption.bNoItemMsg		= ((cCheckBox*)pDlg->GetWindowForID( OTI_CB_NOITEMMSG ))->IsChecked();
		pDlg = (cDialog*)GetTabSheet( 2 );
		m_GameOption.bGamma			= ((cCheckBox*)pDlg->GetWindowForID( OTI_CB_GAMMA ))->IsChecked();
		m_GameOption.nGamma			= ((cGuageBar*)pDlg->GetWindowForID( OTI_GB_GAMMA ))->GetCurValue(); 
		m_GameOption.nSightDistance = ((cGuageBar*)pDlg->GetWindowForID( OTI_GB_SIGHT ))->GetCurValue(); 
		m_GameOption.bShadowHero	= ((cCheckBox*)pDlg->GetWindowForID( OTI_CB_HEROSHADOW ))->IsChecked(  );
		m_GameOption.bShadowMonster = ((cCheckBox*)pDlg->GetWindowForID( OTI_CB_MONSTERSHADOW ))->IsChecked(  ); 
		m_GameOption.bShadowOthers	= ((cCheckBox*)pDlg->GetWindowForID( OTI_CB_OTHERSSHADOW ))->IsChecked( ); 
		m_GameOption.bNoAvatarView = ((cCheckBox*)pDlg->GetWindowForID( OTI_CB_AVATA ))->IsChecked();

		m_GameOption.nEffectSnow    = ((cCheckBox*)pDlg->GetWindowForID( OTI_CB_SNOWEFFECT ))->IsChecked( );	
		m_GameOption.bAutoCtrl		= ((cCheckBox*)pDlg->GetWindowForID( OTI_CB_AUTOCONTROL ))->IsChecked();
		m_GameOption.bAmbientMax	= ((cCheckBox*)pDlg->GetWindowForID( OTI_CB_AMBIENT ))->IsChecked();
		m_GameOption.nLODMode = ((cPushupButton*)pDlg->GetWindowForID( OTI_PB_DOWNGRAPHIC ))->IsPushed(); 
		m_GameOption.nEffectMode = ((cPushupButton*)pDlg->GetWindowForID( OTI_PB_ONEEFFECT ))->IsPushed();	

		m_GameOption.bShowPet		= ((cCheckBox*)pDlg->GetWindowForID( OTI_CB_HIDEPET ))->IsChecked();
		m_GameOption.bShowNpc		= ((cCheckBox*)pDlg->GetWindowForID( OTI_CB_HIDENPC ))->IsChecked();
		m_GameOption.bShowMonster = ((cCheckBox*)pDlg->GetWindowForID(OTI_CB_HIDEMONSTER))->IsChecked(); 

		m_GameOption.bVerticalSync = ((cCheckBox*)pDlg->GetWindowForID(OTI_CB_VSYNC))->IsChecked();
		m_GameOption.bFPSChange = ((cCheckBox*)pDlg->GetWindowForID(OTI_CB_FPS))->IsChecked();

		pDlg = (cDialog*)GetTabSheet( 3 );
		m_GameOption.bSoundBGM			= ((cCheckBox*)pDlg->GetWindowForID( OTI_CB_BGMSOUND ))->IsChecked();
		m_GameOption.bSoundEnvironment	= ((cCheckBox*)pDlg->GetWindowForID( OTI_CB_ENVSOUND ))->IsChecked();
		m_GameOption.nVolumnBGM			= ((cGuageBar*)pDlg->GetWindowForID( OTI_GB_BGMSOUND ))->GetCurValue();
		m_GameOption.nVolumnEnvironment = ((cGuageBar*)pDlg->GetWindowForID( OTI_GB_ENVSOUND ))->GetCurValue();

		AISETMGR->SaveData();
		
	}
	else
	{
		pDlg = (cDialog*)GetTabSheet( 0 );	
		((cCheckBox*)pDlg->GetWindowForID( OTI_CB_NODEAL ))->SetChecked( m_GameOption.bNoDeal );
		((cCheckBox*)pDlg->GetWindowForID( OTI_CB_NOPARTY ))->SetChecked( m_GameOption.bNoParty );
		((cCheckBox*)pDlg->GetWindowForID( OTI_CB_NOFRIEND ))->SetChecked( m_GameOption.bNoFriend );
		((cCheckBox*)pDlg->GetWindowForID( OTI_CB_NOFIGHT ))->SetChecked( m_GameOption.bNoVimu );

		//((cCheckBox*)pDlg->GetWindowForID( OTI_CB_MUNPANAME ))->SetChecked( m_GameOption.bNameMunpa );
		//((cCheckBox*)pDlg->GetWindowForID( OTI_CB_PARTYNAME ))->SetChecked( m_GameOption.bNameParty );
		((cCheckBox*)pDlg->GetWindowForID( OTI_CB_OTHERSNAME ))->SetChecked( m_GameOption.bNameOthers );


		((cCheckBox*)pDlg->GetWindowForID( OTI_CB_MEMBERDAMAGE ))->SetChecked( m_GameOption.bNoMemberDamage );
		((cCheckBox*)pDlg->GetWindowForID( OTI_CB_GAMETIP ))->SetChecked( m_GameOption.bNoGameTip );
		((cCheckBox*)pDlg->GetWindowForID( OTI_CB_MUNPAPR ))->SetChecked( m_GameOption.bMunpaIntro );
		//((cPushupButton*)pDlg->GetWindowForID( OTI_PB_CHATMODE ))->SetPush( !m_GameOption.nMacroMode );
		//((cPushupButton*)pDlg->GetWindowForID( OTI_PB_MACROMODE ))->SetPush( m_GameOption.nMacroMode );
		pDlg = (cDialog*)GetTabSheet( 1 );	
		((cCheckBox*)pDlg->GetWindowForID( OTI_CB_NOWHISPER ))->SetChecked( m_GameOption.bNoWhisper );
		((cCheckBox*)pDlg->GetWindowForID( OTI_CB_NOCHATTING ))->SetChecked( m_GameOption.bNoChatting );
		((cCheckBox*)pDlg->GetWindowForID( OTI_CB_NOBALLOON ))->SetChecked( m_GameOption.bNoBalloon );
		((cCheckBox*)pDlg->GetWindowForID( OTI_CB_AUTOHIDE ))->SetChecked( m_GameOption.bAutoHide );
		((cCheckBox*)pDlg->GetWindowForID( OTI_CB_WORLDHIDE ))->SetChecked( m_GameOption.bNoShoutChat );
		((cCheckBox*)pDlg->GetWindowForID( OTI_CB_MUNPAHIDE ))->SetChecked( m_GameOption.bNoGuildChat );
		((cCheckBox*)pDlg->GetWindowForID( OTI_CB_ALLMUNPAHIDE ))->SetChecked( m_GameOption.bNoAllianceChat );
		((cCheckBox*)pDlg->GetWindowForID( OTI_CB_NOSYSTEMMSG ))->SetChecked( m_GameOption.bNoSystemMsg );
		((cCheckBox*)pDlg->GetWindowForID( OTI_CB_NOACQUIREMSG ))->SetChecked( m_GameOption.bNoExpMsg );
		((cCheckBox*)pDlg->GetWindowForID( OTI_CB_NOITEMMSG ))->SetChecked( m_GameOption.bNoItemMsg );
		pDlg = (cDialog*)GetTabSheet( 2 );
		((cCheckBox*)pDlg->GetWindowForID( OTI_CB_GAMMA ))->SetChecked( m_GameOption.bGamma );
		((cGuageBar*)pDlg->GetWindowForID( OTI_GB_GAMMA ))->SetCurValue( m_GameOption.nGamma );
		((cGuageBar*)pDlg->GetWindowForID( OTI_GB_SIGHT ))->SetCurValue( m_GameOption.nSightDistance );
		((cCheckBox*)pDlg->GetWindowForID( OTI_CB_HEROSHADOW ))->SetChecked( m_GameOption.bShadowHero );
		((cCheckBox*)pDlg->GetWindowForID( OTI_CB_MONSTERSHADOW ))->SetChecked( m_GameOption.bShadowMonster );
		((cCheckBox*)pDlg->GetWindowForID( OTI_CB_OTHERSSHADOW ))->SetChecked( m_GameOption.bShadowOthers );
		((cCheckBox*)pDlg->GetWindowForID( OTI_CB_AVATA ))->SetChecked( m_GameOption.bNoAvatarView );

		((cCheckBox*)pDlg->GetWindowForID( OTI_CB_SNOWEFFECT ))->SetChecked( m_GameOption.nEffectSnow );
		((cCheckBox*)pDlg->GetWindowForID( OTI_CB_AUTOCONTROL ))->SetChecked( m_GameOption.bAutoCtrl );
		((cCheckBox*)pDlg->GetWindowForID( OTI_CB_AMBIENT ))->SetChecked( m_GameOption.bAmbientMax );
		((cPushupButton*)pDlg->GetWindowForID( OTI_PB_BASICGRAPHIC ))->SetPush( !m_GameOption.nLODMode );
		((cPushupButton*)pDlg->GetWindowForID( OTI_PB_DOWNGRAPHIC ))->SetPush( m_GameOption.nLODMode );
		((cPushupButton*)pDlg->GetWindowForID( OTI_PB_BASICEFFECT ))->SetPush( !m_GameOption.nEffectMode );
		((cPushupButton*)pDlg->GetWindowForID( OTI_PB_ONEEFFECT ))->SetPush( m_GameOption.nEffectMode );

		((cCheckBox*)pDlg->GetWindowForID( OTI_CB_HIDEPET ))->SetChecked(m_GameOption.bShowPet );          
		((cCheckBox*)pDlg->GetWindowForID( OTI_CB_HIDENPC ))->SetChecked( m_GameOption.bShowNpc );         
		((cCheckBox*)pDlg->GetWindowForID(OTI_CB_HIDEMONSTER))->SetChecked(m_GameOption.bShowMonster); 

		((cCheckBox*)pDlg->GetWindowForID(OTI_CB_VSYNC))->SetChecked(m_GameOption.bVerticalSync);
		((cCheckBox*)pDlg->GetWindowForID(OTI_CB_FPS))->SetChecked(m_GameOption.bFPSChange);

		DisableGraphicTab(m_GameOption.bAutoCtrl);

		pDlg = (cDialog*)GetTabSheet( 3 );
		((cCheckBox*)pDlg->GetWindowForID( OTI_CB_BGMSOUND ))->SetChecked( m_GameOption.bSoundBGM );
		((cCheckBox*)pDlg->GetWindowForID( OTI_CB_ENVSOUND ))->SetChecked( m_GameOption.bSoundEnvironment );
		((cGuageBar*)pDlg->GetWindowForID( OTI_GB_BGMSOUND ))->SetCurValue( m_GameOption.nVolumnBGM );
		((cGuageBar*)pDlg->GetWindowForID( OTI_GB_ENVSOUND ))->SetCurValue( m_GameOption.nVolumnEnvironment );

		
		AISETMGR->ReadData();
	}
}
void COptionDialog::OnActionEvent(LONG lId, void * p, DWORD we)
{
//--------------------------------------external save function
	//if( lId == OTI_BTN_OK )
	//{
		//UpdateData( TRUE );
		//if( m_GameOption.bMunpaIntro )
		//{
			//m_GameOption.bMunpaIntro = FALSE;
			//OPTIONMGR->SetGameOption( &m_GameOption );
			//OPTIONMGR->ApplySettings();
			//m_GameOption.bMunpaIntro = TRUE;
			//OPTIONMGR->SetGameOption( &m_GameOption );
		//	sCPlayerAI.SaveTemp();
			//AISETMGR->SaveData();
		//}
		//else
		//{
			//OPTIONMGR->SetGameOption( &m_GameOption );
		//	AISETMGR->SaveData();
			//OPTIONMGR->ApplySettings();
		//}
	//}
//--------------------------------------end of external save function
	if( we & WE_BTNCLICK )
	{
		if( lId == OTI_BTN_OK )
		{
			UpdateData( TRUE );
			if( m_GameOption.bMunpaIntro )
			{
				m_GameOption.bMunpaIntro = FALSE;
				OPTIONMGR->SetGameOption( &m_GameOption );
				OPTIONMGR->ApplySettings();
				m_GameOption.bMunpaIntro = TRUE;
				OPTIONMGR->SetGameOption( &m_GameOption );
			}
			else
			{
				OPTIONMGR->SetGameOption( &m_GameOption );
				OPTIONMGR->ApplySettings();
			}
			OPTIONMGR->SendOptionMsg();		
			//SetActive( FALSE );
			sCPlayerAI.HideObject();
			//AISETMGR->SaveData();//kiv
			sCPlayerAI.SaveTemp();//kiv

			if(sCPlayerAI.IsOpen())
				MOVEMGR->HeroMoveStop();

			if (OPTIONMGR->GetFPSStatus() != m_GameOption.bFPSChange)
			{
				OPTIONMGR->SetFPSStatus(m_GameOption.bFPSChange);
				GAMEIN->GetVsyncDialog()->AddMsg("FPS",m_GameOption.bFPSChange ? "60" : "30");

				
			}
			if (OPTIONMGR->GetVSyncStatus() != m_GameOption.bVerticalSync)
			{
				g_pExecutive->GetRenderer()->SetVerticalSync(m_GameOption.bVerticalSync);

				Sleep(200);
				GAMEIN->GetVsyncDialog()->AddMsg("VSYNC", g_pExecutive->GetRenderer()->IsSetVerticalSync() ? "Enabled" : "Disabled");
				//CHATMGR->AddMsg(CTC_SYSMSG, "vSync set: %s", g_pExecutive->GetRenderer()->IsSetVerticalSync() ? "On" : "Off");

				OPTIONMGR->SetVSyncStatus(m_GameOption.bVerticalSync);
			}
				

			CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(2333));
		}
		else if( lId == OTI_BTN_CANCEL )
		{
			OPTIONMGR->CancelSettings();
			SetActive( FALSE );
		}
		else if( lId == OTI_BTN_RESET )
		{
			OPTIONMGR->SetDefaultOption();
			m_GameOption = *( OPTIONMGR->GetGameOption() );
			AISETMGR->SetDefaultOption();
			m_GameAI = *( AISETMGR->GetGameAI() );
			UpdateData( FALSE );
		}
		else if(lId >=OTI_BTN_AUTOHUNT_TIMER_TEST_01 && lId <=OTI_BTN_AUTOHUNT_TIMER_TEST_05) 
		{
			AISETMGR->TestQuickKey(lId);
		}
		else if(lId ==OTI_SKILL_TERAN_BTN) 
		{
			this->SelectTab( 9 );
		}
		else if (lId == OTI_BTN_AUTOHUNT_GET_CURPOS) sCPlayerAI.SetPoint();
	}
	if( !WINDOWMGR->IsMouseDownUsed() )
	if( PtInWindow ( g_UserInput.GetMouse()->GetMouseEventX(), g_UserInput.GetMouse()->GetMouseEventY() ) )
	{
        if(lId == OTI_LIST_PACKET_SELECT)
		{
			DeleteClearItem();
		}
	}
	if( we & WE_PUSHUP )
	{
		if( lId == OTI_PB_BASICGRAPHIC || lId == OTI_PB_DOWNGRAPHIC )
			((cPushupButton*)((cDialog*)GetTabSheet( 2 ))->GetWindowForID( lId ))->SetPush( TRUE );
		if( lId == OTI_PB_BASICEFFECT || lId == OTI_PB_ONEEFFECT )
			((cPushupButton*)((cDialog*)GetTabSheet( 2 ))->GetWindowForID( lId ))->SetPush( TRUE );

		if( lId == OTI_PB_AUTOHUNT_ON )
		{
			sCPlayerAI.SetAIDisable();
		}
		if( lId == OTI_PB_AUTOHUNT_REVIVE_LOGIN || lId == OTI_PB_AUTOHUNT_REVIVE_PRESENT )((cPushupButton*)((cDialog*)GetTabSheet( 4 ))->GetWindowForID( lId ))->SetPush( TRUE );
	}
	if( we & WE_PUSHDOWN )	
	{
		if( lId == OTI_PB_BASICGRAPHIC )
		{
			((cPushupButton*)((cDialog*)GetTabSheet( 2 ))->GetWindowForID( OTI_PB_DOWNGRAPHIC ))->SetPush( FALSE );
		}
		else if( lId == OTI_PB_DOWNGRAPHIC )
		{
			((cPushupButton*)((cDialog*)GetTabSheet( 2 ))->GetWindowForID( OTI_PB_BASICGRAPHIC ))->SetPush( FALSE );
		}
		if( lId == OTI_PB_BASICEFFECT )
		{
			((cPushupButton*)((cDialog*)GetTabSheet( 2 ))->GetWindowForID( OTI_PB_ONEEFFECT ))->SetPush( FALSE );
		}
		else if( lId == OTI_PB_ONEEFFECT )
		{
			((cPushupButton*)((cDialog*)GetTabSheet( 2 ))->GetWindowForID( OTI_PB_BASICEFFECT ))->SetPush( FALSE );
		}

		if( lId == OTI_PB_AUTOHUNT_ON )
		{
			sCPlayerAI.SetAIEnable();
		}
		if( lId == OTI_PB_AUTOHUNT_REVIVE_LOGIN )
		{
			((cPushupButton*)((cDialog*)GetTabSheet( 4 ))->GetWindowForID( OTI_PB_AUTOHUNT_REVIVE_PRESENT ))->SetPush( FALSE );
		}
		else if( lId == OTI_PB_AUTOHUNT_REVIVE_PRESENT )
		{
			((cPushupButton*)((cDialog*)GetTabSheet( 4 ))->GetWindowForID( OTI_PB_AUTOHUNT_REVIVE_LOGIN ))->SetPush( FALSE );
		}
	}
	if( we & WE_CHECKED )
	{
		if( lId == OTI_CB_AUTOCONTROL )
		{
			DisableGraphicTab(TRUE);
		}
	}
	else if( we & WE_NOTCHECKED )
	{
		if( lId == OTI_CB_AUTOCONTROL )
		{
			DisableGraphicTab(FALSE);
		}
	}
}
void COptionDialog::DisableGraphicTab(BOOL bDisable)
{
	DWORD FullColor = 0xffffffff;
	DWORD HalfColor = RGBA_MAKE(200,200,200,255);
	if( bDisable )
	{
		FullColor = HalfColor;
	}
	cWindow* pWnd = NULL;
	cGuageBar* pGuag = NULL;
	pGuag = ((cGuageBar*)((cDialog*)GetTabSheet( 2 ))->GetWindowForID( OTI_GB_GAMMA ));
	pGuag->SetDisable(bDisable);
	pGuag->SetImageRGB(FullColor);
	pGuag->SetGuageLock(bDisable, FullColor);
	((cStatic*)((cDialog*)GetTabSheet( 2 ))->GetWindowForID( OTI_TEXT_GAMMA ))->SetFGColor(FullColor);
	pGuag = ((cGuageBar*)((cDialog*)GetTabSheet( 2 ))->GetWindowForID( OTI_GB_SIGHT ));
	pGuag->SetDisable(bDisable);
	pGuag->SetImageRGB(FullColor);
	pGuag->SetGuageLock(bDisable, FullColor);
	((cStatic*)((cDialog*)GetTabSheet( 2 ))->GetWindowForID( OTI_TEXT_SIGHT ))->SetFGColor(FullColor);
	pWnd = ((cWindow*)((cDialog*)GetTabSheet( 2 ))->GetWindowForID( OTI_CB_HEROSHADOW ));
	pWnd->SetDisable(bDisable);
	pWnd->SetImageRGB(FullColor);
	((cStatic*)((cDialog*)GetTabSheet( 2 ))->GetWindowForID( OTI_TEXT_HEROSHADOW ))->SetFGColor(FullColor);
	pWnd = ((cWindow*)((cDialog*)GetTabSheet( 2 ))->GetWindowForID( OTI_CB_MONSTERSHADOW ));
	pWnd->SetDisable(bDisable);
	pWnd->SetImageRGB(FullColor);
	((cStatic*)((cDialog*)GetTabSheet( 2 ))->GetWindowForID( OTI_TEXT_MONSTERSHADOW ))->SetFGColor(FullColor);
	pWnd = ((cWindow*)((cDialog*)GetTabSheet( 2 ))->GetWindowForID( OTI_CB_OTHERSSHADOW ));
	pWnd->SetDisable(bDisable);
	pWnd->SetImageRGB(FullColor);
	((cStatic*)((cDialog*)GetTabSheet( 2 ))->GetWindowForID( OTI_TEXT_OTHERSSHADOW ))->SetFGColor(FullColor);
	pWnd = ((cWindow*)((cDialog*)GetTabSheet( 2 ))->GetWindowForID( OTI_PB_BASICGRAPHIC ));
	pWnd->SetDisable(bDisable);
	pWnd->SetImageRGB(FullColor);
	((cStatic*)((cDialog*)GetTabSheet( 2 ))->GetWindowForID( OTI_TEXT_BASICGRAPHIC ))->SetFGColor(FullColor);
	pWnd = ((cWindow*)((cDialog*)GetTabSheet( 2 ))->GetWindowForID( OTI_PB_DOWNGRAPHIC ));
	pWnd->SetDisable(bDisable);
	pWnd->SetImageRGB(FullColor);
	((cStatic*)((cDialog*)GetTabSheet( 2 ))->GetWindowForID( OTI_TEXT_DOWNGRAPHIC ))->SetFGColor(FullColor);
	pWnd = ((cWindow*)((cDialog*)GetTabSheet( 2 ))->GetWindowForID( OTI_PB_BASICEFFECT ));
	pWnd->SetDisable(bDisable);
	pWnd->SetImageRGB(FullColor);
	((cStatic*)((cDialog*)GetTabSheet( 2 ))->GetWindowForID( OTI_TEXT_BASICEFFECT ))->SetFGColor(FullColor);
	pWnd = ((cWindow*)((cDialog*)GetTabSheet( 2 ))->GetWindowForID( OTI_PB_ONEEFFECT ));
	pWnd->SetDisable(bDisable);
	pWnd->SetImageRGB(FullColor);
	((cStatic*)((cDialog*)GetTabSheet( 2 ))->GetWindowForID( OTI_TEXT_ONEEFFECT ))->SetFGColor(FullColor);	
}

void COptionDialog::AddClearItemList(cIcon * icon)
{
	//cDialog * pDlg = (cDialog*)GetTabSheet( 8 );//12092023
	//cListDialog * Packet_List =((cListDialog*)pDlg->GetWindowForID(OTI_LIST_PACKET_SELECT));//12092023
	ITEMMGR->AddClearPacketItem(Packet_List,icon->GetData());

	//CheckPacketSelectScrollbar();
	//CHATMGR->AddMsg(CTC_SYSMSG, "%d %d %d", Packet_List->GetMaxLine(), Packet_List->GetTipLineCount(), Packet_List->GetTopListItemIdx());
	//((cListDialog*)GetWindowForID(OTI_LIST_PACKET_SELECT))->CheckScrollbar();

	//cGuageBar* pGuag = NULL;
	/*cDialog * pDlg = (cDialog*)GetTabSheet( 8 );
	cListDialog * Packet_List =((cListDialog*)pDlg->GetWindowForID(OTI_LIST_PACKET_SELECT));
	Packet_List->CheckScrollbar();*/
	//pGuag = ((cGuageBar*)((cDialog*)GetTabSheet( 8 ))->GetWindowForID( OTI_LIST_PACKET_SELECT_SCROLLBAR ));
	//pGuag->SetDisable(TRUE);
}
void COptionDialog::DeleteClearItem()
{
	//cDialog* pDlg = (cDialog*)GetTabSheet( 8 );//12092023
	//cListDialog * m_Packet_Real_List =((cListDialog*)pDlg->GetWindowForID(OTI_LIST_PACKET_SELECT));//12092023

	if(Packet_List)
	{
		int row=Packet_List->GetSelectRowIdx();
		ITEM* pItem =Packet_List->GetSelectedITEM(row);

		if(pItem)
		{
			ITEM_INFO * pItemInfo=ITEMMGR->FindItemInfoForName(pItem->string);

			if(pItemInfo)
			{
				Packet_List->RemoveItem(pItem->string);
				ITEMMGR->DeleteClearPacketItem(pItemInfo->ItemIdx);
			}
		}
		//((cListDialog*)GetWindowForID(OTI_LIST_PACKET_SELECT))->CheckScrollbar();
	}
}
BOOL COptionDialog::FakeMoveIcon(LONG x, LONG y, cIcon * icon)
{
	if ((cDialog*)GetTabSheet(8)->IsActive() && icon->GetType() == WT_ITEM)
	{
		//AddClearItemList(icon);//12092023
		ITEMMGR->AddClearPacketItem(Packet_List,icon->GetData());
		return FALSE;
	}
	return FALSE;
}
LONG COptionDialog::GetFovVal()
{
	return ((cGuageBar*)((cDialog*)GetTabSheet(2))->GetWindowForID(OTI_GB_GAMMA))->GetCurValue();
}
void COptionDialog::Test()
{
	return;
}
int COptionDialog::GetPacketSelectNum()
{
	return Packet_List->GetTopListItemIdx();
}
void COptionDialog::CheckPacketSelectScrollbar()
{
	if (Packet_List->GetTopListItemIdx() != 0)
	{
		//cDialog * pDlg = (cDialog*)GetTabSheet( 8 );//12092023
		//((cListDialog*)pDlg->GetWindowForID(OTI_LIST_PACKET_SELECT))->GetWindowForID(OTI_LIST_PACKET_SELECT_SCROLLBAR)->SetActive(TRUE);//12092023
		//((cButton*)pDlg->GetWindowForID(OTI_LIST_PACKET_SELECT_SCROLLBAR))->SetActive(TRUE);
	//	COptionDialog * m_Dlg = GAMEIN->GetOptionDialog();
//1		((cButton*)((cDialog*)m_Dlg->GetTabSheet(8))->GetWindowForID(OTI_LIST_PACKET_SELECT_SCROLLBAR))->SetActive(TRUE);
	}	
}