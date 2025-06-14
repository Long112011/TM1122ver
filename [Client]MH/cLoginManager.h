#pragma once
#include "Crypt.h"
#define LOGINMGR USINGTON(cLoginManager)
class cLoginManager
{
	//CCrypt m_crypt;
public:
	cLoginManager();
	virtual ~cLoginManager();

	void UserData(char * Username,char * Userpass);
};
EXTERNGLOBALTON(cLoginManager)