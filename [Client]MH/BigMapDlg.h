





// BigMapDlg.h: interface for the CBigMapDlg class.
//
//////////////////////////////////////////////////////////////////////


#if !defined(AFX_BIGMAPDLG_H__F42A6DDD_5716_4053_9CEB_5DE1E5696AC2__INCLUDED_)
#define AFX_BIGMAPDLG_H__F42A6DDD_5716_4053_9CEB_5DE1E5696AC2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./Interface/cDialog.h"
#include "MiniMapHeroIcon.h"
#include "cImageSelf.h"
#include "cButton.h"

class cButton;
class CBigMapDlg : public cDialog
{
protected:

	CMiniMapHeroIcon				m_HeroIcon;	

	CYHHashTable<CMiniMapIcon>		m_IconTable;
	CMemoryPoolTempl<CMiniMapIcon>	m_IconPool;

	CMiniMapIconImage				m_pIconImage[eBMICON_MAX];

	CMiniMapIcon*					m_pIconForToolTip;

	cImageSelf		m_MapImage;
	WORD			m_MapNum;

	cImage			m_TooltipImage;

	cButton*		btnPath;
	VECTOR2			m_PointPos;
	VECTOR3			vDestPos;
	VECTOR2			m_PointPosSave;

	//cImageSelf* m_PathFlag;

	BOOL m_IsPointRender;

public:
	CBigMapDlg();
	virtual ~CBigMapDlg();


	virtual void SetActive( BOOL val );
	virtual void Render();
	void Linking();
	void LoadMinimapImageInfo( MAPTYPE MapNum );
	void InitBigMap( MAPTYPE MapNum );


	void Process();

	void InitIconImage();


	CMiniMapIcon* GetEmptyMiniMapIcon( WORD Kind,CObject* pObject );
	CMiniMapIcon* GetEmptyMiniMapIcon( WORD Kind,PARTY_MEMBER* pMember );
	void ReleaseMinniMapIcon( CMiniMapIcon* pIcon );
	void SetPartyIconObjectPos(DWORD PlayerID, int posX, int posZ);
	void SetPartyIconTargetPos(DWORD PlayerID, int posX, int posZ);
	
	void AddHeroIcon( CObject* pObject );
	void AddStaticNpcIcon( CObject* pObject,BYTE bFlag=0 );
	void AddPartyMemberIcon( PARTY_MEMBER* pMember );
	void AddIcon( WORD Kind, CObject* pObject );
	void AddIcon( WORD Kind, PARTY_MEMBER* pMember );

	void RemoveIcon( DWORD ObjectID );
	virtual DWORD ActionEvent( CMouse* mouseInfo );


	BOOL CanActive();


	void ShowQuestMarkIcon( CObject* pTarget, int nKind );

	void ClearMovePoint() {m_IsPointRender=FALSE; }
	void SetMovePoint(){m_IsPointRender=TRUE;}
	void SetNewMovePoint(VECTOR2 Pos){m_PointPosSave=Pos;}
	void Move_UsePath();
	//void SetPointPos(VECTOR2 Pos){m_PointPos=Pos;}
	VECTOR2 * GetPointPos(){return &m_PointPos;}
};

#endif // !defined(AFX_BIGMAPDLG_H__F42A6DDD_5716_4053_9CEB_5DE1E5696AC2__INCLUDED_)


