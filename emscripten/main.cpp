#include <stdio.h>
#include <iostream>
#include <memory>
#include <bitset>
#include <algorithm>
#include <unordered_map>
#include <string>
#include <cassert>
#include <vector>
#include <type_traits>
#include <random>
#include <vrm/sdl.hpp>

namespace sdl = vrm::sdl;

namespace vrm
{
    namespace sdl
    {
        auto make_2d_projection(float width, float height)
        {
            return glm::ortho(0.0f, width, height, 0.0f, -1.0f, 1.0f);
        }

        namespace impl
        {
            auto make_sprite_renderer_program()
            {
                constexpr auto v_sh_path("vrm/sdl/glsl/sprite.vert");
                constexpr auto f_sh_path("vrm/sdl/glsl/sprite.frag");

                auto v_sh(make_shader_from_file<shader_t::vertex>(v_sh_path));
                auto f_sh(make_shader_from_file<shader_t::fragment>(f_sh_path));

                return make_program(*v_sh, *f_sh);
            }
        }

        namespace impl
        {
            auto get_texture_unit_idx(GLenum texture_unit) noexcept
            {
                return static_cast<sz_t>(texture_unit) -
                       static_cast<sz_t>(GL_TEXTURE0);
            }

            auto get_texture_unit(sz_t idx) noexcept
            {
                return static_cast<GLenum>(
                    static_cast<sz_t>(GL_TEXTURE0) + idx);
            }
        }

        template <sz_t TN>
        struct texture_cache
        {
        private:
            static constexpr sz_t _null_bind{TN};
            GLuint _last_binds[TN];

            auto get_unit_idx(const impl::gltexture2d& t) const noexcept
            {
                for(sz_t i(0); i < TN; ++i)
                    if(_last_binds[i] == t.location()) return i;

                return _null_bind;
            }

            auto get_free_unit_idx() const noexcept
            {
                for(sz_t i(0); i < TN; ++i)
                    if(_last_binds[i] == 0) return i;

                return _null_bind;
            }

        public:
            texture_cache() noexcept
            {
                for(sz_t i(0); i < TN; ++i)
                {
                    // Set cache to "unbound".
                    _last_binds[i] = 0;
                }
            }

            auto use(impl::gltexture2d& t) noexcept
            {
                auto unit_idx(get_unit_idx(t));

                // If the texture is already bound, return the texture unit
                // index.
                if(unit_idx != _null_bind) return unit_idx;

                // Otherwise, find a free texture unit, bind the texture, cache
                // its location and return the texture unit index.
                auto free_unit_idx(get_free_unit_idx());
                t.activate_and_bind(impl::get_texture_unit(free_unit_idx));
                _last_binds[free_unit_idx] = t.location();
                return free_unit_idx;
            }
        };

        struct sprite_renderer
        {
            program _program{impl::make_sprite_renderer_program()};
            sdl::impl::unique_vao _vao;

            sdl::impl::unique_vbo<buffer_target::array> _vbo0;
            sdl::impl::unique_vbo<buffer_target::element_array> _vbo1;

            glm::mat4 _model;
            glm::mat4 _view;
            glm::mat4 _projection;

            sdl::uniform _uf_model;
            sdl::uniform _uf_view;
            sdl::uniform _uf_projection;
            sdl::uniform _uf_tex;

            texture_cache<3> _texture_cache;

            float _vertices[16]{
                0.f, 1.f, // sw
                0.f, 1.f, // sw

                0.f, 0.f, // ne
                0.f, 0.f, // ne

                1.f, 0.f, // nw
                1.f, 0.f, // nw

                1.f, 1.f, // se
                1.f, 1.f, // se
            };

            GLubyte _indices[6]{
                0, 1, 2, // first triangle
                0, 2, 3  // second triangle
            };

            sprite_renderer()
            {
                _projection = make_2d_projection(1000.f, 600.f);
                init_render_data();
            }

            void init_render_data() noexcept
            {
                // TODO: required?
                _program.use();

                _vao = make_vao(1);

                _vbo0 = sdl::make_vbo<buffer_target::array>(1);
                _vbo0->with([&, this]
                    {
                        _vbo0->buffer_data<buffer_usage::static_draw>(
                            _vertices);

                        _vao->with([&, this]
                            {
                                _program.nth_attribute(0)
                                    .enable()
                                    .vertex_attrib_pointer_float(4, false, 0);
                            });
                    });

                _vbo1 = sdl::make_vbo<buffer_target::element_array>(1);
                _vbo1->with([&, this]
                    {
                        _vbo1->buffer_data<buffer_usage::static_draw>(_indices);
                    });

                // Set model/view/projection uniform matrices.
                _uf_model = _program.get_uniform("model");
                _uf_view = _program.get_uniform("view");
                _uf_projection = _program.get_uniform("projection");
                _uf_tex = _program.get_uniform("uf_tex");
            }

            void use()
            {
                _program.use();

                _vao->bind();
                _vbo0->bind();
                _vbo1->bind();
            }

            void draw_sprite(impl::gltexture2d& t, const glm::vec2& position,
                const glm::vec2& origin, const glm::vec2& size,
                float radians) noexcept
            {
                // Reset `model` to identity matrix.
                _model = glm::mat4{};

                // Tranformation order:
                // 1) Scale.
                // 2) Rotate.
                // 3) Translate.

                // They will occur in the opposite order below.

                // Translate to `position`.
                _model = glm::translate(_model, glm::vec3(position, 0.0f));

                // Rotate around origin.
                _model =
                    glm::rotate(_model, radians, glm::vec3(0.0f, 0.0f, 1.0f));

                // Set origin to the center of the quad.
                _model = glm::translate(_model, glm::vec3(-origin, 0.0f));

                // Set origin back to `(0, 0)`.
                _model = glm::translate(
                    _model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));

                // Scale to `size`.
                _model = glm::scale(_model, glm::vec3(size, 1.0f));

                // Set model/view/projection uniform matrices.
                _uf_model.matrix4fv(_model);
                _uf_view.matrix4fv(_view);
                _uf_projection.matrix4fv(_projection);

                // Gets the texture unit index from the cache and uses it.
                _uf_tex.integer(_texture_cache.use(t));

                // Assumes the VBOs, VAO, and texture unit are bound.
                glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0);
            }
        };
    };
}

int main(int argc, char** argv)
{
    auto c_handle(sdl::make_global_context("test game", 1000, 600));
    auto& c(*c_handle);

    auto make_texture_from_image([&](const auto& path)
        {
            auto s(c.make_surface(path));
            return sdl::make_gltexture2d(*s);
        });


    sdl::sprite_renderer sr;

    auto toriel_texture(make_texture_from_image("files/toriel.png"));
    auto soul_texture(make_texture_from_image("files/soul.png"));
    auto fireball_texture(make_texture_from_image("files/fireball.png"));

    struct entity
    {
        glm::vec2 _pos, _origin, _size;
        float _radians{0.f};
        sdl::impl::gltexture2d _texture;

        float _hitbox_radius;
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
        e._origin = glm::vec2{0, 0};
        e._texture = *soul_texture;
        e._size = glm::vec2{soul_texture->size()};

        e._hitbox_radius = 3.f;
        // e._sprite = c.make_sprite(*soul_texture);
        // e._sprite.set_origin_to_center();

        e._update_fn = [&](auto& x, auto step)
        {
            constexpr float speed{5.f};
            sdl::vec2f input;

            if(c.key(sdl::kkey::left))
                input.x = -1;
            else if(c.key(sdl::kkey::right))
                input.x = 1;

            if(c.key(sdl::kkey::up))
                input.y = -1;
            else if(c.key(sdl::kkey::down))
                input.y = 1;

            x._pos += input * (speed * step);
        };

        e._draw_fn = [&](auto& x)
        {
            sr.draw_sprite(x._texture, x._pos, x._origin, x._size, x._radians);
        };

        return e;
    };

    auto make_fireball = [&](auto pos, auto vel, auto speed)
    {
        entity e;
        e._pos = pos;
        e._radians = static_cast<float>(rand() % 6280) / 1000.f;
        e._origin = glm::vec2{0, 0};
        e._texture = *fireball_texture;
        e._size = glm::vec2{fireball_texture->size()};
        e._hitbox_radius = 3.f;

        e._update_fn = [&, vel, speed, life = 100.f, dir = rand() % 2 ](
            auto& x, auto step) mutable
        {
            x._pos += vel * (speed * step);
            x._radians += dir ? 0.2 * step : -0.2 * step;

            life -= step * 0.3f;
            if(life <= 0.f) x.alive = false;
        };

        e._draw_fn = [&](auto& x)
        {
            sr.draw_sprite(x._texture, x._pos, x._origin, x._size, x._radians);
        };

        return e;
    };

    auto make_toriel = [&](auto pos)
    {
        entity e;
        e._pos = pos;
        e._origin = glm::vec2{0, 0};
        e._texture = *toriel_texture;
        e._size = glm::vec2{toriel_texture->size()};
        e._hitbox_radius = 30.f;

        e._update_fn = [&, timer = 25.f ](auto& x, auto step) mutable
        {
            timer -= step;
            if(timer <= 0.f)
            {
                timer = 25.f;

                for(int i = 0; i < 15; ++i)
                    if(entities.size() < max_entities)
                        entities.emplace_back(make_fireball(x._pos,
                            sdl::make_vec2(-2.f + (rand() % 500) / 100.f, 2.f),
                            1.f + (rand() % 100) / 80.f));
            }
        };

        e._draw_fn = [&](auto& x)
        {
            sr.draw_sprite(x._texture, x._pos, x._origin, x._size, x._radians);
        };

        return e;
    };

    entities.emplace_back(make_toriel(sdl::make_vec2(500.f, 100.f)));
    entities.emplace_back(make_soul(sdl::make_vec2(500.f, 500.f)));

    c.update_fn() = [&](auto step)
    {
        for(auto& e : entities) e._update_fn(e, step);

        entities.erase(std::remove_if(std::begin(entities), std::end(entities),
                           [](auto& e)
                           {
                               return !e.alive;
                           }),
            std::end(entities));

        if(c.key(sdl::kkey::q)) c.fps_limit += step;
        if(c.key(sdl::kkey::e)) c.fps_limit -= step;

        if(c.key(sdl::kkey::escape))
        {
            sdl::stop_global_context();
        }

        if(rand() % 100 < 30)
        {
            c.title(std::to_string(c.fps_limit) + " | " + std::to_string(c._static_timer._loops) +
                    " ||| " + std::to_string(c.fps()) + " | " +
                    std::to_string(c.real_ms()) + " | " +
                    std::to_string(c.update_ms()));
        }

        // std::cout << c.real_ms() << "\n";
    };

    c.draw_fn() = [&]
    {
        sr.use();
        for(auto& e : entities) e._draw_fn(e);
    };

    sdl::run_global_context();

    return 0;
}

/*
int main_stress(int argc, char** argv)
{
    auto c_handle(sdl::make_global_context("test game", 1000, 600));
    auto& c(*c_handle);

    auto make_texture_from_image([&](const auto& path)
        {
            auto s(c.make_surface(path));
            return sdl::make_gltexture2d(*s);
        });

    auto toriel_texture(make_texture_from_image("files/toriel.png"));
    auto fireball_texture(make_texture_from_image("files/fireball.png"));
    auto soul_texture(make_texture_from_image("files/soul.png"));

    sdl::sprite_renderer sr;

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    float timer{10};
    float rot = 0.f;

    c.update_fn() = [&](auto)
    {
        if(c.btn(sdl::mbtn::left))
        {
            c.title(std::to_string(c.fps()));
        }
    };


    c.draw_fn() = [&]
    {
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_BLEND);
        sr.use();

        ++timer;

        sr._view = glm::translate(sr._view, glm::vec3(500.f, 300.f, 0.f));
        sr._view = glm::scale(sr._view, glm::vec3(0.995f, 0.995f, 1.f));
        sr._view = glm::translate(sr._view, glm::vec3(-500.f, -300.f, 0.f));

        for(auto ix = 0; ix < 100; ++ix)
            for(auto iy = 0; iy < 70; ++iy)
            {
                sr.draw_sprite(                                // .
                    *soul_texture,                             // texture
                    glm::vec2{10 + ix * 16.f, 10 + iy * 18.f}, // position
                    glm::vec2{ix - iy % 15 * (timer * 3),
                        ix + (iy * ix) % 40}, // origin [(0, 0) is center]
                    glm::vec2{soul_texture->size() *
                              (((ix + iy) % 4) * 0.85f)}, // size
                    rot * (((ix * iy) % 5 + 1) * 0.3f)    // radians
                    );
            }

        for(auto ix = 0; ix < 70; ++ix)
            for(auto iy = 0; iy < 50; ++iy)
            {
                sr.draw_sprite(                                // .
                    *fireball_texture,                         // texture
                    glm::vec2{10 + ix * 24.f, 10 + iy * 24.f}, // position
                    glm::vec2{ix + iy % 20 * timer,
                        ix * iy % 30}, // origin [(0, 0) is center]
                    glm::vec2{fireball_texture->size() *
                              (((ix + iy) % 3) * 0.85f)}, // size
                    rot * (((ix + iy) % 4 + 1) * 0.3f)    // radians
                    );
            }


        for(auto ix = 0; ix < 16; ++ix)
            for(auto iy = 0; iy < 8; ++iy)
            {
                sr.draw_sprite(                                 // .
                    *toriel_texture,                            // texture
                    glm::vec2{10 + ix * 70.f, 10 + iy * 150.f}, // position
                    glm::vec2{(int)timer % 200, (int)timer % 100} *
                        10.f, // origin [(0, 0) is center]
                    glm::vec2{toriel_texture->size() *
                              (((ix + iy) % 3) * 0.95f)}, // size
                    rot * (((ix + iy) % 4 + 1) * 0.3f)    // radians
                    );
            }

        rot += 0.05f;
    };

    sdl::run_global_context();
    return 0;
}
*/