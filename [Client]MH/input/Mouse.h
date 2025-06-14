#pragma once
struct sMOUSEINFO
{

	int nX;
	int nY;
	int nEvent;
	int nKey;
};

#define MAX_QUESIZE 32

#define DLG_NONBUTTON_CONTROL 5000 // First ID of non-button controls

#ifndef WM_XBUTTONDOWN // For Win2K / winME ONLY
#define WM_XBUTTONDOWN 0x020B
#define WM_XBUTTONUP 0x020C
#define WM_XBUTTONDBLCLK 0x020D
#define MK_XBUTTON1 0x0020
#define MK_XBUTTON2 0x0040
#endif

enum eMouseEvent
{
	eME_LDOWN,
	eME_RDOWN,
	eME_MDOWN,
	eME_LUP,
	eME_RUP,
	eME_MUP,
	eME_LDBL,
	eME_RDBL,
	eME_MDBL,
	eME_COUNT,
};




class CMouse  
{

//que

	//sMOUSEINFO*	m_pMouseInfo;
	sMOUSEINFO	m_pMouseInfo[ MAX_QUESIZE ];
	sMOUSEINFO	m_CurMouseInfo;
	int			m_nHead;
	int			m_nTail;
	int			m_nQueSize;
//	
	BOOL		m_bMouseEvent[eME_COUNT];

	BOOL		m_bLBtnPress;
	BOOL		m_bRBtnPress;
	BOOL		m_bMBtnPress;
	BOOL		m_bLBtnDrag;
	BOOL		m_bRBtnDrag;
	BOOL		m_bMBtnDrag;

	int			m_nMouseX;
	int			m_nMouseY;
	int			m_nMouseOldX;
	int			m_nMouseOldY;
	int			m_nMouseEventX;
	int			m_nMouseEventY;

	int			m_nMouseAxisX;
	int			m_nMouseAxisY;

	int			m_nWheelMsg;
	int			m_nWheel;

	int			m_nCombineKey;

protected:

	void RecordMouseEvent( int nEvent, WPARAM wParam, LPARAM lParam );
	void ParseCurMouseEvent( sMOUSEINFO* pMouseInfo );
	BOOL PushMouseInfo( sMOUSEINFO* pIn );
	BOOL PopMouseInfo( sMOUSEINFO* pOut );


public:
	CMouse();
	virtual ~CMouse();

	void Init();
	void Release();

	void GetWindowMessage(HWND hWnd, DWORD wmsg,WPARAM wParam,LPARAM lParam);
	void GetNextMouseEvent();
	BOOL IsEmptyMouseEvent() { return m_nTail == m_nHead ? TRUE : FALSE; }

//Inline
	int		GetMouseX()				{ return m_nMouseX;					}
	int		GetMouseY()				{ return m_nMouseY;					}

	BOOL	LButtonDown()			{ return m_bMouseEvent[eME_LDOWN];	}
	BOOL	RButtonDown()			{ return m_bMouseEvent[eME_RDOWN];	}
	BOOL	MButtonDown()			{ return m_bMouseEvent[eME_MDOWN];	}
	BOOL	LButtonUp()				{ return m_bMouseEvent[eME_LUP];	}
	BOOL	RButtonUp()				{ return m_bMouseEvent[eME_RUP];	}
	BOOL	MButtonUp()				{ return m_bMouseEvent[eME_MUP];	}

	BOOL	LButtonDoubleClick()	{ return m_bMouseEvent[eME_LDBL];	}
	BOOL	RButtonDoubleClick()	{ return m_bMouseEvent[eME_RDBL];	}
	BOOL	MButtonDoubleClick()	{ return m_bMouseEvent[eME_MDBL];	}
	int		GetMouseEventX()		{ return m_nMouseEventX;			}
	int		GetMouseEventY()		{ return m_nMouseEventY;			}
	int		GetMouseAxisX()			{ return m_nMouseAxisX;				}
	int		GetMouseAxisY()			{ return m_nMouseAxisY;				}
	BOOL	LButtonPressed()		{ return m_bLBtnPress;				}

	BOOL	RButtonPressed()		{ return m_bRBtnPress;				}
	BOOL	MButtonPressed()		{ return m_bMBtnPress;				}
	BOOL	LButtonDrag()			{ return m_bLBtnDrag;				}
	BOOL	RButtonDrag()			{ return m_bRBtnDrag;				}
	BOOL	MButtonDrag()			{ return m_bMBtnDrag;				}

	int		GetWheel()				{ return m_nWheel;					}

	BOOL	ControlPressed()		{ return m_nCombineKey & MK_CONTROL; }
	BOOL	ShiftPressed()			{ return m_nCombineKey & MK_SHIFT;	}

	BOOL	GetXButton1Down()			{ return m_nCombineKey & MK_XBUTTON1; }
	BOOL	GetXButton2Down()			{ return m_nCombineKey & MK_XBUTTON2; }
};