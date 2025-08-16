#if !defined(AFX_VIPMGR_H__0620B52A_B018_4B66_9EF9_6A9D6703763D__INCLUDED_)
#define AFX_VIPMGR_H__0620B52A_B018_4B66_9EF9_6A9D6703763D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VIPMGR	USINGTON(VipManager)
class VipManager
{
	CYHHashTable<VIP_INFO> m_VipInfoList;
public:
	VipManager();
	virtual ~VipManager();
	BOOL  LoadVipInfo();
	void    LoadVipInfoFromDB(DWORD CharIdx);
	WORD GetVIpLevel(DWORD TotalGold);
	void    DoVipGetItem(DWORD CharIdx, WORD VipLevel);
	void    DoVipGetItemRet(CPlayer* pPlayer, WORD VipLevel, int Val);
	void ReLoadVipInfo();
};
EXTERNGLOBALTON(VipManager)
#endif


