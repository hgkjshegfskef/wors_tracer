#include "camera.hh"
#include "intersection.hh"
#include "ray.hh"
#include "render.hh"
#include "shape.hh"
#include "util.hh"
#include "world.hh"

#include <SDL2/SDL.h>
#include <oneapi/tbb/blocked_range2d.h>
#include <oneapi/tbb/parallel_for.h>

#include <fmt/chrono.h>
#include <spdlog/spdlog.h>

#include <chrono> // steady_clock
#include <cmath>
#include <cstdlib> //atexit
#include <vector>

using namespace oneapi;

namespace wt {

void render_sdl(camera& camera, world const& world) noexcept {
    if (std::atexit(SDL_Quit)) {
        SPDLOG_ERROR("atexit() registration failed");
    }

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SPDLOG_ERROR("SDL_Init error: {}", SDL_GetError());
        return;
    }

    std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)> window{
        SDL_CreateWindow("worst racer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                         camera.hsize, camera.vsize, 0),
        SDL_DestroyWindow};
    if (!window) {
        SPDLOG_ERROR("SDL_CreateWindow error: {}", SDL_GetError());
        return;
    }

    std::unique_ptr<SDL_Surface, decltype(&SDL_FreeSurface)> surface{
        SDL_GetWindowSurface(window.get()), SDL_FreeSurface};
    if (!surface) {
        SPDLOG_ERROR("SDL_GetWindowSurface error: {}", SDL_GetError());
        return;
    }

    if (SDL_SetRelativeMouseMode(SDL_TRUE) < 0) {
        SPDLOG_ERROR("SDL_SetRelativeMouseMode error: {}", SDL_GetError());
        return;
    }

    SPDLOG_INFO("Pixel format: {}", SDL_GetPixelFormatName(surface->format->format));

    unsigned frame_cnt = 0;

    pnt3 from{0, 1.5f, -5};
    vec3 up{0, 1, 0};
    vec3 forward;
    vec3 back;
    vec3 left;
    vec3 right;

    float yaw = 0.f;
    float pitch = 0.f;
    float sensitivity = 0.05f;

    Uint64 delta_time = 0;
    Uint64 last_frame = 0;

    while (true) {
        Uint64 const current_frame = SDL_GetTicks64();
        delta_time = current_frame - last_frame;
        last_frame = current_frame;
        float const cam_speed = 0.0025f * delta_time;

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
        }
        if (rel_mouse_y) {
            pitch -= rel_mouse_y * sensitivity;
            pitch = std::clamp(pitch, -89.f, 89.f);
        }
        if (mouse_state & SDL_BUTTON(1)) {
            if (SDL_SetRelativeMouseMode(SDL_TRUE) < 0) {
                SPDLOG_ERROR("SDL_SetRelativeMouseMode error: {}", SDL_GetError());
                return;
            }
        }

        forward = {std::sin(deg_to_rad(yaw)) * std::cos(deg_to_rad(pitch)),
                   std::sin(deg_to_rad(pitch)),
                   std::cos(deg_to_rad(yaw)) * std::cos(deg_to_rad(pitch))};
        back = -forward;
        left = normalize(cross(forward, up));
        right = -left;

        auto* kb_state = SDL_GetKeyboardState(nullptr);
        if (kb_state[SDL_SCANCODE_ESCAPE]) {
            if (SDL_SetRelativeMouseMode(SDL_FALSE) < 0) {
                SPDLOG_ERROR("SDL_SetRelativeMouseMode error: {}", SDL_GetError());
                return;
            }
        }
        if (kb_state[SDL_SCANCODE_W]) {
            from = position(from, forward, cam_speed);
        }
        if (kb_state[SDL_SCANCODE_S]) {
            from = position(from, back, cam_speed);
        }
        if (kb_state[SDL_SCANCODE_A]) {
            from = position(from, left, cam_speed);
        }
        if (kb_state[SDL_SCANCODE_D]) {
            from = position(from, right, cam_speed);
        }
        if (kb_state[SDL_SCANCODE_SPACE]) {
            from = position(from, up, cam_speed);
        }

        //        float const radius = 3.f;
        //        float cam_x = std::cos(SDL_GetTicks64() / 1000.f) * radius;
        //        float cam_z = std::sin(SDL_GetTicks64() / 1000.f) * radius;

        camera.tform = v3::view(from, forward, up);
        tform4 const inv_cam_tform = inverse(camera.tform);

        //        SDL_memset(surface->pixels, 0, surface->h * surface->pitch);

        auto start = std::chrono::steady_clock::now();

        tbb::parallel_for(
            tbb::blocked_range2d<int>(0, camera.vsize, 0, camera.hsize), [&](auto const range) {
                std::vector<intersection> world_isecs;
                world_isecs.reserve(world.shapes.size() * 2);
                auto* pixels = reinterpret_cast<unsigned*>(surface->pixels);
                for (int y = range.rows().begin(); y != range.rows().end(); ++y) {
                    for (int x = range.cols().begin(); x != range.cols().end(); ++x) {
                        color const color = color_at(
                            world, ray_for_pixel(camera, inv_cam_tform, x, y), world_isecs);
                        pixels[y * camera.hsize + x] = SDL_MapRGB(
                            surface->format, clamp_and_scale(color, 0) + .5f,
                            clamp_and_scale(color, 1) + .5f, clamp_and_scale(color, 2) + .5f);
                    }
                }
            });

        auto stop = std::chrono::steady_clock::now();

        SDL_UpdateWindowSurface(window.get());

        SPDLOG_INFO("Frame #{} drawn in {}", ++frame_cnt,
                    std::chrono::duration<double, std::milli>(stop - start));
    }
}

} // namespace wt
