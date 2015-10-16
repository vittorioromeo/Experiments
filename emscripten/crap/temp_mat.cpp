#include <cfenv>


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
#include <eigen3/Eigen/Dense>

// batch_size = 2
// vertex_count = 2 * 4 = 8
// index_count = 2 * 6 = 12

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
            vec2f _position;
            vec2f _origin;
            vec2f _size;
            vec4f _color;
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
                        _vbo0->buffer_data_items<buffer_usage::static_draw>(
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
                        _vbo1->buffer_data_items<buffer_usage::static_draw>(
                            _indices);
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
                const vec2f& position, const vec2f& origin,
                const vec2f& size, float radians,
                const vec4f& color = vec4f{1.0f, 1.0f, 1.0f, 1.0f},
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
                _model = glm::translate(_model, vec3f(position, 0.0f));

                // Rotate around origin.
                _model =
                    glm::rotate(_model, radians, vec3f(0.0f, 0.0f, 1.0f));

                // Set origin to the center of the quad.
                _model = glm::translate(_model, vec3f(-origin, 0.0f));

                // Set origin back to `(0, 0)`.
                _model = glm::translate(
                    _model, vec3f(-0.5f * size.x, -0.5f * size.y, 0.0f));

                // Scale to `size`.
                _model = glm::scale(_model, vec3f(size, 1.0f));

                // Set model/view/projection uniform matrices.
                _u_projection_view_model.matrix4fv(
                    _projection * _view * _model);

                // Gets the texture unit index from the cache and uses it.
                _u_texture.integer(_texture_cache.use(t));
                _u_color.vec4(color);
                _u_hue.floating(hue);

                // Assumes the VBOs, VAO, and texture unit are bound.
                _vao->draw_elements<primitive::triangles, index_type::ui_byte>(
                    6);
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
            vec4f _pos_tex_coords;
            vec4f _color;
            float _hue;

            // Required to avoid temporary with `emplace_back`.
            bsr_vertex(const vec4f& pos_tex_coords, const vec4f& color,
                float hue) noexcept : _pos_tex_coords(pos_tex_coords),
                                      _color(color),
                                      _hue(hue)
            {
            }
        };

        struct batched_sprite_renderer
        {
            using gl_index_type = GLuint;
            static constexpr sz_t batch_size{1024 * 8};
            static constexpr sz_t vertex_count{batch_size * 4};
            static constexpr sz_t index_count{batch_size * 6};

            program _program{impl::make_batched_sprite_renderer_program()};
            sdl::impl::unique_vao _vao;

            sdl::impl::unique_vbo<buffer_target::array> _vbo0;
            sdl::impl::unique_vbo<buffer_target::element_array> _vbo1;

            glm::mat4 _view;
            glm::mat4 _projection;

            glm::mat4 _projection_view;

            sdl::attribute _a_pos_tex_coords;
            sdl::attribute _a_color;
            sdl::attribute _a_hue;

            sdl::uniform _u_texture;
            sdl::uniform _u_projection_view;

            std::vector<bsr_vertex> _data;
            std::vector<gl_index_type> _indices;
            gl_index_type lasti{0};

            batched_sprite_renderer() noexcept
            {
                _projection = make_2d_projection(1000.f, 600.f);
                init_render_data();
            }

            void init_render_data() noexcept
            {
                // TODO: required?
                // _program.use();


                _vao = sdl::make_vao(1);
                _vbo0 = sdl::make_vbo<buffer_target::array>(1);
                _vbo1 = sdl::make_vbo<buffer_target::element_array>(1);

                // Get attributes.
                _a_pos_tex_coords = _program.get_attribute("a_pos_tex_coords");
                _a_color = _program.get_attribute("a_color");
                _a_hue = _program.get_attribute("a_hue");

                // Get uniforms.
                _u_texture = _program.get_uniform("u_texture");
                _u_projection_view = _program.get_uniform("u_projection_view");

                // Allocates enough memory for `vertex_count` `bsr_vertex`.
                // Creates vertices VBO.
                _data.reserve(vertex_count);
                _vbo0->bind();
                _vbo0->allocate_buffer_items<buffer_usage::dynamic_draw,
                    bsr_vertex>(vertex_count);

                // Allocates enough memory for `index_count` `gl_index_type`.
                // Creates indices VBO.
                _indices.reserve(index_count);
                _vbo1->bind();
                _vbo1->allocate_buffer_items<buffer_usage::dynamic_draw,
                    gl_index_type>(index_count);


                _vao->bind();

                _a_pos_tex_coords.enable() // .
                    .vertex_attrib_pointer_in<bsr_vertex>(4, GL_FLOAT,
                        true, // .
                        offsetof(bsr_vertex, _pos_tex_coords));

                _a_color.enable() // .
                    .vertex_attrib_pointer_in<bsr_vertex>(4, GL_FLOAT,
                        true, // .
                        offsetof(bsr_vertex, _color));

                _a_hue.enable() // .
                    .vertex_attrib_pointer_in<bsr_vertex>(1, GL_FLOAT,
                        true, // .
                        offsetof(bsr_vertex, _hue));
            }

            void use() noexcept
            {
                _program.use();

                _vao->bind();
                _vbo0->bind();
                _vbo1->bind();

                _projection_view = _projection * _view;
                _u_projection_view.matrix4fv(_projection_view);
            }

            template <typename... Ts>
            void enqueue_v(Ts&&... xs) noexcept
            {
                _data.emplace_back(FWD(xs)...);
            }

            template <typename... Ts>
            void enqueue_i(Ts... xs) noexcept
            {
                sdl::for_args(
                    [this](auto i)
                    {
                        _indices.emplace_back(lasti + i);
                    },
                    xs...);

                lasti += 4;
                if(lasti > vertex_count - 3)
                {
                    lasti = 0;
                }
            }


            void draw_sprite(const impl::gltexture2d& t,
                const vec2f& position, const vec2f& origin,
                const vec2f& size, float radians, const vec4f& color,
                float hue) noexcept
            {
                /*
                glm::mat4 _model;

                // Tranformation order:
                // 1) Scale.
                // 2) Rotate.
                // 3) Translate.

                // They will occur in the opposite order below.

                // Translate to `position`.
                _model = glm::translate(_model, vec3f(position, 0.0f));

                // Rotate around origin.
                _model =
                    glm::rotate(_model, radians, vec3f(0.0f, 0.0f, 1.0f));

                // Set origin to the center of the quad.
                _model = glm::translate(_model, vec3f(-origin, 0.0f));

                // Set origin back to `(0, 0)`.
                _model = glm::translate(
                    _model, vec3f(-0.5f * size.x, -0.5f * size.y, 0.0f));

                // Scale to `size`.
                _model = glm::scale(_model, vec3f(size, 1.0f));

                vec4f pos0(0.f, 1.f, 0.f, 1.f);
                vec4f pos1(0.f, 0.f, 0.f, 1.f);
                vec4f pos2(1.f, 0.f, 0.f, 1.f);
                vec4f pos3(1.f, 1.f, 0.f, 1.f);

                vec4f comp0(_model * pos0);
                vec4f comp1(_model * pos1);
                vec4f comp2(_model * pos2);
                vec4f comp3(_model * pos3);

                vec4f pos_tex_coords_0(comp0.x, comp0.y, 0.f, 1.f);
                vec4f pos_tex_coords_1(comp1.x, comp1.y, 0.f, 0.f);
                vec4f pos_tex_coords_2(comp2.x, comp2.y, 1.f, 0.f);
                vec4f pos_tex_coords_3(comp3.x, comp3.y, 1.f, 1.f);
                */



                /*
                vec3f pos0(position.x + size.x * 0.f, position.y + size.y *
                1.f, 1.f);
                vec3f pos1(position.x + size.x * 0.f, position.y + size.y *
                0.f, 1.f);
                vec3f pos2(position.x + size.x * 1.f, position.y + size.y *
                0.f, 1.f);
                vec3f pos3(position.x + size.x * 1.f, position.y + size.y *
                1.f, 1.f);
                */

                /*
                glm::mat3 scaling{
                    // .
                    1.f, 0.f, 0.f,              // .
                    0.f, 1.f, 0.f,              // .
                    0.f, 0.f, 1.f // .
                };
                */

                using eigen_vec3 = Eigen::Vector3f;
                using eigen_mat3 = Eigen::Matrix<float, 3, 3, Eigen::ColMajor>;

                eigen_vec3 pos0(0.f, 1.f, 1.f);
                eigen_vec3 pos1(0.f, 0.f, 1.f);
                eigen_vec3 pos2(1.f, 0.f, 1.f);
                eigen_vec3 pos3(1.f, 1.f, 1.f);

                eigen_mat3 translation, scaling, rotation, shearing_x,
                    shearing_y, origining, origining_2;

                translation << 1.f, 0.f, 0.f,    // .
                    0.f, 1.f, 0.f,               // .
                    position.x, position.y, 1.f; // .
                /*
                glm::mat3 translation{
                    // .

                };

                eigen_mat3 scaling{
                    // .
                    size.x, 0.f, 0.f, // .
                    0.f, size.y, 0.f, // .
                    0.f, 0.f, 1.f     // .
                };

                glm::mat3 rotation{
                    // .
                    std::cos(radians), std::sin(radians), 0.f,  // .
                    -std::sin(radians), std::cos(radians), 0.f, // .
                    0.f, 0.f, 1.f                               // .
                };

                auto shear_x = 0.0f;
                auto shear_y = 0.0f;

                glm::mat3 shearing_x{
                    // .
                    1.f, 0.f, 0.f,      // .
                    -shear_x, 1.f, 0.f, // .
                    0.f, 0.f, 1.f       // .
                };

                glm::mat3 shearing_y{
                    // .
                    1.f, shear_y, 0.f, // .
                    0.f, 1.f, 0.f,      // .
                    0.f, 0.f, 1.f       // .
                };


                glm::mat3 origining{
                    // .
                    1.f, 0.f, 0.f,          // .
                    0.f, 1.f, 0.f,          // .
                    origin.x, origin.y, 1.f // .
                };

                glm::mat3 origining_2{
                    // .
                    1.f, 0.f, 0.f,                    // .
                    0.f, 1.f, 0.f,                    // .
                    -size.x * 0.5, -size.y * 0.5, 1.f // .
                };

*/
                // auto transform(scaling * origining * rotation * translation);
                auto transform(translation * rotation * origining *
                               origining_2 * scaling * shearing_x * shearing_y);

                // auto transform(                    translation * rotation   *
                // shearing_y * shearing_x * scaling * origining);

                eigen_vec3 comp0(transform * pos0);
                eigen_vec3 comp1(transform * pos1);
                eigen_vec3 comp2(transform * pos2);
                eigen_vec3 comp3(transform * pos3);

                // std::cout << comp0.xy() << "\n";

                vec4f pos_tex_coords_0(comp0.x(), comp0.y(), 0.f, 1.f);
                vec4f pos_tex_coords_1(comp1.x(), comp1.y(), 0.f, 0.f);
                vec4f pos_tex_coords_2(comp2.x(), comp2.y(), 1.f, 0.f);
                vec4f pos_tex_coords_3(comp3.x(), comp3.y(), 1.f, 1.f);

                // TODO: compute color + hue on CPU ?

                enqueue_v(pos_tex_coords_0, color, hue);
                enqueue_v(pos_tex_coords_1, color, hue);
                enqueue_v(pos_tex_coords_2, color, hue);
                enqueue_v(pos_tex_coords_3, color, hue);

                enqueue_i(0, 1, 2, 0, 2, 3);
            }

            void do_it()
            {
                _vao->bind();
                _vbo0->bind();
                _vbo1->bind();

                // TODO:
                _u_texture.integer(0);

                auto times(_data.size() / vertex_count);

                for(decltype(times) i(0); i < times; ++i)
                {
                    // Send `vertex_count` vertices to GPU, from
                    // `_data[vertex_count * i]`.
                    _vbo0->sub_buffer_data_items(
                        _data, vertex_count * i, vertex_count);

                    // Send `index_count` vertices to GPU, from
                    // `_indices[index_count * i]`.
                    _vbo1->sub_buffer_data_items(
                        _indices, index_count * i, index_count);

                    _vao->draw_elements<primitive::triangles,
                        index_type::ui_int>(index_count);
                }

                auto quad_count(_data.size() / 4);
                auto remaining_batch_size(quad_count % batch_size);
                if(remaining_batch_size > 0)
                {
                    auto remaining_offset_count(times * batch_size);

                    auto remaining_offset_count_vertex(
                        remaining_offset_count * 4);

                    auto remaining_offset_count_indices(
                        remaining_offset_count * 6);


                    auto remaining_vertex_count(remaining_batch_size * 4);

                    auto remaining_index_count(remaining_batch_size * 6);

                    // Send `vertex_count` vertices to GPU.
                    _vbo0->sub_buffer_data_items(_data,
                        remaining_offset_count_vertex, remaining_vertex_count);

                    // Send `index_count` vertices to GPU.
                    _vbo1->sub_buffer_data_items(_indices,
                        remaining_offset_count_indices, remaining_index_count);

                    _vao->draw_elements<primitive::triangles,
                        index_type::ui_int>(remaining_index_count);
                }

                _data.clear();
                _indices.clear();
                lasti = 0;
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

constexpr sdl::sz_t my_max_entities{45};

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
    vec2f _pos, _origin, _size;
    float _radians{0.f}, _opacity{1.f};
    float _hitbox_radius;
    bool alive{false};

    vec2f vel;
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
        e._origin = vec2f{0, 0};

        e._size = vec2f{texture(e_type::soul)->size()};
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
        e._origin = vec2f{0, 0};

        e._size =
            vec2f{texture(e_type::fireball)->size()} * rndf(0.9f, 1.2f);
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
            vec4f{1.f, 0.f, 1.f, x._opacity}, x.hue);
    }

    auto make_toriel(game_state_type& state, sdl::vec2f pos)
    {
        entity_type e;
        e.type = e_type::toriel;
        e._pos = pos;
        e._origin = vec2f{0, 0};
        e._size = vec2f{texture(e_type::toriel)->size()};
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
                    auto unit_vec(vec2f(std::cos(angle), std::sin(angle)));
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
            this->texture(e_type::fireball)->activate_and_bind(GL_TEXTURE0);
            state.for_alive([this](const auto& e)
                {
                    // TODO: slow
                    // this->texture(e.type)->activate_and_bind(GL_TEXTURE0);
                    // ----------

                    this->sprite_draw(e);
                });


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
// feenableexcept(FE_DIVBYZERO | FE_INVALID | FE_OVERFLOW);

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