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
	DWORD m_dwGroupID;			//그룹 아이디
	float m_fRatio;				//비율
	DWORD m_dwRegenStartTime;	//리젠 시작 시간
	DWORD m_dwDelayTime;		//리젠 딜레이 시간	
	BOOL m_bRegen;				//리젠 플래그	
};

#endif //_INSDUNGEONREGENCONDITIONINFO_H