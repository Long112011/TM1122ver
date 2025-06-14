#pragma once
#pragma comment(lib, "imm32.lib")
#define CIMEWND	USINGTON(cIMEWnd)
class cIMEex;
class cIMEWnd
{
public:
	cIMEWnd();
	virtual ~cIMEWnd();
	BOOL OnMsg(HWND hWnd, UINT msg, WPARAM wparam, LPARAM lparam);
	void SetDockingEx(cIMEex * box, BOOL bDock);
	BOOL IsDocking() { return m_pDockingEx ? TRUE : FALSE; }
	cIMEex* GetDockingEx() { return m_pDockingEx; }
	void SetCompositing(BOOL bCompositing);
	void SetCtrlPushed(BOOL bPushed) { m_bCtrlPushed = bPushed; }
	BOOL IsCtrlPushed() { return m_bCtrlPushed; }
private:
	BOOL OnKey(HWND hWnd, UINT msg, WPARAM wparam, LPARAM lparam);
	BOOL OnChar(HWND hWnd, UINT msg, WPARAM wparam, LPARAM lparam);
	BOOL OnIMEComposition(HWND hWnd, UINT msg, WPARAM wparam, LPARAM lparam);
	cIMEex*	m_pDockingEx;
	BOOL m_bCompositing;
	BOOL m_bCtrlPushed;
	UINT GetANSICode();
};
EXTERNGLOBALTON(cIMEWnd);