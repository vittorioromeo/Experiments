// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

namespace vrm
{
    namespace sdl
    {
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
    }
}