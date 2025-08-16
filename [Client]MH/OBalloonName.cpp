#include "stdafx.h"
#include "OBalloonName.h"
#include "ObjectManager.h"
#include "./interface/cFont.h"
#include "./Interface/cWindowManager.h"
#include "./interface/cMultiLineText.h"
#include "./interface/cScriptManager.h"
#include "./interface/cFont.h"
#include "FortWarManager.h"
DWORD  g_NameColor[24] = { 0x1C19FF,0x1BBA66,0xFF00FF,0xFF0000,0xCCCC00,0xC14BE1,0x525244,0xEF8C29,0x7B68EE,
						 0x1BBA66,0x24C2C2,0xFF4500,0x7ACC00,0xCB371,0x1E90FF,0x800080,0x808080,0xD2691E,
						 0xB30000,0xCC9900,0x669900,0x9933FF,0x990033,0xFF66FF };

DWORD COLORALPHA = 0xffffff;// 0x00ffffff
#define COLOR_LENSIEGE 3
static const DWORD changeTime = 500; //300     变换时间
#define COLOR_LEN 10
static const DWORD ImagechangeTime = 80;  //图片称号
DWORD VipLink[10] = { 1,11,21,31,41,51,61,71,81,91 };
//DWORD g_NameColor[COLOR_LEN] = {
//	0xFFD700,
//	0xFF3030,
//	0x00CD00,
//	0xDA70D6,
//	0x1E90FF,
//	0xCD9B9B,
//	0x1C1C1C,
//	0x00FFFF,
//};
GLOBALTON(COBalloonName)
COBalloonName::COBalloonName()
{

	m_wFontIdx = 
		m_lFortTall =
		m_lFortPosX =
	m_lTall =
	m_lPosX =
	m_lMasterTall =
	m_lMasterPosX =
	m_lNickTall =
	m_lNickPosX =
	m_lMarryPosX =
	m_lMarryTall =
	m_lShiTuPosX =
	m_lShiTuTall =
	m_NameTime =

	m_nameIndex =
	m_ChangeMode =

	m_lFamePosX =
	m_lFameTall =
	m_lStageLogoPosX =
	m_lStageLogoTall =
	m_lTopPosX =
	m_lTopTall =
	m_lFameRankPosX =
	m_lFameRankTall =
	m_lTopRankPosX =
	m_lTopRankTall =

////////////////////////////////


////////////////////////////
	m_lKillCountTall =
		VIPImageVal = 0;     //初始化VIP图标
	m_FlgName = 0;       //初始化闪名
	m_lKillCountPosX = 0;

	//////////////////////////
	m_lCustomizingPosX = 0;
	m_lCustomizingTall = 0;


	m_fgColor		= RGB_HALF(255, 255, 255);
	m_fgColor3		= m_fgColor;

	m_pSiegeName	= new cMultiLineText;
	m_image			= new cImage;

	ZeroMemory(m_szObjectFortName, sizeof(m_szObjectFortName));
	ZeroMemory(m_szObjectName,		sizeof(m_szObjectName));
	ZeroMemory(m_szObjectNickName,	sizeof(m_szObjectNickName));
	ZeroMemory(m_szPetMasterName,	sizeof(m_szPetMasterName));
	ZeroMemory(m_szMarryName,		sizeof(m_szMarryName));
	ZeroMemory(m_szShiTuName,		sizeof(m_szShiTuName));
	ZeroMemory(m_szStageLogo,		sizeof(m_szStageLogo));
	ZeroMemory(m_szFame,			sizeof(m_szFame));
	ZeroMemory(m_szFameRank,		sizeof(m_szFameRank));
	ZeroMemory(m_KillCount,			sizeof(m_KillCount));
	ZeroMemory(m_szCustomizingName, sizeof(m_szCustomizingName));
//	FortUsingCustomNick = FALSE;
//	ZeroMemory(FortCharacterName, sizeof(FortCharacterName));//kiv
	


	Ischange		= FALSE;
	m_bActive		= FALSE;
	m_bShow			= FALSE;
	m_bSiegeMap		= FALSE;
	m_bIsShowTop	= FALSE;
	TopListFlg[0]	= FALSE;
	TopListFlg[1]	= FALSE;
	TopListFlg[2]	= FALSE;
	TopListFlg[3]	= FALSE;
	TopListFlg[4]	= FALSE;
	TopListFlg[5]	= FALSE;
	TopListFlg[6]	= FALSE;
	TopListFlg[7]	= FALSE;	

	//图片称号
	ImageNameInfo = NULL;
	ImageNameCount = 0;
	m_ImageNameTime = 0;
	Ischange = FALSE;

	VipImgInfo = NULL;
	m_VipImgTime = 0;//vip
	IsVipImgchange = FALSE;
	VipImgCount = 0;
}
COBalloonName::~COBalloonName()
{
	SAFE_DELETE_ARRAY(m_pSiegeName);
	InitTopListFlg();
}
void COBalloonName::InitName(char * name)
{
	if (!name) return;
	if (*name == 0) return;
	SafeStrCpy(m_szObjectName, name, MAX_MONSTER_NAME_LENGTH + 1);
	SetPosX(-CFONT_OBJ->GetTextExtentEx(m_wFontIdx, m_szObjectName, strlen(m_szObjectName)) / 2);
}
void COBalloonName::SetName(char* name)
{
	if (!name) return;
	if (*name == 0) return;
	SafeStrCpy(m_szObjectName, name, MAX_MONSTER_NAME_LENGTH + 1);
	SetPosX(-CFONT_OBJ->GetTextExtentEx(m_wFontIdx, m_szObjectName, strlen(m_szObjectName)) / 2);
}
void COBalloonName::SetFortName(char* name)
{
	if (!name) return;
	if (*name == 0) return;
	SafeStrCpy(m_szObjectFortName, name, MAX_MONSTER_NAME_LENGTH + 1);
	SetFortPosX(-CFONT_OBJ->GetTextExtentEx(m_wFontIdx, m_szObjectFortName, strlen(m_szObjectFortName)) / 2);
}
void COBalloonName::SetNickName(char* nickname)
{
	if (!nickname) return;
	SafeStrCpy(m_szObjectNickName, nickname, MAX_GUILD_NICKNAME + 1);
	SetNickPosX(-CFONT_OBJ->GetTextExtentEx(m_wFontIdx, m_szObjectNickName, strlen(m_szObjectNickName)) / 2);
}
void COBalloonName::SetPetMasterName(char* petname)
{
	if (!petname)	return;
	SafeStrCpy(m_szPetMasterName, petname, MAX_MASTERNAME_LENGTH + 1);
	SetPetMasterNamePosX(-CFONT_OBJ->GetTextExtentEx(m_wFontIdx, m_szPetMasterName, strlen(m_szPetMasterName)) / 2);
}
BOOL COBalloonName::RenderFort(LONG absX, LONG absY,BYTE kind)
{
	if (!FORTWARMGR->getCustomNickNameChecked()) return FALSE;
	if (WINDOWMGR->IsInterfaceHided())	return FALSE;
	if (!m_bActive)	return FALSE;
	if (!m_bShow)	return FALSE;
	LONG PosY = 0;
	
	if (*m_szObjectName != 0)
	{
		if (kind == eObjectKind_Npc)
		{
			RECT rect = { (long)(absX + m_lPosX), (long)(absY + m_lTall), 1, 1 };
			int nSize = strlen(m_szObjectName) + 2;
			CFONT_OBJ->RenderFontWithShadow(m_wFontIdx, m_szObjectName, nSize, &rect, RGBA_MERGE(m_fgColor, 255));
		}
		else
		{
			RECT rect = { (long)(absX + m_lFortPosX), (long)(absY + m_lFortTall), 1, 1 };
			//int nSize = strlen(m_szObjectFortName) + 2;
			//CFONT_OBJ->RenderFontWithShadow(m_wFontIdx, m_szObjectFortName, nSize, &rect, RGBA_MERGE(m_fgColor, 255));
			int nSize = strlen(FORTWARMGR->GetObjectName()) + 2;
			CFONT_OBJ->RenderFontWithShadow(m_wFontIdx, FORTWARMGR->GetObjectName(), nSize, &rect, RGBA_MERGE(m_fgColor, 255));
		}
	}
	
	if (*m_KillCount != 0)//KIV
	{
			PosY = PosY + 55;
			RECT KillCountRect = { (long)(absX + m_lKillCountPosX), (long)(absY + m_lKillCountTall - PosY), 1, 1 };
			int nSize = strlen(m_KillCount) + 2;
			KillCountRect.left -= 1;
			KillCountRect.top -= 1;
			DWORD dwColor = TTCLR_CASTLEGUILD;
			if (strcmp(m_szObjectName, HERO->GetObjectName()) == 0)
				dwColor = RGB_HALF(255, 0, 0);
			else
				dwColor = RGB_HALF(255, 255, 255);
			CFONT_OBJ->RenderNoticeMsg(3, m_KillCount, nSize, &KillCountRect, RGBA_MERGE(dwColor, 200), RGBA_MERGE(RGB_HALF(10, 10, 10), 180));
		
	}
	return TRUE;
}
BOOL COBalloonName::Render(LONG absX, LONG absY)
{
	if (WINDOWMGR->IsInterfaceHided())	return FALSE;
	if (!m_bActive)	return FALSE;
	if (!m_bShow)	return FALSE;
	LONG PosY = 0;
	RECT rect = { (long)(absX + m_lPosX), (long)(absY + m_lTall - 20.f), 1, 1 };
	RECT rect2 = { (long)(absX + m_lPosX - 3.5f), (long)(absY + m_lTall - 20.f), 1, 1 };
	RECT rect3 = { (long)(absX + m_lPosX - 3.5f), (long)(absY + m_lTall - 20.f), 1, 1 };
	RECT rect4 = { (long)(absX + m_lPosX - 3.5f), (long)(absY + m_lTall - 20.f), 1, 1 };
	RECT rect5 = { (long)(absX + m_lPosX - 3.5f), (long)(absY + m_lTall - 20.f), 1, 1 };

	if (m_bSiegeMap)
	{
		int nSize = strlen(m_szObjectName) + 2;
		m_pSiegeName->SetXY(rect.left, rect.top);
		m_pSiegeName->Render();
		CFONT_OBJ->RenderFont(m_wFontIdx, m_szObjectName, nSize, &rect, RGBA_MERGE(RGB_HALF(70, 70, 70), 180));
		rect.left -= 1;
		rect.top -= 1;
		CFONT_OBJ->RenderFont(m_wFontIdx, m_szObjectName, nSize, &rect, RGBA_MERGE(m_fgColor, 255));
	}
	else if (*m_szObjectName != 0)
	{
		// [VIP-1] 计算名字像素宽 → 给 rect/right 赋值，供右锚点使用
		int nameWidth =
#ifdef HAS_GetFontTextWidth
			CFONT_OBJ->GetFontTextWidth(m_wFontIdx, m_szObjectName);
#else
			(int)strlen(m_szObjectName) * 8; // 无接口时先估算
#endif
		rect.right = rect.left + nameWidth;
		rect5.right = rect5.left + nameWidth;

		// [VIP-2] 先画 VIP 框（做底层），后面再画名字
		if (VIPImageVal != 0 && VipImgInfo)
		{
			// 动画帧更新（你的原逻辑）
			if (VipImgInfo->IsTrends && GetTickCount() >= m_VipImgTime)
			{
				m_VipImgTime = GetTickCount() + ImagechangeTime;
				if (!IsVipImgchange) VipImgCount++;
				else { VipImgCount = VipLink[VIPImageVal - 1]; IsVipImgchange = FALSE; }
				if (VipImgCount >= VipImgInfo->ImageCount + VipLink[VIPImageVal - 1] - 1) IsVipImgchange = TRUE;
			}

			// 右徽章 + 左/中 资源
			cImage image;      SCRIPTMGR->GetImage(VipImgCount, &image, PFT_VIPIMGPATH);
			cImage imageLeft;  SCRIPTMGR->GetImage(223, &imageLeft, PFT_HARDPATH);
			cImage imageMid;   SCRIPTMGR->GetImage(224, &imageMid, PFT_HARDPATH);

			int leftW = imageLeft.GetImageRect()->right - imageLeft.GetImageRect()->left;
			int midW = imageMid.GetImageRect()->right - imageMid.GetImageRect()->left;

			const RECT& base = (m_FlgName != 0) ? rect5 : rect;
			VECTOR2 v1 = { 1.f, 1.f };

			// 右徽章（保持你的 -22, -7 偏移）
			VECTOR2 posRight; posRight.x = (float)(base.right - 22); posRight.y = (float)(base.top - 7);
			image.RenderSprite(&v1, NULL, 0, &posRight, RGBA_MERGE(0xffffff, 255));

			// 左 cap（保持 -35, -2 偏移）
			VECTOR2 posLeft; posLeft.x = (float)(base.left - 35); posLeft.y = (float)(base.top - 2);
			imageLeft.RenderSprite(&v1, NULL, 0, &posLeft, RGBA_MERGE(0xffffff, 240));

			// 中段平铺
			float leftEdge = posLeft.x + leftW;
			float fillWidth = posRight.x - leftEdge; if (fillWidth < 0) fillWidth = 0;
			VECTOR2 posMid; posMid.y = (float)(base.top - 2);
			for (float x = 0; x < fillWidth; x += midW) {
				posMid.x = leftEdge + x;
				imageMid.RenderSprite(&v1, NULL, 0, &posMid, RGBA_MERGE(0xffffff, 240));
			}
		}

		if (HERO->IsPKMode())
		{
			int nSize = strlen(m_szObjectName) + 2;
			if (m_FlgName != 0)
			{//设置闪名效果
				rect5.bottom += 1;
				rect5.right += 1;
				rect5.left += 1;
				CFONT_OBJ->RenderFont(10, m_szObjectName, nSize, &rect5, RGBA_MERGE(RGB_HALF(255, 0, 0), 255));
				rect5.top += 1;
				rect2.bottom -= 1;
				rect2.right -= 1;
				rect2.left -= 1;
				CFONT_OBJ->RenderFont(10, m_szObjectName, nSize, &rect2, RGBA_MERGE(RGB_HALF(255, 0, 0), 255));
				rect2.top += 1;
				rect4.top -= 1;
				CFONT_OBJ->RenderFont(10, m_szObjectName, nSize, &rect4, RGBA_MERGE(RGB_HALF(255, 0, 0), 255));
				rect4.bottom += 2;
				rect4.top += 2;
				CFONT_OBJ->RenderFont(10, m_szObjectName, nSize, &rect4, RGBA_MERGE(RGB_HALF(255, 0, 0), 255));
				CFONT_OBJ->RenderFont(10, m_szObjectName, nSize, &rect3, RGBA_MERGE(RGB_HALF(240, 240, 240), 255));
			}
			else
			{
				CFONT_OBJ->RenderFont(m_wFontIdx, m_szObjectName, nSize, &rect, RGBA_MERGE(RGB_HALF(70, 70, 70), 180));

				rect.left -= 1;
				rect.top -= 1;
				CFONT_OBJ->RenderFont(m_wFontIdx, m_szObjectName, nSize, &rect, RGBA_MERGE(m_fgColor, 255));
			}
		}
		else
		{
			int nSize = strlen(m_szObjectName) + 2;
			BOOL bNormalFont = TRUE;
			{
				switch ((int)m_FlgName)
				{
				case 1:
				{
					int nSize = strlen(m_szObjectName) + 2;

					if (m_FlgName != 0)
					{
						// 动态变色计时逻辑
						if (GetTickCount() >= m_NameTime)
						{
							m_NameTime = GetTickCount() + changeTime;

							if (m_ChangeMode == 0)
								m_nameIndex++;
							else
								m_nameIndex--;

							if (m_nameIndex >= COLOR_LEN && m_ChangeMode == 0)
							{
								m_ChangeMode = 1;
								m_nameIndex = COLOR_LEN - 1;
							}
							else if (m_nameIndex <= 0 && m_ChangeMode == 1)
							{
								m_ChangeMode = 0;
								m_nameIndex = 0;
							}
						}

						// 多层描边渲染
						rect5.bottom += 1;
						rect5.right += 1;
						rect5.left += 1;
						CFONT_OBJ->RenderFont(10, m_szObjectName, nSize, &rect5, RGBA_MERGE(g_NameColor[m_nameIndex], 255));

						rect5.top += 1;
						rect2.bottom -= 1;
						rect2.right -= 1;
						rect2.left -= 1;
						CFONT_OBJ->RenderFont(10, m_szObjectName, nSize, &rect2, RGBA_MERGE(g_NameColor[m_nameIndex], 255));

						rect2.top += 1;
						rect4.top -= 1;
						CFONT_OBJ->RenderFont(10, m_szObjectName, nSize, &rect4, RGBA_MERGE(g_NameColor[m_nameIndex], 255));

						rect4.bottom += 2;
						rect4.top += 2;
						CFONT_OBJ->RenderFont(10, m_szObjectName, nSize, &rect4, RGBA_MERGE(g_NameColor[m_nameIndex], 255));

						CFONT_OBJ->RenderFont(10, m_szObjectName, nSize, &rect3, RGBA_MERGE(RGB_HALF(240, 240, 240), 255));

						bNormalFont = FALSE;
					}
				}
				break;

				case 2:
				{
					int nSize = strlen(m_szObjectName);
					SetSiegeName(0, nSize, TRUE);
					bNormalFont = FALSE;
				}
				break;
				case 3:
				{
					int nSize = strlen(m_szObjectName);
					SetSiegeName(2, nSize, TRUE);
					bNormalFont = FALSE;
				}
				break;
				case 4:
				{
					int nSize = strlen(m_szObjectName);
					SetSiegeName(1, nSize, TRUE);
					bNormalFont = FALSE;
				}
				break;
				case 5: break;
				case 6: break;
				case 7:
				{
					//cImage  imageMid; 
					//SCRIPTMGR->GetImage(151, &imageMid, PFT_JACKPATH);
					//cImage  imageRight;
					//SCRIPTMGR->GetImage(152, &imageRight, PFT_JACKPATH);
					//cImage  imageLeft;
					//SCRIPTMGR->GetImage(150, &imageLeft, PFT_JACKPATH);

					//
					//VECTOR2	 curpos, vScale;
					//vScale.x = 1;
					//vScale.y = 1;
					//DOUBLE imgLeft = (DOUBLE)imageLeft.GetImageSrcRect()->right - imageLeft.GetImageRect()->left;
					//DOUBLE imgMidd = (DOUBLE)imageMid.GetImageSrcRect()->right - imageMid.GetImageRect()->left;
					//int nSize = strlen(m_szObjectName) + 2;
					//RECT rect = { (long)(absX + m_lPosX), (long)(absY + m_lTall), 1, 1 };
					//
					//{//--------------------------left
					//	curpos.x = (long)(rect.left - imgLeft);
					//	curpos.y = (long)(absY - m_lTall - 12);
					//	imageLeft.RenderSprite(&vScale, NULL, 0, &curpos, RGBA_MERGE(COLORALPHA, 240));
					//}					
					//{//--------------------------middle
					//	curpos.x = curpos.x + imgMidd;
					//	for (int i = 2; i < nSize; i++)
					//	{
					//		curpos.x = curpos.x + imgMidd;
					//		imageMid.RenderSprite(&vScale, NULL, 0, &curpos, RGBA_MERGE(COLORALPHA, 240));
					//	}								
					//}					
					//{//--------------------------right
					//	curpos.x = curpos.x + imgMidd;
					//	imageRight.RenderSprite(&vScale, NULL, 0, &curpos, RGBA_MERGE(COLORALPHA, 240));
					//}
				}
				break;
				}
			}
			if (bNormalFont)
			{
				int nSize = strlen(m_szObjectName) + 2;
				CFONT_OBJ->RenderFontWithShadow(m_wFontIdx, m_szObjectName, nSize, &rect, RGBA_MERGE(m_fgColor, 255));
			}
		}
		PosY += 40;  // 假设每个名字之间有 15 像素的间隔 
	}	

	////////帮派称号名字
	RECT nickrect = { (long)(absX + m_lNickPosX), (long)(absY + m_lNickTall - PosY), 1, 1 };//-45.0f
	if (*m_szObjectNickName != 0)
	{//帮会称号
		
		int nSize = strlen(m_szObjectNickName) + 2;
		CFONT_OBJ->RenderFont(m_wFontIdx, m_szObjectNickName, nSize, &nickrect, RGBA_MERGE(RGB_HALF(70, 70, 70), 180));
		nickrect.left -= 1;
		nickrect.top -= 1;
		CFONT_OBJ->RenderFont(m_wFontIdx, m_szObjectNickName, nSize, &nickrect, RGBA_MERGE(m_fgColor, 255));
	PosY = PosY += 20;
	}
	if (*m_szObjectNickName && *m_szPetMasterName)	ASSERT(0);
	///////////////////////自定义名字
	RECT Customizingrect1 = { (long)(absX + m_lCustomizingPosX), (long)(absY + m_lCustomizingTall - PosY), 1, 1 };//-45.0f
	RECT Customizingrect2 = { (long)(absX + m_lCustomizingPosX), (long)(absY + m_lCustomizingTall - PosY), 1, 1 };//-45.0f
	RECT Customizingrect3 = { (long)(absX + m_lCustomizingPosX), (long)(absY + m_lCustomizingTall - PosY), 1, 1 };//-45.0f
	RECT Customizingrect4 = { (long)(absX + m_lCustomizingPosX), (long)(absY + m_lCustomizingTall - PosY), 1, 1 };//-45.0f

	if (*m_szCustomizingName != 0)
	{
		
		int nSize = strlen(m_szCustomizingName) + 2;
		if (m_szCustomizingName != 0 /*&& GetTickCount() >= m_NameTime*/)
		{//设置闪名效果
			if (GetTickCount() >= m_NameTime)
			{
				//m_fgColor = g_NameColor[m_nameIndex];
				m_NameTime = GetTickCount() + changeTime;

				if (m_ChangeMode == 0)
					m_nameIndex++;
				else
					m_nameIndex--;

				if (m_nameIndex >= COLOR_LEN && m_ChangeMode == 0)
				{
					m_ChangeMode = 1;
					m_nameIndex = COLOR_LEN - 1;
				}
				else if (m_nameIndex <= 0 && m_ChangeMode == 1)
				{
					m_ChangeMode = 0;
					m_nameIndex = 0;
				}
			}
			//}

			Customizingrect4.bottom += 1;
			Customizingrect4.right += 1;
			Customizingrect4.left += 1;
			CFONT_OBJ->RenderFont(10, m_szCustomizingName, nSize, &Customizingrect4, RGBA_MERGE(g_NameColor[m_nameIndex], 255));
			Customizingrect4.top += 1;
			Customizingrect1.bottom -= 1;
			Customizingrect1.right -= 1;
			Customizingrect1.left -= 1;
			CFONT_OBJ->RenderFont(10, m_szCustomizingName, nSize, &Customizingrect1, RGBA_MERGE(g_NameColor[m_nameIndex], 255));
			Customizingrect1.top += 1;
			Customizingrect3.top -= 1;
			CFONT_OBJ->RenderFont(10, m_szCustomizingName, nSize, &Customizingrect3, RGBA_MERGE(g_NameColor[m_nameIndex], 255));
			Customizingrect3.bottom += 2;
			Customizingrect3.top += 2;
			CFONT_OBJ->RenderFont(10, m_szCustomizingName, nSize, &Customizingrect3, RGBA_MERGE(g_NameColor[m_nameIndex], 255));
			CFONT_OBJ->RenderFont(10, m_szCustomizingName, nSize, &Customizingrect2, RGBA_MERGE(RGB_HALF(240, 240, 240), 255));
			PosY += 20;  // 假设每个名字之间有 15 像素的间隔
		}

	}

	///////////宠物
	RECT masterrect = { (long)(absX + m_lMasterPosX), (long)(absY + m_lMasterTall -45.0f), 1, 1 };

	if (*m_szPetMasterName != 0)
	{
		int nSize = strlen(m_szPetMasterName) + 2;
		CFONT_OBJ->RenderFont(m_wFontIdx, m_szPetMasterName, nSize, &masterrect, RGBA_MERGE(RGB_HALF(70, 70, 70), 180));
		masterrect.left -= 1;
		masterrect.top -= 1;
		CFONT_OBJ->RenderFont(m_wFontIdx, m_szPetMasterName, nSize, &masterrect, RGBA_MERGE(m_fgColor, 255));
	}
	if (*m_szMarryName != 0)
	{
		PosY = PosY += 18;
		RECT marryrect = { (long)(absX + m_lMarryPosX), (long)(absY + m_lMarryTall - PosY+18), 1, 1 };
		int nSize = strlen(m_szMarryName) + 2;
		CFONT_OBJ->RenderFontWithShadow(m_wFontIdx, m_szMarryName, nSize, &marryrect, RGBA_MERGE(m_fgColor, 255),RGBA_MERGE(RGB_HALF(255, 0, 255), 180));
	}	
	if (*m_szShiTuName != 0)
	{
		RECT shiturect = { (long)(absX + m_lShiTuPosX), (long)(absY + m_lShiTuTall - PosY), 1, 1 };
		int nSize = strlen(m_szShiTuName) + 2;
		CFONT_OBJ->RenderFontWithShadow(m_wFontIdx, m_szShiTuName, nSize, &shiturect, RGBA_MERGE(m_fgColor, 255));
		PosY = PosY += 18;
	}
	if (*m_szStageLogo != 0)
	{
		std::string Buffer = m_szStageLogo;
		if (!Buffer.empty())
		{
			int FindStartPos = 0;
			DWORD color = RGBA_MERGE(COLORALPHA, 255);
			// 不要预加：PosY += 8; 也不要 PosY -= 5;

			while (FindStartPos >= 0 && FindStartPos < (int)Buffer.length())
			{
				int pAt = Buffer.find('@', FindStartPos);
				int pAmp = Buffer.find('&', FindStartPos);
				int p = (pAt >= 0) ? pAt : pAmp;
				if (p < 0) break;

				int idxStart = p + 1;
				std::string ImgStr = Buffer.substr(idxStart, 3);
				int ImageIdx = atoi(ImgStr.c_str());
				if (ImageIdx >= 0 && ImageIdx <= 7)
				{
					// 选择资源
					int resIdx = (p == pAt) ? (ImageIdx + 1) : (7 + (ImageIdx + 1));
					SCRIPTMGR->GetImage(resIdx, m_image, PFT_JACKPATH);

					// 计算当前位置（统一基准：m_lTall）
					int imgW = m_image->GetImageSrcRect()->right - m_image->GetImageSrcRect()->left;
					int imgH = m_image->GetImageSrcRect()->bottom - m_image->GetImageSrcRect()->top;

					VECTOR2 curpos, scaling{ 1.f, 1.f };
					curpos.x = (float)(absX - imgW / 2);
					curpos.y = (float)(absY + m_lTall - PosY-20);   // 基于 m_lTall

					// 渲染
					m_image->RenderSprite(&scaling, NULL, 0, &curpos, color);

					// 画完再累计（图片高度 + 你要的间距）
					PosY += imgH + 2;
				}

				FindStartPos = p + 2;
			}
		}
	}

	const int kLine = 13; // 统一行高

	if (*m_szFame != 0)
	{
		// 不要在这里预加 PosY（删掉原来的 +20/+10）
		DWORD color1 = RGBA_MERGE(RGB_HALF(250, 250, 250), 240);
		DWORD color2 = RGBA_MERGE(RGB_HALF(10, 10, 10), 255);

		RECT Famerect = { (long)(absX + m_lFamePosX),
						  (long)(absY + m_lTall - PosY), 1, 1 }; // 统一用 m_lTall
		int nSize = (int)strlen(m_szFame) + 2;
		Famerect.left -= 4; Famerect.top -= 1;

		CFONT_OBJ->RenderNoticeMsg(14, m_szFame, nSize, &Famerect, color1, color2);

		PosY += kLine; // 画完再进入下一行
	}

	if (*m_szFameRank != 0)
	{
		RECT FameRankrect = { (long)(absX + m_lFameRankPosX),
							  (long)(absY + m_lTall - PosY - 2), 1, 1 };
		int nSize = (int)strlen(m_szFameRank) + 2;
		FameRankrect.top -= 2;
		CFONT_OBJ->RenderFontWithShadow(m_wFontIdx, m_szFameRank, nSize, &FameRankrect,
			RGBA_MERGE(RGB_HALF(2, 255, 255), 180));
		PosY = PosY + 4;
	}
	//else PosY = PosY + 2;
	if (m_bIsShowTop)
	{
		PosY = PosY + 1;
		cImage  TopListImage;
		TopListImage.SetNull();
		cImage  ForTopOneImage;
		ForTopOneImage.SetNull();
		BOOL bFlagFour = false;
		char Temp[16] = { 0 };
		if (TopListFlg[0])
		{
			SCRIPTMGR->GetImage(29, &TopListImage, PFT_JACKPATH);
			SCRIPTMGR->GetImage(107, &ForTopOneImage, PFT_JACKPATH);
		}
		else if (TopListFlg[1])
			SCRIPTMGR->GetImage(30, &TopListImage, PFT_JACKPATH);
		else if (TopListFlg[2])
			SCRIPTMGR->GetImage(31, &TopListImage, PFT_JACKPATH);
		for (int i = 0; i<5; i++)
		{
			if (TopListFlg[3 + i])
			{
				bFlagFour = true;
				SCRIPTMGR->GetImage(32, &TopListImage, PFT_JACKPATH);
				wsprintf(Temp, "%d", 4 + i);
			}
		}
		if (!TopListImage.IsNull())
		{
			VECTOR2 curpos, Scaling;
			Scaling.x = Scaling.y = 1.f;
			PosY = PosY + (long)((TopListImage.GetImageSrcRect()->bottom) - (TopListImage.GetImageSrcRect()->top))+2 ;
			curpos.x = (long)(absX - (TopListImage.GetImageSrcRect()->right - TopListImage.GetImageSrcRect()->left) / 2) - 1;
			curpos.y = (long)(absY + m_lTopTall - PosY);
			TopListImage.RenderSprite(&Scaling, NULL, 0, &curpos, RGBA_MERGE(COLORALPHA, 255));
		}
		if (!ForTopOneImage.IsNull())
		{
			VECTOR2 curpos, Scaling;
			Scaling.x = Scaling.y = 1.f;
			PosY = PosY + (long)((ForTopOneImage.GetImageSrcRect()->bottom) - (ForTopOneImage.GetImageSrcRect()->top)) -1;
			curpos.x = (long)(absX - (ForTopOneImage.GetImageSrcRect()->right - ForTopOneImage.GetImageSrcRect()->left) / 2) - 1;
			curpos.y = (long)(absY + m_lTopTall - PosY);
			ForTopOneImage.RenderSprite(&Scaling, NULL, 0, &curpos, RGBA_MERGE(COLORALPHA, 255));

		}
		if (bFlagFour)
		{
			RECT TopRect = { (long)(absX + m_lTopRankPosX - 1.5f), (long)(absY + m_lTopRankTall - PosY + 3), 1, 1 };
			int nSize = strlen(Temp) + 2;
			//CFONT_OBJ->RenderFont(m_wFontIdx, Temp, nSize, &TopRect, RGBA_MERGE(RGB_HALF(70, 70, 70), 180));
			//TopRect.left -= 1;
			//TopRect.top -= 1;
			CFONT_OBJ->RenderFontWithShadow(m_wFontIdx, Temp, nSize, &TopRect, RGBA_MERGE(RGB_HALF(255, 255, 255), 180));
		}

	}
	PosY = PosY += 70;
	if (false == FORTWARMGR->IsWarStart())
	{
		if (m_ImageName != 0 && !HERO->GetAvatarView())
		{
			
			if (ImageNameInfo)
			{
				if (ImageNameInfo->IsTrends && GetTickCount() >= m_ImageNameTime)
				{
					m_ImageNameTime = GetTickCount() + ImagechangeTime;
					if (!Ischange)
					{
						ImageNameCount++;
					}
					else if (Ischange)
					{
						ImageNameCount = m_ImageName;

						Ischange = FALSE;
					}
					if (ImageNameCount >= ImageNameInfo->ImageCount + m_ImageName - 1)
					{
						Ischange = TRUE;
					}
				}

				cImage  image;
				SCRIPTMGR->GetImage(ImageNameCount, &image, PFT_IMAGENAME);
				VECTOR2 curpos;		
				// 如果想让 ImageName 更贴近上一行，可把这个提拉值调为 4~12
				const int kLift = 0;

				curpos.x = (long)((absX - (image.GetImageRect()->right - image.GetImageRect()->left) / 2) + ImageNameInfo->LeftPosition);
				curpos.y = (float)(absY + m_lTall - PosY - kLift);
				image.RenderSprite(&ImageNameInfo->Scaling, NULL, 0, &curpos, 0xffffffff);
			}
		}
	}
	else if (*m_KillCount != 0)//KIV
	{


		PosY = PosY + 55;
		RECT KillCountRect = { (long)(absX + m_lKillCountPosX), (long)(absY + m_lKillCountTall - PosY), 1, 1 };
		int nSize = strlen(m_KillCount) + 2;
		KillCountRect.left -= 1;
		KillCountRect.top -= 1;
		DWORD dwColor = TTCLR_CASTLEGUILD;
		if (strcmp(m_szObjectName, HERO->GetObjectName()) == 0)
		{
			dwColor = RGB_HALF(255, 0, 0);
		}
		else
		{
			dwColor = RGB_HALF(255, 255, 255);
		}
		CFONT_OBJ->RenderNoticeMsg(3, m_KillCount, nSize, &KillCountRect, RGBA_MERGE(dwColor, 200), RGBA_MERGE(RGB_HALF(10, 10, 10), 180));
		
	}
	//if (VIPImageVal != 0)
	//{
	//	if (VipImgInfo)
	//	{
	//		// 动画帧更新
	//		if (VipImgInfo->IsTrends && GetTickCount() >= m_VipImgTime)
	//		{
	//			m_VipImgTime = GetTickCount() + ImagechangeTime;
	//			if (!IsVipImgchange)
	//			{
	//				VipImgCount++;
	//			}
	//			else
	//			{
	//				VipImgCount = VipLink[VIPImageVal - 1];
	//				IsVipImgchange = FALSE;
	//			}
	//			if (VipImgCount >= VipImgInfo->ImageCount + VipLink[VIPImageVal - 1] - 1)
	//			{
	//				IsVipImgchange = TRUE;
	//			}
	//		}

	//		// 右侧 VIP 徽章图
	//		cImage image;
	//		SCRIPTMGR->GetImage(VipImgCount, &image, PFT_VIPIMGPATH);

	//		// 三段框资源
	//		cImage imageLeft, imageMid;
	//		SCRIPTMGR->GetImage(223, &imageLeft, PFT_HARDPATH); // 左
	//		SCRIPTMGR->GetImage(224, &imageMid, PFT_HARDPATH); // 中

	//		int leftW = imageLeft.GetImageRect()->right - imageLeft.GetImageRect()->left;
	//		int midW = imageMid.GetImageRect()->right - imageMid.GetImageRect()->left;

	//		VECTOR2 vScale = { 1.0f, 1.0f };

	//		// 闪名用 rect5，普通用 rect
	//		const RECT& base = (m_FlgName != 0) ? rect5 : rect;

	//		// === 绘制右片（VIP 徽章）
	//		VECTOR2 curposRight;
	//		curposRight.x = (float)(base.right - 22);
	//		curposRight.y = (float)(base.top - 7);
	//		image.RenderSprite(&vScale, NULL, 0, &curposRight, RGBA_MERGE(0xffffff, 255));

	//		// === 绘制左片
	//		VECTOR2 curposLeft;
	//		curposLeft.x = (float)(base.left - 35);
	//		curposLeft.y = (float)(base.top - 2);
	//		imageLeft.RenderSprite(&vScale, NULL, 0, &curposLeft, RGBA_MERGE(0xffffff, 240));

	//		// === 计算中片填充区域
	//		float leftEdge = curposLeft.x + leftW;
	//		float fillWidth = curposRight.x - leftEdge;
	//		if (fillWidth < 0) fillWidth = 0;

	//		// === 中片绘制（平铺模式）
	//		VECTOR2 v1 = { 1.f, 1.f };
	//		VECTOR2 curposMid;
	//		curposMid.y = (float)(base.top - 2);

	//		for (float x = 0; x < fillWidth; x += midW)
	//		{
	//			curposMid.x = leftEdge + x;
	//			imageMid.RenderSprite(&v1, NULL, 0, &curposMid, RGBA_MERGE(0xffffff, 240));
	//		}
	//	}
	//}



	return TRUE;

}
void COBalloonName::SetFortPosX(LONG pos)
{
	m_lFortPosX = pos;
}
void COBalloonName::SetPosX(LONG pos)
{
	m_lPosX = pos;
}
void COBalloonName::SetNickPosX(LONG pos)
{
	m_lNickPosX = pos;
}
void COBalloonName::SetSiegeName(DWORD NameType, DWORD dwLength, BOOL SiegeMap)
{
	cImage Image;
	DWORD Color = TTCLR_CASTLEGUILD;
	switch (NameType)
	{
	case eSiegeWarNameBox_CastleGuild:
	Color = TTCLR_CASTLEGUILD;
	break;
	case eSiegeWarNameBox_SiegeGuild:
	Color = TTCLR_SIEGEGUILD;
	break;
	case eSiegeWarNameBox_Enermy:
	Color = TTCLR_ENERMY;
	break;
	}
	char buf[MAX_NAME_LENGTH] = { 0, };
	SCRIPTMGR->GetImage(63, &Image, PFT_HARDPATH);
	m_pSiegeName->Init(0, RGB_HALF(255, 255, 255), &Image, Color);
	m_pSiegeName->SetImageAlpha(TOOLTIP_ALPHA);
	m_pSiegeName->AddNamePannel(dwLength);
	m_bSiegeMap = SiegeMap;
}
void COBalloonName::SetFontIndex(BOOL bFontIndex)
{
	m_wFontIdx = bFontIndex;
}
void COBalloonName::SetMarryName(char* marryname)
{
	if (!marryname)	return;
	if (strcmp(marryname, "0") == 0)
	{
		ZeroMemory(m_szMarryName, sizeof(m_szMarryName));
		return;
	}
	SafeStrCpy(m_szMarryName, marryname, MAX_MASTERNAME_LENGTH + 1);
	SetMarryNamePosX(-CFONT_OBJ->GetTextExtentEx(m_wFontIdx, m_szMarryName, strlen(m_szMarryName)) / 2);
}
void COBalloonName::SetShiTuName(char* name)
{
	if (!name)	return;
	if (strcmp(name, "") == 0)
	{
		ZeroMemory(m_szShiTuName, sizeof(m_szShiTuName));
		return;
	}
	SafeStrCpy(m_szShiTuName, name, MAX_MASTERNAME_LENGTH + 1);
	SetShiTuNamePosX(-CFONT_OBJ->GetTextExtentEx(m_wFontIdx, m_szShiTuName, strlen(m_szShiTuName)) / 2);
}
//设置图片称号
void COBalloonName::SetImageName(WORD val)
{
	if (val > 0)
	{
		m_ImageName = val;
		ImageNameInfo = SCRIPTMGR->GetImageNameInfo(val);
		ImageNameCount = m_ImageName;
	}
	else
		m_ImageName = 0;
}
void COBalloonName::SetFame(char* Fame)
{
	if (!Fame)	return;
	if (strcmp(Fame, "0") == 0)
	{
		ZeroMemory(m_szFame, sizeof(m_szFame));
		return;
	}
	SafeStrCpy(m_szFame, Fame, MAX_MASTERNAME_LENGTH + 1);
	SetFamePosX(-CFONT_OBJ->GetTextExtentEx(m_wFontIdx, m_szFame, strlen(m_szFame)) / 2);
}
void COBalloonName::SetStageLogo(char* StageLogo)
{
	if (!StageLogo)	return;
	if (strcmp(StageLogo, "0") == 0 || strcmp(StageLogo, "") == 0)
	{
		ZeroMemory(m_szStageLogo, sizeof(m_szStageLogo));
		return;
	}
	SafeStrCpy(m_szStageLogo, StageLogo, MAX_MASTERNAME_LENGTH + 1);
	SetStageLogoPosX(-CFONT_OBJ->GetTextExtentEx(m_wFontIdx, m_szStageLogo, strlen(m_szStageLogo)) / 2);
}
void COBalloonName::SetTopListFlg(WORD val)
{
	TopListFlg[val] = TRUE;
	m_bIsShowTop	= TRUE;
}
void COBalloonName::InitTopListFlg()
{
	TopListFlg[0]	= FALSE;
	TopListFlg[1]	= FALSE;
	TopListFlg[2]	= FALSE;
	TopListFlg[3]	= FALSE;
	TopListFlg[4]	= FALSE;
	TopListFlg[5]	= FALSE;
	TopListFlg[6]	= FALSE;
	TopListFlg[7]	= FALSE;
	m_bIsShowTop	= FALSE;
}
void COBalloonName::SetFameRank(char* fame)
{
	if (!fame)	return;
	if (strcmp(fame, "0") == 0 || strcmp(fame, "") == 0)
	{
		ZeroMemory(m_szFameRank, sizeof(m_szFameRank));
		return;
	}
	SafeStrCpy(m_szFameRank, fame, MAX_MASTERNAME_LENGTH + 1);
	SetFameRankPosX(-CFONT_OBJ->GetTextExtentEx(m_wFontIdx, m_szFameRank, strlen(m_szFameRank)) / 2);
}
//设置闪名
void COBalloonName::SetOBalloonFlgName(WORD val)
{
	if (val > 0)
		m_FlgName = val;
	else
		m_FlgName = 0;
}
void COBalloonName::SetCustomizingName(char* CustomizingName)
{//自定义称号
	if (!CustomizingName)	return;

	if (strcmp(CustomizingName, "0") == 0)
	{
		ZeroMemory(m_szCustomizingName, sizeof(m_szCustomizingName));
		return;
	}

	SafeStrCpy(m_szCustomizingName, CustomizingName, MAX_MONSTER_NAME_LENGTH + 1);

	SetCustomizingNamePosX(-CFONT_OBJ->GetTextExtentEx(m_wFontIdx, m_szCustomizingName, strlen(m_szCustomizingName)) / 2);
}

void COBalloonName::SetKillCount(char * kill)
{
	if (!kill)	return;
	if (strcmp(kill, "") == 0)
	{
		ZeroMemory(m_KillCount, sizeof(m_KillCount));
		return;
	}
	SafeStrCpy(m_KillCount, kill, MAX_NAME_LENGTH + 1);
	SetKillCountPosX(-CFONT_OBJ->GetTextExtentEx(3, m_KillCount, strlen(m_KillCount)) / 2);
}
void COBalloonName::SetOBalloonVIPImage(int val)
{
	if (val > 0)
	{
		VIPImageVal = val;

		VipImgCount = VipLink[VIPImageVal - 1];
		VipImgInfo = SCRIPTMGR->GetVipImgInfo(VipImgCount);

	}
	else
		VIPImageVal = 0;
}