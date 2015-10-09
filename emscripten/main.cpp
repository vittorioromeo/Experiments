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

#define FWD(...) ::std::forward<decltype(__VA_ARGS__)>(__VA_ARGS__)

namespace sdl
{
    constexpr float pi{3.14159265f};
    constexpr float pi_half{pi / 2.f};
    constexpr float tau{pi * 2.f};
    constexpr float rad_deg_ratio{pi / 180.f};

    template <typename T>
    constexpr auto to_rad(const T& x) noexcept
    {
        return x * rad_deg_ratio;
    }

    template <typename T>
    constexpr auto to_deg(const T& x) noexcept
    {
        return x / rad_deg_ratio;
    }

    template <typename T>
    class vec2
    {
    private:
        T _x, _y;

    public:
        vec2() noexcept : _x{T(0)}, _y{T(0)} {};
        vec2(T x, T y) noexcept : _x{x}, _y{y} {}

        template <typename TS>
        vec2(const vec2<TS>& v) noexcept : _x(v.x()), _y(v.y())
        {
        }

        template <typename TS>
        vec2& operator=(const vec2<TS>& v) noexcept
        {
            _x = v.x();
            _y = v.y();
            return *this;
        }

        vec2(const vec2&) = default;
        vec2& operator=(const vec2&) = default;

        vec2(vec2&&) = default;
        vec2& operator=(vec2&&) = default;

        auto& x() noexcept { return _x; }
        const auto& x() const noexcept { return _x; }

        auto& y() noexcept { return _y; }
        const auto& y() const noexcept { return _y; }

        template <typename TV>
        vec2& operator+=(const TV& v) noexcept
        {
            _x += v._x;
            _y += v._y;
            return *this;
        }

        template <typename TV>
        vec2& operator-=(const TV& v) noexcept
        {
            _x -= v._x;
            _y -= v._y;
            return *this;
        }

        template <typename TS>
        vec2& operator*(TS s) noexcept
        {
            _x *= s;
            _y *= s;
            return *this;
        }

        template <typename TS>
        vec2& operator/(TS s) noexcept
        {
            _x /= s;
            _y /= s;
            return *this;
        }
    };



    using vec2i = vec2<int>;
    using vec2u = vec2<unsigned int>;
    using vec2f = vec2<float>;

    template <typename T0, typename T1>
    auto make_vec2(T0 x, T1 y)
    {
        return vec2<std::common_type_t<T0, T1>>(x, y);
    }

    template <typename TV0, typename TV1>
    auto operator+(const TV0& v0, const TV1& v1)
    {
        return make_vec2(v0.x() + v1.x(), v0.y() + v1.y());
    }

    template <typename TV0, typename TV1>
    auto operator-(const TV0& v0, const TV1& v1)
    {
        return make_vec2(v0.x() - v1.x(), v0.y() - v1.y());
    }

    template <typename TV, typename TS>
    auto operator*(const TV& v, TS s)
    {
        return make_vec2(v.x() * s, v.y() * s);
    }

    template <typename TV, typename TS>
    auto operator/(const TV& v, TS s)
    {
        return make_vec2(v.x() / s, v.y() / s);
    }

    template <typename T>
    auto& operator<<(std::ostream& o, const vec2<T>& v)
    {
        o << "(" << v.x() << ", " << v.y() << ")";
        return o;
    }

    using key_code = SDL_Keycode;
    using mouse_coord = Sint32;
    using mouse_btn = Uint8;
    using sz_t = std::size_t;

    using key_event_handler = std::function<void(key_code)>;
    using btn_event_handler = std::function<void(mouse_btn)>;

    using ft = float;

    using update_fn = std::function<void(ft)>;
    using draw_fn = std::function<void()>;

    namespace impl
    {
        void log_sdl_error()
        {
            std::cout << "error: " << SDL_GetError() << std::endl;
        }

        auto& null_key_event_handler()
        {
            static key_event_handler result([](auto)
                {
                });
            return result;
        }

        auto& null_btn_event_handler()
        {
            static btn_event_handler result([](auto)
                {
                });
            return result;
        }

        auto& null_update_fn()
        {
            static update_fn result([](auto)
                {
                });
            return result;
        }

        auto& null_draw_fn()
        {
            static draw_fn result([]
                {
                });
            return result;
        }

        template <typename T, typename TDeleter>
        class unique_resource
        {
        private:
            T* _ptr{nullptr};
            TDeleter deleter;

            void delete_if_necessary() noexcept
            {
                if(_ptr != nullptr) deleter(_ptr);
            }

        public:
            unique_resource() = default;
            unique_resource(T* p) noexcept { reinit(p); }

            unique_resource(const unique_resource&) = delete;
            unique_resource& operator=(const unique_resource&) = delete;

            unique_resource(unique_resource&& s) noexcept : _ptr{s._ptr}
            {
                s._ptr = nullptr;
            }
            unique_resource& operator=(unique_resource&& s) noexcept
            {
                _ptr = s._ptr;
                s._ptr = nullptr;
                return *this;
            }

            ~unique_resource() noexcept { delete_if_necessary(); }

            void reinit(T* p) noexcept
            {
                delete_if_necessary();
                _ptr = p;

                if(_ptr == nullptr)
                {
                    impl::log_sdl_error();
                    std::terminate();
                }
            }

            auto ptr() noexcept
            {
                assert(_ptr != nullptr);
                return _ptr;
            }
            const auto ptr() const noexcept
            {
                assert(_ptr != nullptr);
                return _ptr;
            }

            auto& get() noexcept { return *ptr(); }
            const auto& get() const noexcept { return *ptr(); }

            operator T*() noexcept { return ptr(); }
        };

        struct unique_window_deleter
        {
            void operator()(SDL_Window* p) noexcept { SDL_DestroyWindow(p); }
        };

        struct unique_texture_deleter
        {
            void operator()(SDL_Texture* p) noexcept { SDL_DestroyTexture(p); }
        };

        struct unique_renderer_deleter
        {
            void operator()(SDL_Renderer* p) noexcept
            {
                SDL_DestroyRenderer(p);
            }
        };

        struct unique_surface_deleter
        {
            void operator()(SDL_Surface* p) noexcept { SDL_FreeSurface(p); }
        };

        struct unique_glcontext_deleter
        {
            void operator()(SDL_GLContext* p) noexcept
            {
                SDL_GL_DeleteContext(p);
            }
        };

        struct unique_ttffont_deleter
        {
            void operator()(TTF_Font* p) noexcept { TTF_CloseFont(p); }
        };

        using unique_window =
            unique_resource<SDL_Window, unique_window_deleter>;

        using unique_texture =
            unique_resource<SDL_Texture, unique_texture_deleter>;

        using unique_renderer =
            unique_resource<SDL_Renderer, unique_renderer_deleter>;

        using unique_surface =
            unique_resource<SDL_Surface, unique_surface_deleter>;

        using unique_glcontext =
            unique_resource<SDL_GLContext, unique_glcontext_deleter>;

        using unique_ttffont =
            unique_resource<TTF_Font, unique_ttffont_deleter>;
    }

    class context;
    class window;
    class renderer;
    class texture;
    class surface;
    class ttffont;
    class image;
    class sprite;

    class input_state
    {
        friend class context;

    private:
        std::unordered_map<key_code, bool> _keys;
        std::unordered_map<mouse_btn, bool> _btns;
        mouse_coord _mouse_x{0};
        mouse_coord _mouse_y{0};

        auto& key(key_code c) { return _keys[c]; }
        auto& btn(mouse_btn b) { return _btns[b]; }

        void mouse_x(mouse_coord c) noexcept { _mouse_x = c; }
        void mouse_y(mouse_coord c) noexcept { _mouse_y = c; }

    public:
        auto key(key_code c) const { return _keys.at(c); }
        auto btn(mouse_btn b) const { return _btns.at(b); }

        auto mouse_x() const noexcept { return _mouse_x; }
        auto mouse_y() const noexcept { return _mouse_y; }
        auto mouse_pos() const noexcept
        {
            return make_vec2(mouse_x(), mouse_y());
        }
    };


    class surface : public impl::unique_surface
    {
    private:
        using base_type = impl::unique_surface;

    public:
        using base_type::base_type;

        auto width() const noexcept { return get().w; }
        auto height() const noexcept { return get().h; }
        auto size() const noexcept { return make_vec2(width(), height()); }

        auto format() const noexcept { return get().format; }

        void clear(int r, int g, int b)
        {
            SDL_Rect clear_rect;
            clear_rect.x = 0;
            clear_rect.y = 0;
            clear_rect.w = width();
            clear_rect.h = height();

            SDL_FillRect(ptr(), &clear_rect, SDL_MapRGB(format(), r, g, b));
        }

        void display()
        {
            // SDL_Flip(ptr());
        }

        void blit(const surface& s, const SDL_Rect& src_rect,
            const SDL_Rect& dest_rect)
        {
            SDL_BlitSurface(
                s.ptr(), &src_rect, ptr(), &const_cast<SDL_Rect&>(dest_rect));
        }

        void blit(const image& i, int x, int y);
    };

    class image
    {
    private:
        surface _surface;
        SDL_Rect _rect;

    public:
        auto width() const noexcept { return _surface.width(); }
        auto height() const noexcept { return _surface.height(); }
        auto size() const noexcept { return _surface.size(); }

        image(const std::string& path) : _surface{IMG_Load(path.c_str())}
        {
            _rect.x = 0;
            _rect.y = 0;
            _rect.w = width();
            _rect.h = height();
        }

        auto& surface() noexcept { return _surface; }
        const auto& surface() const noexcept { return _surface; }

        const auto& rect() const noexcept { return _rect; }
    };


    class window : public impl::unique_window
    {
    private:
        using base_type = impl::unique_window;

    public:
        window(sz_t width, sz_t height) noexcept
            : base_type{SDL_CreateWindow(
                  "emscripten window", 0, 0, width, height, SDL_WINDOW_OPENGL)}
        {
        }
    };

    class glcontext : public impl::unique_glcontext
    {
    private:
        using base_type = impl::unique_glcontext;
        SDL_GLContext _glcontext;

    public:
        glcontext(window& w) noexcept : _glcontext{SDL_GL_CreateContext(w)},
                                        base_type{&_glcontext}
        {
            SDL_GL_SetSwapInterval(0);
        }
    };

    class renderer : public impl::unique_renderer
    {
    private:
        using base_type = impl::unique_renderer;

    public:
        renderer(window& w) noexcept
            : base_type{SDL_CreateRenderer(
                  w, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE |
                             SDL_RENDERER_PRESENTVSYNC)}
        {
        }

        void draw_color(int r, int g, int b, int a) noexcept
        {
            SDL_SetRenderDrawColor(*this, r, g, b, a);
        }

        void clear() noexcept { SDL_RenderClear(*this); }
        void clear(int r, int g, int b, int a) noexcept
        {
            draw_color(r, g, b, a);
            clear();
        }

        void blend_mode(SDL_BlendMode m) noexcept
        {
            SDL_SetRenderDrawBlendMode(*this, m);
        }

        void present() noexcept { SDL_RenderPresent(*this); }

        void target(std::nullptr_t) noexcept
        {
            SDL_SetRenderTarget(*this, nullptr);
        }
        void target(texture& t) noexcept;

        void clear_texture(texture& t, int r, int g, int b, int a) noexcept;
        void draw(texture& t) noexcept;
        void draw(texture& t, const vec2f& pos) noexcept;

        void draw(sprite& s) noexcept;

        void load_texture(texture& t, const std::string& path);
    };

    class texture : public impl::unique_texture
    {
    private:
        using base_type = impl::unique_texture;
        vec2f _size;

    public:
        texture() = default;

        texture(renderer& r, sz_t width, sz_t height) noexcept
            : base_type{SDL_CreateTexture(r, SDL_PIXELFORMAT_ARGB8888,
                  SDL_TEXTUREACCESS_TARGET, width, height)},
              _size(width, height)
        {
        }

        texture(renderer& r, surface& s) noexcept
            : base_type{SDL_CreateTextureFromSurface(r, s)},
              _size(s.width(), s.height())
        {
        }

        texture(renderer& r, image& i) noexcept
            : base_type{SDL_CreateTextureFromSurface(r, i.surface())},
              _size(i.width(), i.height())
        {
        }

        const auto& size() const noexcept { return _size; }
    };

    class ttffont : public impl::unique_ttffont
    {
    private:
        using base_type = impl::unique_ttffont;

    public:
        ttffont(const std::string& path, sz_t font_size) noexcept
            : base_type{TTF_OpenFont(path.c_str(), font_size)}
        {
        }
    };

    void renderer::load_texture(texture& t, const std::string& path)
    {
        image temp{path};
        t = texture{*this, temp};
    }

    void renderer::clear_texture(
        texture& t, int r, int g, int b, int a) noexcept
    {
        target(t);
        blend_mode(SDL_BLENDMODE_NONE);
        draw_color(r, g, b, a);
        SDL_RenderFillRect(*this, nullptr);
    }

    void renderer::draw(texture& t) noexcept
    {
        SDL_RenderCopy(*this, t, nullptr, nullptr);
    }

    void renderer::draw(texture& t, const vec2f& pos) noexcept
    {
        SDL_Rect dst;
        dst.x = pos.x();
        dst.y = pos.y();
        dst.w = t.size().x();
        dst.h = t.size().y();

        SDL_RenderCopy(*this, t, nullptr, &dst);
    }



    void renderer::target(texture& t) noexcept
    {
        SDL_SetRenderTarget(*this, t);
    }

    class space_data
    {
    private:
        vec2f _pos, _scale{1.f, 1.f}, _origin;
        float _radians{0.f};

    public:
        space_data() = default;

        space_data(const space_data&) = default;
        space_data& operator=(const space_data&) = default;

        space_data(space_data&&) = default;
        space_data& operator=(space_data&&) = default;

        auto& pos() noexcept { return _pos; }
        const auto& pos() const noexcept { return _pos; }

        auto& scale() noexcept { return _scale; }
        const auto& scale() const noexcept { return _scale; }

        auto& origin() noexcept { return _origin; }
        const auto& origin() const noexcept { return _origin; }

        auto& radians() noexcept { return _radians; }
        const auto& radians() const noexcept { return _radians; }
    };

    void surface::blit(const image& i, int x, int y)
    {
        blit(i.surface(), i.rect(), SDL_Rect{x, y, i.width(), i.height()});
    }

    class sprite
    {
    private:
        texture* _texture{nullptr};
        space_data _sd;

    public:
        sprite() = default;
        sprite(texture& t) noexcept : _texture{&t} {}

        sprite(const sprite& s) = default;
        sprite& operator=(const sprite& s) = default;

        sprite(sprite&& s) = default;
        sprite& operator=(sprite&& s) = default;

        auto valid_texture() const noexcept { return _texture != nullptr; }

        auto& texture() noexcept { return *_texture; }
        const auto& texture() const noexcept { return *_texture; }

        auto& pos() noexcept { return _sd.pos(); }
        const auto& pos() const noexcept { return _sd.pos(); }

        auto& scale() noexcept { return _sd.scale(); }
        const auto& scale() const noexcept { return _sd.scale(); }

        auto& origin() noexcept { return _sd.origin(); }
        const auto& origin() const noexcept { return _sd.origin(); }

        auto& radians() noexcept { return _sd.radians(); }
        const auto& radians() const noexcept { return _sd.radians(); }

        void set_origin_to_center() noexcept
        {
            origin() = texture().size() / 2.f;
        }
    };

    void renderer::draw(sprite& s) noexcept
    {
        assert(s.valid_texture());

        SDL_Rect dst;
        dst.x = s.pos().x() - s.origin().x();
        dst.y = s.pos().y() - s.origin().y();
        dst.w = s.texture().size().x();
        dst.h = s.texture().size().y();

        SDL_Point center{(int)s.origin().x(), (int)s.origin().y()};

        SDL_RenderCopyEx(*this, s.texture(), nullptr, &dst, to_deg(s.radians()),
            &center, SDL_FLIP_NONE);


        // SDL_RenderCopy(*this, t, nullptr, &dst);

        // draw(s.texture(), s.pos() - s.origin());
    }



    class context
    {
    private:
        const std::size_t _width;
        const std::size_t _height;

        window _window;
        glcontext _glcontext;
        renderer _renderer;
        texture _texture;

        SDL_Event _event;

        key_event_handler _on_key_down{impl::null_key_event_handler()};
        key_event_handler _on_key_up{impl::null_key_event_handler()};

        btn_event_handler _on_btn_down{impl::null_btn_event_handler()};
        btn_event_handler _on_btn_up{impl::null_btn_event_handler()};

        update_fn _update_fn{impl::null_update_fn()};
        draw_fn _draw_fn{impl::null_draw_fn()};

        input_state _input_state;

        auto& on_key_up() noexcept { return _on_key_up; }
        auto& on_key_down() noexcept { return _on_key_down; }

        auto& on_btn_up() noexcept { return _on_btn_up; }
        auto& on_btn_down() noexcept { return _on_btn_down; }

    public:
        auto& update_fn() noexcept { return _update_fn; }
        auto& draw_fn() noexcept { return _draw_fn; }

    private:
        void run_events()
        {
            while(SDL_PollEvent(&_event))
            {
                switch(_event.type)
                {
                    case SDL_KEYDOWN:
                        on_key_down()(_event.key.keysym.sym);
                        break;

                    case SDL_KEYUP: on_key_up()(_event.key.keysym.sym); break;

                    case SDL_MOUSEBUTTONDOWN:
                        on_btn_down()(_event.button.button);
                        break;

                    case SDL_MOUSEBUTTONUP:
                        on_btn_up()(_event.button.button);
                        break;

                    case SDL_MOUSEMOTION:
                        _input_state.mouse_x(_event.motion.x);
                        _input_state.mouse_y(_event.motion.y);
                        break;
                }
            }
        }

        void run_update() { update_fn()(1.f); }
        void run_draw()
        {
            _renderer.clear();
            _renderer.clear_texture(_texture, 0, 0, 0, 255);

            _renderer.target(nullptr);
            _renderer.draw(_texture);

            draw_fn()();

            _renderer.present();
        }

    public:
        context(std::size_t width, std::size_t height)
            : _width{width}, _height{height}, _window{width, height},
              _glcontext{_window}, _renderer{_window},
              _texture{_renderer, width, height}
        {
            if(TTF_Init() != 0)
            {
                std::terminate();
            }

            // SDL_SetRenderDrawBlendMode(_renderer, SDL_BLENDMODE_BLEND);

            //            _screen = surface{SDL_SetVideoMode(_width, _height,
            //            32, SDL_SWSURFACE)};


            on_key_down() = [this](auto k)
            {
                _input_state.key(k) = true;
            };

            on_key_up() = [this](auto k)
            {
                _input_state.key(k) = false;
            };

            on_btn_down() = [this](auto b)
            {
                _input_state.btn(b) = true;
            };

            on_btn_up() = [this](auto b)
            {
                _input_state.btn(b) = false;
            };
        }

        context(const context&) = delete;
        context& operator=(const context&) = delete;

        context(context&&) = default;
        context& operator=(context&&) = default;


        void run()
        {
            run_events();
            run_update();
            run_draw();
        }

        // auto& screen() noexcept { return _screen; }
        // const auto& screen() const noexcept { return _screen; }

        auto mouse_x() const noexcept { return _input_state.mouse_x(); }
        auto mouse_y() const noexcept { return _input_state.mouse_y(); }
        auto mouse_pos() const noexcept { return _input_state.mouse_pos(); }

        auto key(key_code c) const { return _input_state.key(c); }
        auto btn(mouse_btn b) const { return _input_state.btn(b); }

        auto make_texture() noexcept { return texture{}; }
        auto make_texture(const std::string& path) noexcept
        {
            texture result;
            _renderer.load_texture(result, path);
            return result;
        }

        auto make_sprite() noexcept { return sprite{}; }
        auto make_sprite(texture& t) noexcept { return sprite{t}; }

        auto make_ttffont(const std::string& path, sz_t font_size)
        {
            return ttffont{path, font_size};
        }

        auto make_ttftext_texture(
            ttffont& f, const std::string& s, SDL_Color color)
        {
            surface temp{TTF_RenderText_Blended(f, s.c_str(), color)};
            return texture{_renderer, temp};
        }

        void draw(texture& t, const vec2f& pos) { _renderer.draw(t, pos); }
        void draw(sprite& s) { _renderer.draw(s); }
    };

    namespace impl
    {
        std::unique_ptr<context> global_context;
        void run_global_context_loop() { global_context->run(); }
    }

    template <typename... Ts>
    auto& make_global_context(Ts&&... xs)
    {
        assert(impl::global_context == nullptr);
        impl::global_context = std::make_unique<context>(FWD(xs)...);
        return *impl::global_context;
    }

    void run_global_context()
    {
        emscripten_set_main_loop(impl::run_global_context_loop, 0, true);
    }
}



int main(int argc, char** argv)
{
    std::cout << "Hello world!\n";
    auto& c(sdl::make_global_context(1000, 600));

    auto toriel_texture(c.make_texture("files/toriel.png"));
    auto toriel_sprite(c.make_sprite(toriel_texture));

    auto test_text_font(c.make_ttffont("files/pixel.ttf", 16));
    auto test_text_texture(c.make_ttftext_texture(
        test_text_font, "hello!", SDL_Color{255, 255, 255, 255}));
    auto test_text_sprite(c.make_sprite(test_text_texture));

    toriel_sprite.set_origin_to_center();
    test_text_sprite.set_origin_to_center();

    std::cout << "origin: " << toriel_sprite.origin() << "\n";

    // sdl::image toriel{"files/toriel.png"};

    c.update_fn() = [&](auto)
    {
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