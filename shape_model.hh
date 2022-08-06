#pragma once

#include "shape.hh"
#include "shape_concept.hh"
#include "sphere.hh"

#include <array>
#include <memory>

namespace wt {

class tform4;
struct material;
struct vec3;
class shape_concept;

template <typename ConcreteShape> class shape::shape_model final : public shape::shape_concept {
  public:
    explicit shape_model(ConcreteShape&& concrete_shape) noexcept;

    [[nodiscard]] std::unique_ptr<shape::shape_concept> clone() const noexcept override;

    // Call free functions on objects implementing the shape interface.

    tform4& do_get_tform() noexcept override;
    tform4 const& do_get_tform() const noexcept override;

    material& do_get_material() noexcept override;
    material const& do_get_material() const noexcept override;

    vec3 do_normal_at(pnt3 const& world_point) const noexcept override;

    std::array<intersection, 2> do_intersect(ray const& world_ray) const noexcept override;

  private:
    ConcreteShape object_;
};

extern template class shape::shape_model<sphere>;

} // namespace wt
