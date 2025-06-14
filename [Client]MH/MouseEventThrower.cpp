#include "stdafx.h"
#include "MouseEventThrower.h"
#include "MouseEventReceiver.h"
#include "Object.h"
#include "interface\cWindowManager.h"
#include "MousePointer.h"
#include "ObjectManager.h"
#include "gamein.h"
#include "MouseCursor.h"
#include "MHMap.h"
#include "MonsterGuageDlg.h"
#include "Reconnect.h"
#include "AutoPath.h"
#include "MoveManager.h"
#include "Input/UserInput.h"
CMouseEventThrower::CMouseEventThrower()
{
	m_pCurrentReceiver = NULL;
	m_pDefaultReceiver = NULL;
	m_bCancelSelect = FALSE;
	m_bAllowMouseClick = TRUE;
}
CMouseEventThrower::~CMouseEventThrower()
{
}
void CMouseEventThrower::SetDefaultReceiver(CMouseEventReceiver* pDefaultReceiver)
{
	m_pDefaultReceiver = pDefaultReceiver;
}
void CMouseEventThrower::SetCurrentReceiver(CMouseEventReceiver* pCurrentReceiver)
{
	m_pCurrentReceiver = pCurrentReceiver;
}
void CMouseEventThrower::SetReceiverToDefault()
{
	ASSERT(m_pDefaultReceiver);
	m_pCurrentReceiver = m_pDefaultReceiver;
}
#define MOUSE_MOVE_TICK		400
#define MOUSE_PRESS_TICK	400
#define MOUSE_ON_TICK		200
void CMouseEventThrower::Process(CMouse* Mouse)
{
	static DWORD dwMouseMoveTick = 0;
	static DWORD dwMouseOnTick = 0;
	static DWORD dwMousePressTick = 0;
	static DWORD previousMouseOverObjectIndex = 0;
	CObject* const previousMouseOverObject = OBJECTMGR->GetObject(
		previousMouseOverObjectIndex);
	if (!m_bAllowMouseClick)
		return;
	if (WINDOWMGR->IsMouseInputProcessed() || WINDOWMGR->IsDragWindow())
	{
		if (previousMouseOverObject)
		{
			m_pCurrentReceiver->OnMouseLeave(previousMouseOverObject);
			previousMouseOverObjectIndex = 0;
		}
		dwMousePressTick = gCurTime;
		return;
	}
	DWORD MouseEvent = MOUSEEVENT_NONE;
	if (Mouse->LButtonDoubleClick())
		MouseEvent = MOUSEEVENT_DBLCLICK;
	else
	{
		if (Mouse->LButtonDown())
		{
			MouseEvent = MOUSEEVENT_LCLICK;
		}
		else if (Mouse->RButtonDown())
		{
			MouseEvent = MOUSEEVENT_RCLICK;
		}
		else if (Mouse->RButtonUp())
			MouseEvent = MOUSEEVENT_RBUTTONUP;
		if (Mouse->LButtonPressed())
			MouseEvent |= MOUSEEVENT_LPRESS;
	}
	if (Mouse->RButtonDown())	m_bCancelSelect = TRUE;
	if (Mouse->RButtonDrag())	m_bCancelSelect = FALSE;
	if (OBJECTMGR->GetSelectedObjectID() && Mouse->RButtonUp() && m_bCancelSelect)
	{
		OBJECTMGR->SetSelectedObject(NULL);
		m_bCancelSelect = FALSE;
	}
	CObject* pOverObject = NULL;
	if (!KEYBOARD->GetKeyPressed(KEY_MENU))
	{
		int nMouseX, nMouseY;
		if (MouseEvent == MOUSEEVENT_NONE)
		{
			nMouseX = Mouse->GetMouseX();
			nMouseY = Mouse->GetMouseY();
		}
		else
		{
			nMouseX = Mouse->GetMouseEventX();
			nMouseY = Mouse->GetMouseEventY();
		}
		pOverObject = (CObject*)GetSelectedObject(nMouseX, nMouseY);
		if (pOverObject && pOverObject->GetObjectKind() == eObjectKind_BossMonster)
		{
			pOverObject = (CObject*)GetSelectedObject(nMouseX, nMouseY);
		}
	}
	if (MouseEvent == MOUSEEVENT_NONE)
	{
		if (gCurTime - dwMouseOnTick > MOUSE_ON_TICK)
		{
			dwMouseOnTick = gCurTime;
			if (previousMouseOverObject != pOverObject)
			{
				m_pCurrentReceiver->OnMouseLeave(previousMouseOverObject);
				m_pCurrentReceiver->OnMouseOver(pOverObject);
				previousMouseOverObjectIndex = (pOverObject ? pOverObject->GetID() : 0);
			}
			if (pOverObject == NULL)
			{
				CURSOR->SetCursor(eCURSOR_DEFAULT);
			}
		}
	}
	else
	{
		if (pOverObject)
		{
			switch (pOverObject->GetEngineObjectType())
			{
			case eEngineObjectType_Character:
			{
				m_pCurrentReceiver->OnClickPlayer(MouseEvent, (CPlayer*)pOverObject);
				break;
			}
			case eEngineObjectType_Monster:
			{
				m_pCurrentReceiver->OnClickMonster(MouseEvent, (CMonster*)pOverObject);
				break;
			}
			case eEngineObjectType_Npc:
			{
				m_pCurrentReceiver->OnClickNpc(MouseEvent, (CNpc*)pOverObject);
				break;
			}
			case eEngineObjectType_SkillObject:
			{
				m_pCurrentReceiver->OnClickSkillObject(MouseEvent, (CSkillObject*)pOverObject);
				break;
			}
			case eEngineObjectType_Pet:
				m_pCurrentReceiver->OnClickPet(MouseEvent, (CPetBase*)pOverObject);
				break;
			case eEngineObjectType_Effect:
			{
				break;
			}
			default:
			{
				ASSERT(0);
				break;
			}
			}
			if (MouseEvent & MOUSEEVENT_DBLCLICK)
			{
				if (pOverObject->GetEngineObjectType() == eEngineObjectType_Character)
				{
					m_pCurrentReceiver->OnDbClickPlayer(MouseEvent, (CPlayer*)pOverObject);
				}
				else if (pOverObject->GetEngineObjectType() == eEngineObjectType_Monster)
				{
					m_pCurrentReceiver->OnDbClickMonster(MOUSEEVENT_LCLICK, (CMonster*)pOverObject);
				}
			}
			if (MouseEvent & MOUSEEVENT_LCLICK && pOverObject != HERO)
			{
				int ObjectType = pOverObject->GetEngineObjectType();
				if (ObjectType == eEngineObjectType_Monster)
				{
					pOverObject->ShowObjectName(TRUE, RGB_HALF(255, 141, 39));
				}
				else
				{
					pOverObject->ShowObjectName(TRUE, NAMECOLOR_SELECTED);
				}
				OBJECTMGR->SetSelectedObject(pOverObject);
			}
		}
		else
		{
			if (MouseEvent & MOUSEEVENT_LCLICK)
			{
				if (gCurTime - dwMouseMoveTick >= MOUSE_MOVE_TICK)
				{
					dwMouseMoveTick = gCurTime;
					dwMousePressTick = gCurTime;
					VECTOR3* TargetPos = GetPickedPosition(Mouse->GetMouseEventX(), Mouse->GetMouseEventY());
					if (!TargetPos)
						return;
					m_pCurrentReceiver->OnClickGround(MouseEvent, TargetPos);
					if (HERO)
					{
						//AUTOPATH->SetFindPath(FALSE);//kiv
						AUTOPATH->SetTargetPos(HERO->GetCurPosition());
						HERO->SetFollowPlayer(0);
					}
				}
			}
			else if (MouseEvent & MOUSEEVENT_LPRESS)
			{
				if (gCurTime - dwMousePressTick >= MOUSE_PRESS_TICK)
				{
					dwMousePressTick = gCurTime;
					VECTOR3* TargetPos = GetPickedPosition(Mouse->GetMouseX(), Mouse->GetMouseY());
					if (!TargetPos)
						return;
					m_pCurrentReceiver->OnClickGround(MouseEvent, TargetPos);
					if (HERO)
					{
						//AUTOPATH->SetFindPath(FALSE);//kiv
						AUTOPATH->SetTargetPos(HERO->GetCurPosition());
						HERO->SetFollowPlayer(0);
					}
				}
			}
		}
	}
}
#define FOLLOW_TICK 500
void CMouseEventThrower::Follow()
{
	static DWORD dwLastFollowTick = 0;
	if (HERO)
	if (HERO->GetFollowPlayer())
	{
		if (gCurTime - dwLastFollowTick >= FOLLOW_TICK)
		{
			dwLastFollowTick = gCurTime;
			CObject* pObject = OBJECTMGR->GetObject(HERO->GetFollowPlayer());
			if (pObject)
			{
				if (pObject->GetObjectKind() == eObjectKind_Player)
				{
					VECTOR3 vMyPos, vTargetPos;
					HERO->GetPosition(&vMyPos);
					pObject->GetPosition(&vTargetPos);
					float dist = CalcDistanceXZ(&vMyPos, &vTargetPos);
					if (dist > 200.0f)
					{
						vTargetPos = (vTargetPos - vMyPos) * (dist - 100.0f) / dist + vMyPos;
						MOVEMGR->SetHeroTarget(&vTargetPos, 1.f);
					}
				}
			}
			else
			{
				return;
			}
		}
	}
}