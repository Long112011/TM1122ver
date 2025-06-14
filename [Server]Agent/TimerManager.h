#pragma once
#define TIMERMGR	USINGTON(TimerManager)
class TimerManager
{
private:
	//DWORD  m_CurTime;
public:
	TimerManager();
	~TimerManager();
	void Process();
	void SendMessageToClient();

	void SendMessageToClientItemLink(DWORD dwCharIdx);
};
EXTERNGLOBALTON(TimerManager);