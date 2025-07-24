#include "Interface\cDialog.h"
#include "itemManager.h"
#include "VirtualItem.h"
#include "Item.h"
class cIconDialog;
class CItem;
class cStatic;
class cIcon;

class CItemQualityChangeDlg : public cDialog
{
	cDialog * pDlg; //����

	cIconDialog*	m_pIconDlgEquip;//��װ��

	CVirtualItem	m_VirtualItemEquip;
	DWORD		m_ItemIdx;
	DWORD		m_ItemPos;
	cStatic* m_pItemQualityText;  // ��ʾ��ǰװ��Ʒ������

public:

	CItemQualityChangeDlg();
	virtual ~CItemQualityChangeDlg();

	void Linking();
	virtual BOOL FakeMoveIcon(LONG x,LONG y,cIcon * pIcon);
	void AddVirtualItem(CItem * pItem);
	void SendItemQualityMsg();
	void RefreshVirtualEquipIcon();
	void Release();
	void SetShopItemInfo(DWORD ItemIdx, DWORD ItemPos)		{	m_ItemIdx=ItemIdx;	m_ItemPos=ItemPos;		}
	void UpdateItemQualityText(WORD quality);
	void OnItemQualityChanged(CItem* pItem);
};