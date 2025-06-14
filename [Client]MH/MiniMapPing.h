#include "stdafx.h"
#include "./Interface/cDialog.h"
#define MINIPINGMGR	USINGTON(CMiniMapPing)
enum{BAD=0,NOTBAD=1,OKLA=2,GOOD=3,DCLIAO=4};
class CMiniMapPing : public cDialog
{
	DWORD p_SendTime;
	DWORD p_RecvTime;
	DWORD p_LastTime;
	cImage m_SignalBar/*[5]*/;
	VECTOR2 mScale;
public:
	CMiniMapPing();
    virtual	~CMiniMapPing();
    void SendPingPacket();
	void SetPingShow(bool IsDisconnected = FALSE);
	void Process();	
};
EXTERNGLOBALTON(CMiniMapPing);