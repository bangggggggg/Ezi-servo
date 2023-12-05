//===========================================================================
//	File Name	: CommInterface.c
//	Description : Plus-R Command Functions.
//===========================================================================

//===========================================================================
//	Include Files
//===========================================================================

#include "ezi_driver/CommInterface.h"

#include "ezi_driver/COMM_Define.h"
#include "ezi_driver/MOTION_FRAME_DEFINE.h"
#include "ezi_driver/ReturnCodes_Define.h"

#include "ezi_driver/MemSet.h"
#include "ezi_driver/MinMax.h"

//===========================================================================
//	Local Definitions
//===========================================================================

//===========================================================================
//	Local Structures
//===========================================================================

typedef union
{
	char			chValue[4];
	unsigned char	byValue[4];
	long			lValue;
	unsigned long	dwValue;
} UNION_4DATA;

typedef union
{
	char			chValue[2];
	unsigned char	byValue[2];
	short			shValue;
	unsigned short	wValue;
} UNION_2DATA;

//===========================================================================
//	External Functions.
//===========================================================================

//------------------------------------------------------------------------------
//					No-Motion Command Functions
//------------------------------------------------------------------------------
int FAS_GetSlaveInfo(unsigned char iSlaveNo, unsigned char* pType, char* lpBuff, int nBuffSize)
{
	char chBuff[256] = "";
	int nRtn;

	nBuffSize = min(nBuffSize, MAX_SWINFO_LENGTH);

	nRtn = DoSendCommand(iSlaveNo, FRAME_GETSLAVEINFO, 0, 0, (unsigned char*)chBuff, 1 + nBuffSize, COMM_WAITTIME, 0);
	if (nRtn == FMM_OK)
	{
		*pType = chBuff[0];
		memcpy(lpBuff, &(chBuff[1]), nBuffSize);
	}

	return nRtn;
}

int FAS_GetMotorInfo(unsigned char iSlaveNo, unsigned char* pType, char* lpBuff, int nBuffSize)
{
	char chBuff[256] = "";
	int nRtn;

	nBuffSize = min(nBuffSize, MAX_SWINFO_LENGTH);

	nRtn = DoSendCommand(iSlaveNo, FRAME_GETMOTORINFO, 0, 0, (unsigned char*)chBuff, 1 + nBuffSize, COMM_WAITTIME, 0);
	if (nRtn == FMM_OK)
	{
		*pType = chBuff[0];
		memcpy(lpBuff, &(chBuff[1]), nBuffSize);
	}

	return nRtn;
}

int FAS_GetFirmwareInfo(unsigned char iSlaveNo, unsigned char* pType)
{
	unsigned char byType = 0;
	int nRtn;

	nRtn = DoSendCommand(iSlaveNo, FRAME_GETFIRMWAREINFO, 0, 0, &byType, 1, COMM_WAITTIME, 1);
	if (nRtn == FMM_OK)
		*pType = byType;

	return nRtn;
}

int FAS_Ack(unsigned char iSlaveNo, unsigned short dwWaitTime)
{
	long lValue;
	int nRtn;

	nRtn = DoSendCommand(iSlaveNo, FRAME_FAS_GETCMDPOS, 0, 0, (unsigned char*)&lValue, 4, dwWaitTime, 1);

	return nRtn;
}

//------------------------------------------------------------------------------
//             Parameter Functions
//------------------------------------------------------------------------------
int FAS_SaveAllParameter(unsigned char iSlaveNo)
{
	return DoSendCommand(iSlaveNo, FRAME_FAS_SAVEALLPARAM, 0, 0, 0, 0, COMM_WAITTIME * 5, 1);
}

int FAS_SetParameter(unsigned char iSlaveNo, unsigned char iParaNo, long lParaValue)
{
	unsigned char byValue[5];
	UNION_4DATA data;

	byValue[0] = iParaNo;
	data.lValue = lParaValue;
	byValue[1] = data.byValue[0];
	byValue[2] = data.byValue[1];
	byValue[3] = data.byValue[2];
	byValue[4] = data.byValue[3];

	return DoSendCommand(iSlaveNo, FRAME_FAS_SETPARAMETER, byValue, 5, 0, 0, COMM_WAITTIME, 0);
}

int FAS_GetParameter(unsigned char iSlaveNo, unsigned char iParaNo, long* lParamValue)
{
	long lValue = 0;
	int nRtn;

	nRtn = DoSendCommand(iSlaveNo, FRAME_FAS_GETPARAMETER, &iParaNo, 1, (unsigned char*)&lValue, 4, COMM_WAITTIME, 0);

	if (nRtn == FMM_OK)
		*lParamValue = lValue;

	return nRtn;
}

int FAS_GetROMParameter(unsigned char iSlaveNo, unsigned char iParaNo, long* lROMParam)
{
	long lValue = 0;
	int nRtn;

	nRtn = DoSendCommand(iSlaveNo, FRAME_FAS_GETROMPARAM, &iParaNo, 1, (unsigned char*)&lValue, 4, COMM_WAITTIME * 5, 0);

	if (nRtn == FMM_OK)
		*lROMParam = lValue;

	return nRtn;
}

//------------------------------------------------------------------------------
//            IO Functions
//------------------------------------------------------------------------------
int FAS_SetIOInput(unsigned char iSlaveNo, unsigned long dwIOSETMask, unsigned long dwIOCLRMask)
{
	unsigned char byValue[8];
	UNION_4DATA data;

	data.dwValue = dwIOSETMask;
	byValue[0] = data.byValue[0];
	byValue[1] = data.byValue[1];
	byValue[2] = data.byValue[2];
	byValue[3] = data.byValue[3];

	data.dwValue = dwIOCLRMask;
	byValue[4] = data.byValue[0];
	byValue[5] = data.byValue[1];
	byValue[6] = data.byValue[2];
	byValue[7] = data.byValue[3];

	return DoSendCommand(iSlaveNo, FRAME_FAS_SETIO_INPUT, byValue, 8, 0, 0, COMM_WAITTIME, 0);
}

int FAS_GetIOInput(unsigned char iSlaveNo, unsigned long* dwIOInput)
{
	unsigned long dwValue;
	int nRtn;

	nRtn = DoSendCommand(iSlaveNo, FRAME_FAS_GETIO_INPUT, 0, 0, (unsigned char*)&dwValue, 4, COMM_WAITTIME, 0);

	if (nRtn == FMM_OK)
		*dwIOInput = dwValue;

	return nRtn;
}

int FAS_SetIOOutput(unsigned char iSlaveNo, unsigned long dwIOSETMask, unsigned long dwIOCLRMask)
{
	unsigned char byValue[8];
	UNION_4DATA data;

	data.dwValue = dwIOSETMask;
	byValue[0] = data.byValue[0];
	byValue[1] = data.byValue[1];
	byValue[2] = data.byValue[2];
	byValue[3] = data.byValue[3];

	data.dwValue = dwIOCLRMask;
	byValue[4] = data.byValue[0];
	byValue[5] = data.byValue[1];
	byValue[6] = data.byValue[2];
	byValue[7] = data.byValue[3];

	return DoSendCommand(iSlaveNo, FRAME_FAS_SETIO_OUTPUT, byValue, 8, 0, 0, COMM_WAITTIME, 0);
}

int FAS_GetIOOutput(unsigned char iSlaveNo, unsigned long* dwIOOutput)
{
	unsigned long dwValue;
	int nRtn;

	nRtn = DoSendCommand(iSlaveNo, FRAME_FAS_GETIO_OUTPUT, 0, 0, (unsigned char*)&dwValue, 4, COMM_WAITTIME, 0);

	if (nRtn == FMM_OK)
		*dwIOOutput = dwValue;

	return nRtn;
}

int FAS_GetIOAssignMap(unsigned char iSlaveNo, unsigned char iIOPinNo, unsigned long* dwIOLogicMask, unsigned char* bLevel)
{
	unsigned char byValue[5];
	UNION_4DATA data;
	int nRtn;

	nRtn = DoSendCommand(iSlaveNo, FRAME_FAS_GET_IO_ASSGN_MAP, &iIOPinNo, 1, byValue, 5, COMM_WAITTIME, 0);

	if (nRtn == FMM_OK)
	{
		data.byValue[0] = byValue[0];
		data.byValue[1] = byValue[1];
		data.byValue[2] = byValue[2];
		data.byValue[3] = byValue[3];
		*dwIOLogicMask = data.dwValue;

		*bLevel = (byValue[4]) ? 0x01 : 0x00;
	}

	return nRtn;
}

int FAS_SetIOAssignMap(unsigned char iSlaveNo, unsigned char iIOPinNo, unsigned long dwIOLogicMask, unsigned char bLevel)
{
	unsigned char byValue[6];
	UNION_4DATA data;

	byValue[0] = iIOPinNo;

	data.dwValue = dwIOLogicMask;
	byValue[1] = data.byValue[0];
	byValue[2] = data.byValue[1];
	byValue[3] = data.byValue[2];
	byValue[4] = data.byValue[3];
	byValue[5] = (bLevel) ? 0x01 : 0x00;

	return DoSendCommand(iSlaveNo, FRAME_FAS_SET_IO_ASSGN_MAP, byValue, 6, 0, 0, COMM_WAITTIME, 0);
}

int FAS_IOAssignMapReadROM(unsigned char iSlaveNo)
{
	unsigned char nRtnCode;
	int nRtn;

	nRtn = DoSendCommand(iSlaveNo, FRAME_FAS_IO_ASSGN_MAP_READROM, 0, 0, &nRtnCode, 1, COMM_WAITTIME * 5, 0);

	if (nRtnCode != 0)
	{
	}

	return nRtn;
}

//------------------------------------------------------------------------------
//             Servo Driver Control Functions
//------------------------------------------------------------------------------
int FAS_ServoEnable(unsigned char iSlaveNo, int bOnOff)
{
	unsigned char byValue = (unsigned char)(bOnOff) ? 0x01 : 0x00;

	return DoSendCommand(iSlaveNo, FRAME_FAS_SERVOENABLE, &byValue, 1, 0, 0, COMM_WAITTIME * 5, 0);
}

int FAS_ServoAlarmReset(unsigned char iSlaveNo)
{
	return DoSendCommand(iSlaveNo, FRAME_FAS_ALARMRESET, 0, 0, 0, 0, COMM_WAITTIME, 0);
}

int FAS_StepAlarmReset(unsigned char iSlaveNo, int bReset)
{
	unsigned char byValue = (unsigned char)(bReset) ? 0x01 : 0x00;

	return DoSendCommand(iSlaveNo, FRAME_FAS_STEPALARMRESET, &byValue, 1, 0, 0, COMM_WAITTIME, 0);
}


//------------------------------------------------------------------------------
//            Read Status and Position
//------------------------------------------------------------------------------
int FAS_GetAxisStatus(unsigned char iSlaveNo, unsigned long* dwAxisStatus)
{
	unsigned long dwValue;
	int nRtn;

	nRtn = DoSendCommand(iSlaveNo, FRAME_FAS_GETAXISSTATUS, 0, 0, (unsigned char*)&dwValue, 4, COMM_WAITTIME, 0);

	if (nRtn == FMM_OK)
		*dwAxisStatus = dwValue;

	return nRtn;
}

int FAS_GetIOAxisStatus(unsigned char iSlaveNo, unsigned long* dwInStatus, unsigned long* dwOutStatus, unsigned long* dwAxisStatus)
{
	int nRtn;
	unsigned long Data[3];

	nRtn = DoSendCommand(iSlaveNo, FRAME_FAS_GETIOAXISSTATUS, 0, 0, (unsigned char*)Data, 12, COMM_WAITTIME, 0);

	if (nRtn == FMM_OK)
	{
		*dwInStatus= Data[0];
		*dwOutStatus= Data[1];
		*dwAxisStatus= Data[2];
	}

	return nRtn;
}

int FAS_GetMotionStatus(unsigned char iSlaveNo, long* lCmdPos, long* lActPos, long* lPosErr, long* lActVel, unsigned short* wPosItemNo)
{
	unsigned long Data[5];
	int nRtn;

	nRtn = DoSendCommand(iSlaveNo, FRAME_FAS_GETMOTIONSTATUS, 0, 0, (unsigned char*)Data, 20, COMM_WAITTIME, 0);

	if (nRtn == FMM_OK)
	{
		*lCmdPos= Data[0];
		*lActPos= Data[1];
		*lPosErr= Data[2];
		*lActVel= Data[3];
		*wPosItemNo= (unsigned short)Data[4];
	}

	return nRtn;
}

int FAS_GetAllStatus(unsigned char iSlaveNo, unsigned long* dwInStatus, unsigned long* dwOutStatus, unsigned long* dwAxisStatus, long* lCmdPos, long* lActPos, long* lPosErr, long* lActVel, unsigned short* wPosItemNo)
{
	unsigned long Data[8];
	int nRtn;

	nRtn = DoSendCommand(iSlaveNo, FRAME_FAS_GETALLSTATUS, 0, 0, (unsigned char*)Data, 32, COMM_WAITTIME, 0);

	if (nRtn == FMM_OK)
	{
		*dwInStatus= Data[0];
		*dwOutStatus= Data[1];
		*dwAxisStatus= Data[2];
		*lCmdPos= Data[3];
		*lActPos= Data[4];
		*lPosErr= Data[5];
		*lActVel= Data[6];
		*wPosItemNo= (unsigned short)Data[7];
	}

	return nRtn;
}

int FAS_SetCommandPos(unsigned char iSlaveNo, long lCmdPos)
{
	UNION_4DATA data;

	data.lValue = lCmdPos;

	return DoSendCommand(iSlaveNo, FRAME_FAS_SETCMDPOS, data.byValue, 4, 0, 0, COMM_WAITTIME, 0);
}

int FAS_SetActualPos(unsigned char iSlaveNo, long lActPos)
{
	UNION_4DATA data;

	data.lValue = lActPos;

	return DoSendCommand(iSlaveNo, FRAME_FAS_SETACTPOS, data.byValue, 4, 0, 0, COMM_WAITTIME, 0);
}

int FAS_ClearPosition(unsigned char iSlaveNo)
{
	return DoSendCommand(iSlaveNo, FRAME_FAS_CLEARPOS, 0, 0, 0, 0, COMM_WAITTIME * 15, 0);
}

int FAS_GetCommandPos(unsigned char iSlaveNo, long* lCmdPos)
{
	long lValue = 0;
	int nRtn;

	nRtn = DoSendCommand(iSlaveNo, FRAME_FAS_GETCMDPOS, 0, 0, (unsigned char*)&lValue, 4, COMM_WAITTIME, 0);

	if (nRtn == FMM_OK)
		*lCmdPos = lValue;

	return nRtn;
}

int FAS_GetActualPos(unsigned char iSlaveNo, long* lActPos)
{
	long lValue = 0;
	int nRtn;

	nRtn = DoSendCommand(iSlaveNo, FRAME_FAS_GETACTPOS, 0, 0, (unsigned char*)&lValue, 4, COMM_WAITTIME, 0);

	if (nRtn == FMM_OK)
		*lActPos = lValue;

	return nRtn;
}

int FAS_GetPosError(unsigned char iSlaveNo, long* lPosErr)
{
	long lValue = 0;
	int nRtn;

	nRtn = DoSendCommand(iSlaveNo, FRAME_FAS_GETPOSERR, 0, 0, (unsigned char*)&lValue, 4, COMM_WAITTIME, 0);

	if (nRtn == FMM_OK)
		*lPosErr = lValue;

	return nRtn;
}

int FAS_GetActualVel(unsigned char iSlaveNo, long* lActVel)
{
	long lValue = 0;
	int nRtn;

	nRtn = DoSendCommand(iSlaveNo, FRAME_FAS_GETACTVEL, 0, 0, (unsigned char*)&lValue, 4, COMM_WAITTIME, 0);

	if (nRtn == FMM_OK)
		*lActVel = lValue;

	return nRtn;
}

int FAS_GetAlarmType(unsigned char iSlaveNo, unsigned char* nAlarmType)
{
	unsigned char byValue;
	int nRtn;

	nRtn = DoSendCommand(iSlaveNo, FRAME_FAS_GETALARMTYPE, 0, 0, &byValue, 1, COMM_WAITTIME, 0);

	if (nRtn == FMM_OK)
	{
		if (nAlarmType != 0)
			*nAlarmType = byValue;
	}

	return nRtn;
}

//------------------------------------------------------------------
//                Motion Functions.
//------------------------------------------------------------------
int FAS_MoveStop(unsigned char iSlaveNo)
{
	int nRtn;

	nRtn = DoSendCommand(iSlaveNo, FRAME_FAS_MOVESTOP, 0, 0, 0, 0, COMM_WAITTIME, 0);

    return nRtn;
}

int FAS_EmergencyStop(unsigned char iSlaveNo)
{
	int nRtn;

	nRtn = DoSendCommand(iSlaveNo, FRAME_FAS_EMERGENCYSTOP, 0, 0, 0, 0, COMM_WAITTIME, 0);

	return nRtn;
}

int FAS_MoveOriginSingleAxis(unsigned char iSlaveNo)
{
	int nRtn;

	nRtn = DoSendCommand(iSlaveNo, FRAME_FAS_MOVEORIGIN, 0, 0, 0, 0, COMM_WAITTIME, 0);

	return nRtn;
}

int FAS_MoveSingleAxisAbsPos(unsigned char iSlaveNo, long lAbsPos, unsigned long lVelocity)
{
	unsigned char byValue[8];
	UNION_4DATA data;
	int nRtn;

	data.lValue = lAbsPos;
	byValue[0] = data.byValue[0];
	byValue[1] = data.byValue[1];
	byValue[2] = data.byValue[2];
	byValue[3] = data.byValue[3];

	data.lValue = lVelocity;
	byValue[4] = data.byValue[0];
	byValue[5] = data.byValue[1];
	byValue[6] = data.byValue[2];
	byValue[7] = data.byValue[3];

	nRtn = DoSendCommand(iSlaveNo, FRAME_FAS_MOVESINGLEABS, byValue, 8, 0, 0, COMM_WAITTIME, 0);

	return nRtn;
}

int FAS_MoveSingleAxisIncPos(unsigned char iSlaveNo, long lIncPos, unsigned long lVelocity)
{
	unsigned char byValue[8];
	UNION_4DATA data;
	int nRtn;

	data.lValue = lIncPos;
	byValue[0] = data.byValue[0];
	byValue[1] = data.byValue[1];
	byValue[2] = data.byValue[2];
	byValue[3] = data.byValue[3];

	data.lValue = lVelocity;
	byValue[4] = data.byValue[0];
	byValue[5] = data.byValue[1];
	byValue[6] = data.byValue[2];
	byValue[7] = data.byValue[3];

	nRtn = DoSendCommand(iSlaveNo, FRAME_FAS_MOVESINGLEINC, byValue, 8, 0, 0, COMM_WAITTIME, 0);

	return nRtn;
}

int FAS_MoveToLimit(unsigned char iSlaveNo, unsigned long lVelocity, int iLimitDir)
{
	unsigned char byValue[5];
	UNION_4DATA data;
	int nRtn;

	data.lValue = lVelocity;
	byValue[0] = data.byValue[0];
	byValue[1] = data.byValue[1];
	byValue[2] = data.byValue[2];
	byValue[3] = data.byValue[3];

	byValue[4] = (unsigned char)iLimitDir;

	nRtn = DoSendCommand(iSlaveNo, FRAME_FAS_MOVETOLIMIT, byValue, 5, 0, 0, COMM_WAITTIME, 0);

	return nRtn;
}

int FAS_MoveVelocity(unsigned char iSlaveNo, unsigned long lVelocity, int iVelDir)
{
	unsigned char byValue[5];
	UNION_4DATA data;

	data.lValue = lVelocity;
	byValue[0] = data.byValue[0];
	byValue[1] = data.byValue[1];
	byValue[2] = data.byValue[2];
	byValue[3] = data.byValue[3];
	byValue[4] = (unsigned char)iVelDir;

	return DoSendCommand(iSlaveNo, FRAME_FAS_MOVEVELOCITY, byValue, 5, 0, 0, COMM_WAITTIME, 0);
}

int FAS_PositionAbsOverride(unsigned char iSlaveNo, long lOverridePos)
{
	UNION_4DATA data;

	data.lValue = lOverridePos;

	return DoSendCommand(iSlaveNo, FRAME_FAS_POSABSOVERRIDE, data.byValue, 4, 0, 0, COMM_WAITTIME, 0);
}

int FAS_PositionIncOverride(unsigned char iSlaveNo, long lOverridePos)
{
	UNION_4DATA data;

	data.lValue = lOverridePos;

	return DoSendCommand(iSlaveNo, FRAME_FAS_POSINCOVERRIDE, data.byValue, 4, 0, 0, COMM_WAITTIME, 0);
}

int FAS_VelocityOverride(unsigned char iSlaveNo, unsigned long lVelocity)
{
	UNION_4DATA data;

	data.dwValue = lVelocity;

	return DoSendCommand(iSlaveNo, FRAME_FAS_VELOVERRIDE, data.byValue, 4, 0, 0, COMM_WAITTIME, 0);
}

int FAS_TriggerPulseOutput(unsigned char iSlaveNo, int bStartTrigger, long lStartPos, unsigned long dwPeriod, unsigned long dwPulseTime, unsigned char nOutputPin, unsigned long dwReserved)
{
	unsigned char byValue[18] = {0};
	UNION_4DATA data;
	int nRtn;

	byValue[0] = (bStartTrigger) ? 1 : 0;

	data.lValue = lStartPos;
	byValue[1] = data.byValue[0];
	byValue[2] = data.byValue[1];
	byValue[3] = data.byValue[2];
	byValue[4] = data.byValue[3];

	data.dwValue = dwPeriod;
	byValue[5] = data.byValue[0];
	byValue[6] = data.byValue[1];
	byValue[7] = data.byValue[2];
	byValue[8] = data.byValue[3];

	data.dwValue = dwPulseTime;
	byValue[9] = data.byValue[0];
	byValue[10] = data.byValue[1];
	byValue[11] = data.byValue[2];
	byValue[12] = data.byValue[3];

	byValue[13] = nOutputPin;

	data.dwValue = dwReserved;
	byValue[14] = data.byValue[0];
	byValue[15] = data.byValue[1];
	byValue[16] = data.byValue[2];
	byValue[17] = data.byValue[3];

	nRtn = DoSendCommand(iSlaveNo, FRAME_FAS_TRIGGER_OUTPUT, byValue, 18, 0, 0, COMM_WAITTIME, 0);

	return nRtn;
}

int FAS_TriggerPulseStatus(unsigned char iSlaveNo, unsigned char* bTriggerStatus)
{
	unsigned char byValue;
	int nRtn;

	nRtn = DoSendCommand(iSlaveNo, FRAME_FAS_TRIGGER_STATUS, 0, 0, &byValue, 1, COMM_WAITTIME, 0);

	if (nRtn == FMM_OK)
	{
		if (bTriggerStatus != 0)
			*bTriggerStatus = byValue;
	}

	return nRtn;
}

int FAS_MovePush(unsigned char iSlaveNo, unsigned long dwStartSpd, unsigned long dwMoveSpd, long lPosition, unsigned short wAccel, unsigned short wDecel, unsigned short wPushRate, unsigned long dwPushSpd, long lEndPosition)
{
	unsigned char byValue[26] = {0};
	UNION_2DATA data2;
	UNION_4DATA data4;
	int nRtn;

	data4.dwValue = dwStartSpd;
	byValue[0] = data4.byValue[0];
	byValue[1] = data4.byValue[1];
	byValue[2] = data4.byValue[2];
	byValue[3] = data4.byValue[3];

	data4.dwValue = dwMoveSpd;
	byValue[4] = data4.byValue[0];
	byValue[5] = data4.byValue[1];
	byValue[6] = data4.byValue[2];
	byValue[7] = data4.byValue[3];

	data4.lValue = lPosition;
	byValue[8] = data4.byValue[0];
	byValue[9] = data4.byValue[1];
	byValue[10] = data4.byValue[2];
	byValue[11] = data4.byValue[3];

	data2.wValue = wAccel;
	byValue[12] = data2.byValue[0];
	byValue[13] = data2.byValue[1];

	data2.wValue = wDecel;
	byValue[14] = data2.byValue[0];
	byValue[15] = data2.byValue[1];

	data2.wValue = wPushRate;
	byValue[16] = data2.byValue[0];
	byValue[17] = data2.byValue[1];

	data4.dwValue = dwPushSpd;
	byValue[18] = data4.byValue[0];
	byValue[19] = data4.byValue[1];
	byValue[20] = data4.byValue[2];
	byValue[21] = data4.byValue[3];

	data4.lValue = lEndPosition;
	byValue[22] = data4.byValue[0];
	byValue[23] = data4.byValue[1];
	byValue[24] = data4.byValue[2];
	byValue[25] = data4.byValue[3];

	nRtn = DoSendCommand(iSlaveNo, FRAME_FAS_MOVEPUSH, byValue, 26, 0, 0, COMM_WAITTIME, 0);

	return nRtn;
}

int FAS_GetPushStatus(unsigned char iSlaveNo, unsigned char* nPushStatus)
{
	unsigned char byValue;
	int nRtn;

	nRtn = DoSendCommand(iSlaveNo, FRAME_FAS_GETPUSHSTATUS, 0, 0, &byValue, 1, COMM_WAITTIME, 0);

	if (nRtn == FMM_OK)
	{
		if (nPushStatus != 0)
			*nPushStatus = byValue;
	}

	return nRtn;
}
/*
//------------------------------------------------------------------
//					Ex-Motion Functions.
//------------------------------------------------------------------
int FAS_MoveSingleAxisAbsPosEx(unsigned char iSlaveNo, long lAbsPos, unsigned long lVelocity, MOTION_OPTION_EX* lpExOption)
{
	unsigned char byValue[40];
	UNION_4DATA data;
	int nRtn;

	data.lValue = lAbsPos;
	byValue[0] = data.byValue[0];
	byValue[1] = data.byValue[1];
	byValue[2] = data.byValue[2];
	byValue[3] = data.byValue[3];

	data.lValue = lVelocity;
	byValue[4] = data.byValue[0];
	byValue[5] = data.byValue[1];
	byValue[6] = data.byValue[2];
	byValue[7] = data.byValue[3];

	memcpy(&(byValue[8]), lpExOption, 32);

	nRtn = DoSendCommand(iSlaveNo, FRAME_FAS_MOVESINGLEABS_EX, byValue, 40, 0, 0, COMM_WAITTIME, 0);

	return nRtn;
}

int FAS_MoveSingleAxisIncPosEx(unsigned char iSlaveNo, long lIncPos, unsigned long lVelocity, MOTION_OPTION_EX* lpExOption)
{
	unsigned char byValue[40];
	UNION_4DATA data;
	int nRtn;

	data.lValue = lIncPos;
	byValue[0] = data.byValue[0];
	byValue[1] = data.byValue[1];
	byValue[2] = data.byValue[2];
	byValue[3] = data.byValue[3];

	data.lValue = lVelocity;
	byValue[4] = data.byValue[0];
	byValue[5] = data.byValue[1];
	byValue[6] = data.byValue[2];
	byValue[7] = data.byValue[3];

	memcpy(&(byValue[8]), lpExOption, 32);

	nRtn = DoSendCommand(iSlaveNo, FRAME_FAS_MOVESINGLEINC_EX, byValue, 40, 0, 0, COMM_WAITTIME, 0);

	return nRtn;
}

int FAS_MoveVelocityEx(unsigned char iSlaveNo, unsigned long lVelocity, int iVelDir, VELOCITY_OPTION_EX* lpExOption)
{
	unsigned char byValue[37];
	UNION_4DATA data;

	data.lValue = lVelocity;
	byValue[0] = data.byValue[0];
	byValue[1] = data.byValue[1];
	byValue[2] = data.byValue[2];
	byValue[3] = data.byValue[3];
	byValue[4] = (unsigned char)iVelDir;

	memcpy(&(byValue[5]), lpExOption, 32);

	return DoSendCommand(iSlaveNo, FRAME_FAS_MOVEVELOCITY_EX, byValue, 37, 0, 0, COMM_WAITTIME, 0);
}
*/
//------------------------------------------------------------------
//					All-Motion Functions.
//------------------------------------------------------------------
int FAS_AllMoveStop()
{
	int nRtn;

	nRtn = DoSendCommandNoResp(SLAVE_ALL_COMMAND, FRAME_FAS_ALLMOVESTOP, 0, 0);

    return nRtn;
}

int FAS_AllEmergencyStop()
{
	int nRtn;

	nRtn = DoSendCommandNoResp(SLAVE_ALL_COMMAND, FRAME_FAS_ALLEMERGENCYSTOP, 0, 0);

    return nRtn;
}

int FAS_AllMoveOriginSingleAxis()
{
	int nRtn;

	nRtn = DoSendCommandNoResp(SLAVE_ALL_COMMAND, FRAME_FAS_ALLMOVEORIGIN, 0, 0);

	return nRtn;
}

int FAS_AllMoveSingleAxisAbsPos(long lAbsPos, unsigned long lVelocity)
{
	unsigned char byValue[8];
	UNION_4DATA data;
	int nRtn;

	data.lValue = lAbsPos;
	byValue[0] = data.byValue[0];
	byValue[1] = data.byValue[1];
	byValue[2] = data.byValue[2];
	byValue[3] = data.byValue[3];

	data.lValue = lVelocity;
	byValue[4] = data.byValue[0];
	byValue[5] = data.byValue[1];
	byValue[6] = data.byValue[2];
	byValue[7] = data.byValue[3];

	nRtn = DoSendCommandNoResp(SLAVE_ALL_COMMAND, FRAME_FAS_ALLMOVESINGLEABS, byValue, 8);

	return nRtn;
}

int FAS_AllMoveSingleAxisIncPos(long lIncPos, unsigned long lVelocity)
{
	unsigned char byValue[8];
	UNION_4DATA data;
	int nRtn;

	data.lValue = lIncPos;
	byValue[0] = data.byValue[0];
	byValue[1] = data.byValue[1];
	byValue[2] = data.byValue[2];
	byValue[3] = data.byValue[3];

	data.lValue = lVelocity;
	byValue[4] = data.byValue[0];
	byValue[5] = data.byValue[1];
	byValue[6] = data.byValue[2];
	byValue[7] = data.byValue[3];

	nRtn = DoSendCommandNoResp(SLAVE_ALL_COMMAND, FRAME_FAS_ALLMOVESINGLEINC, byValue, 8);

	return nRtn;
}

//------------------------------------------------------------------
//					Position Table Functions.
//------------------------------------------------------------------
int FAS_PosTableReadItem(unsigned char iSlaveNo, unsigned short wItemNo, LPITEM_NODE lpItem)
{
	UNION_2DATA data;

	data.wValue = wItemNo;

	return DoSendCommand(iSlaveNo, FRAME_FAS_POSTAB_READ_ITEM, data.byValue, 2, (unsigned char*)lpItem, 64, COMM_WAITTIME, 0);
}

int FAS_PosTableWriteItem(unsigned char iSlaveNo, unsigned short wItemNo, LPITEM_NODE lpItem)
{
	UNION_2DATA data;
	unsigned char byValue[66];
	unsigned char nRtnCode;
	int nRtn;

	data.wValue = wItemNo;
	byValue[0] = data.byValue[0];
	byValue[1] = data.byValue[1];
	memcpy(&(byValue[2]), lpItem, 64);

	nRtn = DoSendCommand(iSlaveNo, FRAME_FAS_POSTAB_WRITE_ITEM, byValue, 66, &nRtnCode, 1, COMM_WAITTIME, 0);

	if (nRtnCode == 0)
		return FMM_POSTABLE_ERROR;

	return nRtn;
}

int FAS_PosTableWriteROM(unsigned char iSlaveNo)
{
	unsigned char nRtnCode;
	int nRtn;

	nRtn = DoSendCommand(iSlaveNo, FRAME_FAS_POSTAB_WRITE_ROM, 0, 0, &nRtnCode, 1, COMM_WAITTIME * 20, 0);

	if (nRtnCode != 0)
		return FMM_POSTABLE_ERROR;

	return nRtn;
}

int FAS_PosTableReadROM(unsigned char iSlaveNo)
{
	unsigned char nRtnCode;
	int nRtn;

	nRtn = DoSendCommand(iSlaveNo, FRAME_FAS_POSTAB_READ_ROM, 0, 0, &nRtnCode, 1, COMM_WAITTIME * 5, 0);

	if (nRtnCode != 0)
		return FMM_POSTABLE_ERROR;

	return nRtn;
}

int FAS_PosTableRunItem(unsigned char iSlaveNo, unsigned short wItemNo)
{
	UNION_2DATA data;

	data.wValue = wItemNo;

	return DoSendCommand(iSlaveNo, FRAME_FAS_POSTAB_RUN_ITEM, data.byValue, 2, 0, 0, COMM_WAITTIME, 0);
}

int FAS_PosTableIsData(unsigned char iSlaveNo, LPPOSTABLE_DATAFLAG pBitFlag)
{
	return DoSendCommand(iSlaveNo, FRAME_FAS_POSTAB_IS_DATA, 0, 0, (unsigned char*)pBitFlag, 32, COMM_WAITTIME, 0);
}

int FAS_PosTableIsDataEx(unsigned char iSlaveNo, unsigned long dwSectionNo, LPPOSTABLE_DATAFLAG_EX pBitFlag)
{
	return DoSendCommand(iSlaveNo, FRAME_FAS_POSTAB_IS_DATA_EX, (unsigned char*)&dwSectionNo, 4, (unsigned char*)pBitFlag, 8, COMM_WAITTIME, 0);
}

int FAS_PosTableRunOneItem(unsigned char iSlaveNo, int bNextMove, unsigned short wItemNo)
{
	unsigned char byValue[3];
	UNION_2DATA data;

	byValue[0] = (bNextMove) ? 1 : 0;		// Start / Next
	data.wValue = wItemNo;
	byValue[1] = data.byValue[0];
	byValue[2] = data.byValue[1];

	return DoSendCommand(iSlaveNo, FRAME_FAS_POSTAB_RUN_ONEITEM, byValue, 3, 0, 0, COMM_WAITTIME, 0);
}

int FAS_PosTableCheckStopMode(unsigned char iSlaveNo, unsigned char* pStopMode)
{
	unsigned char byValue;
	int nRtn;

	nRtn = DoSendCommand(iSlaveNo, FRAME_FAS_POSTAB_CHECK_STOPMODE, 0, 0, &byValue, 1, COMM_WAITTIME, 0);

	if (nRtn == FMM_OK)
		*pStopMode = byValue;

	return nRtn;
}

int FAS_PosTableReadOneItem(unsigned char iSlaveNo, unsigned short wItemNo, unsigned short wOffset, long* lPosItemVal)
{
	unsigned char byValue[4];
	UNION_2DATA data;
	long lValue;
	int nRtn;

	data.wValue = wItemNo;
	byValue[0] = data.byValue[0];
	byValue[1] = data.byValue[1];
	data.wValue = wOffset;
	byValue[2] = data.byValue[0];
	byValue[3] = data.byValue[1];

	nRtn = DoSendCommand(iSlaveNo, FRAME_FAS_POSTAB_READ_ONEITEM, byValue, 4, (unsigned char*)&lValue, 4, COMM_WAITTIME, 0);

	if (nRtn == FMM_OK)
	{
		*lPosItemVal = lValue;
	}

	return nRtn;

}

int FAS_PosTableWriteOneItem(unsigned char iSlaveNo, unsigned short wItemNo, unsigned short wOffset, long lPosItemVal)
{
	unsigned char byValue[8];
	UNION_2DATA data2;
	UNION_4DATA data4;

	data2.wValue = wItemNo;
	byValue[0] = data2.byValue[0];
	byValue[1] = data2.byValue[1];
	data2.wValue = wOffset;
	byValue[2] = data2.byValue[0];
	byValue[3] = data2.byValue[1];
	data4.dwValue = lPosItemVal;
	byValue[4] = data4.byValue[0];
	byValue[5] = data4.byValue[1];
	byValue[6] = data4.byValue[2];
	byValue[7] = data4.byValue[3];

	return DoSendCommand(iSlaveNo, FRAME_FAS_POSTAB_WRITE_ONEITEM, byValue, 8, 0, 0, COMM_WAITTIME, 0);
}
