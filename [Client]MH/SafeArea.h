
#if !defined(AFX_SAFEAREA_H__8C774E0A_9D8C_4535_A3B2_F847517FA424__INCLUDED_)
#define AFX_SAFEAREA_H__8C774E0A_9D8C_4535_A3B2_F847517FA424__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./Interface/cDialog.h"
#include "Player.h"

struct SAFEAREAINFO  //weiye P1-P4 �ֱ��ʾ�����ĸ�������
{
	int MapNum;
	 
	VECTOR2 P1;

	VECTOR2 P2;

	VECTOR2 P3;

	VECTOR2 P4;

	SAFEAREAINFO()
	{
		MapNum=0; //weiye ��ʼ��Ĭ�ϵ�ͼ�����ð�ȫ����
	}
};

#define SAFEAREAMGR	USINGTON(SafeAreaManager)

class SafeAreaManager: public cDialog
{

public:

	SafeAreaManager();

    virtual	~SafeAreaManager();

	void LoadSafeAreaInfo();

	SAFEAREAINFO * GetSafeAreaInfo(int MapNum);

    void  Process();

	int  CheckSafeArea();

	int  CheckSafeArea(VECTOR3 * pMovePos);

private:

	DWORD   m_SafeAreaTime;          //weiye �жϰ�ȫ����ʱ�������

	VECTOR3  Pos;                 //weiye ��ɫ��ά���궨�� 

	CYHHashTable<SAFEAREAINFO>  m_SafeAreaList;

	BOOL      m_IsMsg;

};

EXTERNGLOBALTON(SafeAreaManager)

#endif