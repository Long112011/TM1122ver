









// ItemShow.cpp: implementation of the CItemShow class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "ItemShow.h"


#include "ItemManager.h"
#include "./Interface/cFont.h"
#include "interface/cScriptManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CItemShow::CItemShow()

{
	m_type = WT_ITEMSHOW;
	cIcon::Init( 0, 0, DEFAULT_ICONSIZE, DEFAULT_ICONSIZE, NULL, -1 );
	

	m_wItemIdx	= 0;
	m_Dur		= 0;

	m_CurScrollImage = 0;

	for(int i=0;i<22;i++)
	{
		SCRIPTMGR->GetImage(108+i, &m_ScrollImg[i], PFT_JACKPATH);
	}

}


CItemShow::~CItemShow()
{


}

void CItemShow::Init(WORD wItemIdx, DURTYPE dur, WORD Grade30, WORD Option, WORD Rare, WORD Grow)
{
	m_wItemIdx = wItemIdx;
	m_Dur = dur;
	if(m_wItemIdx)
	{
		cImage tmpImage;

		ITEMMGR->GetIconImage( wItemIdx, &tmpImage );
		SetBasicImage(&tmpImage);

	}
	else
	{
		SetCurImage(NULL);
	}

}
void CItemShow::Render()
{
	cIcon::Render();
//MIX DIALOG PUN GUNA NI //BY JACK
#ifndef _CLASSICONE
	ITEM_INFO * pInfo = ITEMMGR->GetItemInfo(m_wItemIdx);
	if( ITEMMGR->IsDupItem(m_wItemIdx) )
	{
		char Temp[128]={0,};
		wsprintf(Temp,"%4d", m_Dur);
		RECT rect={(LONG)m_absPos.x+15, (LONG)m_absPos.y+29, 1,1};
		CFONT_OBJ->RenderFont(0,Temp,strlen(Temp),&rect,RGBA_MERGE(m_dwImageRGB, m_alpha * m_dwOptionAlpha / 100 ));
		//static char nums[3];
		//wsprintf(nums,"%2d", m_Dur);
		//RECT rect={(LONG)m_absPos.x+27, (LONG)m_absPos.y+29, 1,1};
		//CFONT_OBJ->RenderFont(0,nums,strlen(nums),&rect,RGBA_MERGE(m_dwImageRGB, m_alpha * m_dwOptionAlpha / 100 ));		// color hard coding : taiyo 


	}

	if(pInfo)
	{
		VECTOR2 vScale = { 1.0f, 1.0f };
		VECTOR2 pos = { m_absPos.x, m_absPos.y };
		if(pInfo->ItemGrade>=1&&pInfo->ItemGrade<=9)//ICON LEVEL GRADE
		{
			char Temp[128]={0,};
			sprintf(Temp,"+%d",pInfo->ItemGrade);
			RECT rect={(LONG)m_absPos.x+1, (LONG)m_absPos.y+1, 1,1};

			CFONT_OBJ->RenderNoticeMsg(14,Temp,strlen(Temp),&rect,RGBA_MERGE(RGB_HALF(248 ,248, 255),255),RGBA_MAKE(100,100,100,100));
		}
		if(pInfo->ItemGrade>=11&&pInfo->ItemGrade<=19)//ICON LEVEL GRADE
		{
			char Temp[128]={0,};
			sprintf(Temp,"+%d",pInfo->ItemGrade-10);
			RECT rect={(LONG)m_absPos.x+1, (LONG)m_absPos.y+1, 1,1};

			CFONT_OBJ->RenderNoticeMsg(14,Temp,strlen(Temp),&rect,RGBA_MERGE(RGB_HALF(248 ,248, 255),255),RGBA_MAKE(100,100,100,100));//TTTC_ITEM_PINK
		}
		if(pInfo->ItemGrade>=10&&pInfo->ItemGrade<=19)//ICON LEVEL GRADE
		{
			cImage			m_JackIcon;
			//SCRIPTMGR->GetImage( 19, &m_JackIcon, PFT_JACKPATH );
			//VECTOR2 pos = { m_absPos.x+27, m_absPos.y+1};
			//m_JackIcon.RenderSprite( &vScale, NULL, 0.0f, &pos,0xffffffff);
			SCRIPTMGR->GetImage( 130, &m_JackIcon, PFT_JACKPATH );
			m_JackIcon.RenderSprite( &vScale, NULL, 0.0f, &pos,0xffffffff);

			static int TICK_PER_IMAGE = 30;
			m_CurScrollImage+=gTickTime;
			int curImage = m_CurScrollImage % (TICK_PER_IMAGE*20);
			curImage /= TICK_PER_IMAGE;
			m_ScrollImg[curImage].RenderSprite(NULL,NULL,0,&pos,0xffffffff);

			SCRIPTMGR->GetImage( 19, &m_JackIcon, PFT_JACKPATH );
			VECTOR2 pos = { m_absPos.x+27, m_absPos.y+1};
			m_JackIcon.RenderSprite( &vScale, NULL, 0.0f, &pos,0xffffffff);
		}
	}
#else
	if( ITEMMGR->IsDupItem(m_wItemIdx) )
	{
		static char nums[3];
		wsprintf(nums,"%2d", m_Dur);
		RECT rect={(LONG)m_absPos.x+27, (LONG)m_absPos.y+29, 1,1};
		CFONT_OBJ->RenderFont(0,nums,strlen(nums),&rect,RGBA_MERGE(m_dwImageRGB, m_alpha * m_dwOptionAlpha / 100 ));		// color hard coding : taiyo 
	}
#endif

}






