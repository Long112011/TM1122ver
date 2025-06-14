#pragma once
#include "INTERFACE\cReportItem.h"
#include "cScriptManager.h"
#define    MAX_IMGITEM_NUM  10
class cRITEMEx : public RITEM  
{
public:
	cRITEMEx(WORD num);//:RITEM(num) 
	
	virtual ~cRITEMEx();
	DWORD dwID;
	cImage   ItemImg[MAX_IMGITEM_NUM];
	char     toolTipMsg[256];
	cMultiLineText m_toolTip;
public:
	void AddToolTipLine(char* msg, DWORD color= TTTC_DEFAULT,cImage * m_LineImage=NULL,WORD m_FontIdx=0);
	void Render();
};