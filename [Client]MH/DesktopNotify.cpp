// CDesktopNotify.cpp: implementation of the CDesktopNotify class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "windows.h"
#include "DesktopNotify.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDesktopNotify::CDesktopNotify()
{

}

CDesktopNotify::~CDesktopNotify()
{
	DestroyWindow(hCDesktopNotifyWnd);
}

void CDesktopNotify::Init(HWND hWnd,HINSTANCE hInst,int resid)
{
	hParentWindow=hWnd;
	hCDesktopNotifyWnd=CreateWindowEx(WS_EX_CLIENTEDGE,"STATIC","",
	WS_POPUP|WS_DLGFRAME|SS_BITMAP,300,300,300,300,hWnd,NULL,hInst,NULL);
	SendMessage(hCDesktopNotifyWnd,STM_SETIMAGE,IMAGE_BITMAP,(LPARAM)LoadBitmap(hInst,MAKEINTRESOURCE(resid)));
	this->SHOWING = FALSE;
}

void CDesktopNotify::Show()
{
  //get size of hCDesktopNotifyWnd (width and height)
  int x,y;
  int pwidth,pheight;
  int tx,ty;
  HDWP windefer;
  RECT rect,prect;
  GetClientRect(hCDesktopNotifyWnd,&rect);
  x=rect.right;y=rect.bottom;
  //get parent screen coordinates
  GetWindowRect(this->hParentWindow,&prect);

  //center CDesktopNotify window on parent window
  pwidth=prect.right-prect.left;
  pheight=prect.bottom - prect.top;



  tx=(pwidth/2) - (x/2);
  ty=(pheight/2) - (y/2);

  tx+=prect.left;
  ty+=prect.top;


  windefer=BeginDeferWindowPos(1);
  DeferWindowPos(windefer,hCDesktopNotifyWnd,HWND_NOTOPMOST,tx,ty,50,50,SWP_NOSIZE|SWP_SHOWWINDOW|SWP_NOZORDER);
  EndDeferWindowPos(windefer);

  ShowWindow(hCDesktopNotifyWnd,SW_SHOWNORMAL);
  UpdateWindow(hCDesktopNotifyWnd);
  this->SHOWING = TRUE;
}

void CDesktopNotify::Hide()
{
  ShowWindow(hCDesktopNotifyWnd,SW_HIDE);
  this->SHOWING = FALSE;
}

