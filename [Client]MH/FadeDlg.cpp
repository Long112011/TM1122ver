// 080930 LYW --- FadeDlg : 페이드 인/아웃 다이얼로그 추가.
#include "stdafx.h"
#include ".\fadedlg.h"
#include "./Interface/cScriptManager.h"
#include "./GameResourceManager.h"
#include "chatmanager.h"
#include "FadeManager.h"
CFadeDlg::CFadeDlg(void)
{
	mScale.x = 1 ;
	mScale.y = 1 ;

	mPos.x = 0 ;
	mPos.y = 0 ;

	dwColor = RGBA_MAKE(10, 10, 10, 255) ;

	m_byAlpha = 200 ;

	m_byFadeMode = eFadeNone ;

	m_Initialize = false;
}

CFadeDlg::~CFadeDlg(void)
{
	m_imageLoginBarTop.Release();
}

void CFadeDlg::Initialize()
{
	if (m_Initialize) return; //return if game already initialized this dialog

	m_Initialize = TRUE;

	m_imageLoginBarTop.LoadSprite("image/2D/login_bar00.TIF");
	//const DISPLAY_INFO& dispInfo = GAMERESRCMNGR->GetResolution();
	//m_nDispWidth  = dispInfo.dwWidth ;
	//m_nDispHeight = dispInfo.dwHeight ;

	//SCRIPTMGR->GetImage( 143, &m_ImgBackGround, PFT_HARDPATH ) ;
	VECTOR2 PosScale = { 1, 1 };
	float scaleRate = (float)GAMERESRCMNGR->m_GameDesc.dispInfo.dwWidth / MIDRSLTN_W;
	PosScale.x = scaleRate;
	mScale.x = PosScale.x;//
	mScale.y = PosScale.y;//
	//mScale.x = m_nDispWidth / 2.0f;
	//mScale.y = m_nDispHeight / 2.0f;
}

void CFadeDlg::FadeIn()
{
	if (!m_Initialize) //return if game wasnt initialize this dialog
	{
		CHATMGR->AddMsg(CTC_SYSMSG, "fadein");
		return;
	}
	if (!FADEMGR->GetManagerInit()) //return if FADEMGR wasnt initialize for this dialog
	{
		CHATMGR->AddMsg(CTC_SYSMSG, "FADEMGR failed");
		return;
	}
	m_byAlpha = 255 ;
	m_byFadeMode = eFadeIn ;
	SetActive(TRUE) ;
}

void CFadeDlg::FadeOut()
{
	if (!m_Initialize) //return if game wasnt initialize this dialog
	{
		CHATMGR->AddMsg(CTC_SYSMSG, "FadeOut");
		return;
	}
	if (!FADEMGR->GetManagerInit()) //return if FADEMGR wasnt initialize for this dialog
	{
		CHATMGR->AddMsg(CTC_SYSMSG, "FADEMGR failed");
		return;
	}
	m_byAlpha = 200 ;
	m_byFadeMode = eFadeOut ;
	SetActive(TRUE) ;
}

void CFadeDlg::Render()
{
	if( !m_bActive ) return ;

	if (m_byFadeMode == eFadeNone)return;
		
	if (!m_Initialize)return;

	if (!FADEMGR->GetManagerInit())return;
	
	if( m_byFadeMode == eFadeIn )
	{
		if (m_byAlpha > 200)
		{
			--m_byAlpha;
			--mPos.y;
		}
		else 
		{
			//m_byFadeMode = eFadeNone ;
			m_byFadeMode = eFadeOut;
			//SetActive(FALSE) ;
		}
	}
	else if( m_byFadeMode == eFadeOut )
	{
		if (m_byAlpha < 255)
		{
			++m_byAlpha;
			++mPos.y;
		}
		else
		{
			//m_byFadeMode = eFadeNone ;
			m_byFadeMode = eFadeIn;
			//SetActive(FALSE) ;
		}
	}

	dwColor = RGBA_MAKE(255, 255, 255, m_byAlpha) ;
	

	m_imageLoginBarTop.RenderSprite(&mScale, 0, 0, &mPos, dwColor/*0xffffffff*/);
	//m_ImgBackGround.RenderSprite(&mScale, NULL, 0, &mPos, 0xffffffff/*dwColor*/);
}