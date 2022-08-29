#pragma once

#include "camera.hh"
#include "look_at.hh"
#include "world.hh"

namespace wt {

struct scene {
    struct world world;
    struct camera camera;
    struct look_at look_at;

    scene() noexcept;
    scene(struct world world, struct camera camera, struct look_at look_at) noexcept;
};

struct cli;

scene scene_0(cli const& cli) noexcept;
scene scene_1(cli const& cli) noexcept;
scene scene_2(cli const& cli) noexcept;
scene scene_3(cli const& cli) noexcept;
scene scene_4(cli const& cli) noexcept;
scene scene_5(cli const& cli) noexcept;
scene scene_6(cli const& cli) noexcept;
scene scene_7(cli const& cli) noexcept;
scene scene_8(cli const& cli) noexcept;
scene scene_9(cli const& cli) noexcept;

} // namespace wt
