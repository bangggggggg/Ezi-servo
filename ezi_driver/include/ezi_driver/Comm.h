#ifndef _COMM_H_
#define _COMM_H_
//===========================================================================
//	File Name	: Comm.h
//	Description : Sending/Receiving Protocol Functions
//===========================================================================

//===========================================================================
//	Include Files
//===========================================================================

// Serial Communication Functions
#include "COMM_SETTING.h"

//===========================================================================
//	Communication Functions
//===========================================================================

//------------------------------------------------------------------
//                 Timer Function which have to call in INT.
//------------------------------------------------------------------
void CommTimer_Tick();

//------------------------------------------------------------------
//                 Timer Functions which CommInterface use.
//------------------------------------------------------------------
void CommTimer_Reset(unsigned int nTime);
int  CommTimer_IsExpired();

//------------------------------------------------------------------
//                 Protocol Functions which CommInterface use.
//------------------------------------------------------------------
int  FuncCommand(unsigned char iSlaveNo, unsigned char byCmd, unsigned char* lpIN, unsigned short dwINlen, unsigned char* lpOUT, unsigned short dwOUTlen, unsigned short dwWaitTime, int bTimeOutReaction);
int  FuncCommandNoResp(unsigned char iSlaveNo, unsigned char byCmd, unsigned char* lpIN, unsigned short dwINlen);

#define	DoSendCommand		FuncCommand
#define	DoSendCommandNoResp	FuncCommandNoResp

#endif	// _COMM_H_
