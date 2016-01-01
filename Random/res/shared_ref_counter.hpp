// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include "./shared.hpp"
#include "./shared_metadata.hpp"

namespace resource
{
    namespace impl
    {
        // TODO: test polymorphism with custom shared_ptr
        class shared_ref_counter
        {
        private:
            shared_metadata* _metadata{nullptr};

            auto& access_metadata() noexcept
            {
                assert(!is_null());
                return *_metadata;
            }

            const auto& access_metadata() const noexcept
            {
                assert(!is_null());
                return *_metadata;
            }

            auto real_count() const noexcept
            {
                return access_metadata().count();
            }

            auto weak_count() const noexcept
            {
                return access_metadata().weak_count();
            }

            void decrement() noexcept
            {
                access_metadata().decrement();
            }

            void deallocate() noexcept
            {
                assert(access_metadata().count() == 0);
                delete _metadata;
            }

        public:
            shared_ref_counter() noexcept = default;

            shared_ref_counter(const shared_ref_counter&) = default;
            shared_ref_counter& operator=(const shared_ref_counter&) = default;

            shared_ref_counter(shared_ref_counter&& rhs) noexcept
                : _metadata{rhs._metadata}
            {
                rhs._metadata = nullptr;
            }

            auto& operator=(shared_ref_counter&& rhs) noexcept
            {
                _metadata = rhs._metadata;
                rhs._metadata = nullptr;
                return *this;
            }

            bool is_null() const noexcept
            {
                return _metadata == nullptr;
            }

            auto use_count() const noexcept
            {
                return is_null() ? 0 : real_count();
            }

            void acquire_from_null()
            {
                assert(is_null());
                _metadata = new shared_metadata{1, 0};
                // TODO: could throw `std::bad_alloc`.
            }

            void acquire_existing() noexcept
            {
                assert(!is_null());
                access_metadata().increment();
            }

            template <typename TF>
            void lose_ownership(TF&& deleter) noexcept
            {
                decrement();

                if(real_count() == 0)
                {
                    deleter();
                    deallocate();
                }

                _metadata = nullptr;
            }
        };
    }
}