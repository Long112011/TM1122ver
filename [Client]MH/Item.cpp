#include "stdafx.h"
#include "Item.h"
#include "ItemManager.h"
#include "ObjectManager.h"
#include "QuickItem.h"
#include "GameIn.h"
#include "./Interface/cFont.h"
#include "QuickDialog.h"
#include "ChatManager.h"
#include "PetManager.h"
#include "TitanManager.h"
#include "interface/cScriptManager.h"
#include "Interface/cIcon.h"
CItem::CItem(ITEMBASE* pInfo)
{
	m_type = WT_ITEM;
	m_bAdded = FALSE;
	memcpy(&m_ItemBaseInfo,pInfo,sizeof(ITEMBASE));
	m_pLinkItem	= NULL;
	m_LinkPos	= 0;
	m_Param = 0;	
	SCRIPTMGR->GetImage( 127, &m_DelayImage  );
	m_fgColor	= RGB_HALF(250,250,0);
	m_fgColor2	= RGB_HALF(255,255,255);
//	mData[32] = 0;


	m_CurScrollImage = 0;

	for(int i=0;i<22;i++)
	{
		SCRIPTMGR->GetImage(108+i, &m_ScrollImg[i], PFT_JACKPATH);
	}
	//SCRIPTMGR->GetImage(95, &m_ScrollImg[0], PFT_HARDPATH);
	//SCRIPTMGR->GetImage(96, &m_ScrollImg[1], PFT_HARDPATH);
	//SCRIPTMGR->GetImage(97, &m_ScrollImg[2], PFT_HARDPATH);	
}
CItem::~CItem()
{
	//ZeroMemory(mData,sizeof(mData));
}
eITEM_KINDBIT CItem::GetItemKind()
{
	ITEM_INFO * pInfo = ITEMMGR->GetItemInfo(m_ItemBaseInfo.wIconIdx);
	ASSERT(pInfo);	
	if(pInfo)
		return (eITEM_KINDBIT)pInfo->ItemKind;
	return (eITEM_KINDBIT)0;
}
const ITEM_INFO* CItem::GetItemInfo()
{
	ITEM_INFO * pInfo = ITEMMGR->GetItemInfo(m_ItemBaseInfo.wIconIdx);
	ASSERT(pInfo);
	if(pInfo)
	ASSERT(pInfo->ItemIdx == m_ItemBaseInfo.wIconIdx);
	return pInfo;
}
void CItem::SetItemBaseInfo( ITEMBASE * itemInfo )
{
	memcpy( &m_ItemBaseInfo, itemInfo, sizeof( ITEMBASE ) );
}
void CItem::Render()
{
	if( !m_bActive )		return;	
	ITEM_INFO * pInfo = ITEMMGR->GetItemInfo(m_ItemBaseInfo.wIconIdx);
	SHOPITEMBASE* pInfo2 = ITEMMGR->GetUsedItemInfo(m_ItemBaseInfo.wIconIdx);
	cDialog::Render();
	if( m_ItemBaseInfo.ItemParam & ITEM_PARAM_SEAL && m_ItemBaseInfo.Position>=239 )
	{		
		SCRIPTMGR->GetImage( 22, &m_JackIcon, PFT_JACKPATH );
		VECTOR2 vScale = { 1.0f, 1.0f };
		VECTOR2 pos = { m_absPos.x+7, m_absPos.y+15};
		m_JackIcon.RenderSprite( &vScale, NULL, 0.0f, &pos,0xffffffff);


		if(pInfo->ItemKind == eSHOP_ITEM_CHARM )
		{
			SCRIPTMGR->GetImage( 16, &m_JackIcon, PFT_JACKPATH );
			VECTOR2 vScale = { 1.0f, 1.0f };
			VECTOR2 pos = { m_absPos.x+27, m_absPos.y};
			m_JackIcon.RenderSprite( &vScale, NULL, 0.0f, &pos,0xffffffff);
		}
		if(m_ItemBaseInfo.ItemStatic == ITEM_STATIC_PD)		
		{
			SCRIPTMGR->GetImage( 17, &m_JackIcon, PFT_JACKPATH );
			VECTOR2 vScale = { 1.0f, 1.0f };
			VECTOR2 pos = { m_absPos.x+25, m_absPos.y-1};
			m_JackIcon.RenderSprite( &vScale, NULL, 0.0f, &pos,0xffffffff);
		}
		if(m_ItemBaseInfo.ItemStatic == ITEM_STATIC_GD)		
		{
			SCRIPTMGR->GetImage( 24, &m_JackIcon, PFT_JACKPATH );
			VECTOR2 vScale = { 1.0f, 1.0f };
			VECTOR2 pos = { m_absPos.x+25, m_absPos.y-1};
			m_JackIcon.RenderSprite( &vScale, NULL, 0.0f, &pos,0xffffffff);
		}
		if(m_ItemBaseInfo.ItemStatic == ITEM_STATIC_NEWBIE)
		{
			RECT rect={(LONG)m_absPos.x+4, (LONG)m_absPos.y+26, 1,1};
			CFONT_OBJ->RenderNoticeMsg(0,CHATMGR->GetChatMsg(1794),strlen(CHATMGR->GetChatMsg(1794)),&rect,RGBA_MAKE(180,200,255,255),RGBA_MAKE(10,10,10,100));
		}
	}
	else if( m_ItemBaseInfo.ItemParam & ITEM_PARAM_SEAL && m_ItemBaseInfo.Position<=239 )
	{
		cDialog::Render();
		if(m_ItemBaseInfo.ItemStatic == ITEM_STATIC_PD)		
		{
			SCRIPTMGR->GetImage( 17, &m_JackIcon, PFT_JACKPATH );
			VECTOR2 vScale = { 1.0f, 1.0f };
			VECTOR2 pos = { m_absPos.x+25, m_absPos.y-1};
			m_JackIcon.RenderSprite( &vScale, NULL, 0.0f, &pos,0xffffffff);
		}
	}
	else if(m_ItemBaseInfo.ItemStatic == ITEM_STATIC_NEWBIE)
	{
		cDialog::Render();		
		RECT rect={(LONG)m_absPos.x+4, (LONG)m_absPos.y+26, 1,1};
		CFONT_OBJ->RenderNoticeMsg(0,CHATMGR->GetChatMsg(1794),strlen(CHATMGR->GetChatMsg(1794)),&rect,RGBA_MAKE(180,200,255,255),RGBA_MAKE(10,10,10,100));
	}
	else if(m_ItemBaseInfo.ItemStatic == ITEM_STATIC_GD)
	{
		cDialog::Render();		
		SCRIPTMGR->GetImage( 24, &m_JackIcon, PFT_JACKPATH );
		VECTOR2 vScale = { 1.0f, 1.0f };
		VECTOR2 pos = { m_absPos.x+25, m_absPos.y-1};
		m_JackIcon.RenderSprite( &vScale, NULL, 0.0f, &pos,0xffffffff);
	}
	/*else if( m_ItemBaseInfo.ItemParam & ITEM_PARAM_TITAN_REGIST )
	{
		cDialog::Render();
		DWORD dwRemainTime = 0;
		float delayRate = HERO->GetDelayGroup()->CheckDelay(CDelayGroup::eDK_Item,m_ItemBaseInfo.wIconIdx, &dwRemainTime );
		if( delayRate > 0.0f )
		{
			
			SCRIPTMGR->GetImage( 23, &m_JackIcon, PFT_JACKPATH );
			VECTOR2 vScale = { 1.0f, 1.0f };
			VECTOR2 pos = { m_absPos.x-1, m_absPos.y-1};
			m_JackIcon.RenderSprite( &vScale, NULL, 0.0f, &pos,0xffffffff);
			static char mData[MAX_ITEMNAME_LENGTH+1];
			wsprintf(mData,"%2d",- delayRate);
			RECT rect={(LONG)m_absPos.x+25, (LONG)m_absPos.y+29, 1,1};
			m_DelayImage.RenderSprite( &vScale, NULL, 0.0f, &pos, 0xff000000  );
		}
	}*/
	else if( PETMGR->IsCurPetSummonItem(m_ItemBaseInfo.dwDBIdx) )
	{
	//	cDialog::Render();
	//	m_LockImage.RenderSprite( NULL, NULL, 0, &m_absPos, 
	//	RGBA_MERGE(0xffffff, m_alpha * m_dwOptionAlpha / 100));
		RECT rect={(LONG)m_absPos.x+4, (LONG)m_absPos.y+18, 1,1};
		CFONT_OBJ->RenderNoticeMsg(0,CHATMGR->GetChatMsg(1246),strlen(CHATMGR->GetChatMsg(1246)),&rect,RGBA_MERGE( RGB_HALF(250 ,250 ,100), 255 ),RGBA_MERGE( RGB_HALF(10 ,10 ,10), 255 ));
	}
	else if(pInfo->ItemKind == eSHOP_ITEM_CHARM )
	{
		cDialog::Render();
		RECT rect2={(LONG)m_absPos.x+9, (LONG)m_absPos.y+15, 1,1};
		CFONT_OBJ->RenderFont(0,CHATMGR->GetChatMsg(773),strlen(CHATMGR->GetChatMsg(773)),&rect2,RGBA_MAKE(250,250,0,255));
		cDialog::Render();
		SCRIPTMGR->GetImage( 16, &m_JackIcon, PFT_JACKPATH );
		VECTOR2 vScale = { 1.0f, 1.0f };
		VECTOR2 pos = { m_absPos.x+27, m_absPos.y};
		m_JackIcon.RenderSprite( &vScale, NULL, 0.0f, &pos,0xffffffff);
	}
	else
		cIcon::Render();
	if( m_Param == 1 )
	{
		m_LockImage.RenderSprite( NULL, NULL, 0, &m_absPos, 
		RGBA_MERGE(0xffffff, m_alpha * m_dwOptionAlpha / 100));
	}
	int nLen = strlen( pInfo->ItemName );
	if( *(pInfo->ItemName + nLen - 3) == '.' && pInfo->ItemKind == eCHANGE_ITEM )
	{
		cDialog::Render();
		static char mData[MAX_ITEMNAME_LENGTH+1];
		wsprintf(mData,"Lv.%c",pInfo->ItemName[(nLen - 2)]);		
		RECT rect={(LONG)m_absPos.x, (LONG)m_absPos.y, 1,1};
		CFONT_OBJ->RenderNoticeMsg(14,mData,strlen(mData),&rect,RGBA_MERGE(m_fgColor2,255),RGBA_MAKE(100,100,100,100));
	}
	if( *(pInfo->ItemName + nLen - 4) == '.' && pInfo->ItemKind == eCHANGE_ITEM )
	{
		cDialog::Render();
		static char mData[MAX_ITEMNAME_LENGTH+1];
		wsprintf(mData,"Lv.%c%c",pInfo->ItemName[(nLen - 3)],pInfo->ItemName[(nLen - 2)]);
		RECT rect={(LONG)m_absPos.x-1, (LONG)m_absPos.y, 1,1};
		CFONT_OBJ->RenderNoticeMsg(14,mData,strlen(mData),&rect,RGBA_MERGE(m_fgColor2,255),RGBA_MAKE(100,100,100,100));
	}
	if( PETMGR->IsCurPetSummonItem(m_ItemBaseInfo.dwDBIdx) )
	{
		//cDialog::Render();
		//m_LockImage.RenderSprite( NULL, NULL, 0, &m_absPos, 
		//RGBA_MERGE(0xffffff, m_alpha * m_dwOptionAlpha / 100));
		RECT rect={(LONG)m_absPos.x+4, (LONG)m_absPos.y+18, 1,1};
		CFONT_OBJ->RenderNoticeMsg(0,CHATMGR->GetChatMsg(1246),strlen(CHATMGR->GetChatMsg(1246)),&rect,RGBA_MERGE( RGB_HALF(250 ,250 ,100), 255 ),RGBA_MERGE( RGB_HALF(10 ,10 ,10), 255 ));
	}
	if(GetItemKind() & eEQUIP_ITEM)
	{
		VECTOR2 vScale = { 1.0f, 1.0f };
		VECTOR2 pos = { m_absPos.x, m_absPos.y };
		if(m_ItemBaseInfo.RareIdx!=0)
		{
			SCRIPTMGR->GetImage( 20, &m_JackIcon, PFT_JACKPATH );
			VECTOR2 pos = { m_absPos.x, m_absPos.y+27};
			m_JackIcon.RenderSprite( &vScale, NULL, 0.0f, &pos,0xffffffff);
		}
		if(m_ItemBaseInfo.Durability !=0 )
		{	
			SCRIPTMGR->GetImage( 21, &m_JackIcon, PFT_JACKPATH );
			VECTOR2 pos = { m_absPos.x+27, m_absPos.y+26};
			m_JackIcon.RenderSprite( &vScale, NULL, 0.0f, &pos,0xffffffff);
		}
		if(m_ItemBaseInfo.StoneIdx!=0)
		{	
			SCRIPTMGR->GetImage( 19, &m_JackIcon, PFT_JACKPATH );
			VECTOR2 pos = { m_absPos.x+27, m_absPos.y+1};
			m_JackIcon.RenderSprite( &vScale, NULL, 0.0f, &pos,0xffffffff);
		}
		if (m_ItemBaseInfo.Grade30 != 0)
		{
			const int OFFSET_Y = 14;      // 往下移一点，避开 ItemGrade
			const int FONT_WIDTH = 40;    // 宽度估计
			const int FONT_HEIGHT = 16;   // 高度估计

			char temp[128];
			sprintf(temp, "+%d", m_ItemBaseInfo.Grade30);

			RECT rectTmp;
			rectTmp.left = (LONG)(m_absPos.x + 1);
			rectTmp.top = (LONG)(m_absPos.y + 1 + OFFSET_Y);
			rectTmp.right = rectTmp.left + FONT_WIDTH;
			rectTmp.bottom = rectTmp.top + FONT_HEIGHT;

			CFONT_OBJ->RenderFont(21, temp, strlen(temp), &rectTmp, RGBA_MERGE(RGB_HALF(124, 252, 0), 255));
		}

#ifdef _LIKEPLAYPARK
		if(pInfo->ItemGrade>=1&&pInfo->ItemGrade<=9)
		{
			static char mData[MAX_ITEMNAME_LENGTH+1];
			wsprintf(mData,"+%d",pInfo->ItemGrade);
			RECT rect={(LONG)m_absPos.x+1, (LONG)m_absPos.y+1, 1,1};
			CFONT_OBJ->RenderNoticeMsg(21,mData,strlen(mData),&rect,RGBA_MERGE(RGB_HALF(248 ,248, 255),255),RGBA_MAKE(100,100,100,100));
		}
		if(pInfo->ItemGrade>=11&&pInfo->ItemGrade<=19)
		{
			static char mData[MAX_ITEMNAME_LENGTH+1];
			wsprintf(mData,"+%d",pInfo->ItemGrade-10);
			RECT rect={(LONG)m_absPos.x+1, (LONG)m_absPos.y+1, 1,1};
			CFONT_OBJ->RenderNoticeMsg(21,mData,strlen(mData),&rect,RGBA_MERGE(TTTC_ITEM_PINK,255),RGBA_MAKE(100,100,100,100));
		}
		if(pInfo->ItemGrade>=10&&pInfo->ItemGrade<=19)
		{
			//SCRIPTMGR->GetImage( 19, &m_JackIcon, PFT_JACKPATH );
			//VECTOR2 pos = { m_absPos.x+27, m_absPos.y+1};
			//m_JackIcon.RenderSprite( &vScale, NULL, 0.0f, &pos,0xffffffff);

			SCRIPTMGR->GetImage( 130, &m_JackIcon, PFT_JACKPATH );
			m_JackIcon.RenderSprite( &vScale, NULL, 0.0f, &pos,0xffffffff);

			static int TICK_PER_IMAGE = 30;
			m_CurScrollImage+=gTickTime;
			int curImage = m_CurScrollImage % (TICK_PER_IMAGE*21);
			curImage /= TICK_PER_IMAGE;
			m_ScrollImg[curImage].RenderSprite(NULL,NULL,0,&pos,0xffffffff);

			SCRIPTMGR->GetImage( 19, &m_JackIcon, PFT_JACKPATH );
			VECTOR2 pos = { m_absPos.x+27, m_absPos.y+1};
			m_JackIcon.RenderSprite( &vScale, NULL, 0.0f, &pos,0xffffffff);
		}
#else
		if(pInfo->ItemGrade>=1&&pInfo->ItemGrade<=10)
		{
			static char mData[MAX_ITEMNAME_LENGTH+1];
			wsprintf(mData,"+%d",pInfo->ItemGrade);
			RECT rect={(LONG)m_absPos.x+1, (LONG)m_absPos.y+1, 1,1};
			CFONT_OBJ->RenderNoticeMsg(14,mData,strlen(mData),&rect,RGBA_MERGE(RGB_HALF(248 ,248, 255),255),RGBA_MAKE(100,100,100,100));
		}
#endif
	}
	if (GetItemKind() == eYOUNGYAK_ITEM)  //[吃藥回氣渲染][2017/11/27]
	{
		YOUNGYAK_ITEM_DELAY_TYPE * Tmp = NULL;
		Tmp = GAMERESRCMNGR->GetYoungYakDelay()->GetData(m_ItemBaseInfo.wIconIdx);
		if (Tmp)
		{
			DWORD dwRemainTime = 0;
			float delayRate = HERO->GetDelayGroup()->CheckYoungYakPotionDelay(Tmp->wtype, &dwRemainTime);
			if (delayRate > 0.0f)
			{
				static char nums[5];
				wsprintf(nums, "%4d", dwRemainTime / 1000 + 1);
				RECT rect = { (LONG)m_absPos.x + 13, (LONG)m_absPos.y + 2, 1, 1 };

				VECTOR2 vScale = { 1.0f, delayRate };
				VECTOR2 pos = { m_absPos.x, m_absPos.y + m_height*(1 - delayRate) };
				m_DelayImage.RenderSprite(&vScale, NULL, 0.0f, &pos, 0xff000000);
				CFONT_OBJ->RenderNoticeMsg(14, nums, strlen(nums), &rect, RGBA_MERGE(RGB_HALF(250, 250, 250), 210), RGBA_MERGE(RGB_HALF(185, 185, 185), 255));
			}
			/*if (delayRate > 0.0f)
			{
				VECTOR2 vScale = { 1.0f, delayRate };
				VECTOR2 pos = { m_absPos.x, m_absPos.y + m_height*(1 - delayRate) };
				m_DelayImage.RenderSprite(&vScale, NULL, 0.0f, &pos, 0xff000000);
			}
			if (dwRemainTime)  //Show Popup Using PotionItem  2020-09-03
			{
				static char nums[4];
				wsprintf(nums, "%4d\n", dwRemainTime / 1000 + 1);
				RECT rect = { (LONG)m_absPos.x + 12, (LONG)m_absPos.y + 4, 1, 1 };
				//RECT rect={(LONG)m_absPos.x, (LONG)m_absPos.y, 1,1};
				CFONT_OBJ->RenderFont(12, nums, strlen(nums), &rect, RGBA_MERGE(RGB_HALF(255, 255, 255), 255));
			}*/
		}
	}
	if(m_ItemBaseInfo.ItemStatic == ITEM_STATIC_LUCK)
	{
		SCRIPTMGR->GetImage( 18, &m_JackIcon, PFT_JACKPATH );
		VECTOR2 vScale = { 0.3f, 0.3f };
		VECTOR2 pos = { m_absPos.x+15, m_absPos.y+15};
		m_JackIcon.RenderSprite( &vScale, NULL, 0.0f, &pos,0xffffffff);
	}
	if(m_ItemBaseInfo.ItemStatic == ITEM_STATIC_PD)
	{
		SCRIPTMGR->GetImage( 17, &m_JackIcon, PFT_JACKPATH );
		VECTOR2 vScale = { 1.0f, 1.0f };
		VECTOR2 pos = { m_absPos.x+25, m_absPos.y-1};
		m_JackIcon.RenderSprite( &vScale, NULL, 0.0f, &pos,0xffffffff);
	}
	if(m_ItemBaseInfo.ItemStatic == ITEM_STATIC_GD)
	{
		SCRIPTMGR->GetImage( 24, &m_JackIcon, PFT_JACKPATH );
		VECTOR2 vScale = { 1.0f, 1.0f };
		VECTOR2 pos = { m_absPos.x+25, m_absPos.y-1};
		m_JackIcon.RenderSprite( &vScale, NULL, 0.0f, &pos,0xffffffff);
	}
	if(ITEMMGR->IsDupItem(m_ItemBaseInfo.wIconIdx) && m_ItemBaseInfo.Durability >= 1 )
	{
		static char mData[MAX_ITEMNAME_LENGTH+1];
		wsprintf(mData,"%4d", m_ItemBaseInfo.Durability);
		RECT rect={(LONG)m_absPos.x+15, (LONG)m_absPos.y+29, 1,1};
		CFONT_OBJ->RenderFont(0,mData,strlen(mData),&rect,RGBA_MERGE(m_dwImageRGB, m_alpha * m_dwOptionAlpha / 100 ));
	}
	if(ITEMMGR->GetUsedItemInfo(m_ItemBaseInfo.wIconIdx) && (pInfo->ItemKind == eSHOP_ITEM_CHARM||pInfo->ItemKind ==eSHOP_ITEM_HERB))
	{
		SCRIPTMGR->GetImage( 28, &m_JackIcon, PFT_JACKPATH );
		VECTOR2 vScale = { 1.0f, 1.0f };
		VECTOR2 pos = { m_absPos.x-1, m_absPos.y};
		m_JackIcon.RenderSprite( &vScale, NULL, 0.0f, &pos,0xffffffff);
	}
	if(pInfo->ItemKind ==eCHANGE_ITEM )
	{
		if(pInfo->ItemGrade>=1)
		{
			static char mData[MAX_ITEMNAME_LENGTH+1];
			wsprintf(mData,"Lv.%4d", m_ItemBaseInfo.Durability);
			RECT rect={(LONG)m_absPos.x+15, (LONG)m_absPos.y+29, 1,1};
			CFONT_OBJ->RenderFont(0,mData,strlen(mData),&rect,RGBA_MERGE(m_dwImageRGB, m_alpha * m_dwOptionAlpha / 100 ));
		}
	}
#ifdef _ITEMFUSE
	if(pInfo->ItemKind ==eEXTRA_ITEM_FUSE)
	{
		VECTOR2 vScale = { 1.0f, 1.0f };
		VECTOR2 pos = { m_absPos.x, m_absPos.y };
		if(pInfo->ItemGrade>=1&&pInfo->ItemGrade<=9)
		{
			static char mData[MAX_ITEMNAME_LENGTH+1];
			wsprintf(mData,"+%d",pInfo->ItemGrade);
			RECT rect={(LONG)m_absPos.x+1, (LONG)m_absPos.y+1, 1,1};
			CFONT_OBJ->RenderNoticeMsg(21,mData,strlen(mData),&rect,RGBA_MERGE(TTTC_BRIGHT_YELLOW,255),RGBA_MAKE(100,100,100,100));
		}
		if(pInfo->ItemGrade>=11&&pInfo->ItemGrade<=19)
		{
			static char mData[MAX_ITEMNAME_LENGTH+1];
			wsprintf(mData,"+%d",pInfo->ItemGrade-10);
			RECT rect={(LONG)m_absPos.x+1, (LONG)m_absPos.y+1, 1,1};
			CFONT_OBJ->RenderNoticeMsg(21,mData,strlen(mData),&rect,RGBA_MERGE(TTTC_ITEM_PINK,255),RGBA_MAKE(100,100,100,100));
		}
		if(pInfo->ItemGrade>=10&&pInfo->ItemGrade<=19)
		{
			SCRIPTMGR->GetImage( 19, &m_JackIcon, PFT_JACKPATH );
			VECTOR2 pos = { m_absPos.x+27, m_absPos.y+1};
			m_JackIcon.RenderSprite( &vScale, NULL, 0.0f, &pos,0xffffffff);
		}

		static char mData[MAX_ITEMNAME_LENGTH+1];
		wsprintf(mData,"%4d", m_ItemBaseInfo.Durability);
		RECT rect={(LONG)m_absPos.x+15, (LONG)m_absPos.y+29, 1,1};
		CFONT_OBJ->RenderFont(0,mData,strlen(mData),&rect,RGBA_MERGE(m_dwImageRGB, m_alpha * m_dwOptionAlpha / 100 ));

		if (m_ItemBaseInfo.Grade30 != 0)
		{
			VECTOR2 vScale = { 1.0f, 1.0f };
			VECTOR2 pos = { m_absPos.x + 13, m_absPos.y + 25 };
			//m_OptionFlgImage.RenderSprite( &vScale, NULL, 0.0f, &pos,0xffffffff);
			RECT rectTmp;
			rectTmp.bottom = 1;
			rectTmp.right = 1;
			rectTmp.left = m_absPos.x;
			rectTmp.top = m_absPos.y;
			char temp[128];
			sprintf(temp, "+%d", m_ItemBaseInfo.Grade30);
			//CFONT_OBJ->RenderFont(m_wFontIdx, "x", 128, &rectTmp, RGBA_MERGE(curLineNode->color, m_alpha * m_dwOptionAlpha / 100));
			CFONT_OBJ->RenderFont(21, temp, strlen(temp), &rectTmp, RGBA_MERGE(RGB_HALF(124, 252, 0), 255));
		}
	}
#endif
}

void CItem::SetLock( BOOL bLock )
{
	cIcon::SetLock( bLock );
	SetMovable( !bLock );
}
void CItem::SetLockHold(BOOL bLockHold)
{
	cIcon::SetLockHold(bLockHold);
	SetMovable(!bLockHold);
}
BOOL CItem::CanMoveToDialog( cDialog* ptargetdlg )
{
	if( GetItemKind() == eQUEST_ITEM_EQUIP )
	{
		int type = ptargetdlg->GetType();
		switch( type )
		{
		case WT_STREETSTALLDIALOG:
		case WT_STREETBUYSTALLDIALOG:
		case WT_DEALDIALOG:
		case WT_MUGONGSURYUNDIALOG:
		case WT_GUILDWAREHOUSEDLG:
		case WT_QUESTTOTALDIALOG:
		case WT_ITEMSHOPDIALOG:
			return FALSE;
		}
	}
	return TRUE;
}
BOOL CItem::CanDelete()
{
	if( GetItemKind() == eQUEST_ITEM_EQUIP )
		return FALSE;
	return TRUE;
}
