#pragma once
#define ITEMDROP_OBJ CItemDrop::GetInstance()
class MONSTEREX_LIST;
struct MONSTER_ITEM
{
	WORD wItemIdx;
	DWORD dwDropPercent;
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

class CItemDrop  
{
	CYHHashTable<MONSTER_DROP_ITEM>	m_MonsterItemDropTable;
	BOOL LoadMonsterDropItemList();
	void ClearMonsterDropItemList();
	MONSTER_DROP_ITEM* GetMonsterItemDrop(WORD wItemDropIdx);

public:
	GETINSTANCE(CItemDrop);
 	
	CItemDrop();
	virtual ~CItemDrop();

	int DropItemKind(MONSTEREX_LIST * pMonsterExList, CPlayer* pPlayer);

	void Init();

	MONEYTYPE MoneyItemNoItemPercentCalculator(MONSTEREX_LIST * pMonInfo, CPlayer* pPlayer, WORD MonsterKind);
	MONEYTYPE MonsterDropMoney(MONSTEREX_LIST * pMonsterExList);
	
	void MonsterDropItem(WORD MonsterKind, WORD wDropItemListIdx, CPlayer* pPlayer);
	BOOL ReloadMonsterDropItem(MONSTER_DROP_ITEM* pDropItem);


	void DevMonsterDrop(DWORD idx);

	//void ReloadDropItem(MONSTEREX_LIST * pMonsterExList);
	//void ReloadDropItemPercent(WORD Kind);
	//void CalculItemPercent(WORD Kind, int HaveItem, int idx);
	//BOOL ZeroCheckPercent(WORD Kind);
	//void TestMonsterDropItem(WORD wDropItemListIdx);
};