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

        const char* sprite_vertex_glsl = R"(
    attribute vec4 vd; 
    // vec2 tex_coords;
    
    uniform mat4 model;
    uniform mat4 view;
    uniform mat4 projection;

    varying vec2 var_tex_coords;

    void main()
    {
        var_tex_coords = vd.zw;
        gl_Position = projection * view * model * vec4(vd.xy, 0.0, 1.0);
        // gl_Position = projection * vec4(position.xy, 0.0, 1.0);
        // gl_Position = vec4(position.xy, 0.0, 1.0);
    })";

        const char* sprite_fragment_glsl = R"(
    precision mediump float;

    uniform sampler2D uf_tex;
    varying vec2 var_tex_coords;

    void main()
    {      
       // gl_FragColor = vec4(1.0, 0.0, 1.0, 1.0);        
        gl_FragColor = texture2D(uf_tex, var_tex_coords);
    })";

        struct sprite_renderer
        {
            program _program;
            sdl::impl::unique_vao _vao;

            glm::mat4 model;
            glm::mat4 view;
            glm::mat4 projection;

            std::vector<float> vertices{0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f,
                1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,

                0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
                1.0f, 0.0f};


            sprite_renderer()
            {
                auto v_sh = make_shader(GL_VERTEX_SHADER, &sprite_vertex_glsl);
                auto f_sh =
                    make_shader(GL_FRAGMENT_SHADER, &sprite_fragment_glsl);

                _program = make_program(*v_sh, *f_sh);

                projection =
                    glm::ortho(0.0f, 1000.0f, 0.0f, 600.0f, -1.0f, 1.0f);

                init_render_data();
            }

            void init_render_data()
            {
                auto use_vertex_attribute = [this](
                    const auto& name, auto type, auto n_values, auto offset)
                {
                    auto a(_program.get_attribute(name));

                    // std::cout << "enabling " << name << ": " << a.location()
                    // <<"\n";

                    a.enable();
                    // a.vertex_attrib_pointer(n_values, type, true,
                    // sizeof(float), (void*)offset);
                    // glVertexAttribPointer(a.location(), 2, GL_FLOAT,
                    // GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
                };


                _program.use();

                // glGenVertexArrays(1, &this->quadVAO);
                _vao = make_vao(1);


                // glGenBuffers(1, &VBO);
                auto vbo = sdl::make_vbo<GL_ARRAY_BUFFER>(1);
                vbo->with([&, this]
                    {
                        // glBindBuffer(GL_ARRAY_BUFFER, VBO);
                        //

                        vbo->buffer_data(
                            GL_STATIC_DRAW, vertices.data(), vertices.size());
                        // glBufferData(GL_ARRAY_BUFFER, sizeof(vertices),
                        // vertices, GL_STATIC_DRAW);
                        // VRM_SDL_GLCHECK(glBufferData(GL_ARRAY_BUFFER,
                        // sizeof(vertices), vertices, GL_STATIC_DRAW));


                        _vao->with([&, this]
                            {
                                // glBindVertexArray(this->quadVAO);

                                // Enable pos attribute
                                // use_vertex_attribute(
                                //    "position", GL_FLOAT, 2, 0);

                                VRM_SDL_GLCHECK(glEnableVertexAttribArray(0));

                                VRM_SDL_GLCHECK(glVertexAttribPointer(0, 4,
                                    GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat),
                                    (GLvoid*)0));

                                // use_vertex_attribute("tex_coords", GL_FLOAT,
                                // 2,
                                //  sizeof(float) * 2);

                                // VRM_SDL_GLCHECK(glEnableVertexAttribArray(1));

                                // VRM_SDL_GLCHECK(glVertexAttribPointer(1, 2,
                                //     GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat),
                                //    (GLvoid*)(2 * sizeof(GLfloat))));
                            });
                    });


                // _program.use();
                // glEnableVertexAttribArray(0);
                // glVertexAttribPointer(
                // 0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
                // glBindBuffer(GL_ARRAY_BUFFER, 0);
                // glBindVertexArray(0);
            }

            void draw_sprite(texture& t, const glm::vec2& position,
                const glm::vec2& size, float radians)
            {
                _program.use();

                model = glm::mat4{};


                model = glm::translate(model, glm::vec3(position, 0.0f));

                model = glm::translate(
                    model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));

                model =
                    glm::rotate(model, radians, glm::vec3(0.0f, 0.0f, 1.0f));

                model = glm::translate(
                    model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));

                model = glm::scale(model, glm::vec3(size, 1.0f)); // Last scale

                // printf("%f, %f\n", position.x, position.y);
                glm::vec4 result =
                    projection * view * model * glm::vec4(position, 0.f, 1.f);
                printf("%f, %f\n\n", result.x, result.y);

                _program.get_uniform("uf_tex").integer(0);

                _program.get_uniform("model").matrix4fv(model);
                _program.get_uniform("view").matrix4fv(view);
                _program.get_uniform("projection").matrix4fv(projection);

                VRM_SDL_GLCHECK(glActiveTexture(GL_TEXTURE0));
                t.bind();

// texture works on EMSCRIPTEN, not on DESKTOP

                _vao->with([this]
                    {
                        VRM_SDL_GLCHECK(glDrawArrays(GL_TRIANGLES, 0, 6));
                        // VRM_SDL_GLCHECK(glDrawArrays(GL_TRIANGLES, 0, 12));
                        // VRM_SDL_GLCHECK(glDrawArrays(GL_TRIANGLES, 0, 24));
                    });

                // this->shader.SetMatrix4("model", model);

                // Render textured quad
                // this->shader.SetVector3f("spriteColor", color);

                //
                // texture.Bind();

                // glBindVertexArray(this->quadVAO);
                // glDrawArrays(GL_TRIANGLES, 0, 6);
                // glBindVertexArray(0);
            }
        };
    }
}

int main(int argc, char** argv)
{
    auto c_handle(sdl::make_global_context("test game", 1000, 600));
    auto& c(*c_handle);

    auto toriel_image(c.make_image("files/toriel.png"));
    auto toriel_texture(c.make_texture(*toriel_image));

    sdl::sprite_renderer sr;

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    float timer{10};

    c.update_fn() = [&](auto)
    {
        if(timer-- <= 0)
        {
            // c.title(std::to_string(c.fps()));
        }
    };


    c.draw_fn() = [&]
    {
        sr.draw_sprite(*toriel_texture, glm::vec2{c.mouse_x(), c.mouse_y()},
            glm::vec2{100, 200}, 0.f);
    };

    sdl::run_global_context();
    return 0;
}