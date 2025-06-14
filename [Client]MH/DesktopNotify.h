// CDesktopNotify.h: interface for the CDesktopNotify class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CDesktopNotify_H__41182F11_BB6F_11D6_B0F5_00B0D01AD687__INCLUDED_)
#define AFX_CDesktopNotify_H__41182F11_BB6F_11D6_B0F5_00B0D01AD687__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



class CDesktopNotify  
{
public:
	void Hide();
	void Show();
	void Init(HWND hWnd,HINSTANCE hInst,int resid);
    BOOL SHOWING;
	CDesktopNotify();
	virtual ~CDesktopNotify();

private:
	UINT TimerID;
	HWND hParentWindow;
	HWND hCDesktopNotifyWnd;
    
};

#endif // !defined(AFX_CDesktopNotify_H__41182F11_BB6F_11D6_B0F5_00B0D01AD687__INCLUDED_)
