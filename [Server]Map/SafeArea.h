
struct SAFEAREAINFO  //weiye P1-P4 �ֱ��ʾ�����ĸ�������
{
	int MapNum;
	 
	VECTOR2 P1;

	VECTOR2 P2;

	VECTOR2 P3;

	VECTOR2 P4;
};

#define SAFEAREAMGR	USINGTON(SafeAreaManager)

class SafeAreaManager
{

public:

	SafeAreaManager();

    virtual	~SafeAreaManager();

	void LoadSafeAreaInfo();

	SAFEAREAINFO * GetSafeAreaInfo(int MapNum);

private:

	CYHHashTable<SAFEAREAINFO> m_SafeAreaList;

};
EXTERNGLOBALTON(SafeAreaManager)