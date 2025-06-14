#pragma once
#define LOGINEVENTMGR CLoginEventManager::GetInstance()

struct LOGINEVENTINFO
{
	int		Key;
	DWORD	dwItemIdx;
	int		dwTiming;

	int		dwQuantity;

	DWORD	dwNextLine;
	char	cTextSend[/*MAX_NAME_LENGTH + 2*/128];
	LOGINEVENTINFO()
	{
		Key = 0;
		dwItemIdx =0;
		dwTiming =0;
		dwQuantity =0;
		dwNextLine =0;
		memset( cTextSend, 0, sizeof(char)*(MAX_NAME_LENGTH+1) );		
	}
};

class CLoginEventManager
{
	stTIME		m_FirstStartTime;
	stTIME		m_EndEventTime;
	stTIME		m_WaitTime;

	bool m_Master;
	bool m_MasterExt;
private:
	CYHHashTable<LOGINEVENTINFO>		m_LoginEventListInfo;
public:
	MAKESINGLETON(CLoginEventManager);
	CLoginEventManager();
	virtual ~CLoginEventManager();
	void Process(CPlayer* pPlayer);
	
	void LoadLoginEventItemList();
	
	LOGINEVENTINFO * GetLoginEventItemInfo(int Key)
	{
		return m_LoginEventListInfo.GetData(Key);
	}

	void LoginEventSend(CPlayer* pPlayer,DWORD dwKind,DWORD dwTiming,WORD dwItemIdx,char* TextMsg,int dwQty,int dwTotalTime);

	QSTATETYPE GetCurTimeYear();

	bool CheckEvent();

	char * GetStartTimeInString();
	char * GetEndTimeInString();
	char * GetTotalTimeInString();

	void AddLastLine(DWORD idx);
};