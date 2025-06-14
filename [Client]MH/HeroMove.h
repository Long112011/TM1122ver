




// HeroMove.h: interface for the CHeroMove class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HEROMOVE_H__AD3FC9BB_1B15_4CD4_B83B_6641CCE27C86__INCLUDED_)
#define AFX_HEROMOVE_H__AD3FC9BB_1B15_4CD4_B83B_6641CCE27C86__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MouseEventReceiver.h"

class CHeroMove : public CMouseEventReceiver  
{
	void OnAttack(CObject* pTargetObj,DWORD MouseEvent);

// LBS ³ëÁ¡»ó °ü·Ã 03.09.30
	void OnEnterStreetStall( CObject* pTargetObj );
//

public:
	CHeroMove();

	virtual ~CHeroMove();


	void OnMouseOver( CObject* pObject );
	void OnMouseLeave( CObject* pObject );
	void OnClickPlayer(DWORD MouseEvent,CPlayer* pPlayer);

	void OnClickMonster(DWORD MouseEvent,CMonster* pMonster);
	void OnClickNpc(DWORD MouseEvent,CNpc* pNpc);
	void OnClickGround(DWORD MouseEvent,VECTOR3* pPos);
	void OnClickSkillObject(DWORD MouseEvent,CSkillObject* pObj);

	void OnClickPet(DWORD MouseEvent, CPetBase* pPet);
	void OnClickMapObject(DWORD MouseEvent, CObject* pObj);
	
	void OnDbClickPlayer(DWORD MouseEvent,CPlayer* pPlayer);
	void OnDbClickMonster(DWORD MouseEvent,CMonster* pMonster);
	void OnDbClickNpc(DWORD MouseEvent, CNpc* pNpc);
	
	void AttackSelectedObject();
	

	void SelectLoginPoint( WORD LoginPointIndex );
	
	// magi82(35) MapChange State
	// ¸ÊÀÌµ¿ ÇÒ¶§ ¾î¶² »óÅÂÀÇ ¸ÊÀÌµ¿ÀÎÁö Ã¼Å©ÇÏ°Ô ¼öÁ¤
	void MapChange( WORD MapIndex, DWORD ItemIdx=0, DWORD ItemPos=0, DWORD state = eMapChange_General ,BYTE bFlagType=0);
	void MapPosition(CObject*pObject,VECTOR3*vSavedPos);

	//weiye ÊµÏÖÓÒ¼üË«»÷µ¯³ö½ÇÉ«¹¦ÄÜÃæ°å 2018-03-24
	//void OpenCharacterControlDlg(CMouse* Mouse,CObject* pObject);
};



#endif // !defined(AFX_HEROMOVE_H__AD3FC9BB_1B15_4CD4_B83B_6641CCE27C86__INCLUDED_)


