/*#ifndef _WindowHookHelper_H_
#define _WindowHookHelper_H_
#pragma once
#include <windows.h>
#include <winuser.h>
#include "ChatManager.h"


HHOOK g_KeyBoardHook = NULL;  
HHOOK g_LowKeyBoardHook = NULL;  
HHOOK g_MouseHook = NULL; 
extern HWND _g_hWnd;

#define WH_KEYBOARD_LL       13
#define VK_Q			     81
LRESULT CALLBACK LowLevelKeyboardProc(int nCode,WPARAM wParam,LPARAM lParam)  
{  
              if(nCode==HC_ACTION)  
              {  
                            KBDLLHOOKSTRUCT * kblp=(KBDLLHOOKSTRUCT *)lParam;  

							if(kblp->vkCode==VK_Q && GetAsyncKeyState(VK_MENU)<0)
							{

								      if(IsWindowVisible(_g_hWnd)!=0)
	                                  {
		                                ShowWindow(_g_hWnd,SW_HIDE);
	                                  }
	                                  else
	                                  {
                                        ShowWindow(_g_hWnd,SW_SHOWNA);
	                                  }
								      return FALSE;
                            }  
              }  
              return CallNextHookEx(g_KeyBoardHook, nCode, wParam, lParam);  
}  

VOID __stdcall InstallHookEv(HINSTANCE Instance)  
{                 
              g_LowKeyBoardHook = SetWindowsHookEx(WH_KEYBOARD_LL, (HOOKPROC)LowLevelKeyboardProc,Instance, 0);  
}  
  
BOOL __stdcall UninstallHookEv()  
{   
             return UnhookWindowsHookEx(g_LowKeyBoardHook);   
} 


//BOOL GetWindowPlacement(HWND hWnd,WINDOWPLACEMENT *lpwndpl);
#endif*/