#pragma once

//#include "sphere.hh"

//#include <fmt/core.h>

namespace wt {

struct sphere;

struct intersection {
    sphere const* s{};
    float t{};

    bool empty() const noexcept;

    intersection() noexcept = default;
    intersection(sphere const* s, float t) noexcept;
};

bool operator<(intersection const& l, intersection const& r) noexcept;

} // namespace wt

// template <> struct fmt::formatter<wt::intersection> : fmt::formatter<float> {
//     auto format(wt::intersection const& i, fmt::format_context& ctx) -> decltype(ctx.out());
// };
//
// extern template struct fmt::formatter<wt::intersection>;
