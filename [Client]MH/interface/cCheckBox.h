#ifndef _cCHECKBOX_H_
#define _cCHECKBOX_H_
#if _MSC_VER > 1000
#pragma once
#endif 
#include "cWindow.h"
class cCheckBox : public cWindow
{
public:
	cCheckBox();
	virtual ~cCheckBox();
	virtual void Init(LONG x, LONG y, WORD wid, WORD hei, cImage * basicImage, cImage * checkBoxImage, cImage * checkImage, cbFUNC Func, LONG ID = 0);
	virtual DWORD ActionEvent(CMouse * mouseInfo);
	virtual DWORD ActionKeyboardEvent(CKeyboard * keyInfo);
	virtual void Render();
	BOOL IsChecked(){ return m_fChecked; }
	void SetChecked(BOOL val){ m_fChecked = val; }
	void SetCheckBoxMsg(char * msg, DWORD color)
	{
		strcpy(m_szCheckBoxText, msg);
		m_dwCheckBoxTextColor = color;
	}
protected:
	cImage m_CheckBoxImage;
	cImage m_CheckImage;
	char m_szCheckBoxText[MAX_CHECKBOXTEXT_SIZE];
	DWORD m_dwCheckBoxTextColor;
	BOOL m_fChecked;

	char	m_szTextDebug[MAX_TEXT_SIZE];
	VECTOR2	m_SavedXY;
	WORD	m_SavedWidthHeight[2];
	int		m_SavedID;
	DWORD	m_DebugTimerKDBActivated;
	BOOL	m_DebugBoolean[4];
	void	DebugPressed();
};
#endif 
