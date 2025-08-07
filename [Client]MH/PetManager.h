




#pragma once

#include "./Interface/cImage.h"


#define PETMGR	USINGTON(CPetManager)

#define PET_DIST_CHECKTIME 500
#define PET_MAX_FRIENDSHIP		10000000
#define PET_SKILLCHARGE_CHECKTIME	1000
#define PET_MAX_SKILL_CHARGE	10000
#define PET_RESUMMON_VALID_TIME	30000

#define MAX_PET_BUFF_NUM	3

#define CRISTMAS_EVENTPET 8


enum ePetFeedResult{ePFR_Sucess=0, ePFR_Unsummoned, ePFR_StaminaFull};

enum ePetUpgradeDlgGrade{ePUDG_Default, ePUDG_Grade2=1, ePUDG_Grade3=2};

enum ePetRevivalDlgGrade{ePRDG_Default, ePRDG_Grade1, ePRDG_Grade2, ePRDG_Grade3, ePRDG_Grade_All};	//ePRDG_Grade_All => ShopItem ��

//����

enum ePetBuffKind
{ePB_None,
ePB_Demage_Percent = 1,		//���ӹ�����
ePB_Dodge = 2,				//����
ePB_MasterAllStatUp = 3,		//������������

ePB_Item_DoubleChance = 4,	//�����Ʒ��������
ePB_NoForeAtkMonster = 5,		//������������
ePB_ReduceCriticalDmg = 6,	//�����ܵ�����һ���˺�
ePB_MasterAllStatRound = 7,	//���Ե���
ePB_Item_RareProbUp = 8,		//���ף��װ����������
ePB_MussangTimeIncrease = 9,	//��˫ģʽά��ʱ������
ePB_Kind_Max};

enum {ePetRestBtn,ePetUseBtn};

//������
enum ePetKind{ePK_None, ePK_CommonPet=1, ePK_ShopItemPet=2, ePK_EventPet=4,};

class CPet;

class cDialog;
class CPetStateDlg;
class CPetStateMiniDlg;
class CPetInventoryDlg;
class CPetMultiDlg;
class CPetManager
{
	CYHHashTable<PET_TOTALINFO>		m_PetInfoList;
	CYHHashTable<cImage>			m_PetImageList;

	DWORD				m_dwStateCheckTime;				//�� ���� üũ Ÿ��( 10�� )
	DWORD				m_dwStaminaDecrease;			//���׹̳� ���ҷ�
	DWORD				m_dwOldFriendShipForToolTipChange;	//���� ģ�е� ������ ���� ����

	DWORD				m_dwDistCheckTime;					//���μ��� Ÿ��(����-��Ÿ�üũ)
	BOOL				m_bValidDistance;				//�������� �� �Ÿ��ΰ�
	BOOL				m_bReadytoMove;					//�������� �� �����ΰ�(movemgr���� ����)

	DWORD				m_dwSkillRechargeCheckTime;		//��ų������ üũ Ÿ��(1��)
	DWORD				m_dwSkillRechargeAmount;
	BOOL				m_bSkillGuageFull;
	BOOL				m_bReadyToSendSkillMsg;

	DWORD				m_dwResummonDelayTime;			//�� ���ȯ ����//������ 30�� ī��Ʈ

	CPet*				m_pCurSummonPet;
	BOOL				m_bIsPetStateDlgToggle;			//��� ���� ����
	cDialog*			m_pCurPetStateDlg;				//���� �� ����â
	CPetStateDlg*		m_pStateDlg;					//�� ����ūâ

	CPetStateMiniDlg*	m_pStateMiniDlg;				//�� ��������â
	CPetInventoryDlg*	m_pInvenDlg;

	CPetMultiDlg*		m_pMultiDlg;
	BOOL				m_bReadyToSendRestMsg;			//�� �޽� ���� �������� ���� ���� �� TRUE

	WORD				m_wPetKind;						//������. �Ϲ�/�����۸�/�̺�Ʈ ��

public:
	CPetManager(void);
	~CPetManager(void);

	void	PetMgrInit();
	void	PetMgrRelease();
	void	PetMgrProcess();
	void	AddHeroPet(CPet* pPet);
	void	InitPetInfo(PET_TOTALINFO* pPetInfo, int num);
	void	AddPetInfo(PET_TOTALINFO* pPetInfo);
	void	RemovePetInfo(DWORD dwSummonItemDBIdx);
	void	ReleasePetInfoList();
	PET_TOTALINFO*	GetPetInfo(DWORD dwItemDBIdx);
	void	UpdateCurPetInfo(CPet* pPet);
	void	OnPetRemove(CPet* pPet);

	void	SetResummonCheckTime();
	BOOL	CheckResummonAvailable();
	DWORD	GetPetResummonRestTime();

	BOOL	CheckDefaultFriendship(DWORD dwItemDBIdx);
	DWORD	GetPetFriendship(DWORD dwItemDBIdx);
	BOOL	IsCurPetStaminaFull();
	BOOL	IsCurPetSummonItem(DWORD dwItemDBIdx);
	void	RemovePetFromTable(DWORD dwPetID);
	void	SetCurSummonPet(CPet* pPet)	{	m_pCurSummonPet=pPet;	}
	CPet*	GetCurSummonPet() {	return m_pCurSummonPet;	}
	CPet*	GetHeroPet(DWORD dwPetID);
	DWORD	GetHeroPetID();
	BOOL	CheckPetSummoned();
	BOOL	CheckPetAlive(DWORD dwSummonItemDBIdx);
	void	ReleasePetTable();

	void	NetworkMsgParse(BYTE Protocol, void* pMsg);

//�� ����
	//��ų ������ ����
	void	CalcSkillRecharge();
	void	SetSkillRechargeAmount(DWORD amount);
	BOOL	IsSkillGuageFull()	{	return m_bSkillGuageFull;	}
	void	SetSkillGuageFull(BOOL bVal)	{m_bSkillGuageFull = bVal;}

	//���󰡱�
	void	CheckDistWithMaster();
	BOOL	IsValidDist() {	return m_bValidDistance;	}
	void	SetMoveReady(BOOL bVal)	{	m_bReadytoMove = bVal;	}

	//UI
	void	OpenPetStateDlg();		//���� �� ����â ����
	void	OpenPetInvenDlg();
	void	SetCurPetStateDlg(cDialog* pDlg) {m_pCurPetStateDlg=pDlg;}	//���� �� ����â ����

	void	SetPetStateDlg(CPetStateDlg* pDlg) {m_pStateDlg=pDlg;}
	void	SetPetStateMiniDlg(CPetStateMiniDlg* pDlg) {m_pStateMiniDlg=pDlg;}
	void	SetPetInventoryDlg(CPetInventoryDlg* pDlg) {m_pInvenDlg=pDlg;}

	void	SetPetMultiDlg(CPetMultiDlg* pDlg) { m_pMultiDlg = pDlg; }
	//060310 UI ����
	void	SetPetDlgToggle(BOOL bVal)	{	m_bIsPetStateDlgToggle = bVal;	}	//FALSE �� ūâ(�⺻)
	BOOL	GetPetDlgToggle()	{	return m_bIsPetStateDlgToggle;	}
	cDialog* GetCurPetStateDlg()	{	return m_pCurPetStateDlg;	}

	void	TogglePetStateDlg();
	void	SetPetStateDlgInfo(CPet* pPet);
	void	SetPetStateMiniDlgInfo(CPet* pPet);
	void	SetPetStateDlgUseRestInfo(CPet* pPet);
	void	ClosePetAllDlg();

	void	SetPetGuageText(DWORD dwStamina, DWORD dwFriendShip);

	DWORD	GetPetValidInvenMaxTabNum();
	void	SetPetValidInvenTab();

//	BOOL	GetPetDlgToggle() {return m_bIsPetStateDlgToggle;}			//��ۻ��°�������. ūâ/����â
	
	//rest
	void	SetCurPetRest(BOOL bRest);
	void	SendPetRestMsg(BOOL bRest);//�� ���/�޽� ���� ������
	void	SetReadyToSendRestMsg(BOOL bReady) {	m_bReadyToSendRestMsg = bReady;	}
	BOOL	IsReadyToSendRestMsg()	{	return m_bReadyToSendRestMsg;	}

	//seal

	void	SendPetSealMsg();

	//ability icon. pet skill
	void	CheckRestNSkillUse();	//����â ��ų��ư ���. �޽Ļ��¸� ������Ű�� ������Ǯ�̸� ��ų ���. ��ȹ�ڿ��� ������ �˰Ե�.

	//skill
	void	InitPetSkillGuage();
	void	UseCurPetSkill();
	void	SetReadyToSendSkillMsg(BOOL bReady) {	m_bReadyToSendSkillMsg = bReady;	}
	BOOL	IsReadyToSendSkillMsg()	{	return m_bReadyToSendSkillMsg;	}
	//SW060324 �� ���� �߰�
	void	AddMasterStatFromPetBuff(player_calc_stats* pMasterAdditionalStat);
	void	RefleshPetMaintainBuff();

	//pet equip
	WORD	GetPetEquipItemMax(DWORD dwItemDBIdx);

	void	LoadPetImage();
	cImage* GetPetImage(DWORD dwPetIdx);
	void	RemovePetImage();

	void	Release();

	void	SetCurSummonPetKind(CPet* pPet);
	BOOL	CheckCurSummonPetKindIs(int kind);
};

EXTERNGLOBALTON(CPetManager)

//UI ��������
//HERO's Pet ����, PetObject�� �ϳ��� �ִ�. ��� ���� ����� �⺻������ ����.
//HERO�� �� ��ȯ �����ۿ� ������ �ִ� �� �������� �� ������ ������ �ִ�. ������ ���� � �����ش�.


