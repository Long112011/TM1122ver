







// MHNetwork.cpp: implementation of the CMHNetwork class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <string>          //
#include "MHNetwork.h"
#include "GameState.h"
#include "mmsystem.h"
#include "MainGame.h"
#include "GameResourceManager.h"
#include "Reconnect.h"
#include "../[CC]JACK/JackJack.h"
//#include "../[CC]JACK/JackHeaderStruct.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// 
//

BOOL bReceiving = FALSE;		
GLOBALTON(CMHNetwork)
CMHNetwork* gNetwork = NULL;
HMODULE g_hBaseNetworkHandle = NULL;

CMHNetwork::CMHNetwork()
{
	m_ConnectionIndex = NULL;
	m_CheckSum = 0;

	gNetwork = this;

	m_bDisconnecting = FALSE;
	
#ifdef _DEBUG
	memset(m_MsgIn,0,sizeof(DWORD)*MP_MAX);
	memset(m_MsgOut,0,sizeof(DWORD)*MP_MAX);
#endif

}

CMHNetwork::~CMHNetwork()
{

	Release();
}


BOOL CMHNetwork::CheckNetWork(char *ip,int port)
{
	DWORD AG=0,DIS=0;
	//BOOL IsConnect = FALSE;
	//SEVERLIST* pServerList = GAMERESRCMNGR->m_ServerList;
	//DIS= m_pBaseNetwork->ConnectToServer(REDISCONN->GetReConnectIP(), pServerList[index].DistributePort);
	//
	//if(DIS!=0)
	//{
	//	m_pBaseNetwork->CloseConnection(DIS);

	char hostname[256];
	strcpy(hostname, ip);
	if (isalpha(ip[0]))
	{
		hostent* pHostent = gethostbyname(hostname);
		if (pHostent == NULL) {
			return FALSE;
		}
		hostent& he = *pHostent;
		sockaddr_in sa;
		for (int nAdapter = 0; he.h_addr_list[nAdapter]; nAdapter++) {
			memcpy(&sa.sin_addr.s_addr, he.h_addr_list[nAdapter], he.h_length);
			sprintf(hostname, "%s", inet_ntoa(sa.sin_addr));
		}
	}
	AG = m_pBaseNetwork->ConnectToServer(hostname, port);
	if(AG!=0)
	{
		m_pBaseNetwork->CloseConnection(AG);
		return TRUE;
	}
	else
	{
		return FALSE;
	}
	//}

}
void CMHNetwork::Release()

{
	if(m_pBaseNetwork)
	{
		Disconnect();
	}
	
	SAFE_RELEASE(m_pBaseNetwork);


	if(g_hBaseNetworkHandle)
	{
		FreeLibrary( g_hBaseNetworkHandle );
		g_hBaseNetworkHandle = NULL;
	}

}

#define WM_SOCKEVENTMSG WM_USER+155

BOOL CMHNetwork::Init(HWND hWnd)
{
	m_hMainWnd = hWnd;
	
	HRESULT hr;
	
	//생성
	g_hBaseNetworkHandle = LoadLibrary("INetwork.dll");

	DllGetClassObject_BaseNetwork	pNetFunc;
	pNetFunc = (DllGetClassObject_BaseNetwork)GetProcAddress(g_hBaseNetworkHandle,"DllGetClassObject");
	hr = pNetFunc(CLSID_SC_BASENETWORK_DLL, IID_SC_BASENETWORK_DLL, (void**)&m_pBaseNetwork);		
	if (hr != S_OK)
	{

		//MessageBox(NULL,"BASENETWORK.DLL is Not Exist.","Error",MB_OK);
		return FALSE;
	}
	
	//Network 초기화 
	DESC_BASENETWORK Desc;	memset(&Desc,0,sizeof(Desc));
	Desc.wSockEventWinMsgID = WM_SOCKEVENTMSG;	
	Desc.OnConnect			= OnCon;
	Desc.OnDisconnect		= OnDisCon;
	Desc.ReceivedMsg		= OnRecv;
	
	m_pBaseNetwork->InitNetwork(m_hMainWnd,5,&Desc);

	if(m_pBaseNetwork == NULL)
		return FALSE;

	
	return TRUE;
}


BOOL CMHNetwork::ConnectToServer(char* ip,WORD port)
{	
	SEVERLIST* pServerList = GAMERESRCMNGR->m_ServerList;
	char hostname[256];
	strcpy(hostname,ip);
	if (isalpha(ip[0]))
	{
		hostent* pHostent = gethostbyname(hostname);
		if (pHostent==NULL) {		
			return FALSE;
		}		
		hostent& he = *pHostent;
		sockaddr_in sa;
		for (int nAdapter=0; he.h_addr_list[nAdapter]; nAdapter++) {
			memcpy ( &sa.sin_addr.s_addr, he.h_addr_list[nAdapter],he.h_length);
			sprintf(hostname,"%s", inet_ntoa(sa.sin_addr));
		}	
	}

	m_ConnectionIndex = m_pBaseNetwork->ConnectToServer(hostname,port);
	if(!m_ConnectionIndex)
		return FALSE;
	
	return TRUE;
}

void CMHNetwork::Disconnect()
{
	if(m_ConnectionIndex)
	{
		m_pBaseNetwork->CompulsiveDisconnect(m_ConnectionIndex);
		m_ConnectionIndex = 0;	
		m_bDisconnecting = TRUE;
	}
}

void CMHNetwork::SetCurState(CGameState* pState)
{
	m_pCurState = pState;
	m_RecvCount = 0;
	m_RecvedDataSize = 0;
}
std::string PacketEnDesStr=NetServer;
void CMHNetwork::Send(MSGROOT* pMsg,int MsgLen)
{
	pMsg->Category = pMsg->Category ^ JACKMGR->DecryptForNetwork(NetClient).c_str()[MsgLen % (strlen(JACKMGR->DecryptForNetwork(NetClient).c_str())-1)];
	pMsg->Protocol = pMsg->Protocol ^ JACKMGR->DecryptForNetwork(NetClient).c_str()[MsgLen % (strlen(JACKMGR->DecryptForNetwork(NetClient).c_str())-1)];

#ifdef _CRYPTCHECK_ 

	int ssize = sizeof( MSGROOT );

	if( !m_crypt.Encrypt( ( char * )pMsg + ssize, MsgLen - ssize ) )
	{
		return;
	}
	pMsg->Code = m_crypt.GetEnCRCConvertChar();

#endif	

	pMsg->CheckSum = m_CheckSum++;

	if(m_ConnectionIndex)
	{  
		m_pBaseNetwork->Send(m_ConnectionIndex,(char*)pMsg,MsgLen);
	}

}
void OnCon(DWORD dwConIndex)
{
}

void OnDisCon(DWORD dwConIndex)
{
	if(gNetwork->m_bDisconnecting == FALSE)
	{
		gNetwork->m_pCurState->OnDisconnect();
	}
	else
	{
		gNetwork->m_bDisconnecting = FALSE;
	}
}

void OnRecv(DWORD dwConIndex,char* pMsg,DWORD msglen)
{	
	bReceiving = TRUE;

	MSGROOT* pTemp = (MSGROOT*)pMsg;
	

#ifdef _CRYPTCHECK_ 
	int headerSize = sizeof( MSGROOT );
	if( !gNetwork->m_crypt.Decrypt( ( char * )pTemp + headerSize, msglen - headerSize ) )
	{
		return;
	}
	if( pTemp->Code != gNetwork->m_crypt.GetDeCRCConvertChar() )
	{
		return;
	}
#endif
	if(CGameState::CommonNetworkParser(pTemp->Category,pTemp->Protocol,pMsg) == FALSE)
		gNetwork->m_pCurState->NetworkMsgParse(pTemp->Category,pTemp->Protocol,pMsg);
}




