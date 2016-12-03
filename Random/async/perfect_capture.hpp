#pragma once

#include <ecst/utils.hpp>
#include <experimental/tuple>
#include <tuple>
#include <type_traits>
#include <utility>

#ifndef NDEBUG
#define LL_DEBUG 1
#endif

#define STATIC_ASSERT_SAME_TYPE(T0, T1) static_assert(std::is_same<T0, T1>{})
#define STATIC_ASSERT_SAME(a, T) STATIC_ASSERT_SAME_TYPE(decltype(a), T)

STATIC_ASSERT_SAME(FWD(std::declval<int>()), int&&);
STATIC_ASSERT_SAME(FWD(std::declval<int&>()), int&);
STATIC_ASSERT_SAME(FWD(std::declval<int&&>()), int&&);

template <typename T>
struct value_or_rvalue
{
    using type = T;
};

template <typename T>
struct value_or_rvalue<T&&>
{
    using type = T;
};

template <typename T>
using value_or_rvalue_t = typename value_or_rvalue<T>::type;

STATIC_ASSERT_SAME_TYPE(value_or_rvalue_t<int>, int);
STATIC_ASSERT_SAME_TYPE(value_or_rvalue_t<int&>, int&);
STATIC_ASSERT_SAME_TYPE(value_or_rvalue_t<int&&>, int);

namespace
{
    template <typename T>
    void sanity_test0(T&&)
    {
        STATIC_ASSERT_SAME_TYPE(T, int);
    }

    template <typename T>
    void sanity_test1(T&&)
    {
        STATIC_ASSERT_SAME_TYPE(T, int&);
    }

    [[maybe_unused]] void sanity()
    {
        sanity_test0(int{});

        int a = 0;
        sanity_test0(std::move(a));
        sanity_test1(a);
    }
}

using vrm::core::copy_if_rvalue;

template <typename T>
struct perfect_capture
{
    static_assert(!std::is_rvalue_reference<T>{});
    static_assert(!std::is_lvalue_reference<T>{});

private:
    T _x;

public:
    constexpr perfect_capture(const T& x) noexcept(
        std::is_nothrow_copy_constructible<T>{})
        : _x{x}
    {
    }

    constexpr perfect_capture(T&& x) noexcept(
        std::is_nothrow_move_constructible<T>{})
        : _x{std::move(x)}
    {
    }

    constexpr perfect_capture(perfect_capture&& rhs) noexcept(
        std::is_nothrow_move_constructible<T>{})
        : _x{std::move(rhs).get()}
    {
    }

    constexpr perfect_capture& operator=(perfect_capture&& rhs) noexcept(
        std::is_nothrow_move_assignable<T>{})
    {
        _x = std::move(rhs).get();
        return *this;
    }

    constexpr perfect_capture(const perfect_capture& rhs) noexcept(
        std::is_nothrow_copy_constructible<T>{})
        : _x{rhs.get()}
    {
    }

    constexpr perfect_capture& operator=(const perfect_capture& rhs) noexcept(
        std::is_nothrow_copy_assignable<T>{})
    {
        _x = rhs.get();
        return *this;
    }

    constexpr auto& get() & noexcept
    {
        return _x;
    }
    constexpr const auto& get() const & noexcept
    {
        return _x;
    }

    constexpr operator T&() & noexcept
    {
        return _x;
    }
    constexpr operator const T&() const & noexcept
    {
        return _x;
    }

    constexpr auto get() && noexcept(std::is_nothrow_move_constructible<T>{})
    {
        return std::move(_x);
    }
    constexpr operator T &&() &&
        noexcept(std::is_nothrow_move_constructible<T>{})
    {
        return std::move(_x);
    }
};

template <typename T>
struct perfect_capture<T&>
{
    static_assert(!std::is_rvalue_reference<T>{});

private:
    std::reference_wrapper<T> _x;

public:
    constexpr perfect_capture(T& x) noexcept : _x{x}
    {
    }

    constexpr perfect_capture(perfect_capture&& rhs) noexcept : _x{rhs._x}
    {
    }

    constexpr perfect_capture& operator=(perfect_capture&& rhs) noexcept
    {
        _x = rhs._x;
        return *this;
    }

    // Prevent copies.
    perfect_capture(const perfect_capture&) = delete;
    perfect_capture& operator=(const perfect_capture&) = delete;

    constexpr auto& get() & noexcept
    {
        return _x.get();
    }
    constexpr const auto& get() const & noexcept
    {
        return _x.get();
    }

    constexpr operator T&() & noexcept
    {
        return _x.get();
    }
    constexpr operator const T&() const & noexcept
    {
        return _x.get();
    }
};

namespace impl
{
    template <typename T>
    auto fwd_capture(T&& x)
    {
        return perfect_capture<T>(FWD(x));
    }

    template <typename T>
    auto fwd_copy_capture(T&& x)
    {
        return perfect_capture<T>(copy_if_rvalue(FWD(x)));
    }
}

#define FWD_CAPTURE(...) impl::fwd_capture(FWD(__VA_ARGS__))
#define FWD_COPY_CAPTURE(...) impl::fwd_copy_capture(FWD(__VA_ARGS__))

namespace impl
{
    template <typename... Ts>
    auto fwd_capture_as_tuple(Ts&&... xs)
    {
        return std::make_tuple(FWD_CAPTURE(xs)...);
    }

    template <typename... Ts>
    auto fwd_copy_capture_as_tuple(Ts&&... xs)
    {
        return std::make_tuple(FWD_COPY_CAPTURE(xs)...);
    }
}

#define FWD_CAPTURE_AS_TUPLE(...) \
    impl::fwd_capture_as_tuple(FWD(__VA_ARGS__)...)
#define FWD_COPY_CAPTURE_AS_TUPLE(...) \
    impl::fwd_copy_capture_as_tuple(FWD(__VA_ARGS__)...)

template <typename TF, typename TFwdCapture>
decltype(auto) apply_fwd_capture(TF&& f, TFwdCapture&& fc)
{
    return std::experimental::apply([&f](auto&&... xs) mutable -> decltype(
                                        auto) { return f(FWD(xs).get()...); },
        FWD(fc));
}

STATIC_ASSERT_SAME(impl::fwd_capture(1), perfect_capture<int>);
STATIC_ASSERT_SAME(
    impl::fwd_capture(std::declval<int>()), perfect_capture<int>);
STATIC_ASSERT_SAME(
    impl::fwd_capture(std::declval<int&>()), perfect_capture<int&>);
STATIC_ASSERT_SAME(
    impl::fwd_capture(std::declval<int&&>()), perfect_capture<int>);

namespace
{
    void test0[[maybe_unused]]()
    {
        int x = 0;
        auto p = impl::fwd_capture(x);
        STATIC_ASSERT_SAME(p, perfect_capture<int&>);
    }

    void test1[[maybe_unused]]()
    {
        int x = 0;
        auto p = impl::fwd_capture(std::move(x));
        STATIC_ASSERT_SAME(p, perfect_capture<int>);
    }

    void test2[[maybe_unused]]()
    {
        auto p = impl::fwd_capture(1);
        STATIC_ASSERT_SAME(p, perfect_capture<int>);
    }
}
