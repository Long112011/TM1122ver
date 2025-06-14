
#pragma once


//#include <mmsystem.h>

#include <chrono>													// Used for the C++ 11 time functionality
#include <thread>													// used for the C++ 11 sleep functionality

#define TICK_PER_DAY (24 * 60 * 60 * 1000)
#define TIMEMGR		HTimeManager::GetInstance()
#define HTR_S(x)	HTimeManager::GetInstance()->RecordTime_Start(x);
#define HTR_E(x)	HTimeManager::GetInstance()->RecordTime_End(x);


enum eRecordType	
{
	// 시간기록�?동시�?여러 곳에�?할수 있도�?타입을 두어�?
	// 원하�?타입의 시간�?얻어내게 한다.
	eRT_TotalLoop = 0,
	eRT_ConnectEdge,
	eRT_rtProcessTotal,
	eRT_BeginProcess,
	eRT_DrawDebugText,
	eRT_ProcessGarbageObject,

	eRT_GameStateProcess,
	eRT_SetVertices,
	eRT_PutToVertexBuffer,
	eRT_DrawPrimitive,

	eRT_EndProcess,
	eRT_displayFPS,
	eRT_DynamicCastCost,

	eRT_HeartBeatLastestSend,

	eRT_Present,
	eRT_BeginProcess2,

	eRT_ElapsedTime,
	eRT_FPSControl,

	eRT_CollisionCheck,


	eRT_OneLineCheck,		// 직선으로 이동가능한가
	eRT_TestLoop,		

	eRT_InputPoint,
	eRT_PathFind_Astar,
	eRT_MakeAStarPath,
	eRT_Optimize,
	eRT_Optimize1,
	eRT_Optimize2,

	eRT_AStarTotal,

	eRT_PopCheck,
	eRT_PopCheck2,

	eRT_ThreadCheck,

	eRT_PathManagerLoad,

	eRT_NUMBER_TYPE,
};

#define MHTIMEMGR USINGTON(CMHTimeManager)
class CMHTimeManager  
{
	DWORD m_MHDate;
	DWORD m_MHTime;
	
	DWORD m_lasttime;

	int m_ServerTimeTick;
	__time64_t m_ServerTime;
private:
	///--------------------------------------------------
	/// Variables
	///--------------------------------------------------
	LONG		m_TotalFrames;							// 그냥 기록되는 �?Frame�?

	LONG		m_FPSsetted;							// 셋팅�?FPS�?맞춰지기를 원하�?�?
	LONG		m_FPS;									// 구해�?FPS
	LONG		m_FPS_Average;							// 구해�?누적 평균 FPS


	double		m_ElaspedTime_Acc;						// 누적�?지�?시간
 	LONG		m_Count_OneSec;							// 1�?동안 카운트된 프레임수
	BOOL		m_IsFPSControled;						// FPS가 제어되는가? (기본은 TRUE)

public:
	///--------------------------------------------------
	/// External Method
	///--------------------------------------------------	
	void		Calc_ElapsedTime();				/// ※이전의 Process역할�?하는 �?
												/// 전번 기록 시간과의 차이�?구한�?
												// 더불�?평균 FPS�?잰다.
												// 설정�?프레임수�?조절
	BOOL		ToggleFPSControl();				// 부르면 FPS제어�?�?것인가 안할 것인가�?토글시켜준�?


	// GetSet
	void		SetFPS(LONG FPS);				// 0�?경우 지연시�?없음
	LONG		GetSettedFPS();					// 설정�?FPS�?반환한다.

	DWORD		GetFPS();						// 현재 FPS�?얻는�?
	DWORD		GetFPS_average();				// 누적 FPS 평균�?얻는�?
	LONG		GetCountOneSec();				// 1초동�?카운트되�?프레임수

	BOOL		Get_ToggleFPSControl();			// On/off 체크해준�? 
	LONG		GetTotalFrames();				// 그냥 기록되는 �?Frame�?

	//-------------------------------------------------------
	// Another way to calculate FPS BY JACK
	//-------------------------------------------------------
	// The function to get the instance of the manager, or initialize and return the instance.
	// By creating a static variable of the instance we ensure this only gets created once.
	// This is also thread-safe in C++ 11 according to the specifications.
	static CMHTimeManager& Instance()
	{
		static CMHTimeManager instance;

		return instance;
	}

	// This calculates our current scene's frames per second and displays it in the console
	double CalculateFrameRate();
	double CalculateFrameRate2();

	// This returns the current time in seconds (since 1/1/1970, call "epoch")
	double CMHTimeManager::GetTime();

	// This pauses the current thread for an amount of time in milliseconds
	void Sleep(int milliseconds);

	// This is the time slice that stores the total time in seconds that has elapsed since the last frame
	double DeltaTime = 0;

	// This is the current time in seconds
	double CurrentTime = 0;
public:

//	//MAKESINGLETON(CMHTimeManager);

	

	CMHTimeManager();
	virtual ~CMHTimeManager();


	void Init(DWORD mhDate,DWORD mhTime);

	void Process();

	DWORD GetMHDate();
	DWORD GetMHTime();
	
	void GetMHDate(BYTE& year,BYTE& month,BYTE& day);
	void GetMHTime(BYTE& hour,BYTE& minute);


	DWORD GetNewCalcCurTime();

	static CMHTimeManager* GetInstance()	{static CMHTimeManager gInstance;return &gInstance;}

	void SetServerTime( __time64_t serverTime );
	__time64_t GetServerTime() const { return m_ServerTime; }

//---------------------------------------------DELAY------------
//private:

	bool  CheckTimer12000();
	float m_Timer12000;


	bool  CheckTimer5000();
	float m_Timer5000;

	bool  CheckTimer2000();
	float m_Timer2000;

	bool  CheckTimer1000();
	float m_Timer1000;

	bool  CheckTimer400();
	float m_Timer400;

	bool  CheckTimer100();
	float m_Timer100;
	

//--------------------------------------------------------------
private:
	LARGE_INTEGER		m_StartCount[eRT_NUMBER_TYPE];
	LARGE_INTEGER		m_LastCount[eRT_NUMBER_TYPE];
	LARGE_INTEGER		m_ElaspedCount[eRT_NUMBER_TYPE];
	LARGE_INTEGER		m_ElaspedCountAccum[eRT_NUMBER_TYPE];
	int					m_AccumNumber[eRT_NUMBER_TYPE];
	LARGE_INTEGER		m_Frequency[eRT_NUMBER_TYPE];

	double				m_StartTime[eRT_NUMBER_TYPE];
	double				m_LastTime[eRT_NUMBER_TYPE];
	double				m_ElaspedTime[eRT_NUMBER_TYPE];			// 기록�?시간
	double				m_ElaspedTime_Accum[eRT_NUMBER_TYPE];	// 기록�?누적 시간
	double				m_MinimizeTime[eRT_NUMBER_TYPE];		// 기록�?최소시간
	double				m_MaximizeTime[eRT_NUMBER_TYPE];		// 기록�?최대시간.

public:
	void		RecordTime_Start(eRecordType eType);			// 시간 기록 시작
	void		RecordTime_End(eRecordType eType);				// 시간 기록 �?
	double		GetRecordTime(eRecordType eType);				// 기록�?시간 제공(ms)
	double		GetRecordTime_Sec(eRecordType eType);			// 기록�?시간 제공(Sec)
	float		GetDeltaTime_Sec();								// �?Loop�?걸린시간 - �?함수�?간략버전
	float		GetDeltaTime();									// �?Loop�?걸린시간 - ms
	double		GetRecordTime_Accum(eRecordType eType);			// 누적 시간
	double		GetRecordTime_Min(eRecordType eType);
	double		GetRecordTime_Max(eRecordType eType);
	void		ResetAccumulation(eRecordType eType);			// 변화량�?들쭉날쭉한경우가 있을�?있으므�?누적량을 리셋해줄 필요�?있다
	void		DrawStatus();									// 각종 데이터를 그린�?
private:
	LARGE_INTEGER		m_StartTime_g_Curtime;
	LARGE_INTEGER		m_NowTime_g_Curtime;
	LARGE_INTEGER		m_Frequency_g_Curtime;

	void	SetStartTime_g_Curtime();
	DWORD	GetCurrentTime_g_Curtime();	// DWORD로서 프로그램�?시작�?이후 �?ms)

	DWORD t_Test;
};
EXTERNGLOBALTON(CMHTimeManager)