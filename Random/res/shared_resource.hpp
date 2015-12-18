// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

// TODO: wip

#pragma once

#include "./shared.hpp"
/*
namespace resource
{
    namespace impl
    {
        class shared_data
        {
        };
    }

    template <typename TBehavior>
    class shared
    {
    public:
        using behavior_type = TBehavior;
        using handle_type = typename behavior_type::handle_type;

    private:
        handle_type _handle;

    public:
        shared() noexcept;
        ~shared() noexcept;

        shared(const shared&);
        shared& operator=(const shared&);

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

#include "./shared_resource.inl"*/