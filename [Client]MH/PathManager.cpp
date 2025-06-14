








// PathManager.cpp: implementation of the CPathManager class.

//
//////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "PathManager.h"
#include "GameResourceManager.h"
#include "STRPath.h"
#include "MHMap.h"
//#include "AutoFindPathManager.h"   //weiye 自动寻路头文件引�? 2018-03-28

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
GLOBALTON(CPathManager);


CPathManager::CPathManager()

{
	m_pSTRPath = new CSTRPath;
	m_pMap = NULL;
}


CPathManager::~CPathManager()
{
	delete m_pSTRPath;
}

void CPathManager::SetMap(CMHMap * pMap, MAPTYPE mapNum, int width) 
{ 
	m_pMap = pMap; 
	m_MapType = mapNum; 
	m_pSTRPath->SetWidth(width); 

	//weiye 自动寻路设置MAP信息 2018-03-28
	//AUTOFINDPATHMGR->SetMap(pMap,mapNum,width);
}

//weiye 获取两个点之间的路径  通过碰撞检�?2018-03-12

int CPathManager::GetPath(VECTOR3 * pSrcPos, VECTOR3 * pDestPos, VECTOR3 * pWayPointPos, WORD wBufCount, BYTE& wWayPointNum, CObject* pObject, BOOL bReverse, WORD wDepth)
{
	VECTOR3 TargetPos;

	if(m_pMap->CollisionTilePos((int)pDestPos->x, (int)pDestPos->z, m_MapType,pObject))
	{

		if(m_pMap->CollisionLine(pSrcPos, pDestPos, &TargetPos, m_MapType,pObject))
		{
			pWayPointPos[0] = TargetPos; wWayPointNum = 1;

			if(m_pMap->CollisionTilePos((int)TargetPos.x, (int)TargetPos.z, m_MapType,pObject))
			{
				return PATH_FAILED;
			}
			return PATH_SUCCESS;
		}
		else
		{	
			return PATH_FAILED;
		}
	}

	if(!m_pMap->CollisionLine(pSrcPos, pDestPos, &TargetPos, m_MapType,pObject))
	{
		pWayPointPos[0] = TargetPos; wWayPointNum = 1;
		return PATH_SUCCESS;
	}

	if( m_pMap->NonCollisionLine(pSrcPos, pDestPos, &TargetPos, m_MapType, pObject) )  //weiye 任何时候返回值为0   2018-03-28
	{

		pWayPointPos[0] = TargetPos; wWayPointNum = 1;
		return PATH_SUCCESS;
	}

	//---KES FindPatch Fix 071020
	//---Comment (no need to find a path)
	
	m_pSTRPath->IsValidNode = CPathManager::ValidFunc;
	if(!m_pSTRPath->FindPath(pSrcPos, pDestPos, pWayPointPos, wBufCount, wWayPointNum, wDepth))
	{

		if(bReverse)
		{
			return PATH_BLOCKFAILED;
		}
		else
		{		

			if(m_pMap->CollisionLine(pSrcPos, pDestPos, &TargetPos, m_MapType,pObject))
			{

				pWayPointPos[0] = TargetPos; wWayPointNum = 1;
				return PATH_SUCCESS;
			}
			else
			{
				return PATH_FAILED;
			}
		}
	}
	//pWayPointPos[0] = *pDestPos; wWayPointNum = 1;
	return PATH_SUCCESS;

}

BOOL CPathManager::ValidFunc(int cellX, int cellY,CObject* pObject)
{
	if(!PATHMGR->m_pMap->IsInTile(cellX, cellY, PATHMGR->m_MapType,pObject))
		return FALSE;

	if(!PATHMGR->m_pMap->CollisionTileCell(cellX, cellY, PATHMGR->m_MapType,pObject))
		return FALSE;

	return TRUE;
}

//weiye 检测当前位置和目标位置是否有路径可�?2018-05-01

BOOL  CPathManager::CheckCollisionLine(VECTOR3 * pSrcPos, VECTOR3 * pDestPos,CObject* pObject)
{
	VECTOR3 TargetPos;
	return m_pMap->CollisionLine(pSrcPos,pDestPos,&TargetPos, m_MapType,pObject);
}