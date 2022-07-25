#pragma once

#include "color.hh"
#include "material.hh"
#include "pnt3.hh"
#include "pnt_light.hh"
#include "vec3.hh"

namespace wt {

color lighting(material const& material, pnt_light const& light, pnt3 const& point,
               vec3 const& eye_vector, vec3 const& normal_vector) noexcept;

} // namespace wt
