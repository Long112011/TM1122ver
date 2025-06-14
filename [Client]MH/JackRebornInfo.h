#include "./Interface/cDialog.h"
enum
{
	eStageOne = 0,
	eStageTwo,
	eStageThree,
	eStageMax,
};
class cStatic;
class CJackRebornInfo : public cDialog
{
private:
	//char	timeBuffer[128];
	char	ItemName[MAX_ITEMNAME_LENGTH + 1];
	cImage imgToolTip;
	cImage imgHeader;
protected:
	//cStatic*		m_pItemIconStatic[eStageMax];
	cDialog * pDlg;
	//cStatic * ShowText;

	cStatic * StageOne;
	cStatic * StageTwo;
	cStatic * StageThree;

	cStatic * MaxTurn;

	cStatic * OneAttribute;
	cStatic * OneLevelStart;
	//cStatic * OneLevelEnd;
	cStatic * OneRewardItem;
	cStatic * OneMustNaked;
	cStatic * TwoAttribute;
	cStatic * TwoLevelStart;
	//cStatic * TwoLevelEnd;
	cStatic * TwoRewardItem;
	cStatic * TwoMustNaked;
	cStatic * ThreeAttribute;
	cStatic * ThreeLevelStart;
	//cStatic * ThreeLevelEnd;
	cStatic * ThreeRewardItem;
	cStatic * ThreeMustNaked;

public:
	CJackRebornInfo();
	virtual ~CJackRebornInfo();
	void	ExRender();
	void	Linking();
	void	SetActive(BOOL val);
};
