#include "color.hh"

#include <fmt/format.h> // formatter

namespace wt {

color::color() noexcept = default;

color::color(float r, float g, float b) noexcept : r{r}, g{g}, b{b} {}

color::color(std::uint32_t rgb_hex) noexcept
    : r{((rgb_hex >> 16) & 0xFF) / 255.f}, g{((rgb_hex >> 8) & 0xFF) / 255.f}, b{((rgb_hex)&0xFF) /
                                                                                 255.f} {}

float& color::operator[](std::size_t idx) noexcept {
    switch (idx) {
    case 0:
        return r;
    case 1:
        return g;
    case 2:
        return b;
    default:
        __builtin_unreachable();
    }
}

float const& color::operator[](std::size_t idx) const noexcept {
    switch (idx) {
    case 0:
        return r;
    case 1:
        return g;
    case 2:
        return b;
    default:
        __builtin_unreachable();
    }
}

color& color::operator*=(float s) noexcept {
    r *= s;
    g *= s;
    b *= s;
    return *this;
}

color& color::operator/=(float s) noexcept {
    s = 1.0f / s;
    r *= s;
    g *= s;
    b *= s;
    return *this;
}

color& color::operator+=(const color& c) noexcept {
    r += c.r;
    g += c.g;
    b += c.b;
    return *this;
}

color& color::operator-=(const color& c) noexcept {
    r -= c.r;
    g -= c.g;
    b -= c.b;
    return *this;
}

color& color::operator*=(const color& c) noexcept {
    r *= c.r;
    g *= c.g;
    b *= c.b;
    return *this;
}

color::const_iterator color::cbegin() const noexcept { return &(operator[](0)); }
color::const_iterator color::cend() const noexcept { return &(operator[](2)) + 1; }
color::const_iterator color::begin() const noexcept { return cbegin(); }
color::const_iterator color::end() const noexcept { return cend(); }
color::iterator color::begin() noexcept { return &(operator[](0)); }
color::iterator color::end() noexcept { return &(operator[](2)) + 1; }

color operator*(const color& c, float s) noexcept { return {c.r * s, c.g * s, c.b * s}; }

color operator/(const color& c, float s) noexcept {
    s = 1.f / s;
    return {c.r * s, c.g * s, c.b * s};
}

color operator+(const color& a, const color& b) noexcept {
    return {a.r + b.r, a.g + b.g, a.b + b.b};
}

color operator-(const color& a, const color& b) noexcept {
    return {a.r - b.r, a.g - b.g, a.b - b.b};
}

color operator*(const color& a, const color& b) noexcept {
    return {a.r * b.r, a.g * b.g, a.b * b.b};
}

bool operator==(color const& l, color const& r) noexcept {
    return std::tie(l.r, l.g, l.b) == std::tie(r.r, r.g, r.b);
}

} // namespace wt

auto fmt::formatter<wt::color>::format(const wt::color& c, fmt::format_context& ctx)
    -> decltype(ctx.out()) {
    auto&& out = ctx.out();
    fmt::formatter<float>::format(c.r, ctx);
    fmt::format_to(out, " ");
    fmt::formatter<float>::format(c.g, ctx);
    fmt::format_to(out, " ");
    fmt::formatter<float>::format(c.b, ctx);
    return out;
}

template struct fmt::formatter<wt::color>;
