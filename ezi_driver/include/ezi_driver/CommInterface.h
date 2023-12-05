#ifndef _COMMINTERFACE_H_
#define _COMMINTERFACE_H_
//===========================================================================
//	File Name	: CommInterface.h
//	Description : Plus-R Command Functions.
//===========================================================================

//===========================================================================
//	Include Files
//===========================================================================

#include "MOTION_DEFINE.h"
#include "Comm.h"

//===========================================================================
//	Plus-R Command Functions
//===========================================================================

//------------------------------------------------------------------------------
//					No-Motion Command Functions
//------------------------------------------------------------------------------
int FAS_GetSlaveInfo(unsigned char iSlaveNo, unsigned char* pType, char* lpBuff, int nBuffSize);
int FAS_GetMotorInfo(unsigned char iSlaveNo, unsigned char* pType, char* lpBuff, int nBuffSize);
int FAS_GetFirmwareInfo(unsigned char iSlaveNo, unsigned char* pType);
int FAS_Ack(unsigned char iSlaveNo, unsigned short dwWaitTime);

//------------------------------------------------------------------------------
//					Parameter Functions
//------------------------------------------------------------------------------
int FAS_SaveAllParameter(unsigned char iSlaveNo);
int FAS_SetParameter(unsigned char iSlaveNo, unsigned char iParaNo, long lParaValue);
int FAS_GetParameter(unsigned char iSlaveNo, unsigned char iParaNo, long* lParamValue);
int FAS_GetROMParameter(unsigned char iSlaveNo, unsigned char iParaNo, long* lROMParam);

//------------------------------------------------------------------------------
//					IO Functions
//------------------------------------------------------------------------------
int FAS_SetIOInput(unsigned char iSlaveNo, unsigned long dwIOSETMask, unsigned long dwIOCLRMask);
int FAS_GetIOInput(unsigned char iSlaveNo, unsigned long* dwIOInput);

int FAS_SetIOOutput(unsigned char iSlaveNo, unsigned long dwIOSETMask, unsigned long dwIOCLRMask);
int FAS_GetIOOutput(unsigned char iSlaveNo, unsigned long* dwIOOutput);

int FAS_GetIOAssignMap(unsigned char iSlaveNo, unsigned char iIOPinNo, unsigned long* dwIOLogicMask, unsigned char* bLevel);
int FAS_SetIOAssignMap(unsigned char iSlaveNo, unsigned char iIOPinNo, unsigned long dwIOLogicMask, unsigned char bLevel);

int FAS_IOAssignMapReadROM(unsigned char iSlaveNo);

//------------------------------------------------------------------------------
//					Servo Driver Control Functions
//------------------------------------------------------------------------------		
int FAS_ServoEnable(unsigned char iSlaveNo, int bOnOff);
int FAS_ServoAlarmReset(unsigned char iSlaveNo);
int FAS_StepAlarmReset(unsigned char iSlaveNo, int bReset);

//------------------------------------------------------------------------------
//					Read Status and Position
//------------------------------------------------------------------------------
int FAS_GetAxisStatus(unsigned char iSlaveNo, unsigned long* dwAxisStatus);
int FAS_GetIOAxisStatus(unsigned char iSlaveNo, unsigned long* dwInStatus, unsigned long* dwOutStatus, unsigned long* dwAxisStatus);
int FAS_GetMotionStatus(unsigned char iSlaveNo, long* lCmdPos, long* lActPos, long* lPosErr, long* lActVel, unsigned short* wPosItemNo); /*Question : lPosItemNo는 unsigned short가 아닌가??*/
int FAS_GetAllStatus(unsigned char iSlaveNo, unsigned long* dwInStatus, unsigned long* dwOutStatus, unsigned long* dwAxisStatus, long* lCmdPos, long* lActPos, long* lPosErr, long* lActVel, unsigned short* wPosItemNo);

int FAS_SetCommandPos(unsigned char iSlaveNo, long lCmdPos);
int FAS_SetActualPos(unsigned char iSlaveNo, long lActPos);
int FAS_ClearPosition(unsigned char iSlaveNo);
int FAS_GetCommandPos(unsigned char iSlaveNo, long* lCmdPos);
int FAS_GetActualPos(unsigned char iSlaveNo, long* lActPos);
int FAS_GetPosError(unsigned char iSlaveNo, long* lPosErr);
int FAS_GetActualVel(unsigned char iSlaveNo, long* lActVel);

int FAS_GetAlarmType(unsigned char iSlaveNo, unsigned char* nAlarmType);

//------------------------------------------------------------------
//					Motion Functions.
//------------------------------------------------------------------
int FAS_MoveStop(unsigned char iSlaveNo);
int FAS_EmergencyStop(unsigned char iSlaveNo);

int FAS_MoveOriginSingleAxis(unsigned char iSlaveNo);
int FAS_MoveSingleAxisAbsPos(unsigned char iSlaveNo, long lAbsPos, unsigned long lVelocity);
int FAS_MoveSingleAxisIncPos(unsigned char iSlaveNo, long lIncPos, unsigned long lVelocity);
int FAS_MoveToLimit(unsigned char iSlaveNo, unsigned long lVelocity, int iLimitDir);
int FAS_MoveVelocity(unsigned char iSlaveNo, unsigned long lVelocity, int iVelDir);

int FAS_PositionAbsOverride(unsigned char iSlaveNo, long lOverridePos);
int FAS_PositionIncOverride(unsigned char iSlaveNo, long lOverridePos);
int FAS_VelocityOverride(unsigned char iSlaveNo, unsigned long lVelocity);

int FAS_TriggerPulseOutput(unsigned char iSlaveNo, int bStartTrigger, long lStartPos, unsigned long dwPeriod, unsigned long dwPulseTime, unsigned char nOutputPin, unsigned long dwReserved);
int FAS_TriggerPulseStatus(unsigned char iSlaveNo, unsigned char* bTriggerStatus);

int FAS_MovePush(unsigned char iSlaveNo, unsigned long dwStartSpd, unsigned long dwMoveSpd, long lPosition, unsigned short wAccel, unsigned short wDecel, unsigned short wPushRate, unsigned long dwPushSpd, long lEndPosition);
int FAS_GetPushStatus(unsigned char iSlaveNo, unsigned char* nPushStatus);

//------------------------------------------------------------------
//					Ex-Motion Functions.
//------------------------------------------------------------------
//int FAS_MoveSingleAxisAbsPosEx(unsigned char iSlaveNo, long lAbsPos, unsigned long lVelocity, MOTION_OPTION_EX* lpExOption);
//int FAS_MoveSingleAxisIncPosEx(unsigned char iSlaveNo, long lIncPos, unsigned long lVelocity, MOTION_OPTION_EX* lpExOption);
//int FAS_MoveVelocityEx(unsigned char iSlaveNo, unsigned long lVelocity, int iVelDir, VELOCITY_OPTION_EX* lpExOption);

//------------------------------------------------------------------
//					All-Motion Functions.
//------------------------------------------------------------------
int FAS_AllMoveStop();
int FAS_AllEmergencyStop();
int FAS_AllMoveOriginSingleAxis();
int FAS_AllMoveSingleAxisAbsPos(long lAbsPos, unsigned long lVelocity);
int FAS_AllMoveSingleAxisIncPos(long lIncPos, unsigned long lVelocity);

//------------------------------------------------------------------
//					Position Table Functions.
//------------------------------------------------------------------
int FAS_PosTableReadItem(unsigned char iSlaveNo, unsigned short wItemNo, LPITEM_NODE lpItem);
int FAS_PosTableWriteItem(unsigned char iSlaveNo, unsigned short wItemNo, LPITEM_NODE lpItem);
int FAS_PosTableWriteROM(unsigned char iSlaveNo);
int FAS_PosTableReadROM(unsigned char iSlaveNo);
int FAS_PosTableRunItem(unsigned char iSlaveNo, unsigned short wItemNo);
int FAS_PosTableIsData(unsigned char iSlaveNo, LPPOSTABLE_DATAFLAG pBitFlag);
int FAS_PosTableIsDataEx(unsigned char iSlaveNo, unsigned long dwSectionNo, LPPOSTABLE_DATAFLAG_EX pBitFlag);

int FAS_PosTableRunOneItem(unsigned char iSlaveNo, int bNextMove, unsigned short wItemNo);
int FAS_PosTableCheckStopMode(unsigned char iSlaveNo, unsigned char* pStopMode);

int FAS_PosTableReadOneItem(unsigned char iSlaveNo, unsigned short wItemNo, unsigned short wOffset, long* lPosItemVal);
int FAS_PosTableWriteOneItem(unsigned char iSlaveNo, unsigned short wItemNo, unsigned short wOffset, long lPosItemVal);

#endif	// _COMMINTERFACE_H_