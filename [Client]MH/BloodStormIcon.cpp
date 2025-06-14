#include "stdafx.h"
#include "BloodStormIcon.h"
#include "WindowIDEnum.h"
#include "ChatManager.h"
#include "GameIn.h"
CBloodStormIcon::CBloodStormIcon()
{
	m_type=WT_BLOODSTORM_DLG;
	//m_IconStatic=NULL;
}
CBloodStormIcon::~CBloodStormIcon()
{
	//m_IconStatic=NULL;
}
void CBloodStormIcon::Linking()
{
	//m_IconStatic	= (cStatic*)GetWindowForID( BLOODSTORM_ICON_BTN );
}