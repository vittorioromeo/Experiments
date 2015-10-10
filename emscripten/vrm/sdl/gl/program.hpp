// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <vrm/sdl/common.hpp>
#include <vrm/sdl/context.hpp>
#include <vrm/sdl/gl/shader.hpp>

namespace vrm
{
    namespace sdl
    {
        class program
        {
        private:
            impl::unique_program id;

        public:
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
                        glAttachShader(*id, FWD(s));
                    },
                    FWD(mShaders)...);

                glLinkProgram(*id);

                for_args(
                    [this](auto&& s)
                    {
                        glDetachShader(*id, FWD(s));
                    },
                    FWD(mShaders)...);
            }

            inline void use() noexcept { glUseProgram(*id); }

            inline auto attribute(const std::string& mName) const noexcept {}
        };

        template <typename... TShaders>
        auto make_program(TShaders&&... mShaders) noexcept
        {
            auto id(glCreateProgram());

            impl::unique_program res{id};
            program p{std::move(res)};

            p.attach_and_link(FWD(mShaders)...);

            return p;
        }
    }
}