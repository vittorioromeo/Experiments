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

        struct sprite_renderer
        {
            program _program{impl::make_sprite_renderer_program()};
            sdl::impl::unique_vao _vao;

            glm::mat4 model;
            glm::mat4 view;
            glm::mat4 projection;

            sdl::uniform uf_model;
            sdl::uniform uf_view;
            sdl::uniform uf_projection;
            sdl::uniform uf_tex;

            std::vector<float> vertices_old{
                0.0f, 1.0f, // 0.xy
                0.0f, 1.0f, // 0.zw

                1.0f, 0.0f, // 1.xy
                1.0f, 0.0f, // 1.zw

                0.0f, 0.0f, // 2.xy
                0.0f, 0.0f, // 2.zw

                0.0f, 1.0f, // 3.xy
                0.0f, 1.0f, // 3.zw

                1.0f, 1.0f, // 4.xy
                1.0f, 1.0f, // 4.zw

                1.0f, 0.0f, // 5.xy
                1.0f, 0.0f  // 5.zw
            };

            std::vector<float> vertices{
                -1.f, -1.f, // sw
                -1.f, -1.f, // sw

                -1.f, 1.f, // nw
                -1.f, 1.f, // nw

                1.f, -1.f, // ne
                1.f, -1.f, // ne

                1.f, -1.f, // se
                1.f, -1.f, // se
            };

            std::vector<GLubyte> indices{
                0, 1, 2, // first triangle
                0, 2, 3  // second triangle
            };

            sprite_renderer()
            {
                projection = make_2d_projection(1000.f, 600.f);
                init_render_data();
            }

            void init_render_data() noexcept
            {
                // TODO: required?
                _program.use();

                _vao = make_vao(1);

                auto vbo = sdl::make_vbo<buffer_target::array>(1);
                vbo->with([&, this]
                    {
                        vbo->buffer_data<buffer_usage::static_draw>(vertices);

                        _vao->with([&, this]
                            {
                                _program.nth_attribute(0)
                                    .enable()
                                    .vertex_attrib_pointer_float(4, false, 0);
                            });
                    });

                auto vbo2 = sdl::make_vbo<buffer_target::element_array>(1);
                vbo2->with([&, this]
                    {
                        vbo2->buffer_data<buffer_usage::static_draw>(indices);
                    });

                // Set model/view/projection uniform matrices.
                uf_model = _program.get_uniform("model");
                uf_view = _program.get_uniform("view");
                uf_projection = _program.get_uniform("projection");
                uf_tex = _program.get_uniform("uf_tex");
            }

            void use() { _program.use(); }

            void draw_sprite(impl::gltexture2d& t, const glm::vec2& position,
                const glm::vec2& origin, const glm::vec2& size,
                float radians) noexcept
            {
                // Reset `model` to identity matrix.
                model = glm::mat4{};

                // Tranformation order:
                // 1) Scale.
                // 2) Rotate.
                // 3) Translate.

                // They will occur in the opposite order below.

                // Translate to `position`.
                model = glm::translate(model, glm::vec3(position, 0.0f));

                // Rotate around origin.
                model =
                    glm::rotate(model, radians, glm::vec3(0.0f, 0.0f, 1.0f));

                // Set origin to the center of the quad.
                model = glm::translate(model, glm::vec3(-origin, 0.0f));

                // Set origin back to `(0, 0)`.
                model = glm::translate(
                    model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));

                // Scale to `size`.
                model = glm::scale(model, glm::vec3(size, 1.0f));

                // Set model/view/projection uniform matrices.
                uf_model.matrix4fv(model);
                uf_view.matrix4fv(view);
                uf_projection.matrix4fv(projection);

                // Activate texture on `GL_TEXTURE0` unit, then bind it.
                t.with(GL_TEXTURE0, [&, this]
                    {
                        // Set texture unit uniform ID.
                        uf_tex.integer(0);

                        // Bind VAO, call `glDrawArrays`, unbind VAO.
                        // _vao->with_draw_arrays<primitive::triangles>(0, 6);
                        _vao->bind();
                        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0);
                        _vao->unbind();

                        // Unbind texture.
                    });
            }
        };
    }
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

        sr.view = glm::translate(sr.view, glm::vec3(500.f, 300.f, 0.f));
        sr.view = glm::scale(sr.view, glm::vec3(0.995f, 0.995f, 1.f));
        sr.view = glm::translate(sr.view, glm::vec3(-500.f, -300.f, 0.f));

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