#include <stdio.h>
#include <iostream>


#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#else
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#endif

#include <memory>
#include <bitset>
#include <algorithm>
#include <unordered_map>
#include <string>
#include <cassert>
#include <vector>
#include <type_traits>
#include <vrm/sdl.hpp>

namespace sdl = vrm::sdl;

int main(int argc, char** argv)
{
    auto c_handle(sdl::make_global_context(1000, 600));
    auto& c(*c_handle);

    auto toriel_image(c.make_image("files/toriel.png"));
    auto soul_image(c.make_image("files/soul.png"));
    auto fireball_image(c.make_image("files/fireball.png"));

    auto toriel_texture(c.make_texture(*toriel_image));
    auto soul_texture(c.make_texture(*soul_image));
    auto fireball_texture(c.make_texture(*fireball_image));

    struct entity
    {
        sdl::vec2f _pos;
        float _hitbox_radius;
        sdl::sprite _sprite;
        bool alive{true};
        std::function<void(entity&, sdl::ft)> _update_fn;
        std::function<void(entity&)> _draw_fn;
    };

    constexpr sdl::sz_t max_entities{10000};
    std::vector<entity> entities;
    entities.reserve(max_entities);

    auto make_soul = [&](auto pos)
    {
        entity e;
        e._pos = pos;
        e._hitbox_radius = 3.f;
        e._sprite = c.make_sprite(*soul_texture);
        e._sprite.set_origin_to_center();

        e._update_fn = [&](auto& x, auto)
        {
            constexpr float speed{5.f};
            sdl::vec2i input;

            if(c.key(sdl::kkey::left))
                input.x() = -1;
            else if(c.key(sdl::kkey::right))
                input.x() = 1;

            if(c.key(sdl::kkey::up))
                input.y() = -1;
            else if(c.key(sdl::kkey::down))
                input.y() = 1;

            x._pos += input * speed;
        };

        e._draw_fn = [&](auto& x)
        {
            x._sprite.pos() = x._pos;
            c.draw(x._sprite);
        };

        return e;
    };

    auto make_fireball = [&](auto pos, auto vel, auto speed)
    {
        entity e;
        e._pos = pos;
        e._hitbox_radius = 3.f;
        e._sprite = c.make_sprite(*fireball_texture);
        e._sprite.set_origin_to_center();

        e._update_fn = [&, vel, speed, life = 100.f ](auto& x, auto) mutable
        {
            x._pos += vel * speed;

            if(life-- <= 0.f) x.alive = false;
        };

        e._draw_fn = [&](auto& x)
        {
            x._sprite.pos() = x._pos;
            c.draw(x._sprite);
        };

        return e;
    };

    auto make_toriel = [&](auto pos)
    {
        entity e;
        e._pos = pos;
        e._hitbox_radius = 30.f;
        e._sprite = c.make_sprite(*toriel_texture);
        e._sprite.set_origin_to_center();

        e._update_fn = [&](auto& x, auto)
        {
            if((rand() % 100) > 30)
            {
                for(int i = 0; i < 20; ++i)
                    if(entities.size() < max_entities)
                        entities.emplace_back(make_fireball(x._pos,
                            sdl::make_vec2(-2.f + (rand() % 500) / 100.f, 2.f),
                            1.f + (rand() % 100) / 80.f));
            }
        };

        e._draw_fn = [&](auto& x)
        {
            x._sprite.pos() = x._pos;
            c.draw(x._sprite);
        };

        return e;
    };

    entities.emplace_back(make_toriel(sdl::make_vec2(500.f, 100.f)));
    entities.emplace_back(make_soul(sdl::make_vec2(500.f, 500.f)));

    c.update_fn() = [&](auto ft)
    {

        for(auto& e : entities) e._update_fn(e, ft);
        entities.erase(std::remove_if(std::begin(entities), std::end(entities),
                           [](auto& e)
                           {
                               return !e.alive;
                           }),
            std::end(entities));

        if(c.key(sdl::kkey::escape)) sdl::stop_global_context();
    };

    c.draw_fn() = [&]
    {
        for(auto& e : entities) e._draw_fn(e);
    };

    sdl::run_global_context();

    return 0;
}