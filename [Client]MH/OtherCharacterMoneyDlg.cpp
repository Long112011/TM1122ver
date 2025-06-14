#include "stdafx.h"
#include "OtherCharacterMoneyDlg.h"
#include "WindowIDEnum.h"
#include "cWindowManager.h"

// 2014-11-10 钱包功开发！

COtherCharacerMoneyDlg::COtherCharacerMoneyDlg()
{
  m_LastTime=0;

  p_Dlg=NULL;
  
  p_Chr=NULL;

  p_Money=NULL;

  p_PaoDian=NULL;

  p_Gold=NULL;
}
COtherCharacerMoneyDlg::~COtherCharacerMoneyDlg()
{
  p_Dlg=NULL;

  p_Chr=NULL;

  p_Money=NULL;

  p_PaoDian=NULL;

  p_Gold=NULL;
}

void COtherCharacerMoneyDlg::Linking()
{
   
	p_Dlg = WINDOWMGR->GetWindowForID(OTHER_MONEYPACKETDIALOG);

	p_Chr=(cStatic*)GetWindowForID(OTHER_MONEYPACKET_TITLE);

	p_Money=(cStatic*)GetWindowForID(OTHER_MONEYPACKET_MONEY);

	p_PaoDian=(cStatic*)GetWindowForID(OTHER_MONEYPACKET_PAODIAN);

	p_Gold=(cStatic*)GetWindowForID(OTHER_MONEYPACKET_GOLD);

}

DWORD COtherCharacerMoneyDlg::GetLastTime()
{
	return m_LastTime;
}

void COtherCharacerMoneyDlg::SetLastTime(DWORD Val)
{
    m_LastTime=Val;
}

void COtherCharacerMoneyDlg::SetChr(char * CharName)
{
    p_Chr->SetStaticText(CharName);
}
void COtherCharacerMoneyDlg::SetGoldMoney(long Val)
{
	p_Gold->SetStaticText(AddComma(Val));

	p_Gold->SetAlign(TXT_RIGHT);
}
void COtherCharacerMoneyDlg::SetMallMoney(long Val)
{
	p_PaoDian->SetStaticText(AddComma(Val));

	p_PaoDian->SetAlign(TXT_RIGHT);
}
void COtherCharacerMoneyDlg::SetMoney(long Val)
{
	p_Money->SetStaticText(AddComma(Val));

	p_Money->SetAlign(TXT_RIGHT);
}

