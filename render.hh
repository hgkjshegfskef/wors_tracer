#pragma once

namespace wt {

class canvas;
struct camera;
struct world;

canvas render(camera const& camera, world const& world) noexcept;

} // namespace wt
