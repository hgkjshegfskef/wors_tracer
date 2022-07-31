#include "util.hh"

#include "color.hh"

#include <fmt/core.h>

#include <algorithm> // clamp
#include <cerrno>    // errno
#include <cstdio>    // FILE
#include <cstring>   // strerror
#include <memory>    // unique_ptr
#include <utility>   // pair

namespace wt {

float lerp(float x, std::pair<float, float> left_point,
           std::pair<float, float> right_point) noexcept {
    return left_point.second + (x - left_point.first) * (right_point.second - left_point.second) /
                                   (right_point.first - left_point.first);
}

void write_ppm(char const* fname, std::string_view contents) noexcept {
    auto close = [](FILE* f) { std::fclose(f); };
    std::unique_ptr<FILE, decltype(close)> fp{std::fopen(fname, "w"), std::move(close)};
    if (!fp) {
        fmt::print("Opening file {} failed: {}", fname, std::strerror(errno));
        return;
    }

    std::fwrite(contents.data(), sizeof contents[0], contents.size(), fp.get());

    //    std::ofstream image_file;
    //    try {
    //        image_file = [&fname]() {
    //            std::ofstream f{fname};
    //            if (!f) {
    //                throw std::system_error(errno, std::system_category(),
    //                                        "failed to open '" + fname + "'");
    //            }
    //            return f;
    //        }();
    //    } catch (std::system_error const& e) {
    //        std::cerr << e.what() << " (" << e.code() << ")" << std::endl;
    //        std::terminate();
    //    }
    //
    //    image_file.write(contents.data(), long(contents.size()));
}

// Scale a number between two (possibly overlapping) ranges.
// Use-case example: given a value in range [0;1], find out its respective value in range [0;255].
// Further reading: https://gamedev.stackexchange.com/a/33445
float scale(float value, std::pair<float, float> source_range,
            std::pair<float, float> target_range) noexcept {
    return lerp(value, {source_range.first, target_range.first},
                {source_range.second, target_range.second});
}

float clamp_and_scale(color const& col, unsigned component_idx) noexcept {
    // Clamp to the range of canvas color components' values.
    auto const clamped = std::clamp(col[component_idx], 0.f, 1.f);
    // Scale to the range of PPM color values.
    return scale(std::move(clamped), {0.f, 1.f}, {0.f, 255.f});
}

} // namespace wt
