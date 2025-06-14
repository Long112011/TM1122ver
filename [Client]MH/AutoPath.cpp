#include "StdAfx.h"
#include "AutoPath.h"
#include "ObjectManager.h"
#include "MoveManager.h"
#include "aimanager.h"
//#include "./Input/UserInput.h"
#include "MiniMapDlg.h"
#include "GameIn.h"
#include "MHTimeManager.h"
#include "mhmap.h"
#define FIVEMETERDISTANCE (5 * 100)
GLOBALTON(AutoFindPathManager)
AutoFindPathManager::AutoFindPathManager()
{
	IsFind=FALSE;
}
AutoFindPathManager::~AutoFindPathManager()
{
	IsFind=FALSE;
}
void AutoFindPathManager::Process()
{		
	return;
	if(IsFind)
	{
		BOOL bNextFlag = false;
		if (HERO->GetState() == eObjectState_Engrave)
		{
			IsFind = false;
			return;
		}
		if (HERO->GetState() == eObjectState_Deal)
		{
			IsFind = false;
			return;
		}
		/*if (HERO->IsInSpecialState(eSpecialState_Stun))
		{
			IsFind = false;
			return;
		}*/
			
		/*if (HERO->GetState() != eObjectState_None &&
			HERO->GetState() != eObjectState_Move &&
			HERO->GetState() != eObjectState_TiedUp_CanMove &&
			HERO->GetState() != eObjectState_Immortal)
		{
			if (HERO->GetState() == eObjectState_Society)
			{
				IsFind = false;
				return;
			}
			else
			{
				IsFind = false;
				return;
			}
			IsFind = false;
			return;
		}*/
		if (MHTIMEMGR->CheckTimer400())
		{
			
			//----------------------------------------------------------------------------
			//----------------------------------------------------------------------------
			/*double distuncetmp = (
			(vHeroPos->x - vDestPos->x) * (vHeroPos->x - vDestPos->x)
			+ 		
			(vHeroPos->z - vDestPos->z) * (vHeroPos->z - vDestPos->z));*/


			//----------------------------------------------------------------------------
			//----------------------------------------------------------------------------
			//float dist = CalcDistanceXZ(&vPos, &HERO->GetPosition());
			//if (dist > FIVEMETERDISTANCE)

			//----------------------------------------------------------------------------
			//----------------------------------------------------------------------------
			//VECTOR3 vHeroPos;
			//HERO->GetPosition(&vHeroPos);
			//double distuncetmp = ((vHeroPos.x - vDestPos.x)*(vHeroPos.x - vDestPos.x) + (vHeroPos.z - vDestPos.z)*(vHeroPos.z - vDestPos.z));
			//if (sqrt(distuncetmp)> FIVEMETERDISTANCE)



			if (vPos.x != HERO->GetCurPosition().x || vPos.z != HERO->GetCurPosition().z)
			{
				if (MOVEMGR->IsMoving(HERO))
				{
					MOVEMGR->CalcPositionEx(HERO, gCurTime);
				}
				if (MAP->CollisionCheck_OneLine_New(&HERO->GetPosition(), &vPos) == TRUE)
				{
					MOVEMGR->SetHeroTarget(&vPos, 1.f);
					//HERO->Move_Simple(&vPos);
				//	HERO->m_WayPointManager->ChangeDestination(vPos);
				}
				//if (FALSE == HERO->GetBaseMoveInfo()->bMoving)
				//{
					//MOVEMGR->SetHeroTarget(vDestPos, FALSE);
					/*if (FALSE == MAP->PathFind(&HERO->GetPosition(), vDestPos, HERO->m_WayPointManager, TRUE))
					{
						return;
					}*/
					//MOVEMGR->CalcPositionEx(HERO, gCurTime);
					//HERO->m_WayPointManager->SetFlagDestination(vPos);
					//MOVEPOINT->SetPoint(&vPos);
					//MOVEMGR->SetHeroTarget(&vPos, 1.f);
					//HERO->Move_Simple(&vPos);
				//}
				//VECTOR3 vecDest = HERO->m_WayPointManager->GetFlagDestination();
				//HERO->m_WayPointManager->SetWMStatus(eWMStatus_Inac6tive);
				/*if (MAP->CollisionLine(&HERO->GetPosition(), &vDestPos, &vDestPos, MAP->GetMapNum(), HERO) == FALSE)
				{
					//return;
				}
				if (MOVEMGR->IsMoving(HERO))
				{
					//HERO->SetSkipKyungong(!true);
					MOVEMGR->CalcPositionEx(HERO, gCurTime);
				}*/

				/*if (FALSE == HERO->GetBaseMoveInfo()->bMoving)
				if (MAP->CollisionCheck_OneLine_New(&HERO->m_MoveInfo.CurPosition, vDestPos) == TRUE)
				{

					HERO->Move_Simple(vDestPos);
				}*/


				/*if (HERO->WayPoint_IsWayPointEmpty())
				//if (FALSE == HERO->GetBaseMoveInfo()->bMoving)
				{
					HERO->NextMove();
					MOVEMGR->SetHeroTarget(&vDestPos, 1.f);
				}*/
				bNextFlag = true;
			}
			if (sCPlayerAI.IsOpen())
			{
				bNextFlag = false;
			}
			if (false == bNextFlag)
			{
				IsFind = false;
			}
		}
	}
}