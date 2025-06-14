




// MiniMapDlg.h: interface for the CMiniMapDlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MINIMAPDLG_H__72DF8430_5EB4_480A_B4D4_06BB23A02329__INCLUDED_)
#define AFX_MINIMAPDLG_H__72DF8430_5EB4_480A_B4D4_06BB23A02329__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./Interface/cDialog.h"
#include "MiniMapHeroIcon.h"
#include "cImageSelf.h"
//#include "cFindPatnIcon.h"

//weiye ×Ô¶¯Ñ°Â·Ïà¹Ø³£Á¿¶¨Òå 2018-04-28

#define  POINT_IMG_MAX	6
#define  POINT_IMG_START  226
#define  PATH_IMG_MAX	500
#define  PATH_IMG_IDX      233


class cStatic;
class cPushupButton;

struct MINIMAPIMAGE
{
	cImageSelf pMiniMapImage;
	DWORD ImageWidth, ImageHeight;
};

//SW050802 snake_legs
struct stParticle
{

	VECTOR2 vPos;
	VECTOR2 vDir;
	stParticle()
	{
		vPos.x = vPos.y = 0;
		vDir.x = vPos.y = 0;
	}
};

struct stNumImage	//SW050722 for jackpot number dialog
{
	cImage* pImage;
	DWORD dwW,dwH;
	stNumImage():pImage(NULL),dwW(0),dwH(0){}
};

struct stCipherNum	//SW050722 for jackpot number dialog
{
	stCipherNum():dwNumber(99),dwRealCipherNum(0),bIsAni(FALSE){}
	DWORD dwNumber;// ÀÚ¸®¼ö ¼ýÀÚ (Ãâ·Â¿ë)

	DWORD dwRealCipherNum;// ÀÚ¸®¼ö ¼ýÀÚ (½ÇÁ¦°ª)
	bool bIsAni;// ¼ýÀÚ ¾Ö´Ï ÁøÇà ÁßÀÎÁö..(¾Ö´Ï¿ë)
};
/*
struct  FindPathPoint  //weiye  µØÍ¼×Ô¶¯Ñ°Â·Â·¾¶µã½á¹¹Ìå 2018-03-30
{
	int            Idx;
	cImage    m_Image;
	VECTOR2 m_PathPoint;
};*/

class CMiniMapDlg : public cDialog  
{
//	DWORD TempID;
	
	cStatic* m_pMapName;

	cStatic* m_pPosXY;	

	cPushupButton* m_pZoom;	

	MAPTYPE m_MapNum;

	CMiniMapHeroIcon m_HeroIcon;
//	CFindPatnIcon	m_FindPathIcon;
	CYHHashTable<CMiniMapIcon> m_IconTable;
	CMemoryPoolTempl<CMiniMapIcon> m_IconPool;
	CMiniMapIconImage m_pIconImage[eMNMICON_MAX];
	
	int m_CurMode;
	MINIMAPIMAGE m_MinimapImage[eMINIMAPMODE_MAX];
	cImage m_TooltipImage;

	float m_DisplayMinX,m_DisplayMinZ;		

	float m_DisplayMaxX,m_DisplayMaxZ;		
	VECTOR2 m_DiplayOriginPosOfImage;
	VECTOR2 m_ImageDisplayWH;

	cButton*	pGuildWarBtn;

	void InitIconImage();

	void SetZoomRate(float zoomrate);

	CMiniMapIcon* GetEmptyMiniMapIcon(WORD Kind,CObject* pObject);
	CMiniMapIcon* GetEmptyMiniMapIcon(WORD Kind,PARTY_MEMBER* pMember);
	void ReleaseMinniMapIcon(CMiniMapIcon* pIcon);

	void LoadMinimapImageInfo(MAPTYPE MapNum);

	void AddIcon(WORD Kind,CObject* pObject);
	void AddIcon(WORD Kind,PARTY_MEMBER* pMember);

//SW050722 
#define NUMIMAGE_W 8

#define NUMIMAGE_H 14


#define BASIC_ANI_TIMELENGTH	2000
#define BETWEEN_ANI_TIMELENGTH	500		//interval between Numbers
#define NUM_CHANGE_TIMELENGTH	10		//interval
#define	DOT_NUM					2

	enum{DEFAULT_IMAGE = 99, NUM_0 = 0, NUM_1, NUM_2, NUM_3, NUM_4, NUM_5, NUM_6, NUM_7, NUM_8, NUM_9, IMG_DOT, IMAGE_COUNT, CIPHER_NUM = 9};

	stNumImage	m_stNumImage[IMAGE_COUNT];	// ¼ýÀÚ ÀÌ¹ÌÁö
	VECTOR2		m_vPos[CIPHER_NUM];		// ¼ýÀÚÃ¢ ÀÚ¸®¼ö À§Ä¡
	VECTOR2		m_vDotPos[DOT_NUM];		//SW050805	ÀÚ¸®¼ö ±¸ºÐÁ¡ À§Ä¡ Ãß°¡

	DWORD		m_dwTotalMoney;			// ÃÑ±Ý¾×	// Ãâ·ÂÀ» À§ÇÑ °¡º¯ ±Ý¾×

	DWORD		m_dwOldTotalMoney;			// ÀúÀå ±Ý¾×(º¯È­ºñ±³) // ºñ±³¸¦ À§ÇÑ ½ÇÁ¦ ±Ý¾×
	DWORD		m_dwTempMoney;			// @»çÁ·	//

	stCipherNum m_stCipherNum[CIPHER_NUM];	// °¢ ÀÚ¸®º° Ãâ·ÂÇÒ ¼ýÀÚ
	DWORD		m_dwAniStartTime;		// ¼ýÀÚ ¾Ö´Ï ½ÃÀÛ ½Ã°£
	DWORD		m_dwNumChangeTime;		// ÀÚ¸® ¾Ö´Ï º¯È¯ °£°Ý	// ¼ýÀÚ °»½Å °£°Ý
	DWORD		m_dwIntervalAniTime;	// ÀÚ¸®°£ ¾Ö´Ï º¯È¯ °£°Ý // ¸ÕÀú ÀÚ¸® ¾Ö´Ï°¡ ³¡³ª¸é ..Áö³ª ´ÙÀ½ ÀÚ¸® ¾Ö´Ï Á¾·á ÇÏ´Â ½Ä~
	DWORD		m_dwMaxCipher;			// ±Ý¾× ÃÑ ÀÚ¸®¼ö (¾Ö´Ï¿ë)
	DWORD		m_dwCipherCount;		// ÀÚ¸®¼ö (¾Ö´Ï¿ë)
	bool		m_bIsAnimationing;			// ÀüÃ¼ ¾Ö´Ï ÁøÇà ÁßÀÎÁö..(¾Ö´Ï¿ë)
	bool		m_bDoSequenceAni;			// ForSequenceAni

	VECTOR3		vDestPos;
	VECTOR2		m_PointPosSave;

	//cImageSelf* m_PathFlag;

	cImageSelf* m_PathFlagTest;

/*	//SW050802 snake_legs
#define RND_RANGE	10
#define PARTICLE_PER_NUM 36
#define GRAVITY	0.5f
	stParticle	m_stPtc[CIPHER_NUM][36];
	DWORD	m_BeforeTime;*/
public:


	CMiniMapDlg();
	virtual ~CMiniMapDlg();
//KES
	virtual DWORD ActionEvent( CMouse* mouseInfo );
//	
	void InitMiniMap(MAPTYPE MapNum);
	

	void Linking();
	virtual void SetActive( BOOL val );

	void ToggleMinimapMode();
	
	void AddMonsterIcon(CObject* pObject);
	void AddBossMonsterIcon(CObject* pObject);
	void AddStaticNpcIcon(CObject* pObject,BYTE bFlag=0);


	void AddPartyMemberIcon(PARTY_MEMBER* pMember);
	void AddPartyMasterIcon(CObject* pObject);

	void AddQuestIcon( CObject* pTarget, int nKind );
	
	void AddHeroIcon(CObject* pObject);
//	void SetPartyIcon();

	void RemoveIcon(DWORD ObjectID);

	void FitToCenterIcon(CMiniMapIcon* pIcon);
	void Process();
	void Render();

	void SetPartyIconTargetPos(DWORD PlayerID, int posX, int posZ);
	void SetPartyIconObjectPos(DWORD PlayerID, int posX, int posZ);
	
	int GetCurMode()	{ return m_CurMode; }
	
	BOOL CanActive();

//	CMiniMapIconImage* GetMiniMapImage(WORD Kind) {	return &m_pIconImage[Kind]; }

	void RefreshMode();
	void ShowQuestMarkIcon( CObject* pTarget, int nKind );

//SW050722 
	void InitNumImage();				// ¼ýÀÚ ÀÌ¹ÌÁö µî·Ï
	void ReleaseNumImage();				// ¼ýÀÚ ÀÌ¹ÌÁö ÇØÁ¦
	void SetNumImagePos();				// ¼ýÀÚ ÀÌ¹ÌÁö Ãâ·Â ÁÂÇ¥ ÁöÁ¤	
	void ConvertCipherNum();			// ´ÜÀ§º° ¼ýÀÚ
	bool IsNumChanged();				// ¼ýÀÚ º¯È­ Ã¼Å©	// Check Num charnges
//	void DoNumChangeAni();				// ¼ýÀÚ º¯È¯ ¾Ö´Ï	// Ticktime control
	void DoAni();
	void DoSequenceAni();
	void JPRender();
	void JPProcess();

	//SW050802 snake_legs
/*	void SetParticlePos();
	void CalcMotion();*/
	
	
	void InitForAni();
	void InitForSequenceAni();
	void JPInit();

	//weiye Ð¡µØÍ¼×Ô¶¯Ñ°Â·º¯ÊýÉèÖÃ 2018-03-12

	cImage  m_PointImage[6];  //weiye  6Í¼ÊµÏÖÑ°Â·×ø±êÍ¼±ê¶¯Ì¬»¯

	VECTOR2  m_PointTemp;       //weiye  Ñ°Â·È«ÆÁÄ»ÁÙÊ±×ø±ê

	VECTOR2  m_PointPos;

	//BOOL     m_IsPointRender;  //weiye  ÊÇ·ñäÖÈ¾Ñ°Â·Í¼±ê Ä¬ÈÏ²»äÖÈ¾

	//int      m_PointImgCount;

	void       SetPointPos(VECTOR2 Pos){m_PointPos=Pos;}

	VECTOR2 * GetPointPos(){return &m_PointPos;}

	//void      SetPointRender(BOOL Val) {m_IsPointRender=FALSE;}

	//weiye ×Ô¶¯Ñ°Â·Â·¾¶µãº¯Êý¶¨Òå 2018-03-30

	//void     AddPathPointInfo(float x, float z);
	//void     ReleasePathPointInfo();

	/*void ClearMovePoint() 
	{ 
		m_FindPathIcon.ClearPoint();
	}		
	void SetMovePoint(float x,float z){m_FindPathIcon.SetMousePoint(x,z);}*/
	void Move_UsePath();

	VECTOR2 * GetPointPosSave(){return &m_PointPosSave;}

	void CMiniMapDlg::ShwoGuildWarBtn(BOOL bl);

	void ClearMovePoint() {m_IsPointRender=FALSE; }
	void SetMovePoint();//{m_IsPointRender=TRUE;}

	BOOL m_IsPointRender;
	void SetNewMovePoint(VECTOR2 Pos){m_PointPosSave=Pos;}

	BOOL IsMovePoint(){return m_IsPointRender;}

	//BOOL IsRendering(){return m_FindPathIcon.IsRendering();}
};


#endif // !defined(AFX_MINIMAPDLG_H__72DF8430_5EB4_480A_B4D4_06BB23A02329__INCLUDED_)


