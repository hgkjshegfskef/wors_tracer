#include "scene.hh"

#include <utility> // move

namespace wt {

scene::scene() noexcept = default;

scene::scene(struct world world, struct camera camera, struct look_at look_at) noexcept
    : world{std::move(world)}, camera{std::move(camera)}, look_at{std::move(look_at)} {}

} // namespace wt
