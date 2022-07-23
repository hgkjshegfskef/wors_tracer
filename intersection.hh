#pragma once

#include "sphere.hh"

//#include <fmt/core.h>

namespace wt {

struct intersection {
    float t1;
    float t2;

    intersection() noexcept = default;
    intersection(float t1, float t2) noexcept;
};

} // namespace wt

//template <> struct fmt::formatter<wt::intersection> : fmt::formatter<float> {
//    auto format(wt::intersection const& i, fmt::format_context& ctx) -> decltype(ctx.out());
//};
//
//extern template struct fmt::formatter<wt::intersection>;
