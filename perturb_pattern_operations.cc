#include "perturb_pattern_operations.hh"

#include "color.hh"
#include "pattern_operations.hh"
#include "perlin_noise.hh"
#include "perturb_pattern.hh"
#include "pnt3.hh"
#include "shape.hh"
#include "stub_shape.hh"
#include "tform4.hh"

namespace wt {

tform4& inv_tform(perturb_pattern& pattern) noexcept { return pattern.inv_tform; }
tform4 const& inv_tform(perturb_pattern const& pattern) noexcept { return pattern.inv_tform; }

color pattern_at(perturb_pattern const& pattern, pnt3 const& world_point) noexcept {
    shape stub{stub_shape{}};
    float const jitter = octave_perlin_noise(world_point.x, world_point.y, world_point.z,
                                             pattern.octaves, pattern.persistence);
    return pattern_at(pattern.pattern, stub,
                      {world_point.x + pattern.scale * jitter,
                       world_point.y + pattern.scale * jitter,
                       world_point.z + pattern.scale * jitter});
}

} // namespace wt
