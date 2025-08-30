#pragma once
#include "MacroManager.h"
#define MOVEMGR USINGTON(CMoveManager)
class CObject;
class CAction;
class CActionTarget;
class CPet;
#ifdef  _MUTIPET_
class CPet;//독며  3pet
#endif //  _MUTIPET_
class CMoveManager  
{
	BOOL CalcYPosition(CObject* pObject,DWORD CurTime);
	void CalcAngle(CObject* pObject,DWORD CurTime);
	BOOL m_bSetMove;//
	BYTE m_bMoveCount;//
public:
	CMoveManager();
	virtual ~CMoveManager();
	BOOL CalcPositionEx(CObject* pObject,DWORD CurTime);
	void InitMove(CObject* pObject,BASEMOVE_INFO* pMoveInfo,float AngleDeg = 0);
	void SetPosition(CObject* pObject,VECTOR3* pPos);	
	void EndMove(CObject* pObject);
	void EndYMove(CObject* pObject);	
	void StartMoveEx(CObject* pObject, DWORD CurTime, VECTOR3 * pTargetPos);
	void StartMoveEx(CObject* pObject, VECTOR3* pStartPos,DWORD CurTime);
	int TargetUpdate(CObject* pObject, MOVE_INFO * pMoveInfo, VECTOR3 * TargetPos);
	void SetLookatPos(CObject* pObject, VECTOR3* pPos,float TurnDuration, DWORD CurTime, int AddDeg = 0);
	void SetAngle(CObject* pObject,float AngleDeg,float TurnDuration);
	void MoveProcess(CObject* pObject);
	//BOOL SetHeroTarget(VECTOR3* pPos,BOOL bMousePointDisplay = MACROMGR->IsVisibleCursor());
	BOOL SetHeroTarget(VECTOR3* pPos,float biubiubiu); //
	void SetHeroTarget(VECTOR3* pPos,BOOL bMousePointDisplay = MACROMGR->IsVisibleCursor()); //
#ifdef  _MUTIPET_
	void SetPetTarget(CPet* pPet, MOVE_INFO* pHeroMoveInfo, WORD wPosCount);//독며  3pet
#else
	void SetPetTarget(MOVE_INFO* pHeroMoveInfo, WORD wPosCount);
#endif //  _MUTIPET_
	void GetPetPath(MOVE_INFO* pHeroMoveInfo, WORD wPosCount);
	BOOL GetPetLastPos(VECTOR3* pSrcPos, VECTOR3* pFromPos, VECTOR3* pToPos);
#ifdef  _MUTIPET_
	void SendPetMoveMsg(CPet* pPet);//독며  3pet
#else
	void SendPetMoveMsg();
#endif //  _MUTIPET_
	void SetHeroPetTarget(CObject* object1, CObject* object2, VECTOR3* pPos);
	void SetHeroPetTarget();
	void SetHeroPetTarget(VECTOR3* pPos);
	void SetHeroActionMove(CActionTarget* pTarget,CAction* pAction);
	void MoveStop(CObject* pObject,VECTOR3* pStopPos = NULL);
	void HeroMoveStop();
#ifdef  _MUTIPET_
	void PetMoveStop(CPet* pPet);//독며  3pet
#else
	void PetMoveStop();
#endif //  _MUTIPET_
	float CalcDistance(CObject* pObject1,CObject* pObject2);
	void ToggleHeroMoveMode();
	void ToggleHeroKyungGongMode();
	void SetWalkMode(CObject* pObject);
	void SetRunMode(CObject* pObject);
	BOOL SetKyungGong(CObject* pObject,WORD KGIdx);
	BOOL SetMonsterMoveType(CObject* pObject,WORD KGIdx);
	BOOL IsValidKyungGong(CObject* pObject,WORD KGIdx);
	float GetKyungGongSpeed(WORD KyungGongIdx);
	void NetworkMsgParse(BYTE Protocol,void* pMsg);
	void EffectMove(CObject* pObject,VECTOR3* TargetPos,DWORD Duration);
	void EffectYMove(CObject* pObject,float StartY,float EndY,DWORD Duration);
	void HeroEffectMove(CObject* pObject,VECTOR3* TargetPos,DWORD Duration,CObject* pOperator);
	void EffectMoveInterpolation(CObject* pObject,VECTOR3* TargetPos);
	BYTE GetMoveMode(CObject* pObject);
	BOOL IsMoving(CObject* pObject);
	void RefreshHeroMove();
	void SetFindPath(){ m_bSetMove = FALSE; }//
};
EXTERNGLOBALTON(CMoveManager)