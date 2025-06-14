#pragma once

#include "./interface/cDialog.h"
#include ".\interface\cWindowHeader.h"

#define CHANNEL USINGTON(CChannelDialog)

class cListCtrl;

class CChannelDialog  : public cDialog
{

	int m_BaseChannelIndex;
	cListCtrl * m_pChannelLCtrl;
	cPtrList		m_pDataList;
	BOOL m_bInit;
	int m_SelectRowIdx;

	WORD m_wMoveMapNum;
	DWORD m_dwChangeMapState;

	cPushupButton*	m_pPageBtn[MAX_MOVEPOINT_PAGE];
	BOOL		m_bExtend;
	DWORD		m_SelectedTab;

	MSG_CHANNEL_INFO* pInfoSaved;

	WORD m_wMoveMapNumEx;

	MSG_DWORD5 moMsg;
	BOOL	  IsMove;
	DWORD	  CurTime;
	DWORD	  MoveTime;
	int		  nMcount;

public:
	CChannelDialog();
	virtual ~CChannelDialog();


	virtual DWORD ActionEvent(CMouse * mouseInfo);
	virtual DWORD ActionKeyboardEvent( CKeyboard * keyInfo );
	void Linking();
	void SetChannelList(MSG_CHANNEL_INFO* pInfo);
	virtual void SetActive(BOOL val);

	void SelectChannel(int rowidx);
	void OnConnect();


	void SendMapChannelInfoSYN(WORD wMapNum, DWORD dwState = eMapChange_General);
	void MapChange();

	

	void RefreshMoveInfo(DWORD Idx);
	void SetExtend( BOOL bExtend );
	void SetButton( DWORD Idx );

	MSG_CHANNEL_INFO* GetSavedInfo(){return pInfoSaved;}

	DWORD GetSavedCount()			{	return m_pDataList.GetCount();	}



	void SetMoveMsg(MSG_DWORD5 msg){moMsg = msg;}
	BOOL IsMoveMap(){return IsMove;}
	void SetMove(BOOL val){IsMove = val;}
	void DmoveMap();
	void InitDmove();
	void SetMoveMapNumEx(WORD Val){m_wMoveMapNumEx=Val;}
	WORD GetMoveMapNumEx(){return m_wMoveMapNumEx;}


	void SelectChannelGamePadInc();
	void SelectChannelGamePadDec();

};

EXTERNGLOBALTON(CChannelDialog)