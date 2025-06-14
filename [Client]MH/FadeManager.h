#pragma once
#define MAX_CHATMSGNUM	3000
#define FADEMGR USINGTON(FadeManager)
class  FadeManager
{	
protected:
	char*			m_pChatMsg[MAX_CHATMSGNUM];
	char			m_NULL;
	BOOL			m_fademgrInitialize;
public:
	FadeManager();
	virtual  ~FadeManager();
	char* GetChatMsg(int nMsgNum);
	void LoadMsgFile();
	BOOL GetManagerInit(){ return m_fademgrInitialize; }
	DWORD ConvertStringToDword(char* str);
};
EXTERNGLOBALTON(FadeManager)