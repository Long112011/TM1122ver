#include "stdafx.h"
#include "cScriptManager.h"
#include "GoldShopItem.h"
#include "ItemManager.h"
#include "cFont.h"
#include "cImage.h"
#include "cButton.h"
#include "chatmanager.h"
#include "mhfile.h"
#define FONTOFFER 16
#define GARISX 70
#define GARISY 48
#define NEWPRICEX 100
#define NEWPRICEY 42
void BtnFunc(LONG lId, void * p, DWORD we)
{
   return;
}
CShopItem::CShopItem()
{	
	m_wItemIdx=0;
    m_GoldPrice=0;
    m_MallPrice=0;
	m_BuyPrice=0;
	m_OfferPrice=0;
    m_wItemType=0;//-500;//1
	m_Button=NULL;

	LoadCashShopIcon();
}
CShopItem::~CShopItem()
{
	m_wItemIdx=0;
    m_GoldPrice=0;
    m_MallPrice=0;
	m_BuyPrice=0;
	m_OfferPrice=0;
    m_wItemType=0;//-500;//2
    m_Button=NULL;
}
void CShopItem::InitBtn(LONG x,LONG y, WORD w,WORD h,cImage * BaseImg,cImage * OverImg,cImage * PressImg,cbFUNC Func,LONG lId,char * BtnText)
{
	m_Button = new cButton;
	m_Button->Init(x,y,w,h,BaseImg,OverImg,PressImg,Func,lId);
	m_Button->SetText(BtnText,0xffffffff);
	m_Button->SetFontIdx(0);
	m_Button->SetAlign(TXT_CENTER|TXT_MIDDLE);
	m_Button->SetActive(TRUE);
	Add(m_Button);
}
void CShopItem::LoadCashShopIcon()
{
	char buff[64]={0,};
	CMHFile file;
	if(file.Init("./Resource/Client/cash_shop_icon_decoration.bin","rb")==FALSE)
		return;
	while(1)
	{
		if(file.IsEOF())
			break;
		file.GetString(buff);
		START_STRING_COMPARE(buff)
		COMPARE("#ICON")
			for(int i= 0;i<3;i++)
			{
				sCashShopIcon.vIconPos[i].z = file.GetFloat();
				sCashShopIcon.vIconPos[i].x = file.GetFloat();
				sCashShopIcon.vIconPos[i].y = file.GetFloat();
				file.GetString();
			}
		END_COMPARE
	}
	file.Release();
}
void CShopItem::Render()
{
	if(!m_bActive) return;

//---------------------------------------------------------------------------------
	/*SHOPITEMBASE* pShopItem = NULL;	
	m_UsedItemList.SetPositionHead();
	ITEM_INFO* pInfo = NULL;	
	while( pShopItem = m_UsedItemList.GetData() )
	{
		pInfo = GetItemInfo( pShopItem->ItemBase.wIconIdx );
		if( pInfo &&( pInfo->SellPrice == eShopItemUseParam_Playtime) ||(MAP->IsMapKind(eInsDungeon)&&pInfo->SellPrice == eShopItemuseParam_FBItem))
		{
			if( NOTIFYMGR->IsEventNotifyUse() )
			{
				if( pInfo->ItemKind == eSHOP_ITEM_CHARM && pInfo->MeleeAttackMin && pShopItem->Remaintime )
				{
					if( NOTIFYMGR->IsApplyEvent( pInfo->MeleeAttackMin ) )
						continue;												
				}
			}
			if( pShopItem->Remaintime > gTickTime )
				pShopItem->Remaintime -= gTickTime;
			else
				pShopItem->Remaintime = 0;
		}
	}*/
//---------------------------------------------------------------------------------

	ITEM_INFO * pInfo = ITEMMGR->GetItemInfo(m_wItemIdx);
	SHOPITEMBASE* pInfo2 = ITEMMGR->GetUsedItemInfo(m_wItemIdx);
	cIcon::Render();
	if(pInfo2 && (pInfo->ItemKind == eSHOP_ITEM_CHARM||pInfo->ItemKind ==eSHOP_ITEM_HERB))
	{
		cImage          m_LockImageEx;
		SCRIPTMGR->GetImage( 28, &m_LockImageEx, PFT_JACKPATH );
		VECTOR2 vScale = { 1.0f, 1.0f };
		VECTOR2 pos = { m_absPos.x-1, m_absPos.y};
		m_LockImageEx.RenderSprite( &vScale, NULL, 0.0f, &pos,0xffffffff);
	}

#ifdef _ITEMFUSE
	if(pInfo && pInfo->ItemKind ==eEXTRA_ITEM_FUSE)
	{
		cImage          m_LockImageEx;
		VECTOR2 vScale = { 1.0f, 1.0f };
		VECTOR2 pos = { m_absPos.x, m_absPos.y };
		if(pInfo->ItemGrade>=1&&pInfo->ItemGrade<=9)
		{
			static char mData[MAX_ITEMNAME_LENGTH+1];
			wsprintf(mData,"+%d",pInfo->ItemGrade);
			RECT rect={(LONG)m_absPos.x+1, (LONG)m_absPos.y+1, 1,1};
			CFONT_OBJ->RenderNoticeMsg(14,mData,strlen(mData),&rect,RGBA_MERGE(TTTC_BRIGHT_YELLOW,255),RGBA_MAKE(100,100,100,100));
		}
		if(pInfo->ItemGrade>=11&&pInfo->ItemGrade<=19)
		{
			static char mData[MAX_ITEMNAME_LENGTH+1];
			wsprintf(mData,"+%d",pInfo->ItemGrade-10);
			RECT rect={(LONG)m_absPos.x+1, (LONG)m_absPos.y+1, 1,1};
			CFONT_OBJ->RenderNoticeMsg(14,mData,strlen(mData),&rect,RGBA_MERGE(TTTC_ITEM_PINK,255),RGBA_MAKE(100,100,100,100));
		}
		if(pInfo->ItemGrade>=10&&pInfo->ItemGrade<=19)
		{
			SCRIPTMGR->GetImage( 19, &m_LockImageEx, PFT_JACKPATH );
			VECTOR2 pos = { m_absPos.x+27, m_absPos.y+1};
			m_LockImageEx.RenderSprite( &vScale, NULL, 0.0f, &pos,0xffffffff);
		}
	}
#endif
	char Pricing[16]= { 0, };
	char BuyPriceCanceled[16]= { 0, };
	char OfferPrice[16]= { 0, };
	if(m_wItemType!=-500)
	{
		if( strlen(m_ItemName)>0)
		{
			RECT rect = {(LONG)m_absPos.x+50, (LONG)m_absPos.y+5, 25, 5};
			CFONT_OBJ->RenderFont(0, m_ItemName, strlen(m_ItemName), &rect, RGBA_MERGE(m_dwImageRGB, m_alpha * m_dwOptionAlpha / 100 ));
		}
		if( strlen(m_ItemName2)>4)
		{
			RECT rect = {(LONG)m_absPos.x+50, (LONG)m_absPos.y+25, 25, 5};
			CFONT_OBJ->RenderFont(0, m_ItemName2, strlen(m_ItemName2), &rect, RGBA_MERGE(RGB_HALF(250 ,250 ,200), m_alpha * m_dwOptionAlpha / 100 ));
		}
	}
	else
	{
		if( strlen(m_ItemName2)>4)
		{
			RECT rect = {(LONG)m_absPos.x+50, (LONG)m_absPos.y+25, 25, 5};
			CFONT_OBJ->RenderFont(0, m_ItemName2, strlen(m_ItemName2), &rect, RGBA_MERGE(RGB_HALF(250 ,0 ,0), m_alpha * m_dwOptionAlpha / 100 ));
		}
	}

	if(m_wItemType>=1 && m_wItemType<=3)
	{
		SCRIPTMGR->GetImage(sCashShopIcon.vIconPos[m_wItemType-1].z, &m_GoldOrMallImg[m_wItemType-1], PFT_JACKPATH);
		switch(m_wItemType)
		{
		case 1: strcpy(Pricing, AddComma(m_MallPrice)); break;
		case 2: strcpy(Pricing, AddComma(m_GoldPrice)); break;
		case 3: strcpy(Pricing, AddComma(m_BuyPrice));  break;
		}
		RECT rect = {(LONG)m_absPos.x+70, (LONG)m_absPos.y+50, 25, 5};
		CFONT_OBJ->RenderFont(0, Pricing, strlen(Pricing), &rect, RGBA_MERGE(0xffffff, m_alpha * m_dwOptionAlpha / 100 ));
		if(m_OfferPrice>0)
		{
			strcpy(BuyPriceCanceled, "---------");
			RECT rect2 = {(LONG)m_absPos.x+GARISX, (LONG)m_absPos.y+GARISY, 25, 5};
			CFONT_OBJ->RenderFont(FONTOFFER, BuyPriceCanceled, strlen(Pricing), &rect2, RGBA_MERGE(0xFF0000, 255));
			strcpy(OfferPrice, AddComma(m_OfferPrice));
			RECT rect = {(LONG)m_absPos.x+NEWPRICEX, (LONG)m_absPos.y+NEWPRICEY, 25, 5};
			CFONT_OBJ->RenderFont(1, OfferPrice, strlen(OfferPrice), &rect, RGBA_MERGE(0xFFFF00, m_alpha * m_dwOptionAlpha / 100 ));
		}


		if (strlen(m_ItemVipOnly)>0)//FOR VIP ITEM
		{
			RECT rect = { (LONG)m_absPos.x + 50, (LONG)m_absPos.y + 25, 25, 5 };
			CFONT_OBJ->RenderFont(0, m_ItemVipOnly, strlen(m_ItemVipOnly), &rect, RGBA_MERGE(RGB_HALF(0, 250, 0), m_alpha * m_dwOptionAlpha / 100));
		}
	}
	for(int i= 0;i<3;i++)
	{
		if(!m_GoldOrMallImg[i].IsNull())
		{
			VECTOR2 sc;
			sc.x=(LONG)m_absPos.x+sCashShopIcon.vIconPos[i].x;
			sc.y=(LONG)m_absPos.y+sCashShopIcon.vIconPos[i].y;
			m_GoldOrMallImg[i].RenderSprite( NULL, NULL, 0, &sc,RGBA_MERGE(0xffffff, m_alpha * m_dwOptionAlpha / 100));
		}
	}
	if(m_Button)
	{
		m_Button->Render();
	}
}