#pragma once
#include "jackjack.h"
#ifdef _CLIENTJ_
#define HANTARMGR USINGTON(CHantarManager)
#else
#define HANTARMGR CHantarManager::GetInstance()
#endif



class CHantarManager  
{
public:
#ifndef _CLIENTJ_	//untuk exe server
	MAKESINGLETON(CHantarManager);
#endif				//end of _CLIENTJ_ untuk exe server

	//											©³©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©·
	//											©§							GLOBAL							©§
	//											©»©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¿
	CHantarManager();
	virtual ~CHantarManager();
	void Process();
	BOOL m_IsSending;

	//int PingThis(const char* ipadd, int bLoop = 1);
	
	//unsigned short in_cksum(unsigned short *ptr, int nbytes);





	//											©³©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©·
	//											©§							CLIENT							©§
	//											©»©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¿
#ifdef _CLIENTJ_

	DWORD dwCheckTime;
	BOOL bLock;
	HANDLE m_threadHandle;
	int doThePing();
	
	void threadProcEnable();
	void threadProcDisable();
	void SetLock(BOOL val){ bLock = val; }
	char senderHost[MAX_NAME_LENGTH + 1];
	char * GetSenderHost(){ return senderHost; }
	void SetSenderHost(char * str){ SafeStrCpy(senderHost, str, MAX_NAME_LENGTH + 1); }

	char senderPort[MAX_NAME_LENGTH + 1];
	char * GetSenderPort(){ return senderPort; }
	void SetSenderPort(char * str){ SafeStrCpy(senderPort, str, MAX_NAME_LENGTH + 1); }

	void SetHostAndPort(char*host, char*port){
		SafeStrCpy(senderHost, host, MAX_NAME_LENGTH + 1);
		SafeStrCpy(senderPort, port, MAX_NAME_LENGTH + 1);
	}



	char senderHostForUser[32 + 1];
	char * GetSenderHostForUser(){ return senderHostForUser; }
	void SetSenderHostForUser(char * str){ SafeStrCpy(senderHostForUser, str, 32 + 1); }

	char senderPortForUser[MAX_NAME_LENGTH + 1];
	char * GetSenderPortForUser(){ return senderPortForUser; }
	void SetSenderPortForUser(char * str){ SafeStrCpy(senderPortForUser, str, MAX_NAME_LENGTH + 1); }

	void SetHostAndPortForUser(char*host, char*port){
		SafeStrCpy(senderHostForUser, host, 32 + 1);
		SafeStrCpy(senderPortForUser, port, MAX_NAME_LENGTH + 1);
	}

	void PublicKeyboardInput(CKeyboard* keyInfo);

	BOOL Cheat(char* cheat);
	void NetworkMsgParseGet(BYTE Protocol, void* pMsg);















#endif //end of _CLIENTJ_ for CLIENT
	//											©³©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©·
	//											©§							SERVER							©§
	//											©»©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¿
#ifndef _CLIENTJ_

#endif //end of _CLIENTJ_ for SERVER












};

#ifdef _CLIENTJ_
EXTERNGLOBALTON(CHantarManager)
#endif