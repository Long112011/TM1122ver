#ifndef _cWINDOW_H_
#define _cWINDOW_H_
#if _MSC_VER > 1000
#pragma once
#endif 
#include "cObject.h"
#include "cImage.h"
#include "cMultiLineText.h"
#include "cWindowDef.h"

#define MAX_DISP_MODE	3

class CMouse;
class CKeyboard;
void defaultWindowFunc(LONG lId, void * p, DWORD we);
class cWindow : public cObject
{
public:
	cWindow();
	virtual ~cWindow();
	virtual void Init(LONG x, LONG y, WORD wid, WORD hei, cImage * basicImage, LONG ID=0);
	virtual void TestRender();
	virtual void Render();
	void ToolTipRender();
	virtual DWORD ActionEvent( CMouse * mouseInfo );			
	virtual DWORD ActionKeyboardEvent( CKeyboard* keyInfo ) { return WE_NULL; }	
	void SaveDispPoint(VECTOR2 point1, VECTOR2 point2, VECTOR2 point3);

	void SaveWindowBase(BYTE b){	m_bCheckDispPoint = TRUE; 
									nWindowBase = b; }

	virtual void Add(cWindow * wnd){}
	BOOL PtInWindow(LONG x, LONG y);
	virtual void SetAbsXY(LONG x, LONG y){ m_absPos.x=(float)x; m_absPos.y=(float)y; }
	void SetRelXY(LONG x, LONG y){ m_relPos.x=(float)x; m_relPos.y=(float)y; }
	void SetValidXY(LONG x, LONG y) { m_validPos.x = (float)x; m_validPos.y=(float)y; }
	virtual void SetAutoClose(BOOL val){}
	virtual BOOL IsAutoClose(){ return FALSE; }
	void SetWH(LONG x, LONG y){ m_width=(WORD)x; m_height=(WORD)y; }
	void SetWidth(LONG x){ m_width = (WORD)x; }

	void SetOffSetXY(DWORD x, DWORD y){ m_OffSetX = (DWORD)x; m_OffSetY = (DWORD)y; }

	void SetFontIdx(WORD wIdx){ m_wFontIdx=wIdx; }
	void SetFocus(BOOL val){ m_bFocus=val; }
	void SetDepend(BOOL val){ m_bDepend=val; }
	void SetMovable(BOOL val){ m_bMovable=val; }
	void SetBasicImage(cImage * image){ if(image) { m_BasicImage = *image; m_pCurImage = &m_BasicImage; } }
	void SetToolTip(char * msg, DWORD color=RGB_HALF(255,255,0), cImage * image=NULL, DWORD imgColor = TTCLR_DEFAULT );
	void SetAutoLineToolTip( const char*, DWORD color=RGB_HALF(255,255,0), cImage* image=NULL, DWORD lineSize=46 , cImage * imageOne=NULL,cImage * imageTwo=NULL);
	void SetToolModel(int type){toolTip.SetRenderModel(type);}    
	void SetHeaderImage(cImage * image){toolTip.SetHeaderImage(image);} 
	
	cImage * GetBasicImage() { return &m_BasicImage; }

	WORD GetTipLineCount();
	void IniTipLineCount();

	BOOL IsMovable(){ return m_bMovable; }
	BOOL IsDepend(){ return m_bDepend; }
	BOOL IsFocus(){ return m_bFocus; }
	WORD GetWidth(){ return m_width; } 
	WORD GetHeight(){ return m_height; }

	BYTE GetWindowBase(){ return nWindowBase; }

	virtual void SetHeight(WORD hei) { m_height = hei; }
	float GetAbsX(){ return m_absPos.x;	}
	float GetAbsY(){ return m_absPos.y;	}
	void SetAbsX(LONG x){ m_absPos.x=(float)x;	}
	void SetAbsY(LONG y){ m_absPos.y=(float)y;	}
	float GetRelX(){ return m_relPos.x;	}
	float GetRelY(){ return m_relPos.y;	}
	virtual void SetAlpha(BYTE al){ m_alpha = al; }
	virtual void SetOptionAlpha(DWORD dwAlpha) { m_dwOptionAlpha = dwAlpha; }
	WORD GetFontIdx() { return m_wFontIdx; }
	void SetToolTipImageRGB( DWORD color ) { toolTip.SetImageRGB( color ); }
	void SetImageRGB(DWORD val) { m_dwImageRGB = val; }
	void SetcbFunc(cbFUNC Func=defaultWindowFunc){ if(Func) cbWindowFunc=Func; }
	void ExcuteCBFunc( DWORD we );
	BOOL IsAlwaysTop() { return m_bAlwaysTop; }
	void SetAlwaysTop( BOOL bAlwaysTop ) { m_bAlwaysTop = bAlwaysTop; }
	void SetTopOnActive( BOOL bTop ) { m_bSetTopOnActive = bTop; }
	void SetDestroy() { m_bDestroy = TRUE; }
	void SetImageSrcRect(RECT * pRect){ CopyRect(&m_imgSrcRect, pRect); }
	void SetScale(cImageScale * scale)	{
											m_scale.x = scale->x;
											m_scale.y = scale->y;
										}

	virtual void SetFocusEdit(BOOL val) {}
	void AddToolTipImageStone(cImage * image,int Index);     
	void AddToolTipImageEquip(cImage * image,cImage * image2=NULL);

	cMultiLineText* GetToolTip(){return &toolTip;}
	void SetEquipItem();




	void SetToolTipLink			(cMultiLineText* toolTip){ m_pToolTipLink  = toolTip;}
	void SetRelationToolTip		(cMultiLineText* toolTip){ m_pRelationTip  = toolTip;}
	void SetRelationToolTip1	(cMultiLineText* toolTip){ m_pRelationTip1 = toolTip;}




	void AddToolTipLine( char* msg, DWORD color= TTTC_DEFAULT,cImage * m_LineImage=NULL,WORD m_FontIdx=0,BYTE bWhichLine=0); 
	void AddToolTipLine2( char* msg,char* msg2, DWORD color= TTTC_DEFAULT, DWORD color2= TTTC_DEFAULT,cImage * m_LineImage=NULL,WORD m_FontIdx=0,BYTE bWhichLine=0);

	void	SetDeltaPos(int nIndex, VECTOR2 point);

protected:
	void SetCurImage(cImage * image){ m_pCurImage = image; }
	void GetVariablePostion(SIZE& standardResolution, POINT& standardPosition, SIZE& currentResolution);
	cbFUNC cbWindowFunc;
	WORD m_wFontIdx;
	BOOL m_bFocus;
	BOOL m_bMovable;
	BOOL m_bDepend;				
	BOOL m_bDestroy;
	BOOL m_bAlwaysTop;
	BOOL m_bSetTopOnActive;
	cImage * m_pToolTipImage;			
	DWORD m_toolTipFgColor;

	DWORD m_dwViewCurTime;
	DWORD m_dwViewLastTime;

	DWORD m_dwViewHoldCurTime;
	DWORD m_dwViewHoldLastTime;

	LONG m_ttX, m_ttY;
	cMultiLineText toolTip;
	cMultiLineText StaticToolTip;
	cImage m_BasicImage;
	cImage * m_pCurImage;
	DWORD m_dwImageRGB;
	DWORD m_alpha;
	DWORD m_dwOptionAlpha;   
public:
	VECTOR2 m_absPos;
	VECTOR2 m_relPos;
	RECT	m_imgSrcRect;

	BOOL	m_bCheckDispPoint;
	RECT	m_renderRect;
	RECT	m_renderShadowRect;
	VECTOR2	m_vDispPoint[MAX_DISP_MODE];
private:
	VECTOR2 m_validPos;
protected:
	WORD m_width;
	WORD m_height;
	cImageScale		m_scale;

	DWORD m_OffSetX;
	DWORD m_OffSetY;

	cMultiLineText* m_pRelationTip;
	cMultiLineText* m_pRelationTip1;
	cMultiLineText* m_pToolTipLink;

	VECTOR2 m_vDeltaPos[2];

	BYTE nWindowBase;
protected:
	/// Window Event
	DWORD	m_WindowEvent;

	char	m_szTextDebug[MAX_TEXT_SIZE];
	VECTOR2	m_SavedXY;
	WORD	m_SavedWidthHeight[2];
	int		m_SavedID;
	DWORD	m_DebugTimerKDBActivated;
	BOOL	m_DebugBoolean[4];
	void	DebugPressed();

public:
	DWORD GetWindowEvent() const { return m_WindowEvent; }
	void SetWindowEvent(DWORD val) { m_WindowEvent = val; }
};
#endif 