#pragma once
#define AUTOBACKUPMGR	USINGTON(AUTOBACKUP)
class AUTOBACKUP
{
private:
	DWORD  m_CurTime;
public:
	AUTOBACKUP();
	~AUTOBACKUP();
	void Process();
	void BackupNow();
};
EXTERNGLOBALTON(AUTOBACKUP);