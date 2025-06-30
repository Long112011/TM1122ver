// BuyItem.cpp: implementation of the CBuyItem class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BuyItem.h"
#include "ItemManager.h"
#include "./Interface/cFont.h"
#include "ChatManager.h"

#include "interface/cScriptManager.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBuyItem::CBuyItem()
{
	m_ItemIdx = 0;
	m_nVolume = 0;
	m_nMoney = 0;
	m_dwParam = 0;
	m_nGold=0;

	m_CurScrollImage = 0;

	for(int i=0;i<22;i++)
	{
		SCRIPTMGR->GetImage(108+i, &m_ScrollImg[i], PFT_JACKPATH);
	}
	m_GradeAlexX = 0;
}

CBuyItem::~CBuyItem()
{

}
//CoNaN  2017-09-16  元宝价格设置
void CBuyItem::InitItem(ITEMBASE ItemInfo, WORD Volume, DWORD Money,DWORD Gold)
{
	m_ItemIdx = ItemInfo.wIconIdx;
	m_nVolume = Volume;
	m_nMoney = Money;
	m_nGold  = Gold;   //CoNaN  2017-09-16  元宝价格设置
	m_dwParam = ItemInfo.ItemParam;

	WORD ItemKind = GetItemKind(m_ItemIdx);

	m_bVolume = !(( ItemKind > eEQUIP_ITEM) && ( ItemKind < eEXTRA_ITEM) && !ITEMMGR->IsDupItem(m_ItemIdx));
			
	cImage tmpImage;
	Init( 0, 0, DEFAULT_ICONSIZE, DEFAULT_ICONSIZE,
		ITEMMGR->GetIconImage( ItemInfo.wIconIdx, &tmpImage ) );
	
	SetIconType( eIconType_LinkedItem );
	SetData( ItemInfo.wIconIdx );
	SetGrow( ItemInfo.ItemGrow );  //CoNaN 初始化设置成长数据! 2016-08-20
	SetGradeAlexX(ItemInfo.ItemGradeAlexX);
	ITEMMGR->SetToolTipIcon(this);
}

void CBuyItem::Render()
{
	if( m_bActive == FALSE ) return;

	ITEM_INFO * pInfo = ITEMMGR->GetItemInfo((WORD)m_dwData );

	if( m_dwParam & ITEM_PARAM_SEAL )
	{
		cDialog::Render();
		cImage          m_LockImageEx;
		
		SCRIPTMGR->GetImage( 22, &m_LockImageEx, PFT_JACKPATH );
		VECTOR2 vScale = { 1.0f, 1.0f };
		VECTOR2 pos = { m_absPos.x+7, m_absPos.y+15};
		m_LockImageEx.RenderSprite( &vScale, NULL, 0.0f, &pos,0xffffffff);
		//RECT rect={(LONG)m_absPos.x+9, (LONG)m_absPos.y+15, 1,1};
		//CFONT_OBJ->RenderFont(0,CHATMGR->GetChatMsg(773),strlen(CHATMGR->GetChatMsg(773)),&rect,RGBA_MAKE(250,250,128,255));
	}
	else
		cIcon::Render();
/*#ifdef _LIKEPLAYPARK
	if( ITEMMGR->IsDupItem( (WORD)m_dwData ) )
	{
		char Temp[128]={0,};
		wsprintf(Temp,"%4d", m_nVolume);
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
#endif*/

	if(m_bVolume && m_nVolume > 0)
	{
		static char nums[3];
		wsprintf(nums,"%4d", m_nVolume);
		RECT rect={(LONG)m_absPos.x+15, (LONG)m_absPos.y+29, 1,1};
		CFONT_OBJ->RenderFont(0,nums,strlen(nums),&rect,RGBA_MERGE(m_dwImageRGB, m_alpha * m_dwOptionAlpha / 100));		// color hard coding : taiyo 
	}

	if(pInfo->ItemKind & eEQUIP_ITEM)
	{
		VECTOR2 vScale = { 1.0f, 1.0f };
		//VECTOR2 pos = { m_absPos.x, m_absPos.y };
		/*if(GetRareness()!=0)//GREEN ITEM ICON BY JACK
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
		}*/
#ifdef _LIKEPLAYPARK
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

			CFONT_OBJ->RenderNoticeMsg(14,Temp,strlen(Temp),&rect,RGBA_MERGE(RGB_HALF(248 ,248, 255),255),RGBA_MAKE(100,100,100,100));
		}
		if(pInfo->ItemGrade>=10&&pInfo->ItemGrade<=19)//ICON LEVEL GRADE
		{

			cImage          m_LockImageEx;
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
	}
	
}
