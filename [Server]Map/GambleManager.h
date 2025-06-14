
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define GAMBLEMGR GambleManager::GetInstance()

#define OPEN_HOUR	14	   // 
#define OPEN_MIN	10     // 



class GambleManager
{
private:

  int   Number1;

  int   Number2;

  int   Number3;

public:

  GambleManager();

  virtual ~GambleManager();

  GETINSTANCE(GambleManager);

  void   SetUserGambleInfo(CPlayer * pPlayer,int Number1,int Number2,int Number3);

  BOOL  CheckGambleNumber(int Number);

  void  SendGamblePublicInfoMsg(CPlayer * pPlayer,int Number1,int Number2,int Number3);

  void  SendGambleUserInfoSucceedMsg(CPlayer * pPlayer,int Number1,int Number2,int Number3);

  void  SendGambleUserInfoMapMsg(CPlayer * pPlayer,int Number1,int Number2,int Number3);

  void  SendGambleUserInfoOpenMsg(CPlayer * pPlayer,int Level,DWORD Gold);

  void  SendGambleUserInfoErrorMsg(CPlayer * pPlayer, int ErrorCode);

};