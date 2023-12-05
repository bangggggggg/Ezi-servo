#ifndef _COMM_SETTING_H_
#define _COMM_SETTING_H_
//===========================================================================
//	File Name	: COMM_SETTING.h
//	Description : Serial Communications for NONE
//===========================================================================

//===========================================================================
//	Include Files
//===========================================================================

// Include definition files.
#include "driver/uart.h"

//===========================================================================
//	Serial Communication Functions
//===========================================================================

#define UART_NUM                    (CONFIG_EZI_UART_PORT_NUM)
#define PACKET_READ_TICS            (100 /portTICK_PERIOD_MS)

// send/recv
#define _SERIAL_SEND(s, n)          uart_write_bytes(UART_NUM, s, n);
#define _SERIAL_RECV(s, n)          uart_read_bytes(UART_NUM, s, n, PACKET_READ_TICS)

// check rx buffer
#define _GET_SERIAL_RXBUFF_LEN(ptr) uart_get_buffered_data_len(UART_NUM, ptr)

#endif	// _COMM_SETTING_H_
