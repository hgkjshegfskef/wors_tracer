#include "log.hh"

#include <spdlog/spdlog.h>

using namespace wt;

int main() {
    setup_logging();
    SPDLOG_DEBUG("Hello, {}!", "World");
    SPDLOG_TRACE("Hello, {}!", "World");
    SPDLOG_INFO("Hello, {}!", "World");
}
