#include "./interface/cDialog.h"
#include "WindowIDEnum.h"
#include "./Interface/cIconGridDialog.h"
#include "VirtualItem.h"
class CGradeChangeDlg :public cDialog
{
	cIconGridDialog* ICON_FORM_ITEM;	//����Ʒ
	cIconGridDialog* ICON_TO_ITEM;		//ȥ��Ʒ
	cIconGridDialog* ICON_LAST_ITEM;
	cStatic* GradeColor1; //��ʾ����
	CVirtualItem	m_FORM_VITEM;
	CVirtualItem	m_TO_VITEM;
	CVirtualItem	m_LAST_VITEM;



	DWORD			m_ItemIdx;
	DWORD			m_ItemPos;
public:
	CGradeChangeDlg();
	virtual  ~CGradeChangeDlg();
	void Linking();
	virtual void SetActive(BOOL val);
	void OnActionEvent(LONG lId, void* p, DWORD we);
	virtual BOOL FakeMoveIcon(LONG x, LONG y, cIcon* pOrigIcon);
	void SetShopItemInfo(DWORD ItemIdx, DWORD ItemPos) { m_ItemIdx = ItemIdx;	m_ItemPos = ItemPos; }
};
