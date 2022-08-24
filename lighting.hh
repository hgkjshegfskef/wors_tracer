#pragma once

namespace wt {

struct color;
struct material;
struct pnt_light;
struct light;
struct pnt3;
struct vec3;
class shape;

color lighting(material const& material, shape const& shape, pnt_light const& light,
               pnt3 const& point, vec3 const& eye_vector, vec3 const& normal_vector,
               bool in_shadow = false) noexcept;

} // namespace wt
