// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include "./shared.hpp"
#include "./resource_base.hpp"

namespace resource
{
    /// @brief Resource class with $unique$ ownership semantics.
    template <typename TBehavior>
    class unique : public impl::resource_base<TBehavior>
    {
    public:
        using base_type = impl::resource_base<TBehavior>;
        using behavior_type = typename base_type::behavior_type;
        using handle_type = typename base_type::handle_type;

    public:
        unique() noexcept = default;
        ~unique() noexcept;
        
        explicit unique(const handle_type& handle) noexcept;

        unique(const unique&) = delete;
        unique& operator=(const unique&) = delete;

        unique(unique&& rhs) noexcept;
        auto& operator=(unique&&) noexcept;

        auto release() noexcept;

        void reset() noexcept;
        void reset(const handle_type& handle) noexcept;

        void swap(unique& rhs) noexcept;

        template <typename>
        friend bool operator==(const unique& lhs, const unique& rhs) noexcept;

        template <typename>
        friend bool operator!=(const unique& lhs, const unique& rhs) noexcept;

        template <typename>
        friend void swap(unique& lhs, unique& rhs) noexcept;
    };
}

#include "./unique_resource.inl"