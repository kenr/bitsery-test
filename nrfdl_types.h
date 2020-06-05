/*
 * Copyright (c) 2020 Nordic Semiconductor ASA
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 *   1. Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 *   2. Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 *   3. Neither the name of Nordic Semiconductor ASA nor the names of other
 *   contributors to this software may be used to endorse or promote products
 *   derived from this software without specific prior written permission.
 *
 *   4. This software must only be used in or with a processor manufactured by Nordic
 *   Semiconductor ASA, or in or with a processor manufactured by a third party that
 *   is used in combination with a processor manufactured by Nordic Semiconductor.
 *
 *   5. Any software provided in binary or object form under this license must not be
 *   reverse engineered, decompiled, modified and/or disassembled.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/** @file
 *
 * @brief Type definitions and defines for Nordic Semiconductor devices.
 *
 */

#ifndef NRFDL_TYPES_H__
#define NRFDL_TYPES_H__

#ifdef __cplusplus
#include <cstdbool>
#include <cstddef>
#include <cstdint>
#else
// stdbool.h is deprecated in C++, cstdbool is deprecated in C++17 and removed in the draft C++20 standard
#include <stdbool.h>
#include <stddef.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

/* Version of the public API. Must be incremented if, and only if, the public version of the API is no longer backwards
 * compatible with previous API versions. */
#define NRFDL_API_VERSION 1

#define NRFDL_MAXPATHLEN 512
/* Maximum number of serial ports per device. */
#define NRFDL_MAXSERIALPORTS 100

/**@brief Definition of error codes.
 *
 */
typedef enum
{
    /* Success (no error). */
    NRFDL_ERR_NONE = 0,
    /* Generic error not covered by other error codes. */
    NRFDL_ERR_GENERIC = 1,
    /* Module is closed. */
    NRFDL_ERR_CLOSED = 2,
    /* Module is open already. */
    NRFDL_ERR_OPEN = 3,
    /* Generic memory allocation error. */
    NRFDL_ERR_MEMORY_ALLOCATION = 4,
    /* Generic error in implementation of serial port lister. */
    NRFDL_ERR_SERIALPORT_LISTER = 5,
    /* Generic error in implementation of USB lister. */
    NRFDL_ERR_USB_LISTER = 6,
    /* Error setting up logging. */
    NRFDL_ERR_LOGGING = 7,
    /* Error in event handler. */
    NRFDL_ERR_EVENT_HANDLER = 8,
    /* Error in nrfjprog implementation of nrfjprog lister. */
    NRFDL_ERR_NRFJPROG = 9,
    /* Error in protocol. */
    NRFDL_ERR_PROTOCOL = 10,
    /* Illegal argument. */
    NRFDL_ERR_ARGUMENT = 50,
    /* Resource already open. */
    NRFDL_ERR_RESOURCE_ALREADY_OPEN = 100,
    /* Resource already closed. */
    NRFDL_ERR_RESOURCE_ALREADY_CLOSED = 101,
    /* Resource is in illegal state for this operation.*/
    NRFDL_ERR_RESOURCE_ILLEGAL_STATE = 102,
    /* Resource has an internal error.*/
    NRFDL_ERR_RESOURCE_INTERNAL_ERROR = 103,
    /* Not supported function for given device */
    NRFDL_ERR_UNSUPPORTED_DEVICE = 200
} nrfdl_errorcode_t;

/**@brief Definition of log levels.
 *
 */
typedef enum
{
    NRFDL_LOG_TRACE    = 0,
    NRFDL_LOG_DEBUG    = 1,
    NRFDL_LOG_INFO     = 2,
    NRFDL_LOG_WARNING  = 3,
    NRFDL_LOG_ERROR    = 4,
    NRFDL_LOG_CRITICAL = 5,
    NRFDL_LOG_OFF      = 6
} nrfdl_loglevel_t;

/**@brief Definition of log message structure.
 *
 */
typedef struct
{
    /* The level of the message (critical, warning etc.). */
    nrfdl_loglevel_t level;
    /* Log message. Null-terminated string or nullptr. */
    const char * message;
} nrfdl_log_t;

/**@brief Definition of device traits structure.
 *
 */
typedef struct
{
    /* Device is USB. */
    bool usb;
    /* Device is USB, but has Nordic Semiconductor vendor ID. */
    bool nordic_usb;
    /* Device is USB, but has Nordic Semiconductor vendor ID. */
    bool nordic_dfu;
    /* Device is USB, but has Segger vendor ID. */
    bool segger_usb;
    /* Device is JLink. */
    bool jlink;
    /* Device is serialport. Only virtual serialports are supported. */
    bool serialport;
} nrfdl_traits_t;

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // NRFDL_TYPES_H__
