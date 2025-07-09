#include "./interface/cDialog.h"
#include "WindowIDEnum.h"
#include "./Interface/cIconGridDialog.h"
#include "VirtualItem.h"
#include "item.h"
#include "ItemShow.h"
#include "./interface/cStatic.h"
#include "./interface/cButton.h"
#include "./interface/cCheckBox.h"
#include "./interface/cListDialog.h"
#include "ObjectGuagen.h"

#define ITEM_STONE_1 52035
#define ITEM_STONE_1_RED 52036
#define ITEM_STONE_2 52037
#define ITEM_STONE_2_RED 52038
#define ITEM_STONE_3 52039
#define ITEM_STONE_3_RED 52040
#define ITEM_STONE_4 52041
#define ITEM_STONE_4_RED 52042


class COfficialUpGradeDlg :public cDialog
{
	cDialog* pDlg;
	cIconGridDialog* MainItem;
	CVirtualItem	m_MainItem;

	cIconGridDialog* MainItem_ProTect;
	cIconGridDialog* ClearMainItem_ProTect;
	cIconGridDialog* ClearMainItem;
	cIconGridDialog* ClearMainItemDEC_1;
	cIconGridDialog* ClearMainItemDEC_2;
	cIconGridDialog* ClearMainItemDEC_3;
	cIconGridDialog* ClearMainItemDEC_4;

	CVirtualItem	m_MainItem_ProTect;

	CObjectGuagen* BarPercent;

	cIconGridDialog* VirTual;
	CItemShow	m_VirTual;

	cStatic* ItemName;
	cStatic* BgEffectItem;
	cStatic* GradeColor;
	cStatic* SmallGrade;
	cButton* Btn_Plus[4];
	cButton* Btn_Minus[4];
	cButton* Btn_Max[4];
	cStatic* Matirial_Dur[4];
	cStatic* MoneyStatic;
	cCheckBox* CheckType;



	cStatic* Int_Percent;
	cStatic* Tod_Percent;

	cListDialog* ListInfoMat;
	cStatic* Bg[3];
	cButton* Btn[2];

	cStatic* FreeStatic[5];

	cStatic* MatInfoBg[120];

	cStatic* MatCheck[30];


	DWORD	ItemMatirial_Have[4];

	cIconGridDialog* ImgItem[4];

	cStatic* BgState[3];

	WORD	ItemNeed_1 = 0;
	WORD	ItemNeed_2 = 0;
	WORD	ItemNeed_3 = 0;
	WORD	ItemNeed_4 = 0;

	WORD CollectItemIdx_1[80];
	WORD CollectItemPos_1[80];
	WORD CollectItemDur_1[80];

	WORD CollectItemIdx_2[80];
	WORD CollectItemPos_2[80];
	WORD CollectItemDur_2[80];

	WORD CollectItemIdx_3[80];
	WORD CollectItemPos_3[80];
	WORD CollectItemDur_3[80];

	WORD CollectItemIdx_4[80];
	WORD CollectItemPos_4[80];
	WORD CollectItemDur_4[80];

	DWORD	MaxPercent = 0;
	DWORD	MoneyUse = 0;
	float nowPercent = 0.0;

	DWORD CollectCountMat_1 = 0;
	DWORD CollectCountMat_2 = 0;
	DWORD CollectCountMat_3 = 0;
	DWORD CollectCountMat_4 = 0;
	DWORD m_CanUseTimeCount;
public:
	COfficialUpGradeDlg();
	virtual  ~COfficialUpGradeDlg();
	BOOL FakeMoveIcon(LONG x, LONG y, cIcon* pOrigIcon);
	void SetActive(BOOL val);
	void AddVirtualItem(CItem* pItem, DWORD num, WORD Grade = 0);
	void Linking();
	void OnActionEvent(LONG lId, void* p, DWORD we);
	void FindItem(DWORD ItemIdx, DWORD ItemIdx2, DWORD Count, WORD Type);
	BOOL IsCanUsePack(WORD& Time);
	void AllAfterAllFunction();
	void CheckStatusItem();
	void ResetMatirial();
	bool ValidateMaterialAvailability();
	BOOL CheckPercent();
	void OkBtn();
	void RefreshAlldata(CItem* pItem);
	void SetStateBg(WORD State);
	void clear_VirtualItemEquip();
	void UpdateCurrentEquipmentInfo();
	void SetDataMatirial();
	void clear_VirtualItemStone();
	void ClearMaterial1();
	void ClearMaterial2();
	void ClearMaterial3();
	void ClearMaterial4();
	void FindMaxBtn(WORD ItemIdx1, WORD ItemIDx2, WORD Type);
};