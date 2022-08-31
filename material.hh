#pragma once

#include "color.hh"
#include "pattern.hh"

#include <optional>

namespace wt {

struct material {
    std::optional<class pattern> pattern{std::nullopt};
    color col{1.f, 1.f, 1.f};
    float ambient{.1f};
    float diffuse{.9f};
    float specular{.9f};
    float shininess{200.f};
    float reflective{0.f};
    float transparency{0.f};
    float refractive{1.f};

    static material glass() noexcept;
};

} // namespace wt
