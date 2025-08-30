



// cScriptManager.h: interface for the cScriptManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CSCRIPTMANAGER_H__B9C3CDD2_6869_4A8A_801C_BA26B097B799__INCLUDED_)
#define AFX_CSCRIPTMANAGER_H__B9C3CDD2_6869_4A8A_801C_BA26B097B799__INCLUDED_

#if _MSC_VER > 1000

#pragma once
#endif // _MSC_VER > 1000


#include "cWindowDef.h"

//2007. 5. 18. Parse Type 包访 庆歹颇�?眠啊
#include "ClientGameDefine.h"
#include "ClientGameStruct.h"

//

class cWindow;
class cImage;
class CMHFile;



#define SCRIPTMGR	USINGTON(cScriptManager)
enum ePATH_FILE_TYPE
{
	PFT_HARDPATH = 0,
	PFT_ITEMPATH	,
	PFT_MUGONGPATH	,
	PFT_ABILITYPATH ,
	PFT_BUFFPATH ,
	PFT_MINIMAPPATH ,
	PFT_JACKPOTPATH,
	PFT_IMAGENAME,  //ͼƬ�ƺ�
	PFT_VIPIMGPATH,//vipͼƬ�ƺ�
	PFT_JACKPATH, //BY JACK
	PFT_EMOJIPATH, //BY JACK
};
#ifdef _MUTIPET_

enum ImageDigit//ͼƬ����id ��ɫ//����ȼ�����
{
	eImageDigit = 0,
	eMax_0 = 502,
	eMax_1 = 503,
	eMax_2 = 504,
	eMax_3 = 505,
	eMax_4 = 506,
	eMax_5 = 507,
	eMax_6 = 508,
	eMax_7 = 509,
	eMax_8 = 510,
	eMax_9 = 511,
	eMin_0 = 512,
	eMin_1 = 513,
	eMin_2 = 514,
	eMin_3 = 515,
	eMin_4 = 516,
	eMin_5 = 517,
	eMin_6 = 518,
	eMin_7 = 519,
	eMin_8 = 520,
	eMin_9 = 521,



	eLvSe_0 = 522,
	eLvSe_1 = 523,
	eLvSe_2 = 524,
	eLvSe_3 = 525,
	eLvSe_4 = 526,
	eLvSe_5 = 527,
	eLvSe_6 = 528,
	eLvSe_7 = 529,
	eLvSe_8 = 530,
	eLvSe_9 = 531,
	eQingSe_0 = 532,
	eQingSe_1 = 533,
	eQingSe_2 = 534,
	eQingSe_3 = 535,
	eQingSe_4 = 536,
	eQingSe_5 = 537,
	eQingSe_6 = 538,
	eQingSe_7 = 539,
	eQingSe_8 = 540,
	eQingSe_9 = 541,

};

#endif // _MUTIPET

struct sIMAGHARDPATH
{

	int	idx;
	LONG left;
	LONG top;
	LONG right;
	LONG bottom;
};
struct sEMOJIPATH//BY JACK
{
	DWORD ItemIdx;
	int ImageIdx;
	int ImageCount;
	cImageRect rect[20];
};

class cScriptManager  
{
public:
//	MAKESINGLETON(cScriptManager);
	cScriptManager();

	virtual ~cScriptManager();

	cWindow * GetDlgInfoFromFile(char * filePath, char* mode = "rt");


	cWindow * GetInfoFromFile(cWindow * wnd, CMHFile * fp);
	cWindow * GetCtrlInfo(cWindow * wnd, char * buff, CMHFile * fp);

	void GetImage( int idx, cImage * pImage, cImageRect * rect );
	void GetImage( int hard_idx, cImage * pImage , ePATH_FILE_TYPE type = PFT_HARDPATH );

	void CheckPosForResolution(cPoint* pPos);

	//////////// 2007. 5. 18. Parse 包访 眠啊 //////////////////
	BOOL InitParseTypeData();		//Parse Type 单捞�?檬扁�?�?肺靛
	void ReleaseParseTypeData();	//Parse Type 单捞�?昏力

	////////////////////////////////////////////////////////////
#ifdef _MUTIPET_



	ImageDigit SelecterImageDigit(BYTE Digit, BOOL IsMax);//  3pet
	ImageDigit SelecterImageDigitForLvSe(BYTE Digit);
	ImageDigit SelecterImageDigitForQingSe(BYTE Digit);
#endif // _MUTIPET_

	void InitScriptManager();
	//////////////////////////////////////////
	//	//ͼƬ�ƺ�
	IMAGENAMEINFO* GetImageNameInfo(int idx) { return m_ImageNameInfo.GetData(idx); }
	//vipͼƬ�ƺ�
	VIPIMGINFO* GetVipImgInfo(int idx) { return m_ImageVipInfo.GetData(idx); }
	////////////////////////////////////////////////////////////////////////////
	// 2007. 11. 1. CBH - IMAGEPATH 抛捞�?皋葛�?秦力 窃荐
	void cScriptManager::ReleaseImagePathTable();


	sEMOJIPATH * GetEmoji(DWORD ItemIdx);
protected:
	void GetImage( CMHFile * fp, cImage * pImage );

	char * GetMsg( int idx );

	//////////// 2007. 5. 18. Parse 包访 眠啊 /////////////
	int ChangeHashKey(const char* cpTypeName, int nMaxNum);		//巩磊凯阑 秦浆虐蔼栏肺 官操�?窃荐
	PARSE_TYPE GetParseType(const char* str);		//秦寸 颇教 巩磊凯俊 秦寸窍绰 酒捞�?�?馆券
	CYHHashTable<PARSE_TYPE_INFO> m_ParseTypeTable; //颇教�?巩磊 鸥涝历厘 秦浆抛捞�?
	///////////////////////////////////////////////////////

	CYHHashTable<sIMAGHARDPATH> m_ImageHardPath;
	CYHHashTable<sIMAGHARDPATH>	m_ItemHardPath;
	CYHHashTable<sIMAGHARDPATH> m_MugongHardPath;
	CYHHashTable<sIMAGHARDPATH> m_AbilityHardPath;
	CYHHashTable<sIMAGHARDPATH>	m_BuffHardPath;
	CYHHashTable<sIMAGHARDPATH> m_MiniMapHardPath;
	CYHHashTable<sIMAGHARDPATH> m_JackPotHardPath;

	//ͼƬ�ƺ�
	CYHHashTable<sIMAGHARDPATH> m_ImageNamePath;
	CYHHashTable<IMAGENAMEINFO> m_ImageNameInfo;
	//vipͼƬ�ƺ�
	CYHHashTable<sIMAGHARDPATH> m_ImageVipPath;
	//vipͼƬ�ƺ�
	CYHHashTable<VIPIMGINFO> m_ImageVipInfo;

	CYHHashTable<sIMAGHARDPATH> m_ImageJackPath; //BY JACK
	CYHHashTable<sEMOJIPATH> m_ImageEmojiPath; //BY JACK
};
EXTERNGLOBALTON(cScriptManager);
#endif // !defined(AFX_CSCRIPTMANAGER_H__B9C3CDD2_6869_4A8A_801C_BA26B097B799__INCLUDED_)



