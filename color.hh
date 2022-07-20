#pragma once

#include <fmt/core.h> // fwd

#include <cstddef> // size_t

namespace wt {

struct color {
    float r, g, b;

    color() noexcept = default;
    color(float r, float g, float b) noexcept;

    float& operator[](std::size_t idx) noexcept;
    float const& operator[](std::size_t idx) const noexcept;

    color& operator*=(float s) noexcept;
    color& operator/=(float s) noexcept;

    color& operator+=(color const& c) noexcept;
    color& operator-=(color const& c) noexcept;
    color& operator*=(color const& c) noexcept;

    using iterator = float*;
    using const_iterator = const float*;
    const_iterator cbegin() const noexcept;
    const_iterator cend() const noexcept;
    const_iterator begin() const noexcept;
    const_iterator end() const noexcept;
    iterator begin() noexcept;
    iterator end() noexcept;
};

color operator*(const color& c, float s) noexcept;
color operator/(const color& c, float s) noexcept;

color operator+(const color& a, const color& b) noexcept;
color operator-(const color& a, const color& b) noexcept;
color operator*(const color& a, const color& b) noexcept;

} // namespace wt

template <> struct fmt::formatter<wt::color> : fmt::formatter<float> {
    auto format(wt::color const& c, fmt::format_context& ctx) -> decltype(ctx.out());
};

extern template struct fmt::formatter<wt::color>;
