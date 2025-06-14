
#include "./interface/cDialog.h"

// 2014-11-10 钱包功开发！

class COtherCharacerMoneyDlg : public cDialog
{

  cDialog * p_Dlg;

  cStatic * p_Chr;

  cStatic * p_Money;

  cStatic * p_PaoDian;

  cStatic * p_Gold;

  DWORD   m_LastTime;

public:

  COtherCharacerMoneyDlg();

  virtual  ~COtherCharacerMoneyDlg();

  void Linking();

  DWORD GetLastTime();

  void  SetLastTime(DWORD Val);

  void  SetChr(char * CharName);

  void SetMoney(long Val);

  void SetGoldMoney(long Val);

  void SetMallMoney(long Val);

};