



//----------------------------------------------------------------------------------------------------
//  StallFindDlg   version:  1.0   ��  date: 03/31/2008
//
//  Copyright (C) 2008 - All Rights Reserved
//----------------------------------------------------------------------------------------------------
///	Index	Stiner(8)
///	@file	StallFindDlg.h
///	@author	�̼���
///	@brief	���� �˻��� ���� ���̾�α�
//----------------------------------------------------------------------------------------------------
#pragma once

#include "./Interface/cDialog.h"
#include "GameResourceStruct.h"

#define SEARCH_DELAY		3000	///< �˻� ���� �ð�
#define	ITEMVIEW_DELAY		1000	///< ������ �� ����
#define MAX_RESULT_PAGE		5		///< �ִ� ������
#define MAX_LINE_PER_PAGE	6		///< �ִ� �� ��

class cListDialog;
class cEditBox;
class cButton;
class cPushupButton;
class cComboBox;
class cStatic;

class CMouse;
class CExchangeItem;

/// @class	���� �˻� ���̾�α� Ŭ����
class CStallFindDlg : public cDialog
{
	/// @brief ������ ��/�Һз� ���� ����ü
	struct TItemInfo
	{
		int		Type;		///< ������ ��з�
		int		DetailType;	///< ������ �Һз�
		DWORD	ItemIdx;	///< ������ �ε���
	};


	/// @brief ������ ��з� ����
	enum ITEM_TYPE
	{
		WEAPON,				///< ����
		CLOTHES,			///< �Ǻ�
		ACCESSORY,			///< ��ű�
		POTION,				///< ȸ����
		MATERIAL,			///< ���

		ETC,				///< ��Ÿ
		ITEM_MALL,			///< ������ ��
		TITAN_ITEM,			///< Ÿ��ź ������

		ITEM_TYPE_COUNT,	///< Ÿ�� �ִ� ����
	};

	BOOL		m_bSearchedAll;			///< ��ü�˻��̿����� üũ
	DWORD		m_dwSearchType;			///< �˻� Ÿ��

	cPtrList	m_ptrItemInfo;			///< ������ ����

	int						m_nStallCount;						///< �˻��� ������ ��
	STREETSTALL_PRICE_INFO	m_arrStallInfo[MAX_STALLITEM_NUM];	///< ������ ����

	cComboBox*		m_pItemTypeCombo;							///< ������ ��з� Ÿ�� �޺�
	cComboBox*		m_arrItemDetailTypeCombo[ITEM_TYPE_COUNT];	///< ������ �Һз� Ÿ�� �޺�


	cListDialog*	m_pItemList;		///< ������ ����Ʈ
	cListDialog*	m_pClassList;		///< ��޸���Ʈ

	cPushupButton*	m_pSellModeRadioBtn;	///< �Ǹ� �˻� ������ư
	cPushupButton*	m_pBuyModeRadioBtn;		///< ���� �˻� ������ư

	cStatic*		m_pNameStatic;		///< ������ ����Ʈ �ɸ��͸� ����
	cStatic*		m_pPriceStatic;		///< ������ ����Ʈ �ɸ��͸� ����

	cListDialog*	m_pStallList;					///< ������ ����Ʈ
	cPushupButton*	m_parrPageBtn[MAX_RESULT_PAGE];	///< ������ ��ư
	cButton*		m_parrPageUpDownBtn[2];			///< ������ ��/�ٿ� ��ư(0:��/1:�ٿ�)

	int	m_nBasePage;				///< ������ �⺻ ������
	int	m_nMaxPage;					///< ��ü ������
	int	m_nCurrentPage;				///< ���� ������ ��� ������

	int m_nItemType;				///< ���õ� ��з� ������ Ÿ��
	int m_nItemDetailType;			///< ���õ� �Һз� ������ Ÿ��

	int	m_nSelectedItemListIdx;		///< ���õ� ������
	int	m_nSelectedClassListIdx;	///< ���õ� Ŭ����
	int	m_nSelectedStallListIdx;	///< ���õ� ������

	DWORD	m_dwSelectedObjectIndex;	///< ���� ������ ������ �ε��� ��
	DWORD	m_dwPrevSelectedType;		///< ���� ������ �˻� Ÿ��


	/// @brief	������ ����Ʈ �ε�
	/// @return	void
	void	LoadItemList();

	/// @brief	������ ����Ʈ ����
	/// @return	void
	void	UpdateItemList();

	/// @brief	���� ����Ʈ ����
	/// @return	void
	void	UpdateStallList();


	/// @brief	���� ����
	/// @return	void
	/// @param	flag	- TRUE:��������, FALSE:��������

	void	SortStallList(BOOL flag);

	/// @brief	���� ����Ʈ ������ ����
	/// @return	void
	/// @param	index	- 0 ~ 4�� ��
	void	SetPage(int index);

	/// @brief	�⺻ ������ ����
	/// @return	void
	/// @param	bNext	- TRUE : PageUp / FALSE : PageDown
	void	SetBasePage(BOOL bNext);

	/// @brief	�ð� ���� ýũ
	/// @return	BOOL	- TRUE : dwDelayTime ���� / FALSE : dwDelayTime ����
	/// @param	dwDelayTime	- �ð� ����(�и������� ����)
	/// @param	nID	- ���� üũ ID, ID���� ���� üũ�� �̷���� 0~4���� ����
	BOOL	CheckDelay(DWORD dwDelayTime, int nID);


public:
	CStallFindDlg(void);
	~CStallFindDlg(void);

	/// @brief	������Ʈ ���
	/// @return	void
	void	Linking();


	/// @brief	�˻� Ÿ�� ����
	/// @return	void
	/// @param	val		- eSK_SELL:���Գ��� / eSK_BUY:�Ǹų���
	void	SetSearchType(DWORD val);

	/// @brief	���� �˻� ��� ���� ����
	/// @return	void
	/// @param	pStallInfo	- ������ ���� ����ü�� ������
	void	SetStallPriceInfo(SEND_STREETSTALL_INFO * pStallInfo);

	/// @brief	������ ���� ��Ŷ �۽�
	/// @return	void
	void	SendItemViewMsg();

	/// @brief	���õ� �������� ���� �ε��� ����
	/// @return	DWORD	- ������ ���� �ε���(ID��)
	inline DWORD	GetSelectedObjectIndex()	{ return m_dwSelectedObjectIndex; };

	/// @brief	���õ� �˻� Ÿ�� ����
	/// @return	DWORD	- eSK_BUY / eSK_SELL ������ ���ϵ�
	inline DWORD	GetPrevSelectedType()		{ return m_dwPrevSelectedType; };

	/// @brief	���� �Ǿ��� �������� ���� �ε��� ����
	/// @return	void
	/// @param	dwIndex	- ������ �ε��� ��
	inline void		SetSelectedObjectIndex(DWORD dwIndex)	{ m_dwSelectedObjectIndex = dwIndex; };

	/// @brief	���� �Ǿ��� �˻� Ÿ�� ����
	/// @return	void
	/// @param	dwType	- eSK_BUY / eSK_SELL
	inline void		SetPrevSelectedType(DWORD dwType)		{ m_dwPrevSelectedType = dwType; };


	/// @brief	���̾�α��� �̺�Ʈ �ڵ鷯 \n �� �޼��忡�� ������ �̺�Ʈ ó�� �޼���(On�޼���)�� ȣ��
	/// @return	void

	/// @param	lId	- �̺�Ʈ �߻��� ID��
	/// @param	p	- �̺�Ʈ �߻��� ��ü�� �θ� ��ü ������
	/// @param	we	- �̺�Ʈ ��
	void	OnActionEvent(LONG lId, void * p, DWORD we);

	/// @brief	���̾�α� Ȱ��/��Ȱ��
	/// @return	void
	/// @param	val	- TRUE:Ȱ�� / FALSE:��Ȱ��
	virtual void	SetActive(BOOL val);

	/// @brief	�̺�Ʈ ���� / ó��
	/// @return	DWORD	- �̺�Ʈ ��
	/// @param	mouseInfo	- ���콺 �����ѿ� ���� ����
	virtual DWORD	ActionEvent(CMouse * mouseInfo);

protected:
	//--- �̺�Ʈ ó�� ---//
	void	OnClickSearchBtn();										///< �˻� ��ư
	void	OnClickSearchAllBtn();									///< ��ü�˻� ��ư
	void	OnClickFindTypeBtn(LONG lId, void * p, DWORD we);		///< �˻� Ÿ�� ���� ��ư	void	OnClickPageBtn(LONG lId, void * p, DWORD we);			///< ������ ��ư
	void	OnEventTypeCombo(LONG lId, void * p, DWORD we);			///< ��з� �޺��ڽ�
	void	OnEventDetailTypeCombo(LONG lId, void * p, DWORD we);	///< �Һз� �޺��ڽ�
	void	OnEventItemList(LONG lId, void * p, DWORD we);			///< ������ ����Ʈ
	void	OnEventClassList(LONG lId, void * p, DWORD we);			///< ��� ����Ʈ
	void	OnEventStallList(LONG lId, void * p, DWORD we);			///< �˻���� ����Ʈ
	void	OnClickPageBtn(LONG lId);								///< ������ ��ư
	void	OnClickPageUpDonwBtn(LONG lId);							///< ������ ��/�ٿ� ��ư
	void	OnClose();												///< �ݱ��ư
};


