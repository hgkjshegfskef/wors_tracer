#include "intersection.hh"

//#include <fmt/format.h>

namespace wt {

intersection::intersection(float t1, float t2) noexcept : t1{t1}, t2{t2} {}

} // namespace wt

//auto fmt::formatter<wt::intersection>::format(const wt::intersection& i, fmt::format_context& ctx)
//    -> decltype(ctx.out()) {
//    auto&& out = ctx.out();
//    fmt::format_to(out, "{}, {}", i.t);
//    return out;
//}
//
//template struct fmt::formatter<wt::intersection>;
