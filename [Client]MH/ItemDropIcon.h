#pragma once
#include "INTERFACE\cIcon.h"
#include "cImage.h"
#define RATE_LENGTH 8
class CDropItem : public cIcon
{
private:
	char	item[MAX_NAME_LENGTH+5];
	char	percent[MAX_NAME_LENGTH+5];

	/*char	m_Rate1[RATE_LENGTH];
	char	m_Rate2[RATE_LENGTH];
	char	m_Rate3[RATE_LENGTH];*/
public:
	CDropItem();   
	virtual ~CDropItem();
	void Render();
	
	void SetItemName(char * pName){ SafeStrCpy(item, pName, MAX_NAME_LENGTH + 5); }
	void SetItemPercent(char * pName){ SafeStrCpy(percent, pName, MAX_NAME_LENGTH + 5); }

	/*void SetRate1(char * pName){ SafeStrCpy(m_Rate1, pName, RATE_LENGTH); }
	void SetRate2(char * pName){ SafeStrCpy(m_Rate2, pName, RATE_LENGTH); }
	void SetRate3(char * pName){ SafeStrCpy(m_Rate3, pName, RATE_LENGTH); }*/
};