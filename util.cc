#include "util.hh"

#include <fmt/core.h>

#include <cerrno>  // errno
#include <cstdio>  // FILE
#include <cstring> // strerror
#include <memory>  // unique_ptr

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

} // namespace wt
