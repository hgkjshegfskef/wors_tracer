#include "camera.hh"
#include "cli.hh"
#include "log.hh"
#include "render.hh"
#include "scene.hh"
#include "shape.hh"

#include <CLI/CLI.hpp>

#include <spdlog/spdlog.h>

using namespace wt;

int main(int argc, char** argv) {
    setup_logging();

    CLI::App app{"worst racer"};
    app.set_help_flag("--help", "Print this help message and exit");

    cli cli;

    app.add_option("-r,--render-backend", cli.render_backend, "Where to dump the pixels")
        ->check(CLI::IsMember{{"ppm", "sdl"}});

    auto* opt_w = app.add_option("-w,--width", cli.width, "Canvas width");
    auto* opt_h = app.add_option("-h,--height", cli.height, "Canvas height");
    opt_w->needs(opt_h);
    opt_h->needs(opt_w);

    app.add_option("-s,--scene", cli.scene, "Scene number");

    app.add_option("ppm-file", cli.ppm_fname, "PPM file name");

    CLI11_PARSE(app, argc, argv);

    scene scene = get_scene(cli.scene, cli);

    if (cli.render_backend == "sdl") {
        render_sdl(scene.camera, scene.world);
    } else if (cli.render_backend == "ppm") {
        render_ppm(scene.camera, scene.world);
    } else {
        SPDLOG_ERROR("Incorrect rendering backend: {}", cli.render_backend);
        return 1;
    }
}
