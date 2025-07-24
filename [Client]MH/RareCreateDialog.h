#pragma once


#include "./interface/cDialog.h"
#include "VirtualItem.h"

class cIconDialog;
class CItem;

class CRareCreateDialog : public cDialog
{
	cIconDialog* m_pIconDlg;
	CVirtualItem	m_VirtualItem;
	sRareOptionInfo m_RereItemInfo[RareItemKindMAX];

	DWORD		m_ItemIdx;
	DWORD		m_ItemPos;
	cStatic* Rare_Info;

public:
	CRareCreateDialog(void);
	~CRareCreateDialog(void);

	void Linking();
	virtual BOOL FakeMoveIcon(LONG x, LONG y, cIcon* icon);

	void ReleaseItem();
	void AddVirtualItem(CItem* pItem);

	void SetShopItemInfo(DWORD ItemIdx, DWORD ItemPos) { m_ItemIdx = ItemIdx;	m_ItemPos = ItemPos; }
	void ItemRareCreateSyn();
	void ItemRareCreateAck();
	void ItemRareCreateNAck();
	void SetRareInfo(ITEM_RARE_OPTION_INFO* pRareOptionInfo, DWORD ItemIdx);
	void Clear();
	ITEM_RARE_OPTION_INFO* GetRareItemMaxValue(DWORD ItemIdx);//��ȡ��Ʒף�����ֵ
	bool LoadRareItemOptionInfo();//�ͻ��˼���ף����Ϣ
};
