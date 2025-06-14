#include "stdafx.h"
#include "WindowIDEnum.h"
#ifdef WINDOW_ID
#undef WINDOW_ID
#endif
#ifdef WINDOW_ID_DEFINE
#undef WINDOW_ID_DEFINE
#endif
#define WINDOW_ID(a)	#a
#define WINDOW_ID_DEFINE(a,def)	#a
char IDNAME[MAX_IDENUM][64] =
{
#include "WindowIDs.h"
};
int IDSEARCH(char * idName)
{
	for (int i = 0; i < MAX_IDENUM; i++)
	{
		if (strcmp(idName, IDNAME[i]) == 0)
		{
			return i;
		}
	}
	return -1;
}
char* WINIDSEARCH(int b)
{
	for (int i = 0; i < MAX_IDENUM; i++)
	{
		if (i == b)
		{
			return IDNAME[i];
		}
	}
	return "no_id";
}
LPCTSTR NAMESEARCH(DWORD windowIndex)
{
	const size_t maxWindowIndex = sizeof(IDNAME) / sizeof(*IDNAME);

	if (maxWindowIndex <= windowIndex)
	{
		return _T("?");
	}

	return IDNAME[windowIndex];
}