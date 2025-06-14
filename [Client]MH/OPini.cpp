#include "stdafx.h"
#include ".\opini.h"
COPini::COPini()
{
} 

COPini::~COPini()
{

}

BOOL COPini::WriteString(LPCTSTR section, LPCTSTR key, char *stringtoadd, char *filename)
{
    CHAR FilePath[255]; 
    GetModuleFileName(NULL,FilePath,255); 
    //Scan a string for the last occurrence of a character.
    (strrchr(FilePath,'\\'))[1] = 0; 
    strcat(FilePath,filename);
    return ::WritePrivateProfileString(section,key,stringtoadd,FilePath);
} 


DWORD COPini::ReadString(char *section, char * key,  char stringtoread[],  char * filename)
{
    CHAR FilePath[255]; 
    GetModuleFileName(NULL,FilePath,255); 
    (strrchr(FilePath,'\\'))[1] = 0; 
    strcat(FilePath,filename);
    return ::GetPrivateProfileString(section, key,NULL,stringtoread,255,FilePath);
} 

