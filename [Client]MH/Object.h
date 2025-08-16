



// Object.h: interface for the CObject class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OBJECT_H__A8AFB488_5BB5_45E5_8482_EE4EE2A55DDD__INCLUDED_)
#define AFX_OBJECT_H__A8AFB488_5BB5_45E5_8482_EE4EE2A55DDD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./Engine/EngineObject.h"
#include "./Engine/EngineEffect.h"
#include "Angle.h"
#include "EFFECT/EffectManager.h"

#include "Status.h"
#include "autopath.h"


class CStunState;
class CMOTIONDESC;
class CSkillObject;
class CStatus;
class CObjectBalloon;



struct ROTATEINFO
{
	BOOL bRotating;
	CAngle Angle;
	DWORD Rotate_StartTime;
	float EstimatedRotateTime;
};

#define NAMECOLOR_WHITE		RGB_HALF( 255, 255, 255 )
#define NAMECOLOR_DEFAULT	RGB_HALF( 230, 230, 230 )
#define NAMECOLOR_SELECTED	RGB_HALF( 255, 255, 0 )
#define NAMECOLOR_MONSTER	NAMECOLOR_DEFAULT
#define NAMECOLOR_PLAYER	NAMECOLOR_DEFAULT
#define NAMECOLOR_NPC		RGB_HALF( 255, 214, 0 )

#define NAMECOLOR_PARTY		RGB_HALF( 157, 204, 58 )
#define NAMECOLOR_MUNPA		RGB_HALF( 126, 156, 180 )

#define NAMECOLOR_PKMODE	RGB_HALF( 255, 0, 0 )
#define NAMECOLOR_WANTED	RGB_HALF( 234, 0, 255 )


#define NAMECOLOR_GM		RGB_HALF( 28, 233, 151 )

//2007. 10. 31. CBH - Å¸ÀÌÅº ¸ó½ºÅÍ ÀÌ¸§ »ö»ó
#define NAMECOLOR_TITANMONSTER			RGB_HALF( 230, 190, 130 )
#define NAMECOLOR_TITANMONSTER_SELECT	RGB_HALF( 237, 27, 35 )

struct OBJECTEFFECTDESC
{

	OBJECTEFFECTDESC()		{	SetDesc(0);	}
	OBJECTEFFECTDESC(WORD EffectNum,DWORD dwFlag = EFFECT_FLAG_NORMAL,VECTOR3* pPos = 0)
	{	SetDesc(EffectNum,dwFlag,pPos);	}
	WORD Effect;
	DWORD Flag;
	VECTOR3 Position;
	void SetDesc(WORD EffectNum,DWORD dwFlag = EFFECT_FLAG_NORMAL,VECTOR3* pPos = 0)
	{
		Effect = EffectNum;
		Flag = dwFlag;
		if(pPos)
			Position = *pPos;
		else
			Position.x = Position.y = Position.z = 0;
	}
};

enum eSpecialState
{
	eSpecialState_Stun,
	eSpecialState_AmplifiedPowerPhy,
	eSpecialState_AmplifiedPowerAtt,

	eSpecialState_Max
};
class CSpecialState;

class CObject : public CObjectBase
{
	CSpecialState* m_pSpecialState[eSpecialState_Max];

	int m_bMoveSkipCount;
//	HEFFPROC m_ShadowEff;	//protected·Î ¿Å±è KES
	CYHHashTable<void> m_StateEffectList;
	BOOL m_bIsYMoving;

protected:
	CObject();
	virtual ~CObject();

	DWORD m_DiedTime;
	CEngineEffect m_ShadowObj;
	BOOL m_bInited;
	BOOL m_bSelected;

	BYTE m_ObjectKind;

	BASEOBJECT_INFO m_BaseObjectInfo;
	MOVE_INFO m_MoveInfo;
	ROTATEINFO m_RotateInfo;
	
	STATE_INFO	m_ObjectState;

	CMOTIONDESC	* m_pMotionDESC;			// ¸ð¼Ç Á¤º¸  : taiyo
	CEngineObject m_EngineObject;
	
	BOOL m_bDieFlag;
    // marryballon define 2014-05-09
	CObjectBalloon * m_pObjectBalloon;

	BYTE m_bObjectBalloonBits;

//	void OnMouseLeave();
//	void OnMouseOver();
	// overInfo °ü·Ã --------------------------------------------------------------

	cPtrList m_StatusList;

	friend class CAppearanceManager;
	friend class CMoveManager;
	friend class CMHCamera;
	friend class CObjectStateManager;
	friend class CObjectActionManager;
	friend class CMotionManager;
	friend class AutoFindPathManager;
	

	virtual BOOL Init(EObjectKind kind,BASEOBJECT_INFO* pBaseObjectInfo);
	void InitMove(BASEMOVE_INFO* pMoveInfo);
	virtual void Release();
	int	GetMotionIDX(int mainMotion, int subMotion = 0);
public:
	// overInfo °ü·Ã --------------------------------------------------------------
	void InitObjectBalloon(BYTE bitFlag);
	void SetObjectBalloonTall(LONG Tall);
//	void AddChatBalloon(char * chatMsg);
	
	void SetOverInfoOption( DWORD dwOption );
	void SetGuildMark();
	void SetNickName();
	void SetFortName(char* name);
	void SetName(char* name);
	void ShowObjectName( BOOL bShow, DWORD dwColor = NAMECOLOR_DEFAULT );
	void ShowChatBalloon( BOOL bShow, char* chatMsg, DWORD dwColor = RGB_HALF(70,70,70), DWORD dwAliveTime = 5000 );
	// overInfo °ü·Ã --------------------------------------------------------------

	//SW050913 ¼öÁ¤
	void AddObjectEffect(DWORD KeyValue,OBJECTEFFECTDESC* EffectNumArray,WORD NumOfEffect,CObject* pSkillOperator=NULL);
	void RemoveObjectEffect(DWORD KeyValue);
	void RemoveAllObjectEffect();
	
//	virtual void InitObjectOverInfoEx();
	BYTE GetBattleTeam()						{	return m_BaseObjectInfo.BattleTeam;	}
	void SetBattleTeam( DWORD BattleTeam )		{	m_BaseObjectInfo.BattleTeam = (BYTE)BattleTeam;	}
	DWORD GetBattleID()				{	return m_BaseObjectInfo.BattleID;	}
	void SetBattle(DWORD BattleID,BYTE Team);


	inline BYTE GetObjectKind()						{	return m_ObjectKind;	}
	inline void SetObjectKind(EObjectKind kind)		{	m_ObjectKind = kind;	}
	
	inline char* GetObjectName()	{	return m_BaseObjectInfo.ObjectName;		}

	inline DWORD GetID()	{	return m_BaseObjectInfo.dwObjectID;	}


	void GetBaseObjectInfo(BASEOBJECT_INFO* pRtInfo);
	void GetBaseMoveInfo(BASEMOVE_INFO* pRtInfo);
	MOVE_INFO* GetBaseMoveInfo() {	return &m_MoveInfo;	}

	virtual void Process();

	inline CEngineObject* GetEngineObject()	{	return &m_EngineObject;		}


	friend class CObjectManager;

	inline BOOL IsInited()		{	return m_bInited;	}
	VECTOR3& GetCurPosition() { return m_MoveInfo.CurPosition; }
	float GetAngleDeg()		 { return m_RotateInfo.Angle.ToDeg();	}	

	inline BOOL IsDied()			{	return m_bDieFlag;	}
	inline DWORD GetDiedTime()			{	return m_DiedTime;	}
	void SetDieFlag();
	void SetFlag(BOOL val);
    // set marryname 2014-05-09
	void SetMarryNameBalloon(char* name);

	void SetShiTuNameBalloon(char* name);


	void SetImageNameBalloon(WORD ImageName);

	void SetFameBalloon(char* fame);
	void SetStageLogoBalloon(char* fame);

	void SetTopListFlg(WORD val);
	void InitTopListFlg();

	void SetFameRankBalloon(char* fame);


	void SetKillCountBalloon(char* kill);
	//VIPÍ¼±ê
	void SetObjectVIPImage(int val);
	//ÉèÖÃÉÁÃû
	void SetObjectFlgName(WORD Flg);

	void SetObjectCustomizingName(char* name);

//protected:
	// »óÅÂ ÇÔ¼öµé

	virtual void SetMotionInState(BYTE State)	{};

	virtual BOOL StartSocietyAct( int nStartMotion, int nIngMotion = -1, int nEndMotion = -1, BOOL bHideWeapon = FALSE )	{ return FALSE; }
	virtual BOOL EndSocietyAct()	{ return TRUE; }

protected:	
	void SetState(BYTE State);
public:

	BYTE GetState();
	DWORD GetStateStartTime()	{	return m_ObjectState.State_Start_Time;		}

	// Çàµ¿ ÇÔ¼öµé...
	virtual void Die(CObject* pAttacker,BOOL bFatalDamage,BOOL bCritical, BOOL bDecisive);
	//virtual void Damage(CObject* pAttacker,BYTE DamageKind,DWORD Damage,DWORD ShieldDamage,BOOL bCritical, BOOL bDecisive) {}
	//SW070127 Å¸ÀÌÅº
	virtual void Damage(CObject* pAttacker,BYTE DamageKind,DWORD Damage,DWORD ShieldDamage,BOOL bCritical, BOOL bDecisive, DWORD titanObserbDamage) {}
	virtual void Heal(CObject* pHealer,BYTE HealKind,DWORD HealVal) {}
	virtual void Recharge(CObject* pHealer,BYTE RechargeKind,DWORD RechargeVal) {}	
	virtual void Revive(VECTOR3* pRevivePos);


	//////////////////////////////////////////////////////////////////////////
	// ObjectStateManager¿¡¼­ StateÀÇ ½ÃÀÛ°ú ³¡¿¡¼­ È£ÃâÇØÁÖ´Â ÇÔ¼öµé
	virtual void OnStartObjectState(BYTE State)	{}
	virtual void OnEndObjectState(BYTE State)	{}
	//////////////////////////////////////////////////////////////////////////

	// ¼öÄ¡ 
	virtual void SetLife(DWORD life, BYTE type = 1){}
	virtual DWORD GetLife(){ return 0; }
	virtual void SetShield(DWORD Shield, BYTE type = 1){}
	virtual DWORD GetShield(){ return 0; }
	virtual float GetWeight()		{	return 1.f;	}
	virtual float GetRadius()		{	return 0.f;	}
	virtual DWORD GetNaeRyuk()		{	return 0; }
	virtual void SetNaeRyuk(DWORD val, BYTE type = 1)		{}

	
//////////////////////////////////////////////////////////////////////////
// 
#define GET_STATUS(var_type,func)						\
var_type	func ## ()								\
{														\
	var_type Ori = Do ## func();						\
	var_type Up = 0,Down = 0;							\
	PTRLISTSEARCHSTART(m_StatusList,CStatus*,pSL)		\
		pSL->func(Ori,Up,Down);							\
	PTRLISTSEARCHEND;									\
	return STATUSCALC(Ori,Up,Down);						\
};														\
virtual var_type	Do ## func ## ()	{	return 0; }		
//	
	GET_STATUS(DWORD,GetMaxLife);
	GET_STATUS(DWORD,GetMaxShield);
	GET_STATUS(DWORD,GetMaxNaeRyuk);
	GET_STATUS(DWORD,GetPhyDefense);
	GET_STATUS(DWORD,GetPhyAttackPowerMin);
	GET_STATUS(DWORD,GetPhyAttackPowerMax);
	GET_STATUS(DWORD,GetCritical);
	GET_STATUS(DWORD,GetDecisive);
	GET_STATUS(float,GetMoveSpeed);
	GET_STATUS(float,GetAddAttackRange);
	

	float GetAttDefense(WORD Attrib);	
	virtual float DoGetAttDefense(WORD Attrib)	{return 0;}
//////////////////////////////////////////////////////////////////////////
	
	virtual void SetPosition(VECTOR3* pPos);
	virtual void GetPosition(VECTOR3* pPos);
	virtual VECTOR3 GetPosition(void);
	virtual void SetAngle(float AngleRad);
	virtual float GetAngle();
	virtual DIRINDEX GetDirectionIndex();	

	virtual void Stop(VECTOR3* p_pStopPos = NULL);


	virtual void ChangeMotion(int motion,BOOL bRoop);
	virtual void ChangeBaseMotion(int motion);
	

	//////////////////////////////////////////////////////////////////////////
	// skillManager¿¡¼­ ¾²´Â ÇÔ¼öµé
	virtual void AddStatus(CStatus* pStatus);
	virtual void RemoveStatus(CStatus* pStatus);
	virtual cPtrList& GetSkillStatusList(){ return m_StatusList ;}


	//////////////////////////////////////////////////////////////////////////
	// ObjectManager¿¡¼­ ¾²´Â ÇÔ¼öµé
	void OnSelected();
	void OnDeselected();

	
	//////////////////////////////////////////////////////////////////////////
	// Special State
	void StartSpecialState(DWORD SpecialStateKind,DWORD Time,
							WORD wParam1,WORD wParam2,float fParam3,
							WORD EffectNum,WORD StateIcon,BOOL bHeroOper = FALSE);	//HERO °ü·ÃµÈ »óÅÂÀÌÆåÆ® º¸¿©ÁÖ±â
	void EndSpecialState(DWORD SpecialStateKind);
	BOOL IsInSpecialState(DWORD SpecialStateKind);
	void SpecialStateProcess(DWORD TickTime);	
	WORD GetSpecialStateParam1(DWORD SpecialStateKind);
	WORD GetSpecialStateParam2(DWORD SpecialStateKind);
	//////////////////////////////////////////////////////////////////////////

	// Quest Npc Mark
	BOOL	IsNpcMark( DWORD dwValue );
	
	void	SetKyungGongLevel( WORD wLevel );

	//////////////////////////////////////////////////////////////////////////
	// 06. 06. 2Â÷ ÀüÁ÷ - ÀÌ¿µÁØ
	// Àº½Å/Çý¾È
	// Æ¯¼ö»óÅÂº¯È­
	// ±âÁ¸ÀÇ°Í°ú Àû¿ë¹æ½ÄÀÌ ¸¹ÀÌ ´Þ¶ó »õ·Î ¸¸µë
private:
	DWORD	m_SingleSpecialStateUsedTime[eSingleSpecialState_Max];	// ¸¶Áö¸· »ç¿ëÇÑ ½Ã°£

public:
	void SetSingleSpecialState(WORD State, BOOL bVal) { m_BaseObjectInfo.SingleSpecialState[State] = bVal ? true : false; }
	BOOL GetSingleSpecialState(WORD State) { return m_BaseObjectInfo.SingleSpecialState[State]; }

	void SetSingleSpecialStateUsedTime(WORD State, DWORD Time) 
	{ 
		m_SingleSpecialStateUsedTime[State] = Time; 
	}

	DWORD GetSingleSpecialStateUsedTime(WORD State) 
	{ 
		return m_SingleSpecialStateUsedTime[State]; 
	}
	//////////////////////////////////////////////////////////////////////////
};

#endif // !defined(AFX_OBJECT_H__A8AFB488_5BB5_45E5_8482_EE4EE2A55DDD__INCLUDED_)


