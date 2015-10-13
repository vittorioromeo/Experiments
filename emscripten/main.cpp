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
    };
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
    //std::vector<T> _dense;
    std::array<T, TSize> _dense;
    std::array<std::size_t, TSize> _sparse;
   // std::vector<std::size_t> _sparse;
    std::size_t _size;

public:
    sparse_int_set() noexcept
    {
       // _dense.resize(TSize);
     //   _sparse.resize(TSize);
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

constexpr sdl::sz_t my_max_entities{50000};

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

    my_game_entity() = default;

    my_game_entity(const my_game_entity& rhs) = default;
    my_game_entity& operator=(const my_game_entity& rhs) = default;

    my_game_entity(my_game_entity&& rhs) = default;
    my_game_entity& operator=(my_game_entity&& rhs) = default;
};


struct my_game_state
{
    using my_intset = sparse_int_set<std::size_t, my_max_entities>;
    using entity_type = my_game_entity;

    std::vector<entity_type> _entities;
    my_intset _free, _alive;
    sdl::sz_t _soul_idx;

    my_game_state()
    {
        _entities.resize(my_max_entities);
        for(sdl::sz_t i(0); i < my_max_entities; ++i) _free.add(i);
    }


    my_game_state(const my_game_state&) = default;
    my_game_state& operator=(const my_game_state&) = default;

    my_game_state(my_game_state&&) = default;
    my_game_state& operator=(my_game_state&&) = default;

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
    sdl::sprite_renderer sr;

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
        e.hue = rndf(-0.6f, 0.1f);
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
            glm::vec4{1.f, 1.f, 1.f, x._opacity}, x.hue);
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
        return [ this, timer = 1.f ](auto& x, auto& state, auto step) mutable
        {
            timer -= step;
            if(timer <= 0.f)
            {
                timer = 1.f;

                for(int i = 0; i < 100; ++i)
                {
                    if(!state._free.empty())
                    {
                        auto angle(rndf(0.f, sdl::tau));
                        auto speed(rndf(0.1f, 3.f));
                        auto unit_vec(
                            glm::vec2(std::cos(angle), std::sin(angle)));
                        auto vel(unit_vec * speed);

                        state.add(this->make_fireball(
                            x._pos + unit_vec * rndf(55.f, 90.f), vel,
                            1.f + (rand() % 100) / 80.f));
                    }
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
            // std::cout << state._soul_idx << "\n";


            // auto& soul(entities.back());
        }

        _context.update_fn() = [&, this](const auto& old_state, auto step)
        {
            auto state(old_state);
            const auto& entities(state._entities);
            auto& soul(state.soul());

            state.for_alive([this, &state, step](auto& e)
                {
                    // std::cout << "update\n";
                    // e._update_fn(this, e, step);

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

                    // std::cout << "check\n";
                    /*
                    if(&e != &soul)
                     {
                         if(glm::distance(soul._pos, e._pos) <
                             soul._hitbox_radius + e._hitbox_radius)
                         {
                             soul.alive = false;
                         }
                     }
                     */

                    // std::cout << "after check\n";
                });

            // std::cout << "reclaim\n";
            state.reclaim();
            // std::cout << "after reclaim\n";

            /*
            entities.erase(
                std::remove_if(std::begin(entities), std::end(entities),
                    [](auto& e)
                    {
                        return !e.alive;
                    }),
                std::end(entities));
            */

            if(_context.key(sdl::kkey::q)) _context.fps_limit += step;
            if(_context.key(sdl::kkey::e)) _context.fps_limit -= step;

            if(_context.key(sdl::kkey::escape))
            {
                sdl::stop_global_context();
            }

            if(rand() % 100 < 30)
            {
                _context.title(std::to_string(entities.size()) + " ||| " +
                               std::to_string(_context.fps_limit) + " | " +
                               std::to_string(_context._static_timer._loops) +
                               " ||| " + std::to_string(_context.fps()) +
                               " | " + std::to_string(_context.real_ms()) +
                               " | " + std::to_string(_context.update_ms()));
            }

            return state;

            // std::cout << c.real_ms() << "\n";
        };

        _context.draw_fn() = [&, this](const auto& state)
        {
            // const auto& entities(state._entities);
            sr.use();
            state.for_alive([this](const auto& e)
                {
                    this->sprite_draw(e);
                });
        };

        _context.interpolate_fn() = [&, this](
            const auto& s0, const auto& s1, float t)
        {
            //return game_state_type{s1};

            // std::cout << t << "\n";
            auto interpolated(game_state_type{s1});

            auto lerp = [t](const auto& v0, const auto& v1)
            {
                // auto a = v1 - v0;
                // return a * t * t + v0;

                return (1.f - t) * v0 + t * v1;
            };

            const auto& s0_entities(s0._entities);
            const auto& s1_entities(s1._entities);

            // if(s0_entities.size() != s1_entities.size()) return s0;

            auto& in_entities(interpolated._entities);

            for(auto i(0u); i < in_entities.size(); ++i)
            {
                if(!in_entities[i].alive) continue;

                const auto& e0(s0_entities[i]);
                const auto& e1(s1_entities[i]);
                auto& ei(in_entities[i]);

                ei._pos = lerp(e0._pos, e1._pos);

                // std::cout << "(" << t << ") " << e0._pos.x << " -> " <<
                // ei._pos.x << " -> " << e1._pos.x << "\n";

                ei._origin = lerp(e0._origin, e1._origin);
                ei._size = lerp(e0._size, e1._size);
                // ei._radians = lerp(e0._radians, e1._radians);
                ei._radians = e0._radians;
                ei._opacity = lerp(e0._opacity, e1._opacity);
            }

            return interpolated;
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