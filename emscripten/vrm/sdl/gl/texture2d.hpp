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
    enum class texture_format : GLenum
    {
        alpha = GL_ALPHA,
        luminance = GL_LUMINANCE,
        luminance_alpha = GL_LUMINANCE_ALPHA,
        rgb = GL_RGB,
        rgba = GL_RGBA
    };

    enum class texture_filter : GLenum
    {
        nearest = GL_NEAREST,
        linear = GL_LINEAR,
        nearest_mipmap_nearest = GL_NEAREST_MIPMAP_NEAREST,
        linear_mipmap_nearest = GL_LINEAR_MIPMAP_NEAREST,
        nearest_mipmap_linear = GL_NEAREST_MIPMAP_LINEAR,
        linear_mipmap_linear = GL_LINEAR_MIPMAP_LINEAR
    };

    namespace impl
    {
        class gltexture2d
        {
        private:
            GLuint _id;
            vec2f _size;

            bool bound() const noexcept
            {
                GLint result;
                VRM_SDL_GLCHECK(glGetIntegerv(GL_TEXTURE_BINDING_2D, &result));
                return result == _id;
            }

        public:
            const auto& size() const noexcept { return _size; }

            auto location() const noexcept { return _id; }

            gltexture2d() noexcept { VRM_SDL_GLCHECK(glGenTextures(1, &_id)); }

            void generate(int level, texture_format format, const vec2u& size,
                const void* data, texture_filter f) noexcept
            {
                _size = vec2f(size);

                bind();
                {
                    VRM_SDL_GLCHECK(glTexImage2D(GL_TEXTURE_2D, level,
                        from_enum(format), size.x, size.y, 0, from_enum(format),
                        GL_UNSIGNED_BYTE, data));

                    // Only `GL_CLAMP_TO_EDGE` is supported for
                    // non-power-of-two textures.

                    VRM_SDL_GLCHECK(glTexParameteri(
                        GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));

                    VRM_SDL_GLCHECK(glTexParameteri(
                        GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

                    filter(f);
                }
                unbind();
            }

            void generate(const vec2u& size, const void* data,
                texture_filter f = texture_filter::nearest) noexcept
            {
                generate(0, texture_format::rgba, size, data, f);
            }

            void generate_blank(const vec2u& size,
                texture_filter f = texture_filter::nearest) noexcept
            {
                std::vector<GLubyte> blank_pixels(size.x * size.y * 4, 0);
                generate(size, blank_pixels.data(), f);
            }

            void filter(texture_filter f) noexcept
            {
                assert(bound());

                VRM_SDL_GLCHECK(glTexParameteri(
                    GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, from_enum(f)));

                VRM_SDL_GLCHECK(glTexParameteri(
                    GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, from_enum(f)));
            }

            void sub_image_2d(const vec2i& offset, const vec2u& sub_image_size,
                const GLvoid* data)
            {
                assert(bound());

                VRM_SDL_GLCHECK(glTexSubImage2D(GL_TEXTURE_2D, 0, offset.x,
                    offset.y, sub_image_size.x, sub_image_size.y, GL_RGBA,
                    GL_UNSIGNED_BYTE, data));
            }

            void sub_image_2d(const vec2i& offset, const surface& surface)
            {
                sub_image_2d(offset, vec2u(surface.width(), surface.height()),
                    surface.pixels());
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
                assert(bound());
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

    auto make_gltexture2d() noexcept
    {
        impl::gltexture2d t;
        return impl::unique_gltexture2d{t};
    }

    auto make_gltexture2d(surface & s, texture_filter filter) noexcept
    {
        impl::gltexture2d t;
        t.generate(s.size(), s.pixels(), filter);
        return impl::unique_gltexture2d{t};
    }
}
VRM_SDL_NAMESPACE_END