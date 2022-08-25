#include "util.hh"

#include "color.hh"
#include "pnt3.hh"

#include <spdlog/spdlog.h>

#include <algorithm> // clamp
#include <cerrno>    // errno
#include <cmath>     // floor
#include <cstdio>    // FILE
#include <cstdlib>   // exit
#include <cstring>   // strerror
#include <memory>    // unique_ptr
#include <numbers>
#include <utility> // pair

using namespace std::numbers;

namespace wt {

float lerp(float x, float left_min, float left_max, float right_min, float right_max) noexcept {
    return left_max + (x - left_min) * (right_max - left_max) / (right_min - left_min);
}

void write_ppm(char const* fname, std::string_view contents) noexcept {
    auto close = [](FILE* f) { std::fclose(f); };
    std::unique_ptr<FILE, decltype(close)> fp{std::fopen(fname, "w"), std::move(close)};
    if (!fp) {
        SPDLOG_ERROR("Opening file '{}' failed: {}", fname, std::strerror(errno));
        std::exit(1);
    }

    std::fwrite(contents.data(), sizeof contents[0], contents.size(), fp.get());
}

float deg_to_rad(float deg) noexcept { return deg * pi_v<float> / 180.f; }

float rad_to_deg(float rad) noexcept { return rad * 180.f / pi_v<float>; }

float clamp(float x, float min, float max) noexcept {
    if (x < min)
        x = min;
    if (x > max)
        x = max;
    return x;
}

} // namespace wt
