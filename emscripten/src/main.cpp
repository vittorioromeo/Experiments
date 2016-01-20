// #include <windows.h>
#include <cfenv>
#include <sstream>
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
#include <vrm/gl.hpp>

namespace sdl = vrm::sdl;

VRM_SDL_NAMESPACE
{
    template <typename T>
    VRM_SDL_ALWAYS_INLINE auto nearest_power_of_two(const T& x) noexcept
    {
        return std::pow(2, std::ceil(std::log(x) / std::log(2)));
    }

    namespace impl
    {
        class atlas_builder_input
        {
        private:
            const sdl::surface* _surface;

        public:
            atlas_builder_input(const surface& surface) noexcept
                : _surface{&surface}
            {
            }

            const auto& surface() const noexcept { return *_surface; }
            const auto& width() const noexcept { return _surface->width(); }
            const auto& height() const noexcept { return _surface->height(); }

            auto pixels() const noexcept { return _surface->pixels(); }
        };
    }

    class quad_tex_coords
    {
    private:
        vec2f _tx0; // (0.f, 1.f) (NE)
        vec2f _tx1; // (0.f, 0.f) (NW)
        vec2f _tx2; // (1.f, 0.f) (SE)
        vec2f _tx3; // (1.f, 1.f) (SW)

    public:
        quad_tex_coords() = default;
        quad_tex_coords(const vec2f& tx0, const vec2f& tx1, const vec2f& tx2,
            const vec2f& tx3) noexcept : _tx0{tx0},
                                         _tx1{tx1},
                                         _tx2{tx2},
                                         _tx3{tx3}
        {
        }

        const auto& tx0() const noexcept { return _tx0; }
        const auto& tx1() const noexcept { return _tx1; }
        const auto& tx2() const noexcept { return _tx2; }
        const auto& tx3() const noexcept { return _tx3; }
    };

    namespace atlas_strategy
    {
        struct naive
        {
            template <typename TOutput, typename TInputVec>
            auto build(sz_t capacity, impl::gltexture2d& target,
                TInputVec&& input_vec, const vec2i& spacing)
            {
                std::vector<TOutput> result;
                result.reserve(capacity);

                vec2i target_size{0, 0};

                for(auto& i : input_vec)
                {
                    auto sw(i.width());
                    auto sh(i.height());

                    target_size.x += sw + spacing.x;
                    if(sh > target_size.y) target_size.y = sh;
                }

                // target_size.y += spacing.y;

                target.generate_blank(target_size);
                target.bind();

                auto to_tex_coords([&](float x, float y)
                    {
                        // return v;
                        return vec2f(x / target_size.x, y / target_size.y);
                    });

                vec2i current_offset{0, 0};
                for(auto& i : input_vec)
                {
                    auto sw(i.width());
                    auto sh(i.height());

                    float w(current_offset.x);
                    float e(current_offset.x + sw);
                    float n(current_offset.y);
                    float s(current_offset.y + sh);

                    result.emplace_back(     // .
                        to_tex_coords(e, n), // (0.f, 1.f) (NE)
                        to_tex_coords(w, n), // (0.f, 0.f) (NW)
                        to_tex_coords(e, s), // (1.f, 0.f) (SE)
                        to_tex_coords(w, s)  // (1.f, 1.f) (SW)
                        );

                    target.sub_image_2d(current_offset, i.surface());
                    current_offset.x += sw + spacing.x;
                }

                return result;
            }
        };
    }

    template <typename TStrategy = atlas_strategy::naive>
    class atlas_builder
    {
    private:
        using input_type = impl::atlas_builder_input;
        using output_type = quad_tex_coords;

        std::vector<input_type> _in;
        sz_t _capacity;
        vec2i _spacing{2, 2};
        TStrategy _strategy;

    public:
        atlas_builder(sz_t capacity) : _capacity{capacity}
        {
            _in.reserve(capacity);
        }

        template <typename... Ts>
        void add(Ts&&... xs)
        {
            VRM_CORE_ASSERT(_in.size() < _capacity);
            _in.emplace_back(FWD(xs)...);
        }

        auto build(impl::gltexture2d& target)
        {
            return _strategy.template build<output_type>(
                _capacity, target, _in, _spacing);
        }
    };

    class atlas
    {
    private:
        sdl::impl::unique_gltexture2d _texture;
        std::vector<vec2f> _texture_sizes;
        std::vector<quad_tex_coords> _texture_coords;

    public:
        atlas() = default;

        auto& texture() noexcept { return _texture; }
        const auto& texture() const noexcept { return _texture; }

        void add_size(const vec2f& s) { _texture_sizes.emplace_back(s); }
        void add_coords(const quad_tex_coords& c)
        {
            _texture_coords.emplace_back(c);
        }

        const auto& size(sz_t idx) const noexcept
        {
            VRM_CORE_ASSERT(_texture_sizes.size() > idx);
            return _texture_sizes[idx];
        }

        const auto& coords(sz_t idx) const noexcept
        {
            VRM_CORE_ASSERT(_texture_coords.size() > idx);
            return _texture_coords[idx];
        }
    };

    template <typename TStrategy = atlas_strategy::naive, typename... Ts>
    auto make_atlas_from_surfaces(Ts && ... surfaces)
    {
        atlas result;
        atlas_builder<TStrategy> ab{sizeof...(Ts)};

        vrmc::for_args(
            [&result, &ab](auto&& surface)
            {
                ab.add(*surface);
                result.add_size(vec2f(surface->width(), surface->height()));
            },
            FWD(surfaces)...);

        result.texture() = make_gltexture2d();
        auto ab_output(ab.build(*result.texture()));

        for(const auto& o : ab_output) result.add_coords(o);

        return result;
    }

    template <typename T>
    auto make_surface_from_path(T && path)
    {
        return impl::unique_surface(FWD(path));
    }

    template <typename TStrategy = atlas_strategy::naive, typename... Ts>
    auto make_atlas_from_paths(Ts && ... paths)
    {
        return make_atlas_from_surfaces(FWD(make_surface_from_path(paths))...);
    }

    namespace impl
    {
        const auto& shader_root() noexcept
        {
            static std::string result{"include/vrm/sdl/glsl/"};
            return result;
        }

        auto make_sprite_renderer_program()
        {
            auto v_sh_path(shader_root() + "sprite.vert");
            auto f_sh_path(shader_root() + "sprite.frag");
            return make_program_from_file(v_sh_path.c_str(), f_sh_path.c_str());
        }

        auto make_batched_sprite_renderer_program()
        {
            auto v_sh_path(shader_root() + "batched_sprite.vert");
            auto f_sh_path(shader_root() + "batched_sprite.frag");
            return make_program_from_file(v_sh_path.c_str(), f_sh_path.c_str());
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
}
VRM_SDL_NAMESPACE_END

#define VRM_SDL_AUTO_VERTEX_ATTRIB_POINTER(                         \
    attribute_handle, vertex_type, member_name, normalized)         \
    do                                                              \
    {                                                               \
        attribute_handle.enable()                                   \
            .vertex_attrib_pointer_in<vertex_type,                  \
                decltype(std::declval<vertex_type>().member_name)>( \
                normalized, offsetof(vertex_type, member_name));    \
    } while(false)

VRM_SDL_NAMESPACE
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

        sdl::uniform _u_texture;
        sdl::uniform _u_projection_view;

        std::vector<bsr_vertex> _data;
        std::vector<gl_index_type> _indices;
        gl_index_type _current_batch_vertex_count{0};

        batched_sprite_renderer() noexcept
        {
            // _projection = impl::make_2d_projection(1000.f, 600.f);
            init_render_data();
        }

        void init_render_data() noexcept
        {
            _vao = sdl::make_vao();

            // The VAO "contains" the VBOs.
            _vao->bind();

            _vbo0 = sdl::make_vbo<buffer_target::array>();
            _vbo1 = sdl::make_vbo<buffer_target::element_array>();

            // Get attributes.
            auto _a_pos_tex_coords = _program.attribute("a_pos_tex_coords");
            auto _a_color = _program.attribute("a_color");
            auto _a_hue = _program.attribute("a_hue");

            // Get uniforms.
            _u_texture = _program.uniform("u_texture");
            _u_projection_view = _program.uniform("u_projection_view");

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

            VRM_SDL_AUTO_VERTEX_ATTRIB_POINTER(
                _a_pos_tex_coords, bsr_vertex, _pos_tex_coords, true);

            VRM_SDL_AUTO_VERTEX_ATTRIB_POINTER(
                _a_color, bsr_vertex, _color, true);

            VRM_SDL_AUTO_VERTEX_ATTRIB_POINTER(_a_hue, bsr_vertex, _hue, true);
        }

        void use(const mat4f& projection_view) noexcept
        {
            _program.use();
            _u_projection_view.mat4(projection_view);
        }

        void use(const camera_2d& camera) noexcept
        {
            use(camera.projection_view());
        }

    private:
        template <typename... Ts>
        VRM_SDL_ALWAYS_INLINE void enqueue_v(Ts&&... xs) noexcept
        {
            _data.emplace_back(FWD(xs)...);
        }

        template <typename... Ts>
        VRM_SDL_ALWAYS_INLINE void enqueue_i(Ts&&... xs) noexcept
        {
            vrmc::for_args(
                [this](auto&& i)
                {
                    _indices.emplace_back(_current_batch_vertex_count + FWD(i));
                },
                FWD(xs)...);

            _current_batch_vertex_count += 4;

            if(_current_batch_vertex_count > vertex_count - 3)
            {
                _current_batch_vertex_count = 0;
            }
        }

    public:
        void draw_sprite(const impl::gltexture2d& t,
            const quad_tex_coords& tex_coords, const vec2f& position,
            const vec2f& origin, const vec2f& size, float radians,
            const vec4f& color, float hue) noexcept
        {
            vec2f shear(0.f, 0.f);

            const vec3f pos0(0.f, 1.f, 1.f);
            const vec3f pos1(0.f, 0.f, 1.f);
            const vec3f pos2(1.f, 0.f, 1.f);
            const vec3f pos3(1.f, 1.f, 1.f);

            auto transform(impl::trasform_matrix_2d(
                position, origin, size, radians, shear));

            vec3f comp0(transform * pos0);
            vec3f comp1(transform * pos1);
            vec3f comp2(transform * pos2);
            vec3f comp3(transform * pos3);

            // TODO: "pixel_perfect" parameter or something like that
            vec4f pos_tex_coords_0(vec2i(comp0.xy()), tex_coords.tx2());
            vec4f pos_tex_coords_1(vec2i(comp1.xy()), tex_coords.tx0());
            vec4f pos_tex_coords_2(vec2i(comp2.xy()), tex_coords.tx1());
            vec4f pos_tex_coords_3(vec2i(comp3.xy()), tex_coords.tx3());

            enqueue_v(pos_tex_coords_0, color, hue);
            enqueue_v(pos_tex_coords_1, color, hue);
            enqueue_v(pos_tex_coords_2, color, hue);
            enqueue_v(pos_tex_coords_3, color, hue);

            enqueue_i(0, 1, 2, 0, 2, 3);
        }

        void do_it()
        {
            _vao->bind();

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

                _vao->draw_elements<primitive::triangles, index_type::ui_int>(
                    index_count);
            }

            auto total_quad_count(_data.size() / 4);
            auto rest_quad_count(total_quad_count % batch_size);

            if(rest_quad_count > 0)
            {
                auto rest_offset_count(times * batch_size);
                auto rest_offset_count_vertex(rest_offset_count * 4);
                auto rest_offset_count_indices(rest_offset_count * 6);
                auto rest_vertex_count(rest_quad_count * 4);
                auto rest_index_count(rest_quad_count * 6);

                // Send `vertex_count` vertices to GPU.
                _vbo0->sub_buffer_data_items(
                    _data, rest_offset_count_vertex, rest_vertex_count);

                // Send `index_count` vertices to GPU.
                _vbo1->sub_buffer_data_items(
                    _indices, rest_offset_count_indices, rest_index_count);

                _vao->draw_elements<primitive::triangles, index_type::ui_int>(
                    rest_index_count);
            }

            _data.clear();
            _indices.clear();
            _current_batch_vertex_count = 0;
        }
    };
}
VRM_SDL_NAMESPACE_END

std::random_device rnd_device;
std::default_random_engine rnd_gen{rnd_device()};

auto rndf = [](auto min, auto max)
{
    using common_min_max_t = std::common_type_t<decltype(min), decltype(max)>;
    using dist_t = std::uniform_real_distribution<common_min_max_t>;
    return dist_t(min, max)(rnd_gen);
};



constexpr sdl::sz_t my_max_entities{100000};

enum class e_type : int
{
    soul = 0,
    fireball = 1,
    toriel = 2
};

struct my_game_entity
{
    e_type type;
    sdl::vec2f _pos, _origin, _size;
    float _radians{0.f}, _opacity{1.f};
    float _hitbox_radius;
    bool alive{false};

    sdl::vec2f vel;
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
    using my_intset = sdl::fixed_sparse_int_set<std::size_t, my_max_entities>;
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
        VRM_CORE_ASSERT(!_free.empty());

        auto fi(_free.back());
        _free.pop_back();

        auto& res(_entities[fi]);
        res = e;

        VRM_CORE_ASSERT(!res.alive);
        res.alive = true;



        VRM_CORE_ASSERT(!_alive.has(fi));
        _alive.add(fi);
        VRM_CORE_ASSERT(!_free.has(fi));
        VRM_CORE_ASSERT(_alive.has(fi));

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
        auto to_erase_begin(_free.end());

        _alive.for_each([this](auto i)
            {
                if(!_entities[i].alive)
                {
                    VRM_CORE_ASSERT(_alive.has(i));
                    VRM_CORE_ASSERT(!_free.has(i));

                    _free.add(i);

                    VRM_CORE_ASSERT(_free.has(i));
                    VRM_CORE_ASSERT(_alive.has(i));
                }
            });


        for(auto i(to_erase_begin); i != _free.end(); ++i)
        {
            VRM_CORE_ASSERT(_alive.has(*i));

            _alive.erase(*i);

            VRM_CORE_ASSERT(!_alive.has(*i));
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
    using engine_type = typename std::remove_reference_t<TContext>::engine_type;

    TContext _context;
    engine_type& _engine;

    sdl::batched_sprite_renderer sr;

    sdl::atlas _atlas;

    sdl::window& _window{_context.window()};
    sdl::camera_2d _camera{_window};


    auto& texture_size(e_type type) noexcept
    {
        return _atlas.size(vrmc::from_enum(type));
    }

    auto& texture_coords(e_type type) noexcept
    {
        return _atlas.coords(vrmc::from_enum(type));
    }

    auto make_surface_from_image(const std::string& path)
    {
        return _context.make_surface(path);
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
        e._origin = sdl::vec2f{0, 0};

        e._size = texture_size(e_type::soul);
        e._hitbox_radius = 3.f;

        return e;
    }

    auto soul_update(entity_type& x, game_state_type& state, sdl::ft step)
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
    }

    auto make_fireball(sdl::vec2f pos, sdl::vec2f vel, float speed)
    {
        entity_type e;
        e.type = e_type::fireball;
        e._pos = pos;
        e._radians = rndf(0.f, sdl::tau);
        e._origin = sdl::vec2f{0, 0};

        e._size = texture_size(e_type::fireball) * rndf(0.5f, 3.2f);
        e._hitbox_radius = 3.f;
        e._opacity = 0.f;
        e.vel = vel;
        e.speed = speed;
        e.hue = rndf(-0.f, 360.f);
        e.curve = rndf(-1.f, 1.f);
        e.dir = rand() % 2;
        e.life = 100.f;

        return e;
    }

    auto fireball_update(entity_type& x, game_state_type&, sdl::ft step)
    {
        x._pos += x.vel * (x.speed * step);
        // x._radians += dir ? 0.2 * step : -0.2 * step;
        x._radians = sdl::wrap_rad(x._radians + 0.01f * step);

        // TODO: uses std::sin and std::cos...
        // x.vel = glm::rotate(x.vel, x.curve * 0.1f * step);

        if(std::abs(x.curve) > 0.01)
        {
            x.curve *= 0.5f;
        }

        x.life -= step * 1.f;

        if(x.life <= 0.f) x.alive = false;
        if(x.life <= 10.f) x._opacity -= step * 0.2f;

        if(x._opacity <= 1.f) x._opacity += step * 0.1f;
    }


    auto sprite_draw(const entity_type& x)
    {
        sr.draw_sprite(*_atlas.texture(), texture_coords(x.type), x._pos,
            x._origin, x._size, x._radians,
            sdl::vec4f{1.f, 0.f, 1.f, x._opacity}, x.hue);
    }

    auto make_toriel(game_state_type& state, sdl::vec2f pos)
    {
        entity_type e;
        e.type = e_type::toriel;
        e._pos = pos;
        e._origin = sdl::vec2f{0, 0};
        e._size = texture_size(e_type::toriel);
        e._hitbox_radius = 30.f;
        e.curve = 0.f;

        return e;
    }

    auto toriel_update(entity_type& x, game_state_type& state, sdl::ft step)
    {
        x.curve -= step;
        if(x.curve <= 0.f)
        {
            x.curve = 10.f;

            for(int i = 0; i < 10000; ++i)
            {
                if(state._free.empty()) break;

                auto angle(rndf(0.f, sdl::tau));
                auto speed(rndf(0.1f, 250.f));
                auto unit_vec(sdl::vec2f(std::cos(angle), std::sin(angle)));
                // sdl::vec2f(sdl::tbl_cos(angle), sdl::tbl_sin(angle)));
                auto vel(unit_vec * speed);

                state.add(
                    this->make_fireball(x._pos + unit_vec * rndf(55.f, 90.f),
                        vel, 1.f + (rand() % 100) / 80.f));
            }
        }
    }

    void update(my_game_state& state, sdl::ft step)
    {
        // const auto& entities(state._entities);
        auto& soul(state.soul());

        state.for_alive([this, &state, step](auto& e)
            {
                if(e.type == e_type::soul)
                {
                    this->soul_update(e, state, step);
                }
                else if(e.type == e_type::fireball)
                {
                    this->fireball_update(e, state, step);
                }
                else if(e.type == e_type::toriel)
                {
                    this->toriel_update(e, state, step);
                }
            });

        state.reclaim();

        if(_context.key(sdl::kkey::w)) _camera.move({0.f, -3.f * step});
        if(_context.key(sdl::kkey::s)) _camera.move({0.f, 3.f * step});
        if(_context.key(sdl::kkey::a)) _camera.move({-3.f * step, 0.f});
        if(_context.key(sdl::kkey::d)) _camera.move({3.f * step, 0.f});

        if(_context.key(sdl::kkey::q)) _camera.rotate(-0.05f * step);
        if(_context.key(sdl::kkey::e)) _camera.rotate(0.05f * step);

        if(_context.key(sdl::kkey::z)) _camera.zoom(0.9f * step);
        if(_context.key(sdl::kkey::x)) _camera.zoom(1.1f * step);

        /*
        _camera.move_towards_point(soul._pos,
            (4.f * (glm::length(soul._pos - _camera.position()) * 0.01f) *
                                       step));
                                       */

        // if(_context.key(sdl::kkey::q)) _context.fps_limit += step;
        // if(_context.key(sdl::kkey::e)) _context.fps_limit -= step;

        if(_context.key(sdl::kkey::escape))
        {
            sdl::stop_global_context();
        }

        soul._pos = _camera.window_to_world(_context.mouse_pos());

        if(rand() % 100 < 30)
        {
            auto alive_str(std::to_string(state._alive.size()));
            auto fps_str(std::to_string(_context.fps()));
            auto fps_limit_str(
                std::to_string(static_cast<int>(_context.fps_limit)));
            auto update_ms_str(std::to_string(_context.update_ms()));
            auto draw_ms_str(std::to_string(_context.draw_ms()));

            _context.title(alive_str + " |\tFPS: " + fps_str + "/" +
                           fps_limit_str + "\tU: " + update_ms_str + "\tD: " +
                           draw_ms_str);
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
    }

    void draw(const my_game_state& state)
    {
        _window.use_and_clear_background({0.5f, 0.5f, 0.5f, 1.0f});
        _window.use_and_clear_foreground({0.f, 0.f, 0.f, 1.0f});

        sr.use(_camera);

        // this->texture(e_type::fireball)->activate_and_bind(GL_TEXTURE0);
        _atlas.texture()->activate_and_bind(GL_TEXTURE0);

        state.for_alive([this](const auto& e)
            {
                // TODO: slow
                // this->texture(e.type)->activate_and_bind(GL_TEXTURE0);
                // ----------

                this->sprite_draw(e);
            });


        sr.do_it();

        //  std::cout << "\n\ndraw end\n";
    }

    void interpolate(my_game_state& interpolated, const my_game_state& s0,
        const my_game_state& s1, sdl::ft t)
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
    }

    my_game(TContext&& context)
        : _context(FWD(context)), _engine(_context._engine)
    {
        // texture(e_type::soul) = make_texture_from_image("files/soul.png");

        // texture(e_type::fireball) =
        // make_texture_from_image("files/fireball.png");
        // texture(e_type::toriel) =
        // make_texture_from_image("files/toriel.png");

        _atlas = sdl::make_atlas_from_paths( // .
            "files/soul.png",                // 0
            "files/fireball.png",            // 1
            "files/toriel.png"               // 2
            );

        {
            auto& state(_engine.current_state());
            // auto& entities(state._entities);

            state.add(make_toriel(state, sdl::make_vec(500.f, 100.f)));
            state._soul_idx = state.add(make_soul(sdl::make_vec(500.f, 500.f)));
        }

        _engine.update_fn() = [this](auto& state, auto step)
        {
            this->update(state, step);
        };

        _engine.draw_fn() = [this](const auto& state)
        {
            this->draw(state);
        };

        _engine.interpolate_fn() = [this](
            auto& interpolated, const auto& s0, const auto& s1, float t)
        {
            this->interpolate(interpolated, s0, s1, t);
        };
    }
};

using my_timer = sdl::impl::static_timer;

using my_engine_settings =
    sdl::interpolated_engine_settings<my_timer, my_game_state>;

using my_engine = sdl::impl::non_interpolated_engine<my_engine_settings>;

using my_context_settings = sdl::context_settings<my_engine>;

int main()
{
// std::cout << sdl::impl::n_components_for<glm::tvec4<float,
// glm::precision::highp>> << "\n";
// std::cout << sdl::impl::attrib_type_for<vec4f> << "\n";
//    std::cout << GL_FLOAT << "\n";

// feenableexcept(FE_DIVBYZERO | FE_INVALID | FE_OVERFLOW);

#define COUT_SIZE(...)                                                      \
    std::cout << "sizeof(" << #__VA_ARGS__ << ") = " << sizeof(__VA_ARGS__) \
              << "\n"

    // COUT_SIZE(my_game_entity);
    // COUT_SIZE(std::array<my_game_entity, my_max_entities>);
    // COUT_SIZE(sparse_int_set<std::size_t, my_max_entities>);

    // TODO:

    my_timer timer;
    my_engine engine;
    sdl::window window("hello world!", sdl::vec2u(1440, 900));

    auto c(
        sdl::make_global_context<my_context_settings>(timer, engine, window));

    c->window().mode(sdl::window_mode::windowed);

    auto game(std::make_unique<my_game<decltype(*c)>>(*c));
    sdl::run_global_context();
}
