#pragma once
#include "./interface/cDialog.h"
#include "./interface/cMultiLineText.h"
#include "Item.h"
#include <vector>
#define ITEMLINKDLG USINGTON(CSendEquipSelectDialog)
class CSendEquipSelectDialog : public  cDialog
{

	cMultiLineText* m_pSendItemInfoTip;
	DWORD m_dwItemDBidx;
	std::vector<DWORD> m_NeedDeleteItem;


	RECT m_m_leftTopPos;
public:

	CSendEquipSelectDialog();
	~CSendEquipSelectDialog();

	void SetItem(DWORD ItemDBidx, cMultiLineText *Tip, BOOL bNeedDelete = FALSE);

	void Show();
	DWORD ItemLinkHasItem(){ return m_dwItemDBidx; }

	virtual void SetItemDBIdx(DWORD val){ m_dwItemDBidx = val; }


	virtual void Render();

	void SetXY(LONG x, LONG y){ m_m_leftTopPos.left = x; m_m_leftTopPos.top = y; }

};
EXTERNGLOBALTON(CSendEquipSelectDialog)