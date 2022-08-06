#include "shape.hh"

#include "intersection.hh"
#include "pnt3.hh"
#include "ray.hh"
#include "shape_concept.hh"
#include "shape_model.hh"
#include "sphere.hh"
#include "vec3.hh"

namespace wt {

tform4& get_tform(shape& shape) noexcept { return shape.pimpl_->do_get_tform(); }
tform4 const& get_tform(shape const& shape) noexcept { return shape.pimpl_->do_get_tform(); }

material& get_material(shape& shape) noexcept { return shape.pimpl_->do_get_material(); }
material const& get_material(shape const& shape) noexcept {
    return shape.pimpl_->do_get_material();
}

vec3 normal_at(shape const& shape, pnt3 const& world_point) noexcept {
    return shape.pimpl_->do_normal_at(world_point);
}

std::array<intersection, 2> intersect(shape const& shape, ray const& world_ray) noexcept {
    return shape.pimpl_->do_intersect(world_ray);
}

template <typename ConcreteShape,
          std::enable_if_t<!std::is_same_v<shape, std::decay_t<ConcreteShape>>, bool>>
shape::shape(ConcreteShape&& concrete_shape)
    // Strip the reference, so that either a copy or a move occurs, but not reference binding.
    : pimpl_{std::make_unique<shape_model<std::remove_reference_t<ConcreteShape>>>(
          std::forward<std::remove_reference_t<ConcreteShape>>(concrete_shape))} {}

template shape::shape(sphere&);
template shape::shape(sphere&&);

shape::shape(const shape& other) noexcept : pimpl_{other.pimpl_->clone()} {}

shape& shape::operator=(const shape& rhs) noexcept {
    if (&rhs != this) {
        pimpl_ = rhs.pimpl_->clone();
    }
    return *this;
}

shape::shape(shape&&) noexcept = default;
shape& shape::operator=(shape&&) noexcept = default;

shape::~shape() noexcept = default;

} // namespace wt
