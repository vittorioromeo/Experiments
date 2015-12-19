// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include "./shared.hpp"
#include "./unique_resource.hpp"
#include "./shared_resource.hpp"

namespace access
{
    template <typename TBehavior>
    class unmanaged
    {
    public:
        using behavior_type = TBehavior;
        using handle_type = typename TBehavior::handle_type;

    private:
        handle_type _handle;

    public:
        template <typename... Ts>
        unmanaged(Ts&&... xs) noexcept : _handle{FWD(xs)...}
        {
        }

        unmanaged(const unmanaged&) = default;
        unmanaged& operator=(const unmanaged&) = default;

        unmanaged(unmanaged&&) = default;
        unmanaged& operator=(unmanaged&&) = default;

        auto handle() const noexcept
        {
            return _handle;
        }

        void dispose() noexcept
        {
            behavior_type::deinit(_handle);
            _handle = behavior_type::null_handle();
        }
    };

    namespace impl
    {
        template <typename TBehavior, template <typename> class TResource>
        class resource : public TResource<TBehavior>
        {
        public:
            using behavior_type = TBehavior;
            using resource_type = TResource<TBehavior>;

        public:
            using resource_type::resource_type;

            resource(const resource&) = default;
            resource& operator=(const resource&) = default;

            resource(resource&&) = default;
            resource& operator=(resource&&) = default;
        };
    }

    template <typename TBehavior>
    using unique = impl::resource<TBehavior, resource::unique>;

    template <typename TBehavior>
    using shared = impl::resource<TBehavior, resource::shared>;
}

// TODO: maybe `handle()` and similar methods should be free functions.