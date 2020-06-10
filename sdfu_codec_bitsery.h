#include "sdfu_types.h"

#include <bitsery/bitsery.h>
#include <iostream>

#include <bitsery/ext/std_optional.h>
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

    template <typename S> void serialize(S & s, DfuResponse & obj)
    {
        s.ext(obj, bitsery::ext::DfuResponseExt{});
    };

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
        s.ext(o, bitsery::ext::DfuRequestWriteExt{});
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
        s.ext(o, bitsery::ext::DfuRequestExt{});
    }
} // namespace NRFDL::SDFU

namespace bitsery
{
    namespace ext
    {
        class DfuRequestWriteExt
        {
          public:
            template <typename Ser, typename T, typename Fnc> void serialize(Ser & s, const T & o, Fnc && fnc) const
            {
                for (auto const & value : o.data)
                {
                    s.value1b(value);
                }

                s.value2b(o.len);
            }

            // template <typename Des, typename T, typename Fnc> void deserialize(Des & des, T & obj, Fnc && fnc) const
            template <typename Des, typename T, typename Fnc> void deserialize(Des & s, T & obj, Fnc && fnc) const
            {
                T res{};
                fnc(s, res);
            }
        };

        class DfuRequestExt
        {
          public:
            template <typename Ser, typename T, typename Fnc> void serialize(Ser & ser, const T & obj, Fnc && fnc) const
            {
                // TODO: check that this function is receving the wrapper
                ser.value1b(obj.opcode);
                switch (obj.opcode)
                {
                    case DfuOpcode::NRF_DFU_OP_FIRMWARE_VERSION:
                        if (obj.request)
                        {
                            ser.object(std::get<DfuRequestFirmware>(*(obj.request)));
                        }
                        break;
                    case DfuOpcode::NRF_DFU_OP_OBJECT_CREATE:
                        if (obj.request)
                        {
                            ser.object(std::get<DfuRequestCreate>(*(obj.request)));
                        }
                        break;
                    case DfuOpcode::NRF_DFU_OP_RECEIPT_NOTIF_SET:
                        if (obj.request)
                        {
                            ser.object(std::get<DfuRequestPrn>(*(obj.request)));
                        }
                        break;

                    case DfuOpcode::NRF_DFU_OP_OBJECT_SELECT:
                        if (obj.request)
                        {
                            ser.object(std::get<DfuRequestSelect>(*(obj.request)));
                        }
                        break;
                    case DfuOpcode::NRF_DFU_OP_MTU_GET:
                        if (obj.request)
                        {
                            ser.object(std::get<DfuRequestMtu>(*(obj.request)));
                        }
                        break;
                    case DfuOpcode::NRF_DFU_OP_OBJECT_WRITE:
                        if (obj.request)
                        {
                            ser.object(std::get<DfuRequestWrite>(*(obj.request)));
                        }
                        break;
                    case DfuOpcode::NRF_DFU_OP_PING:
                        if (obj.request)
                        {
                            ser.object(std::get<DfuRequestPing>(*(obj.request)));
                        }
                        break;
                    case DfuOpcode::NRF_DFU_OP_OBJECT_EXECUTE:
                    case DfuOpcode::NRF_DFU_OP_HARDWARE_VERSION:
                    case DfuOpcode::NRF_DFU_OP_ABORT:
                    case DfuOpcode::NRF_DFU_OP_RESPONSE:
                    case DfuOpcode::NRF_DFU_OP_INVALID:
                    case DfuOpcode::NRF_DFU_OP_PROTOCOL_VERSION:
                    case DfuOpcode::NRF_DFU_OP_CRC_GET:
                        // opcodes without any extra arguments
                        break;
                }
            }

            // template <typename Des, typename T, typename Fnc> void deserialize(Des & des, T & obj, Fnc && fnc) const
            template <typename Des, typename T, typename Fnc> void deserialize(Des & s, T & obj, Fnc && fnc) const
            {
                T res{};
                fnc(s, res);
            }
        };

        class DfuResponseExt
        {
          public:
            template <typename Ser, typename T, typename Fnc> void serialize(Ser & ser, const T & obj, Fnc && fnc) const
            {
                ser.value1b(obj.opcode);
                ser.value1b(obj.result);

                switch (obj.opcode)
                {
                    case DfuOpcode::NRF_DFU_OP_PROTOCOL_VERSION:
                        ser.object(std::get<DfuResponseProtocol>(*obj.response));
                        break;
                    case DfuOpcode::NRF_DFU_OP_OBJECT_CREATE:
                        ser.object(std::get<DfuResponseCreate>(*obj.response));
                        break;
                    case DfuOpcode::NRF_DFU_OP_RECEIPT_NOTIF_SET:
                        break;
                    case DfuOpcode::NRF_DFU_OP_CRC_GET:
                        ser.object(std::get<DfuResponseCrc>(*obj.response));
                        break;
                    case DfuOpcode::NRF_DFU_OP_OBJECT_EXECUTE:
                        break;
                    case DfuOpcode::NRF_DFU_OP_OBJECT_SELECT:
                        ser.object(std::get<DfuResponseSelect>(*obj.response));
                        break;
                    case DfuOpcode::NRF_DFU_OP_MTU_GET:
                        ser.object(std::get<DfuResponseSelect>(*obj.response));
                        break;
                    case DfuOpcode::NRF_DFU_OP_OBJECT_WRITE:
                        ser.object(std::get<DfuResponseWrite>(*obj.response));
                        break;
                    case DfuOpcode::NRF_DFU_OP_PING:
                        ser.object(std::get<DfuResponsePing>(*obj.response));
                        break;
                    case DfuOpcode::NRF_DFU_OP_HARDWARE_VERSION:
                        ser.object(std::get<DfuResponseHardware>(*obj.response));
                        break;
                    case DfuOpcode::NRF_DFU_OP_FIRMWARE_VERSION:
                        ser.object(std::get<DfuResponseFirmware>(*obj.response));
                        break;
                    case DfuOpcode::NRF_DFU_OP_ABORT:
                    case DfuOpcode::NRF_DFU_OP_RESPONSE:
                    case DfuOpcode::NRF_DFU_OP_INVALID:
                        break;
                }
            };

            template <typename Des, typename T, typename Fnc> void deserialize(Des & s, T & obj, Fnc && fnc) const
            {
                s.value1b(obj.opcode);
                s.value1b(obj.result);

                switch (obj.opcode)
                {
                    case DfuOpcode::NRF_DFU_OP_PROTOCOL_VERSION:
                        obj.response = DfuResponseProtocol{};
                        s.object(std::get<DfuResponseProtocol>(*obj.response));
                        break;

                    case DfuOpcode::NRF_DFU_OP_OBJECT_CREATE:
                        obj.response = DfuResponseCreate{};
                        s.object(std::get<DfuResponseCreate>(*obj.response));
                        break;

                    case DfuOpcode::NRF_DFU_OP_CRC_GET:
                        obj.response = DfuResponseCrc{};
                        s.object(std::get<DfuResponseCrc>(*obj.response));
                        break;

                    case DfuOpcode::NRF_DFU_OP_OBJECT_SELECT:
                        obj.response = DfuResponseSelect{};
                        s.object(std::get<DfuResponseSelect>(*obj.response));
                        break;

                    case DfuOpcode::NRF_DFU_OP_MTU_GET:
                        obj.response = DfuResponseMtu{};
                        s.object(std::get<DfuResponseMtu>(*obj.response));
                        break;

                    case DfuOpcode::NRF_DFU_OP_OBJECT_WRITE:
                        obj.response = DfuResponseWrite{};
                        s.object(std::get<DfuResponseWrite>(*obj.response));
                        break;

                    case DfuOpcode::NRF_DFU_OP_PING:
                        obj.response = DfuResponsePing{};
                        s.object(std::get<DfuResponsePing>(*obj.response));
                        break;

                    case DfuOpcode::NRF_DFU_OP_HARDWARE_VERSION:
                        obj.response = DfuResponseHardware{};
                        s.object(std::get<DfuResponseHardware>(*obj.response));
                        break;

                    case DfuOpcode::NRF_DFU_OP_FIRMWARE_VERSION:
                        obj.response = DfuResponseFirmware{};
                        s.object(std::get<DfuResponseFirmware>(*obj.response));
                        break;
                }
            }
        };
    }; // namespace ext

    namespace traits
    {
        template <typename T> struct ExtensionTraits<ext::DfuRequestExt, T>
        {
            static_assert(std::is_same_v<T, DfuRequest>, "Only works with DfuRequest type");

            using TValue                                = DfuRequest;
            static constexpr bool SupportValueOverload  = false;
            static constexpr bool SupportObjectOverload = true;
            static constexpr bool SupportLambdaOverload = false;
        };

        template <typename T> struct ExtensionTraits<ext::DfuResponseExt, T>
        {
            static_assert(std::is_same_v<T, DfuResponse>, "Only works with DfuResponse type");

            using TValue                                = DfuResponse;
            static constexpr bool SupportValueOverload  = false;
            static constexpr bool SupportObjectOverload = true;
            static constexpr bool SupportLambdaOverload = true;
        };

        template <typename T> struct ExtensionTraits<ext::DfuRequestWriteExt, T>
        {
            static_assert(std::is_same_v<T, DfuRequestWrite>, "Only works with DfuRequestWrite type");

            using TValue                                = DfuRequestWrite;
            static constexpr bool SupportValueOverload  = false;
            static constexpr bool SupportObjectOverload = true;
            static constexpr bool SupportLambdaOverload = true;
        };

    } // namespace traits
} // namespace bitsery
