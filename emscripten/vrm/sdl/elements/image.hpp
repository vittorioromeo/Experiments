// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

namespace vrm
{
    namespace sdl
    {
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
    }
}