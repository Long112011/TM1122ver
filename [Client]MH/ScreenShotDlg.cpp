









///////////////////////////////////////
// CScreenShotDlg.cpp: implementation of the CScreenShotDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ScreenShotDlg.h"
#include "WindowIDEnum.h"
#include "ObjectManager.h"
#include "./Interface/cWindowManager.h"

#include "./Interface/cStatic.h"
#include "./Interface/cScriptManager.h"
#include "gamein.h"
#include "MHTimeManager.h"
#include "GameResourceManager.h"

extern int g_nServerSetNum;
//extern char g_ServerSetName[255];

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CScreenShotDlg::CScreenShotDlg()
{
	m_type = WT_SCREENSHOT_DLG;
	m_sttime = NULL;
}

CScreenShotDlg::~CScreenShotDlg()
{
}


void CScreenShotDlg::Linking()
{

	SCRIPTMGR->GetImage( 138, &m_Image, PFT_HARDPATH ); //배경이미지 로드

	//캐릭터가 서버에 로그인한 시간 출력 static
	m_sttime = new cStatic; 
	if( m_sttime )
	{
		m_sttime->Init( 10, 10, 0, 0, NULL, -1 );
		m_sttime->SetShadow( TRUE );
		m_sttime->SetFontIdx( 0 );

		m_sttime->SetAlign( TXT_LEFT );
		m_sttime->SetStaticText( "" );

		
		Add( m_sttime );
	}



	//캐릭터가 플레이한 시간 출력 static
	m_playtime = new cStatic;

	if( m_playtime )
	{

		m_playtime->Init( 10, 27, 0, 0, NULL, -1 );
		m_playtime->SetShadow( TRUE );
		m_playtime->SetFontIdx( 0 );
		m_playtime->SetAlign( TXT_LEFT );
		m_playtime->SetStaticText( "" );
		
		Add( m_playtime );
	}

    m_dwDelayTime = 0;
}


void CScreenShotDlg::SetActive( BOOL val, DWORD HeroID )
{
	cDialog::SetActive( val );


	if(val == TRUE)
		ViewDatetime(HeroID);	
}


void CScreenShotDlg::ViewDatetime(DWORD HeroID)
{

	char timetext[255];			// 시간포멧후 static에 출력할 변수
	DWORD currenttime;			// 클라이언트 접후 현재까지 진행된 시간	
	DWORD currentdate;			// 클라이언트 접후 현재까지 진행된 날짜
	DWORD totalcurrenttime;		// 날짜를 시간단위로 변경 (토탈시간)
	DWORD clientlogindate;		// 캐릭터 로그인 당시 클라이언트시계의 날짜
	DWORD clientlogintime;		// 캐릭터 로그인 당시 클라이언트시계의 시간

	DWORD totalclientlogintime; // 날짜를 시간단위로 변경 (토탈시간)
	

	serverti = GAMEIN->GetLoginTime();		// 캐릭터가 서버에 로그인 시간
	GAMEIN->GetClientLoginTime(clientlogindate,clientlogintime);	
	totalclientlogintime = (clientlogindate * 1000 * 60 * 60 * 24) + clientlogintime;


	currentdate	= MHTIMEMGR->GetMHDate();	



	currenttime = MHTIMEMGR->GetMHTime();
	totalcurrenttime = (currentdate * 1000 * 60 * 60 * 24) + currenttime;
		
	//서버 로그인 시간 출력

	// RaMa - 서버명 출력
	char servername[64];

	memset( servername, 0, sizeof(char)*64 );
#ifdef _HK_LOCAL_
	strncpy( servername, GAMERESRCMNGR->GetServerSetName(), 4 );
#else 
	strncpy( servername, GAMERESRCMNGR->GetServerSetName(), 32 );
#endif

	sprintf(timetext, "[%s] %02d-%02d-%02d %02d:%02d:%02d",	servername,
		serverti.wYear, serverti.wMonth, serverti.wDay, 


		serverti.wHour, serverti.wMinute, serverti.wSecond);
	m_sttime->SetStaticText(timetext);
	
	//캐릭터 플레이 시간 출력

	sprintf(timetext, "[PLAYTIME] %2d Day %02d:%02d:%02d", 
		(totalcurrenttime-totalclientlogintime)/1000/60/60/24,
		((totalcurrenttime-totalclientlogintime)/1000/60/60)%24,
		((totalcurrenttime-totalclientlogintime)/1000/60)%60,
		((totalcurrenttime-totalclientlogintime)/1000)%60);
	m_playtime->SetStaticText(timetext);
	
	m_bShow = TRUE;	
	m_dwDelayTime = gCurTime + eDelayTime;
}

void CScreenShotDlg::Render()
{
	//2008. 4. 15. CBH - 스크린샷 2번 빠르게 연타했을때 다이얼로그가 찍히지 않는 
	//버그 때문에 2초정도 다이얼로그를 그린다.	
	if(m_dwDelayTime > gCurTime)
	{
		m_bShow = TRUE;
	}
	else if(m_bShow == TRUE)
	{
		m_dwDelayTime = 0;
		m_bShow = FALSE;
		cDialog::SetActive(FALSE);

	}

	if( m_bShow )
	{
		cDialog::Render();
	}
}

void CScreenShotDlg::Setserverti(SYSTEMTIME servertime)
{
	serverti = servertime;
}
#include "ChatManager.h"
#include "FreeImage/FreeImage.h"
void CScreenShotDlg::TakeSnapShot()
{
	DIRECTORYMGR->SetLoadMode(eLM_Root);
	CreateDirectory( "ScreenShot", NULL );
	WORD wSrv = 0, wY = 0, wM = 0, wD = 0, wN = 0;
	WORD wHH = 0, wMM = 0, wSS = 0;		
	char fname[256];
	//char fname2[256];
	SYSTEMTIME ti;
	GetLocalTime( &ti );
	HANDLE hFile = CreateFile( "ini/ScreenShot.cnt", GENERIC_READ, 0, NULL, 
				OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );
	DWORD dwSize = 0;
	if( hFile != INVALID_HANDLE_VALUE )
	{
		ReadFile( hFile, &wSrv, sizeof(wSrv),	&dwSize, NULL );
		ReadFile( hFile, &wY,	sizeof(wY),		&dwSize, NULL );
		ReadFile( hFile, &wM,	sizeof(wM),		&dwSize, NULL );
		ReadFile( hFile, &wD,	sizeof(wD),		&dwSize, NULL );
		ReadFile( hFile, &wHH,	sizeof(wHH),	&dwSize, NULL );
		ReadFile( hFile, &wMM,	sizeof(wMM),	&dwSize, NULL );
		ReadFile( hFile, &wSS,	sizeof(wSS),	&dwSize, NULL );
		ReadFile( hFile, &wN,	sizeof(wN),		&dwSize, NULL );
		CloseHandle( hFile );
		if( !( wSrv == (WORD)g_nServerSetNum && ti.wYear == wY && ti.wMonth == wM && ti.wDay == wD 
			&& ti.wHour == wHH && ti.wMinute == wMM && ti.wSecond == wSS) )
		{
			wSrv = g_nServerSetNum; wY = ti.wYear; wM = ti.wMonth; wD = ti.wDay; wN = 1; 
			wHH = ti.wHour; wMM = ti.wMinute; wSS = ti.wSecond;
		}					
	}
	else
	{
		wSrv = g_nServerSetNum; wY = ti.wYear; wM = ti.wMonth; wD = ti.wDay; wN = 1; 
		wHH = ti.wHour; wMM = ti.wMinute; wSS = ti.wSecond;
	}

	sprintf(fname,"ScreenShot/DarkStory_%02d_%02d%02d%02d_%02d%02d%02d_%03d.jpg", wSrv, wY%100, wM, wD, wHH, wMM, wSS, wN );
	//sprintf(fname2,"ScreenShot/DarkStory_%02d_%02d%02d%02d_%02d%02d%02d_%03d.txt", wSrv, wY%100, wM, wD, wHH, wMM, wSS, wN );

	BOOL rt = g_pExecutive->GetRenderer()->CaptureScreen(fname);
	if( rt )
	{
		hFile = CreateFile( "ini/ScreenShot.cnt", GENERIC_WRITE, 0, NULL, 
						CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );
		if( hFile != INVALID_HANDLE_VALUE )
		{
			++wN;
			WriteFile( hFile, &wSrv, sizeof(wSrv), &dwSize, NULL );
			WriteFile( hFile, &wY, sizeof(wY), &dwSize, NULL );
			WriteFile( hFile, &wM, sizeof(wM), &dwSize, NULL );
			WriteFile( hFile, &wD, sizeof(wD), &dwSize, NULL );
			WriteFile( hFile, &wHH, sizeof(wHH), &dwSize, NULL );
			WriteFile( hFile, &wMM, sizeof(wMM), &dwSize, NULL );
			WriteFile( hFile, &wSS, sizeof(wSS), &dwSize, NULL );
			WriteFile( hFile, &wN, sizeof(wN), &dwSize, NULL );							
			CloseHandle( hFile );						
		}
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(201), fname );
	}
	else
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(200) );
	}
	FIBITMAP* pBitmap = FreeImage_Load(FIF_TARGA, fname, TARGA_DEFAULT);
	if(pBitmap)
	{
		DeleteFile(fname);
		int len = strlen(fname);
		fname[len-3] = 'j';
		fname[len-2] = 'p';
		fname[len-1] = 'g';
		FreeImage_SaveBMP(pBitmap, fname, BMP_DEFAULT);
		//FreeImage_SaveJPEG(pBitmap,fname,JPEG_QUALITYSUPERB);

		//BYTE* data = FreeImage_GetBits(pBitmap);					
		//CHATMGR->AddMsg( CTC_SYSMSG, "byte %d" , data );

		//unsigned char * bits = FreeImage_GetBits(pBitmap);
		//CHATMGR->AddMsg( CTC_SYSMSG, "char %u" , bits );

		//BYTE* data2;

		//FreeImage_ConvertToRawBits(data,pBitmap,3*1024,24,
		//							0xFF0000,0x0000FF,255,false);
		//CHATMGR->AddMsg( CTC_SYSMSG, "%d" , data2 );

		/*unsigned char *image;
		image = (unsigned char*)malloc((3*1024*768)*sizeof(char));
		FIBITMAP* leftImage = FreeImage_ConvertFromRawBits(image,1024,768,3*1024,24,
									0xFF0000,0x0000FF,255,false);
		FreeImage_Save(FIF_BMP, leftImage, "ScreenShot/left.bmp", 0);


		FIBITMAP* rightImage  = FreeImage_ConvertFromRawBits(image,1024,768,3*1024,24,
									0xFF0000,0x0000FF,255,false);
		FreeImage_Save(FIF_BMP, rightImage, "ScreenShot/right.bmp", 0);
		free(image);*/

		FreeImage_Unload(pBitmap);					
	}
}