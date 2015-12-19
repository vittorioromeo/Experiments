// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

// TODO: wip

#pragma once

#include "./shared.hpp"
#include "./shared_resource.hpp"

namespace resource
{
    template <typename TBehavior>
    auto shared<TBehavior>::is_null_ref_counter() const noexcept
    {
        return _ref_counter.is_null();
    }

    template <typename TBehavior>
    auto shared<TBehavior>::is_null_handle() const noexcept
    {
        return _handle == behavior_type::null_handle();
    }



    template <typename TBehavior>
    shared<TBehavior>::shared() noexcept
    {
        assert(is_null_ref_counter());
    }

    template <typename TBehavior>
    shared<TBehavior>::~shared() noexcept
    {
        reset();
    }

    template <typename TBehavior>
    shared<TBehavior>::shared(const shared& rhs)
        : _handle{rhs._handle}, _ref_counter{rhs._ref_counter}
    {
        // Two construction possibilities:
        // 1. From a non-owning shared pointer (null handle).
        // 2. From an owning shared pointer.

        // There is no way we need to allocate a reference counter.

        if(!is_null_handle())
        {
            _ref_counter.acquire_existing();
        }
    }

    template <typename TBehavior>
    auto& shared<TBehavior>::operator=(const shared& rhs)
    {
        _handle = rhs._handle;
        _ref_counter = rhs._ref_counter;

        if(!is_null_handle())
        {
            _ref_counter.acquire_existing();
        }
    }

    template <typename TBehavior>
    shared<TBehavior>::shared(const handle_type& handle) noexcept
        : _handle{handle}
    {
        assert(_ref_counter.is_null());

        if(!is_null_handle())
        {
            _ref_counter.acquire_from_null();
        }
    }

    template <typename TBehavior>
    shared<TBehavior>::shared(shared&& rhs) noexcept
        : _handle{std::move(rhs._handle)},
          _ref_counter{std::move(rhs._ref_counter)}
    {
        // TODO: Needed?
        rhs._handle = behavior_type::null_handle();
        assert(rhs.is_null_handle());

        assert(rhs.is_null_ref_counter());
    }

    template <typename TBehavior>
    auto& shared<TBehavior>::operator=(shared&& rhs) noexcept
    {
        _handle = std::move(rhs._handle);
        _ref_counter = std::move(rhs._ref_counter);

        // TODO: Needed?
        rhs._handle = behavior_type::null_handle();
        assert(rhs.is_null_handle());

        assert(rhs.is_null_ref_counter());
        return *this;
    }

    template <typename TBehavior>
    void shared<TBehavior>::lose_ownership() noexcept
    {
        assert(!is_null_ref_counter());

        _ref_counter.lose_ownership([this]
            {
                behavior_type::deinit(_handle);
            });

        // TODO: Needed?
        _handle = behavior_type::null_handle();

        assert(is_null_handle());
        assert(is_null_ref_counter());
    }

    template <typename TBehavior>
    void shared<TBehavior>::reset() noexcept
    {
        if(_ref_counter.is_null())
        {
            assert(is_null_handle());
        }
        else
        {
            lose_ownership();
        }
    }

    template <typename TBehavior>
    void shared<TBehavior>::reset(const handle_type& handle) noexcept
    {
        if(_ref_counter.is_null())
        {
            assert(is_null_handle());

            _handle = handle;
            _ref_counter.acquire_from_null();
        }
        else
        {
            assert(!is_null_handle());

            lose_ownership();
            _handle = handle;

            if(!is_null_handle())
            {
                _ref_counter.acquire_from_null();
            }
        }
    }

    template <typename TBehavior>
    void shared<TBehavior>::swap(shared& rhs) noexcept
    {
        using std::swap;
        swap(_handle, rhs._handle);
        swap(_ref_counter, rhs._ref_counter);
    }

    template <typename TBehavior>
    auto shared<TBehavior>::get() const noexcept
    {
        return _handle;
    }

    template <typename TBehavior>
    auto shared<TBehavior>::use_count() const noexcept
    {
        return _ref_counter.use_count();
    }

    template <typename TBehavior>
    bool shared<TBehavior>::unique() const noexcept
    {
        return use_count() == 1;
    }

    template <typename TBehavior>
    shared<TBehavior>::operator bool() const noexcept
    {
        return !is_null_ref_counter();
    }


    template <typename TBehavior>
    bool operator==(
        const shared<TBehavior>& lhs, const shared<TBehavior>& rhs) noexcept
    {
        return lhs._handle == rhs._handle;
    }

    template <typename TBehavior>
    bool operator!=(
        const shared<TBehavior>& lhs, const shared<TBehavior>& rhs) noexcept
    {
        return !(lhs == rhs);
    }

    template <typename TBehavior>
    void swap(shared<TBehavior>& lhs, shared<TBehavior>& rhs) noexcept
    {
        lhs.swap(rhs);
    }
}
