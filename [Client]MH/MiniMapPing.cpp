#include "stdafx.h"
#include "MiniMapPing.h"
#include <MMSystem.h>
#include "WindowIDEnum.h"
#include "./Interface/cWindowManager.h"
#include "./Interface/cScriptManager.h"
int	PingMs;
GLOBALTON(CMiniMapPing);
CMiniMapPing::CMiniMapPing()
{
	p_SendTime=0;
    p_RecvTime=0;
	p_LastTime=0;

	mScale.x = mScale.y = 0.35;
}
CMiniMapPing::~CMiniMapPing()
{	
}
void CMiniMapPing::SendPingPacket()
{
	MSG_DWORD		msg;
	msg.Category	= MP_CHAR;
	msg.Protocol	= MP_CHAR_PING_SYN;
	msg.dwObjectID	= HEROID;
	msg.dwData		= 1234;
	NETWORK->		Send( &msg, sizeof(msg) );
}
void CMiniMapPing::Process()
{
	DWORD Currtime=timeGetTime();
	if(Currtime-p_LastTime>15000/*36000*/)
	{
		SendPingPacket();
		p_SendTime=timeGetTime();
        p_LastTime=timeGetTime();
	}
}
void CMiniMapPing::SetPingShow(bool IsDisconnected)
{
	cDialog * m_pDlg = WINDOWMGR->GetWindowForID(MNM_DIALOG);
	if(m_pDlg)
	{
		cStatic * m_pTitle = (cStatic*)m_pDlg->GetWindowForID(MNM_MAPPING);
		if(m_pTitle)
		{
			char	TimerBuffer[64];
			DWORD	PingColor = 0x000000;
			BYTE	PingState = -1;

			if (IsDisconnected)
			{
				PingColor = RGBA_MAKE(255, 0, 0, 255);
				PingState = DCLIAO;

				sprintf(TimerBuffer, " \n\n loss");
			}
			else
			{
				p_RecvTime = timeGetTime();
				PingMs = (p_RecvTime - p_SendTime);

				sprintf(TimerBuffer, " \n %d ms", PingMs);

				if (PingMs <= 70)
				{
					PingColor = RGBA_MAKE(0, 255, 0, 255);
					PingState = GOOD;
				}
				if (PingMs>70 && PingMs <= 100)
				{
					PingColor = RGBA_MAKE(255, 215, 0, 255);
					PingState = OKLA;
				}
				if (PingMs>100 && PingMs <= 170)
				{
					PingColor = RGBA_MAKE(255, 140, 0, 255);
					PingState = NOTBAD;
				}
				if (PingMs>170)
				{
					PingColor = RGBA_MAKE(255, 69, 0, 255);
					PingState = BAD;
				}
				p_SendTime = 0;
				p_RecvTime = 0;
			}
			SCRIPTMGR->GetImage(137 + PingState, &m_SignalBar/*[PingState]*/, PFT_JACKPATH);
			m_pTitle->SetScale(&mScale);
			m_pTitle->SetBasicImage(&m_SignalBar/*[PingState]*/);
			m_pTitle->SetFGColor(PingColor);
			m_pTitle->SetStaticText(TimerBuffer);
		}
	}
}