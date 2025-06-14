



// MonsterGuageDlg.h: interface for the CMonsterGuageDlg class.
//
//////////////////////////////////////////////////////////////////////


#if !defined(AFX_MONSTERGUAGEDLG_H__3B4654AD_257E_42F7_9996_C2C8E89E535E__INCLUDED_)
#define AFX_MONSTERGUAGEDLG_H__3B4654AD_257E_42F7_9996_C2C8E89E535E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./Interface/cDialog.h"

class cStatic;
class cPushupButton;
class CObjectGuagen;
class CMonster;

enum MonsterGuageMode

{

	eMonGuage_Monster,

	eMonGuage_Character,
	// 06. 01. Ÿ����â �������̽� ���� - �̿���
	eMonGuage_Npc,
	eMonGuage_Pet,

	eMonGuage_Max
};

class CMonsterGuageDlg : public cDialog  
{
public:
	CMonsterGuageDlg();

	virtual ~CMonsterGuageDlg();

//#ifdef _JSKEN_
	void	SetMonsterName(char * szMonsterName,DWORD CharIdx=0,BYTE kind=0);
//#else
//	void	SetMonsterName( char * szMonsterName );
//#endif
	void	SetMonsterLife( DWORD curVal, DWORD maxVal, int type=-1 );
	void	SetMonsterLife( CMonster * pMonster );
	void	SetMonsterShield( DWORD curVal, DWORD maxVal, int type=-1 );
	void	SetMonsterShield( CMonster * pMonster );
	void	SetGuildUnionName(char* GuildName, char* GuildUnionName);
	void	SetActiveMonGuageMode(int mode, BOOL bActive);
	// 06. 01. Ÿ����â �������̽� ���� - �̿���
	void	SetNpcName( char * szNpcName );

	void	Linking();
	void	ShowMonsterGuageMode(int mode);
	virtual void Render();

	CMonster*	GetCurMonster() { return m_pCurMonster; }
	

	// Imsi
	void	SetObjectType( DWORD Type )		{	m_Type = Type;	}
	DWORD	GetObjectType()					{	return m_Type;	}

	void SetMonsterNameColor(DWORD dwColor); //2007. 10. 31. CBH - ���� ����â ���̾�α� ���� �̸� �� ���� �߰�

protected:


	cStatic * m_pName;
	cStatic * m_pLifeText;
	CObjectGuagen * m_pLifeGuage;
	cStatic * m_pShieldText;
	CObjectGuagen * m_pShieldGuage;
	cStatic * m_pGuildName;
	cStatic * m_pGuildUnionName;
	// 06. 01. Ÿ����â �������̽� ���� - �̿���

    // װ����ť ,���棬 2014-07-27 

	cButton * m_pGetEquipList;
	cButton * m_pGetCharacterInfoList;
	cButton * m_pGetCharacterUseAdd;
	cButton * m_pGetCharacterMoney;

	cStatic * m_pNpcName;
	
	cPtrList m_MonGuageCtlListArray[eMonGuage_Max];
	int m_CurMode;

	CMonster * m_pCurMonster;				// ���� ǥ������ ����..
	DWORD	m_Type;							// ObjectType
};

#endif // !defined(AFX_MONSTERGUAGEDLG_H__3B4654AD_257E_42F7_9996_C2C8E89E535E__INCLUDED_)


