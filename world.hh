#pragma once

#include "pnt_light.hh"
#include "sphere.hh"

#include <vector>

namespace wt {

// fwd
struct ray;

struct world {
    pnt_light light;
    std::vector<sphere> shapes;

    world() noexcept = default;
    world(pnt_light light, std::vector<sphere> spheres) noexcept;

    static world make_default() noexcept;
};

// Intersect ray from world space with the world
std::vector<float> intersect(ray const& world_r, world const& w) noexcept;

} // namespace wt
