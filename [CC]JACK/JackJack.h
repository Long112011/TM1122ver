#ifndef JACKMGR_H
#define JACKMGR_H
#pragma once

#include "Crc32Static.h"
//#include "HantarManager.h"
enum 
{
	eJackERR_None0	= 0,	
	eJackERR_None1	= 1,	
	eJackERR_None2	= 2,	
	eJackERR_None3	= 3,	
};
#define JACKMGR JackJack::GetInstance()
class JackJack
{
public:
	GETINSTANCE(JackJack);
	JackJack();
	~JackJack();
	std::string	PGRam(const std::string& input);
	std::string	DecryptForClient(const std::string& input);
	std::string	DecryptForNetwork(const std::string& input);
	std::string	DecryptForServer(const std::string& input);
	char* GetFullRegistry();
	void GetBuildYearMonthDay(int&build_year, int&build_month, int&build_day);
	char* GetFullVersion();
	bool	GetTrialWeek();
#ifndef _CLIENTJ_
	BOOL	CheckFile();
#endif
	virtual	DWORD __forceinline IsInsideVPC_exceptionFilter(LPEXCEPTION_POINTERS ep);
	virtual	BOOL	IsInsideVMWare();
	virtual	BOOL	IsInsideVPC();
	BOOL	IsVirtualMachine();
	void	Process();

	std::string test;

	QSTATETYPE GetCurTimeYear();
	QSTATETYPE GetEndTimeYearStored();
	QSTATETYPE GetTimeForOneYearExpired();
	QSTATETYPE GetTimeOnBuildDay();
	void Init();
	int GetWeek();
	int week_number(const std::tm& tm);
	std::string marquee_limitJ(std::string& text, size_t limit);

	char * CheckCRC(char * str, char * str2);
	char * CheckCRCOne(char * str);
	char * CheckCRCTwo(char * str);
	char * CalFinalCRC();

	char szExeFileName[MAX_PATH];

	LONG CheckCRCOneLONG(char * str);
	LONG CheckCRCTwoLONG(char * str);
	
	char * CheckResourceFileForCRC(char * str, unsigned long ccrc);
	//BOOL CheckResourceFileForCRC();
	BOOL CheckResourceFile();
	BOOL SaveFileList();

	LONG GetCRC1(char * str);
	LONG GetCRC1Self(char * str);

	void AddPalang2(char* pBuf);
	void AddPalang( char* pBuf );
	char* AddPalang( DWORD dwMoney );
	char* RemovePalang( char* str );

#ifdef _CLIENTJ_
	BOOL TimeCheck(DWORD dwType);
	DWORD TimeValue();
	DWORD TimeValue(DWORD dwType);
	void	PrintTimeInfoInChatDlg();
	BOOL	Cheat(char* cheat);
	void	Open(char* string);
	unsigned long CheckFinalCRC(char * str, unsigned long ccrc);
#endif

protected:
	int hari;
	int hour;
	int min;
	int year;
	int month;
	int date;
	int dateTemp;
	int trialTemp;
	int FinalDayCount;
	stTIME m_EndEventTime;
	stTIME m_EndOneYearExpiryTime;
	stTIME m_BuildTime;
	bool m_Inited;
	WORD passwordLength;
	void ErrCode(int Error = -1);

	char m_strClientPath[MAX_PATH];
	unsigned long  CheckOCTREE();

	unsigned long m_dwCrc1;
	unsigned long m_dwCrc2;
	unsigned long m_dwCrcSelf1;
	unsigned long m_dwCrcSelf2;
};
#endif