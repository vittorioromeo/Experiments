// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

// TODO: wip

#pragma once

#include "./shared.hpp"
#include "./shared_metadata.hpp"
#include "./shared_ref_counter.hpp"
#include "./resource_base.hpp"
#include "./shared_resource.hpp"
#include "./weak.hpp"

namespace resource
{
    namespace impl
    {
        template <typename TBehavior, typename TLockPolicy>
        weak<TBehavior, TLockPolicy>::weak() noexcept
        {
        }

        template <typename TBehavior, typename TLockPolicy>
        weak<TBehavior, TLockPolicy>::weak(const weak& rhs) noexcept
        {
        }

        template <typename TBehavior, typename TLockPolicy>
        weak<TBehavior, TLockPolicy>::weak(const shared_type& rhs) noexcept
        {
        }

        template <typename TBehavior, typename TLockPolicy>
        weak<TBehavior, TLockPolicy>::weak(weak&& rhs) noexcept
        {
        }

        template <typename TBehavior, typename TLockPolicy>
        auto& weak<TBehavior, TLockPolicy>::operator=(const weak& rhs) noexcept
        {
            return *this;
        }

        template <typename TBehavior, typename TLockPolicy>
        auto& weak<TBehavior, TLockPolicy>::operator=(
            const shared_type& rhs) noexcept
        {
            return *this;
        }

        template <typename TBehavior, typename TLockPolicy>
        auto& weak<TBehavior, TLockPolicy>::operator=(weak&& rhs) noexcept
        {
            return *this;
        }

        template <typename TBehavior, typename TLockPolicy>
        weak<TBehavior, TLockPolicy>::~weak()
        {
        }

        template <typename TBehavior, typename TLockPolicy>
        void weak<TBehavior, TLockPolicy>::swap(weak& rhs) noexcept
        {
        }

        template <typename TBehavior, typename TLockPolicy>
        void weak<TBehavior, TLockPolicy>::reset() noexcept
        {
        }

        template <typename TBehavior, typename TLockPolicy>
        auto weak<TBehavior, TLockPolicy>::use_count() const noexcept
        {
        }

        template <typename TBehavior, typename TLockPolicy>
        auto weak<TBehavior, TLockPolicy>::expired() const noexcept
        {
        }

        template <typename TBehavior, typename TLockPolicy>
        auto weak<TBehavior, TLockPolicy>::lock() const noexcept
        {
            return shared_type{};
        }
    }

    // TODO: swap, comparison ops
}