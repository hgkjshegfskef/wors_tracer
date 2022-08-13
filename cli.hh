#pragma once

#include <string>

namespace wt {

struct cli {
    std::string_view render_backend;
    unsigned win_width;
    unsigned win_height;
    unsigned tex_width;
    unsigned tex_height;
    unsigned scene;
    std::string_view ppm_fname;
    bool vsync;
};

} // namespace wt
