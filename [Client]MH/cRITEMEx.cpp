#include "stdafx.h"
#include "cRITEMEx.h"
#include "./Input/UserInput.h"
#include "./Interface/cWindowManager.h"
cRITEMEx::cRITEMEx(WORD num):RITEM(num) 
{
	memset(toolTipMsg,0,sizeof(toolTipMsg)); 
	cImage ToolTipImg;
	SCRIPTMGR->GetImage( 63, &ToolTipImg, PFT_HARDPATH );
	m_toolTip.Init( 0, TTTC_DEFAULT, &ToolTipImg, TTCLR_ITEM_CANEQUIP );
	m_toolTip.SetImageAlpha( TOOLTIP_ALPHA );
}
cRITEMEx::~cRITEMEx()
{
	m_toolTip.Release();
}
void cRITEMEx::AddToolTipLine(char* msg, DWORD color,cImage * m_LineImage,WORD m_FontIdx)
{
	m_toolTip.AddLine(msg,color,m_LineImage,m_FontIdx);
}
void cRITEMEx::Render()
{
	if(m_toolTip.IsValid())
	{
		int mx = g_UserInput.GetMouse()->GetMouseX();
		int my = g_UserInput.GetMouse()->GetMouseY();
		if( WINDOWMGR->IsMouseOverUsed() /*&& g_UserInput.GetKeyboard()->GetKeyPressed( KEY_SHIFT )*/)
		{
			m_toolTip.SetXY( mx+TOOLTIP_MOUSESIZE_X, my+TOOLTIP_MOUSESIZE_Y );
			WINDOWMGR->SetToolTipWindow(&m_toolTip );
		}
	}
}