/*
 * Copyright (c) 2008-2012 the MansOS team. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *  * Redistributions of source code must retain the above copyright notice,
 *    this list of  conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef MANSOS_SPI_H
#define MANSOS_SPI_H

/// \file
/// SPI bus protocol, master mode only
///
/// Automatically chooses to use hardware or software SPI, depending on busId
/// The actual implementation is in HPL.
/// HAL layer provides macros and pin assignments
///

#include <stdtypes.h>

//! The constant used to specify software-SPI bus
#define SPI_BUS_SW 128

#include <hil/spi_hw.h>

/**
 * Initializes SPI bus in either master or slave mode
 * Does not change any Slave-Select pins!
 * Calls sw_spiInit or hw_spiBusInit (depending on busId), which are
 * defined in HPL!
 *
 * @param   busId   SPI bus ID
 * @param   mode    SPI bus mode: either master or slave
 * @return  0       on success, -1 on error
 */
#if USE_SOFT_SPI
int8_t spiBusInit(uint8_t busId, SpiBusMode_t spiBusMode);
#else
#define spiBusInit(busId, mode) hw_spiBusInit(busId, mode)
#endif

/**
 * Enable a SPI device - set SS pin LOW
 */
#define spiSlaveEnable(sePort, sePin) \
    pinClear(sePort, sePin)

/**
 * Disable a SPI device - set SS pin HIGH
 */
#define spiSlaveDisable(sePort, sePin) \
    pinSet(sePort, sePin)

/**
 * Exchange byte with a slave: write a byte to SPI and returns response,
 * received from the slave in full-duplex mode
 * Does not change any Slave-Select pin!
 *
 * Calls sw_spiExchByte or hw_spiExchByte (depending on busId), which are
 * defined in HPL!
 *
 * @param   busId   SPI bus ID
 * @param   b       byte to transmit
 * @return          byte received from the slave
 */
#ifdef USE_SOFT_SPI
uint8_t spiExchByte(uint8_t busId, uint8_t b);
#else
#define spiExchByte(busId, mode) hw_spiExchByte(busId, mode)
#endif

/**
 * Exchange a 16-bit word with a slave: write 2 bytes to SPI and return response,
 * received from the slave in full-duplex mode
 * Does not change any Slave-Select pin!
 *
 * Calls 2x spiExchByte
 *
 * @param   busId   SPI bus ID
 * @param   w       word to transmit
 * @return          word received from the slave
 */
uint16_t spiExchWord(uint8_t busId, uint16_t w);

/**
 * Just a wrapper to exchange-byte function
 *
 * @param   busId   SPI bus ID
 * @param   b       byte to transmit
 * @return          byte received from the slave
 */
#define spiWriteByte(busId, b) \
        spiExchByte(busId, b)

/*
 * Reads a byte from SPI - writes a 0 on the MOSI line and waits for response
 * @param   busId     SPI bus ID (use SPI_BUS_SW for soft SPI)
 * @return  received byte
 */
#define spiReadByte(busId) \
        spiExchByte(busId, 0)

/*
 * Writes a 16-bit word to SPI (performs 2 byte writes)
 * @param   busId   SPI bus ID (use SPI_BUS_SW for soft SPI)
 * @param   w       the word to be written
 * @return          word received from the slave
 */
#define spiWriteWord(busId, w) \
    spiExchWord(busId, w)

/*
 * Reads a 16-bit word from SPI - writes a 0 on the MOSI line and waits for response
 * @param   busId     SPI bus ID (use SPI_BUS_SW for soft SPI)
 * @return  received byte
 */
#define spiReadWord(busId) \
        spiExchWord(busId, 0)

/*
 * Writes a string to SPI (performs len byte writes)
 * @param   busId   SPI bus ID (use SPI_BUS_SW for soft SPI)
 * @param   buf     the buffer containing the string
 * @param   len     buffer length in bytes
 * @return  len     bytes written
 */
void spiWrite(uint8_t busId, const void *buf, uint16_t len);

/*
 * Reads a message into buffer from SPI (performs len byte reads)
 *
 * @param   busId   SPI bus ID (use SPI_BUS_SW for soft SPI)
 * @param   buf     the buffer to store the message
 * @param   len     buffer length in bytes
 * @return  received byte count
 */
void spiRead(uint8_t busId, void *buf, uint16_t len);

/*
 * Reads a message, discards it (without storing anywhere)
 * @param   busId   SPI bus ID (use SPI_BUS_SW for soft SPI)
 * @param   len     discarded message length in bytes
 * @return  received byte count
 */
uint8_t spiReadAndDiscard(uint8_t busId, uint16_t len);


#endif
