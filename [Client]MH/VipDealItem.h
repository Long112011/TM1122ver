#pragma once
#include "./BaseItem.h"
class cVipDealItem : public CBaseItem
{
public:
	cVipDealItem(void);
	~cVipDealItem(void);
	char* GetItemName();
	void SetItemIdx(WORD itemIdx)
	{
		m_wItemIdx = itemIdx;
	}
	WORD GetItemIdx()
	{
		return m_wItemIdx;
	}
	void Render();
	eITEM_KINDBIT GetItemKind();
private:
	WORD m_wItemIdx;

};
