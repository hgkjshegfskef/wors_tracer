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

    std::unique_ptr<SDL_Renderer, decltype(&SDL_DestroyRenderer)> renderer{
        SDL_CreateRenderer(window.get(), -1, SDL_RENDERER_ACCELERATED), SDL_DestroyRenderer};
    if (!renderer) {
        SPDLOG_ERROR("SDL_CreateRenderer error: {}", SDL_GetError());
        return;
    }

    std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)> screen_texture{
        SDL_CreateTexture(renderer.get(), SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING,
                          camera.hsize, camera.vsize),
        SDL_DestroyTexture};
    if (!screen_texture) {
        SPDLOG_ERROR("SDL_CreateTexture error: {}", SDL_GetError());
        return;
    }

    std::unique_ptr<SDL_PixelFormat, decltype(&SDL_FreeFormat)> pixel_format{
        SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888), SDL_FreeFormat};
    if (!pixel_format) {
        SPDLOG_ERROR("SDL_AllocFormat error: {}", SDL_GetError());
        return;
    }

    auto pixels{std::make_unique<std::uint32_t[]>(camera.hsize * camera.vsize)};

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

        camera.tform = v3::view(from, forward, up);
        tform4 const inv_cam_tform = inverse(camera.tform);

        tbb::parallel_for(
            tbb::blocked_range2d<int>(0, camera.vsize, 0, camera.hsize), [&](auto const range) {
                std::vector<intersection> world_isecs;
                world_isecs.reserve(world.spheres.size * 2);
                for (int y = range.rows().begin(); y != range.rows().end(); ++y) {
                    for (int x = range.cols().begin(); x != range.cols().end(); ++x) {
                        color const color = color_at(
                            world, ray_for_pixel(camera, inv_cam_tform, x, y), world_isecs);
                        pixels[y * camera.hsize + x] = SDL_MapRGB(
                            pixel_format.get(), clamp_and_scale(color, 0) + .5f,
                            clamp_and_scale(color, 1) + .5f, clamp_and_scale(color, 2) + .5f);
                    }
                }
            });

        if (SDL_UpdateTexture(screen_texture.get(), nullptr, pixels.get(),
                              camera.hsize * sizeof *pixels.get()) < 0) {
            SPDLOG_ERROR("SDL_UpdateTexture error: {}", SDL_GetError());
            return;
        }

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
