#pragma once
class CEditValidate
{
public:
	CEditValidate() {};
	~CEditValidate() {};
	virtual CString validate(CString& str) { return(_T("")); };
};
class CDayValidate : public CEditValidate
{
	virtual CString validate(CString& str/*, BOOL bFlag=TRUE*/);
};