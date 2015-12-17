// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include "./shared.hpp"

namespace resource
{
    template <typename TBehavior>
    class unique
    {
    public:
        using behavior_type = TBehavior;
        using handle_type = typename behavior_type::handle_type;

    private:
        handle_type _handle;

    public:
        unique() noexcept;
        ~unique() noexcept;

        unique(const unique&) = delete;
        unique& operator=(const unique&) = delete;

        explicit unique(const handle_type& handle) noexcept;

        unique(unique&& rhs) noexcept;
        auto& operator=(unique&&) noexcept;

        auto release() noexcept;

        void reset() noexcept;
        void reset(const handle_type& handle) noexcept;

        void swap(unique& rhs) noexcept;

        auto get() const noexcept;

        explicit operator bool() const noexcept;

        template <typename>
        friend bool operator==(const unique& lhs, const unique& rhs) noexcept;

        template <typename>
        friend bool operator!=(const unique& lhs, const unique& rhs) noexcept;

        template <typename>
        friend void swap(unique& lhs, unique& rhs) noexcept;
    };
}

#include "./unique_resource.inl"