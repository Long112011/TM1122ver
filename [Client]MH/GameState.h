#pragma once
class CGameState  
{
protected:
	BOOL	m_bBlockScreen;
	BOOL	m_bDisconnected;
public:
	CGameState();
	virtual ~CGameState();

	virtual BOOL Init(void* pInitParam) = 0;
	virtual void Release(CGameState* pNextGameState) = 0;		// NULL 檜賊 部釭朝剪�?

	virtual void Process() = 0;
	virtual void BeforeRender() = 0;
	virtual void AfterRender() = 0;
	
	virtual void NetworkMsgParse(BYTE Category,BYTE Protocol,void* pMsg) = 0;
	static	BOOL CommonNetworkParser(BYTE Category,BYTE Protocol,void* pMsg);

	void	SetBlock(BOOL val);
	BOOL	IsBlock(){ return m_bBlockScreen;	}
	
	virtual BOOL IsDisconnect();
	virtual void OnDisconnect();
};