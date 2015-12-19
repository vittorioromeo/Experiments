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
    void shared<TBehavior>::nullify_and_assert() noexcept
    {
        base_type::nullify();
        assert(base_type::is_null_handle());
        assert(_ref_counter.is_null());
    }

    template <typename TBehavior>
    void shared<TBehavior>::acquire_from_null_if_required()
    {
        if(base_type::is_null_handle()) return;
        _ref_counter.acquire_from_null();
    }

    template <typename TBehavior>
    void shared<TBehavior>::acquire_existing_if_required()
    {
        if(base_type::is_null_handle()) return;
        _ref_counter.acquire_existing();
    }

    template <typename TBehavior>
    shared<TBehavior>::~shared() noexcept
    {
        reset();
    }

    template <typename TBehavior>
    shared<TBehavior>::shared(const shared& rhs)
        : base_type{rhs._handle}, _ref_counter{rhs._ref_counter}
    {
        // Two construction possibilities:
        // 1. From a non-owning shared pointer (null handle).
        // 2. From an owning shared pointer.

        // There is no way we need to allocate a reference counter.
        acquire_existing_if_required();
    }

    template <typename TBehavior>
    auto& shared<TBehavior>::operator=(const shared& rhs)
    {
        base_type::_handle = rhs._handle;
        _ref_counter = rhs._ref_counter;

        acquire_existing_if_required();
        return *this;
    }

    template <typename TBehavior>
    shared<TBehavior>::shared(const handle_type& handle) noexcept
        : base_type{handle}
    {
        acquire_from_null_if_required();
    }

    template <typename TBehavior>
    shared<TBehavior>::shared(shared&& rhs) noexcept
        : base_type{rhs._handle},
          _ref_counter{std::move(rhs._ref_counter)}
    {
        rhs.nullify_and_assert();
    }

    template <typename TBehavior>
    auto& shared<TBehavior>::operator=(shared&& rhs) noexcept
    {
        base_type::_handle = std::move(rhs._handle);
        _ref_counter = std::move(rhs._ref_counter);

        rhs.nullify_and_assert();
        return *this;
    }

    template <typename TBehavior>
    void shared<TBehavior>::lose_ownership() noexcept
    {
        _ref_counter.lose_ownership([this]
            {
                base_type::deinit();
            });

        nullify_and_assert();
    }

    template <typename TBehavior>
    void shared<TBehavior>::reset() noexcept
    {
        if(_ref_counter.is_null())
        {
            assert(base_type::is_null_handle());
        }
        else
        {
            lose_ownership();
        }
    }

    template <typename TBehavior>
    void shared<TBehavior>::reset(const handle_type& handle) noexcept
    {
        if(base_type::is_null_handle())
        {
            base_type::_handle = handle;
            _ref_counter.acquire_from_null();
        }
        else
        {
            lose_ownership();

            base_type::_handle = handle;
            acquire_from_null_if_required();
        }
    }

    template <typename TBehavior>
    void shared<TBehavior>::swap(shared& rhs) noexcept
    {
        using std::swap;
        base_type::swap(rhs);
        swap(_ref_counter, rhs._ref_counter);
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
