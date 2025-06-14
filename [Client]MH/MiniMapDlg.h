




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

//weiye �Զ�Ѱ·��س������� 2018-04-28

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
	DWORD dwNumber;// �ڸ��� ���� (��¿�)

	DWORD dwRealCipherNum;// �ڸ��� ���� (������)
	bool bIsAni;// ���� �ִ� ���� ������..(�ִϿ�)
};
/*
struct  FindPathPoint  //weiye  ��ͼ�Զ�Ѱ··����ṹ�� 2018-03-30
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

	stNumImage	m_stNumImage[IMAGE_COUNT];	// ���� �̹���
	VECTOR2		m_vPos[CIPHER_NUM];		// ����â �ڸ��� ��ġ
	VECTOR2		m_vDotPos[DOT_NUM];		//SW050805	�ڸ��� ������ ��ġ �߰�

	DWORD		m_dwTotalMoney;			// �ѱݾ�	// ����� ���� ���� �ݾ�

	DWORD		m_dwOldTotalMoney;			// ���� �ݾ�(��ȭ��) // �񱳸� ���� ���� �ݾ�
	DWORD		m_dwTempMoney;			// @����	//

	stCipherNum m_stCipherNum[CIPHER_NUM];	// �� �ڸ��� ����� ����
	DWORD		m_dwAniStartTime;		// ���� �ִ� ���� �ð�
	DWORD		m_dwNumChangeTime;		// �ڸ� �ִ� ��ȯ ����	// ���� ���� ����
	DWORD		m_dwIntervalAniTime;	// �ڸ��� �ִ� ��ȯ ���� // ���� �ڸ� �ִϰ� ������ ..���� ���� �ڸ� �ִ� ���� �ϴ� ��~
	DWORD		m_dwMaxCipher;			// �ݾ� �� �ڸ��� (�ִϿ�)
	DWORD		m_dwCipherCount;		// �ڸ��� (�ִϿ�)
	bool		m_bIsAnimationing;			// ��ü �ִ� ���� ������..(�ִϿ�)
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
	void InitNumImage();				// ���� �̹��� ���
	void ReleaseNumImage();				// ���� �̹��� ����
	void SetNumImagePos();				// ���� �̹��� ��� ��ǥ ����	
	void ConvertCipherNum();			// ������ ����
	bool IsNumChanged();				// ���� ��ȭ üũ	// Check Num charnges
//	void DoNumChangeAni();				// ���� ��ȯ �ִ�	// Ticktime control
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

	//weiye С��ͼ�Զ�Ѱ·�������� 2018-03-12

	cImage  m_PointImage[6];  //weiye  6ͼʵ��Ѱ·����ͼ�궯̬��

	VECTOR2  m_PointTemp;       //weiye  Ѱ·ȫ��Ļ��ʱ����

	VECTOR2  m_PointPos;

	//BOOL     m_IsPointRender;  //weiye  �Ƿ���ȾѰ·ͼ�� Ĭ�ϲ���Ⱦ

	//int      m_PointImgCount;

	void       SetPointPos(VECTOR2 Pos){m_PointPos=Pos;}

	VECTOR2 * GetPointPos(){return &m_PointPos;}

	//void      SetPointRender(BOOL Val) {m_IsPointRender=FALSE;}

	//weiye �Զ�Ѱ··���㺯������ 2018-03-30

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


