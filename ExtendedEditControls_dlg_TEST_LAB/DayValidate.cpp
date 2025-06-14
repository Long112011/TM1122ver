#include "stdafx.h"
#include "DayValidate.h"
BOOL bjFlag;
CString toastMsg;
CString CDayValidate::validate(CString& str)
{
	if (bjFlag)
		return(toastMsg);
	else
		return(_T(""));
}