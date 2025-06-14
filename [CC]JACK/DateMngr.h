#ifndef DATEMGR_H
#define DATEMGR_H
#pragma once

//#include "Crc32Static.h"

#define DATEMGR DateMngr::GetInstance()
class DateMngr
{
public:
	GETINSTANCE(DateMngr);
	DateMngr();
	~DateMngr();

	int GetWeek(struct tm * date);
	int GetWeek(int day, int month, int year);
	int GetCurrentWeek();
};
#endif