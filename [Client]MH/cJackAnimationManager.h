#pragma once
#include "cLinkedList.h"
class cWindow;
class cDialog;
enum JESHAKETYPE
{
	JSHAKE_NORMAL_TYPE,
	JSHAKE_NORMAL2_TYPE,
	JSHAKE_TYPE_MAX,
};
class JANISHAKEVECTOR
{
public:
	VECTOR2 varVector;
	DWORD	Duration;
};
class JANISHAKEINFO
{
public:
	void SetParam( JANISHAKEVECTOR  v [], DWORD max )
	{
		pV = v; nN = max;
	}
	JANISHAKEVECTOR *	pV;
	DWORD				nN;
};
JANISHAKEVECTOR JnormallShake[];
enum { JANI_FADE_IN, JANI_FADE_OUT };
#define JACKFADEMGR	USINGTON(cJackAnimationManager)
class cJackAnimationManager  
{
	static WORD MAG_GAB;
public:
	cJackAnimationManager();
	virtual ~cJackAnimationManager();
	void Init();
	static BOOL MagProcess(RECT * dockingBarRect, cWindow* window, VECTOR2 * outWindowPos);
	void StartShake(JESHAKETYPE shakeType, cWindow * window);
	void ShakeProcess();
	static JANISHAKEINFO m_SHAKEMODE[JSHAKE_TYPE_MAX];
	void StartFade(BYTE fadeType, short fadeStart, short fadeEnd, DWORD fadeTime, cDialog * pWnd);
	void FadeProcess();
	void EndFadeAll();
protected:
	void RollBack();
	BOOL FindFadeNode(cDialog * window);
	JESHAKETYPE		m_shakeType;
	DWORD			m_dwLastShakingPosChangedTime;
	LONG			m_OrgX;
	LONG			m_OrgY;
	WORD			m_CurIdx;
	BOOL			m_bShaking;
	cWindow *		m_pWindow;
	class JFADE_INFO
	{
	public:
		BYTE fadeType;
		short fadeStartValue;
		short fadeEndValude;
		DWORD fadeTime;
		DWORD fadeStartTime;
		cDialog * pWnd;
		BOOL bEnd;
	};
	cLinkedList<JFADE_INFO> m_FadeList;
};
EXTERNGLOBALTON(cJackAnimationManager);