// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include "./shared.hpp"

namespace resource
{
    namespace impl
    {
        template <typename TBehavior, typename TLockPolicy>
        class weak;

        template <typename TBehavior>
        class resource_base
        {
            template <typename, typename>
            friend class weak;

        public:
            using behavior_type = TBehavior;
            using handle_type = typename behavior_type::handle_type;

        private:
            static auto null_handle() noexcept;

        protected:
            handle_type _handle;

            resource_base() noexcept;
            explicit resource_base(const handle_type& handle) noexcept;

            auto is_null_handle() const noexcept;

            void deinit();
            void nullify() noexcept;
            auto release_and_nullify() noexcept;

            void swap(resource_base& rhs) noexcept;

        public:
            auto get() const noexcept;
            explicit operator bool() const noexcept;
        };
    }
}

#include "./resource_base.inl"