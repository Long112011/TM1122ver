#if !defined(AFX_OBALLOONNAME_H__24D73409_A142_4E80_8783_48688340245F__INCLUDED_)
#define AFX_OBALLOONNAME_H__24D73409_A142_4E80_8783_48688340245F__INCLUDED_
#if _MSC_VER > 1000
#pragma once
#endif
#include "MHfile.h"
#define BALLOONMGR 	USINGTON(COBalloonName)
/*
struct sBALLOONNAMECFG
{
	LONG SiegeName;
	LONG PkName;
	LONG CharName;
	LONG MunpaName;
	LONG MarryName;
	LONG ShifuName;
	
	LONG StageLogo;

	LONG FameNoLogo;
	LONG FameHasLogo;
	
	LONG ImageName;

	//char TestName[MAX_NAME_LENGTH+1];
};*/
class cMultiLineText;
class COBalloonName  
{
public:

	//sBALLOONNAMECFG sCharBalloon;
	//sBALLOONNAMECFG & GetMakeCfg(){return sCharBalloon;}
	
	//void InitConfig();


	BOOL m_bIsShowTop;
	COBalloonName();
	virtual ~COBalloonName();
	void InitName(char * name);
	void SetName(char* nickname);
	void SetNickName(char* nickname);
	void SetPetMasterName(char* petname);
	void SetMarryName(char* marryname);
	void SetShiTuName(char* name);
	void SetFlashNameFlag(WORD Flag){m_FlashNameFlag=Flag;}
	void SetFlashName(char * FlashName);
	void SetKillCount(char * kill);
	BOOL IsActive()						{ return m_bActive;	}
	BOOL IsShow()						{ return m_bShow;	}
	void SetActive(BOOL val)			{ m_bActive = val;	}
	void Show( BOOL val )				{ m_bShow	= val; 	}
	BOOL Render(LONG absX, LONG absY);
	void SetTall(LONG	Tall)			{ m_lTall = Tall;	}
	void SetPosX(LONG	pos);
	void SetNickPosX(LONG pos); 
	void SetPetMasterNameTall(LONG Tall)			{ m_lMasterTall = Tall;	}
	void SetPetMasterNamePosX(LONG pos)				{ m_lMasterPosX = pos;	}
	void SetNickNameTall(LONG	nickTall)			{ m_lNickTall = nickTall;	}
	void SetNickNamePosX(LONG	nickpos)			{ m_lNickPosX = nickpos;	}
	void SetMarryNamePosX(LONG pos)					{ m_lMarryPosX = pos; }
	void SetShiTuNamePosX(LONG pos)					{ m_lShiTuPosX = pos;}
	void SetFlashNameTall(LONG pos)					{m_lFlashNameTall = pos; }
	void SetFlashNamePosX(LONG pos)				{m_lFlashNamePosX = pos;}

	void SetKillCountTall(LONG pos)					{ m_lKillCountTall = pos; }
	void SetKillCountPosX(LONG pos)				{ m_lKillCountPosX = pos; }

	void SetFGColor(DWORD col)			{ m_fgColor = col;	}
	LONG GetPosX()						{ return m_lPosX; }
	LONG GetTall()						{ return m_lTall; }
	LONG GetNickPosX()					{ return m_lNickPosX; }
	void SetSiegeName( DWORD NameType, DWORD dwLength,BOOL SiegeMap=TRUE );
	void SetFontIndex(BOOL bFontIndex);

	void SetImageName(WORD val);

	void SetFame(char* Fame);
	void SetFamePosX(LONG pos)					{ m_lFamePosX = pos; }

	void SetStageLogo(char* StageLogo);
	void SetStageLogoPosX(LONG pos)					{ m_lStageLogoPosX = pos; }

	void SetTopListFlg(WORD val);
	void InitTopListFlg();

	void SetFameRank(char* fame);
	void SetFameRankPosX(LONG pos)					{ m_lFameRankPosX = pos; }
protected:
	BOOL m_bActive;			
	BOOL m_bShow;			
	LONG m_lTall;
	LONG m_lPosX;
	LONG m_lNickTall;
	LONG m_lNickPosX;
	LONG m_lMarryPosX;
	LONG m_lMarryTall;
	LONG m_lShiTuPosX;
    LONG m_lShiTuTall;
	LONG m_lMasterTall;
	LONG m_lMasterPosX;
	char m_szObjectName[MAX_MONSTER_NAME_LENGTH +1];
	BOOL m_wFontIdx;
	DWORD m_fgColor;
	DWORD m_fgColor2;
	DWORD m_fgColor3;
	char m_szObjectNickName[MAX_GUILD_NICKNAME+1];
	char m_szPetMasterName[MAX_MASTERNAME_LENGTH+1];
	char m_szMarryName[MAX_MASTERNAME_LENGTH+1];
	char m_szShiTuName[MAX_MASTERNAME_LENGTH+1];
	WORD m_FlashNameFlag;  
	DWORD m_NameTime;
	int m_nameIndex;
	BYTE m_ChangeMode;
	BOOL m_bSiegeMap;
	cMultiLineText* m_pSiegeName;

	WORD m_ImageName;
	int ImageNameCount;
	DWORD m_ImageNameTime;
	BOOL  Ischange;
	IMAGENAMEINFO* ImageNameInfo;

	LONG m_lFamePosX;
	LONG m_lFameTall;
	char m_szFame[MAX_MASTERNAME_LENGTH+1];

	LONG m_lStageLogoPosX;
	LONG m_lStageLogoTall;
	char m_szStageLogo[MAX_MASTERNAME_LENGTH+1];

	cImage* m_image;

	BOOL TopListFlg[8];
	LONG m_lTopPosX;
	LONG m_lTopTall;

	LONG m_lFameRankPosX;
	LONG m_lFameRankTall;
	char m_szFameRank[MAX_MASTERNAME_LENGTH+1];

	LONG m_lTopRankPosX;
	LONG m_lTopRankTall;

	LONG m_lFlashNameTall;
	LONG m_lFlashNamePosX;
	char m_FlashName[MAX_NAME_LENGTH+1];
	//char m_szTopRank[MAX_MASTERNAME_LENGTH+1];

	LONG m_lKillCountTall;
	LONG m_lKillCountPosX;
	char m_KillCount[MAX_NAME_LENGTH + 1];
};
EXTERNGLOBALTON(COBalloonName)
#endif 