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


	char	FortCharacterName[MAX_NAME_LENGTH + 1];
	BOOL	FortUsingCustomNick;
public:
	inline char* GetObjectName() { return FortCharacterName; }
	inline DWORD getCustomNickNameChecked() { return FortUsingCustomNick; }
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
	
	void SetUniqueNpc(DWORD Val){ dwStoredUniqueNpc = Val; }
	DWORD GetUniqueNpc(){ return dwStoredUniqueNpc; }
	void SetNpcID(DWORD Val){ dwStoredNpcID = Val; }
	DWORD GetNpcID(){ return dwStoredNpcID; }
	DWORD GetGuildIdx(){ return dwGuildIdx; }





	void Mp_Fortwar_Customplayernick(void* pMsg);
	void Mp_Fortwar_Start(void* pMsg);
	void Mp_Fortwar_Newrandom(void* pMsg);
	void Mp_Fortwar_Ing(void* pMsg);
	void Mp_Fortwar_End(void* pMsg);
	void Mp_Fortwar_Engrave_Start_Ack(void* pMsg);
	void Mp_Fortwar_Engrave_Start_Nack(void* pMsg);
	void Mp_Fortwar_Start_Before10min(void* pMsg);
	void Mp_Fortwar_Info(void* pMsg);
	void NetworkMsgParse(BYTE Protocol, void* pMsg);
};
EXTERNGLOBALTON(CFortWarManager)
