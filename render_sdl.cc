#include "camera.hh"
#include "cli.hh"
#include "intersection.hh"
#include "ray.hh"
#include "render.hh"
#include "util.hh"
#include "world.hh"

#include <SDL2/SDL.h>
#include <oneapi/tbb/blocked_range2d.h>
#include <oneapi/tbb/parallel_for.h>

#include <fmt/chrono.h>
#include <spdlog/spdlog.h>

#include <chrono> // steady_clock
#include <cmath>
#include <cstdint>
#include <cstdlib> //atexit
#include <vector>

using namespace oneapi;

namespace wt {

void render_sdl(camera& camera, world const& world, cli const& cli) noexcept {
    if (std::atexit(SDL_Quit)) {
        SPDLOG_ERROR("atexit() registration failed");
    }

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SPDLOG_ERROR("SDL_Init error: {}", SDL_GetError());
        return;
    }

    std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)> window{
        SDL_CreateWindow("worst racer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                         cli.win_width, cli.win_height, 0),
        SDL_DestroyWindow};
    if (!window) {
        SPDLOG_ERROR("SDL_CreateWindow error: {}", SDL_GetError());
        return;
    }

    SDL_SetWindowMinimumSize(window.get(), camera.hsize, camera.vsize);

    std::uint32_t renderer_flags = SDL_RENDERER_ACCELERATED;
    if (cli.vsync) {
        SPDLOG_ERROR("vsync enable");
        renderer_flags |= SDL_RENDERER_PRESENTVSYNC;
    }
    std::unique_ptr<SDL_Renderer, decltype(&SDL_DestroyRenderer)> renderer{
        SDL_CreateRenderer(window.get(), -1, renderer_flags), SDL_DestroyRenderer};
    if (!renderer) {
        SPDLOG_ERROR("SDL_CreateRenderer error: {}", SDL_GetError());
        return;
    }

    SDL_RendererInfo renderer_info;
    if (SDL_GetRendererInfo(renderer.get(), &renderer_info)) {
        SPDLOG_ERROR("SDL_GetRendererInfo error: {}", SDL_GetError());
        return;
    }
    if (renderer_info.num_texture_formats < 1) {
        SPDLOG_ERROR("No supported texture formats");
        return;
    }
    SPDLOG_DEBUG("Suggested pixel format: {}",
                 SDL_GetPixelFormatName(renderer_info.texture_formats[1]));

    // linear filtering (anti-aliasing when scaling)
    if (SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1") != SDL_TRUE) {
        SPDLOG_ERROR("SDL_HINT_RENDER_SCALE_QUALITY was not set to 1");
        return;
    }

    std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)> screen_texture{
        SDL_CreateTexture(renderer.get(), renderer_info.texture_formats[1],
                          SDL_TEXTUREACCESS_STREAMING, camera.hsize, camera.vsize),
        SDL_DestroyTexture};
    if (!screen_texture) {
        SPDLOG_ERROR("SDL_CreateTexture error: {}", SDL_GetError());
        return;
    }

    unsigned pixel_format_enum;
    if (SDL_QueryTexture(screen_texture.get(), &pixel_format_enum, nullptr, nullptr, nullptr) < 0) {
        SPDLOG_ERROR("SDL_QueryTexture error: {}", SDL_GetError());
        return;
    }
    SPDLOG_DEBUG("Actual pixel format: {}", SDL_GetPixelFormatName(pixel_format_enum));

    std::unique_ptr<SDL_PixelFormat, decltype(&SDL_FreeFormat)> pixel_format{
        SDL_AllocFormat(pixel_format_enum), SDL_FreeFormat};
    if (!pixel_format) {
        SPDLOG_ERROR("SDL_AllocFormat error: {}", SDL_GetError());
        return;
    }

    if (SDL_SetRelativeMouseMode(SDL_TRUE) < 0) {
        SPDLOG_ERROR("SDL_SetRelativeMouseMode error: {}", SDL_GetError());
        return;
    }

    if (SDL_RenderSetLogicalSize(renderer.get(), camera.hsize, camera.vsize) < 0) {
        SPDLOG_ERROR("SDL_RenderSetLogicalSize error: {}", SDL_GetError());
        return;
    }

    if (SDL_RenderSetIntegerScale(renderer.get(), SDL_TRUE) < 0) {
        SPDLOG_ERROR("SDL_RenderSetIntegerScale error: {}", SDL_GetError());
        return;
    }

    pnt3 from{0, 1.5f, -5};
    vec3 up{0, 1, 0};

    float yaw = 0.f;
    float pitch = 0.f;
    float rad_yaw = yaw;
    float rad_pitch = pitch;
    float sensitivity = 0.05f;
    vec3 forward = {std::sin(rad_yaw) * std::cos(rad_pitch), std::sin(rad_pitch),
                    std::cos(rad_yaw) * std::cos(rad_pitch)};

    unsigned frame_cnt = 0;
    std::chrono::duration<double> frame_delta{};

    while (true) {
        auto begin_frame = std::chrono::high_resolution_clock::now();

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_QUIT:
                return;
            }
        }

        int rel_mouse_x, rel_mouse_y;
        auto mouse_state = SDL_GetRelativeMouseState(&rel_mouse_x, &rel_mouse_y);
        if (rel_mouse_x) {
            yaw += rel_mouse_x * sensitivity;
            rad_yaw = deg_to_rad(yaw);
        }
        if (rel_mouse_y) {
            pitch -= rel_mouse_y * sensitivity;
            pitch = std::clamp(pitch, -89.f, 89.f);
            rad_pitch = deg_to_rad(pitch);
        }
        if (rel_mouse_x || rel_mouse_y) {
            forward = {std::sin(rad_yaw) * std::cos(rad_pitch), std::sin(rad_pitch),
                       std::cos(rad_yaw) * std::cos(rad_pitch)};
        }
        if (mouse_state & SDL_BUTTON(1)) {
            if (SDL_SetRelativeMouseMode(SDL_TRUE) < 0) {
                SPDLOG_ERROR("SDL_SetRelativeMouseMode error: {}", SDL_GetError());
                return;
            }
        }

        auto* kb_state = SDL_GetKeyboardState(nullptr);
        if (kb_state[SDL_SCANCODE_ESCAPE]) {
            if (SDL_SetRelativeMouseMode(SDL_FALSE) < 0) {
                SPDLOG_ERROR("SDL_SetRelativeMouseMode error: {}", SDL_GetError());
                return;
            }
        }

        float const cam_speed = 2.5f * frame_delta.count();
        if (kb_state[SDL_SCANCODE_W]) {
            from = position(from, forward, cam_speed);
        }
        if (kb_state[SDL_SCANCODE_S]) {
            from = position(from, -forward, cam_speed);
        }
        if (kb_state[SDL_SCANCODE_A]) {
            from = position(from, normalize(cross(forward, up)), cam_speed);
        }
        if (kb_state[SDL_SCANCODE_D]) {
            from = position(from, -normalize(cross(forward, up)), cam_speed);
        }
        if (kb_state[SDL_SCANCODE_SPACE]) {
            from = position(from, up, cam_speed);
        }

        camera.inv_tform = inverse(v3::view(from, forward, up));

        std::uint32_t* pixels;
        int texture_width; // unused
        if (SDL_LockTexture(screen_texture.get(), nullptr, reinterpret_cast<void**>(&pixels),
                            &texture_width) < 0) {
            SPDLOG_ERROR("SDL_LockTexture error: {}", SDL_GetError());
            return;
        }

        tbb::parallel_for(
            tbb::blocked_range2d<unsigned>(0, camera.vsize, 0, camera.hsize),
            [&](auto const& range) {
                std::vector<intersection> world_isecs;
                world_isecs.reserve(world.spheres.size * 2);
                for (unsigned y = range.rows().begin(); y != range.rows().end(); ++y) {
                    for (unsigned x = range.cols().begin(); x != range.cols().end(); ++x) {
                        color const color =
                            color_at(world, ray_for_pixel(camera, x, y), world_isecs);
                        pixels[y * camera.hsize + x] =
                            SDL_MapRGB(pixel_format.get(),
                                       lerp(clamp(color[0], 0.f, 1.f), 0.f, 0.f, 1.f, 255.f) + .5f,
                                       lerp(clamp(color[1], 0.f, 1.f), 0.f, 0.f, 1.f, 255.f) + .5f,
                                       lerp(clamp(color[2], 0.f, 1.f), 0.f, 0.f, 1.f, 255.f) + .5f);
                    }
                }
            });

        SDL_UnlockTexture(screen_texture.get());

        if (SDL_RenderCopy(renderer.get(), screen_texture.get(), nullptr, nullptr) < 0) {
            SPDLOG_ERROR("SDL_RenderCopy error: {}", SDL_GetError());
            return;
        }

        SDL_RenderPresent(renderer.get());

        auto end_frame = std::chrono::high_resolution_clock::now();

        frame_delta = end_frame - begin_frame;
        SPDLOG_INFO("Frame #{} drawn in {}", ++frame_cnt,
                    std::chrono::duration<double, std::milli>(frame_delta));
    }
}

} // namespace wt
