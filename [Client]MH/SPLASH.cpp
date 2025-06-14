// SPLASH.cpp: implementation of the SPLASH class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "windows.h"
#include "SPLASH.h"
#include "GameResourceManager.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

SPLASH::SPLASH()
{

}

SPLASH::~SPLASH()
{
 DestroyWindow(hSplashWnd);
 DeleteObject(hbitmap);
}

void SPLASH::Init(HWND hWnd,HINSTANCE hInst/*,int resid*/)
{
	 hParentWindow=hWnd;
	 hSplashWnd=CreateWindowEx(NULL,"STATIC","",WS_POPUP/*|WS_DLGFRAME*/|SS_BITMAP,300,300,300,300,hWnd,NULL,hInst,NULL);
	 hbitmap = (HBITMAP)LoadImage(hInst, GAMERESRCMNGR->m_GameDesc.splashBitmap/*"image/2d/log.bmp"*/, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	 //SendMessage(hSplashWnd,STM_SETIMAGE,IMAGE_BITMAP,(LPARAM)LoadBitmap(hInst,MAKEINTRESOURCE(resid)));
	 SendMessage(hSplashWnd, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hbitmap);
	 this->SHOWING = FALSE;
	// 
}

void SPLASH::Show()
{
  //get size of hSplashWnd (width and height)
  int x,y;
  int pwidth,pheight;
  int tx,ty;
  HDWP windefer;
  RECT rect,prect;
  GetClientRect(hSplashWnd,&rect);
  x=rect.right;y=rect.bottom;
  //get parent screen coordinates
  GetWindowRect(this->hParentWindow,&prect);

  //center splash window on parent window
  pwidth=prect.right-prect.left;
  pheight=prect.bottom - prect.top;



  tx=(pwidth/2) - (x/2);
  ty=(pheight/2) - (y/2);

  tx+=prect.left;
  ty+=prect.top;


  windefer=BeginDeferWindowPos(1);
  DeferWindowPos(windefer,hSplashWnd,HWND_NOTOPMOST,tx,ty,50,50,SWP_NOSIZE|SWP_SHOWWINDOW|SWP_NOZORDER);
  EndDeferWindowPos(windefer);

  ShowWindow(hSplashWnd,SW_SHOWNORMAL);
  UpdateWindow(hSplashWnd);
  this->SHOWING = TRUE;
}

void SPLASH::Hide()
{
  ShowWindow(hSplashWnd,SW_HIDE);
  this->SHOWING = FALSE;
  DeleteObject(hbitmap);
}

