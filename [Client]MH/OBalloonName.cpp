#include "stdafx.h"
#include "OBalloonName.h"
#include "ObjectManager.h"
#include "./interface/cFont.h"
#include "./Interface/cWindowManager.h"
#include "./interface/cMultiLineText.h"
#include "./interface/cScriptManager.h"
#include "./interface/cFont.h"
#include "FortWarManager.h"

DWORD COLORALPHA = 0xffffff;// 0x00ffffff
#define COLOR_LENSIEGE 3
static const DWORD changeTime = 320;
#define COLOR_LEN 8
static const DWORD ImagechangeTime = 80;  //Í¼Æ¬³ÆºÅ
DWORD VipLink[10] = { 1,11,21,31,41,51,61,71,81,91 };
DWORD g_NameColor[COLOR_LEN] = {
	0xFFD700,
	0xFF3030,
	0x00CD00,
	0xDA70D6,
	0x1E90FF,
	0xCD9B9B,
	0x1C1C1C,
	0x00FFFF,
};
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
	m_FlashNameFlag =
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
	m_lFlashNameTall =
	m_lFlashNamePosX =
////////////////////////////////


////////////////////////////
	m_lKillCountTall =
	m_lKillCountPosX = 0;

	//////////////////////////



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
	ZeroMemory(m_FlashName,			sizeof(m_FlashName));
	ZeroMemory(m_KillCount,			sizeof(m_KillCount));

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

	//Í¼Æ¬³ÆºÅ
	ImageNameInfo = NULL;
	ImageNameCount = 0;
	m_ImageNameTime = 0;
	Ischange = FALSE;

	VipImgInfo = NULL;
	m_VipImgTime = 0;
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
	RECT rect = { (long)(absX + m_lPosX), (long)(absY + m_lTall), 1, 1 };
	if (m_bSiegeMap)
	{
		int nSize = strlen(m_szObjectName) + 2;
		m_pSiegeName->SetXY(rect.left, rect.top);
		m_pSiegeName->Render();
		CFONT_OBJ->RenderFontWithShadow(m_wFontIdx, m_szObjectName, nSize, &rect, RGBA_MERGE(m_fgColor, 255));
	}
	else if (*m_szObjectName != 0)
	{
		if (GetTickCount() >= m_NameTime)
		{
			m_fgColor2 = g_NameColor[m_nameIndex + 1];
			m_fgColor3 = g_NameColor[m_nameIndex];
			m_NameTime = GetTickCount() + changeTime;
			if (m_ChangeMode == 0)
				++m_nameIndex;
			if (m_nameIndex >= COLOR_LEN)
			{
				m_nameIndex = 0;
			}
		}
		if (HERO->IsPKMode() == TRUE)
		{
			int nSize = strlen(m_szObjectName) + 2;
			CFONT_OBJ->RenderFontWithShadow(m_wFontIdx, m_szObjectName, nSize, &rect, RGBA_MERGE(m_fgColor, 255));
		}
		else
		{
			int nSize = strlen(m_szObjectName) + 2;
			BOOL bNormalFont = TRUE;
			{
				switch ((int)m_FlashNameFlag)
				{
				case 1:
				{
					RECT rectFlash = { (long)(absX + m_lPosX), (long)(absY + m_lTall), 1, 1 };
					int nSize = strlen(m_szObjectName) + 2;
					rectFlash.left -= 2;
					rectFlash.top -= 1;
					CFONT_OBJ->RenderNoticeMsg(7, m_szObjectName,
												nSize, &rectFlash, RGBA_MERGE(RGB_HALF(255, 255, 255), 255), RGBA_MERGE(m_fgColor3, 255));
					bNormalFont = FALSE;
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
					cImage  imageMid; 
					SCRIPTMGR->GetImage(151, &imageMid, PFT_JACKPATH);
					cImage  imageRight;
					SCRIPTMGR->GetImage(152, &imageRight, PFT_JACKPATH);
					cImage  imageLeft;
					SCRIPTMGR->GetImage(150, &imageLeft, PFT_JACKPATH);

					
					VECTOR2	 curpos, vScale;
					vScale.x = 1;
					vScale.y = 1;
					DOUBLE imgLeft = (DOUBLE)imageLeft.GetImageSrcRect()->right - imageLeft.GetImageRect()->left;
					DOUBLE imgMidd = (DOUBLE)imageMid.GetImageSrcRect()->right - imageMid.GetImageRect()->left;
					int nSize = strlen(m_szObjectName) + 2;
					RECT rect = { (long)(absX + m_lPosX), (long)(absY + m_lTall), 1, 1 };
					
					{//--------------------------left
						curpos.x = (long)(rect.left - imgLeft);
						curpos.y = (long)(absY - m_lTall - 12);
						imageLeft.RenderSprite(&vScale, NULL, 0, &curpos, RGBA_MERGE(COLORALPHA, 240));
					}					
					{//--------------------------middle
						curpos.x = curpos.x + imgMidd;
						for (int i = 2; i < nSize; i++)
						{
							curpos.x = curpos.x + imgMidd;
							imageMid.RenderSprite(&vScale, NULL, 0, &curpos, RGBA_MERGE(COLORALPHA, 240));
						}								
					}					
					{//--------------------------right
						curpos.x = curpos.x + imgMidd;
						imageRight.RenderSprite(&vScale, NULL, 0, &curpos, RGBA_MERGE(COLORALPHA, 240));
					}
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
	}	
	if (*m_szObjectNickName != 0)
	{
		RECT nickrect = { (long)(absX + m_lNickPosX), (long)(absY + m_lNickTall - 15.0f), 1, 1 };
		int nSize = strlen(m_szObjectNickName) + 2;
		CFONT_OBJ->RenderFontWithShadow(m_wFontIdx, m_szObjectNickName, nSize, &nickrect, RGBA_MERGE(m_fgColor, 255));
		PosY = PosY + 15;
	}
	if (*m_szObjectNickName && *m_szPetMasterName)	ASSERT(0);
	
	if (*m_szPetMasterName != 0)
	{
		RECT masterrect = { (long)(absX + m_lMasterPosX), (long)(absY + m_lMasterTall - 15.0f), 1, 1 };
		int nSize = strlen(m_szPetMasterName) + 2;
		CFONT_OBJ->RenderFontWithShadow(m_wFontIdx, m_szPetMasterName, nSize, &masterrect, RGBA_MERGE(m_fgColor, 255));
	}
	if (*m_szMarryName != 0)
	{
		PosY = PosY + 15;
		RECT marryrect = { (long)(absX + m_lMarryPosX), (long)(absY + m_lMarryTall - PosY), 1, 1 };
		int nSize = strlen(m_szMarryName) + 2;
		CFONT_OBJ->RenderFontWithShadow(m_wFontIdx, m_szMarryName, nSize, &marryrect, RGBA_MERGE(m_fgColor, 255),RGBA_MERGE(RGB_HALF(255, 0, 255), 180));
	}	
	if (*m_szShiTuName != 0)
	{
		RECT shiturect = { (long)(absX + m_lShiTuPosX), (long)(absY + m_lShiTuTall - PosY), 1, 1 };
		int nSize = strlen(m_szShiTuName) + 2;
		CFONT_OBJ->RenderFontWithShadow(m_wFontIdx, m_szShiTuName, nSize, &shiturect, RGBA_MERGE(m_fgColor, 255));
		PosY = PosY + 15;
	}
	if (*m_FlashName != 0)
	{
		PosY = PosY + 15;
		RECT FlashNameRect = { (long)(absX + m_lFlashNamePosX), (long)(absY + m_lFlashNameTall - PosY), 1, 1 };
		int nSize = strlen(m_FlashName) + 2;
		FlashNameRect.left -= 1;
		FlashNameRect.top -= 1;
		VECTOR3 position = { 0 };
		const float fontHeight = 14.0f;
		position.x *= GET_MAINWIN_W - 100;
		position.y *= GET_MAINWIN_H - fontHeight;
		CFONT_OBJ->RenderNoticeMsg(9, m_FlashName, nSize, &FlashNameRect, RGBA_MERGE(RGB_HALF(255, 255, 255), 255), RGBA_MERGE(m_fgColor2, 255));
	}
	if (*m_szStageLogo != 0)
	{
		PosY = PosY + 5;
		std::string Buffer = m_szStageLogo;
		if (Buffer.length()>0)
		{
			int FindStartPos = 0;
			static BYTE al = 255;
			DWORD color = RGBA_MERGE(COLORALPHA, 255);
			while (FindStartPos >= 0 && FindStartPos< Buffer.length())
			{
				int FindPos = Buffer.find('@', FindStartPos);
				int FindPos2 = Buffer.find('&', FindStartPos);
				if (FindPos >= 0)
				{
					std::basic_string <char> ImgStr = Buffer.substr(FindPos + 1, 3);
					std::basic_string <char> FlagStr = Buffer.substr(0, FindPos);
					int ImageIdx = atoi(ImgStr.c_str());
					if (ImageIdx != 0 && ImageIdx >= 0 && ImageIdx <= 7)
					{
						PosY = PosY - 5;
						VECTOR2 curpos;
						VECTOR2 Scaling;
						Scaling.x = Scaling.y = 1.f;
						SCRIPTMGR->GetImage(ImageIdx + 1, m_image, PFT_JACKPATH);
						PosY = PosY + (long)((m_image->GetImageSrcRect()->bottom) - (m_image->GetImageSrcRect()->top));
						curpos.x = (long)(absX - (float)0.5f - (m_image->GetImageSrcRect()->right - m_image->GetImageSrcRect()->left) / 2);
						curpos.y = (long)(absY + m_lStageLogoTall - PosY);
						m_image->RenderSprite(&Scaling, NULL, 0, &curpos, color);
					}
					FindStartPos = FindPos + 2;
				}
				else if (FindPos2 >= 0)
				{
					std::basic_string <char> ImgStr = Buffer.substr(FindPos2 + 1, 3);
					std::basic_string <char> FlagStr = Buffer.substr(0, FindPos2);
					int ImageIdx = atoi(ImgStr.c_str());
					if (ImageIdx != 0 && ImageIdx >= 0 && ImageIdx <= 7)
					{
						PosY = PosY - 5;
						VECTOR2 curpos;
						VECTOR2 Scaling;
						Scaling.x = Scaling.y = 1.f;
						SCRIPTMGR->GetImage(7 + (ImageIdx + 1), m_image, PFT_JACKPATH);
						PosY = PosY + (long)((m_image->GetImageSrcRect()->bottom) - (m_image->GetImageSrcRect()->top));
						curpos.x = (long)(absX - (float)0.5f - (m_image->GetImageSrcRect()->right - m_image->GetImageSrcRect()->left) / 2);
						curpos.y = (long)(absY + m_lStageLogoTall - PosY);
						m_image->RenderSprite(&Scaling, NULL, 0, &curpos, color);
					}
					FindStartPos = FindPos2 + 2;
				}
			}
		}
	}
	if (*m_szFame != 0)
	{
		if (*m_szStageLogo == 0)
			PosY = PosY + 14;
		else
			PosY = PosY + 7;
		DWORD color1 = RGBA_MERGE(RGB_HALF(250, 250, 250), 240);
		DWORD color2 = RGBA_MERGE(RGB_HALF(10, 10, 10), 255);
		RECT Famerect = { (long)(absX + m_lFamePosX), (long)(absY + m_lFameTall - PosY), 1, 1 };
		int nSize = strlen(m_szFame) + 2;
		Famerect.left -= 1;
		Famerect.top -= 1;
		CFONT_OBJ->RenderNoticeMsg(14, m_szFame, nSize, &Famerect, color1, color2);
	}
	if (*m_szFameRank != 0)
	{
		if (*m_szStageLogo == 0)
			PosY = PosY + 11;
		else
			PosY = PosY + 11; 
		RECT FameRankrect = { (long)(absX + m_lFameRankPosX), (long)(absY + m_lFameRankTall - PosY - 2), 1, 1 };
		int nSize = strlen(m_szFameRank) + 2;
		//CFONT_OBJ->RenderFont(m_wFontIdx, m_szFameRank, nSize, &FameRankrect, RGBA_MERGE(RGB_HALF(70, 70, 70), 180));
		//FameRankrect.left -= 1;
		FameRankrect.top -= 2;
		CFONT_OBJ->RenderFontWithShadow(m_wFontIdx, m_szFameRank, nSize, &FameRankrect, RGBA_MERGE(RGB_HALF(2, 255, 255), 180));
		PosY = PosY + 5;
	}
	else PosY = PosY + 2;
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
	if (false == FORTWARMGR->IsWarStart())
	{
		if (m_ImageName != 0 && !HERO->GetAvatarView())
		{
			PosY = PosY + 20;
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

				curpos.x = (long)((absX - (image.GetImageRect()->right - image.GetImageRect()->left) / 2) + ImageNameInfo->LeftPosition);
				curpos.y = (long)(absY - ImageNameInfo->Hight);
				image.RenderSprite(&ImageNameInfo->Scaling, NULL, 0, &curpos, 0xffffffff);
			}
		}
	}
	else if (*m_KillCount != 0)//KIV
	{
		
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
	}
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
//ÉèÖÃÍ¼Æ¬³ÆºÅ
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
void COBalloonName::SetFlashName(char * FlashName)
{
	if (!FlashName)	return;
	if (strcmp(FlashName, "") == 0 || strcmp(FlashName, "0") == 0)
	{
		ZeroMemory(m_FlashName, sizeof(m_FlashName));
		return;
	}
	SafeStrCpy(m_FlashName, FlashName, MAX_NAME_LENGTH + 1);
	SetFlashNamePosX(-CFONT_OBJ->GetTextExtentEx(9, m_FlashName, strlen(m_FlashName)) / 2);
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