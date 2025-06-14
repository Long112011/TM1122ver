#include "stdafx.h"
#include "GraphicEngine.h"
#include "EngineObject.h"
#include "ObjectManager.h"
#include "MHFile.h"
#define LIGHT_CONFIG_FILE "./Resource/Client/light.cfg"
float gTickPerFrame=0;
I4DyuchiGXExecutive* g_pExecutive = NULL;
cPtrList GarbageObjectList;
cPtrList EffectPool;
BOOL bRenderSky = FALSE;
BOOL g_bDisplayFPS = FALSE;
DWORD g_bColor;
BOOL g_bFixHeight = FALSE;
float g_fFixHeight = 0;
DISPLAY_INFO g_ScreenInfo; 
int g_SlowCount=0;
int g_FastCount=0;
HMODULE        g_hExecutiveHandle=0;

int m_PakCount=0;
DWORD __stdcall MHErrorHandleProc(ERROR_TYPE type,DWORD dwErrorPriority,void* pCodeAddress,char* szStr)
{
	if(dwErrorPriority == 0)
	{
		ASSERTMSG(0,szStr ? szStr : "?");
	}
	return 0;
}
CGraphicEngine::CGraphicEngine(HWND hWnd,DISPLAY_INFO* pDispInfo,DWORD MaxShadowNum,DWORD ShadowMapDetail,float SightDistance,DWORD FPS)
{
	m_BackGroungColor = 0;
	ASSERT(!g_pExecutive);
	CreateExecutive(hWnd,pDispInfo,MaxShadowNum,ShadowMapDetail,SightDistance,FPS);
	g_pExecutive = m_pExecutive;
	g_ScreenInfo = *pDispInfo;
	
	m_bRender = TRUE;

	m_PakCount = 0;
	//LoadMixItemInfo();
}
CGraphicEngine::~CGraphicEngine()
{
	UnLoadPack();
	ReleasePool();
	g_pExecutive = NULL;
	if(m_pExecutive)
	{
		m_pExecutive->DeleteAllGXEventTriggers();
		m_pExecutive->DeleteAllGXLights();
		m_pExecutive->DeleteAllGXObjects();
		m_pExecutive->UnloadAllPreLoadedGXObject(0);
		m_pExecutive->DeleteGXMap(NULL);
		m_pExecutive->Release();
		m_pExecutive = NULL;
	}
	
	FreeLibrary(g_hExecutiveHandle);
}
void CGraphicEngine::LoadMixItemInfo()
{
	char filename[64];
	CMHFile file;
	sprintf(filename, "./resource/client/GraphicEngine.BIN");
	if (!file.Init(filename, "rb"))
		return;
	//MessageBox(NULL, "loaded", "0", MB_OK);
	int n = 0;
	while (1)
	{
		if (file.IsEOF())
			break;

		m_ExCommand[n].index = file.GetWord();
		SafeStrCpy((char*)m_ExCommand[n].Command, file.GetString(), MAX_NAME_LENGTH + 1);

		//MessageBox(NULL, "1", (char*)m_ExCommand[n].Command, MB_OK);
		LoadPack((char*)m_ExCommand[n].Command);
		++m_PakCount;
		++n;
	}
	//MessageBox(NULL, "release", "0", MB_OK);
	file.Release();
}
void CGraphicEngine::LoadPack(LPCTSTR packedFileName)
{
	if (mPackedFileContainer.end() != mPackedFileContainer.find(packedFileName))
	{
		return;
	}

	I4DyuchiFileStorage* fileStorage = 0;
	m_pExecutive->GetFileStorage(
		&fileStorage);

	if (fileStorage)
	{
		mPackedFileContainer[packedFileName] = fileStorage->MapPackFile(
			LPTSTR(packedFileName));
	}

	//MessageBox(NULL, "loaded", "0", MB_OK);
}
void CGraphicEngine::UnLoadPack()
{
	I4DyuchiFileStorage* fileStorage = 0;
	m_pExecutive->GetFileStorage(
		&fileStorage);

	if (0 == fileStorage)
	{
		return;
	}

	while (false == mPackedFileContainer.empty())
	{
		const PackedFileContainer::iterator iterator = mPackedFileContainer.begin();
		HANDLE fileHandle = iterator->second;

		fileStorage->UnmapPackFile(
			fileHandle);

		mPackedFileContainer.erase(
			iterator);
	}
}
PACKFILE_NAME_TABLE CGraphicEngine::GetString()
{
	for(int i=0; i<m_PakCount; i++)
	{
		PACKFILE_NAME_TABLE table[] = 
		{
			{(char)m_ExCommand[i].Command,		0},
		};

		return (const PACKFILE_NAME_TABLE &)table;
	}
}
BOOL CGraphicEngine::CreateExecutive(HWND hWnd,DISPLAY_INFO* pDispInfo,DWORD MaxShadowNum,DWORD ShadowMapDetail,float SightDistance,DWORD FPS)
{
	g_SlowCount = 0;
	g_FastCount = 0;
	HRESULT hr = E_FAIL;
	g_hExecutiveHandle = LoadLibrary("SS3DExecutiveForMuk.dll");
	if (NULL != g_hExecutiveHandle)
	{
		CREATE_INSTANCE_FUNC        pFunc;
		pFunc = (CREATE_INSTANCE_FUNC)GetProcAddress(g_hExecutiveHandle,"DllCreateInstance");
		if (NULL != pFunc)
		{
			hr = pFunc((void**)&m_pExecutive);
			if (hr != S_OK)
			{		
				MessageBox(hWnd,"Error","Error",MB_OK);
				return FALSE;
			}
			if(g_bUsingEnginePack)
			{
				/*PACKFILE_NAME_TABLE nameTable[] = {
					{ "map.pak", 0 },
				};
				m_pExecutive->InitializeFileStorageWithoutRegistry(
					"SS3DFileStorage.dll",
					35000,
					10000,
					MAX_PATH,
					FILE_ACCESS_METHOD_FILE_OR_PACK,
					nameTable,
					_countof(nameTable));*/


				PACKFILE_NAME_TABLE table[] = 
				{
					//{"jack_map.pak",			0},	
					{"Effect.pak",		0},
					{"Character.pak",	0},
					{"Monster.pak",		0},
					{"Titan.pak",		0},
					{"Jack.pak",		0},
					{"Map.pak",			0},					
					{"Npc.pak",			0},
					{"Pet.pak",			0},	
					{ "MOD.pak",		0 },
					
				};
				m_pExecutive->InitializeFileStorageWithoutRegistry("SS3DFileStorage.dll", 40000, 10240, MAX_PATH, FILE_ACCESS_METHOD_FILE_OR_PACK, table, _countof(table));

				/*PACKFILE_NAME_TABLE table[] =
				{
					{ (char)m_ExCommand[m_PakCount].Command, 0 },
				};

				m_pExecutive->InitializeFileStorageWithoutRegistry("SS3DFileStorage.dll", 40000, 10240, MAX_PATH, FILE_ACCESS_METHOD_FILE_OR_PACK, table, sizeof(table) / sizeof(*table));*/
			}
			else
			{
				m_pExecutive->InitializeFileStorageWithoutRegistry("SS3DFileStorage.dll",40000,10240,MAX_PATH,FILE_ACCESS_METHOD_ONLY_FILE,NULL,0);
			}

			pDispInfo->dwRefreshRate = 0;

			m_pExecutive->InitializeWithoutRegistry("SS3DGeometryForMuk.dll","SS3DRendererForMuk.dll",hWnd, pDispInfo, 10000, 300,0,1, 0,MHErrorHandleProc);
						
			m_pExecutive->PreCreateLight(LIGHT_CONFIG_FILE,0);			
			m_pExecutive->GetGeometry()->SetDrawDebugFlag(0);			
			m_pExecutive->GetGeometry()->SetViewport(NULL,0);			
			m_pExecutive->GetGeometry()->SetAmbientColor(0, 0xaaaaaaaa);
			m_pExecutive->GetGeometry()->SetShadowFlag(ENABLE_PROJECTION_SHADOW | ENABLE_PROJECTION_TEXMAP);

			VECTOR3 pos;
			pos.x = pos.y = pos.z = 0;
			m_pExecutive->GetGeometry()->ResetCamera(&pos,100,SightDistance,gPHI/4,0);
			//m_pExecutive->GetRenderer()->BeginRender(0,0,0);
			//m_pExecutive->GetRenderer()->EndRender();
			m_pExecutive->SetFramePerSec(FPS);			
			//m_pExecutive->GetRenderer()->SetVerticalSync(TRUE);			
			m_pExecutive->GetRenderer()->EnableSpecular(FALSE);
			m_pExecutive->GetRenderer()->SetAlphaRefValue(5);

			return TRUE;
		}
	}
	return FALSE;
}


BOOL CGraphicEngine::BeginProcess(GX_FUNC pBeforeRenderFunc,GX_FUNC pAfterRenderFunc)
{
	if(NULL == m_pExecutive)
	{
		return FALSE;
	}

//	if (g_bDisplayFPS)
//		m_pExecutive->GetRenderer()->BeginPerformanceAnalyze();

	if(gTickTime > 100)//100
		++g_SlowCount;
	else if(g_SlowCount>0)
		--g_SlowCount;
	if(gTickTime < 50)
		++g_FastCount;
	else if(g_FastCount>0)
		--g_FastCount;
#ifndef _DEBUG
	try
	{
#endif
		return m_pExecutive->Run(m_BackGroungColor,pBeforeRenderFunc,pAfterRenderFunc,
			TRUE == bRenderSky ? BEGIN_RENDER_FLAG_DONOT_CLEAR_FRAMEBUFFER : 0);
			
			
			
			
#ifndef _DEBUG
	}
	catch(...)
	{
		return FALSE;
	}
#endif

	/*DWORD t_ReturnValue = 0;	
	if (bRenderSky)
	{
 		t_ReturnValue = m_pExecutive->Run(m_BackGroungColor,pBeforeRenderFunc,pAfterRenderFunc,BEGIN_RENDER_FLAG_DONOT_CLEAR_FRAMEBUFFER);
	}
	else
	{
		t_ReturnValue = m_pExecutive->Run(m_BackGroungColor,pBeforeRenderFunc,pAfterRenderFunc,0);
	}
	return t_ReturnValue;*/
}

void CGraphicEngine::EndProcess()
{	
#ifdef _DEBUG
	if(g_bDisplayFPS)
		m_pExecutive->GetRenderer()->EndPerformanceAnalyze();
#endif

	if( m_bRender )	//KES
		m_pExecutive->GetGeometry()->Present(NULL);

	ProcessGarbageObject();
}

void CGraphicEngine::SetBackGroundColor(DWORD color)
{
	m_BackGroungColor = color;
	g_bColor = color;
}

void CGraphicEngine::SetFixHeight(BOOL bFix,float height)
{
	g_bFixHeight = bFix;
	g_fFixHeight = height;
}

CObjectBase* GetSelectedObject(int MouseX,int MouseY, int PickOption)
{
	/*static CObjectBase* pSelectedObject;
	CEngineObject* pEngineObject;
	static VECTOR3 pos;
	static float dist;
	static POINT pt;
	pt.x = MouseX;
	pt.y = MouseY;
	DWORD modelindex,objectindex;
	GXOBJECT_HANDLE handle = g_pExecutive->GXOGetObjectWithScreenCoord(&pos,&modelindex,&objectindex,&dist,&pt,0,PickOption);	
	if(handle == NULL)
		return NULL;
	pEngineObject = (CEngineObject*)g_pExecutive->GetData(handle);
	if(pEngineObject == NULL)
		return NULL;	
	return pEngineObject->m_pObject;*/

	CEngineObject* pEngineObject = NULL;
	POINT pt = { MouseX, MouseY };
	PICK_GXOBJECT_DESC gxoDesc[5];	
	DWORD dwPickNum = g_pExecutive->GXOGetMultipleObjectWithScreenCoord( gxoDesc, 5, &pt, 0, PickOption   );
	for( DWORD i = 0 ; i < dwPickNum ; ++i )
	{
		if(gxoDesc[i].gxo)
		{
			pEngineObject = (CEngineObject*)g_pExecutive->GetData(gxoDesc[i].gxo);
			if( pEngineObject != NULL )
			{
				if( HERO )
				if( HERO->GetEngineObject() != pEngineObject )
					break;	
			}
		}
	}
	if(pEngineObject == NULL)
		return NULL;
	return pEngineObject->m_pObject;
}

CObjectBase* GetSelectedObjectBoneCheck(int MouseX, int MouseY)
{
	//static CObjectBase* pSelectedObject;
	CEngineObject* pEngineObject;
	static VECTOR3 pos;
	static float dist;
	static POINT pt;
	pt.x = MouseX;
	pt.y = MouseY;
	

	DWORD modelindex,objectindex;
	GXOBJECT_HANDLE handle = g_pExecutive->GXOGetObjectWithScreenCoord(&pos,&modelindex,&objectindex,&dist,&pt,0,PICK_TYPE_PER_BONE_OBJECT);	
	if(handle == NULL)
		return NULL;

	pEngineObject = (CEngineObject*)g_pExecutive->GetData(handle);
	if(pEngineObject == NULL)
		return NULL;
	
	return pEngineObject->m_pObject;
}

VECTOR3* GetPickedPosition(int MouseX,int MouseY)
{
	static VECTOR3 TargetPos;
	float fDist;
	POINT pt;
	pt.x = MouseX;
	pt.y = MouseY;
	BOOL bFound = TRUE;
	if(g_bFixHeight)
		TargetPos = GetXYZFromScreenXY2(g_pExecutive->GetGeometry(),MouseX,MouseY,g_ScreenInfo.dwWidth,g_ScreenInfo.dwHeight,g_fFixHeight);
	else
		bFound = g_pExecutive->GXMGetHFieldCollisionPointWithScreenCoord(
			&TargetPos, &fDist, &pt);
		
	if(bFound == FALSE)
		return NULL;

	return &TargetPos;
}

void AddGarbageObject(GXOBJECT_HANDLE handle)
{
	if(g_pExecutive)
	{
		g_pExecutive->SetData(handle,0);
		GarbageObjectList.AddHead(handle);
	}
}

void ProcessGarbageObject()
{
	GXOBJECT_HANDLE handle;
	while((handle = (GXOBJECT_HANDLE)GarbageObjectList.RemoveTail()) != NULL)//while(handle = (GXOBJECT_HANDLE)GarbageObjectList.RemoveTail())
	{
		if(g_pExecutive)
			g_pExecutive->DeleteGXObject(handle);
	}
}

void AddPool(GXOBJECT_HANDLE handle,char* filename)
{
	EffectGarbage* pGarbage = new EffectGarbage;
	strcpy(pGarbage->filename,filename);
	pGarbage->handle = handle;
	EffectPool.AddHead(pGarbage);
}
GXOBJECT_HANDLE GetObjectHandle(char* szFileName,GXSchedulePROC pProc,void* pData,DWORD dwFlag)
{
	EffectGarbage* pGarbage;
	PTRLISTPOS pos = EffectPool.GetHeadPosition();
	PTRLISTPOS beforepos;
	while(pos)
	{
		beforepos = pos;
		pGarbage = (EffectGarbage *)EffectPool.GetNext(pos);
		if(strcmp(pGarbage->filename,szFileName)==0)
		{
			EffectPool.RemoveAt(beforepos);
			GXOBJECT_HANDLE h = pGarbage->handle;
			g_pExecutive->SetData(h,pData);
			delete pGarbage;
			return h;
		}
	}

	return g_pExecutive->CreateGXObject(szFileName,pProc,pData,dwFlag);
}

void ReleasePool()
{
	EffectGarbage* pGarbage;
	while((pGarbage = (EffectGarbage*)EffectPool.RemoveTail())!=NULL)//while(pGarbage = (EffectGarbage*)EffectPool.RemoveTail())
	{
		delete pGarbage;
	}
}

BOOL IsGameSlow()
{
	return g_SlowCount > 5;
}

BOOL IsGameFast()
{
	return g_FastCount > 5;
}

void GetFieldHeight(VECTOR3* pPos)
{
	g_pExecutive->GXMGetHFieldHeight(&pPos->y,pPos->x,pPos->z);
}


BOOL GetCollisonPointWithRay(VECTOR3& From, VECTOR3& To,float height,VECTOR3& Result)
{
	if(From.y <= To.y)
		return FALSE;
	
	float	t = (-1*(height+From.y)) / (To.y - From.y);
	float	x = From.x+t * (To.x-From.x);
	float	z = From.z+t * (To.z-From.z);
	Result.x	=	x;
	Result.y	=	height;
	Result.z	=	z;
	
	return	TRUE;

	/*return	NULL;*/
}
