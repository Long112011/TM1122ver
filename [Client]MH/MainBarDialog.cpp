







// MainBarDialog.cpp: implementation of the CMainBarDialog class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "MainBarDialog.h"
#include "GameIn.h"
#include "../GlobalEventFunc.h"
#include "./BarIcon.h"
#include "WindowIdEnum.h"
#include "./Interface/cWindowManager.h"
#include "./Interface/cScriptManager.h"
#include "./Interface/cResourceManager.h"
#include "ObjectManager.h"
#include "StreetStallManager.h"
#include "ExchangeManager.h"
#include "VimuManager.h"
#include "./Interface/cAnimationManager.h"
#include "FriendManager.h"
#include "NoteManager.h"
#include "MacroDialog.h"
#include "./Input/Mouse.h"
#include "MapChange.h"

#include "QuestDialog.h"
#include "CharacterDialog.h"
#include "PartyManager.h"
#include "Interface/cFont.h"
#include "AbilityManager.h"
#include "ChatManager.h"
#include "./Interface/cStatic.h"

#include "PKManager.h"
#include "ExitManager.h"

#include "GameEventManager.h"
#include "HelpDialog.h"
#include "EventMapInfo.h"
#include "BattleSystem_Client.h"
#include "GameResourceManager.h"

#include "GuageDialog.h"
#include "GoldShopDialog.h"
#include "SafeNumberIcon.h"           //weiye 갛홍傑庫匡숭 2018-04-08

#include "OptionDialog.h" //BY JACK
#include "MHMap.h" //BY JACK
#include "ItemManager.h"
#include "InventoryExDialog.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMainBarDialog::CMainBarDialog()
{
	m_IconBarWidth = 42;				// 15
	m_IconBarHeight = 42;				// 15

	m_IconBarSpace = 0;

	m_IconIndexCreator.Init( 100, IG_MAINBARICON_START );

	m_OldDragFlag = FALSE;
	m_crossStep = -1;
	m_crossBarHalfX = -6;
	m_crossBarHalfY	= -1;

	m_bFold			= FALSE;
	m_nSlidingDir	= 0;

//	m_pStcTail		= NULL;
	m_pBtnSize		= NULL;

	m_bFlicker=false;
	m_bStaticFlicker=false;
	NotifyTitleMsgLoop=0;
}

CMainBarDialog::~CMainBarDialog()
{
	ReleaseBar();
}


void CMainBarDialog::InitBar()
{
/*
	ASSERT(!m_pBasicImage);
	
	char buff[128];
	wsprintf( buff, "image/GameIn/15/150111.tif" );
	m_pBasicImage = WINDOWMGR->GetImageOfFile( buff, 40 );
	
	wsprintf( buff, "image/GameIn/15/CROSS_BAR.TIF" );
	m_pMoveCrossImage = WINDOWMGR->GetImageOfFile(buff, 40);
	
	wsprintf( buff, "image/GameIn/spot.tif" );
	m_pIconToolTipImg = WINDOWMGR->GetImageOfFile(buff, 40);
*/

	//m_pStcTail = (cStatic*)GetWindowForID( MI_STC_TAIL );
	m_pBtnSize = (cButton*)GetWindowForID( MI_BTN_SIZE );

	m_lOriX = (LONG)GetAbsX();
	m_lOriY = (LONG)GetAbsY();
	m_lSlidingX = 0;
	m_lOffSetX = GetCaptionRect()->right;		//먼저 세팅(아래 아이 등록전에)

	SCRIPTMGR->GetImage( 48, &m_MoveCrossImage );
	LoadInfo();

	// 늪뇹쏵契 2015-12-07 
	Fold(TRUE);



	m_bFlicker=false;
	m_bStaticFlicker=false;
	NotifyTitleMsgLoop=0;

	/*SCRIPTMGR->GetImage(153, &m_ImgNgAlram[0], PFT_JACKPATH);
	SCRIPTMGR->GetImage(154, &m_ImgNgAlram[1], PFT_JACKPATH);
	SCRIPTMGR->GetImage(155, &m_ImgNgAlram[2], PFT_JACKPATH);
	SCRIPTMGR->GetImage(156, &m_ImgNgAlram[3], PFT_JACKPATH);
	SCRIPTMGR->GetImage(157, &m_ImgNgAlram[4], PFT_JACKPATH);
	SCRIPTMGR->GetImage(158, &m_ImgNgAlram[5], PFT_JACKPATH);
	SCRIPTMGR->GetImage(159, &m_ImgNgAlram[6], PFT_JACKPATH);
	SCRIPTMGR->GetImage(160, &m_ImgNgAlram[7], PFT_JACKPATH);*/

	for (int i = 0; i < 8; i++)
	{
		SCRIPTMGR->GetImage(153+i, &m_ImgNgAlram[i], PFT_JACKPATH);
	}
}

void CMainBarDialog::ReleaseBar()
{
	SaveInfo();

	int n = 0;
	CBarIcon * pIcon = NULL;
	while( pIcon = (CBarIcon *)m_MainList.RemoveTail() )
	{
		m_IconIndexCreator.ReleaseIndex( pIcon->GetID() );
		delete pIcon;
	}
}

void CMainBarDialog::LoadInfo()
{
	CMHFile file;
#ifdef _FILE_BIN_
//	if( !file.Init("./ini/barInfo.bin", "rb") )
	if( !file.Init("./Image/InterfaceScript/barInfo.bin", "rb") )
		return;
#else
	if( !file.Init("./Image/barInfo.txt", "r") )
		return;
#endif

	m_nIconNum = file.GetInt();

	for( int i = 0 ; i < m_nIconNum ; ++i )
	{
		BARICON_INFO info;
		info.type			= file.GetByte();
		info.kind			= file.GetWord();
		info.barPos			= file.GetWord();
		info.nToolTipMsg	= file.GetWord();

		CBarIcon * pIcon = GetIcon(info.kind);
		pIcon->InitInfo(&info);

		AddIcon(info.barPos, pIcon);
	}

	file.Release();
}

void CMainBarDialog::Refresh()
{

////단축키 세팅용
	int s_nMacro[] = {
#ifndef _JAPAN_LOCAL_
#ifndef _HK_LOCAL_
#ifndef _TL_LOCAL_
		ME_COUNT,	//키없다.	//국내 아이템몰
#endif
#endif
#endif
		ME_TOGGLE_HELP,
		ME_TOGGLE_CHARACTERDLG,
		ME_TOGGLE_INVENTORYDLG,
		ME_TOGGLE_MUGONGDLG,
		ME_TOGGLE_QUESTDLG,
		ME_TOGGLE_MINIMAP,
		ME_TOGGLE_MUNPADLG,
		ME_TOGGLE_FRIENDLIST,
		ME_SEND_MEMO,		
		ME_TOGGLE_OPTIONDLG,
		ME_TOGGLE_EXITDLG,
		ME_COUNT,	//키없다.
	};
///

	cImage ToolTipImg;
	SCRIPTMGR->GetImage( 63, &ToolTipImg, PFT_HARDPATH );	

	char imagePath[128];
	char strMacro[32];
	
	PTRLISTPOS pos = m_MainList.GetHeadPosition();
	while(pos)
	{
		CBarIcon * pIcon = (CBarIcon * )m_MainList.GetNext(pos);
		BARICON_INFO* pInfo = pIcon->GetInfo();

		strcpy( imagePath, RESRCMGR->GetMsg(pInfo->nToolTipMsg) );

		sMACRO* pMacro = MACROMGR->GetCurMacroKey( s_nMacro[pInfo->kind] );
		if( pMacro )
		{
			GAMEIN->GetMacroDialog()->ConvertMacroToText( strMacro, pMacro );
			wsprintf( imagePath, "%s(%s)", imagePath, strMacro );
		}

		pIcon->SetToolTip(imagePath, RGB_HALF(255, 255, 255), &ToolTipImg, TTCLR_DEFAULT);
	}
}
void CMainBarDialog::RefreshBBPoint()
{
	int s_nMacro[] = {
		ME_COUNT,
		ME_TOGGLE_HELP,
		ME_TOGGLE_CHARACTERDLG,
		ME_TOGGLE_INVENTORYDLG,
		ME_TOGGLE_MUGONGDLG,
		ME_TOGGLE_QUESTDLG,
		ME_TOGGLE_MINIMAP,
		ME_TOGGLE_MUNPADLG,
		ME_TOGGLE_FRIENDLIST,
		ME_SEND_MEMO,
		ME_TOGGLE_OPTIONDLG,
		ME_TOGGLE_EXITDLG,
		ME_COUNT,
	};
	cImage ToolTipImg;
	SCRIPTMGR->GetImage(63, &ToolTipImg, PFT_HARDPATH);
	char imagePath[128];
	char strMacro[32];
	PTRLISTPOS pos = m_MainList.GetHeadPosition();
	while (pos)
	{
		CBarIcon * pIcon = (CBarIcon *)m_MainList.GetNext(pos);
		BARICON_INFO* pInfo = pIcon->GetInfo();
		sMACRO* pMacro2 = MACROMGR->GetCurMacroKey(s_nMacro[pInfo->kind == 0]);
		if (pMacro2)
		{
			GAMEIN->GetMacroDialog()->ConvertMacroToText(strMacro, pMacro2);
			wsprintf(imagePath, CHATMGR->GetChatMsg(2581), ITEMMGR->GetStoredPDPoint());
			pIcon->AddToolTipLine(imagePath, RGB_HALF(0, 255, 0), &ToolTipImg);
		}
	}
}
void CMainBarDialog::SaveInfo()
{
//	CMHFile file;
//	if( !file.Init("./ini/barInfo.txt", "w+") )
//		return;

}


CBarIcon * CMainBarDialog::GetIcon(WORD idx)
{
	int ImgIdxForIdx [][3]=
	{

#ifndef _JAPAN_LOCAL_
#ifndef _HK_LOCAL_
#ifndef _TL_LOCAL_
		{116,116,116},	//국내 아이템몰
#endif
#endif
#endif
		{98,98,98},
		{0,1,2},
		{3,4,5},
		{6,7,8},
		{9,10,11},
		{12,13,14},
		{15,16,17},
		{18,19,20},
		{21,22,23},
		{24,25,26},
		{27,28,29},
		{77,78,79},
		//	{80,81,82},

		//	{83,84,85},
	};

	// ------------- 인터페이스 이미지 하드 코딩로딩 방법 ------------------------------
	// taiyo 
	cImage		ImageBasic;
	cImage		ImageOver;
	cImage		ImagePress;
	SCRIPTMGR->GetImage( ImgIdxForIdx[idx][0], &ImageOver  );
	SCRIPTMGR->GetImage( ImgIdxForIdx[idx][1], &ImagePress );
	SCRIPTMGR->GetImage( ImgIdxForIdx[idx][2], &ImageBasic );
	// ------------- 하드 코딩로딩 방법 ------------------------------------------------

//	cImage		ImageAlram;
//	SCRIPTMGR->GetImage( 92, &ImageAlram );
		
	CBarIcon * pIcon = new CBarIcon;
	pIcon->Init( 0, 0, m_IconBarWidth, m_IconBarHeight, &ImageBasic, &ImageOver, &ImagePress, NULL, m_IconIndexCreator.GenerateIndex() );
	pIcon->SetMovable( TRUE );
//	pIcon->SetAlramImage( &ImageAlram );
	//WINDOWMGR->AddWindow(pIcon);
	
	return pIcon;
}

DWORD CMainBarDialog::ActionEvent(CMouse * mouseInfo)
{
	DWORD we = WE_NULL;
	if( !m_bActive ) return we;
	
	if( m_bFold == FALSE )
	{
		if( m_nSlidingDir == eSLIDING_NONE )	//Sliding중 에는 안함
		{
			PTRLISTPOS pos = m_MainList.GetHeadPosition();
			while(pos)
			{
				CBarIcon * pIcon = (CBarIcon * )m_MainList.GetNext(pos);
				pIcon->ActionEvent(mouseInfo);
			}
/*			
			if(m_OldDragFlag)
			{
				m_crossStep = (int)(((mouseInfo->GetMouseX() - m_absPos.x  + m_IconBarWidth / 2 )/(m_IconBarWidth + m_IconBarSpace)));
				
				if(mouseInfo->LButtonUp())
				{
					if(0 <= m_crossStep && m_crossStep <= m_MainList.GetCount())
						if(m_pDragIcon->GetData() != m_crossStep)

							MoveIcon(m_pDragIcon->GetData(), m_crossStep);
						m_OldDragFlag = FALSE;
						m_pDragIcon->SetAlpha(255);
				}
				if(0 > m_crossStep || m_crossStep > m_MainList.GetCount())
					m_crossStep = -1;
				if(m_pDragIcon->PtInWindow(mouseInfo->GetMouseX(), mouseInfo->GetMouseY()))
					m_crossStep = -1;
			}
*/
		}
		else if( m_nSlidingDir == eSLIDING_RIGHT )
		{
			m_lSlidingX += gTickTime;

			if( m_lSlidingX >= m_IconBarWidth*m_nIconNum - 1 )	//-1 : buffer
			{
				m_lSlidingX = m_IconBarWidth*m_nIconNum;

				m_pBtnSize->SetRelXY( m_lOffSetX, 0 );
				SetAbsXY( m_lOriX + m_lSlidingX, m_lOriY );
				SetWH( m_lOffSetX + m_pBtnSize->GetWidth(), GetHeight() );
				m_nSlidingDir = eSLIDING_NONE;
				
				m_lOriX = (LONG)GetAbsX() - m_lSlidingX;
				m_lOriY = (LONG)GetAbsY();

				m_bFold = TRUE;
			}
			else
			{
				m_pBtnSize->SetRelXY( m_lOffSetX + m_IconBarWidth*m_nIconNum - m_lSlidingX, 0 );
				SetAbsXY( m_lOriX + m_lSlidingX, m_lOriY );
				SetWH( m_lOffSetX + m_pBtnSize->GetWidth() + m_IconBarWidth*m_nIconNum - m_lSlidingX, GetHeight() );
			}
		}
		else
		{
			m_lSlidingX -= gTickTime;

			if( m_lSlidingX <= 1 ) 	//1 : buffer
			{
				m_lSlidingX = 0;
				m_pBtnSize->SetRelXY( m_lOffSetX + m_IconBarWidth*m_nIconNum, 0 );
				SetAbsXY( m_lOriX, m_lOriY );
				SetWH( m_lOffSetX + m_pBtnSize->GetWidth() + m_IconBarWidth*m_nIconNum, GetHeight() );

				m_nSlidingDir = eSLIDING_NONE;

				m_lOriX = (LONG)GetAbsX() - m_lSlidingX;
				m_lOriY = (LONG)GetAbsY();
			}
			else
			{
				m_pBtnSize->SetRelXY( m_lOffSetX + m_IconBarWidth*m_nIconNum - m_lSlidingX, 0 );

				SetAbsXY( m_lOriX + m_lSlidingX, m_lOriY );
				SetWH( m_lOffSetX + m_pBtnSize->GetWidth() + m_IconBarWidth*m_nIconNum - m_lSlidingX, GetHeight() );
			}
		}
	}
	

//	we |= cDialog::ActionEvent(mouseInfo);

	we |= cWindow::ActionEvent(mouseInfo);

	if( !m_bDisable && m_nSlidingDir == eSLIDING_NONE )		//슬라이딩중엔 드래그 안되도록..
		we |= ActionEventWindow(mouseInfo);

	we |= ActionEventComponent(mouseInfo);

	return we;
}

void CMainBarDialog::SetAbsXY(LONG x, LONG y)
{
	PTRLISTPOS pos = m_MainList.GetHeadPosition();
	
	while(pos)
	{
		CBarIcon * pIcon = (CBarIcon * )m_MainList.GetNext(pos);
	
		pIcon->SetAbsXY( x + m_lOffSetX + pIcon->GetData()*m_IconBarWidth, y );
	}

	cDialog::SetAbsXY(x,y);
}

void CMainBarDialog::OnActionEvent(LONG lId, void * p, DWORD we)
{
	if( we & WE_BTNCLICK )
	{
		if( lId == MI_BTN_SIZE )
		{
			if( m_nSlidingDir == eSLIDING_NONE )
			{
				if( !m_bFold )
				{
					if( m_nSlidingDir == eSLIDING_NONE )
					{
						m_lOriX = (LONG)GetAbsX();
						m_lOriY = (LONG)GetAbsY();
						m_lSlidingX = 0;	
					}
					m_nSlidingDir = eSLIDING_RIGHT;
				}
				else
				{
					if( m_nSlidingDir == eSLIDING_NONE )
					{
						m_lOriX = (LONG)GetAbsX() - m_IconBarWidth*m_nIconNum;
						m_lOriY = (LONG)GetAbsY();
						m_lSlidingX = m_IconBarWidth*m_nIconNum;	
					}
					m_nSlidingDir = eSLIDING_LEFT;
					m_bFold = FALSE;
				}
			}
			else if( m_nSlidingDir == eSLIDING_LEFT )
			{
				if( m_nSlidingDir == eSLIDING_NONE )
				{
					m_lOriX = (LONG)GetAbsX();
					m_lOriY = (LONG)GetAbsY();
					m_lSlidingX = 0;	
				}
				m_nSlidingDir = eSLIDING_RIGHT;
			}
			else
			{
				if( m_nSlidingDir == eSLIDING_NONE )
				{
					m_lOriX = (LONG)GetAbsX() - m_IconBarWidth*m_nIconNum;
					m_lOriY = (LONG)GetAbsY();
					m_lSlidingX = m_IconBarWidth*m_nIconNum;	
				}
				m_nSlidingDir = eSLIDING_LEFT;
			}
		}
	}
}
void CMainBarDialog::RepositionIconAbs(POSTYPE AtPos, CBarIcon * pIcon)
{
	pIcon->SetData(AtPos);
	WORD real_x = (WORD)(GetAbsX() + m_lOffSetX + (m_IconBarWidth*AtPos)+(m_IconBarSpace*(AtPos+1)));
	WORD real_y = (WORD)GetAbsY();
	pIcon->SetAbsXY(real_x, real_y);
}
void CMainBarDialog::RepositionIcon(int offset, CBarIcon * pIcon)
{
	POSTYPE AtPos = pIcon->GetData()+offset;
	RepositionIconAbs(AtPos, pIcon);
}
BOOL CMainBarDialog::AddIcon(POSTYPE AtPos, CBarIcon * pIcon)
{
	RepositionIconAbs(AtPos, pIcon);
	m_MainList.AddTail(pIcon);
	PTRLISTPOS pos = m_MainList.GetHeadPosition();
	while(pos)
	{
		CBarIcon * pIcon = (CBarIcon * )m_MainList.GetAt(pos);
		if(pIcon->GetData() == AtPos)
		{
		}
		else if(pIcon->GetData() > AtPos)
		{
			RepositionIcon(1, pIcon);
		}
		m_MainList.GetNext(pos);
	}
	return TRUE;
}
BOOL CMainBarDialog::RemoveIcon(POSTYPE AtPos, CBarIcon ** ppIcon)
{
	PTRLISTPOS pos = m_MainList.GetHeadPosition();
	while(pos)
	{
		CBarIcon * pIcon = (CBarIcon * )m_MainList.GetAt(pos);
		if(pIcon->GetData() == AtPos)
		{
			m_MainList.RemoveAt(pos);
			*ppIcon = pIcon;
		}
		else if(pIcon->GetData() > AtPos)
		{
			RepositionIcon(-1, pIcon);
		}
		m_MainList.GetNext(pos);
	}
	return TRUE;
}
BOOL CMainBarDialog::MoveIcon(POSTYPE FromPos, POSTYPE ToPos)
{
	if( FromPos == ToPos)
	{
		ASSERT(0);
		return TRUE;
	}
	int offset = -1;
	POSTYPE lowPos = FromPos;
	POSTYPE HighPos = ToPos;
	if(FromPos > ToPos)
	{
		lowPos = ToPos;
		HighPos = FromPos;
		offset = 1;
	}
	PTRLISTPOS pos = m_MainList.GetHeadPosition();
	while(pos)
	{
		CBarIcon * pIcon = (CBarIcon * )m_MainList.GetAt(pos);
		if( pIcon->GetData() == FromPos )
		{
			if(FromPos < ToPos)
				RepositionIconAbs(ToPos-1, pIcon);
			else
				RepositionIconAbs(ToPos, pIcon);
		}
		else
		{		
			if(FromPos < ToPos)
			{
				if( lowPos < pIcon->GetData() && pIcon->GetData() < HighPos )
				{
					RepositionIcon(offset, pIcon);
				}				
			}
			else
			{
				if( lowPos <= pIcon->GetData() && pIcon->GetData() < HighPos )
				{
					RepositionIcon(offset, pIcon);
				}				
			}
		}
		m_MainList.GetNext(pos);
	}
	return TRUE;
}
void CMainBarDialog::Render()
{
	if(!m_bActive) return;
	if( !m_bFold )
	{
		PTRLISTPOS pos = m_MainList.GetHeadPosition();
		while(pos)
		{
			CBarIcon * pIcon = (CBarIcon * )m_MainList.GetNext(pos);
			if( pIcon )
			if( pIcon->GetAbsX() + pIcon->GetWidth() < GetAbsX() + GetWidth() )
				pIcon->Render();			
		}
	}

	//RenderNotifyTitle();

	if (AISETMGR->GetGameAI()->sPage5.pbAutoLeveling)
	{
		static DWORD dwTime = gCurTime;
		static DWORD dwSpecialTime = gCurTime;
		static int n = 0;
		static int nSpecial = 0;

		VECTOR2 vPos;
		vPos.x = m_absPos.x + 59;
		vPos.y = m_absPos.y + 5;
		m_ImgNgAlram[n].RenderSprite(&m_scale, NULL, 0, &vPos, 0xffffffff);
		if (gCurTime - dwTime > 70)
		{
			n = (n + 1) % 8;
			dwTime = gCurTime;
		}
	}
	cDialog::Render();
}
void CMainBarDialog::PushMenuID( DWORD windowID, BOOL bPushed )
{
	int idx = -1;
	switch(windowID)
	{
	case HI_DLG:
		{
			idx = OPT_HELPDLGICON;
		}
		break;
	case CI_CHARDLG:
		{
			idx = OPT_CHARACTERDLGICON;
		}
		break;
	case IN_INVENTORYDLG:
		{
			idx = OPT_INVENTORYDLGICON;
		}
		break;
	case MGI_MUGONGSURYUNDLG:
		{
			idx = OPT_MUGONGDLGICON;
		}
		break;
	case MNM_DIALOG:
		{
			idx = OPT_MINIMAPDLGICON;
		}
		break;
	case FRI_FRIENDDLG:
		{
			idx = OPT_FRIENDDLGICON;
		}
		break;
	case NOTE_NOTEDLG:
		{
			idx = OPT_NOTEDLGICON;
		}
		break;
	case OTI_TABDLG:
		{
			idx = OPT_OPTIONDLGICON;
		}
		break;	
	case EXT_DIALOG:
		{
			idx = OPT_EXITDLGICON;
		}
		break;
	}
	if( idx != -1 )
	{
		CBarIcon * bar = GetBarIcon(idx);
		if( bar )
		{
			bar->SetPush( bPushed );
		}
	}
}
#include "MHCamera.h"
void CMainBarDialog::PushDirectMenu( WORD idx )
{
	if( idx != -1 && CAMERA->GetCameraMode() != eCM_EyeView)
	{
		PushMenu( idx );
	}
}
CBarIcon * CMainBarDialog::GetBarIcon(WORD kind)
{
	PTRLISTPOS pos = m_MainList.GetHeadPosition();
	while(pos)
	{
		CBarIcon * pIcon = (CBarIcon * )m_MainList.GetNext(pos);
		if( pIcon->GetInfo()->kind == kind)
			return pIcon;
	}
	return NULL;
}
void CMainBarDialog::PushMenu( WORD idx )
{
	cDialog * pHandler = NULL;
	CBarIcon* pBarIcon = GetBarIcon( idx );
	if( !pBarIcon ) return;
	if( GAMEIN->GetGameInInitKind() == eGameInInitKind_SuryunEnter || 
		GAMEIN->GetGameInInitKind() == eGameInInitKind_EventMapEnter )
		return;

	switch(idx)
	{
		case OPT_MINIMAPDLGICON:
		{
			pHandler = (cDialog*)GAMEIN->GetMiniMapDialog();
			if( !pHandler ) return ;
			if( pHandler->IsActive() )
			{
				pHandler->SetActive(FALSE);
			}
			else
			{
				pHandler->SetActive(TRUE);
				GAMEEVENTMGR->AddEvent( eGameEvent_MapInterface );
			}
		}
		break;
	
	case OPT_EXITDLGICON:
		{
			if( BATTLESYSTEM->GetBattle(HERO)->GetBattleKind() == eBATTLE_KIND_GTOURNAMENT )
				return;
			pHandler = (cDialog*)GAMEIN->GetExitDialog();
			if( !pHandler ) return ;
			if( pHandler->IsActive() )
			{
				pHandler->SetActive(FALSE);
			}
			else
			{
				if( !WINDOWMGR->GetWindowForID( MBI_EXIT ) )
				if( !EXITMGR->IsExitWaiting() )
					pHandler->SetActive(TRUE);
				GAMEEVENTMGR->AddEvent( eGameEvent_ExitInterface );
			}
		}
		break;
	default:
		{
			ASSERT(0);
		}
	}





	if(!GAMEIN->GetSafeNumberIcon()->GetSafeStatic()) 
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(2599) ); 
		return;
	}
	switch(idx)
	{
	case OPT_MALLNOTICEDLGICON:
		{
			//if(!GAMEIN->GetSafeNumberIcon()->GetSafeStatic()) 
			//{
			//	CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(2599) ); 
			//	return;
			//}
		    CGoldShopDialog* pHandlerGold = (CGoldShopDialog*)GAMEIN->GetGoldShopDialog();
			if( !pHandlerGold ) return;
			//pHandlerGold->Linking();
			if( pHandlerGold->IsActive() )
			{
				pHandlerGold->SetShopOpen(TRUE);
				pHandlerGold->HideDealer();
				pHandlerGold->SetActive( FALSE );
			}
			else
			{
				pHandlerGold->SetShopOpen(FALSE);
				pHandlerGold->ShowDealer();
				pHandlerGold->SetActive( TRUE );
			}
			GAMEIN->UpdataGoldMoney();
		//	GAMEIN->GetGoldShopDialog()->SendSelectRequest(HEROID);
		}
		break;
	case OPT_HELPDLGICON:    
		{
			//if(!GAMEIN->GetSafeNumberIcon()->GetSafeStatic()) 
			//{
			//	CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(2599) ); 
			//	return;
			//}
			DWORD dwCheckBit = eEventMap | eTournament | eQuestRoom | eSurvivalMap;
			if(MAP->IsMapKind(dwCheckBit))return;
			pHandler = (cDialog*)GAMEIN->GetOptionDialog();
			if( !pHandler ) return ;
			if( pHandler->IsActive() )
			{
				pHandler->SetActive(FALSE);
				GAMEIN->GetOptionDialog()->SelectTab( 4 );
			}
			else
			{
				if(!MAP->CheckMapKindIs(eSurvival) ) 
				{
					pHandler->SetActive(TRUE);
					GAMEIN->GetOptionDialog()->SelectTab( 4 );
				}
				GAMEEVENTMGR->AddEvent( eGameEvent_OptionInterface );
			}
		}
		break;
	case OPT_CHARACTERDLGICON:
		{
			//if(!GAMEIN->GetSafeNumberIcon()->GetSafeStatic()) 
			//{
			//	CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(2599) ); 
			//	return;
			//}
			pHandler = GAMEIN->GetCharacterDialog();
			if( !pHandler )	return;
			if( pHandler->IsActive() )
			{
				pHandler->SetActive( FALSE );
			}
			else 
			{
				((CCharacterDialog*)pHandler)->UpdateData();
				pHandler->SetActive( TRUE );	
				GAMEEVENTMGR->AddEvent( eGameEvent_CharInterface );
			}
			pBarIcon->SetPush( pHandler->IsActive() );
			GAMEIN->GetGuageDlg()->SetTopOnActive(TRUE);
			GAMEIN->GetGuageDlg()->SetFocus(TRUE);
			GAMEIN->GetGuageDlg()->SetActive(TRUE);
		}
		break;
	case OPT_INVENTORYDLGICON:
		{
			//if(!GAMEIN->GetSafeNumberIcon()->GetSafeStatic()) 
			//{
			//	CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(2599) ); 
			//	return;
			//}
			pHandler = (cDialog*)GAMEIN->GetInventoryDialog();
			if( !pHandler ) return ;
			if( pHandler->IsActive() )
			{
				pHandler->SetActive( FALSE );
			}
			else 
			{
				GAMEIN->GetInventoryDialog()->SetBackGoldMoney(HERO->GetGoldMoney());
				GAMEIN->GetInventoryDialog()->SetBackMallMoney(HERO->GetMallMoney());
				GAMEIN->GetInventoryDialog()->SetBackMoney(HERO->GetMoney());

				pHandler->SetActive( TRUE );
				GAMEIN->UpdataGoldMoney();
				GAMEEVENTMGR->AddEvent( eGameEvent_InvenInterface );
			}
			GAMEIN->GetInventoryDialog()->CheckItemForTidyBtn();
			pBarIcon->SetPush( pHandler->IsActive() );
		}
		break;
	case OPT_MUGONGDLGICON:
		{
			//if(!GAMEIN->GetSafeNumberIcon()->GetSafeStatic()) 
			//{
			//	CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(2599) ); 
			//	return;
			//}
			pHandler = (cDialog*)GAMEIN->GetMugongSuryunDialog();
			if( !pHandler ) return ;
			if( pHandler->IsActive() )
			{
				pHandler->SetActive( FALSE );
			}
			else 
			{
				pHandler->SetActive( TRUE );
				GAMEEVENTMGR->AddEvent( eGameEvent_MugongInterface );
			}
			pBarIcon->SetPush( pHandler->IsActive() );
		}
		break;
	case OPT_QUESTDLGICON:
		{
			//if(!GAMEIN->GetSafeNumberIcon()->GetSafeStatic()) 
			//{
			//	CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(2599) ); 
			//	return;
			//}
			pHandler = (cDialog*)GAMEIN->GetQuestTotalDialog();
			if( !pHandler ) return ;
			if( pHandler->IsActive() )
			{
				pHandler->SetActive(FALSE);
			}
			else
			{
				GAMEIN->GetQuestDialog()->CheckQuestDate();
				pHandler->SetActive(TRUE);
				GAMEEVENTMGR->AddEvent( eGameEvent_QuestInterface );
			}
			pBarIcon->SetPush( pBarIcon->IsActive() );	
		}
		break;
	case OPT_GUILDDLGICON:
		{
			//if(!GAMEIN->GetSafeNumberIcon()->GetSafeStatic()) 
			//{
			//	CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(2599) ); 
			//	return;
			//}
			if( BATTLESYSTEM->GetBattle(HERO)->GetBattleKind() == eBATTLE_KIND_GTOURNAMENT )
				return;
			pHandler =  (cDialog*)GAMEIN->GetGuildDlg();
			if( !pHandler ) return ;
			if( pHandler->IsActive() )
			{
				pHandler->SetActive(FALSE);
			}
			else
			{
				if(HERO->GetGuildIdx() != 0)
					pHandler->SetActive(TRUE);
				else
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 35 ));
				GAMEEVENTMGR->AddEvent( eGameEvent_MunpaInterface );
			}
			pBarIcon->SetPush( !pBarIcon->IsPushed() );
		}
		break;
	case OPT_FRIENDDLGICON:
		{
			//if(!GAMEIN->GetSafeNumberIcon()->GetSafeStatic()) 
			//{
			//	CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(2599) ); 
			//	return;
			//}
			pHandler = (cDialog*)GAMEIN->GetFriendDialog();
			if( !pHandler ) return ;
			if( pHandler->IsActive() )
			{
				pHandler->SetActive(FALSE);
			}
			else
			{
				FRIENDMGR->FriendListSyn(1);
				GAMEEVENTMGR->AddEvent( eGameEvent_FriendInterface );
			}
			pBarIcon->SetPush( pHandler->IsActive() );
		}
		break;
	case OPT_NOTEDLGICON:
		{
			//if(!GAMEIN->GetSafeNumberIcon()->GetSafeStatic()) 
			//{
			//	CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(2599) ); 
			//	return;
			//}
			pHandler = (cDialog*)GAMEIN->GetNoteDialog();
			if( !pHandler ) return ;
			if( pHandler->IsActive() )
			{
				pHandler->SetActive(FALSE);
			}
			else
			{
				NOTEMGR->NoteListSyn(1);
				GAMEEVENTMGR->AddEvent( eGameEvent_NoteInterface );
			}
			pBarIcon->SetPush( pHandler->IsActive() );
		}
		break;
	case OPT_OPTIONDLGICON:
		{
			//if(!GAMEIN->GetSafeNumberIcon()->GetSafeStatic()) 
			//{
			//	CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(2599) ); 
			//	return;
			//}
			pHandler = (cDialog*)GAMEIN->GetOptionDialog();
			if( !pHandler ) return ;
			if( pHandler->IsActive() )
			{
				pHandler->SetActive(FALSE);
			}
			else
			{
				pHandler->SetActive(TRUE);
				GAMEEVENTMGR->AddEvent( eGameEvent_OptionInterface );
			}
		}
		break;
	case OPT_PKMODEICON:
		{
			//if(!GAMEIN->GetSafeNumberIcon()->GetSafeStatic()) 
			//{
			//	CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(2599) ); 
			//	return;
			//}
			if( !PKMGR->ToggleHeroPKMode() )
				pBarIcon->SetPush( !pBarIcon->IsPushed() );	
		}
		break;
	default:
		{
			ASSERT(0);
		}
	}
}


void CMainBarDialog::SetPushBarIcon( WORD idx, BOOL bPushed )
{
	CBarIcon* pBarIcon = GetBarIcon( idx );
	if( pBarIcon )
		pBarIcon->SetPush( bPushed );
}

void CMainBarDialog::Fold( BOOL bFold )
{
	if( bFold == m_bFold ) return;

	m_bFold = bFold;

	if( bFold )
	{
		m_lSlidingX = m_IconBarWidth*m_nIconNum;

		m_pBtnSize->SetRelXY( m_lOffSetX, 0 );
		SetAbsXY( m_lOriX + m_lSlidingX, m_lOriY );
		SetWH( m_lOffSetX + m_pBtnSize->GetWidth(), GetHeight() );
		m_nSlidingDir = eSLIDING_NONE;
				
		m_lOriX = (LONG)GetAbsX() - m_lSlidingX;
		m_lOriY = (LONG)GetAbsY();
	}
	else
	{
		m_lSlidingX = 0;
		m_pBtnSize->SetRelXY( m_lOffSetX + m_IconBarWidth*m_nIconNum, 0 );
		SetAbsXY( m_lOriX, m_lOriY );
		SetWH( m_lOffSetX + m_pBtnSize->GetWidth() + m_IconBarWidth*m_nIconNum, GetHeight() );
		
		m_nSlidingDir = eSLIDING_NONE;
		
		m_lOriX = (LONG)GetAbsX() - m_lSlidingX;
		m_lOriY = (LONG)GetAbsY();
		
	}
}
#define MAX_LOADSTRING 512
extern TCHAR szTitle[MAX_LOADSTRING];
extern TCHAR szTempTitle[MAX_LOADSTRING];
extern HWND _g_hWnd;
void CMainBarDialog::SetAlram( int idx, BOOL bAlram )
{
	if(idx==OPT_PRESETFORDEAD)
		goto deadbody;
	CBarIcon* pBarIcon = GetBarIcon( idx );
	pBarIcon->SetAlram( bAlram );

	DWORD dwError=0;
	TCHAR szTemp[MAX_LOADSTRING];
	//char * szTemp={0,};

	if(idx==OPT_NOTEDLGICON /*&& bAlram==true*/)
	{
		dwError=1;
		wsprintf(szTemp,CHATMGR->GetChatMsg(95));
		//SetNotifyTitle(CHATMGR->GetChatMsg(95));
	}
	else if(idx==OPT_PRESETFORDEAD /*&& bAlram==true*/)
	{
deadbody:
		dwError=1;
		wsprintf(szTemp,CHATMGR->GetChatMsg(2495));
	}
	else
	{
		dwError=2;
	}

	if(dwError == bAlram)
	{
		SetNotifyTitle(szTemp);
	}
	else
	{
		SetDefaultTitle();
	}
}







extern BOOL bAllowToShowNotify;
void CMainBarDialog::RenderNotifyTitle()
{
	if(!m_bFlicker)	return;

	if(!bAllowToShowNotify)
	{
		SetWindowTextA(_g_hWnd,szTitle);
		return;
	}


	static int TICK_PER_IMAGE;
	//static bool bTemp=false;
	if(gCurTime-TICK_PER_IMAGE>1500)
	{
		TICK_PER_IMAGE=gCurTime;

		m_bStaticFlicker=!m_bStaticFlicker;
	}


	if(m_bStaticFlicker)
		SetWindowTextA(_g_hWnd,"_");
	else
		SetWindowTextA(_g_hWnd,szTempTitle);
}




void CMainBarDialog::SetNotifyTitle(char * msg)
{
	m_bFlicker=true;
	SafeStrCpy(szTempTitle,msg,512);	
}


void CMainBarDialog::SetDefaultTitle()
{
	m_bFlicker=m_bStaticFlicker=false;
	SetWindowTextA(_g_hWnd,szTitle);
}