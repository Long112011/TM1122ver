#pragma once

#include "./Interface/cDialog.h"
#include "./Interface/cIconGridDialog.h"
#include "./Interface/cIcon.h"

class cEditBox;
class cPushupButton;
class cListDialog;
class cIcon;

enum 
{
	WHOLE,
	PARTY,
	GUILD,
	ALLIANCE,
	SHOUT,
	MAX_CHAT_COUNT,
};

#define CHATLIST_TEXTLEN	65

#ifdef _TL_LOCAL_
#define CHATLIST_TEXTEXTENT	390
#endif

//#define CHARFACELIST        56         // ����ϵͳ 56�� 2014-12-15
//#define FACELISTSTARTIDX	148        // ����λ��ID 148 Ϊ��һ������λ�� 2014-12-15
enum {ChatExtend=0,ChatReduction=1,};
class CChatDialog  : public cDialog
{

protected:

	//cListDialog*	m_pChatList;

	cDialog	*	pDlg;

	cImage          m_ImageFace[CHARFACELIST];   // 2014-12-15 ����ϵͳ

	cIcon           m_IconFace[CHARFACELIST];    // 2014-12-15 ����ϵͳ

	cIconGridDialog * m_ImageGrid;     // 2014-12-15 �����

	cEditBox*		m_pChatEditBox;

	cListDialog*	m_pSheet[MAX_CHAT_COUNT];
	cPushupButton*	m_pPBMenu[MAX_CHAT_COUNT];

	int				m_nCurSheetNum;

//	LONG			m_lMenuX;
//	LONG			m_lMenuY;
//	LONG			m_lMenuHeight;
	
//	BOOL			m_bShowMenu;
	BOOL			m_bHideChatDialog;



	char			m_cPreWord[MAX_CHAT_COUNT];
	
	BOOL			m_bShowGuildTab;
	cPushupButton*	m_pAllShout;
	
	char			m_SelectedName[MAX_NAME_LENGTH+1];

	DWORD		dwSelectedItem;


	cButton*	m_ChatDlgSizeBtn[2];
public:


	CChatDialog();
	virtual ~CChatDialog();


	void Linking();
	//cEditBox * GetChatEditBox();
	BOOL	IsChatEditBoxActive();
	virtual DWORD ActionEvent( CMouse * mouseInfo );
	void AddMsg( BYTE ChatLimit, DWORD MsgColor, char* str );
	void RemoveMsg(char* str);
	void AddMsgAllRecall(DWORD MsgColor, char* str);
	void AddMsgAll( DWORD MsgColor, char* str );
	void AddMsgAll(int MsgColor, char* str);

	void OnSize(BOOL bFlag);
	void OnActionEvent(LONG lId, void * p, DWORD we);

	cEditBox* GetChatEditBox() { return m_pChatEditBox;	}

	cListDialog* GetSheet( int nSheet ) { return m_pSheet[nSheet]; }
	int	GetCurSheetNum()				{ return m_nCurSheetNum; }
	BYTE GetLineNum();

	void SelectMenu( int nSheet );
	void SetEditBoxPreWord();

	BOOL IsPreWord( char c );

	void HideChatDialog( BOOL bHide );

	void ShowGuildTab( BOOL bShow );
	
	WORD GetSheetPosY();
	WORD GetSheetHeight();
	
	void SetAllShoutBtnPushed( BOOL val );

	//SW070720 ��ȭâ �̸� ����
	void GetSelectedName(CMouse* mouseInfo);

	// 2014-12-15 ���鵽ϵͳ

	void LoadFaceList();

	void ExtendDlg() ;
	void ReductionDlg() ;

	BOOL IsChatFocus();

};