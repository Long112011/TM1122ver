#pragma once
#include "./interface/cDialog.h"
#include "./interface/cGuagen.h"
class cStatic;
class cButton;
class cGuagen;
class cPushupButton;

//class CPet;
#define MAX_PET_MULTI 3

struct PET_MULTI_DLG
{
	DWORD Slot;
	DWORD PetIdx;
};
class CPetMultiDlg : public cDialog
{
	cDialog* pDlg;
	cStatic*		m_pNameDlg;		
	cButton*		m_pDetailBtn;
	cButton*		m_pPetInvenBtn;
	cStatic*		m_pPetFaceImg	[MAX_PET_MULTI];
	cGuagen*		m_pStaminaGuage	[MAX_PET_MULTI];
	cPushupButton*	m_pSelectPet[MAX_PET_MULTI];

	CYHHashTable<PET_MULTI_DLG> pPetMultiTable;
public:
	CPetMultiDlg(void);
	virtual ~CPetMultiDlg(void);
	void Linking();
	void OnActionEvent(LONG lId, void * p, DWORD we);
	void SetBtnClick(int btnKind);


	cStatic* GetNameTextWin()		{ return m_pNameDlg; }
	cStatic** Get2DImageWin()		{ return m_pPetFaceImg; }
	cGuagen** GetStaminaGuage()		{ return m_pStaminaGuage; }

	DWORD InitPet(DWORD idx, DWORD ObjId);
	void SeeSlot(DWORD ObjId);
	BOOL CheckSeeSlot(DWORD ObjId);
	void SeeALL();
};