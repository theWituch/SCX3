#include <xc.h>

/* ----------------------- System includes ----------------------------------*/
#include "system.h

/* ----------------------- Platform includes --------------------------------*/

/* ----------------------- CRC includes -------------------------------------*/
#include "crc/crc.h

/* ----------------------- Constant variables -------------------------------*/

static const uint8_t aucCRCHi[] = {
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40
};

static const uint8_t aucCRCLo[] = {
    0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06, 0x07, 0xC7,
    0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD, 0x0F, 0xCF, 0xCE, 0x0E,
    0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09, 0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9,
    0x1B, 0xDB, 0xDA, 0x1A, 0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC,
    0x14, 0xD4, 0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
    0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3, 0xF2, 0x32,
    0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4, 0x3C, 0xFC, 0xFD, 0x3D,
    0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A, 0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38,
    0x28, 0xE8, 0xE9, 0x29, 0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF,
    0x2D, 0xED, 0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
    0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60, 0x61, 0xA1,
    0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67, 0xA5, 0x65, 0x64, 0xA4,
    0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F, 0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB,
    0x69, 0xA9, 0xA8, 0x68, 0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA,
    0xBE, 0x7E, 0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
    0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71, 0x70, 0xB0,
    0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92, 0x96, 0x56, 0x57, 0x97,
    0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C, 0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E,
    0x5A, 0x9A, 0x9B, 0x5B, 0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89,
    0x4B, 0x8B, 0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
    0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42, 0x43, 0x83,
    0x41, 0x81, 0x80, 0x40
};

/* ----------------------- Static variables ---------------------------------*/

/* ----------------------- Function implementations -------------------------*/

static uint16_t
usCRC16Modbus(uint32_t *pulInput, size_t xLength)
{
    uint8_t* pucPtr = (uint8_t*) pulInput;
    uint8_t ucCRCHi = 0xFF;
    uint8_t ucCRCLo = 0xFF;
    int iIndex;

    while (xLength--)
    {
        iIndex = ucCRCLo ^ *(pucPtr++);
        ucCRCLo = (uint8_t) (ucCRCHi ^ aucCRCHi[iIndex]);
        ucCRCHi = aucCRCLo[iIndex];
    }

    return (uint16_t) (ucCRCHi << 8 | ucCRCLo);
}

uint32_t
ulCRCCalculate(eCRCType eType, void *pulInput, size_t xLength)
{
    return usCRC16Modbus(pulInput, xLength);
}


//int
//setupCRC16(void)
//{
//    unsigned int blockCrc; // CRC of the Flash block
//    IEC1CLR = 0x00010000; // disable DMA channel 0 interrupts
//    IFS1CLR = 0x00010000; // clear any existing DMA channel 0 interrupt flag
//    DMACONSET = 0x00008000; // enable the DMA controller
//    DCRCDATA = 0xffff; // seed the CRC generator
//    DCRCXOR = 0x1021; // Use the standard CCITT CRC 16 polynomial: X^16+X^12+X^5+1
//    DCRCCON = 0x0fc0; // CRC enabled, polynomial length 16, append mode
//    // CRC attached to the DMA channel 0.
//    DCH0CON = 0x03; // channel off, priority 3, no chaining
//    DCH0ECON = 0; // no start irqs, no match enabled
//    // program channel transfer
//    DCH0SSA = KVA_TO_PA(flashBuff); // transfer source physical address
//    DCH0DSA = KVA_TO_PA(&blockCrc); // transfer destination physical address
//    DCH0SSIZ = 200; // source size
//    DCH0DSIZ = 200; // dst size
//    DCHOCSIZ = 200; // 200 bytes transferred per event
//    DCH0INTCLR = 0x00ff00ff; // DMA0: clear events, disable interrupts
//    DCH1INTCLR = 0x00ff00ff; // DMA1: clear events, disable interrupts
//    DCH0CONSET = 0x80; // channel 0 on
//    // initiate a transfer
//    DCH0ECONSET = 0x00000080; // set CFORCE to 1
//    // do something else while the CRC calculation takes place
//    // poll to see that the transfer was done
//    BOOL error = 0;
//    while (1)
//    {
//        register int pollCnt; // don’t poll in a tight loop
//        int dmaFlags = DCH0INT;
//
//        if ((dmaFlags & 0x3)
//        {
//            // CHERIF (DCHxINT<0>) or CHTAIF (DCHxINT<1> set
//            error = 1; // error or aborted...
//            break;
//        } else
//        if (dmaFlags & 0x8)
//        {
//            // CHBCIF (DCHxINT<3>) set
//            break; // transfer completed normally
//        }
//
//        pollCnt = 100; // use an adjusted value here
//        while (pollCnt--); // wait before polling again
//    }
//}
//
//            int
//calcCRC16(void)
//{
//
//
//
//            DCRCCON = 0;
//            DCRCCONbits.MOD = 0; // legacy mode
//            DCRCCONbits.ON = 1; // enable CRC
//            DCRCCONbits.CRCISEL = 0; // interrupt when all shifts are done
//            DCRCCONbits.LENDIAN = 1; // little endian
//            DCRCCONbits.DWIDTH = 16 - 1; // 16-bit data width
//            DCRCCONbits.PLEN = 16 - 1; // 16-bit polynomial order
//            DCRCXOR = CRC16_POLYNOMIAL; // set polynomial
//            DCRCDATA = CRC16_SEED_VALUE; // set initial value
//            DCRCCONbits.CRCEN = 1; // start CRC calculation
//
//            pointer = (unsigned short*) message;
//            length = sizeof (message) / sizeof (unsigned short);
//
//            while (length--)
//    {
//            while (CRCCONbits.CRCFUL); // wait if FIFO is full
//            data = *pointer++; // load data
//            *((unsigned short*) &DCRCDAT) = data; // 16-bit word access to FIFO
//        }
//
//            while (DCRCCONbits.CRCFUL); // wait if FIFO is full
//
//            DCRCCONbits.CRCEN = 0; // suspend CRC calculation
//            IFS0CLR = _IFS0_DCRCIF_MASK; // clear the interrupt flag
//            *((unsigned short*) &CRCDAT) = 0; // 16-bit dummy data to shift out CRC result
//            DCRCCONbits.CRCEN = 1; // resume CRC calculation
//
//            while (!IFS0bits.CRCIF); // wait until shifts are done
//            crcResultCRC16 = (unsigned short) CRCWDAT; // get CRC result (must be 0xE716)
//
//            return 1;
//        }