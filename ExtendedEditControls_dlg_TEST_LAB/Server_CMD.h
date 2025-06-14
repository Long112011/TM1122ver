#pragma once
#define SERVERCMD	USINGTON(ServerCMD)
class ServerCMD
{
	DWORD ProcessID;
public:
	ServerCMD();
	virtual ~ServerCMD();
	void OnCommand(char* szCommand);
	int Command(char*text, ...);
	BOOL OnSubCommand(char* cmd);
	void Kill(DWORD pid);
};
EXTERNGLOBALTON(ServerCMD)