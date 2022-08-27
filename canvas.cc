#include "canvas.hh"

#include "color.hh"
#include "util.hh" // lerp

#include <fmt/format.h> // formatter

#include <algorithm> // fill, clamp
#include <iterator>  // back_inserter
#include <string>    // string
#include <utility>   // pair

namespace wt {

canvas::canvas(unsigned int w, unsigned int h) noexcept
    : canvas_{std::make_unique<color[]>(/*may overflow*/ w * h)}, w_{w}, h_{h} {}

color& canvas::operator()(unsigned int x, unsigned int y) noexcept { return canvas_[y * w_ + x]; }

color const& canvas::operator()(unsigned int x, unsigned int y) const noexcept {
    return canvas_[y * w_ + x];
}

unsigned canvas::size() const noexcept { return /*may overflow*/ w_ * h_; }

void canvas::fill(const color& c) noexcept { std::fill(canvas_.get(), canvas_.get() + size(), c); }

std::string canvas::as_ppm() const noexcept {
    // Max length of line in PPM file.
    constexpr unsigned ppm_line_length = 70;
    // How much text space one color component (r, g, or b) occupies?
    constexpr unsigned rgb_width = 4;
    // How many primary colors does one color have?
    constexpr unsigned rgb_count = 3;
    // How much text space one color (r, g, and b) occupies?
    constexpr unsigned color_width = rgb_width * rgb_count;
    // How many colors fully fit into one line?
    constexpr unsigned batch_size = ppm_line_length / color_width;
    // How many full batches of colors are there to print?
    const unsigned batch_count = size() / batch_size;

    auto ss = fmt::memory_buffer();
    fmt::format_to(std::back_inserter(ss), "P3\n{} {}\n{}\n", w_, h_, 255);

    // Print out batch_count batches of batch_size amount of colors each, one batch per line.
    for (unsigned nth_batch = 0; nth_batch < batch_count; ++nth_batch) {
        for (unsigned nth_color = 0; nth_color < batch_size; ++nth_color) {
            color col{std::clamp(canvas_[nth_batch * batch_size + nth_color][0], 0.f, 1.f) * 255.f,
                      std::clamp(canvas_[nth_batch * batch_size + nth_color][1], 0.f, 1.f) * 255.f,
                      std::clamp(canvas_[nth_batch * batch_size + nth_color][2], 0.f, 1.f) * 255.f};
            fmt::format_to(std::back_inserter(ss), "{:3.0f}", col);
            if (nth_color != batch_size - 1) {
                fmt::format_to(std::back_inserter(ss), " ");
            }
        }
        fmt::format_to(std::back_inserter(ss), "\n");
    }

    if ((size() - batch_count * batch_size) != 0) { // Any colors remaining to be processed?
        unsigned already_processed = batch_count * batch_size;
        for (unsigned leftover = already_processed; leftover < size(); ++leftover) {
            color col{
                std::clamp(canvas_[leftover][0], 0.f, 1.f) * 255.f,
                std::clamp(canvas_[leftover][1], 0.f, 1.f) * 255.f,
                std::clamp(canvas_[leftover][2], 0.f, 1.f) * 255.f,
            };
            fmt::format_to(std::back_inserter(ss), "{:3.0f}", col);
            if (leftover != size() - 1) {
                fmt::format_to(std::back_inserter(ss), " ");
            }
        }
        fmt::format_to(std::back_inserter(ss), "\n");
    }

    return std::string(ss.data(), ss.size());
}

} // namespace wt

auto fmt::formatter<wt::canvas>::format(const wt::canvas& c, fmt::format_context& ctx)
    -> decltype(ctx.out()) {
    fmt::formatter<wt::color> color_formatter;
    auto&& out = ctx.out();
    for (int y = c.h_ - 1; y >= 0; --y) {
        for (unsigned x = 0; x < c.w_ - 1; ++x) {
            //            fmt::formatter<wt::color>::format(c(x, y), ctx);
            color_formatter.format(c(x, y), ctx);
            fmt::format_to(out, " ");
        }
        //        fmt::formatter<wt::color>::format(c(c.w_ - 1, y), ctx);
        color_formatter.format(c(c.w_ - 1, y), ctx);
        fmt::format_to(out, "\n");
    }
    return out;
}

template struct fmt::formatter<wt::canvas>;
template class std::unique_ptr<wt::color[]>;
