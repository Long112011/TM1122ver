







#include "stdafx.h"
#include "NpcScriptDialog.h"
#include "NpcScriptManager.h"
#include "cDialogueList.h"
#include "cHyperTextList.h"
#include "WindowIDEnum.h"
#include "ObjectStateManager.h"
#include "ObjectManager.h"

#include "./Interface/cStatic.h"
#include "ChatManager.h"
#include "./Interface/cWindowManager.h"
#include "./Input/UserInput.h"

#include "Quest.h"
#include "QuestManager.h"
#include "ChatManager.h"
// 2014-05-08 include marry Header file
#include "Gamein.h"
#include "MiniMarryDialog.h"
#include "MiniBaishiDialog.h"

#include "CodeDialog.h"
cNpcScriptDialog::cNpcScriptDialog()
{
	// LBS 03.10.16
	m_type = WT_NPCSCRIPDIALOG;
	m_nHyperCount = 0;
	m_dwCurNpc = 0;

	m_pListDlg = NULL;
	m_pTitleText = NULL;

	for(int i=0 ; i< MAX_REGIST_HYPERLINK;++i)
		m_sHyper[i].Init();

	m_dwQuestIdx = 0;
}

cNpcScriptDialog::~cNpcScriptDialog()

{
}

void cNpcScriptDialog::Linking()
{
	m_pListDlg = (cListDialogEx*)this->GetWindowForID( NSI_LISTDLG );

	m_pTitleText = new cStatic;
	if( m_pTitleText )
	{
		m_pTitleText->Init( 13, 4, 0, 0, NULL, -1 );
		m_pTitleText->SetShadow( TRUE );
		m_pTitleText->SetFontIdx( 2 );
		m_pTitleText->SetAlign( TXT_LEFT );
		
		m_pTitleText->SetStaticText( "" );
		
		Add( m_pTitleText );

	}
}

BOOL cNpcScriptDialog::OpenDialog(DWORD dwNpcId, DWORD npcObjId)
{
	// �ʱ�ȭ

	for(int i=0 ; i< MAX_REGIST_HYPERLINK;++i)
		m_sHyper[i].Init();


	m_pListDlg->RemoveAll();
	m_nHyperCount = 0;

	// �������� ���� ������ ��û�Ѵ�.
	m_dwCurNpc = dwNpcId; // ���� Npc�� �����Ѵ�.
	m_npcObjId = npcObjId;

	cPage* pMainPage = NPCSCRIPTMGR->GetMainPage( m_dwCurNpc );
	if( pMainPage == NULL ) return FALSE;

	m_dwCurPageId = pMainPage->GetPageId(); // �ӽ÷� ���� ǥ�� �������� ������.

	// ������ ������ �̿��Ͽ� �������� �����Ѵ�.
	DWORD dwMsg = pMainPage->GetRandomDialogue(); // �޼����� ��ȣ

	// �޼��� ��ȣ�� �̿��Ͽ� ����� �޼����� �����´�.
	cDialogueList* pList = NPCSCRIPTMGR->GetDialogueList();
	if( pList == NULL ) return FALSE;

	cHyperTextList* pHyper = NPCSCRIPTMGR->GetHyperTextList();
	if( pHyper == NULL ) return FALSE;

// ��� ���� ���� ����
	WORD wIdx = 0;
	WORD LineInfo = 0;
	DIALOGUE* temp = NULL;

	LINKITEM* pItem = NULL;
	LINKITEM* Prev = NULL;

	while(1)
	{
		temp = pList->GetDialogue( dwMsg, wIdx );
		if( temp == NULL ) break;

		// ���������� ������ �������� ÷���Ѵ�.(�ʼ�!!)
		pItem = new LINKITEM;
//		pItem->Init();
		strcpy(pItem->string, temp->str);
		pItem->rgb = temp->dwColor;

		if( Prev == NULL )
		{
			m_pListDlg->cListItem::AddItem(pItem);
			LineInfo = temp->wLine;
		}

		else
		{
			if( LineInfo == temp->wLine )
			{
				Prev->NextItem = pItem;
			}
			else
			{
				m_pListDlg->cListItem::AddItem(pItem);
				LineInfo = temp->wLine;
			}
		}
		
		Prev = pItem;

		++wIdx;
	}


// ��ũ ������ �����ͼ� �����Ѵ�. (���� �۾� : ������ �˻��Ͽ� ������ ��ũ�� ���� ǥ��)
	HYPERLINK* pLink;

	int nLinkCount = pMainPage->GetHyperLinkCount(); 


// ��ũ�� ȭ�鿡 ����ϱ����� ������ ����.
	if( nLinkCount > 0 )
	{
		for( int i=0; i< 3;++i)
		{
			LINKITEM* pItem = new LINKITEM;
//			pItem->Init();
			char* temp = " ";
			strcpy(pItem->string, temp);
			
			m_pListDlg->cListItem::AddItem( pItem );
		}
	}

	// ���� ������ ���� ����
	BOOL bQuest = QUESTMGR->IsNpcRelationQuest( m_dwCurNpc );
/*	DWORD data = QUESTMGR->GetNpcScriptPage( m_dwCurNpc );
	BOOL aa = FALSE;
	if( data == 0 || data == 30 )
		aa = TRUE;
*/
	for( int j = 0 ; j < nLinkCount ; ++j )
	{
		pLink = pMainPage->GetHyperText(j);
		if( pLink == NULL ) break;

//		if( pLink->wLinkType == emLink_Quest && aa == TRUE )

		if( pLink->wLinkType == emLink_Quest && !bQuest )

			continue;

		temp = pHyper->GetHyperText( pLink->wLinkId );
		if( temp )
		{
			// �߰�
			LINKITEM* pItem = new LINKITEM;
//			pItem->Init();
			strcpy(pItem->string, temp->str);
			pItem->dwType = pLink->wLinkType;
			
			m_pListDlg->cListItem::AddItem( pItem );

			// Data
			m_sHyper[m_nHyperCount].bUse = TRUE;
			m_sHyper[m_nHyperCount].dwListItemIdx = m_pListDlg->GetItemCount()-1;

			m_sHyper[m_nHyperCount].sHyper = *pLink;
			++m_nHyperCount;
		}
	}

//	SetActiveRecursive( TRUE );
	SetActive( TRUE );

	//�׼ǿ��� �̸� ó����. - hs
//	OBJECTSTATEMGR->StartObjectState(HERO, eObjectState_Deal);

	CNpc* pObj = NPCSCRIPTMGR->GetSelectedNpc();
	if( pObj )
		SetDlgTitleText( pObj->GetObjectName() );
	else
		SetDlgTitleText( CHATMGR->GetChatMsg(232) );

	return TRUE;
}

BOOL cNpcScriptDialog::OpenLinkPage( DWORD dwPageId )
{
	// �ʱ�ȭ
	for(int i=0 ; i< MAX_REGIST_HYPERLINK;++i)
		m_sHyper[i].Init();

	m_pListDlg->RemoveAll();
	m_nHyperCount = 0;

	// �������� ���� ������ ��û�Ѵ�.
	cPage* pPage = NPCSCRIPTMGR->GetPage( m_dwCurNpc, dwPageId );
	if( pPage == NULL ) return FALSE;

	m_dwCurPageId = dwPageId; // �ӽ÷� ���� ǥ�� �������� ������.

	// ������ ������ �̿��Ͽ� �������� �����Ѵ�.
	DWORD dwMsg = pPage->GetRandomDialogue(); // �޼����� ��ȣ

	// �޼��� ��ȣ�� �̿��Ͽ� ����� �޼����� �����´�.
	cDialogueList* pList = NPCSCRIPTMGR->GetDialogueList();
	if( pList == NULL ) return FALSE;

	cHyperTextList* pHyper = NPCSCRIPTMGR->GetHyperTextList();
	if( pHyper == NULL ) return FALSE;

// ��� ���� ���� ����
	WORD wIdx = 0;
	WORD LineInfo = 0;
	DIALOGUE* temp = NULL;


	LINKITEM* pItem = NULL;
	LINKITEM* Prev = NULL;

	while(1)
	{
		temp = pList->GetDialogue( dwMsg, wIdx );
		if( temp == NULL ) break;

		// ���������� ������ �������� ÷���Ѵ�.(�ʼ�!!)
		pItem = new LINKITEM;
//		pItem->Init();
		strcpy(pItem->string, temp->str);
		pItem->rgb = temp->dwColor;

		if( Prev == NULL )
		{
			m_pListDlg->cListItem::AddItem(pItem);
			LineInfo = temp->wLine;
		}
		else
		{
			if( LineInfo == temp->wLine )
			{
				Prev->NextItem = pItem;
			}
			else
			{
				m_pListDlg->cListItem::AddItem(pItem);
				LineInfo = temp->wLine;
			}
		}
		
		Prev = pItem;

		++wIdx;

	}


// ��ũ ������ �����ͼ� �����Ѵ�. (���� �۾� : ������ �˻��Ͽ� ������ ��ũ�� ���� ǥ��)
	HYPERLINK* pLink;
	int nLinkCount = pPage->GetHyperLinkCount(); 


// ��ũ�� ȭ�鿡 ����ϱ����� ������ ����.
	if( nLinkCount > 0 )
	{
		for( int i=0; i< 3;++i)
		{
			LINKITEM* pItem = new LINKITEM;
//			pItem->Init();
			char* temp = " ";
			strcpy(pItem->string, temp);
			
			m_pListDlg->cListItem::AddItem( pItem );
		}
	}
/*
	for( int j = 0 ; j < nLinkCount ; ++j )
	{
		pLink = pPage->GetHyperText(j);
		if( pLink == NULL ) break;

		temp = pHyper->GetHyperText( pLink->wLinkId );
		if( temp )
		{
			// �߰�
			LINKITEM* pItem = new LINKITEM;
//			pItem->Init();
			strcpy(pItem->string, temp->str);
			pItem->dwType = pLink->wLinkType;
			
			m_pListDlg->cListItem::AddItem( pItem );

			// Data
			m_sHyper[m_nHyperCount].bUse = TRUE;
			m_sHyper[m_nHyperCount].dwListItemIdx = m_pListDlg->GetItemCount()-1;
			m_sHyper[m_nHyperCount].sHyper = *pLink;
			++m_nHyperCount;
		}
	}
*/	
	// ���� ������ ���� ����
	BOOL bQuest = QUESTMGR->IsNpcRelationQuest( m_dwCurNpc );
	for( int j = 0 ; j < nLinkCount ; ++j )
	{
		pLink = pPage->GetHyperText(j);
		if( pLink == NULL ) break;
		if( pLink->wLinkType == emLink_Quest && !bQuest )
			continue;

		temp = pHyper->GetHyperText( pLink->wLinkId );
		if( temp )
		{
			// �߰�
			LINKITEM* pItem = new LINKITEM;
			strcpy(pItem->string, temp->str);
			pItem->dwType = pLink->wLinkType;
			
			m_pListDlg->cListItem::AddItem( pItem );

			// Data
			m_sHyper[m_nHyperCount].bUse = TRUE;
			m_sHyper[m_nHyperCount].dwListItemIdx = m_pListDlg->GetItemCount()-1;
			m_sHyper[m_nHyperCount].sHyper = *pLink;
			++m_nHyperCount;
		}
	}

	return TRUE;
}

void cNpcScriptDialog::EndDialog()
{
	for(int i=0 ; i< MAX_REGIST_HYPERLINK;++i)
		m_sHyper[i].Init();

	m_pListDlg->RemoveAll();

	m_nHyperCount = 0;
	m_dwCurNpc = 0;

	NPCSCRIPTMGR->SetSelectedNpc( NULL );
	
	//�����Ŀ� ���� ���� �ִ�.
	if( HERO->GetState() != eObjectState_Die )

		OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);

//	SetActive( FALSE );
	SetDlgTitleText("");
}

HYPER* cNpcScriptDialog::GetHyperInfo( DWORD dwIdx )
{
	for(int i=0;i<m_nHyperCount;++i)
	{

		if( m_sHyper[i].bUse && m_sHyper[i].dwListItemIdx == dwIdx)
			return &m_sHyper[i];
	}

	return NULL;
}

void cNpcScriptDialog::HyperLinkParser( DWORD dwIdx )
{
	if( m_nHyperCount == 0 ) return;

	int nType = -1;
	DWORD dwData;
	int	hypertext = 0;

	for( int i=0 ; i<m_nHyperCount ; ++i)
	{
		if( m_sHyper[i].dwListItemIdx == dwIdx )
		{
			nType = m_sHyper[i].sHyper.wLinkType;
			dwData = m_sHyper[i].sHyper.dwData;
			hypertext = m_sHyper[i].sHyper.wLinkId;
			break;
		}
	}

	switch( nType )
	{
	case emLink_Page:
		{
			OpenLinkPage( dwData );
			m_pListDlg->ResetGuageBarPos();
		}
		break;
	case emLink_Open:
		{
			// �ŷ�â ����
			NPCSCRIPTMGR->StartNpcBusiness( dwData );		
		}
		break;
	case emLink_End:
		{
			EndDialog();
			DisActiveEx();
		}
		break;
	case emLink_Quest:
		{
			OpenQuestLinkPage( dwData );
		}
		break;
	case emLink_QuestLink:
		{
			m_dwQuestIdx = 0;
			DWORD dwPage = 0;
			CQuest* pQuest = QUESTMGR->GetQuest( dwData );
			if( pQuest )
			{
				dwPage = pQuest->GetNpcScriptPage( m_dwCurNpc );
				if( dwPage == 0 )
					dwPage = 30;
				OpenLinkPage( dwPage );

				m_dwQuestIdx = dwData;
			}						
		}
		break;
	case emLink_QuestStart:
		{
			// for quest - jsd : ���� hard coding ��.��
			if( m_dwQuestIdx == 45 || m_dwQuestIdx == 46 || m_dwQuestIdx == 47 )
			{
				CQuest* pQuest = QUESTMGR->GetQuest( m_dwQuestIdx );
				if( pQuest )
				if( pQuest->IsTimeCheck() )
				{
				}
				else
				{
                    if( hypertext == 347 || hypertext == 349 )
					{
						HERO->StartSocietyActForQuest( 259, -1, -1, TRUE );
					}
					OpenLinkPage( dwData );
				}
			}
			else
			{
				OpenLinkPage( dwData );

			}
//			OpenLinkPage( dwData );
			if( m_dwQuestIdx )
				QUESTMGR->SendNpcTalkEvent(m_dwCurNpc, m_dwQuestIdx, m_npcObjId);
			if(NPCSCRIPTMGR->GetSelectedNpc())
			if(NPCSCRIPTMGR->GetSelectedNpc()->GetNpcJob() == BOMUL_ROLE
#ifdef _NAGA_				
				|| NPCSCRIPTMGR->GetSelectedNpc()->GetNpcJob() == DRAGON_ROLE
#endif				
				)
			{
				MSG_DWORD msg;
				msg.Category = MP_NPC;
				msg.Protocol = MP_NPC_DOJOB_SYN;
				msg.dwData = NPCSCRIPTMGR->GetSelectedNpc()->GetID();
				msg.dwObjectID = HEROID;
				NETWORK->Send(&msg, sizeof(msg));
			}
		}
		break;
	case emLink_EventQuestStart:
		{
			if( dwData )
				QUESTMGR->SendNpcTalkEventForEvent( m_dwCurNpc, dwData );
			EndDialog();
			DisActiveEx();
		}
		break;
	case emLink_QuestContinue:
		{
			OpenLinkPage( dwData );
		}
		break;
	case emLink_QuestEnd:
		{
			QUESTMGR->QuestAbandon( m_dwQuestIdx );
			OpenLinkPage( dwData );
		}
		break;
	case emLink_MapChange:
		{
			g_UserInput.GetHeroMove()->MapChange((WORD)dwData );
		}
		break;
	case emLink_Change_State_1:  // ChangeStage Func 2014-05-05
		{
           if( HERO->GetState() != eObjectState_Die)
		   {
				if (HERO->GetLevel() < 40 || HERO->GetLevel() > 69)
				{
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(2074));
					return;
				}
				if(HERO->GetStage()!=0 )
				{
                    CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(2080));
					return;
				}

				MSG_DWORD msg;
				SetProtocol(&msg, MP_CHAR, MP_CHAR_CHANGESTAGE_SYN);
				msg.dwObjectID = HEROID;
				msg.dwData = 1;
				NETWORK->Send(&msg, sizeof(msg));
		   }
		}
		break;

	    case emLink_Change_State_2:  
		{
           if( HERO->GetState() != eObjectState_Die)
		   {
				if (HERO->GetLevel() < 40 || HERO->GetLevel() > 69)
				{
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(2074));
					return;
				}
				if(HERO->GetStage()!=0 )
				{
                    CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(2080));
					return;
				}
				MSG_DWORD msg;
				SetProtocol(&msg, MP_CHAR, MP_CHAR_CHANGESTAGE_SYN);
				msg.dwObjectID = HEROID;
				msg.dwData = 2;
				NETWORK->Send(&msg, sizeof(msg));
		   }
		}
		break;
	    case emLink_Change_State_3:  
		{
           if( HERO->GetState() != eObjectState_Die)
		   {
				if (HERO->GetLevel() < 70)
				{
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(2075));
					return;
				}
				if(HERO->GetStage()!=64)
				{
                    CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(2078));
					return;
				}
				MSG_DWORD msg;
				SetProtocol(&msg, MP_CHAR, MP_CHAR_CHANGESTAGE_SYN);
				msg.dwObjectID = HEROID;
				msg.dwData = 3;
				NETWORK->Send(&msg, sizeof(msg));
		   }
		}
		break;

		case emLink_Change_State_4:  
		{
           if( HERO->GetState() != eObjectState_Die)
		   {
				if (HERO->GetLevel() < 70)
				{
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(2075));
					return;
				}
				if(HERO->GetStage()!=128)
				{
                    CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(2079));
					return;
				}
				MSG_DWORD msg;
				SetProtocol(&msg, MP_CHAR, MP_CHAR_CHANGESTAGE_SYN);
				msg.dwObjectID = HEROID;
				msg.dwData = 4;
				NETWORK->Send(&msg, sizeof(msg));
		   }
		}
		break;

	case emLink_Change_Live:    // ChangeLive Func 2014-05-05
		{
			if( HERO->GetState() != eObjectState_Die )
			{
				MSG_DWORD msg;
				SetProtocol(&msg, MP_CHAR, MP_CHAR_CHANGELIFE_SYN);
				msg.dwObjectID = HEROID;
				msg.dwData = 4;
				NETWORK->Send(&msg, sizeof(msg));
			}
		}
		break;
	 
	case emLink_MaryDlg:      // MarryDlalog Func 2014-05-08
		{
			if(GAMEIN->GetMiniMaryDialog()->IsActive())
			{
				GAMEIN->GetMiniMaryDialog()->SetActive(FALSE);
			}
			else
			{
				GAMEIN->GetMiniMaryDialog()->SetActive(TRUE);
				char Name[MAX_NAME_LENGTH+1] = {0};
				CObject* pTarget = OBJECTMGR->GetSelectedObject();
				if(pTarget && (pTarget->GetObjectKind() == eObjectKind_Player))
				{
					SafeStrCpy(Name, OBJECTMGR->GetSelectedObject()->GetObjectName(), MAX_NAME_LENGTH+1);
					GAMEIN->GetMiniMaryDialog()->SetName(Name);
				}
			}
		}
		break;
	case emLink_MaryDelDlg:
		{
			if (!HERO->IsMarry())
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1826 ) );
				return;
			}
			MSG_NAME msg;
			SetProtocol(&msg, MP_MARRY, MP_MARRY_DEL_SYN);
			msg.dwObjectID = HEROID;
			SafeStrCpy(msg.Name, HERO->GetMarryName(), MAX_NAME_LENGTH + 1);
			NETWORK->Send(&msg, sizeof(msg));
		}
		break;
		case emLink_Baishi:
		{
			if(GAMEIN->GetMiniBaishiDialog()->IsActive())
			{
				GAMEIN->GetMiniBaishiDialog()->SetActive(FALSE);
			}
			else
			{
				GAMEIN->GetMiniBaishiDialog()->SetActive(TRUE);
				char Name[MAX_NAME_LENGTH+1] = {0};
				CObject* pTarget = OBJECTMGR->GetSelectedObject();
				if(pTarget && (pTarget->GetObjectKind() == eObjectKind_Player))
				{
					SafeStrCpy(Name, OBJECTMGR->GetSelectedObject()->GetObjectName(), MAX_NAME_LENGTH+1);
					GAMEIN->GetMiniBaishiDialog()->SetName(Name);
				}
			}
		}
		break;
		case emLink_JiechuBaishi:
		{
			MSG_DWORD msg;
			SetProtocol(&msg, MP_SHITU, MP_SHITU_DEL_SYN);
			msg.dwObjectID = HEROID;
			NETWORK->Send(&msg, sizeof(msg));
		}
		break;
	case emLink_NpcCode:
		{
			GAMEIN->GetCodeDialog()->SetActive(TRUE);
			OBJECTSTATEMGR->StartObjectState( HERO, eObjectState_Deal );
			DisActiveEx();
		}
		break;
	}	

}

void cNpcScriptDialog::SetDlgTitleText( char* strTitle )
{
	if( !m_pTitleText ) return;

	m_pTitleText->SetStaticText( strTitle );
}

void cNpcScriptDialog::SetActive( BOOL val )
{
	if( m_bActive == val ) return;

	if(!val)
	{
		CNpc* pNpc = NPCSCRIPTMGR->GetSelectedNpc();
		if(pNpc) 
		{			
			if( pNpc->GetNpcJob() == BOMUL_ROLE 
#ifdef _NAGA_
				|| pNpc->GetNpcJob() == DRAGON_ROLE
#endif
				)
			{
				MSG_DWORD msg;
				msg.Category = MP_NPC;
				msg.Protocol = MP_NPC_CLOSEBOMUL_SYN;
				msg.dwObjectID = HERO->GetID();
				msg.dwData = NPCSCRIPTMGR->GetSelectedNpc()->GetID();
				NETWORK->Send(&msg, sizeof(msg));
			}
		}
		EndDialog();
	}

	cDialog::SetActiveRecursive( val );
}

// �ٸ������� ���� ȣ������ ���ÿ�!!
void cNpcScriptDialog::DisActiveEx()
{
	for(int i=0 ; i< MAX_REGIST_HYPERLINK;++i)
		m_sHyper[i].Init();

	m_pListDlg->RemoveAll();

	m_nHyperCount = 0;
	m_dwCurNpc = 0;

	NPCSCRIPTMGR->SetSelectedNpc( NULL );

	SetDlgTitleText("");

	cDialog::SetActiveRecursive( FALSE );
	WINDOWMGR->SetMouseInputProcessed();

}

void cNpcScriptDialog::OpenQuestLinkPage( DWORD dwPageId )
{
	// �ʱ�ȭ
	for( int i = 0; i < MAX_REGIST_HYPERLINK; ++i )
		m_sHyper[i].Init();

	m_pListDlg->RemoveAll();
	m_nHyperCount = 0;

	// �������� ���� ������ ��û�Ѵ�.
	cPage* pPage = NPCSCRIPTMGR->GetPage( m_dwCurNpc, dwPageId );
	if( pPage == NULL ) return;

	// �ӽ÷� ���� ǥ�� �������� ������.
	m_dwCurPageId = dwPageId; 

	// ������ ������ �̿��Ͽ� �������� �����Ѵ�.
	DWORD dwMsg = pPage->GetRandomDialogue();

	// �޼��� ��ȣ�� �̿��Ͽ� ����� �޼����� �����´�.
	cDialogueList* pList = NPCSCRIPTMGR->GetDialogueList();
	if( pList == NULL ) return;

	cHyperTextList* pHyper = NPCSCRIPTMGR->GetHyperTextList();
	if( pHyper == NULL ) return;

	// ��� ���� ���� ����
	WORD wIdx = 0;
	WORD LineInfo = 0;
	DIALOGUE* temp = NULL;

	LINKITEM* pItem = NULL;
	LINKITEM* Prev = NULL;


	while(1)
	{
		temp = pList->GetDialogue( dwMsg, wIdx );
		if( temp == NULL ) break;

		// ���������� ������ �������� ÷���Ѵ�.(�ʼ�!!)
		pItem = new LINKITEM;
		strcpy(pItem->string, temp->str);
		pItem->rgb = temp->dwColor;

		if( Prev == NULL )
		{
			m_pListDlg->cListItem::AddItem(pItem);

			LineInfo = temp->wLine;
		}
		else
		{
			if( LineInfo == temp->wLine )
			{

				Prev->NextItem = pItem;
			}
			else
			{
				m_pListDlg->cListItem::AddItem(pItem);
				LineInfo = temp->wLine;
			}
		}
		Prev = pItem;
		++wIdx;
	}

	// ��ũ ������ �����ͼ� �����Ѵ�.
	HYPERLINK* pLink;
	int nLinkCount = pPage->GetHyperLinkCount(); 

	pItem = new LINKITEM;
	strcpy( pItem->string, CHATMGR->GetChatMsg(649) );
	pItem->rgb = RGBA_MAKE( 0, 255, 0, 255 );
	m_pListDlg->cListItem::AddItem( pItem );

	// ���� ����
	pItem = new LINKITEM;
	strcpy( pItem->string, "" );
	m_pListDlg->cListItem::AddItem( pItem );

	BOOL bCheck = FALSE;
	// ���⼭ ���� ���� �˻� �� ������ ��ũ�� ���� ǥ��
	// ���డ���� ����
	for( int j = 0; j < nLinkCount; ++j )
	{
		pLink = pPage->GetHyperText(j);
		if( pLink == NULL ) break;

		CQuest* pQuest = QUESTMGR->GetQuest( pLink->dwData );
		if( pQuest )
		{
			if( pQuest->IsQuestState( m_dwCurNpc ) != 1 )
				continue;
		}
		else
			continue;

		if( !bCheck )
		{
			pItem = new LINKITEM;
			strcpy( pItem->string, CHATMGR->GetChatMsg(653) );
			pItem->rgb = RGBA_MAKE( 0, 255, 0, 255 );
			m_pListDlg->cListItem::AddItem( pItem );
			bCheck = TRUE;
		}

		temp = pHyper->GetHyperText( pLink->wLinkId );
		if( temp )
		{
			// �߰�

			pItem = new LINKITEM;
			strcpy( pItem->string, temp->str );
			pItem->dwType = pLink->wLinkType;
			
			m_pListDlg->cListItem::AddItem( pItem );

			// Data
			m_sHyper[m_nHyperCount].bUse = TRUE;
			m_sHyper[m_nHyperCount].dwListItemIdx = m_pListDlg->GetItemCount()-1;
			m_sHyper[m_nHyperCount].sHyper = *pLink;
			++m_nHyperCount;
		}
	}

	if( bCheck )
	{
		// ���� ����
		pItem = new LINKITEM;
		strcpy( pItem->string, "" );
		m_pListDlg->cListItem::AddItem( pItem );
	}

	bCheck = FALSE;	
	// �������� ����
	for( int j = 0; j < nLinkCount; ++j )
	{
		pLink = pPage->GetHyperText(j);
		if( pLink == NULL ) break;

		CQuest* pQuest = QUESTMGR->GetQuest( pLink->dwData );
		if( pQuest )
		{
			if( pQuest->IsQuestState( m_dwCurNpc ) != 2 )
				continue;
		}
		else
			continue;

		if( !bCheck )
		{
			pItem = new LINKITEM;
			strcpy( pItem->string, CHATMGR->GetChatMsg(654) );
			pItem->rgb = RGBA_MAKE( 0, 255, 0, 255 );

			m_pListDlg->cListItem::AddItem( pItem );
			bCheck = TRUE;
		}

		temp = pHyper->GetHyperText( pLink->wLinkId );
		if( temp )
		{
			// �߰�
			pItem = new LINKITEM;
			strcpy( pItem->string, temp->str );
			pItem->dwType = pLink->wLinkType;
			
			m_pListDlg->cListItem::AddItem( pItem );

			// Data
			m_sHyper[m_nHyperCount].bUse = TRUE;
			m_sHyper[m_nHyperCount].dwListItemIdx = m_pListDlg->GetItemCount()-1;
			m_sHyper[m_nHyperCount].sHyper = *pLink;
			++m_nHyperCount;
		}
	}
}




