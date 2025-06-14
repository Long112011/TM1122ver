



#pragma once

#define BOBUSANGMGR	USINGTON(BobusangManager_Agent)


#define BOBUSANG_POSNUM_MAX	4	// �ʺ� ���� �ִ� ��ġ����
#define DEALITEM_BIN_TABNUM	7	// deallist.bin ���� �� ���� // ���λ��� ���� �Ѱ��� �����ϰ� �Ǹ��Ѵ�.

#define BOBUSANG_CHECKTIME	60000	// 1�� ���� Ȯ��

struct BobusangPosPerMap
{
	BobusangPosPerMap() {memset(this, 0, sizeof(this));};
	VECTOR3	ApprPos;
	float	ApprDir;
};


struct BOBUSANG_MAPINFO
{
	BOBUSANG_MAPINFO() : dataIdx(0), mapNum(0) {};
	DWORD dataIdx;
	DWORD mapNum;


	BobusangPosPerMap Pos[BOBUSANG_POSNUM_MAX];
};

enum bobusangInfotime{ eBBSIT_CUR, eBBSIT_NEXT, eBBSIT_KINDNUM, };
enum bobusangAppearedState{
	eBBSAS_NONE = -1,
	eBBSAS_DISAPPEAR			= 0,	// ���� ����
	eBBSAS_APPEAR				= 2,	// ���� ����
	eBBSAS_APPEAR_DELAYED		= 4,	// ���� �޽����� �� ���� ����
	eBBSAS_DISAPPEAR_DELAYED	= 8,	// ���� �޽����� �� ���� ����
	eBBSAS_TIME_DELAYED			= 16,	// �ð� üũ �ʰ�
};

class BobusangManager_Agent

{
	BOOL				m_bManager;

	BOOL				m_bOnProcessing;		// OnOff

	DWORD				m_dwBobusangCheckTime;	// Process üũ interval

	int*				m_piAppearedState;		// ���� ��ȯ ���ΰ�	//bobusangAppearedState
	DWORD				m_nChannelTotalNum;		// ��ü ä�� ����
	DWORD				m_dwAppearTimeMin;		// ���� �������� �ּ� �ð�
	DWORD				m_dwAppearTimeMax;		// ���� �������� �ִ� �ð�
	DWORD				m_dwDurationTimeMin;	// ���� ���� �ּ� �ð�
	DWORD				m_dwDurationTimeMax;	// ���� ���� �ִ� �ð�


	//BobusangPosPerMap	m_PosPerMap[MAX_MAP_ENUMCOUNT][BOBUSANG_POSNUM_MAX];	// �ʺ� ���� ��ġ
	CYHHashTable<BOBUSANG_MAPINFO>	m_mapInfo;

	BOBUSANGINFO*		m_pBobusangInfo;

public:
	BobusangManager_Agent(void);
	~BobusangManager_Agent(void);


	void	CheckManager();
	void	SetManager(BOOL bVal)
	{
		m_bManager = bVal;
	}

	void	BobusangMgr_Start();
	void	BobusangMgr_Init();
	void	BobusangMgr_Process();		// check time to Appear & inform to map
	void	BobusangMgr_Release();

	BOOL	LoadChannelInfo();		// set serverset & channel info
	BOOL	LoadBobusangInfo();			// regentime interval & etc.
	void	InitBobusangInfo();
	void	SetBobusangInfo(DWORD iChannel);			// set struct BOBUSANGINFO(...)	//eBBSIT_NEXT
	void	SendBobusangInfoMsgToMap(BOBUSANGINFO* pInfo);	// send bobusangInfo for map
	void	SendBobusangLeaveMsgToMap(BOBUSANGINFO* pInfo);	// bobusang remove from map
	void	SendBobusangLeaveNotifyToMap(BOBUSANGINFO* pInfo);

	void	SetChannelState(DWORD iChannel, int bobusangAppearedState);

	//for cheat
	void	DeveloperRequest(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);

	BOOL	SetProcessing(BOOL bVal);

	stTIME*	SetstTIMEfromMinValue(DWORD min);
};

EXTERNGLOBALTON(BobusangManager_Agent)


