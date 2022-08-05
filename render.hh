#pragma once

namespace wt {

class canvas;
struct camera;
struct world;
struct cli;

void render_ppm(camera const& camera, world const& world) noexcept;
void render_sdl(camera& camera, world const& world, cli const& cli) noexcept;

} // namespace wt
