#if !defined(AFX_CLISTITEM_H__EFECF4A3_00CC_4F48_A2CA_775C28D96531__INCLUDED_)
#define AFX_CLISTITEM_H__EFECF4A3_00CC_4F48_A2CA_775C28D96531__INCLUDED_
#if _MSC_VER > 1000
#pragma once
#endif 
#include "PtrList.h"
#include "cWindowDef.h"
class cListItem
{
public:
	cListItem() :m_wMaxLine(0)
	{
	}
	virtual ~cListItem()
	{
		RemoveAll();
	}
	LONG GetItemCount()
	{
		return m_ListItem.GetCount();
	}
	virtual void AddItem(ITEM* pItem)
	{
		if (m_wMaxLine < 1) return;
		if (m_wMaxLine <= GetItemCount())
		{
			ITEM* item = (ITEM*)m_ListItem.GetHead();
			SAFE_DELETE(item);
			m_ListItem.RemoveHead();
		}
		m_ListItem.AddTail(pItem);
	}
	virtual void AddItem(ITEM* pItem, LONG idx)
	{
		if (m_wMaxLine < 1) return;
		if (m_wMaxLine <= GetItemCount())
		{
			ITEM* item = (ITEM*)m_ListItem.GetHead();
			SAFE_DELETE(item);
			m_ListItem.RemoveHead();
		}
		PTRLISTPOS p = m_ListItem.FindIndex(idx);
		if (p == NULL) return;
		m_ListItem.InsertAfter(p, pItem);
	}
	virtual void RemoveAll()
	{
		PTRLISTSEARCHSTART(m_ListItem, ITEM*, item)
			delete item;
		PTRLISTSEARCHEND
			m_ListItem.RemoveAll();
	}
	virtual void RemoveItem(LONG idx)
	{
		PTRLISTPOS p = m_ListItem.FindIndex(idx);
		if (p)
		{
			ITEM * item = (ITEM*)m_ListItem.GetAt(p);
			SAFE_DELETE(item);
			m_ListItem.RemoveAt(p);
		}
	}
	virtual void RemoveItem(char * str)
	{
		PTRLISTPOS p = m_ListItem.GetHeadPosition();
		while (p)
		{
			ITEM* item = (ITEM*)m_ListItem.GetAt(p);
			if (item)
			{
				if (strcmp(item->string, str) == 0)
				{
					SAFE_DELETE(item);
					m_ListItem.RemoveAt(p);
					break;
				}
			}
			m_ListItem.GetNext(p);
		}
	}
	//-----------------------------------------------------------------------
	int RemoveItem2(char * str)
	{
		int nIndex = -1;
		PTRLISTPOS p = m_ListItem.GetHeadPosition();
		while (p)
		{
			ITEM* item = (ITEM*)m_ListItem.GetAt(p);

			if (item)
			{
				nIndex++;
				if (strcmp(item->string, str) == 0)
				{
					SAFE_DELETE(item);

					m_ListItem.RemoveAt(p);
					break;
				}
			}

			//m_ListItem.GetNext( p );
		}
		return nIndex;
	}
	//-----------------------------------------------------------------------
	void SetMaxLine(WORD maxLine)	{ m_wMaxLine = maxLine; }
	WORD GetMaxLine()				{ return m_wMaxLine; }
protected:
	WORD		m_wMaxLine;
	cPtrList	m_ListItem;
};
#endif 
