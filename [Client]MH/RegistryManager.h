#pragma once
#define REGMGR USINGTON(RegManager)
class COptionDialog;
class  RegManager
{	
	char  CurrentVersion[MAX_NAME_LENGTH + 1];
protected:
	COptionDialog*	m_pOptionDlg;
public:
	RegManager();
	virtual  ~RegManager();
	void Init();
	HKEY OpenKey(HKEY hRootKey, char* strKey);
	char * registry_read(LPCTSTR subkey, LPCTSTR name, DWORD type);
	void registry_write(LPCTSTR subkey, LPCTSTR name, DWORD type, const char* value);

	void ReadConfig();
	void WriteConfig(char * version);
	
};
EXTERNGLOBALTON(RegManager)