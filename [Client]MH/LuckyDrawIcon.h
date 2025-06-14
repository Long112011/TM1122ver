#pragma once
#include "./interface/cDialog.h"
class cButton;

class CLuckyDrawIcon: public cDialog
{
	cButton * m_IconBtn;

	bool IsLuckyDraw;

public:
	
	CLuckyDrawIcon();

	~CLuckyDrawIcon();

	void Linking();

	void  OnActionEvent(LONG lId, void * p, DWORD we);

	virtual DWORD ActionEvent(CMouse * mouseInfo);

	void SetOpen(bool b);// { IsLuckyDraw = b; }
};