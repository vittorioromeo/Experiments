// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <vrm/sdl/common.hpp>
#include <vrm/sdl/context.hpp>
#include <vrm/sdl/gl/shader.hpp>
#include <vrm/sdl/gl/attribute.hpp>

namespace vrm
{
    namespace sdl
    {
        class program
        {
        private:
            impl::unique_program id;

        public:
            program() = default;

            program(impl::unique_program&& mResource) : id{std::move(mResource)}
            {
            }

            operator GLint() const noexcept { return *id; }

            template <typename... TShaders>
            inline void attach_and_link(TShaders&&... mShaders) noexcept
            {
                for_args(
                    [this](auto&& s)
                    {
                        VRM_SDL_GLCHECK(glAttachShader(*id, FWD(s)));
                    },
                    FWD(mShaders)...);

                VRM_SDL_GLCHECK(glLinkProgram(*id));

                for_args(
                    [this](auto&& s)
                    {
                        VRM_SDL_GLCHECK(glDetachShader(*id, FWD(s)));
                    },
                    FWD(mShaders)...);
            }

            void use() noexcept { glUseProgram(*id); }

            auto get_attribute(const std::string& a_name) const noexcept
            {
                GLuint location;

                VRM_SDL_GLCHECK(
                    location = glGetAttribLocation(*id, a_name.c_str()););

                assert(location != GL_INVALID_OPERATION);

                return attribute{location};
            }

            auto get_uniform(const std::string& a_name) const noexcept
            {
                GLuint location;

                VRM_SDL_GLCHECK(
                    location = glGetUniformLocation(*id, a_name.c_str()););

                assert(location != GL_INVALID_OPERATION);

                return uniform{location};
            }
        };

        template <typename... TShaders>
        auto make_program(TShaders&&... mShaders) noexcept
        {
            GLuint id;
            VRM_SDL_GLCHECK(id = glCreateProgram(););

            impl::unique_program res{id};
            program p{std::move(res)};

            p.attach_and_link(FWD(mShaders)...);

            return p;
        }

        namespace impl
        {
            class gltexture2d
            {
            private:
                GLuint _id;

                GLuint _width{0}, _height{0};

                // GLuint _internal_format{GL_RGB};
                // GLuint _image_format{GL_RGB};

                // GLuint _wrap_s{GL_REPEAT};
                // GLuint _wrap_t{GL_REPEAT};
                //GLuint _filter_min{GL_LINEAR};
               // GLuint _filter_max{GL_LINEAR};

            public:
                gltexture2d() { VRM_SDL_GLCHECK(glGenTextures(1, &_id)); }

                void generate(
                    GLuint mode, GLuint width, GLuint height, const void* data)
                {
                    _width = width;
                    _height = height;

                    bind();

                    VRM_SDL_GLCHECK(glTexImage2D(GL_TEXTURE_2D, 0, mode, width,
                        height, 0, mode, GL_UNSIGNED_BYTE, data));

                    VRM_SDL_GLCHECK(glTexParameteri(
                        GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));

                    VRM_SDL_GLCHECK(glTexParameteri(
                        GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

                    VRM_SDL_GLCHECK(glTexParameteri(
                        GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));

                    VRM_SDL_GLCHECK(glTexParameteri(
                        GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

                    unbind();
                }

                void bind()
                {
                    VRM_SDL_GLCHECK(glBindTexture(GL_TEXTURE_2D, _id));
                }
                void unbind()
                {
                    VRM_SDL_GLCHECK(glBindTexture(GL_TEXTURE_2D, 0));
                }

                template <typename TF>
                void with(TF&& f)
                {
                    bind();
                    f();
                    unbind();
                }

                void deletetx() { VRM_SDL_GLCHECK(glDeleteTextures(1, &_id)); }
            };

            struct gltexture2d_deleter
            {
                void operator()(gltexture2d& t) noexcept { t.deletetx(); }
            };

            using unique_gltexture2d =
                unique_resource<impl::gltexture2d, gltexture2d_deleter>;
        }

        auto next_pow2(int x)
        {
            return std::pow(2, std::ceil(std::log(x) / std::log(2)));
        }

        auto make_gltexture2d(surface& s)
        {
            impl::gltexture2d t;

            auto mode(GL_RGB);

            if(s.format()->BytesPerPixel == 4)
            {
                mode = GL_RGBA;
            }

            t.generate(mode, s.width(), s.height(), s.pixels());

            return impl::unique_gltexture2d{t};
        }
    }
}