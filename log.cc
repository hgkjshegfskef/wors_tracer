#include "log.hh"

#include <spdlog/cfg/env.h>
#include <spdlog/spdlog.h>

namespace wt {

void setup_logging() {
    spdlog::set_level(spdlog::level::trace);
    spdlog::cfg::load_env_levels();
}

} // namespace wt
