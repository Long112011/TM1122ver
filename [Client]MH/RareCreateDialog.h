#pragma once
#include "./interface/cDialog.h"
#include "VirtualItem.h"
class cIconDialog;
class CItem;
class CRareCreateDialog : public cDialog
{
	cIconDialog*	m_pIconDlg;
	CVirtualItem	m_VirtualItem;
	DWORD		m_ItemIdx;
	DWORD		m_ItemPos;
protected://js	
	cStatic * Rare_Info;//js	
	cStatic * RareMax_Info;//js	
	ITEM_RARE_Max_INFO RARE_Max_INFO[3];//js	
	void SetRareInfo(ITEM_RARE_OPTION_INFO* RareInfo);//js	
	void LoadRARE_Max_INFO();//js	
	ITEM_RARE_Max_INFO GetRARE_Max_INFO(int Index);//js	
	void SetRARE_Max_INFO(int Index);//js							  
public:
	void Clear_RARE_INFO();//js					
	CRareCreateDialog(void);
	~CRareCreateDialog(void);
	void Linking();
	virtual BOOL FakeMoveIcon(LONG x, LONG y, cIcon * icon);
	void ReleaseItem();
	void AddVirtualItem(CItem* pItem);
	void SetShopItemInfo(DWORD ItemIdx, DWORD ItemPos)		{ m_ItemIdx = ItemIdx;	m_ItemPos = ItemPos; }
	void ItemRareCreateSyn();
	void ItemRareCreateAck();
	void ItemRareCreateNAck();
	BOOL Ishint();//js		   
};
