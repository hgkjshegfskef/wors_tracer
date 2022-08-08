#pragma once

#include "sphere.hh"

#include <memory>

namespace wt {

class tform4;
struct material;

struct spheres {
    std::unique_ptr<tform4[]> tforms;
    std::unique_ptr<tform4[]> inv_tforms;
    std::unique_ptr<material[]> materials;

    unsigned size;
    unsigned capacity;

    spheres() noexcept;
    explicit spheres(unsigned max_spheres) noexcept;

    bool add_sphere(sphere sphere) noexcept;
};

} // namespace wt
