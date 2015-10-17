// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <vrm/sdl/common.hpp>
#include <vrm/sdl/context.hpp>
#include <vrm/sdl/gl/check.hpp>
#include <vrm/sdl/gl/shader.hpp>
#include <vrm/sdl/gl/attribute.hpp>

VRM_SDL_NAMESPACE
{
    namespace impl
    {
        class gltexture2d
        {
        private:
            GLuint _id;
            vec2f _size;

        public:
            const auto& size() const noexcept { return _size; }

            auto location() const noexcept { return _id; }

            gltexture2d() noexcept { VRM_SDL_GLCHECK(glGenTextures(1, &_id)); }

            void generate(GLuint mode, GLuint width, GLuint height,
                const void* data) noexcept
            {
                _size.x = width;
                _size.y = height;

                bind();
                {
                    VRM_SDL_GLCHECK(glTexImage2D(GL_TEXTURE_2D, 0, mode, width,
                        height, 0, mode, GL_UNSIGNED_BYTE, data));

                    // Only `GL_CLAMP_TO_EDGE` is supported for
                    // non-power-of-two textures.

                    VRM_SDL_GLCHECK(glTexParameteri(
                        GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));

                    VRM_SDL_GLCHECK(glTexParameteri(
                        GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

                    VRM_SDL_GLCHECK(glTexParameteri(
                        GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));

                    VRM_SDL_GLCHECK(glTexParameteri(
                        GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
                }
                unbind();
            }

            void activate(GLenum texture_unit) const noexcept
            {
                VRM_SDL_GLCHECK(glActiveTexture(texture_unit));
            }

            void bind() const noexcept
            {
                VRM_SDL_GLCHECK(glBindTexture(GL_TEXTURE_2D, _id));
            }

            void unbind() const noexcept
            {
                VRM_SDL_GLCHECK(glBindTexture(GL_TEXTURE_2D, 0));
            }

            void activate_and_bind(GLenum texture_unit) const noexcept
            {
                activate(texture_unit);
                bind();
            }

            template <typename TF>
            void with(GLenum texture_unit, TF&& f) const noexcept
            {
                activate_and_bind(texture_unit);
                f();
                unbind();
            }

            void deletetx() noexcept
            {
                VRM_SDL_GLCHECK(glDeleteTextures(1, &_id));
            }
        };

        struct gltexture2d_deleter
        {
            void operator()(gltexture2d& t) noexcept { t.deletetx(); }
        };

        using unique_gltexture2d =
            unique_resource<impl::gltexture2d, gltexture2d_deleter>;

        constexpr auto get_texture_unit_idx(GLenum texture_unit) noexcept
        {
            return to_num<sz_t>(texture_unit) - to_num<sz_t>(GL_TEXTURE0);
        }

        constexpr auto get_texture_unit(sz_t idx) noexcept
        {
            return static_cast<GLenum>(static_cast<sz_t>(GL_TEXTURE0) + idx);
        }

        constexpr auto get_max_texture_unit_count() noexcept
        {
            return GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS;
        }

        constexpr auto get_valid_texture_unit_count(sz_t desired) noexcept
        {
            return std::min(
                desired, to_num<sz_t>(get_max_texture_unit_count()));
        }
    }

    auto make_gltexture2d(surface & s) noexcept
    {
        impl::gltexture2d t;

        // auto mode(s.format()->BytesPerPixel == 4 ? GL_RGBA : GL_RGB);
        auto mode(GL_RGBA);
        t.generate(mode, s.width(), s.height(), s.pixels());

        return impl::unique_gltexture2d{t};
    }
}
VRM_SDL_NAMESPACE_END