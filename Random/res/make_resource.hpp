// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include "./shared.hpp"
#include "./unique_resource.hpp"
#include "./shared_resource.hpp"

template <template <typename> class TResource, typename TBehavior>
class resource_maker
{
public:
    using behavior_type = TBehavior;
    using resource_type = TResource<behavior_type>;

private:
    template <typename... Ts>
    decltype(auto) init_resource(Ts&&... xs) noexcept(noexcept(true))
    {
        return behavior_type::init(FWD(xs)...);
    }

public:
    template <typename... Ts>
    auto operator()(Ts&&... xs) noexcept(noexcept(true))
    {
        return resource_type{init_resource(FWD(xs)...)};
    }
};

template <template <typename> class TResource, typename TBehavior,
    typename... Ts>
auto make_resource(Ts&&... xs) noexcept(noexcept(true))
{
    return resource_maker<TResource, TBehavior>{}(FWD(xs)...);
}

template <typename TBehavior, typename... Ts>
auto make_unique_resource(Ts&&... xs) noexcept(noexcept(true))
{
    return make_resource<resource::unique, TBehavior>(FWD(xs)...);
}

template <typename TBehavior, typename... Ts>
auto make_shared_resource(Ts&&... xs) noexcept(noexcept(true))
{
    return make_resource<resource::shared, TBehavior>(FWD(xs)...);
}