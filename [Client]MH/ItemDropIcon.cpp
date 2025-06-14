#include "stdafx.h"
#include "ItemDropIcon.h"
#include "cScriptManager.h"
#include "ItemManager.h"
#include "cFont.h"
#include "cImage.h"
#include "cButton.h"
#include "chatmanager.h"
#include "mhfile.h"
#define FONTOFFER 16
#define GARISX 65
#define GARISY 44
#define NEWPRICEX 100
#define NEWPRICEY 42

#define YPOS 5

CDropItem::CDropItem()
{	
}
CDropItem::~CDropItem()
{
}

void CDropItem::Render()
{
	if(!m_bActive) return;
	cIcon::Render();

	if (*item)
	{
		RECT rect = { (LONG)m_absPos.x + 50, (LONG)m_absPos.y + YPOS, 25, 5 };
		CFONT_OBJ->RenderFont(0, item, strlen(item), &rect, RGBA_MERGE(m_dwImageRGB, m_alpha * m_dwOptionAlpha / 100));
	}
	if (*percent)
	{
		RECT rect = { (LONG)m_absPos.x + 200, (LONG)m_absPos.y + YPOS, 25, 5 };
		CFONT_OBJ->RenderFont(0, percent, strlen(percent), &rect, RGBA_MERGE(RGB_HALF(250, 250, 200), m_alpha * m_dwOptionAlpha / 100));
	}
}