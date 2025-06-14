#pragma once
#include "./interface/cDialog.h"
enum MoneyPacketDlg
{eWalletGold=0, eWalletIngot, eWalletBubble};
class CMoneyPacketDialog : public cDialog
{
  cDialog * p_Dlg;
  cStatic * p_WalletStatic[3];

public:

  CMoneyPacketDialog();
  virtual ~CMoneyPacketDialog();

  void Render();

  void Linking();

  void SetMoney(long Val);
  void SetGoldMoney(long Val);
  void SetMallMoney(long Val);
};