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

            auto weak_count() const noexcept
            {
                return access_metadata().weak_count();
            }

            auto total_count() const noexcept
            {
                return access_metadata().total_count();
            }

            auto has_any_ref() const noexcept
            {
                return access_metadata().has_any_ref();
            }

            void increment_owner() noexcept
            {
                access_metadata().increment_owner();
            }

            void decrement_owner() noexcept
            {
                access_metadata().decrement_owner();
            }

            void deallocate() noexcept
            {
                // TODO: total_count?

                assert(access_metadata().owner_count() == 0);
                delete _metadata;
            }

            template <typename TF>
            void deallocate_if_required(TF&& deleter) noexcept
            {
                if(!has_any_ref())
                {
                    deleter();
                    deallocate();
                }

                _metadata = nullptr;
            }

            void decrement_weak() noexcept
            {
                access_metadata().decrement_weak();
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

            auto owner_count() const noexcept
            {
                return access_metadata().owner_count();
            }

            auto use_count() const noexcept
            {
                // TODO: owner count?

                return is_null() ? 0 : owner_count();
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
                increment_owner();
            }

            void increment_weak() noexcept
            {
                access_metadata().increment_weak();
            }



            template <typename TF>
            void lose_ownership(TF&& deleter) noexcept
            {
                decrement_owner();
                deallocate_if_required(FWD(deleter));
            }

            template <typename TF>
            void lose_weak(TF&& deleter)
            {
                decrement_weak();
                deallocate_if_required(FWD(deleter));
            }
        };
    }
}