#include "stdafx.h"
#include "MHMap.h"
#include "GameResourceManager.h"
#include "TileManager.h"
#include "MHFile.h"
#include "ObjectManager.h"
#include "MainGame.h"
#include "Engine/Engine.h"
#include "GameIn.h"
#include "./Audio/MHAudioManager.h"
#include "MHCamera.h"
#include "MiniMapDlg.h"
#include "QuestManager.h"
#include "ItemManager.h"
#include "GameResourceManager.h"
#include "CWayPointManager.h"
#include "BigMapDlg.h"
#include "MiniMapDlg.h"
#include "aimanager.h"
#include "AiSetManager.h"
#include "MHTimeManager.h"
#include "ChatManager.h"
DWORD WINAPI PathFind_Thread(LPVOID p_Param);
VECTOR3 g_PathFindStart;
VECTOR3 g_PathFindEnd;
CTileManager * g_TileManager = NULL;
CWayPointManager* g_WayPointManager = NULL;
BOOL g_SimpleMode = TRUE;
volatile LONG g_PathThreadActivated = 0;
int p = 0;
static DWORD k = 0;
DWORD m_dwViewCurTime;
DWORD m_dwViewLastTime;
GLOBALTON(CMHMap)
CMHMap::CMHMap()
{
	sky = NULL;
	m_pTileManager = NULL;
	mapDesc = NULL;
	m_bIsInited = FALSE;
	m_CloudTable.Initialize(64);
	
	m_bVillage = FALSE;

	m_dwQuestNpcId = 1000;

	m_iMapKind = eNormalMap;

	m_bRenderTileData = FALSE;
}

CMHMap::~CMHMap()
{
	SAFE_DELETE(m_pTileManager);
//	Release();
	SAFE_DELETE(mapDesc);

	//std::vector<VECTOR3>().swap(m_Path_Debug);
	//std::deque<VECTOR3>().swap(m_Path_Optimize_Debug);
}
void CMHMap::TestUji()
{

	map.TestCreateTile();
}
void CMHMap::InitMap(MAPTYPE MapNum)
{
	m_dwQuestNpcId = 1000;
	m_bIsInited = TRUE;
	m_MapNum = MapNum;
	SetMapKind(MapNum);
	LoadMapDesc(MapNum);
	DIRECTORYMGR->SetLoadMode(eLM_Map);
	{
		BOOL rt;
		rt = map.Init(mapDesc->MapFileName);
		SAFE_DELETE(sky);
		if(mapDesc->bSky)
		{
			sky = new CEngineSky;
			sky->CreateSky(mapDesc->SkyMod,mapDesc->SkyAnm, &mapDesc->SkyOffset);
		}
		DIRECTORYMGR->SetLoadMode(eLM_Resource);
		SAFE_DELETE(m_pTileManager);
		m_pTileManager = new CTileManager;
		if(m_pTileManager->LoadTileInfo(mapDesc->TileFileName) == FALSE)
			SAFE_DELETE(m_pTileManager);		
	}
	DIRECTORYMGR->SetLoadMode(eLM_Root);
	ApplyMapDesc();
	LoadPreData(MapNum);
}

/*
void CMHMap::PreLoadData()
{
	BASE_MONSTER_LIST* pMonsterInfo = GAMERESRCMNGR->GetMonsterListInfo(13);

}
*/

void CMHMap::LoadPreData(MAPTYPE MapNum)
{	
	sPRELOAD_INFO* pPreLoadInfo = GAMERESRCMNGR->GetPreDataInfo(MapNum);
	if(!pPreLoadInfo)		return;
	
	//
	BASE_MONSTER_LIST* pBMonInfo = NULL;

	//----------------------------------------------------------------
	// Monster
	//----------------------------------------------------------------
	DIRECTORYMGR->SetLoadMode(eLM_Monster);

	for(int i=0; i<pPreLoadInfo->Count[ePreLoad_Monster]; i++)
	{
		pBMonInfo = GAMERESRCMNGR->GetMonsterListInfo(pPreLoadInfo->Kind[ePreLoad_Monster][i]);
		if(pBMonInfo)
			CEngineObject::PreLoadObject(pBMonInfo->ChxName);
	}

	//----------------------------------------------------------------
	// Item
	//----------------------------------------------------------------
	DIRECTORYMGR->SetLoadMode(eLM_Character);

	MOD_LIST* pModList_Man = &GAMERESRCMNGR->m_ModFileList[GENDER_MALE];
	MOD_LIST* pModList_Woman = &GAMERESRCMNGR->m_ModFileList[GENDER_FEMALE];
	StaticString* pString = NULL;
	int PartModelNum = 0;
	for(int i=0; i<pPreLoadInfo->Count[ePreLoad_Item]; i++)
	{		
		PartModelNum = ITEMMGR->GetItemInfo(pPreLoadInfo->Kind[ePreLoad_Item][i])->Part3DModelNum;

		// Man
		pString = &pModList_Man->ModFile[PartModelNum];
		if(pString == 0)
		{
			char buf[128];
			sprintf(buf, "No Name ModList! Plz Check again.ItemNum : %d", pPreLoadInfo->Kind[ePreLoad_Item][i]);
			ASSERTMSG(0, buf);
		}				
		if(pString->Str)
			CEngineObject::PreLoadObject(pString->Str);

		// Woman
		pString = &pModList_Woman->ModFile[PartModelNum];
		if(pString->Str)
			CEngineObject::PreLoadObject(pString->Str);
	}
	
	//----------------------------------------------------------------
	// Mod
	//----------------------------------------------------------------
	PRELOAD* pMod = NULL;
	cPtrList* pList = GAMERESRCMNGR->GetPreLoadModlist();

	PTRLISTPOS pos = pList->GetHeadPosition();
	while(pos)


	{
		pMod = (PRELOAD*)pList->GetNext(pos);
		if(pMod)
		{
			CEngineObject::PreLoadObject(pMod->FileName.Str);
		}
	}

	//----------------------------------------------------------------
	// Effect
	//----------------------------------------------------------------
	DIRECTORYMGR->SetLoadMode(eLM_Effect);

	PRELOAD* pEff = NULL;
	pList = GAMERESRCMNGR->GetPreLoadEfflist();

	pos = pList->GetHeadPosition();
	while(pos)
	{
		pEff = (PRELOAD*)pList->GetNext(pos);
		if(pEff)
		{
			g_pExecutive->PreLoadGXObject(pEff->FileName.Str);			
		}
	}

	///////////////////////////////////////////////////////////////////////////////
	// 06. 04. PreLoadData 추가기능 - 이영준
	// 아이템 PreLoad
	WORD* ItemList = GAMERESRCMNGR->GetPreLoadItemlist();

	if(ItemList)
	{
		WORD count = ItemList[0];

		for(WORD cnt = 1; cnt < count + 1; cnt++)
		{
			PartModelNum = ITEMMGR->GetItemInfo(ItemList[cnt])->Part3DModelNum;

			// Man
			pString = &pModList_Man->ModFile[PartModelNum];
			if(pString == 0)
			{
				char buf[128];
				sprintf(buf, "No Name ModList! Plz Check again.ItemNum : %d", pPreLoadInfo->Kind[ePreLoad_Item][cnt]);
				ASSERTMSG(0, buf);
			}			
			if(pString->Str)
				CEngineObject::PreLoadObject(pString->Str);

			// Woman
			pString = &pModList_Woman->ModFile[PartModelNum];
			if(pString->Str)
				CEngineObject::PreLoadObject(pString->Str);
		}
	}
	///////////////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////////////
	// 06. 05. PreLoadData 추가기능 - 이영준
	// 이펙트 PreLoad
	sPRELOAD_EFFECT_INFO* pPreLoadEffectInfo = GAMERESRCMNGR->GetPreEffectInfo(MapNum);
	if(!pPreLoadEffectInfo)		return;

	PRELOAD* pEffect = NULL;
	pList = &(pPreLoadEffectInfo->Effect);

	pos = pList->GetHeadPosition();
	while(pos)
	{
		pEffect = (PRELOAD*)pList->GetNext(pos);
		if(pEffect)
		{
			CEngineObject::PreLoadObject(pEffect->FileName.Str);
		}
	}
	///////////////////////////////////////////////////////////////////////////////

	DIRECTORYMGR->SetLoadMode(eLM_Root);
}

void CMHMap::ApplyMapDesc()
{	
	if(mapDesc == NULL)
	{
		//LOGFILE("!!!!mapDesc == NULL!!!!");
	}
	if(mapDesc->CameraFilter[0])
	{
		m_CameraFilterObject.Init(mapDesc->CameraFilter,NULL,eEngineObjectType_Effect);
		m_CameraFilterObject.AttachToCamera(mapDesc->CameraFilterDist);
		CAMERA->SetCameraFilter(&m_CameraFilterObject);
	}

	MAINGAME->GetEngine()->GetGraphicsEngine()->SetBackGroundColor(mapDesc->backColor);
	MAINGAME->GetEngine()->GetGraphicsEngine()->SetFixHeight(mapDesc->bFixHeight,mapDesc->FixHeight);
	
	//YH2DO Remove Global Variable
	bRenderSky = mapDesc->bSky;

	g_pExecutive->GetGeometry()->SetAmbientColor(0,mapDesc->Ambient);

	if(mapDesc->fogdesc.bEnable)
	{
		// 임시
		g_pExecutive->GetGeometry()->EnableFog(0);
		g_pExecutive->GetGeometry()->SetFog(&mapDesc->fogdesc,0);
	}
	else
	{
		g_pExecutive->GetGeometry()->DisableFog(0);

	}

	VECTOR3 pos;
	pos.x = 0;
	pos.y = 0;
	pos.z = 0;
	
	g_pExecutive->GetGeometry()->ResetCamera(&pos,CAMERA_NEAR,mapDesc->DefaultSight,DEGTORAD(mapDesc->Fov),0);
	CAMERA->SetMaxSightDistance( mapDesc->DefaultSight );
	CAMERA->SetCameraFov( DEGTORAD(mapDesc->Fov) );

	//////////////////////////////////////////////////////////////////////////
	// BGM 
//	if(mapDesc->BGMSoundNum)
//	{
		//LOGFILE("AUDIOMGR->PlayBGM");
		AUDIOMGR->PlayBGM( mapDesc->BGMSoundNum );

		//LOGFILE("AUDIOMGR->PlayBGM End");
		
		//MP3->PlayFile(AUDIOMGR->GetFileName(mapDesc->BGMSoundNum),TRUE);
		//mapDesc->BGMSound[0] = 0;
//	}

	
	//////////////////////////////////////////////////////////////////////////
	// 방향성 라이트
	g_pExecutive->GetGeometry()->EnableDirectionalLight(0);
	
	DIRECTIONAL_LIGHT_DESC LightDesc;
	LightDesc.dwAmbient = 0;
	LightDesc.dwDiffuse = 0xffffffff;
	LightDesc.dwSpecular = 0xffffffff;

	LightDesc.v3Dir = mapDesc->SunPos * -1.f;

	LightDesc.bEnable = TRUE;
	g_pExecutive->GetGeometry()->SetDirectionalLight(&LightDesc,0);
	//////////////////////////////////////////////////////////////////////////
	

	//////////////////////////////////////////////////////////////////////////
	// 그림자
	if(GAMERESRCMNGR->m_GameDesc.bShadow == MHSHADOW_DETAIL)
	{
		VECTOR3 to;
		to.x = 0;
		to.y = 0;
		to.z = 0;
		//float len = VECTOR3Length(&mapDesc->SunPos);
		float len = VECTOR3Length(&m_SunPosForShadow);
		m_Shadowlight.InitShadowLight(0,&pos,&to,gPHI/16, len + 1000);
	}
	//////////////////////////////////////////////////////////////////////////


	//////////////////////////////////////////////////////////////////////////
	// 해 와 달
	if(mapDesc->bSun)
	{
		DIRECTORYMGR->SetLoadMode(eLM_Map);
		m_SunObject.Init(mapDesc->SunObject,NULL,eEngineObjectType_Effect);
		DIRECTORYMGR->SetLoadMode(eLM_Root);
	}
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// 구름
	if(mapDesc->CloudNum)
	{
		CEngineCloud::LoadCloudList(mapDesc->CloudListFile);
		CEngineCloud::SetCloudHeight(mapDesc->CloudHMin,mapDesc->CloudHMax);
		VECTOR3 vel;
		vel.x = 20;
		vel.y = 0;
		vel.z = 10;
		CEngineCloud::SetCloudVelocity(&vel);
		ASSERT(CEngineCloud::GetMaxCloudKindNum() != 0);
		DWORD numperside = (DWORD)sqrt((double)mapDesc->CloudNum);
		DWORD n=0;
		float sizeperside = 51200.f / numperside;
		for(DWORD x=0;x<numperside;++x)
		{
			for(DWORD z=0;z<numperside;++z)
			{
				CEngineCloud* pCloud = new CEngineCloud;
				float fx = x*sizeperside + rand()%(DWORD(sizeperside));
				float fz = z*sizeperside + rand()%(DWORD(sizeperside));
				pCloud->Init(rand()%pCloud->GetMaxCloudKindNum(),fx,fz);
				m_CloudTable.Add(pCloud,n++);
			}
		}
	}

	//LOGFILE("Apply MapDesc Ends");
}

void CMHMap::Restore()
{

}

//bool x=sCPlayerAI.IsRun;
void CMHMap::Release()
{
	//AUDIOMGR->StopBGM();

	//sprintf("%s", x ? "true" : "false");

	//LOGFILE("-------------------------start");
	//LOGFILE("AUTO= %s", AISETMGR->GetGameAI()->sPage5.pbAutoLeveling ? "true" : "false");
	//LOGFILE("IsRun= %s", sCPlayerAI.IsRun ? "true" : "false");
	//LOGFILE(" ");
	

	//sCPlayerAI.SetOpen(false);
	//sCPlayerAI.IsRun=FALSE;
	//AISETMGR->GetGameAI()->sPage5.pbAutoLeveling=FALSE;

	//LOGFILE("auto released= %s", AISETMGR->GetGameAI()->sPage5.pbAutoLeveling ? "true" : "false");
	//LOGFILE("Released= %s", sCPlayerAI.IsRun ? "true" : "false");
	
	//LOGFILE("-------------------------end");
	//LOGFILE(" ");
	//LOGFILE(" ");
	
	m_CameraFilterObject.DetachFromCamera();
	CAMERA->SetCameraFilter(NULL);
	m_CameraFilterObject.Release();

//	AUDIOMGR->StopBGM();

	m_CloudTable.SetPositionHead();
	while(CEngineCloud* pCloud = m_CloudTable.GetData())
	{
		delete pCloud;
	}

	m_CloudTable.RemoveAll();
	CEngineCloud::ReleaseCloudList();

	m_bIsInited = FALSE;
	ReleasePool();
	ProcessGarbageObject();
	m_Shadowlight.Release();
	SAFE_DELETE(mapDesc);
	SAFE_DELETE(sky);
	m_SunObject.Release();

	map.Release();
	
	SAFE_DELETE(m_pTileManager);
}

void CMHMap::Invalidate()

{
//	ReleasePool();
//	ProcessGarbageObject();
}


void CMHMap::Process(DWORD CurTime)
{	
	if( !m_bIsInited ) return;
	
	static DWORD SumTick = 0;
	static DWORD LastTime = 0;
	static DWORD Tick = 0;

	if(LastTime)
	{
		Tick = CurTime - LastTime;
		LastTime = CurTime;
	}
	else
	{
		LastTime = CurTime;
		return;
	}


	if(mapDesc->bSky && sky)
	{
		SumTick += Tick;
		if(SumTick > 50)

		{
			sky->IncreaseSkyAniFrame(1);
			SumTick -= 50;
		}
	
		sky->RenderSky();
	}
/*
	if(GAMERESRCMNGR->m_GameDesc.bShadow == MHSHADOW_DETAIL)	//
	{
		VECTOR3 pos;
		pos = m_ShadowPivotPos + mapDesc->SunPos;
		m_Shadowlight.SetPosition(&pos);
		LIGHT_DESC desc;
		m_Shadowlight.GetLightDesc(&desc);
		desc.v3Point = pos;
		desc.v3To = m_ShadowPivotPos;
		m_Shadowlight.SetLightDesc(&desc);
	}

*/	

	if(mapDesc->bSun)
	{
		VECTOR3 pos;
		Normalize(&pos,&mapDesc->SunPos);
		pos = pos * mapDesc->SunDistance;
		pos = m_ShadowPivotPos;// + pos;
		pos.x += 10000;
		pos.y -= 2000;
		m_SunObject.SetEngObjPosition(&pos);
	}
	
	
}
DWORD CMHMap::GetTileWidth()
{
	if(m_pTileManager)
		return m_pTileManager->GetTileWidth();
	return 100;
}
BOOL CMHMap::CollisionLine(VECTOR3* pStart,VECTOR3* pEnd, VECTOR3* Target, MAPTYPE MapNum,CObject* pObject)
{
	if(m_pTileManager == NULL)
		return FALSE;

	return m_pTileManager->CollisionLine(pStart, pEnd, Target, MapNum,pObject);
}

BOOL CMHMap::NonCollisionLine(VECTOR3* pStart, VECTOR3* pEnd, VECTOR3* Target, MAPTYPE MapNum, CObject* pObject)
{
	if(m_pTileManager == NULL)
		return FALSE;

	return m_pTileManager->NonCollisionLine(pStart, pEnd, Target, MapNum, pObject);
}

BOOL CMHMap::IsInTile(int cellX, int cellY, MAPTYPE MapNum,CObject* pObject)
{
	if(m_pTileManager == NULL)
		return FALSE;

	if(m_pTileManager->IsInTile(cellX, cellY, MapNum, pObject))
	{
		return TRUE;
	}
	return FALSE;
}
BOOL CMHMap::CollisionTileCell(int x, int y, MAPTYPE MapNum,CObject* pObject)
{
	if(m_pTileManager == NULL)
		return FALSE;

	if(m_pTileManager->CollisionTile(x, y, MapNum,pObject))
	{
		return FALSE;
	}
	return TRUE;
}
BOOL CMHMap::CollisionTilePos(int x, int y, MAPTYPE MapNum,CObject* pObject)
{
	if(m_pTileManager == NULL)
		return FALSE;

	if(m_pTileManager->CollisionTile(int(x/ TILECOLLISON_DETAIL), int(y/ TILECOLLISON_DETAIL), MapNum, pObject))
	{
		return TRUE;
	}
	return FALSE;
}
BOOL CMHMap::CollisonCheck(VECTOR3* pStart,VECTOR3* pEnd,VECTOR3* pRtCollisonPos,CObject* pObj)
{
	if(m_pTileManager == NULL)
		return FALSE;
	if(m_pTileManager->CollisonCheck(pStart,pEnd,pRtCollisonPos,pObj) == TRUE)
		return TRUE;


	return FALSE;
}

void CMHMap::SetShadowPivotPos(VECTOR3* pPivotPos)
{
	m_ShadowPivotPos = *pPivotPos;
		
	VECTOR3 pos;
//	pos = m_ShadowPivotPos + mapDesc->SunPos;
	pos = m_ShadowPivotPos + m_SunPosForShadow;
	
	m_Shadowlight.SetPosition(&pos);
	LIGHT_DESC desc;
	m_Shadowlight.GetLightDesc(&desc);
	desc.v3Point = pos;
	desc.v3To = m_ShadowPivotPos;
	m_Shadowlight.SetLightDesc(&desc);
}

void CMHMap::ToggleSunPosForShadow( BOOL bTitan )
{
	float dist = VECTOR3Length( &mapDesc->SunPos );
	if( bTitan )
	{
		m_SunPosForShadow = mapDesc->SunPos / dist * 2200;	//거리는 천? //반경보다 짧아야?
		m_SunPosForShadow.y += 1600;
	}
	else
	{
		m_SunPosForShadow = mapDesc->SunPos / dist * 2000;	//거리는 천? //반경보다 짧아야?
		m_SunPosForShadow.y += 1000;
	}
}

BOOL CMHMap::LoadMapDesc(MAPTYPE MapNum)
{	
	char mapdescfile[256];
	CMHFile file;
#ifdef _FILE_BIN_
//	sprintf(mapdescfile,"ini\\MAP%d.bmhm",MapNum);
	sprintf(mapdescfile,"./Resource/Map/MAP%d.bmhm",MapNum);
	if(file.Init(mapdescfile,"rb") == FALSE)
		return FALSE;
#else
//	sprintf(mapdescfile,"ini\\MAP%d.mhm",MapNum);	
	sprintf(mapdescfile,"./Resource/Map/MAP%d.mhm",MapNum);
	if(file.Init(mapdescfile,"r") == FALSE)
		return FALSE;
#endif

	
	char value[64] = {0,};
	SAFE_DELETE(mapDesc);
	mapDesc = new MAPDESC;

	while(1)
	{
		if(file.IsEOF())
			break;

		strcpy(value,strupr(file.GetString()));

		if(strcmp(value,"*SIGHT") == 0)
		{
			mapDesc->DefaultSight = file.GetFloat();
		}
		else if(strcmp(value,"*FOV") == 0)
		{
			mapDesc->Fov = file.GetFloat();
		}
		else if(strcmp(value,"*FOG") == 0)
		{
			mapDesc->fogdesc.bEnable = file.GetBool();
		}
		else if(strcmp(value,"*BRIGHT") == 0)
		{
			DWORD dd;
			dd = file.GetDword();
			mapDesc->Ambient = RGBA_MAKE(dd,dd,dd,dd);
		}
		else if(strcmp(value,"*FOGCOLOR") == 0)
		{
			DWORD r,g,b,a;
			r = file.GetDword();
			g = file.GetDword();
			b = file.GetDword();
			a = file.GetDword();

			mapDesc->fogdesc.dwColor = RGBA_MAKE(r,g,b,a);
		}
		else if(strcmp(value,"*FOGDENSITY") == 0)
		{
			mapDesc->fogdesc.fDensity = file.GetFloat();
		}
		else if(strcmp(value,"*FOGSTART") == 0)
		{
			mapDesc->fogdesc.fStart = file.GetFloat();
		}
		else if(strcmp(value,"*FOGEND") == 0)
		{
			mapDesc->fogdesc.fEnd = file.GetFloat();
		}

		else if(strcmp(value,"*MAP") == 0)
		{
			file.GetString(mapDesc->MapFileName);
		}
		else if(strcmp(value,"*TILE") == 0)
		{
			file.GetString(mapDesc->TileFileName);
//			char temp[256] ={0,};
            sprintf( value, "Map/%s", mapDesc->TileFileName );
			strcpy( mapDesc->TileFileName, value );
		}
		else if(strcmp(value,"*SKYMOD") == 0)
		{
			file.GetString(mapDesc->SkyMod);
		}
		else if(strcmp(value,"*SKYANM") == 0)
		{
			file.GetString(mapDesc->SkyAnm);
		}
		else if(strcmp(value,"*SKYBOX") == 0)
		{
			mapDesc->bSky = file.GetBool();
		}
		else if(strcmp(value,"*BGM") == 0)
		{
			mapDesc->BGMSoundNum = file.GetWord();
		}
		else if(strcmp(value,"*COLOR") == 0)
		{
			BYTE r = file.GetByte();
			BYTE g = file.GetByte();
			BYTE b = file.GetByte();
			mapDesc->Ambient = RGBA_MAKE(r,g,b,255);
		}
		else if(strcmp(value,"*SUNPOS") == 0)
		{
			mapDesc->SunPos.x = file.GetFloat();
			mapDesc->SunPos.y = file.GetFloat();
			mapDesc->SunPos.z = file.GetFloat();

			float dist = VECTOR3Length( &mapDesc->SunPos );
			m_SunPosForShadow = mapDesc->SunPos / dist * 2000;	//거리는 천? //반경보다 짧아야?
			m_SunPosForShadow.y += 1000;
			
		}
		else if(strcmp(value,"*SUNOBJECT") == 0)
		{
			file.GetString(mapDesc->SunObject);
		}
		else if(strcmp(value,"*SUN") == 0)
		{
			mapDesc->bSun = file.GetBool();
		}
		else if(strcmp(value,"*SUNDISTANCE") == 0)
		{
			mapDesc->SunDistance = file.GetFloat();
		}
		else if(strcmp(value,"*BACKCOLOR") == 0)
		{
			DWORD r,g,b,a;
			r = file.GetDword();
			g = file.GetDword();
			b = file.GetDword();
			a = file.GetDword();
			mapDesc->backColor = RGBA_MAKE(r,g,b,a);
		}
		else if(strcmp(value,"*FIXHEIGHT") == 0)
		{
			mapDesc->bFixHeight = TRUE;
			mapDesc->FixHeight = file.GetFloat();			
		}
		else if(strcmp(value,"*CLOUD") == 0)

		{
			mapDesc->CloudNum = file.GetDword();			
		}
		else if(strcmp(value,"*CLOUDLIST") == 0)
		{
			file.GetString(mapDesc->CloudListFile);
		}
		else if(strcmp(value,"*CLOUDHEIGHT") == 0)
		{
			mapDesc->CloudHMin = file.GetInt();
			mapDesc->CloudHMax = file.GetInt();
		}
		else if(strcmp(value,"*CAMERAFILTER") == 0)
		{
			file.GetString(mapDesc->CameraFilter);
		}		
		else if(strcmp(value,"*CAMERAFILTERDIST") == 0)
		{
			mapDesc->CameraFilterDist = file.GetFloat();
		}	
		//trustpak 2005/04/15
		else if(strcmp(value, "*SKYOFFSET") == 0)

		{
			mapDesc->SkyOffset.x = file.GetFloat();

			mapDesc->SkyOffset.y = file.GetFloat();
			mapDesc->SkyOffset.z = file.GetFloat();
		}
		///
	}
	
	file.Release();

	return TRUE;
}


void CMHMap::LoadStaticNpc(MAPTYPE MapNum)
{
	MAPTYPE npcMapnum;
	char value[256] = {0,};

	CMHFile file;
#ifdef _FILE_BIN_
	file.Init("Resource/StaticNpc.bin","rb");
#else
	file.Init("Resource/StaticNpc.txt","rt");
#endif

	if(file.IsInited() == FALSE)
		return;

	NPC_TOTALINFO tinfo;
	BASEOBJECT_INFO binfo;
	BASEMOVE_INFO minfo;

	DWORD n=0;
	char idxbuf[128] = { 0, };
	char seps[] = ",";
	char* token = NULL;

	while(1)
	{
		if(file.IsEOF())
			break;

		npcMapnum = file.GetWord();
		if(npcMapnum != MapNum)
		{
			file.GetLine(value,256);
			continue;
		}

		binfo.dwObjectID = STATICNPC_ID_START + n++;
		binfo.ObjectState = 0;
		tinfo.MapNum = MapNum;
		minfo.bMoving = FALSE;
		minfo.KyungGongIdx = 0;
		minfo.MoveMode = 0;

		tinfo.NpcKind = file.GetWord();
		//file.GetString(binfo.ObjectName);
		SafeStrCpy( binfo.ObjectName, file.GetString(), MAX_NAME_LENGTH+1 );
		tinfo.NpcUniqueIdx = file.GetWord();
		NPC_LIST* pInfo = GAMERESRCMNGR->GetNpcInfo(tinfo.NpcKind);
		if(pInfo)
		{
			tinfo.NpcJob = pInfo->JobKind;
		}
		else
		{
			tinfo.NpcJob = 0;
		}
		
		//jop이 0이면 이름을 지우자.
		if( tinfo.NpcJob == 0 )
		{
			binfo.ObjectName[0] = 0;
		}

		minfo.CurPosition.x = file.GetFloat();
		minfo.CurPosition.y = 0;
		minfo.CurPosition.z = file.GetFloat();
		
		CNpc* pNpc = OBJECTMGR->AddNpc(&binfo,&minfo,&tinfo);

		//방향.....
		float fDir = file.GetFloat();
		pNpc->SetAngle( DEGTORAD( fDir ) );
		
		GAMEIN->GetMiniMapDialog()->AddStaticNpcIcon(pNpc);
		
		// Npc Add
		QUESTMGR->SetNpcData( pNpc );

		//SW061205 깃발 NPC
		//우선 깃발NPC이면 초기 상태를 HIDE로..
		if( 65 <= tinfo.NpcKind && tinfo.NpcKind <= 70 )
		{
			pNpc->GetEngineObject()->Hide();
		}

	}
	
	//SW061205 깃발 NPC
	//SYSTEMTIME st;
	//GetLocalTime(&st);
	//WORD day = st.wDayOfWeek;
	//DWORD GTFlg, SGFlg;

	//GTFlg = GAMERESRCMNGR->GetFlagFromDate(eGTFlg, day);
	//OBJECTMGR->ChangeFlagNPC(eGTFlg, GTFlg);

	//SGFlg = GAMERESRCMNGR->GetFlagFromDate(eSGFlg, day);
	//OBJECTMGR->ChangeFlagNPC(eSGFlg, SGFlg);

	//file.Release();
}

void CMHMap::AddQuestNpc( QUESTNPCINFO* pInfo )
{
	if( pInfo->wMapNum != GetMapNum() )

		return;
	CObject* pObject = OBJECTMGR->GetObject( pInfo->dwObjIdx );
	if( pObject )
		return;

	NPC_TOTALINFO tinfo;
	BASEOBJECT_INFO binfo;
	BASEMOVE_INFO minfo;

	binfo.dwObjectID = STATICNPC_ID_START + m_dwQuestNpcId++;
	pInfo->dwObjIdx = binfo.dwObjectID;
	binfo.ObjectState = 0;
	tinfo.MapNum = pInfo->wMapNum;
	minfo.bMoving = FALSE;
	minfo.KyungGongIdx = 0;
	minfo.MoveMode = 0;

	tinfo.NpcKind = pInfo->wNpcKind;
//	strcpy( binfo.ObjectName, pInfo->sName );
	SafeStrCpy( binfo.ObjectName, pInfo->sName, MAX_NAME_LENGTH+1 );
	tinfo.NpcUniqueIdx = pInfo->wNpcIdx;
	NPC_LIST* pList = GAMERESRCMNGR->GetNpcInfo(tinfo.NpcKind);
	if(pInfo)
	{

		tinfo.NpcJob = pList->JobKind;
	}
	else
	{
		tinfo.NpcJob = 0;
		return;
	}

	minfo.CurPosition.x = pInfo->vPos.x;
	minfo.CurPosition.y = 0;
	minfo.CurPosition.z = pInfo->vPos.z;

	CNpc* pNpc = OBJECTMGR->AddNpc(&binfo,&minfo,&tinfo);

	//방향.....
	float fDir = pInfo->fDir;

	pNpc->SetAngle( DEGTORAD( fDir ) );
	
	GAMEIN->GetMiniMapDialog()->AddStaticNpcIcon(pNpc);

	// Npc Add
	QUESTMGR->SetNpcData( pNpc );


	TARGETSET set;
	set.pTarget = pNpc;
	EFFECTMGR->StartEffectProcess(eEffect_MonRegen,pNpc,&set,0,pNpc->GetID());
}

void CMHMap::SetMapKind(WORD wMapNum)
{
	if(wMapNum == SURVIVALMAP)
		m_iMapKind = eSurvival;
	else
		m_iMapKind = eNormalMap;
}

BOOL CMHMap::CheckMapKindIs(int eMapkind)
{
	return (m_iMapKind & eMapkind);
}

///////// 2007. 6. 19. CBH - 현재 맵이 타이탄 맵인지 구분하는 함수 추가 ////////
BOOL CMHMap::IsTitanMap()
{
	//맵번호가 100 이상이면 타이탄 맵이다.
	if( ( GAMEIN->GetInitForGame() ) && (m_MapNum > TITANMAP_START_INDEX ) )
	{
		return TRUE;
	}

	return FALSE;
}
////////////////////////////////////////////////////////////////////////////

// magi82(37) 맵 성 데이터
char temp[64];	// 예외처리용 전역변수

char* CMHMap::GetMapName(DWORD dwMapNum)
{
	if(dwMapNum == 0)
		dwMapNum = m_MapNum;	// m_MapNum 에는 현재 자신이 있는 맵의 번호가 있다.

	stMAPKINDINFO* pInfo = GAMERESRCMNGR->GetMapKindInfo(dwMapNum);
	if( !pInfo )
	{
		//ASSERT(0);
		ZeroMemory(temp, sizeof(temp));
		wsprintf(temp, "NoMapInfo : %d", dwMapNum);

		return temp;
	}

	return pInfo->strMapName;
}

DWORD CMHMap::GetMapNumForName(char* strName)
{
	CYHHashTable<stMAPKINDINFO>* pTable = GAMERESRCMNGR->GetMapKindInfoTable();
	if( !pTable )
	{
		ASSERT(0);
		return NULL;
	}

	stMAPKINDINFO* pInfo = NULL;

	pTable->SetPositionHead();
	while(pInfo = pTable->GetData())
	{
		if( pInfo )
		{
			if( strcmp(strName, pInfo->strMapName) == 0 )
				return pInfo->dwMapNum;
		}
	}
	
	return NULL;
}

BOOL CMHMap::IsMapKind(DWORD dwMapKind, DWORD dwMapNum)
{
	if(dwMapNum == 0)
		dwMapNum = m_MapNum;	// m_MapNum 에는 현재 자신이 있는 맵의 번호가 있다.

	stMAPKINDINFO* pInfo = GAMERESRCMNGR->GetMapKindInfo(dwMapNum);
	if( !pInfo )
	{
		//ASSERT(0);
		ZeroMemory(temp, sizeof(temp));
		wsprintf(temp, "NoMapInfo : %d", dwMapNum);
		return FALSE;
	}

	return ( pInfo->dwMapStateBItFlag & dwMapKind );
}

BOOL CMHMap::IsMapSame(DWORD dwMapKind, DWORD dwMapNum)
{
	if(dwMapNum == 0)
		dwMapNum = m_MapNum;	// m_MapNum 에는 현재 자신이 있는 맵의 번호가 있다.

	stMAPKINDINFO* pInfo = GAMERESRCMNGR->GetMapKindInfo(dwMapNum);
	if( !pInfo )
	{
		//ASSERT(0);
		ZeroMemory(temp, sizeof(temp));
		wsprintf(temp, "NoMapInfo : %d", dwMapNum);
		return FALSE;
	}

	DWORD MapKind = pInfo->dwMapStateBItFlag;	// 스크립트 상의 비트플래그
	DWORD CurMap = dwMapKind;	// 체크해야할 비트플래그

	DWORD nHighBit = MapKind & CurMap;	// 상위 비트
	DWORD nSubBit = MapKind - CurMap;	// 상위 비트를 뺀 나머지(하위비트 포함)

	DWORD CheckFlag = 0x000003F;	// 하위비트 체크를 위한 플래그
	DWORD nLowBit = ( nSubBit & CheckFlag );

	return ( (nHighBit == CurMap) & !nLowBit);
}

BOOL CMHMap::CollisionCheck_OneLine_New( VECTOR3* pStart,VECTOR3* pEnd )
{
	if(m_pTileManager == NULL)
	{
		return FALSE;
	}
	return m_pTileManager->CollisionCheck_OneLine_New(pStart,pEnd);
}
BOOL CMHMap::PathFind_Line(VECTOR3* pStart, VECTOR3* pEnd, VECTOR3* pRtCollisonPos, CObject* pObj)
{
	if (m_pTileManager == NULL)
	{
		return FALSE;
	}

	return m_pTileManager->CollisionCheck_OneLine(pStart, pEnd, pRtCollisonPos);
}
BOOL CMHMap::CollisionCheck_OneTile( VECTOR3* pDestPos )
{
	if(m_pTileManager == NULL)
	{
		return FALSE;
	}
	CTile* l_Node = m_pTileManager->GetNode1x(pDestPos->x, pDestPos->z);
	if (l_Node)
	{
		if ( l_Node->IsCollisonTile() == FALSE )
		{
			return TRUE;
		}
	}
	return FALSE;
}
BOOL QuickPath();
BOOL NonQuickPath();
DWORD WINAPI PathFind_Thread( LPVOID p_Param )
{
	g_WayPointManager->SetWMStatus(eWMStatus_Active);
	InterlockedExchange(&g_PathThreadActivated, 0);

	if ( g_SimpleMode )
	{
		return QuickPath();
	}
	else
	{
		return NonQuickPath();
	}	
	return FALSE;
}
BOOL QuickPath()
{
	g_TileManager->SetPathFindMode(ePathFindLayer_1x);
	g_TileManager->AStarDataReset();
	BOOL t_InputPoint = g_TileManager->InputPoint(g_PathFindStart, g_PathFindEnd);
	if (t_InputPoint)
	{
		BOOL t_PathFind_AStar = g_TileManager->PathFind_AStar(500000);
		if (t_PathFind_AStar == TRUE)
		{
			g_TileManager->MakeAStarPath();
			g_TileManager->Optimize();
			g_WayPointManager->GetVec_WayPoint() = g_TileManager->GetPathResult_Optimize(eDefaultTileRatio);
			MAP->SetPath_Debug(g_TileManager->GetPathResult(eDefaultTileRatio));
			MAP->SetPath_Optimize_Debug(g_TileManager->GetPathResult_Optimize_Debug(eDefaultTileRatio));			
			g_WayPointManager->Debug_TRACEWayPoint();

			g_WayPointManager->SetWMStatus(eWMStatus_Success);
			InterlockedExchange(&g_PathThreadActivated, 0);
			return TRUE;
		}
	}
	g_WayPointManager->SetWMStatus(eWMStatus_Fail);
	InterlockedExchange(&g_PathThreadActivated, 0);
	return FALSE;
}
BOOL NonQuickPath()
{
	g_TileManager->SetPathFindMode(ePathFindLayer_4x);
	g_TileManager->AStarDataReset();
	IPathNode* l_StartNode4x = NULL;
	g_TileManager->FindClosest4xTile(g_PathFindStart, l_StartNode4x);
	IPathNode* l_DestNode4x = NULL;
	g_TileManager->FindClosest4xTile(g_PathFindEnd, l_DestNode4x);
	BOOL t_InputPoint = g_TileManager->InputPoint(l_StartNode4x, l_DestNode4x);
	if (t_InputPoint)
	{
		BOOL t_PathFind_AStar = g_TileManager->PathFind_AStar(500000);
		if (t_PathFind_AStar == TRUE)
		{
			g_TileManager->MakeAStarPath();
			g_TileManager->Optimize();
			g_WayPointManager->GetVec_WayPoint() = g_TileManager->GetPathResult_Optimize(200);
			MAP->SetPath_Debug(g_TileManager->GetPathResult(200));
			MAP->SetPath_Optimize_Debug(g_TileManager->GetPathResult_Optimize_Debug(200));
			
			g_TileManager->SetPathFindMode(ePathFindLayer_1x);

			g_WayPointManager->SetWMStatus(eWMStatus_Success);
			InterlockedExchange(&g_PathThreadActivated, 0);
			return TRUE;
		}
	}
	g_WayPointManager->SetWMStatus(eWMStatus_Fail);
	InterlockedExchange(&g_PathThreadActivated, 0);
	return FALSE;
}
BOOL bEnable;
#define ALPHA_PROCESS_TIME	500
UINT i;
UINT o = 100;
BOOL GoRenderS[2];
DWORD m_dwStartTime[2];
UINT r;
void CMHMap::RenderTileData_Debug()
{
	if (!m_bRenderTileData) return;
	if (!HERO) return;
	VECTOR3 l_Vector, heroPos;
	HERO->GetPosition(&heroPos);
	int bo;
	if (GoRenderS[0])
	{
		DWORD dwElapsed = MHTIMEMGR->GetNewCalcCurTime() - m_dwStartTime[0];
		DWORD alphamove = (dwElapsed * 255 / (ALPHA_PROCESS_TIME));
		DWORD alphatime = dwElapsed > (ALPHA_PROCESS_TIME);
		if (alphatime)
		{
			if (bo < 150)
			{
				bo = 0 + alphamove;
				bEnable = FALSE;
				//	return;
			}
			else
			{
				m_dwStartTime[0] = MHTIMEMGR->GetNewCalcCurTime();
				bEnable = TRUE;
			}
		}
		else
		{
			bEnable = TRUE;
			bo = 255 - alphamove;
		}
	}

	if (m_Path_Debug.size() > 2)
	{
		const int maxpathtile = m_Path_Debug.size() - 1;
		for (r = 0; r < maxpathtile; r++)
		{
			if (bo > 255)
				bo = 255;
			if ((r > 0) && (r != (maxpathtile)))
			{
				if (r > maxpathtile || r >= maxpathtile)
				{
					return;
				}
				if (bEnable)
				{
					bo++;
				}
				else
				{
					if (bo>10)
						--bo;
				}

				l_Vector = m_Path_Debug[r];
				l_Vector.y = heroPos.y + 500;
				RenderOneTile(l_Vector, RGB_HALF(0, bo, 0), TRUE);
			}
		}
		VECTOR3 l_Vector2 = m_Path_Debug[maxpathtile];
		VECTOR3 l_Vector3 = m_Path_Debug[0];
		l_Vector2.y = heroPos.y + 500;
		l_Vector3.y = heroPos.y + 500;
		RenderOneTile(l_Vector2, 0xFFFF0000, FALSE);
		RenderOneTile(l_Vector3, 0xFF0000FF, FALSE);
	}
}
void CMHMap::RenderOneTile(const VECTOR3& p_TileOriPos, const DWORD p_Color, BOOL mode)
{
	if (mode)
	{
		VECTOR3 Lefttop, RightTop, LeftBottom, RightBottom;
		Lefttop.x = p_TileOriPos.x + 45;
		Lefttop.y = p_TileOriPos.y;
		Lefttop.z = p_TileOriPos.z + 5;
		RightTop.x = p_TileOriPos.x + 5;
		RightTop.y = p_TileOriPos.y;
		RightTop.z = p_TileOriPos.z + 5;
		LeftBottom.x = p_TileOriPos.x + 5;
		LeftBottom.y = p_TileOriPos.y;
		LeftBottom.z = p_TileOriPos.z + 45;
		RightBottom.x = p_TileOriPos.x + 45;
		RightBottom.y = p_TileOriPos.y;
		RightBottom.z = p_TileOriPos.z + 45;
		//g_pExecutive->GetRenderer()->RenderLine3D(&Lefttop, &RightTop, p_Color);
		//g_pExecutive->GetRenderer()->RenderLine3D(&RightTop, &RightBottom, p_Color);
		//g_pExecutive->GetRenderer()->RenderLine3D(&RightBottom, &LeftBottom, p_Color);
		//g_pExecutive->GetRenderer()->RenderLine3D(&LeftBottom, &Lefttop, p_Color);
		return;
	}

	VECTOR3 inLefttop, inRightTop, inLeftBottom, inRightBottom;
	VECTOR3 outLefttop, outRightTop, outLeftBottom, outRightBottom;
	VECTOR2 Lefttop, RightTop, LeftBottom, RightBottom;
	inLefttop.x = p_TileOriPos.x + 5;
	inLefttop.y = p_TileOriPos.y;
	inLefttop.z = p_TileOriPos.z + 5;
	inRightTop.x = p_TileOriPos.x + 55;
	inRightTop.y = p_TileOriPos.y;
	inRightTop.z = p_TileOriPos.z + 5;
	inLeftBottom.x = p_TileOriPos.x + 5;
	inLeftBottom.y = p_TileOriPos.y;
	inLeftBottom.z = p_TileOriPos.z + 55;
	inRightBottom.x = p_TileOriPos.x + 55;
	inRightBottom.y = p_TileOriPos.y;
	inRightBottom.z = p_TileOriPos.z + 55;
	GetScreenXYFromXYZ(g_pExecutive->GetGeometry(), 0, &inLefttop, &outLefttop);
	GetScreenXYFromXYZ(g_pExecutive->GetGeometry(), 0, &inRightTop, &outRightTop);
	GetScreenXYFromXYZ(g_pExecutive->GetGeometry(), 0, &inLeftBottom, &outLeftBottom);
	GetScreenXYFromXYZ(g_pExecutive->GetGeometry(), 0, &inRightBottom, &outRightBottom);
	DWORD dwWidth = GET_MAINWIN_W;
	DWORD dwHeight = GET_MAINWIN_H;
	Lefttop.x = outLefttop.x * dwWidth;
	Lefttop.y = outLefttop.y * dwHeight;
	RightTop.x = outRightTop.x * dwWidth;
	RightTop.y = outRightTop.y * dwHeight;
	LeftBottom.x = outLeftBottom.x * dwWidth;
	LeftBottom.y = outLeftBottom.y * dwHeight;
	RightBottom.x = outRightBottom.x * dwWidth;
	RightBottom.y = outRightBottom.y * dwHeight;
	g_pExecutive->GetRenderer()->RenderLine(&Lefttop, &RightTop, p_Color);
	g_pExecutive->GetRenderer()->RenderLine(&RightTop, &RightBottom, p_Color);
	g_pExecutive->GetRenderer()->RenderLine(&RightBottom, &LeftBottom, p_Color);
	g_pExecutive->GetRenderer()->RenderLine(&LeftBottom, &Lefttop, p_Color);
}
BOOL CMHMap::PathFind( VECTOR3* pStart,VECTOR3* pEnd, CWayPointManager* p_pWayPointManager, BOOL p_SimpleMode  )
{
	if (m_pTileManager == NULL || p_pWayPointManager == NULL)
	{
		return FALSE;
	}

	if(InterlockedCompareExchange(&g_PathThreadActivated, 1, 0) == 1)
	{
		return FALSE;
	}

	if (p_pWayPointManager->GetWMStatus() == eWMStatus_Active)
	{
		return FALSE;
	}

	DWORD l_ThreadID;
	HANDLE l_ThreadHandle;

	g_PathFindStart		= *pStart;
	g_PathFindEnd		= *pEnd;
	g_TileManager		= m_pTileManager;
	g_WayPointManager	= p_pWayPointManager;
	g_SimpleMode		= p_SimpleMode;
	
	//AUTOPATH->SetTargetPos(&g_PathFindEnd, TRUE);
	m_bRenderTileData = FALSE;// TRUE;
	HERO->SetMoveDebugInfo(TRUE);

	m_dwStartTime[0] = GetTickCount();
	m_dwStartTime[1] = MHTIMEMGR->GetNewCalcCurTime();
	GoRenderS[0] = TRUE;
	GoRenderS[1] = TRUE;
	k = 0;
	i = 0;
	r = 0;

	l_ThreadHandle = CreateThread(NULL, 0, PathFind_Thread, NULL, 0, &l_ThreadID);
	if(l_ThreadHandle == NULL)
	{
		InterlockedExchange(&g_PathThreadActivated, 0);
		return FALSE;
	}
	

	CloseHandle(l_ThreadHandle);
	return TRUE;
}

#define MINIMAPMASTER 10
#define MINIMAPW 7.32f
#define MINIMAPH 6.42f
#define MINIMAPDIVIDE .01f / 100
#define DEFTILERATIO eDefaultTileRatio / 2
#define MINIMAPPOSX +4+
#define MINIMAPPOSY -327+
#include "PathManager.h"
void CMHMap::RenderTile2D()
{
	if (!m_bRenderTileData) return;
	if (!HERO) return;
	CMiniMapDlg* pMiniMapDlg = GAMEIN->GetMiniMapDialog();
	if (!pMiniMapDlg) return;
	CBigMapDlg* pBigMapDlg = GAMEIN->GetBigMapDialog();
	if (!pBigMapDlg) return;
	static int bo;
	static BOOL FinalFadeInit2;
	static BOOL FinalFade2;
	if (GoRenderS[1])
	{
		DWORD dwElapsed = MHTIMEMGR->GetNewCalcCurTime() - m_dwStartTime[1];
		if (dwElapsed > (ALPHA_PROCESS_TIME))
		{
			if (bo < 5)
			{
				bo = (BYTE)(max(0, 19) + (dwElapsed * 255 / (ALPHA_PROCESS_TIME)));
				bEnable = FALSE;
			}
			else
			{
				m_dwStartTime[1] = MHTIMEMGR->GetNewCalcCurTime();
				bEnable = TRUE;
			}
		}
		else
		{
			bEnable = TRUE;
			bo = (BYTE)(max(0, 19) - (dwElapsed * 255 / (ALPHA_PROCESS_TIME)));
		}
	}
	/*VECTOR3 heroPos;
	HERO->GetPosition(&heroPos);
	float dist, dist2,dist3;
	DOUBLE total_marks, obtained_marks, percentage;
	const DOUBLE db = 100;
	percentage = 0;
	if (m_Path_Optimize_Debug.size() > 2)
	{
		VECTOR3 l_Vector2;
		VECTOR3 l_Vector;
		for (i = 0; i < m_Path_Optimize_Debug.size()-1; i++)
		{
			l_Vector = m_Path_Optimize_Debug[i];
			l_Vector2 = m_Path_Optimize_Debug[i + 1];

			dist2+=   CalcDistanceXZ(&l_Vector, &l_Vector2);		
		}
		dist = dist + CalcDistanceXZ(&heroPos, &l_Vector2);	
		percentage = db-((db * dist) / dist2);
		if (percentage>0)
			CHATMGR->AddMsg(CTC_SYSMSG, "%3.0f%%\t\t%7.0f\t%7.0f\t\t%d", percentage, sqrt(dist), sqrt(dist2), m_Path_Optimize_Debug.size() - 1);
	}*/
	
	if (m_Path_Optimize_Debug.size() > 2)
	{
		VECTOR2 l_2DPos1, l_2DPos2, f_2DPos1, f_2DPos2;
		DWORD inLine = RGBA_MERGE(RGB_HALF( bo, 173, 255), bo);
		DWORD outLine = RGBA_MERGE(RGB_HALF(18, 105, 172), 255);
		for (i = 0; i < m_Path_Optimize_Debug.size() - 1; i++)
		{
			VECTOR3 l_Vector1 = m_Path_Optimize_Debug[i];
			VECTOR3 l_Vector2 = m_Path_Optimize_Debug[i + 1];
			if (pBigMapDlg->IsActive())
			{
				f_2DPos1.x = pBigMapDlg->GetAbsX() + 1.1f + (l_Vector1.x / DEFTILERATIO);
				f_2DPos1.y = pBigMapDlg->GetAbsY() + 1.1f + 510 - (l_Vector1.z / DEFTILERATIO);
				f_2DPos2.x = pBigMapDlg->GetAbsX() + 1.1f + (l_Vector2.x / DEFTILERATIO);
				f_2DPos2.y = pBigMapDlg->GetAbsY() + 1.1f + 510 - (l_Vector2.z / DEFTILERATIO);
				RenderBoldOneTile(f_2DPos1, f_2DPos2, TRUE, outLine);
				f_2DPos1.x = pBigMapDlg->GetAbsX() - 1.1f + (l_Vector1.x / DEFTILERATIO);
				f_2DPos1.y = pBigMapDlg->GetAbsY() - 1.1f + 510 - (l_Vector1.z / DEFTILERATIO);
				f_2DPos2.x = pBigMapDlg->GetAbsX() - 1.1f + (l_Vector2.x / DEFTILERATIO);
				f_2DPos2.y = pBigMapDlg->GetAbsY() - 1.1f + 510 - (l_Vector2.z / DEFTILERATIO);
				RenderBoldOneTile(f_2DPos1, f_2DPos2, TRUE, outLine);
				f_2DPos1.x = pBigMapDlg->GetAbsX() + 0.1f + (l_Vector1.x / DEFTILERATIO);
				f_2DPos1.y = pBigMapDlg->GetAbsY() + 0.1f + 510 - (l_Vector1.z / DEFTILERATIO);
				f_2DPos2.x = pBigMapDlg->GetAbsX() + 0.1f + (l_Vector2.x / DEFTILERATIO);
				f_2DPos2.y = pBigMapDlg->GetAbsY() + 0.1f + 510 - (l_Vector2.z / DEFTILERATIO);
				RenderBoldOneTile(f_2DPos1, f_2DPos2, TRUE, outLine);

				f_2DPos1.x = pBigMapDlg->GetAbsX() - 0.1f + (l_Vector1.x / DEFTILERATIO);
				f_2DPos1.y = pBigMapDlg->GetAbsY() - 0.1f + 510 - (l_Vector1.z / DEFTILERATIO);
				f_2DPos2.x = pBigMapDlg->GetAbsX() - 0.1f + (l_Vector2.x / DEFTILERATIO);
				f_2DPos2.y = pBigMapDlg->GetAbsY() - 0.1f + 510 - (l_Vector2.z / DEFTILERATIO);
				RenderBoldOneTile(f_2DPos1, f_2DPos2, TRUE, inLine);

				f_2DPos1.x = pBigMapDlg->GetAbsX() + (l_Vector1.x / DEFTILERATIO);
				f_2DPos1.y = pBigMapDlg->GetAbsY() + 510 - (l_Vector1.z / DEFTILERATIO);
				f_2DPos2.x = pBigMapDlg->GetAbsX() + (l_Vector2.x / DEFTILERATIO);
				f_2DPos2.y = pBigMapDlg->GetAbsY() + 510 - (l_Vector2.z / DEFTILERATIO);
				RenderBoldOneTile(f_2DPos1, f_2DPos2, TRUE, inLine);
				f_2DPos1.x = pBigMapDlg->GetAbsX() + (l_Vector1.x / DEFTILERATIO);
				f_2DPos1.y = pBigMapDlg->GetAbsY() + 510 - (l_Vector1.z / DEFTILERATIO);
				f_2DPos2.x = pBigMapDlg->GetAbsX() + (l_Vector2.x / DEFTILERATIO);
				f_2DPos2.y = pBigMapDlg->GetAbsY() + 510 - (l_Vector2.z / DEFTILERATIO);
				RenderBoldOneTile(f_2DPos1, f_2DPos2, TRUE, inLine);
			}
			if (pMiniMapDlg->IsActive())
			{
				l_2DPos1.x = pMiniMapDlg->GetAbsX() + 0.3f MINIMAPPOSX((l_Vector1.x / eDefaultTileRatio / MINIMAPW) / MINIMAPDIVIDE);
				l_2DPos1.y = pMiniMapDlg->GetAbsY() + 0.3f MINIMAPPOSY(510 - (l_Vector1.z / eDefaultTileRatio / MINIMAPH) / MINIMAPDIVIDE);
				l_2DPos2.x = pMiniMapDlg->GetAbsX() + 0.3f MINIMAPPOSX((l_Vector2.x / eDefaultTileRatio / MINIMAPW) / MINIMAPDIVIDE);
				l_2DPos2.y = pMiniMapDlg->GetAbsY() + 0.3f MINIMAPPOSY(510 - (l_Vector2.z / eDefaultTileRatio / MINIMAPH) / MINIMAPDIVIDE);
				RenderBoldOneTile(l_2DPos1, l_2DPos2, FALSE, outLine);
				l_2DPos1.x = pMiniMapDlg->GetAbsX() - 0.3f MINIMAPPOSX((l_Vector1.x / eDefaultTileRatio / MINIMAPW) / MINIMAPDIVIDE);
				l_2DPos1.y = pMiniMapDlg->GetAbsY() - 0.3f MINIMAPPOSY(510 - (l_Vector1.z / eDefaultTileRatio / MINIMAPH) / MINIMAPDIVIDE);
				l_2DPos2.x = pMiniMapDlg->GetAbsX() - 0.3f MINIMAPPOSX((l_Vector2.x / eDefaultTileRatio / MINIMAPW) / MINIMAPDIVIDE);
				l_2DPos2.y = pMiniMapDlg->GetAbsY() - 0.3f MINIMAPPOSY(510 - (l_Vector2.z / eDefaultTileRatio / MINIMAPH) / MINIMAPDIVIDE);
				RenderBoldOneTile(l_2DPos1, l_2DPos2, FALSE, outLine);
				l_2DPos1.x = pMiniMapDlg->GetAbsX() + 0.1f MINIMAPPOSX((l_Vector1.x / eDefaultTileRatio / MINIMAPW) / MINIMAPDIVIDE);
				l_2DPos1.y = pMiniMapDlg->GetAbsY() + 0.1f MINIMAPPOSY(510 - (l_Vector1.z / eDefaultTileRatio / MINIMAPH) / MINIMAPDIVIDE);
				l_2DPos2.x = pMiniMapDlg->GetAbsX() + 0.1f MINIMAPPOSX((l_Vector2.x / eDefaultTileRatio / MINIMAPW) / MINIMAPDIVIDE);
				l_2DPos2.y = pMiniMapDlg->GetAbsY() + 0.1f MINIMAPPOSY(510 - (l_Vector2.z / eDefaultTileRatio / MINIMAPH) / MINIMAPDIVIDE);
				RenderBoldOneTile(l_2DPos1, l_2DPos2, FALSE, outLine);

				l_2DPos1.x = pMiniMapDlg->GetAbsX() - 0.1f MINIMAPPOSX((l_Vector1.x / eDefaultTileRatio / MINIMAPW) / MINIMAPDIVIDE);
				l_2DPos1.y = pMiniMapDlg->GetAbsY() - 0.1f MINIMAPPOSY(510 - (l_Vector1.z / eDefaultTileRatio / MINIMAPH) / MINIMAPDIVIDE);
				l_2DPos2.x = pMiniMapDlg->GetAbsX() - 0.1f MINIMAPPOSX((l_Vector2.x / eDefaultTileRatio / MINIMAPW) / MINIMAPDIVIDE);
				l_2DPos2.y = pMiniMapDlg->GetAbsY() - 0.1f MINIMAPPOSY(510 - (l_Vector2.z / eDefaultTileRatio / MINIMAPH) / MINIMAPDIVIDE);
				RenderBoldOneTile(l_2DPos1, l_2DPos2, FALSE, inLine);

				l_2DPos1.x = pMiniMapDlg->GetAbsX() MINIMAPPOSX((l_Vector1.x / eDefaultTileRatio / MINIMAPW) / MINIMAPDIVIDE);
				l_2DPos1.y = pMiniMapDlg->GetAbsY() MINIMAPPOSY(510 - (l_Vector1.z / eDefaultTileRatio / MINIMAPH) / MINIMAPDIVIDE);
				l_2DPos2.x = pMiniMapDlg->GetAbsX() MINIMAPPOSX((l_Vector2.x / eDefaultTileRatio / MINIMAPW) / MINIMAPDIVIDE);
				l_2DPos2.y = pMiniMapDlg->GetAbsY() MINIMAPPOSY(510 - (l_Vector2.z / eDefaultTileRatio / MINIMAPH) / MINIMAPDIVIDE);
				RenderBoldOneTile(l_2DPos1, l_2DPos2, FALSE, inLine);
			}
		}
	}
}
#define TAMBAH +
#define TOLAK -
#define TAMBAHSAMA =
enum WARNAFLAG
{
	SATU = 0,
	DUA = 1,
	TIGA = 2,
	EMPAT = 3,
	HITAM = 4,
};
#define TAMBAH1 0.5f
#define TAMBAH2 0.8f
#define TAMBAH3 1.5f
#define TAMBAH4 2.0f
#define TAMBAH5 2.5f
void CMHMap::RenderBoldOneTile(VECTOR2 f_2DPos1, VECTOR2 f_2DPos2, BOOL ForBigMap, DWORD color)
{
	VECTOR2 o_2DPos1[5], o_2DPos2[5], t_2DPos1[5], t_2DPos2[5], g_2DPos1[5], g_2DPos2[5];
	t_2DPos1[SATU].x = f_2DPos1.x;
	t_2DPos1[SATU].y = f_2DPos1.y;
	t_2DPos2[SATU].x = f_2DPos2.x;
	t_2DPos2[SATU].y = f_2DPos2.y;
	o_2DPos1[SATU].x TAMBAHSAMA f_2DPos1.x TOLAK TAMBAH1;
	o_2DPos2[SATU].x TAMBAHSAMA f_2DPos2.x TOLAK TAMBAH1;
	g_2DPos1[SATU].x TAMBAHSAMA f_2DPos1.x TAMBAH TAMBAH1;
	g_2DPos2[SATU].x TAMBAHSAMA f_2DPos2.x TAMBAH TAMBAH1;
	o_2DPos1[SATU].y TAMBAHSAMA f_2DPos1.y TOLAK TAMBAH1;
	o_2DPos2[SATU].y TAMBAHSAMA f_2DPos2.y TOLAK TAMBAH1;
	g_2DPos1[SATU].y TAMBAHSAMA f_2DPos1.y TAMBAH TAMBAH1;
	g_2DPos2[SATU].y TAMBAHSAMA f_2DPos2.y TAMBAH TAMBAH1;
	o_2DPos1[DUA].x TAMBAHSAMA f_2DPos1.x TOLAK TAMBAH2;
	o_2DPos2[DUA].x TAMBAHSAMA f_2DPos2.x TOLAK TAMBAH2;
	g_2DPos1[DUA].x TAMBAHSAMA f_2DPos1.x TAMBAH TAMBAH2;
	g_2DPos2[DUA].x TAMBAHSAMA f_2DPos2.x TAMBAH TAMBAH2;
	o_2DPos1[DUA].y TAMBAHSAMA f_2DPos1.y TOLAK TAMBAH2;
	o_2DPos2[DUA].y TAMBAHSAMA f_2DPos2.y TOLAK TAMBAH2;
	g_2DPos1[DUA].y TAMBAHSAMA f_2DPos1.y TAMBAH TAMBAH2;
	g_2DPos2[DUA].y TAMBAHSAMA f_2DPos2.y TAMBAH TAMBAH2;
	o_2DPos1[TIGA].x TAMBAHSAMA f_2DPos1.x TOLAK TAMBAH3;
	o_2DPos2[TIGA].x TAMBAHSAMA f_2DPos2.x TOLAK TAMBAH3;
	g_2DPos1[TIGA].x TAMBAHSAMA f_2DPos1.x TAMBAH TAMBAH3;
	g_2DPos2[TIGA].x TAMBAHSAMA f_2DPos2.x TAMBAH TAMBAH3;
	o_2DPos1[TIGA].y TAMBAHSAMA f_2DPos1.y TOLAK TAMBAH3;
	o_2DPos2[TIGA].y TAMBAHSAMA f_2DPos2.y TOLAK TAMBAH3;
	g_2DPos1[TIGA].y TAMBAHSAMA f_2DPos1.y TAMBAH TAMBAH3;
	g_2DPos2[TIGA].y TAMBAHSAMA f_2DPos2.y TAMBAH TAMBAH3;
	o_2DPos1[EMPAT].x TAMBAHSAMA f_2DPos1.x TOLAK TAMBAH4;
	o_2DPos2[EMPAT].x TAMBAHSAMA f_2DPos2.x TOLAK TAMBAH4;
	g_2DPos1[EMPAT].x TAMBAHSAMA f_2DPos1.x TAMBAH TAMBAH4;
	g_2DPos2[EMPAT].x TAMBAHSAMA f_2DPos2.x TAMBAH TAMBAH4;
	o_2DPos1[EMPAT].y TAMBAHSAMA f_2DPos1.y TOLAK TAMBAH4;
	o_2DPos2[EMPAT].y TAMBAHSAMA f_2DPos2.y TOLAK TAMBAH4;
	g_2DPos1[EMPAT].y TAMBAHSAMA f_2DPos1.y TAMBAH TAMBAH4;
	g_2DPos2[EMPAT].y TAMBAHSAMA f_2DPos2.y TAMBAH TAMBAH4;
	o_2DPos1[HITAM].x TAMBAHSAMA f_2DPos1.x TOLAK TAMBAH5;
	o_2DPos2[HITAM].x TAMBAHSAMA f_2DPos2.x TOLAK TAMBAH5;
	g_2DPos1[HITAM].x TAMBAHSAMA f_2DPos1.x TAMBAH TAMBAH5;
	g_2DPos2[HITAM].x TAMBAHSAMA f_2DPos2.x TAMBAH TAMBAH5;
	o_2DPos1[HITAM].y TAMBAHSAMA f_2DPos1.y TOLAK TAMBAH5;
	o_2DPos2[HITAM].y TAMBAHSAMA f_2DPos2.y TOLAK TAMBAH5;
	g_2DPos1[HITAM].y TAMBAHSAMA f_2DPos1.y TAMBAH TAMBAH5;
	g_2DPos2[HITAM].y TAMBAHSAMA f_2DPos2.y TAMBAH TAMBAH5;
	DWORD inLine = color;
	DWORD outLine = inLine;
	if (ForBigMap)
	{
		g_pExecutive->GetRenderer()->RenderLine(&o_2DPos1[SATU], &o_2DPos2[SATU], inLine);
		g_pExecutive->GetRenderer()->RenderLine(&t_2DPos1[SATU], &t_2DPos2[SATU], inLine);
		g_pExecutive->GetRenderer()->RenderLine(&g_2DPos1[SATU], &g_2DPos2[SATU], inLine);
		g_pExecutive->GetRenderer()->RenderLine(&o_2DPos1[DUA], &o_2DPos2[DUA], inLine);
		g_pExecutive->GetRenderer()->RenderLine(&g_2DPos1[DUA], &g_2DPos2[DUA], inLine);
		g_pExecutive->GetRenderer()->RenderLine(&o_2DPos1[TIGA], &o_2DPos2[TIGA], inLine);
		g_pExecutive->GetRenderer()->RenderLine(&g_2DPos1[TIGA], &g_2DPos2[TIGA], inLine);
		g_pExecutive->GetRenderer()->RenderLine(&o_2DPos1[EMPAT], &o_2DPos2[EMPAT], inLine);
		g_pExecutive->GetRenderer()->RenderLine(&g_2DPos1[EMPAT], &g_2DPos2[EMPAT], inLine);
		g_pExecutive->GetRenderer()->RenderLine(&o_2DPos1[HITAM], &o_2DPos2[HITAM], outLine);
		g_pExecutive->GetRenderer()->RenderLine(&g_2DPos1[HITAM], &g_2DPos2[HITAM], outLine);
	}
	else
	{
		g_pExecutive->GetRenderer()->RenderLine(&o_2DPos1[SATU], &o_2DPos2[SATU], inLine);
		g_pExecutive->GetRenderer()->RenderLine(&t_2DPos1[SATU], &t_2DPos2[SATU], inLine);
		g_pExecutive->GetRenderer()->RenderLine(&g_2DPos1[SATU], &g_2DPos2[SATU], inLine);
		g_pExecutive->GetRenderer()->RenderLine(&o_2DPos1[DUA], &o_2DPos2[DUA], inLine);
		g_pExecutive->GetRenderer()->RenderLine(&g_2DPos1[DUA], &g_2DPos2[DUA], inLine);
	}
}
void CMHMap::RenderOneWayPoint(const VECTOR3& p_WayPointPos, DWORD p_Color)
{
	VECTOR3 l_L1, l_L2, l_L3, l_L4;
	VECTOR3 outl_L1, outl_L2, outl_L3, outl_L4;
	VECTOR2 v2_L1, v2_L2, v2_L3, v2_L4;
	l_L1.x = p_WayPointPos.x + 20;
	l_L1.y = p_WayPointPos.y;
	l_L1.z = p_WayPointPos.z - 40;
	l_L2.x = p_WayPointPos.x;
	l_L2.y = p_WayPointPos.y;
	l_L2.z = p_WayPointPos.z + 20;
	l_L3.x = p_WayPointPos.x - 40;
	l_L3.y = p_WayPointPos.y;
	l_L3.z = p_WayPointPos.z;
	l_L4.x = p_WayPointPos.x + 20;
	l_L4.y = p_WayPointPos.y;
	l_L4.z = p_WayPointPos.z;
	GetScreenXYFromXYZ(g_pExecutive->GetGeometry(), 0, &l_L1, &outl_L1);
	GetScreenXYFromXYZ(g_pExecutive->GetGeometry(), 0, &l_L2, &outl_L2);
	GetScreenXYFromXYZ(g_pExecutive->GetGeometry(), 0, &l_L3, &outl_L3);
	GetScreenXYFromXYZ(g_pExecutive->GetGeometry(), 0, &l_L4, &outl_L4);
	DWORD width = 0;
	DWORD height = 0;
	DISPLAY_INFO	dispInfo;
	GAMERESRCMNGR->GetDispInfo(&dispInfo);
	width = dispInfo.dwWidth;
	height = dispInfo.dwHeight;
	v2_L1.x = outl_L1.x * width;
	v2_L1.y = outl_L1.y * height;
	v2_L2.x = outl_L2.x * width;
	v2_L2.y = outl_L2.y * height;
	v2_L3.x = outl_L3.x * width;
	v2_L3.y = outl_L3.y * height;
	v2_L4.x = outl_L4.x * width;
	v2_L4.y = outl_L4.y * height;
	g_pExecutive->GetRenderer()->RenderLine(&v2_L1, &v2_L2, p_Color);
	g_pExecutive->GetRenderer()->RenderLine(&v2_L3, &v2_L4, p_Color);
}
VECTOR3 CMHMap::GetSafePosition( VECTOR3* p_NowPosition )
{
	VECTOR3 l_SafePosition;
	l_SafePosition.x = 0; l_SafePosition.y = 0; l_SafePosition.z = 0;	
	if (m_pTileManager && p_NowPosition)
	{
		DWORD l_NowPosX = (DWORD)(p_NowPosition->x/eDefaultTileRatio);
		DWORD l_NowPosZ = (DWORD)(p_NowPosition->z/eDefaultTileRatio);
		BOOL l_Result = FALSE;
		l_Result = m_pTileManager->FindClosest1xTile(l_SafePosition, l_NowPosX, l_NowPosZ);
		if (l_Result)
		{
			return l_SafePosition;
		}
	}
	return l_SafePosition;
}