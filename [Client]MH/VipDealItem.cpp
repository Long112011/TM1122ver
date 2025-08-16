#include "stdafx.h"
#include "VipDealItem.h"
#include "ItemManager.h"
#include "./Interface/cFont.h"
#include "ChatManager.h"
#include "interface/cScriptManager.h"

cVipDealItem::cVipDealItem(void)
{
	m_type = WT_DEALITEM;
}

cVipDealItem::~cVipDealItem(void)
{
}
char* cVipDealItem::GetItemName()
{
	ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo(m_wItemIdx);
	if (pItemInfo)

		return pItemInfo->ItemName;
	return NULL;

}
eITEM_KINDBIT cVipDealItem::GetItemKind()
{
	ITEM_INFO* pInfo = ITEMMGR->GetItemInfo(m_wItemIdx);
	ASSERT(pInfo);	//KES 040512
	if (pInfo)
		return (eITEM_KINDBIT)pInfo->ItemKind;

	return (eITEM_KINDBIT)0;
}
void cVipDealItem::Render()
{
	cDialog::Render();
	static char nums[3];
	wsprintf(nums, "1");
	RECT rect = { (LONG)m_absPos.x + 32, (LONG)m_absPos.y + 30, 1,1 };
	CFONT_OBJ->RenderFont(0, nums, strlen(nums), &rect, RGBA_MERGE(m_dwImageRGB, m_alpha * m_dwOptionAlpha / 100));		// color hard coding : taiyo 
	//if (GetItemKind()&eSHOP_ITEM)
	//{//百宝使用后，显示已使用图标
	//	if( ITEMMGR->GetUsedItemInfo( m_wItemIdx ) )
	//	{
	//		cImage  imgBaseInfo;
	//		SCRIPTMGR->GetImage( 208, &imgBaseInfo, PFT_HARDPATH );
	//		VECTOR2 vScale = { 1.0f, 1.0f };
	//		VECTOR2 pos = { m_absPos.x, m_absPos.y};
	//		imgBaseInfo.RenderSprite( &vScale, NULL, 0.0f, &pos,0xffffffff);
	//	}
	//}
}