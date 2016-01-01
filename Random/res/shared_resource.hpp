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

namespace resource
{
    namespace impl
    {
        namespace shared_lock_policy
        {
            struct none
            {
            };
        }

        template <typename TBehavior, typename TLockPolicy>
        class shared : public impl::resource_base<TBehavior>, TLockPolicy
        {
        public:
            using base_type = impl::resource_base<TBehavior>;
            using behavior_type = typename base_type::behavior_type;
            using handle_type = typename base_type::handle_type;
            using ref_counter_type = impl::shared_ref_counter;
            using lock_policy_type = TLockPolicy;

        private:
            ref_counter_type _ref_counter;

            auto& access_ref_counter() & noexcept;
            const auto& access_ref_counter() const& noexcept;
            auto access_ref_counter() && noexcept;

            void lose_ownership() noexcept;
            void nullify_and_assert() noexcept;
            void acquire_from_null_if_required();
            void acquire_existing_if_required();

        public:
            shared() noexcept = default;
            ~shared() noexcept;

            explicit shared(const handle_type& handle) noexcept;

            shared(const shared&);
            auto& operator=(const shared&);

            shared(shared&& rhs) noexcept;
            auto& operator=(shared&&) noexcept;

            void reset() noexcept;
            void reset(const handle_type& handle) noexcept;

            void swap(shared& rhs) noexcept;

            auto use_count() const noexcept;
            bool unique() const noexcept;

            template <typename>
            friend bool operator==(
                const shared& lhs, const shared& rhs) noexcept;

            template <typename>
            friend bool operator!=(
                const shared& lhs, const shared& rhs) noexcept;

            template <typename>
            friend void swap(shared& lhs, shared& rhs) noexcept;
        };
    }

    template <typename TBehavior>
    using shared = impl::shared<TBehavior, impl::shared_lock_policy::none>;
}

// TODO:
// shared_resource/shared_resource.hpp
// shared_resource/shared_resource.inl
// shared_resource.hpp (includes both .hpp and .inl and that's it)

#include "./shared_resource.inl"