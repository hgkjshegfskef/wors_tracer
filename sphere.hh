#pragma once

#include "material.hh"
#include "tform4.hh"

#include <vector>

namespace wt {

class tform4;
struct material;
struct vec3;
struct pnt3;
struct ray;
struct intersection;

// Sphere has radius 1 and is centered at the world origin (0,0,0).
struct sphere {
    tform4 tform;
    tform4 inv_tform;
    struct material material;

    sphere() noexcept;
    sphere(tform4 const& tform) noexcept;
    sphere(tform4 const& tform, struct material const& material) noexcept;

    tform4 const& get_tform() const noexcept { return tform; }
    tform4 const& get_inv_tform() const noexcept { return inv_tform; }
    struct material const& get_material() const noexcept { return material; }

    tform4& get_tform() noexcept { return tform; }
    tform4& get_inv_tform() noexcept { return inv_tform; }
    struct material& get_material() noexcept { return material; }

    //    void set_tform(tform4 t) noexcept { tform = std::move(t); }
    //    void set_inv_tform(tform4 inv_t) noexcept { inv_tform = std::move(inv_t); }
    //    void set_material(struct material m) noexcept { material = std::move(m); }

    void intersect(ray const& object_ray, unsigned shape_id,
                   std::vector<intersection>& world_isecs) const noexcept;
};

} // namespace wt
