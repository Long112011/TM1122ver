#include "StdAfx.h"
#include "SendEquipSelectDialog.h"
#include "cWindowManager.h"
#include "ItemManager.h"
GLOBALTON(CSendEquipSelectDialog)
CSendEquipSelectDialog::CSendEquipSelectDialog()
{
	m_pSendItemInfoTip = new cMultiLineText;
	m_dwItemDBidx = -1;
	SetRect(&m_m_leftTopPos, 0, 0, 296, 27);
}
CSendEquipSelectDialog::~CSendEquipSelectDialog()
{
	//SAFE_DELETE(m_pSendItemInfoTip);
	if (IsActive())
		SetActive(FALSE);
	if (m_NeedDeleteItem.size() == 0) return;
	std::vector<DWORD>::iterator iter;
	iter = m_NeedDeleteItem.begin();
	for (; iter != m_NeedDeleteItem.end(); iter++)
	{
		CItem * pItem = ITEMMGR->GetItem(*iter);
		if (pItem)
			ITEMMGR->ItemDelete(pItem);
	}
	if (m_NeedDeleteItem.size() > 0)
	{
		m_NeedDeleteItem.clear();
		std::vector<DWORD>().swap(m_NeedDeleteItem);
	}
}
void CSendEquipSelectDialog::SetItem(DWORD ItemDBidx, cMultiLineText *Tip, BOOL bNeedDelete)
{
	m_dwItemDBidx = ItemDBidx;
	if (bNeedDelete)
	{
		m_NeedDeleteItem.push_back(ItemDBidx);
	}
	if (m_pSendItemInfoTip)
	{
		m_pSendItemInfoTip->SetItemDBIdx(0);
	}
	m_pSendItemInfoTip = Tip;
}
void CSendEquipSelectDialog::Show()
{
	if (!m_pSendItemInfoTip) return;
	cDialog::SetActive(TRUE);
}
void CSendEquipSelectDialog::Render()
{
	if (!m_bActive)		return;
	m_pSendItemInfoTip->SetXY(GetAbsX() + 10, GetAbsY() + 5, FALSE);
	m_pSendItemInfoTip->Render();
	cDialog::Render();
	WINDOWMGR->SetSendItemInfoTip(NULL);
}
