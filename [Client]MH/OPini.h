#pragma once

class COPini  
{
public:
	static DWORD ReadString (char *section, char * key,  char stringtoread[],  char * filename);
	static BOOL WriteString(LPCTSTR section, LPCTSTR key,char* stringtoadd, char *filename);
	COPini();
	virtual ~COPini(); 

}; 
