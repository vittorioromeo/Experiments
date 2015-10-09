#include <stdio.h>
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <emscripten.h>
#include <unordered_map>
#include <string>
#include <cassert>
#include <type_traits>

namespace sdl
{
    template <typename T>
    class vec2
    {
    private:
        T _x, _y;

    public:
        vec2() noexcept : _x{T(0)}, _y{T(0)} {};
        vec2(T x, T y) noexcept : _x{x}, _y{y} {}

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
        return vec2<std::common_type_t<T0, T1>>{x, y};
    }

    using key_code = SDL_Keycode;
    using mouse_coord = Sint32;
    using mouse_btn = Uint8;

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

        void log_img_error()
        {
            std::cout << "img error: " << IMG_GetError() << std::endl;
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

        public:
            unique_resource() = default;
            unique_resource(T* p) : _ptr{p}
            {
                if(_ptr == nullptr)
                {
                    impl::log_sdl_error();
                    std::terminate();
                }
            }

            unique_resource(const unique_resource&) = delete;
            unique_resource& operator=(const unique_resource&) = delete;

            unique_resource(unique_resource&& s) : _ptr{s._ptr}
            {
                s._ptr = nullptr;
            }
            unique_resource& operator=(unique_resource&& s)
            {
                _ptr = s._ptr;
                s._ptr = nullptr;
                return *this;
            }

            ~unique_resource()
            {
                if(_ptr != nullptr) deleter(_ptr);
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
            void operator()(SDL_Renderer* p) { SDL_DestroyRenderer(p); }
        };

        struct unique_surface_deleter
        {
            void operator()(SDL_Surface* p) { SDL_FreeSurface(p); }
        };

        using unique_window =
            unique_resource<SDL_Window, unique_window_deleter>;
        using unique_texture =
            unique_resource<SDL_Texture, unique_texture_deleter>;
        using unique_renderer =
            unique_resource<SDL_Renderer, unique_renderer_deleter>;
        using unique_surface =
            unique_resource<SDL_Surface, unique_surface_deleter>;
    }

    class context;

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

    class image;
    class sprite;

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

    class texture : public impl::unique_texture
    {
    private:
        using base_type = impl::unique_texture;

    public:
        using base_type::base_type;
    };

    class renderer : public impl::unique_renderer
    {
    private:
        using base_type = impl::unique_renderer;

    public:
        using base_type::base_type;
    };


    class window : public impl::unique_window
    {
    private:
        using base_type = impl::unique_window;

    public:
        using base_type::base_type;
    };



    struct space_data
    {
        vec2f _pos, _size, _origin;
        float _radians{0.f};

        space_data() = default;

        space_data(const space_data&) = default;
        space_data& operator=(const space_data&) = default;

        space_data(space_data&&) = default;
        space_data& operator=(space_data&&) = default;
    };

    void surface::blit(const image& i, int x, int y)
    {
        blit(i.surface(), i.rect(), SDL_Rect{x, y, i.width(), i.height()});
    }

    class sprite
    {
    private:
        image* _image{nullptr};
        space_data _sd;

    public:
        sprite() = default;
        sprite(image& i) noexcept : _image{&i} {}

        sprite(const sprite& s) = default;
        sprite& operator=(const sprite& s) = default;

        sprite(sprite&& s) = default;
        sprite& operator=(sprite&& s) = default;
    };

    void fillTexture(SDL_Renderer* renderer, SDL_Texture* texture, int r, int g,
        int b, int a)
    {
        SDL_SetRenderTarget(renderer, texture);
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
        SDL_SetRenderDrawColor(renderer, r, g, b, a);
        SDL_RenderFillRect(renderer, nullptr);
    }

    class context
    {
    private:
        const std::size_t _width;
        const std::size_t _height;

        window _window;
        renderer _renderer;
        texture _texture;

        // surface _screen;
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
            // _screen.clear(0, 0, 0);
            draw_fn()();


            SDL_SetRenderTarget(_renderer, nullptr);
            SDL_RenderClear(_renderer);
            fillTexture(_renderer, _texture, 255, 0, 0, 255);
            SDL_RenderCopy(_renderer, _texture, nullptr, nullptr);
            SDL_RenderPresent(_renderer);

            //_screen.display();
        }

    public:
        context(std::size_t width, std::size_t height)
            : _width{width}, _height{height}
        {

            /*
            if(SDL_Init(SDL_INIT_VIDEO) != 0)
            {
                impl::log_sdl_error();
                std::terminate();
            }
            */

            /*
            constexpr int img_flags{IMG_INIT_PNG};
            if(!(IMG_Init(img_flags) & img_flags))
            {
                impl::log_img_error();
                std::terminate();
            }
            */


            _window = SDL_CreateWindow(
                "emscripten window", 0, 0, _width, _height, SDL_WINDOW_OPENGL);

            if(_window == nullptr)
            {
                impl::log_img_error();
                std::terminate();
            }

            _renderer = SDL_CreateRenderer(_window, -1,
                SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);

            if(_renderer == nullptr)
            {
                impl::log_img_error();
                std::terminate();
            }

            _texture = SDL_CreateTexture(_renderer, SDL_PIXELFORMAT_ARGB8888,
                SDL_TEXTUREACCESS_TARGET, _width, _height);

            if(_texture == nullptr)
            {
                impl::log_img_error();
                std::terminate();
            }

            SDL_SetRenderTarget(_renderer, nullptr);
            SDL_SetRenderDrawBlendMode(_renderer, SDL_BLENDMODE_BLEND);
            SDL_SetRenderDrawColor(_renderer, 128, 128, 128, 255);

            //            _screen = surface{SDL_SetVideoMode(_width, _height,
            //            32, SDL_SWSURFACE)};

            SDL_GL_SetSwapInterval(0);

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

        ~context()
        {
            SDL_Quit();
        }

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
    };

    context* global_context;
    void init_global_context(context& c) { global_context = &c; }
    void run_global_context() { global_context->run(); }
}



int main(int argc, char** argv)
{
    std::cout << "Hello world!\n";
    sdl::context c{1000, 480};
    sdl::init_global_context(c);

    /*sdl::image toriel{"files/toriel.png"};

    c.draw_fn() = [&c, &toriel]
    {

        c.screen().blit(toriel, c.mouse_x(), c.mouse_y());
    };*/



    emscripten_set_main_loop(sdl::run_global_context, 0, true);

    return 0;
}