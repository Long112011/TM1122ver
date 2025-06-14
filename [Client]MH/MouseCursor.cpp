









// MouseCursor.cpp: implementation of the CMouseCursor class.
//
//////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "MouseCursor.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
GLOBALTON(CMouseCursor)



CMouseCursor::CMouseCursor()
{

}

CMouseCursor::~CMouseCursor()
{

}


void CMouseCursor::Init( HINSTANCE hInstance )
{
//	ShowCursor(TRUE);

//	m_hCursor[eCURSOR_DEFAULT] = LoadCursorFromFile("Cursor/default.cur");
//	m_hCursor[eCURSOR_ATTACK] = LoadCursorFromFile("Cursor/attack.cur");
//	m_hCursor[eCURSOR_ITEMSELECT] = LoadCursorFromFile("Cursor/ItemSelect.cur");
//	m_hCursor[eCURSOR_DEALER] = LoadCursorFromFile("Cursor/dealer.cur");
//	m_hCursor[eCURSOR_DONTMOVE] = LoadCursorFromFile("Cursor/dontmove.cur");

	m_bActive = FALSE;

	m_hCursor[eCURSOR_DEFAULT]		= LoadCursorFromFile("./image/mouse/default.cur");


	m_hCursor[eCURSOR_ATTACK]		= LoadCursorFromFile("./image/mouse/attack.cur");
	m_hCursor[eCURSOR_ITEMSELECT]	= LoadCursorFromFile("./image/mouse/itemsel.cur");
	m_hCursor[eCURSOR_ITEMDRAGGING]	= LoadCursorFromFile("./image/mouse/itemsel.cur");
	m_hCursor[eCURSOR_DEALER]		= LoadCursorFromFile("./image/mouse/dealer.cur");
	m_hCursor[eCURSOR_CANTMOVE]		= LoadCursorFromFile("./image/mouse/cantmove.cur");
	m_hCursor[eCURSOR_DEFAULTCHEAT] = LoadCursorFromFile("./image/mouse/defaultcheat.cur");
	m_hCursor[eCURSOR_TITANREPAIR]	= LoadCursorFromFile("./image/mouse/repair.cur");	// magi82 - Titan(070511) Ÿ��ź Ŀ���� ��� �ӽ÷� �ƹ��ų� ����
	m_hCursor[eCURSOR_MINING]	= LoadCursorFromFile("./image/mouse/Mining.cur");			// 2007. 7. 11. CBH - ä����ų Ŀ�� �߰�
	m_hCursor[eCURSOR_COLLECTION]	= LoadCursorFromFile("./image/mouse/Collection.cur");	// 2007. 7. 11. CBH - ä����ų Ŀ�� �߰�
	m_hCursor[eCURSOR_HUNT]	= LoadCursorFromFile("./image/mouse/Hunt.cur");					// 2007. 7. 11. CBH - ��ɽ�ų Ŀ�� �߰�
	m_hCursor[eCURSOR_PLUS] = LoadCursorFromFile("./image/mouse/blueplus.cur");
	

	m_curCursor = eCURSOR_DEFAULT;
	
	m_bCheatCursor = FALSE;
}

void CMouseCursor::Release()
{
//	ClipCursor(&m_rcOldClip);
/*
//	BOOL b;
	DeleteObject( m_hCursor[eCURSOR_DEFAULT] );
//	b =
	DeleteObject( m_hCursor[eCURSOR_ATTACK] );
//	b =
	DeleteObject( m_hCursor[eCURSOR_ITEMSELECT] );
//	b =
	DeleteObject( m_hCursor[eCURSOR_ITEMDRAGGING] );	

//	b =
	DeleteObject( m_hCursor[eCURSOR_DEALER] );
//	b =
	DeleteObject( m_hCursor[eCURSOR_CANTMOVE] );
//	b =
	DeleteObject( m_hCursor[eCURSOR_DEFAULTCHEAT] );

*/
}


void CMouseCursor::Process()
{
	::SetCursor(m_hCursor[m_curCursor]);
}

void CMouseCursor::SetCursor( eCURSOR cursor )

{
#ifdef _GMTOOL_
//	if( CURSOR->GetCursor() == eCURSOR_DEFAULTCHEAT )
//		return;

	if( m_bCheatCursor )
		return;
#endif

	if( m_curCursor == cursor ) return;

	
	m_curCursor = cursor;

	::SetCursor(m_hCursor[m_curCursor]);
}

void CMouseCursor::SetActive( BOOL bActive )
{


	if( m_bActive == bActive ) return;	//showcursor�� ���� ���ڷ� �� ȣ��Ǵ°� ����
										//show, hide�� �����ȴ�. �׷��Ƿ� �ݺ�ȣ��Ǹ� 
										//Ŀ���� �Ⱥ��̰ų� �Ⱦ������� ��찡 �ִ�.
	m_bActive = bActive;
	if( bActive )
		::SetCursor(m_hCursor[m_curCursor]);
	
	::ShowCursor( bActive );
}


void CMouseCursor::SetCheatMode( BOOL bCheatMode )

{
	if( m_bCheatCursor == bCheatMode )
		return;

	if( bCheatMode )
		SetCursor(eCURSOR_DEFAULTCHEAT);
	else

		SetCursor(eCURSOR_DEFAULT);

	m_bCheatCursor = bCheatMode;

	SetActive( TRUE );
}






