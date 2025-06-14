#pragma once
#include "cWindow.h"
class cStatic : public cWindow  
{
public:
	cStatic();
	virtual ~cStatic();
	virtual void Init(LONG x, LONG y, WORD wid, WORD hei, cImage * basicImage, LONG ID=0);
	virtual void Render();
	virtual DWORD ActionEvent(CMouse * mouseInfo);
	void SetFontIdx(WORD fontIdx);
	void SetStaticText(char * text);
	void SetStaticText(char * text, bool IsMove , int mSize);
	LONG GetStaticValue()				{	return (LONG)atoi(m_szStaticText);	}
	char * GetStaticText()				{	return m_szStaticText;				}
	void SetStaticValue(LONG text)		{	itoa(text, m_szStaticText, 10);		}
	void SetMultiLine(BOOL val=TRUE)	{	m_fMultiLine = val;					}
	void InitMultiLine();
	void SetTextXY(LONG x, LONG y)		{	m_textXY.x = x; m_textXY.y = y;		}
	void SetFGColor(DWORD color)		{	m_fgColor = color;					}
	void SetShadowTextXY(LONG x, LONG y)	{	m_shadowTextXY.x = x; m_shadowTextXY.y = y;		}
	void SetShadowColor(DWORD color)		{	m_shadowColor = color;							}
	void SetShadow(BOOL val)				{	m_bShadow = val;								}
	void SetAlign( int nAlign )				{	m_nAlign = nAlign; }
	void SetFontOutline(BOOL val)			{	m_gFontOutline = val; }
	void RenderFaceImage(LONG X,LONG Y,char * StaticText); 
	void SetRenderFaceImg(BOOL val);  
	void ClearFaceImg(char * StaticText,int StrLen);

	void SetFlickerRoar(BOOL bFlicker);
	void FlickerRoar();

	void SetMarqueeSpeed(DWORD dwSpeed=0){ m_gMarqueeSpeed = dwSpeed; }

	std::string marquee_limit(std::string& text, size_t limit);
protected:
	cCoord			m_textXY;
	cMultiLineText	m_StaticText;
	char			m_szStaticText[MAX_TEXT_SIZE];
	BOOL			m_fMultiLine;
	DWORD			m_fgColor;
	BOOL			m_bShadow;
	cCoord			m_shadowTextXY;
	DWORD			m_shadowColor;
	int				m_nAlign;

	BOOL            m_bFaceImage;         
	DWORD           m_glistenCount;
	DWORD           m_glisttenTime;

	DWORD			m_fgColor2;
	cImage			pNameImage[20];
	DWORD           m_glistenCount2;
	DWORD           m_glisttenTime2;
	BOOL			m_bFlicker2;
	BOOL			m_bFlActive2;
	DWORD			m_dwFlickerSwapTime2;
	DWORD			count2;

	BOOL			m_gIsMarquee;
	int				m_gMarqueeSize;
	DWORD			m_gMarqueeSpeed;

	BOOL			m_gFontOutline;

	char	m_szTextDebug[MAX_TEXT_SIZE];
	VECTOR2	m_SavedXY;
	WORD	m_SavedWidthHeight[2];
	int		m_SavedID;
	DWORD	m_DebugTimerKDBActivated;
	BOOL	m_DebugBoolean[4];
	void	DebugPressed();
};