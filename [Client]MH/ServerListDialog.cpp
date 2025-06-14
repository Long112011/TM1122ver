// ServerListDialog.cpp: implementation of the CServerListDialog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ServerListDialog.h"

#include "WindowIDEnum.h"
#include "GameResourceManager.h"
#include "./Interface/cWindowManager.h"
#include "./Interface/cButton.h"
#include "./Interface/cStatic.h"
#include "./Interface/cListCtrl.h"
#include "MainTitle.h"
#include "ErrorMsg.h"
#include "./Input/UserInput.h"
#include "reconnect.h"

#include "ReconnectDialog.h"
CServerListDialog::CServerListDialog()
{
	m_pServerListCtrl = NULL;
	m_nMaxServerNum = 0;
	m_nIndex = -1;
}

CServerListDialog::~CServerListDialog()
{
	if(m_pServerListCtrl)
	{
	   m_pServerListCtrl->DeleteAllItems();
	}
}

void CServerListDialog::Linking()
{
	m_pServerListCtrl = (cListCtrl*)GetWindowForID(SL_LISTCTRL);
	m_pConnectBtn = (cButton*)GetWindowForID(SL_BTN_CONNECT);
	m_pExitBtn = (cButton*)GetWindowForID(SL_BTN_EXIT);

	LoadServerList();
}

void CServerListDialog::LoadServerList()
{
	SEVERLIST* pServerList = GAMERESRCMNGR->m_ServerList;
	m_nMaxServerNum = GAMERESRCMNGR->m_nMaxServerList;

	for( int i = 0; i < m_nMaxServerNum; ++i )
	{
		cRITEMEx* ritem = new cRITEMEx(2);
		ritem->dwID = i+1;
		sprintf( ritem->pString[0], "%d", i+1 );
		strcpy( ritem->pString[1], pServerList[i].ServerName );
		if( !pServerList[i].bEnter )
		{
			ritem->rgb[0] = RGBA_MAKE(255,0,0,255);
			ritem->rgb[1] = RGBA_MAKE(255,0,0,255);
		}

		m_pServerListCtrl->InsertItem( i, ritem );
	}

}

DWORD CServerListDialog::ActionEvent( CMouse * mouseInfo )
{
	DWORD we = WE_NULL;
	if (!m_bActive) return we;

	CReconnectDialog* m_RecDlg = (CReconnectDialog*)WINDOWMGR->GetWindowForID(RECONNECT_DLG);
	if (m_RecDlg && m_RecDlg->IsActive())
	{
		return we;
	}
	
	

	we = cDialog::ActionEvent(mouseInfo);
	
	cRITEMEx* pRItem = NULL;
	
	/*for( int i = 0 ; i < 9 ; ++i )
	{
		if(KEYBOARD->GetKeyPressed(KEY_PAD1+i)||KEYBOARD->GetKeyPressed(KEY_1+i))
		{
			REDISCONN->SetServerIdx(0+i);
			//m_ServerIndex=0+i;
			TITLE->ConnectToServer( 0+i );
			return we;
		}
	}
	if(KEYBOARD->GetKeyPressed(KEY_PAD0)||KEYBOARD->GetKeyPressed(KEY_0))
	{
		//m_ServerIndex=9;
		REDISCONN->SetServerIdx(9);
		TITLE->ConnectToServer( 9 );
		return we;
	}*/

	


	if( we & WE_ROWCLICK )
	{
		SEVERLIST* pServerList = GAMERESRCMNGR->m_ServerList;
		int i = m_pServerListCtrl->GetSelectedRowIdx();
		if( i < m_nMaxServerNum )
		{
			pRItem = (cRITEMEx*)m_pServerListCtrl->GetRItem(i);
			if( m_nIndex != i )
			{
				pRItem->rgb[0] = RGBA_MAKE(255,234,0,255);
				pRItem->rgb[1] = RGBA_MAKE(255,234,0,255);
				if( !pServerList[i].bEnter )
				{
					pRItem->rgb[0] = RGBA_MAKE(255,0,0,255);
					pRItem->rgb[1] = RGBA_MAKE(255,0,0,255);

				}	
				if( m_nIndex > -1 )
				{
					pRItem = (cRITEMEx*)m_pServerListCtrl->GetRItem(m_nIndex);
					pRItem->rgb[0] = 0xffffffff;
					pRItem->rgb[1] = 0xffffffff;

					if( !pServerList[m_nIndex].bEnter )
					{
						pRItem->rgb[0] = RGBA_MAKE(255,0,0,255);
						pRItem->rgb[1] = RGBA_MAKE(255,0,0,255);

					}
				}					
			}


			m_nIndex = i;
		}
	}
	else if( we & WE_ROWDBLCLICK )
	{
		int i = m_pServerListCtrl->GetSelectedRowIdx();

		if( i < m_nMaxServerNum )
		{
			REDISCONN->SetServerIdx(i);
			TITLE->ConnectToServer(i);
		}
	}
	return we;
}

void CServerListDialog::SetSelectedIndex(int Index)
{
	m_nIndex=Index;
}



