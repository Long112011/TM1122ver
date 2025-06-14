#include "Interface\cDialog.h"
#include "itemManager.h"
#include "VirtualItem.h"
#include "Item.h"
#include "cImageSelf.h"
class cIconDialog;
class CItem;
class cStatic;
class cIcon;
class cButton;
class cCheckBox;
class cTextArea;
class cGuagen;

struct stParticle2
{

	VECTOR2 vPos;
	VECTOR2 vDir;
	stParticle2()
	{
		vPos.x = vPos.y = 0;
		vDir.x = vPos.y = 0;
	}
};

struct stNumImage2	//SW050722 for jackpot number dialog
{
	cImage* pImage;
	DWORD dwW, dwH;
	stNumImage2() :pImage(NULL), dwW(0), dwH(0){}
};

struct stCipherNum2	//SW050722 for jackpot number dialog
{
	stCipherNum2() :dwNumber(99), dwRealCipherNum(0), bIsAni(FALSE){}
	DWORD dwNumber;// 자리수 숫자 (출력용)

	DWORD dwRealCipherNum;// 자리수 숫자 (실제값)
	bool bIsAni;// 숫자 애니 진행 중인지..(애니용)
};
#define IMAGE_COUNT2 11
class CItemStepReinforceDialog : public cDialog
{

#define	DOT_NUM					2
	enum{ DEFAULT_IMAGE2 = 99, NUM_0 = 0, NUM_1, NUM_2, NUM_3, NUM_4, NUM_5, NUM_6, NUM_7, NUM_8, NUM_9, IMG_DOT, IMG_COUNT2, CIPHER_NUM2 = 9 };
	stNumImage2	m_stNumImage[IMAGE_COUNT2];
	VECTOR2		m_vPos[CIPHER_NUM2];
	VECTOR2		m_vDotPos[DOT_NUM];

	DWORD		m_dwTotalMoney;

	DWORD		m_dwOldTotalMoney;
	DWORD		m_dwTempMoney;

	stCipherNum2 m_stCipherNum[CIPHER_NUM2];
	DWORD		m_dwAniStartTime;
	DWORD		m_dwNumChangeTime;
	DWORD		m_dwIntervalAniTime;
	DWORD		m_dwMaxCipher;
	DWORD		m_dwCipherCount;
	bool		m_bIsAnimationing;
	bool		m_bDoSequenceAni;















	cGuagen		* pGuagene;
	cTextArea	* pTextArea;

	cCheckBox	* pCheckBox;

	cDialog * pDlg; 

	cStatic * pStatic[19];

	CItem   * pXqItem;

	cButton	* pButton[14];

	//cIconDialog*	m_pIconDlgEquip;

	cIconDialog*	m_pIconDlgEquip0;
	cIconDialog*	m_pIconDlgEquip1;
	cIconDialog*	m_pIconDlgStone2;
	//cIconDialog*	m_pIconDlgStone3;
	//cIconDialog*	m_pIconDlgStone4;

	cIcon  *        m_IconStone0;
	cIcon  *        m_IconStone1;
	cIcon  *        m_IconStone2;
	cIcon  *        m_IconStone3;
	cIcon  *        m_IconStone4;

	CVirtualItem	m_VirtualItemEquip;
	CVirtualItem    m_VirtualItemStone0;
	CVirtualItem    m_VirtualItemStone1;
	CVirtualItem    m_VirtualItemStone2;
	CVirtualItem    m_VirtualItemStone3;
	CVirtualItem    m_VirtualItemStone4;

	LONG	m_MaxItemCount[4];

	GUAGEVAL gval;

	WORD	m_bItemGrade;
	WORD	m_bItemNextGrade;

	WORD	m_RenderwhichButton;
	BOOL	m_StartRenderSecondProcess;
public:

       CItemStepReinforceDialog();

	   virtual ~CItemStepReinforceDialog();
	   virtual void Render();
	   virtual void SetActive(BOOL flag);
	   void StartRenderSecondProcess(WORD whichButton);
	   void RenderSecondProcess();
	   void OnActionEvent( LONG lId, void * p, DWORD we );
	   void Linking();

	   void SetXqItem(CItem * pItem);

	   CItem * GetXqItem();

	   void  DeleteXqStoneAll();

	   virtual BOOL FakeMoveIcon(LONG x, LONG y, cIcon * icon);

	   void AddVirtualItem( CItem* pItem );

	   void AddVirtualIcon( CItem* pItem );

	   int  GetStoneXqCount(ITEM_STONE_OPTION_INFO * pStoneInfo);

	   int  GetStoneVirtualCount();

	   void SendItemStoneXqMsg();

	   void ResetItemInfo();

	   void Release();

	   void AdaStone(int Stone,BOOL ada=FALSE);


	   void Process();
	   void InitNumImage();				// 숫자 이미지 등록
	   void ReleaseNumImage();				// 숫자 이미지 해제
	   void SetNumImagePos();				// 숫자 이미지 출력 좌표 지정	
	   void ConvertCipherNum();			// 단위별 숫자
	   bool IsNumChanged();				// 숫자 변화 체크	// Check Num charnges
	   //	void DoNumChangeAni();				// 숫자 변환 애니	// Ticktime control
	   void DoAni();
	   void DoSequenceAni();
	   void JPRender();
	   void JPProcess();

	   void InitForAni();
	   void InitForSequenceAni();
	   void JPInit();
};


