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
        using shared_counter_type = int;

        class shared_metadata
        {
        private:
            shared_counter_type _count{0};

        public:
            shared_metadata() = default;

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
        };
    }
}