#include "stdafx.h"
#include "BigMapDlg.h"
#include "MHFile.h"
#include "GameIn.h"
#include "Npc.h"
#include "./Interface/cScriptManager.h"
#include "./Interface/cWindowManager.h"
#include "./Input/Mouse.h"
#include "./MiniMapDlg.h"
#include "MHMap.h"
#include "MoveManager.h"
#include "MouseCursor.h"
//#include "AutoFindPathManager.h"
#include "./Input/UserInput.h"
#include "OptionManager.h"
#include "cSpin.h"
#include "OptionDialog.h"
#include "WindowIDEnum.h"
#include "autopath.h"
#include "AIManager.h"
#ifdef _TESTCLIENT_
#include "GameResourceManager.h"
#endif
#define Speed	20
#define Inline	27
static float dwTestMini[Inline]={0.5,6.5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,4,4,3,3,2,2,1.5,1,0.5
};
DWORD BIGMAPICON_DESC[] = 
{
	13,	15,	19,			
	12,	15,	15,			
	25,	20,	20,			
	26,	20,	20,			
	16,	20,	20,			
	14,	20,	20,			
	17,	20,	20,			
	19,	20,	20,			
	18,	20,	20,			
	23,	20,	20,			
	20,	20,	20,			
	22,	20,	20,			
	15,	20,	20,			
	21,	20,	20,			
	33, 20, 20,			
	24,	20,	20,			
	27,	26,	26,			
	28,	26,	26,			
	29,	26,	26,			
	34, 21, 48,		

	35, 11, 13,			//eMNMICON_NEW_17
	36, 11, 14,			//eMNMICON_NEW_18 23
	37, 20, 22,			//eMNMICON_NEW_19 
	38, 20, 22,			//eMNMICON_NEW_20
	39, 33, 42,			//eMNMICON_NEW_21 
};
#define BIGMAP_WIDTH	(512)
#define BIGMAP_HEIGHT	(512)
CBigMapDlg::CBigMapDlg()
{
	m_IconTable.Initialize(64);
	m_IconPool.Init(100, 10, "MinimapIconPool");
	m_dwImageRGB = RGBA_MAKE(255,255,255,120);
	btnPath=NULL;

	//m_PathFlag = new cImageSelf;
	//m_PathFlag->LoadSprite( "image/2d/flag.tif"/*, 64, 64 */);
}
CBigMapDlg::~CBigMapDlg()
{
	//SAFE_DELETE(m_PathFlag);
	m_MapImage.Release();
	CMiniMapIcon* pIcon;
	m_IconTable.SetPositionHead();
	while(pIcon = m_IconTable.GetData())
		ReleaseMinniMapIcon(pIcon);
	m_IconTable.RemoveAll();
}
void CBigMapDlg::Linking()
{
	memset(m_pIconImage,0,sizeof(cImage*)*eBMICON_MAX);
	InitIconImage();
	btnPath = new cButton; 
	if( btnPath )
	{
		btnPath->Init( 0, 0, 512, 512, NULL,NULL,NULL,NULL, -1 );
		Add( btnPath );
	}
	btnPath->SetActive(TRUE);
}
void CBigMapDlg::LoadMinimapImageInfo( MAPTYPE MapNum )
{
	DIRECTORYMGR->SetLoadMode(eLM_Minimap);
	char descFile[64];
	char MiniMapImageFilename[256];
#ifdef _TESTCLIENT_
	MapNum = GAMERESRCMNGR->m_TestClientInfo.Map;
#endif
	CMHFile file;
#ifdef _FILE_BIN_
	sprintf(descFile,"Minimap%d.bin",MapNum);	
	file.Init(descFile,"rb",MHFILE_FLAG_DONOTCHANGE_DIRECTORY_TOROOT);
#else
	sprintf(descFile,"Minimap%d.txt",MapNum);	
	file.Init(descFile,"r",MHFILE_FLAG_DONOTCHANGE_DIRECTORY_TOROOT);
#endif
	if(file.IsInited() == FALSE)
	{
		DIRECTORYMGR->SetLoadMode(eLM_Root);
		return;
	}
	for( int i = 0 ; i <eMINIMAPMODE_MAX ; ++i )
	{
		file.GetString( MiniMapImageFilename );
		if( file.IsEOF() )
			break;
		file.GetDword();
		file.GetDword();
		if( i == eMINIMAPMODE_SMALL )
		{
			m_MapImage.LoadSprite( MiniMapImageFilename );
			cImageRect rect = { 0, 0, BIGMAP_WIDTH, BIGMAP_HEIGHT };
			m_MapImage.SetImageSrcRect(&rect);
		}
	}
	file.Release();	
	DIRECTORYMGR->SetLoadMode(eLM_Root);
}
void CBigMapDlg::InitBigMap( MAPTYPE MapNum )
{
	if( GAMEIN->GetGameInInitKind() == eGameInInitKind_SuryunEnter ||
		GAMEIN->GetGameInInitKind() == eGameInInitKind_EventMapEnter)
	{
		SetActive(FALSE);
		if (MapNum == 23 || MapNum == 24 || MapNum == 26 || MapNum == 29 )
		{
			SetActive(TRUE);
		}
	}
	BOOL rt = MAP->IsMapKind(eMapView, MapNum);
	if( FALSE == rt )
		SetActive(FALSE);
	if( MAP->IsMapKind(eSurvivalMap) )
	{
		SetActive(FALSE);
		if (MapNum == 23 || MapNum == 24 || MapNum == 26 || MapNum == 29 )
		{
			SetActive(TRUE);
		}
	}
	m_MapNum = MapNum;
	LoadMinimapImageInfo( MapNum );
}
void CBigMapDlg::AddStaticNpcIcon(CObject* pObject,BYTE bFlag)
{
	CNpc* pNpc = (CNpc*)pObject;
	WORD kind = pNpc->GetNpcKind();
	switch( kind )
	{
	case 1:		AddIcon(eBMICON_WEAPON,pObject);	break;
	case 2:		AddIcon(eBMICON_DRESS,pObject);		break;
	case 3:		AddIcon(eBMICON_ACCESSARY,pObject);	break;
	case 4:		AddIcon(eBMICON_DOCTOR,pObject);	break;
	case 5:		AddIcon(eBMICON_BOOK,pObject);		break;
	case 8:		AddIcon(eBMICON_MUNPA,pObject);		break;
	case 10:
	case 84:
		AddIcon(eBMICON_WAREHOUSE,pObject);
		break;
	case 11:	AddIcon(eBMICON_CASTLEMANAGER, pObject);	break;
	case 12:	AddIcon(eBMICON_PYOGUK,pObject);	break;
	case 13:	AddIcon(eBMICON_GWANJOL,pObject);	break;
	case 15:	AddIcon(eBMICON_YEONMU,pObject);	break;

	//case 16:	AddIcon(eBMICON_NEW_21,pObject);	break;
	case 17:	
		{
			if(bFlag==1)
				AddIcon(eBMICON_NEW_22,pObject);
			if(bFlag==2)
				AddIcon(eBMICON_NEW_23,pObject);
		}break;


	case 19:
	case 20:
	case 21:
	case 22:
	case 23:
	case 24:
	case 25:
	case 26:
	case 39:
	case 43:
	case 44:
	case 45:
	case 48:
	case 49:
	case 50:
	case 51:
	case 52:
	case 53:
	case 54:
	case 55:
	case 56:
	case 57:
	case 58:
	case 59:
	case 71:
	case 72:
	case 74:
	case 75:
	case 76:
	case 77:
	case 78:
	case 79:
	case 80:
		AddIcon(eBMICON_ETC,pObject);
		break;		
	case 31:
	case 81:
	case 82:	
		AddIcon(eBMICON_MAPCHANGE,pObject);
		break;
	case 30:
	case 83:
		AddIcon(eBMICON_LOGIN,pObject);
		break;
	default:
		AddIcon(eBMICON_ETC,pObject);
		break;
	}
}
void CBigMapDlg::AddPartyMemberIcon(PARTY_MEMBER* pMember)
{
	AddIcon(eBMICON_PARTYMEMBER,pMember);
}
void CBigMapDlg::AddHeroIcon(CObject* pObject)
{
	m_HeroIcon.Init(&m_pIconImage[eBMICON_SELF],pObject, eBMICON_SELF);
	m_HeroIcon.SetRGBColor( RGBA_MAKE(255,255,255,220) );
	m_HeroIcon.SetAlwaysTooltip( TRUE );
	m_HeroIcon.SetCameraImage( &m_pIconImage[eBMICON_CAMERA] );
}
CMiniMapIcon* CBigMapDlg::GetEmptyMiniMapIcon(WORD Kind,CObject* pObject)
{
	CMiniMapIcon* pIcon = m_IconPool.Alloc();
	pIcon->Init(&m_pIconImage[Kind],pObject, Kind);
	pIcon->SetMarkImage(&m_pIconImage[eBMICON_QUEST_EXCLAMATION1],NULL);
	return pIcon;
}
CMiniMapIcon* CBigMapDlg::GetEmptyMiniMapIcon(WORD Kind,PARTY_MEMBER* pMember)
{
	CMiniMapIcon* pIcon = m_IconPool.Alloc();
	pIcon->Init(&m_pIconImage[Kind], pMember, Kind);
	return pIcon;
}
void CBigMapDlg::ReleaseMinniMapIcon(CMiniMapIcon* pIcon)
{
	m_IconPool.Free(pIcon);
}
void CBigMapDlg::AddIcon(WORD Kind,CObject* pObject)
{
	ASSERT(m_IconTable.GetData(pObject->GetID()) == NULL);
	RemoveIcon(pObject->GetID());
	CMiniMapIcon* pIcon = GetEmptyMiniMapIcon(Kind,pObject);
	m_IconTable.Add(pIcon,pObject->GetID());
	pIcon->SetRGBColor( RGBA_MAKE(255,255,255,220) );
	pIcon->SetAlwaysTooltip( TRUE );
}
void CBigMapDlg::AddIcon(WORD Kind,PARTY_MEMBER* pMember)
{
	ASSERT(m_IconTable.GetData(pMember->dwMemberID) == NULL);
	RemoveIcon(pMember->dwMemberID);
	CMiniMapIcon* pIcon = GetEmptyMiniMapIcon(Kind,pMember);
	m_IconTable.Add(pIcon,pMember->dwMemberID);
	pIcon->SetRGBColor( RGBA_MAKE(255,255,255,220) );
	pIcon->SetAlwaysTooltip( TRUE );
}
void CBigMapDlg::RemoveIcon(DWORD ObjectID)
{
	CMiniMapIcon* pIcon = (CMiniMapIcon*)m_IconTable.GetData(ObjectID);
	if(pIcon == NULL)
		return;
	ReleaseMinniMapIcon(pIcon);
	m_IconTable.Remove(ObjectID);
}
void CBigMapDlg::Render()
{
	if( !m_bActive ) return;
	if( m_MapImage.IsNull() ) return;
	cDialog::RenderWindow();
	DWORD color = RGBA_MAKE(255,255,255,130);
	VECTOR2 vScale = { BIGMAP_WIDTH/512.f, BIGMAP_HEIGHT/512.f };
	m_MapImage.RenderSprite( &vScale, NULL, 0, &m_absPos, color );
	CMiniMapIcon* pIcon;
	m_IconTable.SetPositionHead();
	while(pIcon = m_IconTable.GetData())
	{
		pIcon->Render( &m_absPos, NULL );
		//pIcon->RenderQuestMark(&m_absPos,NULL);//kiv
	}
	m_IconTable.SetPositionHead();
	while(pIcon = m_IconTable.GetData())
	{
		pIcon->RenderQuestMark(&m_absPos,NULL);
	}
	m_HeroIcon.Render(&m_absPos);
	if( m_pIconForToolTip )
		m_pIconForToolTip->Render( &m_absPos, NULL );

	if(m_IsPointRender)
	{
		VECTOR2 trans;
		static int nn=2;
		trans.x = GetAbsX()+ BIGMAP_WIDTH/512.f-8;
		trans.y = GetAbsY()+ BIGMAP_WIDTH-50+ dwTestMini[nn];
		trans.x += m_PointPosSave.x;
		trans.y -= m_PointPosSave.y;

		cImage	m_JackIcon;
		SCRIPTMGR->GetImage( 133, &m_JackIcon, PFT_JACKPATH );
		m_JackIcon.RenderSprite( 0, NULL, 0.0f, &trans,0xffffffff);

		//m_PathFlag->RenderSprite( 0, NULL, 0, &trans, 0xffffffff );
		static DWORD dwSpecialTime = gCurTime;	
		if( gCurTime - dwSpecialTime > Speed )
		{
			nn = (++nn)%Inline;
			dwSpecialTime = gCurTime;
		}
	}
	cDialog::RenderComponent();
	//MAP->RenderTileData_Debug();
}
void CBigMapDlg::SetPartyIconObjectPos(DWORD PlayerID, int posX, int posZ)
{
	CMiniMapIcon* pIcon = m_IconTable.GetData(PlayerID);
	if(pIcon)
	{
		pIcon->SetObjectPos(posX, posZ);
		pIcon->SetTargetPos(posX, posZ);
	}
}
void CBigMapDlg::SetPartyIconTargetPos(DWORD PlayerID, int posX, int posZ)
{
	CMiniMapIcon* pIcon = m_IconTable.GetData(PlayerID);
	if(pIcon)
		pIcon->SetTargetPos(posX, posZ);
}
BOOL CBigMapDlg::CanActive()
{
	return MAP->IsMapKind(eMapView);
}
void CBigMapDlg::InitIconImage()
{
	cImage* pImg;
	DWORD w,h;
	for(int n=0;n<eBMICON_MAX;++n)
	{
		pImg = new cImage;
		SCRIPTMGR->GetImage(BIGMAPICON_DESC[3*n+0],pImg,PFT_MINIMAPPATH);
		w = BIGMAPICON_DESC[3*n+1];
		h = BIGMAPICON_DESC[3*n+2];
		m_pIconImage[n].InitIconImage(pImg,w,h);
	}
	SCRIPTMGR->GetImage( 63, &m_TooltipImage, PFT_HARDPATH );
	m_pIconForToolTip = NULL;
}
void CBigMapDlg::Process()
{
	m_HeroIcon.Update(BIGMAP_WIDTH,BIGMAP_HEIGHT);
	CMiniMapIcon* pIcon;
	m_IconTable.SetPositionHead();
	while(pIcon = m_IconTable.GetData())
	{
		pIcon->Update(BIGMAP_WIDTH,BIGMAP_HEIGHT);
	}
}
DWORD CBigMapDlg::ActionEvent( CMouse* mouseInfo )
{
	if( !m_bActive ) return WE_NULL;
	DWORD we = WE_NULL;
	if( !m_bActive || m_bDestroy ) return we;
	if( !WINDOWMGR->IsMouseOverUsed() && PtInWindow(mouseInfo->GetMouseX(), mouseInfo->GetMouseY() ) )
	{
		if( !(WINDOWMGR->IsDragWindow() && (cWindow*)WINDOWMGR->GetDragDlg() == this ) )
		{
			m_dwViewCurTime = GetTickCount();
			if(m_dwViewCurTime - m_dwViewLastTime > TOOLTIP_DELAY_TIME)
			{
				m_ttX = mouseInfo->GetMouseX();
				m_ttY = mouseInfo->GetMouseY();
			}
		}
	}
	else
	{
		m_dwViewLastTime = m_dwViewCurTime = 0;
	}
	if( !m_bDisable )
		we |= ActionEventWindow(mouseInfo);
	we |= ActionEventComponent(mouseInfo);
	{
		if (!WINDOWMGR->IsMouseOverUsed() && PtInWindow(mouseInfo->GetMouseX(), mouseInfo->GetMouseY()))
		{
			char temp[64] = { 0, };
			int x = mouseInfo->GetMouseX();
			int y = mouseInfo->GetMouseY();
			VECTOR2 trans;
			trans.x = GetAbsX();
			trans.y = GetAbsY();
			CMiniMapIcon* pIcon;
			float dist;
			float olddist = 10000.0f;
			float  pectentX = 1.0;
			float  pectentY = 1.0;
			m_PointPos.x = (int)((mouseInfo->GetMouseX() - GetAbsX())*pectentX) / .01f / 100;
			m_PointPos.y = (int)((GetAbsY() + 510 - mouseInfo->GetMouseY())*pectentY) / .01f / 100;
			VECTOR2 * D2Point = GAMEIN->GetBigMapDialog()->GetPointPos();
			if (D2Point)
			{
				vDestPos.x = D2Point->x * 100;
				vDestPos.z = D2Point->y * 100;
				vDestPos.y = 0;
				vDestPos.x = (int(vDestPos.x / 51.2f) + 0.5f) * 51.2f;
				vDestPos.y = 0;
				vDestPos.z = (int(vDestPos.z / 51.2f) + 0.5f) * 51.2f;			
			}

			DWORD dwFontColor;
			if (MAP->CollisionCheck_OneTile(&vDestPos) == FALSE)
			{
				dwFontColor = RGB_HALF(255, 20, 20);
				CURSOR->SetCursor(eCURSOR_CANTMOVE);
			}
			else
			{
				dwFontColor = RGB_HALF(20, 255, 20);
				CURSOR->SetCursor(eCURSOR_DEFAULT);
			}

			ZeroMemory(&temp, sizeof(temp));
			sprintf(temp, "%1.0f,%1.0f", m_PointPos.x, m_PointPos.y);
			SetToolTip(temp, dwFontColor, &m_TooltipImage, RGBA_MERGE(RGB_HALF(0, 0, 0), 100));

			m_pIconForToolTip = NULL;
			m_IconTable.SetPositionHead();
			while (pIcon = m_IconTable.GetData())
			{
				int sort = pIcon->GetToolTip(x, y, &trans, temp, &dist);
				if (sort == 1)
				{
					if (dist < olddist)
					{
						m_pIconForToolTip = pIcon;
						olddist = dist;
						SetToolTip(temp, RGB_HALF(255, 255, 255), &m_TooltipImage, RGB_HALF(0, 0, 0));
						SetWH(BIGMAP_WIDTH, BIGMAP_HEIGHT);
					}
				}
				else if (sort == 2)
				{
					if (dist < olddist)
					{
						m_pIconForToolTip = pIcon;
						olddist = dist;
						SetToolTip(temp, RGB_HALF(185, 241, 69), &m_TooltipImage, RGB_HALF(0, 0, 0));
						SetWH(BIGMAP_WIDTH, BIGMAP_HEIGHT);
					}
				}
			}
			if (temp[0] == 0)
			{
				SetToolTip("");
				SetWH(0, 0);
			}
		}
	}
	if( we & WE_LBTNCLICK )
	{
		if(KEYBOARD->GetKeyPressed(KEY_X) )
		{
			MOVE_POS msg;
			msg.Category = MP_CHEAT;
			msg.Protocol = MP_CHEAT_MOVE_SYN;
			msg.dwObjectID = HEROID;
			msg.dwMoverID = HEROID;
			msg.cpos.Compress(&vDestPos);
			NETWORK->Send(&msg,sizeof(msg));
			return we;
		}

		if(sCPlayerAI.IsOpen())
			return we;
		if (MAP->CollisionCheck_OneTile(&vDestPos) == TRUE)
		{
			float  pectentX = 1.0;
			float  pectentY = 1.0;
			m_PointPosSave.x =(int)((mouseInfo->GetMouseX()-GetAbsX())*pectentX)/.01f/100;
			m_PointPosSave.y= (int)((GetAbsY()+510-mouseInfo->GetMouseY())*pectentY)/.01f/100;
			m_IsPointRender=TRUE;

			/*if(KEYBOARD->GetKeyPressed(KEY_SHIFT))
			{
				//HERO->GetPosition(&m_PointPosSave);
				OPTIONMGR->GetGameAI()->sPage5.spPosX=(int)(m_PointPosSave.x);
				OPTIONMGR->GetGameAI()->sPage5.spPosY=(int)(m_PointPosSave.y);
				char TempPosX[10];
				char TempPosZ[10];
				itoa(OPTIONMGR->GetGameAI()->sPage5.spPosX,TempPosX,10);
				itoa(OPTIONMGR->GetGameAI()->sPage5.spPosY,TempPosZ,10);
				COptionDialog * pDlg = GAMEIN->GetOptionDialog();
				cDialog* m_Dlg =(cDialog*)pDlg->GetTabSheet(4);
				if (m_Dlg)  
				{
					((cSpin*)m_Dlg->GetWindowForID(  OTI_SP_AUTOHUNT_LOCA_X ))->SetEditText(TempPosX);
					((cSpin*)m_Dlg->GetWindowForID(  OTI_SP_AUTOHUNT_LOCA_Y ))->SetEditText(TempPosZ);
				}
				return we;
			}*/

			GAMEIN->GetMiniMapDialog()->SetNewMovePoint(m_PointPosSave);
			GAMEIN->GetMiniMapDialog()->SetMovePoint();

			//if(!sCPlayerAI.IsOpen())
				Move_UsePath();
		}
	}
	return we;
}
void CBigMapDlg::Move_UsePath()
{
	static DWORD dwMouseMoveTick;
	if( gCurTime - dwMouseMoveTick >= 400 )
	{
		dwMouseMoveTick = gCurTime;
		//MOVEMGR->SetHeroTarget( &vDestPos, 1.f );

		if(MOVEMGR->SetHeroTarget(&vDestPos, 1.f))
		{
			//AUTOPATH->SetTargetPos(vDestPos, TRUE);
		}
		//AUTOPATH->SetTargetPos(&vDestPos,TRUE);
	}
}
void CBigMapDlg::ShowQuestMarkIcon( CObject* pTarget, int nKind )
{
	CMiniMapIcon* pIcon = m_IconTable.GetData( pTarget->GetID() );
	if( pIcon == NULL ) return;
	pIcon->ShowQuestMark( nKind );
}
void CBigMapDlg::SetActive( BOOL val )
{
	if( CanActive() == FALSE )
		val = FALSE;
	cDialog::SetActive( val );
	if( GAMEIN->GetMiniMapDialog() )
		GAMEIN->GetMiniMapDialog()->RefreshMode();
}