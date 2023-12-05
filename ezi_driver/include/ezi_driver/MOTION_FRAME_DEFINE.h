#ifndef _MOTION_FRAME_DEFINE_H_
#define _MOTION_FRAME_DEFINE_H_
//===========================================================================
//	File Name	: MOTION_FRAME_DEFINE.h
//	Description : Command Type Definitions.
//===========================================================================

//------------------------------------------------------------------
//                 Pre-defined Slave No.
//------------------------------------------------------------------

#define SLAVE_ALL_COMMAND					99

//------------------------------------------------------------------
//                 FRAME TYPE Defines.
//------------------------------------------------------------------
#define FRAME_GETSLAVEINFO					0x01
#define FRAME_GETMOTORINFO					0x05
#define FRAME_GETENCODER					0x06

#define FRAME_GETFIRMWAREINFO				0x07

#define FRAME_FAS_SAVEALLPARAM				0x10
#define FRAME_FAS_GETROMPARAM				0x11
#define FRAME_FAS_SETPARAMETER				0x12
#define FRAME_FAS_GETPARAMETER				0x13

#define FRAME_FAS_SETIO_OUTPUT				0x20
#define FRAME_FAS_SETIO_INPUT				0x21
#define FRAME_FAS_GETIO_INPUT				0x22
#define FRAME_FAS_GETIO_OUTPUT				0x23

#define FRAME_FAS_SET_IO_ASSGN_MAP			0x24
#define FRAME_FAS_GET_IO_ASSGN_MAP			0x25
#define FRAME_FAS_IO_ASSGN_MAP_READROM		0x26

#define FRAME_FAS_TRIGGER_OUTPUT			0x27
#define FRAME_FAS_TRIGGER_STATUS			0x28

#define FRAME_FAS_SERVOENABLE				0x2A
#define FRAME_FAS_ALARMRESET				0x2B
#define FRAME_FAS_STEPALARMRESET			0x2C	// Step Alarm Reset function.

#define FRAME_FAS_GETALARMTYPE				0x2E	// Alarm Type

#define FRAME_FAS_GETAXISSTATUS				0x40
#define FRAME_FAS_GETIOAXISSTATUS			0x41
#define FRAME_FAS_GETMOTIONSTATUS			0x42
#define FRAME_FAS_GETALLSTATUS				0x43

#define FRAME_FAS_SETCMDPOS					0x50
#define FRAME_FAS_GETCMDPOS					0x51
#define FRAME_FAS_SETACTPOS					0x52
#define FRAME_FAS_GETACTPOS					0x53
#define FRAME_FAS_GETPOSERR					0x54
#define FRAME_FAS_GETACTVEL					0x55
#define FRAME_FAS_CLEARPOS					0x56

#define FRAME_FAS_MOVESTOP					0x31
#define FRAME_FAS_EMERGENCYSTOP				0x32

#define FRAME_FAS_MOVEORIGIN				0x33
#define FRAME_FAS_MOVESINGLEABS				0x34
#define FRAME_FAS_MOVESINGLEINC				0x35
#define FRAME_FAS_MOVETOLIMIT				0x36
#define FRAME_FAS_MOVEVELOCITY				0x37

#define FRAME_FAS_POSABSOVERRIDE			0x38
#define FRAME_FAS_POSINCOVERRIDE			0x39
#define FRAME_FAS_VELOVERRIDE				0x3A

#define FRAME_FAS_ALLMOVESTOP				0x3B
#define FRAME_FAS_ALLEMERGENCYSTOP			0x3C
#define FRAME_FAS_ALLMOVEORIGIN				0x3D
#define FRAME_FAS_ALLMOVESINGLEABS			0x3E
#define FRAME_FAS_ALLMOVESINGLEINC			0x3F

// Position Table specific commands.
#define FRAME_FAS_POSTAB_READ_ITEM			0x60
#define FRAME_FAS_POSTAB_WRITE_ITEM			0x61
#define FRAME_FAS_POSTAB_READ_ROM			0x62
#define FRAME_FAS_POSTAB_WRITE_ROM			0x63
#define FRAME_FAS_POSTAB_RUN_ITEM			0x64
#define FRAME_FAS_POSTAB_IS_DATA			0x65

#define FRAME_FAS_POSTAB_IS_DATA_EX			0x6C

#define FRAME_FAS_POSTAB_RUN_ONEITEM		0x68
#define FRAME_FAS_POSTAB_CHECK_STOPMODE		0x69

#define FRAME_FAS_POSTAB_READ_ONEITEM		0x6A
#define FRAME_FAS_POSTAB_WRITE_ONEITEM		0x6B
#define FRAME_FAS_POSTAB_WRITE_MULTIITEMS	0x6D

// Linear Motion commands.
#define FRAME_FAS_SETLINEARINFO				0x70
#define FRAME_FAS_MOVELINEARINC				0x71

// Push Motion commands.
#define FRAME_FAS_MOVEPUSH					0x78
#define FRAME_FAS_GETPUSHSTATUS				0x79

// Ex-Motion Commands
#define FRAME_FAS_MOVESINGLEABS_EX			0x80
#define FRAME_FAS_MOVESINGLEINC_EX			0x81
#define FRAME_FAS_MOVEVELOCITY_EX			0x82

// Controlling GAP Commands
#define FRAME_FAS_GAPCONTROL_ENABLE			0x94
#define FRAME_FAS_GAPCONTROL_DISABLE		0x95
#define FRAME_FAS_GAPCONTROL_ISENABLE		0x96

#define FRAME_FAS_GAPCONTROL_GETADCVALUE	0x97
#define FRAME_FAS_GAPONERESULT_MONITOR		0x98

//------------------------------------------------------------------
//                 FRAME TYPE Defines.
//------------------------------------------------------------------

#define IS_FUNCTION_RETRIABLE(cmd)\
	((FRAME_GETSLAVEINFO == cmd) ||\
	(FRAME_GETMOTORINFO == cmd) ||\
	(FRAME_GETENCODER == cmd) ||\
	(FRAME_GETFIRMWAREINFO == cmd) ||\
	/*(FRAME_FAS_SAVEALLPARAM == cmd) ||*/\
	(FRAME_FAS_GETROMPARAM == cmd) ||\
	/*(FRAME_FAS_SETPARAMETER == cmd) ||*/\
	(FRAME_FAS_GETPARAMETER == cmd) ||\
	(FRAME_FAS_SETIO_OUTPUT == cmd) ||\
	(FRAME_FAS_SETIO_INPUT == cmd) ||\
	(FRAME_FAS_GETIO_INPUT == cmd) ||\
	(FRAME_FAS_GETIO_OUTPUT == cmd) ||\
	(FRAME_FAS_SET_IO_ASSGN_MAP == cmd) ||\
	(FRAME_FAS_GET_IO_ASSGN_MAP == cmd) ||\
	(FRAME_FAS_IO_ASSGN_MAP_READROM == cmd) ||\
	(FRAME_FAS_TRIGGER_OUTPUT == cmd) ||\
	(FRAME_FAS_TRIGGER_STATUS == cmd) ||\
	(FRAME_FAS_SERVOENABLE == cmd) ||\
	(FRAME_FAS_ALARMRESET == cmd) ||\
	(FRAME_FAS_STEPALARMRESET == cmd) ||\
	(FRAME_FAS_GETALARMTYPE == cmd) ||\
	(FRAME_FAS_GETAXISSTATUS == cmd) ||\
	(FRAME_FAS_GETIOAXISSTATUS == cmd) ||\
	(FRAME_FAS_GETMOTIONSTATUS == cmd) ||\
	(FRAME_FAS_GETALLSTATUS == cmd) ||\
	(FRAME_FAS_SETCMDPOS == cmd) ||\
	(FRAME_FAS_GETCMDPOS == cmd) ||\
	(FRAME_FAS_SETACTPOS == cmd) ||\
	(FRAME_FAS_GETACTPOS == cmd) ||\
	(FRAME_FAS_GETPOSERR == cmd) ||\
	(FRAME_FAS_GETACTVEL == cmd) ||\
	(FRAME_FAS_CLEARPOS == cmd) ||\
	(FRAME_FAS_MOVESTOP == cmd) ||\
	(FRAME_FAS_EMERGENCYSTOP == cmd) ||\
	/*(FRAME_FAS_MOVEORIGIN == cmd) ||*/\
	/*(FRAME_FAS_MOVESINGLEABS == cmd) ||*/\
	/*(FRAME_FAS_MOVESINGLEINC == cmd) ||*/\
	/*(FRAME_FAS_MOVETOLIMIT == cmd) ||*/\
	/*(FRAME_FAS_MOVEVELOCITY == cmd) ||*/\
	/*(FRAME_FAS_POSABSOVERRIDE == cmd) ||*/\
	/*(FRAME_FAS_POSINCOVERRIDE == cmd) ||*/\
	/*(FRAME_FAS_VELOVERRIDE == cmd) ||*/\
	/*(FRAME_FAS_ALLMOVESTOP == cmd) ||*/\
	/*(FRAME_FAS_ALLEMERGENCYSTOP == cmd) ||*/\
	/*(FRAME_FAS_ALLMOVEORIGIN == cmd) ||*/\
	/*(FRAME_FAS_ALLMOVESINGLEABS == cmd) ||*/\
	/*(FRAME_FAS_ALLMOVESINGLEINC == cmd) ||*/\
	(FRAME_FAS_POSTAB_READ_ITEM == cmd) ||\
	(FRAME_FAS_POSTAB_WRITE_ITEM == cmd) ||\
	(FRAME_FAS_POSTAB_READ_ROM == cmd) ||\
	(FRAME_FAS_POSTAB_WRITE_ROM == cmd) ||\
	/*(FRAME_FAS_POSTAB_RUN_ITEM == cmd) ||*/\
	(FRAME_FAS_POSTAB_IS_DATA == cmd) ||\
	(FRAME_FAS_POSTAB_IS_DATA_EX == cmd) ||\
	/*(FRAME_FAS_POSTAB_RUN_ONEITEM == cmd) ||*/\
	(FRAME_FAS_POSTAB_CHECK_STOPMODE == cmd) ||\
	(FRAME_FAS_POSTAB_READ_ONEITEM == cmd) ||\
	(FRAME_FAS_POSTAB_WRITE_ONEITEM == cmd) ||\
	(FRAME_FAS_POSTAB_WRITE_MULTIITEMS == cmd) ||\
	/*(FRAME_FAS_SETLINEARINFO == cmd) ||*/\
	/*(FRAME_FAS_MOVELINEARINC == cmd) ||*/\
	/*(FRAME_FAS_MOVEPUSH == cmd) ||*/\
	(FRAME_FAS_GETPUSHSTATUS == cmd) ||\
	/*(FRAME_FAS_MOVESINGLEABS_EX == cmd) ||*/\
	/*(FRAME_FAS_MOVESINGLEINC_EX == cmd) ||*/\
	/*(FRAME_FAS_MOVEVELOCITY_EX == cmd) ||*/\
	/*(FRAME_FAS_GAPCONTROL_ENABLE == cmd) ||*/\
	(FRAME_FAS_GAPCONTROL_DISABLE == cmd) ||\
	(FRAME_FAS_GAPCONTROL_ISENABLE == cmd) ||\
	(FRAME_FAS_GAPCONTROL_GETADCVALUE == cmd) ||\
	(FRAME_FAS_GAPONERESULT_MONITOR == cmd))

#endif	// _MOTION_FRAME_DEFINE_H_