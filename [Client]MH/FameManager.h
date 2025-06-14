#pragma once

#include <string>
#define FAMEMGR USINGTON(FameManager)

struct FAMEStruct
{
	int		Idx;	// test
	FAMETYPE		FameVal;
	char	FameName[32];

	LONG	PowerSwitch;
	LONG	Power[4];
};


class  FameManager
{
	CYHHashTable<FAMEStruct> m_FameLogoList;

	CYHHashTable<STAGELOGO> m_StageLogoList;
public:
	FameManager();
	virtual  ~FameManager();

	void LoadFameStage();

	FAMEStruct * GetFameStage(int idx);
	char * GetFameNameString(FAMETYPE pFame);


	/*LONG GetSTR(FAMETYPE pFame);
	LONG GetAGI(FAMETYPE pFame);
	LONG GetCON(FAMETYPE pFame);
	LONG GetINT(FAMETYPE pFame);*/
	LONG GetPowerSwitch(FAMETYPE pFame)
	{
		FAMETYPE fMax = GetFameStage(0)->FameVal;
		for (WORD i = 0; i < fMax + 1; ++i)
		{
			if (pFame < GetFameStage(i)->FameVal + 1) return GetFameStage(i)->PowerSwitch;
			else if (pFame >= GetFameStage(fMax)->FameVal) return GetFameStage(fMax)->PowerSwitch;
		}
	}

	LONG GetSTR(FAMETYPE pFame)
	{
		FAMETYPE fMax = GetFameStage(0)->FameVal;
		for (WORD i = 0; i < fMax + 1; ++i)
		{
			if (pFame < GetFameStage(i)->FameVal + 1) return GetFameStage(i)->Power[0];
			else if (pFame >= GetFameStage(fMax)->FameVal) return GetFameStage(fMax)->Power[0];
		}
	}

	LONG GetAGI(FAMETYPE pFame)
	{
		FAMETYPE fMax = GetFameStage(0)->FameVal;
		for (WORD i = 0; i < fMax + 1; ++i)
		{
			if (pFame < GetFameStage(i)->FameVal + 1) return GetFameStage(i)->Power[1];	
			else if (pFame >= GetFameStage(fMax)->FameVal) return GetFameStage(fMax)->Power[1];
		}
	}

	LONG GetCON(FAMETYPE pFame)
	{
		FAMETYPE fMax = GetFameStage(0)->FameVal;
		for (WORD i = 0; i < fMax + 1; ++i)
		{
			if (pFame < GetFameStage(i)->FameVal + 1) return GetFameStage(i)->Power[2];
			else if (pFame >= GetFameStage(fMax)->FameVal) return GetFameStage(fMax)->Power[2];				
		}
	}

	LONG GetINT(FAMETYPE pFame)
	{
		FAMETYPE fMax = GetFameStage(0)->FameVal;
		for (WORD i = 0; i < fMax + 1; ++i)
		{
			if (pFame < GetFameStage(i)->FameVal + 1) return GetFameStage(i)->Power[3];
			else if (pFame >= GetFameStage(fMax)->FameVal) return GetFameStage(fMax)->Power[3];
		}
	}

	std::string GetFameNameStrTrainee();
	bool LoadStageLogo();
	STAGELOGO * GetStageLogoVal(int idx);
	DWORD GetFameLogoChange(FAMETYPE pFame);
};
EXTERNGLOBALTON(FameManager)