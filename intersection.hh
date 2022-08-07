#pragma once

#include <cstddef>

namespace wt {

struct sphere;

struct intersection {
    unsigned shape_id;
    float t;

    intersection() noexcept;
    intersection(unsigned shape_id, float t) noexcept;
};

bool operator<(intersection const l, intersection const r) noexcept;

} // namespace wt

// template <> struct fmt::formatter<wt::intersection> : fmt::formatter<float> {
//     auto format(wt::intersection const& i, fmt::format_context& ctx) -> decltype(ctx.out());
// };
//
// extern template struct fmt::formatter<wt::intersection>;
