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


const char* vShaderStr = R"(
    attribute vec2 position; 
    attribute vec2 tex_coords;
    attribute vec4 color;

    uniform mat4 model;
    uniform mat4 view;
    uniform mat4 projection;

    varying vec2 var_tex_coords;
    varying vec4 var_color;

    void main()
    {
        var_color = color;
        
        gl_Position = projection * view * model * vec4(position.xy, 0.0, 1.0);      
    })";

const char* fShaderStr = R"(
    precision mediump float;

    uniform sampler2D uf_tex;
    
    varying vec2 var_tex_coords;
    varying vec4 var_color;

    void main()
    {        

            

        gl_FragColor = var_color;
    })";



namespace sdl = vrm::sdl;


namespace vrm
{
    namespace sdl
    {
        enum class primitive
        {
            points,
            lines,
            line_strip,
            line_loop,
            triangles,
            triangle_strip,
            triangle_fan

            // quads, // use index buffer (element buffer) + triangles
        };

        namespace impl
        {
            template <primitive TP>
            struct primitive_traits;

            template <>
            struct primitive_traits<primitive::points>
            {
                static constexpr GLenum gl_value{GL_POINTS};
            };

            template <>
            struct primitive_traits<primitive::lines>
            {
                static constexpr GLenum gl_value{GL_LINES};
            };

            template <>
            struct primitive_traits<primitive::line_strip>
            {
                static constexpr GLenum gl_value{GL_LINE_STRIP};
            };

            template <>
            struct primitive_traits<primitive::line_loop>
            {
                static constexpr GLenum gl_value{GL_LINE_LOOP};
            };

            template <>
            struct primitive_traits<primitive::triangles>
            {
                static constexpr GLenum gl_value{GL_TRIANGLES};
            };

            template <>
            struct primitive_traits<primitive::triangle_strip>
            {
                static constexpr GLenum gl_value{GL_TRIANGLE_STRIP};
            };

            template <>
            struct primitive_traits<primitive::triangle_fan>
            {
                static constexpr GLenum gl_value{GL_TRIANGLE_FAN};
            };
        }

        struct color_byte
        {
            GLubyte _r, _g, _b, _a;

            color_byte() noexcept : _r{255}, _g{255}, _b{255}, _a{255} {}

            color_byte(GLubyte r, GLubyte g, GLubyte b) noexcept : _r{r},
                                                                   _g{g},
                                                                   _b{b},
                                                                   _a{255}
            {
            }

            color_byte(GLubyte r, GLubyte g, GLubyte b, GLubyte a) noexcept
                : _r{r},
                  _g{g},
                  _b{b},
                  _a{a}
            {
            }
        };



        // consider?:
        // http://codereview.stackexchange.com/questions/52272/standard-layout-tuple-implementation

        struct vertex2
        {
            vec2<GLfloat> _position;
            vec2<GLfloat> _tex_coords;
            color_byte _color;
            // GLubyte padding[12];

            vertex2(const vec2f& position) noexcept : _position{position} {}

            vertex2(const vec2f& position, const color_byte& color) noexcept
                : _position{position},
                  _color{color}
            {
            }

            vertex2(const vec2f& position, const vec2f& tex_coords) noexcept
                : _position{position},
                  _tex_coords{tex_coords},
                  _color{255, 255, 255, 255}
            {
            }

            vertex2(const vec2f& position, const vec2f& tex_coords,
                const color_byte& color) noexcept : _position{position},
                                                    _tex_coords{tex_coords},
                                                    _color{color}
            {
            }
        };

        static_assert(std::is_standard_layout<vertex2>{}, "");

        constexpr auto vertex2_position_offset(offsetof(vertex2, _position));
        constexpr auto vertex2_tex_coords_offset(
            offsetof(vertex2, _tex_coords));
        constexpr auto vertex2_color_offset(offsetof(vertex2, _color));

        template <primitive TP>
        class primitive_vector
        {
            // TODO:
        public:
            std::vector<vertex2> _vertices;
            sdl::impl::unique_vao _vao;

            using my_primitive_traits = impl::primitive_traits<TP>;

            static constexpr auto primitive_gl_value =
                my_primitive_traits::gl_value;

            static constexpr auto vertex_dimensions = 2;


        public:
            primitive_vector() noexcept { _vao = sdl::make_vao(1); }

            void init(program& p)
            {
                auto use_vertex_attribute = [this, &p](
                    const auto& name, auto type, auto n_values, auto offset)
                {
                    auto a(p.get_attribute(name));
                    a.enable();
                    a.vertex_attrib_pointer(
                        n_values, type, true, sizeof(vertex2), (void*)offset);
                };

                auto vbo = sdl::make_vbo<GL_ARRAY_BUFFER>(1);
                vbo->with([&, this]
                    {
                        vbo->buffer_data(
                            GL_STATIC_DRAW, _vertices.data(), _vertices.size());

                        // vao has to be unbound before the vbo
                        _vao->with([&, this]
                            {
                                // Enable pos attribute
                                use_vertex_attribute("position", GL_FLOAT, 2,
                                    vertex2_position_offset);

                                // use_vertex_attribute("tex_coords", GL_FLOAT,
                                // 2,
                                //     vertex2_position_offset);

                                use_vertex_attribute("color", GL_UNSIGNED_BYTE,
                                    4, vertex2_color_offset);
                            });
                    });
            }

            template <typename... Ts>
            void add(Ts&&... xs)
            {
                _vertices.emplace_back(FWD(xs)...);
            }

            template <typename... Ts>
            void add_more(Ts&&... xs)
            {
                for_args(
                    [this](auto&& x)
                    {
                        add(FWD(x));
                    },
                    FWD(xs)...);
            }



            void draw(program& p)
            {
                _vao->with([this]
                    {
                        glDrawArrays(primitive_gl_value, 0, _vertices.size());
                    });
            }
        };

        const char* sprite_vertex_glsl = R"(
    attribute vec2 position; 
    attribute vec2 tex_coords;
    
    uniform mat4 model;
    uniform mat4 view;
    uniform mat4 projection;

    varying vec2 var_tex_coords;

    void main()
    {
        var_tex_coords = tex_coords;
        gl_Position = projection * view * model * vec4(position.xy, 0.0, 1.0);
    })";

        const char* sprite_fragment_glsl = R"(
    precision mediump float;

    uniform sampler2D uf_tex;
    varying vec2 var_tex_coords;

    void main()
    {        
        gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0) * texture(uf_tex, var_tex_coords);
    })";

        struct sprite_renderer
        {
            program _program;
            sdl::impl::unique_vao _vao;

            mat4f model;
            mat4f view;
            mat4f projection;

            sprite_renderer()
            {
                auto v_sh = make_shader(GL_VERTEX_SHADER, &sprite_vertex_glsl);
                auto f_sh =
                    make_shader(GL_FRAGMENT_SHADER, &sprite_fragment_glsl);

                _program = make_program(*v_sh, *f_sh);

                projection =
                    glm::ortho(0.0f, 1000.0f, 600.0f, 0.0f, -1.0f, 1.0f);

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

                GLfloat vertices[] = {0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
                    0.0f, 0.0f, 0.0f, 0.0f, 0.0f,

                    0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
                    1.0f, 0.0f};

                _vao = make_vao(1);

                _program.use();

                auto vbo = sdl::make_vbo<GL_ARRAY_BUFFER>(1);
                vbo->with([&, this]
                    {
                        // vbo->buffer_data(GL_STATIC_DRAW, vertices);

                        VRM_SDL_GLCHECK(glBufferData(GL_ARRAY_BUFFER,
                            sizeof(vertices), vertices, GL_STATIC_DRAW));

                        // vao has to be unbound before the vbo
                        _vao->with([&, this]
                            {
                                // Enable pos attribute
                                use_vertex_attribute(
                                    "position", GL_FLOAT, 2, 0);

                                VRM_SDL_GLCHECK(glVertexAttribPointer(0, 2,
                                    GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat),
                                    (GLvoid*)0));

                                use_vertex_attribute("tex_coords", GL_FLOAT, 2,
                                    sizeof(float) * 2);

                                VRM_SDL_GLCHECK(glVertexAttribPointer(1, 2,
                                    GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat),
                                    (GLvoid*)(2 * sizeof(GLfloat))));
                            });
                    });

                // Configure VAO/VBO
                // GLuint VBO;


                // glGenVertexArrays(1, &this->quadVAO);
                // glGenBuffers(1, &VBO);

                // glBindBuffer(GL_ARRAY_BUFFER, VBO);
                // glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices,
                //     GL_STATIC_DRAW);

                // glBindVertexArray(this->quadVAO);
                // glEnableVertexAttribArray(0);
                // glVertexAttribPointer(
                // 0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
                // glBindBuffer(GL_ARRAY_BUFFER, 0);
                // glBindVertexArray(0);
            }

            void draw_sprite(texture& t, const vec2f& position,
                const vec2f& size, float radians)
            {
                _program.use();

                model = glm::translate(model, vec3f(position, 0.0f));

                model = glm::translate(
                    model, vec3f(0.5f * size.x, 0.5f * size.y, 0.0f));

                model = glm::rotate(model, radians, vec3f(0.0f, 0.0f, 1.0f));

                model = glm::translate(
                    model, vec3f(-0.5f * size.x, -0.5f * size.y, 0.0f));

                model = glm::scale(model, vec3f(size, 1.0f)); // Last scale

                _program.get_uniform("model").matrix4fv(model);
                _program.get_uniform("view").matrix4fv(view);
                _program.get_uniform("projection").matrix4fv(projection);

                VRM_SDL_GLCHECK(glActiveTexture(GL_TEXTURE0));
                t.bind();

                _vao->with([this]
                    {
                        VRM_SDL_GLCHECK(glDrawArrays(GL_TRIANGLES, 0, 6));
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


    std::random_device rnd_device;
    std::default_random_engine rnd_gen{rnd_device()};

    auto rndf = [&](auto min, auto max)
    {
        using common_min_max_t =
            std::common_type_t<decltype(min), decltype(max)>;

        using dist_t = std::uniform_real_distribution<common_min_max_t>;

        return dist_t(min, max)(rnd_gen);
    };

    auto rnd_color = [&]
    {
        return sdl::color_byte(rndf(0.f, 255.f), rndf(0.f, 255.f),
            rndf(0.f, 255.f), rndf(0.f, 255.f));
    };

    auto add_rnd_triangle = [&](auto& v)
    {
        auto c_x = rndf(0.f, 1000.f);
        auto c_y = rndf(0.f, 1000.f);
        auto sz = rndf(1.01f, 50.05f);

        auto p0 = sdl::make_vec2(c_x - sz, c_y - sz);
        auto p1 = sdl::make_vec2(c_x + sz, c_y - sz);
        auto p2 = sdl::make_vec2(c_x + sz, c_y);

        auto v0 = sdl::vertex2{p0, rnd_color()};
        auto v1 = sdl::vertex2{p1, rnd_color()};
        auto v2 = sdl::vertex2{p2, rnd_color()};

        v.add_more(v0, v1, v2);
    };

    auto c_handle(sdl::make_global_context("test game", 1000, 600));
    auto& c(*c_handle);

    auto toriel_image(c.make_image("files/toriel.png"));
    auto soul_image(c.make_image("files/soul.png"));
    auto fireball_image(c.make_image("files/fireball.png"));

    auto toriel_texture(c.make_texture(*toriel_image));
    auto soul_texture(c.make_texture(*soul_image));
    auto fireball_texture(c.make_texture(*fireball_image));

    sdl::sprite_renderer sr;

    sdl::primitive_vector<sdl::primitive::triangles> triangles;
    add_rnd_triangle(triangles);
    add_rnd_triangle(triangles);
    add_rnd_triangle(triangles);

    // GLfloat vertices[] = {0.0f, 0.5f, 0.5f, -0.5f, -0.5f, -0.5f};

    // Create Vertex Array Object
    /*
    auto vao(sdl::make_vao(1));
    vao->bind();

    // Create a Vertex Buffer Object and copy the vertex data to it
    auto vbo(sdl::make_vbo(1));
    vbo->bind(GL_ARRAY_BUFFER);
    vbo->buffer_data(GL_ARRAY_BUFFER, GL_STATIC_DRAW, vertices);
    */

    auto v_shader = sdl::make_shader(GL_VERTEX_SHADER, &vShaderStr);
    auto f_shader = sdl::make_shader(GL_FRAGMENT_SHADER, &fShaderStr);
    auto program = sdl::make_program(*v_shader, *f_shader);
    program.use();

    mat4f model;
    mat4f view;
    mat4f projection = glm::ortho(0.0f, 1000.0f, 600.0f, 0.0f, -1.0f, 1.0f);

    program.get_uniform("model").matrix4fv(model);
    program.get_uniform("view").matrix4fv(view);
    program.get_uniform("projection").matrix4fv(projection);


    // Specify the layout of the vertex data
    /*
    auto posAttrib(program.get_attribute("position"));
    posAttrib.enable();
    posAttrib.vertex_attrib_pointer(2, GL_FLOAT);
    */

    auto enablePos = [&]
    {
        auto posAttrib(program.get_attribute("position"));
        posAttrib.enable();
        posAttrib.vertex_attrib_pointer(2, GL_FLOAT);
    };

    // enablePos();
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // glEnableVertexAttribArray(posAttrib);
    // glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);

    float timer{10};

    c.update_fn() = [&](auto)
    {
        for(auto& t : triangles._vertices) {
            t._position.y() -= 0.0001f;
        }

        if(c.key(sdl::kkey::space)) {

            for(auto i = 0; i < 10; ++i) add_rnd_triangle(triangles);
        }

        triangles.init(program);

        if(timer-- <= 0) {
            c.title(std::to_string(c.fps()) + " (" +
                    std::to_string(triangles._vertices.size()) + ")");

            timer = 10;
        }
    };


    c.draw_fn() = [&]
    {
        // glDrawArrays(GL_TRIANGLES, 0, 3);

        sr.draw_sprite(*toriel_texture, vec2f{100, 200}, vec2f{100, 200}, 0.f);

        triangles.draw(program);


        // triangle1.draw(enablePos);
    };

    sdl::run_global_context();
    return 0;
}


int main_old(int argc, char** argv)
{
    // SDL_Init(SDL_INIT_VIDEO);

    auto c_handle(sdl::make_global_context("test game", 1000, 600));
    auto& c(*c_handle);

    auto v_shader = sdl::make_shader(GL_VERTEX_SHADER, &vShaderStr);
    auto f_shader = sdl::make_shader(GL_FRAGMENT_SHADER, &fShaderStr);
    auto program = sdl::make_program(*v_shader, *f_shader);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

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
            if((rand() % 100) > 30) {
                for(int i = 0; i < 30; ++i)
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

        // if(rand() % 100 < 20)
        //    std::cout << "(" << c.fps() << ") " << entities.size() << "\n";
    };

    c.draw_fn() = [&]
    {
        /* program.use();
         GLfloat vVertices[] = {
             0.0f, 0.5f, 0.0f, -0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 0.0f};

         // Set the viewport
         glViewport(0, 0, 1000, 600);

         // Clear the color buffer
         glClear(
             GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);


         // Use the program object

         // Load the vertex data
         glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, vVertices);
         glEnableVertexAttribArray(0);

         glDrawArrays(GL_TRIANGLES, 0, 3);*/

        for(auto& e : entities) e._draw_fn(e);

    };

    sdl::run_global_context();

    return 0;
}