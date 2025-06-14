#pragma once
#define AUTOPATH USINGTON(AutoFindPathManager)
class  AutoFindPathManager
{	
	BOOL IsFind;	
	VECTOR3 * vDestPos;
	VECTOR3 vPos;
public:
	AutoFindPathManager();
	virtual  ~AutoFindPathManager();
	void Process();

	//void SetFindPath(BOOL bFlag){IsFind=bFlag;}
	//void SetTargetPos(VECTOR3 * Pos, BOOL bFlag = FALSE){ vDestPos = Pos; IsFind = bFlag; }
	
	VECTOR3 GetFlagDestination() const { return vPos; }
	void SetTargetPos(VECTOR3 val, BOOL bFlag = FALSE) { vPos = val; IsFind = bFlag; }

	BOOL IsRunning(){ return IsFind; }
};
EXTERNGLOBALTON(AutoFindPathManager)