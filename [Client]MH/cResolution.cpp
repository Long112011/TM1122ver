#include "stdafx.h"
#include "MHFile.h"
#include "cResolution.h"
#include <d3d8.h>
#pragma comment (lib,"d3d8.lib")
#include <string>
#include <set>
#include <map>
#include "chatmanager.h"
GLOBALTON(CScreenResolution);
CScreenResolution::CScreenResolution()
{
	m_ResolutionInfoTable.Initialize(20);
}
CScreenResolution::~CScreenResolution()
{
	cRESOLUTION* pSInfo = NULL;
	m_ResolutionInfoTable.SetPositionHead();
	while(pSInfo = m_ResolutionInfoTable.GetData())
	{
		delete pSInfo;
		pSInfo=NULL;
	}
	m_ResolutionInfoTable.RemoveAll();
}
void CScreenResolution::Resolution()
{
	UpdateResolutionCombo();
}
void CScreenResolution::UpdateResolutionCombo()
{
	LPDIRECT3D8 videoDevice = Direct3DCreate8(
		D3D_SDK_VERSION);
	typedef std::pair< UINT, UINT > Resolution;
	typedef std::set< Resolution > ResolutionSet;
	ResolutionSet resolutionSet;
	UINT	adapterIndex	=	0;
	D3DADAPTER_IDENTIFIER8 adapterIdentifier = {0};
	if(FAILED(videoDevice->GetAdapterIdentifier(
		adapterIndex,
		D3DENUM_NO_WHQL_LEVEL,
		&adapterIdentifier)))
	{
		return;
	}
	D3DCAPS8 caps;
	ZeroMemory(
		&caps,
		sizeof(caps));
	if(FAILED(videoDevice->GetDeviceCaps(
		adapterIndex,
		D3DDEVTYPE_HAL,
		&caps)))
	{
		return;
	}
	int MaxX, MaxY;
	int TotalMaxX, TotalMaxY;
	const UINT displayModeSize = videoDevice->GetAdapterModeCount(adapterIndex);
	for(UINT modeIndex = 0; modeIndex < displayModeSize; ++modeIndex)
	{

		
		D3DDISPLAYMODE displayMode = {0};
		if(FAILED(videoDevice->EnumAdapterModes(
			adapterIndex,
			modeIndex,
			&displayMode)))
		{
			continue;
		}
		else if(800 > displayMode.Width)
		{
			continue;
		}
		else if(600 > displayMode.Height)
		{
			continue;
		}
		

		/*else */

		const Resolution resolution(displayMode.Width,displayMode.Height);
		
		/*if ((displayMode.Width + displayMode.Height) < (MaxX + MaxY))
		{
			continue;
		}*/

		/*if ((displayMode.Width&&displayMode.Height)<(MaxX&&MaxY))
		{
			continue;
		}*/
		

		if (displayMode.Width<MaxX)
		{
			continue;
		}
		/*else if (displayMode.Height<MaxY)
		{
			continue;
		}*/

			
		/*if (MaxX<displayMode.Width)
		{
			continue;
		}
		else if (MaxY<displayMode.Height)
		{
			continue;
		}*/


		if(resolutionSet.end() != resolutionSet.find(resolution))
		{
			continue;
		}

		resolutionSet.insert(resolution);
		MaxX = displayMode.Width;
		MaxY = displayMode.Height;
		TCHAR textResolution[MAX_PATH] = {0};
		_stprintf(textResolution,	_T("%4d x %d | %4d x %d"),			
			displayMode.Width,//resolution.first,
			displayMode.Height);// resolution.second);

		CHATMGR->AddMsg(CTC_SYSMSG,textResolution);
		//if (Check(resolution.first, resolution.second) == FALSE)
		//	continue;

			AddResolution(GetIndex(),resolution.first,resolution.second);
	}
}
void CScreenResolution::CreateResolution()
{
	FILE* fp = fopen("./Data/ScreenResolution.opt","w");
	//fprintf(fp,"%d\n",m_ResolutionInfoTable.GetDataNum());
	cRESOLUTION* pInfo;
	m_ResolutionInfoTable.SetPositionHead();
	while(pInfo = m_ResolutionInfoTable.GetData())
	{
		fprintf(fp,"%d\t%d\t%d\n",pInfo->Index,pInfo->x,pInfo->y);
	}
	fclose(fp);
}
BOOL CScreenResolution::Check(int x,int y)
{
	cRESOLUTION* pInfo;
	m_ResolutionInfoTable.SetPositionHead();
	while (pInfo = m_ResolutionInfoTable.GetData())
	{
		if(pInfo->x==x && pInfo->y==y);
		return FALSE;
	}
	return TRUE;
}
void CScreenResolution::AddResolution(int idx,int x,int y)
{
	cRESOLUTION* pInfo = m_ResolutionInfoTable.GetData(idx);
	if(!pInfo)
	{
		pInfo = new cRESOLUTION;
		pInfo->Index = idx;
		pInfo->x = x;
		pInfo->y = y;
		m_ResolutionInfoTable.Add(pInfo,pInfo->Index);
	}
	CreateResolution();
}
DWORD CScreenResolution::GetIndex()
{
	if(m_dwStartIndex<65533)
	{
		DWORD Temp = m_dwStartIndex;
		m_dwStartIndex++;
		return Temp;
	}
	else
	{
		m_dwStartIndex = 1;
		return m_dwStartIndex;
	}
}
#include "MHMap.h"
#include "GameResourceManager.h"
#include "ItemManager.h"
#include "ObjectManager.h"
BOOL CScreenResolution::Tumpang()
{
	CMHFile fp;		
	if( !fp.Init( "regen.txt", "rt" ) )
		return FALSE;
	char buff[256]={0,};
	while(1)
	{
		fp.GetString(buff);
		if(fp.IsEOF())
			break;
		if(buff[0] == '@')
		{
			fp.GetLineX(buff, 256);
			continue;
		}
		if( strcmp( buff, "$REGEN" ) == 0 )
		{
			DWORD dwGroup = fp.GetDword();
			if( dwGroup == 0 )
			{
				fp.Release();
				return FALSE;
			}
			if((fp.GetString())[0] == '{')
			{
				//VECTOR3 vHeroPos;
				float xPos;
				float zPos;
				MSG_EVENT_MONSTERREGEN msg;
				msg.Category	= MP_CHEAT;
				msg.Protocol	= MP_CHEAT_EVENT_MONSTER_REGEN;
				msg.dwObjectID	= HEROID;
				msg.MonsterKind = GAMERESRCMNGR->GetMonsterIndexForName( fp.GetString() );
				msg.cbMobCount	= fp.GetByte();
				msg.wMap		= MAP->GetMapNumForName( fp.GetString() );
				msg.cbChannel	= fp.GetByte();

				xPos			= (float)fp.GetWord() * 100.0f;
				zPos			= (float)fp.GetWord() * 100.0f;
				
				if( xPos == 0 && zPos == 0 )
				{
					
					//HERO->GetPosition(&vHeroPos);
					msg.Pos.x=(float)HERO->GetCurPosition().x;
					msg.Pos.z=(float)HERO->GetCurPosition().z;

					//CHATMGR->AddMsg( CTC_TOGM, CHATMGR->GetChatMsg( 431 ),
					//	"here", msg.cbChannel,
					//(int)(msg.Pos.x/100.0f), (int)(msg.Pos.z/100.0f) );
				}
				else
				{
					msg.Pos.x		= xPos;
					msg.Pos.z		= zPos;
				}

				msg.wRadius		= fp.GetWord()*100;

				ITEM_INFO* pInfo = ITEMMGR->FindItemInfoForName( fp.GetString() );
				if( pInfo )
					msg.ItemID		= pInfo->ItemIdx;
				else
					msg.ItemID		= 0;
				msg.dwDropRatio	= fp.GetDword();
				if( msg.MonsterKind == 0 || msg.wMap == 0 //|| msg.cbChannel > m_cbChannelCount
					|| ( msg.ItemID == 0 && msg.dwDropRatio != 0 ) )
				{
					ASSERT(0);
				}
				else
				{
					NETWORK->Send( &msg,sizeof(msg) );
				}					
			}
			else
			{
				fp.Release();
				return FALSE;
			}
			if((fp.GetString())[0] != '}')
			{
				fp.Release();
				return FALSE;
			}
		}
	}
	fp.Release();
}
BOOL CScreenResolution::Tumpang2()
{
	CMHFile fp;		
	if( !fp.Init( "regen.txt", "rt" ) )
		return FALSE;
	char buff[256]={0,};
	while(1)
	{
		fp.GetString(buff);
		if(fp.IsEOF())
			break;
		if(buff[0] == '@')
		{
			fp.GetLineX(buff, 256);
			continue;
		}
		if( strcmp( buff, "$REGENNEW" ) == 0 )
		{
			DWORD dwGroup = fp.GetDword();
			if( dwGroup == 0 )
			{
				fp.Release();
				return FALSE;
			}
			if((fp.GetString())[0] == '{')
			{
				//VECTOR3 vHeroPos;
				float xPos;
				float zPos;

				MSG_EVENT_MONSTERREGEN msg;
				msg.Category	= MP_CHEAT;
				msg.Protocol	= MP_CHEAT_EVENT_MONSTER_REGEN;
				msg.dwObjectID	= HEROID;

				msg.MonsterKind = fp.GetDword();//GAMERESRCMNGR->GetMonsterIndexForName( fp.GetString() );

				msg.cbMobCount	= fp.GetByte();

				msg.wMap		= fp.GetDword();//MAP->GetMapNumForName( fp.GetString() );

				msg.cbChannel	= fp.GetByte();

				//msg.Pos.x		= (float)fp.GetWord() * 100.0f;

				//msg.Pos.z		= (float)fp.GetWord() * 100.0f;

				xPos			= (float)fp.GetWord() * 100.0f;
				zPos			= (float)fp.GetWord() * 100.0f;
				
				if( xPos == 0 && zPos == 0 )
				{
					
					//HERO->GetPosition(&vHeroPos);
					msg.Pos.x=(float)HERO->GetCurPosition().x;
					msg.Pos.z=(float)HERO->GetCurPosition().z;

					//CHATMGR->AddMsg( CTC_TOGM, CHATMGR->GetChatMsg( 431 ),
					//	"here", msg.cbChannel,
					//(int)(msg.Pos.x/100.0f), (int)(msg.Pos.z/100.0f) );
				}
				else
				{
					msg.Pos.x		= xPos;
					msg.Pos.z		= zPos;
				}

				msg.wRadius		= fp.GetWord()*100;

				ITEM_INFO* pInfo = ITEMMGR->GetItemInfo(fp.GetDword());//ITEMMGR->FindItemInfoForName( fp.GetString() );

				if( pInfo )
					msg.ItemID		= pInfo->ItemIdx;
				else
					msg.ItemID		= 0;

				msg.dwDropRatio	= fp.GetDword();

				if( msg.MonsterKind == 0 || msg.wMap == 0 //|| msg.cbChannel > m_cbChannelCount
					|| ( msg.ItemID == 0 && msg.dwDropRatio != 0 ) )
				{
					ASSERT(0);
				}
				else
				{
					NETWORK->Send( &msg,sizeof(msg) );
				}					
			}
			else
			{
				fp.Release();
				return FALSE;
			}
			if((fp.GetString())[0] != '}')
			{
				fp.Release();
				return FALSE;
			}
		}
	}
	fp.Release();
}