#include "catch.hpp"
#include "fmt/format.h"

#include "sdfu/sdfu_codec.h"
#include "sdfu/sdfu_types.h"

using namespace NRFDL::SDFU;

namespace
{
    TEST_CASE("Test encoding", "[sdfu]")
    {
        Codec codec;

        SECTION("Encode requests")
        {
            DfuRequest req;
            NRFDL::data_t data;

            req.opcode   = DfuOpcode::NRF_DFU_OP_MTU_GET;
            req.mtu.size = 100;

            REQUIRE(codec.encode(req, data) == NRFDL_ERR_NONE);
            REQUIRE(data.size() == 3);

            req.opcode                = DfuOpcode::NRF_DFU_OP_FIRMWARE_VERSION;
            req.firmware.image_number = 5;
            REQUIRE(codec.encode(req, data) == NRFDL_ERR_NONE);
            REQUIRE(data.size() == 2);

            req.opcode = DfuOpcode::NRF_DFU_OP_OBJECT_WRITE;
            req.write.data.push_back(0);
            req.write.len = 10;

            REQUIRE(codec.encode(req, data) == NRFDL_ERR_NONE);
            REQUIRE(data.size() == 13);
        }

        SECTION("Decode responses")
        {
            NRFDL::data_t input{
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
                0x00

            };
            DfuResponse resp;
            REQUIRE(codec.decode(input, resp) == NRFDL_ERR_NONE);
            REQUIRE(resp.opcode == DfuOpcode::NRF_DFU_OP_FIRMWARE_VERSION);
            REQUIRE(resp.result == DfuResult::NRF_DFU_RES_CODE_OPERATION_FAILED);
            REQUIRE(resp.firmware.version == 0x01);
            REQUIRE(resp.firmware.addr == 0x02);
            REQUIRE(resp.firmware.len == 0xff);
        }
    }

}; // namespace