#include "Interface\cDialog.h"
#include "itemManager.h"
#include "VirtualItem.h"
#include "Item.h"
#define STONEJUEXING     52043  // 觉醒石
#define STONECHANGE100   52044 // 洗练 Entry1 + Entry2
#define STONEENTRY1      52045 // 洗 Entry1
#define STONEENTRY2      52046  // 洗 Entry2
class cIconDialog;
class CItem;
class cStatic;
class cIcon;

class CItemQualityDlg : public cDialog
{
	cDialog * pDlg; //界面

	CItem   * pQualityItem;

	cIconDialog*	m_pIconDlgEquip;//主装备
	cIconDialog*	m_pIconDlgExtraEquip;//主材料装备
	cIconDialog*	m_pIconDlgEquipExt;//主装备

	CVirtualItem	m_VirtualItemEquip;
	CVirtualItem	m_VirtualItemExtraEquip;
	CVirtualItem	m_VirtualItemEquipExt;
	
	DWORD		m_ItemIdx;
	DWORD		m_ItemPos;

	cStatic*	m_ItemRand; //成功率

public:

	CItemQualityDlg();
	virtual ~CItemQualityDlg();

	void Linking();
	virtual BOOL FakeMoveIcon(LONG x,LONG y,cIcon * pIcon);
	void AddVirtualItem(CItem * pItem);
	void SendItemQualityMsg();
	void Release(int val);
	void ReleaseExt();
	void AddVirtualItemExt(CItem * pItem);
	void SetShopItemInfo(DWORD ItemIdx, DWORD ItemPos)		{	m_ItemIdx=ItemIdx;	m_ItemPos=ItemPos;		}
	void SetItemQualityRand(WORD val);
};