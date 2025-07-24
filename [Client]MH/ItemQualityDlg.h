#include "Interface\cDialog.h"
#include "itemManager.h"
#include "VirtualItem.h"
#include "Item.h"
#define STONEJUEXING     52043  // ����ʯ
#define STONECHANGE100   52044 // ϴ�� Entry1 + Entry2
#define STONEENTRY1      52045 // ϴ Entry1
#define STONEENTRY2      52046  // ϴ Entry2
class cIconDialog;
class CItem;
class cStatic;
class cIcon;

class CItemQualityDlg : public cDialog
{
	cDialog * pDlg; //����

	CItem   * pQualityItem;

	cIconDialog*	m_pIconDlgEquip;//��װ��
	cIconDialog*	m_pIconDlgExtraEquip;//������װ��
	cIconDialog*	m_pIconDlgEquipExt;//��װ��

	CVirtualItem	m_VirtualItemEquip;
	CVirtualItem	m_VirtualItemExtraEquip;
	CVirtualItem	m_VirtualItemEquipExt;
	
	DWORD		m_ItemIdx;
	DWORD		m_ItemPos;

	cStatic*	m_ItemRand; //�ɹ���

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