#ifndef _INSDUNGEONREGENCONDITIONINFO_H
#define _INSDUNGEONREGENCONDITIONINFO_H

class CInsDungeonRegenConditionInfo
{
public:
	CInsDungeonRegenConditionInfo();
	~CInsDungeonRegenConditionInfo();

	inline void SetGroupID(DWORD dwGroupID)		{ m_dwGroupID = dwGroupID; }
	inline DWORD GetGroupID()					{ return m_dwGroupID; }

	inline void SetRatio(float fRatio)		{ m_fRatio = fRatio; }
	inline float GetRatio()					{ return m_fRatio; }

	inline void SetRegenStartTime(DWORD dwRegenStartTime)		{ m_dwRegenStartTime = dwRegenStartTime; }
	inline DWORD GetRegenStartTime()							{ return m_dwRegenStartTime; }

	inline void SetDelayTime(DWORD dwDelayTime)		{ m_dwDelayTime = dwDelayTime; }
	inline DWORD GetDelayTime()						{ return m_dwDelayTime; }	

	inline void SetRegen(BOOL bRegen)		{ m_bRegen = bRegen; }
	inline BOOL IsRegen()					{ return m_bRegen; }

protected:
	DWORD m_dwGroupID;			//�׷� ���̵�
	float m_fRatio;				//����
	DWORD m_dwRegenStartTime;	//���� ���� �ð�
	DWORD m_dwDelayTime;		//���� ������ �ð�	
	BOOL m_bRegen;				//���� �÷���	
};

#endif //_INSDUNGEONREGENCONDITIONINFO_H