#pragma once




// CharacterDialog.h: interface for the CCharacterDialog class.
//
//////////////////////////////////////////////////////////////////////



#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./Interface/cDialog.h"
#include "./Interface/cStatic.h"
#include "./Interface/cGuagen.h"


class CMunpaMarkDialog;
class cStatic;
class cPushupButton;

struct CHARSTATICCTRL_PVP
{
	cStatic* PVPCri;
	cStatic* PVPAttack;
	cStatic* PVPDef;
	cStatic* PVPADef;
	cStatic* PVPADodge;
	cStatic* PVPHit;
	cStatic* PVPCriticalDamage;
	cStatic* PVPStun;
	cStatic* PVPDAMAGE_Des;
	cStatic* cheryuk;
	cStatic* level;
	cStatic* expPercent;
	cStatic* point;

	cStatic* meleeattack;
	cStatic* rangeattack;
	cStatic* defense;
	cStatic* life;
	cStatic* Shield;
	cStatic* naeryuk;

	cStatic* spname;
	cStatic* critical;
	cStatic* attackdistance;
	cStatic* reset;
	cStatic* idx;
	cStatic* criticaldamage;
	cStatic* dodge;
	cStatic* stunpvp;
	cStatic* phyandattdamage;

};

#ifdef _JAPAN_LOCAL_
struct ATTRDEFENCE
{
	ATTRIBUTE_VAL<cStatic*> pStatic;
	critical_damage
		void SetValue(WORD attrib, WORD value, DWORD color = 0)
	{
		char temp[32];
		sprintf(temp, "%d", value);
		pStatic.GetElement_Val(attrib)->SetStaticText(temp);
		if (color)
			pStatic.GetElement_Val(attrib)->SetFGColor(color);
	}
};

struct ATTROFFENCE
{
	ATTRIBUTE_VAL<cStatic*> pStatic;

	void SetValue(WORD attrib, DWORD valueMin, DWORD valueMax, DWORD color = 0)	//SW050920 荐沥
	{
		char temp[32];

		sprintf(temp, "%d ~ %d", valueMin, valueMax);
		pStatic.GetElement_Val(attrib)->SetStaticText(temp);
		if (color)
			pStatic.GetElement_Val(attrib)->SetFGColor(color);
	}
};


#else
struct ATTRDEFENCE_PVP
{
	ATTRIBUTE_VAL<cStatic*> pStatic;
	ATTRIBUTE_VAL<cGuagen*> pGuage;

	void SetValue(WORD attrib, WORD value, DWORD color = 0)
	{
		char temp[32];
		sprintf(temp, "%d%%", value);
		pStatic.GetElement_Val(attrib)->SetStaticText(temp);
		if (color)
			pStatic.GetElement_Val(attrib)->SetFGColor(color);
	}
};
struct ATTROFFENCE_PVP
{
	ATTRIBUTE_VAL<cStatic*> pStatic;

	void SetValue(WORD attrib, DWORD valueMin, DWORD valueMax, DWORD color = 0)	//SW050920 荐沥
	{
		char temp[32];

		sprintf(temp, "%d/%d%%", valueMin, valueMax);
		pStatic.GetElement_Val(attrib)->SetStaticText(temp);
		if (color)
			pStatic.GetElement_Val(attrib)->SetFGColor(color);
	}
};
#endif


class cGuagen;


class CCharacterPvpDialog : public cDialog
{
	CHARSTATICCTRL_PVP		m_ppStatic;
	cButton* m_pPointBtn[MAX_BTN_POINT];

	cButton* m_pPointMinusBtn[MAX_BTN_POINT];

	cButton* m_pPointBtn_ALL[MAX_BTN_POINT];	//天墨技术团 增加一键加点

	cStatic* m_MoveSpeed;	//天墨技术团 移动速度增加

	int					m_nocoriPoint;

	CMunpaMarkDialog* m_pMunpaMarkDlg;

	ATTRDEFENCE_PVP			m_AttrDefComponent;
	ATTROFFENCE_PVP			m_AttrOffComponent;


	int					m_MinusPoint;

public:
	CCharacterPvpDialog();
	virtual ~CCharacterPvpDialog();
	virtual void Init(LONG x, LONG y, WORD wid, WORD hei, cImage* basicImage, LONG ID = 0);
	void Linking();
	virtual void SetActive(BOOL val);

	void UpdateData();				// 芭狼 函窍瘤 臼绰巴

	void SetPVPDefens();
	void SetPVPATTDefens();
	void SetPVPCritical();
	void SetPVPCriticalDamage();
	void SetPVPDodge();
	void SetPVPDAMAGE_DES();
	void SetAttackRate();//攻击力
	CHARSTATICCTRL_PVP* GetCharacterData() { return &m_ppStatic; }




	void RefreshInfo();

	void SetALLPhyDefense();
	void SetALLATTDefense();
	void SetALLCritical();
	void SetALLCriticalDamage();
	void SetALLDodge();
	void SetALLStun();
	void SetALLAttack();
	void SetCriticalDamage();
	void SetDodge();
	void SetStun();
	void SetPhyAndAttDamage();
};




