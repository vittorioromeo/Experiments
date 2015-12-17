// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include "./unique_resource.hpp"

namespace resource
{
    template <typename TBehavior>
    unique<TBehavior>::unique() noexcept : _handle{behavior_type::null_handle()}
    {
    }

    template <typename TBehavior>
    unique<TBehavior>::~unique() noexcept
    {
        reset();
    }

    template <typename TBehavior>
    unique<TBehavior>::unique(const handle_type& handle) noexcept
        : _handle{handle}
    {
    }

    template <typename TBehavior>
    unique<TBehavior>::unique(unique&& rhs) noexcept : _handle{rhs.release()}
    {
    }

    template <typename TBehavior>
    auto& unique<TBehavior>::operator=(unique&& rhs) noexcept
    {
        reset(rhs.release());
        return *this;
    }

    template <typename TBehavior>
    auto unique<TBehavior>::release() noexcept
    {
        auto temp_handle(_handle);
        _handle = behavior_type::null_handle();
        return temp_handle;
    }

    template <typename TBehavior>
    void unique<TBehavior>::reset() noexcept
    {
        behavior_type::deinit(_handle);
        _handle = behavior_type::null_handle();
    }

    template <typename TBehavior>
    void unique<TBehavior>::reset(const handle_type& handle) noexcept
    {
        behavior_type::deinit(_handle);
        _handle = handle;
    }

    template <typename TBehavior>
    void unique<TBehavior>::swap(unique& rhs) noexcept
    {
        using std::swap;
        swap(_handle, rhs._handle);
    }

    template <typename TBehavior>
    auto unique<TBehavior>::get() const noexcept
    {
        return _handle;
    }

    template <typename TBehavior>
    unique<TBehavior>::operator bool() const noexcept
    {
        return _handle != behavior_type::null_handle();
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