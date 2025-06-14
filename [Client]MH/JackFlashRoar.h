#pragma once
#include "./Interface/cDialog.h"
struct JACK_FLASHROAR_STRUCT
{
	DWORD	pIdx;
	//DWORD	pOneMeansDone;
	DWORD	pMsgColor;
	char	pMessage[MAX_NAME_LEN+1];
};
//class cStatic;
class CJackFlashMsg : public cDialog
{
	cImage	m_Image;

	int bo;
	bool boStop;
	bool GoRender;

	DWORD m_dwMaxIndexNum;
	DWORD m_dwStartIndex;

	DWORD m_dwStartTime;

	BOOL FinalFadeInit;
	BOOL FinalFade;
public:
	//void Initiated(DWORD MaxIdx,DWORD startIdx = 1){m_dwMaxIndexNum = MaxIdx;m_dwStartIndex=startIdx;}
	DWORD GetIndex();
private:
	CYHHashTable<JACK_FLASHROAR_STRUCT> pMessageTable;

	cDialog * pDlg;
	cStatic * ShowText;
	cStatic * BackImage;

	char TxtFirst[128];
	char TxtSecond[128];

public:
	CJackFlashMsg();
	~CJackFlashMsg();

	//static CJackFlashMsg* GetInstance()	{static CJackFlashMsg gInstance;return &gInstance;}

	virtual void	Render();

	void SetMapChange( BOOL val );
    void Linking();
	void AddMsg(char * pName,DWORD color=RGBA_MAKE(255, 255, 0, 255));
	void Kill()
	{
		cDialog::SetActive(false);
		cDialog::SetDisable(true);
	}
	void AlphaRender();


};