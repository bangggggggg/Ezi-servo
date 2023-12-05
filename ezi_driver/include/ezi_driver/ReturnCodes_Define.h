#ifndef _RETURNCODES_DEFINE_H_
#define _RETURNCODES_DEFINE_H_

enum _FMM_ERROR
{
	FMM_OK = 0,

	FMM_NOT_OPEN,
	FMM_INVALID_PORT_NUM,
	FMM_INVALID_SLAVE_NUM,

	FMC_DISCONNECTED = 5,
	FMC_TIMEOUT_ERROR,
	FMC_CRCFAILED_ERROR,
	FMC_RECVPACKET_ERROR,	// PACKET SIZE ERROR, QUEUE FULL ERROR

	FMM_POSTABLE_ERROR,

	FMP_FRAMETYPEERROR = 0x80,
	FMP_DATAERROR,
	FMP_PACKETERROR,
	FMP_RUNFAIL = 0x85,
	FMP_RESETFAIL,
	FMP_SERVOONFAIL1,
	FMP_SERVOONFAIL2,
	FMP_SERVOONFAIL3,

	FMP_PACKETCRCERROR = 0xAA,

	FMM_SENDPACKET_ERROR = 0xFE,
	FMM_UNKNOWN_ERROR = 0xFF,
};

// static char *STR_FROM_FFM_ERROR(int errno)
// {
// 	switch (errno) {
// 		case FMM_OK:
// 		  return "Communication is normal.";
// 		case FMM_NOT_OPEN:
// 		  return "Not Open.";
// 		case FMM_INVALID_PORT_NUM:
// 		  return "Invalid Port Number.";
// 		case FMM_INVALID_SLAVE_NUM:
// 		  return "Invalid Slave Number.";
// 		case FMC_DISCONNECTED:
// 		  return "Disconnected Error.";
// 		case FMC_TIMEOUT_ERROR:
// 		  return "Timeout Error.";
// 		case FMC_CRCFAILED_ERROR:
// 		  return "CRC Error.";
// 		case FMC_RECVPACKET_ERROR:
// 		  return "Receive Packet Error.";
// 		case FMM_POSTABLE_ERROR:
// 		  return "Position Table Error.";
// 		case FMP_FRAMETYPEERROR:
// 		  return "Frame Type Error : Responded Frame type cannot be recognized.";
// 		case FMP_DATAERROR:
// 		  return (
// 				"Data error, ROM data read/write error : "
// 				"Data value responded is without the given range."
// 			);
// 		case FMP_PACKETERROR:
// 		  return (
// 				"Received Frame Error : "
// 				"Frame data received is out of this specification."
// 			);
//
// 		case FMP_RUNFAIL:
// 		  return (
// 				"Running Command Failure : The user has tried to execute new running "
// 				"commands in wrong condition as follows.\n"
// 				"  1) currently motor is running\n"
// 				"  2) currently motor is stopping\n"
// 				"  3) currently Servo is OFF status\n"
// 				"  4) try to Z-pulse Origin without encoder other wrong motion command\n"
// 			);
// 		case FMP_RESETFAIL:
// 		  return (
// 				"RESET Failure : The user has tried to execute new running commands in "
// 				"wrong condition as follows.\n"
// 				"	 1) While the servo is ON\n"
// 				"  2) Already RESET in ON by external input signal\n"
// 			);
// 		case FMP_SERVOONFAIL1:
// 		  return (
// 				"Servo ON Failure 1 : While an alarm occurs, the user has tried to "
// 				"execute Servo ON command."
// 			);
// 		case FMP_SERVOONFAIL2:
// 		  return (
// 				"Servo ON Failure 2 : While Emergency Stop occurs, the user has tried "
// 				"to execute Servo ON command."
// 			);
// 		case FMP_SERVOONFAIL3:
// 		  return (
// 				"Servo ON Failure 3 : ‘ServoON’ signal is assigned to input pin already. "
// 				"Servo ON/OFF can execute by external input signal only."
// 			);
// 		case FMP_PACKETCRCERROR:
// 			return "CRC Error : Frame data sent is out of CRC format";
// 		case FMM_SENDPACKET_ERROR:
// 			return "Send Packet Error";
// 		case FMM_UNKNOWN_ERROR:
// 			return "Unknown Error";
// 	}
// 	return "";
// }

#endif	// _RETURNCODES_DEFINE_H_
