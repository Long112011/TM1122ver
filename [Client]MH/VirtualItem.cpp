








// VirtualItem.cpp: implementation of the CVirtualItem class.

//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"


#include "VirtualItem.h"
#include "ItemManager.h"


#include "./Interface/cFont.h"
#include "interface/cScriptManager.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction

//////////////////////////////////////////////////////////////////////


CVirtualItem::CVirtualItem()
{
	m_pItemLink = NULL;

	m_type = WT_VIRTUALITEM;
	cIcon::Init( 0, 0, DEFAULT_ICONSIZE, DEFAULT_ICONSIZE, NULL, -1 );

	m_CurScrollImage = 0;

	for(int i=0;i<22;i++)
	{
		SCRIPTMGR->GetImage(108+i, &m_ScrollImg[i], PFT_JACKPATH);
	}
	//IsWTF = 0;
}

CVirtualItem::~CVirtualItem()

{
	//IsWTF = 0;
	m_pItemLink = NULL;
}
void CVirtualItem::SetLinkItem(CBaseItem * pItem)
{
	m_pItemLink = pItem;
	if(pItem)
	{


		SetBasicImage(m_pItemLink->GetBasicImage());

	}

	else
	{
		SetCurImage(NULL);

	}

}
eITEM_KINDBIT CVirtualItem::GetSrcItemKind()
{
	if(m_pItemLink)
	{
		ITEM_INFO * pInfo = ITEMMGR->GetItemInfo(m_pItemLink->GetItemIdx());
		return (eITEM_KINDBIT)pInfo->ItemKind;
	}
	return eKIND_ITEM_MAX;

}
void CVirtualItem::Render()

{
	cIcon::Render();
//UTK MIX DIALOG JGK.. TAPI YG SINGLE GRID PUNYA //utk yg lain adjust dlm itemshow.cpp //BY JACK
#ifndef _CLASSICONE
	ITEM_INFO * pInfo = ITEMMGR->GetItemInfo(m_pItemLink->GetItemIdx());
	if (m_pItemLink && m_pItemLink->GetGrade() > 0)
	{
		const int FONT_WIDTH = 40;    // 估算字体最大宽度
		const int FONT_HEIGHT = 16;   // 估算字体高度
		const int OFFSET_Y = 14;      // 向下偏移量

		char temp[128];
		sprintf(temp, "+%d", m_pItemLink->GetGrade());

		RECT rectTmp;
		rectTmp.left = (LONG)(m_absPos.x + 1);
		rectTmp.top = (LONG)(m_absPos.y + 1 + OFFSET_Y);  // 比 ItemGrade 多移 OFFSET_Y 像素
		rectTmp.right = rectTmp.left + FONT_WIDTH;
		rectTmp.bottom = rectTmp.top + FONT_HEIGHT;

		CFONT_OBJ->RenderFont(21, temp, strlen(temp), &rectTmp, RGBA_MERGE(RGB_HALF(124, 252, 0), 255));
	}

	if( m_pItemLink && ITEMMGR->IsDupItem(m_pItemLink->GetItemIdx()) )//m_pItemLink->GetDurability() >= 1 )
	{
		char Temp[128]={0,};
		wsprintf(Temp,"%4d", m_pItemLink->GetDurability());
		RECT rect={(LONG)m_absPos.x+15, (LONG)m_absPos.y+29, 1,1};
		CFONT_OBJ->RenderFont(0,Temp,strlen(Temp),&rect,RGBA_MERGE(m_dwImageRGB, m_alpha * m_dwOptionAlpha / 100 ));

		//static char nums[3];
		//wsprintf(nums,"%2d", m_pItemLink->GetDurability());
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

			CFONT_OBJ->RenderNoticeMsg(21,Temp,strlen(Temp),&rect,RGBA_MERGE(RGB_HALF(248 ,248, 255),255),RGBA_MAKE(100,100,100,100));
		}
		if(pInfo->ItemGrade>=11&&pInfo->ItemGrade<=19)//ICON LEVEL GRADE
		{
			char Temp[128]={0,};
			sprintf(Temp,"+%d",pInfo->ItemGrade-10);
			RECT rect={(LONG)m_absPos.x+1, (LONG)m_absPos.y+1, 1,1};

			CFONT_OBJ->RenderNoticeMsg(21,Temp,strlen(Temp),&rect,RGBA_MERGE(RGB_HALF(248 ,248, 255),255),RGBA_MAKE(100,100,100,100));//TTTC_ITEM_PINK
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
	if (pInfo->ItemKind >= eTITAN_ITEM && pInfo->ItemKind <= eCHANGE_ITEM_LOCK)
	{
		if (m_pItemLink && m_pItemLink->GetGrade() > 0)
		{
			const int FONT_WIDTH = 40;    // 估算字体最大宽度
			const int FONT_HEIGHT = 16;   // 估算字体高度
			const int OFFSET_Y = 14;      // 向下偏移量

			char temp[128];
			sprintf(temp, "+%d", m_pItemLink->GetGrade());

			RECT rectTmp;
			rectTmp.left = (LONG)(m_absPos.x + 1);
			rectTmp.top = (LONG)(m_absPos.y + 1 + OFFSET_Y);  // 比 ItemGrade 多移 OFFSET_Y 像素
			rectTmp.right = rectTmp.left + FONT_WIDTH;
			rectTmp.bottom = rectTmp.top + FONT_HEIGHT;
			//CFONT_OBJ->RenderFont(m_wFontIdx, "x", 128, &rectTmp, RGBA_MERGE(curLineNode->color, m_alpha * m_dwOptionAlpha / 100));
			CFONT_OBJ->RenderFont(21, temp, strlen(temp), &rectTmp, RGBA_MERGE(RGB_HALF(124, 252, 0), 255));
		}
	}
	if(GetSrcItemKind() & eEQUIP_ITEM)
	{
		VECTOR2 vScale = { 1.0f, 1.0f };
		VECTOR2 pos = { m_absPos.x, m_absPos.y };
		if(GetRareness()!=0)
		{
			cImage			m_JackIcon;
			SCRIPTMGR->GetImage( 20, &m_JackIcon, PFT_JACKPATH );
			VECTOR2 pos = { m_absPos.x, m_absPos.y+27};
			m_JackIcon.RenderSprite( &vScale, NULL, 0.0f, &pos,0xffffffff);
		}
		if(GetSrcDurability() !=0 )
		{	
			cImage			m_JackIcon;
			SCRIPTMGR->GetImage( 21, &m_JackIcon, PFT_JACKPATH );
			VECTOR2 pos = { m_absPos.x+27, m_absPos.y+26};
			m_JackIcon.RenderSprite( &vScale, NULL, 0.0f, &pos,0xffffffff);
		}
		if(GetStoneIdx()!=0)
		{	
			cImage			m_JackIcon;
			SCRIPTMGR->GetImage( 19, &m_JackIcon, PFT_JACKPATH );
			VECTOR2 pos = { m_absPos.x+27, m_absPos.y+1};
			m_JackIcon.RenderSprite( &vScale, NULL, 0.0f, &pos,0xffffffff);
		}
	}
#else
	if( m_pItemLink && ITEMMGR->IsDupItem(m_pItemLink->GetItemIdx()) )//m_pItemLink->GetDurability() >= 1 )
	{
		static char nums[3];
		wsprintf(nums,"%2d", m_pItemLink->GetDurability());
		RECT rect={(LONG)m_absPos.x+27, (LONG)m_absPos.y+29, 1,1};
		CFONT_OBJ->RenderFont(0,nums,strlen(nums),&rect,RGBA_MERGE(m_dwImageRGB, m_alpha * m_dwOptionAlpha / 100 ));		// color hard coding : taiyo 
	}
#endif


	if (pInfo->ItemKind & eEQUIP_ITEM || pInfo->ItemKind == eEQUIP_ITEM_UNIQUE)
	{
		if (m_pItemLink->GetQuality() == 4)
		{
			cImage  imgBaseInfo;
			SCRIPTMGR->GetImage(221, &imgBaseInfo, PFT_HARDPATH);
			VECTOR2 vScale = { 1.0f, 1.0f };
			VECTOR2 pos = { m_absPos.x, m_absPos.y };
			imgBaseInfo.RenderSprite(&vScale, NULL, 0.0f, &pos, 0xffffffff);
		}
		if (m_pItemLink->GetQuality() == 3)
		{
			cImage  imgBaseInfo;
			SCRIPTMGR->GetImage(220, &imgBaseInfo, PFT_HARDPATH);
			VECTOR2 vScale = { 1.0f, 1.0f };
			VECTOR2 pos = { m_absPos.x, m_absPos.y };
			imgBaseInfo.RenderSprite(&vScale, NULL, 0.0f, &pos, 0xffffffff);
		}
		if (m_pItemLink->GetQuality() == 2)
		{
			cImage  imgBaseInfo;
			SCRIPTMGR->GetImage(217, &imgBaseInfo, PFT_HARDPATH);
			VECTOR2 vScale = { 1.0f, 1.0f };
			VECTOR2 pos = { m_absPos.x, m_absPos.y };
			imgBaseInfo.RenderSprite(&vScale, NULL, 0.0f, &pos, 0xffffffff);
		}
		if (m_pItemLink->GetQuality() == 1)
		{
			cImage  imgBaseInfo;
			SCRIPTMGR->GetImage(219, &imgBaseInfo, PFT_HARDPATH);
			VECTOR2 vScale = { 1.0f, 1.0f };
			VECTOR2 pos = { m_absPos.x, m_absPos.y };
			imgBaseInfo.RenderSprite(&vScale, NULL, 0.0f, &pos, 0xffffffff);
		}
		if (m_pItemLink->GetQuality() == 0)
		{
			cImage  imgBaseInfo;
			SCRIPTMGR->GetImage(222, &imgBaseInfo, PFT_HARDPATH);
			VECTOR2 vScale = { 1.0f, 1.0f };
			VECTOR2 pos = { m_absPos.x, m_absPos.y };
			imgBaseInfo.RenderSprite(&vScale, NULL, 0.0f, &pos, 0xffffffff);
		}
	}
}




