
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define GAMBLEMGR GambleManager::GetInstance()

#define OPEN_HOUR	14	   //
#define OPEN_MIN	10     // 


class GambleManager
{
private:

  BOOL  IsCreate;

  DWORD CheckTimer;

  int   Number1;

  int   Number2;

  int   Number3;

public:

  GambleManager();

  virtual ~GambleManager();

  GETINSTANCE(GambleManager);

  void  Process();

  void  GambleOnceExec(); // 

  void  CreateGambleInfo(SYSTEMTIME st);

  int   GetRandNumber(int Index);

  void  ClearUserData();

};