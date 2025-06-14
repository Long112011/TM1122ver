#pragma once
class CObject;
class CDataBlock;
class CDataBlockManager;
#define PACKEDDATA_OBJ CPackedData::GetInstance()
class CPackedData
{
	SEND_PACKED_DATA m_SendData;
	BOOL m_bInited;
	BOOL m_bUsing;
	DWORD m_MaxConnectionIndexPlusOne;
	WORD* m_PackedIDNum;
	CDataBlock** m_PackedIDs;
	CDataBlockManager* m_pDataBlockManager;
	void Init(int MaxConnectionIndex, void* pData, WORD MsgLen);
public:
	GETINSTANCE(CPackedData)
		CPackedData();
	virtual ~CPackedData();
	void AddObject(CObject* pObject);
	void Send();
	void QuickSend(CObject* pObject, MSGBASE* pMsg, int MsgLen);
	void QuickSendExceptObjectSelf(CObject* pObject, MSGBASE* pMsg, int MsgLen);
	void SendToMapServer(DWORD AgentNum, MAPTYPE ToMapNum, MSGBASE* pMsg, WORD MsgLen);
	void SendToBroadCastMapServer(MSGBASE* pMsg, WORD MsgLen);
	MSGBASE* GetMsg()	{ return (MSGBASE*)m_SendData.Data; }
	int GetMsgLen()		{ return m_SendData.wRealDataSize; }
	friend class CGridSystem;
	friend class CGeneralGridTable;
	DWORD GetLastSendIDNum();
};