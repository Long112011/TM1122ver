#include "stdafx.h"
#include "cMultiLineText.h"
#include "GameResourceManager.h"
#include "cFont.h"
#include "./Input/UserInput.h"
#include "cScriptManager.h"
#include "chatManager.h"
cImage	cMultiLineText::m_bgImageBottomLine;
cImage	cMultiLineText::m_ShiftIcon;
cMultiLineText::cMultiLineText()
{
	m_fgColor = 0x00ffffff;
	m_imgColor = 0x00ffffff;
	m_line_idx = 0;
	topLine = NULL;
	m_wFontIdx = 0;
	m_max_line_width = 0;
	m_fValid = FALSE;
	SetRect(&m_m_leftTopPos, 0, 0, 0, 0);
	m_alpha = 255;
	m_dwOptionAlpha = 100;
	m_bNamePannel = FALSE;
	m_RenderModel = 0;
	m_bMovedText = FALSE;

	m_LineCount = 0;
	m_IsShowCur = false;

	EndLine = false;

	IsBuffIcon = FALSE;
}
cMultiLineText::~cMultiLineText()
{
	Release();
}
void cMultiLineText::Release()
{
	LINE_NODE * curLineNode = topLine;
	LINE_NODE * tmpNode = NULL;
	while (curLineNode)
	{
		tmpNode = curLineNode;
		curLineNode = curLineNode->nextLine;
		SAFE_DELETE(tmpNode);
	}
	topLine = NULL;
	m_fValid = FALSE;
	m_line_idx = 0;
	m_max_line_width = 0;
	m_bNamePannel = FALSE;

	IsBuffIcon = FALSE;
}
void cMultiLineText::Init(WORD fontIdx, DWORD fgColor, cImage * bgImage, DWORD imgColor)
{
	Release();
	m_wFontIdx = fontIdx;
	m_fgColor = fgColor;
	if (bgImage)
		m_bgImage = *bgImage;
	m_imgColor = imgColor;

	SCRIPTMGR->GetImage(134, &m_bgImageBottomLine, PFT_JACKPATH);
	SCRIPTMGR->GetImage(136, &m_ShiftIcon, PFT_JACKPATH);
}

enum LINEENUM
{
	eTopLine = 0,
	eTitleLine,
	eFirstInfo,
	eSecondInfo,
	eMaxLine
};
void cMultiLineText::Render()
{
	LONG Divide;
	Divide = 57.5;
	DWORD dwForAlpha = m_alpha * m_dwOptionAlpha / 100;
	LINE_NODE * curLineNode = topLine;
	VECTOR2 scaleRate, Pos, PosLineEnd;
	WORD LineCount = 0;

	VECTOR2 LinescaleRate, LinePos;
	LinescaleRate.x = 2;
	LinescaleRate.y = 1;

	if (m_bMovedText)
	{
		rect = m_m_leftTopPos;
		if (!m_ImageEquip.IsNull() || !m_ImageEquip2.IsNull())
		{
			if (g_UserInput.GetKeyboard()->GetKeyPressed(KEY_SHIFT))
				SetRenderArea();
			else
				SetRenderAreaEx();
		}
		else
			SetRenderArea();
	}
	else
	{
		rect = m_m_leftTopPos;
	}

	LinePos.x = (float)rect.left + 20;



	if (!m_ImageTop.IsNull())
	{
		VECTOR2 scaleRate1, Pos1;
		scaleRate1.x = 1;

		scaleRate1.y = 1;

		Pos1.x = (float)rect.left - 8;
		Pos1.y = (float)rect.top - 33;
		m_ImageTop.RenderSprite(&scaleRate1, NULL, 0, &Pos1, 0xffffffff);
	}
	if (!m_bgImage.IsNull())
	{
		Pos.x = (float)rect.left - 5;
		Pos.y = (float)rect.top - 3;

		if (m_RenderModel == 1)
		{
			scaleRate.y = 1 + (float)((m_line_idx + 0.5) * LINE_HEIGHT - Divide) / Divide;
			scaleRate.x = 1;

			Pos.x = (float)Pos.x - 5;
			Pos.y = (float)Pos.y - 2;

			m_bgImage.RenderSprite(&scaleRate, NULL, 0, &Pos, RGBA_MERGE(0xffffff, 255));
			EndLine = true;

			PosLineEnd.x = Pos.x;
			PosLineEnd.y = Pos.y + scaleRate.y;

			scaleRate.y = 1;
			m_HeaderImage.RenderSprite(&scaleRate, NULL, 0, &Pos, RGBA_MERGE(0xffffff, 255));

			if (!m_ImageEquip.IsNull() || !m_ImageEquip2.IsNull())
			{
				scaleRate.x = scaleRate.y = 1;
				Pos.x = (float)rect.left + 1;
				Pos.y = (float)rect.top + 33;

				m_ImageEquip.RenderSprite(&scaleRate, NULL, 0, &Pos, RGBA_MERGE(0xffffff, 255));
				m_ImageEquip2.RenderSprite(&scaleRate, NULL, 0, &Pos, RGBA_MERGE(0xffffff, 255));
			}
		}
		else
		{
			EndLine = false;
			scaleRate.x = (float)((m_max_line_width + 10) >> 2);
			scaleRate.y = 1.0f + (float)(((m_line_idx + 1) * LINE_HEIGHT + 4) >> 2);
			m_bgImage.RenderSprite(&scaleRate, NULL, 0, &Pos, RGBA_MERGE(m_imgColor, m_alpha * m_dwOptionAlpha / 100));
		}
	}
	VECTOR2 StorescaleRate, StorePos;
	StorescaleRate.x = StorescaleRate.y = 0.55f;
	StorePos.x = rect.left + 127.0f;
	StorePos.y = rect.top - 1.75f;
	for (int i = 0; i<5; i++)
	{
		if (!m_ImageStone[i].IsNull())
		{
			StorePos.x = StorePos.x + 26;
			m_ImageStone[i].RenderSprite(&StorescaleRate, NULL, 0, &StorePos, RGBA_MERGE(0xffffff, 255));
		}
	}
	while (curLineNode)
	{
		//------------------------------------------------------------------------------------------------------------
		if (EndLine && LineCount == eTopLine)
		{
			RECT rectTmp;
			rectTmp.bottom = rect.top + 1;
			rectTmp.right = rect.left + 2;
			rectTmp.left = rect.left;
			rectTmp.top = rect.top;
			CFONT_OBJ->RenderFont(curLineNode->fontIdx, curLineNode->line, curLineNode->len, &rectTmp, RGBA_MERGE(curLineNode->color, dwForAlpha));
			CFONT_OBJ->RenderFont(curLineNode->fontIdx, curLineNode->line2, curLineNode->len2, &rectTmp, RGBA_MERGE(curLineNode->color2, dwForAlpha));

			if (m_IsShowCur)
			{
				rectTmp.left += CFONT_OBJ->GetTextWidth(6)*curLineNode->len + 23;
				CFONT_OBJ->RenderFont(6, CHATMGR->GetChatMsg(2683), strlen(CHATMGR->GetChatMsg(2683)), &rectTmp, RGB(0, 255, 0));
			}

			rect.top += LINE_HEIGHT;
			curLineNode = curLineNode->nextLine;
			LineCount++;
			continue;
		}
		else if (EndLine && LineCount == eTitleLine)
		{
			RECT rectTmp;
			rectTmp.bottom = rect.top + 1;
			rectTmp.right = rect.left + 2;
			rectTmp.left = rect.left + 60;
			rectTmp.top = rect.top - 22;
			CFONT_OBJ->RenderFont(6, curLineNode->line, curLineNode->len, &rectTmp, RGBA_MERGE(curLineNode->color, dwForAlpha));
			CFONT_OBJ->RenderFont(6, curLineNode->line2, curLineNode->len2, &rectTmp, RGBA_MERGE(curLineNode->color2, dwForAlpha));
			rect.top += LINE_HEIGHT;
			curLineNode = curLineNode->nextLine;
			LineCount++;
			continue;
		}
		else if (EndLine && LineCount == eFirstInfo)
		{
			RECT rectTmp;
			rectTmp.bottom = rect.top + 1;
			rectTmp.right = rect.left + 2;
			rectTmp.left = rect.left;
			rectTmp.top = rect.top + LINE_HEIGHT - 9;//makin ++ makin keatas
			CFONT_OBJ->RenderFont(m_wFontIdx, curLineNode->line, curLineNode->len, &rectTmp, RGBA_MERGE(curLineNode->color, dwForAlpha));
			CFONT_OBJ->RenderFont(m_wFontIdx, curLineNode->line2, curLineNode->len2, &rectTmp, RGBA_MERGE(curLineNode->color2, dwForAlpha));
			rect.top += LINE_HEIGHT;//kiv
			curLineNode = curLineNode->nextLine;
			LineCount++;
			continue;
		}
		else if (EndLine && LineCount == eSecondInfo)
		{
			RECT rectTmp;
			rectTmp.bottom = rect.top + 1;
			rectTmp.right = rect.left + 2;
			rectTmp.left = rect.left;
			rectTmp.top = rect.top + LINE_HEIGHT - 3;
			CFONT_OBJ->RenderFont(m_wFontIdx, curLineNode->line, curLineNode->len, &rectTmp, RGBA_MERGE(curLineNode->color, dwForAlpha));
			CFONT_OBJ->RenderFont(m_wFontIdx, curLineNode->line2, curLineNode->len2, &rectTmp, RGBA_MERGE(curLineNode->color2, dwForAlpha));
			rect.top += LINE_HEIGHT;
			curLineNode = curLineNode->nextLine;
			LineCount++;
			continue;
		}
		else
		{
			rect.right = rect.left + 2;
			CFONT_OBJ->RenderFont(curLineNode->fontIdx, curLineNode->line, curLineNode->len, &rect, RGBA_MERGE(curLineNode->color, dwForAlpha));
			CFONT_OBJ->RenderFont(curLineNode->fontIdx, curLineNode->line2, curLineNode->len2, &rect, RGBA_MERGE(curLineNode->color2, dwForAlpha));
			rect.top += LINE_HEIGHT;
			curLineNode = curLineNode->nextLine;
			LineCount++;
		}
		//------------------------------------------------------------------------------------------------------------
	}
	if (EndLine)
	{
		VECTOR2 scaleRateEE;
		scaleRateEE.x = 1.0;
		scaleRateEE.y = 1.0;

		PosLineEnd.y = rect.bottom - 15;
		m_bgImageBottomLine.RenderSprite(&scaleRateEE, NULL, 0, &PosLineEnd, RGBA_MERGE(0x00ffffff, 255));
	}

	/*if (bIsShiftIcon)
	{
	VECTOR2 scaleRateEE;
	scaleRateEE.x = 1.0;
	scaleRateEE.y = 1.0;
	m_ShiftIcon.RenderSprite(&scaleRateEE, NULL, 0, &PosLineEnd, RGBA_MERGE(0x00ffffff, 255));
	}*/
}
void cMultiLineText::AddLine(const char* text, DWORD dwColor, cImage * m_LineImage, WORD m_FontIdx, BYTE bWhichLine)
{
	if (text == NULL) return;
	const char* sp = text;
	LINE_NODE* pLineNode = new LINE_NODE;
	pLineNode->nextLine = NULL;
	pLineNode->color = dwColor;

	pLineNode->bWhichLine = bWhichLine;

	if (m_FontIdx == 0)
	{
		pLineNode->fontIdx = m_wFontIdx;
	}
	else
	{
		pLineNode->fontIdx = m_FontIdx;
	}
	strcpy(pLineNode->line, text);
	pLineNode->len = strlen(pLineNode->line);
	m_fValid = TRUE;
	if (m_LineImage)
	{
		pLineNode->m_LineImage = *m_LineImage;
	}
	LINE_NODE* pTail = topLine;
	if (pTail)
	{
		while (pTail->nextLine)
			pTail = pTail->nextLine;
		pTail->nextLine = pLineNode;
		++m_line_idx;
	}
	else
	{
		topLine = pLineNode;
	}
	int	real_len = CFONT_OBJ->GetTextExtentEx(pLineNode->fontIdx, pLineNode->line, pLineNode->len);

	if (m_max_line_width < real_len)
	{
		m_max_line_width = real_len;
	}
	m_LineCount++;
}
void cMultiLineText::AddLine2(char* text, char* text2, DWORD dwColor, DWORD dwColor2, cImage * m_LineImage, WORD m_FontIdx, BYTE bWhichLine)
{
	if (text == NULL) return;
	char* sp = text;
	LINE_NODE* pLineNode = new LINE_NODE;
	pLineNode->nextLine = NULL;
	pLineNode->color = dwColor;
	pLineNode->color2 = dwColor2;

	pLineNode->bWhichLine = bWhichLine;

	if (m_FontIdx == 0)
	{
		pLineNode->fontIdx = m_wFontIdx;
	}
	else
	{
		pLineNode->fontIdx = m_FontIdx;
	}
	strcpy(pLineNode->line, text);
	strcpy(pLineNode->line2, text2);
	pLineNode->len = strlen(pLineNode->line);
	pLineNode->len2 = strlen(pLineNode->line2);
	m_fValid = TRUE;
	if (m_LineImage)
	{
		pLineNode->m_LineImage = *m_LineImage;
	}
	LINE_NODE* pTail = topLine;
	if (pTail)
	{
		while (pTail->nextLine)
			pTail = pTail->nextLine;
		pTail->nextLine = pLineNode;
		++m_line_idx;
	}
	else
	{
		topLine = pLineNode;
	}
	int	real_len = CFONT_OBJ->GetTextExtentWidth(pLineNode->fontIdx, pLineNode->line, pLineNode->len);
	//static int temp_real_len= 78 - real_len ;

	if (real_len > 46)
		m_max_line_width;
	else if (m_max_line_width < real_len)
	{
		m_max_line_width = real_len/*-temp_real_len*/;//set instead of real_len dpt buat tooltip stay kat bucu
	}
	//else
	//	m_max_line_width = real_len;
}
void cMultiLineText::AddNamePannel(DWORD dwLength)
{
	LINE_NODE* pLineNode = new LINE_NODE;
	pLineNode->nextLine = NULL;
	pLineNode->color = 0xffffffff;
	pLineNode->len = strlen(pLineNode->line);
	if (pLineNode->fontIdx == 0)
	{
		pLineNode->fontIdx = m_wFontIdx;
	}
	m_fValid = TRUE;
	LINE_NODE* pTail = topLine;
	if (pTail)
	{
		while (pTail->nextLine)
			pTail = pTail->nextLine;
		pTail->nextLine = pLineNode;
		++m_line_idx;
	}
	else
	{
		topLine = pLineNode;
	}
	char buf[MAX_NAME_LENGTH + 1];
	strncpy(buf, "AAAAAAAAAAAAAAAA", MAX_NAME_LENGTH);
	int real_len = CFONT_OBJ->GetTextExtentEx(pLineNode->fontIdx, buf, dwLength);
	if (m_max_line_width < real_len)
	{
		m_max_line_width = real_len;
	}
	m_bNamePannel = TRUE;
}
BOOL cMultiLineText::SetText(VECTOR3 * pPos3, char * szText)
{
	VECTOR3 OutPos3;
	GetScreenXYFromXYZ(g_pExecutive->GetGeometry(), 0, pPos3, &OutPos3);
	if (OutPos3.x < 0 || OutPos3.x > 1 || OutPos3.y < 0 || OutPos3.y > 1)
		return FALSE;
	SetXY((long)(GAMERESRCMNGR->m_GameDesc.dispInfo.dwWidth*OutPos3.x - 30), (long)(GAMERESRCMNGR->m_GameDesc.dispInfo.dwHeight*OutPos3.y));
	SetText(szText);
	return TRUE;
}
void cMultiLineText::AddImageStone(cImage * Image, int Index)
{
	m_ImageStone[Index] = *Image;
}
int cMultiLineText::GetImageStoneCount()
{
	int StoneCount = 0;
	for (int i = 0; i<5; i++)
	{
		if (!m_ImageStone[i].IsNull())
		{
			StoneCount = StoneCount + 1;
		}
	}
	return StoneCount;
}
void cMultiLineText::AddImageEquip(cImage * Image, cImage * Image2)
{
	m_ImageEquip = *Image;
	if (Image2)
		m_ImageEquip2 = *Image2;
}
void cMultiLineText::ClearLineCount()
{
	m_LineCount = 0;
	for (int i = 0; i<50; i++)
	{
		if (!m_ImageTopLine[i].IsNull())
		{
			m_ImageTopLine[i].SetNull();
		}
	}
}
void cMultiLineText::SetText(const char* text)
{
	if (text == NULL) return;
	if (*text == 0) return;
	if (topLine != NULL) Release();
	const char * sp = text;
	m_line_idx = 0;
	m_max_line_width = 0;
	LINE_NODE * curLineNode = topLine = new LINE_NODE;
	curLineNode->nextLine = NULL;
	curLineNode->color = m_fgColor;
	if (curLineNode->fontIdx == 0)
	{
		curLineNode->fontIdx = m_wFontIdx;
	}
	char * cur_line = topLine->line;
	while (*sp != 0)
	{
		if (IsDBCSLeadByte(*sp))
		{
			*cur_line++ = *sp++;
			*cur_line++ = *sp++;
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
								   case TEXT_NEWLINECHAR:
								   {
															*cur_line = 0;
															curLineNode->len = strlen(curLineNode->line);
															int real_len = CFONT_OBJ->GetTextExtentEx(curLineNode->fontIdx, curLineNode->line, curLineNode->len);
															if (m_max_line_width < real_len)
															{
																m_max_line_width = real_len;
															}
															m_line_idx++;
															curLineNode = curLineNode->nextLine = new LINE_NODE;
															curLineNode->nextLine = NULL;
															cur_line = curLineNode->line;
															curLineNode->color = m_fgColor;
								   }
									   break;
								   case TEXT_TABCHAR:
								   {
								   }
									   break;
								   case TEXT_SPACECHAR:
								   {
														  *cur_line = ' ';
														  ++cur_line;
								   }
									   break;
								   }
			}
				break;
			default:
			{
					   *cur_line = *sp;
					   ++cur_line;
			}
				break;
			}
			++sp;
		}
	}
	curLineNode->len = strlen(curLineNode->line);
	curLineNode->nextLine = NULL;
	int real_len = CFONT_OBJ->GetTextExtentEx(curLineNode->fontIdx, curLineNode->line, curLineNode->len);
	if (m_max_line_width < real_len)
	{
		m_max_line_width = real_len;
	}
	m_fValid = TRUE;
}
void cMultiLineText::SetRenderArea()
{
	if ((DWORD)(rect.left + m_max_line_width) > GET_MAINWIN_W || (DWORD)(rect.top + (m_line_idx + 1) * LINE_HEIGHT) > GET_MAINWIN_H)
	{
		//-----------
		if (rect.left - (TOOLTIP_MOUSESIZE_X + 4 + m_max_line_width) >= 0)
		{
			rect.left -= TOOLTIP_MOUSESIZE_X + 4 + m_max_line_width;
		}
		else
		{
			rect.left -= TOOLTIP_MOUSESIZE_X;
		}
		//-----------
		if (rect.top - (TOOLTIP_MOUSESIZE_Y + (m_line_idx + 1)*LINE_HEIGHT) >= 0)
		{
			rect.top -= TOOLTIP_MOUSESIZE_Y + 4 + (m_line_idx + 1)*LINE_HEIGHT;
		}
		else
		{
			rect.top -= TOOLTIP_MOUSESIZE_Y - 4;
		}
	}
	//m_bMovedText = false ;
}
void cMultiLineText::SetRenderAreaEx()
{
	if ((DWORD)rect.left + m_max_line_width > !(DWORD)GET_MAINWIN_W)
	{
		//-----------
		if (rect.left - (TOOLTIP_MOUSESIZE_X + 14 + m_max_line_width) >= 0)
		{
			rect.left -= TOOLTIP_MOUSESIZE_X + 4 + m_max_line_width;
		}
		/*else
		{
		rect.right += TOOLTIP_MOUSESIZE_X;
		}*/
		//-----------
		if (rect.bottom - (TOOLTIP_MOUSESIZE_Y + (m_line_idx + 1)*LINE_HEIGHT) >= 0)
		{
			rect.bottom += TOOLTIP_MOUSESIZE_Y + (m_line_idx + 1)*LINE_HEIGHT;
		}
		else
		{
			rect.top -= TOOLTIP_MOUSESIZE_Y - 4;
		}
	}

	if ((DWORD)(rect.top + (m_line_idx + 1) * LINE_HEIGHT - 12) > (DWORD)GET_MAINWIN_H)
	{
		//-----------
		if (rect.top - (TOOLTIP_MOUSESIZE_Y + (m_line_idx + 1)*LINE_HEIGHT - 28) >= 0)
		{
			rect.top -= TOOLTIP_MOUSESIZE_Y - 4 + (m_line_idx + 1)*LINE_HEIGHT - 28;
		}
		else
		{
			rect.top -= TOOLTIP_MOUSESIZE_Y - 4;
		}
	}
	//m_bMovedText = false ;
}
void cMultiLineText::AddImageTop(cImage * Image)
{
	m_ImageTop = *Image;
}