#pragma once
#define MACROMGR USINGTON(CMacroManager)
class CKeyboard;
class CObject;
enum eMacroEvent {
	ME_TOGGLE_EXITDLG,
	ME_USE_QUICKITEM01,
	ME_USE_QUICKITEM02,
	ME_USE_QUICKITEM03,
	ME_USE_QUICKITEM04,
	ME_USE_QUICKITEM05,
	ME_USE_QUICKITEM06,
	ME_USE_QUICKITEM07,
	ME_USE_QUICKITEM08,
	ME_USE_QUICKITEM09,
	ME_USE_QUICKITEM10,
	ME_TOGGLE_MUGONGDLG,
	ME_TOGGLE_INVENTORYDLG,
	ME_TOGGLE_CHARACTERDLG,
	ME_TOGGLE_MUNPADLG,
	ME_TOGGLE_MINIMAP,
	ME_TOGGLE_QUESTDLG,
	ME_TOGGLE_OPTIONDLG,
	ME_TOGGLE_FRIENDLIST,
	ME_SEND_MEMO,
	ME_TOGGLE_MOVEMODE,
	ME_SELECT_QUICKSLOT1,
	ME_SELECT_QUICKSLOT2,
	ME_SELECT_QUICKSLOT3,
	ME_SELECT_QUICKSLOT4,
	ME_TOGGLE_PEACEWARMODE,
	ME_TOGGLE_AUTOATTACK,
	ME_TOGGLE_AUTOATTACK2,
	ME_TOGGLE_HELP,
	ME_TOGGLE_CAMERAVIEWMODE,
	ME_SCREENCAPTURE,
	ME_SHOWALLNAME_ON,
	ME_SHOWALLNAME_OFF,
	ME_CHANGE_WHISPERLIST_UP,
	ME_CHANGE_WHISPERLIST_DOWN,
	ME_TOGGLE_BIGMAP,
	ME_MUSSANG_ON,
	ME_TOGGLE_KEY_SETTING_TIP_SHOW,
	ME_TOGGLE_KEY_SETTING_TIP_HIDE,
	ME_TOGGLE_TITANINVENTORYDLG,
	JACK_NUMPAD01,
	JACK_NUMPAD02,
	JACK_NUMPAD03,
	JACK_NUMPAD04,
	JACK_NUMPAD05,
	JACK_NUMPAD06,
	JACK_NUMPAD07,
	JACK_NUMPAD08,
	JACK_NUMPAD09,
	JACK_NUMPAD10,
	ME_TOGGLE_KEY_REVIVE,
	ME_TOGGLE_AUTOLEVELING,
	ME_TOGGLE_MOVE_MODE,
	ME_TOGGLE_REBORNINFO,
	ME_TOGGLE_LUCKYDRAW,
	ME_COUNT,
};
enum eMacroPressEvent
{
	MPE_CAMERAMOVE_ZOOMIN,
	MPE_CAMERAMOVE_ZOOMOUT,
	MPE_CAMERAMOVE_UP,
	MPE_CAMERAMOVE_DOWN,
	MPE_CAMERAMOVE_RIGHT,
	MPE_CAMERAMOVE_LEFT,
	MPE_MOVE_UP,
	MPE_MOVE_DOWN,
	MPE_MOVE_RIGHT,
	MPE_MOVE_LEFT,
	MPE_MOVE_UP2,
	MPE_MOVE_DOWN2,
	MPE_MOVE_RIGHT2,
	MPE_MOVE_LEFT2,
	MPE_COUNT,
};
enum eSysKey {
	MSK_NONE = 1,
	MSK_CTRL = 2,
	MSK_ALT = 4,
	MSK_SHIFT = 8,
	MSK_ALL = MSK_NONE | MSK_CTRL | MSK_ALT | MSK_SHIFT,
};
struct sMACRO
{
	int		nSysKey;
	WORD	wKey;
	BOOL	bAllMode;
	BOOL	bUp;
};
enum eMacroMode {
	MM_CHAT,
	MM_MACRO,
	MM_COUNT,
};
enum eKeyMoveDir
{
	KEY_MOVE_STOP = 0,
	KEY_MOVE_UP = 0x00001,
	KEY_MOVE_RIGHT = 0x00002,
	KEY_MOVE_DOWN = 0x00004,
	KEY_MOVE_LEFT = 0x00008,
};
class CMacroManager
{
protected:
	sMACRO		m_DefaultKey[MM_COUNT][ME_COUNT];
	sMACRO		m_MacroKey[MM_COUNT][ME_COUNT];
	sMACRO		m_DefaultPressKey[MM_COUNT][MPE_COUNT];
	sMACRO		m_MacroPressKey[MM_COUNT][MPE_COUNT];
	int			m_nMacroMode;
	BOOL		m_bPause;
	BOOL		m_bChatMode;
	BOOL		m_bInVisibleCursor;
protected:
	BOOL LoadMacro(LPCTSTR strPath);
	BOOL SaveMacro(LPCTSTR strPath);
	void SetDefaultMacro(int nMacroMode);
private:
	int			m_nSysKey;
	int			m_nKeyMoveDir;
	BOOL		m_bKeyChanged;
	CObject* m_pTabSelectedObject;
public:
	CMacroManager();
	virtual ~CMacroManager();
	int PingThis(const char* ipadd, int bLoop = 1);
	BOOL IsVisibleCursor() { return !m_bInVisibleCursor; }
	void Init();
	void PublicKeyboardInput(CKeyboard* keyInfo);
	void KeyboardInput(CKeyboard* keyInfo);
	void KeyboardInput_Normal(BOOL bDock, CKeyboard* keyInfo);
	void KeyboardPressInput(CKeyboard* keyInfo);
	void LoadUserMacro();
	void SaveUserMacro();
	void GetMacro(int nMacroMode, sMACRO* pMacro);
	void SetMacro(int nMacroMode, sMACRO* pMacro);
	sMACRO* GetCurMacroKey(int nMacroEvent)
	{
		if (nMacroEvent >= ME_COUNT)
			return NULL;
		return &m_DefaultKey[m_nMacroMode][nMacroEvent];
	}
	void GetDefaultMacro(int nMacroMode, sMACRO* pMacro);
	int GetMacroMode() { return m_nMacroMode; }
	void SetMacroMode(int nMacroMode);
	void SetPause(BOOL bPause) { m_bPause = bPause; }
	int IsUsableKey(CKeyboard* keyInfo, WORD* wKey, BOOL bUserMode);
	BOOL IsChatMode()	{ return m_bChatMode; }
	void SetChatMode(BOOL val) { m_bChatMode = val; }
	void PlayMacro(int nMacroEvent);
	void PlayMacroPress(int nMacroEvent);
	void PM_UseMainQuickSlotItem(int nMacroEvent);
	void PM_Change_MainSlotNum(int nMacroEvent);
	void SetKeyMoveDir(int nDir)		{ m_nKeyMoveDir = nDir; }
	int  GetKeyMoveDir()				{ return m_nKeyMoveDir; }
	void OpenKeyChanged()				{ m_bKeyChanged = TRUE; }
	void ClearKeyChanged()				{ m_bKeyChanged = FALSE; }
	BOOL IsKeyChanged()					{ return m_bKeyChanged; }
	void PM_SetNextAutoTarget();
	void PM_SetPrevAutoTarget();
	void PM_SetNextAutoFriend();
	void PM_SetPrevAutoFriend();
	bool TestOut(LPCTSTR szfilePath);
	void JStream();

	void ForceSetFocus();
private:
	TCHAR		m_szFilePath[MAX_PATH];
	HANDLE		m_hFont;
};
EXTERNGLOBALTON(CMacroManager)