#pragma once

#include <tuple>
#include <experimental/tuple>
#include <thread>
#include <mutex>
#include <mingw.thread.h>
#include <mingw.mutex.h>
#include <mingw.condition_variable.h>
#include <ecst/utils.hpp>
#include <utility>
#include <type_traits>
#include <vrm/core/type_traits/forward_like.hpp>
#include "perfect_capture.hpp"

namespace ll
{
    struct nothing_t
    {
    };

    constexpr nothing_t nothing{};

    template <typename T>
    constexpr auto is_nothing(T&&) noexcept { return std::is_same<std::decay_t<T>, nothing_t>{}; }

    template <typename T>
    struct void_to_nothing
    {
        using type = T;
    };

    template <>
    struct void_to_nothing<void>
    {
        using type = nothing_t;
    };

    template <typename T>
    using void_to_nothing_t = typename void_to_nothing<T>::type;

    template <typename TF>
    decltype(auto) call_ignoring_nothing(TF&& f);

    template <typename TF, typename... Ts>
    decltype(auto) call_ignoring_nothing(TF&& f, nothing_t, Ts&&... xs);

    template <typename TF, typename T, typename... Ts>
    decltype(auto) call_ignoring_nothing(TF&& f, T&& x, Ts&&... xs)
    {
        // Bind `x` to the function call and recurse.
        return call_ignoring_nothing(
            [ f = fwd_capture(FWD(f)), x = fwd_capture(FWD(x)) ](auto&&... ys) mutable
                ->decltype(auto)
            {
                return f.get()(vrm::core::forward_like<T>(x.get()), FWD(ys)...);
            },
            FWD(xs)...);
    }

    template <typename TF, typename... Ts>
    decltype(auto) call_ignoring_nothing(TF&& f, nothing_t, Ts&&... xs)
    {
        // Skip the `nothing_t` argument and recurse.
        return call_ignoring_nothing(FWD(f), FWD(xs)...);
    }

    template <typename TF>
    decltype(auto) call_ignoring_nothing(TF&& f)
    {
        // Base case.
        return FWD(f)();
    }

    template <typename TF, typename... Ts>
    decltype(auto) with_void_to_nothing(TF&& f, Ts&&... xs)
    {
#define BOUND_F() call_ignoring_nothing(f, FWD(xs)...)

        if constexpr(std::is_same<decltype(BOUND_F()), void>{})
        {
            BOUND_F();
            return nothing;
        }
        else
        {
            return BOUND_F();
        }

#undef BOUND_F
    }

    template <typename TF>
    auto bind_return_void_to_nothing(TF&& f)
    {
        return [f = fwd_capture(f)](auto&&... xs) mutable -> decltype(auto)
        {
            return with_void_to_nothing(vrm::core::forward_like<TF>(f.get()), FWD(xs)...);
        };
    }

    template <typename TF, typename TTuple>
    decltype(auto) apply_ignore_nothing(TF&& f, TTuple&& t)
    {
        return std::experimental::apply([f = fwd_capture(FWD(f))](auto&&... xs) mutable -> decltype(auto)
        {
            return call_ignoring_nothing(vrm::core::forward_like<TF>(f.get()), FWD(xs)...);
        }, FWD(t));
    }

    template <typename TF, typename TTuple>
    decltype(auto) for_tuple(TF&& f, TTuple&& t)
    {
        return std::experimental::apply(
            [&f](auto&&... xs) { (f(FWD(xs)), ...); }, FWD(t));
    }

    template <typename TF, typename TTuple>
    decltype(auto) for_tuple_ignore_nothing(TF&& f, TTuple&& t)
    {
        return for_tuple([&f](auto&&... xs) {
            return with_void_to_nothing(f, FWD(xs)...);
        }, FWD(t));
    }
}
