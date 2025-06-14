#include "stdafx.h"
#include "MoneyPacketDialog.h"
#include "WindowIDEnum.h"
#include "cWindowManager.h"
#include "InventoryExDialog.h"
#include "gamein.h"
CMoneyPacketDialog::CMoneyPacketDialog()
{
	m_type = WT_WALLET_DLG;
	p_Dlg=NULL;
	for(int i;i<3;i++)
	{
		p_WalletStatic[i]=NULL;
	}
}
CMoneyPacketDialog::~CMoneyPacketDialog()
{
	p_Dlg=NULL;

	for(int i;i<3;i++)
	{
		p_WalletStatic[i]=NULL;
	}
}
void CMoneyPacketDialog::Linking()
{
	p_Dlg = WINDOWMGR->GetWindowForID( MONEYPACKETDIALOG );

	p_WalletStatic[eWalletGold]		 = (cStatic*)GetWindowForID(MONEYPACKET_MONEY);
	p_WalletStatic[eWalletIngot]	 = (cStatic*)GetWindowForID(MONEYPACKET_GOLD);
	p_WalletStatic[eWalletBubble]	 = (cStatic*)GetWindowForID(MONEYPACKET_PAODIAN);
}
void CMoneyPacketDialog::SetGoldMoney(long Val)
{
	p_WalletStatic[eWalletIngot]->SetStaticText(AddComma(Val));
	p_WalletStatic[eWalletIngot]->SetAlign(TXT_CENTER);
	p_WalletStatic[eWalletIngot]->SetTextXY(1,6);
}
void CMoneyPacketDialog::SetMallMoney(long Val)
{
	p_WalletStatic[eWalletBubble]->SetStaticText(AddComma(Val));
	p_WalletStatic[eWalletBubble]->SetAlign(TXT_CENTER);
	p_WalletStatic[eWalletBubble]->SetTextXY(1, 6);
}
void CMoneyPacketDialog::SetMoney(long Val)
{//ingame gold
	p_WalletStatic[eWalletGold]->SetStaticText(AddComma(Val));
	p_WalletStatic[eWalletGold]->SetAlign(TXT_CENTER);
	p_WalletStatic[eWalletGold]->SetTextXY(1, 6);
}
void CMoneyPacketDialog::Render()
{
	float x = GAMEIN->GetInventoryDialog()->GetAbsX();
	float y = GAMEIN->GetInventoryDialog()->GetAbsY();

	float invX = GAMEIN->GetInventoryDialog()->GetHeight();

	this->SetAbsXY((LONG)x, (LONG)y + invX);

	//cDialog::Render();
	cDialog::RenderWindow();
	cDialog::RenderComponent();
}