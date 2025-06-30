#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./Interface/cDialog.h"
#include "cSpin.h"
#include "cCheckBox.h"
#include "VirtualItem.h"

class cGuage;
class cIconDialog;
class cStatic;
class cButton;
class CItem;
class CObjectGuagen;

/*enum RateFull
{
	UpGradeRate,
	MoneyRate,
	GENGOL_MINCHUB_CHERYUK_SIMMEK,
	LITE_SHIELD_NAERYUK_PHYDEF,
	CRITICAL,
	MELEE_RANGE_ATTACK,
	ELEMENT_ATTACK_REGIST,
	eMaxSetUp,
};
struct ALEXX_UPGRADESETUP
{
	float SetupGrade_AlexX[eMaxSetUp];
	ALEXX_UPGRADESETUP()
	{
		memset(&SetupGrade_AlexX, 0, sizeof(eMaxSetUp));
	}
};*/

class CNewUpGrareAlexXDlg : public cDialog
{
	cDialog * m_Dlg;
	cCheckBox *	m_UseRedStone;
	cIconDialog * m_pIconDlgEquip;
	cIconDialog * m_pIconDlgEquip2;
	cStatic * m_pNameIconDlgEquip;
	cIconDialog * m_pIconDlgProtecion;
	CObjectGuagen * m_pGuagepercent;
	cStatic * m_pMATERIAL_COUNT_01;
	cStatic * m_pMATERIAL_COUNT_02;
	cStatic * m_pMATERIAL_COUNT_03;
	cStatic * m_pMATERIAL_COUNT_04;

	DWORD REAL_MATERIAL_COUNT_01;
	DWORD REAL_MATERIAL_COUNT_02;
	DWORD REAL_MATERIAL_COUNT_03;
	DWORD REAL_MATERIAL_COUNT_04;
	cStatic * m_percent_total;
	float real_percent_total;
	int real_lv_item;
	WORD real_fullPercen;
	cStatic * m_percent_max;
	cStatic * m_r_img;
	BOOL real_r_img;
	BOOL realFuck_WTF_GradeAlexX;
	cStatic * m_money_total;
	DWORD real_money_total;

	cStatic * m_next_grade_text;
	cStatic * m_grade_text;

protected:
	CVirtualItem m_VirtualItemEquip;
	CVirtualItem m_VirtualItem_MATERIAL_1;
	CVirtualItem m_VirtualItem_MATERIAL_2;
	CVirtualItem m_VirtualItem_MATERIAL_3;
	CVirtualItem m_VirtualItem_MATERIAL_4;
	CVirtualItem m_VirtualItemEquip2;
	CVirtualItem m_VirtualItemStone;

public:

	CNewUpGrareAlexXDlg();
	virtual  ~CNewUpGrareAlexXDlg();
	void Linking();
	BOOL FakeMoveIcon(LONG x, LONG y, cIcon * icon);
	BOOL OnActionEvent(LONG lId, void * p, DWORD we);
	void AddVirtualItem(CItem * pItem);
	void AddVirtualItem2(CItem * pItem);
	void AddVirtualItemStone(CItem * pItem);
	void SetNameItemStaticText(char name[33], DWORD grade, WORD color);
	void SendItemUpGrareAlexXMsg();
	BOOL IsCanUpGrareItem(DWORD wItemIdx, eITEM_KINDBIT bits);
	void Add_MATERIAL_COUNT(DWORD lv);
	void Del_MATERIAL_COUNT(DWORD lv);
	void AddMAX_MATERIAL_COUNT(DWORD lv);
	DWORD WhatLvMATERIAL(DWORD lv);
	void UpdateGuagePercent(float curVal);
	void SetUpNew();
	void Re_UpdatePercent();
	void MP_BakcGame_Msg(MSG_NEWYPGRARE_ALEXX_BACKGAME* msg);
	void MP_BakcGame_DelItem_Msg(MSG_NEWYPGRARE_ALEXX_DELITEM* msg);
	void ReLoad_MATERIAL_COUNT();
	void SetImg_R(DWORD val);
	void close_thisdlg();
	float calculateMaterialCount(float max_naterial, DWORD lv);
	float calculateUpGradeItemPercent();
	void clear_VirtualItemEquip();
	void ClearMAX_MATERIAL_COUNT(DWORD lv);
	void Reload_VirtualItem(bool what);
	void SetrealFuck_WTF_GradeAlexX(bool Val) { realFuck_WTF_GradeAlexX = Val; }
	bool GetrealFuck_WTF_GradeAlexX(){ return realFuck_WTF_GradeAlexX; }
	DWORD calculateUpGradeItemMoeny();
	void UpdateMoney();
	void clear_VirtualItemStone();
	void UpdateNameItem();
	void LoadUpGradeItem_info();
	//ALEXX_UPGRADESETUP AlexXUpGradeConfig;
};