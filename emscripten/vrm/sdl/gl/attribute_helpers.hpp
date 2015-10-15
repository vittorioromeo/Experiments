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
        namespace impl
        {
            template <typename T>
            struct n_components_for_helper
            {
                static_assert(std::is_arithmetic<T>{}, "");
                constexpr static sz_t value{1};
            };

            template <typename T, glm::precision P>
            struct n_components_for_helper<glm::tvec1<T, P>>
            {
                constexpr static sz_t value{1};
            };

            template <typename T, glm::precision P>
            struct n_components_for_helper<glm::tvec2<T, P>>
            {
                constexpr static sz_t value{2};
            };

            template <typename T, glm::precision P>
            struct n_components_for_helper<glm::tvec3<T, P>>
            {
                constexpr static sz_t value{3};
            };

            template <typename T, glm::precision P>
            struct n_components_for_helper<glm::tvec4<T, P>>
            {
                constexpr static sz_t value{4};
            };

            template <typename T>
            constexpr sz_t n_components_for{n_components_for_helper<T>::value};
        }

        namespace impl
        {
            template <typename T>
            struct attrib_type_for_helper
            {
            };

            template <>
            struct attrib_type_for_helper<GLbyte>
            {
                static constexpr GLenum value{GL_BYTE};
            };

            template <>
            struct attrib_type_for_helper<GLubyte>
            {
                static constexpr GLenum value{GL_UNSIGNED_BYTE};
            };

            template <>
            struct attrib_type_for_helper<GLshort>
            {
                static constexpr GLenum value{GL_SHORT};
            };

            template <>
            struct attrib_type_for_helper<GLushort>
            {
                static constexpr GLenum value{GL_UNSIGNED_SHORT};
            };

            template <>
            struct attrib_type_for_helper<GLfloat>
            {
                static constexpr GLenum value{GL_FLOAT};
            };

            template <typename T, glm::precision P>
            struct attrib_type_for_helper<glm::tvec1<T, P>>
            {
                static constexpr GLenum value{attrib_type_for_helper<T>::value};
            };

            template <typename T, glm::precision P>
            struct attrib_type_for_helper<glm::tvec2<T, P>>
            {
                static constexpr GLenum value{attrib_type_for_helper<T>::value};
            };

            template <typename T, glm::precision P>
            struct attrib_type_for_helper<glm::tvec3<T, P>>
            {
                static constexpr GLenum value{attrib_type_for_helper<T>::value};
            };

            template <typename T, glm::precision P>
            struct attrib_type_for_helper<glm::tvec4<T, P>>
            {
                static constexpr GLenum value{attrib_type_for_helper<T>::value};
            };

            template <typename T>
            constexpr GLenum attrib_type_for{attrib_type_for_helper<T>::value};
        }
    }
}