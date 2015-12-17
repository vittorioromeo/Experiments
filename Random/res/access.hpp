// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include "./shared.hpp"
#include "./unique_resource.hpp"

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

        auto handle() const noexcept
        {
            return _handle;
        }
    };

    namespace impl
    {
        template <typename TBehavior, template <typename> class TResource>
        class resource
        {
        public:
            using behavior_type = TBehavior;
            using resource_type = TResource<TBehavior>;

        private:
            resource_type _resource;

        public:
            template <typename... Ts>
            resource(Ts&&... xs) noexcept : _resource{FWD(xs)...}
            {
            }

            auto handle() const noexcept
            {
                return _resource.get();
            }
        };
    }

    template <typename TBehavior>
    using unique = impl::resource<TBehavior, resource::unique>;
}