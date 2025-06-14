#ifndef _cMULTILINETEXT_H_
#define _cMULTILINETEXT_H_
#if _MSC_VER > 1000
#pragma once
#endif 
#include "cImage.h"
#define TOOLTIP_MOUSESIZE_X	34
#define TOOLTIP_MOUSESIZE_Y	28

#define MAX_TOOLTIP_LINEEX	3

class LINE_NODE
{
public:
	LINE_NODE()
	{
		memset(line, 0, 256);
		memset(line2, 0, 256);
		nextLine = NULL;
		len = 0;
		len2 = 0;
		color = 0xffffffff;
		color2 = 0xff000000;
		fontIdx = 0;

		ZeroMemory(lineEx, sizeof(lineEx));
		ZeroMemory(colorEx, sizeof(colorEx));
		lineExNum = 0;
	}
	~LINE_NODE(){}
	DWORD	color;
	DWORD	color2;
	char	line[256];
	char	line2[256];
	DWORD	len;
	DWORD	len2;
	cImage  m_LineImage;
	WORD   fontIdx;
	LINE_NODE * nextLine;

	BYTE	bWhichLine;

	DWORD	colorEx[MAX_TOOLTIP_LINEEX];
	char	lineEx[MAX_TOOLTIP_LINEEX][256];
	UINT	lineExNum;
	
	//BOOL	m_DisableRect;
};
class cMultiLineText
{
public:
	cMultiLineText();
	virtual ~cMultiLineText();
	void Init(WORD fontIdx, DWORD fgColor, cImage * bgImage = NULL, DWORD imgColor = 0xffffffff);
	void Release();
	void Render();
	BOOL IsValid() { return m_fValid; }
	void SetText(const char* text);
	virtual BOOL SetText(VECTOR3* pPos3, char* szText);

	void AddNamePannel(DWORD dwLength);
	void SetFontIdx(WORD fontIdx){ m_wFontIdx = fontIdx; }
	void SetXY(LONG x, LONG y, BOOL bFlag = TRUE){ m_m_leftTopPos.left = x; m_m_leftTopPos.top = y; m_bMovedText = bFlag; }
	void operator=(const char* text){ SetText(text); }
	void SetImageRGB(DWORD color)				{ m_imgColor = color; }
	void SetImageAlpha(DWORD dwAlpha)			{ m_alpha = dwAlpha; }
	void SetOptionAlpha(DWORD dwOptionAlpha)	{ m_dwOptionAlpha = dwOptionAlpha; }
	void AddImageStone(cImage * Image, int Index);
	int  GetImageStoneCount();
	void AddImageEquip(cImage * Image, cImage * Image2 = NULL);
	void SetRenderModel(int type){ m_RenderModel = type; }
	void SetHeaderImage(cImage * Image){ m_HeaderImage = *Image; }
	void SetRenderArea(/*VECTOR2* scaleRate*/);
	void SetRenderAreaEx(/*VECTOR2* scaleRate*/);

	WORD GetLineCount(){ return m_LineCount; }
	void ClearLineCount();
	//void GetLineHight(int line_idx,LONG & AddNum,LONG & MulNum,LONG & Divide);

	void SetShowCurItem(BOOL val){ m_IsShowCur = val; }
	WORD GetHight(){ return (m_line_idx + 3)*LINE_HEIGHT; }
	void SetItemDBIdx(DWORD val){ m_ItemDBIdx = val; }
	DWORD GetItemDBIdx(){ return m_ItemDBIdx; }
	void SetEquipItem(BOOL val){ bIsEquipItem = val; }
	static void Init();



	void AddLine(const char* text, DWORD dwColor = 0xffffffff, cImage * m_LineImage = NULL, WORD m_FontIdx = 0, BYTE bWhichLine = 0);
	void AddLine2(char* text, char* text2, DWORD dwColor = 0xffffffff, DWORD dwColor2 = 0xffffffff, cImage * m_LineImage = NULL, WORD m_FontIdx = 0, BYTE bWhichLine = 0);

	void SetShowShiftIcon(BOOL val){ bIsShiftIcon = val; }

	void SetDisableRect(){ m_bMovedText = FALSE; }

	void AddImageTop(cImage * Image);
	BOOL	IsBuffIcon;
private:
	LINE_NODE* topLine;
	BOOL	m_fValid;
	WORD	m_wFontIdx;
	cImage  m_bgImage;
	static	cImage  m_bgImageBottomLine;

	static	cImage  m_ShiftIcon;

	DWORD	m_fgColor;
	DWORD	m_imgColor;
	RECT	m_m_leftTopPos;
	int		m_line_idx;
	int		m_max_line_width;
	DWORD	m_alpha;
	DWORD	m_dwOptionAlpha;
	WORD	m_LineCount;

	cImage  m_ImageStone[5];
	cImage  m_ImageEquip;
	cImage  m_ImageEquip2;
	//cImage  m_ImageTop;
	BOOL	m_bNamePannel;
	int    m_RenderModel;
	cImage  m_HeaderImage;
	BOOL	m_bMovedText;
	RECT	rect;
	RECT	m_renderRect;
	RECT	m_JackRect;
	VECTOR2	JackVector;

	BOOL bIsEquipItem;
	BOOL	m_IsShowCur;

	bool bIsShiftIcon;

	//static cImage  m_LUpRightImage;
	//static cImage  m_RUpRightImage;
	//static cImage  m_bLineImage;
	//static cImage	m_LiftQuarter;
	//static cImage	m_RightQuarter;

	cImage  m_ImageTop;
	cImage  m_ImageTopLine[50];
	DWORD   m_ItemDBIdx;

	BOOL EndLine;
};
#endif 