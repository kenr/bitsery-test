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

#include "nrfdl_usb_types.h"

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

/* Definition of a list of USB endpoints. */
typedef struct
{
    /* Number of interfaces. */
    uint8_t length;
    /* List of endpoint descriptors (of 'length' size). */
    const nrfdl_usb_endpoint_descriptor_t * descriptors;
} nrfdl_usb_endpoint_list_t;

/* Definition of a list of USB interfaces. */
typedef struct
{
    /* Number of interfaces. */
    uint8_t length;
    /* List of interface descriptors (of 'length' size). */
    const nrfdl_usb_interface_descriptor_t * descriptors;
    /* List of endpoint lists (of 'length' size - one list per interface). */
    const nrfdl_usb_endpoint_list_t * endpointLists;
} nrfdl_usb_interface_list_t;

/* Definition of a list of USB configurations. */
typedef struct
{
    /* Number of configurations. */
    uint8_t length;
    /* List of configuration descriptors (of 'length' size). */
    const nrfdl_usb_config_descriptor_t * descriptors;
    /* List of interface lists (of 'length' size - one list per configuration). */
    const nrfdl_usb_interface_list_t * interfaceLists;
} nrfdl_usb_config_list_t;

/* Definition of an USB device structure. Contains standard, USB-specific information. */
typedef struct
{
    /* The number of the bus. */
    uint16_t busNumber;
    /* Address for the device on the bus. */
    uint16_t address;
    /* Device descriptor. */
    nrfdl_usb_device_descriptor_t descriptor;
    /* List of configurations. */
    nrfdl_usb_config_list_t configList;
} nrfdl_usb_device_t;

/* Definition of a top-level USB structure for the Nordic Semiconductor library. */
typedef struct
{
    /* Serial number. Null-terminated string or nullptr. */
    const char * serialnumber;
    /* Manufacturer. Null-terminated string or nullptr. */
    const char * manufacturer;
    /* Product. Null-terminated string or nullptr. */
    const char * product;
    /* USB device structure. Represents standard, USB-specific information. May be nullptr/empty. */
    const nrfdl_usb_device_t * device;
} nrfdl_usb_t;

typedef struct
{
    /* Serial number. Null-terminated string or nullptr. */
    const char * serialnumber;

    /* Board version. Null-terminated string or nullptr. */
    const char * boardversion;

    /* COM name. Null-terminated string or nullptr. */
    const char * com_name;

    /* Manufacturer. Null-terminated string or nullptr. */
    const char * manufacturer;

    /* PNP ID. Null-terminated string or nullptr. */
    const char * pnp_id;

    /* Location ID. Null-terminated string or nullptr. */
    const char * location_id;

    /* Product ID. Null-terminated string or nullptr. */
    const char * product_id;

    /* Vendor ID. Null-terminated string or nullptr. */
    const char * vendor_id;

    /* VCOM (Virtual COM) number */
    uint32_t vcom;
} nrfdl_serialport_t;

typedef struct
{
    /* Number of serial ports. */
    size_t len;

    /* List of serial ports represented as one-dimensional C array. Is nullptr if len is 0. */
    const nrfdl_serialport_t * serialports;
} nrfdl_serialport_list_t;

/* Unique ID for a device. Is unique as long as library is loaded or until
 * integer type of ID wraps around it's maximum value. */
typedef uint32_t nrfdl_device_id_t;

typedef struct
{
    /* Serial number. Null-terminated string or nullptr. */
    const char * serial_number;
    /* Board version. Null-terminated string or nullptr. */
    const char * board_version;
    const char * jlink_ob_firmware_version;

    const char * device_family;
    const char * device_version;
} nrfdl_jlink_t;

typedef struct
{
    /* Unique ID for a device. */
    nrfdl_device_id_t id;
    /* The serial number of the device. */
    const char * serialnumber;
    /* The traits of the device (USB, JLink etc.). */
    nrfdl_traits_t traits;
    /* USB information. May be empty/nullptr. */
    const nrfdl_usb_t * usb;
    /* JLink information. May be empty/nullptr. */
    const nrfdl_jlink_t * jlink;
    /* List of serial ports. */
    nrfdl_serialport_list_t serialports;
} nrfdl_device_t;

typedef struct
{
    /* Number of devices in list. */
    size_t length;
    /* List of devices represented as one-dimensional C array. Is nullptr if length is 0. */
    const nrfdl_device_t * devices;
} nrfdl_device_list_t;

typedef enum
{
    NRFDL_DEVICE_EVENT_ARRIVED = 0,
    NRFDL_DEVICE_EVENT_LEFT    = 1
} nrfdl_device_event_type_t;

typedef struct
{
    /* Device ID. */
    nrfdl_device_id_t device_id;
    /* Type of device event. */
    nrfdl_device_event_type_t event_type;
} nrfdl_device_event_t;

/**@brief Callback type definition for a device event.
 *
 * This is a type definition of the function to be called when a conflated device is ready in the
 * API.
 *
 * @param[in]  event  The event structure identifying the conflated device.
 */
typedef void (*nrfdl_device_cb_fn)(nrfdl_device_event_t event);

/**@brief Callback type definition for a shutdown event.
 *
 * This callback is to be called in case of a critical, unrecoverable error that shuts down the library.
 */
typedef void (*nrfdl_shutdown_cb_fn)(nrfdl_errorcode_t error);

/**@brief Callback type definition for a log message event.
 *
 * This is a type definition of the function to be called for a log message.
 *
 * @param[in]  log  The log message represented as a struct. The data pointed to is owned by the library. The pointer is
 *                  invalid and the data is freed automatically when the callback goes out of scope.
 */
typedef void (*nrfdl_log_cb_fn)(const nrfdl_log_t * log);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // NRFDL_TYPES_H__
