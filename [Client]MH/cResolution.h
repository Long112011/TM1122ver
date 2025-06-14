struct cRESOLUTION
{
	int Index;
	int x;
	int y;
};
#define SCRRESOLUMGR  USINGTON(CScreenResolution)
class CScreenResolution
{
	CYHHashTable<cRESOLUTION>	m_ResolutionInfoTable;
	DWORD m_dwStartIndex;
public:
	CScreenResolution();
	~CScreenResolution();
	
	void Resolution();
	DWORD GetIndex();
	void CreateResolution();
	BOOL Check(int x, int y);
	void AddResolution(int idx,int x,int y);

	BOOL Tumpang();
	BOOL Tumpang2();
private:
	void UpdateResolutionCombo();
	/* BOOL LoadScreenResolution();
     SCREENRESOLUTION * GetScreenResolution(int Index);
	 void SetScreenResolutionIndex(int Index){m_ScreenResolutionIndex=Index;}
	 int  GetScreenResolutionIndex(){return m_ScreenResolutionIndex;}*/
};
EXTERNGLOBALTON(CScreenResolution);