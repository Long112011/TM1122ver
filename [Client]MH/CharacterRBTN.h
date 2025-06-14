#pragma once
#include "./interface/cDialog.h"
#include "WindowIDEnum.h"

#define CHARRBTN GAMEIN->GetCharacterRBTNDlg()
class CObject;
class CItem;

enum 
{
	eSelfOtherPlayer=0,
	eSelfMenu,
	eSelfItemName,
	eSelfChatMenu,
	eSelfItemDelelteSell,
};
class CharacterRBTN :public cDialog
{
	
	cDialog * pDlg;
	cStatic * bStatic;
	cButton * bButton[13];

	CObject * m_pSelectedObj;
	CItem	* m_pSelecItem;

	DWORD StoredItemIdx;
	DWORD m_InventoryFlag;
	char  m_cPreWord[5];

	char  m_cPreWordT[5];

	//char * bufName;
	DWORD m_Height;

	BYTE SetSelf;

	char bufName[MAX_CHAT_LENGTH + 1];
	char RecallMsg[MAX_CHAT_LENGTH + 1] ;

public:
	CharacterRBTN(void);
	virtual ~CharacterRBTN(void);

	virtual DWORD ActionEvent(CMouse*);

	void SetCharName(char * buf, DWORD color=0x00FF0000FF00);
	void Linking();

	void OnActionEvent(LONG lId, void * p, DWORD we);

	void RefreshPosition();
	void ShowOtherPlayerMenu( CObject* pObj );
	void ShowSelfMenu( CObject* pObj );
	void SetItemNameExt(DWORD flag);
	void SetItemName(DWORD ItemIdx,DWORD flag=0);

	void Hide();
	void Buka();

	CItem * GetSelectedItem(){return m_pSelecItem;}
	DWORD GetInventoryFlag(){return m_InventoryFlag;}
	

	char * GetCharNameForChat() { return bufName;}
	void SetTinggi(DWORD dwTinggi){m_Height=dwTinggi;}

	void RequestMsgForItemLink(DWORD ItemDBIdx);

	bool ItemCheckExt();
	//bool ItemCheck();

	char * ReqSmallChar(int i);
	

	//DWORD CheckMixCanOrNot();
	//BOOL CheckCanWearOrNot();

	//void SetSelectItem(CItem * pItem){m_pSelecItem = pItem;}
	

	void ShowChatMenu( char* buf , BOOL bRecallBtn=FALSE);

	void SetRecallMsg(char * msg){ SafeStrCpy(RecallMsg, msg, MAX_CHAT_LENGTH + 1); }

	//void ShowForChatBox( char* buf );


	void ButtonAction01();
	void ButtonAction02();
	void ButtonAction03();
	void ButtonAction04();
	void ButtonAction05();

	void ButtonAction06();
	void ButtonAction07();
	void ButtonAction08();
	void ButtonAction09();
	void ButtonAction10();

	void ButtonAction11();
	void ButtonAction12();
	void ButtonAction13();
};




