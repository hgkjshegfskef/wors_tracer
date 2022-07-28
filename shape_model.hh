#pragma once

#include "shape.hh"
#include "shape_concept.hh"
// temporary
#include "intersection.hh"
#include "pnt3.hh"
#include "ray.hh"
#include "sphere_operations.hh"
#include "vec3.hh"

#include <array>
#include <memory>

namespace wt {

template <typename ConcreteShape> class shape::shape_model final : public shape_concept {
  public:
    explicit shape_model(ConcreteShape&& concrete_shape)
        // Copy-construct or move-construct the concrete type.
        : object_{std::forward<ConcreteShape>(concrete_shape)} {}

    // Call free functions on objects implementing the shape interface.

    tform4& do_get_tform() noexcept override { return get_tform(object_); }
    tform4 const& do_get_tform() const noexcept override { return get_tform(object_); }

    material& do_get_mat() noexcept override { return get_mat(object_); }
    material const& do_get_mat() const noexcept override { return get_mat(object_); }

    vec3 do_normal_at(pnt3 const& world_point) const noexcept override {
        return normal_at(object_, world_point);
    }

    std::array<intersection, 2> do_intersect(ray const& world_ray) const noexcept override {
        return intersect(object_, world_ray);
    }

    [[nodiscard]] std::unique_ptr<shape_concept> clone() const override {
        return std::make_unique<shape_model>(*this);
    }

  private:
    ConcreteShape object_;
};

} // namespace wt
