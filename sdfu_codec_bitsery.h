#include "sdfu_types.h"

#include <bitsery/bitsery.h>
#include <iostream>

#include <bitsery/ext/std_variant.h>

using namespace NRFDL::SDFU;

namespace NRFDL::SDFU
{
    template <typename S> void serialize(S & s, DfuResponseProtocol & o)
    {
        s.value1b(o.version);
    };

    template <typename S> void serialize(S & s, DfuResponseHardware & o)
    {
        s.value4b(o.part);
        s.value4b(o.variant);
        s.object(o.memory);
    };

    template <typename S> void serialize(S & s, DfuResponseHardwareMemory & o)
    {
        s.value4b(o.rom_size);
        s.value4b(o.ram_size);
        s.value4b(o.rom_page_size);
    };

    template <typename S> void serialize(S & s, DfuResponseFirmware & o)
    {
        s.value1b(o.type);
        s.value4b(o.version);
        s.value4b(o.addr);
        s.value4b(o.len);
    };

    template <typename S> void serialize(S & s, DfuResponseSelect & o)
    {
        s.value4b(o.offset);
        s.value4b(o.crc);
        s.value4b(o.max_size);
    };

    template <typename S> void serialize(S & s, DfuResponseCreate & o)
    {
        s.value4b(o.offset);
        s.value4b(o.crc);
    };

    template <typename S> void serialize(S & s, DfuResponseWrite & o)
    {
        s.value4b(o.offset);
        s.value4b(o.crc);
    };

    template <typename S> void serialize(S & s, DfuResponseCrc & o)
    {
        s.value4b(o.offset);
        s.value4b(o.crc);
    };

    template <typename S> void serialize(S & s, DfuResponsePing & o)
    {
        s.value1b(o.id);
    };

    template <typename S> void serialize(S & s, DfuResponseMtu & o)
    {
        s.value2b(o.size);
    };

    template <typename S> void serialize(S & s, DfuResponse & o)
    {
        s.value1b(o.opcode);
        s.value1b(o.result);

        switch (o.opcode)
        {
            case DfuOpcode::NRF_DFU_OP_PROTOCOL_VERSION:
                s.object(o.protocol);
                break;
            case DfuOpcode::NRF_DFU_OP_OBJECT_CREATE:
                s.object(o.create);
                break;
            case DfuOpcode::NRF_DFU_OP_CRC_GET:
                s.object(o.crc);
                break;
            case DfuOpcode::NRF_DFU_OP_OBJECT_SELECT:
                s.object(o.select);
                break;
            case DfuOpcode::NRF_DFU_OP_MTU_GET:
                s.object(o.mtu);
                break;
            case DfuOpcode::NRF_DFU_OP_OBJECT_WRITE:
                s.object(o.write);
                break;
            case DfuOpcode::NRF_DFU_OP_PING:
                s.object(o.ping);
                break;
            case DfuOpcode::NRF_DFU_OP_HARDWARE_VERSION:
                s.object(o.hardware);
                break;
            case DfuOpcode::NRF_DFU_OP_FIRMWARE_VERSION:
                s.object(o.firmware);
                break;
        }
    }

    template <typename S> void serialize(S & s, DfuRequestFirmware & o)
    {
        s.value1b(o.image_number);
    };

    template <typename S> void serialize(S & s, DfuRequestSelect & o)
    {
        s.value4b(o.object_type);
    };

    template <typename S> void serialize(S & s, DfuRequestCreate & o)
    {
        s.value4b(o.object_type);
        s.value4b(o.object_size);
    };

    template <typename S> void serialize(S & s, DfuRequestWrite & o)
    {
        s.container(o.data, bitsery::FtorExtObject<bitsery::ext::MyExtension>{});
        s.value2b(o.len);
    };

    template <typename S> void serialize(S & s, DfuRequestPing & o)
    {
        s.value1b(o.id);
    };

    template <typename S> void serialize(S & s, DfuRequestMtu & o)
    {
        s.value2b(o.size);
    };

    template <typename S> void serialize(S & s, DfuRequestPrn & o)
    {
        s.value4b(o.target);
    };

    template <typename S> void serialize(S & s, DfuRequest & o)
    {
        s.value1b(o.opcode);

        // Requests without extra arguments are not extending the object
        switch (o.opcode)
        {
            case DfuOpcode::NRF_DFU_OP_FIRMWARE_VERSION:
                s.object(o.firmware);
                break;
            case DfuOpcode::NRF_DFU_OP_OBJECT_CREATE:
                s.object(o.create);
                break;
            case DfuOpcode::NRF_DFU_OP_RECEIPT_NOTIF_SET:
                s.object(o.prn);
                break;
            case DfuOpcode::NRF_DFU_OP_OBJECT_SELECT:
                s.object(o.select);
                break;
            case DfuOpcode::NRF_DFU_OP_MTU_GET:
                s.object(o.mtu);
                break;
            case DfuOpcode::NRF_DFU_OP_OBJECT_WRITE:
                s.object(o.write);
                break;
            case DfuOpcode::NRF_DFU_OP_PING:
                s.object(o.ping);
                break;
        }
    }
} // namespace NRFDL::SDFU

namespace bitsery
{
    namespace ext
    {
        class MyExtension
        {
          public:
            template <typename Ser, typename T, typename Fnc> void serialize(Ser & ser, const T & obj, Fnc && fnc) const
            {
                std::cout << "SERIALIZE"
                          << "\n";
            }

            template <typename Des, typename T, typename Fnc>
            void deserialize(Des & des, NRFDL::SDFU::DfuRequest & obj, Fnc && fnc) const
            {
                std::cout << "DESERIALIZE"
                          << "\n";
            }
        };
    } // namespace ext
} // namespace bitsery
