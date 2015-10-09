#include <stdio.h>
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <emscripten.h>
#include <unordered_map>
#include <string>
#include <cassert>
#include <type_traits>
#include <vrm/sdl.hpp>

namespace sdl = vrm::sdl;

int main(int argc, char** argv)
{
    std::cout << "Hello world!\n";
    auto& c(sdl::make_global_context(1000, 600));

    auto toriel_texture(c.make_texture("files/toriel.png"));
    auto toriel_sprite(c.make_sprite(toriel_texture));

    auto test_text_font(c.make_ttffont("files/pixel.ttf", 16));
    auto test_text_texture(c.make_ttftext_texture(
        test_text_font, "xd", SDL_Color{255, 255, 255, 255}));

    auto test_text_sprite(c.make_sprite());
    toriel_sprite.set_origin_to_center();


    std::cout << "origin: " << toriel_sprite.origin() << "\n";

    // sdl::image toriel{"files/toriel.png"};

    c.update_fn() = [&](auto)
    {
        test_text_texture = c.make_ttftext_texture(
            test_text_font, std::to_string(c.fps()), SDL_Color{255, 255, 255, 255});

        // std::cout << c.fps() << "\n";

        test_text_sprite = c.make_sprite(test_text_texture);
        test_text_sprite.set_origin_to_center();

        toriel_sprite.pos() = c.mouse_pos();
        toriel_sprite.radians() += 0.05f;
        test_text_sprite.pos() = toriel_sprite.pos() - sdl::make_vec2(0, 100.f);
    };

    c.draw_fn() = [&]
    {

        // c.draw(toriel, sdl::make_vec2(0.f, 0.f));
        c.draw(toriel_sprite);
        c.draw(test_text_sprite);
    };

    sdl::run_global_context();
    return 0;
}