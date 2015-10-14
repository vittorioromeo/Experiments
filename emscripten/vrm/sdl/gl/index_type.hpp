// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <vrm/sdl/common.hpp>

namespace vrm
{
    namespace sdl
    {
        enum class index_type
        {
            ui_byte,
            ui_short,
            ui_int
        };

        namespace impl
        {
            template <index_type TP>
            constexpr GLenum index_type_value{GL_UNSIGNED_BYTE};

            template <>
            constexpr GLenum index_type_value<index_type::ui_short>{
                GL_UNSIGNED_SHORT};

            template <>
            constexpr GLenum index_type_value<index_type::ui_int>{
                GL_UNSIGNED_INT};
        }
    }
}