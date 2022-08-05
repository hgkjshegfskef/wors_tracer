#pragma once

#include <string>

namespace wt {

struct cli {
    std::string render_backend;
    unsigned win_width;
    unsigned win_height;
    unsigned tex_width;
    unsigned tex_height;
    unsigned scene;
    std::string ppm_fname;
};

} // namespace wt
