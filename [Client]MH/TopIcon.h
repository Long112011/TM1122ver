#pragma once
#include "./Interface/cDialog.h"
#include "./interface/cWindowHeader.h"
#define TOPICONDLG GAMEIN->GetTopIcon()
class CTopIcon : public cDialog
{
	cStatic * m_Herolb;
	cStatic * m_MunpLb;
	cDialog * pDlg;
	
	cButton *  m_ShowBtn;
	cButton *  m_HideBtn;
	BOOL       m_IsHide;

	cStatic * m_BossNameTemp;
public:

	CTopIcon();

	virtual ~CTopIcon();

    void	Linking();
	//void	CheckShowOrHide();

	void SetShow(bool p);
	/*void	SetShow();
	void	SetHide();
	BOOL	IsShow();*/
	void    SetHeroFirst(char * pName);
	void    SetMunpFirst(char * pName);

	void	SetBossNameTemp(char * pName);
	void    OnActionEvent(LONG lId, void * p, DWORD we);

	char *	GetBossNameTemp(){return m_BossNameTemp->GetStaticText();}
};