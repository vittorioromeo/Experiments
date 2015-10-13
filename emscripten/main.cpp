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

// batch_size = 2
// vertex_count = 2 * 4 = 8
// indices_count = 2 * 6 = 12

// batch_______________________________    batch_______________________________
// 0---------------   1----------------|   2---------------   3---------------|
//
//                                         0    1    2    3    4   5    6    7
// 0    1    2    3    4    5    6    7    8    9    10   11   12  13   14   15
// v0_0 v0_1 v0_2 v0_3 v1_0 v1_1 v1_2 v1_3 v2_0 v2_1 v2_2 v2_3 v3_0 v3_1 v3_2
// v3_3

// 0 1 2 0 2 3         4 5 6 4 6 7         0 1 2 0 2 3         4 5 6 4 6 7

// i+0 i+1 i+2 i+0 i+2 i+3
// i+=4
// i+0 i+1 i+2 i+0 i+2 i+3
// i+=4
// 4 > vertex_count? yes -> reset i
// i+0 i+1 i+2 i+0 i+2 i+3

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
            auto make_program_from_file(
                const char* vert_path, const char* frag_path)
            {
                auto v_sh(make_shader_from_file<shader_t::vertex>(vert_path));
                auto f_sh(make_shader_from_file<shader_t::fragment>(frag_path));
                return make_program(*v_sh, *f_sh);
            }

            auto make_sprite_renderer_program()
            {
                constexpr auto v_sh_path("vrm/sdl/glsl/sprite.vert");
                constexpr auto f_sh_path("vrm/sdl/glsl/sprite.frag");
                return make_program_from_file(v_sh_path, f_sh_path);
            }

            auto make_batched_sprite_renderer_program()
            {
                constexpr auto v_sh_path("vrm/sdl/glsl/batched_sprite.vert");
                constexpr auto f_sh_path("vrm/sdl/glsl/batched_sprite.frag");
                return make_program_from_file(v_sh_path, f_sh_path);
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

            constexpr auto get_max_texture_unit_count() noexcept
            {
                return GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS;
            }

            constexpr auto get_valid_texture_unit_count(int desired) noexcept
            {
                return std::min(desired, get_max_texture_unit_count());
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
            texture_cache() noexcept { clear(); }

            void clear()
            {
                for(sz_t i(0); i < TN; ++i)
                {
                    // Set cache to "unbound".
                    _last_binds[i] = 0;
                }
            }

            auto use(const impl::gltexture2d& t) noexcept
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

        struct sprite_data
        {
            impl::gltexture2d _texture;
            glm::vec2 _position;
            glm::vec2 _origin;
            glm::vec2 _size;
            glm::vec4 _color;
            float _radians;
            float _hue;
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

            sdl::uniform _u_projection_view_model;
            sdl::uniform _u_texture;
            sdl::uniform _u_color;
            sdl::uniform _u_hue;

            texture_cache<impl::get_valid_texture_unit_count(500)>
                _texture_cache;

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

            void do_it() {}

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
                _u_projection_view_model =
                    _program.get_uniform("u_projection_view_model");

                _u_texture = _program.get_uniform("u_texture");
                _u_color = _program.get_uniform("u_color");
                _u_hue = _program.get_uniform("u_hue");
            }

            void use()
            {
                _program.use();

                _vao->bind();
                _vbo0->bind();
                _vbo1->bind();

                // _texture_cache.clear();
            }



            void draw_sprite(const impl::gltexture2d& t,
                const glm::vec2& position, const glm::vec2& origin,
                const glm::vec2& size, float radians,
                const glm::vec4& color = glm::vec4{1.0f, 1.0f, 1.0f, 1.0f},
                float hue = 0.f) noexcept
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
                _u_projection_view_model.matrix4fv(
                    _projection * _view * _model);

                // Gets the texture unit index from the cache and uses it.
                _u_texture.integer(_texture_cache.use(t));
                _u_color.vec4(color);
                _u_hue.floating(hue);

                // Assumes the VBOs, VAO, and texture unit are bound.
                _vao->draw_elements<primitive::triangles>(GL_UNSIGNED_BYTE, 6);
            }

            void draw_sprite(const sprite_data& d) noexcept
            {
                draw_sprite(d._texture, d._position, d._origin, d._size,
                    d._radians, d._color, d._hue);
            }
        };
    }

    namespace sdl
    {
        struct bsr_vertex
        {
            glm::mat4 _projection_view_model;
            glm::vec4 _pos_tex_coords;
            glm::vec4 _color;
            float _hue;
        };

        struct batched_sprite_renderer
        {
            program _program{impl::make_batched_sprite_renderer_program()};
            sdl::impl::unique_vao _vao;

            sdl::impl::unique_vbo<buffer_target::array> _vbo0;
            sdl::impl::unique_vbo<buffer_target::element_array> _vbo1;

            // glm::mat4 _model;
            glm::mat4 _view;
            glm::mat4 _projection;

            sdl::attribute _a_projection_view_model;
            sdl::attribute _a_pos_tex_coords;
            sdl::attribute _a_color;
            sdl::attribute _a_hue;

            sdl::uniform _u_texture;

            /*
            sdl::uniform _u_projection_view_model;
            sdl::uniform _u_color;
            sdl::uniform _u_hue;
            */

            /*
            texture_cache<impl::get_valid_texture_unit_count(500)>
                _texture_cache;
            */

            std::vector<bsr_vertex> _data;
            std::vector<GLuint> _indices;
            GLuint lasti = 0;
            // int added = 0;

            /*
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
            */

            batched_sprite_renderer()
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
                _vbo1 = sdl::make_vbo<buffer_target::element_array>(1);

                /*
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

                _vbo1->with([&, this]
                    {
                        _vbo1->buffer_data<buffer_usage::static_draw>(_indices);
                    });
                */

                _a_projection_view_model =
                    _program.get_attribute("a_projection_view_model");

                _a_pos_tex_coords = _program.get_attribute("a_pos_tex_coords");
                _a_color = _program.get_attribute("a_color");
                _a_hue = _program.get_attribute("a_hue");

                std::cout << _a_projection_view_model.location() << "\n";
                std::cout << _a_pos_tex_coords.location() << "\n";
                std::cout << _a_color.location() << "\n";
                std::cout << _a_hue.location() << "\n";

                // int temp;
                // std::cin >> temp;


                _u_texture = _program.get_uniform("u_texture");

                /*
                glm::mat4 _projection_view_model;
                glm::vec4 _pos_tex_coords;
                glm::vec4 _color;
                float _hue;
                */
            }

            void use()
            {
                _program.use();

                _vao->bind();
                _vbo0->bind();
                _vbo1->bind();

                constexpr auto pvm_stride_0(
                    sizeof(glm::mat4) + sizeof(glm::vec4) + sizeof(glm::vec4) +
                    sizeof(float) + (sizeof(glm::vec4) * 0));

                constexpr auto pvm_stride_1(
                    sizeof(glm::mat4) + sizeof(glm::vec4) + sizeof(glm::vec4) +
                    sizeof(float) + (sizeof(glm::vec4) * 1));

                constexpr auto pvm_stride_2(
                    sizeof(glm::mat4) + sizeof(glm::vec4) + sizeof(glm::vec4) +
                    sizeof(float) + (sizeof(glm::vec4) * 2));

                constexpr auto pvm_stride_3(
                    sizeof(glm::mat4) + sizeof(glm::vec4) + sizeof(glm::vec4) +
                    sizeof(float) + (sizeof(glm::vec4) * 3));

                constexpr auto pvm_first_0(sizeof(glm::vec4) * 0);
                constexpr auto pvm_first_1(sizeof(glm::vec4) * 1);
                constexpr auto pvm_first_2(sizeof(glm::vec4) * 2);
                constexpr auto pvm_first_3(sizeof(glm::vec4) * 3);

                constexpr auto ptc_stride(sizeof(glm::vec4) +
                                          sizeof(glm::vec4) + sizeof(float) +
                                          sizeof(glm::mat4));

                constexpr auto color_stride(sizeof(glm::vec4) + sizeof(float) +
                                            sizeof(glm::mat4) +
                                            sizeof(glm::vec4));

                constexpr auto hue_stride(sizeof(float) + sizeof(glm::mat4) +
                                          sizeof(glm::vec4) +
                                          sizeof(glm::vec4));

                auto pvm_vap(
                    [this](auto& pvma, auto offset, auto stride, auto first)
                    {
                        pvma.vertex_attrib_pointer(offset, 4, GL_FLOAT, true,
                            stride, (const void*)first);
                    });

                // Set attribute layout.
                //_vao->with([&, this]
                //{
                _a_projection_view_model.enable(4);
                pvm_vap(_a_projection_view_model, 0, 100, pvm_first_0);
                pvm_vap(_a_projection_view_model, 1, 100, pvm_first_1);
                pvm_vap(_a_projection_view_model, 2, 100, pvm_first_2);
                pvm_vap(_a_projection_view_model, 3, 100, pvm_first_3);

                _a_pos_tex_coords.enable().vertex_attrib_pointer(4, GL_FLOAT,
                    true, ptc_stride, (const void*)sizeof(glm::mat4));

                _a_color.enable().vertex_attrib_pointer(4, GL_FLOAT, true,
                    color_stride,
                    (const void*)(sizeof(glm::mat4) + sizeof(glm::vec4)));

                _a_hue.enable().vertex_attrib_pointer(1, GL_FLOAT, true,
                    hue_stride,
                    (const void*)(sizeof(glm::mat4) + sizeof(glm::vec4) +
                                                          sizeof(glm::vec4)));

                /*
                                        vertex_attrib_pointer(layout_offset,
                   sz_t n_components,
                                            GLenum type, bool normalized
                   = true,
                                            sz_t stride = 0,
                                            const GLvoid* first_element
                   = nullptr)
                                            */
                // });



                // _texture_cache.clear();
            }



            void enqueue_v(bsr_vertex&& v) { _data.emplace_back(v); }

            static constexpr sz_t batch_size{1};

            static constexpr sz_t vertex_count{batch_size * 4};

            static constexpr sz_t indices_count{batch_size * 6};

            static constexpr sz_t vertex_bytes{
                vertex_count * sizeof(bsr_vertex)};

            static constexpr sz_t indices_bytes{
                indices_count * sizeof(GLuint)};


            template <typename... Ts>
            void enqueue_i(Ts... xs)
            {
                sdl::for_args(
                    [this](auto i)
                    {
                        _indices.emplace_back(lasti + i);
                      //  std::cout << lasti + i << ", ";
                    },
                    xs...);

                ///std::cout << "\n";
            }


            void draw_sprite(const impl::gltexture2d& t,
                const glm::vec2& position, const glm::vec2& origin,
                const glm::vec2& size, float radians, const glm::vec4& color,
                float hue) noexcept
            {
                // Reset `model` to identity matrix.
                glm::mat4 _model;

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

                /*
                glm::mat4 _projection_view_model;
                glm::vec4 _pos_tex_coords;
                glm::vec4 _color;
                float _hue;

                0.f, 1.f, // sw
                0.f, 1.f, // sw

                0.f, 0.f, // ne
                0.f, 0.f, // ne

                1.f, 0.f, // nw
                1.f, 0.f, // nw

                1.f, 1.f, // se
                1.f, 1.f, // se

                0, 1, 2, // first triangle
                0, 2, 3  // second triangle
                */

                glm::mat4 pvm(_projection * _view * _model);
                // glm::mat4 pvm(_model);
                glm::vec4 pos_tex_coords_0(0.f, 1.f, 0.f, 1.f);
                glm::vec4 pos_tex_coords_1(0.f, 0.f, 0.f, 0.f);
                glm::vec4 pos_tex_coords_2(1.f, 0.f, 1.f, 0.f);
                glm::vec4 pos_tex_coords_3(1.f, 1.f, 1.f, 1.f);

                _data.emplace_back(
                    bsr_vertex{pvm, pos_tex_coords_0, color, hue});
                _data.emplace_back(
                    bsr_vertex{pvm, pos_tex_coords_1, color, hue});
                _data.emplace_back(
                    bsr_vertex{pvm, pos_tex_coords_2, color, hue});
                //_data.emplace_back(bsr_vertex{pvm, pos_tex_coords_0, color,
                // hue});
                //_data.emplace_back(bsr_vertex{pvm, pos_tex_coords_2, color,
                // hue});
                _data.emplace_back(
                    bsr_vertex{pvm, pos_tex_coords_3, color, hue});



                enqueue_i(0, 1, 2, 0, 2, 3);
                lasti += 4;

                //++added;

                if(lasti > vertex_count - 3)
                {
                    lasti = 0;
                    //  added = 0;
                }

                // Set model/view/projection uniform matrices.
                //_u_projection_view_model.matrix4fv();

                // Gets the texture unit index from the cache and uses it.
                //_u_texture.integer(_texture_cache.use(t));
                //_u_color.vec4(color);
                //_u_hue.floating(hue);

                // Assumes the VBOs, VAO, and texture unit are bound.
                //_vao->draw_elements<primitive::triangles>(GL_UNSIGNED_BYTE,
                // 6);
            }

            void do_it()
            {



                _vao->bind();
                _vbo0->bind();
                _vbo1->bind();
                _u_texture.integer(0);

                auto times(_data.size() / vertex_count);

                // std::cout << "times = " << times << "\n\n";

                if(times > 0)
                {
                    /*
                    std::cout << "vertex_count = " << vertex_count << "\n";
                    std::cout << "indices_count = " << indices_count << "\n";

                    std::cout << "\n";

                    std::cout << "vertex_bytes = " << vertex_bytes << "\n";
                    std::cout << "indices_bytes = " << indices_bytes << "\n";

                    std::cout << "\n";

                    std::cout << "_data.size() = " << _data.size() << "\n";
                    std::cout << "_indices.size() = " << _indices.size()
                              << "\n";

                    std::cout << "\n";

                    std::cout
                        << "_data bytes = " << sizeof(bsr_vertex) * _data.size()
                        << "\n";
                    std::cout << "_indices bytes = "
                              << sizeof(GLubyte) * _indices.size() << "\n";

                    std::cout << "\n";
*/
                    glBufferData(GL_ARRAY_BUFFER, vertex_bytes, nullptr,
                        GL_DYNAMIC_DRAW);

                    /*
                                        _indices.clear();
                                        for(auto i = 0; i < batch_size; ++i)
                                        {
                                            auto x = i*4;
                                            enqueue_i(x + 0, x + 1, x + 2, x +
                       0, x + 2, x + 3);
                                        }

                                        for(auto i : _indices)
                                        {
                                            std::cout << (int)i << ", ";
                                        }
                    */
                    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_bytes,
                        nullptr, GL_DYNAMIC_DRAW);

                    // _vbo0->buffer_data<buffer_usage::dynamic_draw>(_data,
                    // vertex_count, vertex_count);
                    // _vbo1->buffer_data<buffer_usage::dynamic_draw>(_indices,
                    // indices_count, indices_count);
                }

                for(decltype(times) i(0); i < times; ++i)
                {


                    /*
                    std::cout << "vertex offset = " << vertex_count * i << "\n";
                    std::cout << "indices offset = " << indices_count * i
                              << "\n";
*/

                    // Send `vertex_count` vertices to GPU.
                    _vbo0->sub_buffer_data(GL_ARRAY_BUFFER, 0,
                        (GLsizeiptr)vertex_bytes,
                        (const void*)(_data.data() + vertex_count * i));

                    //_vbo0->buffer_data<buffer_usage::static_draw>(_data,
                    // vertex_count, vertex_count * i);

                    // Send `indices_count` vertices to GPU.
                    _vbo1->sub_buffer_data(GL_ELEMENT_ARRAY_BUFFER, 0,
                        (GLsizeiptr)indices_bytes,
                        (const void*)(_indices.data() + indices_count * i));

                    // _vbo1->buffer_data<buffer_usage::static_draw>(_indices,
                    // indices_count, indices_count * i);

                    // std::cout << "\nbuffers filled\n";

                    // glDrawArrays(GL_TRIANGLES, 0, vertex_count);
                    glDrawElements(
                        GL_TRIANGLES, indices_count, GL_UNSIGNED_INT, 0);

                    // _vao->draw_elements<primitive::triangles>(GL_UNSIGNED_BYTE,
                    // indices_count);

                    // std::cout << "elements drawn\n\n\n";
                }

                _data.clear();
                _indices.clear();
                lasti = 0;
                // added = 0;
            }
        };
    }
}

std::random_device rnd_device;
std::default_random_engine rnd_gen{rnd_device()};

auto rndf = [](auto min, auto max)
{
    using common_min_max_t = std::common_type_t<decltype(min), decltype(max)>;

    using dist_t = std::uniform_real_distribution<common_min_max_t>;

    return dist_t(min, max)(rnd_gen);
};

template <typename T, std::size_t TSize>
class sparse_int_set
{
private:
    static constexpr std::size_t null_value{TSize + 1};

    std::vector<T> _dense;
    std::vector<std::size_t> _sparse;
    std::size_t _size;

    // std::array<T, TSize> _dense;
    // std::array<std::size_t, TSize> _sparse;

public:
    sparse_int_set() noexcept
    {
        _dense.resize(TSize);
        _sparse.resize(TSize);
        clear();
    }

    sparse_int_set(const sparse_int_set& rhs)
        : _dense(rhs._dense), _sparse(rhs._sparse), _size(rhs._size)
    {
    }

    sparse_int_set& operator=(const sparse_int_set& rhs)
    {
        _dense = rhs._dense;
        _sparse = rhs._sparse;
        _size = rhs._size;

        return *this;
    }

    sparse_int_set(sparse_int_set&&) = default;
    sparse_int_set& operator=(sparse_int_set&&) = default;

    bool has(const T& x) const noexcept
    {
        assert(x < TSize);
        return _sparse[x] != null_value;
    }

    bool add(const T& x) noexcept
    {
        assert(x < TSize);
        if(has(x)) return false;

        assert(_size < TSize);
        _dense[_size] = x;

        _sparse[x] = _size;
        ++_size;

        return true;
    }

    bool erase(const T& x) noexcept
    {
        assert(x < TSize);
        if(!has(x)) return false;

        auto ptr(_sparse[x]);
        assert(_size > 0);

        assert(has(_dense[_size - 1]));
        auto last(_dense[_size - 1]);
        assert(ptr != null_value);

        if(_dense[ptr] != last)
        {
            _dense[ptr] = last;
            _sparse[last] = ptr;
        }

        assert(has(x));
        _sparse[x] = null_value;

        assert(_size > 0);
        --_size;

        return true;
    }

    void clear() noexcept
    {
        for(auto& p : _sparse) p = null_value;
        _size = 0;
    }

    bool empty() const noexcept { return _size == 0; }

    void pop_back() noexcept
    {
        assert(_size > 0);
        erase(back());
    }

    auto back() const noexcept
    {
        assert(_size > 0);

        assert(has(_dense[_size - 1]));
        return _dense[_size - 1];
    }

    template <typename TF>
    void for_each(TF&& f) const noexcept
    {
        assert(_size <= TSize);

        for(decltype(_size) i(0); i < _size; ++i)
        {
            assert(has(_dense[i]));
            f(_dense[i]);
        }
    }

    auto operator[](std::size_t i) const noexcept
    {
        assert(i < _size);

        assert(has(_dense[i]));
        return _dense[i];
    }

    auto size() const noexcept { return _size; }
};

constexpr sdl::sz_t my_max_entities{8};

enum class e_type : int
{
    soul = 0,
    fireball = 1,
    toriel = 2
};

constexpr int e_type_count{3};

struct my_game_entity
{
    e_type type;
    glm::vec2 _pos, _origin, _size;
    float _radians{0.f}, _opacity{1.f};
    float _hitbox_radius;
    bool alive{false};

    glm::vec2 vel;
    float speed, hue{0.f}, curve, life;
    int dir;

    my_game_entity() noexcept {}

    my_game_entity(const my_game_entity& rhs) noexcept = default;
    my_game_entity& operator=(const my_game_entity& rhs) noexcept = default;

    my_game_entity(my_game_entity&& rhs) noexcept = default;
    my_game_entity& operator=(my_game_entity&& rhs) noexcept = default;
};


struct my_game_state
{
    using my_intset = sparse_int_set<std::size_t, my_max_entities>;
    using entity_type = my_game_entity;

    // std::array<entity_type, my_max_entities> _entities;
    std::vector<entity_type> _entities;
    my_intset _free, _alive;
    sdl::sz_t _soul_idx;

    my_game_state()
    {
        _entities.resize(my_max_entities);
        for(sdl::sz_t i(0); i < my_max_entities; ++i) _free.add(i);
    }


    my_game_state(const my_game_state& rhs)
        : _entities(rhs._entities), _free(rhs._free), _alive(rhs._alive),
          _soul_idx(rhs._soul_idx)
    {
        // std::cout << "copy ctor :(\n";
    }

    my_game_state& operator=(const my_game_state& rhs)
    {
        // std::cout << "copy :(\n";

        _entities = rhs._entities;
        _free = rhs._free;
        _alive = rhs._alive;
        _soul_idx = rhs._soul_idx;

        return *this;
    }

    my_game_state(my_game_state&& rhs) noexcept
        : _entities(std::move(rhs._entities)),
          _free(std::move(rhs._free)),
          _alive(std::move(rhs._alive)),
          _soul_idx(std::move(rhs._soul_idx))
    {
        //  std::cout << "move ctor!\n";
    }

    my_game_state& operator=(my_game_state&& rhs) noexcept
    {
        // std::cout << "move!\n";

        _entities = std::move(rhs._entities);
        _free = std::move(rhs._free);
        _alive = std::move(rhs._alive);
        _soul_idx = std::move(rhs._soul_idx);

        return *this;
    }

    auto add(const entity_type& e)
    {
        assert(!_free.empty());

        auto fi(_free.back());
        _free.pop_back();

        auto& res(_entities[fi]);
        res = e;

        assert(!res.alive);
        res.alive = true;



        assert(!_alive.has(fi));
        _alive.add(fi);
        assert(!_free.has(fi));
        assert(_alive.has(fi));

        return fi;
    }

    template <typename TF>
    void for_alive_indices(TF&& f)
    {
        _alive.for_each([this, &f](auto i)
            {
                f(i);
            });
    }

    template <typename TF>
    void for_alive_indices(TF&& f) const
    {
        _alive.for_each([this, &f](auto i)
            {
                f(i);
            });
    }

    template <typename TF>
    void for_alive(TF&& f)
    {
        _alive.for_each([this, &f](auto i)
            {
                f(_entities[i]);
            });
    }

    template <typename TF>
    void for_alive(TF&& f) const
    {
        _alive.for_each([this, &f](auto i)
            {
                f(_entities[i]);
            });
    }

    void reclaim()
    {
        auto to_erase_begin(_free.size());


        _alive.for_each([this](auto i)
            {
                if(!_entities[i].alive)
                {
                    assert(_alive.has(i));
                    assert(!_free.has(i));

                    _free.add(i);

                    assert(_free.has(i));
                    assert(_alive.has(i));
                }
            });


        for(auto i(to_erase_begin); i < _free.size(); ++i)
        {

            assert(_alive.has(_free[i]));

            _alive.erase(_free[i]);

            assert(!_alive.has(_free[i]));
        }
    }


    auto& soul() noexcept { return _entities[_soul_idx]; }
};


template <typename TContext>
struct my_game
{
    using this_type = my_game<TContext>;
    using game_state_type = my_game_state;
    using entity_type = typename game_state_type::entity_type;

    TContext _context;
    sdl::batched_sprite_renderer sr;
    // sdl::sprite_renderer sr;

    std::array<sdl::impl::unique_gltexture2d, e_type_count> _texture_array;

    auto& texture(e_type type) noexcept
    {
        return _texture_array[static_cast<int>(type)];
    }


    auto make_texture_from_image(const std::string& path)
    {
        auto s(_context.make_surface(path));
        return sdl::make_gltexture2d(*s);
    }

    auto make_soul(sdl::vec2f pos)
    {
        entity_type e;
        e.type = e_type::soul;
        e._pos = pos;
        e._origin = glm::vec2{0, 0};

        e._size = glm::vec2{texture(e_type::soul)->size()};
        e._hitbox_radius = 3.f;

        return e;
    };

    auto soul_update()
    {
        return [this](auto& x, auto&, auto step)
        {
            constexpr float speed{5.f};
            sdl::vec2f input;

            if(_context.key(sdl::kkey::left))
                input.x = -1;
            else if(_context.key(sdl::kkey::right))
                input.x = 1;

            if(_context.key(sdl::kkey::up))
                input.y = -1;
            else if(_context.key(sdl::kkey::down))
                input.y = 1;

            x._pos += input * (speed * step);
        };
    }



    auto make_fireball(sdl::vec2f pos, sdl::vec2f vel, float speed)
    {
        entity_type e;
        e.type = e_type::fireball;
        e._pos = pos;
        e._radians = static_cast<float>(rand() % 6280) / 1000.f;
        e._origin = glm::vec2{0, 0};

        e._size =
            glm::vec2{texture(e_type::fireball)->size()} * rndf(0.9f, 1.2f);
        e._hitbox_radius = 3.f;
        e._opacity = 0.f;
        e.vel = vel;
        e.speed = speed;
        e.hue = rndf(-0.6f, 1.1f);
        e.curve = rndf(-1.f, 1.f);
        e.dir = rand() % 2;
        e.life = 100.f;

        return e;
    };

    auto fireball_update()
    {
        return [this](auto& x, auto&, auto step)
        {
            x._pos += x.vel * (x.speed * step);
            // x._radians += dir ? 0.2 * step : -0.2 * step;
            x._radians += 0.01f * step;

            x.vel = glm::rotate(x.vel, x.curve * 0.1f * step);

            if(std::abs(x.curve) > 0.01)
            {
                x.curve *= 0.5f;
            }

            x.life -= step * 0.3f;

            if(x.life <= 0.f) x.alive = false;
            if(x.life <= 10.f) x._opacity -= step * 0.2f;

            if(x._opacity <= 1.f) x._opacity += step * 0.1f;
        };
    }

    auto sprite_draw(const entity_type& x)
    {
        sr.draw_sprite(*texture(x.type), x._pos, x._origin, x._size, x._radians,
            glm::vec4{1.f, 0.f, 1.f, x._opacity}, x.hue);
    }

    auto make_toriel(game_state_type& state, sdl::vec2f pos)
    {
        entity_type e;
        e.type = e_type::toriel;
        e._pos = pos;
        e._origin = glm::vec2{0, 0};
        e._size = glm::vec2{texture(e_type::toriel)->size()};
        e._hitbox_radius = 30.f;


        return e;
    };

    auto toriel_update()
    {
        return [this](auto& x, auto& state, auto step)
        {
            x.curve -= step;
            if(x.curve <= 0.f)
            {
                x.curve = 10.f;

                for(int i = 0; i < 1000; ++i)
                {
                    if(state._free.empty()) break;

                    auto angle(rndf(0.f, sdl::tau));
                    auto speed(rndf(0.1f, 3.f));
                    auto unit_vec(glm::vec2(std::cos(angle), std::sin(angle)));
                    auto vel(unit_vec * speed);

                    state.add(this->make_fireball(
                        x._pos + unit_vec * rndf(55.f, 90.f), vel,
                        1.f + (rand() % 100) / 80.f));
                }
            }
        };
    }


    my_game(TContext&& context) : _context(FWD(context))
    {
        texture(e_type::soul) = make_texture_from_image("files/soul.png");

        texture(e_type::fireball) =
            make_texture_from_image("files/fireball.png");

        texture(e_type::toriel) = make_texture_from_image("files/toriel.png");

        {
            auto& state(_context.current_state());
            auto& entities(state._entities);

            state.add(make_toriel(state, sdl::make_vec2(500.f, 100.f)));
            state._soul_idx =
                state.add(make_soul(sdl::make_vec2(500.f, 500.f)));
        }

        _context.update_fn() = [&, this](auto& state, auto step)
        {
            const auto& entities(state._entities);
            auto& soul(state.soul());

            state.for_alive([this, &state, step](auto& e)
                {
                    if(e.type == e_type::soul)
                    {
                        this->soul_update()(e, state, step);
                    }
                    else if(e.type == e_type::fireball)
                    {
                        this->fireball_update()(e, state, step);
                    }
                    else if(e.type == e_type::toriel)
                    {
                        this->toriel_update()(e, state, step);
                    }
                });

            state.reclaim();

            if(_context.key(sdl::kkey::q)) _context.fps_limit += step;
            if(_context.key(sdl::kkey::e)) _context.fps_limit -= step;

            if(_context.key(sdl::kkey::escape))
            {
                sdl::stop_global_context();
            }

            if(rand() % 100 < 30)
            {
                auto fps_str(std::to_string(_context.fps()));
                auto fps_limit_str(
                    std::to_string(static_cast<int>(_context.fps_limit)));
                auto update_ms_str(std::to_string(_context.update_ms()));
                auto draw_ms_str(std::to_string(_context.draw_ms()));

                _context.title("FPS: " + fps_str + "/" + fps_limit_str +
                               "\tU: " + update_ms_str + "\tD: " + draw_ms_str);
            }


            /*
            if(rand() % 100 < 30)
            {
                _context.title(std::to_string(entities.size()) + " ||| " +
                               std::to_string(_context.fps_limit) + " | " +
                               std::to_string(_context._static_timer._loops) +
                               " ||| " + std::to_string(_context.fps()) +
                               " | " + std::to_string(_context.real_ms()) +
                               " | " + std::to_string(_context.update_ms()));
            }
            */
        };

        _context.draw_fn() = [&, this](const auto& state)
        {
            sr.use();
            state.for_alive([this](const auto& e)
                {
                    this->sprite_draw(e);
                });

            this->texture(e_type::fireball)->activate_and_bind(GL_TEXTURE0);
            sr.do_it();

            //  std::cout << "\n\ndraw end\n";
        };

        _context.interpolate_fn() = [&, this](
            auto& interpolated, const auto& s0, const auto& s1, float t)
        {
            auto lerp = [t](const auto& v0, const auto& v1)
            {
                return (1.f - t) * v0 + t * v1;
            };

            auto& in_entities(interpolated._entities);

            interpolated._alive = s0._alive;
            interpolated.for_alive_indices([&](auto i)
                {
                    const auto& e0(s0._entities[i]);
                    const auto& e1(s1._entities[i]);
                    auto& ei(in_entities[i]);

                    ei._pos = lerp(e0._pos, e1._pos);
                    ei.type = e1.type;
                    ei._origin = lerp(e0._origin, e1._origin);
                    ei._size = lerp(e0._size, e1._size);
                    ei._radians = e0._radians; // TODO: lerpradians
                    ei._opacity = lerp(e0._opacity, e1._opacity);
                    ei.hue = e0.hue;
                });

        };
    }
};

using my_context_settings = sdl::context_settings<my_game_state>;

int main()
{
#define COUT_SIZE(...)                                                      \
    std::cout << "sizeof(" << #__VA_ARGS__ << ") = " << sizeof(__VA_ARGS__) \
              << "\n"

    COUT_SIZE(my_game_entity);
    COUT_SIZE(std::array<my_game_entity, my_max_entities>);
    COUT_SIZE(sparse_int_set<std::size_t, my_max_entities>);

    auto c_handle(
        sdl::make_global_context<my_context_settings>("test game", 1000, 600));

    auto& c(*c_handle);
    auto game(std::make_unique<my_game<decltype(c)>>(c));
    sdl::run_global_context();

    return 0;
}