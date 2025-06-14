#pragma once
#define ITEMDROPMGR USINGTON(ItemDropManager)
class MONSTEREX_LIST;
struct MONSTER_ITEM
{
	WORD	wItemIdx;
	DWORD	dwDropPercent;
};
#define MAX_DROP_ITEM_PERCENT			10000
struct MONSTER_DROP_ITEM
{
	WORD			wItemDropIndex;
	WORD			wTotalNum;
	DWORD			dwCurTotalRate;
	DWORD			dwCurMonsterDropItemPercent[MAX_DROPITEM_NUM];
	MONSTER_ITEM	MonsterDropItemPercent[MAX_DROPITEM_NUM];
};

struct MONSTER_DROP_TEMP
{
	DWORD	wIndex;
	DWORD	wItemIdx;
	DWORD	wRate;
	DWORD	wPercent;
};

class  ItemDropManager
{
	CYHHashTable<MONSTER_DROP_TEMP>	m_MonsterItemDropTemp;
	void ClearMonsterDropItemTempList();
	void AddItemToTempList(DWORD idx, DWORD itemidx, DWORD hardrate, DWORD percent);

	CYHHashTable<MONSTER_DROP_ITEM>	m_MonsterItemDropTable;
	BOOL LoadMonsterDropItemList();
	void ClearMonsterDropItemList();
	MONSTER_DROP_ITEM* GetMonsterItemDrop(WORD wItemDropIdx);

	DWORD m_dwMaxIndexNum;
	DWORD m_dwStartIndex;

	WORD m_dwMobKind;

	WORD m_dwAfterCalTotalRate[3];
public:
	ItemDropManager();
	virtual  ~ItemDropManager();
	void Init();

	BOOL ReloadMonsterDropItem(MONSTER_DROP_ITEM* pDropItem);
	void DevMonsterDrop(DWORD idx);
	DWORD GetIndex();
	void GetStoredIdx();
	void ClearTemp();

	MONSTER_DROP_TEMP* GetItemTempInfo(WORD Idx);
	int  GetItemTempNum();

	void SetKind(WORD kind){ m_dwMobKind = kind; }
	WORD GetKind(){ return m_dwMobKind; }

	WORD GetCalcRate(BYTE i){ return m_dwAfterCalTotalRate[i]; }
};
EXTERNGLOBALTON(ItemDropManager)