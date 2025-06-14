#include "stdafx.h"
#include <map>
#include <string>
#include "MHFile.h"
using namespace std;
BOOL g_bAssertMsgBox = FALSE;
char _g_AssertErrorMessage[255];
#ifndef _MURIMNET_
#include <math.h>
#include "CommonGameFunc.h"
#ifdef _MHCLIENT_
#include "../ItemManager.h"
#include "../mhMap.h"
#include "../ChatManager.h"
#endif
#ifdef __MAPSERVER__
#ifndef _MURIMNET_
#include "itemManager.h"
#endif
#include "GameResourceManager.h"
#include "Object.h"
#endif
DWORD DayOfMonth[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
DWORD DayOfMonth_Yundal[12] = { 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
static char Days[7][16] = { "SUN", "MON", "TUE", "WED", "THU", "FRI", "SAT" };
static char WeatherState[][16] = { "CLEAR", "SNOW", "FLOWER" };
#ifdef _MHCLIENT_
void AddComma( char* pBuf )
{
	if( *pBuf == 0 ) return;
	char buf[64];
	SafeStrCpy( buf, pBuf, 64 );
	char* p = buf;
	*(pBuf++) = *(p++);
	int n = strlen( pBuf ) % 3;
	while( *p )
	{
		if( n != 0 )
		{
			*pBuf = *p;
			++p;
		}
		else
		{
			*pBuf = ',';
		}
		++pBuf;
		n = ( n == 0 ) ? 3 : n - 1;
	}
	*pBuf = 0;
}
char* AddComma( DWORD dwMoney )
{
	static char buf[32];
	wsprintf( buf, "%u", dwMoney );
	AddComma( buf );
	return buf;
}
char* RemoveComma( char* str )
{
	static char buf[32];
	char* p = buf;
	while( *str )
	{
		if( *str != ',' )
		{
			*p = *str;
			++p;
		}
		++str;
	}
	*p = 0;
	return buf;
}
#else
#endif
void SetProtocol(MSGBASE* pMsg,BYTE bCategory,BYTE bProtocol)
{
	pMsg->Category = bCategory;
	pMsg->Protocol = bProtocol;	
}
WP_KIND GetWeaponType(WORD mugongIdx)
{
	if(MIN_ATTACK_MUGONG_INDEX <= mugongIdx && mugongIdx < MAX_ATTACK_MUGONG_INDEX)
	{
		return (WP_KIND)((mugongIdx - MIN_ATTACK_MUGONG_INDEX)/ATTACK_MUGONG_INTERVAL);
	}
	else if(MIN_DEFENSE_MUGONG_INDEX <= mugongIdx && mugongIdx < MAX_DEFENSE_MUGONG_INDEX)
	{
		return (WP_KIND)((mugongIdx - MIN_DEFENSE_MUGONG_INDEX)/DEFENSE_MUGONG_INTERVAL);
	}
	else if(MIN_ASSIST_MUGONG_INDEX <= mugongIdx && mugongIdx < MAX_ASSIST_MUGONG_INDEX)
	{
		return (WP_KIND)((mugongIdx - MIN_ASSIST_MUGONG_INDEX)/ASSIST_MUGONG_INTERVAL);
	}
	return WP_ERR;
}
eITEMTABLE GetTableIdxPosition(WORD abs_pos)
{
	if(TP_INVENTORY_START <= abs_pos && abs_pos < TP_INVENTORY_END)
	{
		return eItemTable_Inventory;
	}
	else if(TP_WEAR_START <= abs_pos && abs_pos < TP_WEAR_END)
	{
		return eItemTable_Weared;
	}
	else if(TP_PYOGUK_START <= abs_pos && abs_pos < TP_PYOGUK_END)
	{
		return eItemTable_Pyoguk;
	}	
	else if(TP_QUICK_START <= abs_pos && abs_pos < TP_QUICK_END)
	{
		return eItemTable_Quick;
	}
#ifdef _JAPAN_LOCAL_
	else if(TP_MUGONG_START <= abs_pos && abs_pos < TP_MUGONG_END)
	{
		return eItemTable_Mugong;
	}
#elif defined _HK_LOCAL_
	else if(TP_MUGONG_START <= abs_pos && abs_pos < TP_MUGONG_END)
	{
		return eItemTable_Mugong;
	}
#elif defined _TL_LOCAL_
	else if(TP_MUGONG_START <= abs_pos && abs_pos < TP_MUGONG_END)
	{
		return eItemTable_Mugong;
	}
#else
	else if(TP_MUGONG1_START <= abs_pos && abs_pos < TP_MUGONG2_END)
	{
		return eItemTable_Mugong;
	}
#endif 
	else if(TP_JINBUB_START <= abs_pos && abs_pos < TP_JINBUB_END)
	{
		return eItemTable_Mugong;
	}
	else if(TP_ABILITY_START <= abs_pos && abs_pos < TP_ABILITY_END)
	{
		return eItemTable_Ability;
	}
	else if(TP_SHOPINVEN_START <= abs_pos && abs_pos < TP_SHOPINVEN_END)
	{
		return eItemTable_ShopInven;
	}
	else if(TP_PETINVEN_START <= abs_pos && abs_pos < TP_PETINVEN_END)
	{
		return eItemTable_PetInven;
	}
	else if(TP_TITANWEAR_START <= abs_pos && abs_pos < TP_TITANWEAR_END)
	{
		return eItemTable_Titan;
	}
	else if(TP_TITANMUGONG_START <= abs_pos && abs_pos < TP_TITANMUGONG_END)
	{
		return eItemTable_Mugong;
	}
	else if(TP_TITANSHOPITEM_START <= abs_pos && abs_pos < TP_TITANSHOPITEM_END)
	{
		return eItemTable_TitanShopItem;
	}

	else if (TP_CHANGELOOKINVEN_START <= abs_pos && abs_pos < TP_CHANGELOOKINVEN_END)
	{
		return eItemTable_ChangeLookInven;
	}
	else if (TP_CHANGELOOKWEAR_START <= abs_pos && abs_pos < TP_CHANGELOOKWEAR_END)
	{
		return eItemTable_ChangeLookWear;
	}

	return eItemTable_Max;
}
BOOL IsEquipItem(eITEM_KINDBIT type)
{
	if (type>=eEQUIP_ITEM&&type<=eEQUIP_ITEM_UNIQUE)
		return TRUE;
	else
		return FALSE;
}
eITEM_KINDBIT GetItemKind(WORD iconIdx)
{
#ifdef _MHCLIENT_
	if( ITEMMGR->GetItemInfo(iconIdx) )
		return (eITEM_KINDBIT)ITEMMGR->GetItemInfo(iconIdx)->ItemKind;
#endif
#ifdef __MAPSERVER__
	ITEM_INFO* pInfo = ITEMMGR->GetItemInfo(iconIdx);
	if(pInfo)
		return (eITEM_KINDBIT)pInfo->ItemKind;
	else
	{
		return eKIND_ITEM_MAX;
	}
#endif
	return (eITEM_KINDBIT)0;
}
eQUICKICON_KIND GetIconKind(WORD wIdx)
{
	if(wIdx >= MIN_ABILITY_INDEX)
		return eABILITY_ICON;	
	if(wIdx >= MIN_YOUNGYAKITEM_INDEX)
		return eITEM_ICON;
	else
		return eMUGONG_ICON;
}
eMUGONGITEM_KIND GetItemKindDetail(WORD iconIdx)
{
	eITEM_KINDBIT kind = GetItemKind(iconIdx);
	if( (kind & 1024) == FALSE)
		return eERROR_BOOK;
	if(kind == 1038)
	{
		return eJINBUBBOOK;
	}
	if(kind == 1037)
	{
		return eSIMBUBBOOK;
	}
	return eMUGONGBOOK;
}
POSTYPE ConvAbsPos2MugongPos(POSTYPE abspos)
{
#ifdef _JAPAN_LOCAL_
	if(abspos < TP_MUGONG_START)
	{
		ASSERT(0);
		return 0;
	}
	if(abspos >= TP_JINBUB_END)
	{
		ASSERT(0);
		return 0;
	}
	return abspos - TP_MUGONG_START;
#elif defined _HK_LOCAL_
	if(abspos < TP_MUGONG_START)
	{
		ASSERT(0);
		return 0;
	}
	if(abspos >= TP_JINBUB_END)
	{
		ASSERT(0);
		return 0;
	}
	return abspos - TP_MUGONG_START;
#elif defined _TL_LOCAL_
	if(abspos < TP_MUGONG_START)
	{
		ASSERT(0);
		return 0;
	}
	if(abspos >= TP_JINBUB_END)
	{
		ASSERT(0);
		return 0;
	}
	return abspos - TP_MUGONG_START;
#else
	if( abspos >= TP_MUGONG1_START && abspos < TP_TITANMUGONG_END )
		return abspos - TP_MUGONG1_START;
	else
	{
		ASSERT(0);
		return 0;
	}
#endif
}
DWORD GetMainTargetID(MAINTARGET* pMainTarget)
{
	if(pMainTarget->MainTargetKind == MAINTARGET::MAINTARGETKIND_OBJECTID)
		return pMainTarget->dwMainTargetID;
	else
		return 0;
}
#ifdef _MAPSERVER_
#ifndef _MURIMNET_
#include "UserTable.h"
#include "CharMove.h"
#endif
#endif
#ifdef _MHCLIENT_
#include "ObjectManager.h"
#endif
BOOL GetMainTargetPos(MAINTARGET* pMainTarget,VECTOR3* pRtPos, CObject ** ppObject)
{	
	if(pMainTarget->MainTargetKind == MAINTARGET::MAINTARGETKIND_POS)
	{
		pMainTarget->cpTargetPos.Decompress(pRtPos);
		if(ppObject)
			*ppObject = NULL;
		return TRUE;
	}
	else
	{
#ifdef _MAPSERVER_
		CObject* pTarget;
		pTarget = g_pUserTable->FindUser(pMainTarget->dwMainTargetID);
		if(ppObject)
			*ppObject = pTarget;
		if(pTarget == NULL)
			return FALSE;
		*pRtPos = *CCharMove::GetPosition(pTarget);
#endif
#ifdef _MHCLIENT_
		CObject* pTarget;
		pTarget = OBJECTMGR->GetObject(pMainTarget->dwMainTargetID);
		if(ppObject)
			*ppObject = pTarget;
		if(pTarget == NULL)
			return FALSE;
		pTarget->GetPosition(pRtPos);
#endif
		return TRUE;
	}
}
void SetVector3(VECTOR3* vec,float x,float y,float z)
{
	vec->x = x;
	vec->y = y;
	vec->z = z;
}
#ifdef _MHCLIENT_
void VRand(VECTOR3* pResult,VECTOR3* pOriginPos,VECTOR3* pRandom)
{
	int temp;
	temp = (int)pRandom->x;
	pResult->x = temp ? pOriginPos->x + (rand() % (2*temp)) - temp : pOriginPos->x;
	temp = (int)pRandom->y;
	pResult->y = temp ? pOriginPos->y + (rand() % (2*temp)) - temp : pOriginPos->y;
	temp = (int)pRandom->z;
	pResult->z = temp ? pOriginPos->z + (rand() % (2*temp)) - temp : pOriginPos->z;
}
void TransToRelatedCoordinate(VECTOR3* pResult,VECTOR3* pOriginPos,float fAngleRadY)
{
	if(pResult == pOriginPos)
	{
		VECTOR3 tempOri = *pOriginPos;
		pOriginPos = &tempOri;
	}
	static BOOL bb = FALSE;
	if(bb == FALSE)
	{
		MATRIX4 my;
		SetRotationYMatrix(&my,-fAngleRadY);
		TransformVector3_VPTR2(pResult,pOriginPos,&my,1);
	}
	else
	{
		RotateVectorAxisY(pResult,pOriginPos,fAngleRadY);
	}
}
void RotateVectorAxisX(VECTOR3* pResult,VECTOR3* pOriginVector,float fAngleRadX)
{
	if(fAngleRadX  == 0)
	{
		*pResult = *pOriginVector;
	}
	else
	{
		MATRIX4 mx;
		SetRotationXMatrix(&mx,-fAngleRadX);
		TransformVector3_VPTR2(pResult,pOriginVector,&mx,1);
	}
}
void RotateVectorAxisZ(VECTOR3* pResult,VECTOR3* pOriginVector,float fAngleRadZ)
{
	if(fAngleRadZ  == 0)
	{
		*pResult = *pOriginVector;
	}
	else
	{
		pResult->x = pOriginVector->x*cosf(fAngleRadZ) - pOriginVector->y*sinf(fAngleRadZ);
		pResult->y = pOriginVector->x*sinf(fAngleRadZ) + pOriginVector->y*cosf(fAngleRadZ);
		pResult->z = pOriginVector->z;
	}
}
void RotateVector( VECTOR3* pResult, VECTOR3* pOriginVector, float x, float y, float z )
{
	RotateVectorAxisX( pResult, pOriginVector, x );
	RotateVectorAxisY( pResult, pResult, y );
	RotateVectorAxisZ( pResult, pResult, z );
}
void RotateVector( VECTOR3* pResult, VECTOR3* pOriginVector, VECTOR3 vRot )
{
	RotateVectorAxisX( pResult, pOriginVector, vRot.x );
	RotateVectorAxisY( pResult, pResult, vRot.y );
	RotateVectorAxisZ( pResult, pResult, vRot.z );
}
#endif
void RotateVectorAxisY(VECTOR3* pResult,VECTOR3* pOriginVector,float fAngleRadY)
{
	if(fAngleRadY == 0)
	{
		*pResult = *pOriginVector;
	}
	else
	{
		pResult->x = pOriginVector->x*cosf(fAngleRadY) - pOriginVector->z*sinf(fAngleRadY);
		pResult->y = pOriginVector->y;
		pResult->z = pOriginVector->x*sinf(fAngleRadY) + pOriginVector->z*cosf(fAngleRadY);
	}
}
void BeelinePoint(VECTOR3 * origin, VECTOR3 * dir, float dist, VECTOR3 * result)
{
	float rrr = sqrt( dir->x*dir->x + dir->z*dir->z );
	result->x = origin->x + dir->x * dist / rrr;
	result->z = origin->z + dir->z * dist / rrr;
}
void AdjacentPointForOriginPoint(VECTOR3 * origin, VECTOR3 * target, float dist, VECTOR3 * result)
{
	float rrr = CalcDistanceXZ(origin,target);
	if(rrr < dist)
	{
		result->x = target->x;
		result->z = target->z;
		return;
	}
	result->x = origin->x + ( target->x - origin->x ) * dist / rrr;
	result->z = origin->z + ( target->z - origin->z ) * dist / rrr;
}
float CalcDistanceXZ(VECTOR3* v1,VECTOR3* v2)
{
	return sqrtf( (v1->x - v2->x)*(v1->x - v2->x) + (v1->z - v2->z)*(v1->z - v2->z) );
}
BOOL IsSameDay(char* strDay,WORD Year,WORD Month,WORD Day)
{
	char temp[64];
	sprintf(temp,"%d.%02d.%02d",Year,Month,Day);
	return strcmp(strDay,temp) == 0;
}
#endif	
#ifdef _MHCLIENT_
extern DWORD gUserID;
void SendAssertMsg(char* pStrFileName,int Line,char* pMsg)
{
	if(g_bAssertMsgBox)
	{
		wsprintf(_g_AssertErrorMessage, "ASSERT() file:%s,line:%d (%s) do you debug?", pStrFileName, Line, pMsg);
		if(IDYES == MessageBox(NULL, _g_AssertErrorMessage, NULL, MB_YESNO|MB_TOPMOST))
			__asm int 3;
		return;
	}
	char temp[256];
	if(pMsg)
		sprintf(temp,"%s\t%d\t%s",pStrFileName,Line,pMsg);
	else
		sprintf(temp,"%s\t%d",pStrFileName,Line);
	MSG_ASSERT msg;
	msg.SetMsg(gUserID,gHeroID,temp);
	NETWORK->Send(&msg,sizeof(msg));
}
#endif
void WriteAssertMsg(char* pStrFileName,int Line,char* pMsg)
{
	if(g_bAssertMsgBox)
	{
		wsprintf(_g_AssertErrorMessage, "ASSERT() file:%s,line:%d (%s) do you debug?", pStrFileName, Line, pMsg);
		if(IDYES == MessageBox(NULL, _g_AssertErrorMessage, NULL, MB_YESNO|MB_TOPMOST))
			__asm int 3;
		return;
	}
	char temp[256];
	if(pMsg)
		sprintf(temp,"%s line:%d, msg:%s, ",pStrFileName,Line,pMsg);
	else
		sprintf(temp,"%s line:%d, ",pStrFileName,Line);
	SYSTEMTIME time;
	char szFile[256] = {0,};
	GetLocalTime( &time );
#ifdef _MAPSERVER_
	sprintf( szFile, "./Log/MapServer/Assert_%02d_%4d%02d%02d.txt", g_pServerSystem->GetMapNum(), time.wYear, time.wMonth, time.wDay );
#else
	sprintf( szFile, "./Log/Client/Assert_%4d%02d%02d.txt", time.wYear, time.wMonth, time.wDay );
#endif
	static BOOL bWrite = FALSE;
	if( bWrite == FALSE )
	{
		DWORD dwFileSize;
		DWORD dwSizeLow;
		DWORD dwSizeHigh;
		HANDLE hFile;
		OFSTRUCT data;
		hFile = (HANDLE)::OpenFile(szFile,&data,OF_READ);
		if((HFILE)hFile == 0xffffffff)
		{
			::CloseHandle(hFile);
			{
				FILE* fp = fopen(szFile,"a+");
				if(fp)
				{
					fprintf(fp,"%stime:%s\n",temp,GetCurTimeToString());
					fclose(fp);
				}
			}
			return ;
		}
		dwSizeLow = GetFileSize(hFile, &dwSizeHigh);
		dwFileSize = dwSizeHigh << 16 | dwSizeLow;
		::CloseHandle(hFile);
		if(dwFileSize <= 1000000) 
		{
			FILE* fp = fopen(szFile,"a+");
			if(fp)
			{
				fprintf(fp,"%stime:%s\n",temp,GetCurTimeToString());
				fclose(fp);
			}
		}
		else
		{
			bWrite = TRUE;
		}
	}
}
void JackLogOutWithColorConsole(char* pMsg,DWORD rgb,char* pStrFileName)
{
	char path[256]/* = { 0, }*/;
	sprintf(path, ".\\ServerSet\\Report\\%s",pStrFileName);
#ifndef _CLIENTJ_
	g_Console.LOGANDFILE(path, rgb,pMsg);
#endif
}
void CriticalAssertMsg(char* pStrFileName,int Line,char* pMsg)
{
	wsprintf(_g_AssertErrorMessage, "ASSERT() file:%s,line:%d (%s) do you debug?", pStrFileName, Line, pMsg);
	if(IDYES == MessageBox(NULL, _g_AssertErrorMessage, NULL, MB_YESNO|MB_TOPMOST))
		__asm int 3;
	return;
}
void SafeStrCpy( char* pDest, const char* pSrc, int nDestBufSize )
{
	strncpy( pDest, pSrc, nDestBufSize -1 );
	pDest[nDestBufSize -1] = 0;
}
void SafeStrCpyEmptyChange( char* pDest, const char* pSrc, int nDestBufSize )
{
	char tmp[256] = { 0, };
	strncpy(tmp, pSrc, nDestBufSize-1);
	for(int i=0; i<256; i++)
	{
		if(tmp[i] == '_')		pDest[i] = ' ';
		else					pDest[i] = tmp[i];		
	}
	pDest[nDestBufSize-1] = 0;
}
char* GetCurTimeToString()
{
	static char temp[256];
	SYSTEMTIME sysTime;
	GetLocalTime( &sysTime );
	sprintf(temp,"[%04d-%02d-%02d %02d:%02d:%02d]",
		sysTime.wYear, sysTime.wMonth, sysTime.wDay, sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
	return temp;
}
#ifdef _MHCLIENT_
char* GetCurTimeToString(DWORD milliSec)
{
	DWORD dwCurTime = milliSec / 1000;
	DWORD year = 0, month = 0, day = 0, hour = 0, min = 0, sec = 0;
	static CHAR strTime[256] = { 0, };
	CHAR strTemp[128] = { 0, };
	ZeroMemory(strTime, sizeof(strTime));
	year = dwCurTime / YEARTOSECOND;
	dwCurTime = dwCurTime % YEARTOSECOND;
	month = dwCurTime / MONTHTOSECOND;
	dwCurTime = dwCurTime % MONTHTOSECOND;
	day = dwCurTime / DAYTOSECOND;
	dwCurTime = dwCurTime % DAYTOSECOND;
	hour = dwCurTime / HOURTOSECOND;
	dwCurTime = dwCurTime % HOURTOSECOND;
	min = dwCurTime / MINUTETOSECOND;
	dwCurTime = dwCurTime % MINUTETOSECOND;
	sec = dwCurTime;
	if(year)
	{
		sprintf(strTemp, CHATMGR->GetChatMsg(1600), year);
		strcat(strTime, strTemp);
	}
	if(month)
	{
		if(year)	strcat(strTime, " ");
		sprintf(strTemp, CHATMGR->GetChatMsg(1601), month);
		strcat(strTime, strTemp);
	}
	if(day)
	{
		if(year||month)	strcat(strTime, " ");
		sprintf(strTemp, CHATMGR->GetChatMsg(1602), day);
		strcat(strTime, strTemp);
	}
	if(hour)
	{
		if(year||month||day)	strcat(strTime, " ");
		sprintf(strTemp, CHATMGR->GetChatMsg(1603), hour);
		strcat(strTime, strTemp);
	}
	if(min)
	{
		if(year||month||day||hour)	strcat(strTime, " ");
		sprintf(strTemp, CHATMGR->GetChatMsg(1604), min);
		strcat(strTime, strTemp);
	}
	if(sec)
	{
		if(year||month||day||hour||min)	strcat(strTime, " ");
		sprintf(strTemp, CHATMGR->GetChatMsg(1605), sec);
		strcat(strTime, strTemp);
	}
	return strTime;
}
char* GetCurTimeToString(stTIME* stTime)
{
	static CHAR strTime[256] = { 0, };
	CHAR strTemp[128] = { 0, };
	DWORD year = 0, month = 0, day = 0, hour = 0, min = 0, sec = 0;
	ZeroMemory(strTime, sizeof(strTime));
	year = stTime->GetYear();
	month = stTime->GetMonth();
	day = stTime->GetDay();
	hour = stTime->GetHour();
	min = stTime->GetMinute();
	sec = stTime->GetSecond();
	if(year)
	{
		sprintf(strTemp, CHATMGR->GetChatMsg(1600), year);
		strcat(strTime, strTemp);
	}
	if(month)
	{
		if(year)	strcat(strTime, " ");
		sprintf(strTemp, CHATMGR->GetChatMsg(1601), month);
		strcat(strTime, strTemp);
	}
	if(day)
	{
		if(year||month)	strcat(strTime, " ");
		sprintf(strTemp, CHATMGR->GetChatMsg(1602), day);
		strcat(strTime, strTemp);
	}
	if(hour)
	{
		if(year||month||day)	strcat(strTime, " ");
		sprintf(strTemp, CHATMGR->GetChatMsg(1603), hour);
		strcat(strTime, strTemp);
	}
	if(min)
	{
		if(year||month||day||hour)	strcat(strTime, " ");
		sprintf(strTemp, CHATMGR->GetChatMsg(1604), min);
		strcat(strTime, strTemp);
	}
	if(sec)
	{
		if(year||month||day||hour||min)	strcat(strTime, " ");
		sprintf(strTemp, CHATMGR->GetChatMsg(1605), sec);
		strcat(strTime, strTemp);
	}
	return strTime;
}
#endif
DWORD GetCurTimeValue()
{
	char temp2[256];
	SYSTEMTIME ti;
	GetLocalTime(&ti);
	sprintf(temp2,"%d%02d%02d",ti.wYear,ti.wMonth,ti.wDay);
	DWORD v2 = atoi(temp2);
	return v2;
}
QSTATETYPE GetCurTime()	
{
	SYSTEMTIME systime;
	GetLocalTime( &systime );
	stTIME time;
	time.SetTime( systime.wYear-2015, systime.wMonth, systime.wDay, systime.wHour, systime.wMinute, systime.wSecond );
	return time.value;
}
DWORD GetCurTimeToSecond()
{
	stTIME curtime;
	curtime.value = GetCurTime();
	DWORD timetosecond = 0;
	timetosecond += curtime.GetYear() * YEARTOSECOND;
	timetosecond += curtime.GetMonth() * MONTHTOSECOND;
	timetosecond += curtime.GetDay() * DAYTOSECOND;
	timetosecond += curtime.GetHour() * HOURTOSECOND;
	timetosecond += curtime.GetMinute() * MINUTETOSECOND;
	timetosecond += curtime.GetSecond();
	return timetosecond;
}
void StringtimeToSTTIME(stTIME* Time, char* strTime)
{
 	char year[3]		= { 0, };
 	char month[3]		= { 0, };
 	char day[3]			= { 0, };
 	char hour[3]		= { 0, };
 	char minute[3]		= { 0, };
	strncpy(year, &strTime[2], 2);
 	strncpy(month, &strTime[5], 2);
 	strncpy(day, &strTime[8], 2);
 	strncpy(hour, &strTime[11], 2);
 	strncpy(minute, &strTime[14],2);
 	Time->SetTime( atoi(year), atoi(month), atoi(day), atoi(hour), atoi(minute), 0 );
}
BOOL CheckValidPosition(VECTOR3& pos)
{
	if(pos.x < 0 || pos.z < 0)
		return FALSE;
	if(pos.x >= 51200 || pos.z >= 51200)
		return FALSE;
	return TRUE;
}
float roughGetLength( float fDistX,float fDistY )
{
	if( fDistX < 0.0f ) fDistX = -fDistX;
	if( fDistY < 0.0f ) fDistY = -fDistY;
	int min = (int)(( fDistX > fDistY ) ? fDistY : fDistX);
	return ( fDistX + fDistY - ( min >> 1 ) - ( min >> 2 ) + ( min >> 4 ) );
}
map<string,DWORD> g_fileTable;
void LoadEffectFileTable(char* pListFileName)
{
#ifdef _MHCLIENT_
	CMHFile file;
	file.Init(pListFileName,"rb",MHFILE_FLAG_DONOTCHANGE_DIRECTORY_TOROOT);
	DWORD MaxEffectDesc = file.GetDword();
	DWORD effnum;
	char efffile[256];
	for(DWORD n=0;n<MaxEffectDesc;++n)
	{
		effnum = file.GetDword();
		file.GetString(efffile);
		strupr(efffile);
		if(strcmp(efffile,"NULL") == 0)
			continue;
		g_fileTable.insert(map<string,DWORD>::value_type(efffile,effnum));
	}	
	file.Release();
#endif
}
int FindEffectNum(char* pFileName)
{
#ifdef _MHCLIENT_
	if(pFileName[0] == '0' && pFileName[1] == 0)
		return 0;
	static char tempbuf[64];
	strcpy(tempbuf,pFileName);
	map<string,DWORD>::iterator iter;
	iter = g_fileTable.find(strupr(tempbuf));
	if(iter == g_fileTable.end())
	{
		return -1;
	}
	else
		return iter->second;
#else
	if(pFileName[0] == '0' && pFileName[1] == 0)
		return 0;
	else
		return 1;
#endif
}
void ERRORBSGBOX(char* str,...)
{
	char msg2[255];
	va_list argList;
	va_start(argList, str);
	vsprintf(msg2,str,argList);
	va_end(argList);
	MessageBox(NULL,msg2,NULL,NULL);
}
const char* GetDay(WORD nDay)
{
	return Days[nDay];
}
WORD GetDay(const char* strDay)
{
	for(WORD i = 0; i < 7; i++)
	{
		if(strcmp(strDay, Days[i]) == 0)
			return i;
	}
	return -1;
}
const char* GetWeather(WORD nWeatherState)
{
	return WeatherState[nWeatherState];
}
WORD GetWeather(const char* strWeather)
{
	for(WORD i = 0; i < eWS_Max; i++)
	{
		if(strcmp(strWeather, WeatherState[i]) == 0)
			return i;
	}
	return -1;
}
DWORD Convert2MilliSecond( DWORD Day, DWORD Hour, DWORD Minute, DWORD Second )
{
	DWORD dwValue = 0;
	dwValue += Day * DAYTOSECOND * SECTOMILLISEC;
	dwValue += Hour * HOURTOSECOND * SECTOMILLISEC;
	dwValue += Minute * MINUTETOSECOND * SECTOMILLISEC;
	dwValue += Second * SECTOMILLISEC;
	return dwValue;
}
eObjectKindGroup GetObjectKindGroup(WORD wObjectKind)
{
	if( (wObjectKind == eObjectKind_Mining) || (wObjectKind == eObjectKind_Collection) || (wObjectKind == eObjectKind_Hunt) )
	{
		return eOBJECTKINDGROUP_JOB;
	}
	return eOBJECTKINDGROUP_NONE;
}
float GetAlphaValue(DWORD dwTime, DWORD dwStartTime, BOOL bFlag)
{
	DWORD ToTime = dwTime;
	DWORD ElTime = gCurTime - dwStartTime;
	float alp = 0.0f;
	if(bFlag)	
	{
		alp = ElTime / (float)ToTime + 0.01f;
	}
	else	
	{
		alp = 1 - (ElTime / (float)ToTime);
	}
	return alp;
}
void WriteDebugFile( char* pStrFileName, int Line, char* pMsg )
{
	char temp[512] = {0,};
	if(pMsg)
        sprintf(temp,"%s line:%d, msg:%s, ", pStrFileName, Line, pMsg);
	else
		sprintf(temp,"%s line:%d, ", pStrFileName, Line);
	SYSTEMTIME time;
	char szFile[256] = {0,};
	GetLocalTime( &time );
	sprintf( szFile, "./Log/Debug_%4d%02d%02d.txt", time.wYear, time.wMonth, time.wDay );
	static BOOL bWrite = FALSE;
	if( bWrite == FALSE )
	{
		DWORD dwFileSize;
		DWORD dwSizeLow;
		DWORD dwSizeHigh;
		HANDLE hFile;
		OFSTRUCT data;
		hFile = (HANDLE)::OpenFile(szFile,&data,OF_READ);
		if((HFILE)hFile == 0xffffffff)
		{
			::CloseHandle(hFile);
			FILE* fp = fopen(szFile,"a+");
            if(fp)
			{
				fprintf(fp,"%stime:%s\n",temp,GetCurTimeToString());
				fclose(fp);
			}
			return ;
		}
		dwSizeLow = GetFileSize(hFile, &dwSizeHigh);
		dwFileSize = dwSizeHigh << 16 | dwSizeLow;
		::CloseHandle(hFile);
		if(dwFileSize <= 1000000) 
		{
			FILE* fp = fopen(szFile,"a+");
			if(fp)
			{
				fprintf(fp,"%stime:%s\n",temp,GetCurTimeToString());
				fclose(fp);
			}
		}
		else
		{
			bWrite = TRUE;
		}
	}
}
void _parsingKeywordString(char * inString, char ** outString)
{
	char * sp = inString;
	char * sp2 = *outString;
	while (*sp != 0)
	{

		if (IsDBCSLeadByte(*sp))
		{
			*sp2++ = *sp++;
			*sp2++ = *sp++;
		}
		else
		{
			switch (*sp)
			{
			case TEXT_DELIMITER:
			{
								   ++sp;
								   char tmp = *sp;
								   switch (tmp)
								   {
								   case TEXT_SPACECHAR:
								   {
														  *sp2 = ' ';
														  ++sp2;
								   }
									   break;
								   case TEXT_EMPTYCHAR:
								   {
														  *outString[0] = 0;
								   }
									   break;
								   case TEXT_DELIMITER:	//KES Ãß°¡
								   {
															*sp2 = '^';
															++sp2;
								   }
									   break;
								   }

			}
				break;
			default:
			{
					   *sp2 = *sp;
					   ++sp2;
			}
			}
			++sp;
		}
	}
	*sp2 = 0;
}
char * parsingKeyword(char * input)
{//utk version number client
	char buf[256] = { 0, };
	char * aa = buf;
	_parsingKeywordString(input, &aa);

	return buf;
}
void GetVec2(VECTOR2 m_absPos, WORD w, WORD h, VECTOR2 & TopLeft, VECTOR2& TopRight, VECTOR2& BottomLeft, VECTOR2& BottomRight)
{
	VECTOR2 o_MeasurePen;
	//the math
	{
		o_MeasurePen.x = m_absPos.x	- 1;
		o_MeasurePen.y = m_absPos.y	- 1;
		TopLeft = o_MeasurePen;

		o_MeasurePen.x = m_absPos.x	+ 1 + w - 1;
		o_MeasurePen.y = m_absPos.y	- 1;
		TopRight = o_MeasurePen;

		o_MeasurePen.x = m_absPos.x	+ 1 + w - 1;
		o_MeasurePen.y = m_absPos.y	+ 1 + h - 1;
		BottomRight = o_MeasurePen;

		o_MeasurePen.x = m_absPos.x	- 1;
		o_MeasurePen.y = m_absPos.y	+ 1 + h - 1;
		BottomLeft = o_MeasurePen;

		//----------------------------------------------seperated
		/*o_MeasurePen.x = m_absPos.x	- 1;
		o_MeasurePen.y = m_absPos.y	- 1;
		TopLeft = o_MeasurePen;

		o_MeasurePen.x = m_absPos.x	+ w;
		//o_MeasurePen.y = m_absPos.y	- 1;
		TopRight = o_MeasurePen;

		o_MeasurePen.x = m_absPos.x	- 1;
		o_MeasurePen.y = m_absPos.y	+ h		- 1;
		BottomLeft = o_MeasurePen;

		o_MeasurePen.x = m_absPos.x	+ w;
		o_MeasurePen.y = m_absPos.y	+ h;
		BottomRight = o_MeasurePen;

		//----------------------------------------------
		o_MeasurePen.x = m_absPos.x	- 1;
		o_MeasurePen.y = m_absPos.y	+ h;
		BottomLeft2 = o_MeasurePen;

		o_MeasurePen.x = m_absPos.x	+ w		- 1;
		o_MeasurePen.y = m_absPos.y	+ h;
		BottomRight2 = o_MeasurePen;	*/
	}
}
char * Vector2Text(float Vrect1, float Vrect2, DWORD x, DWORD y)
{
	char m_szTextDebugRect[128] = { 0, };
	sprintf(m_szTextDebugRect, "%1.0f %1.0f %d %d", Vrect1, Vrect2,x,y);
	return m_szTextDebugRect;
}
void printInt(int int2print)
{
	TCHAR str[32];
	wsprintf(str, TEXT("%d"), int2print);
	::MessageBox(NULL, str, TEXT(""), MB_OK);
}