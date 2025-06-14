




#ifndef __STREETSTALL__
#define __STREETSTALL__


#define MAX_STREETSTALL_CELLNUM 25
#define MAX_STREETBUYSTALL_CELLNUM 5

enum STALL_KIND
{
	eSK_NULL,
	eSK_SELL,
	eSK_BUY,
};

//#define DEFAULT_USABLE_INVENTORY	3

struct sCELLINFO 
{

	void Init()

	{
		wVolume = 0;
		dwMoney = 0;
		bLock = FALSE;
		bFill = FALSE;
		memset(&sItemBase, 0, sizeof(ITEMBASE));
	}

	ITEMBASE	sItemBase;
	DWORD		dwMoney;
    DWORD		dwGold ; // 2014-10-30 !
	WORD		wVolume;
	BOOL		bLock;
	BOOL		bFill;
};

class CPlayer;


class cStreetStall 
{

protected:
	sCELLINFO	m_sArticles[MAX_STREETSTALL_CELLNUM];		// º¸?¯¸ñ·?
	CPlayer*	m_pOwner;									// »ó?¡ ¿î¿µ??
	cPtrList	m_GuestList;								// »ó?¡?? ¼?´?
	int			m_nCurRegistItemNum;
	WORD		m_wStallKind;
//	WORD		m_nUsable;
	DWORD		m_nTotalMoney;

	DWORD       m_nTotalGold ; // 2014-10-30 ל

public:
	cStreetStall();
	virtual ~cStreetStall();
	void Init();

	DWORD	GetTotalMoney() { return m_nTotalMoney; };
	DWORD   GetTotalGold()  { return m_nTotalGold ;};
//	WORD	GetUsable() { return m_nUsable; };
//	void	SetMaxUsable() { m_nUsable = MAX_STREETSTALL_CELLNUM; };

//	void	SetExtraUsable(WORD num) { m_nUsable += num; };
//	void	SetDefaultUsable() { m_nUsable = DEFAULT_USABLE_INVENTORY; };


	// »ó?¡¿î¿µ ?¦¾î ??¼ö
	
	// 2014-10-30 
	BOOL FillCell(ITEMBASE* pBase, DWORD money,DWORD gold, BOOL bLock = FALSE, DWORD Volume = 0, WORD wAbsPosition = 0);
	void EmptyCell( ITEMBASE* pBase, eITEMTABLE tableIdx );
	void EmptyCell( POSTYPE pos );
	void EmptyCellAll();
	void ChangeCellState( WORD pos, BOOL bLock );

	// °ã?¡±â ¾?????¿¡ ´ë?? ?³¸®
	void UpdateCell( WORD pos, DURTYPE dur );


	void SetMoney( WORD pos, DWORD money );

	void SetGold( WORD pos, DWORD gold );  // 2014-10-30 ɨփʽ¾ݍ

	void SetVolume( WORD pos, WORD Volume );

	// LYJ ±¸ÀԳ끡 ß°¡
	WORD GetStallKind() { return m_wStallKind; }
	void SetStallKind(WORD wStallKind) { m_wStallKind = wStallKind; }

	// »ó?¡ ¿î¿µ??¿¡ ´ë?? ??¼ö 
	CPlayer* GetOwner() { return m_pOwner; }
	void SetOwner( CPlayer* pOwner ) { m_pOwner = pOwner; }
	
	// »ó?¡¿¡ ´ë?? ?ü?¼ ?¤º¸ ??¼ö
	void GetStreetStallInfo( STREETSTALL_INFO& stall );
	sCELLINFO* GetCellInfo( POSTYPE pos ) { return &m_sArticles[pos];}

	// »ó?¡ ¼?´?¿¡ ´ë?? ?¦¾î ??¼ö
	void AddGuest( CPlayer* pGuest );
	void DeleteGuest( CPlayer* pGuest );
	void DeleteGuestAll();
	void SendMsgGuestAll( MSGBASE* pMsg, int nMsgLen, BOOL bChangeState = FALSE );

	int GetCurRegistItemNum() { return m_nCurRegistItemNum; }
	
	BOOL IsFull();

	BOOL CheckItemDBIdx(DWORD idx);
	BOOL CheckItemIdx(DWORD idx);// ??º¹ °?»ç (???÷ DB¿¡ ??´? ??µ¦½º¿¡ ´ë?? ºñ±³¸¦ ??´?.)
};


#endif //__STREETSTALL__


