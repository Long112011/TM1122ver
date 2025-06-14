#ifndef _HELPER_MANAGER_
#define _HELPER_MANAGER_
#include "PtrList.h"
#include "../MHFile.h"
#define HELPERMGR USINGTON(cHelperManager)
class cPageBase;
class cDialogueList;
class cHelper;
class cHelperSpeechDlg;
class cHelperManager 
{
protected:
	cPtrList				m_HelperSpeechList;		
	cDialogueList*			m_pDialogue;			
	cHelper*				m_pHelper;				
	cHelperSpeechDlg*		m_pSpeechDlg;
	BOOL					m_bInit;
public:
	cHelperManager();
	virtual ~cHelperManager();
	void Init();
	void Release();
	void SetActive(BOOL bActive);
	void LoadSpeechList();
	void LoadPageInfo(cPageBase* pPage, CMHFile* fp);
	void LoadHelper();
	void LoadHelperInfo( CMHFile* fp );
	void LoadFrameInfo( CMHFile* fp );
	void AddPage(cPageBase* pPage);
	void DeleteAllPageInfo();
	cDialogueList* GetDialogueList() { return m_pDialogue; }
	cPageBase* GetPage( DWORD dwPageId );
	cHelper* GetHelper() { return m_pHelper; }
	cHelperSpeechDlg* GetHelperDlg() { return m_pSpeechDlg; }
public: 
	void	ShowRespect( DWORD dwPageId, BOOL bComp = TRUE );
	void	ChangeSpeechDlg( DWORD dwPageId, BOOL bComp = TRUE );
	BOOL	IsCompleteInit() { return m_bInit; }
};
EXTERNGLOBALTON(cHelperManager)
#endif 