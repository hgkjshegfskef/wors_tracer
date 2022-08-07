#include "spheres.hh"

#include "material.hh"
#include "tform4.hh"

namespace wt {

spheres::spheres() noexcept = default;

spheres::spheres(unsigned max_spheres) noexcept
    : tforms{std::make_unique_for_overwrite<tform4[]>(max_spheres)},
      materials{std::make_unique_for_overwrite<material[]>(max_spheres)}, size{0},
      capacity{max_spheres} {}

bool spheres::add_sphere(wt::sphere sphere) noexcept {
    if (size == capacity) {
        return false;
    }
    tforms[size] = std::move(sphere.tform);
    materials[size] = std::move(sphere.material);
    ++size;
    return true;
}

} // namespace wt
