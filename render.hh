#pragma once

namespace wt {

class canvas;
struct cli;
struct scene;
struct world;
struct look_at;
struct camera;

void render_ppm(cli const& cli, scene const& scene, canvas& image, size_t frames = 1) noexcept;

void render_sdl(cli const& cli, world const& world, look_at const& look_at,
                camera& camera) noexcept;

} // namespace wt
