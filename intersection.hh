#pragma once

namespace wt {

struct sphere;

struct intersection {
    sphere const* s{};
    float t{};

    intersection() noexcept;
    intersection(sphere const* s, float t) noexcept;

    bool empty() const noexcept;
};

bool operator<(intersection const& l, intersection const& r) noexcept;

} // namespace wt

// template <> struct fmt::formatter<wt::intersection> : fmt::formatter<float> {
//     auto format(wt::intersection const& i, fmt::format_context& ctx) -> decltype(ctx.out());
// };
//
// extern template struct fmt::formatter<wt::intersection>;
