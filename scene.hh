#pragma once

#include "camera.hh"
#include "world.hh"

namespace wt {

struct scene {
    struct world world;
    struct camera camera;
};

struct cli;

scene get_scene(unsigned num, cli const& cli) noexcept;

scene scene_0(cli const& cli) noexcept;
scene scene_1(cli const& cli) noexcept;
scene scene_2(cli const& cli) noexcept;

} // namespace wt
