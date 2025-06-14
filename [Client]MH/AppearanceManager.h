



// AppearanceManager.h: interface for the CAppearanceManager class.
//
//////////////////////////////////////////////////////////////////////


#if !defined(AFX_APPEARANCEMANAGER_H__6121D532_2201_469B_A298_35F0C2E66876__INCLUDED_)
#define AFX_APPEARANCEMANAGER_H__6121D532_2201_469B_A298_35F0C2E66876__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define APPEARANCEMGR USINGTON(CAppearanceManager)

class CPlayer;
class CMonster;
class CNpc;
class CObject;
class CBossMonster;
class CPet;
class CMapObject;

class CEngineObject;
class CEngineEffect;

#include "PtrList.h"
#include "GameResourceStruct.h"
//#include "./AppearPlayer.h"

struct RESERVATION_ITEM_PLAYER
{

	CPlayer*					pObject;

	SEND_CHARACTER_TOTALINFO	Msg;
};



#define MAX_RESERVATION_ITEM_PLAYER 1024


class CAppearanceManager  
{
	struct APPEAR_CHANGE
	{
		CPlayer* pObject;
	};
	cPtrList m_PtrList;

	//2007. 11. 29. CBH - �ƹ�Ÿ �������� ����ó�� ������ ����ü
	struct AVATARITEM_EXCEPTION
	{
		DWORD dwItemIndex;
		DWORD Kind;
	};


	//2007. 11. 29. CBH - SetCharacterAppearanceToEngineObject�Լ��� ĳ���� ���� Appearance���� ����ü
	struct APPEARANCEDATA_INFO
	{
		BYTE gender;

		BYTE hair;
		BYTE face;
		WORD *WearedItemIdx;
		MOD_LIST* pHairList;
		MOD_LIST* pFaceList;
		MOD_LIST* pModList;
		WORD* AvatarItemIdx;
	};

	CYHHashTable<AVATARITEM_EXCEPTION> m_AvatarItemExceptionTable;	//2007. 11. 29. CBH - �ƹ�Ÿ ������ ����ó�� ������ ����Ʈ
	BOOL LoadAvatarItemExcetionList();	//2007. 11. 29. CBH - �ƹ�Ÿ ������ ����ó�� ������ ����Ʈ �ε�
	AVATARITEM_EXCEPTION* GetAvatarItemExcetion(WORD wItemIndex);	//2007. 12. 4. CBH - �ƹ�Ÿ ������ ����ó�� ������ ����Ʈ���� ã��

	void SetCharacterAppearance(CPlayer* pPlayer);
	void SetMonsterAppearance(CMonster* pMonster);
	void SetBossMonsterAppearance(CBossMonster* pMonster);
	void SetNpcAppearance(CNpc* pNpc);
	void SetPetAppearance(CPet* pPet);
    void SetTitanAppearance(CPlayer* pPlayer);

	BOOL SetCharacterAppearanceToEngineObject(CPlayer* pPlayer,CEngineObject* pEngineObject);
	void SetCharacterAppearanceToEffect(CPlayer* pPlayer,CEngineEffect* pRtEffect);
	BOOL SetTitanAppearanceToEngineObject( CPlayer* pPlayer, CEngineObject* pEngineObject );
	void SetCharacterEventAppearance(CPlayer* pPlayer, CEngineObject* pEngineObject, APPEARANCEDATA_INFO* pDataInfo); //2007. 11. 29. CBH - �̺�Ʈ ���� ĳ���� ��� ���� �Լ�
	void SetCharacterWearedAppearance(CPlayer* pPlayer, CEngineObject* pEngineObject, APPEARANCEDATA_INFO* pDataInfo); //2007. 11. 29. CBH - �Ϲ���� ���� ĳ���� ��� ���� �Լ�
	void SetCharacterAvatarAppearance(CPlayer* pPlayer, CEngineObject* pEngineObject, APPEARANCEDATA_INFO* pDataInfo); //2007. 11. 29. CBH - �ƹ�Ÿ ��� ���� ĳ���� ��� ���� �Լ�
	void SetCharacterSkinAppearance(CPlayer* pPlayer, CEngineObject* pEngineObject, APPEARANCEDATA_INFO* pDataInfo); //2007. 11. 30. CBH - �ƹ�Ÿ ��� ���� ĳ���� ��� ���� �Լ�

#ifndef _RESOURCE_WORK_NOT_APPLY_	
	
	cPtrList	m_lstReserveToAddPlayerList;
	//cPtrList	m_lstReserveToAddMonsterList;		


	cPtrList	m_lstAlphaProcessing;
	DWORD		m_dwLastProcessReservationTime;	
	BOOL		m_bUseReservation;	


	RESERVATION_ITEM_PLAYER*	m_pReserItemPlayer;
	cPtrList					m_lstFreeReserItemPlayer;

	DWORD						m_dwUsedReserItemPlayerNum;

	cPtrList	m_lstRemoveReserPlayer;
#endif

//private : 
	//CAppearPlayer m_AppearPlayer ;
public:

	CAppearanceManager();
	~CAppearanceManager();

	void Init();
	void Release();
	
	void InitAppearance(CObject* pObject);
	void AddCharacterPartChange(DWORD PlayerID);
	void ProcessAppearance();
	
	void ShowWeapon(CPlayer* pPlayer);
	void HideWeapon(CPlayer* pPlayer);
	void ShowWeapon(CPlayer* pPlayer,CEngineObject* pEngineObject);
	void HideWeapon(CPlayer* pPlayer,CEngineObject* pEngineObject);

	
	void InitAppearanceToEffect(CObject* pObject,CEngineEffect* pEffect);
	void SetScalebyGuageBar(DWORD PlayerID);
	//
	
	// 
	void SetAvatarItem( CPlayer* pPlayer, WORD ItemIdx, BOOL bAdd=TRUE );
	void SetMapObjectAppearance(CMapObject* pMapObject, char* strData, float fScale );
	void SetDecorationAppearance(CMapObject* pMapObject, CPlayer* pPlayer, DWORD dwStreetStallKind);


    BYTE GetGenderFromMap(CPlayer* pPlayer);	//2007. 11. 29. CBH - �ʰ��� ���� ��ȯ �Լ�	

	//2008. 6. 18. CBH - +10 �̻� ������ ����Ʈ ����
	void SetPlusItemEffect(CPlayer* pPlayer, CEngineObject* pEngineObject, DWORD dwItemIdx, char* pObjectName);

	void SetExtraItemEffect(CPlayer* pPlayer, CEngineObject* pEngineObject, DWORD dwItemIdx, char* pObjectName);
	void SetPairItemEffect(CPlayer* pPlayer, CEngineObject* pEngineObject, DWORD dwItemIdx, char* pObjectName);


#ifndef _RESOURCE_WORK_NOT_APPLY_

	VOID ProcessAlphaAppearance(VOID);

	BOOL ReserveToAppearObject(CObject* pObject, void* pMsg);	
	VOID ProcessReservation(void);
	
	VOID CancelReservation(CObject* pObject);
	VOID CancelAlphaProcess(CObject* pObject);

	VOID EnbaleReservation(void)		{ m_bUseReservation = TRUE; }
	VOID DisableReservation(void)		{ m_bUseReservation = FALSE; }

	RESERVATION_ITEM_PLAYER*	AllocReservationItem(VOID);
	BOOL						FreeReservationItem(RESERVATION_ITEM_PLAYER* pItem);

#endif


};




EXTERNGLOBALTON(CAppearanceManager)


#ifndef _RESOURCE_WORK_NOT_APPLY_

DWORD ConfirmAddPlayer( CPlayer* pPlayer, SEND_CHARACTER_TOTALINFO* pmsg );
void PostProcessAddPlayer(CPlayer* pPlayer, SEND_CHARACTER_TOTALINFO* pMsg);

#endif




#endif // !defined(AFX_APPEARANCEMANAGER_H__6121D532_2201_469B_A298_35F0C2E66876__INCLUDED_)



