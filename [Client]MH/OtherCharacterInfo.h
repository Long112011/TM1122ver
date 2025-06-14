#pragma once
#include "CharacterDialog.h"
#include "WearedExDialog.h"
#include "Item.h"
class CPlayer;
class CCharactorInfoDlg: public cDialog
{
public:

	CHARSTATICCTRL		m_ppStatic;			// °ªÀÌ Ãâ·ÂµÉ º¯¼öµé
	CMunpaMarkDialog*	m_pMunpaMarkDlg;
	ATTRDEFENCE			m_AttrDefComponent;
public:
	CCharactorInfoDlg(void);
	virtual ~CCharactorInfoDlg(void);
	virtual void Init(LONG x, LONG y, WORD wid, WORD hei, cImage * basicImage, LONG ID=0);
	virtual void SetActive( BOOL val );
	void Linking();

	void SetLevel(LEVELTYPE level);
	void SetLife(CPlayer* pPlayer);
	void SetShield(DWORD Shield);
	void SetNaeRyuk(DWORD naeryuk);
	void SetGenGol(WORD resVal);
	void SetMinChub(WORD resVal);
	void SetCheRyuk(WORD resVal);
	void SetSimMek(WORD resVal);
	void SetExpPointPercent(float perc);
	void SetAttackRate();
	void SetDefenseRate(WORD value);

	void SetCritical(DWORD resVal1);
	void SetAttackRange(WORD val);
	void SetFame(FAMETYPE fame);
	void SetBadFame(FAMETYPE badfame);
	void SetStage( BYTE Stage );
	void SetReStage(BYTE ReStage);  // 2014-08-11 ÊôÐÔ×ªÉú
	void SetIdx(DWORD Idx);         // 2014-08-11 ÊôÐÔId

	void ShowDialog(CPlayer * pPlayer);
};