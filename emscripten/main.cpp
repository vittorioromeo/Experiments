#include <stdio.h>
#include <iostream>
#include <SDL/SDL.h>
#include <SDL_image.h>
#include <emscripten.h>
#include <unordered_map>
#include <string>
#include <cassert>

namespace sdl
{
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
    };

    class image;

    class surface
    {
    private:
        SDL_Surface* _surface{nullptr};

    public:
        surface() = default;
        surface(SDL_Surface* p) : _surface{p}
        {
            if(_surface == nullptr)
            {
                impl::log_sdl_error();
                std::terminate();
            }
        }

        surface(const surface&) = delete;
        surface& operator=(const surface&) = delete;

        surface(surface&& s) : _surface{s._surface} { s._surface = nullptr; }
        surface& operator=(surface&& s)
        {
            _surface = s._surface;
            s._surface = nullptr;
            return *this;
        }

        ~surface()
        {
            if(_surface != nullptr) SDL_FreeSurface(_surface);
        }

        auto ptr() noexcept
        {
            assert(_surface != nullptr);
            return _surface;
        }
        const auto ptr() const noexcept
        {
            assert(_surface != nullptr);
            return _surface;
        }

        auto& get() noexcept { return *ptr(); }
        const auto& get() const noexcept { return *ptr(); }

        auto width() const noexcept { return get().w; }
        auto height() const noexcept { return get().h; }

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

        void display() { SDL_Flip(ptr()); }

        void blit(const surface& s, const SDL_Rect& src_rect,
            const SDL_Rect& dest_rect)
        {
            SDL_BlitSurface(
                s.ptr(), &src_rect, ptr(), &const_cast<SDL_Rect&>(dest_rect));
        }

        void blit(const image& i, const SDL_Rect& dest_rect);
    };

    class image
    {
    private:
        surface _surface;
        SDL_Rect _rect;

    public:
        auto width() const noexcept { return _surface.width(); }
        auto height() const noexcept { return _surface.height(); }

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

    void surface::blit(const image& i, const SDL_Rect& dest_rect)
    {
        blit(i.surface(), i.rect(), dest_rect);
    }

    class context
    {
    private:
        const std::size_t _width;
        const std::size_t _height;

        surface _screen;
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
            _screen.clear(0, 0, 0);

            draw_fn()();

            _screen.display();
        }

    public:
        context(std::size_t width, std::size_t height)
            : _width{width}, _height{height}
        {
            if(SDL_Init(SDL_INIT_VIDEO) != 0)
            {
                impl::log_sdl_error();
                std::terminate();
            }

            constexpr int img_flags{IMG_INIT_PNG};
            if(!(IMG_Init(img_flags) & img_flags))
            {
                impl::log_img_error();
                std::terminate();
            }

            _screen =
                surface{SDL_SetVideoMode(_width, _height, 32, SDL_SWSURFACE)};

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

        ~context() { SDL_Quit(); }

        void run()
        {
            run_events();
            run_update();
            run_draw();
        }

        auto& screen() noexcept { return _screen; }
        const auto& screen() const noexcept { return _screen; }

        auto mouse_x() const noexcept { return _input_state.mouse_x(); }
        auto mouse_y() const noexcept { return _input_state.mouse_y(); }

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
    sdl::context c{640, 480};
    sdl::init_global_context(c);

    sdl::image toriel{"files/toriel.png"};

    c.draw_fn() = [&c, &toriel]
    {
        SDL_Rect dest;

        dest.x = c.mouse_x();
        dest.y = c.mouse_y();
        dest.w = toriel.width();
        dest.h = toriel.height();

        // c.screen().blit(toriel.surface(), src, dest);
        c.screen().blit(toriel, dest);
    };



    emscripten_set_main_loop(sdl::run_global_context, 0, true);

    return 0;
}