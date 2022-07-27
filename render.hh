#pragma once

namespace wt {

class canvas;
struct camera;
struct world;

canvas render(camera const& cam, world const& w) noexcept;

} // namespace wt
