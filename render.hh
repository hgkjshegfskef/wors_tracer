#pragma once

namespace wt {

class canvas;
struct camera;
struct world;

canvas render_ppm(camera const& camera, world const& world, char const* ppm_fname,
                  unsigned frames) noexcept;
void render_sdl(camera const& camera, world const& world) noexcept;

} // namespace wt
