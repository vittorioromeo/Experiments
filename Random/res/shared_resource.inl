// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

// TODO: wip

#pragma once

#include "./shared.hpp"
#include "./shared_resource.hpp"

// TODO: always check metadata==nullptr, not null_handle

namespace resource
{
    template <typename TBehavior>
    auto shared<TBehavior>::is_null() const noexcept
    {
        return _metadata == nullptr;
    }

    template <typename TBehavior>
    void shared<TBehavior>::increment_metadata_if_not_null() noexcept
    {
        if(!is_null())
            _metadata->increment();
    }

    template <typename TBehavior>
    shared<TBehavior>::shared() noexcept
        : _handle{behavior_type::null_handle()},
          _metadata{nullptr}
    {
        assert(is_null());
    }

    template <typename TBehavior>
    shared<TBehavior>::~shared() noexcept
    {
        reset();
    }

    template <typename TBehavior>
    shared<TBehavior>::shared(const shared& rhs)
        : _handle{rhs._handle}, _metadata{rhs._metadata}
    {
        increment_metadata_if_not_null();
    }

    template <typename TBehavior>
    auto& shared<TBehavior>::operator=(const shared& rhs)
    {
        _handle = rhs._handle;
        _metadata = rhs._metadata;

        increment_metadata_if_not_null();
    }

    template <typename TBehavior>
    shared<TBehavior>::shared(const handle_type& handle) noexcept
        : _handle{behavior_type::null_handle()},
          _metadata{nullptr}
    {
        reset(handle);
    }

    template <typename TBehavior>
    shared<TBehavior>::shared(shared&& rhs) noexcept
        : _handle{rhs.release_handle()},
          _metadata{rhs.release_metadata()}
    {
        assert(rhs.is_null());
    }

    template <typename TBehavior>
    auto& shared<TBehavior>::operator=(shared&& rhs) noexcept
    {
        _handle = rhs.release_handle();
        _metadata = rhs.release_metadata();

        assert(rhs.is_null());
        return *this;
    }

    template <typename TBehavior>
    auto shared<TBehavior>::release_handle() noexcept
    {
        // TODO: this is ALWAYS called with `release_metadata`.

        auto temp_handle(_handle);
        _handle = behavior_type::null_handle();
        return temp_handle;
    }

    template <typename TBehavior>
    auto shared<TBehavior>::release_metadata() noexcept
    {
        auto temp_metadata(_metadata);
        _metadata = nullptr;
        return temp_metadata;
    }

    template <typename TBehavior>
    void shared<TBehavior>::reset() noexcept
    {
        if(_metadata != nullptr)
        {
            _metadata->decrement();

            if(_metadata->count() == 0)
            {
                behavior_type::deinit(_handle);
                delete _metadata;

                release_handle();
                release_metadata();
            }
        }
    }

    template <typename TBehavior>
    void shared<TBehavior>::reset(const handle_type& handle) noexcept
    {
        if(_metadata == nullptr)
        {
            _metadata = new metadata_type{};
            _metadata->increment();

            assert(_metadata->count() == 1);
        }
        else
        {
            behavior_type::deinit(_handle);
        }

        _handle = handle;
    }

    template <typename TBehavior>
    void shared<TBehavior>::swap(shared& rhs) noexcept
    {
        using std::swap;
        swap(_handle, rhs._handle);
        swap(_metadata, rhs._metadata);
    }

    template <typename TBehavior>
    auto shared<TBehavior>::get() const noexcept
    {
        return _handle;
    }

    template <typename TBehavior>
    auto shared<TBehavior>::use_count() const noexcept
    {
        if(_metadata == nullptr)
            return 0;

        return _metadata->count();
    }

    template <typename TBehavior>
    bool shared<TBehavior>::unique() const noexcept
    {
        return use_count() == 1;
    }

    template <typename TBehavior>
    shared<TBehavior>::operator bool() const noexcept
    {
        return _handle != behavior_type::null_handle();
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
