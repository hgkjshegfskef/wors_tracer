#include "log.hh"

#include <spdlog/cfg/env.h>
#include <spdlog/spdlog.h>

namespace wt {

void setup_logging() {
    // By default, print all log messages (trace level). Note that this is only the runtime setting,
    // and logging of all levels will still be compiled into the executable regardless of this
    // setting. To change logging level at compile time, i.e. to omit compilation of certain levels,
    // use SPDLOG_ACTIVE_LEVEL definition before including spdlog headers. Available values are:
    // SPDLOG_LEVEL_TRACE, SPDLOG_LEVEL_DEBUG, SPDLOG_LEVEL_INFO, SPDLOG_LEVEL_WARN,
    // SPDLOG_LEVEL_ERROR, SPDLOG_LEVEL_CRITICAL, SPDLOG_LEVEL_OFF
    spdlog::set_level(spdlog::level::trace);

    // To change the default level at runtime, use SPDLOG_LEVEL=level environment variable on
    // executable invocation.
    spdlog::cfg::load_env_levels();

    //    SPDLOG_TRACE("ON");
    //    SPDLOG_DEBUG("ON");
    //    SPDLOG_INFO("ON");
    //    SPDLOG_WARN("ON");
    //    SPDLOG_ERROR("ON");
    //    SPDLOG_CRITICAL("ON");
}

} // namespace wt
