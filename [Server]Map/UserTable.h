
#pragma once

#include <set>
#include <map>
class CSkillArea;

class CUserTable : public CYHHashTable<CObject>
{
public:
	struct Team
	{
		typedef DWORD ObjectIndex;
		ObjectIndex mLeaderObjectIndex;
		typedef std::set< ObjectIndex > ObjectIndexContainer;
		ObjectIndexContainer mMemberContainer;

		Team() :
		mLeaderObjectIndex(0)
		{}
	};
private:
	DWORD m_dwUserCount;
	DWORD m_dwMonsterCount;
	DWORD m_dwNpcCount;
	DWORD m_dwExtraCount;

	typedef std::set<DWORD>					UserIndexSet;
	typedef std::map<DWORD, UserIndexSet>	UserIndexListInChannel;
	UserIndexSet							m_SearchUserIndexSet;
	UserIndexSet::iterator					m_SearchUserIndexIterator;
	UserIndexListInChannel					m_UserIndexListInChannel;

	typedef DWORD ObjectIndex;
	typedef std::string AliasName;
	typedef std::map< AliasName, ObjectIndex > AliasContainer;
	AliasContainer mAliasContainer;

	typedef std::string TeamName;
	typedef std::map< TeamName, Team > TeamContainer;
	TeamContainer mTeamContainer;


public:
	CUserTable();
	virtual ~CUserTable();

	void Init(DWORD dwBucket);
	CObject * FindUser(DWORD dwKey);
	CObject * FindUserForName( char* strCharacterName );
	BOOL AddUser(CObject* pObject,DWORD dwKey);
	CObject * RemoveUser(DWORD dwKey);
	void RemoveAllUser();
	
	void SetObjectIndex(LPCTSTR, ObjectIndex);
	ObjectIndex GetObjectIndex(LPCTSTR) const;
	///////////////////////////////////////////////////////////////////////////////////////////////////
	/// 06. 08. 2차 보스 - 이영준
	/// 기존 GetTargetInRange 함수에 마지막 인자로 안전거리 추가
	void GetTargetInRange(VECTOR3* pPivotPos,float Radius,CTargetList* pTList,float SafeRange = 0);

	///////////////////////////////////////////////////////////////////////////////////////////////////
	void GetTargetInArea(CSkillArea* pSkillArea,CTargetList* pTList);
	void GetTargetInDonutRangeInChannel(DWORD MonGridID, VECTOR3* pPivotPos,float MaxRadius, float MinRadius, CObject* rtpObject);


	
	DWORD GetUserCount(){ return m_dwUserCount; }
	DWORD GetMonsterCount(){ return m_dwMonsterCount; }
	DWORD GetNpcCount(){ return m_dwNpcCount; }
	DWORD GetExtraCount(){ return m_dwExtraCount; }

	void SetPositionUserHead() {  CYHHashTable<CObject>::SetPositionHead(); }
	CObject * GetUserData() { return CYHHashTable<CObject>::GetData(); }

	void SetPositionUserHeadChannel(DWORD dwChannel);
	CObject * GetUserDataChannel(DWORD dwChannel);
	const Team& GetTeam(LPCTSTR) const;
	void AddTeamMember(LPCTSTR, ObjectIndex);
	void SetTeamLeader(LPCTSTR, ObjectIndex);


};

extern CUserTable * g_pUserTable;							// key : connectionidx