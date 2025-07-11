




#ifndef __AGENTNETWORKMSGPARSER_H__
#define __AGENTNETWORKMSGPARSER_H__

void MP_POWERUPMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void MP_AGENTSERVERMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);

void MP_USERCONNMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void MP_CHATMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void MP_CHATServerMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);

void TransToClientMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void TransToMapServerMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void MP_PACKEDMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void MP_PARTYServerMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);

// 2014-05-09 marry NetParam func !
void MP_MARRYMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);

// 2014-05-16 global Client Msg Send

void MP_CLIENTMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);

void MP_PARTYUserMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);	//2008. 6. CBH - ��Ƽ ��Ī ���� �߰�
void MP_FRIENDMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void MP_NOTEMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void MP_NOTEServerMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void MP_WANTEDServerMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);


//void MP_MORNITORTOOLMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void MP_MURIMNETServerMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);

void MP_MURIMNETUserMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void MP_OPTIONUserMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);

void MP_MonitorMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);

void MP_DebugMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void MP_CHEATUserMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void MP_CHEATServerMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void MP_HACKCHECKMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void MP_GUILDServerMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void MP_GUILDUserMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);

void MP_GUILD_FIELDWARServerMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void MP_GUILD_FIELDWARUserMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);

BOOL CheckCharacterMakeInfo( CHARACTERMAKEINFO* pmsg );

//
void MP_ITEMUserMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void MP_ITEMUserMsgParserExt(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void MP_ITEMServerMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void MP_ITEMServerMsgParserExt(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);

void MP_GTOURNAMENTUserMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void MP_GTOURNAMENTServerMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);

void MP_JACKPOTUserMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void MP_JACKPOTServerMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);


//-- skill
void MP_SkillUserMsgParser( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength );
void MP_SkillServerMsgParser( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength );

// guildunion
void MP_GUILD_UNIONUserMsgParser( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength );
void MP_GUILD_UNIONServerMsgParser( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength );

// Siege War
void MP_SIEGEWARUserMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void MP_SIEGEWARServerMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);

// siegewarprofit
void MP_SIEGEWARPROFITUserMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void MP_SIEGEWARPROFITServerMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);

// 2005 ũ�������� �̺�Ʈ
// GM�� ���� ���� ������ �ʺ��� �ϱ� ����...

void MP_WEATHERUserMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);

void MP_STREETSTALLUserMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void MP_EXCHANGEUserMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);


void MP_SURVIVALUserMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void MP_SURVIVALServerMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);

//SW070626 ���λ�NPC
void MP_BOBUSANGUserMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void MP_BOBUSANGServerMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);

// itemlimit - tamoo
void MP_ITEMLIMITServerMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);

// autonote - tamoo
void MP_AUTONOTEUserMsgParser( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength );
void MP_AUTONOTEServerMsgParser( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength );



// autonote - tamoo
//void MP_FORTWARUserMsgParser( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength );
void MP_FORTWARServerMsgParser( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength );


#ifdef _HACK_SHIELD_
void MP_HACKSHIELDUserMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
#endif

#ifdef _NPROTECT_
void MP_NPROTECTUserMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
#endif
void MP_GUILDWARServerMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);

void MP_DUNGEON_UserMsgParser( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void UserConn_DungeonEntrace_Syn(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength) ;
//void MP_FBTIMERServerMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
//void MP_FBTIMERUserMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);

void MP_INSDUNGEONUserMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void MP_INSDUNGEONServerMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
#endif //__AGENTNETWORKMSGPARSER_H__


