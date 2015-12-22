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
    namespace impl
    {
        template <typename TBehavior, typename TLockPolicy>
            auto& shared<TBehavior, TLockPolicy>::access_ref_counter() &
            noexcept
        {
            // TODO: use lock policy
            return _ref_counter;
        }

        template <typename TBehavior, typename TLockPolicy>
        const auto&
        shared<TBehavior, TLockPolicy>::access_ref_counter() const& noexcept
        {
            return _ref_counter;
        }

        template <typename TBehavior, typename TLockPolicy>
            auto shared<TBehavior, TLockPolicy>::access_ref_counter() &&
            noexcept
        {
            return std::move(_ref_counter);
        }

        template <typename TBehavior, typename TLockPolicy>
        void shared<TBehavior, TLockPolicy>::nullify_and_assert() noexcept
        {
            base_type::nullify();
            assert(base_type::is_null_handle());
            assert(access_ref_counter().is_null());
        }

        template <typename TBehavior, typename TLockPolicy>
        void shared<TBehavior, TLockPolicy>::acquire_from_null_if_required()
        {
            if(base_type::is_null_handle())
                return;

            access_ref_counter().acquire_from_null();
        }

        template <typename TBehavior, typename TLockPolicy>
        void shared<TBehavior, TLockPolicy>::acquire_existing_if_required()
        {
            if(base_type::is_null_handle())
                return;

            access_ref_counter().acquire_existing();
        }

        template <typename TBehavior, typename TLockPolicy>
        shared<TBehavior, TLockPolicy>::~shared() noexcept
        {
            reset();
        }

        template <typename TBehavior, typename TLockPolicy>
        shared<TBehavior, TLockPolicy>::shared(const shared& rhs)
            : base_type{rhs._handle}, _ref_counter{rhs.access_ref_counter()}
        {
            // Two construction possibilities:
            // 1. From a non-owning shared pointer (null handle).
            // 2. From an owning shared pointer.

            // There is no way we need to allocate a reference counter.
            acquire_existing_if_required();
        }

        template <typename TBehavior, typename TLockPolicy>
        auto& shared<TBehavior, TLockPolicy>::operator=(const shared& rhs)
        {
            assert(this != &rhs);

            base_type::_handle = rhs._handle;
            _ref_counter = rhs.access_ref_counter();

            acquire_existing_if_required();
            return *this;
        }

        template <typename TBehavior, typename TLockPolicy>
        shared<TBehavior, TLockPolicy>::shared(
            const handle_type& handle) noexcept : base_type{handle}
        {
            acquire_from_null_if_required();
        }

        template <typename TBehavior, typename TLockPolicy>
        shared<TBehavior, TLockPolicy>::shared(shared&& rhs) noexcept
            : base_type{rhs._handle},
              _ref_counter{std::move(rhs.access_ref_counter())}
        {
            rhs.nullify_and_assert();
        }

        template <typename TBehavior, typename TLockPolicy>
        auto& shared<TBehavior, TLockPolicy>::operator=(shared&& rhs) noexcept
        {
            assert(this != &rhs);

            base_type::_handle = std::move(rhs._handle);
            _ref_counter = std::move(rhs.access_ref_counter());

            rhs.nullify_and_assert();
            return *this;
        }

        template <typename TBehavior, typename TLockPolicy>
        void shared<TBehavior, TLockPolicy>::lose_ownership() noexcept
        {
            access_ref_counter().lose_ownership([this]
                {
                    base_type::deinit();
                });

            nullify_and_assert();
        }

        template <typename TBehavior, typename TLockPolicy>
        void shared<TBehavior, TLockPolicy>::reset() noexcept
        {
            if(access_ref_counter().is_null())
            {
                assert(base_type::is_null_handle());
            }
            else
            {
                lose_ownership();
            }
        }

        template <typename TBehavior, typename TLockPolicy>
        void shared<TBehavior, TLockPolicy>::reset(
            const handle_type& handle) noexcept
        {
            if(base_type::is_null_handle())
            {
                base_type::_handle = handle;
                access_ref_counter().acquire_from_null();
            }
            else
            {
                lose_ownership();

                base_type::_handle = handle;
                acquire_from_null_if_required();
            }
        }

        template <typename TBehavior, typename TLockPolicy>
        void shared<TBehavior, TLockPolicy>::swap(shared& rhs) noexcept
        {
            using std::swap;
            base_type::swap(rhs);

            // TODO: access?
            swap(_ref_counter, rhs._ref_counter);
        }


        template <typename TBehavior, typename TLockPolicy>
        auto shared<TBehavior, TLockPolicy>::use_count() const noexcept
        {
            return access_ref_counter().use_count();
        }

        template <typename TBehavior, typename TLockPolicy>
        bool shared<TBehavior, TLockPolicy>::unique() const noexcept
        {
            return use_count() == 1;
        }

        template <typename TBehavior, typename TLockPolicy>
        bool operator==(const shared<TBehavior, TLockPolicy>& lhs,
            const shared<TBehavior, TLockPolicy>& rhs) noexcept
        {
            return lhs._handle == rhs._handle;
        }

        template <typename TBehavior, typename TLockPolicy>
        bool operator!=(const shared<TBehavior, TLockPolicy>& lhs,
            const shared<TBehavior, TLockPolicy>& rhs) noexcept
        {
            return !(lhs == rhs);
        }

        template <typename TBehavior, typename TLockPolicy>
        void swap(shared<TBehavior, TLockPolicy>& lhs,
            shared<TBehavior, TLockPolicy>& rhs) noexcept
        {
            lhs.swap(rhs);
        }
    }
}
