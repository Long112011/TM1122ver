#include "stdafx.h"
#include "cFindPatnIcon.h"
#include "./Interface/cScriptManager.h"
#include "MiniMapDlg.h"
#include "BigMapDlg.h"
#include "GameIn.h"

#define Speed	20
#define Inline	27
#define POS_X	(3)
#define POS_Y	(21)

static float dwTestMini[Inline]={0.5,6.5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,4,4,3,3,2,2,1.5,1,0.5
};
CFindPatnIcon::CFindPatnIcon(void)
: m_PathFlag(NULL)
{
	m_bShowFindPathMark = FALSE;
	m_Image = NULL;
	m_Image = new cImage;
	//SCRIPTMGR->GetImage(45,m_Image,PFT_HARDPATH);

	m_PathFlag = new cImageSelf;
	m_PathFlag->LoadSprite( "image/2d/flag_mini.tif"/*, 64, 64 */);

	m_ImageWH.x = 13;
	m_ImageWH.y = 18;
	nMouseX = -1;
	nMouseY = -1;
}

CFindPatnIcon::~CFindPatnIcon(void)
{
	if(m_Image)
	{
		delete m_Image;
		m_Image = NULL;
	}
	SAFE_DELETE(m_PathFlag);
}

void CFindPatnIcon::Render()
{
	if(!m_bShowFindPathMark) return;
	VECTOR2 trans;
	static DWORD dwSpecialTime = gCurTime;
	static int n = 2;
	VECTOR2 pv2Scaling;
	pv2Scaling.x = 1.0f;
	pv2Scaling.y = 1.0f;
	CMiniMapDlg* pMiniMapDlg = GAMEIN->GetMiniMapDialog();
	if( m_Image )
	{
		trans.x = pMiniMapDlg->GetAbsX()- POS_X;
		trans.y = pMiniMapDlg->GetAbsY()+152 + dwTestMini[n];
		trans.x += pMiniMapDlg->GetPointPosSave()->x/3.32;
		trans.y -= pMiniMapDlg->GetPointPosSave()->y/3.12;
		m_PathFlag->RenderSprite( 0, NULL, 0, &trans, 0xffffffff );
	}
	if( gCurTime - dwSpecialTime > Speed )
	{
		n = (++n)%Inline;
		dwSpecialTime = gCurTime;
	}
}

