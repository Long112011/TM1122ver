



#ifdef DEBUG






#pragma once
//#include "d:\darkstory\main_dev\[client]mh\interface\cdialog.h"
#include "./interface/cDialog.h"
#include "VirtualItem.h"
#include "./interface/cGuagen.h"
#include "cIconGridDialog.h"
class cStatic;
class cButton;
class CPet;

class CPetStateMiniDlg : public cDialog
{
	cStatic* m_pNameDlg;		//Æê ÀÌ¸§
	cStatic* m_pStateDlg;

	cStatic* m_pFriend;
	cStatic* m_pStamina;

	cGuagen* m_pFriendGuage[3];
	//	cGuagen*		m_pStaminaGuage;

	cButton* m_pPetUseRestBtn;	//Æê È°µ¿/ÈÞ½Ä ¹öÆ°
	cButton* m_pPetInvenBtn;	//Æê ÀÎº¥Åä¸®

	cButton* m_pDlgToggleBtn;
	cIconGridDialog* PetIcon;
	//	CPet* m_Pet[3];
	cIcon m_PetIcon[3];
public:
	CPetStateMiniDlg(void);
	virtual ~CPetStateMiniDlg(void);


	void Linking();
	void OnActionEvent(LONG lId, void* p, DWORD we);


	cStatic* GetNameTextWin() { return m_pNameDlg; }
	cStatic* GetUseRestTextWin() { return m_pStateDlg; }


	cStatic* GetFriendShipTextWin() { return m_pFriend; }
	cStatic* GetStaminaTextWin() { return m_pStamina; }

	//	cGuagen* GetFriendShipGuage()	{	return m_pFriendGuage;	}
	//	cGuagen* GetStaminaGuage()		{	return m_pStaminaGuage;	}
	void AddPetIcon(CPet** pPet);
	void RemovePetIcon(BYTE i);
	void SetPetFriendShipGuage(BYTE i, float friendShip);
	virtual void Render();
};



#else



#pragma once
//#include "d:\darkstory\main_dev\[client]mh\interface\cdialog.h"
#include "./interface/cDialog.h"

#include "./interface/cGuagen.h"

class cStatic;
class cButton;

class cGuagen;



class CPetStateMiniDlg : public cDialog
{
	cStatic*		m_pNameDlg;		//Æê ÀÌ¸§
	cStatic*		m_pStateDlg;

	cStatic*		m_pFriend;
	cStatic*		m_pStamina;

	cGuagen*		m_pFriendGuage;
	cGuagen*		m_pStaminaGuage;

	cButton*		m_pPetUseRestBtn;	//Æê È°µ¿/ÈÞ½Ä ¹öÆ°
	cButton*		m_pPetInvenBtn;	//Æê ÀÎº¥Åä¸®

	cButton*		m_pDlgToggleBtn;

public:
	CPetStateMiniDlg(void);
	virtual ~CPetStateMiniDlg(void);


	void Linking();
	void OnActionEvent(LONG lId, void *p, DWORD we);


	cStatic* GetNameTextWin()		{	return m_pNameDlg;	}
	cStatic* GetUseRestTextWin()	{	return m_pStateDlg;	}


	cStatic* GetFriendShipTextWin()	{	return m_pFriend;	}
	cStatic* GetStaminaTextWin()	{	return m_pStamina;	}

	cGuagen* GetFriendShipGuage()	{	return m_pFriendGuage;	}
	cGuagen* GetStaminaGuage()		{	return m_pStaminaGuage;	}
};


#endif // DEBUG