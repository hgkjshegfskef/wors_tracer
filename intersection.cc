#include "intersection.hh"

#include "sphere.hh"

namespace wt {

intersection::intersection() noexcept = default;

intersection::intersection(unsigned shape_id, float t) noexcept
    : shape_id{std::move(shape_id)}, t{std::move(t)} {}

bool operator<(intersection const l, intersection const r) noexcept {
    if (l.t < r.t) {
        return true;
    }
    return false;
}

} // namespace wt

// auto fmt::formatter<wt::intersection>::format(const wt::intersection& i, fmt::format_context&
// ctx)
//     -> decltype(ctx.out()) {
//     auto&& out = ctx.out();
//     fmt::format_to(out, "{}, {}", i.t);
//     return out;
// }
//
// template struct fmt::formatter<wt::intersection>;
