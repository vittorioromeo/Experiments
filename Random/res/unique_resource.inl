// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include "./unique_resource.hpp"

namespace resource
{
    template <typename TBehavior>
    unique<TBehavior>::~unique() noexcept
    {
        reset();
    }

    template <typename TBehavior>
    unique<TBehavior>::unique(const handle_type& handle) noexcept
        : base_type{handle}
    {
    }

    template <typename TBehavior>
    unique<TBehavior>::unique(unique&& rhs) noexcept : base_type{rhs.release()}
    {
    }

    template <typename TBehavior>
    auto& unique<TBehavior>::operator=(unique&& rhs) noexcept
    {
        assert(this != &rhs);
        
        reset(rhs.release());
        return *this;
    }

    template <typename TBehavior>
    auto unique<TBehavior>::release() noexcept
    {
        return base_type::release_and_nullify();
    }

    template <typename TBehavior>
    void unique<TBehavior>::reset() noexcept
    {
        base_type::deinit();
        base_type::nullify();
    }

    template <typename TBehavior>
    void unique<TBehavior>::reset(const handle_type& handle) noexcept
    {
        base_type::deinit();
        base_type::_handle = handle;
    }

    template <typename TBehavior>
    void unique<TBehavior>::swap(unique& rhs) noexcept
    {
        base_type::swap(rhs);
    }

    template <typename TBehavior>
    bool operator==(
        const unique<TBehavior>& lhs, const unique<TBehavior>& rhs) noexcept
    {
        return lhs._handle == rhs._handle;
    }

    template <typename TBehavior>
    bool operator!=(
        const unique<TBehavior>& lhs, const unique<TBehavior>& rhs) noexcept
    {
        return !(lhs == rhs);
    }

    template <typename TBehavior>
    void swap(unique<TBehavior>& lhs, unique<TBehavior>& rhs) noexcept
    {
        lhs.swap(rhs);
    }
}