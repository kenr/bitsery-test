#include "catch.hpp"
#include "fmt/format.h"

#include "sdfu_codec.h"
#include "sdfu_types.h"

using namespace NRFDL::SDFU;

namespace
{
    TEST_CASE("Test encoding", "[sdfu]")
    {
        Codec codec;

        SECTION("Encode requests")
        {
            SECTION("MTU")
            {
                DfuRequest req;
                std::vector<uint8_t> data;

                req.opcode = DfuOpcode::NRF_DFU_OP_MTU_GET;
                // req.mtu.size = 100;
                DfuRequestMtu mtu;
                mtu.size    = 100;
                req.request = mtu;

                REQUIRE(codec.encode(req, data) == NRFDL_ERR_NONE);
                REQUIRE(data.size() == 3);
            }

            SECTION("FirmwareVersion")
            {
                DfuRequest req;
                std::vector<uint8_t> data;

                req.opcode = DfuOpcode::NRF_DFU_OP_FIRMWARE_VERSION;
                DfuRequestFirmware firmware;
                firmware.image_number = 5;
                req.request           = firmware;
                REQUIRE(codec.encode(req, data) == NRFDL_ERR_NONE);
                REQUIRE(data.size() == 2);
            }

            SECTION("ObjectWrite")
            {
                DfuRequest req;
                std::vector<uint8_t> data;

                req.opcode = DfuOpcode::NRF_DFU_OP_OBJECT_WRITE;
                DfuRequestWrite write;
                write.data  = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
                write.len   = 10;
                req.request = write;
                REQUIRE(codec.encode(req, data) == NRFDL_ERR_NONE);
                REQUIRE(data.size() == 13);
                std::vector<uint8_t> expected{
                    static_cast<std::underlying_type<DfuOpcode>::type>(DfuOpcode::NRF_DFU_OP_OBJECT_WRITE),
                    0x00,
                    0x01,
                    0x02,
                    0x03,
                    0x04,
                    0x05,
                    0x06,
                    0x07,
                    0x08,
                    0x09,
                    0x0a,
                    0x00 // length
                };

                REQUIRE(data == expected);
            }
        }

        SECTION("Decode responses")
        {
            SECTION("FirmwareVersion")
            {
                std::vector<uint8_t> input{
                    static_cast<std::underlying_type<DfuOpcode>::type>(DfuOpcode::NRF_DFU_OP_FIRMWARE_VERSION),
                    static_cast<std::underlying_type<DfuResult>::type>(DfuResult::NRF_DFU_RES_CODE_OPERATION_FAILED),
                    static_cast<std::underlying_type<DfuFirmwareType>::type>(
                        DfuFirmwareType::NRF_DFU_FIRMWARE_TYPE_BOOTLOADER),
                    0x01, // Version
                    0x00,
                    0x00,
                    0x00,
                    0x02, // Address
                    0x00,
                    0x00,
                    0x00,
                    0xff, // Length
                    0x00,
                    0x00,
                    0x00};

                DfuResponse resp;
                REQUIRE(codec.decode(input, resp) == NRFDL_ERR_NONE);
                REQUIRE(resp.opcode == DfuOpcode::NRF_DFU_OP_FIRMWARE_VERSION);
                REQUIRE(resp.result == DfuResult::NRF_DFU_RES_CODE_OPERATION_FAILED);
                auto firmware = std::get<DfuResponseFirmware>(*(resp.response));
                REQUIRE(firmware.version == 0x01);
                REQUIRE(firmware.addr == 0x02);
                REQUIRE(firmware.len == 0xff);
            }

            SECTION("MTU")
            {
                std::vector<uint8_t> input{
                    static_cast<std::underlying_type<DfuOpcode>::type>(DfuOpcode::NRF_DFU_OP_MTU_GET),
                    static_cast<std::underlying_type<DfuResult>::type>(DfuResult::NRF_DFU_RES_CODE_SUCCESS),
                    0x64, // Size
                    0x00};

                DfuResponse resp;
                REQUIRE(codec.decode(input, resp) == NRFDL_ERR_NONE);
                REQUIRE(resp.opcode == DfuOpcode::NRF_DFU_OP_MTU_GET);
                REQUIRE(resp.result == DfuResult::NRF_DFU_RES_CODE_SUCCESS);
                const auto mtuResponse = std::get<DfuResponseMtu>(*(resp.response));
                REQUIRE(mtuResponse.size == 100);
            }

            SECTION("MTU - missing data")
            {
                std::vector<uint8_t> input{
                    static_cast<std::underlying_type<DfuOpcode>::type>(DfuOpcode::NRF_DFU_OP_MTU_GET),
                    static_cast<std::underlying_type<DfuResult>::type>(DfuResult::NRF_DFU_RES_CODE_SUCCESS)};

                DfuResponse resp;
                REQUIRE(codec.decode(input, resp) == NRFDL_ERR_PROTOCOL);
                REQUIRE(resp.opcode == DfuOpcode::NRF_DFU_OP_MTU_GET);
                REQUIRE(resp.result == DfuResult::NRF_DFU_RES_CODE_SUCCESS);
            }
        }
    }

}; // namespace
