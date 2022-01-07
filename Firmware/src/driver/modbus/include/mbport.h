/*
 * FreeModbus Libary: A portable Modbus implementation for Modbus ASCII/RTU.
 * Copyright (c) 2006 Christian Walter <wolti@sil.at>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * File: $Id: mbport.h,v 1.19 2010/06/06 13:54:40 wolti Exp $
 */

#ifndef _MB_PORT_H
#define _MB_PORT_H

#ifdef __cplusplus
PR_BEGIN_EXTERN_C
#endif

/* ----------------------- Type definitions ---------------------------------*/

typedef enum
{
    EV_READY,                   /*!< Startup finished. */
    EV_FRAME_RECEIVED,          /*!< Frame received. */
    EV_EXECUTE,                 /*!< Execute function. */
    EV_FRAME_SENT               /*!< Frame sent. */
} eMBEventType;

/*! \ingroup modbus
 * \brief Parity used for characters in serial mode.
 *
 * The parity which should be applied to the characters sent over the serial
 * link. Please note that this values are actually passed to the porting
 * layer and therefore not all parity modes might be available.
 */
typedef enum
{
    MB_PAR_NONE,                /*!< No parity. */
    MB_PAR_ODD,                 /*!< Odd parity. */
    MB_PAR_EVEN                 /*!< Even parity. */
} eMBParity;

/*! \ingroup modbus
 * \brief Parity used for characters in serial mode.
 *
 * The parity which should be applied to the characters sent over the serial
 * link. Please note that this values are actually passed to the porting
 * layer and therefore not all parity modes might be available.
 */
typedef enum
{
    MB_STOPB_ONE,               /*!< 1 Stop bit */
    MB_STOPB_TWO                /*!< 2 Stop bits */
} eMBStopBits;

/* ----------------------- Constant variables -------------------------------*/
const UCHAR ucSlaveID[SLAVEID_LENGTH] = SLAVEID;

/* ----------------------- Supporting functions -----------------------------*/
BOOL            xMBPortEventInit(void);
BOOL            xMBPortEventPost(eMBEventType eEvent);
BOOL            xMBPortEventGet( /*@out@ */ eMBEventType *eEvent);

/* ----------------------- Serial port functions ----------------------------*/
BOOL            xMBPortSerialInit(uint8_t ucPort, uint32_t ulBaudRate,
                                  uint8_t ucDataBits, eMBParity eParity,
                                  eMBStopBits eStopBits);
void            vMBPortClose(void);
void            xMBPortSerialClose(void);
void            vMBPortSerialEnable(BOOL xRxEnable, BOOL xTxEnable);
BOOL            xMBPortSerialGetByte(int8_t *pucByte);
BOOL            xMBPortSerialPutByte(int8_t ucByte);

/* ----------------------- Timers functions ---------------------------------*/
BOOL            xMBPortTimersInit(uint16_t usTimeOut50us);
void            xMBPortTimersClose(void);
void            vMBPortTimersEnable(void);
void            vMBPortTimersDisable(void);
void            vMBPortTimersDelay(uint16_t usTimeOutMS);

/* ----------------------- Callback for the protocol stack ------------------*/

/*!
 * \brief Callback function for the porting layer when a new byte is
 *   available.
 *
 * Depending upon the mode this callback function is used by the RTU or
 * ASCII transmission layers. In any case a call to xMBPortSerialGetByte()
 * must immediately return a new character.
 *
 * \return <code>TRUE</code> if a event was posted to the queue because
 *   a new byte was received. The port implementation should wake up the
 *   tasks which are currently blocked on the eventqueue.
 */
extern BOOL     (*pxMBFrameCBByteReceived)(void);
extern BOOL     (*pxMBFrameCBTransmitterEmpty)(void);
extern BOOL     (*pxMBPortCBTimerExpired)(void);

/* ----------------------- TCP port functions -------------------------------*/
BOOL            xMBTCPPortInit(uint16_t usTCPPort);
void            vMBTCPPortClose(void);
void            vMBTCPPortDisable(void);
BOOL            xMBTCPPortGetRequest(uint8_t **ppucMBTCPFrame, uint16_t *usTCPLength);
BOOL            xMBTCPPortSendResponse(const uint8_t *pucMBTCPFrame, uint16_t usTCPLength);

#ifdef __cplusplus
PR_END_EXTERN_C
#endif
#endif
