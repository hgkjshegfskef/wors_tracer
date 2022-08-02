#include "scene.hh"

#include "cli.hh"
#include "shape.hh"

#include <spdlog/spdlog.h>

#include <cstdlib>

namespace wt {

scene get_scene(unsigned num, cli const& cli) noexcept {
    switch (num) {
    case 1:
        return scene_1(cli);
    default:
        SPDLOG_ERROR("Incorrect scene number. Available numbers: {}--{}", 1, num);
        std::exit(1);
    }
}

} // namespace wt
