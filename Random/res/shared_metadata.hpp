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
        using shared_counter_type = unsigned int;

        class shared_metadata
        {
        private:
            shared_counter_type _count;
            shared_counter_type _weak_count;

        public:
            shared_metadata(shared_counter_type count,
                shared_counter_type weak_count) noexcept
                : _count{count},
                  _weak_count{weak_count}
            {
            }

            shared_metadata(const shared_metadata&) = delete;
            shared_metadata& operator=(const shared_metadata&) = delete;

            shared_metadata(shared_metadata&&) = delete;
            shared_metadata& operator=(shared_metadata&&) = delete;

            void increment() noexcept
            {
                ++_count;
            }

            void decrement() noexcept
            {
                assert(_count > 0);
                --_count;
            }

            auto count() const noexcept
            {
                return _count;
            }

            void increment_weak() noexcept
            {
                ++_weak_count;
            }

            void decrement_weak() noexcept
            {
                assert(_weak_count > 0);
                --_weak_count;
            }

            auto weak_count() const noexcept
            {
                return _weak_count;
            }
        };
    }
}