#include "stdafx.h"
#include "MugongBase.h"
#include "./Interface/cFont.h"
#include "./Input/Mouse.h"
#include "interface/cScriptManager.h"
#include "SkillManager_client.h"
#include "ChatManager.h"
#include "ObjectManager.h"
#include "MugongManager.h"
CMugongBase::CMugongBase()
{
	SCRIPTMGR->GetImage( 127, &m_DelayImage  );	
	m_bRemainTime	= FALSE;
	m_dwLastSecond	= 0;
	//DWORD dwRemainTime=0;
	SCRIPTMGR->GetImage( 87, &m_OptionImage, PFT_MUGONGPATH );
}
CMugongBase::~CMugongBase()
{
}
void CMugongBase::SetMugongBase(MUGONGBASE * mugong)
{
	memcpy(&m_MugongBaseInfo, mugong, sizeof(MUGONGBASE));
	SetExpPoint(mugong->ExpPoint);
}
void CMugongBase::Init(LONG x, LONG y, WORD wid, WORD hei, cImage * lowImage, cImage * highImage, LONG ID)
{
	cIcon::Init(x,y,wid,hei,lowImage,ID);
	m_type = WT_MUGONG;
	if(highImage)
		m_pHighLayerImage = *highImage;
	cImage bimg,gimg;
	cImageRect rect;
	SetRect(&rect,1010,216,1011,226);
	SCRIPTMGR->GetImage(0,&bimg,&rect);	
	SetRect(&rect,1006,216,1007,226);
	SCRIPTMGR->GetImage(0,&gimg,&rect); 
	m_Guage.Init(x-1,y+42,40,6, &bimg);
	cImageScale scale;
	scale.x = 40;
	scale.y = 0.6f;
	m_Guage.SetScale(&scale);
	m_Guage.SetGuageImagePos(0,0);
	m_Guage.SetValue(0);
	m_Guage.SetPieceImage(&gimg);
	m_Guage.SetGuageWidth(40);
	m_Guage.SetGuagePieceWidth(1);
	m_Guage.SetGuagePieceHeightScale((float)0.6);
	m_dwFontColor = 0xffffffff;
}
void CMugongBase::Render()
{
	cIcon::Render();
	if(!m_pHighLayerImage.IsNull())
		m_pHighLayerImage.RenderSprite( NULL, NULL, 0, &m_absPos, RGBA_MERGE(m_dwImageRGB, m_alpha * m_dwOptionAlpha / 100));
	if(m_MugongBaseInfo.OptionIndex)
		m_OptionImage.RenderSprite( NULL, NULL, 0, &m_absPos, RGBA_MERGE(m_dwImageRGB, m_alpha * m_dwOptionAlpha / 100));
	if(m_MugongBaseInfo.Sung > 0)
	{
		DWORD color2 = RGBA_MAKE(70,70,70,255);
		static char nums[5];
		if(m_MugongBaseInfo.Sung >= 12)
			wsprintf(nums,"%s", CHATMGR->GetChatMsg(179));
		else
			wsprintf(nums,"%d", m_MugongBaseInfo.Sung);
		RECT rect={(LONG)m_absPos.x+3, (LONG)m_absPos.y+28, 1,1};
		CFONT_OBJ->RenderFont( 0, nums, strlen(nums), &rect, color2 );
		RECT rect2={(LONG)m_absPos.x+2, (LONG)m_absPos.y+27, 1,1};
		CFONT_OBJ->RenderFont( 0, nums, strlen(nums), &rect2, m_dwFontColor );
		DWORD dwRemainTime = 0;
		float delayRate = HERO->GetDelayGroup()->CheckDelay(CDelayGroup::eDK_Skill, GetItemIdx(), &dwRemainTime );
		if( delayRate > 0.0f )
		{
			wsprintf(nums,"%4d", dwRemainTime/1000+1);
			RECT rect={(LONG)m_absPos.x+13, (LONG)m_absPos.y+2, 1,1};			
			VECTOR2 vScale = { 1.0f, delayRate };
			VECTOR2 pos = { m_absPos.x, m_absPos.y + m_height*(1 - delayRate) };
			m_DelayImage.RenderSprite( &vScale, NULL, 0.0f, &pos, 0xff000000  );
			CFONT_OBJ->RenderNoticeMsg(14, nums, strlen(nums), &rect, RGBA_MERGE(RGB_HALF(250, 250, 250), 210), RGBA_MERGE(RGB_HALF(185, 185, 185), 255));
		}
	}
	if(IsDepend())
	{
		BYTE Sung = GetSung();
		if(Sung < 12 && Sung > 0)
		{
			m_Guage.SetAbsXY((LONG)(m_absPos.x-1),(LONG)(m_absPos.y+39));
			m_Guage.Render();
		}
	}
}
void CMugongBase::SetExpPoint(DWORD point)
{
	m_MugongBaseInfo.ExpPoint = point;
	CSkillInfo* pSkillInfo = SKILLMGR->GetSkillInfo(GetItemIdx());	
	if(pSkillInfo == NULL)
		return;
	BYTE Sung = GetSung();
	if(Sung != 12 && Sung != 0)
	{
		GUAGEVAL gval = point / (GUAGEVAL)pSkillInfo->GetNeedExp(Sung);
		m_Guage.SetValue(gval);
	}
}