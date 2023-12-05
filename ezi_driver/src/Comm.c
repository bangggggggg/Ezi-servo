//===========================================================================
//	File Name	: Comm.c
//	Description : Sending/Receiving Protocol Functions
//===========================================================================

//===========================================================================
//	Include Files
//===========================================================================

#include "ezi_driver/Comm.h"

#include "ezi_driver/COMM_Define.h"
#include "ezi_driver/MOTION_FRAME_DEFINE.h"
#include "ezi_driver/ReturnCodes_Define.h"

// Utility Functions
#include "ezi_driver/CalcCRC.h"

#include "ezi_driver/MemSet.h"

//===========================================================================
//	Local Definitions
//===========================================================================

// ASCII for PROTOCOL
#define	ASCII_NODE				0xAA
#define	ASCII_NODESTART			0xCC
#define	ASCII_NODEEND			0xEE

// Receive level
#define	LEVL_NONE				0
#define	LEVL_HEADER				1
#define	LEVL_DATA				2
#define	LEVL_NODEMARK			3
#define	LEVL_FINISHED			4

// Offsets of received packet
#define	PACKET_OFFSET_SLAVE		0
#define	PACKET_OFFSET_CMD		1
#define	PACKET_OFFSET_RTNCODE	2
#define	PACKET_OFFSET_DATA		3

#define	PACKET_SIZE_PREFIXED	5	//Slave No.(1) + Command (1) + Checksum (2) + ReturnCode (1)

// Receiving status
#define	RETURN_OK					0
#define	RETURN_TIMEOUT				1
#define	RETURN_RECVNOTENOUGH		2
//#define	RETURN_INCORRECTTARGET	3
#define	RETURN_RECVNODATA			4
#define	RETURN_DATAOVERFLOW			5
#define	RETURN_CORRUPTSENTDATA		6
#define	RETURN_CORRUPTRECVDATA		7
#define	RETURN_NOTSUPPORTED			8

//===========================================================================
//	Local Variables
//===========================================================================

unsigned char	m_buffRecv[MAX_RECV_BUFFER_SIZE];			// Buffer for _RecvPacket function
unsigned char	m_buffSend[MAX_SEND_BUFFER_SIZE];			// Buffer for _SendPacket function

//===========================================================================
//	Local Functions Definition
//===========================================================================

int _SendPacket(unsigned char iSlaveNo, unsigned char byCmd, unsigned char* lpIN, unsigned short dwINlen);
int _RecvPacket(unsigned char iSlaveNo, unsigned char byCmd, unsigned char* pRtnCode, unsigned char* lpOUT, unsigned short dwOUTlen, unsigned short dwWaitTime);

int _SendPacket(unsigned char iSlaveNo, unsigned char byCmd, unsigned char* lpIN, unsigned short dwINlen)
{
	unsigned char* ptr;
	unsigned char* ptrFrame;
	unsigned short wCRCValue;
	unsigned short nFrameLen;
	unsigned short nSendBuffLen;
	unsigned short i;

	nFrameLen = 4 + dwINlen;	// Slave, Cmd, Checksum(2), data
	nSendBuffLen = 4 + nFrameLen;	// Header(2), Tail(2)

	if (nSendBuffLen >= MAX_SEND_BUFFER_SIZE)	//Slave, Cmd, Checksum(2), data + Header, Tail
		return 0;

	// DATA FRAME ---------------------------------------------------
	ptrFrame = m_buffSend + MAX_SEND_BUFFER_SIZE - nFrameLen;
	ptr = ptrFrame;

	// Address
	*(ptr++) = iSlaveNo;

	// Frame Type
	*(ptr++) = byCmd;

	// Data
	if ((lpIN != 0x00) && (dwINlen > 0))
	{
		for (i=0; i<dwINlen; i++)
			*(ptr++) = *(((unsigned char*)lpIN) + i);
	}

	// CRC
	wCRCValue = CalcCRC(ptrFrame, (unsigned short)(ptr - ptrFrame));
	*(ptr++) = (wCRCValue & 0xFF);
	*(ptr++) = (wCRCValue >> 8) & 0xFF;

	// PACKET FRAME -------------------------------------------------
	ptr = m_buffSend;

	// Header
	*(ptr++) = ASCII_NODE;
	*(ptr++) = ASCII_NODESTART;

	for (i=0; i<nFrameLen; i++)
	{
		*(ptr++) = *(ptrFrame + i);
		if (*(ptrFrame + i) == ASCII_NODE)
		{
			nSendBuffLen++;

			if (nSendBuffLen >= MAX_SEND_BUFFER_SIZE)	//Slave, Cmd, Checksum(2), data + Header, Tail + byte stuffing
				return 0;

			*(ptr++) = ASCII_NODE;
		}
	}

	// Tail
	*(ptr++) = ASCII_NODE;
	*(ptr++) = ASCII_NODEEND;

	// SEND FRAME ---------------------------------------------------
	_SERIAL_SEND(m_buffSend, nSendBuffLen);

	return nSendBuffLen;
}

int _RecvPacket(unsigned char iSlaveNo, unsigned char byCmd, unsigned char* pRtnCode, unsigned char* lpOUT, unsigned short dwOUTlen, unsigned short dwWaitTime)
{
	int nLevl = LEVL_NONE;
	unsigned int nBuffReadCount = 0;
	unsigned char by;

	// Receive Packet
	CommTimer_Reset(dwWaitTime);
	do
	{
		size_t len;
		_GET_SERIAL_RXBUFF_LEN(&len);
		if (!len)
		{
			if (CommTimer_IsExpired())
				return RETURN_TIMEOUT;
			static int cnt = 0;
			if (++cnt >= 400) {
				cnt = 0;
				CommTimer_Tick();
			}
			continue;
		}
		else
		{
			_SERIAL_RECV(&by, 1);
		}
		CommTimer_Reset(dwWaitTime);

		if (nBuffReadCount >=  MAX_RECV_BUFFER_SIZE)
		{
			//TraceMsg(_T("_RecvPacket(P%d, S%d, C0x%02X) Recv. queue was overflowed.\r\n"), m_nPortNo, iSlaveNo, byCmd);
			return RETURN_DATAOVERFLOW;
		}
		switch (nLevl)
		{
		case LEVL_NONE:
			if (by == ASCII_NODE)
				nLevl = LEVL_HEADER;
			break;
		case LEVL_HEADER:
			if (by == ASCII_NODESTART)
			{
				nLevl = LEVL_DATA;
				nBuffReadCount = 0;
			}
			else
			{
				nLevl = LEVL_NONE;
				//TraceMsg(_T("_RecvPacket(P%d, S%d, C0x%02X) HEADER failed.\r\n"), m_nPortNo, iSlaveNo, byCmd);
			}
			break;
		case LEVL_DATA:
			if (by == ASCII_NODE)
				nLevl = LEVL_NODEMARK;
			else
			{
				m_buffRecv[nBuffReadCount] = by;
				nBuffReadCount++;
			}
			break;
		case LEVL_NODEMARK:
			if (by == ASCII_NODE)
			{
				nLevl = LEVL_DATA;

				m_buffRecv[nBuffReadCount] = by;
				nBuffReadCount++;
			}
			else if (by == ASCII_NODESTART)
			{
				nLevl = LEVL_DATA;
				nBuffReadCount = 0;

				//TraceMsg(_T("_RecvPacket(P%d, S%d, C0x%02X) DATA canceled.\r\n"), m_nPortNo, iSlaveNo, byCmd);
			}
			else if (by == ASCII_NODEEND)
			{
				// Analysis
				if ((m_buffRecv[PACKET_OFFSET_SLAVE] != iSlaveNo) || (m_buffRecv[PACKET_OFFSET_CMD] != byCmd))
				{
					//if (m_buffRecv[PACKET_OFFSET_SLAVE] != iSlaveNo)
					//	TraceMsg(_T("_RecvPacket(P%d, S%d, C0x%02X) Different Slave Number. (Recv_Slave %d)\r\n"), m_nPortNo, iSlaveNo, byCmd, m_buffRecv[PACKET_OFFSET_SLAVE]);

					//if (m_buffRecv[PACKET_OFFSET_CMD] != byCmd)
					//	TraceMsg(_T("_RecvPacket(P%d, S%d, C0x%02X) Different Command. (Recv_Cmd 0x%02X)\r\n"), m_nPortNo, iSlaveNo, byCmd, m_buffRecv[PACKET_OFFSET_CMD]);

					nLevl = LEVL_NONE;
				}
				else
				{
					nLevl = LEVL_FINISHED;
				}
			}
			break;
		}
	}
	while (nLevl != LEVL_FINISHED);

	if (CalcCRC(m_buffRecv, nBuffReadCount) != 0x00)
	{
		//TraceMsg(_T("_RecvPacket(P%d, S%d, C0x%02X) CRC Failed.\r\n"), m_nPortNo, iSlaveNo, byCmd);
		return RETURN_CORRUPTRECVDATA;
	}

	// 5 = Slave No.(1) + Command (1) + Checksum (2) + ReturnCode (1)
	// Copy Return code.
	if (pRtnCode != 0x00)
	{
		if (nBuffReadCount < PACKET_SIZE_PREFIXED)
		{
			//TraceMsg(_T("_RecvPacket(P%d, S%d, C0x%02X) Packet is smaller than %d.\r\n"), m_nPortNo, iSlaveNo, byCmd, PACKET_SIZE_PREFIXED);
			return RETURN_RECVNOTENOUGH;
		}

		*pRtnCode = m_buffRecv[PACKET_OFFSET_RTNCODE];
	}

	switch (m_buffRecv[PACKET_OFFSET_RTNCODE])
	{
	case FMP_FRAMETYPEERROR:
		return RETURN_OK;
		break;
	case FMP_PACKETCRCERROR:
		return RETURN_CORRUPTSENTDATA;
		break;
	}

	// Copy Data.
	if ((lpOUT != 0x00) && (dwOUTlen > 0))
	{
		switch (byCmd)
		{
		case FRAME_GETSLAVEINFO:
		case FRAME_GETMOTORINFO:
			if (nBuffReadCount <= PACKET_SIZE_PREFIXED)
			{
				//TraceMsg(_T("_RecvPacket(P%d, S%d, C0x%02X) Packet must be bigger than %d if it has data.\r\n"), m_nPortNo, iSlaveNo, byCmd, PACKET_SIZE_PREFIXED);
				return RETURN_RECVNODATA;
			}
			break;

		default:
			if ((nBuffReadCount - PACKET_SIZE_PREFIXED) != dwOUTlen)
			{
				//TraceMsg(_T("_RecvPacket(P%d, S%d, C0x%02X) Packet must be bigger than %d.\r\n"), m_nPortNo, iSlaveNo, byCmd, PACKET_SIZE_PREFIXED);
				return RETURN_RECVNODATA;
			}
			break;
		}

		memcpy(
			lpOUT,
			&(m_buffRecv[PACKET_OFFSET_DATA]),
			((nBuffReadCount - PACKET_SIZE_PREFIXED) > dwOUTlen) ? (nBuffReadCount - PACKET_SIZE_PREFIXED) : dwOUTlen
		);
	}
	return RETURN_OK;
}

//------------------------------------------------------------------
//                 Protocol Functions which CommInterface use.
//------------------------------------------------------------------
int FuncCommand(unsigned char iSlaveNo, unsigned char byCmd, unsigned char* lpIN, unsigned short dwINlen, unsigned char* lpOUT, unsigned short dwOUTlen, unsigned short dwWaitTime, int bTimeOutReaction)
{
	int nRetry, nMaxRetry;
	int nLastErr;
	unsigned char nPacketRtnCode;
	int nReturn;

	nMaxRetry = (bTimeOutReaction) ? MAX_ALLOWED_TIMEOUT : 0;
	nRetry = nMaxRetry;
	nPacketRtnCode = 0;
	nLastErr = RETURN_OK;

	do
	{
		if (_SendPacket(iSlaveNo, byCmd, lpIN, dwINlen) <= 0)
		{
			return FMM_SENDPACKET_ERROR;
		}
		memset(lpOUT, 0x00, dwOUTlen);
		nReturn = _RecvPacket(iSlaveNo, byCmd, &nPacketRtnCode, lpOUT, dwOUTlen, dwWaitTime);

		switch (nReturn)
		{
		//case RETURN_OK:		break;
		//case RETURN_TIMEOUT:	break;

		//case RETURN_RECVNOTENOUGH:
		//case RETURN_RECVNODATA:
		//	nReturn = FMC_RECVPACKET_ERROR;
		//	break;
		case RETURN_DATAOVERFLOW:
			nReturn = FMC_RECVPACKET_ERROR;
			break;

		//case RETURN_CORRUPTSENTDATA:	break;

		case RETURN_CORRUPTRECVDATA:
			if (IS_FUNCTION_RETRIABLE(byCmd) == 0)
				nReturn = FMC_CRCFAILED_ERROR;
			break;
		}

		// ���� �����̸� nRetry Ƚ�� ����, �ٸ� �����̸� Default������ ����.
		nRetry = (nReturn != nLastErr) ? (nMaxRetry) : (nRetry - 1);
		nLastErr = nReturn;
	}
	while ((nReturn != RETURN_OK) && (nRetry > 0));

	switch (nReturn)
	{
	case RETURN_TIMEOUT:
		nReturn = FMC_TIMEOUT_ERROR;
		break;
	case RETURN_RECVNOTENOUGH:
	case RETURN_RECVNODATA:
	case RETURN_DATAOVERFLOW:
		nReturn = FMC_RECVPACKET_ERROR;
		break;
	case RETURN_CORRUPTSENTDATA:
		nReturn = FMP_PACKETCRCERROR;
		break;
	case RETURN_CORRUPTRECVDATA:
		nReturn = FMC_CRCFAILED_ERROR;
		break;

	default:
		nReturn = (int)nPacketRtnCode;
		break;
	}

	return nReturn;
}

int FuncCommandNoResp(unsigned char iSlaveNo, unsigned char byCmd, unsigned char* lpIN, unsigned short dwINlen)
{
	if (_SendPacket(iSlaveNo, byCmd, lpIN, dwINlen) <= 0)
	{
		return FMM_SENDPACKET_ERROR;
	}

	return FMM_OK;
}

//------------------------------------------------------------------
//                 Timer Function which have to call in INT.
//------------------------------------------------------------------
volatile unsigned int glb_CommTimer_Remain = 0;

void CommTimer_Tick()
{
	if (glb_CommTimer_Remain > 0)
		glb_CommTimer_Remain--;
}

//------------------------------------------------------------------
//                 Timer Functions which CommInterface use.
//------------------------------------------------------------------
void CommTimer_Reset(unsigned int nTime)
{
	glb_CommTimer_Remain = nTime;
}

int CommTimer_IsExpired()
{
	return (glb_CommTimer_Remain == 0);
}
