#include "stdafx.h"
#include "CheckResourceFile.h"
#include <fstream>

CheckResourceFile::CheckResourceFile(void)
{
}

CheckResourceFile::~CheckResourceFile(void)
{
}
bool CheckResourceFile::CheckFile(std::vector<std::string>& WatiDownLoadFile)
{
	std::string buf,Path,Md5;
	std::vector<std::string>Tmp;
	std::ifstream InFile;
	std::map<std::string, std::string>::iterator iter;  

	InFile.open(".\\Image\\Autopatch\\MyUpdata.txt",std::ios::in);
	if (!InFile.is_open())
	{
		return false;
	}
	while (InFile>>buf)
	{
		Path = ".";
		Tmp = TextPars.split(buf,"|");
		m_PatchFile[Tmp[0]]=Tmp[1];
		Path+=Tmp[0];
		if(MakeMD5.GetFileMd5(Md5,Path.c_str()))
		{
			iter=m_PatchFile.find(Tmp[0]);
			if (iter!=m_PatchFile.end())
			{
				if(iter->second!=Md5)
				{
					WatiDownLoadFile.push_back(iter->first);
				}
			}

		}
		else
		{
			WatiDownLoadFile.push_back(Tmp[0]);
		}
		Md5.clear();
		Tmp.clear();
	}
	InFile.close();
	return true;
}