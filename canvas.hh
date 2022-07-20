#pragma once

#include "color.hh"
#include "string_fwd.hh" // string

#include <fmt/core.h> // fwd

#include <memory> // unique_ptr

namespace wt {

struct color;
class canvas {
  public:
    canvas() noexcept = default;
    canvas(unsigned w, unsigned h) noexcept;

    color& operator()(unsigned x, unsigned y) noexcept;
    color const& operator()(unsigned x, unsigned y) const noexcept;

    unsigned size() const noexcept;
    void fill(color const& c) noexcept;
    std::string as_ppm() const noexcept;

    friend struct fmt::formatter<wt::canvas>;

  private:
    std::unique_ptr<color[]> canvas_;
    unsigned w_;
    unsigned h_;
};

} // namespace wt

template <> struct fmt::formatter<wt::canvas> : fmt::formatter<wt::color> {
    auto format(wt::canvas const& c, fmt::format_context& ctx) -> decltype(ctx.out());
};

extern template struct fmt::formatter<wt::canvas>;
