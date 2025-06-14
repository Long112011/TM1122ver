








#include "stdafx.h"
#include "cMsgBox.h"
#include "./interface/cButton.h"
#include "WindowIDEnum.h"
#include "./interface/cWindowManager.h"
#include "./interface/cStatic.h"
#include "./interface/cTextArea.h"
#include "./interface/cScriptManager.h"
#include "./Interface/cResourceManager.h"
#include "./input/cImeWnd.h"

#include "./Input/Keyboard.h"
#include "ChatManager.h"
#include "GameResourceManager.h"

#include "GameState.h"
#include "MainGame.h"

#include "ChatDialog.h"
#include "gamein.h"

#include "JackFlashRoar.h"
#include "aimanager.h"

#define YButtonPos 120

#define MB_WIDTH	197
#define MB_HEIGHT	150

#define MB_BTN1_X	131
#define MB_BTN1_Y	YButtonPos
#define MB_BTN2_X	69
#define MB_BTN2_Y	YButtonPos
#define MB_BTN_W	56
#define MB_BTN_H	19

#define MB_TEXT_X	15
#define MB_TEXT_Y	50


cImage	cMsgBox::m_DlgImg;
cImage	cMsgBox::m_BtnImgBasic;
cImage	cMsgBox::m_BtnImgOver;
cImage	cMsgBox::m_BtnImgPress;
char	cMsgBox::m_BtnText[MB_BTN_COUNT][32]		= {0, };
BOOL	cMsgBox::m_bInit = FALSE;

DWORD	cMsgBox::m_dwBtnTextColor[3] = { 0x00ffffff, 0x00ffff00, 0x00ffff00 };

DWORD	TextLegthHeight = 0;

int bTempBoxT=0;
cMsgBox::cMsgBox()
{
	m_pMsg			= NULL;
//	m_pMsgText		= NULL;
	m_pButton[0]	= NULL;
	m_pButton[1]	= NULL;

	m_dwMsgColor = 0x00ffffff;

	m_nDefaultBtn	= -1;
	m_dwParam  = 0;
	m_type = WT_MSGBOX;

	m_nMBType = MBT_NOBTN;

	bTempBox=-1;

	bTempBoxT=0;
}

//=========================================================================
//	NAME : ~cMsgBox
//	DESC : The function construc destructor.
//=========================================================================
cMsgBox::~cMsgBox()
{
}


void cMsgBox::InitMsgBox()
{
	char imagePath[256]={0,};
	char imageMsg[32]={0,};

	SCRIPTMGR->GetImage( 30, &m_DlgImg );
	SCRIPTMGR->GetImage( 31, &m_BtnImgBasic );
	SCRIPTMGR->GetImage( 32, &m_BtnImgOver );
	SCRIPTMGR->GetImage( 33, &m_BtnImgPress );


	strcpy( m_BtnText[MB_BTN_OK], RESRCMGR->GetMsg( 221 ) );
	strcpy( m_BtnText[MB_BTN_YES], RESRCMGR->GetMsg( 222 ) );
	strcpy( m_BtnText[MB_BTN_NO], RESRCMGR->GetMsg( 223 ) );

	strcpy( m_BtnText[MB_BTN_CANCEL], RESRCMGR->GetMsg( 224 ) );

	m_bInit = TRUE;

	bTempBoxT=0;
}


void cMsgBox::MsgBox( LONG lId, int nMBType, char* strMsg, cbFUNC cbMBFunc )
{
	RECT rtCaption = { 0, 0, MB_WIDTH, 20 };


	cDialog::Init( ( GET_MAINWIN_W - MB_WIDTH ) / 2, ( GET_MAINWIN_H - MB_HEIGHT ) / 2,
		MB_WIDTH, MB_HEIGHT, &m_DlgImg, lId );

	SetMovable(TRUE);
	SetCaptionRect(&rtCaption);

	m_cbMBFunc = cbMBFunc;
	
	cStatic* pStatic = new cStatic;
	pStatic->Init( 13, 2, 0, 0, NULL, -1 );

	pStatic->SetStaticText( CHATMGR->GetChatMsg(210) );
	pStatic->SetShadow( TRUE );
	pStatic->SetFontIdx( 2 );
	pStatic->SetAlign( TXT_LEFT );
	Add( pStatic );

	m_nMBType = nMBType;
	switch( nMBType )
	{
	case MBT_NOBTN:
		{
		}
		break;

	case MBT_OK:
		{
			LONG btn1x = (MB_WIDTH - MB_BTN_W)/2;


			m_pButton[0] = new cButton;
			m_pButton[0]->Init( btn1x, MB_BTN1_Y, MB_BTN_W, MB_BTN_H, &m_BtnImgBasic, &m_BtnImgOver, &m_BtnImgPress, NULL, MBI_OK );

			m_pButton[0]->SetText( m_BtnText[MB_BTN_OK], m_dwBtnTextColor[0], m_dwBtnTextColor[1], m_dwBtnTextColor[2] );
			m_pButton[0]->SetAlign( TXT_CENTER );
			m_pButton[0]->SetActive( m_bActive );
			m_pButton[0]->SetAlpha( (BYTE)m_alpha );
			Add( m_pButton[0] );
			bTempBoxT=bTempBoxT+1;
		}
		break;

	case MBT_YESNO:
		{
			LONG btn1x = (MB_WIDTH - (MB_BTN_W*2) - 6)/2;
			LONG btn2x = btn1x + MB_BTN_W + 6;

			m_pButton[0] = new cButton;
			m_pButton[0]->Init( btn1x, MB_BTN2_Y, MB_BTN_W, MB_BTN_H, &m_BtnImgBasic, &m_BtnImgOver, &m_BtnImgPress, NULL, MBI_YES );

			m_pButton[0]->SetText( m_BtnText[MB_BTN_YES], m_dwBtnTextColor[0], m_dwBtnTextColor[1], m_dwBtnTextColor[2] );
			m_pButton[0]->SetAlign( TXT_CENTER );
			m_pButton[0]->SetActive( m_bActive );
			m_pButton[0]->SetAlpha( (BYTE)m_alpha );
			Add( m_pButton[0] );

			m_pButton[1] = new cButton;

			m_pButton[1]->Init( btn2x, MB_BTN1_Y, MB_BTN_W, MB_BTN_H, &m_BtnImgBasic, &m_BtnImgOver, &m_BtnImgPress, NULL, MBI_NO );

			m_pButton[1]->SetText( m_BtnText[MB_BTN_NO], m_dwBtnTextColor[0], m_dwBtnTextColor[1], m_dwBtnTextColor[2] );
			m_pButton[1]->SetAlign( TXT_CENTER );
			m_pButton[1]->SetActive( m_bActive );

			m_pButton[1]->SetAlpha( (BYTE)m_alpha );
			Add( m_pButton[1] );
			bTempBoxT=bTempBoxT+1;
		}
		break;

	case MBT_CANCEL:
		{
			LONG btn1x = (MB_WIDTH - MB_BTN_W)/2;

			m_pButton[0] = new cButton;
			m_pButton[0]->Init( btn1x, MB_BTN1_Y, MB_BTN_W, MB_BTN_H, &m_BtnImgBasic, &m_BtnImgOver, &m_BtnImgPress, NULL, MBI_CANCEL );


			m_pButton[0]->SetText( m_BtnText[MB_BTN_CANCEL], m_dwBtnTextColor[0], m_dwBtnTextColor[1], m_dwBtnTextColor[2] );

			m_pButton[0]->SetAlign( TXT_CENTER );
			m_pButton[0]->SetActive( m_bActive );
			m_pButton[0]->SetAlpha( (BYTE)m_alpha );
			Add( m_pButton[0] );
		}
		break;


	}

	m_pMsg = new cTextArea;

	if(strlen(strMsg)>100)
	{
		TextLegthHeight=-15;
	}
	else
	{
		TextLegthHeight=0;
	}

	RECT rtText = { 0, TextLegthHeight/*0*/, MB_WIDTH/*196*/ - MB_TEXT_X * 2, 90 };

	

	m_pMsg->Init(MB_TEXT_X, MB_TEXT_Y, MB_WIDTH - MB_TEXT_X, 90, NULL );

	m_pMsg->InitTextArea( &rtText, 256 );
	m_pMsg->SetAlpha((BYTE)m_alpha);
	m_pMsg->SetScriptText( strMsg );
	m_pMsg->SetActive(m_bActive);	
	Add( m_pMsg );

	
	if(MAINGAME->GetCurStateNum() == eGAMESTATE_GAMEIN && GAMEIN->GetJackFlashDlg()->IsActive())
	{
		GAMEIN->GetJackFlashDlg()->SetActive(false);
		GAMEIN->GetJackFlashDlg()->SetDisable(true);
	}
	
	m_type = WT_MSGBOX;
}


DWORD cMsgBox::ActionEvent( CMouse * mouseInfo )
{

	DWORD we = WE_NULL;
	DWORD weBtn = WE_NULL;
	
	if( !m_bActive ) return we;

	we |= cWindow::ActionEvent( mouseInfo );
	we |= cDialog::ActionEventWindow( mouseInfo);

//	BOOL bClicked = FALSE;

	for( int i = 0 ; i < 2 ; ++ i )
	{
		if( m_pButton[i] )
		{
			weBtn = m_pButton[i]->ActionEvent( mouseInfo );
			if( weBtn & WE_BTNCLICK )
			{
				ForcePressButton(m_pButton[i]->GetID());
				//bTempBoxT=bTempBoxT-1;
				//if( m_cbMBFunc )
				//	m_cbMBFunc( m_ID, this, m_pButton[i]->GetID() );
				//bClicked = TRUE;
				break;
			}
		}		
	}
/*
	if( bClicked )
	{
		SetActive( FALSE );
		WINDOWMGR->AddListDestroyWindow( this );
	}
*/
	return we;
}


DWORD cMsgBox::ActionKeyboardEvent( CKeyboard * keyInfo )
{
	if( !m_bActive ) return WE_NULL;
	//DWORD l_dwRet = WE_NULL;

	//if (CIMEWND->IsDocking()) return WE_NULL;
	
	if( MAINGAME->GetCurStateNum() == eGAMESTATE_GAMEIN  )
	{		
		if( MBT_YESNO == m_nMBType ) 
		{
			for( int i = 0 ; i < 2 ; ++ i )
			{
				if( !m_pButton[i] ) continue ;
					

				if ((keyInfo->GetKeyUp(KEY_RETURN) || keyInfo->GetKeyUp(KEY_PADENTER)) && (MBI_YES == m_pButton[i]->GetID()))
				{
					if( FALSE==CIMEWND->IsDocking() )
					{
						//CHATMGR->AddMsg(CTC_SYSMSG,"docking");
						ForcePressButton(MBI_YES);
						break;
					}
					
				}
				else if( keyInfo->GetKeyDown( KEY_ESCAPE ) && (MBI_NO == m_pButton[i]->GetID()) ) 
				{
					ForcePressButton( MBI_NO ) ;
					break;
				}
			}
		}
		else if( MBT_OK == m_nMBType )
		{
			if( keyInfo->GetKeyDown( KEY_RETURN )||keyInfo->GetKeyDown( KEY_PADENTER ))
			{
				if( CIMEWND->IsDocking() )
				{
					//CHATMGR->AddMsg(CTC_SYSMSG,"docking2");
					return WE_NULL;
				}
				ForcePressButton( MBT_OK ) ;
					
			}
		}
	}

	//return l_dwRet;
	/*if( !m_bActive ) return WE_NULL;

	if( MBT_YESNO == m_nMBType ) 
	{
		for( int i = 0 ; i < 2 ; ++ i )
		{
			if( !m_pButton[i] ) continue ;										

			if( (keyInfo->GetKeyDown( KEY_RETURN ) || 
				keyInfo->GetKeyDown( KEY_PADENTER ))
				&& (MBI_YES == m_pButton[i]->GetID()) ) 
			{
				ForcePressButton( MBI_YES ) ;
				break;
			}
			else if( keyInfo->GetKeyDown( KEY_ESCAPE ) && (MBI_NO == m_pButton[i]->GetID()) ) 
			{
				ForcePressButton( MBI_NO ) ;
				break;
			}
		}
	}
	else if( MBT_OK == m_nMBType )
	{
		if( keyInfo->GetKeyDown( KEY_RETURN )||keyInfo->GetKeyDown( KEY_PADENTER ))
		{
			ForcePressButton( MBT_OK ) ;
		}
	}*/

	return WE_NULL;
}

BOOL cMsgBox::PressDefaultBtn()
{
	if( m_nDefaultBtn == 0 || m_nDefaultBtn == 1 )
	if( m_pButton[m_nDefaultBtn] )
	{
		//bTempBoxT=bTempBoxT-1;
		return ForcePressButton( m_pButton[m_nDefaultBtn]->GetID() );
	}

	return FALSE;
}

//=========================================================================
//	NAME : ForcePressButton
//	DESC : The function to process press event of buttons.
//=========================================================================
BOOL cMsgBox::ForcePressButton( LONG lBtnID )
{
	if( lBtnID <= 0 ) return FALSE ;

	if( !m_cbMBFunc ) return FALSE ;

	m_cbMBFunc( m_ID, this, lBtnID ) ;

	SetActive( FALSE ) ;
	SetDisable( TRUE ) ;

	//if(bTempBoxT>0)
		bTempBoxT=bTempBoxT-1;

	WINDOWMGR->AddListDestroyWindow( this ) ;

	return TRUE ;
}

//=========================================================================
//	NAME : ForceClose
//	DESC : The function to process close event of buttons.
//=========================================================================
void cMsgBox::ForceClose()
{
	bTempBoxT=0;
	switch( m_nMBType )
	{
	case MBT_NOBTN :	ForcePressButton( 0 ) ;				break ;
	case MBT_OK :		ForcePressButton( MBI_OK ) ;		break ;
	case MBT_YESNO :	ForcePressButton( MBI_NO ) ;		break ;
	case MBT_CANCEL :	ForcePressButton( MBI_CANCEL ) ;	break ;
	default :												break ;
	}
}
void cMsgBox::SetMsgBoxText(char* pStrMsg)
{
	m_pMsg->SetScriptText( pStrMsg );
}

//=========================================================================
//	NAME : SetActive
//	DESC : The function to process when it is activated.
//=========================================================================
void cMsgBox::SetActive( BOOL val )
{
	cDialog::SetActive(val);

	if(!m_bDisable && val)
	{
		SetFocus(TRUE);
	}

	return;
}
BOOL cMsgBox::IsFocus()
{
	//CHATMGR->AddMsg(0,"IsFocus() bTempBox %d",bTempBoxT);
	return cDialog::IsFocus();
}
void cMsgBox::ClearOpenedBox()
{
	bTempBox=-1;
}