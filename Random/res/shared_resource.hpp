// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

// TODO: wip

#pragma once

#include "./shared.hpp"
#include "./shared_metadata.hpp"

namespace resource
{
    template <typename TBehavior>
    class shared
    {
    public:
        using behavior_type = TBehavior;
        using handle_type = typename behavior_type::handle_type;
        using metadata_type = impl::shared_metadata;
        using ref_counter_type = impl::shared_ref_counter;

    private:
        handle_type _handle{behavior_type::null_handle()};
        ref_counter_type _ref_counter;

        auto is_null_ref_counter() const noexcept;
        auto is_null_handle() const noexcept;

        void lose_ownership() noexcept;

    public:
        shared() noexcept;
        ~shared() noexcept;

        shared(const shared&);
        auto& operator=(const shared&);

        explicit shared(const handle_type& handle) noexcept;

        shared(shared&& rhs) noexcept;
        auto& operator=(shared&&) noexcept;

        void reset() noexcept;
        void reset(const handle_type& handle) noexcept;

        void swap(shared& rhs) noexcept;

        auto get() const noexcept;

        auto use_count() const noexcept;
        bool unique() const noexcept;

        explicit operator bool() const noexcept;

        template <typename>
        friend bool operator==(const shared& lhs, const shared& rhs) noexcept;

        template <typename>
        friend bool operator!=(const shared& lhs, const shared& rhs) noexcept;

        template <typename>
        friend void swap(shared& lhs, shared& rhs) noexcept;
    };
}

#include "./shared_resource.inl"