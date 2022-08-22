#include "scene.hh"

#include "cli.hh"
#include "shape.hh"

#include <spdlog/spdlog.h>

#include <cstdlib>

namespace wt {

scene get_scene(unsigned num, cli const& cli) noexcept {
    unsigned const min_scene = 0;
    unsigned const max_scene = 2;
    switch (num) {
    case 0:
        return scene_0(cli);
    case 1:
        return scene_1(cli);
    case 2:
        return scene_2(cli);
    default:
        SPDLOG_ERROR("Incorrect scene number. Available numbers: {}--{}", min_scene, max_scene);
        std::exit(1);
    }
}

} // namespace wt
