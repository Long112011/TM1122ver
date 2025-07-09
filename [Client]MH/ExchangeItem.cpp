#include "stdafx.h"
#include "ExchangeItem.h"
#include "./interface/cWindowManager.h"
#include "./interface/cFont.h"
#include "ItemManager.h"
#include "ChatManager.h"
#include "interface/cScriptManager.h"
CExchangeItem::CExchangeItem()
{
	m_pLinkItem		= NULL;
	m_LinkPos		= -1;
	m_Durability	= 1;
	m_type			= WT_EXCHANGEITEM;
	m_dwParam		= 0;
	m_Grow			= 0;
	m_CurScrollImage = 0;
	for(int i=0;i<22;i++)
	{
		SCRIPTMGR->GetImage(108+i, &m_ScrollImg[i], PFT_JACKPATH);
	}
	m_Grade = 0;
}
CExchangeItem::~CExchangeItem()
{
}
void CExchangeItem::InitItem( CItem* pItem, POSTYPE Pos, LONG id )
{
	cImage tmpImage;
	Init( 0, 0, DEFAULT_ICONSIZE, DEFAULT_ICONSIZE, ITEMMGR->GetIconImage( pItem->GetItemIdx(), &tmpImage ), id );		 
	m_pLinkItem = pItem;
	m_Position	= Pos;
	m_LinkPos	= pItem->GetPosition();
	m_dwParam = pItem->GetItemParam();
	SetIconType( eIconType_LinkedItem );
	SetData( pItem->GetItemIdx() );
	if( ITEMMGR->IsPetSummonItem( pItem->GetItemIdx() ) )
	{
		ITEMMGR->SetToolTipIcon( this, NULL, NULL, pItem->GetDBIdx() );
	}
	else if( ITEMMGR->IsOptionItem( pItem->GetItemIdx(), pItem->GetDurability() ) )
	{
		ITEMMGR->SetToolTipIcon( this, ITEMMGR->GetItemOption( pItem->GetDurability() ),
			ITEMMGR->GetItemRareOption( pItem->GetRareness() ),0,ITEMMGR->GetItemStoneOption(pItem->GetStoneIdx()),0,0,0,0, pItem->GetGrade());//+30 зЂвт
	}
	else
		ITEMMGR->SetToolTipIcon( this, NULL, ITEMMGR->GetItemRareOption( pItem->GetRareness() ),0,ITEMMGR->GetItemStoneOption(pItem->GetStoneIdx()),0,0,0,0, pItem->GetGrade());
}
void CExchangeItem::SetLock( BOOL bLock )
{
	cIcon::SetLock( bLock );
	SetMovable( !bLock );
}
void CExchangeItem::SetLockHold(BOOL bLockHold)
{
	cIcon::SetLockHold(bLockHold);
	SetMovable(!bLockHold);
}
void CExchangeItem::Render()
{
	if( m_bActive == FALSE ) return;
	ITEM_INFO * pInfo = ITEMMGR->GetItemInfo((WORD)m_dwData );
	if( m_dwParam & ITEM_PARAM_SEAL )
	{
		cDialog::Render();
		if(pInfo->ItemKind & !eEQUIP_ITEM)
		{			
			SCRIPTMGR->GetImage( 22, &m_LockImageEx, PFT_JACKPATH );
			VECTOR2 vScale = { 1.0f, 1.0f };
			VECTOR2 pos = { m_absPos.x+7, m_absPos.y+15};
			m_LockImageEx.RenderSprite( &vScale, NULL, 0.0f, &pos,0xffffffff);
		}
	}
	else
		cIcon::Render();
#ifdef _LIKEPLAYPARK
	if( ITEMMGR->IsDupItem( (WORD)m_dwData ) )
	{
		char Temp[128]={0,};
		wsprintf(Temp,"%4d", m_Durability);
		RECT rect={(LONG)m_absPos.x+15, (LONG)m_absPos.y+29, 1,1};
		CFONT_OBJ->RenderFont(0,Temp,strlen(Temp),&rect,RGBA_MERGE(m_dwImageRGB, m_alpha * m_dwOptionAlpha / 100 ));
	}
#else
	if( ITEMMGR->IsDupItem( (WORD)m_dwData ) )
	{
		static char nums[3];
		wsprintf(nums,"%2d", m_Durability);
		RECT rect={(LONG)m_absPos.x+27, (LONG)m_absPos.y+29, 1,1};
		CFONT_OBJ->RenderFont(0,nums,strlen(nums),&rect,RGBA_MERGE(m_dwImageRGB, m_alpha * m_dwOptionAlpha / 100));		// color hard coding : taiyo 
	}
#endif
	if(pInfo->ItemKind & eEQUIP_ITEM)
	{
		VECTOR2 vScale = { 1.0f, 1.0f };
		if(GetRareness()!=0)//GREEN ITEM ICON BY JACK
		{
			SCRIPTMGR->GetImage( 20, &m_LockImageEx, PFT_JACKPATH );
			VECTOR2 pos = { m_absPos.x, m_absPos.y+27};
			m_LockImageEx.RenderSprite( &vScale, NULL, 0.0f, &pos,0xffffffff);
		}
		if(GetDurability()!=0 )//ITEM BLUE ICON PU BY JACK
		{	
			SCRIPTMGR->GetImage( 21, &m_LockImageEx, PFT_JACKPATH );
			VECTOR2 pos = { m_absPos.x+27, m_absPos.y+26};
			m_LockImageEx.RenderSprite( &vScale, NULL, 0.0f, &pos,0xffffffff);
		}
		if(GetStoneIdx()!=0)//QQ ITEM ICON BY JACK	
		{	
			SCRIPTMGR->GetImage( 19, &m_LockImageEx, PFT_JACKPATH );
			VECTOR2 pos = { m_absPos.x+27, m_absPos.y+1};
			m_LockImageEx.RenderSprite( &vScale, NULL, 0.0f, &pos,0xffffffff);
		}
#ifdef _LIKEPLAYPARK
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
			CFONT_OBJ->RenderNoticeMsg(21,Temp,strlen(Temp),&rect,RGBA_MERGE(RGB_HALF(248 ,248, 255),255),RGBA_MAKE(100,100,100,100));
		}
		if(pInfo->ItemGrade>=10&&pInfo->ItemGrade<=19)//ICON LEVEL GRADE
		{
			VECTOR2 pos = { m_absPos.x, m_absPos.y };
			SCRIPTMGR->GetImage( 130, &m_LockImageEx, PFT_JACKPATH );
			m_LockImageEx.RenderSprite( &vScale, NULL, 0.0f, &pos,0xffffffff);
			static int TICK_PER_IMAGE = 30;
			m_CurScrollImage+=gTickTime;
			int curImage = m_CurScrollImage % (TICK_PER_IMAGE*20);
			curImage /= TICK_PER_IMAGE;
			m_ScrollImg[curImage].RenderSprite(NULL,NULL,0,&pos,0xffffffff);
			SCRIPTMGR->GetImage( 19, &m_LockImageEx, PFT_JACKPATH );
			VECTOR2 pos2 = { m_absPos.x+27, m_absPos.y+1};
			m_LockImageEx.RenderSprite( &vScale, NULL, 0.0f, &pos2,0xffffffff);
		}
#else
		if(pInfo->ItemGrade>0)//ICON LEVEL GRADE
		{
			char buf1[12];
			sprintf(buf1,"+%d",pInfo->ItemGrade);
			RECT rect={(LONG)m_absPos.x, (LONG)m_absPos.y, 1,1};
			CFONT_OBJ->RenderNoticeMsg(14,buf1,strlen(buf1),&rect,RGBA_MERGE(RGB_HALF(248 ,248, 255),255),RGBA_MAKE(70,70,70,255));
			/*CFONT_OBJ->RenderFont(7,buf1,strlen(buf1),&rect,RGBA_MAKE(70,70,70,255));
			rect.left -= 1;
			rect.top -= 1;
			CFONT_OBJ->RenderFont(7,buf1,strlen(buf1),&rect,RGBA_MERGE(m_fgColor,255));*/
		}
#endif
		if (pInfo->ItemKind >= eEQUIP_ITEM && pInfo->ItemKind <= eEQUIP_ITEM_UNIQUE)
		{
			if (m_Grade > 0)
			{
				const int FONT_WIDTH = 40;
				const int FONT_HEIGHT = 16;

				char temp[128];
				sprintf(temp, "+%d", m_Grade);

				RECT rectTmp;
				rectTmp.left = (LONG)(m_absPos.x + 13);
				rectTmp.top = (LONG)(m_absPos.y + 25);
				rectTmp.right = rectTmp.left + FONT_WIDTH;
				rectTmp.bottom = rectTmp.top + FONT_HEIGHT;

				CFONT_OBJ->RenderFont(21, temp, strlen(temp), &rectTmp, RGBA_MERGE(RGB_HALF(124, 252, 0), 255));
			}
		}


	}
}
char* CExchangeItem::GetItemName()
{
	ITEM_INFO * pItemInfo = ITEMMGR->GetItemInfo((WORD)GetData());
	if(pItemInfo)
		return pItemInfo->ItemName;
	return NULL;
}