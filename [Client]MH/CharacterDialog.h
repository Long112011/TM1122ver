



// CharacterDialog.h: interface for the CCharacterDialog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHARACTERDIALOG_H__EF27ABB3_EA18_4466_88E9_4035E3807912__INCLUDED_)
#define AFX_CHARACTERDIALOG_H__EF27ABB3_EA18_4466_88E9_4035E3807912__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./Interface/cDialog.h"
#include "./Interface/cStatic.h"
#include "./Interface/cGuagen.h"


class CMunpaMarkDialog;
class cStatic;


struct CHARSTATICCTRL
{
	cStatic* munpa;
	cStatic* jikwe;
	cStatic* fame;
	cStatic* badfame;
	cStatic* name;
	cStatic* stage;		// ����
#ifdef _JAPAN_LOCAL_
	cStatic* attrib;
#endif
//	cStatic* job;		// ����
//	cStatic* ideology;	// ��ġ��

	cStatic* genGoal;
	cStatic* simmak;
	cStatic* minchub;
	cStatic* cheryuk;
	cStatic* level;
	cStatic* expPercent;		// �������� ǥ��
	cStatic* point;		// �ܿ�����Ʈ

	cStatic* meleeattack;	// ���ݷ�
	cStatic* rangeattack;	// ���ݷ�
	cStatic* defense;
//	cStatic* kyeongkong;
//	cStatic* move;
	cStatic* life;
	cStatic* Shield;
	cStatic* naeryuk;
//	cStatic* dok;


	cStatic* spname;
	cStatic* critical;
	cStatic* attackdistance;
    cStatic* reset;            // reset idx cStatic  define 2014-05-06
	cStatic* idx;
	
};

#ifdef _JAPAN_LOCAL_
struct ATTRDEFENCE
{
	ATTRIBUTE_VAL<cStatic*> pStatic;

	void SetValue(WORD attrib,WORD value,DWORD color=0)
	{
		char temp[32];
		sprintf(temp,"%d",value);
		pStatic.GetElement_Val(attrib)->SetStaticText(temp);
		if( color )
			pStatic.GetElement_Val(attrib)->SetFGColor(color);
	}
};

struct ATTROFFENCE
{
	ATTRIBUTE_VAL<cStatic*> pStatic;

	void SetValue(WORD attrib,DWORD valueMin, DWORD valueMax, DWORD color=0)	//SW050920 ����
	{
		char temp[32];

		sprintf(temp,"%d ~ %d",valueMin, valueMax);
		pStatic.GetElement_Val(attrib)->SetStaticText(temp);
		if( color )
			pStatic.GetElement_Val(attrib)->SetFGColor(color);
	}
};


#else
struct ATTRDEFENCE
{
	ATTRIBUTE_VAL<cStatic*> pStatic;
	ATTRIBUTE_VAL<cGuagen*> pGuage;

	void SetValue(WORD attrib,WORD value,DWORD color=0)
	{
		char temp[32];
		sprintf(temp,"%d%%",value);
		pStatic.GetElement_Val(attrib)->SetStaticText(temp);
		pGuage.GetElement_Val(attrib)->SetValue(value*0.01f);
		if( color )
			pStatic.GetElement_Val(attrib)->SetFGColor(color);
	}
};
#endif


class cGuagen;


class CCharacterDialog  : public cDialog
{
	CHARSTATICCTRL		m_ppStatic;			// ���� ��µ� ������
	cButton *			m_pPointBtn[MAX_BTN_POINT];

	cButton*			m_pPointMinusBtn[MAX_BTN_POINT];
	int					m_nocoriPoint;

	CMunpaMarkDialog*	m_pMunpaMarkDlg;

	ATTRDEFENCE			m_AttrDefComponent;


#ifdef _JAPAN_LOCAL_
	ATTROFFENCE			m_AttrOffComponent;

#endif

	int					m_MinusPoint;

public:
	CCharacterDialog();
	virtual ~CCharacterDialog();
	virtual void Init(LONG x, LONG y, WORD wid, WORD hei, cImage * basicImage, LONG ID=0);
	void Linking();
	virtual void SetActive( BOOL val );	

	void UpdateData();				// ���� ������ �ʴ°�

	void UpdateForStageAbility();
	CHARSTATICCTRL * GetCharacterData(){return &m_ppStatic;}
	void RefreshGuildInfo();

	void SetLevel(LEVELTYPE level);
	void SetLife(DWORD life);
	void SetShield(DWORD Shield);
	void SetNaeRyuk(DWORD naeryuk);
	void SetGenGol();
	void SetMinChub();
	void SetCheRyuk();
	void SetSimMek();
	void SetExpPointPercent(float perc);
	void SetAttackRate();
	void SetDefenseRate();

	void SetReSet();   // 2014-05-06 Reset idx Set 2014-05-06

	void SetIdx();
	
	void SetCritical();
	void SetAttackRange();
	void SetFame(FAMETYPE fame);
	void SetBadFame(FAMETYPE badfame);

	void SetPointLeveling(BOOL val, WORD point=0);	
	void OnAddPoint(BYTE whatsPoint);
	void RefreshPointInfo();
	void OnMinusPoint(BYTE whatsPoint);
	
	// RaMa
	void SetMinusPointValue( WORD point )		{	m_MinusPoint = point;	}
	void SetPointLevelingHide();
	void RefreshInfo();

	void SetStage( BYTE Stage );
};

#endif // !defined(AFX_CHARACTERDIALOG_H__EF27ABB3_EA18_4466_88E9_4035E3807912__INCLUDED_)


