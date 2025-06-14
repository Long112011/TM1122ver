#pragma once
#include "../[cc]Header/CommonStruct.h"
//#include "message.h"

#define TRIGGERMGR (&Trigger::CManager::GetInstance())

enum EObjectKind;
class CPlayer;

namespace Trigger
{
	class CTrigger;

	class CTimer;

	
	
	class CManager
	{
	public:
		static CManager& GetInstance();

		void Release();
		void Process();
	

		void StartTimer(DWORD alias, DWORD dwChannel, eTimerType type, DWORD dwDuration);
		BOOL SetTimerState(DWORD alias, eTimerState state);
		CTimer* GetTimer(DWORD alias);		
	private:
		
		typedef stdext::hash_map< DWORD, CTimer* > TimerMap;
		TimerMap mTimerMap;	
	};
}
