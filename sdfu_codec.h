#pragma once

#include <vector>

using data_t = std::vector<uint8_t>;

#include "nrfdl_types.h"
#include "sdfu_types.h"

#include <memory>

#include <spdlog/spdlog.h>

namespace NRFDL::SDFU
{
    class Codec
    {
      public:
        Codec();

        auto encode(const DfuRequest & request, data_t & data) -> nrfdl_errorcode_t;
        auto decode(const data_t & data, DfuResponse & response) -> nrfdl_errorcode_t;

      private:
        std::shared_ptr<spdlog::logger> _logger;
    };
} // namespace NRFDL::SDFU
