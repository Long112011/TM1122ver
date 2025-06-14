#include <crtdbg.h>
#include "ChatManager.h"
#include "CheatMsgParser.h"
#include "MainGame.h"
#include "WindowIDEnum.h"
#include "./Interface/cWindowManager.h"
#include "./Interface/cEditBox.h"
#include "./Interface/cTextArea.h"
#include "./Interface/cFont.h"
#include "cMsgBox.h"
#include "./Input/UserInput.h"
#include "./Input/cIMEWnd.h"
#include "ChatManager.h"
#include "../[CC]Header/GameResourceManager.h"
#include "MouseCursor.h"
#include "mhFile.h"
#include "OptionManager.h"
#include "MHAudioManager.h"
#include "ObjectManager.h"
#include "MHMap.h"
#include "ExitManager.h"
#include "QuestManager.h"
#include "ServerConnect.h"
#include <string>
#include <vfw.h>
#include "cScriptManager.h"
#include "CharConvert.h"
#include "../[CC]JACK/JackJack.h"
//#include "../[CC]JACK/HantarManager.h"
#include "FadeManager.h"
#ifdef _GMTOOL_
#include "GMToolManager.h"
#include "CommCtrl.h"
#endif

char g_CLIENTVERSION[256] = "MYMC00000001";
char g_CLIENTREGISTRY[128] = "MYMC00000001";
#define MAX_LOADSTRING 512