
#if _MSC_VER > 1000

#pragma once
#endif // _MSC_VER > 1000

#include "cDialog.h"
#include "VirtualItem.h"
#include "ItemShow.h"
#include "./Interface/cScriptManager.h"
//宠物合成规则
//1.物品列表宠物ID必须连续 结果宠物=主宠id+1  并且星级(主宠pInfo->wLevel+1)=(副宠pInfo->wLevel)为判断条件
//2.材料宠物为相同星级的三阶段宠物
//3.成功率=100-宠物星级*10
class cGuagen;
class cStatic;
class CVirtualItem;
class cImage;
class cIconDialog;
class CItem;

class PetMixDlg :
	public cDialog
{
	DWORD		m_ShopItemIdx;
	DWORD		m_ShopItemPos;
	cGuagen* m_pOddsGuage;
	CVirtualItem VirtualItem[3];
	cIconDialog* ItemIcon[3];
	cIconDialog* ResultPetIcon;
	CItemShow ResultPetShow;
	ImageDigit m_ImageDigit[5];
	// 	cImage  WhiteDigitMax[10];
	// 	cImage  WhiteDigitMin[10];
public:
	PetMixDlg();
	~PetMixDlg();
	void Linking();
	void OnActionEvent(LONG lId, void* p, DWORD we);
	void SetShopItemInfo(DWORD ItemIdx, DWORD ItemPos) { m_ShopItemIdx = ItemIdx;	m_ShopItemPos = ItemPos; }
	void Show(BOOL val);
	virtual void Render();
	virtual BOOL FakeMoveIcon(LONG x, LONG y, cIcon* icon);
	void AddPetItemIcon(BYTE i, CItem* pOrigItem);
	void RemovePetItemIcon(BYTE i);
	void ResultPetAddIcon();
	void RemoveResultPetIcon();
	//ITEM_INFO* IsCanMix(CItem* Pet);
	BOOL IsCanMix(CItem* Pet);
	void CalculateSuccessRate();


	void MixSyn();
	void MixAck(MSG_DWORD3* pmsg);
};

