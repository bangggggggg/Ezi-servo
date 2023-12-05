#ifndef _MOTION_DEFINE_H_
#define _MOTION_DEFINE_H_
//===========================================================================
//	File Name	: MOTION_DEFINE.h
//	Description : Structures/Definitions for Plus-R Command.
//===========================================================================

//===========================================================================
//	Ezi-Motion Plus-R Definitions
//===========================================================================

//------------------------------------------------------------------
//                 POSITION TABLE Defines.
//------------------------------------------------------------------
#define	MAX_LOOP_COUNT		100
#define	MAX_WAIT_TIME		60000

#define	PUSH_RATIO_MIN		10
#define	PUSH_RATIO_MAX		100

typedef enum
{
	CMD_ABS_LOWSPEED = 0,
	CMD_ABS_HIGHSPEED,
	CMD_ABS_HIGHSPEEDDECEL,
	CMD_ABS_NORMALMOTION,
	CMD_INC_LOWSPEED,
	CMD_INC_HIGHSPEED,
	CMD_INC_HIGHSPEEDDECEL,
	CMD_INC_NORMALMOTION,
	CMD_MOVE_ORIGIN,
	CMD_COUNTERCLEAR,
	CMD_PUSH_ABSMOTION,

	CMD_MAX_COUNT,

	CMD_NO_COMMAND = 0xFFFF,
} COMMAND_LIST;

//------------------------------------------------------------------
//                 POSITION TABLE ITEM Structure.
//------------------------------------------------------------------

#ifndef	DEFINE_ITEM_NODE
#define DEFINE_ITEM_NODE

typedef struct
{
	unsigned long dwBitArray[8];	// 256 bit -> 8 DWORD
} POSTABLE_DATAFLAG, *LPPOSTABLE_DATAFLAG;

typedef struct
{
	unsigned char byBitArray[8];		// 64 bit -> 8 BYTE
} POSTABLE_DATAFLAG_EX, *LPPOSTABLE_DATAFLAG_EX;


#pragma pack(2)

typedef union
{
	unsigned short	wBuffer[32];		// 64 bytes

	struct
	{
		long			lPosition;
		
		unsigned long	dwStartSpd;
		unsigned long	dwMoveSpd;
		
		unsigned short	wAccelRate;
		unsigned short	wDecelRate;
		
		unsigned short	wCommand;
		unsigned short	wWaitTime;
		unsigned short	wContinuous;
		unsigned short	wBranch;
		
		unsigned short	wCond_branch0;
		unsigned short	wCond_branch1;
		unsigned short	wCond_branch2;
		unsigned short	wLoopCount;
		unsigned short	wBranchAfterLoop;
		unsigned short	wPTSet;
		unsigned short	wLoopCountCLR;

		unsigned short	bCheckInpos;		// 0 : Check Inpos, 1 : Don't Check.

		long			lTriggerPos;
		unsigned short	wTriggerOnTime;

		unsigned short	wPushRatio;
		unsigned long	dwPushSpeed;
		long			lPushPosition;
	};
}ITEM_NODE, *LPITEM_NODE;

#pragma pack()

#endif

//------------------------------------------------------------------
//                 POSITION TABLE Offset.
//------------------------------------------------------------------

#define	OFFSET_POSITION			0
#define	OFFSET_LOWSPEED			4
#define	OFFSET_HIGHSPEED		8
#define	OFFSET_ACCELTIME		12
#define	OFFSET_DECELTIME		14
#define	OFFSET_COMMAND			16
#define	OFFSET_WAITTIME			18
#define	OFFSET_CONTINUOUS		20
#define	OFFSET_JUMPTABLENO		22
#define	OFFSET_JUMPPT0			24
#define	OFFSET_JUMPPT1			26
#define	OFFSET_JUMPPT2			28
#define	OFFSET_LOOPCOUNT		30
#define	OFFSET_LOOPJUMPTABLENO	32
#define	OFFSET_PTSET			34
#define	OFFSET_LOOPCOUNTCLEAR	36
#define	OFFSET_CHECKINPOSITION	38
#define	OFFSET_TRIGGERPOSITION	40
#define	OFFSET_TRIGGERONTIME	44
#define	OFFSET_PUSHRATIO		46
#define	OFFSET_PUSHSPEED		48
#define	OFFSET_PUSHPOSITION		52

#define	OFFSET_BLANK			56

#endif	// _MOTION_DEFINE_H_
