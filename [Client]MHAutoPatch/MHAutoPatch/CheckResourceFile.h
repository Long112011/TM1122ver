#pragma once
#include <vector>
#include <map>
#include "cTextParsing.h"
#include "md5.h"
class CheckResourceFile
{
	MD5_CTX		 MakeMD5;
	cTextParsing TextPars;
	std::map<std::string,std::string> m_PatchFile;
public:
	CheckResourceFile(void);
	~CheckResourceFile(void);
	bool CheckFile(std::vector<std::string>& WatiDownLoadFile);
};
