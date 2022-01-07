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
 * File: $Id: mbfuncholding.c,v 1.12 2007/02/18 23:48:22 wolti Exp $
 */

/* ----------------------- System includes ----------------------------------*/
#include "stdlib.h
#include "string.h

/* ----------------------- Platform includes --------------------------------*/
#include "modbus/port.h

/* ----------------------- Modbus includes ----------------------------------*/
#include "modbus/include/mb.h
#include "modbus/include/mbframe.h
#include "modbus/include/mbproto.h
#include "modbus/include/mbconfig.h

/* ----------------------- Defines ------------------------------------------*/
#define MB_PDU_FUNC_READ_ADDR_OFF               (MB_PDU_DATA_OFF + 0)
#define MB_PDU_FUNC_READ_REGCNT_OFF             (MB_PDU_DATA_OFF + 2)
#define MB_PDU_FUNC_READ_SIZE                   (4)
#define MB_PDU_FUNC_READ_REGCNT_MAX             (0x007D)

#define MB_PDU_FUNC_READ_SEL_REGCNT_OFF         (MB_PDU_DATA_OFF + 0)
#define MB_PDU_FUNC_READ_SEL_ADDR_OFF           (MB_PDU_DATA_OFF + 2)
#define MB_PDU_FUNC_READ_SEL_SIZE_MIN           (1)

#define MB_PDU_FUNC_WRITE_ADDR_OFF              (MB_PDU_DATA_OFF + 0)
#define MB_PDU_FUNC_WRITE_VALUE_OFF             (MB_PDU_DATA_OFF + 2)
#define MB_PDU_FUNC_WRITE_SIZE                  (4)

#define MB_PDU_FUNC_WRITE_MUL_ADDR_OFF          (MB_PDU_DATA_OFF + 0)
#define MB_PDU_FUNC_WRITE_MUL_REGCNT_OFF        (MB_PDU_DATA_OFF + 2)
#define MB_PDU_FUNC_WRITE_MUL_BYTECNT_OFF       (MB_PDU_DATA_OFF + 4)
#define MB_PDU_FUNC_WRITE_MUL_VALUES_OFF        (MB_PDU_DATA_OFF + 5)
#define MB_PDU_FUNC_WRITE_MUL_SIZE_MIN          (5)
#define MB_PDU_FUNC_WRITE_MUL_REGCNT_MAX        (0x007B)

#define MB_PDU_FUNC_READWRITE_READ_ADDR_OFF     (MB_PDU_DATA_OFF + 0)
#define MB_PDU_FUNC_READWRITE_READ_REGCNT_OFF   (MB_PDU_DATA_OFF + 2)
#define MB_PDU_FUNC_READWRITE_WRITE_ADDR_OFF    (MB_PDU_DATA_OFF + 4)
#define MB_PDU_FUNC_READWRITE_WRITE_REGCNT_OFF  (MB_PDU_DATA_OFF + 6)
#define MB_PDU_FUNC_READWRITE_BYTECNT_OFF       (MB_PDU_DATA_OFF + 8)
#define MB_PDU_FUNC_READWRITE_WRITE_VALUES_OFF  (MB_PDU_DATA_OFF + 9)
#define MB_PDU_FUNC_READWRITE_SIZE_MIN          (9)
#define MB_PDU_FUNC_READWRITE_REGCNT_MAX        (0x0079)

/* ----------------------- Static functions ---------------------------------*/
eMBException    prveMBError2Exception(eMBErrorCode eErrorCode);

/* ----------------------- Start implementation -----------------------------*/
#if MB_FUNC_WRITE_HOLDING_ENABLED > 0

eMBException
eMBFuncWriteHoldingRegister(uint8_t *pucFrame, uint16_t *usLen)
{
    uint16_t        usRegAddress;
    eMBErrorCode    eRegStatus;

    /* Can't be a valid request because the length is incorrect. */
    if (*usLen != (MB_PDU_FUNC_WRITE_SIZE + MB_PDU_SIZE_MIN))
        return MB_EX_ILLEGAL_DATA_VALUE;

    usRegAddress  = (uint16_t)(pucFrame[MB_PDU_FUNC_WRITE_ADDR_OFF] << 8);
    usRegAddress |= (uint16_t)(pucFrame[MB_PDU_FUNC_WRITE_ADDR_OFF + 1]);
    //usRegAddress++;

    /* Make callback to update the value. */
    eRegStatus =
        eMBRegHoldingCB(&pucFrame[MB_PDU_FUNC_WRITE_VALUE_OFF],
                        usRegAddress, 1, MB_REG_WRITE);

    /* If an error occured convert it into a Modbus exception. */
    if (eRegStatus != MB_ENOERR)
        return prveMBError2Exception(eRegStatus);

    return MB_EX_NONE;
}

#endif

#if MB_FUNC_WRITE_MULTIPLE_HOLDING_ENABLED > 0

eMBException
eMBFuncWriteMultipleHoldingRegister(uint8_t *pucFrame, uint16_t *usLen)
{
    uint16_t        usRegAddress;
    uint16_t        usRegCount;
    uint8_t         ucRegByteCount;
    eMBErrorCode    eRegStatus;

    /* Can't be a valid request because the length is incorrect. */
    if (*usLen <= (MB_PDU_FUNC_WRITE_MUL_SIZE_MIN + MB_PDU_SIZE_MIN))
        return MB_EX_ILLEGAL_DATA_VALUE;

    usRegAddress  = (uint16_t)(pucFrame[MB_PDU_FUNC_WRITE_MUL_ADDR_OFF] << 8);
    usRegAddress |= (uint16_t)(pucFrame[MB_PDU_FUNC_WRITE_MUL_ADDR_OFF + 1]);
    //usRegAddress++;

    usRegCount  = (uint16_t)(pucFrame[MB_PDU_FUNC_WRITE_MUL_REGCNT_OFF] << 8);
    usRegCount |= (uint16_t)(pucFrame[MB_PDU_FUNC_WRITE_MUL_REGCNT_OFF + 1]);

    ucRegByteCount = pucFrame[MB_PDU_FUNC_WRITE_MUL_BYTECNT_OFF];

    if ((usRegCount == 0) ||
        (usRegCount > MB_PDU_FUNC_WRITE_MUL_REGCNT_MAX) ||
        (ucRegByteCount != (uint8_t)(2 * usRegCount)))
        return MB_EX_ILLEGAL_DATA_VALUE;

    /* Make callback to update the register values. */
    eRegStatus =
        eMBRegHoldingCB(&pucFrame[MB_PDU_FUNC_WRITE_MUL_VALUES_OFF],
                        usRegAddress, usRegCount, MB_REG_WRITE);

    /* If an error occured convert it into a Modbus exception. */
    if (eRegStatus != MB_ENOERR)
        return prveMBError2Exception(eRegStatus);

    /* The response contains the function code, the starting
     * address and the quantity of registers. We reuse the
     * old values in the buffer because they are still valid.
     */
    *usLen = MB_PDU_FUNC_WRITE_MUL_BYTECNT_OFF;
    return MB_EX_NONE;
}

#endif

#if MB_FUNC_READ_HOLDING_ENABLED > 0

eMBException
eMBFuncReadHoldingRegister(uint8_t *pucFrame, uint16_t *usLen)
{
    uint16_t        usRegAddress;
    uint16_t        usRegCount;
    uint8_t        *pucFrameCur;
    eMBErrorCode    eRegStatus;

    /* Can't be a valid request because the length is incorrect. */
    if (*usLen != (MB_PDU_FUNC_READ_SIZE + MB_PDU_SIZE_MIN))
        return MB_EX_ILLEGAL_DATA_VALUE;

    usRegAddress  = (uint16_t)(pucFrame[MB_PDU_FUNC_READ_ADDR_OFF] << 8);
    usRegAddress |= (uint16_t)(pucFrame[MB_PDU_FUNC_READ_ADDR_OFF + 1]);
    //usRegAddress++;

    usRegCount  = (uint16_t)(pucFrame[MB_PDU_FUNC_READ_REGCNT_OFF] << 8);
    usRegCount |= (uint16_t)(pucFrame[MB_PDU_FUNC_READ_REGCNT_OFF + 1]);

    /* Check if the number of registers to read is valid. If not
     * return Modbus illegal data value exception.
     */
    if ((usRegCount == 0) ||
        (usRegCount > MB_PDU_FUNC_READ_REGCNT_MAX))
        return MB_EX_ILLEGAL_DATA_VALUE;


    /* Set the current PDU data pointer to the beginning. */
    pucFrameCur = &pucFrame[MB_PDU_FUNC_OFF];
    *usLen = MB_PDU_FUNC_OFF;

    /* First byte contains the function code. */
    *pucFrameCur++ = MB_FUNC_READ_HOLDING_REGISTER;
    *usLen += 1;

    /* Second byte in the response contain the number of bytes. */
    *pucFrameCur++ = (uint8_t)(usRegCount * 2);
    *usLen += 1;

    /* Make callback to fill the buffer. */
    eRegStatus =
        eMBRegHoldingCB(pucFrameCur, usRegAddress, usRegCount, MB_REG_READ);

    /* If an error occured convert it into a Modbus exception. */
    if (eRegStatus != MB_ENOERR)
        return prveMBError2Exception(eRegStatus);

    *usLen += usRegCount * 2;
    return MB_EX_NONE;
}

#endif

#if MB_FUNC_READ_SELECTED_HOLDING_ENABLED > 0

eMBException
eMBFuncReadSelectedHoldingRegister(uint8_t *pucFrame, uint16_t *usLen)
{
    uint16_t        usRegCount;
    uint16_t       *usRegAddresses;
    uint8_t        *pucFrameCur;
    eMBErrorCode    eRegStatus;

    /* Can't be a valid request because the length is incorrect. */
    if (*usLen <= (MB_PDU_FUNC_READ_SEL_SIZE_MIN + MB_PDU_SIZE_MIN))
        return MB_EX_ILLEGAL_DATA_VALUE;

    usRegCount  = (__uint16_t)(pucFrame[MB_PDU_FUNC_READ_SEL_REGCNT_OFF] << 8);
    usRegCount |= (__uint16_t)(pucFrame[MB_PDU_FUNC_READ_SEL_REGCNT_OFF + 1]);

    usRegAddresses  = (__uint16_t*)&pucFrame[MB_PDU_FUNC_READ_SEL_ADDR_OFF];

    /* Check if the number of registers to read is valid. If not
     * return Modbus illegal data value exception.
     */
    if ((usRegCount == 0) ||
        (usRegCount > MB_PDU_FUNC_READ_REGCNT_MAX))
        return MB_EX_ILLEGAL_DATA_VALUE;


    /* Set the current PDU data pointer to the beginning. */
    pucFrameCur = &pucFrame[MB_PDU_FUNC_OFF];
    *usLen = MB_PDU_FUNC_OFF;

    /* First byte contains the function code. */
    *pucFrameCur++ = MB_FUNC_READ_SELECTED_HOLDING_REGISTER;
    *usLen += 1;

    /* Second byte in the response contain the number of bytes. */
    *pucFrameCur++ = (uint8_t)(usRegCount * 2);
    *usLen += 1;

    /* Make callback to fill the buffer. */
    eRegStatus =
        eMBRegSelectedHoldingCB(pucFrameCur, usRegAddresses, usRegCount, MB_REG_READ);

    /* If an error occured convert it into a Modbus exception. */
    if (eRegStatus != MB_ENOERR)
        return prveMBError2Exception(eRegStatus);

    *usLen += usRegCount * 2;
    return MB_EX_NONE;
}

#endif

#if MB_FUNC_READWRITE_HOLDING_ENABLED > 0

eMBException
eMBFuncReadWriteMultipleHoldingRegister(uint8_t *pucFrame, uint16_t *usLen)
{
    uint16_t        usRegReadAddress;
    uint16_t        usRegReadCount;
    uint16_t        usRegWriteAddress;
    uint16_t        usRegWriteCount;
    uint8_t         ucRegWriteByteCount;
    uint8_t        *pucFrameCur;

    eMBErrorCode    eRegStatus;

    if (*usLen <= (MB_PDU_FUNC_READWRITE_SIZE_MIN + MB_PDU_SIZE_MIN))
        return MB_EX_ILLEGAL_DATA_VALUE;

    usRegReadAddress  = (uint16_t)(pucFrame[MB_PDU_FUNC_READWRITE_READ_ADDR_OFF] << 8U);
    usRegReadAddress |= (uint16_t)(pucFrame[MB_PDU_FUNC_READWRITE_READ_ADDR_OFF + 1]);
    //usRegReadAddress++;

    usRegReadCount  = (uint16_t)(pucFrame[MB_PDU_FUNC_READWRITE_READ_REGCNT_OFF] << 8U);
    usRegReadCount |= (uint16_t)(pucFrame[MB_PDU_FUNC_READWRITE_READ_REGCNT_OFF + 1]);

    usRegWriteAddress  = (uint16_t)(pucFrame[MB_PDU_FUNC_READWRITE_WRITE_ADDR_OFF] << 8U);
    usRegWriteAddress |= (uint16_t)(pucFrame[MB_PDU_FUNC_READWRITE_WRITE_ADDR_OFF + 1]);
    //usRegWriteAddress++;

    usRegWriteCount  = (uint16_t)(pucFrame[MB_PDU_FUNC_READWRITE_WRITE_REGCNT_OFF] << 8U);
    usRegWriteCount |= (uint16_t)(pucFrame[MB_PDU_FUNC_READWRITE_WRITE_REGCNT_OFF + 1]);

    ucRegWriteByteCount = pucFrame[MB_PDU_FUNC_READWRITE_BYTECNT_OFF];

    if ((usRegReadCount  == 0) || (usRegReadCount  > MB_PDU_FUNC_READ_REGCNT_MAX) ||
        (usRegWriteCount == 0) || (usRegWriteCount > MB_PDU_FUNC_READWRITE_REGCNT_MAX) ||
        ((2 * usRegWriteCount) != ucRegWriteByteCount))
        return MB_EX_ILLEGAL_DATA_VALUE;

    /* Make callback to update the register values. */
    eRegStatus =
        eMBRegHoldingCB(&pucFrame[MB_PDU_FUNC_READWRITE_WRITE_VALUES_OFF],
                        usRegWriteAddress, usRegWriteCount, MB_REG_WRITE);

    if (eRegStatus != MB_ENOERR)
        return prveMBError2Exception(eRegStatus);

    /* Set the current PDU data pointer to the beginning. */
    pucFrameCur = &pucFrame[MB_PDU_FUNC_OFF];
    *usLen = MB_PDU_FUNC_OFF;

    /* First byte contains the function code. */
    *pucFrameCur++ = MB_FUNC_READWRITE_MULTIPLE_REGISTERS;
    *usLen += 1;

    /* Second byte in the response contain the number of bytes. */
    *pucFrameCur++ = (uint8_t)(usRegReadCount * 2);
    *usLen += 1;

    /* Make the read callback. */
    eRegStatus =
        eMBRegHoldingCB(pucFrameCur, usRegReadAddress, usRegReadCount, MB_REG_READ);

    if (eRegStatus != MB_ENOERR)
        return prveMBError2Exception(eRegStatus);

    *usLen += 2 * usRegReadCount;
    return MB_EX_NONE;
}

#endif