#pragma once

#include <cstdint>
#include <optional>
#include <variant>
#include <vector>

namespace NRFDL::SDFU
{
    /**
     * @brief DFU object types.
     */
    enum class DfuObjecType : uint8_t
    {
        NRF_DFU_OBJ_TYPE_INVALID,
        NRF_DFU_OBJ_TYPE_COMMAND,
        NRF_DFU_OBJ_TYPE_DATA,
    };

    /**
     * @brief DFU protocol operation.
     */
    enum class DfuOpcode : uint8_t
    {
        NRF_DFU_OP_PROTOCOL_VERSION  = 0x00,
        NRF_DFU_OP_OBJECT_CREATE     = 0x01,
        NRF_DFU_OP_RECEIPT_NOTIF_SET = 0x02,
        NRF_DFU_OP_CRC_GET           = 0x03,
        NRF_DFU_OP_OBJECT_EXECUTE    = 0x04,
        NRF_DFU_OP_OBJECT_SELECT     = 0x06,
        NRF_DFU_OP_MTU_GET           = 0x07,
        NRF_DFU_OP_OBJECT_WRITE      = 0x08,
        NRF_DFU_OP_PING              = 0x09,
        NRF_DFU_OP_HARDWARE_VERSION  = 0x0A,
        NRF_DFU_OP_FIRMWARE_VERSION  = 0x0B,
        NRF_DFU_OP_ABORT             = 0x0C,
        NRF_DFU_OP_RESPONSE          = 0x60,
        NRF_DFU_OP_INVALID           = 0xFF,
    };

    /**
     * @brief DFU operation result code.
     */
    enum class DfuResult : uint8_t
    {
        NRF_DFU_RES_CODE_INVALID                 = 0x00,
        NRF_DFU_RES_CODE_SUCCESS                 = 0x01,
        NRF_DFU_RES_CODE_OP_CODE_NOT_SUPPORTED   = 0x02,
        NRF_DFU_RES_CODE_INVALID_PARAMETER       = 0x03,
        NRF_DFU_RES_CODE_INSUFFICIENT_RESOURCES  = 0x04,
        NRF_DFU_RES_CODE_INVALID_OBJECT          = 0x05,
        NRF_DFU_RES_CODE_UNSUPPORTED_TYPE        = 0x07,
        NRF_DFU_RES_CODE_OPERATION_NOT_PERMITTED = 0x08,
        NRF_DFU_RES_CODE_OPERATION_FAILED        = 0x0A,
        NRF_DFU_RES_CODE_EXT_ERROR               = 0x0B
    };

    enum class DfuFirmwareType : uint8_t
    {
        NRF_DFU_FIRMWARE_TYPE_SOFTDEVICE  = 0x00,
        NRF_DFU_FIRMWARE_TYPE_APPLICATION = 0x01,
        NRF_DFU_FIRMWARE_TYPE_BOOTLOADER  = 0x02,
        NRF_DFU_FIRMWARE_TYPE_UNKNOWN     = 0xFF,
    };

    /**
     * @brief @ref NRF_DFU_OP_PROTOCOL_VERSION response details.
     */
    struct DfuResponseProtocol
    {
        uint8_t version;
    };

    struct DfuResponseHardwareMemory
    {
        uint32_t rom_size;
        uint32_t ram_size;
        uint32_t rom_page_size;
    };

    /**
     * @brief @ref NRF_DFU_OP_HARDWARE_VERSION response details.
     */
    struct DfuResponseHardware
    {
        uint32_t part;
        uint32_t variant;
        DfuResponseHardwareMemory memory;
    };

    /**
     * @brief @ref NRF_DFU_OP_FIRMWARE_VERSION response details.
     */
    struct DfuResponseFirmware
    {
        DfuFirmwareType type;
        uint32_t version;
        uint32_t addr;
        uint32_t len;
    };

    /**
     * @brief @ref NRF_DFU_OP_OBJECT_SELECT response details.
     */
    struct DfuResponseSelect
    {
        uint32_t offset;
        uint32_t crc;
        uint32_t max_size;
    };

    /**
     * @brief @ref NRF_DFU_OP_OBJECT_CREATE response details.
     */
    struct DfuResponseCreate
    {
        uint32_t offset;
        uint32_t crc;
    };

    /**
     * @brief @ref NRF_DFU_OP_OBJECT_WRITE response details.
     */
    struct DfuResponseWrite
    {
        uint32_t offset;
        uint32_t crc;
    };

    /**
     * @brief @ref NRF_DFU_OP_CRC_GET response details.
     */
    struct DfuResponseCrc
    {
        uint32_t offset;
        uint32_t crc;
    };

    /**
     * @brief @ref NRF_DFU_OP_PING response details.
     */
    struct DfuResponsePing
    {
        uint8_t id;
    };

    /**
     * @brief @ref NRF_DFU_OP_MTU_GET response details.
     */
    struct DfuResponseMtu
    {
        uint16_t size;
    };

    using DfuResponseType = std::variant<DfuResponseProtocol,
                                         DfuResponseHardware,
                                         DfuResponseFirmware,
                                         DfuResponseSelect,
                                         DfuResponseCreate,
                                         DfuResponseWrite,
                                         DfuResponseCrc,
                                         DfuResponsePing,
                                         DfuResponseMtu>;

    /**
     * @brief DFU response message.
     */
    struct DfuResponse
    {
        DfuResponse()  = default;
        ~DfuResponse() = default;

        DfuOpcode opcode;
        DfuResult result;
        std::optional<DfuResponseType> response;
    };

    /**
     * @brief @ref NRF_DFU_OP_FIRMWARE_VERSION request details.
     */
    struct DfuRequestFirmware
    {
        uint8_t image_number;
    };

    /**
     * @brief @ref NRF_DFU_OP_OBJECT_SELECT request details.
     */
    struct DfuRequestSelect
    {
        uint32_t object_type;
    };

    /**
     * @brief @ref NRF_DFU_OP_OBJECT_CREATE request details.
     */
    struct DfuRequestCreate
    {
        uint32_t object_type;
        uint32_t object_size;
    };

    /**
     * @brief @ref NRF_DFU_OP_OBJECT_WRITE request details.
     */
    struct DfuRequestWrite
    {
        DfuRequestWrite(){};
        std::vector<uint8_t> data;
        uint16_t len;
    };

    /**
     * @brief @ref NRF_DFU_OP_PING request details.
     */
    struct DfuRequestPing
    {
        uint8_t id;
    };

    /**
     * @brief @ref NRF_DFU_OP_MTU_GET request details.
     */
    struct DfuRequestMtu
    {
        uint16_t size;
    };

    /**
     * @brief @ref NRF_DFU_OP_RECEIPT_NOTIF_SET request details.
     */
    struct DfuRequestPrn
    {
        uint32_t target;
    };

    using DfuRequestType = std::variant<DfuRequestFirmware,
                                        DfuRequestSelect,
                                        DfuRequestCreate,
                                        DfuRequestWrite,
                                        DfuRequestPing,
                                        DfuRequestMtu,
                                        DfuRequestPrn>;

    /**
     *@brief DFU request.
     */
    struct DfuRequest
    {
        DfuOpcode opcode;
        std::optional<DfuRequestType> request;
    };
} // namespace NRFDL::SDFU