#pragma once
#define FORTWARMGR USINGTON(CFortWarManager)
class CFortWarManager
{
protected:
	DWORD m_nRelationFortWarIDX;
	int m_nTaxRate;
	BOOL m_WarStart;
	char cNpcName[17];
	DWORD dw_MoveMapNum;
	DWORD dw_MoveMapChannel;
	char m_MasterName[MAX_NAME_LENGTH + 1];
	DWORD dwStoredUniqueNpc;
	DWORD dwStoredNpcID;
	DWORD dwGuildIdx;
public:
	DWORD GetMoveMapNum(){ return dw_MoveMapNum; }
	void SetMoveMapNum(DWORD dwMap){ dw_MoveMapNum = dwMap; }
	DWORD GetMoveMapChannel(){ return dw_MoveMapChannel; }
	void SetMoveMapChannel(DWORD dwChannel){ dw_MoveMapChannel = dwChannel; }
	CFortWarManager(void);
	~CFortWarManager(void);
	void Init();
	void AddSymbolNpc(float x, float z, DWORD dwStaticNpcIdx = 15001, DWORD dwStaticNpcStartId = 0, char* pNpcName = NULL, DWORD dwRandomPick = 15001);
	void RemoveSymbolNpc(DWORD dwStaticNpcStartId = 0);
	DWORD CalTexRateForSell(DWORD dwMoney);
	DWORD CalTexRateForBuy(DWORD dwMoney);
	BOOL IsWarStart(){ return m_WarStart; }
	void NetworkMsgParse(BYTE Protocol, void* pMsg);
	void SetUniqueNpc(DWORD Val){ dwStoredUniqueNpc = Val; }
	DWORD GetUniqueNpc(){ return dwStoredUniqueNpc; }
	void SetNpcID(DWORD Val){ dwStoredNpcID = Val; }
	DWORD GetNpcID(){ return dwStoredNpcID; }
	DWORD GetGuildIdx(){ return dwGuildIdx; }
};
EXTERNGLOBALTON(CFortWarManager)
