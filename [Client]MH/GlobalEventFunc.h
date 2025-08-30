




#ifndef _GLOBALEVENTFUNC_H_
#define _GLOBALEVENTFUNC_H_

/* ���� Ÿ��Ʋ */
struct FUNC
{
	void (*_func)(LONG lId, void * p, DWORD we);
	const char * _funcName;
};
extern FUNC g_mt_func[];
extern int FUNCSEARCH(char * funcName);


//pjs �α׾��̵� â������ ��ư ó��...
extern void		MT_LogInOkBtnFunc(LONG lId, void* p, DWORD we);	 
extern void		MT_ExitBtnFunc(LONG lId, void* p, DWORD we);
extern void		MT_EditReturnFunc(LONG lId, void* p, DWORD we); //3
extern void		MT_DynamicBtnFunc(LONG lId, void* p, DWORD we); //3
extern void		MT_DynamicEditReturnFunc(LONG lId, void* p, DWORD we);

//pjs �α�ĳ���� â���� ��ư ó��...
extern void		CS_BtnFuncCreateChar(LONG lId, void* p, DWORD we);	 
extern void		CS_BtnFuncDeleteChar(LONG lId, void* p, DWORD we);	 
extern void		CS_BtnFuncFirstChar(LONG lId, void* p, DWORD we);	 
extern void		CS_BtnFuncSecondChar(LONG lId, void* p, DWORD we);	 
extern void		CS_BtnFuncThirdChar(LONG lId, void* p, DWORD we);	 
extern void		CS_BtnFuncFourthchar(LONG lId, void* p, DWORD we);	 
extern void		CS_BtnFuncFifthchar(LONG lId, void* p, DWORD we);	 
extern void		CS_BtnFuncEnter(LONG lId, void* p, DWORD we);
extern void		CS_FUNC_OkISee(LONG lId, void* p, DWORD we);	 //8->11
extern void		CS_BtnFuncLogOut(LONG lId, void* p, DWORD we);	 //9->12
//�ɸ��� ����â [5/13/2003]
extern void	CM_OverlapCheckBtnFunc(LONG lId, void* p, DWORD we);
extern void	CM_ComboBoxCheckFunc(LONG lId, void* p, DWORD we);
extern void	CM_CharMakeBtnFunc(LONG lId, void* p, DWORD we);
extern void	CM_CharCancelBtnFunc(LONG lId, void* p, DWORD we);
extern void CMFUNC_OkISee(LONG lId, void* p, DWORD we); //5->16
extern void CM_OptionSelectBtnFunc(LONG lId, void* p, DWORD we);

/*main interface*/
extern void MI_CharBtnFunc(LONG lId, void* p, DWORD we);
extern void MI_ExchangeBtnFunc(LONG lId, void* p, DWORD we);
extern void MI_InventoryBtnFunc(LONG lId, void* p, DWORD we);
extern void MI_MugongBtnFunc(LONG lId, void* p, DWORD we);
//extern void MI_SystemBtnFunc(LONG lId, void* p, DWORD we);
extern void MI_ChatEditBoxFunc(LONG lId, void * p, DWORD we); //7->23

extern void MI_DlgFunc(LONG lId, void * p, DWORD we);

/*Inventory interface*/
extern void IN_CreateStreetStallFunc(LONG lId, void * p, DWORD we);
extern void IN_DlgFunc(LONG lId, void * p, DWORD we);
/*Mugong Dialog Interface*/
extern void MGI_DlgFunc(LONG lId, void * p, DWORD we);
extern void MGI_MugongDlgFunc(LONG lId, void * p, DWORD we);
extern void MGI_SuryunDlgFunc(LONG lId, void * p, DWORD we);
/*character infomation interface*/
extern void CI_DlgFunc(LONG lId, void * p, DWORD we);
extern void CI_AddExpPoint(LONG lId, void * p, DWORD we);
/*party interface*/
extern void PA_DlgFunc(LONG lId, void * p, DWORD we); //5->28

/*Quick Dialog Interface */
extern void QI_QuickDlgFunc(LONG lId, void * p, DWORD we);

// LYJ 051017 ���Գ����� �߰�
extern void SO_DlgFunc(LONG lId, void * p, DWORD we);

extern void BRS_DlgFunc(LONG lId, void * p, DWORD we);
extern void BRS_CloseFunc(LONG lId, void * p, DWORD we);
extern void BRS_DeleteFunc(LONG lId, void * p, DWORD we);
extern void BRS_TypeListFunc(LONG lId, void * p, DWORD we);
extern void BRS_ItemListFunc(LONG lId, void * p, DWORD we);
extern void BRS_ClassListFunc(LONG lId, void * p, DWORD we);
extern void BRS_REGBtnFunc(LONG lId, void * p, DWORD we);
extern void BRS_VolumeEditFunc(LONG lId, void * p, DWORD we);
extern void BRS_MoneyEditFunc(LONG lId, void * p, DWORD we);

extern void BS_DlgFunc(LONG lId, void * p, DWORD we);
extern void BS_TitleEditBoxFunc(LONG lId, void * p, DWORD we);
extern void BS_MoneyEditBoxFunc(LONG lId, void * p, DWORD we);
extern void BS_SellBtnFunc(LONG lId, void * p, DWORD we);
extern void BS_REGBtnFunc(LONG lId, void * p, DWORD we);
extern void BS_SPFunc(LONG lId, void * p, DWORD we);

/*StreetStall interface*/
extern void SSI_DlgFunc(LONG lId, void * p, DWORD we);
extern void SSI_TitleEditBoxFunc(LONG lId, void * p, DWORD we);
extern void SSI_MoneyEditBoxFunc(LONG lId, void * p, DWORD we);
extern void SSI_BuyBtnFunc(LONG lId, void * p, DWORD we);
extern void SSI_RegistBtnFunc(LONG lId, void * p, DWORD we);
extern void SSI_EditBtnFunc(LONG lId, void * p, DWORD we);	//6->34

/*NPC ���â*/
extern void NSI_DlgFunc(LONG lId, void * p, DWORD we);
extern void NSI_HyperLinkFunc(LONG lId, void * p, DWORD we);

/*Helpâ*/
extern void HI_DlgFunc(LONG lId, void * p, DWORD we);
extern void HI_HyperLinkFunc(LONG lId, void * p, DWORD we);
extern void HI_BtnFunc(LONG lId, void * p, DWORD we);
extern void HSI_ExitBtnFunc(LONG lId, void * p, DWORD we);

/*����â*/
extern void CMI_MoneyOkFunc(LONG lId, void * p, DWORD we);
extern void CMI_MoneySpinFunc(LONG lId, void * p, DWORD we);
extern void CMI_AlertFunc(LONG lId, void * p, DWORD we);

/*����â*/
extern void DE_DlgFunc(LONG lId, void * p, DWORD we);
extern void DE_BuyItemFunc(LONG lId, void * p, DWORD we);
extern void DE_SellItemFunc(LONG lId, void * p, DWORD we);
extern void DE_CloseHideBtn(LONG lId, void * p, DWORD we); //7->41
/*��Ƽ*/
extern void PA_BtnFunc(LONG lId, void * p, DWORD we);


/*���*/
extern void GD_DlgFunc( LONG lId, void* p, DWORD we );

/*ǥ��*/
extern void PYO_DlgBtnFunc(LONG lId, void * p, DWORD we);
extern void Note_DlgBtnFunc(LONG lId, void * p, DWORD we);
extern void Friend_DlgBtnFunc(LONG lId, void * p, DWORD we);
// 2014-05-08 Marry func 
extern void Marry_DlgBtnFunc(LONG lId, void * p, DWORD we);

// 2014-05-10 ShiTu func 
extern void ShiTu_DlgBtnFunc(LONG lId, void * p, DWORD we);



/*ĳ���� ���*/
extern void CR_DlgBtnFunc(LONG lId, void * p, DWORD we);

/*�����NPC*/
extern void WANTNPC_DlgFunc( LONG lId, void* p, DWORD we );
extern void WANTREG_DlgFunc( LONG lId, void* p, DWORD we );

/*����Ʈ*/
extern void QUE_QuestDlgFunc( LONG lId, void* p, DWORD we );
extern void QUE_JournalDlgFunc( LONG lId, void* p, DWORD we );


/*������*/
extern void MP_RegistDlgFunc( LONG lId, void* p, DWORD we );

/*������*/
extern void BAIL_BailDlgFunc( LONG lId, void* p, DWORD we );

//KES DIALOG
/*��ũ��*/

extern void MAC_DlgFunc(LONG lId, void * p, DWORD we);
//extern void MAC_ModeDlgFunc(LONG lId, void * p, DWORD we);
/*ä��â*/
extern void CTI_DlgFunc(LONG lId, void * p, DWORD we);
extern void COI_DlgFunc(LONG lId, void * p, DWORD we);
extern void GAMEIN_ChatFunc(LONG lId, void * p, DWORD we);
/*�ɼ�â*/
extern void OTI_DlgFunc(LONG lId, void * p, DWORD we);
/*����â*/
extern void EXT_DlgFunc(LONG lId, void * p, DWORD we);
/*��ȯâ*/
extern void XCI_DlgFunc(LONG lId, void * p, DWORD we);
/*ĳ���ͻ���â*/
extern void CSS_DlgFunc(LONG lId, void * p, DWORD we);
extern void MSI_MenuSlotDlgFunc( LONG lId, void* p, DWORD we );
/*PKLOOTING*/
extern void PLI_DlgFunc( LONG lId, void* p, DWORD we );
/*CNA_BtnOKFunc*/
extern void CNA_BtnOkFunc( LONG lId, void* p, DWORD we );
/*�����ۺ���*/
extern void DIS_DlgFunc( LONG lId, void* p, DWORD we );
extern void ITEM_DlgFunc( LONG lId, void* p, DWORD we );
// MallShop Func 2014-08-16
extern void TC_DlgFunc( LONG lId, void* p, DWORD we );
extern void TCBuy_DlgFunc(LONG lId, void * p, DWORD we);
/*������*/
extern void MNPRI_DlgFunc(LONG lId, void * p, DWORD we);
extern void MNPRI_ChatFunc( LONG lId, void * p, DWORD we );
extern void MNCNL_DlgFunc(LONG lId, void * p, DWORD we);
extern void MNCNL_ChatFunc(LONG lId, void * p, DWORD we);

/*�޼����ڽ�*/
extern void MessageBox_Func(LONG lId, void * p, DWORD we);
extern void DIVIDEBOX_Func(LONG lId, void * p, DWORD we);
/*�̴ϸ�*/
extern void MNM_DlgFunc(LONG lId, void * p, DWORD we);

/*��޾� â*/
extern void ITD_DlgFunc(LONG lId, void * p, DWORD we);
/*���� â*/
extern void ITMD_DlgFunc(LONG lId, void * p, DWORD we);
/*Ÿ��ź��������ũâ*/
extern void TPMD_DlgFunc(LONG lId, void * p, DWORD we);	// magi82 - Titan(070112)
// magi82 - Titan(070119) ////////////////////////////////////////////////
extern void TitanMix_DlgFunc(LONG lId, void * p, DWORD we);	
extern void Titan_Repair_DlgFunc(LONG lId, void * p, DWORD we);	//2007. 9. 11. CBH - Ÿ��ź ����â �̺�Ʈ
extern void Titan_Recall_DlgFunc(LONG lId, void * p, DWORD we); //2007. 9. 14. CBH - Ÿ��ź ��ȯ �̺�Ʈ
//////////////////////////////////////////////////////////////////////////
extern void TitanUpgrade_DlgFunc(LONG lId, void * p, DWORD we);	// magi82 - Titan(070131)
extern void TitanBreak_DlgFunc(LONG lId, void * p, DWORD we);	// magi82 - Titan(070205)
extern void titan_inventory_DlgFunc(LONG lId, void * p, DWORD we);	// magi82 - Titan(070226)
extern void Titan_guage_DlgFunc(LONG lId, void * p, DWORD we);		// magi82 - Titan(070306)
//extern void Titan_MugongMix_DlgFunc(LONG lId, void * p, DWORD we);		// magi82 - Titan(070611) Ÿ��ź ������ȯ �ּ�ó��
extern void Titan_Use_DlgFunc(LONG lId, void * p, DWORD we);		// magi82 - Titan(070320)
extern void Titan_Bongin_DlgFunc(LONG lId, void * p, DWORD we);		// magi82 - Titan(070323)
/* ��ȭ â*/
extern void ITR_DlgFunc(LONG lId, void * p, DWORD we);
/*ä�� â*/
extern void CHA_DlgBtnFunc(LONG lId, void * p, DWORD we);

/*�����۸�â��*/
extern void ITMALL_DlgBtnFunc(LONG lId, void * p, DWORD we);

/*�̵�â*/
extern void SA_DlgBtnFunc(LONG lId, void * p, DWORD we);


/*������ġ*/
extern void CHA_DlgFunc(LONG lId, void * p, DWORD we);

// ServerList
extern void SL_DlgBtnFunc( LONG lId, void* p, DWORD we );

// guildfieldwar
extern void GFW_DlgFunc( LONG lId, void* p, DWORD we );

// SkillPoint Redistribute
extern void SK_DlgBtnFunc( LONG lId, void* p, DWORD we );

// partywar
extern void PW_DlgFunc( LONG lId, void* p, DWORD we );

// chase

extern void CHS_DlgFunc( LONG lId, void* p, DWORD we );

//
extern void CHAN_DlgFunc(LONG lId, void * p, DWORD we);

// Guild Tournament
extern void GDT_DlgFunc(LONG lId, void * p, DWORD we);


// SeigeWar
extern void SW_DlgFunc(LONG lId, void* p, DWORD we);

// Character change
extern void CHA_ChangeDlgFunc(LONG lId, void* p, DWORD we);

extern void IT_DlgFunc(LONG lId, void* p, DWORD we);

//
extern void CJOB_DlgFunc(LONG lId, void* p, DWORD we);

extern void CG_DlgFunc(LONG lId, void *p, DWORD we);

extern void RFDefault_DlgFunc(LONG lId, void *p, DWORD we);
extern void RareCreate_DlgFunc(LONG lId, void *p, DWORD we);

extern void ItemStoneDk_DlgFunc(LONG lId, void *p, DWORD we);  // ��ť 2014-11-15

extern void ItemStoneXq_DlgFunc(LONG lId, void *p, DWORD we);  // ��ť 2014-11-18

// Pet
extern void PET_DlgFunc(LONG lId, void *p, DWORD we);
extern void PET_MiniDlgFunc(LONG lId, void *p, DWORD we);
extern void PET_InvenDlgFunc(LONG lId, void *p, DWORD we);
extern void PET_UpgradeDlgFunc(LONG lId, void *p, DWORD we);
extern void PET_RevivalDlgFunc(LONG lId, void *p, DWORD we);
// 06. 02. ��ȭ ��� �ȳ� �������̽� �߰� - �̿���
extern void RFDGUIDE_DlgFunc(LONG lId, void *p, DWORD we);

// 06. 02. ������â ������ �߰� - �̿���
extern void TB_DlgFunc(LONG lId, void *p, DWORD we);
extern void TB_STATE_DlgFunc(LONG lId, void *p, DWORD we);


extern void GN_DlgBtnFunc(LONG lId, void *p, DWORD we);
extern void AN_DlgBtnFunc(LONG lId, void *p, DWORD we);

// 06. 03. ���İ��� - �̿���
extern void GNotice_DlgBtnFunc(LONG lId, void *p, DWORD we);

extern void GuildPlusTime_DlgFunc(LONG IId, void* p, DWORD we);//Add 060803 by wonju

//////////////////////////////////////////////////////////////////
// 06. 06. 2�� ���� - �̿���
// ������ȯ
extern void SkillTrans_DlgFunc(LONG lId, void *p, DWORD we);
extern void TDefault_DlgFunc(LONG lId, void *p, DWORD we);
//////////////////////////////////////////////////////////////////

extern void JO_DlgFunc(LONG lId, void* p, DWORD we);
extern void GU_DlgFunc(LONG lId, void* p, DWORD we);	// magi82 - ���ϻ� �������ý���
extern void GT_DlgFunc(LONG lId, void* p, DWORD we);	// magi82 - ���ϻ� �������ý���

extern void UniqueItemCurseCancellationDlgFunc(LONG lId, void* p, DWORD we);	// magi82 - Unique(070703)
extern void UniqueItemMixDlgFunc(LONG lId, void* p, DWORD we);	// magi82 - Unique(070709)

extern void SOSDlgFunc(LONG lId, void* p, DWORD we);	// magi82 - SOS(070724)


// 2007. 10. 19. CBH - ����ũ ������ ���� ������ //////////////////////////////////////
extern void UniqueItemMix_ProgressBar_DlgFunc(LONG lId, void * p, DWORD we);
///////////////////////////////////////////////////////////////////////////
// 2007. 10. 22. CBH - Ÿ��ź ���� ������ //////////////////////////////////////
extern void TitanMix_ProgressBar_DlgFunc(LONG lId, void * p, DWORD we);
///////////////////////////////////////////////////////////////////////////
// 2007. 10. 23. CBH - Ÿ��ź ���� ���� ������ //////////////////////////////////////
extern void TitanParts_ProgressBar_DlgFunc(LONG lId, void * p, DWORD we);
///////////////////////////////////////////////////////////////////////////

// 2007. 12. 4. CBH - ��Ų ���� //////////////////////////////////////
extern void SkinSelect_DlgFunc(LONG lId, void * p, DWORD we);
///////////////////////////////////////////////////////////////////////////
// 2008. 1. 16. CBH - �ڽ�Ƭ ��Ų ���� //////////////////////////////////////
extern void CostumeSkinSelect_DlgFunc(LONG lId, void * p, DWORD we);
///////////////////////////////////////////////////////////////////////////

// magi82(42) - �������� �߰�(����ġ �ʱ�ȭ)
extern void SK_Reset_DlgFunc(LONG lId, void * p, DWORD we);


// 2008. 3. 17. CBH - ���ȹ�ȣ �Է� �̺�Ʈ  //////////////////////////////////////
extern void NumberPad_DlgFunc(LONG lId, void * p, DWORD we);
//////////////////////////////////////////////////////////////////////////
//2008. 3. 19. CBH - �α��� ���̾�α� �޺��ڽ� �̺�Ʈ �߰� //////////////////////
extern void MT_LOGINDLGFunc(LONG lId, void * p, DWORD we);
//////////////////////////////////////////////////////////////////////////

// ������ �˻� �߰� by Stiner(8)
extern void SFR_DlgFunc(LONG lId, void * p, DWORD we);

// ������ ���� �߰� by Stiner(8)
extern void SVD_DlgFunc(LONG lId, void * p, DWORD we);

// autonote
extern void AutoNoteDlg_Func(LONG lId, void * p, DWORD we);
extern void AutoAnswerDlg_Func(LONG lId, void * p, DWORD we);

///////////////////////////////////////////////////////////////////
// 2008. 5. 21. CBH - ��Ƽ ���� �������̽� ��ư �̺�Ʈ �߰�
extern void PA_CreateBtnFunc(LONG lId, void * p, DWORD we);
//2008. 6. 2. CBH - ��Ƽ ��Ī ��� ���̾�α� �̺�Ʈ
extern void Party_MatchingList_DlgFunc(LONG lId, void * p, DWORD we);
/////////////////////////////////////////////////////////////////////

// �����
extern void FW_DlgFunc(LONG lId, void* p, DWORD we);
extern void FD_WareHouseDlgFunc( LONG lId, void* p, DWORD we );

// 2014-05-01 Create MallItem Window
extern void MALL_DIALOGFunc(LONG lId,void *p,DWORD we);

// 2014-05-21 Create bar Control Window 

extern void  BARCONTROL_DIALOGFunc(LONG lId, void * p, DWORD we);

// 2014-05-21 Create toplist Window 

extern void Ranking_DlgFunc(LONG lId,void * p,DWORD we);
extern void DGRanking_DlgFunc(LONG lId, void * p, DWORD we);
// 2018-01-12 �������ͷ�ļ����� 

extern void RankingGuild_DlgFunc(LONG lId, void * p, DWORD we);

//weiye �齱������������ 2018-01-12

extern void Lucker_DlgFunc(LONG lId,void * p,DWORD we);

// 2014-12-05 Item luck func define !

extern void IT_LOCKDlgFunc(LONG lId,void * p,DWORD we);

// 2014-12-07 Item unLuck func define !

extern void IT_UNLOCKDlgFunc(LONG lId,void * p,DWORD we);

// 2014-08-11 Create characer info and Equip Window !

extern void CG_TargetObjectBtnFunc( LONG lId, void* p, DWORD we );
 
// 2015-01-26 �Ĳ�

extern void ITEMGAMBLE_DlgFunc( LONG lId, void* p, DWORD we );

//weiye ��ȫ�뺯�����塡2017-10-20

//extern void Safe_DlgBtnFunc(LONG lId, void * p, DWORD we); 

//weiye ��ȫ��ͼ�꺯�����塡2017-10-20

extern void Safe_ICONBtnFunc(LONG lId, void * p, DWORD we);

//weiye ��ȫ�뾲̬ͼ�꺯�����塡2018-01-20

extern void Safe_IMGBtnFunc(LONG lId, void * p, DWORD we);

//weiye  ��ȫ��������� 2018-04-08

//extern void SafeLock_DlgBtnFunc(LONG lId, void * p, DWORD we);

//weiye  ��ȫ���޸ĺ���  2018-04-08

//extern void SafeChange_DlgBtnFunc(LONG lId, void * p, DWORD we);

//weiye  С��ͼ�·����ڶ���  2018-01-17

extern void TOP_ICONBtnFunc(LONG lId, void * p, DWORD we);
 
//weiye  Ԫ��ϴE������������ 2018-02-10

extern void FAMEGOLD_DlgFunc(LONG lId, void * p, DWORD we);

//weiye  ���ټӵ㺯������ 2017-12-31

//extern void CG_CHARCONTROLBtnFunc(LONG lId, void * p, DWORD we);

//weiye ����װ�������������� 2018-03-10

extern void SENDEQUIP_BtnFunc(LONG lId, void * p, DWORD we);

//weiye  ��Ļ¼�����ô��� 2018-05-14

extern void VIDEOCAPTUREDlg_Func(LONG lId, void * p, DWORD we);

//weiye  ��Ļ¼��ͼ�괰�ں���  2018-05-14

extern void VIDEOCAPTUREICON_Func(LONG lId, void * p, DWORD we);

extern void CD_DlgFunc(LONG lId, void * p, DWORD we);

extern void RANKBUTTON_DIALOGFunc(LONG lId, void * p, DWORD we);

extern void InsFUBENInfo_DlgFunc(LONG lId, void * p, DWORD we);

extern void HIDE_BuffDlgFunc(LONG lId,void *p,DWORD we);


extern void TB_AdPointDlgFunc(LONG lId,void *p,DWORD we);


extern void Reconnect_DlgFunc(LONG lId,void *p,DWORD we);

extern void CI_RegIDDlgFunc(LONG lId,void *p,DWORD we);

extern void RCM_DlgFunc(LONG lId, void * p, DWORD we);

extern void InsFUBENEntranceInfo_DlgFunc(LONG lId, void * p, DWORD we);

extern void LuckyDraw_DlgFunc(LONG lId, void * p, DWORD we); 

extern void ChangeLook_DlgFunc(LONG lId, void * p, DWORD we); 

extern void ItemDrop_DlgFunc(LONG lId, void * p, DWORD we); 

//extern void ItemLink_DlgFunc(LONG lId, void * p, DWORD we);

extern void PetMulti_DlgFunc(LONG lId, void * p, DWORD we);

extern void FPSDialog_Func(LONG lId, void * p, DWORD we);



void InsDGInfo_DlgFunc(LONG lId, void* p, DWORD we);
void InsDGEntranceInfo_DlgFunc(LONG lId, void* p, DWORD we);
void InsDGPartyMake_DlgFunc(LONG lId, void* p, DWORD we);
void InsDGRank_DlgFunc(LONG lId, void* p, DWORD we);

extern void ItemStepReinforce_DlgFunc(LONG lId, void * p, DWORD we);

extern void LoginIDDialog_hide(LONG lId, void * p, DWORD we);

extern void		CS_BtnChangePass(LONG lId, void* p, DWORD we);
extern void OfficialUpGradeDlg_Func(LONG lId, void* p, DWORD we);
//װ������
extern void ItemQuality_DlgFunc(LONG lId, void* p, DWORD we);
extern void ItemQualityChange_DlgFunc(LONG lId, void* p, DWORD we);
extern void GGD_Func(LONG lId, void* p, DWORD we);// ��������ֵת�ƾ�
extern void VIP_BtnFunc(LONG lId, void* p, DWORD we);//VIPϵͳ
//�Զ���ƺ�
extern void Customizing_DlgFunc(LONG lId, void* p, DWORD we);
#ifdef  _MUTIPET_
extern void PetMerge_DlgFunc(LONG lId, void* p, DWORD we);//����  3pet
#endif //  _MUTIPET_
#endif //_GLOBALEVENTFUNC_H_


