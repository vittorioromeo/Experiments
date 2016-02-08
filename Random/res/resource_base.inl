// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include "./shared.hpp"
#include "./resource_base.hpp"

namespace resource
{
    namespace impl
    {
        template <typename TBehavior>
        auto resource_base<TBehavior>::null_handle() noexcept
        {
            return behavior_type::null_handle();
        }

        // TODO: clang fmt
        template <typename TBehavior>
        resource_base<TBehavior>::resource_base() noexcept :
        _handle{null_handle()} {}

        template <typename TBehavior>
        resource_base<TBehavior>::resource_base(
            const handle_type& handle) noexcept : _handle{handle}
        {
        }

        template <typename TBehavior>
        auto resource_base<TBehavior>::is_null_handle() const noexcept
        {
            return _handle == null_handle();
        }

        template <typename TBehavior>
        void resource_base<TBehavior>::deinit()
        {
            behavior_type::deinit(_handle);
        }

        template <typename TBehavior>
        void resource_base<TBehavior>::nullify() noexcept
        {
            _handle = null_handle();
        }

        template <typename TBehavior>
        auto resource_base<TBehavior>::release_and_nullify() noexcept
        {
            auto temp(_handle);
            nullify();
            return temp;
        }

        template <typename TBehavior>
        void resource_base<TBehavior>::swap(resource_base& rhs) noexcept
        {
            using std::swap;
            swap(_handle, rhs._handle);
        }

        template <typename TBehavior>
        auto resource_base<TBehavior>::get() const noexcept
        {
            return _handle;
        }

        template <typename TBehavior>
        resource_base<TBehavior>::operator bool() const noexcept
        {
            return !is_null_handle();
        }
    }
}