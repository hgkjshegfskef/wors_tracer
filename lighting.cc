#include "lighting.hh"

#include "tform4.hh"

#include <cmath> // pow

namespace wt {

color lighting(material const& material, pnt_light const& light, pnt3 const& point,
               vec3 const& eye_vector, vec3 const& normal_vector) noexcept {
    // combine the surface color with the light's color/intensity
    color effective_color = material.col * light.intensity;

    // find the direction of the light source
    vec3 light_vector = normalize(light.position - point);

    // compute the ambient contribution
    color ambient = effective_color * material.ambient;

    // light_dot_normal represents the cosine of the angle between the light vector and the normal
    // vector. A negative number means the light is on the other side of the surface.
    float light_dot_normal = dot(light_vector, normal_vector);

    color diffuse{0.f, 0.f, 0.f};  // black
    color specular{0.f, 0.f, 0.f}; // black

    if (light_dot_normal >= 0.) {
        // compute diffuse contribution
        diffuse = effective_color * material.diffuse * light_dot_normal;

        // reflect_dot_eye represents the cosine of the angle between the reflection vector and the
        // eye vector. A negative number means the light reflects away from the eye.
        vec3 reflect_vector = normalize(tform4(reflection(normal_vector)) * -light_vector);
        float reflect_dot_eye = dot(reflect_vector, eye_vector);

        if (reflect_dot_eye > 0.) {
            // compute specular contribution
            specular =
                light.intensity * material.specular * std::pow(reflect_dot_eye, material.shininess);
        }
    }

    return ambient + diffuse + specular;
}

} // namespace wt