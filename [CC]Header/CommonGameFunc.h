#ifndef __COMMONGAMEFUNC_H__
#define __COMMONGAMEFUNC_H__
class CObject;
extern BOOL g_bAssertMsgBox;
extern char g_szHeroIDName[];
char* GetCurTimeToString();
#ifdef _MHCLIENT_
char* GetCurTimeToString(DWORD milliSec);	
char* GetCurTimeToString(stTIME* stTime);	
#endif
DWORD GetCurTimeValue();
QSTATETYPE GetCurTime();	
DWORD GetCurTimeToSecond();
void StringtimeToSTTIME(stTIME* Time, char* strTime);
void SetProtocol(MSGBASE* pMsg,BYTE bCategory,BYTE bProtocol);
void SendAssertMsg(char* pStrFileName,int Line,char* pMsg);
void WriteAssertMsg(char* pStrFileName,int Line,char* pMsg);
void JackLogOutWithColorConsole(char* pMsg,DWORD rgb,char* pStrFileName);
void CriticalAssertMsg(char* pStrFileName,int Line,char* pMsg);
WP_KIND GetWeaponType(WORD mugongIdx);
eITEMTABLE GetTableIdxPosition(WORD abs_pos);
eQUICKICON_KIND GetIconKind(WORD wIdx);
BOOL IsEquipItem(eITEM_KINDBIT type);
eITEM_KINDBIT GetItemKind(WORD iconIdx);
eMUGONGITEM_KIND GetItemKindDetail(WORD iconIdx);
POSTYPE ConvAbsPos2MugongPos(POSTYPE abspos);
DWORD GetMainTargetID(MAINTARGET* pMainTarget);
BOOL GetMainTargetPos(MAINTARGET* pMainTarget,VECTOR3* pRtPos, CObject ** ppObject);
BOOL IsSameDay(char* strDay,WORD Year,WORD Month,WORD Day);
void SetVector3(VECTOR3* vec,float x,float y,float z);
float CalcDistanceXZ(VECTOR3* v1,VECTOR3* v2);
void VRand(VECTOR3* pResult,VECTOR3* pOriginPos,VECTOR3* pRandom);
void TransToRelatedCoordinate(VECTOR3* pResult,VECTOR3* pOriginPos,float fAngleRadY);
void RotateVectorAxisX(VECTOR3* pResult,VECTOR3* pOriginVector,float fAngleRadX);
void RotateVectorAxisY(VECTOR3* pResult,VECTOR3* pOriginVector,float fAngleRadY);
void RotateVectorAxisZ(VECTOR3* pResult,VECTOR3* pOriginVector,float fAngleRadZ);
void RotateVector( VECTOR3* pResult, VECTOR3* pOriginVector, float x, float y, float z );
void RotateVector( VECTOR3* pResult, VECTOR3* pOriginVector, VECTOR3 vRot );
void BeelinePoint(VECTOR3 * origin, VECTOR3 * dir, float dist, VECTOR3 * result);
void AdjacentPointForOriginPoint(VECTOR3 * origin, VECTOR3 * target, float dist, VECTOR3 * result);
#ifdef _MHCLIENT_
void AddComma( char* pBuf );
char* AddComma( DWORD dwMoney );
char* RemoveComma( char* str );
#endif
BOOL IsVillage();
void SafeStrCpy( char* pDest, const char* pSrc, int nDestBufSize );
void SafeStrCpyEmptyChange( char* pDest, const char* pSrc, int nDestBufSize );
BOOL CheckValidPosition(VECTOR3& pos);
float roughGetLength( float fDistX,float fDistY );
void LoadEffectFileTable(char* pListFileName);
int FindEffectNum(char* pFileName);
#define ASSERTVALID_POSITION(vec)	CheckValidPosition(vec)
void ERRORBSGBOX(char* str,...);
const char* GetDay(WORD nDay);
WORD GetDay(const char* strDay);
const char* GetWeather(WORD nWeatherState);
WORD GetWeather(const char* strWeather);
DWORD Convert2MilliSecond( DWORD Day, DWORD Hour, DWORD Minute, DWORD Second );
template <class T> void SetOnBit(T* BitFlag, int nIndex)
{
	int ddd = sizeof(T);
	if( nIndex < 0 || nIndex > ((sizeof(T))*8) -1 )
		return;
	if(BitFlag != NULL)
		*BitFlag |= (1 << nIndex);
}
template <class T> void SetOffBit(T* BitFlag, int nIndex)
{
	if( nIndex < 0 || nIndex > ((sizeof(T))*8) -1 )
		return;
	if(BitFlag != NULL)
		*BitFlag &= ~(0x01 << nIndex);
}
template <class T> BOOL CheckBit(T BitFlag, int nIndex)
{
	if( nIndex < 0 || nIndex > ((sizeof(T))*8) -1 )
		return FALSE;
	return BitFlag & (0x01 << nIndex);
}
eObjectKindGroup GetObjectKindGroup(WORD wObjectKind);
float GetAlphaValue(DWORD dwTime, DWORD dwStartTime, BOOL bFlag);
void WriteDebugFile( char* pStrFileName, int Line, char* pMsg );

void _parsingKeywordString(char * inString, char ** outString);
char * parsingKeyword(char * input);
void GetVec2(VECTOR2 m_absPos, WORD w, WORD h, VECTOR2 & TopLeft, VECTOR2& TopRight, VECTOR2& BottomLeft, VECTOR2& BottomRight);
char * Vector2Text(float Vrect1, float Vrect2, DWORD x, DWORD y);
void printInt(int int2print);
#endif 