#include "shape_model.hh"

#include "intersection.hh"
#include "vec3.hh"

namespace wt {

template <typename ConcreteShape>
shape::shape_model<ConcreteShape>::shape_model(ConcreteShape&& concrete_shape) noexcept
    // Copy-construct or move-construct the concrete type.
    : object_{std::forward<ConcreteShape>(concrete_shape)} {}

template <typename ConcreteShape>
tform4& shape::shape_model<ConcreteShape>::do_get_tform() noexcept {
    return get_tform(object_);
}

template <typename ConcreteShape>
tform4 const& shape::shape_model<ConcreteShape>::do_get_tform() const noexcept {
    return get_tform(object_);
}

template <typename ConcreteShape>
material& shape::shape_model<ConcreteShape>::do_get_material() noexcept {
    return get_material(object_);
}

template <typename ConcreteShape>
material const& shape::shape_model<ConcreteShape>::do_get_material() const noexcept {
    return get_material(object_);
}

template <typename ConcreteShape>
vec3 shape::shape_model<ConcreteShape>::do_normal_at(const pnt3& world_point) const noexcept {
    return normal_at(object_, world_point);
}

template <typename ConcreteShape>
std::array<intersection, 2>
shape::shape_model<ConcreteShape>::do_intersect(const ray& world_ray) const noexcept {
    return intersect(object_, world_ray);
}

template <typename ConcreteShape>
std::unique_ptr<shape::shape_concept> shape::shape_model<ConcreteShape>::clone() const noexcept {
    return std::make_unique<class shape_model>(*this);
}

template class shape::shape_model<sphere>;

} // namespace wt
